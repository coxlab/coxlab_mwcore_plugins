//
//  MouseViewTrackingProxy.m
//  MouseViewIO
//
//  Created by David Cox on 5/4/11.
//  Copyright 2011 The Rowland Institute at Harvard. All rights reserved.
//

#import "MouseViewTrackingProxy.h"


@implementation MouseViewTrackingProxy

- (id)initWithIODevice: (MouseViewIODevice *) dev
{
    self = [super init];
    if (self) {
        device = dev;
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}


- (void)mouseMoved: (NSEvent *) event {

}

@end
