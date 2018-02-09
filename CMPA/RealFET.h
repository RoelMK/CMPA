// RealFET.h

#ifndef _REALFET_h
#define _REALFET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Constants.h"


class RealFET
{
public:
	void Init();							// Init RealFET
	int Update(unsigned long time);			// Update RealFET timer
	int FETPowered;							// FET powered on (NO_FET_POWERED if none)
	bool SwitchFET(int fet, uint8_t state);	// Switch on/off FET
	void Panic();							// Panic: switch off all FETs
	int State;								// RealFET state
private:
	uint8_t FETState[FETCount];			// FET state per FET (HIGH or LOW)
	
	void SwitchPIN(int fet, uint8_t state);	// Set pin of FET HIGH or LOW

	// Time
	int timeFETOn;							// How long a FET is powered (in ms)
	unsigned long lastTime;

	// FET power spam
	int LastSwitchFET;						// Last FET in SwitchFET
	int SwitchFETTries;						// Number of tries to turn on the FET in a row
	const int MAXIMAL_FETSWITCH_TRIES = 5;	// Maximal number of times in a row a FETSwitch is allowed for the same FET (even if false is returned)

	// FET pins
	const int FirstFETpin = 2;				// First FET pin
	const int LastFETpin = 17;				// Last FET pin
};

#endif

