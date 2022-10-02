/*
  Temperature Sensor
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
*/

#include <DHT.h>
#include "temp.hpp"

TEMP::Sensor::Sensor(uint8_t pin, uint8_t type, DATA_BUFFER *buffer, const DATA_BUFFER *offset)
{
    this->dht = new DHT(pin, type);
    this->dht->begin();
    this->buffer = buffer;
    this->offset = offset;
}

bool TEMP::Sensor::sample()
{
    // Read Fahrenheit Temperature
    
    // Sample Temperature and Apply Offset (Interrupts MUST be enabled)
    float temp_f = dht->readTemperature(true) + offset->temp_f;

    // Modify critical data
    cli();
    buffer->temp_f = temp_f;
    sei();

    // Return true on success
    return !(isnan(buffer->temp_f));
}