/*
 Name:		CMPA.ino
 Created:	7/14/2017 11:26:19 AM
 Author:	Roel
*/

#include "MOSFET.h"
#include "LDR.h"

#define SERIAL_RATE 115200	// Serial communiciation bit
#define DELAY 150			// Delay (microseconds)
const bool debug = false;	// In debug mode? (execute special loop code)


#pragma region Objects
LDR ldr;
MOSFET fet;
#pragma endregion

#pragma region Data
bool panic = false;			// In panic mode?
#pragma endregion


// The setup function runs once when you press reset or power the board
void setup() 
{
	Serial.begin(SERIAL_RATE);		// Init serial
	Serial.println("Starting CMPA...");
	ldr.Init();				// Init LDRs
	fet.Init();				// Init FETs
}

// The loop function runs over and over again until power down or reset
void loop() 
{
	if (!panic && !debug)		// Do not continue if in panic mode
	{
		int LDRStatus = ldr.Update(millis());		// Update LDR state
		//Serial.print("  >> Status: ");
		//Serial.println(LDRStatus);
		//Serial.print("  >> Speed: ");
		//Serial.println(ldr.getSpeedV2());
		if (LDRStatus != LDR_SENSOR_FAILURE)
		{
			int FETStatus = fet.Update(LDRStatus, millis(), ldr.getSpeed());	// Update FETs
			if (FETStatus != FET_OK)
			{
				switch (FETStatus)
				{
				case CRITICAL_FET_POWER_ERROR:
					Panic("[CRITICAL] FET power failure");
					break;
				case CRITICAL_FET_SELECTION_ERROR:
					Panic("[CRITICAL] FET selection failure");
					break;
				default:
					Panic("[CRITICAL] FET unknown failure");
					break;
				}
			}
			else
			{
				delayMicroseconds(DELAY);
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
		int LDRStatus = ldr.Update(millis());		// Update LDR state
		//Serial.print("  >> Status: ");
		//Serial.println(LDRStatus);
		//Serial.print("  >> Speed: ");
		//Serial.println(ldr.getSpeed());
		delayMicroseconds(DELAY);
		//delay(1000);
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
