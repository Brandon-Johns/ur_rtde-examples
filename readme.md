# Sample Project to Control UR5 Robot in C++
A sample project to control a UR5 robot using the C++ library ur_rtde
- https://sdurobotics.gitlab.io/ur_rtde/

Features
- Installation instructions to entirely contain ur_rtde into a local directory (not a system install)
- Utility scripts to build and run the project
- Utility command to mode the robot to a preset 'home' position
- Sample Servoj / Speedj files with inbuilt soft-stop on CTRL+C


# Install:
These instructions assume that you have cloned this git repository to the path `/home/acrv/ur_rtde-Examples`. Where ever you see this string in the instructions, you will need to change it to your actual install path.

## Install ur_rtde
`local_lib` will hold the installation of the [C++ library ur_rtde](https://sdurobotics.gitlab.io/ur_rtde/). The installation is contained into a local directory, so that it does not interfere with any other projects on the computer.

The install instructions are in: [Install_Instructions.sh](./Install_Instructions.sh)


## Using ur_rtde
To use the local ur_rtde installation, you must adjust
- The LD_LIBRARY_PATH bash environment variable
- The paths used by CMake

Assuming that the `local_lib` is at the path `/home/acrv/ur_rtde-Examples/local_lib/`, this would look like: 

In `./bashrc`
```bash
rtdeLibDir='/home/acrv/ur_rtde-Examples/local_lib/lib'
export LD_LIBRARY_PATH=$rtdeLibDir:$LD_LIBRARY_PATH
```

In your project's cmake files
```cmake
# Specify path to cmake
set(URRTDE_PKG_DIR "/home/acrv/ur_rtde-Examples/local_lib/lib/cmake/ur_rtde" CACHE PATH "dir ur_rtde pkg")
find_package("ur_rtde" REQUIRED PATHS ${URRTDE_PKG_DIR} NO_DEFAULT_PATH)
set(URRTDE_Lib "ur_rtde::rtde")

# Link source files to output file (<exeName> <source1.c> [source2.c|.h] ...)
add_executable("MoveServoj" "MoveServoj.cpp")

# Libraries to link against
target_link_libraries("MoveServoj" PUBLIC ${URRTDE_Lib})
```

The sample project implements this for you, as described in the next section.


## Install the sample project
After installing ur_rtde, as described in the previous section:

1. Edit `./sample_project/src/CMakeLists.txt`
    - Set `BJ_PROJECT_DIR` as the path to this repository
    - Set `BJ_LIB_DIR` as the path to `./local_lib`
2. Edit `sample_project/scripts/bashrc_append.sh`
    - Set `BJ_PROJECT_DIR` likewise
3. Edit your `~/.bashrc` file as per the instructions in `./sample_project/scripts/bashrc_append.sh`


## sample_project
Test compiling and running a sample program

```bash
# Compile
bjbuild

# Run
bjready
bjrun

# Move robot to home configuration
bjready h

```




