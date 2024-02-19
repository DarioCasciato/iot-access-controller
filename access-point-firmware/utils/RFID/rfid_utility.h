// =============================================================================
// arduino-access-system | Dario Casciato
// =============================================================================

#include <Arduino.h>
#include <MFRC522.h>
#include "hardware.h"

#ifndef ARDUINO_ACCESS_SYSTEM_RFID_UTILITY_
#define ARDUINO_ACCESS_SYSTEM_RFID_UTILITY_

namespace Tag
{
    /// @brief Check if the tag is the master tag
    ///
    /// @return True if the tag is the master tag, false otherwise
    bool checkMaster();

    /// @brief Check if a tag is present
    ///
    /// @return True if a tag is present, false otherwise
    bool tagPresent();

    /// @brief Get the UID of the tag
    ///
    /// @return The UID of the tag
    uint32_t getUID();
} // namespace Tag

#endif // ARDUINO_ACCESS_SYSTEM_RFID_UTILITY_