// LightSpeed.h

#ifndef _LIGHTSPEED_h
#define _LIGHTSPEED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Constants: calibration
const int LED_ONOFF_PIN = 21;			// Digital pin of LED transistor
const int LED_ONOFF_TIME = 10;			// Wait for the LEDs to turn on/off	(in ms)
const int ANALOGREAD_DELAY = 150;		// How long it takes to run analogRead (in us)	
const int calibrateTime = 1000;			// Time to wait to detect noise (in ms)
const int calibrationResistorValue = 1000; // Resistance of resistor in parallel with LED transistor (in Ohm)

const int CALIBRATION_OK = 1;					// Return: calibration OK
const int FAILED_NOISE_LOWDELTAMAXMIN = -1;	// Return: too much background light
const int minimalDeltaMaxMin = 50;
const int FAILED_NOISE_LOWDELTANOISEDETECT = -2;	// Return: too much noise
const int minimalDeltaNoiseDetect = 25;
const int FAILED_LDRDELTA_HIGH = -3;				// Return: difference between two calibration readings is too high
const int calibrateDifferenceWarningThreshold = 150;

const int defaultNoise = 30;
const int maxDifferenceBetweenLightAndDark = 250;
#pragma endregion

const int sensorCount = 16;				// Begin analog A0 (1=A0, 2=A0,A1)
const int noDetection = -1;				// lastSensorDetectedProjectile default value
const double LDRlength = 0.015;			// In meters, short-axis (source: https://www.vanallesenmeer.nl/LDR-5528)

class LightSpeed
{
public:
	int Start();								// Start LightSpeed
	bool Update(int readings[sensorCount], long time);		// Update LightSpeed

	double getSpeed()
	{
		return this->speed;
	}

	int getFET()
	{
		return this->fetON;
	}

private:
	int sensorUMax[sensorCount];				// Sensor highest analog reading
	int sensorUMin[sensorCount];				// Sensor lowest analog reading
	int sensorUNoise[sensorCount];				// Sensor highest noise reading
	int sensorNoiseThreshold[sensorCount];		// Reading below this value = normally no noise
	int sensorDetectionThreshold[sensorCount];	// Reading below this value = projectile detection

	int lastSensorDetectedProjectile;			// Last sensor which has detected a projectile
	long timeBelowNoiseThreshold;				// Time sensor is below noise threshold
	long lastTime;								// Last time
	bool isReliable;							// Has the sensor been below the detection threshold?
	int detectingSensor;						// Sensor which is detecting a projectile

	int ConvertAnalogReadLEDFETClosedToAnalogReadLEDFETOpen(int analogData);	// Convert result from AnalogRead() with LED FET closed to 'real' analog value (with LED FET open)
	void NoiseCalibration(int sensor, int correction);			// Calibrate sensor
	bool UpdateDetectingSensor(int sensor, int reading, long time);		// Update sensor data
	
	void CalculateFETData();					// Update speed & FET
	double speed;								// Speed of projectile
	int fetON;								// FET to turn on
};


#endif

