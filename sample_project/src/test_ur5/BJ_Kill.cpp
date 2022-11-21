#include "BJ_Kill.h"

namespace BJ_Kill
{
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Program termination routine
	// Called on abnormal program termination
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// This flag should be frequently checked in the main program
	// 1 = The program should be safely stopped ASAP
	uint8_t Flag_TerminateProgramCalled = 0;

	// This may be called at ANY point during execution
	// => it is unsafe to do anything in here other than set a flag or force terminate
	void ProgramTerminationHandler(int signal)
	{
		if(Flag_TerminateProgramCalled)
		{
			// 2nd call to terminate => force terminate now
			exit(signal);
		}
		else
		{
			// First call to terminate
			// Set flag to tell main program to safely exit
			Flag_TerminateProgramCalled = 1;
		}
	}

	// Call this in main to enable
	void ProgramTerminationEnable()
	{
		// Register program termination routine for Event: CTRL+C
		signal(SIGINT, BJ_Kill::ProgramTerminationHandler);
	}

}
