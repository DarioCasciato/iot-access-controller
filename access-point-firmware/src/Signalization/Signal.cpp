// =============================================================================
// Template-Project | Signal
// =============================================================================

#include "Signal.h"
//#include "../hardware.h"

/*
namespace
{
    RGBW color_red = {100, 0, 0, 0}; // Values from 0-255
    RGBW color_green = {0, 100, 0, 0};
    RGBW color_blue = {0, 0, 50, 0};
    RGBW color_off = {0, 0, 0, 0};
}

namespace Signal
{
    void granted()
    {
        Hardware::buzzer.play(3000);
        Hardware::led.set_rgbw(0, color_green);
        Hardware::led.sync();
        delay(150);
        Hardware::led.set_rgbw(0, color_off);
        Hardware::led.sync();
        Hardware::buzzer.pause();
    }

    void denied()
    {
        for (char i = 0; i < 4; i++)
        {
            Hardware::buzzer.play(3000);
            Hardware::led.set_rgbw(0, color_red);
            Hardware::led.sync();
            delay(120);

            Hardware::led.set_rgbw(0, color_off);
            Hardware::led.sync();
            Hardware::buzzer.pause();
            delay(120);
        }
    }

    void connectionEstablished()
    {
        Hardware::led.set_rgbw(0, color_blue);
        Hardware::led.sync();
        delay(150);
        Hardware::led.set_rgbw(0, color_off);
        Hardware::led.sync();
    }

    void connectionError()
    {
        Hardware::led.set_rgbw(0, color_off);
        Hardware::led.sync();
        Hardware::buzzer.play(3000);
        delay(350);
        Hardware::led.set_rgbw(0, color_red);
        Hardware::led.sync();
        delay(150);
        Hardware::led.set_rgbw(0, color_off);
        Hardware::led.sync();
        Hardware::buzzer.pause();
    }
} // namespace Signal
*/