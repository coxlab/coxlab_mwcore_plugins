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
    

MouseViewIODevice::~MouseViewIODevice(){ }


}
