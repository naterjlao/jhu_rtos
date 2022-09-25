

#define LCD_ENABLE 1

#include <DHT.h>
#if LCD_ENABLE
#include <LiquidCrystal.h>
#endif

#include "isr_timer.hpp"
#include "temp.hpp"

// Sampling Variables
const unsigned long INTERVAL = 10;
volatile unsigned long time;
volatile bool trigger;
volatile TEMP::DATA_BUFFER sample;

// Temperature Sensor
const uint8_t DHTPIN = 2;
const uint8_t DHTTYPE = DHT11;
const TEMP::DATA_BUFFER sensor_offset = {-1.66};
TEMP::DATA_BUFFER sensor_buffer;
TEMP::Sensor *sensor = 0;

#if LCD_ENABLE
// LCD Pins (if available)
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;

// LCD Object
LiquidCrystal *lcd = 0;
#endif

void setup()
{
    // Initialize Interrupt Variables
    time = 0;
    trigger = false;
    sample.temp_f = 0.0;

    // Setup 1 Hz Timer Interrupt
    cli(); // Disable Interupts
    TIMER::setISRTimer(1);
    sei(); // Enable Interrupts

    // Initialize Temperature Sensor and Buffer
    memset(&sensor_buffer, 0, sizeof(sensor_buffer));
    sensor = new TEMP::Sensor(DHTPIN, DHTTYPE, &sensor_buffer, &sensor_offset);

    // Setup Serial and Start CSV Logging
    Serial.begin(9600);
    Serial.println("time_sec,temperature_f");

#if LCD_ENABLE
    lcd = new LiquidCrystal(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);
    lcd->begin(16,2); // 16 col, 2 rows
    lcd->setCursor(0,0);
    lcd->print("TIME");
    lcd->setCursor(8,0);
    lcd->print("TEMP (F)");
#endif
}

void loop()
{
    // Sample on every frame and update local buffer
    sensor->sample();

#if LCD_ENABLE
    lcd->setCursor(0,1);
    lcd->print(time);
    lcd->setCursor(8,1);
    lcd->print(sample.temp_f);
#endif

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