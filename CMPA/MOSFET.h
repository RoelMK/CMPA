// MOSFET.h

#ifndef _MOSFET_h
#define _MOSFET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LDR.h"

#define FirstFETpin 10
#define LastFETpin 11

#define NoFETsON -1
#define DistanceBetweenLDRAndCoil 0.005	// in m

#define CRITICAL_FET_ERROR -1
#define FET_OK 1

class MOSFET
{
public:
	void Init();													// Init FETs
	void SwitchFET(int fet, uint8_t state);							// Switch FET low/high
	int Update(int LDRState, unsigned long time, double speed);		// Update FETs
	void Panic();													// Panic: turn off all FETs
private:
	int fetON;					// Number of FET on
	unsigned long lastTime;		// Last time
	int timeWaited;				// Time waited
};

#endif

