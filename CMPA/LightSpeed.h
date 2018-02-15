// LightSpeed.h

#ifndef _LIGHTSPEED_h
#define _LIGHTSPEED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"

class LightSpeed
{
public:
	int Start();								// Start LightSpeed
	bool Update(int readings[SensorCount], long time);		// Update LightSpeed

	double GetSpeed()
	{
		return this->speed;
	}

	int GetSensor()
	{
		return this->sensorDetection;
	}

private:
	// Sensor calibration data
	int sensorUMax[SensorCount];				// Sensor highest analog reading
	int sensorUMin[SensorCount];				// Sensor lowest analog reading
	int sensorUNoise[SensorCount];				// Sensor highest noise reading
	int sensorNoiseThreshold[SensorCount];		// Reading below this value = normally no noise
	int sensorDetectionThreshold[SensorCount];	// Reading below this value = projectile detection

	// Detection data
	int lastSensorDetectedProjectile;			// Last sensor which has detected a projectile
	long timeBelowNoiseThreshold;				// Time sensor is below noise threshold
	long lastTime;								// Last time
	bool isReliable;							// Has the sensor been below the detection threshold?
	int detectingSensor;						// Sensor which is detecting a projectile

	// Functions
	int ConvertAnalogReadLEDFETClosedToAnalogReadLEDFETOpen(int analogData);	// Convert result from AnalogRead() with LED FET closed to 'real' analog value (with LED FET open)
	void NoiseCalibration(int sensor, int correction);			// Calibrate sensor
	bool UpdateDetectingSensor(int sensor, int reading, long time);		// Update sensor data
	
	// External data
	void CalculateFETData();					// Update speed & FET
	double speed;								// Speed of projectile
	int sensorDetection;						// FET to turn on

	// Length of LDR
	const double LDRlength = 0.016;				// In meters, short-axis (source: https://www.vanallesenmeer.nl/LDR-5528)

	// Calibration constants
	const int LED_ONOFF_PIN = 18;						// Digital pin of LED transistor
	const int LED_ONOFF_TIME = 100;						// Wait for the LEDs to turn on/off	(in ms)
	const int ANALOGREAD_DELAY = 150;					// How long it takes to run analogRead (in us)	
	const int calibrationReadings = 500;				// Number of calibration readings
	const int calibrationResistorValue = 1120;			// Resistance of resistor in parallel with LED transistor (in Ohm)
	const int R2VoltageDividerResistance = 10000;		// Resistance of second resistor in voltage divider (in Ohm)

	const int FAILED_NOISE_LOWDELTAMAXMIN = -1;				// Return: too much background light
	const int minimalDeltaMaxMin = 50;						// If difference between UMax and UMin is less than this value, CMPA will show a warning
	const int FAILED_NOISE_LOWDELTANOISEDETECT = -2;		// Return: too much noise
	const int minimalDeltaNoiseDetect = 25;					// If difference between noise threshold and detection threshold is lower than this value, CMPA will show a warning
	const int FAILED_LDRDELTA_HIGH = -3;					// Return: difference between two calibration readings is too high
	const int calibrateDifferenceWarningThreshold = 150;	// If difference between two calibration readings is higher than this value, CMPA will show a warning
	const int minimalResistanceLDR = 2500;					// Minimal resistance which a LDR should have (in Ohm). If it is higher, CMPA will show a warning

	const int maximalPartOfDeltaSensorIsNoise = 0.25;		// Maximal part of delta sensor is allowed to be noise
	const int maximalNoise = 60;						// Maximal noise reading (corrupt if higher)
	const int defaultNoise = 20;						// 'Default noise'
	const int maxDifferenceBetweenLightAndDark = 80;	// Maximal difference between light and dark (150...)
};


#endif

