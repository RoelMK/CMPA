// 
// 
// 

#include "OptiLightData.h"

OptiLightData::OptiLightData(double vLow , double vHigh)
{
	VLow = vLow;
	VHigh = vHigh;
	EstimatedFETopenTime = 1;
	EstimatedFETcloseTime = 2;
	Tries = 0;
	HighestAcceleration = 0;
}

OptiLightData::OptiLightData(double vLow, int vHigh, double estimatedFETopenTime, double estimatedFETcloseTime, double highestAcceleration, int tries)
{
	VLow = vLow;
	VHigh = vHigh;
	EstimatedFETopenTime = estimatedFETopenTime;
	EstimatedFETcloseTime = estimatedFETcloseTime;
	HighestAcceleration = highestAcceleration;
	Tries = tries;
}

String OptiLightData::ToString()
{
	return VLow + String(";") + VHigh + ";" + EstimatedFETopenTime + ";" + EstimatedFETcloseTime + ";" + HighestAcceleration + ";" + Tries;
}

