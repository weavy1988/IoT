#ifndef LEDBLINK_H
#define LEDBLINK_H

#include <Arduino.h>

class Blink {
public:
    Blink(int pin);             // Constructor that sets up the pin
    void blinkRate(int rate);   // Method to control the blink rate

private:
    int _pin;                   // Variable to store the pin number
};

#endif // LEDBLINK_H
