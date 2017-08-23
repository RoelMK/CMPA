// LDR.h

#ifndef _LDR_h
#define _LDR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define sensorCount 4							// Begin analog A0 (1=A0, 2=A0,A1)
#define calibrateDifferenceWarningThreshold 250	// If difference between two calibration readings is greater than this value, the software will display a warning
#define differenceBetweenLightAndDarkForLDR 40	// Difference between light and dark
#define distanceBetweenSensors 0.13				// In meters (0.52/4 for POC)
#define distanceBetweenSensorAndNextCoil 0.065	// In meters
#define LDRlength 0.004							// In meters, short-axis
#define LDR_SENSOR_OK -1						// Sensors OK, no projectile found
#define LDR_SENSOR_FAILURE -2					// Sensors not OK 

class LDR
{
public:
	double getSpeed()
	{
		if (speedV1 == 0)
		{
			return this->speedV2;
		}
		else
		{
			return this->speedV1;
		}
	}

	double getSpeedV1()						// Get speed
	{
		return this->speedV1;
	}
	double getSpeedV2()
	{
		return this->speedV2;
	}
	int Update(unsigned long time);			// Update all sensor data
	void Reset();							// Reset all data
	void Init();							// Init LDRs

private:
	void Calc(bool detectedProjectile, unsigned long time);		// Calculate data
	bool sensorObjectDetection[sensorCount];		// All sensor data
	int sensorObjectDetectionThreshold[sensorCount];	// Sensor detection threshold (analog read value threshold (< = detection, > = no detection))
	int previousSensorDetected;						// Previous sensor which has detected an object
	int sensorDetected;								// Sensor which detected an object
	void inRuntimeCalibration(int sensor);
	unsigned long lastSensorDetectionTime;			// Last sensor detection time		
	unsigned long timeRequiredToReachNextSensor;	// Time required to reach next sensor
	
	double speedV1;									// Speed, calculated using the time the projectile needed to get to the next sensor (high speed, less accurate)
	double speedV2;									// Speed, calculated using the time the projectile needed to get off the sensor (only works when the speed is low, high accuracy)
	
	unsigned long timeSensorOn;						// Time the LDR is on
	unsigned long previousTime;						// Previous time
};

#endif

