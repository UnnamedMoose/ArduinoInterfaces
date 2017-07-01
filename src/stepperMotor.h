/* A class that serves to interact with stepper motors using Darlington transistor arrays
 *
 * This header and the source have to be placed in the Arduino libraries' directory,
 * e.g. /usr/share/arduino/libraries/ in stepperMotor folder.
 *
 * @author: Artur Lidtke
 * @version: 1.0.0
 *
 * CHANGELOG
 * 18 Nov 2016 - 1.0.0 - released the first version.
 */

#ifndef stepperMotor_H
#define stepperMotor_H

#include "Arduino.h" // Basic Arduino stuff.
#include <Stepper.h> // stepper motor library from Arduino

#include "Module.h" // The base class of all the actuators and sensors.
#include "passerModule.h" // used to control individual pieces of funcitonality of this class

class stepperMotor : public Module
{
	private:
		// how many steps per revolution
		int stepsPerRevolution_;

		// counts the number of steps the motor has taken
		int stepCount_;

		// controls the motor speed
		int motorSpeed_;

		// provides an interface the stepper library
		Stepper stepper_;

		// private modules for controlling the motor speed
		passerModule<stepperMotor,int> speedModule_;

	public:
		stepperMotor(const char* sensorID, int stepsPerRevolution, int motorSpeed, int pin0, int pin1, int pin2, int pin3);
		stepperMotor(void);
		~stepperMotor(void);

		// Override parent method to set inputs - orders the motor to take a given number of steps
		// sign of the value denotes direction
		void setValue(int newValue);

		// take a single step; sign of the argument denotes direction
		void step(int dir);

		// set the new speed of the motor
		void setSpeed(int newSpeed);

		// return pointers to passer functions
		Module* speedModulePtr() { return &speedModule_; };

		// access
		int getStepsPerRevolution(void) const { return stepsPerRevolution_; };
		int getStepCount(void) const { return stepCount_; };
};

#endif
