#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <Arduino.h>

#define DEBUG 0

namespace DRIVER
{
    bool input(const int limit, char *message, int *length);
    void output(uint8_t led_pin, char *message, size_t length, unsigned long unit_time = 75, bool enable_tone = false);
}

#endif