/*
 *  DisplayBitCodeStimulusPlugin.cpp
 *  DisplayBitCodeStimulus
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */


#include "DisplayBitCodeStimulus.h"
#include "DisplayBitCodeStimulusPlugin.h"
//#include "DisplayBitCodeStimulusFactory.h"
#include "MWorksCore/ComponentFactory.h"
#include "MWorksCore/StandardStimulusFactory.h"


MW_SYMBOL_PUBLIC
extern "C" Plugin* getPlugin(){
    return new DisplayBitCodeStimulusPlugin();
}


namespace mw{
    
void DisplayBitCodeStimulusPlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	
    registry->registerFactory<StandardStimulusFactory, DisplayBitCodeStimulus>();
}
    
}

