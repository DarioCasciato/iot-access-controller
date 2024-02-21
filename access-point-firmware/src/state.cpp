// =============================================================================
// Template-Project | State
// =============================================================================

#include <inttypes.h>
#include "state.h"
#include "hardware.h"
#include "Timer.h"
#include "Flash.h"
#include "Logging.h"
#include "RFID.h"
#include "../utils/Connectivity/APIHandler/APIHandler.h"
#include "../utils/Connectivity/ESPWiFi/espWiFi.h"

void checkAccess(const String& uid);

//------------------------------------------------------------------------------

namespace State
{
    States state = States::st_idle;

    void stateDriver()
    {
        switch (State::state)
        {
        case State::st_idle: stateIdle(); break;
        case State::st_end: stateEnd(); break;

        default:    // catch invalid state (implement safety backup)
        goto exception;
            break;
        }

        return;

        exception:
            for(;;) { delay(1000); }
    }


    // State implementations (can also be moved to separate files)
    void stateIdle()
    {
        if(RFID::tagAvailable.getEdgePos())
        {
            Logging::log("Tag detected! UID: %x", RFID::getUID());

            Logging::log("Checking access...");
            checkAccess(String(RFID::getUID()));
        }
    }

    void stateEnd()
    {
        delay(1000);
    }
} // namespace State

//------------------------------------------------------------------------------

// Other Functions




void checkAccess(const String& uid)
{
    String serverUrl = "http://192.168.1.153:3000/check-access"; // Replace with your actual server URL
    String payload = "{\"uid\":\"" + uid + "\"}";
    String response;

    // check wifi connection
    if (!Wifi::isConnected())
    {
        Wifi::establish();
    }

    if (APIHandler::post(serverUrl, payload, response))
    {
        // Parse JSON response
        bool accessGranted;
        String accessLevel;
        if (APIHandler::JSON::extract(response, "accessGranted", &accessGranted))
        {
            if (accessGranted)
            {
                APIHandler::JSON::extract(response, "accessLevel", &accessLevel); // Optional: Extract access level if needed
                Logging::log("Access Granted! Level: %s", accessLevel.c_str());
                // Handle access granted (e.g., open a door)
            }
            else
            {
                Logging::log("Access Denied.");
                // Handle access denied
            }
        }
        else
        {
            Logging::log("Failed to parse response.");
        }
    }
    else
    {
        Logging::log("Failed to send request or receive response.");
    }
}