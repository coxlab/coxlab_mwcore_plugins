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

#define GAZE_INFO_CONDUIT_EVENT_ID  100
#define EYETRACKER_INFO_CONDUIT_EVENT_ID   101

namespace mw {

using namespace std;



class CobraDevice  : public IODevice {

public:

    // For now, these need to be locked down, because the Python side of things
    // will need to know these values
    // Going forward, maybe there is a way to have the Python side of the
    // conduit be built with these values as constants
    enum CobraCapability{   gaze_h = 0,
                            gaze_v = 1,
                            pupil_radius = 2,
                            timestamp = 3,
                            calibration_status = 4,
                            pupil_h = 5,
                            pupil_v = 6,
                            cr_h = 7,
                            cr_v = 8,
                            top_led = 9,
                            side_led = 10,
                            eyetracker_info = 11,
                        };

protected:

    shared_ptr<SimpleConduit> conduit;

    shared_ptr<Variable> gaze_h_variable,
                         gaze_v_variable,
                         pupil_radius_variable,
                         pupil_h_variable,
                         pupil_v_variable,
                         cr_h_variable,
                         cr_v_variable,
                         calibration_status_variable,
                         timestamp_variable,
                         eyetracker_info_variable,
                         top_led_variable,
                         side_led_variable;

    NSTask *task;

public:

    CobraDevice(string _resource_name = "cobra_eye_tracker"){

        shared_ptr<IPCEventTransport> transport(new IPCEventTransport(EventTransport::client_event_transport,
                                                                        EventTransport::bidirectional_event_transport,
                                                                        _resource_name));

        conduit = shared_ptr<SimpleConduit>(new SimpleConduit(transport, true)); // correct incoming timestamps

        conduit->registerCallback(GAZE_INFO_CONDUIT_EVENT_ID, bind(&CobraDevice::handleIncomingGazeEvent, this, _1));
        conduit->registerCallback(EYETRACKER_INFO_CONDUIT_EVENT_ID, bind(&CobraDevice::handleIncomingInfoEvent, this, _1));
    }

    virtual ~CobraDevice(){
        conduit->finalize();
    }


    virtual void handlePingEvent(shared_ptr<Event> event){

        fprintf(stderr, "PING");
        fflush(stderr);

    }

    virtual void handleIncomingGazeEvent(shared_ptr<Event> event){

        // forward the event into the data stream?


        Datum combined_gaze_info = event->getData();

        Datum gaze_h_datum = combined_gaze_info.getElement(gaze_h);
        Datum gaze_v_datum = combined_gaze_info.getElement(gaze_v);
        Datum pupil_radius_datum = combined_gaze_info.getElement(pupil_radius);
        Datum timestamp_datum = combined_gaze_info.getElement(timestamp);

        Datum calibration_status_datum = combined_gaze_info.getElement(calibration_status);

        Datum pupil_h_datum = combined_gaze_info.getElement(pupil_h);
        Datum pupil_v_datum = combined_gaze_info.getElement(pupil_v);
        Datum cr_h_datum = combined_gaze_info.getElement(cr_h);
        Datum cr_v_datum = combined_gaze_info.getElement(cr_v);
        Datum top_led_datum = combined_gaze_info.getElement(top_led);
        Datum side_led_datum = combined_gaze_info.getElement(side_led);


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


        if(combined_gaze_info.getNElements() > 4){
            Datum calibration_status_datum = combined_gaze_info.getElement(calibration_status);
            Datum pupil_h_datum = combined_gaze_info.getElement(pupil_h);
            Datum pupil_v_datum = combined_gaze_info.getElement(pupil_v);
            Datum cr_h_datum = combined_gaze_info.getElement(cr_h);
            Datum cr_v_datum = combined_gaze_info.getElement(cr_v);


            if(calibration_status_variable != NULL){
                calibration_status_variable->setValue(calibration_status_datum, time);
            }

            if(pupil_h_variable != NULL){
                pupil_h_variable->setValue(pupil_h_datum, time);
            }

            if(pupil_v_variable != NULL){
                pupil_v_variable->setValue(pupil_v_datum, time);
            }

            if(cr_h_variable != NULL){
                cr_h_variable->setValue(cr_h_datum, time);
            }

            if(cr_v_variable != NULL){
                cr_v_variable->setValue(cr_v_datum, time);
            }
        }

        if(top_led_variable != NULL){
            top_led_variable->setValue(top_led_datum, time);
        }

        if(side_led_variable != NULL){
            side_led_variable->setValue(side_led_datum, time);
        }
    }


    virtual void handleIncomingInfoEvent(shared_ptr<Event> event){

        // forward the event into the data stream?
        Datum et_info = event->getData();
        eyetracker_info_variable->setValue(et_info, event->getTime());

    }

    // Garbage that should be removed from the base class

    virtual bool initialize();

    virtual void addChild(std::map<std::string, std::string> parameters,
                            ComponentRegistry *reg,
                          shared_ptr<mw::Component> _child);


    virtual void setActive(bool _active){
        // no-op
    }

    virtual bool getActive(){
        // no-op
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
        } else if(cap == "calibration_status"){
            capability = CobraDevice::calibration_status;
        } else if(cap == "pupil_h"){
            capability = CobraDevice::pupil_h;
        } else if(cap == "pupil_v"){
            capability = CobraDevice::pupil_v;
        } else if(cap == "cr_h"){
            capability = CobraDevice::cr_h;
        } else if(cap == "cr_v"){
            capability = CobraDevice::cr_v;
        } else if(cap == "eyetracker_info"){
            capability = CobraDevice::eyetracker_info;
        } else if(cap == "top_led"){
            capability = CobraDevice::top_led;
        } else if(cap == "side_led"){
            capability = CobraDevice::side_led;
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

		string capability_string = boost::to_lower_copy(parameters["capability"]);

		shared_ptr<Variable> variable = reg->getVariable(parameters["variable"]);

		shared_ptr <mw::Component> new_channel(new CobraChannel(capability_string, variable));
		return new_channel;
	}

};



class CobraDeviceFactory : public ComponentFactory {

	shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
													 ComponentRegistry *reg);
};

}

#endif






