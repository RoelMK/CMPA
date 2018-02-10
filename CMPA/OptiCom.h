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
	bool GetData(int nextFET, OptiLightData* data, int* arrayLength);
	void SetData(int nextFET, OptiLightData* data, int arrayLength);
	bool SyncData();
private:

	// Sensor data
	OptiLightData* dataSensor0;
	int dataSensor0Length;
	OptiLightData* dataSensor1;
	int dataSensor1Length;
	OptiLightData* dataSensor2;
	int dataSensor2Length;
	OptiLightData* dataSensor3;
	int dataSensor3Length;
	OptiLightData* dataSensor4;
	int dataSensor4Length;
	OptiLightData* dataSensor5;
	int dataSensor5Length;
	OptiLightData* dataSensor6;
	int dataSensor6Length;
	OptiLightData* dataSensor7;
	int dataSensor7Length;
	OptiLightData* dataSensor8;
	int dataSensor8Length;
	OptiLightData* dataSensor9;
	int dataSensor9Length;
	OptiLightData* dataSensor10;
	int dataSensor10Length;
	OptiLightData* dataSensor11;
	int dataSensor11Length;
	OptiLightData* dataSensor12;
	int dataSensor12Length;
	OptiLightData* dataSensor13;
	int dataSensor13Length;
	OptiLightData* dataSensor14;
	int dataSensor14Length;
	OptiLightData* dataSensor15;
	int dataSensor15Length;

	// OptiCom
	bool ExecuteOptiComCommand(String command, String *response);

	const String OptiComGetData = "*G%";
	const String OptiComGetResponseNODATA = "NODATA";
	const String OptiComSetData = "*S%";
	const String OptiComSetResponseOK = "SETOK";
	const int OptiComPreCommandWait = 10;
	const int OptiComAfterCommandWait = 10;

	// Conversion 
	bool SimpleSplit(String *dataIn, OptiLightData *dataOut, int *length);

	bool ConvertFromSerialInputData(String dataIn);
	String ConvertToSerialData();
	void AddObjectDataToString(String *stringToAddDataTo, OptiLightData *data, int length);
	bool SplitSingleOptiLightSerialData(String *strData, double *vLow, double *vHigh, double *estimatedFETcloseTime, double *highestAcceleration, int *tries);
	bool SplitSensorCoilBlockOptiLightSerialData(String *dataIn, String *dataOut, int *arrayLength);
	bool SplitMultipleOptiLightSerialData(String *dataIn, String *dataOut, int *arrayLength);
	const int amountOfSerialData = 6;
	const int MAGIC_NUMBER = 255;
	const String MAGIC_NUMBER_STR = "255";

	// String <-> Object
	OptiLightData ToObject(String *strData, bool *result);
	int convertToInt(String dataToConvert);
	bool isValidNumber(String str);
	void splitString(String *strToSplit, char *separator, String *out);
	int countChars(String *str, char charToCount);
	double convertToDouble(String dataToConvert);
};

#endif

