/*
  Morse Code Transmitter
  Author: Nate Lao (nlao1@jh.edu)
  Designed for Arduino UNO
*/

#ifndef __MORSE_HPP__
#define __MORSE_HPP__

namespace MORSE
{
    enum CODE
    {
        NON,
        DIT,
        DAH
    };

    const unsigned int LIMIT = 5;

    /// @brief Defines the Morse Encodings for Numbers
    const CODE NUMER[10][LIMIT] =
    {
        {DAH, DAH, DAH, DAH, DAH}, // 0
        {DIT, DAH, DAH, DAH, DAH}, // 1
        {DIT, DIT, DAH, DAH, DAH}, // A
        {DIT, DIT, DIT, DAH, DAH}, // A
        {DIT, DIT, DIT, DIT, DAH}, // A
        {DIT, DIT, DIT, DIT, DIT}, // A
        {DAH, DIT, DIT, DIT, DIT}, // A
        {DAH, DAH, DIT, DIT, DIT}, // A
        {DAH, DAH, DAH, DIT, DIT}, // A
        {DAH, DAH, DAH, DAH, DIT}, // A
    };

    /// @brief Defines the Morse Encodings for Letters
    const CODE ALPHA[26][LIMIT] =
    {
        {DIT, DAH, NON, NON, NON}, // A
        {DAH, DIT, DIT, DIT, NON}, // B
        {DAH, DIT, DAH, DIT, NON}, // C
        {DAH, DIT, DIT, NON, NON}, // D
        {DIT, NON, NON, NON, NON}, // E
        {DIT, DIT, DAH, DIT, NON}, // F
        {DAH, DAH, DIT, NON, NON}, // G
        {DIT, DIT, DIT, DIT, NON}, // H
        {DIT, DIT, NON, NON, NON}, // I
        {DIT, DAH, DAH, DAH, NON}, // J
        {DAH, DIT, DAH, NON, NON}, // K
        {DIT, DAH, DIT, DIT, NON}, // L
        {DIT, DIT, NON, NON, NON}, // M
        {DAH, DIT, NON, NON, NON}, // N
        {DAH, DAH, DAH, NON, NON}, // O
        {DIT, DAH, DAH, DIT, NON}, // P
        {DAH, DAH, DIT, DAH, NON}, // Q
        {DIT, DAH, DIT, NON, NON}, // R
        {DIT, DIT, DIT, NON, NON}, // S
        {NON, DAH, NON, NON, NON}, // T
        {DIT, DIT, DAH, NON, NON}, // U
        {DIT, DIT, DIT, DAH, NON}, // V
        {DIT, DAH, DAH, NON, NON}, // W
        {DAH, DIT, DIT, DAH, NON}, // X
        {DAH, DIT, DAH, DAH, NON}, // Y
        {DAH, DAH, DIT, DIT, NON}, // Z
    };

    bool valid(char in);

    /// @brief Returns the address to the morse code encoding of the given alphanumeric character.
    /// @param in Alphanumeric character to encode.
    /// @return The starting address of the morse code CODE encoding. It is guaranteed that the address segment is 5 bytes long.
    /// @return 0 if the character is invalid
    const CODE* encode(char in);
}



#endif