/*
 *  PythonDynamicNoiseStimulus.h
 *  DynamicNoiseStimulus
 *
 *  Created by David Cox on 9/23/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */


#include <boost/python.hpp>
#include <vector>


#include "DynamicNoiseStimulus.h"

using namespace boost::python;

namespace mw{

class DynamicNoiseStimulusWrapper {

protected:

    shared_ptr<DynamicNoiseGenerator> generator;
    double spatial_lowpass_cutoff, spatial_highpass_cutoff, temporal_lowpass_cutoff, temporal_highpass_cutoff;
    
public:

    DynamicNoiseStimulusWrapper() { }

    DynamicNoiseStimulusWrapper(int _power_spectrum, 
                                int _frames_per_sequence,
                                int _pixel_width,
                                int _pixel_height,
                                double _spatial_lowpass_cutoff,
                                double _spatial_highpass_cutoff,
                                double _temporal_lowpass_cutoff,
                                double _temporal_highpass_cutoff){
        
        spatial_lowpass_cutoff = _spatial_lowpass_cutoff;
        spatial_highpass_cutoff = _spatial_highpass_cutoff;
        temporal_lowpass_cutoff = _temporal_lowpass_cutoff;
        temporal_highpass_cutoff = _temporal_highpass_cutoff;
        
        shared_ptr<Variable> dummy_variable(new ConstantVariable(0L));
        shared_ptr<Scheduler> scheduler;
                                                        
        generator = shared_ptr<DynamicNoiseGenerator>( new DynamicNoiseGenerator((DynamicNoiseGenerator::power_spectrum_type)_power_spectrum, 
                                                                          _frames_per_sequence,
                                                                          _pixel_width,
                                                                          _pixel_height ));
        
        
        
    }
    
   
    std::vector<float> getData(long _random_seed, unsigned long long _rng_count){
        generator->seekRNG(_random_seed, _rng_count);
        
        generator->generateModulusImage(spatial_lowpass_cutoff, spatial_highpass_cutoff, temporal_lowpass_cutoff, temporal_highpass_cutoff);
        generator->generateNoiseImage();
                
        float *result = generator->getNoiseData();
        int width = generator->getNoiseDataWidth();
        int height = generator->getNoiseDataHeight();
        int n_frames = generator->getNoiseDataNFrames();
        
        // needn't be particularly efficient
        std::vector<float> thedata;
        for(int i = 0; i < width * height * n_frames; i++){
            thedata.push_back(result[i]);
        }
        
        return thedata;
    }
    
    
    int hello(){ return 1;}
                                                                                       
};


BOOST_PYTHON_MODULE(DynamicNoiseStimulusPythonBindings)
{
    
    //PyEval_InitThreads();
    
    class_<DynamicNoiseStimulusWrapper>("DynamicNoiseStimulus", init<int,int,int,int,double,double,double,double>())
        .def(init<>())
        .def("_get_data", &DynamicNoiseStimulusWrapper::getData)
        //.def("hello_world", &DynamicNoiseStimulusWrapper::hello)
    ;
}

}