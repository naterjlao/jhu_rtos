/*
  Temperature Sensor
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
*/

#include <DHT.h>
#include "weather_sensor.hpp"

WEATHER::Sensor::Sensor(uint8_t pin, uint8_t type, DATA_BUFFER *buffer, const DATA_BUFFER *offset)
{
    this->dht = new DHT(pin, type);
    this->dht->begin();
    this->buffer = buffer;
    this->offset = offset;
}

bool WEATHER::Sensor::sample()
{
    
    // Poll Sensor Data (Interrupts MUST be ENABLED)

    // ----- Temperature Degrees Fahrenheit ----- //
    float temp_f = dht->readTemperature(true) + offset->temp_f;

    // ----- Humidity Percentage ----- //
    float humidity = dht->readHumidity(true) + offset->humidity;

    // Copy to shared data atomically
    cli();
    buffer->temp_f = temp_f;
    buffer->humidity = humidity;
    sei();

    // Return false on bad readings, true otherwise.
    return !(isnan(buffer->temp_f) || isnan(buffer->humidity));
}