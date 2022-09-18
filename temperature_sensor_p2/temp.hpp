
#ifndef __TEMP_HPP__
#define __TEMP_HPP__

#include <Arduino.h>
#include <DHT.h>

namespace TEMP
{
    class Sensor
    {
        public:
        Sensor();



        private:
        DHT* dht;

    };
}

#endif