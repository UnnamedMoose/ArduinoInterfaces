#include "loadCellModule.h"

/*
- use the read() method to get an int withot bothering with the conversion stuff
- change getValue() to return an int that's been scaled and zeroed

*/

loadCellModule::loadCellModule(const char* sensorID, int dOutPin, int pdSckPin, int gain)
:
	Module(sensorID),
	dOutPin_(dOutPin),
	pdSckPin_(pdSckPin),
	gain_(gain),
	transducer_(dOutPin_, pdSckPin_, gain_),
	calibrationModule_(this, "Calibrate", &loadCellModule::calibrate), // individual calibration module
	tareModule_(this, "Tare", &loadCellModule::tare)
{};

loadCellModule::loadCellModule(void)
:
	// need a dummy because the library doesn't support a default constructor (fail)
	// use the LED_PIN which will never do anything
	transducer_(14, 14, 128)
{};

loadCellModule::~loadCellModule(void) {};

// read the value of the transducer and round to the nearest int
int loadCellModule::getValue(void)
{
	return (int) floor(transducer_.get_units()+0.5);
}

// return the reading without casting to an int
double loadCellModule::getReading(void)
{
	return transducer_.get_units();
}

// set sensor reading to zero
void loadCellModule::tare(void)
{
	#ifdef DEBUG_PRINTOUT
	Serial.print(getIdentifier());
	Serial.print(": Setting reading to zero at the current excitation of ");
	Serial.println(getReading());
	#endif

	transducer_.tare();
}

// set the calibration constant
void loadCellModule::calibrate(double newConstant)
{
	#ifdef DEBUG_PRINTOUT
	Serial.print(getIdentifier());
	Serial.print(": Calibrating with value ");
	Serial.println(newConstant);
	#endif

	transducer_.set_scale(newConstant);
}

void loadCellModule::powerDown(void)
{
	transducer_.power_down();
}

void loadCellModule::powerUp(void)
{
	transducer_.power_up();
}
