// =============================================================================
// Template-Project | Hardware
// =============================================================================

#ifndef _TEMPLATE_PROJECT_HARDWARE_
#define _TEMPLATE_PROJECT_HARDWARE_

#include <Arduino.h>
#include "configurations.h"
#include "../utils/Arduino/gpio.h"
#include "../.pio/libdeps/nodemcu/MFRC522/src/MFRC522.h"
#include "../.pio/libdeps/nodemcu/NeoPixelBus/src/NeoPixelBus.h"


// Include all hardware drivers here as absolute paths
// Example: #include "../utils/LED/LED.h"
#include "../utils/Buzzer/Buzzer.h"


namespace Hardware
{
    // Port definitions
    enum class Port : uint8_t
    {
        // Define your ports here
        // Example: LED = (uint8_t)GPIO::Port::D0,
        Reader_RST =    (uint8_t)GPIO::Port::D2,
        Reader_SS =     (uint8_t)GPIO::Port::D1,

        Buzzer =        (uint8_t)GPIO::Port::D0,
        LED =           (uint8_t)GPIO::Port::D3
    };


    // Create extern instances of all hardware drivers here
    extern MFRC522 reader;

    // signalization
    extern Buzzer buzzer;
    extern NeoPixelBus<NeoRgbwFeature, NeoEsp8266BitBangSk6812Method> led;


    /// @brief Initializes hardware
    ///
    void init();

    /// @brief Fetches hardware values
    ///
    void updateHardware();
} // namespace Hardware


#endif // _TEMPLATE_PROJECT_HARDWARE_
