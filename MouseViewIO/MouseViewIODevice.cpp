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
    wheel_scale(p[WHEEL_SCALE_VARIABLE]), 
    wheel_min(p[WHEEL_MIN_VARIABLE]),
    wheel_max(p[WHEEL_MAX_VARIABLE]),
    counter(p[COUNTER_VARIABLE]),
    counter_min(p[COUNTER_MIN_VARIABLE]),
    counter_max(p[COUNTER_MAX_VARIABLE]),
    enabled(false)
{  }
    
void MouseViewIODevice::describeComponent(ComponentInfo& info){
    IODevice::describeComponent(info);
    info.setSignature("iodevice/display_mouse_control");
    info.addParameter(X_VARIABLE);
    info.addParameter(Y_VARIABLE);
    info.addParameter(WHEEL_VARIABLE);
    info.addParameter(WHEEL_SCALE_VARIABLE);
    info.addParameter(WHEEL_MIN_VARIABLE);
    info.addParameter(WHEEL_MAX_VARIABLE);
    info.addParameter(COUNTER_VARIABLE);
    info.addParameter(COUNTER_MIN_VARIABLE);
    info.addParameter(COUNTER_MAX_VARIABLE);
}

const std::string MouseViewIODevice::X_VARIABLE("x");
const std::string MouseViewIODevice::Y_VARIABLE("y");
const std::string MouseViewIODevice::WHEEL_VARIABLE("wheel");
const std::string MouseViewIODevice::WHEEL_SCALE_VARIABLE("wheel_scale");
const std::string MouseViewIODevice::COUNTER_VARIABLE("counter");
const std::string MouseViewIODevice::COUNTER_MIN_VARIABLE("counter_min");
const std::string MouseViewIODevice::COUNTER_MAX_VARIABLE("counter_max");
const std::string MouseViewIODevice::WHEEL_MIN_VARIABLE("wheel_min");
const std::string MouseViewIODevice::WHEEL_MAX_VARIABLE("wheel_max");

MouseViewIODevice::~MouseViewIODevice(){ }


}
