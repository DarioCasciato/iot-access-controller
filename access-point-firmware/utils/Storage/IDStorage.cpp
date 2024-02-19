// =============================================================================
// Utils | ID Storage
// =============================================================================

#include "IDStorage.h"
#include "Arduino.h"
#include <EEPROM.h>

#include "../../src/Flash/FlashStructure.h"
namespace
{
    constexpr uint8_t tagAndLengthSize = 2; ///< Size of the tag and length fields
    constexpr uint8_t lengthOffset = 1; ///< Offset for the length field
} // namespace


IDStorage::IDStorage(uint8_t* startAddr, uint16_t storageSize)
    : initialized_(false)
{
    // Similar calculation for start address offset
    header_.startAddr_ = startAddr - reinterpret_cast<uint8_t*>(&Flash::flashLayout);
    header_.storageSize_ = storageSize + sizeof(header_);
    header_.numEntries_ = 0;
    header_.nextAddr_ = header_.startAddr_ + sizeof(header_);
    header_.magic = magicNumber;
}

void IDStorage::init()
{
    if (initialized_)
    {
        return; // Already initialized, do nothing
    }

    // Read header from EEPROM
    uint8_t byteData[sizeof(header_)];  // Create a byte array to hold data
    for (uint16_t i = 0; i < sizeof(header_); i++)
    {
        byteData[i] = EEPROM.read(header_.startAddr_ + i);
    }

    // Cast the read byte array back to a Header struct for easier comparison
    Header *readHeader = reinterpret_cast<Header*>(byteData);

    if(readHeader->magic != header_.magic)
    {
        // Header is invalid, initialize it
        updateHeader();
    }
    else
    {
        // Header is valid, populate it from EEPROM data
        memcpy(&header_, byteData, sizeof(header_));
    }

    initialized_ = true;  // Mark as initialized
}

void IDStorage::updateHeader()
{
    uint8_t byteData[sizeof(header_)];  // Create a byte array to hold data
    memcpy(byteData, &header_, sizeof(header_));  // Copy data to byte array

    // Write byte array to EEPROM
    for (uint16_t i = 0; i < sizeof(header_); i++)
    {
        EEPROM.write(header_.startAddr_ + i, byteData[i]);
    }
}

bool IDStorage::write(uint8_t id, void* data, uint8_t size)
{
    uint16_t addr = findID(id);
    TLV newTlv = {id, size, (uint8_t*)data};

    if(addr != 0)
    {
        uint8_t currentSize = EEPROM.read(addr + lengthOffset);

        if(newTlv.length == currentSize) // Option 1: size is the same
        {
            // write data to eeprom
            for(uint8_t i = 0; i < newTlv.length; i++)
            {
                EEPROM.write(addr + tagAndLengthSize + i, newTlv.value[i]);
            }
        }
        else if(newTlv.length > currentSize) // Option 2: size is greater than current size
        {
            if(!checkSize(newTlv.length - currentSize))
            {
                return false; // storage is full
            }

            // Overwrite current tlv with 0xFF
            for(uint8_t i = 0; i < (currentSize + tagAndLengthSize); i++)
            {
                EEPROM.write(addr + i, 0xFF);
            }

            // copy data from end of deleted tlv to nextAddr
            uint8_t tempData[header_.storageSize_];
            for(uint16_t i = 0; i < (header_.nextAddr_ - (addr + currentSize + tagAndLengthSize)); i++)
            {
                tempData[i] = EEPROM.read(addr + currentSize + tagAndLengthSize + i);
            }

            // paste copied datablock to startAddr of deleted tlv
            for(uint16_t i = 0; i < (header_.nextAddr_ - (addr + currentSize + tagAndLengthSize)); i++)
            {
                EEPROM.write(addr + i, tempData[i]);
            }

            // set from nextAddr to (nextAddr - sizeof deleted tlv) to 0xFF
            for(uint16_t i = 0; i < (header_.nextAddr_ - (currentSize + tagAndLengthSize)); i++)
            {
                EEPROM.write(header_.nextAddr_ - i, 0xFF);
            }

            header_.nextAddr_ -= (currentSize + tagAndLengthSize);

            // write new tlv to eeprom
            EEPROM.write(header_.nextAddr_, newTlv.tag);
            EEPROM.write(header_.nextAddr_ + 1, newTlv.length);
            for(uint8_t i = 0; i < newTlv.length; i++)
            {
                EEPROM.write(header_.nextAddr_ + tagAndLengthSize + i, newTlv.value[i]);
            }

            header_.nextAddr_ += tagAndLengthSize + newTlv.length;
        }
        else // Option 3: size is less than current size
        {
            // Overwrite length and value of current tlv with new length and value
            EEPROM.write(addr + lengthOffset, newTlv.length);
            for(uint8_t i = 0; i < newTlv.length; i++)
            {
                EEPROM.write(addr + tagAndLengthSize + i, newTlv.value[i]);
            }

            // write from end of new tlv to end of old tlv 0xFF
            uint8_t sizeDiff = currentSize - newTlv.length;
            for(uint16_t i = 0; i < sizeDiff; i++)
            {
                EEPROM.write(addr + tagAndLengthSize + newTlv.length + i, 0xFF);
            }

            // copy from end of old tlv to nextAddr, paste to end of new tlv
            uint8_t tempData[header_.storageSize_];
            for(uint16_t i = 0; i < (header_.nextAddr_ - (addr + currentSize + tagAndLengthSize)); i++)
            {
                tempData[i] = EEPROM.read(addr + currentSize + tagAndLengthSize + i);
            }

            // paste copied datablock to end of new tlv
            //! Check validity here
            for(uint16_t i = 0; i < (header_.nextAddr_ - (addr + currentSize + tagAndLengthSize)); i++)
            {
                EEPROM.write(addr + tagAndLengthSize + newTlv.length + i, tempData[i]);
            }

            // write from nextAddr to (nextAddr - sizeDiff) to 0xFF
            for(uint16_t i = 0; i < sizeDiff; i++)
            {
                EEPROM.write(header_.nextAddr_ - i - 1, 0xFF);
            }

            header_.nextAddr_ -= sizeDiff;
        }
    }
    else // Option 4: id does not exist
    {
        if(!checkSize(size))
        {
            return false; // storage is full
        }

        // write tag and length to eeprom
        EEPROM.write(header_.nextAddr_, id);
        EEPROM.write(header_.nextAddr_ + 1, size);

        // write data to eeprom
        for(uint8_t i = 0; i < size; i++)
        {
            EEPROM.write(header_.nextAddr_ + tagAndLengthSize + i, newTlv.value[i]);
        }

        header_.numEntries_++;

        // finish writing, update header data
        header_.nextAddr_ += tagAndLengthSize + size;
    }


    updateHeader();

#if defined(ESP8266) || defined(ESP32)
    EEPROM.commit();
#endif

    return true;
}

