/*
  Morse Code Transmitter
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
*/

#include "driver.hpp"
#include "morse.hpp"

bool DRIVER::input(const int limit, char *message, int *length)
{
    bool halt = false;
    int input_len = Serial.available();
    
    while (input_len > 0)
    {
        char input = (char) Serial.read();
        Serial.print(input);

        // Escape singleton Ctrl-Z
        halt = input == 26;

        // Populate the message buffer
        if (!halt && (MORSE::valid(input)))
        {
            message[*length] = input;
            *length = (*length + 1) % limit;
        }

        input_len--;
    }

    if (halt)
    {
        Serial.print("\r\n");
        Serial.println(">>> HALT! <<<");
    }

    return halt;
}