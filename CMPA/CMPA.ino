/*
 Name:		CMPA.ino
 Created:	7/14/2017 11:26:19 AM
 Author:	Roel
*/

#include "OptiCom.h"
#include "OptiLightData.h"
#include "OptiLight.h"
#include "SpeedToTime.h"
#include "LightSpeed.h"
#include "MOSFET.h"
#include "LDR.h"

#pragma region Constants
const unsigned long SERIAL_RATE = 38400;	// Serial communiciation bit 115200
const int DELAY = 150;		// Delay (microseconds)
const bool debug = false;	// In debug mode? (execute special loop code)
#pragma endregion

#pragma region Objects
LDR ldr;
MOSFET fet;
LightSpeed lightSpeed;
SpeedToTime speedTime;
OptiLight optiLight;
OptiCom optiCom;
#pragma endregion

#pragma region Data
bool panic = false;			// In panic mode?
#pragma endregion


// The setup function runs once when you press reset or power the board
void setup() 
{
	Serial.begin(SERIAL_RATE);		// Init serial
	delay(1000);
	Serial.println("[INFO] Starting CMPA...");
	ldr.Init(&lightSpeed);			// Init LDRs
	fet.Init(&speedTime);			// Init FETs
	//optiLight.Init(&lightSpeed, &optiCom);	// Init OptiLight
	Serial.println("[INFO] CMPA is ready");
}

// The loop function runs over and over again until power down or reset
void loop() 
{
	if (!panic && !debug)		// Do not continue if in panic mode
	{
		int LDRStatus = ldr.Update(millis());		// Update LDR state
		if (LDRStatus != LDR_SENSOR_FAILURE)
		{
			//optiLight.Update(LDRStatus);
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