bool IDStorage::write(uint8_t id, String data)
{
    if(!write(id, (void*)data.c_str(), data.length() + 1))
    {
        return false;
    }

    return true;
}

bool IDStorage::read(uint8_t id, void* data, uint8_t size)
{
    uint16_t addr = findID(id);

    if (addr == 0)
    {
        return false; // ID not found
    }

    uint8_t* cData = static_cast<uint8_t*>(data);
    uint8_t dataSize = EEPROM.read(addr + lengthOffset);

    if (size < dataSize)
    {
        return false; // data buffer too small
    }

    // read from EEPROM
    for (uint8_t i = 0; i < dataSize; i++)
    {
        cData[i] = EEPROM.read(addr + tagAndLengthSize + i);
    }

    return true;
}

bool IDStorage::deleteID(uint8_t id)
{
    uint16_t addr = findID(id);

    if (addr == 0)
    {
        return false; // ID not found
    }

    uint8_t size = EEPROM.read(addr + lengthOffset);

    // Overwrite tlv with 0xFF
    for(uint8_t i = 0; i < (size + tagAndLengthSize); i++)
    {
        EEPROM.write(addr + i, 0xFF);
    }

    // copy data from end of deleted tlv to nextAddr
    uint8_t tempData[header_.storageSize_];
    for(uint16_t i = 0; i < (header_.nextAddr_ - (addr + size + tagAndLengthSize)); i++)
    {
        tempData[i] = EEPROM.read(addr + size + tagAndLengthSize + i);
    }

    // paste copied datablock to startAddr of deleted tlv
    for(uint16_t i = 0; i < (header_.nextAddr_ - (addr + size + tagAndLengthSize)); i++)
    {
        EEPROM.write(addr + i, tempData[i]);
    }

    // set from nextAddr to (nextAddr - sizeof deleted tlv) to 0xFF
    for(uint16_t i = 0; i < (header_.nextAddr_ - (size + tagAndLengthSize)); i++)
    {
        EEPROM.write(header_.nextAddr_ - i, 0xFF);
    }


    header_.nextAddr_ -= (size + tagAndLengthSize);
    header_.numEntries_--;

    updateHeader();

#if defined(ESP8266) || defined(ESP32)
    EEPROM.commit();
#endif

    return true;
}

bool IDStorage::clear()
{
    for (uint16_t i = sizeof(header_); i < header_.storageSize_; i++)
    {
        EEPROM.write((header_.startAddr_ + sizeof(header_)) + i, 0);
    }

    header_.numEntries_ = 0;
    header_.nextAddr_ = header_.startAddr_ + sizeof(header_);

    updateHeader();

#if defined(ESP8266) || defined(ESP32)
    EEPROM.commit();
#endif

    return true;
}


uint16_t IDStorage::findID(uint8_t id)
{
    uint16_t addr = header_.startAddr_ + sizeof(header_);

    for(uint8_t i = 0; i < header_.numEntries_; i++)
    {
        uint8_t readId = EEPROM.read(addr);
        if(addr > header_.startAddr_ + header_.storageSize_)
        {
            return 0; // end of storage
        }
        if(readId == id)
        {
            return addr;
        }

        addr += tagAndLengthSize + EEPROM.read(addr + lengthOffset); // move to next entry
    }

    return 0; // ID not found
}

bool IDStorage::checkSize(uint8_t size)
{
    if((header_.nextAddr_ + size + tagAndLengthSize) > (header_.startAddr_ + header_.storageSize_))
    {
        return false; // storage is full
    }

    return true;
}
