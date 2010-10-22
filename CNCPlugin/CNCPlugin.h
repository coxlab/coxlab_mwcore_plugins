/*
 *  CNCPlugins.h
 *  CoxlabCorePlugins
 *
 *  Created by Brett Graham on 10/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <MWorksCore/Plugin.h>
using namespace mw;

extern "C"{
    Plugin *getPlugin();
}



class CNCPlugin : public Plugin {
    
	virtual void registerComponents(shared_ptr<ComponentRegistry> registry);	
};
