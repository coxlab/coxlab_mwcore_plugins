/*
 *  CNCConduit.cpp
 *  CoxlabCorePlugins
 *
 *  Created by Brett Graham on 10/18/10.
 *  Copyright 2010 The Rowland Institute at Harvard. All rights reserved.
 *
 */

#include "CNCConduit.h"

namespace mw {


bool CNCDevice::initialize(){  
    return conduit->initialize();
}

	
void CNCDevice::addChild(std::map<std::string, std::string> parameters,
										ComponentRegistry *reg,
                           shared_ptr<mw::Component> _child){

	shared_ptr<CNCChannel> channel = boost::dynamic_pointer_cast<CNCChannel, mw::Component>(_child);
	if(channel == NULL){
		throw SimpleException("Attempt to access an invalid CNC channel object");
	}
    
    if(channel->getCapability() == path_origin_x){
        path_origin_x_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == path_origin_y){
        path_origin_y_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == path_origin_z){
        path_origin_z_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == path_slope_x){
        path_slope_x_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == path_slope_y){
        path_slope_y_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == path_slope_z){
        path_slope_z_variable = channel->getVariable();
    }
    
    if(channel->getCapability() == path_depth){
        path_depth_variable = channel->getVariable();
    }
    
    //conduit->registerCallback(channel->getCapability(), bind(&CNCChannel::update, channel, _1)); 
}


shared_ptr<mw::Component> CNCDeviceFactory::createObject(std::map<std::string, std::string> parameters,
												 ComponentRegistry *reg) {
												 
	REQUIRE_ATTRIBUTES(parameters, "device_name");
	
    string executable_path = parameters["executable_path"];
    if(executable_path == ""){
        executable_path = "/Applications/cncController.app/Contents/MacOS/cncController";
    }
	
    //NSTask *task = [NSTask launchedTaskWithLaunchPath:[NSString stringWithCString:executable_path.c_str() encoding:NSASCIIStringEncoding] arguments:[[NSArray alloc] init]];
   
    shared_ptr<mw::Clock> clock = mw::Clock::instance();
    //clock->sleepMS(1000);
    
    //if(![task isRunning]){
    //    throw SimpleException("Failed to launch eye tracker application");
    //}
    
    NSLog(@"%s",executable_path.c_str());
    
	string resource_name = parameters["device_name"];
    shared_ptr <mw::Component> newDevice(new CNCDevice(resource_name));

    return newDevice;
    
}
    
}

