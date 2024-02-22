// =============================================================================
// Template-Project | Hardware
// =============================================================================

#include <Arduino.h>
#include "hardware.h"
#include <SPI.h>

namespace Hardware
{
    // Create the instances of all hardware drivers here
    // Example: LED statusLed((uint8_t)Port::LED, ActiveLevel::AHIGH)
    MFRC522 reader((uint8_t)Port::Reader_SS, (uint8_t)Port::Reader_RST);

    // signalization
    Buzzer buzzer((uint8_t)Port::Buzzer);
    NeoPixelBus<NeoRgbwFeature, NeoEsp8266BitBangSk6812Method> led(1, (uint8_t)Port::LED);


    void init() // Initialize hardware, function is called once on startup
    {
        SPI.begin(); // Init SPI bus
        reader.PCD_Init(); // Init RFID reader

        led.Begin(); // Init LEDs
        led.SetPixelColor(0, RgbwColor(0, 0, 0, 0)); // Turn off LED
        led.Show(); // Update LED
    }

    void updateHardware() // Fetch hardware values, function is called every loop
    {

    }
} // namespace Hardware
