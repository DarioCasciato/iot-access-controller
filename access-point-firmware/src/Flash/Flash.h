// =============================================================================
// Utils | FlashStorage
// =============================================================================

#ifndef UTILS_FLASH_
#define UTILS_FLASH_

#include "RollStorage.h"
#include "IDStorage.h"
#include "FlashStructure.h"
#include <EEPROM.h>

namespace Flash
{
    // add flash memory definition here
    // example: extern RollStorage testMemory;
    extern IDStorage configStore;

    /// @brief initializes the flash memory
    ///
    void init();

    /// @brief clears all flash storages
    ///
    void clear();


    enum class ID : uint8_t
    {
        ServerIP = 0,
    };

} // namespace Flash

#endif // UTILS_FLASH_
