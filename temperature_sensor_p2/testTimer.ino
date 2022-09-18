//https://electronoobs.com/eng_arduino_tut12.php

#include "timer.hpp"
#include "temp.hpp"

TEMP::Sensor temp_sensor;

boolean state;

void setup()
{
    pinMode(3,OUTPUT);
    DDRD |= B00000100;
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
    if (state)
        PORTD |= B00000100;
    else
        PORTD &= B11111011;
    state = !state;
}

#if 0
void setup()
{
    //pinMode(2,OUTPUT);
    DDRD |= B00000100;
}

void loop()
{
    //digitalWrite(2,HIGH);
    //digitalWrite(2,LOW);
    PORTD |= B00000100;
    PORTD &= B11111011;
    delay(100);
}
#endif