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
	bool Init();																			// Init OptiCom
	bool GetData(double speed, double *estimatedFETOffTime);								// Get OptiCom data for speed ...
	bool GetData(double speed, double *estimatedFETOffTime, double *highestAcceleration);	// Get OptiCom data for speed ... and also return the highestAcceleration
	void SetData(double speed, double estimatedFETOffTime, double highestAcceleration);		// Set OptiCom data for speed ...
	void SetData(int ID, double estimatedFETOffTime, double highestAcceleration);			// Set OptiCom data for ID ...
	bool SyncData();																		// Send OptiCom data to PC
private:
	double *VLow;					// VLow array
	double *VHigh;					// VHigh array
	double *EstimatedFETOffTime;	// EstimatedFETOffTime array
	double *HighestAcceleration;	// Highest acceleration array
	int *Tries;						// Tries array
	int NumberOfSpeedBlocks;		// Length of array

	// OptiCom
	bool ExecuteOptiComCommand(String command, String *response);	// Execute OptiCom command, can falsely return false when syncing

	const String OptiComGetData = "*G%";							// Command: get data
	const String OptiComGetResponseNODATA = "NODATA";				// Response: no data available
	const String OptiComSetData = "*S%";							// Command: set data
	const String OptiComSetResponseOK = "SETOK";					// Response: data set
	const int OptiComPreCommandWait = 10;							// Ms to wait before executing command
	const int OptiComAfterCommandWait = 10;							// Ms to wait after executing command

	// Conversion 
	String *SingleOptiLightSerialData;				// Pointer to data from SplitSingleOptiLightSerialData
	String *SensorCoilBlockOptiLightSerialData;		// Pointer to data from SplitSensorCoilBlockOptiLightSerialData

	bool UpdateLocalData(String *dataIn);			// Update local data using serial input 
	String ConvertToSerialData();					// Convert local data to serial output
	void AddObjectDataToString(String *stringToAddDataTo, double *vLow, double *vHigh, double *estimatedFETOffTime, double *highestAcceleration, int *tries, int length);	// Convert OptiCom data to string
	bool SplitSingleOptiLightSerialData(String *strData, double *vLow, double *vHigh, double *estimatedFETcloseTime, double *highestAcceleration, int *tries);				// Split at sensor coil block level
	bool SplitSensorCoilBlockOptiLightSerialData(String *dataIn, int *arrayLength);			// Split at speed block level
	bool SplitMultipleOptiLightSerialData(String *dataIn, int *arrayLength);				// Split at FET level (not supported)
	const int amountOfSerialData = 6;			// Amount of serial data per speed block
	const int MAGIC_NUMBER = 255;				// Magic number: for verification
	const String MAGIC_NUMBER_STR = "255";		// String of magic number

	// String <-> Object
	bool ToOptiLightData(String *strData, double *vLow, double *vHigh, double *estimatedFETOffTime, double *highestAcceleration, int *tries);	// Convert string to OptiLight data
	int convertToInt(String dataToConvert);								// Convert string to int
	bool isValidNumber(String str);										// Is string number?
	void splitString(String *strToSplit, char *separator, String *out);	// Split a string
	int countChars(String *str, char charToCount);						// Count chars in string
	double convertToDouble(String dataToConvert);						// Convert string to double
};

#endif

