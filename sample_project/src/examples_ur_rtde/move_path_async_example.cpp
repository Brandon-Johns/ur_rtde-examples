/*
MIT License

Copyright (c) 2019 Anders Prier Lindvig - SDU Robotics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// Original file: https://gitlab.com/sdurobotics/ur_rtde/-/tree/v1.5.4/examples/cpp
// This file: Modified to work with our robot the Monash HRI Lab

#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>

#include <chrono>
#include <iostream>
#include <thread>

using namespace ur_rtde;
using namespace std::chrono;

int main(int argc, char* argv[])
{
  std::string hostname = "10.0.0.2";
  RTDEControlInterface rtde_control(hostname);
  RTDEReceiveInterface rtde_receive(hostname);

  ur_rtde::Path path;
  double velocity = 0.5;
  double acceleration = 4;
  path.addEntry({PathEntry::MoveJ,
                 PathEntry::PositionTcpPose,
                 {-0.140, -0.400, 0.100, 0, 3.14, 0, velocity, acceleration,
                  0}});  // move to initial position using movej with inverse kinematics
  path.addEntry({PathEntry::MoveL,
                 PathEntry::PositionTcpPose,
                 {-0.140, -0.400, 0.300, 0, 3.14, 0, velocity, acceleration, 0.099}});
  path.addEntry({PathEntry::MoveL,
                 PathEntry::PositionTcpPose,
                 {-0.140, -0.600, 0.300, 0, 3.14, 0, velocity, acceleration, 0.099}});
  path.addEntry({PathEntry::MoveL,
                 PathEntry::PositionTcpPose,
                 {-0.140, -0.600, 0.100, 0, 3.14, 0, velocity, acceleration, 0.099}});
  path.addEntry({PathEntry::MoveL,
                 PathEntry::PositionTcpPose,
                 {-0.140, -0.400, 0.100, 0, 3.14, 0, velocity, acceleration, 0}});

  // First move given path synchronously
  std::cout << "Move path synchronously..." << std::endl;
  rtde_control.movePath(path, false);
  std::cout << "Path finished...\n\n" << std::endl;

  // Now move given path asynchronously
  std::cout << "Move path asynchronously with progress feedback..." << std::endl;
  rtde_control.movePath(path, true);
  // Wait for start of asynchronous operation
  while (rtde_control.getAsyncOperationProgress() < 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  std::cout << "Async path started.. " << std::endl;
  // Wait for end of asynchronous operation
  int waypoint = -1;
  while (rtde_control.getAsyncOperationProgress() >= 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    int new_waypoint = rtde_control.getAsyncOperationProgress();
    if (new_waypoint != waypoint)
    {
      waypoint = new_waypoint;
      std::cout << "Moving to path waypoint " << waypoint << std::endl;
    }
  }
  std::cout << "Async path finished...\n\n" << std::endl;

  rtde_control.stopScript();
  return 0;
}
