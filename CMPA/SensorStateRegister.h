// SensorStateRegister.h

#ifndef _SENSORSTATEREGISTER_h
#define _SENSORSTATEREGISTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"

class SensorStateRegister
{
public:
	void Init();				// Init SSR
	int SensorDetection;		// Sensor which has detected a projectile (NO_SENSOR_DETECTION if none)
	double LastSpeed;			// Last measured speed
private:

};

#endif

