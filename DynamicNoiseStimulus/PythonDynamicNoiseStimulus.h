/*
 *  PythonDynamicNoiseStimulus.h
 *  DynamicNoiseStimulus
 *
 *  Created by David Cox on 9/23/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DynamicNoiseStimulus.h"

namespace mw{

class DynamicNoiseStimulusWrapper {

protected:

    shared_ptr<DynamicNoiseStimulus> stim;
    
public:

    DynamicNoiseStimulusWrapper(int _power_spectrum, 
                                int _frames_per_sequence,
                                int _pixel_width,
                                int _pixel_height,
                                double _spatial_lowpass_cutoff,
                                double _spatial_highpass_cutoff,
                                double _temporal_lowpass_cutoff,
                                double _temporal_highpass_cutoff){
                                
        shared_ptr<Variable> spatial_lowpass_cutoff(new ConstantVariable(_spatial_lowpass_cutoff));
        shared_ptr<Variable> spatial_highpass_cutoff(new ConstantVariable(_spatial_highpass_cutoff));
        shared_ptr<Variable> temporal_lowpass_cutoff(new ConstantVariable(_temporal_lowpass_cutoff));
        shared_ptr<Variable> temporal_highpass_cutoff(new ConstantVariable(_temporal_highpass_cutoff));
        shared_ptr<Variable> random_seed(new ConstantVariable(0));
        shared_ptr<Variable> rng_count(new ConstantVariable(0));

        shared_ptr<Variable> dummy_variable(new ConstantVariable(0L));
        shared_ptr<Scheduler> scheduler;
        shared_ptr<StimulusDisplay> display;
                                                        
        stim = shared_ptr<DynamicNoiseStimulus>( new DynamicNoiseStimulus("dummy",
                                                                          _power_spectrum, 
                                                                          _frames_per_sequence,
                                                                          _pixel_width,
                                                                          _pixel_height,
                                                                          spatial_lowpass_cutoff
                                                                          spatial_highpass_cutoff,
                                                                          temporal_lowpass_cutoff,
                                                                          temporal_highpass_cutoff,
                                                                          random_seed,
                                                                          rng_count,
                                                                          dummy_variable, //_load_announce_variable,
                                                                          scheduler, // empty
                                                                          display, // empty
                                                                          dummy_variable, //_frames_per_second,
                                                                          dummy_variable, //_xoffset, 
                                                                          dummy_variable, //_yoffset,
                                                                          dummy_variable, //_xscale, 
                                                                          dummy_variable, //_yscale, 
                                                                          dummy_variable, //_rot,
                                                                          dummy_variable //_alpha
                                                                          ));
        
    }
    
   
    float *getData(){
        stim->seekRNG(_random_seed, _rng_count);
        stim->load(display); // load, but with a nonexistant stim display
        return stim->getNoiseData();
    }
    
    
                                                                         
};


BOOST_PYTHON_MODULE(_conduit)
{
    
    PyEval_InitThreads();
    
    class_<DynamicNoiseStimulusWrapper>("DyanmicNoiseStimulus", init<int,int,int,int,double,double,double,double>())
    .def("_get_data", &DynamicNoiseStimulusWrapper::getData)
    
    ;
}

}