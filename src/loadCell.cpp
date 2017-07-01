#include "loadCell.h"

loadCell::loadCell(const char* sensorID, int dOutPin, int pdSckPin, int gain)
:
	Module(sensorID),
	dOutPin_(dOutPin),
	pdSckPin_(pdSckPin),
	gain_(gain),
	transducer_(dOutPin_, pdSckPin_, gain_),
	calibrationModule_(this, "Calibrate", &loadCell::calibrate), // individual calibration module
	tareModule_(this, "Tare", &loadCell::tare)
{};

loadCell::loadCell(void)
:
	// need a dummy because the library doesn't support a default constructor (fail)
	// use the LED_PIN which will never do anything
	transducer_(14, 14, 128)
{};

loadCell::~loadCell(void) {};

// read the value of the transducer and round to the nearest int
int loadCell::getValue(void)
{
	return (int) floor(transducer_.get_units()+0.5);
}

// return the reading without casting to an int
float loadCell::getReading(void)
{
	return transducer_.get_units();
}

// set sensor reading to zero
void loadCell::tare(void)
{
/*
Serial.print(getIdentifier());
Serial.print(": Setting reading to zero at the current excitation of ");
Serial.println(getReading());*/
	transducer_.tare();
}

// set the calibration constant
void loadCell::calibrate(float newConstant)
{
/*
Serial.print(getIdentifier());
Serial.print(": Calibrating with value ");
Serial.println(newConstant);*/
	transducer_.set_scale(newConstant);
}

void loadCell::powerDown(void)
{
	transducer_.power_down();
}

void loadCell::powerUp(void)
{
	transducer_.power_up();
}
