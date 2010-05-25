/*
 *  DynamicNoiseStimulusPlugin.cpp
 *  MWPlugin
 *
 *  Created by David Cox on 3/26/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DynamicNoiseStimulusPlugin.h"
#include "DynamicNoiseStimulusFactory.h"
#include "MWorksCore/ComponentFactory.h"
using namespace mw;

Plugin *getPlugin(){
    return new DynamicNoiseStimulusPlugin();
}


void DynamicNoiseStimulusPlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	
    // TODO: you need to customize the "signature" of the object your plugin will create
    //       The signature is of the form component/type Ð(e.g. stimulus/circle, or iodevice/NIDAQ)
    registry->registerFactory(std::string("stimulus/dynamic_noise_stimulus"),
							  (ComponentFactory *)(new DynamicNoiseStimulusFactory()));
}

