/*
 *  MouseViewIODevicePlugin.cpp
 *  MouseViewIODevice
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "MouseViewIODevicePlugin.h"
#include "MWorksCore/ComponentFactory.h"
#include "MWorksCore/StandardComponentFactory.h"
#include "MouseViewIODevice.h"


Plugin *getPlugin(){
    return new MouseViewIODevicePlugin();
}


namespace mw{
    
void MouseViewIODevicePlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	
    registry->registerFactory<StandardComponentFactory, MouseViewIODevice>();
}
    
}

