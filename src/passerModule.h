/* A class that serves to pass values to specific functions inside a parent object
 *
 * This header and the source have to be placed in the Arduino libraries' directory,
 * e.g. /usr/share/arduino/libraries/ in passerModule folder.
 *
 * @author: Artur Lidtke
 * @version: 1.0.0
 * 
 * CHANGELOG
 * 12 Aug 2016 - 1.0.0 - released the first version.
 */
#ifndef passerModule_H
#define passerModule_H

#include "Arduino.h" // Basic Arduino stuff.
#include "Module.h" // The base class of all the actuators and sensors.

template <class parentType, typename varType> class passerModule : public Module
{
	private:
		// parent object
		parentType* parentModule_;
		
		// function in the parent object (or elsewhere) which gets called when this
		// module's setValue() method gets called
		void (parentType::*functionPtr_)(varType);
		
	public:
		passerModule(parentType* parentModulePtr, const char* name, void (parentType::*functionPtr)(varType));
		passerModule(void) {};
		~passerModule(void) { delete parentModule_; }; // clean up 'em pointers
		
		// Override parent method.
		void setValue(int);
};

// to allow void functions to be called - needs a special definition with one
// template parameter overriden
template <class parentType> class passerModule<parentType,void> : public Module
{
	private:
		parentType* parentModule_;
		void (parentType::*functionPtr_)(void);
		
	public:
		passerModule(parentType* parentModulePtr, const char* name, void (parentType::*functionPtr)(void));
		passerModule(void) {};
		~passerModule(void) { delete parentModule_; };
		void setValue(int);
};

//-----------------
// need to define the functions in the header to avoid unresolved references when
// a new type of object gets used with the template
// an alternative would be to use forward declarations but that's not very general!
// see here for more info: http://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor

template <class parentType, typename varType> passerModule<parentType,varType>::passerModule(
	parentType* parentModulePtr, const char* name, void (parentType::*functionPtr)(varType))
:
	Module(name), // this is just a dummy name, will get changed in a sec
	parentModule_(parentModulePtr),
	functionPtr_(functionPtr)
{
	// append the desired name of this functionality to the name of the parent module
	char * fullNewID = (char *) malloc(1 + strlen(parentModulePtr->getIdentifier())+ strlen(name) );
	strcpy(fullNewID, parentModulePtr->getIdentifier());
	strcat(fullNewID, name);
    // set it as the name of this module for calling via serial
	this->setIdentifier(fullNewID);
};

// this uses the pointer to the parent object and its member class in order to pass
// the input value directly where it's needed
template <class parentType, typename varType> void passerModule<parentType,varType>::setValue(int newValue)
{
	((parentModule_)->*(functionPtr_))((varType) newValue);
};

//**********************************
// void funciton type
template <class parentType> passerModule<parentType,void>::passerModule(
	parentType* parentModulePtr, const char* name, void (parentType::*functionPtr)(void))
:
	Module(name),
	parentModule_(parentModulePtr),
	functionPtr_(functionPtr)
{
	char * fullNewID = (char *) malloc(1 + strlen(parentModulePtr->getIdentifier())+ strlen(name) );
	strcpy(fullNewID, parentModulePtr->getIdentifier());
	strcat(fullNewID, name);
	this->setIdentifier(fullNewID);
};

template <class parentType> void passerModule<parentType,void>::setValue(int newValue)
{
	((parentModule_)->*(functionPtr_))();
};

#endif
