// 
// 
// 

#include "LightSpeed.h"

int LightSpeed::Calibrate()
{
	// Init hardware
	pinMode(LED_ONOFF_PIN, OUTPUT);		// Set transistor pin to output
	digitalWrite(LED_ONOFF_PIN, HIGH);	// Turn on LEDs

	// Step 1. LED on calibration
	int maxValue[sensorCount];
	int delayus = 1000 - ANALOGREAD_DELAY * sensorCount;

	for (int t = 0; t < calibrateTime; t++)
	{
		for (int s = 0; s < sensorCount; s++)
		{
			int read = analogRead(s);						// Read analog input
			if (read > maxValue[s]) maxValue[s] = read;		// Set max value
			int noise = abs(maxValue[s] - read);			// Calculate noise
			if (noise > sensorUNoise[s])
			{
				sensorUNoise[s] = noise;					// Set max noise
			}
			sensorUMax[s] = sensorUMax[s] + read;			// Save reading
		}
		delayMicroseconds(delayus);							// Delay
	}
	for (int s = 0; s < sensorCount; s++)
	{
		sensorUMax[s] = sensorUMax[s] / calibrateTime;		// Calculate average
	}

	// Step 2. LED off calibration

	// Step 3. set values
}
