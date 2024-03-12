// =============================================================================
// Utils | FlashStorage
// =============================================================================

#ifndef UTILS_FLASH_STRUCTURE_
#define UTILS_FLASH_STRUCTURE_

#include <stdint.h>

namespace Flash
{
    struct Layout
    {
        // Define your memory areas here
        uint8_t memory1[0x80]; // 128 bytes
        // Add more as needed
    };

    extern Layout flashLayout; // Declaration of the global instance
} // namespace Flash


#endif // UTILS_FLASH_STRUCTURE_
