/*
 Name:		CMPA.ino
 Created:	7/14/2017 11:26:19 AM
 Author:	Roel
*/

#include "SensorStateRegister.h"
#include "FETController.h"
#include "RealFET.h"
#include "OptiCom.h"
#include "OptiLightData.h"
#include "OptiLight.h"
#include "LightSpeed.h"
#include "LDR.h"

#pragma region Constants
const unsigned long SERIAL_RATE = 38400;	// Serial communiciation bit 38400
const int DELAY = 150;						// Delay (microseconds)
const bool debug = false;					// In debug mode? (execute special loop code)
#pragma endregion

#pragma region Objects
LDR ldr;
LightSpeed lightSpeed;
OptiLight optiLight;
OptiCom optiCom;
RealFET realFET;
FETController FETControl;
SensorStateRegister SSR;
#pragma endregion

#pragma region Data
bool panic = false;			// In panic mode?
#pragma endregion


// The setup function runs once when you press reset or power the board
void setup() 
{
	Serial.begin(SERIAL_RATE);						// Init serial
	delay(1000);
	Serial.println("[INFO] Starting CMPA...");
	ldr.Init(&lightSpeed, &SSR);					// Init LDRs & LightSpeed & SSR
	optiLight.Init(&optiCom);						// Init OptiLight & OptiCom
	FETControl.Init(&optiLight, &SSR, &realFET);	// Init FET controller & RealFET
	Serial.println("[INFO] CMPA is ready");
}

// The loop function runs over and over again until power down or reset
void loop() 
{
	if (!panic && !debug)		// Do not continue if in panic mode
	{
		unsigned long startTime = micros();

		// Update sensors
		int LDRStatus = ldr.Update(millis());		// Update LDR state
		if (LDRStatus == LDR_SENSOR_FAILURE)
		{
			Panic("[CRITICAL] LDR failure");
		}

		// Update FET controller & check state
		int FETControllerState = FETControl.Update(millis());
		if(FETControllerState != FETCONTROLLER_OK)
		{
			switch (FETControllerState)
			{
			case FETCONTROLLER_OVERSPEED:
				Serial.println("[WARNING] Overspeed detected!");
				break;
			case FETCONTROLLER_REALFET_ERROR:
				Panic("[CRITICAL] RealFET error");
				break;
			default:
				Panic("[CRITICAL] Unknown error");
				break;
			}
		}

		// Update OptiLight
		// TODO

		// Delay
		unsigned deltaTime = micros() - startTime;
		if (deltaTime < DELAY)
		{
			delayMicroseconds(DELAY - deltaTime);
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
	realFET.Panic();
	panic = true;
	Serial.println(error);
}
