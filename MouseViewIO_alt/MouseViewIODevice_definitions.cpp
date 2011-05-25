// THIS IS AN AUTO-GENERATED FILE: DO NOT EDIT IT DIRECTLY
// The content of this file is pulled from the project YAML description 
// see mw_idl.yml for details

/*
 *  MouseViewIODevice_definitions.cpp
 *  MouseViewIODevice
 *
 *
 */

#include "MouseViewIODevice_definitions.h"

namespace mw{

    const std::string X("x");
    const std::string Y("y");
    const std::string WHEEL("wheel");
    const std::string WHEEL_SCALE("wheel_scale");
    const std::string WHEEL_MIN("wheel_min");
    const std::string WHEEL_MAX("wheel_max");
    const std::string COUNTER("counter");
    const std::string COUNTER_MIN("counter_min");
    const std::string COUNTER_MAX("counter_max");

    void MouseViewIODevice::describeComponent(ComponentInfo& info){
        IODevice::describeComponent(info);
        info.setSignature("iodevice/display_mouse_control");
        info.addParameter(X);
        info.addParameter(Y);
        info.addParameter(WHEEL);
        info.addParameter(WHEEL_SCALE);
        info.addParameter(WHEEL_MIN);
        info.addParameter(WHEEL_MAX);
        info.addParameter(COUNTER);
        info.addParameter(COUNTER_MIN);
        info.addParameter(COUNTER_MAX);
    }

}