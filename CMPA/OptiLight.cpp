// 
// 
// 

#include "OptiLight.h"

void OptiLight::Init(OptiCom *optiComPNT)
{
	//optiCom = optiComPNT;
	//bool result = optiCom->Init();	// Start OptiCom
	//if (!result)
	//{
	//	Serial.println("[INFO] Failed to load OptiLight data");
	//	optiLightWorking = false;
	//	//OptiLightData newData = OptiLightData(0, 1200, 0.0015, 0, 0);
	//	//for (int s = 0; s < SensorCount; s++)
	//	//{
	//	//	optiCom->SetData(s, &newData, 1);
	//	//}
	//	//optiCom->SyncData();
	//}
	//else
	//{
	//	Serial.println("[INFO] OptiLight data loaded");
	//	optiLightWorking = true;
	//}
}

double OptiLight::GetFETOnTime(int FET, double speed)
{
	// Is speed data available?
	if (speed < OptiLightConstants[speedLevels - 1][OPTILIGHT_VHIGH])
	{
		for (int i = 0; i < speedLevels; i++)
		{
			// Yes: return data for correct speed
			if (speed >= OptiLightConstants[i][OPTILIGHT_VLOW] && speed < OptiLightConstants[i][OPTILIGHT_VHIGH])
			{
				return OptiLightConstants[i][OPTILIGHT_ESTIMATED_FETOFF_TIME];
			}
			// Failed to find OptiLight data, return primitive data
			Serial.print("[WARNING] Failed to load OptiLight data for FET");
			Serial.print(FET);
			Serial.print(", speed: ");
			Serial.println(speed);
			return GetPrimitiveTimeData(FET, speed);
		}
	}
	else
	{
		// No: return data for highest speed
		Serial.print("[WARNING] Overspeed, failed to load OptiLight data for FET");
		Serial.print(FET);
		Serial.print(", speed: ");
		Serial.println(speed);
		return OptiLightConstants[speedLevels - 1][OPTILIGHT_ESTIMATED_FETOFF_TIME];
	}
}


void OptiLight::Update()
{
	// TODO: self-optimizing algorithm
}
//
//double OptiLight::GetFETOnTime(int FET, double speed)
//{
//	// Not working? Return primitive data
//	if (!optiLightWorking)
//	{
//		return GetPrimitiveTimeData(FET, speed);
//	}
//
//	// Load OptiLight data
//	int tmpArrayLength;									// Length OptiLight data array
//	OptiLightData *tmpData;								// Data array
//	optiCom->GetData(FET, tmpData, &tmpArrayLength);	// Load data
//
//	// Is speed data available?
//	if (tmpData[tmpArrayLength - 1].VHigh >= speed)
//	{
//		// Yes: return data for correct speed
//		for (int vLevel = 0; vLevel < tmpArrayLength; vLevel++)
//		{
//			// Find OptiLight data
//			if (speed >= tmpData[vLevel].VLow && speed < tmpData[vLevel].VHigh)
//			{
//				return tmpData[vLevel].EstimatedFETcloseTime;
//			}
//		}
//		// Failed to find OptiLight data, return primitive data
//		Serial.print("[WARNING] Failed to load OptiLight data for FET");
//		Serial.print(FET);
//		Serial.print(", speed: ");
//		Serial.println(speed);
//		return GetPrimitiveTimeData(FET, speed);
//	}
//	else
//	{
//		// No: return data for highest speed
//		Serial.print("[WARNING] Overspeed, failed to load OptiLight data for FET");
//		Serial.print(FET);
//		Serial.print(", speed: ");
//		Serial.println(speed);
//		return tmpData[tmpArrayLength - 1].EstimatedFETcloseTime;
//	}
//}

double OptiLight::GetPrimitiveTimeData(int FET, double speed)
{
	double distance = default_distance;

	switch (FET)
	{
	case 0:
		distance = LDR0_DTNC;
		break;
	case 1:
		distance = LDR1_DTNC;
		break;
	case 2:
		distance = LDR2_DTNC;
		break;
	case 3:
		distance = LDR3_DTNC;
		break;
	case 4:
		distance = LDR4_DTNC;
		break;
	case 5:
		distance = LDR5_DTNC;
		break;
	case 6:
		distance = LDR6_DTNC;
		break;
	case 7:
		distance = LDR7_DTNC;
		break;
	case 8:
		distance = LDR8_DTNC;
		break;
	case 9:
		distance = LDR9_DTNC;
		break;
	case 10:
		distance = LDR10_DTNC;
		break;
	case 11:
		distance = LDR11_DTNC;
		break;
	case 12:
		distance = LDR12_DTNC;
		break;
	case 13:
		distance = LDR13_DTNC;
		break;
	case 14:
		distance = LDR14_DTNC;
		break;
	case 15:
		distance = LDR15_DTNC;
		break;
	default:
		distance = default_distance;
		break;
	}

	return distance / speed * 1000;
}