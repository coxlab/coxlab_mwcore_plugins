/*
 *  DisplayBitCodeStimulusPlugin.h
 *  DisplayBitCodeStimulus
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */
#ifndef DisplayBitCodeStimulus_PLUGIN_H_
#define DisplayBitCodeStimulus_PLUGIN_H_

#include <MWorksCore/Plugin.h>
using namespace mw;


namespace mw {
    
class DisplayBitCodeStimulusPlugin : public Plugin {
    
	virtual void registerComponents(shared_ptr<mw::ComponentRegistry> registry);	
};

    
}

#endif