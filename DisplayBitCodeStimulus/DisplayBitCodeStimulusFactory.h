/*
 *  DisplayBitCodeStimulusFactory.h
 *  DisplayBitCodeStimulus
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */


#ifndef DisplayBitCodeStimulus_FACTORY_H
#define DisplayBitCodeStimulus_FACTORY_H

#include "DisplayBitCodeStimulus.h"

#include "MWorksCore/ComponentFactory.h"


namespace mw { 

class DisplayBitCodeStimulusFactory : public ComponentFactory {
	virtual shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
                                                   mw::ComponentRegistry *reg);
};
}

#endif
