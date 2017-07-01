#include "rotaryEncoder.h"

rotaryEncoder::rotaryEncoder(const char* sensorID, int pinA, int pinB, int pinI, int noSteps)
:
	Module(sensorID),
	pinChannelA_(pinA),
	pinChannelB_(pinB),
	pinChannelI_(pinI),
	noSteps_(noSteps),
	lastValueChannelA_(LOW),
	valueChanged_(false)
{
	pinMode (pinChannelA_,INPUT);
	pinMode (pinChannelB_,INPUT);
	pinMode (pinChannelI_,INPUT);
};

rotaryEncoder::rotaryEncoder(void) : noSteps_(1) // initialise the const member
{
	pinMode (pinChannelA_,INPUT);
	pinMode (pinChannelB_,INPUT);
	pinMode (pinChannelI_,INPUT);
};

rotaryEncoder::~rotaryEncoder(void) {};

void rotaryEncoder::loop()
{
	/* This method should get called on each pass of the loop() in the main Arduino
	code. It read the current state of the encoder channels, compares it the the old
	state, and based on that determines the direction of rotation, if any.
	*/
	int pinChannelAnew = digitalRead(pinChannelA_);

	if ((lastValueChannelA_ == LOW) && (pinChannelAnew == HIGH))
	{
		(digitalRead(pinChannelB_) == LOW) ? currentValue_-- : currentValue_++;
		valueChanged_ = true;
		// Serial.println(this->getValue());
	}
	else
		valueChanged_ = false;

	lastValueChannelA_ = pinChannelAnew;
}
