/*
 *  CobraEyeTrackerConduit.cpp
 *  CoxlabCorePlugins
 *
 *  Created by David Cox on 9/24/08.
 *  Copyright 2008 The Rowland Institute at Harvard. All rights reserved.
 *
 */

#include "CobraEyeTrackerConduit.h"

using namespace mw;

	
void CobraDevice::addChild(std::map<std::string, std::string> parameters,
										ComponentRegistry *reg,
                           shared_ptr<mw::Component> _child){

	shared_ptr<CobraChannel> channel = dynamic_pointer_cast<CobraChannel, mw::Component>(_child);
	if(channel == NULL){
		throw SimpleException("Attempt to access an invalid Cobra EyeTracker channel object");
	}
    
    conduit->registerCallback(channel->getCapability(), bind(&CobraChannel::update, channel, _1)); 
}


shared_ptr<mw::Component> CobraDeviceFactory::createObject(std::map<std::string, std::string> parameters,
												 ComponentRegistry *reg) {
												 
	REQUIRE_ATTRIBUTES(parameters, "device_name");
	
	string resource_name = parameters["device_name"];
    shared_ptr <mw::Component> newDevice(new CobraDevice(resource_name));

	
    return newDevice;
}

