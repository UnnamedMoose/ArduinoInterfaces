# Welcome to the set of libraries for communicating with an Arduino!

These provide a basic framework for interacting with an Arduino via a serial port
and using it to control simple pieces of hardware. A Python interface for parsing
the oncoming data is also provided and may be used to automate the control and
logging process.

Tested with Ubuntu 14.04 Linux, Python 2.7.6 and gcc 4.8.4

## Requirements

1. HX711 drivers
	https://github.com/bogde/HX711

2. Arduino makefile (not necessary but recommended)
	https://github.com/sudar/Arduino-Makefile

3. Dedicated serial monitor (not necessary, can use any other variant, including
	the one provided with the Arduino IDE)
	https://github.com/UnnamedMoose/serialMonitor

## Using the examples

1. Install the code provided in `src` by executing the `installLibraries` bash script

2. Go to `examples`, upload each `.ino` file to the Arduino either using the makefile
	provided (make sure to use the correct designation for your board and the right
	serial port) or the Ardunio IDE.

3. Log the data and interact with the micro-controller using the serial monitor
	or the Python interface for example 2.

4. ???

5. Profit.
