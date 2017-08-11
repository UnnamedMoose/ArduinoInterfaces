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

// NOTE Debug output flag
// #define DEBUG_PRINTOUT

// Buffer size for parsing inputs.
#define DATABUFFERSIZE 180

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

    // Send a list of value with labels printed to an arbitrary precision (default 2).
    // needs an external char buffer for composing the message
    template <typename T> void printValues(const char* labels[], T values[],
        const unsigned int nValues, char msgBuffer[], const unsigned int floatPrecision = 2);

    // Send data from a buffer array assumming each row of the array should
    // go to a separate line in the serial output.
    // - to pass a 2D array with unknown number of columns need to use a trick
    //   and pass it through a void pointer which then gets cast to an appropriate
    //   type using the number of columns and rows. NOTE: on some compilers it
    //   should be possible to use "T values[][nRows]" instead, which is easier to read.
    // - msgBuffer is an externally declared char array that is meant to hold the
    //   concatenated string. It is the user's responsibility to ensure this has sufficient size.
    template <typename T> void printArray(const unsigned int nRows, const unsigned int nCols,
        const char* labels[], void *vals, char msgBuffer[], const unsigned int floatPrecision = 2);
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

// Send a list of value with labels.
template <typename T> void CommunicationInterface::printValues(const char* labels[], T values[],
    const unsigned int nValues, char msgBuffer[], const unsigned int floatPrecision)
{
    char num[10];

    strcat(msgBuffer, String(OUTPUT_START_CHAR).c_str());

    // add each value
    for (unsigned int j = 0; j < nValues; j++)
    {
        strcat(msgBuffer, labels[j]);
        strcat(msgBuffer, String(DATA_DELIMITER).c_str());
        dtostrf(values[j], 0, floatPrecision, num);
        strcat(msgBuffer, num);

        // add a delimiter or an EOL
        (j<(nValues-1)) ? strcat(msgBuffer, ",") : strcat(msgBuffer, String(END_CHAR).c_str());
    }

    Serial.println(msgBuffer);
    msgBuffer[0] = '\0';
}

// Send data from a buffer array assumming each row of the array should
// go to a separate line in the serial output.
template <typename T> void CommunicationInterface::printArray(
    const unsigned int nRows, const unsigned int nCols,
    const char* labels[], void *vals, char msgBuffer[], const unsigned int floatPrecision)
{
    // cast the void pointer to an array type
    T (*values)[nCols] = static_cast<T (*)[nCols]>(vals);

    // temporary buffer for converting numbers to strings
    char num[10];

    for (size_t i = 0; i < nRows; i++)
    {
        // start a new line of the message
        strcat(msgBuffer, String(OUTPUT_START_CHAR).c_str());

        // add each reading
        for (size_t j = 0; j < nCols; j++)
        {
            strcat(msgBuffer, labels[j]);
            strcat(msgBuffer, String(DATA_DELIMITER).c_str());

            // NOTE: sprintf on a standard Arduino only works with integers,
            // if want to do this with a float, need to use dtostrf
            // use minimum width of 0 characters and a desired floating point precision
            dtostrf(values[i][j], 0, floatPrecision, num);
            // sprintf(num, "%d", values[i][j]); // this may be used for integers only
            strcat(msgBuffer, num);

            // add a delimiter or an EOL
            (j<(nCols-1)) ? strcat(msgBuffer, ",") : strcat(msgBuffer, String(END_CHAR).c_str());
        }
        strcat(msgBuffer, "\n");
    }
    // terminate the message and send to the serial port
    strcat(msgBuffer, "\0");
    Serial.print(msgBuffer);

    // set the first element of the buffer as termination character to make sure
    // it may be re-used for something else
    msgBuffer[0] = '\0';
}

#endif
