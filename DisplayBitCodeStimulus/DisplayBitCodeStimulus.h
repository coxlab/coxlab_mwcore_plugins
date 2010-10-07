/*
 *  DisplayBitCodeStimulus.h
 *  DisplayBitCodeStimulus
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#ifndef DisplayBitCodeStimulus_H_
#define DisplayBitCodeStimulus_H_

#include <MWorksCore/Plugin.h>
#include <MWorksCore/StandardStimuli.h>
#include <boost/random.hpp>


namespace mw{
    
class DisplayBitCodeStimulus : public BasicTransformStimulus{

protected:

    // a variable for recording the current code value
    shared_ptr<Variable> code_variable;
    
    // number of bit markers to encode
    shared_ptr<Variable> n_markers_variable;
    
    // ratio of the size of the separation to the size of the bit markers
    shared_ptr<Variable> separation_ratio_variable;
    
    shared_ptr<Variable> bg_luminance_variable, fg_luminance_variable;
    
    boost::minstd_rand generator;
    boost::uniform_real<> uni_dist;
    boost::variate_generator< boost::minstd_rand, boost::uniform_real<> > random_generator;
    
    

public:
	DisplayBitCodeStimulus(std::string _tag, shared_ptr<Variable> _code,
                           shared_ptr<Variable> _n_markers, 
                           shared_ptr<Variable> _separation,
                           shared_ptr<Variable> _bg_luminance,
                           shared_ptr<Variable> _fg_luminance,
                           shared_ptr<Variable> _xoffset, 
                           shared_ptr<Variable> _yoffset,
                           shared_ptr<Variable> _xscale, 
                           shared_ptr<Variable> _yscale, 
                           shared_ptr<Variable> _rot,
                           shared_ptr<Variable> _alpha);
    
	//DisplayBitCodeStimulus(const DisplayBitCodeStimulus &tocopy);
	~DisplayBitCodeStimulus();

    virtual void drawInUnitSquare(shared_ptr<StimulusDisplay> display);
    virtual Datum getCurrentAnnounceDrawData();
};

}
#endif 
