// =============================================================================
// Template-Project | RFID
// =============================================================================

#include <stdint.h>
#include <MFRC522.h>
#include "EdgeDetection.h"
#include "../hardware.h"
#include "RFID.h"
#include "Logging.h"

namespace
{
    uint32_t uid = 0;
    uint8_t tagAvailableVal = false;

    bool tagPresent()
    {
        static bool lastState = false;
        bool currentState = Hardware::reader.PICC_IsNewCardPresent() &&
                            Hardware::reader.PICC_ReadCardSerial();

        bool diff = currentState ^ lastState;
        lastState = currentState;
        return diff;
    }

    uint32_t getCardUID()
    {
        uint32_t uid = 0;
        for (byte i = 0; i < 4; i++)
        {
            uid <<= 8;
            uid |= Hardware::reader.uid.uidByte[i];
        }

        Hardware::reader.PICC_HaltA();
        Hardware::reader.PCD_StopCrypto1();
        return uid;
    }
} // namespace


namespace RFID
{
    EdgeDetection tagAvailable(&tagAvailableVal);

    void updateTagPresence()
    {
        tagAvailableVal = tagPresent();
    }

    void updateTagUID()
    {
        static bool negativePulse = false;

        if (negativePulse) // If a negative edge was detected in the previous iteration, reset UID and flag
        {
            uid = 0;
            negativePulse = false;
            return;
        }

        // If a positive edge is detected, read the UID
        if (tagAvailable.getEdgePos())
        {
            uid = getCardUID();
            negativePulse = false;
        }
        else if (tagAvailable.getEdgeNeg())
        {
            negativePulse = true;
        }
    }

    uint32_t getUID() { return uid; }
} // namespace RFID