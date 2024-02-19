// =========================================
// Utils | Buzzer
// =========================================

#include <Arduino.h>

#ifndef ARDUINO_OBJECTS_BUZZER_
#define ARDUINO_OBJECTS_BUZZER_

class Buzzer
{
private:
    uint8_t _pin;

public:
    /// @brief Buzzer constructor
    ///
    /// @param pin The pin number to which the buzzer is connected
    ///
    Buzzer(uint8_t pin);

    /// @brief Play a sound with the specified frequency
    ///
    /// @param freq The frequency of the sound to be played
    ///
    void play(uint16_t freq);

    /// @brief Pause the sound being played
    ///
    void pause();
};

#endif // ARDUINO_OBJECTS_BUZZER_
