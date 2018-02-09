// 
// 
// 

#include "LDR.h"

void LDR::Init(LightSpeed* lightSpeedPNT, SensorStateRegister *ssrPNT)
{
	// Set pointers
	lightSpeed = lightSpeedPNT;	
	ssr = ssrPNT;

	// Init
	ssr->Init();
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
	int sensorReadings[SensorCount];
	for (int s = 0; s < SensorCount; s++)
	{
		sensorReadings[s] = analogRead(s);
	}

	// Update LightSpeed
	bool result = lightSpeed->Update(sensorReadings, time);
	if (result)
	{
		// Update Sensor State Register
		ssr->SensorDetection = lightSpeed->GetSensor();
		ssr->LastSpeed = lightSpeed->GetSpeed();

		Serial.print("Detected: ");
		Serial.println(lightSpeed->GetSensor());
		Serial.print("Speed: ");
		Serial.println(lightSpeed->GetSpeed());

		return LDR_SENSOR_OK;
	}
	else
	{
		ssr->SensorDetection = NO_SENSOR_DETECTION;
		return LDR_SENSOR_OK;
	}
}
