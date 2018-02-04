// MOSFET.h

#ifndef _MOSFET_h
#define _MOSFET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LDR.h"
#include "SpeedToTime.h"

#define FirstFETpin 2
#define LastFETpin 17

#define NoFETsON -1
#define maxFETPoweredTime 1000	// Max time a FET is allowed to be powered on (in ms)

#define CRITICAL_FET_POWER_ERROR -2
#define CRITICAL_FET_SELECTION_ERROR -1
#define FET_OK 1

class MOSFET
{
public:
	void Init(SpeedToTime *speedTimePNT);							// Init FETs
	void SwitchFET(int fet, uint8_t state);							// Switch FET low/high
	int Update(int LDRState, unsigned long time, double speed);		// Update FETs
	void Panic();													// Panic: turn off all FETs
private:
	SpeedToTime *speedTime;		// Speed to time object
	int fetON;					// Number of FET on
	unsigned long lastTime;		// Last time
	int timeWaited;				// Time waited
};

#endif

