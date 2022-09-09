/*
  Morse Code Transmitter
  Designed for Arduino UNO
*/

#include <string.h>
#include "driver.hpp"

// ----- CONSTANTS ----- //
const uint8_t LED_PIN = 12;
const unsigned long UNIT_TIME = 50;
const int MESSAGE_LIMIT = 255;
bool ENABLE_TONE = true;

// ----- GLOABALS ----- //
char message[MESSAGE_LIMIT];
int length;
bool halt;

/// @brief Initialization
void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);

  memset(message, 0, MESSAGE_LIMIT);
  length = 0;
  halt = false;

  Serial.println("MORSE CODE TRANSMITTER");
  Serial.print("Enter: ");
}

/// @brief Main Loop
void loop()
{
  if (!halt)
  {
    DRIVER::output(LED_PIN, message, length, UNIT_TIME, ENABLE_TONE);
    halt = DRIVER::input(MESSAGE_LIMIT, message, &length);
  }
  // halt indefinitely
}
