# CMakeList.txt : Top-level CMake project file
#	Global configuration and include sub-projects
cmake_minimum_required (VERSION 3.10)
project ("ur_rtde_sample_project")

# Written by:		Brandon Johns
# Version created:	2022-11-21
# Last edited:		2023-11-23
# Purpose:			Sample project to use the ur_rtde library

####################################################################################
# General options
##########################################
# Not required, but why not
set(CMAKE_CXX_STANDARD 20)

# Compiler flags
# Could split between debug and release versions... not nessesary rn
#	More compiler warnings: -Wall -Wextra
#	Add debugging symbols to executable: -g
#	Opimise: -O3
#	Disable assertions: -DNDEBUG
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# This won't seem to do much... but if it ever causes warnings and you're completely lost
# Then it may be just the thing that saves you
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wabi-tag")

# For VSCode extention "clangd"
#	CMAKE_EXPORT_COMPILE_COMMANDS generates the "compile_commands.json" file in the build dir
#	Reload the "clangd" extension to make it find the file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

####################################################################################
# Host machine dependent
##########################################
# Path to library and project
set(BJ_PROJECT_DIR "/home/acrv/ur_rtde-Examples/sample_project")

# Paths to: Libraries, Includes (library header files), Packages
#	Root directory of locally installed libraries
set(BJ_LIB_DIR "/home/acrv/ur_rtde-Examples/local_lib")
#	ur_rtde
set(URRTDE_PKG_DIR "${BJ_LIB_DIR}/lib/cmake/ur_rtde" CACHE PATH "dir ur_rtde pkg")
#	OpenBLAS
#set(OPENBLAS_INC_DIR "" CACHE PATH "dirs OpenBLAS h") # Not used, I think
#set(OPENBLAS_LIB_DIR "" CACHE PATH "dir OpenBLAS lib") # It can find itself in the find_library() call

# Path for output exe
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BJ_PROJECT_DIR}/bin")


# SECTION DISABLED
#	It's not really nessesary for general users.
#	Uncomment if you need it
# If this triggers, then change the settings in this section to match your project
#	If you want to run this project on multiple computers,
#	then rewrite this section to test the computer you're on and set the correct variables

#cmake_host_system_information(RESULT HOSTNAME_THIS_PC QUERY HOSTNAME)
#if(NOT ("${HOSTNAME_THIS_PC}" STREQUAL "acrv-ubuntu-18"))
#	message("ERROR: Running on a different computer than this project was setup for.")
#	message("NOTE: Hostname for this comuter: ${HOSTNAME_THIS_PC}")
#	message(FATAL_ERROR "Change the settings in the root CMakeLists.txt file!")

#elseif(NOT ("${CMAKE_SOURCE_DIR}" STREQUAL "${BJ_PROJECT_DIR}/src"))
#	message("ERROR: Running in a different project directory than this project was setup for.")
#	message("NOTE: Directory of root CMakeLists.txt file: ${CMAKE_SOURCE_DIR}")
#	message(FATAL_ERROR "Change the settings in the root CMakeLists.txt file!")
#endif()

####################################################################################
# Find all libraries
##########################################
# Settings for find_library()
#	This setting assumes that for a library "x"
#		Windows filename: x.lib
#		Windows filename: x.dll (shared library)
#		Linux filename: libx.a
#		Linux filename: libx.so (shared library)
#	Well, apparrently the person who compiled OpenBLAS on Windows decided to do their own thing T_T
if(WIN32)
	set(CMAKE_FIND_LIBRARY_PREFIXES ""  CACHE PATH "CMAKE_FIND_LIBRARY_PREFIXES")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" CACHE PATH "CMAKE_FIND_LIBRARY_SUFFIXES")
else() # Ubuntu
	set(CMAKE_FIND_LIBRARY_PREFIXES "lib"  CACHE PATH "CMAKE_FIND_LIBRARY_PREFIXES")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a" CACHE PATH "CMAKE_FIND_LIBRARY_SUFFIXES")
endif()

# Find libraries (<varName> <fileName> <filePath> DOC <docstring>)
find_library(OPENBLAS_LIB openblas ${OPENBLAS_LIB_DIR} DOC "openblas lib")

####################################################################################
# Find all packages
##########################################
# Specify path because I intalled it in a different location
find_package("ur_rtde" REQUIRED PATHS ${URRTDE_PKG_DIR} NO_DEFAULT_PATH)
set(URRTDE_Lib "ur_rtde::rtde") # Some black magic

# Apparently it can find itself... at least on the ur5 computer
find_package("Armadillo" REQUIRED)
set(ARMADILLO_Lib ${ARMADILLO_LIBRARY})
set(ARMADILLO_INC_DIR ${ARMADILLO_INCLUDE_DIR})
add_definitions(-DARMA_DONT_USE_WRAPPER) # Something about armadillo and including openblas

find_package(Threads REQUIRED)
set(Threads_Lib "Threads::Threads") # Some black magic

####################################################################################
# Projects to build
##########################################
# Include sub-projects (folders with other CMakeList.txt files)
add_subdirectory ("examples_armadillo")
add_subdirectory ("examples_ur_rtde")
add_subdirectory ("test_ur5")


