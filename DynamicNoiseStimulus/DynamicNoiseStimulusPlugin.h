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

BEGIN_NAMESPACE_MW

extern "C"{
    Plugin *getPlugin();
}

class DynamicNoiseStimulusPlugin : public Plugin {
    
	virtual void registerComponents(shared_ptr<mw::ComponentRegistry> registry);	
};



END_NAMESPACE_MW

#endif