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
    shared_ptr<Variable> rng_count;
    char *hash_string;
    long long rng_count_internal;
    
    long long starting_rng_count, ending_rng_count;
    
    // an "announce" variable to dump image data to
    shared_ptr<Variable> load_announce_variable;
    
    Datum random_seed_datum;
    boost::lagged_fibonacci1279 rng;
    boost::uniform_real<> phase_distribution;
    boost::variate_generator<boost::lagged_fibonacci1279, boost::uniform_real<> > random_phase_gen;
    
    
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
                         shared_ptr<Variable> rng_count,
                         shared_ptr<Variable> _load_announce_variable,
                         shared_ptr<Scheduler> _scheduler,
                         shared_ptr<StimulusDisplay> _stimulus_display,
                         shared_ptr<Variable> _frames_per_second,
                         shared_ptr<Variable> _xoffset, 
                         shared_ptr<Variable> _yoffset,
                         shared_ptr<Variable> _xscale, 
                         shared_ptr<Variable> _yscale, 
                         shared_ptr<Variable> _rot,
                         shared_ptr<Variable> _alpha);
    
	~DynamicNoiseStimulus();
    
    
    virtual void load(shared_ptr<StimulusDisplay> _display);
    virtual void drawInUnitSquare(shared_ptr<StimulusDisplay> _display);
    
    void generateModulusImage();
    void generateNoiseImage(int width, int height, int frames, 
                            fftw_complex *modulus_image, fftw_complex *random_phase_storage,
                            fftw_complex *fft_in_storage, fftw_complex *fft_out_storage,
                            float *result_storage);
    
    void generateNoiseSequence();
    
    virtual Datum getCurrentAnnounceDrawData();
    
    
    // Methods for accessing / manipulating state outside of an experiment
    // (e.g. using the python bindings)
    float *getNoiseData(){  return result_storage; }
        
    void seek_rng(long seed, long rng_count){
        rng.seed(seed);
        for(int i = 0; i < rng_count; i++){
            random_phase_gen();
        }
    };
    
};

#endif 
