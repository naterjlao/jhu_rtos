/*
  Morse Code Transmitter
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
*/

#include "driver.hpp"
#include "morse.hpp"

static void transmit_morse(char in, uint8_t pin, const unsigned long unit, bool enable_tone);
static void debug(const char *in);

void DRIVER::output(uint8_t led_pin, char* message, size_t length, unsigned long unit_time, bool enable_tone)
{
    size_t idx = 0;
    while (idx < length)
    {
        transmit_morse(message[idx], led_pin, unit_time, enable_tone);
        idx++;
    }
    debug("\r\n");
    delay(1000);
}

static void transmit_morse(char in, uint8_t led_pin, const unsigned long unit, bool enable_tone)
{
    const MORSE::CODE *code = MORSE::encode(in);
    if (code > 0)
    {
        unsigned int idx = 0;
        while (idx < MORSE::LIMIT)
        {
            switch (code[idx])
            {
            case MORSE::DIT:
                digitalWrite(led_pin, HIGH);
                if (enable_tone) tone(8, 440);
                delay(unit);
                digitalWrite(led_pin, LOW);
                if (enable_tone) noTone(8);
                debug(".");
                break;
            case MORSE::DAH:
                digitalWrite(led_pin, HIGH);
                if (enable_tone) tone(8, 440);
                delay(unit * 3);
                digitalWrite(led_pin, LOW);
                if (enable_tone) noTone(8);
                debug("-");
                break;
            default:
                break;
            }
            idx++;
            debug(" ");
            delay(unit);
        }
        debug("  ");
        delay(unit * 2);
    }
    else if (in == ' ')
    {
        debug("    ");
        delay(unit * 4);
    }
}

static void debug(const char *in)
{
#if DEBUG
    Serial.print(in);
#endif
}