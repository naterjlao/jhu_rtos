#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <Arduino.h>

namespace DRIVER
{
    const unsigned long UNIT_TIME = 75;

    void initialize(uint8_t led_pin);
    void main(uint8_t led_pin);
}

#endif