// =============================================================================
// Template-Project | Hardware
// =============================================================================

#include <Arduino.h>
#include "hardware.h"
#include "gpioHandler.h"
#include "RFID.h"

namespace Hardware
{
    // Create the instances of all hardware drivers here
    // Example: LED statusLed((uint8_t)Port::LED, ActiveLevel::AHIGH)
    MFRC522 reader((uint8_t)Port::Reader_SS, (uint8_t)Port::Reader_RST);

    // signalization
    Buzzer buzzer((uint8_t)Port::Buzzer);


    void init() // Initialize hardware, function is called once on startup
    {

    }

    void updateHardware() // Fetch hardware values, function is called every loop
    {
        RFID::updateTagPresence();
    }
} // namespace Hardware
