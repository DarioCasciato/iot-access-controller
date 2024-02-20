// =============================================================================
// Template-Project | RFID
// =============================================================================

#pragma once

#include <MFRC522.h>
#include "EdgeDetection.h"

namespace RFID
{
    extern MFRC522::StatusCode status;
    extern MFRC522::MIFARE_Key key;

    extern EdgeDetection tagAvailable;


    /// @brief Check if the card is a master
    /// @return True if the card is a master, false otherwise
    ///
    bool getMasterState();

    /// @brief Get the UID of the card
    /// @return The UID of the card
    ///
    uint32_t getUID();

    /// @brief Update the presence of the card
    ///
    void updateTagPresence();

    /// @brief Update the attributes of the card
    ///
    void updateTagAttributes();
}