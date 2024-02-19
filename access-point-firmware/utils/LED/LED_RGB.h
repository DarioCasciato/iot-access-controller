// =========================================
// Utils | LED_RGB
// =========================================

#include <Arduino.h>

#ifndef ARDUINO_OBJECTS_LED_RGB_
#define ARDUINO_OBJECTS_LED_RGB_

enum ActiveLevel
{
    COMMON_ANODE,
    COMMON_CATHODE
};

typedef union
{
    uint32_t hex_code;
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } components;
} Colors_RGB;

class LED_RGB
{
private:
    uint8_t _pin_R;
    uint8_t _pin_G;
    uint8_t _pin_B;
    ActiveLevel _activeLevel;

public:
    /// @brief LED_RGB constructor
    ///
    /// @param pin_r Pin number for the red component
    /// @param pin_g Pin number for the green component
    /// @param pin_b Pin number for the blue component
    /// @param ActiveLevel Active level configuration (COMMON_ANODE or COMMON_CATHODE)
    ///
    LED_RGB(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, ActiveLevel ActiveLevel);

    /// @brief Set the color of the RGB LED
    ///
    /// @param colors Colors_RGB object containing the RGB values to set
    ///
    void set(Colors_RGB colors);

    /// @brief Turn off the RGB LED
    ///
    void off();
};

#endif // ARDUINO_OBJECTS_LED_RGB_
