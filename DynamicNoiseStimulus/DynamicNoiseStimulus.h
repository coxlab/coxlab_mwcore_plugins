/*
 *  DynamicNoiseStimulus.h
 * DynamicNoiseStimulus
 *  MWPlugin
 *
 *  Created by David Cox on 3/26/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#ifndef DynamicNoiseStimulus_H_
#define DynamicNoiseStimulus_H_

#include <MWorksCore/Plugin.h>
#include <MWorksCore/DynamicStimulusDriver.h>
#include <MWorksCore/StandardStimuli.h>
#include <MWorksCore/StimulusDisplay.h>


#include <MWorksCore/VideoFilterShader.h>

#include <vector>
#include <complex>
//#include <fftw3.h>
#include <boost/random.hpp>
#include "fftw3.h"

using namespace mw;
using namespace std;


class DynamicNoiseStimulus : public DynamicStimulusDriver, public BasicTransformStimulus {

public: enum power_spectrum_type {
    white = 1,
    one_over_f = 2,
    bandlimited_gwn = 3
};
    
protected:
    
    // An MW clock object to get the time from
    shared_ptr<Clock> clock;
    
    // the type of power spectrum to use
    power_spectrum_type power_spectrum;
    
    // the magnitude image
    fftw_complex *modulus_image;

    // fft storage
    fftw_complex *random_phase_storage;    
    fftw_complex *fft_in_storage;
    fftw_complex *fft_out_storage;
    float *result_storage;
    
    // actual GL frames.  The outer vector is indexed by frame
    // the inner vectors are indexed by display
    vector< vector<GLuint> > frame_textures;
    
    int frames_per_sequence;
    int pixel_width, pixel_height;
    
    
    shared_ptr<Variable> spatial_lowpass_cutoff;
    shared_ptr<Variable> spatial_highpass_cutoff;
    shared_ptr<Variable> temporal_lowpass_cutoff;
    shared_ptr<Variable> temporal_highpass_cutoff;
    shared_ptr<Variable> random_seed;
    
    boost::mt19937 rng;
    boost::uniform_real<> phase_distribution;
    boost::variate_generator<boost::mt19937, boost::uniform_real<> > random_phase_gen;
    
    shared_ptr<Shaders::ConvolutionFilterShader> bicubic_filter_shader;
    
    void loadDataToGLTexture(float *data, int width, int height, vector<GLuint> texture);
    void preallocateStorage();
    
public:
	
        
    DynamicNoiseStimulus(std::string _tag, 
                         power_spectrum_type _power_spectrum, 
                         int _frames_per_sequence,
                         int _pixel_width,
                         int _pixel_height,
                         shared_ptr<Variable> spatial_lowpass_cutoff,
                         shared_ptr<Variable> spatial_highpass_cutoff,
                         shared_ptr<Variable> temporal_lowpass_cutoff,
                         shared_ptr<Variable> temporal_highpass_cutoff,
                         shared_ptr<Variable> random_seed,
                         shared_ptr<Scheduler> _scheduler,
                         shared_ptr<StimulusDisplay> _stimulus_display,
                         shared_ptr<Variable> _frames_per_second,
                         shared_ptr<Variable> _statistics_reporting,
                         shared_ptr<Variable> _error_reporting,
                         shared_ptr<Variable> _xoffset, 
                         shared_ptr<Variable> _yoffset,
                         shared_ptr<Variable> _xscale, 
                         shared_ptr<Variable> _yscale, 
                         shared_ptr<Variable> _rot,
                         shared_ptr<Variable> _alpha);
    
	//DynamicNoiseStimulus(const DynamicNoiseStimulus &tocopy);
	~DynamicNoiseStimulus();
    
    
    virtual void load(StimulusDisplay* _display);
    virtual void drawInUnitSquare(StimulusDisplay* _display);
    
    void generateModulusImage();
    void generateNoiseImage(int width, int height, int frames, long random_seed, 
                            fftw_complex *modulus_image, fftw_complex *random_phase_storage,
                            fftw_complex *fft_in_storage, fftw_complex *fft_out_storage,
                            float *result_storage);
    
    void generateNoiseSequence();
};

#endif 
