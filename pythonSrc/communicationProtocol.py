# -*- coding: utf-8 -*-
"""
Created on Sun May 24 16:34:31 2015

@author: artur
"""

# TODO wrap-up sending and receiving data protocols from Arduino here

#TODO hold serial connection to the Arduino here

import sys, serial, glob

# constants for dfinig the format of a data packet
OUTPUT_START_CHAR = '>'
INPUT_START_CHAR = '<'
MESSAGE_END_CHAR = ';'
DATA_DELIMITER = ','

def getActivePorts():
    """ find the open ports - main part of the code from:
    http://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
    """
    if sys.platform.startswith('win'):
        candidatePorts = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        candidatePorts = glob.glob('/dev/tty[A-Za-z]*')

    elif sys.platform.startswith('darwin'):
        candidatePorts = glob.glob('/dev/tty.*')

    else:
        raise EnvironmentError('Unsupported platform')

    ports = []
    for port in candidatePorts:
        try:
            s = serial.Serial(port)
            s.close()
            ports.append(port)
        except (OSError, serial.SerialException):
            pass

    return ports

def sendMessage(serialConnection,keywords,returnMsg=True,sendRaw=False):
    """ Send a message to the given serial connection
    Arguments
    ---------
        @param serialConnection - serial port
        @param keywords - dictionary of values and their descriptors to be sent
    Optional
    ---------
        @param returnMsg - whether to return the final message or not, default True
        @param sendraw - if this is True, keywords will be treated as a string and
            will not be modified; useful for passing ready commands from a propmpt
    """
    if sendRaw:
        msg = keywords
    else:
        msg = OUTPUT_START_CHAR
        for key in keywords:
            msg += '{}{}{}{}'.format(key, DATA_DELIMITER, keywords[key], DATA_DELIMITER)
        msg = msg.rstrip(DATA_DELIMITER) + MESSAGE_END_CHAR # oerwrite the last data delimiter with end char

    serialConnection.write(msg)
    if returnMsg:
        return msg

def readMessage(s):
    """ Decode an input string into a set of keywords and values
    Arguments
    ---------
        @param s - string representation of an input message
    """
    try:
        if s:
            s = s.strip()
            if s[0] == INPUT_START_CHAR and s[-1] == MESSAGE_END_CHAR:
                # split into values and keys
                s = s.lstrip(INPUT_START_CHAR).rstrip(MESSAGE_END_CHAR).split(DATA_DELIMITER)

                readings = {}
                try:
                    # concentrate into a dictionary
                    for iKey in range(0,len(s),2):
                        readings[s[iKey]] = s[iKey+1]
                except IndexError:
                    pass
                return readings
    except:
        pass
    return {}

"""
# TODO
# check what ports are currently open
ports = communicationProtocol.getActivePorts()


# TODO
if self.portOpen and self.checkConnection:
    communicationProtocol.sendMessage(self.arduinoSerialConnection,self.armModulesCommand)

    line = self.arduinoSerialConnection.readline()
        
    readings = communicationProtocol.readMessage(line)
    

# TODO
# ask the Arduino nicely to return the current readings
communicationProtocol.sendMessage(self.arduinoSerialConnection,
                                  self.sensorReadingsRequestObject)

# get the most recent line from the serial port
line = self.arduinoSerialConnection.readline()

# pass on to the parser
readings = communicationProtocol.readMessage(line)

# update sensor and other readings
for readingKey in readings:
    self.sensorParameters[readingKey] = float(readings[readingKey])
"""

# TODO the more efficient variant of input reading
"""
# if incoming bytes are waiting to be read from the serial input buffer
if (self.arduinoSerialConnection.inWaiting()>0):
    # read the bytes and convert from binary array to ASCII
    dataStr = self.arduinoSerialConnection.read(
        self.arduinoSerialConnection.inWaiting() ).decode('ascii')

    # pass to the buffer
    self.arduinoOutputBuffer += dataStr

    # keep track of all the new readings in numerical format
    newLoadCellReadings = []
    loadCellKeyBase = "loadCell"

    # extract any full lines and log them - there can be more than
    # one, depending on the loop frequencies on either side of the
    # serial conneciton
    lines = self.arduinoOutputBuffer.rpartition("\n")

    if lines[0]:
        for line in lines[0].split("\n"):

            # move to the end of the text control in case the user has clicked somewhere
            # since the last message
            self.tabConsole.logFileTextControl.MoveEnd()
            # log the line
            self.tabConsole.logFileTextControl.WriteText(line+"\n")

            # add the readings or other comms to the output file
            # if data is being acquired
            if self.acquisitionRunning:
                self.outputFileBuffer += line+"\n"

            # let the comms protocol make sense of the outputs
            readings = communicationProtocol.readMessage(line)
"""
