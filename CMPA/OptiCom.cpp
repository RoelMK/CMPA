// 
// 
// 

#include "OptiCom.h"

bool OptiCom::Init()
{
	// Serial init
	Serial.println("[INFO] OptiCom is loading... please make sure OptiCom is running on your PC");
	Serial.println("[OPTICOM] GET");

	// Get input
	String input;
	while (Serial.available() > 0) {
		input = Serial.readString();
	}

	// Init data and return
	return ConvertFromSerialInputData(input);
}

bool OptiCom::GetData(int sensorCoilblock, OptiLightData* data, int* arrayLength)
{
	switch (sensorCoilblock)
	{
	case 0:
		data = dataSensor0;
		arrayLength = &dataSensor0Length;
		break;
	case 1:
		data = dataSensor1;
		arrayLength = &dataSensor1Length;
		break;
	case 2:
		data = dataSensor2;
		arrayLength = &dataSensor2Length;
		break;
	case 3:
		data = dataSensor3;
		arrayLength = &dataSensor3Length;
		break;
	default:
		return false;
	}
	return true;
}

String OptiCom::ConvertToSerialData()
{

}

bool OptiCom::ConvertFromSerialInputData(String dataIn)
{
	String *dataSplitted;
	int arrayLength = 0;

	// Step 1. Split data on sensor level
	bool result = SplitMultipleOptiLightSerialData(dataIn, dataSplitted, &arrayLength);
	if (result && arrayLength > sensorCount)
	{
		for (int s = 1; s < arrayLength; s++)
		{
			// Step 2. Split data on noise threshold level
			String *dataForSensorCoilBlock;
			int arrayLengthForSensorCoilBlock = 0;
			bool resultForSensorCoilBlock = SplitSensorCoilBlockOptiLightSerialData(dataSplitted[s], dataForSensorCoilBlock, &arrayLengthForSensorCoilBlock);

			// Step 3. Split data and convert to objects
			if (resultForSensorCoilBlock && arrayLengthForSensorCoilBlock > 0)
			{
				OptiLightData *data;

				for (int x = 1; x < arrayLengthForSensorCoilBlock; x++)
				{
					bool resultForThresholdBlock;
					data[x] = ToObject(dataForSensorCoilBlock[x], &resultForThresholdBlock);
					// TODO: use resultForThresholdBlock
				}

				// Store data
				switch (s)
				{
				case 1:
					dataSensor0 = data;
					dataSensor0Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 2:
					dataSensor1 = data;
					dataSensor1Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 3:
					dataSensor2 = data;
					dataSensor2Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 4:
					dataSensor3 = data;
					dataSensor3Length = arrayLengthForSensorCoilBlock - 1;
					break;
				default:
					break;
				}

				delete[] dataForSensorCoilBlock;
			}
			else
			{
				delete[] dataSplitted;
				return false;
			}
		}

		delete[] dataSplitted;
		return true;
	}
	else
	{
		return false;
	}
}

OptiLightData OptiCom::ToObject(String strData, bool *result)
{
	int lowNoiseThreshold = 0;
	int highNoiseThreshold = 1023;
	double detectionAreaLength = LDRlength;
	double highestAcceleration = 0;
	int tries = 0;

	*result = SplitSingleOptiLightSerialData(strData, &lowNoiseThreshold, &highNoiseThreshold, &detectionAreaLength, &highestAcceleration, &tries);

	OptiLightData data(lowNoiseThreshold, highNoiseThreshold, detectionAreaLength, highestAcceleration, tries);
	return data;
}


#pragma region Splitting
bool OptiCom::SplitMultipleOptiLightSerialData(String dataIn, String *dataOut, int *arrayLength)
{
	if (dataIn.indexOf("|") > 0) // = valid data
	{
		// 1. Split data 
		int amountOfData = countChars(dataIn, '|') + 1;

		String* data = new String[amountOfData]; //String data[amountOfData];
		char separatorTokenSemicolon[] = "|";

		splitString(dataIn, separatorTokenSemicolon, data);

		// 2. Check magic number
		if (convertToInt(data[0]) != MAGIC_NUMBER)
		{
			delete[] data;
			return false;
		}

		// 3. Return everything
		*arrayLength = amountOfData;
		*dataOut = *data;		// Do not forget to clean!

		return true;
	}

	return false;
}

