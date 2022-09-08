/*
  Morse Code
*/

#include "driver.hpp"

uint8_t LED_PIN = 12;

// the setup function runs once when you press reset or power the board
void setup()
{
  DRIVER::initialize(LED_PIN);
}

// the loop function runs over and over again forever
void loop()
{
  DRIVER::main(LED_PIN);
}
