//
//  ClockSync.h
//  DisplayBitCodeStimulus
//
//  Created by David Cox on 7/7/11.
//  Copyright 2011 The Rowland Institute at Harvard. All rights reserved.
//
#ifndef CLOCK_SYNC_H_
#define CLOCK_SYNC_H_


#include <zmq.hpp>

#include <MWorksCore/GenericVariable.h>
#include <MWorksCore/VariableNotification.h>
#include <MWorksCore/Scheduler.h>

#include <vector>
#include <string>

#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include "pixel_clock_info.pb.h"


namespace mw {


class ClockSync {

protected:

    // address for the zmq sockets
    std::string zmq_address;

    // zmq context
    zmq::context_t ctx;
    
    // zmq sockets
    vector< shared_ptr<zmq::socket_t> > zmq_sockets; 
    
    int n_channels;
    
    // the "current" code value (supplied by the stimulus
    int current_mw_code;
    
    // a reconstruction of the code (from the zmq socket)
    int external_reconstructed_code;
    
    // mw variables
    shared_ptr<Variable> display_update_variable;
    shared_ptr<VariableNotification> display_update_notification;
    
    shared_ptr<Variable> code_variable;
    shared_ptr<VariableNotification> code_change_notification;
    
    
    shared_ptr<Variable> offset_variable;
    
    // frame times for incoming transition events
    // negative values mean no transition
    // reset whenever a new "current" code is supplied
    vector<MWTime> external_transition_frame_times;
    
    // the time when the display update event arrives
    // negative means not arrived yet
    // reset whenever a new "current" code is supplied
    MWTime mw_display_update_time;

    // FIFO queue with recently computed offsets
    
    // A mutex to lock the object
    boost::mutex resource_mutex;
    
    // A mechanism for stopping the polling thread
    boost::mutex stopping_mutex;
    bool stopping;
    
public:

    // needs to know...
    // - IP address of zmq host
    // - display update variable
    // - nchannels
    // - variable to post clock offset info to
    ClockSync(int _n_channels, 
              std::string _zmq_address, 
              shared_ptr<Variable> _code_variable,
              shared_ptr<Variable> _display_update_variable,
              shared_ptr<Variable> _offset_variable) :
              
              ctx(1),
              code_variable(_code_variable),
              n_channels(_n_channels),
              zmq_address(_zmq_address),
              display_update_variable(_display_update_variable),
              offset_variable(_offset_variable),
              current_mw_code(-1),
              stopping(false)
    {
        // register a notification on display update
        boost::function<void (const Datum& data, MWTime time)> cb;
        cb = boost::bind( &ClockSync::stimulusDisplayUpdateCallback, this, _1, _2);
        display_update_notification = 
                shared_ptr<VariableNotification>( new VariableCallbackNotification(cb) );
        display_update_variable->addNotification(display_update_notification);
        
        // register a notification on code update
        boost::function<void (const Datum& data, MWTime time)> cb2;
        cb2 = boost::bind( &ClockSync::mwCodeChangeCallback, this, _1, _2);
        code_change_notification = 
                shared_ptr<VariableNotification>( new VariableCallbackNotification(cb2) );
        code_variable->addNotification(code_change_notification);
        
        // reset the state of the object
        reset();
        
        // connect the zmq listener sockets
        connectSockets();
        
        // launch the polling thread
        launchZMQListener();
    }


    ~ClockSync(){
        stop();
    }

    // reset the state of the clock sync object
    // in response to a new "round" of syncing:
    // - display_update_time is reset
    // - ...
    void reset(){
        boost::mutex::scoped_lock l(resource_mutex);
        
        mw_display_update_time = -1;
        
        
        external_transition_frame_times.clear();
        for(int i = 0; i < n_channels; i++){
            external_transition_frame_times.push_back(-1);
        }
    }

    // receive a new code to work with
    // this is assumed to be the first in the sequence
    // of evts driving this object
    void mwCodeChangeCallback(const Datum& data, MWTime time)
    {  
        mprintf("mw code change callback!");
        
        current_mw_code = (int)data;
        reset();
    }
    
