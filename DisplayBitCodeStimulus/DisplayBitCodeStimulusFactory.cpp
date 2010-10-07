/*
 *  DisplayBitCodeStimulusFactory.cpp
 *  DisplayBitCodeStimulus
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DisplayBitCodeStimulusFactory.h"

#include <boost/regex.hpp>
#include "MWorksCore/ComponentRegistry.h"

namespace mw{

shared_ptr<mw::Component> DisplayBitCodeStimulusFactory::createObject(std::map<std::string, std::string> parameters,
                                                               mw::ComponentRegistry *reg) {
	REQUIRE_ATTRIBUTES(parameters, 
					   "tag",
                       "code",
                       "n_markers",
                       "separation_ratio",
                       "x_position",
                       "y_position",
                       "x_scale",
                       "y_scale",
                       "rotation",
                       "alpha_multiplier",
                       "bg_luminance",
                       "fg_luminance");
	
	std::string tagname(parameters["tag"]);

    shared_ptr<Variable> code = reg->getVariable(parameters["code"]);
    shared_ptr<Variable> n_markers = reg->getVariable(parameters["n_markers"]); 
    shared_ptr<Variable> separation = reg->getVariable(parameters["separation_ratio"]);
    shared_ptr<Variable> bg = reg->getVariable(parameters["bg_luminance"]);
	shared_ptr<Variable> fg = reg->getVariable(parameters["fg_luminance"]);
    
    // BasicTransformStimulus attributes
    shared_ptr<Variable> xoffset = reg->getVariable(parameters["x_position"]); 
    shared_ptr<Variable> yoffset = reg->getVariable(parameters["y_position"]);
    shared_ptr<Variable> xscale = reg->getVariable(parameters["x_scale"]);
    shared_ptr<Variable> yscale = reg->getVariable(parameters["y_scale"]);
    shared_ptr<Variable> rot = reg->getVariable(parameters["rotation"]);
    shared_ptr<Variable> alpha = reg->getVariable(parameters["alpha_multiplier"]);
	
    
	shared_ptr <DisplayBitCodeStimulus> new_stimulus(
                       new DisplayBitCodeStimulus(tagname, 
                                                  code,
                                                  n_markers,
                                                  separation,
                                                  bg,
                                                  fg,
                                                  xoffset,
                                                  yoffset,
                                                  xscale,
                                                  yscale,
                                                  rot,
                                                  alpha));
        
    
	shared_ptr <StimulusNode> thisStimNode = shared_ptr<StimulusNode>(new StimulusNode(new_stimulus));
	reg->registerStimulusNode(tagname, thisStimNode);
	
	return new_stimulus;
}

}
