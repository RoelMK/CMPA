// 
// 
// 

#include "OptiCom.h"

bool OptiCom::Init()
{
	// Serial init
	Serial.println("[INFO] OptiCom is loading... please make sure OptiCom is running on your PC");
	String response;
	bool result = ExecuteOptiComCommand(OptiComGetData, &response);
	Serial.print("[INFO] OptiCom result: ");
	Serial.println(result);
	Serial.print("[INFO] OptiCom response: ");
	Serial.println(response);
	

	// Init data and return
	if (result)
	{
		return ConvertFromSerialInputData(response);
	}
	else
	{
		return false;
	}
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
	case 4:
		data = dataSensor4;
		arrayLength = &dataSensor4Length;
		break;
	case 5:
		data = dataSensor5;
		arrayLength = &dataSensor5Length;
		break;
	case 6:
		data = dataSensor6;
		arrayLength = &dataSensor6Length;
		break;
	case 7:
		data = dataSensor7;
		arrayLength = &dataSensor7Length;
		break;
	case 8:
		data = dataSensor8;
		arrayLength = &dataSensor8Length;
		break;
	case 9:
		data = dataSensor9;
		arrayLength = &dataSensor9Length;
		break;
	case 10:
		data = dataSensor10;
		arrayLength = &dataSensor10Length;
		break;
	case 11:
		data = dataSensor11;
		arrayLength = &dataSensor11Length;
		break;
	case 12:
		data = dataSensor12;
		arrayLength = &dataSensor12Length;
		break;
	case 13:
		data = dataSensor13;
		arrayLength = &dataSensor13Length;
		break;
	case 14:
		data = dataSensor14;
		arrayLength = &dataSensor14Length;
		break;
	case 15:
		data = dataSensor15;
		arrayLength = &dataSensor15Length;
		break;
	default:
		return false;
	}
	return true;
}

void OptiCom::SetData(int sensorCoilblock, OptiLightData* data, int arrayLength)
{
	switch (sensorCoilblock)
	{
	case 0:
		dataSensor0 = data;
		dataSensor0Length = arrayLength;
		break;
	case 1:
		dataSensor1 = data;
		dataSensor1Length = arrayLength;
		break;
	case 2:
		dataSensor2 = data;
		dataSensor2Length = arrayLength;
		break;
	case 3:
		dataSensor3 = data;
		dataSensor3Length = arrayLength;
		break;
	case 4:
		dataSensor4 = data;
		dataSensor4Length = arrayLength;
		break;
	case 5:
		dataSensor5 = data;
		dataSensor5Length = arrayLength;
		break;
	case 6:
		dataSensor6 = data;
		dataSensor6Length = arrayLength;
		break;
	case 7:
		dataSensor7 = data;
		dataSensor7Length = arrayLength;
		break;
	case 8:
		dataSensor8 = data;
		dataSensor8Length = arrayLength;
		break;
	case 9:
		dataSensor9 = data;
		dataSensor9Length = arrayLength;
		break;
	case 10:
		dataSensor10 = data;
		dataSensor10Length = arrayLength;
		break;
	case 11:
		dataSensor11 = data;
		dataSensor11Length = arrayLength;
		break;
	case 12:
		dataSensor12 = data;
		dataSensor12Length = arrayLength;
		break;
	case 13:
		dataSensor13 = data;
		dataSensor13Length = arrayLength;
		break;
	case 14:
		dataSensor14 = data;
		dataSensor14Length = arrayLength;
		break;
	case 15:
		dataSensor15 = data;
		dataSensor15Length = arrayLength;
		break;
	default:
		break;
	}
}

