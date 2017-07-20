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

void setup() {
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	ldr.Update(millis());
}
