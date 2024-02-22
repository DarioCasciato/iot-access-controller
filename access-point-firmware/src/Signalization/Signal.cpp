// =============================================================================
// Template-Project | Signal
// =============================================================================

#include "Signal.h"
#include "../hardware.h"
#include <SPI.h> //bypass build error


namespace
{
    RgbwColor red(255, 0, 0, 0); // Red
    RgbwColor green(0, 255, 0, 0); // Green
    RgbwColor blue(0, 0, 255, 0); // Blue
    RgbwColor off(0, 0, 0, 0); // Off
}


namespace Signal
{
    void granted()
    {
        Hardware::buzzer.play(3000);
        Hardware::led.SetPixelColor(0, green);
        Hardware::led.Show();
        delay(150);
        Hardware::led.SetPixelColor(0, off);
        Hardware::led.Show();
        Hardware::buzzer.pause();

        Hardware::led.ClearTo(1);
        Hardware::led.Show();
    }

    void denied()
    {
        for (char i = 0; i < 4; i++)
        {
            Hardware::buzzer.play(3000);
            Hardware::led.SetPixelColor(0, red);
            Hardware::led.Show();
            delay(120);

            Hardware::led.SetPixelColor(0, off);
            Hardware::led.Show();
            Hardware::buzzer.pause();
            delay(120);
        }

        Hardware::led.ClearTo(1);
        Hardware::led.Show();
    }

    void connectionEstablished()
    {
        Hardware::led.SetPixelColor(0, blue);
        Hardware::led.Show();
        delay(750);
        Hardware::led.SetPixelColor(0, off);
        Hardware::led.Show();

        Hardware::led.ClearTo(1);
        Hardware::led.Show();
    }

    void connectionError()
    {
        Hardware::led.SetPixelColor(0, off);
        Hardware::led.Show();
        Hardware::buzzer.play(3000);
        delay(350);
        Hardware::led.SetPixelColor(0, red);
        Hardware::led.Show();
        delay(150);
        Hardware::led.SetPixelColor(0, off);
        Hardware::led.Show();
        Hardware::buzzer.pause();

        Hardware::led.ClearTo(1);
        Hardware::led.Show();
    }
} // namespace Signal