bool OptiCom::SyncData()
{
	String toSend = ConvertToSerialData();
	String response;
	bool result = ExecuteOptiComCommand(OptiComSetData + toSend, &response);
	if (result)
	{
		if (response == OptiComSetResponseOK)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool OptiCom::ExecuteOptiComCommand(String command, String *response)
{
	// Read data
	delay(OptiComPreCommandWait);
	Serial.println(command);
	delay(OptiComAfterCommandWait);

	// Return data
	if (Serial.available() > 0)
	{
		String serialRead = Serial.readString();
		String correctedData = "";

		for (int c = 0; c < serialRead.length(); c++)
		{
			if ((' ' <= serialRead[c]) && (serialRead[c] <= '~'))	// Correct input, remove all non-printable chars
			{
				correctedData += serialRead[c];
			}
		}

		*response = correctedData;
		return true;
	}
	else
	{
		return false;
	}
}

String OptiCom::ConvertToSerialData()
{
	String toReturn = MAGIC_NUMBER_STR;

	for (int s = 0; s < sensorCount; s++)
	{
		toReturn += ("|" + MAGIC_NUMBER_STR);
		switch(s)
		{
		case 0:
			AddObjectDataToString(&toReturn, dataSensor0, dataSensor0Length);
			break;
		case 1:
			AddObjectDataToString(&toReturn, dataSensor1, dataSensor1Length);
			break;
		case 2:
			AddObjectDataToString(&toReturn, dataSensor2, dataSensor2Length);
			break;
		case 3:
			AddObjectDataToString(&toReturn, dataSensor3, dataSensor3Length);
			break;
		case 4:
			AddObjectDataToString(&toReturn, dataSensor4, dataSensor4Length);
			break;
		case 5:
			AddObjectDataToString(&toReturn, dataSensor5, dataSensor5Length);
			break;
		case 6:
			AddObjectDataToString(&toReturn, dataSensor6, dataSensor6Length);
			break;
		case 7:
			AddObjectDataToString(&toReturn, dataSensor7, dataSensor7Length);
			break;
		case 8:
			AddObjectDataToString(&toReturn, dataSensor8, dataSensor8Length);
			break;
		case 9:
			AddObjectDataToString(&toReturn, dataSensor9, dataSensor9Length);
			break;
		case 10:
			AddObjectDataToString(&toReturn, dataSensor10, dataSensor10Length);
			break;
		case 11:
			AddObjectDataToString(&toReturn, dataSensor11, dataSensor11Length);
			break;
		case 12:
			AddObjectDataToString(&toReturn, dataSensor12, dataSensor12Length);
			break;
		case 13:
			AddObjectDataToString(&toReturn, dataSensor13, dataSensor13Length);
			break;
		case 14:
			AddObjectDataToString(&toReturn, dataSensor14, dataSensor14Length);
			break;
		case 15:
			AddObjectDataToString(&toReturn, dataSensor15, dataSensor15Length);
			break;
		default:
			break;
		}
	}

	return toReturn;
}

void OptiCom::AddObjectDataToString(String *stringToAddDataTo, OptiLightData *data, int length)
{
	for (int x = 0; x < length; x++)
	{
		*stringToAddDataTo += "@" + MAGIC_NUMBER_STR + ";" + data[x].ToString();
	}
}

bool OptiCom::ConvertFromSerialInputData(String dataIn)
{
	// Is data OK?
	if (dataIn.indexOf("|") < 1)
	{
		Serial.println("[INFO] OptiLight data is not present");
		return false;
	}
	else
	{
		Serial.println("[INFO] OptiLight data is present");
	}

	String *dataSplitted;
	int arrayLength = 0;

	// Step 1. Split data on sensor level
	bool result = SplitMultipleOptiLightSerialData(dataIn, dataSplitted, &arrayLength);
	if (result && arrayLength > sensorCount)
	{
		Serial.print("Array0: ");
		Serial.println(dataSplitted[0]);
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
				case 5:
					dataSensor4 = data;
					dataSensor4Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 6:
					dataSensor5 = data;
					dataSensor5Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 7:
					dataSensor6 = data;
					dataSensor6Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 8:
					dataSensor7 = data;
					dataSensor7Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 9:
					dataSensor8 = data;
					dataSensor8Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 10:
					dataSensor9 = data;
					dataSensor9Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 11:
					dataSensor10 = data;
					dataSensor10Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 12:
					dataSensor11 = data;
					dataSensor11Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 13:
					dataSensor12 = data;
					dataSensor12Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 14:
					dataSensor13 = data;
					dataSensor13Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 15:
					dataSensor14 = data;
					dataSensor14Length = arrayLengthForSensorCoilBlock - 1;
					break;
				case 16:
					dataSensor15 = data;
					dataSensor15Length = arrayLengthForSensorCoilBlock - 1;
					break;
				default:
					break;
				}

				delete[] dataForSensorCoilBlock;
			}
			else
			{
				Serial.println("[WARNING] Corrupt OptiLight data");
				delete[] dataSplitted;
				return false;
			}
		}

		delete[] dataSplitted;
		return true;
	}
	else
	{
		Serial.println("[WARNING] Corrupt OptiLight data");
		return false;
	}
}

