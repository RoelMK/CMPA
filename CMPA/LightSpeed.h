// LightSpeed.h

#ifndef _LIGHTSPEED_h
#define _LIGHTSPEED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LDR.h"

#define LED_ONOFF_PIN 10	// Digital pin of LED transistor
#define calibrateTime 1000	// Time to wait to detect noise (in ms)
#define ANALOGREAD_DELAY 150
#define sensorCount 4

#define LIGHTSPEED_CALIBRATION_OK 1


class LightSpeed
{
public:
	int Calibrate();
private:
	int sensorUMax[sensorCount];
	int sensorUMin[sensorCount];
	int sensorUNoise[sensorCount];
	int sensorNoiseThreshold[sensorCount];
	int sensorDetectionThreshold[sensorCount];

};


#endif

