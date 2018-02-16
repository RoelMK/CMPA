// 
// 
// 

#include "OptiLight.h"

void OptiLight::Init(OptiCom *optiComPNT)
{
	optiCom = optiComPNT;
	bool result = optiCom->Init();	// Start OptiCom
	if (!result)
	{
		Serial.println("[INFO] Failed to load OptiLight data");
		optiComWorking = false;
	}
	else
	{
		Serial.println("[INFO] OptiLight data loaded");

		//double estimatedFETOff = -1;
		//double hs = -1;
		//optiCom->GetData(1.0, &estimatedFETOff, &hs);
		//Serial.print("Test: ");
		//Serial.println(estimatedFETOff);
		//optiCom->SetData(1.0, estimatedFETOff + 1, hs);
		//Serial.println("Sync: ");
		//Serial.println(optiCom->SyncData());

		optiComWorking = true;
	}
}

double OptiLight::GetFETOnTime(int FET, double speed)
{
	// Try OptiCom data first
	if (optiComWorking)
	{
		// Load data
		double estimatedTime = -1;
		bool result = optiCom->GetData(speed, &estimatedTime);

		// Return
		if (result && estimatedTime > 0) return ImproveTimingForEntranceExit(FET, estimatedTime) * GetArtificalTimingMultiplicationFactor(FET, estimatedTime);
	}

	Serial.print("[WARNING] Failed to load OptiCom data for FET");
	Serial.print(FET);
	Serial.print(", speed: ");
	Serial.println(speed);
	return GetBackupFETOnTime(FET, speed);
}

double OptiLight::GetBackupFETOnTime(int FET, double speed)
{
	// Is speed data available?
	if (speed < OptiLightConstants[speedLevels - 1][OPTILIGHT_VHIGH])
	{
		for (int i = 0; i < speedLevels; i++)
		{
			// Yes: return data for correct speed
			if (speed >= OptiLightConstants[i][OPTILIGHT_VLOW] && speed < OptiLightConstants[i][OPTILIGHT_VHIGH])
			{
				return ImproveTimingForEntranceExit(FET, OptiLightConstants[i][OPTILIGHT_ESTIMATED_FETOFF_TIME]) * GetArtificalTimingMultiplicationFactor(FET, OptiLightConstants[i][OPTILIGHT_ESTIMATED_FETOFF_TIME]);
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
		return ImproveTimingForEntranceExit(FET, OptiLightConstants[speedLevels - 1][OPTILIGHT_ESTIMATED_FETOFF_TIME]) * GetArtificalTimingMultiplicationFactor(FET, OptiLightConstants[speedLevels - 1][OPTILIGHT_ESTIMATED_FETOFF_TIME]);
	}
}

double OptiLight::GetArtificalTimingMultiplicationFactor(int FET, double modelData)
{
	if (FET < FETCount)
	{
		if (ARTMF_FET_ENABLED[FET])
		{
			return ARTMF_FET[FET];
		}
		else
		{
			return DEFAULT_ARTM;
		}
	}
	else
	{
		return DEFAULT_ARTM;
	}
}


void OptiLight::Update()
{
	// TODO: self-optimizing algorithm
}

double OptiLight::ImproveTimingForEntranceExit(int FET, double time)
{
	if (FET == FETAfterEntrance || FET == FETAfterExit)
	{
		return time * (DEFAULT_DISTANCE_AFTER_ENTRANCEEXIT / DEFAULT_DISTANCE);	// Increase time, extra distance
	}
	else
	{
		return time;
	}
}

double OptiLight::GetPrimitiveTimeData(int FET, double speed)
{
	double distance = DEFAULT_DISTANCE;

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
		distance = DEFAULT_DISTANCE;
		break;
	}

	return distance / speed * 1000;
}