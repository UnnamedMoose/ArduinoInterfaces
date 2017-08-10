#include "loadCellModule.h"

loadCellModule::loadCellModule(const char* sensorID, int dOutPin, int pdSckPin, int gain)
:
	Module(sensorID),
	dOutPin_(dOutPin),
	pdSckPin_(pdSckPin),
	gain_(gain),
	transducer_(dOutPin_, pdSckPin_, gain_),
	offset_(0),
	constant_(1.),
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

// read the value of the transducer as an integer
// int loadCellModule::getValue(void)
// {
// 	return int( (transducer_.read() - offset_)/constant_ );
// }

// return the reading without casting to an int
double loadCellModule::getReading(void)
{
	// use the get_value method as that returns a double, not a float like
	// get_units does. That means scaling needs to be done here.
	return transducer_.get_value()/constant_;
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
	offset_ = transducer_.get_offset();
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
	constant_ = newConstant;
}

void loadCellModule::powerDown(void)
{
	transducer_.power_down();
}

void loadCellModule::powerUp(void)
{
	transducer_.power_up();
}
