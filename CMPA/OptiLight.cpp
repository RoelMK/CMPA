// 
// 
// 

#include "OptiLight.h"

void OptiLight::Init(LightSpeed *lightSpeedPNT, OptiCom *optiComPNT)
{
	lightSpeed = lightSpeedPNT;
	optiCom = optiComPNT;
	optiCom->Init();	// Start OptiCom
}

void OptiLight::Update(int LDRStatus)
{

}

