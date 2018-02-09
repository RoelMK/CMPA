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
		//OptiLightData newData = OptiLightData(0, 1200, 0.0015, 0, 0);
		//for (int s = 0; s < SensorCount; s++)
		//{
		//	optiCom->SetData(s, &newData, 1);
		//}
		//optiCom->SyncData();
	}
	else
	{
		Serial.println("[INFO] OptiLight data loaded");
	}
}

void OptiLight::Update(int LDRStatus)
{

}

double OptiLight::GetFETOnTime(int FET, double speed)
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