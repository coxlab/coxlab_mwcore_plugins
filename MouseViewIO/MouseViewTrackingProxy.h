//
//  MouseViewTrackingProxy.h
//  MouseViewIO
//
//  Created by David Cox on 5/4/11.
//  Copyright 2011 The Rowland Institute at Harvard. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <boost/shared_ptr.hpp>

namespace mw{
    class MouseViewIODevice; // foward decl.
}

@interface MouseViewTrackingProxy : NSObject {
@private

   mw::MouseViewIODevice *device;
   id event_monitor;
    
}

@end
