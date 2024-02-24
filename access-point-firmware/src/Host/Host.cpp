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


namespace Host
{
    void init()
    {
        server.on("/ping", []()
        {
            Signal::ping();
            server.send(200, "text/plain", "pong");
        });

        server.on("/getdeviceid", []()
        {
            String jsonString = "{\"deviceID\":\"" + deviceID + "\"}";
            server.send(200, "application/json", jsonString);
        });

        server.begin();
    }

    void handleClient()
    {
        server.handleClient();
    }
}