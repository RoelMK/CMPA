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
	void SwitchFET(int fet, uint8_t state, int turnOnAfterMs);		// Switch FET low/high
	int UpdateFETVirtualState(int LDRState, unsigned long time, double speed);		// Update FET virtual state
	int UpdateFETRealState(unsigned long time);					// Update FET real state
	void Panic();													// Panic: turn off all FETs
private:
	int CalculateTimeToWait(int estimatedTurnOnTime);	// Calculate how long to wait before turning on FET
	SpeedToTime *speedTime;		// Speed to time object
	int fetON;					// ID of FET which is (virtually) on
	int timeToWaitBeforeTurningOnFET;	// Time to wait before turning on FET
	int fetToSwitchON;					// FET to switch on (only used when timeToWaitBeforeTurningOnFET > 0
	int fetOnTime;						// How long a FET is on
	unsigned long lastTime;				// Last time (for FET timer)
	unsigned long lastTimeFETSwitch;	// Last time (for FET on/off timer)
	int timeWaited;				// Time waited, how long a FET is 'virtually' on
};

#endif

