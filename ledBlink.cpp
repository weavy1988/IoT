#include "ledBlink.h"

// Constructor: Initializes the LED pin
Blink::Blink(int pin) {
    _pin = pin;                 // Store the pin number
    pinMode(_pin, OUTPUT);     // Set the pin as an OUTPUT
}

// Method to control the blinking rate of the LED
void Blink::blinkRate(int rate) {
    digitalWrite(_pin, HIGH);  // Turn the LED on
    delay(rate);                // Wait for the specified rate
    digitalWrite(_pin, LOW);   // Turn the LED off
    delay(rate);                // Wait for the specified rate
}
