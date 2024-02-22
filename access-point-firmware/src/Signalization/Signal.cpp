// =============================================================================
// Template-Project | Signal
// =============================================================================

#include "Signal.h"
#include "../hardware.h"
#include <SPI.h> //bypass build error


namespace
{
    RgbColor red(255, 0, 0); // Red
    RgbColor green(0, 255, 0); // Green
    RgbColor blue(0, 0, 255); // Blue
    RgbColor off(0, 0, 0); // Off
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
    }

    void connectionEstablished()
    {
        Hardware::led.SetPixelColor(0, blue);
        Hardware::led.Show();
        delay(150);
        Hardware::led.SetPixelColor(0, off);
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
    }
} // namespace Signal
