// =========================================
// Utils | Potentiometer
// =========================================

#include <Arduino.h>

#ifndef ARDUINO_OBJECTS_POTENTIOMETER_
#define ARDUINO_OBJECTS_POTENTIOMETER_

class Potentiometer
{
    enum Direction
    {
        UP,
        DOWN
    };

private:
    uint8_t _pin;
    uint16_t _lastValue;
    Direction _direction;

public:
    /// @brief Potentiometer constructor
    ///
    /// @param pin The analog pin connected to the potentiometer
    /// @param direction The direction of the potentiometer (UP or DOWN)
    ///
    Potentiometer(uint8_t pin, Direction direction);

    /// @brief Get the current value of the potentiometer
    ///
    /// @return The current value of the potentiometer
    ///
    uint16_t get();

    /// @brief Get the current percentage value of the potentiometer
    ///
    /// @return The current percentage value of the potentiometer (0-100)
    ///
    uint8_t getPercentage();
};

#endif // ARDUINO_OBJECTS_POTENTIOMETER_
