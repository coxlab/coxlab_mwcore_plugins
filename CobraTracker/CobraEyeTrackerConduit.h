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
#include "MonkeyWorksCore/Utilities.h"
#include "MonkeyWorksCore/Plugin.h"
#include "MonkeyWorksCore/IODevice.h"
#include "MonkeyWorksCore/ComponentFactory.h"

#include "MonkeyWorksCore/SimpleConduit.h"
#include "MonkeyWorksCore/IPCEventTransport.h"

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
                         pupil_radius = 2  };

protected:

    shared_ptr<SimpleConduit> conduit;


public:

    CobraDevice(string _resource_name = "cobra_eye_tracker"){
    
        shared_ptr<IPCEventTransport> transport(new IPCEventTransport(EventTransport::server_event_transport,
                                                                        EventTransport::bidirectional_event_transport,
                                                                        _resource_name));
        conduit = shared_ptr<SimpleConduit>(new SimpleConduit(transport));
        
        // now doing this in the channels themselves
        //conduit->registerCallback(mCobraDevice::gaze_h, bind(&mCobraDevice::handleIncomingEvent, this, _1));
        //conduit->registerCallback(mCobraDevice::gaze_v, bind(&mCobraDevice::handleIncomingEvent, this, _1));
        //conduit->registerCallback(mCobraDevice::pupil_radius, bind(&mCobraDevice::handleIncomingEvent, this, _1));
        
        // Moved to attachPhysicalDevice
        //conduit->initialize();
    
    }
    
    virtual ~CobraDevice(){
        conduit->finalize();
    }
    
    
    virtual void handleIncomingEvent(shared_ptr<Event> event){
    
        fprintf(stderr, "Got event, yo: %d", event->getEventCode()); fflush(stderr);
    }
    

    // Garbage that should be removed from the base class
    
    virtual bool attachPhysicalDevice(){  
        return conduit->initialize();
    }
    
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
        }
    }
    
    shared_ptr<Variable> getVariable(){  return variable; }
    int getCapability(){ return capability; }
    
    void update(shared_ptr<Event> event){
        //fprintf(stderr, "Got event: code=%d, capability=%d, data=%g\n", event->getEventCode(), capability, (double)event->getData()); fflush(stderr);
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






