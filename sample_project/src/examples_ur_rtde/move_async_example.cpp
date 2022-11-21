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
#include <thread>

using namespace ur_rtde;
using namespace std::chrono;

int main(int argc, char* argv[])
{
  RTDEControlInterface rtde_control("10.0.0.2");
  RTDEReceiveInterface rtde_receive("10.0.0.2");
  std::vector<double> init_q = rtde_receive.getActualQ();

  // Target in the robot base
  std::vector<double> new_q = init_q;
  new_q[0] += 0.2;

  /**
   * Move asynchronously in joint space to new_q, we specify asynchronous behavior by setting the async parameter to
   * 'true'. Try to set the async parameter to 'false' to observe a default synchronous movement, which cannot be
   * stopped by the stopJ function due to the blocking behaviour.
   */
  rtde_control.moveJ(new_q, 1.05, 1.4, true);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  // Stop the movement before it reaches new_q
  rtde_control.stopJ(0.5);

  // Target 10 cm up in the Z-Axis of the TCP
  std::vector<double> target = rtde_receive.getActualTCPPose();
  target[2] += 0.10;

  /**
   * Move asynchronously in cartesian space to target, we specify asynchronous behavior by setting the async parameter
   * to 'true'. Try to set the async parameter to 'false' to observe a default synchronous movement, which cannot be
   * stopped by the stopL function due to the blocking behaviour.
   */
  rtde_control.moveL(target, 0.25, 0.5, true);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  // Stop the movement before it reaches target
  rtde_control.stopL(0.5);

  // Move to initial joint position with a regular moveJ
  rtde_control.moveJ(init_q);

  // Stop the RTDE control script
  rtde_control.stopScript();
  return 0;
}
