# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

# Path to the user libraries (this is the default to where all the built-in code
# that ships with Arduino deb-pack installation gets placed).
USER_LIB_PATH = /usr/share/arduino/libraries/

# List of custom libraries to link with this executable.
ARDUINO_LIBS  = Module LEDModule CommunicationInterface

# Tag describing the type of board being used. See the gihub page above for more info.
BOARD_TAG     = uno

# Serial port to which the USB cable is connected.
ARDUINO_PORT  = /dev/ttyACM0

# This should point to where the makefile is installed.
# This is the default for a deb-pack (apt-get) installation.
include /usr/share/arduino/Arduino.mk
