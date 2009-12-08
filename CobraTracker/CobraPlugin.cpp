/*
 *  CoxlabCorePlugins.cpp
 *  CoxlabCorePlugins
 *
 *  Created by David Cox on 4/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "CobraPlugin.h"
#include "CobraEyeTrackerConduit.h"

Plugin *getPlugin(){
    return new CobraPlugin();
}


void CobraPlugin::registerComponents(shared_ptr<ComponentRegistry> registry) {
	

	registry->registerFactory(std::string("iodevice/cobra_eye_tracker"),
								  (ComponentFactory *)(new CobraDeviceFactory()));

	registry->registerFactory(std::string("iochannel/cobra_eye_tracker"),
								  (ComponentFactory *)(new CobraDeviceChannelFactory()));
	
}
