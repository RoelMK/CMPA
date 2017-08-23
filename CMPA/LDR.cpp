// 
// 
// 

#include "LDR.h"

void LDR::Init()
{
	Serial.println("Calibrating LDRs...");
	bool failure = false;
	for (int sensor = 0; sensor < sensorCount; sensor++)
	{
		int data = analogRead(sensor);						// Read value
		sensorObjectDetectionThreshold[sensor] = data - differenceBetweenLightAndDarkForLDR;		// Set value
		if (sensor > 0)
		{
			if (abs(sensorObjectDetectionThreshold[sensor] - sensorObjectDetectionThreshold[sensor - 1]) > calibrateDifferenceWarningThreshold)
			{
				failure = true;
			}
		}

		// Print value
		Serial.print("LDR");
		Serial.print(sensor);
		Serial.print(" calibrated value: ");
		Serial.println(data);
	}

	if (failure)
	{
		Serial.println("");
		Serial.println("WARNING: LDR calibration failure detected, waiting for serial input...");
		while (!Serial.available()) {}
	}

	Serial.println("...ready!");
}

int LDR::Update(unsigned long time)
{
	bool detectedProjectile = false;

	// Read analog data
	for (int sensor = 0; sensor < sensorCount; sensor++)
	{
		int data = analogRead(sensor);		// Read analog value	
		
		if (data < sensorObjectDetectionThreshold[sensor])	// Is light dimmed?
		{
			//Serial.println(sensorObjectDetectionThreshold[sensor] - data);
			if (!detectedProjectile)
			{
				detectedProjectile = true;
				sensorObjectDetection[sensor] = true;	// Yes: object detected
				sensorDetected = sensor;			// Set last sensor detection
				
			}
			else
			{
				return LDR_SENSOR_FAILURE;			// Multiple sensors detect projectile
			}
		}
		else
		{
			sensorObjectDetection[sensor] = false;	// No: object not detected
		}
	}

	// Calculate & return
	if (detectedProjectile)
	{
		Calc(true, time);
		return sensorDetected;
	}
	else
	{
		Calc(false, time);
		return LDR_SENSOR_OK;
	}
}

void LDR::Calc(bool detectedProjectile, unsigned long time)
{
	// Timing
	unsigned long timePassed = time - previousTime;
	previousTime = time;

	// Calculate
	if (detectedProjectile)
	{
		timeSensorOn += timePassed;

		if (sensorDetected != previousSensorDetected)
		{
			Serial.print("DETECTED: ");
			Serial.println(sensorDetected);

			timeRequiredToReachNextSensor = time - lastSensorDetectionTime;		// Calculate time passed
			lastSensorDetectionTime = time;										// Set detection time
			inRuntimeCalibration(previousSensorDetected);		// Calibrate
			double timeRequiredToReachNextSensorSeconds = timeRequiredToReachNextSensor / 1000.0;
			speedV1 = distanceBetweenSensors / timeRequiredToReachNextSensorSeconds;		// Calculate speed
		}
		
		previousSensorDetected = sensorDetected;
	}
	else
	{
		if (timeSensorOn != 0)					// Calculate high accuracy speed
		{
			//speedV2 = LDRlength / ((double)timeSensorOn / 1000);
			speedV2 = 0.12;
			timeSensorOn = 0;
		}
	}
}

void LDR::inRuntimeCalibration(int sensor)
{
	int data = analogRead(sensor);						// Read value
	sensorObjectDetectionThreshold[sensor] = data - differenceBetweenLightAndDarkForLDR;		// Set value	
	Serial.print("IRT_calibrate: ");
	Serial.print(sensor);
	Serial.print("|");
	Serial.println(sensorObjectDetectionThreshold[sensor]);
}

void LDR::Reset()
{
	previousTime = 0;
	timeRequiredToReachNextSensor = 0;
	lastSensorDetectionTime = 0;
	sensorDetected = 0;
	speedV1 = 0;
	timeSensorOn = 0;
	speedV2 = 0;
}