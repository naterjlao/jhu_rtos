
#ifndef __TEMP_HPP__
#define __TEMP_HPP__

#include <Arduino.h>
#include <DHT.h>

namespace TEMP
{
    typedef struct data_buffer
    {
        float temp_f;
    } DATA_BUFFER;

    class Sensor
    {
    public:
        Sensor(uint8_t pin, uint8_t type, DATA_BUFFER *buffer, const DATA_BUFFER *offset);
        bool sample();

    private:
        DHT *dht;
        DATA_BUFFER *buffer;
        const DATA_BUFFER *offset;
    };
}

#endif