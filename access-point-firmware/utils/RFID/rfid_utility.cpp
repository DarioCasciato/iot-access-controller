#include <Arduino.h>
#include "hardware.h"
#include "rfid_utility.h"

uint8_t blockData[16] = {"MasterMediumCard"};
static uint8_t bufferLen = 18;
static uint8_t readBlockData[18];
static bool repeatFlagPresent = 0;
static uint32_t TagUID = 0;


bool Tag::checkMaster()
{
    Hardware::status = NULL;
    uint8_t status = Hardware::reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 2, &Hardware::key, &(Hardware::reader.uid));

    if (status != MFRC522::STATUS_OK)
        return 0;

    /* Reading data from the Block */
    status = Hardware::reader.MIFARE_Read(2, readBlockData, &bufferLen);
    if (status != MFRC522::STATUS_OK)
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

bool Tag::tagPresent()
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

uint32_t Tag::getUID()
{
    Hardware::status = NULL;
    uint8_t status = Hardware::reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 2, &Hardware::key, &(Hardware::reader.uid));

    if (status != MFRC522::STATUS_OK)
        return TagUID;

    /* Reading data from the Block */
    status = Hardware::reader.MIFARE_Read(2, readBlockData, &bufferLen);
    if (status != MFRC522::STATUS_OK)
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