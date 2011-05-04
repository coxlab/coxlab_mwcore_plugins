/*
 *  MouseViewIODevice.cpp
 *  MouseViewIODevice
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#include "MouseViewIODevice.h"

namespace mw{


MouseViewIODevice::MouseViewIODevice(const ParameterValueMap &p) :
    IODevice(),
    x(p[X_VARIABLE]),
    y(p[Y_VARIABLE]),
    wheel(p[WHEEL_VARIABLE]),
    wheel_scale(p[WHEEL_SCALE_VARIABLE])
{ 

    // get ahold of the main stimulus display view
    
    // add a tracking area to enable mouse event capture

}
    
void MouseViewIODevice::describeComponent(ComponentInfo& info){
    BasicTransformStimulus::describeComponent(info);
    info.setSignature("stimulus/display_bit_code");
    info.addParameter(X_VARIABLE);
    info.addParameter(Y_VARIABLE);
    info.addParameter(WHEEL_VARIABLE);
    info.addParameter(WHEEL_SCALE_VARIABLE);
}

const std::string MouseViewIODevice::X_VARIABLE("x");
const std::string MouseViewIODevice::Y_VARIABLE("y");
const std::string MouseViewIODevice::WHEEL_VARIABLE("wheel");
const std::string MouseViewIODevice::WHEEL_SCALE_VARIABLE("wheel_scale");


MouseViewIODevice::~MouseViewIODevice(){ }


}
