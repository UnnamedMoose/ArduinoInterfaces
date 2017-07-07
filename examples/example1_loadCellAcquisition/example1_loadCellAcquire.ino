/*
Record forces measured by a load cell transducer conencted to a HX711 amplifier circuit.

@author: Artur Lidtke
@email: artur.lidtke@gmail.com
@since: 2 Jul 2017
@version: 1.0.0

CHANGELOG
2 Jul 2017 - 1.0.0 - Artur Lidtke - released the first version.
*/

#include "LEDModule.h"
#include "loadCellModule.h"
#include "CommunicationInterface.h"

// Define a LED module instance connected to the pin with LED on most Uno boards.
// This will not be connected to the comms protocol and instead will be utilised
// to visually indicate whether data is being acquired or not.
LEDModule LED("builtInLED", 13);

// Define a load cell amplifier sensor.
loadCellModule loadCell("loadCell", A0, A1, 128);

// Simple on/off module that will start and stop data acquisition on demand.
Module acquisitionRunningModule("acquireData");

// Create the communication handling interface
CommunicationInterface comms;

void setup()
{
    // Start serial communications.
    Serial.begin(19200);

    // Create a list of pointers to all of the modules that are to be connected
    // to the message parsing interface.
    Module* modules[] = { &acquisitionRunningModule,
        // Each loadCellModule instance has internal modules that may be used to
        // interact with the object directly - either tare or set the calibration constant.
        // These may be called by passing >loadCellTare,1; or >loadCellCalibrate,1;
        // into the serial communications port.
        loadCell.calibrationModulePtr(), loadCell.tareModulePtr() };

    // Initialise the communication protocol.
    comms.setup(modules, sizeof(modules)/sizeof(modules[0]));

    // Set values to boolean modules (default is 0 so can omit, included here for clarity)
    acquisitionRunningModule.setValue(0);

    // Calibrate the load cells.
    // NOTE: the value used here was obtained for HX711 and a 1 kg cell and should
    // yield readings in grams. Should be rectified for each new set up.
    loadCell.calibrate(1971.f);

    // Tare the load at the beginning of the run.
    loadCell.tare();

    Serial.println("Finished set up");
}

void loop()
{
    // Read in serial inputs and pass them to connected modules, setting their values.
    comms.loop();

    // send the outputs through the serial port if required and turn on the LED
	if (acquisitionRunningModule.getValue())
    {
        LED.setValue(1);
        comms.printValue<double>(loadCell.getIdentifier(), loadCell.getReading());
    }
    else
        // Switch off the LED when not recording data.
        LED.setValue(0);

    delay(100);
}
