// =============================================================================
// Template-Project | Main
// =============================================================================

#include <Arduino.h>
#include "hardware.h"
#include "EdgeDetection.h"
#include "configurations.h"
#include "state.h"
#include "Flash/Flash.h"
#include "rfid_utility.h"


namespace
{
    bool isMaster = 0;
    uint32_t uid = 0;

    uint8_t tagAvailableVal = 0;
    EdgeDetection tagAvailable(&tagAvailableVal);

    void refreshData()
    {
        tagAvailableVal = Tag::tagPresent();
        EdgeDetection::updateEdges();

        if(Tag::checkMaster())
        {
            isMaster = 1;
        }

        if(tagAvailable.getEdgePos())
        {
            uid = Tag::getUID();
        }

        if ((!tagAvailable.getEdgeNeg()) && (!tagAvailable.getActState()))
        {
            isMaster = 0;
        }

        for (uint8_t i = 0; i < 6; i++)
            Hardware::key.keyByte[i] = 0xFF;

        //Hardware::updateHardware();
    }

} // namespace



//------------------------------------------------------------------------------

void setup()
{
#if defined(ESP8266) || defined(ESP32)
    ESP.wdtEnable(WDTO_1S);
    Serial.begin(115200);
#else
    Serial.begin(9600);
#endif

    Flash::init();
}

void loop()
{
    for (;;)
    {
        refreshData();

        State::stateDriver();
    }
}

//------------------------------------------------------------------------------