    // callback to receive stimulus display update notification
    void stimulusDisplayUpdateCallback(const Datum& data, MWTime time)
    {
        mprintf("stimulus update callback!");
        
        boost::mutex::scoped_lock l(resource_mutex);
        
        mw_display_update_time = time;
    }
    
    // callback to receive transitions from the zmq socket
    void codeTransitionCallback(int channel, int value, long long frame_number)
    {
        mprintf("Code Transition callback!");
    
        boost::mutex::scoped_lock l(resource_mutex);
    
        // adjust bit
        int shifted = value << channel;
        external_reconstructed_code &= shifted;
        
        
        #define SAMPLING_RATE   44100
        
        double external_seconds = (double)frame_number / (double)SAMPLING_RATE;
        
        external_frame_transition_times[channel] = external_seconds;
        
        checkForMatch();
    }
    
    
    void checkForMatch(){
    
        // if the internal and external codes are consistent
        // and an mw-generated time is available...
        if(current_mw_code == external_reconstructed_code && mw_display_update_time > 0){
        
            // compute a new time offset
            
        
        }
    
    }
    
    // poll the zmq socket (in a separate thread)
    // - call codeTransitionCallback to update the reconstructed code
    void* poll()
    {
 
        // construct a zmq poll set
        zmq_pollitem_t items[n_channels];
        
        for(int i = 0; i < n_channels; i++){
            items[i].socket = *(zmq_sockets[i].get());
            items[i].events = ZMQ_POLLIN;
        }
        
        // poll
        while(true){
            
            // check if we should stop
            bool _stopping;
            {
                boost::mutex::scoped_lock s(stopping_mutex);
                _stopping = stopping;
            }
            
            if(_stopping){
                break;
            }
        
            
            zmq::poll(items, n_channels);
        
            
            
            // check items[n].revents to see what we got
            for(int c = 0; c < n_channels; c++){
                if( items[c].revents & ZMQ_POLLIN ){
                
                    // receive an event
                    zmq::message_t msg;
                    zmq_sockets[c]->recv(&msg);
                    
                    // deserialize the event
                    string data((const char *)msg.data(), msg.size());
                    
                    PixelClockInfoBuffer evt;
                    evt.ParseFromString(data);
                    
                    int chan = evt.channel_id();
                    int direction = evt.direction();
                    long long frame_number = evt.time_stamp();
                    
                    codeTransitionCallback(chan, direction, frame_number);
                    
                }
            }
        
        }
        
        return NULL;
    }
    
    void stop()
    {
        boost::mutex::scoped_lock l(stopping_mutex);
        stopping = true;
    }
    
    
    #define     BASE_PORT   11000
    
    void connectSockets(){
                
        // dispose of any old sockets
        zmq_sockets.clear();
        
        for(int i = 0; i < n_channels; i++){
            // generate a fresh socket
            shared_ptr<zmq::socket_t> zmq_socket(new zmq::socket_t(ctx, ZMQ_SUB));
            
            uint64_t hwm = 1000;
            zmq_socket->setsockopt(ZMQ_HWM, &hwm, sizeof(uint64_t));
            
            // subscribe to all messages on receive
            zmq_socket->setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
            
            // construct the url
            ostringstream filename_stream, url_stream;
            
            string host_address = zmq_address;
            
            url_stream << host_address << ":" << BASE_PORT + i;
            try {
                zmq_socket->connect(url_stream.str().c_str());
                std::cerr << "ZMQ client bound successfully to " << url_stream.str() << std::endl;
            } catch (zmq::error_t& e) {
                std::cerr << "ZMQ: " << e.what() << std::endl;
            }
            
            zmq_sockets.push_back(zmq_socket);
        }
    }


    void launchZMQListener(){
        
        shared_ptr<Scheduler> scheduler = Scheduler::instance();
        scheduler->scheduleUS( "pixel_clock_zmq_poll", 0, 0, 1, 
                               boost::bind( &ClockSync::poll, this ), 
                               M_DEFAULT_NETWORK_PRIORITY, 0, 0, M_MISSED_EXECUTION_DROP );
    }

    
};


} // end namespace mw

#endif