// 
// 
// 

#include "MOSFET.h"

void MOSFET::Init(SpeedToTime *speedTimePNT)
{
	speedTime = speedTimePNT;
	for (int i = FirstFETpin; i <= LastFETpin; i++)
	{
		pinMode(i, OUTPUT);
	}
	fetON = NoFETsON;
}

void MOSFET::SwitchFET(int fet, uint8_t state, int turnOnAfterMs)
{
	if (turnOnAfterMs != 0)
	{
		timeToWaitBeforeTurningOnFET = turnOnAfterMs;
		fetToSwitchON = fet;
	}
	else
	{
		if (fet == fetToSwitchON && state == LOW)
		{
			fetToSwitchON = NoFETsON;
			timeToWaitBeforeTurningOnFET = 0;
		}
		else if(state == HIGH)
		{
			fetToSwitchON = NoFETsON;
		}

		digitalWrite(fet + FirstFETpin, state);
	}
}

int MOSFET::UpdateFETRealState(unsigned long time)
{
	// Check if any FETs should be turned on
	if (fetToSwitchON != NoFETsON)
	{
		timeToWaitBeforeTurningOnFET -= (time - lastTime);
		if (timeToWaitBeforeTurningOnFET <= 0)
		{
			SwitchFET(fetToSwitchON, HIGH, 0);
			fetToSwitchON = NoFETsON;
			timeToWaitBeforeTurningOnFET = 0;
		}
	}

	// Check FET time state
	// -> Calculate turn on time
	if (fetON != NoFETsON && fetToSwitchON == NoFETsON)
	{
		fetOnTime += (time - lastTimeFETSwitch);
	}
	else
	{
		fetOnTime = 0;
	}
	// -> Check time
	if (fetOnTime > maxFETPoweredTime)
	{
		return CRITICAL_FET_POWER_ERROR;
	}

	lastTimeFETSwitch = time;
	return FET_OK;
}

int MOSFET::UpdateFETVirtualState(int LDRState, unsigned long time, double speed)
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
			// Is time waited longer than threshold?
			if (speed != 0.00)
			{
				if ((timeWaited / 1000.0) > speedTime->getFETOnTime(fetON, speed))
				{
					SwitchFET(fetON, LOW, 0);	// Turn off FET
					fetON = NoFETsON;
					timeWaited = 0;
				}
			}
			else
			{
				SwitchFET(fetON, LOW, 0);	// Turn off FET
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
			SwitchFET(LDRState, HIGH, CalculateTimeToWait(speedTime->getFETOnTime(fetON, speed) * 1000));	// Turn on FET
			fetON = LDRState;
			return FET_OK;
		}
		else
		{
			// 2. The projectile is not for the first time between the LDR and the light source
			if (fetON == LDRState)
			{
				return FET_OK;		// It is the same LDR, so it's OK
			}
			else
			{
				Serial.println("[WARNING] Overspeed!");		// It is a different FET, but another FET is still on, overspeeding!
				SwitchFET(fetON, LOW, 0);
				SwitchFET(LDRState, HIGH, CalculateTimeToWait(speedTime->getFETOnTime(fetON, speed) * 1000));
				fetON = LDRState;
				timeWaited = 0;
				lastTime = time;
				return FET_OK;		
			}
		}
	}
}

int MOSFET::CalculateTimeToWait(int estimatedTurnOnTime)
{
	if (estimatedTurnOnTime > maxFETPoweredTime * 0.9)
	{
		return estimatedTurnOnTime - maxFETPoweredTime * 0.9;
	}
	else
	{
		return 0;
	}
}

void MOSFET::Panic()
{
	if (fetON != NoFETsON)
	{
		SwitchFET(fetON, LOW, 0);
	}
}
