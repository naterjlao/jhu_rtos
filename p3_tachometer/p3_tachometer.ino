// https://learn.sparkfun.com/tutorials/driving-motors-with-arduino/all

#include <LiquidCrystal.h>
#include "isr_timer.hpp"

// Debug Pin
const uint8_t PIN_DEBUG = 1; // Optional debugger pin. Indicates blade detection

// LCD Variables
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
const unsigned long LCD_DELAY = 1000;   // LCD Delay Time in milliseconds
const unsigned long DELAY_FACTOR = 8;   // Do not modify, LCD_DELAY multiplier
volatile unsigned long lcd_time;        // Tracks the update LCD frame
volatile bool lcd_flag;                 // Indicates LCD update
LiquidCrystal *lcd = 0;

// Motor Variables
const uint8_t PIN_MOTOR = 3;            // Motor Pin
const unsigned int RAMP_DELAY = 200;    // Ramp Speed delay in Milliseconds
const unsigned char MIN_SPEED = 50;     // Minimal Speed (duty cycle)
const unsigned char MAX_SPEED = 0xFF;   // Maximum Speed (duty cycle)
unsigned char speed;                    // Current Speed
bool ramp_up;                           // Ramp Up flag
unsigned int time;                  // Current Time in Milliseconds

// Tachometer Variables
const uint8_t PIN_IR = A1;                            // IR Phototransitor Pin
const int IR_THRESHOLD = 650;                         // Threshold: Blade Detected=Above Threshold, Otherwise=Below Threshold
const unsigned long PROP_BLADES = 3;                  // Number of Propeller Blades
const unsigned long FRAME_USEC = 125;                 // Frame delay for 8kHz Interrupt
const unsigned long RPM_FACTOR_USEC = (60 * 1000000); // Minutes * Microseconds
volatile unsigned long frames;                        // Tachometer frame counter
volatile unsigned long measured_phase;                // Delta between two rising frames
unsigned long current_phase;                          // Current Phase Measurement
unsigned long rpm;                                    // Revolutions per minute

void setup()
{
    // Setup Interrupt Timer
    cli();
    TIMER::setISRTimer_1kH();
    sei();

    // Setup Debug Pin
    pinMode(PIN_DEBUG, OUTPUT);

    // Setup Motor PWM Pin
    pinMode(PIN_MOTOR, OUTPUT);

    // Setup Serial
    Serial.begin(9600);
    Serial.println("time,speed,rpm");

    // Setup LCD
    lcd = new LiquidCrystal(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);
    lcd->begin(16, 2); // 16 col, 2 rows
    lcd->setCursor(0, 0);
    lcd->print("SPEED");
    lcd->setCursor(8, 0);
    lcd->print("RPM");

    // Setup LCD variables
    lcd_time = 0;
    lcd_flag = false;

    // Setup Motor to Ramp Up
    ramp_up = true;
    speed = MIN_SPEED;
    time = 0;

    // Setup tachometer variables
    frames = 0;
    measured_phase = 0;
    current_phase = 0;
    rpm = 0;


}

ISR(TIMER1_COMPA_vect)
{
    // Blade is detected
    if (analogRead(PIN_IR) > IR_THRESHOLD)
    {
        // Record the number of frames detected since last reset
        if (frames > 0)
            measured_phase = frames;

        // Reset frames
        frames = 0;

        // Set debug pin high for detection
        digitalWrite(PIN_DEBUG, HIGH);
    }
    else
    {
        // Increment frames between blades
        frames++;

        // Set debug pin low for off frames
        digitalWrite(PIN_DEBUG, LOW);
    }

    // Issue LCD update
    if (lcd_time == LCD_DELAY * DELAY_FACTOR)
    {
        lcd_flag = true;
        lcd_time = 0;
    }
    lcd_time++;
}

void loop()
{
    // Determine ramp phase if at limits
    if (speed == MIN_SPEED)
        ramp_up = true;
    if (speed == MAX_SPEED)
        ramp_up = false;

    // Apply acceleration
    if (ramp_up)
        speed++;
    else
        speed--;

    // Set duty cycle to control speed
    analogWrite(PIN_MOTOR, speed);

    // Poll the measured phase delta
    cli();
    current_phase = measured_phase;
    sei();

    // Calculate RPM
    current_phase = (current_phase * PROP_BLADES) * FRAME_USEC;
    rpm = RPM_FACTOR_USEC / current_phase;

    // Log out to host
    Serial.print(time);
    Serial.print(",");
    Serial.print(speed);
    Serial.print(",");
    Serial.println(rpm);

    // Display RPM at 1 second intervals
    if (lcd_flag)
    {
        // Display LCD
        lcd->setCursor(0, 1);
        lcd->print(speed);
        lcd->print("    ");
        lcd->setCursor(8, 1);
        lcd->print(rpm);
        lcd->print("    ");
        lcd_flag = false;
    }

    // Ramp Delay
    delay(RAMP_DELAY);
    time += RAMP_DELAY;
}