// =============================================================================
// Utils | Encryption
// =============================================================================

#include "encryption.h"


namespace Security::Encryption
{
    bool encrypt(uint64_t key, void* data, uint8_t size)
    {
        if (data == nullptr || size == 0)
        {
            return false;
        }

        uint8_t* byteData = static_cast<uint8_t*>(data);
        for (uint8_t i = 0; i < size; i++)
        {
            byteData[i] ^= ((uint8_t*)&key)[(i + size) % sizeof(key)];
        }
        return true;
    }

    bool decrypt(uint64_t key, void* data, uint8_t size)
    {
        if (data == nullptr || size == 0)
        {
            return false;
        }

        uint8_t* byteData = static_cast<uint8_t*>(data);
        for (uint8_t i = 0; i < size; i++)
        {
            byteData[i] ^= ((uint8_t*)&key)[(i + size) % sizeof(key)];
        }
        return true;
    }
} // namespace Security::Encryption
