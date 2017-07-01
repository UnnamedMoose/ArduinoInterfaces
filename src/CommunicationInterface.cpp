#include "CommunicationInterface.h"

// constructor
CommunicationInterface::CommunicationInterface(void)
:
    outputDataBuffer_(""),
    placeInBuffer_(false),
	dataBufferIndex_(0)
{
    // Do nothing.
}

// Destructor - make sure to clean up those pointers!
CommunicationInterface::~CommunicationInterface(void)
{
    for(long int moduleI=0; moduleI<sizeof(modules_)/sizeof(modules_[0]); moduleI++)
        delete modules_[moduleI];
}

// Initialises the communications and stores pointers to all linked modules
// inside the current instance.
// @param modulePtrs - list of pointers to objects of type or derived from Module
void CommunicationInterface::setup(Module* modulePtrs[])
{
    // Keep these pointers.
    modules_ = modulePtrs;

    Serial.println("Initialised communications with modules:");
    for(long int moduleI=0; moduleI<sizeof(modules_)/sizeof(modules_[0]); moduleI++)
    {
        Serial.print("    ");
        Serial.print(moduleI);
        Serial.print(": ");
        Serial.println(modules_[moduleI]->getIdentifier());
    }
}

boolean CommunicationInterface::getSerial(void)
/* Read input from the serial port and store it in the inputDataBuffer buffer.
 * The serial input has to conform to a protocol, whereby the messages begin with
 * INPUT_START_CHAR and terminate with END_CHAR. The message in between is
 * a sequence of strings and integers. Strings represent the destination of the
 * command (e.g. engine throttle) and integers are the arguments for the command
 * (e.g. throttle value).
 *
 * @return - true if we've parsed the entire command successfully; false otherwise,
 * 			e.g. when we didn't receive a command.
 */
{
	char incomingbyte; // The currently read serial input byte.

	while(Serial.available()>0)
	{
		incomingbyte = Serial.read(); // Read bytes from the serial input one by one.

		if(incomingbyte==INPUT_START_CHAR) // A command has been received.
		{
			dataBufferIndex_ = 0; // We'll be reading the first input value next.
			placeInBuffer_ = true; // We should be storing the inputs in the buffer now.
		}

		if(placeInBuffer_)
		{
			if(dataBufferIndex_==DATABUFFERSIZE)
			{
				// Index is pointing to an array element outside our buffer.
                // We're done reading this command, so have to start reading the next one from the beginning.
				dataBufferIndex_ = 0;
                // Exit the while loop, we're done with this command.
				break;
			}

			if(incomingbyte==END_CHAR) // We've reached the end of this command.
			{
                // Null terminate the C string
				inputDataBuffer_[dataBufferIndex_] = 0;
				// Don't store anything here any more.
                placeInBuffer_ = false;
				// Make sure next command gets written to the start of the buffer.
                dataBufferIndex_ = 0;
				// Say that we've parsed the whole command.
                return true;
			}
			else if (incomingbyte!=INPUT_START_CHAR) // Simply record the telecommand byte.
			{
				inputDataBuffer_[dataBufferIndex_++] = incomingbyte;
			}
		}
	}

    Serial.println("Current buffer:");
    Serial.println(inputDataBuffer_);
    Serial.println("");
    
	return false; // Something went wrong or we didn't receive an actual command.
}