OptiLightData OptiCom::ToObject(String strData, bool *result)
{
	double vLow = 0;
	double vHigh = 1023;
	double estimatedFETopenTime = LDRlength;				// UPDATE: incorrect data!
	double estimatedFETcloseTime;
	double highestAcceleration = 0;
	int tries = 0;

	*result = SplitSingleOptiLightSerialData(strData, &vLow, &vHigh, &estimatedFETopenTime, &estimatedFETcloseTime, &highestAcceleration, &tries);

	OptiLightData data(vLow, vHigh, estimatedFETopenTime, estimatedFETcloseTime, highestAcceleration, tries);
	return data;
}


#pragma region Splitting
bool OptiCom::SplitMultipleOptiLightSerialData(String dataIn, String *dataOut, int *arrayLength)
{
	if (dataIn.indexOf("|") > 0) // = valid data
	{
		// 1. Split data 
		int amountOfData = countChars(dataIn, '|') + 1;

		String* data = new String[amountOfData]; //String data[amountOfData]; <--- is the issue
		char separatorTokenSemicolon[] = "|";
		
		splitString(dataIn, separatorTokenSemicolon, data);

		Serial.print("Internal1: ");
		Serial.println(data[1]);

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
			Serial.print("[DEBUG] AFailed for: ");
			Serial.println(dataIn);
			return false;
		}

		// 3. Return everything
		*arrayLength = amountOfData;
		*dataOut = *data;		// Do not forget to clean!

		return true;
	}

	Serial.print("[DEBUG] BFailed for: ");
	Serial.println(dataIn);
	return false;
}

bool OptiCom::SplitSingleOptiLightSerialData(String strData, double *vLow, double *vHigh, double *estimatedFETopenTime, double *estimatedFETcloseTime, double *highestAcceleration, int *tries)
{
	if (strData.indexOf(";") > 0) // = valid data
	{
		// 1. Split data 
		int amountOfData = countChars(strData, ';') + 1;

		if (amountOfData < amountOfSerialData) // Is not enough data available?
		{
			Serial.print("[DEBUG] AFailed for: ");
			Serial.println(strData);
			return false;
		}

		String* data = new String[amountOfData]; //String data[amountOfData];
		char separatorTokenSemicolon[] = ";";

		splitString(strData, separatorTokenSemicolon, data);

		// 2. Check magic number
		if (convertToInt(data[0]) != MAGIC_NUMBER)
		{
			Serial.print("[DEBUG] BFailed for: ");
			Serial.println(strData);
			return false;
		}

		// 3. Return everything
		*vLow = convertToDouble(data[1]);
		*vHigh = convertToDouble(data[2]);
		*estimatedFETopenTime = convertToDouble(data[3]);
		*estimatedFETcloseTime = convertToDouble(data[4]);
		*highestAcceleration = convertToDouble(data[5]);
		*tries = convertToInt(data[6]);

		// 4. Clean and return
		delete[] data;
		Serial.println("[DEBUG] OK");
		return true;
	}
	Serial.print("[DEBUG] CFailed for: ");
	Serial.println(strData);
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

