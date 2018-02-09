#ifndef _CONSTANTS_h
#define _CONSTANTS_h

// Developer options
const bool NO_FET_SWITCHING = false;

// LightSpeed
const int CALIBRATION_OK = 1;				// Return: calibration OK

// Sensors
const int SensorCount = 16;					// Begin analog A0 (1=A0, 2=A0,A1)
const int NO_SENSOR_DETECTION = -1;			// SensorDetection: none of the sensors has detected a projectile
const int LDR_SENSOR_OK = -1;				// Sensors OK
const int LDR_SENSOR_FAILURE = -2;			// Sensors not OK 

// FETController
const int FETCONTROLLER_OK = 0;
const int FETCONTROLLER_REALFET_ERROR = 1;
const int FETCONTROLLER_OVERSPEED = 2;

// RealFET
const int FETCount = 16;					// Number of FETs
const int NO_FET_POWERED = -1;				// FETPowered / LastSwitchFET: no FET on
const int MAXIMAL_TIME_FET_ON = 1000;		// Max time a FET is turned on (in ms)

const int REALFET_STATE_PANIC = 0;				// Panic: turn off always FETs
const int REALFET_STATE_OK = 1;					// OK: everything is fine
const int REALFET_STATE_FETSWITCH_ERROR = 2;	// SwitchFETTries > MAXIMAL_FETSWITCH_TRIES --> Critical error!
const int REALFET_STATE_POWER_FAILURE = 3;		// timeFETOn > MAXIMAL_TIME_FET_ON --> Critical error!

#endif