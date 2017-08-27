// LDR.h

#ifndef _LDR_h
#define _LDR_h

#include "LightSpeed.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


const int LDR_SENSOR_OK = -1;				// Sensors OK, no projectile found
const int LDR_SENSOR_FAILURE = -2;			// Sensors not OK 

class LDR
{
public:
	double getSpeed();						// Get best speed
	int Update(unsigned long time);			// Update all sensor data
	void Init(LightSpeed* lightSpeedPNT);	// Init LDRs

private:
	LightSpeed *lightSpeed;
};

#endif

