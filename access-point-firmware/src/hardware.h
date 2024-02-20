// =============================================================================
// Template-Project | Hardware
// =============================================================================

#pragma once

#ifndef _TEMPLATE_PROJECT_HARDWARE_
#define _TEMPLATE_PROJECT_HARDWARE_

#include <Arduino.h>
#include "configurations.h"
#include "gpio.h"
#include <MFRC522.h>

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
        Reader_RST =    (uint8_t)GPIO::Port::D9,
        Reader_SS =     (uint8_t)GPIO::Port::D10,
        Buzzer =        (uint8_t)GPIO::Port::D5
    };


    // Create extern instances of all hardware drivers here
    extern MFRC522 reader;
    extern MFRC522::StatusCode status;
    extern MFRC522::MIFARE_Key key;

    // signalization
    extern Buzzer buzzer;


    /// @brief Initializes hardware
    ///
    void init();

    /// @brief Fetches hardware values
    ///
    void updateHardware();
} // namespace Hardware


#endif // _TEMPLATE_PROJECT_HARDWARE_
