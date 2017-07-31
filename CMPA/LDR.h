// LDR.h

#ifndef _LDR_h
#define _LDR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define sensorCount 4							// Begin analog A0 (1=A0, 2=A0,A1)
#define calibrateDifferenceWarningThreshold 100	// If difference between two calibration readings is greater than this value, the software will display a warning
#define differenceBetweenLightAndDarkForLDR 100	// Difference between light and dark
#define distanceBetweenSensors 0.13				// In meters (0.52/4 for POC)
#define distanceBetweenSensorAndNextCoil 0.065	// In meters
#define LDR_SENSOR_OK -1						// Sensors OK, no projectile found
#define LDR_SENSOR_FAILURE -2					// Sensors not OK 

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
	void Reset();							// Reset all data
	void Init();							// Init LDRs

private:
	void Calc(bool detectedProjectile, unsigned long time);		// Calculate data
	bool sensorObjectDetection[sensorCount];		// All sensor data
	int sensorObjectDetectionThreshold[sensorCount];	// Sensor detection threshold (analog read value threshold (< = detection, > = no detection))
	int lastSensorDetected;							// Last sensor which has detected an object
	unsigned long lastSensorDetectionTime;			// Last sensor detection time		
	unsigned long timeRequiredToReachNextSensor;	// Time required to reach next sensor
	double speed;									// Speed

	unsigned long previousTime;						// Previous time
	double distanceTraveled;						// Distance traveled (estimation)
	double timeRequiredToReachNextCoil;				// Time required to reach next coil (estimation)
};

#endif

