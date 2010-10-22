/*
 *  CNCPlugins.cpp
 *  CoxlabCorePlugins
 *
 *  Created by David Cox on 10/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CNCPlugin.h"
#include "CNCConduit.h"

Plugin *getPlugin(){
    return new CNCPlugin();
}


void CNCPlugin::registerComponents(shared_ptr<ComponentRegistry> registry) {
	

	registry->registerFactory(std::string("iodevice/cnc"),
								  (ComponentFactory *)(new CNCDeviceFactory()));

	registry->registerFactory(std::string("iochannel/cnc"),
								  (ComponentFactory *)(new CNCDeviceChannelFactory()));
	
}
