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

// NOTE Flag to enable debug printing into serial.
// #define DEBUG_PRINTOUT

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

		// keep the offset and calibration constants in this object for easy
		// access without calling the get() methods on the transducer_
		int offset_;
		double constant_;

		// private module for setting the calibration constant individually
		passerModule<loadCellModule,double> calibrationModule_;
		passerModule<loadCellModule,void> tareModule_;

	public:
		loadCellModule(const char* sensorID, int dOutPin, int pdSckPin, int gain);
		loadCellModule(void);
		~loadCellModule(void);

		// NOTE: getValue() not implemented as leads to loss of precision is calibration
		// constant is small due to a long int being cast to an int. Since there's
		// no fail-safe way of doing that conversion without lack of precision
		// the getReading() method should be used instead.
		// int getValue(void);

		// getReading is the main data acquisition function and returns a double.
		double getReading(void);

		// set the value to zero; the function ignores the value passed to the
		// associated passer module, it treats the call to the passer object as
		// a boolean flag.
		void tare(void);

		// set the scaling constant
		void calibrate(double newConstant);

		// power the transducer down
		void powerDown(void);

		// power back up
		void powerUp(void);

		// access methods allowing the object to be interrogated
		// NOTE these should be const functions but the HX711 lib doesn't allow that...
		inline double getConstant() { return transducer_.get_scale(); }
		inline double getZero() { return transducer_.get_offset(); }

		// return pointers to passer functions
		Module* calibrationModulePtr() { return &calibrationModule_; };
		Module* tareModulePtr() { return &tareModule_; };
};

#endif
