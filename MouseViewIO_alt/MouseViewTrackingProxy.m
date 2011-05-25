//
//  MouseViewTrackingProxy.m
//  MouseViewIO
//
//  Created by David Cox on 5/4/11.
//  Copyright 2011 The Rowland Institute at Harvard. All rights reserved.
//

#import "MouseViewTrackingProxy.h"
#include "MouseViewIODevice.h"

@implementation MouseViewTrackingProxy

//- (id)init {
//
//   self = [super init];
//    if (self) {
//        //device = dev;
//    }
//    
//    return self; 
//}

- (id)initWithIODevice: (mw::MouseViewIODevice *) dev
{
    self = [super init];
    if (self) {
        device = dev;
    }
    
    event_monitor = [NSEvent addLocalMonitorForEventsMatchingMask:
                                (NSLeftMouseDownMask | NSRightMouseDownMask | NSScrollWheelMask)
                             handler: ^(NSEvent *incoming_event){
                                        [self handleEvents:incoming_event];
                                        return incoming_event;
                                      }
                    ];
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}


- (void)mouseEntered: (NSEvent *) event { }

- (void)mouseExited: (NSEvent *) event { }

- (void)mouseMoved: (NSEvent *) event {
    
    NSLog(@"Mouse moved!");
    NSPoint mouse_location = [event locationInWindow];
    double x = mouse_location.x;
    double y = mouse_location.y;
    
    device->updatePosition(x,y);
}

- (void) handleEvents: (NSEvent *) event {
    
    //NSLog(@"handling events....");
    NSEventType type = [event type];

    if(type == NSLeftMouseDown){
        device->toggleEnabled();
    } else if(type == NSRightMouseDown){
        device->incrementCounter();
    } else if(type == NSScrollWheel){
        device->scrollWheel([event deltaY]);
    }
}


@end
