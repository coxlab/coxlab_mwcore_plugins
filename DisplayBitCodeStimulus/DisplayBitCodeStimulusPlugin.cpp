/*
 *  DisplayBitCodeStimulusPlugin.cpp
 *  DisplayBitCodeStimulus
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DisplayBitCodeStimulusPlugin.h"
#include "DisplayBitCodeStimulusFactory.h"
#include "MWorksCore/ComponentFactory.h"


Plugin *getPlugin(){
    return new DisplayBitCodeStimulusPlugin();
}


namespace mw{
    
void DisplayBitCodeStimulusPlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	
    // TODO: you need to customize the "signature" of the object your plugin will create
    //       The signature is of the form component/type Ð(e.g. stimulus/circle, or iodevice/NIDAQ)
    registry->registerFactory(std::string("stimulus/display_bit_code"),
							  (ComponentFactory *)(new DisplayBitCodeStimulusFactory()));
}
    
}

