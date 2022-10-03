/*
  Temperature Sensor
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
*/

#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <Arduino.h>

namespace TIMER
{
    void setISRTimer(const unsigned long hertz);
    void setISRTimer_1kH();
}

#endif