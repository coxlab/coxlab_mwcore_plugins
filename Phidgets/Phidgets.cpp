/*
 *  Phidgets.cpp
 *  MWorksCore
 *
 *  Created by David Cox on 1/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Phidgets.h"
#include "MWorksCore/ComponentRegistry.h"
#include "MWorksCore/StandardComponentFactory.h"

using namespace mw;


template<>
PhidgetChannelType ParameterValue::convert(const std::string &s, ComponentRegistryPtr reg) {
    std::string type_string(boost::algorithm::to_lower_copy(s));
    
    if (type_string == "analog_input") {
        return M_PHIDGET_ANALOG_INPUT;
//    } else if (type_string == "analog_output") {
//        return M_PHIDGET_ANALOG_OUTPUT;
    } else if (type_string == "digital_input") {
        return M_PHIDGET_DIGITAL_INPUT;
    } else if (type_string == "digital_output") {
        return M_PHIDGET_DIGITAL_OUTPUT;
    } else {
        throw SimpleException("Unknown channel type (must be one of: [analog_input, analog_output, digital_input, digital_output])", type_string);
    }
}


PhidgetDeviceChannel::PhidgetDeviceChannel(const ParameterValueMap &p) :
    channel_type((PhidgetChannelType)p[CHANNEL_TYPE]),
    index((int)p[INDEX]),
    sensitivity((int)p[SENSITIVITY]),
    rate((int)p[RATE]),
    variable(p[VARIABLE])
{
    ifKit = NULL;
}

void PhidgetDeviceChannel::describeComponent(ComponentInfo& info){
    info.setSignature("iochannel/phidgets");
    info.addParameter(CHANNEL_TYPE);
    info.addParameter(INDEX);
    info.addParameter(SENSITIVITY);
    info.addParameter(RATE);
    info.addParameter(VARIABLE);
}

const std::string PhidgetDeviceChannel::CHANNEL_TYPE("type");
const std::string PhidgetDeviceChannel::VARIABLE("variable");
const std::string PhidgetDeviceChannel::INDEX("index");
const std::string PhidgetDeviceChannel::SENSITIVITY("sensitivity");
const std::string PhidgetDeviceChannel::RATE("rate");







PhidgetDevice::~PhidgetDevice(){
	//since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
	
	stopDeviceIO();
	
	if(ifKit){
		CPhidget_close((CPhidgetHandle)ifKit);
		CPhidget_delete((CPhidgetHandle)ifKit);
	}
}
		


bool PhidgetDevice::startDeviceIO(){
	setActive(true);
	return true;
}
	

bool PhidgetDevice::stopDeviceIO(){
	setActive(false);
	return true;
}


void PhidgetDevice::addChild(std::map<std::string, std::string> parameters,
								ComponentRegistry *reg,
								shared_ptr<Component> _child){
	shared_ptr<PhidgetDeviceChannel> child = boost::dynamic_pointer_cast<PhidgetDeviceChannel, Component>(_child);
	
	
	if(child == NULL){
		throw SimpleException("Failed to cast phidget device channel");
	}
	
	int index = child->getIndex();
	PhidgetChannelType type = child->getType();
	
	child->setDevice(ifKit);
	
	weak_ptr<PhidgetDeviceChannel> wchild(child);
	
	if(type == M_PHIDGET_ANALOG_INPUT){
		analog_input_channels[index] = wchild;
        configureAnalogChannel(index);
	} else if(type == M_PHIDGET_DIGITAL_INPUT){
		digital_input_channels[index] = wchild;
	} else if(type == M_PHIDGET_DIGITAL_OUTPUT){
		digital_output_channels[index] = wchild;
		shared_ptr<Variable> var = child->getVariable();
		shared_ptr<VariableNotification> notif(new PhidgetDeviceOutputNotification(child));
		var->addNotification(notif);
	} else {
		throw SimpleException("Unknown phidget channel type");
	}
}

bool PhidgetDevice::configureAnalogChannel(int i){
    shared_ptr<PhidgetDeviceChannel> channel = getAnalogInputChannel(i);
    if(channel == NULL){
        mwarning(M_SYSTEM_MESSAGE_DOMAIN, "Could not configure channel");
        printf("failed to acquire and configure channel %i", i);
        return false;
    }
    CPhidgetInterfaceKit_setDataRate(ifKit, i, channel->getRate());
    printf("phidget channel %i rate set to %i\n", i, channel->getRate());
    CPhidgetInterfaceKit_setSensorChangeTrigger(ifKit, i, channel->getSensitivity());
    printf("phidget channel %i sensitivity set to %i\n", i, channel->getSensitivity());
    return true;
}


int AttachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName(IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int DetachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
	printf("Error handled. %i - %s", ErrorCode, unknown);
	return 0;
}

//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
int InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
	PhidgetDevice *device = (PhidgetDevice *)usrptr;
	
	if(device->getActive()){
	
		shared_ptr<PhidgetDeviceChannel> channel = device->getDigitalInputChannel(Index);
		
		if(channel == NULL){
			return 0;
		}
		
		
		shared_ptr<Variable> var = channel->getVariable();
		if(var != NULL){
			var->setValue((long)State);
		}
	}
	
	return 0;
}

//callback that will run if an output changes.
//Index - Index of the output that generated the event, State - boolean (0 or 1) representing the output state (on or off)
int OutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
	printf("Digital Output: %d > State: %d\n", Index, State);
	return 0;
}

//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
int SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
	
	
	PhidgetDevice *device = (PhidgetDevice *)usrptr;
	
	if(device->getActive()){
	
		shared_ptr<PhidgetDeviceChannel> channel = device->getAnalogInputChannel(Index);
		
		if(channel == NULL){
			mwarning(M_SYSTEM_MESSAGE_DOMAIN, "Invalid channel change trigger from phidgets device");
			return 0;
		}

		shared_ptr<Variable> var = channel->getVariable();
		var->setValue((long)Value);
	}
	
	return 0;
}


