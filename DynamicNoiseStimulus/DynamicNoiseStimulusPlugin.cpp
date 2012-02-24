/*
 *  DynamicNoiseStimulusPlugin.cpp
 *  MWPlugin
 *
 *  Created by David Cox on 3/26/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DynamicNoiseStimulus.h"
#include "DynamicNoiseStimulusPlugin.h"
#include <MWorksCore/StandardStimulusFactory.h>


BEGIN_NAMESPACE_MW

Plugin *getPlugin(){
    return new DynamicNoiseStimulusPlugin();
}


void DynamicNoiseStimulusPlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	
    registry->registerFactory<StandardStimulusFactory, DynamicNoiseStimulus>();
}


END_NAMESPACE_MW
