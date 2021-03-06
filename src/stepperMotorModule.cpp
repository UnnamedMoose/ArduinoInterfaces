#include "stepperMotorModule.h"

stepperMotorModule::stepperMotorModule(const char* sensorID, int stepsPerRevolution, int motorSpeed,
		int pin0, int pin1, int pin2, int pin3)
:
	Module(sensorID),
	stepsPerRevolution_(stepsPerRevolution),
	stepCount_(0),
	motorSpeed_(motorSpeed),
	stepper_(stepsPerRevolution,pin0,pin1,pin2,pin3),
	speedModule_(this, "SetSpeed", &stepperMotorModule::setSpeed)
{
	setSpeed(motorSpeed);
};

stepperMotorModule::stepperMotorModule(void)
:
	// Need a dummy default constructor for the Stepper module instance since, of course,
	// it's not provided in the Arduino base; use the LED pin and hope it'll be OK
	stepper_(10,13,13,13,13)
{};

stepperMotorModule::~stepperMotorModule(void) {};

// take as many steps as required
void stepperMotorModule::setValue(int newValue)
{
	for (int i = 0; i < abs(newValue); i++)
		step(newValue);
}

// take a single step; sign of the argument denotes direction
void stepperMotorModule::step(int dir)
{
	if (dir >= 0)
	{
		stepper_.step(1);
		stepCount_++;
	}
	else
	{
		stepper_.step(-1);
		stepCount_--;
	}
}

// set the speed
void stepperMotorModule::setSpeed(int newSpeed)
{
	/*Serial.print(getIdentifier());
	Serial.print(": Setting new speed to ");
	Serial.println(newSpeed);*/
	stepper_.setSpeed(newSpeed);
}
