// OptiLight.h

#ifndef _OPTILIGHT_h
#define _OPTILIGHT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LightSpeed.h"
#include "OptiLightData.h"
#include "OptiCom.h"

class OptiLight
{
public:
	void Init(LightSpeed *lightSpeedPNT, OptiCom *optiComPNT);		// Init OptiLight
	void Update(int LDRStatus);					// Update OptiLight
private:
	LightSpeed *lightSpeed;
	OptiCom *optiCom;
};

#endif

