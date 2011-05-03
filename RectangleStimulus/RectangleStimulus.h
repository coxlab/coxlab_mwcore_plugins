/*
 *  RectangleStimulus.h
 *  MWorksCore
 *
 *  Created by bkennedy on 8/26/08.
 *  Copyright 2008 MIT. All rights reserved.
 *
 */

#ifndef RECTANGLE_STIMULUS_H
#define RECTANGLE_STIMULUS_H

#include "MWorksCore/StandardStimuli.h"
#include "MWorksCore/ParameterValue.h"

using namespace mw;

class RectangleStimulus : public PointStimulus {

public:

    RectangleStimulus(const ParameterValueMap &parameters);
    
    static void describeComponent(ComponentInfo& info);

	RectangleStimulus(const RectangleStimulus &tocopy);
	~RectangleStimulus();
	
	virtual Datum getCurrentAnnounceDrawData();
};

#endif 
