/*
Turns on an LED on for one second, then off for one second, when the user
sends a command.

@author: Artur Lidtke
@email: artur.lidtke@gmail.com
@since: 1 Jul 2017
@version: 1.0.0

CHANGELOG
1 Jul 2017 - 1.0.0 - Artur Lidtke - released the first version.
*/

#include "LEDModule.h"
#include "CommunicationInterface.h"

// Define a LED module instance connected to the pin with LED on most Uno boards.
LEDModule LED("builtInLED", 13);

// Create the communication handling interface
CommunicationInterface comms;

void setup()
{
    Serial.begin(19200);

    // Create a list of pointers to all of the modules that are to be connected
    // to the message parsing interface.
    Module* modules[] = { &LED };

    // Initialise the communication protocol.
    comms.setup(modules);

    Serial.println("Finished set up");
}

void loop()
{
    comms.getSerial();
    
    LED.setValue(HIGH);
    delay(1000);
    LED.setValue(LOW);
    delay(1000);
}
