exit 1 # This file is not a script (This line safeguards in case it is ever called as a script)

# Written by:		Brandon Johns
# Version created:	2022-11-21
# Last edited:		2023-11-23

# Purpose:
#   Install C++ library ur_rtde: https://sdurobotics.gitlab.io/ur_rtde/
#   The installation is contained into a local directory, to not mess with the rest of the computer

# VERSION:
#   This install is not compatible with the Vicon DataStreamSDK (for the motion capture)
#   Instead, it installs with the new C++11 String ABI, which is more compatible with other modern C++ libraries
#   Refer to: https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html

# TO USE:
#   Do not run this as a script
#   Follow the instructions in order and copy paste the relevant commands into the bash shell


####################################################################################################################################
####################################################################################################################################
####################################################################################################################################
# Installation
####################################################################################################################################
# Install basic utils & dependencies
#################################
sudo apt-get update
sudo apt install git -y

### Dependancies of boost
sudo apt-get install python-dev autotools-dev libicu-dev libbz2-dev libopenblas-dev -y

# Also install a new version of CMake from
# https://cmake.org/download/
# The apt-get version is ancient
# NOTE: I've already done this for the UR5 control computer


##################################################################
# Setup this repository
#################################
# Clone this repository with git

# Set bash variable the root of this repository
# Change as required, such that this variable is the correct path
ProjectDir=${HOME}'/ur_rtde-examples'


##################################################################
# Setup directory to install into
#################################
### Install dir
myLibDir=${ProjectDir}'/local_lib'

# These are required in the installation process
mkdir -p ${myLibDir}/src


##################################################################
# Install Boost
#	https://stackoverflow.com/a/41272796
#################################
cd ${myLibDir}/src

### Download
wget "https://sourceforge.net/projects/boost/files/boost/1.80.0/boost_1_80_0.tar.gz"
tar xzvf boost_1_80_0.tar.gz


### Pre-install
cd boost_1_80_0/
./bootstrap.sh --prefix=$myLibDir --includedir=include --libdir=lib

n=`cat /proc/cpuinfo | grep "cpu cores" | uniq | awk '{print $NF}'`
echo "Num CPU Cores: $n"

### Install 
#   Check installation options with
#	    ./b2 --help
#   Check libraries being build with the flag --show-libraries
#   Then remove the --show-libraries flag and run again to perform the install
sudo ./b2 install -j$n -a --with=all --layout=tagged threading=multi --prefix=$myLibDir --show-libraries

##################################################################
# Install ur_rtde
#################################
cd ${myLibDir}/src

### Download
git clone https://gitlab.com/sdurobotics/ur_rtde.git
cd ur_rtde
git checkout tags/v1.5.4

git submodule update --init --recursive

### Build
#   DESTDIR doesn't seem to do anything but oh well
mkdir build
cd build
cmake -DPYTHON_BINDINGS:BOOL=OFF -DCMAKE_INSTALL_PREFIX=$myLibDir -DCMAKE_PREFIX_PATH=$myLibDir DESTDIR=$myLibDir ..
make

### Install
sudo make install
### ACTION: (To fix paths so it works from my custom install dir)
#   NOTE:
#       In performing the following edits, you will need to change `/home/acrv/ur_rtde-examples/local_lib`
#       to the where you have installed $myLibDir
#   Edit: ${myLibDir}/lib/cmake/ur_rtde/ur_rtdeBuildConfig.cmake
#       Change line 3
#           FROM:   set(RTDE_BOOST_LIBRARY_DIR )
#           TO:     set(RTDE_BOOST_LIBRARY_DIR /home/acrv/ur_rtde-examples/local_lib/lib )
#   Edit: ${myLibDir}/lib/cmake/ur_rtde/ur_rtdeConfig.cmake
#       Add lines just before last endif() in the file
#           # BJ:START Added
#           else()
#             find_package(Boost REQUIRED COMPONENTS system thread PATHS "/home/acrv/ur_rtde-examples/local_lib/lib/cmake/Boost-1.80.0")
#           # BJ:END Added
#           endif() # BJ: This is the last line in the file


##################################################################
# Finishing up
#################################
cd ${myLibDir}

# Give everything full permissions & set user to self
#   Check current permissions & owner/group with: ls -ld
#   UR5 control computer is "arcv:arcv"
sudo find include -exec chown acrv:acrv {} \;
sudo find lib -exec chown acrv:acrv {} \;


##################################################################
# Install Armadillo (Not a local install)
#################################
### ACTION:
#	Download latest stable version from: http://arma.sourceforge.net/download.html
#	Unzip
#	cd into unzipped dir
cmake . -DCMAKE_INSTALL_PREFIX:PATH=/usr/local
sudo make install


####################################################################################################################################
####################################################################################################################################
####################################################################################################################################
# Uninstalling / Resetting between subsequent compilations
####################################################################################################################################
# Boost
#################################
### Uninstall
sudo rm -rf ${myLibDir}/include/boost # Dir
sudo rm -rf ${myLibDir}/lib/libboost_* # Total 95 files starting with "libboost_"
sudo rm -rf ${myLibDir}/lib/cmake/Boost* # Many dirs
sudo rm -rf ${myLibDir}/lib/cmake/boost_* # Many dirs

### Reset before recompiling with b2
cd ${myLibDir}/src/boost_1_80_0/
sudo ./b2 --clean
### ACTION:
#	Also follow uninstall instructions

### Reset completely
### ACTION:
#	Nuke boost src dir from orbit
#	Also follow uninstall instructions


##################################################################
# ur_rtde
#################################
### Uninstall
rm -rf ${myLibDir}/include/urcl # Dir
rm -rf ${myLibDir}/include/ur_rtde # Dir
rm -rf ${myLibDir}/lib/librtde.so* # Total 3 files
rm -rf ${myLibDir}/lib/cmake/ur_rtde # Dir

### Reset before recompiling
cd ${myLibDir}/src/ur_rtde
rm -rf build
mkdir build
cd build
### ACTION:
#	Also follow uninstall instructions


##################################################################
# Armadillo
#################################
### To uninstall
sudo rm -rf /usr/local/include/armadillo_bits # Dir
sudo rm -rf /usr/local/include/armadillo # 1 file
sudo rm -rf /usr/local/lib/libarmadillo.so* # Total 3 files
sudo rm -rf /usr/local/share/Armadillo # Dir
sudo rm -rf /usr/local/lib/pkgconfig/armadillo.pc # 1 file

### To reset before recompiling
rm CMakeCache.txt



