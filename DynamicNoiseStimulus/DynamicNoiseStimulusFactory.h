/*
 *  DynamicNoiseStimulusFactory.h
 *  MWPlugin
 *
 *  Created by David Cox on 3/26/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */


#ifndef DynamicNoiseStimulus_FACTORY_H
#define DynamicNoiseStimulus_FACTORY_H

#include "DynamicNoiseStimulus.h"

#include "MWorksCore/ComponentFactory.h"
using namespace mw;

class DynamicNoiseStimulusFactory : public ComponentFactory {
	virtual shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
                                                   mw::ComponentRegistry *reg);
};

#endif
