/* A class that serves to interact with rotary encoder sensors.
 * Tested on: HEDS-9040 (http://uk.rs-online.com/web/p/rotary-encoders/7967874/)
 *
 * This header and the source have to be placed in the Arduino libraries' directory,
 * e.g. /usr/share/arduino/libraries/ in rotaryEncoder folder.
 *
 * @author: Artur Lidtke
 * @version: 1.0.0
 *
 * CHANGELOG
 * 18 Nov 2016 - 1.0.0 - released the first version.
 */
#ifndef rotaryEncoder_H
#define rotaryEncoder_H

#include "Arduino.h" // Basic Arduino stuff.

#include "Module.h" // The base class of all the actuators and sensors.
#include "passerModule.h" // used to control individual pieces of funcitonality of this class

class rotaryEncoder : public Module
{
	private:
		// pins for channels A, B, and I
		int pinChannelA_, pinChannelB_, pinChannelI_;

		// number of steps on the encoder wheel
		const int noSteps_;

		// stores the previous value recorded through the A channel
		int lastValueChannelA_;

		// indicates that the recorded value has changed since the last loop() call
		bool valueChanged_;

	public:
		rotaryEncoder(const char* sensorID, int pinA, int pinB, int pinI, int noSteps);
		rotaryEncoder(void);
		~rotaryEncoder(void);

		// this method must be called every time Arduino loop is executed
		// otherwise risk missing out some of the steps
		void loop();

		// access
		bool valueChanged(void) const { return valueChanged_; };
};

#endif
