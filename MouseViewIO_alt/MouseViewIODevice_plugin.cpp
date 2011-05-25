// THIS IS AN AUTO-GENERATED FILE: DO NOT EDIT IT DIRECTLY
// The content of this file is pulled from the project YAML description 
// see mw_idl.yml for details

/*
 *  MouseViewIODevicePlugin.cpp
 *  MouseViewIODevice
 *
 *
 */

#include "MouseViewIODevice_plugin.h"
#include "MWorksCore/ComponentFactory.h"
#include "MWorksCore/StandardComponentFactory.h"
#include "MouseViewIODevice_definitions.h"


Plugin *getPlugin(){
    return new MouseViewIODevicePlugin();
}


namespace mw{
    
void MouseViewIODevicePlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	
    registry->registerFactory<StandardComponentFactory, MouseViewIODevice>();
}
    
}

