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
	OptiLightData(double vLow, double vHigh);
	OptiLightData(double vLow, int vHigh, double estimatedFETopenTime, double estimatedFETcloseTime, double highestAcceleration, int tries);
	String ToString();

	double VLow;
	double VHigh;
	double EstimatedFETopenTime;
	double EstimatedFETcloseTime;
	double HighestAcceleration;
	int Tries;
private:

};

#endif

