// OptiCom.h

#ifndef _OPTICOM_h
#define _OPTICOM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LightSpeed.h"
#include "OptiLightData.h"

class OptiCom
{
public:
	bool Init();	
	bool GetData(int sensorCoilblock, OptiLightData* data, int* arrayLength);
private:
	OptiLightData* dataSensor0;
	int dataSensor0Length;
	OptiLightData* dataSensor1;
	int dataSensor1Length;
	OptiLightData* dataSensor2;
	int dataSensor2Length;
	OptiLightData* dataSensor3;
	int dataSensor3Length;


	bool ConvertFromSerialInputData(String dataIn);
	String ConvertToSerialData();
	bool SplitSingleOptiLightSerialData(String strData, int *lowNoiseThreshold, int *highNoiseThreshold, double *detectionAreaLength, double *highestAcceleration, int *tries);
	bool SplitSensorCoilBlockOptiLightSerialData(String dataIn, String *dataOut, int *arrayLength);
	bool SplitMultipleOptiLightSerialData(String dataIn, String *dataOut, int *arrayLength);
	const int amountOfSerialData = 6;
	const int MAGIC_NUMBER = 255;

	OptiLightData ToObject(String strData, bool *result);
	int convertToInt(String dataToConvert);
	bool isValidNumber(String str);
	void splitString(String strToSplit, char *separator, String *out);
	int countChars(String str, char charToCount);
	double convertToDouble(String dataToConvert);
};

#endif

