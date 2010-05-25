/*
 *  DynamicNoiseStimulusPlugin.h
 *  MWPlugin
 *
 *  Created by David Cox on 3/26/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */
#ifndef DynamicNoiseStimulus_PLUGIN_H_
#define DynamicNoiseStimulus_PLUGIN_H_

#include <MWorksCore/Plugin.h>
using namespace mw;

extern "C"{
    Plugin *getPlugin();
}

class DynamicNoiseStimulusPlugin : public Plugin {
    
	virtual void registerComponents(shared_ptr<mw::ComponentRegistry> registry);	
};


#endif