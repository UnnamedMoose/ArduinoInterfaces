#include "Module.h"

// Initialise the static members.
const int Module::serialBaudRate_ = 19200; // Have to repeat const int in initialisation, for some reason.

Module::Module(void)
/* Default constructor, initialises all the fields with clearly default values
 * that will throw errors when they are used without conscious setting.
 */
{
	identifier_ = "UNKNOWN_MODULE";
	currentValue_ = 0;
	Serial.begin(serialBaudRate_); // Start serial comms.
}

Module::Module(const char* newIdentifier)
/* Initialise this instance of the Module.
 */
{
	identifier_ = newIdentifier;
	currentValue_ = 0;
	Serial.begin(serialBaudRate_); // Start serial comms.
}

Module::~Module(void){};/* Do nothing special here.*/

void Module::setValue(int newValue)
/* Set the current value for this piece of equipment. If it's an actuator it
 * will set e.g. the motor throttle to the desired value.
 *
 * N.B. it will also override the current sensor value, so be careful and don't
 * use it on sensors!
 *
 * @param newValue - the value to override the currentValue field with.
 */
{
	currentValue_ = newValue;
}

int Module::getValue(void)
/* Get the currentValue held by this instance, e.g. sensor reading or the intended
 * actuator value, say engine thrust.
 *
 * @return - the currently held value of the module.
 */
{
	return currentValue_;
}

// Send current sensor value as a dobule precision float.
double Module::getReading(void)
{
	return double(getValue());
}


int Module::arm(void)
/* Method encompassing setup of the module, i.e. the block of code called
 * during setup() in the main routine
 *
 * @return - the delay required for the module to finish preparing
 */
{
// Do nothing by default.
	return 0;
}

void Module::setIdentifier(const char* newID)
{
	identifier_ = newID;
}

const char* Module::getIdentifier(void) const
/* Get the identifer of this Module. */
{
	return identifier_;
}
