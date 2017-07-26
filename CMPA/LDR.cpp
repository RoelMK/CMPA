// 
// 
// 

#include "LDR.h"

int LDR::Update(unsigned long time)
{
	bool sensorDetected = false;

	// Read analog data
	for (int sensor = 0; sensor < sensorCount; sensor++)
	{
		int data = analogRead(sensor);		// Read analog value		
		Serial.println(data);
		if (data < detectObjectThreshold)	// Is light dimmed?
		{
			if (!sensorDetected)
			{
				sensorDetected = true;
				sensorObjectDetection[sensor] = true;	// Yes: object detected
				lastSensorDetected = sensor;			// Set last sensor detection
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
	if (sensorDetected)
	{
		Calc(true, time);
		return lastSensorDetected;
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
		timeRequiredToReachNextSensor = time - lastSensorDetectionTime;		// Calculate time passed
		lastSensorDetectionTime = time;										// Set detection time
		speed = timeRequiredToReachNextSensor / distanceBetweenSensors;		// Calculate speed
		distanceTraveled = 0;												// Reset distance
		timeRequiredToReachNextCoil = distanceBetweenSensorAndNextCoil / speed;		// Reset time
	}
	else
	{
		distanceTraveled = speed * timePassed;	// Calculate distance traveled
		timeRequiredToReachNextCoil = (distanceBetweenSensorAndNextCoil - distanceTraveled) / speed;
	}
}

void LDR::Reset()
{
	previousTime = 0;
	timeRequiredToReachNextSensor = 0;
	timeRequiredToReachNextCoil = 0;
	distanceTraveled = 0;
	lastSensorDetectionTime = 0;
	lastSensorDetected = 0;
	speed = 0;
}