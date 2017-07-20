// LDR.h

#ifndef _LDR_h
#define _LDR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define sensorCount 1
#define detectObjectThreshold 800
#define distanceBetweenSensors 0.1				// In meters
#define distanceBetweenSensorAndNextCoil 0.09	// In meters
#define LDR_SENSOR_OK 0
#define LDR_SENSOR_FAILURE -1 

class LDR
{
public:
	double getSpeed()						// Get speed
	{
		return this->speed;
	}
	double getTime()						// Get time required
	{
		return this->timeRequiredToReachNextCoil;
	}
	int Update(unsigned long time);			// Update all sensor data

private:
	void Calc(bool detectedProjectile, unsigned long time);		// Calculate data

	bool sensorObjectDetection[sensorCount];		// All sensor data
	int lastSensorDetected;							// Last sensor which has detected an object
	unsigned long lastSensorDetectionTime;			// Last sensor detection time		
	unsigned long timeRequiredToReachNextSensor;	// Time required to reach next sensor
	double speed;									// Speed

	unsigned long previousTime;						// Previous time
	double distanceTraveled;						// Distance traveled (estimation)
	double timeRequiredToReachNextCoil;				// Time required to reach next coil (estimation)
};

#endif

