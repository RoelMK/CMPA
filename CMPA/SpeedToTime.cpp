// 
// 
// 

#include "SpeedToTime.h"

double SpeedToTime::getFETOnTime(int FET, double speed)
{
	double distance;

	switch (FET)
	{
	case 0:
		distance = LDR0_Coil0_distance;
		break;
	case 1:
		distance = LDR1_Coil1_distance;
		break;
	case 2:
		distance = LDR2_Coil2_distance;
		break;
	case 3:
		distance = LDR3_Coil3_distance;
		break;
	default:
		distance = default_distance;
		break;
	}

	return distance / speed;
}
