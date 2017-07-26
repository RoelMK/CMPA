/*
 Name:		CMPA.ino
 Created:	7/14/2017 11:26:19 AM
 Author:	Roel
*/

// the setup function runs once when you press reset or power the board
#include "LDR.h"

#pragma region Objects
LDR ldr;
#pragma endregion

#pragma region Data
int LDRStatus;
#pragma endregion

void setup() {
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	// Update LDR
	LDRStatus = ldr.Update(millis());
	if (LDRStatus != LDR_SENSOR_FAILURE)
	{
		Serial.println(LDRStatus);
		Serial.println(ldr.getSpeed());


		delay(10);
	}
	else
	{
		Serial.println("[CRITICAL] LDR failure");
		delay(1000);
	}
}
