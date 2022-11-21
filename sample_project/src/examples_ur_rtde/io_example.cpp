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

#include <ur_rtde/rtde_io_interface.h>
#include <ur_rtde/rtde_receive_interface.h>
#include <iostream>
#include <thread>

using namespace ur_rtde;

int main(int argc, char* argv[])
{
  RTDEIOInterface rtde_io("10.0.0.2");
  RTDEReceiveInterface rtde_receive("10.0.0.2");

  /** How-to set and get standard and tool digital outputs. Notice that we need the
    * RTDEIOInterface for setting an output and RTDEReceiveInterface for getting the state
    * of an output.
    */

  if (rtde_receive.getDigitalOutState(7))
    std::cout << "Standard digital out (7) is HIGH" << std::endl;
  else
    std::cout << "Standard digital out (7) is LOW" << std::endl;

  if (rtde_receive.getDigitalOutState(16))
    std::cout << "Tool digital out (16) is HIGH" << std::endl;
  else
    std::cout << "Tool digital out (16) is LOW" << std::endl;

  rtde_io.setStandardDigitalOut(7, true);
  rtde_io.setToolDigitalOut(0, true);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  if (rtde_receive.getDigitalOutState(7))
    std::cout << "Standard digital out (7) is HIGH" << std::endl;
  else
    std::cout << "Standard digital out (7) is LOW" << std::endl;

  if (rtde_receive.getDigitalOutState(16))
    std::cout << "Tool digital out (16) is HIGH" << std::endl;
  else
    std::cout << "Tool digital out (16) is LOW" << std::endl;

  // How to set a analog output with a specified current ratio
  rtde_io.setAnalogOutputCurrent(1, 0.25);

  return 0;
}