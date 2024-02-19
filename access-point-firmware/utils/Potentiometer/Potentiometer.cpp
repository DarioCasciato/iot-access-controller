// =========================================
// Utils | Potentiometer
// =========================================

#include <Potentiometer.h>

Potentiometer::Potentiometer(uint8_t pin, Direction direction)
{
    _pin = pin;
    _direction = direction;
}

uint16_t Potentiometer::get()
{
    _lastValue = analogRead(_pin);

    return (_direction == UP) ? _lastValue : 1023 - _lastValue;
}

uint8_t Potentiometer::getPercentage()
{
    _lastValue = analogRead(_pin);

    return (_direction == UP) ? map(_lastValue, 0, 1023, 0, 100) : map(1023 - _lastValue, 0, 1023, 0, 100);
}
