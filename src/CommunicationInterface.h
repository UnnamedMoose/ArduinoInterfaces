/*
Provides functions that automatically parse inputs given to the Arduino over the
serial port and assigns actions to defined sensor and actuator instances.

@author: Artur Lidtke
@email: artur.lidtke@gmail.com
@since: 1 Jul 2017
@version: 1.0.0

CHANGELOG
1 Jul 2017 - 1.0.0 - Artur Lidtke - released the first version.
*/

#ifndef COMMUNICATION_INTERFACE_H
#define COMMUNICATION_INTERFACE_H

#include "Arduino.h" // Basic Arduino stuff.
#include "Module.h"

// Buffer size for parsing inputs.
#define DATABUFFERSIZE 180

// Debug output flag
#define DEBUG_PRINTOUT

class CommunicationInterface
{
private:
    // Where the received command will be temporarily held. Add 1 for NULL terminator at the end.
    char inputDataBuffer_[DATABUFFERSIZE+1];

    // We'll store a message here before sending it.
    String outputDataBuffer_;

    // Data chunk delimiters - used to split commands and values that follow them.
    static const char INPUT_START_CHAR = '>';  // At the start of every command message sent TO the Arduino.
    static const char OUTPUT_START_CHAR = '<'; // At the start of every command message sent FROM the Arduino.
    static const char END_CHAR = ';'; 		   // End of the command message sent to and from the Arduino.
    static const char DATA_DELIMITER = ',';     // Splits the command name and value.

    // Signals whether to write data to the buffer or not (yes if we're receiving an input).
    bool placeInBuffer_;

    // Current position in the input buffer.
	size_t dataBufferIndex_;

    // Array which holds pointers to all linked modules
    Module** modules_;

    // Counter of elements inside the modules_ array.
    size_t nModules_;

public:
    // Default constructor
    CommunicationInterface(void);

    // Destructor which cleans up pointers
    ~CommunicationInterface(void);

    // ---
    // SET UP AND EXECUTION

    // Initialises the communications and stores pointers to all linked modules
    // inside the current instance.
    // @param modulePtrs - list of pointers to objects of type or derived from Module
    // @para nPtrs - size of the array
    void setup(Module* modulePtrs[], size_t nPtrs);

    // Main funciton which calles getSerial() and parseInput() sequentially to
    // update the state of all of the connected modules.
    void loop(void);

    // ---
    // INPUT HANDLING

    // Read input from the serial port and store it in the inputDataBuffer buffer.
    boolean getSerial(void);

    // Extract commands and corresponding numbers from the inputDataBuffer input buffer.
    void parseInput(void);

    // ---
    // OUTPUT HANDLING

    // Send a single value with a label over the serial port.
    template <typename T> void printValue(const char* label, T value);
};

// TEMPLATE FUNCTION DEFINITIONS

// Send a single value with a label over the serial port.
template <typename T> void CommunicationInterface::printValue(const char* label, T value)
{
    Serial.print(OUTPUT_START_CHAR);
    Serial.print(label);
    Serial.print(DATA_DELIMITER);
    Serial.print(value);
    Serial.println(END_CHAR);
}


#endif
