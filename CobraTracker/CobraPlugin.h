/*
 *  CoxlabCorePlugins.h
 *  CoxlabCorePlugins
 *
 *  Created by David Cox on 4/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <MWorksCore/Plugin.h>
using namespace mw;

extern "C"{
    Plugin *getPlugin();
}



class CobraPlugin : public Plugin {
    
	virtual void registerComponents(shared_ptr<ComponentRegistry> registry);	
};
