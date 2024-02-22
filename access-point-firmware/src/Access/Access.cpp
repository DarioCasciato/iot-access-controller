// =============================================================================
// Template-Project | Access
// =============================================================================

#include <Arduino.h>
#include "Access.h"
#include "../../utils/Connectivity/Connectivity.h"


namespace Access
{
    ResponseStatus request(const String& uid)
    {
        String serverUrl = "http://192.168.1.153:3000/check-access"; // Replace with your actual server URL
        String payload = "{\"uid\":\"" + uid + "\"}";
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
        bool accessGranted;
        String accessLevel;
        if (!APIHandler::JSON::extract(response, "accessGranted", &accessGranted))
        {
            return ResponseStatus::ParseError;
        }

        if (accessGranted)
        {
            APIHandler::JSON::extract(response, "accessLevel", &accessLevel); // Optional: Extract access level if needed
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