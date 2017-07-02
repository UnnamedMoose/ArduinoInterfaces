/* A class that serves as an interface between an Arduino and a generic piece of
 * equipment, e.g. a sensor or an actuator. Other specific classes used to control
 * e.g brushless DC motors, LEDs or depth sensors are derived from this class.
 *
 * This header and the source have to be placed in the Arduino libraries' directory,
 * e.g. /usr/share/arduino/libraries/ in Module folder.
 *
 * @author: Aleksander & Artur Lidtke
 * @email: aleksadner.lidtke@gmail.com, artur.lidtke@gmail.com
 * @url: www.aleksanderlidtke.com
 * @since: 26 Sep 2015
 * @version: 1.0.1
 *
 * CHANGELOG
 *  6 Sep 2015 - 1.0.0 - Alek Lidtke - released the first version.
 * 26 Sep 2015 - 1.0.1 - Alek Lidtke - started to return 0 in the arm method by
 	default - now we use Modules directly to parse SW commands so need to call
	this method and return something meaningful.
 * 12 Aug 2016 - 1.0.2 - Artur Lidtke - added an option to set the identifiers
 	and set the destructor to virtual to allow derived classes to implement it as they please
 * 2 Jul 2017 - 1.0.3 - Artur Lidtke - added the getReading() method which allows
 	inhertiting classes to return floating point values. The idea is to then implement
	a method similar to CommunicationInterface::parseInputs() that will send readings
	for all connected sensors irresepctively of their type.
 */
#ifndef MODULE_H
#define MODULE_H

#include "Arduino.h" // Basic Arduino stuff.

class Module
{
	protected:
		const char* identifier_;
		int currentValue_;

	public:
		Module(void);
		Module(const char* newIdentifier);
		// need to implement in all derived classes if they do stuff with pointers
		virtual ~Module(void);

		// Sets control value of the module, e.g. thrust.
		virtual void setValue(int newValue);

		// Returns the current control value, e.g. sensor reading as an integer.
		virtual int getValue(void);

		// Same as getValue but returns a floating point number. This is needed for
		// sending outputs for which precision is important without converting to
		// integers on the Arduino side and then back to floats at the listening
		// device. Default behaviour is to simply cast double(getValue()), which allows
		// modules implementing integer functionality to just implement a single
		// virtual method.
		virtual double getReading(void);

		// Method encompassing setup of the module, i.e. the block of code called
		// during setup() in the main routine; returns the delay required for the
		// module to finish preparing
		virtual int arm(void);

		// set a new identifier
		void setIdentifier(const char* newID);

		// Returns the name of the module.
		const char* getIdentifier(void) const;

		// Serial frequency common for all modules.
		static const int serialBaudRate_;
};

#endif
