// 
// 
// 

#include "OptiLightData.h"

OptiLightData::OptiLightData(int lowNoiseThreshold, int highNoiseThreshold)
{
	LowNoiseThreshold = lowNoiseThreshold;
	HighNoiseThreshold = highNoiseThreshold;
	DetectionAreaLength = LDRlength;
	Tries = 0;
	HighestAcceleration = 0;
}

OptiLightData::OptiLightData(int lowNoiseThreshold, int highNoiseThreshold, double detectionAreaLength, double highestAcceleration, int tries)
{
	LowNoiseThreshold = lowNoiseThreshold;
	HighNoiseThreshold = highNoiseThreshold;
	DetectionAreaLength = detectionAreaLength;
	HighestAcceleration = highestAcceleration;
	Tries = tries;
}

String OptiLightData::ToString()
{
	return LowNoiseThreshold + String(";") + HighNoiseThreshold + ";" + DetectionAreaLength + ";" + HighestAcceleration + ";" + Tries;
}

