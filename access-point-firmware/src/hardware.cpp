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
    CRGB leds[1] = {CRGB(0, 0, 0)};

    void init() // Initialize hardware, function is called once on startup
    {
        FastLED.addLeds<SK6812, (uint8_t)Port::LED>(leds, 1); // Init LED strip
        SPI.begin(); // Init SPI bus
        reader.PCD_Init(); // Init RFID reader
    }

    void updateHardware() // Fetch hardware values, function is called every loop
    {

    }
} // namespace Hardware
