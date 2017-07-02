/* A class that serves to interact with load cell sensors
 *
 * Relies on the HX711 library from: https://github.com/bogde/HX711
 *
 * This header and the source have to be placed in the Arduino libraries' directory,
 * e.g. /usr/share/arduino/libraries/ in loadCellModule folder.
 *
 * @author: Artur Lidtke
 * @version: 1.0.0
 *
 * CHANGELOG
 * 10 Aug 2016 - 1.0.0 - released the first version.
 */
#ifndef loadCellModule_H
#define loadCellModule_H

#include "Arduino.h" // Basic Arduino stuff.
#include "Module.h" // The base class of all the actuators and sensors.
#include "HX711.h" // class used to interact with the sensors
#include "passerModule.h" // used to control individual pieces of funcitonality of this class

// Flag to enable debug printing into serial.
#define DEBUG_PRINTOUT

class loadCellModule : public Module
{
	private:
		// data output analog pin
		int dOutPin_;

		// power down and clock pin
		int pdSckPin_;

		// amplification factor - 32, 64, or 128
		int gain_;

		// transducer object from HX711 library
		HX711 transducer_;

		// private module for setting the calibration constant individually
		passerModule<loadCellModule,double> calibrationModule_;
		passerModule<loadCellModule,void> tareModule_;

	public:
		loadCellModule(const char* sensorID, int dOutPin, int pdSckPin, int gain);
		loadCellModule(void);
		~loadCellModule(void);

		// Override parent method to get outputs. getValue rounds to the nearest int
		int getValue(void);

		// getReading is the main data acquisition function and returns a double.
		double getReading(void);

		// set the value to zero
		void tare(void);

		// set the scaling constant
		void calibrate(double newConstant);

		// power the transducer down
		void powerDown(void);

		// power back up
		void powerUp(void);

		// return pointers to passer functions
		Module* calibrationModulePtr() { return &calibrationModule_; };
		Module* tareModulePtr() { return &tareModule_; };
};

#endif
