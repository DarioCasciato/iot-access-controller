// =============================================================================
// Template-Project | RFID
// =============================================================================

#include <stdint.h>
#include <MFRC522.h>
#include "EdgeDetection.h"


namespace RFID
{
    extern EdgeDetection tagAvailable;


    /// @brief Get the UID of the card
    /// @return The UID of the card
    ///
    uint32_t getUID();

    /// @brief Update the presence of the card
    ///
    void updateTagPresence();

    /// @brief Update the attributes of the card
    ///
    void updateTagUID();
}