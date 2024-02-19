// =============================================================================
// Utils | ID Storage
// =============================================================================

#ifndef UTILS_ID_STORAGE_
#define UTILS_ID_STORAGE_

#include <stdint.h>
#include <Arduino.h>

/// @class IDStorage
/// @brief Class for handling ID-based storage in flash memory. Entries are encoded as TLV (Tag-Length-Value).
class IDStorage
{
private:
    bool initialized_;  ///< Flag to indicate if the storage area has been initialized
    const uint16_t magicNumber {0xECEC};  ///< Magic number to identify the header

    struct Header
    {
        uint16_t magic;  ///< Magic number to identify the header
        uint16_t storageSize_;  ///< Size of the storage area in flash memory
        uint16_t startAddr_;  ///< Starting address for this instance
        uint16_t numEntries_;  ///< Number of entries currently stored
        uint16_t nextAddr_;  ///< Next available address for writing
    } header_;

    struct TLV
    {
        uint8_t tag;  ///< Tag for the data
        uint8_t length;  ///< Length of the data
        uint8_t* value;  ///< Pointer to the data
    };


    /// @brief Update the header in flash memory.
    void updateHeader();

    /// @brief Find an ID in the storage area.
    ///
    /// @return address of ID
    ///
    uint16_t findID(uint8_t id);

    /// @brief Check if the storage area has enough space for the given size.
    ///
    /// @param size Size to check
    /// @return True if the storage area has enough space, false otherwise
    ///
    bool checkSize(uint8_t size);

public:
    /// @brief Constructor for the IDStorage class.
    ///
    /// @param storageSize Size of the storage area in flash memory
    /// @param magicNumber Magic number to identify the header
    ///
    IDStorage(uint8_t* startAddr, uint16_t storageSize);

    /// @brief Initialize the storage area.
    ///
    void init();

    /// @brief Write function as template.
    ///
    /// @param id ID to which the data should be associated
    /// @param data data to be written
    /// @return True if the write was successful, false otherwise
    ///
    template <typename T>
    bool write(uint8_t id, T data)
    {
        return write(id, &data, sizeof(T));
    }

    /// @brief Write data associated with a specific ID.
    ///
    /// @param id ID to which the data should be associated
    /// @param data Pointer to the data to be written
    /// @param size Size of the data to be written
    /// @return True if the write was successful, false otherwise
    ///
    bool write(uint8_t id, void* data, uint8_t size);

    /// @brief Write data associated with a specific ID. (overload)
    ///
    /// @param id ID to which the data should be associated
    /// @param str String to be written
    /// @return True if the write was successful, false otherwise
    ///
    bool write(uint8_t id, String data);

    /// @brief Read function as template.
    ///
    /// @param id ID from which the data should be read
    /// @param destination Pointer to store the read data
    /// @return True if the read was successful, false otherwise
    ///
    template <typename T>
    bool read(uint8_t id, T* destination)
    {
        return read(id, destination, sizeof(T));
    }

    /// @brief Read data associated with a specific ID.
    ///
    /// @param id ID from which the data should be read
    /// @param destination Pointer to store the read data
    /// @return True if the read was successful, false otherwise
    ///
    bool read(uint8_t id, void* destination, uint8_t size);

    /// @brief Delete data associated with a specific ID.
    ///
    /// @param id ID from which the data should be deleted
    /// @return True if the delete operation was successful, false otherwise
    ///
    bool deleteID(uint8_t id);

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

    /// @brief Get the size of the storage area.
    ///
    /// @return Storage size
    ///
    uint16_t getStorageSize() { return header_.storageSize_; }
};

#endif // UTILS_ID_STORAGE_
