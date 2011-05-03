/*
 *  RectangleStimulusPlugins.cpp
 *  RectangleStimulusPlugins
 *
 *  Created by bkennedy on 8/14/08.
 *  Copyright 2008 MIT. All rights reserved.
 *
 */

#include "RectangleStimulusPlugin.h"
#include "MWorksCore/ComponentFactory.h"
#include "MWorksCore/StandardStimulusFactory.h"
#include "RectangleStimulus.h"

using namespace mw;

Plugin *getPlugin(){
    return new mRectangleStimulusPlugin();
}


void mRectangleStimulusPlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	registry->registerFactory<StandardStimulusFactory, RectangleStimulus>();
}
