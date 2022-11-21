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
#include <ncurses.h>
#include <chrono>
#include <iostream>
#include <thread>

using namespace ur_rtde;
using namespace std::chrono;

int main(int argc, char* argv[])
{
  RTDEControlInterface rtde_control("10.0.0.2");

  // Curses Initialisations
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  timeout(10);

  // Parameters
  double speed_magnitude = 0.15;
  std::vector<double> speed_vector = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  rtde_control.jogStart(speed_vector, RTDEControlInterface::FEATURE_TOOL);

  std::string instructions("[ Use arrow keys to control the robot, to exit press 'q' ]");
  int c, row, col;
  getmaxyx(stdscr, row, col);
  mvprintw(row / 2, (col-strlen(instructions.c_str())) / 2, "%s", instructions.c_str());

  while ((c = getch()) != 'q')
  {
    c = getch();
    switch (c)
    {
      case KEY_UP:
        speed_vector = {0.0, 0.0, -speed_magnitude, 0.0, 0.0, 0.0};
        rtde_control.jogStart(speed_vector, RTDEControlInterface::FEATURE_TOOL);
        break;
      case KEY_DOWN:
        speed_vector = {0.0, 0.0, speed_magnitude, 0.0, 0.0, 0.0};
        rtde_control.jogStart(speed_vector, RTDEControlInterface::FEATURE_TOOL);
        break;
      case KEY_LEFT:
        speed_vector = {speed_magnitude, 0.0, 0.0, 0.0, 0.0, 0.0};
        rtde_control.jogStart(speed_vector, RTDEControlInterface::FEATURE_TOOL);
        break;
      case KEY_RIGHT:
        speed_vector = {-speed_magnitude, 0.0, 0.0, 0.0, 0.0, 0.0};
        rtde_control.jogStart(speed_vector, RTDEControlInterface::FEATURE_TOOL);
        break;
      default:
        speed_vector = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        rtde_control.jogStart(speed_vector, RTDEControlInterface::FEATURE_TOOL);
        break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  endwin();
  rtde_control.jogStop();
  rtde_control.stopScript();

  return 0;
}
