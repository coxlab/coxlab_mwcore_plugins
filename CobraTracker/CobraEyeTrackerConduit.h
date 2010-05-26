/*
 *  CobraEyeTrackerConduit.h
 *  CoxlabCorePlugins
 *
 *  Created by David Cox on 9/24/08.
 *  Copyright 2008 The Rowland Institute at Harvard. All rights reserved.
 *
 */

#ifndef	_COBRA_CONDUIT_H_
#define _COBRA_CONDUIT_H_

#include <string>
#include <boost/format.hpp>
#include "MWorksCore/Utilities.h"
#include "MWorksCore/Plugin.h"
#include "MWorksCore/IODevice.h"
#include "MWorksCore/ComponentFactory.h"

#include "MWorksCore/SimpleConduit.h"
#include "MWorksCore/IPCEventTransport.h"

using namespace std;
using namespace mw;


class CobraDevice  : public IODevice {

public:

    // For now, these need to be locked down, because the Python side of things
    // will need to know these values
    // Going forward, maybe there is a way to have the Python side of the 
    // conduit be built with these values as constants
    enum CobraCapability {   gaze_h = 0, 
                         gaze_v = 1,
                         pupil_radius = 2,
                         timestamp = 3,
                         combined_gaze_info = 4,
                        };

protected:

    shared_ptr<SimpleConduit> conduit;

    shared_ptr<Variable> gaze_h_variable, gaze_v_variable, pupil_radius_variable, timestamp_variable;
    
    NSTask *task;
    
public:

    CobraDevice(string _resource_name = "cobra_eye_tracker"){
    
        fprintf(stderr, _resource_name.c_str()); fflush(stderr);
        shared_ptr<IPCEventTransport> transport(new IPCEventTransport(EventTransport::client_event_transport,
                                                                        EventTransport::bidirectional_event_transport,
                                                                        _resource_name));
        //transport->flush();
        conduit = shared_ptr<SimpleConduit>(new SimpleConduit(transport));
        
        conduit->registerCallback(CobraDevice::combined_gaze_info, bind(&CobraDevice::handleIncomingEvent, this, _1));
        
        
    }
    
    virtual ~CobraDevice(){
        conduit->finalize();
    }
    
    
    virtual void handleIncomingEvent(shared_ptr<Event> event){
    
        // forward the event into the data stream?
        
        
        Datum combined_gaze_info = event->getData();
        
        Datum gaze_h_datum = combined_gaze_info.getElement(gaze_h);
        Datum gaze_v_datum = combined_gaze_info.getElement(gaze_v);
        Datum pupil_radius_datum = combined_gaze_info.getElement(pupil_radius);
        Datum timestamp_datum = combined_gaze_info.getElement(timestamp);
        
        MWorksTime time = event->getTime();
        
        if(gaze_h_variable != NULL){
            gaze_h_variable->setValue(gaze_h_datum, time);
        }
        
        if(gaze_v_variable != NULL){
            gaze_v_variable->setValue(gaze_v_datum, time);
        }
        
        if(pupil_radius_variable != NULL){
            pupil_radius_variable->setValue(pupil_radius_datum, time);
        }
        
        if(timestamp_variable != NULL){
            timestamp_variable->setValue(timestamp_datum, time);
        }
    }
    

    // Garbage that should be removed from the base class
    
    virtual bool attachPhysicalDevice();
    
    virtual ExpandableList<IOCapability> *getCapabilities(){ return NULL; }
    virtual bool mapRequestsToChannels(){  return true;  }
    virtual bool initializeChannels(){  return true;  }
    virtual bool startup(){  return true;  }

    virtual void addChild(std::map<std::string, std::string> parameters,
                            ComponentRegistry *reg,
                          shared_ptr<mw::Component> _child);
    

    virtual void setActive(bool _active){
//        boost::mutex::scoped_lock active_lock(active_mutex);
//        active = _active;
    }

    virtual bool getActive(){
//        boost::mutex::scoped_lock active_lock(active_mutex);
//        bool is_active = active;
//        return is_active;
        return true;
    }



    virtual bool startDeviceIO(){  setActive(true); return true; }
    virtual bool stopDeviceIO(){  setActive(false); return true; }
    
    
    // this will stop anyIO behavior on a device and put the device in a shutdown state (if the device has one) -- e.g. turn off x-ray
    virtual bool shutdown(){ return true; }

};


class CobraChannel : public mw::Component {

    protected:

    shared_ptr<Variable> variable;
    int capability;
    
    public:

    CobraChannel(string cap, shared_ptr<Variable> var){
        variable = var;
        if(cap == "gaze_h"){
            capability = CobraDevice::gaze_h;
        } else if(cap == "gaze_v"){
            capability = CobraDevice::gaze_v;
        } else if(cap == "pupil_radius"){
            capability = CobraDevice::pupil_radius;
        } else if(cap == "timestamp"){
            capability = CobraDevice::timestamp;
        }
    }
    
    shared_ptr<Variable> getVariable(){  return variable; }
    int getCapability(){ return capability; }
    
    // Deprecated!: now sending events as bound-together groups
    void update(shared_ptr<Event> event){
        fprintf(stderr, "Got event: code=%d, capability=%d, data=%g\n", event->getEventCode(), capability, (double)event->getData()); fflush(stderr);
        variable->operator=(event->getData());
    }
    
};

class CobraDeviceChannelFactory : public ComponentFactory {

	shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
													 ComponentRegistry *reg) {
		
		REQUIRE_ATTRIBUTES(parameters, "variable", "capability");
		
		string capability_string = to_lower_copy(parameters["capability"]);
		
		shared_ptr<Variable> variable = reg->getVariable(parameters["variable"]);
		
		shared_ptr <mw::Component> new_channel(new CobraChannel(capability_string, variable));
		return new_channel;
	}

};



class CobraDeviceFactory : public ComponentFactory {

	shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
													 ComponentRegistry *reg);
};


#endif






