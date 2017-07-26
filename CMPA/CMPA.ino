/*
 Name:		CMPA.ino
 Created:	7/14/2017 11:26:19 AM
 Author:	Roel
*/

#include "MOSFET.h"
#include "LDR.h"

#pragma region Objects
LDR ldr;
MOSFET fet;
#pragma endregion

#pragma region Data
bool panic = false;			// In panic mode?
#pragma endregion

const bool debug = true;	// In debug mode? (execute special loop code)

// The setup function runs once when you press reset or power the board
void setup() 
{
	Serial.begin(9600);		// Init serial
	//fet.Init();				// Init FETs
	pinMode(3, OUTPUT);
}

// The loop function runs over and over again until power down or reset
void loop() 
{
	if (!panic && !debug)		// Do not continue if in panic mode
	{
		int LDRStatus = ldr.Update(millis());		// Update LDR state
		if (LDRStatus != LDR_SENSOR_FAILURE)
		{
			//Serial.println(LDRStatus);
			//Serial.println(ldr.getSpeed());
			int FETStatus = fet.Update(LDRStatus, millis(), ldr.getSpeed());	// Update FETs
			if (FETStatus != FET_OK)
			{
				Panic("[CRITICAL] FET failure");
			}
			else
			{
				delay(10);
			}
		}
		else
		{
			Panic("[CRITICAL] LDR failure");
		}
	}
	else if(debug)
	{
		// Debug code here
		digitalWrite(3, HIGH);
		Serial.println("HIGH");
		delay(1000);
		digitalWrite(3, LOW);
		Serial.println("LOW");
		delay(1000);
	}
	else
	{
		delay(1000);
	}
}

// Critical error occured: go into panic mode and do not continue executing code
void Panic(char *error)
{
	fet.Panic();
	panic = true;
	Serial.println(error);
}
