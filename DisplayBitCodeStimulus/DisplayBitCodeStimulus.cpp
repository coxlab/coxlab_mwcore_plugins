/*
 *  DisplayBitCodeStimulus.cpp
 *  DisplayBitCodeStimulus
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "DisplayBitCodeStimulus.h"

namespace mw{


DisplayBitCodeStimulus::DisplayBitCodeStimulus(const ParameterValueMap &p) :
    BasicTransformStimulus(p),
    code_variable(p[CODE_VARIABLE]),
    n_markers_variable(p[N_MARKERS]),
    separation_ratio_variable(p[SEPARATION]),
    bg_luminance_variable(p[BG_LUMINANCE]),
    fg_luminance_variable(p[FG_LUMINANCE]),
    generator(42u),
    uni_dist(1, 1 << (int)(n_markers_variable->getValue())),
    random_generator(generator, uni_dist)
{ }
    
void DisplayBitCodeStimulus::describeComponent(ComponentInfo& info){
    BasicTransformStimulus::describeComponent(info);
    info.setSignature("stimulus/display_bit_code");
    info.addParameter(CODE_VARIABLE);
    info.addParameter(N_MARKERS);
    info.addParameter(SEPARATION);
    info.addParameter(BG_LUMINANCE);
    info.addParameter(FG_LUMINANCE);
}

const std::string DisplayBitCodeStimulus::CODE_VARIABLE("code");
const std::string DisplayBitCodeStimulus::N_MARKERS("n_markers");
const std::string DisplayBitCodeStimulus::SEPARATION("separation_ratio");
const std::string DisplayBitCodeStimulus::BG_LUMINANCE("bg_luminance");
const std::string DisplayBitCodeStimulus::FG_LUMINANCE("fg_luminance");



DisplayBitCodeStimulus::~DisplayBitCodeStimulus(){ }


void DisplayBitCodeStimulus::drawInUnitSquare(shared_ptr<StimulusDisplay> display){

    
    int n = (int)(n_markers_variable->getValue());
    double sep_ratio = (double)(separation_ratio_variable->getValue());
    double marker_width = 1.0 / ((double)n + (double)(n+1)*sep_ratio);
    double sep_width = sep_ratio * marker_width;
    double marker_height = 1.0 / (1.0 + 2.0*sep_ratio);
    double sep_height = marker_height * sep_ratio;
    
    double bg_lum = (double)(bg_luminance_variable->getValue());
    double fg_lum = (double)(fg_luminance_variable->getValue());
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
    
    GLfloat z = 0.0;
    
    // draw background
    glBegin(GL_QUADS);
    glColor4f(bg_lum, bg_lum, bg_lum, *alpha_multiplier);
    glVertex3f(0.0,0.0,z);
    glVertex3f(1.0,0.0,z);
    glVertex3f(1.0,1.0,z);
    glVertex3f(0.0,1.0,z);
    glEnd();
    
    // generate a unique code
    int old_code = (int)(code_variable->getValue());
    int new_code = old_code;
    while(new_code == old_code){
        new_code = random_generator();
    }
    
    code_variable->setValue(Datum((long)new_code));
    
    int counter = 0;
    for(double x = sep_width; x <= 1.0; x += marker_width+sep_width){
        
        int bitmask = 1 << counter;
        counter++;
        
        if((new_code & bitmask) == bitmask){
            
            glBegin(GL_QUADS);
            glColor4f(fg_lum, fg_lum, fg_lum, *alpha_multiplier);
            glVertex3f(x,sep_height,z);
            glVertex3f(x+marker_width,sep_height,z);
            glVertex3f(x+marker_width,sep_height+marker_height,z);
            glVertex3f(x,sep_height+marker_height,z);
            glEnd();
        }
    }
    
    glDisable(GL_BLEND);
    
}


// override of base class to provide more info
Datum DisplayBitCodeStimulus::getCurrentAnnounceDrawData() {
    
    //mprintf("getting announce DRAW data for point stimulus %s",tag );
    
    Datum announceData(M_DICTIONARY, 2);
    announceData.addElement(STIM_NAME,tag);        // char
    announceData.addElement("bit_code",Datum((long)(*code_variable))); 

    return (announceData);
}

}
