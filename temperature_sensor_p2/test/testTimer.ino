//https://electronoobs.com/eng_arduino_tut12.php

#include "isr_timer.hpp"
#include "temp.hpp"

TEMP::Sensor* temp_sensor;

volatile unsigned int second;
volatile bool state;

void setup()
{
    pinMode(3,OUTPUT);
    DDRD |= B00000100;
    second = 0;
    state = false;
    cli(); // disable interrupts
    TIMER::setISRTimer(1);
    sei(); 
}

void loop()
{
    digitalWrite(3,HIGH);
    delay(1500);
    digitalWrite(3,LOW);
    delay(1500);
}

// This 
ISR(TIMER1_COMPA_vect)
{
    second = (second + 1) % 10;
    if (second == 0)
    {
        if (state)
            PORTD |= B00000100;
        else
            PORTD &= B11111011;
        state = !state;
    }
}