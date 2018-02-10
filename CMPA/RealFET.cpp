// 
// 
// 

#include "RealFET.h"

void RealFET::Init()
{
	// Set state
	for (int f = 0; f < FETCount; f++)
	{
		FETState[f] = LOW;
	}
	LastSwitchFET = NO_FET_POWERED;
	FETPowered = NO_FET_POWERED;
	SwitchFETTries = 0;
	State = REALFET_STATE_OK;

	// Set pins
	for (int i = FirstFETpin; i <= LastFETpin; i++)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}
}

int RealFET::Update(unsigned long time)
{
	if (FETPowered != NO_FET_POWERED)
	{
		// Check time FET is on
		timeFETOn = timeFETOn + (time - lastTime);
		if (timeFETOn > MAXIMAL_TIME_FET_ON)
		{
			Serial.println("[ERROR] REALFET_STATE_POWER_FAILURE");
			State = REALFET_STATE_POWER_FAILURE;	// Report critical error
			SwitchFET(LastSwitchFET, LOW);			// Turn off FET
		}
	}
	else
	{
		timeFETOn = 0;
	}
	lastTime = time;
	return State;
}

bool RealFET::SwitchFET(int fet, uint8_t state)
{
	if (state == HIGH)
	{
		// Check 1: FET tries
		if (fet == LastSwitchFET)
		{
			SwitchFETTries++;
		}
		else
		{
			if (SwitchFETTries > 0) SwitchFETTries--;
		}
		if (SwitchFETTries > MAXIMAL_FETSWITCH_TRIES)
		{
			// CRITICAL ERROR: too many times same FET in SwitchFET
			Serial.println("[ERROR] REALFET_STATE_FETSWITCH_ERROR");
			State = REALFET_STATE_FETSWITCH_ERROR;
			return false;
		}
		LastSwitchFET = fet;

		// Check 2: are any FETs powered?
		for (int f = 0; f < FETCount; f++)
		{
			if (FETState[f] == HIGH)
			{
				return false;	// Failed to turn on FET: another FET is already turned on
			}
		}

		// Turn on, update variables
		FETPowered = fet;
		SwitchPIN(fet, HIGH);
	}
	else if(state == LOW)
	{
		// Turn off
		SwitchPIN(fet, LOW);
		FETPowered = NO_FET_POWERED;
	}
}

void RealFET::SwitchPIN(int fet, uint8_t state)
{
	if (state == HIGH)
	{
		Serial.print("Powering FET: ");
		Serial.println(fet);
	}
	
	FETState[fet] = state;
	if(!NO_FET_SWITCHING) digitalWrite(fet + FirstFETpin, state);
}

void RealFET::Panic()
{
	for (int f = 0; f < FETCount; f++)
	{
		SwitchPIN(f, LOW);
	}
	State = REALFET_STATE_PANIC;
}