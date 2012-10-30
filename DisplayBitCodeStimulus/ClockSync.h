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


#define FAKE_ZMQ

namespace mw {


class ClockSync {

protected:

    // address for the zmq sockets
    std::string zmq_address;

    // zmq context
    zmq::context_t ctx;
    
    // zmq sockets
    vector< shared_ptr<zmq::socket_t> > zmq_sockets; 
    
    
    #ifdef FAKE_ZMQ
    vector< shared_ptr<zmq::socket_t> > fake_zmq_sockets;
    #endif
    
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
    vector<MWTime> external_transition_times;
    
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
              external_reconstructed_code(0),
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
        
        #ifdef FAKE_ZMQ
        // set up fake zmq publish sockets to pipe out fake data
        connectFakeZMQSockets();
        #endif
        
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
        
        
        external_transition_times.clear();
        for(int i = 0; i < n_channels; i++){
            external_transition_times.push_back(-1);
        }
    }

    // receive a new code to work with
    // this is assumed to be the first in the sequence
    // of evts driving this object
    void mwCodeChangeCallback(const Datum& data, MWTime time)
    {  
        //mprintf("mw code change callback!");
        
        int last_mw_code = current_mw_code;
        current_mw_code = (int)data;
        reset();
        
        #ifdef FAKE_ZMQ
        emitFakeZMQEvents(last_mw_code, current_mw_code);
        #endif
    }
    
    // callback to receive stimulus display update notification
    void stimulusDisplayUpdateCallback(const Datum& data, MWTime time)
    {
        //mprintf("stimulus update callback!. MWTime = %lld", time);
        
        boost::mutex::scoped_lock l(resource_mutex);
        
        mw_display_update_time = time;
        
        checkForMatch();
    }
    
    // callback to receive transitions from the zmq socket
    void codeTransitionCallback(int channel, int value, long long frame_number)
    {
    
        boost::mutex::scoped_lock l(resource_mutex);
    
        // adjust bit
        int shifted = 1 << channel;
        if(value){
            external_reconstructed_code |= shifted;
        } else {
            external_reconstructed_code &= ~shifted;
        }
        
        #define SAMPLING_RATE   44100
        
        double external_seconds = (double)frame_number / (double)SAMPLING_RATE;
        
        external_transition_times[channel] = (MWTime)(external_seconds * 1e6);
        
        
        //mprintf("Code Transition callback! (ch %d, val %d, frame %lld).  Reconstructed code = %d, MW time = %lld", 
        //         channel, value, frame_number, external_reconstructed_code, external_transition_times[channel]);

        
        checkForMatch();
    }
    
    
    void checkForMatch(){
    
        // if the internal and external codes are consistent
        // and an mw-generated time is available...
        if(current_mw_code == external_reconstructed_code && mw_display_update_time > 0){
        
            // compute a new time offset
            MWTime max_time = -1;
            vector<MWTime>::iterator i;
            for(i = external_transition_times.begin(); i != external_transition_times.end(); i++){
                MWTime t = *i;
                if(t > max_time) max_time = t;
            }
            
            // TODO: correct for channel offset latencies
            MWTime offset = mw_display_update_time - max_time;
            
            offset_variable->setValue(offset);
            
            //mprintf("Dropping offset event: %lld", offset);
        
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
            std::ostringstream filename_stream, url_stream;
            
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




    #ifdef FAKE_ZMQ
    void connectFakeZMQSockets(){
                
        // dispose of any old sockets
        fake_zmq_sockets.clear();
        
        for(int i = 0; i < n_channels; i++){
            // generate a fresh socket
            shared_ptr<zmq::socket_t> zmq_socket(new zmq::socket_t(ctx, ZMQ_PUB));
            
            uint64_t hwm = 1000;
            zmq_socket->setsockopt(ZMQ_HWM, &hwm, sizeof(uint64_t));
            
            
            // construct the url
            std::ostringstream filename_stream, url_stream;
            
            string host_address = zmq_address;
            
            url_stream << host_address << ":" << BASE_PORT + i;
            try {
                zmq_socket->bind(url_stream.str().c_str());
                std::cerr << "ZMQ client bound successfully to " << url_stream.str() << std::endl;
            } catch (zmq::error_t& e) {
                std::cerr << "ZMQ: " << e.what() << std::endl;
            }
            
            fake_zmq_sockets.push_back(zmq_socket);
        }
    }
    
    void emitFakeZMQEvents(int last_code, int current_code){
        
        shared_ptr<Clock> clock = Clock::instance();
        MWTime fake_now = clock->getCurrentTimeUS() + 4*1e6;
        
        for(int i = 0; i < n_channels; i++){
            bool last_bit = ((last_code & (1 << i)) > 0);
            bool this_bit = ((current_code & (1 << i)) > 0);

            if(last_bit != this_bit){
                PixelClockInfoBuffer evt;
                evt.set_direction(this_bit);
                evt.set_channel_id(i);
                
                long long fake_frame_number = (MWTime)(((double)fake_now/ (double)1e6) * (double)41000);
                evt.set_time_stamp(fake_frame_number);
                
                string serialized;
                evt.SerializeToString(&serialized);
                zmq::message_t msg(serialized.length());
                memcpy(msg.data(), serialized.c_str(), serialized.length());
                bool rc = fake_zmq_sockets[i]->send(msg);
                
            }
        }
    }
    
    #endif

    
};


} // end namespace mw

#endif