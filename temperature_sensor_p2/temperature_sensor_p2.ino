// https://electronoobs.com/eng_arduino_tut12.php

#include <DHT.h>

#include "isr_timer.hpp"
#include "temp.hpp"

// Sampling Variables
const unsigned long INTERVAL = 1;
volatile unsigned long time;
volatile bool trigger;
volatile TEMP::DATA_BUFFER sample;

// Temperature Sensor
const uint8_t DHTPIN = 2;
const uint8_t DHTTYPE = DHT11;
const TEMP::DATA_BUFFER sensor_offset = {-1.66};
TEMP::DATA_BUFFER sensor_buffer;
TEMP::Sensor *sensor = 0;

void setup()
{
    // Initialize Interrupt Variables
    time = 0;
    trigger = false;
    sample.temp_f = 0.0;

    // Setup 1 Hz Timer Interrupt
    cli(); // Disable Interupts
    TIMER::setISRTimer(1);
    sei();  // Enable Interrupts

    // Initialize Temperature Sensor and Buffer
    memset(&sensor_buffer, 0, sizeof(sensor_buffer));
    sensor = new TEMP::Sensor(DHTPIN, DHTTYPE, &sensor_buffer, &sensor_offset);

    // Setup Serial and Start CSV Loggin
    Serial.begin(9600);
    Serial.println("time_sec,temperature_f");
}

void loop()
{
    // Sample on every frame and update local buffer
    sensor->sample();
 
    // If the interval trigger is set - log out to Serial
    // Note: this operation is SLOW - interrupts SHOULD BE enabled
    if (trigger)
    {
        Serial.print(time);
        Serial.print(",");
        Serial.println(sample.temp_f);
        trigger = false;
    }
}

// This triggers at 1 Hz (every second)
ISR(TIMER1_COMPA_vect)
{
    // Increment seconds counter
    time++;

    // Determine trigger state
    trigger = (time % INTERVAL) == 0;

    // Set sample value to the value(s) stored in the current buffer
    sample.temp_f = sensor_buffer.temp_f;
}