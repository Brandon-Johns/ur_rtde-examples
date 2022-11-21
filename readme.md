This directory contains
- local_lib:      The installation of the C++ library ur_rtde
- sample_project: A sample project to use this library

### Note
If you downloaded this from GitHub, you get to install `local_lib` yourself

Follow the instructions `Install Instructions.sh`


# local_lib
Installation of the C++ library ur_rtde
https://sdurobotics.gitlab.io/ur_rtde/

The installation is contained into a local directory, so that it does not mess with the rest of the projects on the computer
As such, to use this installation, you must adjust
- The LD_LIBRARY_PATH bash environment variable
- The paths used by CMake

```bash
# Specify path to bash
# Copy these lines into your .bashrc file so that the path is included in every terminal you open
rtdeLibDir='/home/acrv/ur_rtde/local_lib/lib'
export LD_LIBRARY_PATH=$rtdeLibDir:$LD_LIBRARY_PATH
```

```cmake
# Specify path to cmake
set(URRTDE_PKG_DIR "/home/acrv/ur_rtde/local_lib/lib/cmake/ur_rtde" CACHE PATH "dir ur_rtde pkg")
find_package("ur_rtde" REQUIRED PATHS ${URRTDE_PKG_DIR} NO_DEFAULT_PATH)
set(URRTDE_Lib "ur_rtde::rtde")

# Link source files to output file (<exeName> <source1.c> [source2.c|.h] ...)
add_executable("MoveServoj" "MoveServoj.cpp")

# Libraries to link against
target_link_libraries("MoveServoj" PUBLIC ${URRTDE_Lib})
```


# sample_project
The sample project implements the above requirements to use the local install.

INTENDED USE
Copy the whole `sample_project` folder somewhere else
- Edit `./sample_project/src/CMakeLists.txt`
    - Set `BJ_PROJECT_DIR` to the new path
- Edit `sample_project/scripts/bashrc_append.sh`
    - Set the following variables to match the new path
    - `bjScriptsDir`
    - `bjCMakeDir`
    - Do not change `bjLibDir`. Leave it pointing to the installation in this folder
- Edit `/home/acrv/.bashrc` as per the instructions in `sample_project/scripts/bashrc_append.sh`
- Test compiling and running a sample program

```bash
# Compile
bjbuild

# Run
./MoveServoj
```




