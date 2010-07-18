/*
 *  DynamicNoiseStimulus.cpp
 *  MWPlugin
 *
 *  Created by David Cox on 3/26/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DynamicNoiseStimulus.h"
#include <iostream>
#include <fstream>

#define USE_FFTW_MALLOC     1


DynamicNoiseStimulus::DynamicNoiseStimulus(std::string _tag, 
                        power_spectrum_type _power_spectrum, 
                        int _frames_per_sequence,
                        int _pixel_width,
                        int _pixel_height,
                        shared_ptr<Variable> _spatial_lowpass_cutoff,
                        shared_ptr<Variable> _spatial_highpass_cutoff,
                        shared_ptr<Variable> _temporal_lowpass_cutoff,
                        shared_ptr<Variable> _temporal_highpass_cutoff,
                        shared_ptr<Variable> _random_seed,
                        shared_ptr<Variable> _rng_count,
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
                        shared_ptr<Variable> _alpha):
                        DynamicStimulusDriver (_scheduler,
                                               _stimulus_display,
                                               _frames_per_second),
                                               //_statistics_reporting,
                                               //_error_reporting),
                        BasicTransformStimulus(_tag,
                                               _xoffset,
                                               _yoffset,
                                               _xscale,
                                               _yscale,
                                               _rot,
                                               _alpha),
                        random_seed_datum(time(0)),
                        rng((long)random_seed_datum), 
                        phase_distribution(-3.14,3.14), 
                        random_phase_gen(rng,phase_distribution){
                             
    _random_seed->setValue(random_seed_datum);
    random_seed = _random_seed; 
    
    rng_count_internal = 0LL;
    Datum rng_count_datum(rng_count_internal);
    _rng_count->setValue(rng_count_datum);
    rng_count = _rng_count;
                            
    power_spectrum = _power_spectrum;
    frames_per_sequence = _frames_per_sequence;
    pixel_width = _pixel_width;
    pixel_height = _pixel_height;                            
    spatial_lowpass_cutoff = _spatial_lowpass_cutoff;
    spatial_highpass_cutoff = _spatial_highpass_cutoff;
    temporal_lowpass_cutoff = _temporal_lowpass_cutoff;
    temporal_highpass_cutoff = _temporal_highpass_cutoff;
                            
                                
    // 2. pre-allocate storage
    preallocateStorage();
                                                
    // 3. get a seed and load an initial salvo of frames
    
    
    // create a GLSL bicubic interpolation filter
    bicubic_filter_shader = shared_ptr<Shaders::ConvolutionFilterShader>(new Shaders::ConvolutionFilterShader(VS_SCALINGMETHOD_LANCZOS3, false));
    
    
}

//DynamicNoiseStimulus::DynamicNoiseStimulus(const DynamicNoiseStimulus &tocopy) { }
DynamicNoiseStimulus::~DynamicNoiseStimulus(){ 

    
#if USE_FFTW_MALLOC
    fftw_free(modulus_image);
    fftw_free(random_phase_storage);
    fftw_free(fft_in_storage);
    fftw_free(fft_out_storage);
#else
    free(modulus_image);
    free(random_phase_storage);
    free(fft_in_storage);
    free(fft_out_storage);
#endif
    
    delete [] result_storage;
    
    // Some funkiness below; for now, let the GL context take care of itself
    // Generate texture maps in each OpenGL context
    //int n_contexts = display->getNContexts();
//    for(int f = 0; f < frames_per_sequence; f++){
//        
//        vector<GLuint> inner = frame_textures[f];
//        
//        shared_ptr<OpenGLContextManager> ctx_mgr = OpenGLContextManager::instance(false);
//        if(ctx_mgr != NULL){
//            for(int c = 0; c < n_contexts; c++){
//                display->setCurrent(c);
//                GLuint tex_id = inner[c];
//                glDeleteTextures(1, &tex_id);
//            }
//        }
//    }
    
    fftw_cleanup();

}


void DynamicNoiseStimulus::load(shared_ptr<StimulusDisplay> _display){
    
    
    for(int i = 0; i < _display->getNContexts(); i++){
        _display->setCurrent(i);
        if (!bicubic_filter_shader->CompileAndLink()){
            merror(M_DISPLAY_MESSAGE_DOMAIN, "GL: Error compiling and linking video filter shader");
            bicubic_filter_shader->Free();
            bicubic_filter_shader = shared_ptr<Shaders::ConvolutionFilterShader>();
        }
    }
    
    long seed = (long)0;//(random_seed->getValue());

    generateModulusImage();

    generateNoiseImage(pixel_width, pixel_height, frames_per_sequence, seed, 
                       modulus_image, random_phase_storage,
                       fft_in_storage,fft_out_storage,
                       result_storage);
    
    
    for(int f = 0; f < frames_per_sequence; f++){
        
        int offset = f * pixel_width * pixel_height;
        for(int i = 0; i < _display->getNContexts(); i++){
            _display->setCurrent(i);
            loadDataToGLTexture(result_storage + offset, pixel_width, pixel_height, frame_textures[f]);
        }
    }
}

void DynamicNoiseStimulus::drawInUnitSquare(shared_ptr<StimulusDisplay> display) {
    
    int frame_number = getFrameNumber();
    
    if(frame_number >= frames_per_sequence){
        return;
    }
    
    double aspect = (double)pixel_width / (double)pixel_height;
    if(1 || loaded) {
        
        GLuint texture_id = frame_textures[frame_number][display->getCurrentContextIndex()];
        
        glActiveTextureARB(GL_TEXTURE0_ARB); 
        
        
        glEnable(GL_TEXTURE_2D);
        //glEnable(GL_TEXTURE_RECTANGLE_EXT);
		glBindTexture(GL_TEXTURE_2D, texture_id);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        bicubic_filter_shader->SetSourceTexture(0);
        bicubic_filter_shader->SetWidth((float)pixel_width);
        bicubic_filter_shader->SetHeight((float)pixel_height);
        bicubic_filter_shader->Enable();
        
        
        
//        glEnable (GL_BLEND); 
//		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        glBegin(GL_QUADS);
		
		double a = alpha_multiplier->getValue().getFloat();
		glColor4f(1., 1., 1., a);
        
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
        double m = 1.0;
        
        if(aspect > 1) {
			
            glTexCoord2f(0.0,0.0); 
            glVertex3f(0.0,(0.5-0.5/aspect),0.0);
            
            glTexCoord2f(m,0.0); 
			glVertex3f(1.0,(0.5-0.5/aspect),0.0);
            
            glTexCoord2f(m,m); 
            glVertex3f(1.0,(0.5-0.5/aspect) + 1.0/aspect,0.0);
			
            glTexCoord2f(0.0,m); 
            glVertex3f(0.0,(0.5-0.5/aspect) + 1.0/aspect,0.0);
        } else {
			
            glTexCoord2f(0.0,0.0); 
            glVertex3f((1.0 - aspect)/2.0,0.0,0.0);
            
            glTexCoord2f(0.9,0.0);
			glVertex3f((1.0 - aspect)/2.0 + aspect,0.0,0.0);
            
            glTexCoord2f(0.9,0.9); 
            glVertex3f((1.0 - aspect)/2.0 + aspect,1.0,0.0);
            
            glTexCoord2f(0.0,0.9); 
            glVertex3f((1.0 - aspect)/2.0,1.0,0.0);
        }
		
        glEnd();
		
                
		
        bicubic_filter_shader->Disable();
        
        glBindTexture(GL_TEXTURE_2D, 0); // unbind that fucker
		
		
        
		//glActiveTexture(0);
		
    } else {
        mwarning(M_DISPLAY_MESSAGE_DOMAIN,
                 "Stimulus image did not properly load. Showing nothing");
        mwarning(M_DISPLAY_MESSAGE_DOMAIN,
                 "%s:%d", __FILE__, __LINE__);
    }
    
}

void DynamicNoiseStimulus::preallocateStorage(){
    //      modulus_image:               an fftw_complex width * height array for storing the desired power spectrum of the noise 
    //      random_phase_storage:  an fftw_complex width * height array for storing a randomly generated phase image
    //      fft_in_storage:        an fftw_complex width * height array that stores the full complex input, combining the
    //                             random phase and the modulus
    //      fft_out_storage:       an fftw_complex width * height array containing the inverted (to image domain) result
    //      result_storage:        a doubleing point array for storing the result
    
    
    // 1. generate magnitude image
    
#if USE_FFTW_MALLOC
    modulus_image = (fftw_complex *)fftw_malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));
    random_phase_storage = (fftw_complex*) fftw_malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));    
    fft_in_storage = (fftw_complex*) fftw_malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));
    fft_out_storage = (fftw_complex*) fftw_malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));
#else
    modulus_image = (fftw_complex *)malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));
    random_phase_storage = (fftw_complex*) malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));    
    fft_in_storage = (fftw_complex*) malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));
    fft_out_storage = (fftw_complex*) malloc(pixel_height * pixel_width * frames_per_sequence * sizeof(fftw_complex));
#endif
    
    result_storage = new float[pixel_height * pixel_width * frames_per_sequence];
    
    // Generate texture maps in each OpenGL context
    int n_contexts = display->getNContexts();
    for(int f = 0; f < frames_per_sequence; f++){
        
        vector<GLuint> inner;
        
        for(int c = 0; c < n_contexts; c++){
            display->setCurrent(c);
            GLuint tex_id = 0;
            glGenTextures(1,&tex_id);
            inner.push_back(tex_id);
        }
        
        frame_textures.push_back(inner);
    }
}

void DynamicNoiseStimulus::generateModulusImage(){

    double filter_order = 2.0;
    double space_lowpass_cutoff = (double)spatial_lowpass_cutoff->getValue() / pixel_width;
    double space_highpass_cutoff = (double)spatial_highpass_cutoff->getValue() / pixel_width;
    double time_lowpass_cutoff = (double)temporal_lowpass_cutoff->getValue() / frames_per_sequence;
    double time_highpass_cutoff = (double)temporal_highpass_cutoff->getValue() / frames_per_sequence;
    
    
    if(power_spectrum == white){
        for(int i = 0; i < pixel_width * pixel_height * frames_per_sequence; i++){
            modulus_image[i][0] = 255.0 / (pixel_width * pixel_height * frames_per_sequence);
            //modulus_image[i][0] = 255.0;
        }
    } else if(power_spectrum == bandlimited_gwn){
        
        std::cerr << "space lowpass cutoff: " << space_lowpass_cutoff << std::endl;
        std::cerr << "space highpass cutoff: " << space_highpass_cutoff << std::endl;
        std::cerr << "time lowpass cutoff: " << time_lowpass_cutoff << std::endl;
        std::cerr << "time highpass cutoff: " << time_highpass_cutoff << std::endl;
        
        double u,v,w;
        for(int i = 0; i < pixel_height; i++){
            
            if(i < (double)pixel_width / 2.0){
                // positive quadrant
                u = (double)i / (double)pixel_width;
            } else {
                u = ((double)(i - pixel_width) / (double)pixel_width);
            }
            
            
            for(int j = 0; j < pixel_width; j++){
                
                if(j < (double)pixel_height / 2.0){
                    // positive quadrant
                    v = (double)j / pixel_height;
                } else {
                    v = ((double)(j - pixel_height) / (double)pixel_height);
                }
                
                
                for(int k = 0; k < frames_per_sequence; k++){
                    
                    if(k < (double)frames_per_sequence / 2.0){
                        // positive quadrant
                        w = (double)k / frames_per_sequence;
                    } else {
                        w = ((double)(k - frames_per_sequence) / (double)frames_per_sequence);
                    }
                    
                    double r = sqrt( u*u + v*v );
                    
                    double s_high = 1.0 / (1.0 + pow((r / space_highpass_cutoff), 2.0*filter_order));
                    //s_high *= 1.0 / (1.0 + pow(w / time_highpass_cutoff, 2.0 * filter_order));
                    
                    double s_low = 1.0 / (1.0 + pow((r / space_lowpass_cutoff), 2.0*filter_order));
                    //s_low *= 1.0 / (1.0 + pow(fabs(w) / time_lowpass_cutoff, 2.0 * filter_order));
                    
                    
                    modulus_image[k*pixel_height*pixel_width + i*pixel_width + j][0] =  s_low - s_high;
                    //modulus_image[k*pixel_height*pixel_width + i*pixel_width + j][0] =  r;

                }
            }
        }
    
    } else if(power_spectrum == one_over_f){
        
        for(int i = 0; i < pixel_height; i++){
            for(int j = 0; j < pixel_width; j++){
                
                double u = 0.0;
                double v = 0.0;
                double s = 0.0;
 

                if(i < (double)pixel_width / 2.0){
                    // positive quadrant
                    u = (double)i / (double)pixel_width;
                } else {
                    u = ((double)(i - pixel_width) / (double)pixel_width);// + 1.0 / pixel_width;
                }
                
                if(j < (double)pixel_height / 2.0){
                    // positive quadrant
                    v = (double)j / pixel_height;
                } else {
                    v = ((double)(j - pixel_height) / (double)pixel_height);// + 1.0 / pixel_height;
                }
                
                //s = u;
                s = sqrt( u*u + v*v ) / 2.0;
                
                modulus_image[i*pixel_width + j][0] =  -s;
            }
        }
    }
}


void DynamicNoiseStimulus::loadDataToGLTexture(float *data, int width, int height, vector<GLuint> textures){
    
    // move data to texture maps on each display
    for(int i = 0; i < display->getNContexts(); i++){
		display->setCurrent(i);
        GLuint texture_map;
        texture_map = textures[i];

        glBindTexture(GL_TEXTURE_2D, texture_map);
        
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        
        //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_LUMINANCE, GL_FLOAT, data);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,  width, height, 0, GL_LUMINANCE, GL_FLOAT, data);
//        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  width, height, 0, GL_LUMINANCE, GL_FLOAT, data);
        
        glBindTexture(GL_TEXTURE_2D, 0);
	}
    
}

// Generate a random noise image using preallocated temporary storage arrays passed in from outside
//
// These are:
//      random_phase_storage:  an fftw_complex width * height array for storing a randomly generated phase image
//      fft_in_storage:        an fftw_complex width * height array that stores the full complex input, combining the
//                             random phase and the modulus
//      fft_out_storage:       an fftw_complex width * height array containing the inverted (to image domain) result
//      result_storage:        a doubleing point array for storing the result
//
// Also passed in:
//      modulus_image:       an fftw_complex array containing the power spectrum info for this stimulus
//      random_seed:   a seed for the random number generator

void DynamicNoiseStimulus::generateNoiseImage(int width, int height, int frames_per_sequence, long random_seed, 
                                              fftw_complex *modulus_image, fftw_complex *random_phase_storage,
                                              fftw_complex *fft_in_storage, fftw_complex *fft_out_storage,
                                              float *result_storage) {
    
    
    Datum rng_count_datum((long long)rng_count_internal);
    rng_count->setValue(rng_count_datum);
    
    // Generate a random phase image:
    // use the rng to fill the preallocated random phase array
    for (int i = 0; i < (height * width * frames_per_sequence); i++) {
        complex<double> temp_phase(0.0,random_phase_gen());
        rng_count_internal++;
        temp_phase = exp(temp_phase);
        random_phase_storage[i][0] = real(temp_phase);
        random_phase_storage[i][1] = imag(temp_phase);
    }
    
    
        
    // print out the storage locations
    //std::cerr << "fft_in_storage: \t\t\t" << fft_in_storage << " - to - " << fft_in_storage + (width*height) <<  std::endl;
//    std::cerr << "fft_out_storage: \t\t\t" << fft_out_storage <<  " - to - " << fft_out_storage + (width*height) << std::endl;
//    std::cerr << "result_storage: \t\t\t" << result_storage <<  " - to - " << result_storage + (width*height) << std::endl;
//    std::cerr << "modulus_image: \t\t\t" << modulus_image <<   " - to - " << modulus_image + (width*height) <<std::endl;
//    std::cerr << "random_phase_storage: \t\t\t" << random_phase_storage <<  " - to - " << random_phase_storage + (width*height) << std::endl;
//    
    // Combine with modulus and do inverse

    // note from Brett: making the plan fubars the input
    fftw_plan fft_plan = fftw_plan_dft_3d(frames_per_sequence, height, width, fft_in_storage, fft_out_storage, FFTW_BACKWARD, FFTW_ESTIMATE);
    
    //fftw_print_plan(fft_plan);
    
    bool problem_flag = false;
    
    
    // fill input
    for (int j = 0; j < (height * width * frames_per_sequence); j++) {
        //for (int j = 0; j < ((height * width)/2 + 1); j++) {
        complex<double> temp_modulus(modulus_image[j][0],modulus_image[j][1]);
        //complex<double> temp_modulus = modulus_image[j];
        complex<double> temp_phase(random_phase_storage[j][0],random_phase_storage[j][1]);
        complex<double> temp_result;
        
        temp_result = temp_modulus * temp_phase;

        if(real(temp_result) == NAN || imag(temp_result) == NAN){
            temp_result = complex<double>(0.0, 0.0);
        }
        
        fft_in_storage[j][0] = real(temp_result);
        fft_in_storage[j][1] = imag(temp_result);
    
    }
    
    // a special case
    //fft_in_storage[0][0] = 0.0;
    //fft_in_storage[0][0] = 0.0;
    
    // exectue fft
    fftw_execute(fft_plan);
    
    // copy out to preallocated result storage
    double running_mean = 0.0;
    double running_m2 = 0.0;
    
    double min = 1000000.0;
    double max = -1000000.0;
    
    for (int j = 0; j < (height * width * frames_per_sequence); j++) {
        double val = fft_out_storage[j][0];
        //double val = modulus_image[j][0];
        
        if(val < min) min = val;
        if(val > max) max = val;
        
        //double val = (1.0 + random_phase_storage[j][0]) / 2.;
        result_storage[j] = val;
        
        if(isinf(val)){
            problem_flag = true;
        }
        
        // compute running mean and variance
        // this is the classic "Knuth" algorithm
        double delta = val - running_mean;
        if(isinf(delta)){
            problem_flag = true;
        }
           
        running_mean += delta / (j+1);
        
        
        
        running_m2 += delta * (val - running_mean);
        
       if(isinf(running_m2)){
           problem_flag = true;
       }
           
//        if(j % 500 == 0){
//            std::cerr << result_storage[j] << std::endl;
//        }
    }
    
    std::cerr << "max: " << max << " min: " << min << std::endl;
    
    double stdev = sqrt(running_m2 / (width*height - 1));
    
    // Clean up the "plan"
    fftw_destroy_plan(fft_plan);    

    
    
    // renormalize the resulting mean luminance?
#define NORMALIZE  
#ifdef NORMALIZE
        for(int i = 0; i < (height*width*frames_per_sequence); i++){
            double newval = (0.5 + (result_storage[i] - running_mean) / stdev);
            
            result_storage[i] = newval;
            if(newval > max){
                max = newval;
            }
            if(newval < min){
                min = newval;
            }
        }

    mprintf("max: %g", max);
    mprintf("min: %g", min);
#endif    
    
    
//#define DUMP_FILE
    
#ifdef DUMP_FILE
    
    ofstream dumpfile;
    dumpfile.open("/Users/davidcox/Desktop/dump.dat", ios::out | ios::binary | ios::trunc);
    
    for(int i = 0; i < (height*width*frames_per_sequence); i++){
        dumpfile << result_storage[i] << std::endl;
    }
    
    dumpfile.close();
    
#endif
    
    // And we're done

}

