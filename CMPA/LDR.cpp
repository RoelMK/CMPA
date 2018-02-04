// 
// 
// 

#include "LDR.h"

void LDR::Init(LightSpeed* lightSpeedPNT)
{
	lightSpeed = lightSpeedPNT;
	Serial.println("[INFO] Calibrating...");
	int returnCode = lightSpeed->Start();		// Start LightSpeed
	if (returnCode != CALIBRATION_OK)			// If any error: wait for input
	{
		Serial.println("");
		Serial.println(">> LDR calibration failure detected, waiting for serial input...");
		while (Serial.available() == 0) {}
		Serial.readString();
	}
	Serial.println("...ready!");
}

int LDR::Update(unsigned long time)
{
	// Read sensor data
	int sensorReadings[sensorCount];
	for (int s = 0; s < sensorCount; s++)
	{
		sensorReadings[s] = analogRead(s);
	}

	// Update LightSpeed
	bool result = lightSpeed->Update(sensorReadings, time);
	if (result)
	{
		//Serial.print("Detected: ");
		//Serial.println(lightSpeed->getFET());
		//Serial.print("Speed: ");
		//Serial.println(lightSpeed->getSpeed());
		return lightSpeed->getFET();
	}
	else
	{
		return LDR_SENSOR_OK;
	}
}

double LDR::getSpeed()
{
	return lightSpeed->getSpeed();
}

