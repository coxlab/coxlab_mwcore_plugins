/*
 *  MouseViewIODevice.h
 *  MouseViewIODevice
 *
 *  Created by David Cox on 5/4/10.
 *  Copyright 2010 Harvard University. All rights reserved.
 *
 */

#ifndef MouseViewIODevice_H_
#define MouseViewIODevice_H_

#import "MouseViewIODevice_definitions.h"
#include <MWorksCore/ComponentInfo.h>
#include <MworksCore/ParameterValue.h>


#include <MWorksCore/StandardStimuli.h>
#include <MWorksCore/IODevice.h>
#include <MWorksCore/OpenGLContextManager.h>


#import "MouseViewTrackingProxy.h"
#include <iostream>

namespace mw{
    
class MouseViewIODevice : public IODevice {

protected:

    // variables for x, y, wheel
    shared_ptr<Variable> x, y, wheel, wheel_scale, counter, 
                         counter_max, counter_min,
                         wheel_max, wheel_min;
    
    bool enabled;
    
    
    double x_offset, x_scale, y_offset, y_scale;
    
    
    // an ObjC object for capturing mouse events
    MouseViewTrackingProxy * tracker;
    NSTrackingArea *tracking_area;
    

public:

    
    MouseViewIODevice(const ParameterValueMap &parameters);
    
    static void describeComponent(ComponentInfo& info);

	virtual ~MouseViewIODevice();

    virtual void finalize(std::map<std::string, std::string> parameters,
							  ComponentRegistry *reg){
                            
        // grab the display view, create a proxy object
        shared_ptr<OpenGLContextManager> context_manager = OpenGLContextManager::instance();
        NSView *display_view = context_manager->getMirrorView();        
        NSRect bounds = [display_view bounds];
        
        // grab the stimulus display object (this probably should be refactored)
        shared_ptr<StimulusDisplay> stim_display = GlobalCurrentExperiment->getStimulusDisplay();
        
        GLdouble l, r, b, t;
        stim_display->getDisplayBounds(l, r, b, t);
        
        x_scale = (double)(r - l) / (double)(bounds.size.width);
        y_scale = (double)(t - b) / (double)(bounds.size.height);
        x_offset = l - x_scale * bounds.origin.x;
        y_offset = b - y_scale * bounds.origin.y;
        
        NSLog(@"bounds: %f, %f, %f, %f", bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
        
        tracker = [[MouseViewTrackingProxy alloc] initWithIODevice:this];
        
        
        tracking_area = [[NSTrackingArea alloc] initWithRect: bounds
                                                options:NSTrackingMouseMoved | NSTrackingActiveAlways | NSTrackingInVisibleRect | NSTrackingAssumeInside
                                                owner: tracker
                                                userInfo: Nil];
        [display_view addTrackingArea:tracking_area];
        
        
    }
    
    void updatePosition(double x_, double y_){
    
        if(enabled){
            NSLog(@"Updating position %f, %f", x_, y_);
            
            double x_set = x_offset + x_scale * x_;
            double y_set = y_offset + y_scale * y_;
            x->setValue( x_set );
            y->setValue( y_set );
            NSLog(@"aka  %f, %f", x_set, y_set);
        }
    }

    void scrollWheel(double w){
        
        if(enabled){
        
            double wheel_val = wheel->getValue();
            
            wheel_val += (double)wheel_scale->getValue() * w;
            
            if(wheel_val > (double)*wheel_max){
                wheel_val = (double)*wheel_max;
            }
            
            if(wheel_val < (double)*wheel_min){
                wheel_val = (double)*wheel_min;
            }
            
            wheel->setValue(wheel_val);
        }
        
        //std::cerr << "wheel: " << wheel_val << std::endl;
    }


    void incrementCounter(){
        
        if(enabled){
            counter->setValue( (long)*counter + 1) ;
            
            if((int)counter->getValue() > (int)*counter_max){
                counter->setValue( (long)*counter_min );
            }
            //std::cerr << "counter: " << counter << std::endl;
        }
    }
    
    void toggleEnabled(){
        enabled = !enabled;
        NSLog(@"toggled: %d", enabled);

    }
    

};

}
#endif 
