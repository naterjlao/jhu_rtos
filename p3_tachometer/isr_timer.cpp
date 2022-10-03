/*
  Temperature Sensor
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
  Based on implementation from https://www.instructables.com/Arduino-Timer-Interrupts/
*/

#include "isr_timer.hpp"

void TIMER::setISRTimer(const unsigned long hertz)
{
    // Enables the TIMER1_COMPA_vect Interrupt Service Routine

    // The base value of the OCR1A timer compare register
    // for 1 Hz. To increase rate divide by the desired
    // frequency (input:hertz).
    const unsigned long TIMER_COMPARE_REGISTER_1HZ = 15624;
    
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1 = 0;  // initialize counter value to 0

    // set compare match register for x hz increments
    OCR1A = TIMER_COMPARE_REGISTER_1HZ / hertz;

    // turn on CTC mode
    TCCR1B |= (1 << WGM12);

    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
}

void TIMER::setISRTimer_1kH()
{
//set timer2 interrupt at 8kHz = 0.125us
  TCCR1A = 0;// set entire TCCR2A register to 0
  TCCR1B = 0;// same for TCCR2B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR1A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS21 bit for 8 prescaler
  TCCR1B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}
