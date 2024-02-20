// =============================================================================
// Template-Project | RFID
// =============================================================================

#include <stdint.h>
#include <MFRC522.h>
#include "EdgeDetection.h"
#include "../hardware.h"
#include "RFID.h"

namespace
{
    MFRC522::StatusCode status;
    MFRC522::MIFARE_Key key;
    uint8_t blockData[17] = {"MasterMediumCard"};
    uint8_t bufferLen = 18;
    uint8_t readBlockData[18];
    bool repeatFlagPresent = 0;
    uint32_t TagUID = 0;

    uint8_t tagAvailableVal = 0;
    bool isMaster = 0;
    uint32_t uid = 0;


    bool checkMaster()
    {
        status = (MFRC522::StatusCode)NULL;
        uint8_t uStatus = Hardware::reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 2, &key, &(Hardware::reader.uid));

        if (uStatus != MFRC522::STATUS_OK)
            return 0;

        /* Reading data from the Block */
        uStatus = Hardware::reader.MIFARE_Read(2, readBlockData, &bufferLen);
        if (uStatus != MFRC522::STATUS_OK)
            return 0;
        else
        {
            for (int j = 0; j < 16; j++)
            {
                if (blockData[j] != readBlockData[j])
                {
                    j = 17;
                    return 0;
                }
                if (j == 15)
                    return 1;
            }
        }
        return 0;
    }

    bool tagPresent()
    {
        if (!Hardware::reader.PICC_IsNewCardPresent())
        {
            if (repeatFlagPresent)
            {
                repeatFlagPresent = 0;
                return 1;
            }
            return 0;
        }
        if (Hardware::reader.PICC_ReadCardSerial())
        {
            if (Hardware::reader.PICC_IsNewCardPresent())
            {
            }
            if (!Hardware::reader.PICC_IsNewCardPresent())
            {
                if (repeatFlagPresent)
                {
                    repeatFlagPresent = 0;
                    return 1;
                }
                return 0;
            }

            /* Select one of the cards */
            if (!Hardware::reader.PICC_ReadCardSerial())
            {
                if (repeatFlagPresent)
                {
                    repeatFlagPresent = 0;
                    return 1;
                }
                return 0;
            }
            repeatFlagPresent = 1;
            return 1;
        }
        return 0;
    }

    uint32_t getTagUID()
    {
        status = (MFRC522::StatusCode)NULL;
        uint8_t uStatus = Hardware::reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 2, &key, &(Hardware::reader.uid));

        if (uStatus != MFRC522::STATUS_OK)
            return TagUID;

        /* Reading data from the Block */
        uStatus = Hardware::reader.MIFARE_Read(2, readBlockData, &bufferLen);
        if (uStatus != MFRC522::STATUS_OK)
            return 0;
        else
        {
            unsigned long tempUID[4] = {0};
            for (uint8_t i = 0; i < Hardware::reader.uid.size; i++)
                tempUID[i] = Hardware::reader.uid.uidByte[i];

            volatile unsigned long UID = 0;
            UID |= (tempUID[0] << 24);
            UID |= (tempUID[1] << 16);
            UID |= tempUID[2] << 8;
            UID |= tempUID[3];

            return UID;
        }
    }
} // namespace


namespace RFID
{
    EdgeDetection tagAvailable(&tagAvailableVal);


    void updateTagPresence()
    {
        tagAvailableVal = tagPresent();
    }

    void updateTagAttributes()
    {
        if(checkMaster())
        {
            isMaster = 1;
        }

        if(tagAvailable.getEdgePos())
        {
            uid = getTagUID();
        }

        if ((!tagAvailable.getEdgeNeg()) && (!tagAvailable.getActState()))
        {
            isMaster = 0;
        }

        for (uint8_t i = 0; i < 6; i++)
            key.keyByte[i] = 0xFF;
    }

    bool getMasterState() { return isMaster; }

    uint32_t getUID() { return uid; }
} // namespace RFID