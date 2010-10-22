/*
 *  DynamicNoiseStimulusFactory.cpp
 *  DynamicNoiseStimulus
 *
 *  Created by David Cox on 3/26/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DynamicNoiseStimulusFactory.h"

#include <boost/regex.hpp>
#include "MWorksCore/ComponentRegistry.h"

using namespace mw;

shared_ptr<mw::Component> DynamicNoiseStimulusFactory::createObject(std::map<std::string, std::string> parameters,
                                                               mw::ComponentRegistry *reg) {
	
	std::string tagname(parameters["tag"]);

    // what kind of noise to produce
    std::string power_spectrum_type_string = reg->getValueForAttribute("power_spectrum_type", parameters);
    DynamicNoiseStimulus::power_spectrum_type power_spectrum = DynamicNoiseStimulus::white; 
    if(power_spectrum_type_string == "white"){
        power_spectrum = DynamicNoiseStimulus::white; 
    } else if(power_spectrum_type_string == "one_over_f"){
        power_spectrum = DynamicNoiseStimulus::one_over_f;
    } else if(power_spectrum_type_string == "bandlimited_gwn"){
        power_spectrum = DynamicNoiseStimulus::bandlimited_gwn;
    }
    
    // how fast should the frames go by
	shared_ptr<Variable> frames_per_second = reg->getVariableForAttribute("frames_per_second", parameters);

    // what is the maximum number of frames that will be presented at a go
    // This number is fixed at parse time so that the frames can be preallocated
    int frames_per_sequence = reg->getNumberForAttribute("frames_per_sequence", parameters);
    
    int pixel_width = (int)(reg->getNumberForAttribute("pixel_width", parameters));
    int pixel_height = (int)(reg->getNumberForAttribute("pixel_height", parameters));
    
    shared_ptr<Variable> random_seed = reg->getVariableForAttribute("random_seed", parameters);
    shared_ptr<Variable> rng_count = reg->getVariableForAttribute("rng_count", parameters);
    
    shared_ptr<Variable> x_size = reg->getVariableForAttribute("x_size", parameters);	
	shared_ptr<Variable> y_size = reg->getVariableForAttribute("y_size", parameters);	
	shared_ptr<Variable> x_position = reg->getVariableForAttribute("x_position", parameters);	
	shared_ptr<Variable> y_position = reg->getVariableForAttribute("y_position", parameters);	
	shared_ptr<Variable> rotation = reg->getVariableForAttribute("rotation", parameters);	
	
    shared_ptr<Variable> spatial_lowpass_cutoff = reg->getVariableForAttribute("spatial_lowpass_cutoff", parameters, "100.");
    shared_ptr<Variable> spatial_highpass_cutoff = reg->getVariableForAttribute("spatial_highpass_cutoff", parameters, ".1");
    shared_ptr<Variable> temporal_lowpass_cutoff = reg->getVariableForAttribute("temporal_lowpass_cutoff", parameters, "100.");
    shared_ptr<Variable> temporal_highpass_cutoff = reg->getVariableForAttribute("temporal_highpass_cutoff", parameters, ".1");
    
    //shared_ptr<Variable> load_announce = reg->getVariable(ANNOUNCE_STIMULUS_LOAD_TAGNAME);
    shared_ptr<Variable> load_announce = reg->getVariableForAttribute("load_announce_variable", parameters);

    
	shared_ptr<Variable> alpha_multiplier = 
        reg->getVariableForAttribute("alpha_multiplier", parameters, std::string("1.0"));	
	
    
    if(GlobalCurrentExperiment == 0) {
		throw SimpleException("no experiment currently defined");		
	}
	
	shared_ptr<StimulusDisplay> default_display = GlobalCurrentExperiment->getStimulusDisplay();
	if(default_display == 0) {
		throw SimpleException("no stimulusDisplay in current experiment");
	}
	
	shared_ptr <Scheduler> scheduler = Scheduler::instance();
	if(scheduler == 0) {
		throw SimpleException("no scheduler registered");		
	}
    
    shared_ptr<Stimulus> new_stimulus(new DynamicNoiseStimulus(tagname, 
                                                               power_spectrum, 
                                                               frames_per_sequence,
                                                               pixel_width,
                                                               pixel_height,
                                                               spatial_lowpass_cutoff,
                                                               spatial_highpass_cutoff,
                                                               temporal_lowpass_cutoff,
                                                               temporal_highpass_cutoff,
                                                               random_seed,
                                                               rng_count,
                                                               load_announce,
                                                               scheduler,
                                                               default_display,
                                                               frames_per_second,
                                                               x_position, 
                                                               y_position,
                                                               x_size, 
                                                               y_size, 
                                                               rotation,
                                                               alpha_multiplier)); 

    shared_ptr <StimulusNode> stim_node = shared_ptr<StimulusNode>(new StimulusNode(new_stimulus));
	reg->registerStimulusNode(tagname, stim_node);
    
    // TODO: this probably should be factored out somewhere
    Stimulus::load_style deferred = Stimulus::nondeferred_load;
    if(!parameters["deferred"].empty()){
        string deferred_value = parameters["deferred"];
        boost::algorithm::to_lower(deferred_value);
        if(deferred_value == "yes" || deferred_value == "1" || deferred_value == "true"){
            deferred = Stimulus::deferred_load;
        } else if(deferred_value == "explicit"){
            deferred = Stimulus::explicit_load;
        }
    }
    
    new_stimulus->setDeferred(deferred);
    
    // TODO: deferred load?
    if(deferred != Stimulus::deferred_load && deferred != Stimulus::explicit_load){
        new_stimulus->load(default_display);
    }
    
    
	return dynamic_pointer_cast<Stimulus, mw::Component>(new_stimulus);
}
