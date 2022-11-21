#!/usr/bin/bash
# Written by:		Brandon Johns
# Version created:	2022-11-21
# Last edited:		2022-11-21

# Purpose: Brandon's custom shell commands. Effectively works as if in ~/.bashrc

# To activate, add the folowing to ~/.bashrc
#	# Source my settings form another file
#	chmod +x /home/acrv/ur_rtde/sample_project/scripts/bashrc_append.sh
#	source /home/acrv/ur_rtde/sample_project/scripts/bashrc_append.sh

### NOTES ###
# aliases are garbage (something about them not recursively expanding). Use functions
# Basic function template:
#	myFunction() { myCommand "$@"; }
# Note: Use exact spacing and terminate with semicolon
# Note: (not required) The "$@" passes all arg (except $0) from myFunction to myCommand

######################################################
export EDITOR=code

bjScriptsDir='/home/acrv/ur_rtde/sample_project/scripts'
bjCMakeDir='/home/acrv/ur_rtde/sample_project'
bjLibDir='/home/acrv/ur_rtde/local_lib/lib'

# Include paths
export LD_LIBRARY_PATH=$bjLibDir:$LD_LIBRARY_PATH

# Starting Dir
cd /home/acrv/ur_rtde/sample_project/bin

# # Macros - pure
echo 'PURE: bjbuild | bjrebuild | bjready (h) | bjrun'
bjbuild() { ${bjScriptsDir}/bj_cmake_build.sh ${bjCMakeDir}; }
bjrebuild() { ${bjScriptsDir}/bj_cmake_rebuild.sh ${bjCMakeDir}; }
bjready() { ${bjCMakeDir}/bin/MoveToHome "$@"; }
bjrun() { ${bjCMakeDir}/bin/MoveSpeedj "$@"; }



