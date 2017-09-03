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
	const double default_distance = 0.015;			// All distances are in meters
	const double LDR0_Coil0_distance = 0.019;
	const double LDR1_Coil1_distance = 0.017;
	const double LDR2_Coil2_distance = 0.019;
	const double LDR3_Coil3_distance = 0.013;
};


#endif

