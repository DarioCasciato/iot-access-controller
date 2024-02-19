// =============================================================================
// Utils | RollStorage
// =============================================================================

#ifndef UTILS_ROLL_STORAGE_
#define UTILS_ROLL_STORAGE_

#include <stdint.h>


/// @class RollStorage
/// @brief Class for handling storage in flash memory.
class RollStorage
{
private:
    bool initialized_;  ///< Flag to indicate if the storage area has been initialized
    const uint16_t magicNumber {0xB6A7};  ///< Magic number to identify the header

    struct Header
    {
        uint16_t magic;  ///< Magic number to identify the header
        uint16_t storageSize_;  ///< Size of the storage area in flash memory
        uint16_t startAddr_;  ///< Starting address for this instance
        uint8_t dataSize_;  ///< Size of individual data entries
        uint16_t numMaxEntries_;  ///< Maximum number of entries that can be stored
        uint16_t numEntries_;  ///< Number of entries currently stored
        uint16_t nextAddr_;  ///< Next available address for writing
        uint8_t padding_; ///< Padding to ensure the header is a multiple of 4 bytes
    } header_;

    /// @brief Update the header in flash memory.
    void updateHeader();

public:
    /// @brief Constructor for the RollStorage class.
    ///
    /// @param storageSize Size of the storage area in flash memory
    /// @param dataSize Size of individual data entries
    ///
    RollStorage(uint8_t* startAddr, uint16_t storageSize, uint8_t dataSize);

    /// @brief Initialize the storage area.
    ///
    void init();

    /// @brief Write data to the next available address.
    ///
    /// @param data Pointer to the data to be written
    /// @return True if the write was successful, false otherwise
    ///
    bool write(void* data);

    /// @brief Read data from a specific index.
    ///
    /// @param index Index from which the data should be read
    /// @param data Pointer to store the read data
    /// @return True if the read was successful, false otherwise
    ///
    bool read(uint16_t index, void* data);

    /// @brief Read the last written data.
    ///
    /// @param data Pointer to store the read data
    /// @return True if the read was successful, false otherwise
    ///
    bool readLast(void* data);

    /// @brief Clear all data in the storage area.
    ///
    /// @return True if the clear operation was successful, false otherwise
    ///
    bool clear();

    /// @brief Get the number of entries currently stored.
    ///
    /// @return Number of entries
    ///
    uint16_t getNumEntries() { return header_.numEntries_; }

    /// @brief Get the maximum number of entries that can be stored.
    ///
    /// @return Maximum number of entries
    ///
    uint16_t getMaxEntries() { return header_.numMaxEntries_; }

    /// @brief Get the size of the storage area.
    ///
    /// @return Storage size
    ///
    uint16_t getStorageSize() { return header_.storageSize_; }

    /// @brief Get the size of individual data entries.
    ///
    /// @return Data size
    ///
    uint16_t getDataSize() { return header_.dataSize_; }
};


#endif // UTILS_ROLL_STORAGE_
