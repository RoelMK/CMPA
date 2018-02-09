// 
// 
// 

#include "FETController.h"
void FETController::Init(OptiLight *optiLightPNT, SensorStateRegister *ssrPNT, RealFET *realFETPnt)
{
	// Set pointers
	optiLight = optiLightPNT;
	ssr = ssrPNT;
	realFET = realFETPnt;

	// Init
	realFET->Init();
}

int FETController::Update(unsigned long time)
{
	bool overspeedError = false;
	// Sensor detection or not?
	if (ssr->SensorDetection != NO_SENSOR_DETECTION)
	{
		// Yes: any FET powered or in fetToTurnOn?
		if (realFET->FETPowered == NO_FET_POWERED && fetToTurnOn == NO_FET_POWERED)
		{
			// No: switch on FET
			// > Get all required data
			fetToTurnOn = GetNextFET(ssr->SensorDetection);							// Get and set the FET to switch next
			int totalTimeOn = GetEstimatedFETOnTime(fetToTurnOn, ssr->LastSpeed);	// Get the total time the FET 'could' be on (most important is the turn off timing)
			
			// > When to switch on/off the FET?
			if (totalTimeOn > MAXIMAL_TIME_FET_ON * 0.9)		// Never let the FET be powered longer than 90% of the maximal time
			{
				timeToWaitBeforeTurningOnFET = totalTimeOn - MAXIMAL_TIME_FET_ON * 0.9;
				timeToWaitBeforeTurningOffFET = totalTimeOn;
			}
			else
			{
				timeToWaitBeforeTurningOnFET = 0;
				timeToWaitBeforeTurningOffFET = totalTimeOn;
			}
		}
		else
		{
			// Yes: check for issues
			// > Issue 1: it is trying to power a FET, but another FET is still on, overspeeding!
			if (GetNextFET(ssr->SensorDetection) != realFET->FETPowered && GetNextFET(ssr->SensorDetection) != fetToTurnOn)
			{
				overspeedError = true;	// Overspeed
			}

		}
	}
	else
	{
		// No: any FET powered?
		if (realFET->FETPowered != NO_FET_POWERED || fetToTurnOn != NO_FET_POWERED)
		{
			// Yes
			// > Expected location: between sensor and coil
		}
		else
		{
			// No
			// > Expected location: between coil and sensor
			// > Do nothing
		}
	}

	// Update FETController components
	UpdateTimer(time);							// Update timer
	int realFETState = realFET->Update(time);	// Update RealFET

	// Return
	if (realFETState != REALFET_STATE_OK)
	{
		return FETCONTROLLER_REALFET_ERROR;
	}
	else if(overspeedError)
	{
		return FETCONTROLLER_OVERSPEED;
	}
	else
	{
		return FETCONTROLLER_OK;
	}
}

void FETController::UpdateTimer(unsigned long time)
{
	unsigned long deltaTime = time - lastTime;
	// Any FETs to turn on?
	if (fetToTurnOn != NO_FET_POWERED)
	{
		// Yes: check and update how long to wait before turning on FET
		timeToWaitBeforeTurningOnFET -= deltaTime;

		if (timeToWaitBeforeTurningOnFET <= 0)		// Should FET be powered?
		{
			realFET->SwitchFET(fetToTurnOn, HIGH);	// Switch on FET
			fetToTurnOn = NO_FET_POWERED;			// Reset fetToTurnOn 
			timeToWaitBeforeTurningOnFET = 0;		// Reset timeToWaitBeforeTurningOnFET
		}
	}
	else
	{
		// No: any FETs powered?
		if (realFET->FETPowered != NO_FET_POWERED)
		{
			// Yes: check how long to keep the FET on
			timeToWaitBeforeTurningOffFET -= deltaTime;

			if (timeToWaitBeforeTurningOffFET <= 0)				// Should FET be switched off?
			{
				realFET->SwitchFET(realFET->FETPowered, LOW);	// Switch off FET
				timeToWaitBeforeTurningOffFET = 0;				// Reset
			}
		}
		else
		{
			// No: do nothing
		}
	}

	lastTime = time;
}

int FETController::GetEstimatedFETOnTime(int fet, double speed)
{
	// OptiLight
}

int FETController::GetNextFET(int sensor)
{
	return sensor;
}