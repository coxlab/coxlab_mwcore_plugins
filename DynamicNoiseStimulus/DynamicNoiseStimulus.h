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

#include <boost/random.hpp>


#include <MWorksCore/ParameterValue.h>

#include <MWorksCore/Plugin.h>
#include <MWorksCore/StandardDynamicStimulus.h>
#include <MWorksCore/StandardStimuli.h>
#include <MWorksCore/StimulusDisplay.h>


#include <MWorksCore/VideoFilterShader.h>

#include <vector>
#include <complex>
#include "fftw3.h"

BEGIN_NAMESPACE_MW

using namespace std;



class DynamicNoiseGenerator {

  public: 
    
    enum power_spectrum_type {
        white = 1,
        one_over_f = 2,
        bandlimited_gwn = 3
    };

    
  protected:
    
    // the type of power spectrum to use
    power_spectrum_type power_spectrum;
    
    // the magnitude image
    fftw_complex *modulus_image;
    
    // fft storage
    fftw_complex *random_phase_storage;    
    fftw_complex *fft_in_storage;
    fftw_complex *fft_out_storage;
    float *result_storage;
    
    char *hash_string;
    //long long rng_count_internal;
    
    //long long starting_rng_count, ending_rng_count;
    
    VariablePtr random_seed_variable;
    Datum random_seed_datum;
    boost::lagged_fibonacci1279 rng;
    boost::uniform_real<> phase_distribution;
    boost::variate_generator<boost::lagged_fibonacci1279, boost::uniform_real<> > random_phase_gen;
    
    int frames_per_sequence;
    int pixel_width, pixel_height;
    
  
      
  public:

    DynamicNoiseGenerator(power_spectrum_type _power_spectrum, 
                          int _frames_per_sequence,
                          int _pixel_width,
                          int _pixel_height,
                          long initial_seed) :
        rng(initial_seed), 
        phase_distribution(-3.14,3.14), 
        random_phase_gen(rng,phase_distribution)
    {
        
        power_spectrum = _power_spectrum;
        frames_per_sequence = _frames_per_sequence;
        pixel_width = _pixel_width;
        pixel_height = _pixel_height; 
        preallocateStorage();
        
    }

    void preallocateStorage();
    void generateModulusImage(double spatial_lowpass_cutoff, 
                              double spatial_highpass_cutoff,
                              double temporal_lowpass_cutoff,
                              double temporal_highpass_cutoff);
    void generateNoiseImage();
    
    void generateNoiseSequence();
    
    void reseed(long new_seed){
        random_phase_gen.engine().seed(new_seed);
        random_phase_gen.distribution().reset();
    }
    
//    void seekRNG(long seed, unsigned long long rng_count){
//        rng.seed(seed);
//        for(int i = 0; i < rng_count; i++){
//            random_phase_gen();
//        }
//    }

    
    // Methods for accessing / manipulating state outside of an experiment
    // (e.g. using the python bindings)
    float *getNoiseData(){  return result_storage; }
    int getNoiseDataWidth(){ return pixel_width; }
    int getNoiseDataHeight(){ return pixel_height; }
    int getNoiseDataNFrames(){ return frames_per_sequence; }   
    
    
        
};




template<>
DynamicNoiseGenerator::power_spectrum_type ParameterValue::convert(const std::string &s, ComponentRegistryPtr reg);




class DynamicNoiseStimulus : public DynamicStimulusDriver, 
                             public BasicTransformStimulus, 
                             public DynamicNoiseGenerator {


    
  protected:
    
    
    // actual GL frames.  The outer vector is indexed by frame
    // the inner vectors are indexed by display
    vector< vector<GLuint> > frame_textures;
    
    shared_ptr<Variable> spatial_lowpass_cutoff;
    shared_ptr<Variable> spatial_highpass_cutoff;
    shared_ptr<Variable> temporal_lowpass_cutoff;
    shared_ptr<Variable> temporal_highpass_cutoff;
    shared_ptr<Variable> random_seed_variable;
    //shared_ptr<Variable> rng_count;
    
    // an "announce" variable to dump image data to
    shared_ptr<Variable> load_announce_variable;
    
    
    shared_ptr<Shaders::ConvolutionFilterShader> bicubic_filter_shader;
    
    void loadDataToGLTexture(shared_ptr<StimulusDisplay>, 
                             float *data, int width, int height, 
                             vector<GLuint> texture);
                             
    void preallocateTextures(shared_ptr<StimulusDisplay> display);
    
    int last_frame_drawn;
    int frame_number;
    
  public:
	
        
    DynamicNoiseStimulus(const ParameterValueMap& parameters);
    
	~DynamicNoiseStimulus();
    
    static void describeComponent(ComponentInfo &info);
    
    
    virtual void load(shared_ptr<StimulusDisplay> _display);
    virtual void drawInUnitSquare(shared_ptr<StimulusDisplay> _display);
    
    
    virtual void didStop();
        
    bool needDraw() { return isPlaying(); }
    
    virtual Datum getCurrentAnnounceDrawData();
    

    
};

END_NAMESPACE_MW

#endif 
