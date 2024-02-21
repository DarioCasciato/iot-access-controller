// =============================================================================
// Shelly1 | API Handler
// =============================================================================

#if defined(ESP8266) || defined(ESP32)

#include "APIHandler.h"
#include "Logging.h"
#include "../ESPWiFi/espWiFi.h"

using namespace ArduinoJson;


namespace APIHandler
{
    bool get(String url, String& response)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Logging::log("WIFI: Not connected!");
            return false;
        }

        HTTPClient http;
        WiFiClient client;

        http.begin(client, url); // Specify the URL
        int httpCode = http.GET(); // Make the request

        if (httpCode > 0)
        { // Check the returning code
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                response = http.getString(); // Get the request response payload
                http.end(); // Close connection
                return true;
            }
        }
        else
        {
            Logging::log("WIFI: GET request failed: %s", http.errorToString(httpCode).c_str());
        }

        http.end(); // Close connection
        return false;
    }

    bool get(const char* url, String& response)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Logging::log("WIFI: Not connected!");
            return false;
        }

        HTTPClient http;
        WiFiClient client;

        http.begin(client, url); // Specify the URL
        int httpCode = http.GET(); // Make the request

        if (httpCode > 0)
        { // Check the returning code
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                response = http.getString(); // Get the request response payload
                http.end(); // Close connection
                return true;
            }
        }
        else
        {
            Logging::log("WIFI: GET request failed: %s", http.errorToString(httpCode).c_str());
        }

        http.end(); // Close connection
        return false;
    }

    bool post(String url, String payload, String& response)
    {
        if (!Wifi::isConnected())
        {
            Wifi::establish();
        }

        HTTPClient http;
        WiFiClient client;

        http.begin(client, url); // Specify the URL
        http.addHeader("Content-Type", "application/json"); // Specify the content-type header

        int httpCode = http.POST(payload); // Make the request

        if (httpCode > 0) // Check the returning code
        {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                response = http.getString(); // Get the response payload
                http.end(); // Close connection
                return true;
            }
        }
        else
        {
            Logging::log("WIFI: POST request failed: %s", http.errorToString(httpCode).c_str());
        }

        http.end(); // Close connection
        return false;
    }

    bool post(const char* url, String payload, String& response)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Logging::log("WIFI: Not connected!");
            return false;
        }

        HTTPClient http;
        WiFiClient client;

        http.begin(client, url); // Specify the URL
        http.addHeader("Content-Type", "application/json"); // Specify the content-type header

        int httpCode = http.POST(payload); // Make the request

        if (httpCode > 0) // Check the returning code
        {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                response = http.getString(); // Get the response payload
                http.end(); // Close connection
                return true;
            }
        }
        else
        {
            Logging::log("WIFI: POST request failed: %s", http.errorToString(httpCode).c_str());
        }

        http.end(); // Close connection
        return false;
    }


    namespace JSON
    {
        template <typename T>
        bool extract(const String& jsonResponse, const String& path, T* result)
        {
            StaticJsonDocument<1024> doc;

            DeserializationError error = deserializeJson(doc, jsonResponse);
            if (error)
            {
                Logging::log("JSON: deserializeJson() failed: %s", error.f_str());
                Logging::log("JSON: %s", jsonResponse.c_str());
                return false;
            }

            JsonVariant value = doc;
            int start = 0;
            int end = path.indexOf('/', start);

            while (end != -1)
            {
                String key = path.substring(start, end);
                value = value[key];
                start = end + 1;
                end = path.indexOf('/', start);
            }

            // Get the last part of the path
            String lastKey = path.substring(start);
            value = value[lastKey];

            if (!value.isNull() && value.is<T>())
            {
                *result = value.as<T>();
                return true;
            }
            else
            {
                Logging::log("JSON: Path not found or type mismatch");
                return false;
            }
        }
    }
} // namespace APIHandler


// Explicit template instantiation for types you expect to use.
template bool APIHandler::JSON::extract<bool>(const String&, const String&, bool*);
template bool APIHandler::JSON::extract<String>(const String&, const String&, String*);

#endif // defined(ESP8266) || defined(ESP32)
