// =============================================================================
// Utils | Encryption
// =============================================================================

#include <stdint.h>

#ifndef UTILS_ENCRYPTION_
#define UTILS_ENCRYPTION_


namespace Security::Encryption
{
    /// @brief Encrypts the given data with the given key.
    /// @param key encryption key
    /// @param data pointer to the data to encrypt
    /// @param size size of the data in bytes
    /// @return true if the encryption was successful, otherwise false
    ///
    bool encrypt(uint64_t key, void* data, uint8_t size);

    /// @brief Decrypts the given data with the given key.
    /// @param key encryption key
    /// @param data pointer to the data to decrypt
    /// @param size size of the data in bytes
    /// @return true if the decryption was successful, otherwise false
    ///
    bool decrypt(uint64_t key, void* data, uint8_t size);

} // namespace Security::Encryption



#endif // UTILS_ENCRYPTION_
