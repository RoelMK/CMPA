// OptiLightData.h

#ifndef _OPTILIGHTDATA_h
#define _OPTILIGHTDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LightSpeed.h"

class OptiLightData
{
public:
	OptiLightData(int lowNoiseThreshold, int highNoiseThreshold);
	OptiLightData(int lowNoiseThreshold, int highNoiseThreshold, double detectionAreaLength, double highestAcceleration, int tries);
	String ToString();

	int LowNoiseThreshold;
	int HighNoiseThreshold;
	double DetectionAreaLength;
	double HighestAcceleration;
	int Tries;
private:

};

#endif

