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

mPlugin *getPlugin(){
    return new CobraPlugin();
}


void CobraPlugin::registerComponents(shared_ptr<mComponentRegistry> registry) {
	

	registry->registerFactory(std::string("iodevice/cobra_tracker"),
								  (mComponentFactory *)(new mCobraDeviceFactory()));

	registry->registerFactory(std::string("iochannel/cobra_tracker"),
								  (mComponentFactory *)(new mCobraDeviceChannelFactory()));
	
}
