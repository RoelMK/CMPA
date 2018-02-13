// 
// NOT WORKING!
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

	// Init data and return
	if (result)
	{
		Serial.println("[INFO] Updating local data");
		return UpdateLocalData(&response);
	}
	else
	{
		return false;
	}
}

bool OptiCom::GetData(double speed, double *estimatedFETOffTime)
{
	double tmp;
	return GetData(speed, estimatedFETOffTime, &tmp);
}

bool OptiCom::GetData(double speed, double *estimatedFETOffTime, double *highestAcceleration)
{
	// Is speed data available?
	if (speed < VHigh[NumberOfSpeedBlocks - 1])
	{
		for (int i = 0; i < NumberOfSpeedBlocks; i++)
		{
			Serial.print(VLow[i]);
			Serial.print(" - ");
			Serial.print(VHigh[i]);
			Serial.print(": ");
			Serial.println(EstimatedFETOffTime[i]);
			// Yes: return data for correct speed
			if (speed >= VLow[i] && speed < VHigh[i])
			{
				*estimatedFETOffTime = EstimatedFETOffTime[i];
				*highestAcceleration = HighestAcceleration[i];
				return true;
			}
		}
		Serial.print("[WARNING] Failed to load OptiLight data for FET, speed: ");
		Serial.println(speed);
		return false;
	}
	else
	{
		Serial.print("[WARNING] Overspeed, failed to load OptiLight data for FET, speed: ");
		Serial.println(speed);
		return false;
	}
}

void OptiCom::SetData(double speed, double estimatedFETOffTime, double highestAcceleration)
{
	for (int i = 0; i < NumberOfSpeedBlocks; i++)
	{
		if (speed >= VLow[i] && speed < VHigh[i])
		{
			SetData(i, estimatedFETOffTime, highestAcceleration);
		}
	}
}

void OptiCom::SetData(int ID, double estimatedFETOffTime, double highestAcceleration)
{
	if (ID < NumberOfSpeedBlocks)
	{
		EstimatedFETOffTime[ID] = estimatedFETOffTime;
		Tries[ID] += 1;
		HighestAcceleration[ID] = highestAcceleration;
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
	Serial.readString();
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
	toReturn += ("|" + MAGIC_NUMBER_STR);	// Old syntax (for FET level support, needs for() loop)
	AddObjectDataToString(&toReturn, VLow, VHigh, EstimatedFETOffTime, HighestAcceleration, Tries, NumberOfSpeedBlocks);
	return toReturn;
}

void OptiCom::AddObjectDataToString(String *stringToAddDataTo, double *vLow, double *vHigh, double *estimatedFETOffTime, double *highestAcceleration, int *tries, int length)
{
	for (int x = 0; x < length; x++)
	{
		*stringToAddDataTo += "@" + MAGIC_NUMBER_STR + ";" + String(vLow[x], 2) + ";" + String(vHigh[x], 2) + ";" + String(estimatedFETOffTime[x], 3) + ";" + String(highestAcceleration[x], 3) + ";" + String(tries[x]);
	}
}

bool OptiCom::UpdateLocalData(String *dataIn)
{
	// Is data OK?
	if (dataIn->indexOf("|") < 1)
	{
		Serial.println("[INFO] OptiLight data is not present");
		return false;
	}
	else
	{
		Serial.println("[INFO] OptiLight data is present");
	}

	int arrayLength = 0;

	// Step 1. Split data on FET level
	bool result = SplitMultipleOptiLightSerialData(dataIn, &arrayLength);
	if (result && arrayLength > 1)
	{
		for (int s = 1; s < arrayLength; s++)
		{
			// Step 2. Split data on speed block level
			int arrayLengthForSensorCoilBlock = 0;
			bool resultForSensorCoilBlock = SplitSensorCoilBlockOptiLightSerialData(&SingleOptiLightSerialData[s], &arrayLengthForSensorCoilBlock);

			// Step 3. Split data and convert to usuable data
			if (resultForSensorCoilBlock && arrayLengthForSensorCoilBlock > 0)
			{
				VLow = new double[arrayLengthForSensorCoilBlock - 1];
				VHigh = new double[arrayLengthForSensorCoilBlock - 1];
				EstimatedFETOffTime = new double[arrayLengthForSensorCoilBlock - 1];
				HighestAcceleration = new double[arrayLengthForSensorCoilBlock - 1];
				Tries = new int[arrayLengthForSensorCoilBlock - 1];
				NumberOfSpeedBlocks = arrayLengthForSensorCoilBlock - 1;

				for (int x = 1; x < arrayLengthForSensorCoilBlock; x++)
				{
					//Serial.print("(");
					//Serial.print(x);
					//Serial.print("/");
					//Serial.print(arrayLengthForSensorCoilBlock);
					//Serial.print("): ");
					//Serial.println(SensorCoilBlockOptiLightSerialData[x]);
					bool resultForSpeedBlock = ToOptiLightData(&SensorCoilBlockOptiLightSerialData[x], &VLow[x - 1], &VHigh[x - 1], &EstimatedFETOffTime[x - 1], &HighestAcceleration[x - 1], &Tries[x - 1]);
					if (!resultForSpeedBlock)
					{
						Serial.println("[WARNING] Corrupt speed block found");
					}
				}
			}
			else
			{
				Serial.println("[WARNING] Corrupt OptiLight data");
				delete[] SingleOptiLightSerialData;
				delete[] SensorCoilBlockOptiLightSerialData;
				return false;
			}
		}

		delete[] SingleOptiLightSerialData;
		delete[] SensorCoilBlockOptiLightSerialData;
		return true;
	}
	else
	{
		Serial.println("[WARNING] Corrupt OptiLight data");
		return false;
	}
}

bool OptiCom::ToOptiLightData(String *strData, double *vLow, double *vHigh, double *estimatedFETOffTime, double *highestAcceleration, int *tries)
{
	return SplitSingleOptiLightSerialData(strData, vLow, vHigh, estimatedFETOffTime, highestAcceleration, tries);
}


#pragma region Splitting
bool OptiCom::SplitMultipleOptiLightSerialData(String *dataIn, int *arrayLength)
{
	if (dataIn->indexOf("|") > 0) // = valid data
	{
		// 1. Split data 
		int amountOfData = countChars(dataIn, '|') + 1;

		String* data = new String[amountOfData];
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
		SingleOptiLightSerialData = data;

		return true;
	}
	return false;
}

bool OptiCom::SplitSensorCoilBlockOptiLightSerialData(String *dataIn, int *arrayLength)
{
	if (dataIn->indexOf("@") > 0) // = valid data
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
			//Serial.print("[DEBUG] AFailed for: ");
			//Serial.println(*dataIn);
			return false;
		}

		// 3. Return everything
		*arrayLength = amountOfData;
		SensorCoilBlockOptiLightSerialData = data;
		return true;
	}

	//Serial.print("[DEBUG] BFailed for: ");
	//Serial.println(*dataIn);
	return false;
}

