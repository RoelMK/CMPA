// 
// 
// 

#include "LightSpeed.h"

int LightSpeed::Start()
{
	int returnCode = CALIBRATION_OK;
	// Init hardware
	pinMode(LED_ONOFF_PIN, OUTPUT);							// Set transistor pin to output
	digitalWrite(LED_ONOFF_PIN, HIGH);						// Turn on LEDs
	delay(LED_ONOFF_TIME);
	int delayus = ANALOGREAD_DELAY * SensorCount;			// Calculate delay
	lastSensorDetectedProjectile = NO_SENSOR_DETECTION;		// Set lastSensorDetectedProjectile to noDetection
	long readings[SensorCount];								// Init readings array
	for (int s = 0; s < SensorCount; s++)
	{
		readings[s] = 0;
	}

	// Step 1. LED on calibration
	Serial.println(">> LED ON");
	int maxValue[SensorCount];

	for (int t = 0; t < calibrationReadings; t++)
	{
		for (int s = 0; s < SensorCount; s++)
		{
			int read = analogRead(s);						// Read analog input
			if (maxValue[s] >= 1024)						// Check if data is usable
			{
				maxValue[s] = 0;
				Serial.print("[Info] Received corrupt analog data, ignoring reading (LDR");
				Serial.print(s);
				Serial.println(")");
			}
			else
			{
				if (read > maxValue[s]) maxValue[s] = read;		// Set max value
				int noise = abs(maxValue[s] - read);			// Calculate noise
				if (noise > sensorUNoise[s])
				{
					if (noise > maximalNoise) noise = maximalNoise;
					sensorUNoise[s] = noise;					// Set max noise
				}
				readings[s] = readings[s] + read;				// Save reading
			}
		}
		delayMicroseconds(delayus);							// Delay
	}
	for (int s = 0; s < SensorCount; s++)
	{
		sensorUMax[s] = readings[s] / calibrationReadings;		// Calculate average
		readings[s] = 0;

		if (s > 0)
		{
			if (abs(sensorUMax[s] - sensorUMax[s - 1]) > calibrateDifferenceWarningThreshold)
			{
				returnCode = FAILED_LDRDELTA_HIGH;
				Serial.print("[WARNING] LDR failure detected in LDR");
				Serial.println(s);
			}
		}
		
		Serial.print("LDR");
		Serial.print(s);
		Serial.print(" calibrated UMax value: ");
		Serial.println(sensorUMax[s]);
	}


	// Step 2. LED off calibration
	Serial.println(">> LED OFF");
	digitalWrite(LED_ONOFF_PIN, LOW);		// Turn off LEDs
	delay(LED_ONOFF_TIME);

	for (int t = 0; t < calibrationReadings; t++)
	{
		for (int s = 0; s < SensorCount; s++)
		{
			int read = ConvertAnalogReadLEDFETClosedToAnalogReadLEDFETOpen(analogRead(s));						// Read analog input (and convert)
			readings[s] = readings[s] + read;				// Save reading
		}
		delayMicroseconds(delayus);							// Delay
	}
	for (int s = 0; s < SensorCount; s++)
	{
		sensorUMin[s] = readings[s] / calibrationReadings;		// Calculate average
		readings[s] = 0;
		Serial.print("LDR");
		Serial.print(s);
		Serial.print(" calibrated UMin value: ");
		Serial.println(sensorUMin[s]);
	}


	// Step 3. set & check values
	Serial.println(">> Calculating noise and detection thresholds...");
	digitalWrite(LED_ONOFF_PIN, HIGH);		// Turn on LEDs
	delay(LED_ONOFF_TIME);


	for (int s = 0; s < SensorCount; s++)
	{
		if (sensorUMax[s] - sensorUMin[s] < minimalDeltaMaxMin)
		{
			returnCode = FAILED_NOISE_LOWDELTAMAXMIN;
			Serial.print("[WARNING] Difference between max and min is low for LDR");
			Serial.println(s);
		}

		int deltaSensor = sensorUMax[s] - sensorUMin[s];
		if (deltaSensor > maxDifferenceBetweenLightAndDark)
		{
			deltaSensor = maxDifferenceBetweenLightAndDark;
		}
		
		int maxNoise = deltaSensor * maximalPartOfDeltaSensorIsNoise;
		if (defaultNoise > maxNoise)
		{
			sensorUNoise[s] = maxNoise;
		}
		else
		{
			sensorUNoise[s] = sensorUNoise[s] + defaultNoise;
			if (sensorUNoise[s] > maxNoise)
			{
				sensorUNoise[s] = maxNoise;
			}
		}

		sensorDetectionThreshold[s] = sensorUMax[s] - ((deltaSensor) / 2);						// Calculate detection threshold
		sensorNoiseThreshold[s] = sensorUMax[s] - sensorUNoise[s];								// Calculate noise threshold
		
		Serial.print("LDR");
		Serial.print(s);
		Serial.print(" calibrated noise and detection thresholds: ");
		Serial.print(sensorNoiseThreshold[s]);
		Serial.print(" - ");
		Serial.println(sensorDetectionThreshold[s]);

		if (sensorNoiseThreshold[s] - sensorDetectionThreshold[s] < minimalDeltaNoiseDetect)
		{
			returnCode = FAILED_NOISE_LOWDELTANOISEDETECT;
			Serial.print("[WARNING] Difference between noise and detection is low for LDR");
			Serial.println(s);
		}
	}

	// Return
	return returnCode;
}

