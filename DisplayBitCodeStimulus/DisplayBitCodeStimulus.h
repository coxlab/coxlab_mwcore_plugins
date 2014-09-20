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

#include <boost/random.hpp>
#include <MWorksCore/Plugin.h>
#include <MWorksCore/StandardStimuli.h>
#include "MWorksCore/ComponentInfo.h"
#include "MworksCore/ParameterValue.h"
//#include "ClockSync.h"


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

    // RNG
    boost::minstd_rand generator;
    boost::uniform_int<> uni_dist;
    boost::variate_generator< boost::minstd_rand, boost::uniform_int<> > random_generator;
    
    
//    shared_ptr<ClockSync> clock_sync;
    shared_ptr<Variable> clock_offset_variable;
    

public:

    
    DisplayBitCodeStimulus(const ParameterValueMap &parameters);
    
    static void describeComponent(ComponentInfo& info);

    static const std::string CODE_VARIABLE;
    static const std::string N_MARKERS;
    static const std::string SEPARATION;
    static const std::string BG_LUMINANCE;
    static const std::string FG_LUMINANCE;
    static const std::string CLOCK_OFFSET;


	~DisplayBitCodeStimulus();

    virtual void drawInUnitSquare(shared_ptr<StimulusDisplay> display);
    virtual Datum getCurrentAnnounceDrawData();
};

}
#endif 
