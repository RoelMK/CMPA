// 
// 
// 

#include "MOSFET.h"

void MOSFET::Init()
{
	for (int i = FirstFETpin; i <= LastFETpin; i++)
	{
		pinMode(i, OUTPUT);
	}
	fetON = NoFETsON;
}

void MOSFET::SwitchFET(int fet, uint8_t state)
{
	digitalWrite(fet + FirstFETpin, state);
}

int MOSFET::Update(int LDRState, unsigned long time, double speed)
{
	if (fetON != NoFETsON)
	{
		Serial.print("FET ON: ");
		Serial.print(fetON);
		Serial.print(" - ");
		Serial.print(timeWaited);
		Serial.print(" ms");
		Serial.print(" - ");
		Serial.print(speed);
		Serial.println(" m/s");
	}
	

	if (LDRState == LDR_SENSOR_OK)
	{
		// LDR sensor has found no projectile
		// 1. The projectile is between the LDR and the coil
		if (fetON != NoFETsON)
		{
			// Calculate time waited
			timeWaited += (time - lastTime);
			lastTime = time;
			if (time - lastTime + timeWaited > maxFETPoweredTime)
			{
				return CRITICAL_FET_POWER_ERROR;
			}
			// Is time waited longer than threshold?
			if (speed != 0.00)
			{
				if ((timeWaited / 1000.0) > (DistanceBetweenLDRAndCoil / speed))
				{
					SwitchFET(fetON, LOW);	// Turn off FET
					fetON = NoFETsON;
					timeWaited = 0;
				}
			}
			else
			{
				SwitchFET(fetON, LOW);	// Turn off FET
				fetON = NoFETsON;
				timeWaited = 0;
			}
		}
		// 2. The projectile is between the coil and the LDR
		//  -> Do nothing, it's OK
		return FET_OK;
	}
	else
	{
		// LDR sensor has found the projectile
		// 1. The projectile is for the first time between the LDR and the light source
		if (fetON == NoFETsON)
		{
			lastTime = time;
			SwitchFET(LDRState, HIGH);	// Turn on FET
			fetON = LDRState;
			return FET_OK;
		}
		else
		{
			// 2. The projectile is not for the first time between the LDR and the light source
			if (fetON == LDRState)
			{
				if (time - lastTime + timeWaited > maxFETPoweredTime)
				{
					return CRITICAL_FET_POWER_ERROR;		
				}
				return FET_OK;		// It is the same LDR, so it's OK
			}
			else
			{
				Serial.println("[WARNING] Overspeed!");		// It is a different FET, but another FET is still on, overspeeding!
				SwitchFET(fetON, LOW);
				SwitchFET(LDRState, HIGH);
				fetON = LDRState;
				timeWaited = 0;
				lastTime = time;
				return FET_OK;		
			}
		}
	}
}

void MOSFET::Panic()
{
	if (fetON != NoFETsON)
	{
		SwitchFET(fetON, LOW);
	}
}
