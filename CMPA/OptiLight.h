// OptiLight.h

#ifndef _OPTILIGHT_h
#define _OPTILIGHT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LightSpeed.h"
#include "OptiLightData.h"
#include "OptiCom.h"

class OptiLight
{
public:
	void Init(OptiCom *optiComPNT);						// Init OptiLight
	void Update();										// Update OptiLight
	double GetFETOnTime(int FET, double speed);			// Estimate FET power time
	double GetBackupFETOnTime(int FET, double speed);	// Estimate FET power time using data in backup
private:
	OptiCom *optiCom;									// Pointer to OptiCom
	double GetPrimitiveTimeData(int FET, double speed);	// Get primitive FET power time (using speed/distance)
	double ImproveTimingForEntranceExit(int FET, double time);	// Improve timing for entrance / exit (always check)

	bool optiComWorking;								// Is OptiCom working?
	const int speedLevels = 5;							// Number of 'speedLevels' ~ speed blocks
	const double OptiLightConstants[5][3] = { { 0.0,1.0,0.10744 },{ 1.0,2.0,0.07020 },{ 2.0,3.0,0.05495 },{ 3.0,4.0,0.04533 },{ 4.0,5.0,0.03856 } };	// Constants
	const int OPTILIGHT_VLOW = 0;						// VLow ID
	const int OPTILIGHT_VHIGH = 1;						// VHigh ID
	const int OPTILIGHT_ESTIMATED_FETOFF_TIME = 3;		// Estimated turn off time ID
	const double ArtificalTimingMultiplicationFactor = 1.58;	// Artifically increase/decrease FET on time

	// Distance between LDR and coil center
	const double FETAfterExit = 0;			// FET after entrance
	const double FETAfterEntrance = 8;		// FET after exit
	const double DEFAULT_DISTANCE_AFTER_ENTRANCEEXIT = 0.4228;	// Default distance between LDR and next coil when an entrance/exit is between
	const double DEFAULT_DISTANCE = 0.2628;						// Default distance (no entrance / exit);

	const double LDR0_DTNC = 0.4228;	// LDR0 = exit, DTNC = Distance to next coil (in m) R
	const double LDR1_DTNC = 0.2628;	// L
	const double LDR2_DTNC = 0.2628;	// L
	const double LDR3_DTNC = 0.2628;	// L
	const double LDR4_DTNC = 0.2628;	// L
	const double LDR5_DTNC = 0.2628;	// L
	const double LDR6_DTNC = 0.2628;	// L
	const double LDR7_DTNC = 0.2628;	// L
	const double LDR8_DTNC = 0.4228;	// LDR8 = entrance L
	const double LDR9_DTNC = 0.2628;	// R
	const double LDR10_DTNC = 0.2628;	// R
	const double LDR11_DTNC = 0.2628;	// R
	const double LDR12_DTNC = 0.2628;	// R
	const double LDR13_DTNC = 0.2628;	// R
	const double LDR14_DTNC = 0.2628;	// R
	const double LDR15_DTNC = 0.2628;	// R
};

#endif

