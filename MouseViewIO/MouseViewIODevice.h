/*
 *  MouseViewIODevice.h
 *  MouseViewIODevice
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#ifndef MouseViewIODevice_H_
#define MouseViewIODevice_H_

#include <MWorksCore/Plugin.h>
#include <MWorksCore/StandardStimuli.h>
#include <boost/random.hpp>
#include "MWorksCore/ComponentInfo.h"
#include "MworksCore/ParameterValue.h"


namespace mw{
    
class MouseViewIODevice : public IODevice {

protected:

    // variables for x, y, wheel
    shared_ptr<Variable> x, y;
    shared_ptr<Variable> wheel;
    
    // scaling factors so that x + y are in degrees
    double scaling_factor;
    
    // an ObjC object for capturing mouse events
    MouseViewTrackingProxy *tracker;
    

public:

    static const std::string MouseViewIODevice::X_VARIABLE;
    static const std::string MouseViewIODevice::Y_VARIABLE;
    static const std::string MouseViewIODevice::WHEEL_VARIABLE;
    static const std::string MouseViewIODevice::WHEEL_SCALE_VARIABLE;

    
    MouseViewIODevice(const ParameterValueMap &parameters);
    
    static void describeComponent(ComponentInfo& info);

	virtual ~MouseViewIODevice();

};

}
#endif 
