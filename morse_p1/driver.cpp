
#include "string.h"
#include "driver.hpp"
#include "morse.hpp"

#define DEBUG 1

static void transmit_morse(char in, uint8_t pin);
#if DEBUG
static void print_morse(char in);
#endif

void DRIVER::initialize(uint8_t led_pin)
{
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);
    Serial.begin(9600);
}

void DRIVER::main(uint8_t led_pin)
{
    char message[] = "the quick brown fox jumped over the lazy dog";
    int idx = 0;
    while (idx < strlen(message))
    {
        transmit_morse(message[idx],led_pin);
        print_morse(message[idx]);
        idx++;
    }
    delay(1000);
    Serial.print("\r\n");
}

static void transmit_morse(char in, uint8_t led_pin)
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
                digitalWrite(led_pin,HIGH);
                tone(8, 440);
                delay(DRIVER::UNIT_TIME);
                digitalWrite(led_pin,LOW);
                noTone(8);
                break;
            case MORSE::DAH:
                digitalWrite(led_pin,HIGH);
                tone(8, 440);
                delay(DRIVER::UNIT_TIME * 3);
                digitalWrite(led_pin,LOW);
                noTone(8);
                break;
            default:
                break;
            }
            idx++;
            delay(DRIVER::UNIT_TIME);
        }
        delay(DRIVER::UNIT_TIME * 2);
    }
    else if (in == ' ')
    {
        delay(DRIVER::UNIT_TIME * 4);
    }
}

#if DEBUG
static void print_morse(char in)
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
                Serial.print(".");
                break;
            case MORSE::DAH:
                Serial.print("-");
                break;
            default:
                break;
            }
            idx++;
            Serial.print(" "); // one unit
        }
        Serial.print("  "); // three units (+1 from char)
    }
    else if (in == ' ')
    {
        Serial.print("    "); // seven units
    }
}
#endif