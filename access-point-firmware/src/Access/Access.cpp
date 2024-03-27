// =============================================================================
// Template-Project | Access
// =============================================================================

#include <Arduino.h>
#include "Access.h"
#include "../configurations.h"
#include "../../utils/Connectivity/Connectivity.h"


namespace
{
    bool validateResponse(const String& response)
    {
        if (response == "granted")
        {
            return true;
        }
        else if (response == "denied")
        {
            return false;
        }
        else
        {
            return false;
        }
    }
}

namespace Access
{
    ResponseStatus request(const String& uid)
    {
        if(getServerIP() == "")
        {
            return ResponseStatus::ConnectionFailed;
        }

        String serverUrl = "http://" + getServerIP() + "/api/access-request";
        String payload = "{\"deviceId\":\"" + getDeviceID() + "\",\"uid\":\"" + uid + "\"}";
        String response;

        // check wifi connection
        if (!Wifi::isConnected())
        {
            Wifi::establish();
        }

        if (!APIHandler::post(serverUrl, payload, response))
        {
            return ResponseStatus::ConnectionFailed;
        }

        // Parse JSON response
        String accessDecision;
        if (!APIHandler::JSON::extract(response, "accessDecision", &accessDecision))
        {
            return ResponseStatus::ParseError;
        }

        if (validateResponse(accessDecision))
        {
            // Handle access granted (e.g., open a door)
            return ResponseStatus::Ok;
        }
        else
        {
            // Handle access denied
            return ResponseStatus::Denied;
        }
    }

} // namespace Access