/*
Written by:			Brandon Johns
Version created:	2021-07-23
Last edited:		2021-07-23

Purpose:	Move UR5 robot to a saved position

Call from Bash:
MoveToHome
MoveToHome h

*/

#include <cstdint>
#include <cstring>
#include <cmath>

#include <ur_rtde/rtde_control_interface.h>
namespace rtde = ur_rtde;

int main(int argc, char* argv[])
{
	// Input options
	std::vector<double> q_start; // Start position [rad]
	if (argc==2 && std::strcmp(argv[1], "h")==0)
	{
		// Home position
		q_start = {0, -M_PI_2, 0, -M_PI_2, 0, 0};
	}
	else // Default
	{
		// Ready position
		// q_start = {-M_PI, -M_PI_2, -M_PI_2, 0, M_PI_2, 0}; // Bend at elbow
		q_start = {-M_PI, -M_PI, 0, 0, M_PI_2, 0}; // Bend at shoulder
	}

	// Initialise controller
	rtde::RTDEControlInterface rtde_control("10.0.0.2");

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Start
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Move to start position
	rtde_control.moveJ(q_start);

    // End
	rtde_control.stopScript();

	return 0;
}
