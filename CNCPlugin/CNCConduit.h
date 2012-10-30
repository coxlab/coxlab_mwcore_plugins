/*
 *  CNCConduit.h
 *  CoxlabCorePlugins
 *
 *  Created by Brett Graham on 10/18/2010.
 *  Copyright 2010 The Rowland Institute at Harvard. All rights reserved.
 *
 */

#ifndef	_CNC_CONDUIT_H_
#define _CNC_CONDUIT_H_

#include <string>
#include <boost/format.hpp>
#include "MWorksCore/Utilities.h"
#include "MWorksCore/Plugin.h"
#include "MWorksCore/IODevice.h"
#include "MWorksCore/ComponentFactory.h"

#include "MWorksCore/SimpleConduit.h"
#include "MWorksCore/IPCEventTransport.h"

namespace mw {

using namespace std;


#define PATH_INFO_CONDUIT_EVENT_CODE    100
#define PING_CONDUIT_EVENT_CODE         101

class CNCDevice  : public IODevice {

public:

    // For now, these need to be locked down, because the Python side of things
    // will need to know these values
    // Going forward, maybe there is a way to have the Python side of the 
    // conduit be built with these values as constants
    enum CNCCapability {   path_origin_x = 0, 
                         path_origin_y = 1,
                         path_origin_z = 2,
                         path_slope_x = 3,
                         path_slope_y = 4,
                         path_slope_z = 5,
                         path_depth = 6,
                         path_info = 7,
                         ping = 8
                        };

protected:

    shared_ptr<SimpleConduit> conduit;

    shared_ptr<Variable> path_origin_x_variable, path_origin_y_variable, path_origin_z_variable,
                        path_slope_x_variable, path_slope_y_variable, path_slope_z_variable,
                        path_depth_variable;
    
    NSTask *task;
    
public:

    CNCDevice(string _resource_name = "cnc"){
    
        fprintf(stderr, _resource_name.c_str()); fflush(stderr);
        shared_ptr<IPCEventTransport> transport(new IPCEventTransport(EventTransport::client_event_transport,
                                                                        EventTransport::bidirectional_event_transport,
                                                                        _resource_name));
        //transport->flush();
        conduit = shared_ptr<SimpleConduit>(new SimpleConduit(transport, true)); // correct incoming timestamps
        
        conduit->registerCallback(PATH_INFO_CONDUIT_EVENT_CODE, bind(&CNCDevice::handleIncomingEvent, this, _1));
        conduit->registerCallback(PING_CONDUIT_EVENT_CODE, bind(&CNCDevice::handlePingEvent, this, _1));
        
    }
    
    virtual ~CNCDevice(){
        conduit->finalize();
    }
        
    
    virtual void handlePingEvent(shared_ptr<Event> event){
    
        fprintf(stderr, "PING");
        fflush(stderr);
    
    }
    
    virtual void handleIncomingEvent(shared_ptr<Event> event){
        // forward the event into the data stream?
        
        
        Datum path_info = event->getData();
        
        Datum path_origin_x_datum = path_info.getElement(path_origin_x);
        Datum path_origin_y_datum = path_info.getElement(path_origin_y);
        Datum path_origin_z_datum = path_info.getElement(path_origin_z);
        Datum path_slope_x_datum = path_info.getElement(path_slope_x);
        Datum path_slope_y_datum = path_info.getElement(path_slope_y);
        Datum path_slope_z_datum = path_info.getElement(path_slope_z);
        Datum path_depth_datum = path_info.getElement(path_depth);
        
        MWorksTime time = event->getTime();
        
        if(path_origin_x_variable != NULL){
            path_origin_x_variable->setValue(path_origin_x_datum, time);
        }
        
        if(path_origin_y_variable != NULL){
            path_origin_y_variable->setValue(path_origin_y_datum, time);
        }
        
        if(path_origin_z_variable != NULL){
            path_origin_z_variable->setValue(path_origin_z_datum, time);
        }
        
        if(path_slope_x_variable != NULL){
            path_slope_x_variable->setValue(path_slope_x_datum, time);
        }
        
        if(path_slope_y_variable != NULL){
            path_slope_y_variable->setValue(path_slope_y_datum, time);
        }
        
        if(path_slope_z_variable != NULL){
            path_slope_z_variable->setValue(path_slope_z_datum, time);
        }
        
        if(path_depth_variable != NULL){
            path_depth_variable->setValue(path_depth_datum, time);
        }
    }
    

    // Garbage that should be removed from the base class
    
    virtual bool initialize();
    
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


class CNCChannel : public mw::Component {

    protected:

    shared_ptr<Variable> variable;
    int capability;
    
    public:

    CNCChannel(string cap, shared_ptr<Variable> var){
        variable = var;
        if(cap == "path_origin_x"){
            capability = CNCDevice::path_origin_x;
        } else if(cap == "path_origin_y"){
            capability = CNCDevice::path_origin_y;
        } else if(cap == "path_origin_z"){
            capability = CNCDevice::path_origin_z;
        } else if(cap == "path_slope_x"){
            capability = CNCDevice::path_slope_x;
        } else if(cap == "path_slope_y"){
            capability = CNCDevice::path_slope_y;
        } else if(cap == "path_slope_z"){
            capability = CNCDevice::path_slope_z;
        } else if(cap == "path_depth"){
            capability = CNCDevice::path_depth;
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

class CNCDeviceChannelFactory : public ComponentFactory {

	shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
													 ComponentRegistry *reg) {
		
		REQUIRE_ATTRIBUTES(parameters, "variable", "capability");
		
		string capability_string = boost::to_lower_copy(parameters["capability"]);
		
		shared_ptr<Variable> variable = reg->getVariable(parameters["variable"]);
		
		shared_ptr <mw::Component> new_channel(new CNCChannel(capability_string, variable));
		return new_channel;
	}

};



class CNCDeviceFactory : public ComponentFactory {

	shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
													 ComponentRegistry *reg);
};

}

#endif






