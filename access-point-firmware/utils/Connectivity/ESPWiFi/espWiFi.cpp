// =============================================================================
// Template-Project | Main
// =============================================================================

#if defined(ESP8266) || defined(ESP32)

#include "espWiFi.h"
#include "Logging.h"


namespace
{
    const char* fixSsid = "SSID";
    const char* fixPassword = "PASSWORD";
} // namespace



bool Wifi::establish()
{
    // Connect to the WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(fixSsid, fixPassword);

    for(uint8_t i = 0; i < 10; i++)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Logging::log("Connected to WiFi!");
            return true;
        }

        delay(1000);
        Logging::log("Connecting to WiFi...");
    }

    // If the connection failed, log an error
    Logging::log("Failed to connect to WiFi!");
    return false;
}

bool Wifi::establish(String ssid, String password)
{
    // Connect to the WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    for(uint8_t i = 0; i < 10; i++)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Logging::log("Connected to WiFi!");
            return true;
        }

        delay(1000);
        Logging::log("Connecting to WiFi...");
    }

    // If the connection failed, log an error
    Logging::log("Failed to connect to WiFi!");
    return false;
}

bool Wifi::establish(char* ssid, char* password)
{
    // Connect to the WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    for(uint8_t i = 0; i < 10; i++)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Logging::log("Connected to WiFi!");
            return true;
        }

        delay(1000);
        Logging::log("Connecting to WiFi...");
    }

    // If the connection failed, log an error
    Logging::log("Failed to connect to WiFi!");
    return false;
}

void Wifi::disconnect()
{
    WiFi.disconnect();
}

bool Wifi::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

#endif
