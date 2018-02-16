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
	const int speedLevels = 50;							// Number of 'speedLevels' ~ speed blocks
	const double OptiLightConstants[50][3] = {{0.0,0.1,0.48672},{0.1,0.2,0.21473},{0.2,0.3,0.15864},{0.3,0.4,0.13348},{0.4,0.5,0.11870},{0.5,0.6,0.10867},{0.6,0.7,0.10124},{0.7,0.8,0.09540},{0.8,0.9,0.09062},{0.9,1.0,0.08656},{1.0,1.1,0.08306},{1.1,1.2,0.07996},{1.2,1.3,0.07719},{1.3,1.4,0.07469},{1.4,1.5,0.07240},{1.5,1.6,0.07028},{1.6,1.7,0.06832},{1.7,1.8,0.06650},{1.8,1.9,0.06480},{1.9,2.0,0.06319},{2.0,2.1,0.06166},{2.1,2.2,0.06024},{2.2,2.3,0.05888},{2.3,2.4,0.05758},{2.4,2.5,0.05635},{2.5,2.6,0.05517},{2.6,2.7,0.05405},{2.7,2.8,0.05297},{2.8,2.9,0.05194},{2.9,3.0,0.05095},{3.0,3.1,0.04999},{3.1,3.2,0.04908},{3.2,3.3,0.04819},{3.3,3.4,0.04733},{3.4,3.5,0.04651},{3.5,3.6,0.04572},{3.6,3.7,0.04495},{3.7,3.8,0.04420},{3.8,3.9,0.04348},{3.9,4.0,0.04278},{4.0,4.1,0.04210},{4.1,4.2,0.04146},{4.2,4.3,0.04081},{4.3,4.4,0.04019},{4.4,4.5,0.03959},{4.5,4.6,0.03901},{4.6,4.7,0.03844},{4.7,4.8,0.03788},{4.8,4.9,0.03735},{4.9,5.0,0.03682} };	// Constants
	const int OPTILIGHT_VLOW = 0;						// VLow ID
	const int OPTILIGHT_VHIGH = 1;						// VHigh ID
	const int OPTILIGHT_ESTIMATED_FETOFF_TIME = 2;		// Estimated turn off time ID

	// Distance between LDR and coil center
	const double FETAfterExit = 0;			// FET after entrance
	const double FETAfterEntrance = 8;		// FET after exit
	const double DEFAULT_DISTANCE_AFTER_ENTRANCEEXIT = 0.4228;	// Default distance between LDR and next coil when an entrance/exit is between
	const double DEFAULT_DISTANCE = 0.2628;						// Default distance (no entrance / exit);

	const double LDR0_DTNC = 0.4228;	// LDR0 = next to exit, DTNC = Distance to next coil (in m) L
	const double LDR1_DTNC = 0.2628;	// L
	const double LDR2_DTNC = 0.2628;	// L
	const double LDR3_DTNC = 0.2628;	// L
	const double LDR4_DTNC = 0.2628;	// L
	const double LDR5_DTNC = 0.2628;	// L
	const double LDR6_DTNC = 0.2628;	// L
	const double LDR7_DTNC = 0.2628;	// L
	const double LDR8_DTNC = 0.4228;	// LDR8 = next to entrance R
	const double LDR9_DTNC = 0.2628;	// R
	const double LDR10_DTNC = 0.2628;	// R
	const double LDR11_DTNC = 0.2628;	// R
	const double LDR12_DTNC = 0.2628;	// R
	const double LDR13_DTNC = 0.2628;	// R
	const double LDR14_DTNC = 0.2628;	// R
	const double LDR15_DTNC = 0.2628;	// R

	// ARTM: Artifical Timing Multiplication Factor = correction of CMPAModelling data
	double GetArtificalTimingMultiplicationFactor (int FET, double modelData);
	const double DEFAULT_ARTM = 2.5;
	const bool ARTMF_FET_ENABLED[FETCount] = {false,false,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false };
	const double ARTMF_FET[FETCount] = { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 };

	//const double FET0_ARTM = 1.0;	// L
	//const double FET1_ARTM = 1.0;	// L
	//const double FET2_ARTM = 1.0;	// L
	//const double FET3_ARTM = 1.0;	// L
	//const double FET4_ARTM = 1.0;	// L
	//const double FET5_ARTM = 1.0;	// L
	//const double FET6_ARTM = 1.0;	// L
	//const double FET7_ARTM = 1.0;	// L
	//const double FET8_ARTM = 1.0;	// R
	//const double FET9_ARTM = 1.0;	// R
	//const double FET10_ARTM = 1.0;	// R
	//const double FET11_ARTM = 1.0;	// R
	//const double FET12_ARTM = 1.0;	// R
	//const double FET13_ARTM = 1.0;	// R
	//const double FET14_ARTM = 1.0;	// R
	//const double FET15_ARTM = 1.0;	// R
};

#endif

