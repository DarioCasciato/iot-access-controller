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
        // Example: uint8_t memory1[0x200]; // 512 bytes
        // Add more as needed
        uint8_t idTestMemory[0x80]; // 128 bytes
    };

    extern Layout flashLayout; // Declaration of the global instance
} // namespace Flash


#endif // UTILS_FLASH_STRUCTURE_
