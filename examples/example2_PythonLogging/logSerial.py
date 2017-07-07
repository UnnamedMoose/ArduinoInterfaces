#!/usr/bin/python
"""
Read data from a serial port and divert them to a log file.

@author: Artur Lidtke
@email: artur.lidtke@gmail.com
@since: 7 Jul 2017
@version: 1.0.0

CHANGELOG
7 Jul 2017 - 1.0.0 - Artur Lidtke - released the first version.
"""

# Python library for interacting with the serial port
import serial

# used for timing the reading form the buffer
import time

# # Add the Python library to PYTHONPATH.
# # NOTE: a much better way to do this is to set your PYTHONPATH environment variable
# # to include the locaiton of the library. This example does it the hard way to
# # make sure the code will work right after downloading.
# import sys
# sys.path.append("../../pythonSrc")

# # load the custom Python module
# import serialCommunicationProtocol as comms

#==========

# top-level control of the script
serialPort = "/dev/ttyACM0"
logfile = "./serial.log"
baudRate = 19200

# other constants
readDelay = 1. # in second
linesBeforeWrite = 5 # keep N lines before dumping to the file

#==========

# This will keep a list of lines ready to be written to the log file. The buffer
# will be written once a fixed number of lines has been accumulated to avoid
# I/O operations slowing the program down.
logFileBuffer = []

# Keeps all the data read from the serial port in raw format, before it is split
# into individual lines and handled properly.
arduinoOutputBuffer = ""

# attempt to open a connection. serial library will raise a specific error if it fails to do so
arduinoSerialConnection = serial.Serial(serialPort, baudRate, timeout = 2)

# use a non-blocking approach to read the data - this is generally
# much less disruptive to the overall program flow than relying
# on Serial.readline()

# open the log file for writing
with open(logfile,"w") as f:
    # continue reading and logging lines indefinitely (until interrupt)
    while True:
        try:
            # if incoming bytes are waiting to be read from the serial input buffer
            if (arduinoSerialConnection.inWaiting()>0):

                # read the bytes and convert from binary array to ASCII, keep them in the buffer
                arduinoOutputBuffer += arduinoSerialConnection.read(
                    arduinoSerialConnection.inWaiting() ).decode('ascii')

                # extract any full lines and log them - there can be more than
                # one, depending on the loop frequencies on either side of the
                # serial conneciton
                lines = arduinoOutputBuffer.rpartition("\n")
                if lines[0]:
                    for line in lines[0].split("\n"):
                        # keep the line for loging
                        logFileBuffer.append(line+"\n")

                    # keep the last chunk without any EOL chars in the buffer as it may
                    # contain an unfinished line
                    arduinoOutputBuffer = lines[2]

        except UnicodeDecodeError:
            # sometimes rubbish gets fed to the serial port upon initialisation,
            # just let it go
            pass

        # see if logged enough lines to write to file
        if len(logFileBuffer) > linesBeforeWrite:
            # write
            f.writelines(logFileBuffer)
            # clear the buffer
            logFileBuffer[:] = []

        # wait until next pass
        time.sleep(readDelay)
