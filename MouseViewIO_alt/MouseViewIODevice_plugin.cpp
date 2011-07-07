// THIS IS AN AUTO-GENERATED FILE: DO NOT EDIT IT DIRECTLY
// The content of this file is pulled from the project YAML description 
// see plugin_definition.yml for details

/*
 *  MouseViewIODevicePlugin.cpp
 *  MouseViewIODevice
 *
 *
 */

#include "MouseViewIODevice.h"
#include "MouseViewIODevice_plugin.h"
#include "MWorksCore/ComponentFactory.h"
#include "MWorksCore/StandardComponentFactory.h"
#include "MouseViewIODevice_definitions.h"


Plugin *getPlugin(){
    return new MouseViewIODevicePlugin();
}


using namespace mw;

void MouseViewIODevicePlugin::registerComponents(shared_ptr<ComponentRegistry> registry) {
	
    registry->registerFactory<StandardComponentFactory, MouseViewIODevice>();
}

