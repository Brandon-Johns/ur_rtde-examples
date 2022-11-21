#pragma once
// Allow handling program termination
#include <bits/stdint-uintn.h>
#include <cstdlib>
#include <csignal>

namespace BJ_Kill
{
	extern uint8_t Flag_TerminateProgramCalled;
	extern void ProgramTerminationHandler(int signal);
	extern void ProgramTerminationEnable();
}