bool OptiCom::SplitSingleOptiLightSerialData(String *strData, double *vLow, double *vHigh, double *estimatedFETcloseTime, double *highestAcceleration, int *tries)
{
	if (strData->indexOf(";") > 0) // = valid data
	{
		// 1. Split data 
		int amountOfData = countChars(strData, ';') + 1;

		if (amountOfData < amountOfSerialData) // Is not enough data available?
		{
			//Serial.print("[DEBUG] AFailed for: ");
			//Serial.println(*strData);
			return false;
		}

		String* data = new String[amountOfData]; //String data[amountOfData];
		char separatorTokenSemicolon[] = ";";

		splitString(strData, separatorTokenSemicolon, data);

		// 2. Check magic number
		if (convertToInt(data[0]) != MAGIC_NUMBER)
		{
			//Serial.print("[DEBUG] BFailed for: ");
			//Serial.println(*strData);
			return false;
		}

		// 3. Return everything
		*vLow = convertToDouble(data[1]);
		*vHigh = convertToDouble(data[2]);
		*estimatedFETcloseTime = convertToDouble(data[3]);
		*highestAcceleration = convertToDouble(data[4]);
		*tries = convertToInt(data[5]);

		// 4. Clean and return
		delete[] data;
		return true;
	}
	//Serial.print("[DEBUG] CFailed for: ");
	//Serial.println(*strData);
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
void OptiCom::splitString(String *strToSplit, char *separator, String *out)
{
	// Split string
	int arrayLength = strToSplit->length() + 1;
	char* chrArray = new char[arrayLength]; //char chrArray[strToSplit.length() + 1];
	strToSplit->toCharArray(chrArray, strToSplit->length() + 1);

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
int OptiCom::countChars(String *str, char charToCount)
{
	int arrayLength = str->length() + 1;
	char* chrArray = new char[arrayLength]; //char chrArray[strToSplit.length() + 1];
	str->toCharArray(chrArray, str->length() + 1);

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

