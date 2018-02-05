// SpeedToTime.h

#ifndef _SPEEDTOTIME_h
#define _SPEEDTOTIME_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class SpeedToTime
{
public:
	double getFETOnTime(int FET, double speed);
private:
	const double default_distance = 0.2628;			// All distances are in meters

	const double LDR0_DTNC = 0.4228;	// LDR0 = exit, DTNC = Distance to next coil (in m) R
	const double LDR1_DTNC = 0.2628;	// L
	const double LDR2_DTNC = 0.2628;	// L
	const double LDR3_DTNC = 0.2628;	// L
	const double LDR4_DTNC = 0.2628;	// L
	const double LDR5_DTNC = 0.2628;	// L
	const double LDR6_DTNC = 0.2628;	// L
	const double LDR7_DTNC = 0.2628;	// L
	const double LDR8_DTNC = 0.4228;	// LDR8 = entrance L
	const double LDR9_DTNC = 0.2628;	// R
	const double LDR10_DTNC = 0.2628;	// R
	const double LDR11_DTNC = 0.2628;	// R
	const double LDR12_DTNC = 0.2628;	// R
	const double LDR13_DTNC = 0.2628;	// R
	const double LDR14_DTNC = 0.2628;	// R
	const double LDR15_DTNC = 0.2628;	// R
};


#endif

