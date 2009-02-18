COBRA_PLUGIN=CobraTracker

all: cobra-plugin 

cobra-plugin:
	cd $(COBRA_PLUGIN); \
	xcodebuild clean -alltargets -configuration Debug; \
	xcodebuild build -target Everything -configuration Debug
