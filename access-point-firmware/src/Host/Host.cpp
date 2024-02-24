// =============================================================================
// Template-Project | Signal
// =============================================================================

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Host.h"

#include "../hardware.h"
#include "Signal.h"


ESP8266WebServer server(80);

namespace
{
    const String jsonString = "{\"deviceID\":\"" + deviceID + "\"}";
}

namespace Host
{
    void init()
    {
        server.on("/ping", []()
        {
            server.sendHeader("Access-Control-Allow-Origin", "*"); // Add this line
            server.send(200, "text/plain", "pong");
            Signal::ping();
        });

        server.on("/getdeviceid", []()
        {
            server.sendHeader("Access-Control-Allow-Origin", "*"); // Add this line
            server.send(200, "application/json", jsonString);
        });

        server.begin();
    }

    void handleClient()
    {
        server.handleClient();
    }
}