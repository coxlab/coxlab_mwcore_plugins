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
    x(p[X_PARAMETER]),
    y(p[Y_PARAMETER]),
    wheel(p[WHEEL_PARAMETER]), 
    wheel_scale(p[WHEEL_SCALE_PARAMETER]), 
    wheel_min(p[WHEEL_MIN_PARAMETER]),
    wheel_max(p[WHEEL_MAX_PARAMETER]),
    counter(p[COUNTER_PARAMETER]),
    counter_min(p[COUNTER_MIN_PARAMETER]),
    counter_max(p[COUNTER_MAX_PARAMETER]),
    enabled(false)
{  }
    

MouseViewIODevice::~MouseViewIODevice(){ }


}
