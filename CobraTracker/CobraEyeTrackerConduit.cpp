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

	
void CobraDevice::addChild(std::map<std::string, std::string> parameters,
										ComponentRegistry *reg,
                           shared_ptr<mw::Component> _child){

	shared_ptr<CobraChannel> channel = dynamic_pointer_cast<CobraChannel, mw::Component>(_child);
	if(channel == NULL){
		throw SimpleException("Attempt to access an invalid Cobra EyeTracker channel object");
	}
    
    if(channel->getCapability() == gaze_h){
        gaze_h_variable = channel->getVariable();
    }

    if(channel->getCapability() == gaze_v){
        gaze_v_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == pupil_radius){
        pupil_radius_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == timestamp){
        timestamp_variable = channel->getVariable();
    }
    
    //conduit->registerCallback(channel->getCapability(), bind(&CobraChannel::update, channel, _1)); 
}


shared_ptr<mw::Component> CobraDeviceFactory::createObject(std::map<std::string, std::string> parameters,
												 ComponentRegistry *reg) {
												 
	REQUIRE_ATTRIBUTES(parameters, "device_name");
	
    string executable_path = parameters["executable_path"];
    if(executable_path == ""){
        executable_path = "/Applications/EyeTracker.app/Contents/MacOS/EyeTracker";
    }
	
    //NSTask *task = [NSTask launchedTaskWithLaunchPath:[NSString stringWithCString:executable_path.c_str() encoding:NSASCIIStringEncoding] arguments:[[NSArray alloc] init]];
   
    shared_ptr<mw::Clock> clock = mw::Clock::instance();
    //clock->sleepMS(1000);
    
    //if(![task isRunning]){
    //    throw SimpleException("Failed to launch eye tracker application");
    //}
    
    NSLog(@"%s",executable_path.c_str());
    
	string resource_name = parameters["device_name"];
    shared_ptr <mw::Component> newDevice(new CobraDevice(resource_name));

    return newDevice;
    
}
    
}