bool OptiCom::SplitSensorCoilBlockOptiLightSerialData(String dataIn, String *dataOut, int *arrayLength)
{
	if (dataIn.indexOf("@") > 0) // = valid data
	{
		// 1. Split data 
		int amountOfData = countChars(dataIn, '@') + 1;

		String* data = new String[amountOfData]; //String data[amountOfData];
		char separatorTokenSemicolon[] = "@";

		splitString(dataIn, separatorTokenSemicolon, data);

		// 2. Check magic number
		if (convertToInt(data[0]) != MAGIC_NUMBER)
		{
			delete[] data;
			return false;
		}

		// 3. Return everything
		*arrayLength = amountOfData;
		*dataOut = *data;		// Do not forget to clean!

		return true;
	}

	return false;
}

bool OptiCom::SplitSingleOptiLightSerialData(String strData, int *lowNoiseThreshold, int *highNoiseThreshold, double *detectionAreaLength, double *highestAcceleration, int *tries)
{
	if (strData.indexOf(";") > 0) // = valid data
	{
		// 1. Split data 
		int amountOfData = countChars(strData, ';') + 1;

		if (amountOfData < amountOfSerialData) // Is not enough data available?
		{
			return false;
		}

		String* data = new String[amountOfData]; //String data[amountOfData];
		char separatorTokenSemicolon[] = ";";

		splitString(strData, separatorTokenSemicolon, data);

		// 2. Check magic number
		if (convertToInt(data[0]) != MAGIC_NUMBER)
		{
			return false;
		}

		// 3. Return everything
		*lowNoiseThreshold = convertToInt(data[1]);
		*highNoiseThreshold = convertToInt(data[2]);
		*detectionAreaLength = convertToDouble(data[3]);
		*highestAcceleration = convertToDouble(data[4]);
		*tries = convertToInt(data[5]);

		// 4. Clean and return
		delete[] data;

		return true;
	}

	return false;
}
#pragma endregion

#pragma region String
// Convert string to int
int OptiCom::convertToInt(String dataToConvert)
{
	if (isValidNumber(dataToConvert) == true)
	{
		return dataToConvert.toInt();
	}
	else
	{
		return -1;
	}
}

// Convert string to double
double OptiCom::convertToDouble(String dataToConvert)
{
	if (isValidNumber(dataToConvert) == true)
	{
		return dataToConvert.toFloat();
	}
	else
	{
		return -1;
	}
}

// Check if string is a number
bool OptiCom::isValidNumber(String str) {
	bool isNum = false;
	for (byte i = 0; i<str.length(); i++)
	{
		isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
		if (!isNum) return false;
	}
	return isNum;
}

// Split a string
void OptiCom::splitString(String strToSplit, char *separator, String *out)
{
	// Split string
	int arrayLength = strToSplit.length() + 1;
	char* chrArray = new char[arrayLength]; //char chrArray[strToSplit.length() + 1];
	strToSplit.toCharArray(chrArray, strToSplit.length() + 1);

	// Some data
	char *token;
	int x = 0;

	// Get the first token
	token = strtok(chrArray, separator);

	// Find any more?
	while (token != NULL)
	{
		out[x] = token;
		x++;

		token = strtok(NULL, separator);
	}

	// Clean
	delete[] chrArray;
}

// Count number of chars in a string
int OptiCom::countChars(String str, char charToCount)
{
	int arrayLength = str.length() + 1;
	char* chrArray = new char[arrayLength]; //char chrArray[strToSplit.length() + 1];
	str.toCharArray(chrArray, str.length() + 1);

	int x = 0;
	int count = 0;
	while (x < arrayLength)
	{
		if (chrArray[x] == charToCount)
		{
			count++;
		}
		x++;
	}

	// Clean
	delete[] chrArray;

	return count;
}
#pragma endregion

