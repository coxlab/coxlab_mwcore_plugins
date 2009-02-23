Phidgets for MacOS X
Version 2.1.5
October 16, 2008
--------------------------------------------------------------------------
Installation:
Run the Phidgets.mpkg package installer to install both the Kernel Driver, the Framework, and the JNI library, or you can install them seperately with their own installers.

The Kernel extension Phidget.kext is installed into /System/Library/Extensions
The Framework Phidget21.framework is installed into /Library/Frameworks
The JNI Library libphidget21.jnilib is installed into /System/Library/Frameworks/JavaVM.Framework/Libraries

Once these are installed, any programs built against the framework will run for any user on the system.

These is also a Preference Pane installed into System Preferences. This will list all Phidgets attached to the system. Double clicking on a Phidget in the list will bring up the example for that Phidget. The preference pane also has controls for starting the webservice and setting device labels.
--------------------------------------------------------------------------
Compatibility:
MacOS X 10.3.9 and higher
Distribution is in the form of Universal Binaries, and support Intel and PowerPC based Macs.
--------------------------------------------------------------------------
Usage:
The framework is basically just a dynamic shared library. The easiest way to use it is to create an xcode project and add the Phidget21 framework, then in your sourcecode you just have to define top level header:

	#include <Phidget21/phidget21.h>

In order for the xcode project to find Phidget21.h, you might have to add /Library/Frameworks to the frameworks search path for that projects.

Note that Phidgets can only be opened exclusively (if you open a phidget anywhere and then try to open it again in another program, it will fail).

The included examples are in XCode 2.4 format. If you can't open the project files, you probably need to upgrade, or you can create your own project and just add the sources. There are Cocoa examples for every device and one command line example. The cocoa built tools are included in the base examples folder.

There is also an included tool for setting labels, which are nonvolatile strings that can be set on most current phidgets. Each device has room for one 10 character string, which can be used to identify a device, or set of devices, rather then using serial numbers (or for any other purpose you like). The function for setting this string is not exposed, and the tool's source code not distributed, because setting the tag should not normally be done in a user program, as it will wear out the flash.
--------------------------------------------------------------------------
Phidget WebService:

The webservice is installed to /usr/bin as phidgetwebservice21

To use, just run phidgetwebservice21 from a command line. Use the -h switch to get options.
Alternatively, the webservice can be run from the Phidgets preference pane in System Preferences.