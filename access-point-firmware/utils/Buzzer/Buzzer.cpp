// =========================================
// Utils | Buzzer
// =========================================

#include <Arduino.h>
#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
}

void Buzzer::play(uint16_t freq)
{
    tone(_pin, freq);
}

void Buzzer::pause()
{
    noTone(_pin);
}
