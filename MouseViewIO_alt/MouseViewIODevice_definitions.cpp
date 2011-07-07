// THIS IS AN AUTO-GENERATED FILE: DO NOT EDIT IT DIRECTLY
// The content of this file is pulled from the project YAML description 
// see plugin_definition.yml for details

/*
 *  MouseViewIODevice_definitions.cpp
 *  MouseViewIODevice
 *
 *
 */

#include "MouseViewIODevice_definitions.h"
#include "MouseViewIODevice.h"

namespace mw{

    void MouseViewIODevice::describeComponent(ComponentInfo& info){
        IODevice::describeComponent(info);
        info.setSignature("iodevice/display_mouse_control");
        info.addParameter(X_PARAMETER, "0.0" );
        info.addParameter(Y_PARAMETER, "0.0" );
        info.addParameter(WHEEL_PARAMETER, "0.0" );
        info.addParameter(WHEEL_SCALE_PARAMETER, "0.0" );
        info.addParameter(WHEEL_MIN_PARAMETER, "0.0" );
        info.addParameter(WHEEL_MAX_PARAMETER, "0.0" );
        info.addParameter(COUNTER_PARAMETER, "0" );
        info.addParameter(COUNTER_MIN_PARAMETER, false);
        info.addParameter(COUNTER_MAX_PARAMETER, false);
    }

}