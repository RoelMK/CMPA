// LDR.h

#ifndef _LDR_h
#define _LDR_h

#include "LightSpeed.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"
#include "SensorStateRegister.h"

class LDR
{
public:
	int Update(unsigned long time);			// Update all sensor data
	void Init(LightSpeed* lightSpeedPNT, SensorStateRegister *ssrPNT);	// Init LDRs

private:
	LightSpeed *lightSpeed;
	SensorStateRegister *ssr;
};

#endif

