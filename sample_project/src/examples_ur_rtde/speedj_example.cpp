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
#include <thread>
#include <chrono>

using namespace ur_rtde;
using namespace std::chrono; 

int main(int argc, char* argv[])
{
  RTDEControlInterface rtde_control("10.0.0.2");

  // Parameters
  double acceleration = 0.5;
  double dt = 1.0/500; // 2ms
  std::vector<double> joint_q = {-1.54, -1.83, -2.28, -0.59, 1.60, 0.023};
  std::vector<double> joint_speed = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  // Move to initial joint position with a regular moveJ
  rtde_control.moveJ(joint_q);

  // Execute 500Hz control loop for 2 seconds, each cycle is ~2ms
  for (unsigned int i=0; i<1000; i++)
  {
    steady_clock::time_point t_start = rtde_control.initPeriod();
    rtde_control.speedJ(joint_speed, acceleration, dt);
    joint_speed[0] += 0.0005;
    joint_speed[1] += 0.0005;
    rtde_control.waitPeriod(t_start);
  }

  rtde_control.speedStop();
  rtde_control.stopScript();

  return 0;
}
