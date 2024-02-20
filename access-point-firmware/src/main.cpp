// =============================================================================
// Template-Project | Main
// =============================================================================

#include <Arduino.h>
#include "hardware.h"
#include "EdgeDetection.h"
#include "configurations.h"
#include "state.h"
#include "Flash/Flash.h"
#include "RFID.h"


namespace
{

void refreshData()
{
    Hardware::updateHardware();
    RFID::updateTagPresence();
    EdgeDetection::updateEdges();
    RFID::updateTagAttributes();
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


