// =============================================================================
// Template-Project | Signal
// =============================================================================

#include "Signal.h"
//#include "../hardware.h"

/*
namespace
{
    CRGB color_red = {100, 0, 0}; // Values from 0-255
    CRGB color_green = {0, 100, 0};
    CRGB color_blue = {0, 0, 50};
    CRGB color_off = {0, 0, 0};
}

namespace Signal
{
    void granted()
    {
        Hardware::buzzer.play(3000);
        Hardware::leds[0] = color_green;
        FastLED.show();
        delay(150);
        Hardware::leds[0] = color_off;
        FastLED.show();
        Hardware::buzzer.pause();
    }

    void denied()
    {
        for (char i = 0; i < 4; i++)
        {
            Hardware::buzzer.play(3000);
            Hardware::leds[0] = color_red;
            FastLED.show();
            delay(120);

            Hardware::leds[0] = color_off;
            FastLED.show();
            Hardware::buzzer.pause();
            delay(120);
        }
    }

    void connectionEstablished()
    {
        Hardware::leds[0] = color_blue;
        FastLED.show();
        delay(150);
        Hardware::leds[0] = color_off;
        FastLED.show();
    }

    void connectionError()
    {
        Hardware::leds[0] = color_off;
        FastLED.show();
        Hardware::buzzer.play(3000);
        delay(350);
        Hardware::leds[0] = color_red;
        FastLED.show();
        delay(150);
        Hardware::leds[0] = color_off;
        FastLED.show();
        Hardware::buzzer.pause();
    }
} // namespace Signal
*/