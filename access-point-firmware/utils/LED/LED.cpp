// =========================================
// Utils | LED
// =========================================

#include "LED.h"

// constructor that initializes the LED pin and active level
LED::LED(uint8_t pin, ActiveLevel activeLevel)
{
    _pin = pin;
    _activeLevel = activeLevel;

    pinMode(_pin, OUTPUT);

    // set the initial state of the LED based on active level
    if (_activeLevel == LOW_ACTIVE)
        digitalWrite(_pin, HIGH);
    else
        digitalWrite(_pin, LOW);
}

// function to set the LED state based on the active level
void LED::set(level setLevel)
{
    if (_activeLevel == LOW_ACTIVE)
        digitalWrite(_pin, ~setLevel);
    else
        digitalWrite(_pin, setLevel);
}

// function to get the current state of the LED
bool LED::get()
{
    return digitalRead(_pin) == _activeLevel;
}

// function to toggle the LED state
void LED::toggle()
{
    digitalWrite(_pin, digitalRead(_pin) ^ 1);
}

// function to turn on the LED
void LED::on()
{
    if (_activeLevel == ALOW)
        digitalWrite(_pin, LOW);
    else
        digitalWrite(_pin, HIGH);
}

// function to turn off the LED
void LED::off()
{
    if (_activeLevel == AHIGH)
        digitalWrite(_pin, HIGH);
    else
        digitalWrite(_pin, LOW);
}
