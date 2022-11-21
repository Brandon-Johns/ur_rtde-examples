/*
Written by:			Brandon Johns
Version created:	2022-11-21
Last edited:		2022-11-21

Purpose:
	Move UR5 robot on specified trajectory
	Using Speedj

Call from Bash:
MoveSpeedj

*/

#include <thread>
#include <chrono>
#include <cmath>

// Allow handling program termination
#include "BJ_Kill.h"

// UR5 Control
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>
namespace rtde = ur_rtde;


// Enum the joint names to improve readability & prevent errors
namespace {
	enum UR5_JointName
	{
		Base = 0,
		Shoulder = 1,
		Elbow = 2,
		Wrist1 = 4,
		Wrist2 = 5,
		Wrist3 = 6,
	};
	UR5_JointName base = Base;
	UR5_JointName shdr = Shoulder;
	UR5_JointName ebow = Elbow;
	UR5_JointName wst1 = Wrist1;
	UR5_JointName wst2 = Wrist2;
	UR5_JointName wst3 = Wrist3;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Main
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char* argv[])
{
	// Register program termination routine for Event: CTRL+C
	BJ_Kill::ProgramTerminationEnable();
	
	// Register UR5 controller
	rtde::RTDEControlInterface rtde_control("10.0.0.2");
	rtde::RTDEReceiveInterface rtde_receive("10.0.0.2");

	// Start Position [rad]
	std::vector<double> q_start = {-M_PI_2, -M_PI_2, -M_PI_2, 0, M_PI_2, 0};

	// Time [seconds]
	double dt = 1.0/125; // 125 Hz
	double t_now = 0;
	double t_max = 3;

	// Controlled param [rad/s, rad/s^2]
	std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	double acceleration = 10;
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Start
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Move to initial position
	rtde_control.moveJ(q_start);

	// Control loop
	for(t_now=0; t_now<t_max; t_now+=dt)
	{
		// Check program termination flag (set on first call to CTRL+C)
		if(BJ_Kill::Flag_TerminateProgramCalled) { rtde_control.stopScript(); return 0; }

		// Start timer - cycle duration
		auto t_start = std::chrono::high_resolution_clock::now();

		// Control
		rtde_control.speedJ(qd, acceleration, dt);
		qd[base] += 0.001;
		qd[ebow] += 0.002;

		// Wait to end of cycle
		auto t_stop = std::chrono::high_resolution_clock::now();
		auto t_duration = std::chrono::duration<double>(t_stop - t_start);
		if (t_duration.count() < dt)
		{
			std::this_thread::sleep_for(std::chrono::duration<double>(dt - t_duration.count()));
		}
	}
	rtde_control.speedStop();

	// Query the current pose of the UR5
	std::vector<double> q_actual = rtde_receive.getActualQ();
	std::vector<double> qd_actual = rtde_receive.getActualQd();

	std::cout << "Commanded Postion: " << std::endl;
	std::cout << "NA" << std::endl;
	// std::cout << "Base: "  << q[base] << std::endl;
	// std::cout << "Elbow: " << q[ebow] << std::endl;
	std::cout << "Actual Postion: " << std::endl;
	std::cout << "Base: "  << q_actual[base] << std::endl;
	std::cout << "Elbow: " << q_actual[ebow] << std::endl;

	std::cout << "Commanded Velocity: " << std::endl;
	std::cout << "Base: "  << qd[base] << std::endl;
	std::cout << "Elbow: " << qd[ebow] << std::endl;
	std::cout << "Actual Velocity: " << std::endl;
	std::cout << "Base: "  << qd_actual[base] << std::endl;
	std::cout << "Elbow: " << qd_actual[ebow] << std::endl;


	rtde_control.stopScript();
	return 0;
}
