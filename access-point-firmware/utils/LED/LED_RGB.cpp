// =========================================
// Utils | LED_RGB
// =========================================

#include "LED_RGB.h"

// constructor that initializes the RGB LED pin and active level
LED_RGB::LED_RGB(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, ActiveLevel ActiveLevel)
{
    _pin_R = pin_r;
    _pin_G = pin_g;
    _pin_B = pin_b;
    _activeLevel = ActiveLevel;
}

// Function to set the LED color
void LED_RGB::set(Colors_RGB color)
{
    if (_activeLevel == COMMON_CATHODE)
    {
        analogWrite(_pin_R, color.components.r);
        analogWrite(_pin_G, color.components.g);
        analogWrite(_pin_B, color.components.b);
    }

    if (_activeLevel == COMMON_ANODE)
    {
        analogWrite(_pin_R, (255 - color.components.r));
        analogWrite(_pin_G, (255 - color.components.g));
        analogWrite(_pin_B, (255 - color.components.b));
    }
}

// Function to turn off LED
void LED_RGB::off()
{
    if (_activeLevel == COMMON_CATHODE)
    {
        analogWrite(_pin_R, 0);
        analogWrite(_pin_G, 0);
        analogWrite(_pin_B, 0);
    }

    if (_activeLevel == COMMON_ANODE)
    {
        analogWrite(_pin_R, 255);
        analogWrite(_pin_G, 255);
        analogWrite(_pin_B, 255);
    }
}
