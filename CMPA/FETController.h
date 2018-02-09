// FETController.h

#ifndef _FETCONTROLLER_h
#define _FETCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "OptiLight.h"
#include "RealFET.h"
#include "SensorStateRegister.h"
#include "Constants.h"

class FETController
{
public:
	void Init(OptiLight *optiLightPNT, SensorStateRegister *ssrPNT, RealFET *realFETPnt);
	int Update(unsigned long time);
private:
	// FET states
	int fetToTurnOn;							// FET to turn on
	int timeToWaitBeforeTurningOnFET;			// How long to wait before turning on the FET in fetToTurnOn (in ms)
	int timeToWaitBeforeTurningOffFET;			// How long to wait before turning off the FET in RealFET (in ms)
	int GetNextFET(int sensor);					// Convert sensor ID to FET ID
	RealFET *realFET;							// RealFET object: all 'real' FET data

	// Timing
	unsigned long lastTime;						// Last time in update
	void UpdateTimer(unsigned long time);		// Update timing

	// Sensor states
	SensorStateRegister *ssr;					// Sensor state register: speed, last sensor detection (all LightSpeed data)

	// OptiLight
	int GetEstimatedFETOnTime(int fet, double speed);	// Let OptiLight calculate the estimated FET on time
	OptiLight *optiLight;						// OptiLight object
};

#endif

