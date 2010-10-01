COBRA_PLUGIN=CobraTracker
PHIDGETS_PLUGIN=Phidgets

all: cobra-plugin phidgets-plugin

cobra-plugin:
	cd $(COBRA_PLUGIN); \
	xcodebuild clean -alltargets -configuration Development; \
	xcodebuild build -target Everything -configuration Development

phidgets-plugin:
	cd $(PHIDGETS_PLUGIN); \
	xcodebuild clean -alltargets -configuration Development; \
	xcodebuild build -target Everything -configuration Development
