// 
// 
// 

#include "OptiLight.h"

void OptiLight::Init(LightSpeed *lightSpeedPNT, OptiCom *optiComPNT)
{
	lightSpeed = lightSpeedPNT;
	optiCom = optiComPNT;
	bool result = optiCom->Init();	// Start OptiCom
	if (!result)
	{
		Serial.println("[INFO] Failed to load OptiLight data");
		//OptiLightData newData = OptiLightData(0, 1200, 0.0015, 0, 0);
		//for (int s = 0; s < sensorCount; s++)
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

