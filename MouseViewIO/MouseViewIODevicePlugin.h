/*
 *  MouseViewIODevicePlugin.h
 *  MouseViewIODevice
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */
#ifndef MouseViewIODevice_PLUGIN_H_
#define MouseViewIODevice_PLUGIN_H_


#include <MWorksCore/Plugin.h>
using namespace mw;

extern "C"{
    Plugin *getPlugin();
}


namespace mw {
    
class MouseViewIODevicePlugin : public Plugin {
    
	virtual void registerComponents(shared_ptr<mw::ComponentRegistry> registry);	
};

    
}

#endif