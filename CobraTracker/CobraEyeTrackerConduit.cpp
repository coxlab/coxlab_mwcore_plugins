/*
 *  CobraEyeTrackerConduit.cpp
 *  CoxlabCorePlugins
 *
 *  Created by David Cox on 9/24/08.
 *  Copyright 2008 The Rowland Institute at Harvard. All rights reserved.
 *
 */

#include "CobraEyeTrackerConduit.h"

namespace mw {


bool CobraDevice::initialize(){  
    return conduit->initialize();
}


#define SET_CAPABILITY_VAR(cap, var) if(channel->getCapability() == cap) var = channel->getVariable();

void CobraDevice::addChild(std::map<std::string, std::string> parameters,
										ComponentRegistry *reg,
                           shared_ptr<mw::Component> _child){

	shared_ptr<CobraChannel> channel = boost::dynamic_pointer_cast<CobraChannel, mw::Component>(_child);
	if(channel == NULL){
		throw SimpleException("Attempt to access an invalid Cobra EyeTracker channel object");
	}
    
    SET_CAPABILITY_VAR( gaze_h, gaze_h_variable );
    SET_CAPABILITY_VAR( gaze_v, gaze_v_variable );
    SET_CAPABILITY_VAR( pupil_radius, pupil_radius_variable );
    SET_CAPABILITY_VAR( pupil_h, pupil_h_variable );
    SET_CAPABILITY_VAR( pupil_v, pupil_v_variable );
    SET_CAPABILITY_VAR( cr_h, cr_h_variable );
    SET_CAPABILITY_VAR( cr_v, cr_v_variable );
    SET_CAPABILITY_VAR( timestamp, timestamp_variable );
    SET_CAPABILITY_VAR( calibration_status, calibration_status_variable );
    SET_CAPABILITY_VAR( eyetracker_info, eyetracker_info_variable );
    SET_CAPABILITY_VAR( top_led, top_led_variable );
    SET_CAPABILITY_VAR( side_led, side_led_variable );
    
}


shared_ptr<mw::Component> CobraDeviceFactory::createObject(std::map<std::string, std::string> parameters,
												 ComponentRegistry *reg) {
												 
	REQUIRE_ATTRIBUTES(parameters, "device_name");
	
    string executable_path = parameters["executable_path"];
    if(executable_path == ""){
        executable_path = "/Applications/EyeTracker.app/Contents/MacOS/EyeTracker";
    }
	
   
    shared_ptr<mw::Clock> clock = mw::Clock::instance();
    
    NSLog(@"%s",executable_path.c_str());
    
	string resource_name = parameters["device_name"];
    shared_ptr <mw::Component> newDevice(new CobraDevice(resource_name));

    return newDevice;
    
}
    
}