int LightSpeed::ConvertAnalogReadLEDFETClosedToAnalogReadLEDFETOpen(int analogData)
{
	double voltage = analogData * (5.0 / 1023.0);		// Convert analogRead to voltage
	double resistanceLDR = R2VoltageDividerResistance * (5.0 / voltage - 1) - calibrationResistorValue;		// Calculate resistance of LDR
	if (resistanceLDR < minimalResistanceLDR)
	{
		Serial.print("[WARNING] LDR low resistance: ");
		Serial.print(resistanceLDR);
		Serial.print(" for analogRead ");
		Serial.println(analogData);
		return analogData;
	}
	return (int) (R2VoltageDividerResistance / (resistanceLDR + R2VoltageDividerResistance) * 1023);		// Return 'real' analogRead (without extra resistance in serie with LDR)
}


bool LightSpeed::Update(int readings[SensorCount], long time)
{
	// Step 1. below noise threshold?
	int belowNoise = NO_SENSOR_DETECTION;

	for (int s = 0; s < SensorCount; s++)
	{
		if (readings[s] < sensorNoiseThreshold[s])
		{
			if (belowNoise != NO_SENSOR_DETECTION)			// Check if any sensors has been below the noise threshold
			{
				int nextSensor = lastSensorDetectedProjectile + 1;	// Get next sensor
				if (nextSensor >= SensorCount) nextSensor = 0;

				if (s == nextSensor)
				{
					NoiseCalibration(belowNoise, abs(sensorNoiseThreshold[belowNoise] - readings[belowNoise]));	// s is right sensor, calibrate the wrong sensor (is in belowNoise)
					belowNoise = s;					// Update belowNoise
				}
				else
				{
					NoiseCalibration(s, abs(sensorNoiseThreshold[s] - readings[s]));			// belowNoise is the right sensor, calibrate the wrong sensor (is in s)
				}
			}
			else
			{
				belowNoise = s;		// Update belowNoise with the number of sensor which has been below the noise threshold
			}
		}
	}

	// Step 2. Update algorithm data
	int reading = 2000;
	if (belowNoise != NO_SENSOR_DETECTION)
	{
		reading = readings[belowNoise];
	}

	bool trigger = UpdateDetectingSensor(belowNoise, reading, time);

	// Step 3. Trigger FETs
	if (trigger)
	{	
		return true;
	}
	else
	{
		return false;
	}
}

bool LightSpeed::UpdateDetectingSensor(int sensor, int reading, long time)
{
	bool trigger = false;

	if (sensor != NO_SENSOR_DETECTION)
	{
		if (sensor == detectingSensor)
		{
			timeBelowNoiseThreshold += (time - lastTime);	// Update time
			if (reading < sensorDetectionThreshold[sensor])	// Update reliability check
			{
				isReliable = true;
			}
		}
		else
		{
			detectingSensor = sensor;		// Set detecting sensor
			timeBelowNoiseThreshold = 0;	// Reset time
			isReliable = false;				// Reset reliability check 
		}
	}
	else
	{
		if (detectingSensor != NO_SENSOR_DETECTION)
		{
			if (isReliable)		// If reliable: trigger FETs and update speed etc.
			{
				trigger = true;										// Set trigger return
				CalculateFETData();									// Update speed, timing, etc.
				isReliable = false;									// Reset
				lastSensorDetectedProjectile = detectingSensor;		// Set last sensor
			}
			
			timeBelowNoiseThreshold = 0;		// Reset time
			detectingSensor = NO_SENSOR_DETECTION;		// Reset detecting sensor
		}
		else
		{
			// All OK, no detections
		}
	}
	lastTime = time;
	return trigger;
}


void LightSpeed::CalculateFETData()
{
	sensorDetection = detectingSensor;
	speed = LDRlength / ((double)timeBelowNoiseThreshold / 1000);
}

void LightSpeed::NoiseCalibration(int sensor, int correction)
{
	if (correction >= 1024)
	{
		return;
	}

	int newThreshold = sensorNoiseThreshold[sensor] - correction;
	if (newThreshold < sensorDetectionThreshold[sensor])
	{
		Serial.print("[WARNING] Noise calibration failed for LDR");
		Serial.println(sensor);
	}
	else
	{
		Serial.print("[INFO] Calibrated LDR");
		Serial.print(sensor);
		Serial.print(", corrected: ");
		Serial.println(correction);
		sensorNoiseThreshold[sensor] = newThreshold;
	}
}
