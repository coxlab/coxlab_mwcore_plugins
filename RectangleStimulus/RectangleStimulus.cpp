/*
 *  RectangleStimulus.cpp
 *  MWorksCore
 *
 *  Created by bkennedy on 8/26/08.
 *  Copyright 2008 MIT. All rights reserved.
 *
 */

#include "RectangleStimulus.h"
#include "MWorksCore/ComponentInfo.h"
#include <boost/regex.hpp>

using namespace mw;


RectangleStimulus::RectangleStimulus(const ParameterValueMap &parameters) :
    PointStimulus(parameters) { }


void RectangleStimulus::describeComponent(ComponentInfo& info){
    PointStimulus::describeComponent(info);
    info.setSignature("stimulus/rectangle");
}

RectangleStimulus::RectangleStimulus(const RectangleStimulus &tocopy) : PointStimulus((const PointStimulus&)tocopy){}

RectangleStimulus::~RectangleStimulus(){ }


Datum RectangleStimulus::getCurrentAnnounceDrawData() {
    
    Datum announceData(PointStimulus::getCurrentAnnounceDrawData());
	
    announceData.addElement(STIM_TYPE,"rectangle");
    
    return (announceData);
}

