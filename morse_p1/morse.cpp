/*
  Morse Code Transmitter
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
*/

#include "morse.hpp"

bool MORSE::valid(char in)
{
    return (in >= 48 && in <= 57) || (in >= 65 && in <= 90) || (in >= 97 && in <= 122);
}

const MORSE::CODE* MORSE::encode(char in)
{
    const MORSE::CODE *code = 0;

    // Numbers
    if (in >= 48 && in <= 57)
    {
        in = in - 48;
        code = &MORSE::NUMER[in][0];
    }

    // Uppercase
    else if (in >= 65 && in <= 90)
    {
        in = in - 65;
        code = &MORSE::ALPHA[in][0];
    }

    // Lowercase
    else if (in >= 97 && in <= 122)
    {
        in = in - 32; // to uppercase
        in = in - 65; // offset
        code = &MORSE::ALPHA[in][0];
    }

    else
    {
        code = 0;
    }

    return code;
}