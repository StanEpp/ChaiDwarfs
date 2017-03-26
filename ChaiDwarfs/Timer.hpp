/*
*  Copyright(c) 2016 - 2017 Stanislaw Eppinger
*  Scripting based game called ChaiDwarfs
*
*  This file is part of ChaiDwarfs.
*
*  ChaiDwarfs is free software : you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _TIMER_
#define _TIMER_

#include <chrono>

struct Timer {
  using TimerType = std::chrono::time_point<std::chrono::high_resolution_clock>;

  const int ticksPerSecond = 25;
  const int maxFrameskip = 5;
  int skippedFrames{ 0 };
  int framesRendered{ 0 };

  TimerType start;
  TimerType currTime;
  TimerType dt;
  TimerType nextGameTick;
  TimerType lastFpsUpdate;
  TimerType lastFrameRendered;

  std::chrono::milliseconds skipTicks{ 0 };

  Timer() : 
    start(currentTime()), currTime(currentTime()), dt(currentTime()),
    nextGameTick(currentTime()), lastFpsUpdate(currentTime()), lastFrameRendered(currentTime())
  {
    skipTicks = std::chrono::milliseconds(1000 / ticksPerSecond);
  }

  TimerType currentTime() { return std::chrono::high_resolution_clock::now(); }

  bool haveMilliSecondsPassed(unsigned int ms, const TimerType& timePoint) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime() - timePoint) >= std::chrono::milliseconds(ms);
  }

};

#endif