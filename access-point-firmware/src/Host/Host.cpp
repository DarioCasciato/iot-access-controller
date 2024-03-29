// =============================================================================
// Template-Project | Signal
// =============================================================================

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Host.h"

#include "../hardware.h"
#include "Signal.h"
#include "RFID.h"
#include "Timer.h"
#include "Logging.h"
#include "EdgeDetection.h"
#include "configurations.h"
#include "Connectivity.h"


ESP8266WebServer server(80);

namespace
{
    const String jsonString = "{\"deviceID\":\"" + getDeviceID() + "\"}";
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

        server.on("/setserverip", []()
        {
            // The "on setserverip" endpoint recieves a JSON payload with the new server IP as (ServerIP)
            String payload = server.arg("plain");
            String serverIP;

            APIHandler::JSON::extract(payload, "ServerIP", &serverIP);

            setServerIP(serverIP);
        });

        server.on("/getcarduid", []()
        {
            Logging::log("getcarduid");
            Timer timer;
            timer.start();

            Hardware::led.SetPixelColor(0, RgbwColor(0, 0, 255, 0));
            Hardware::led.Show();

            while(1)
            {
                noTone((uint8_t)Hardware::Port::Buzzer);
                RFID::updateTagPresence();
                EdgeDetection::updateEdges();
                RFID::updateTagUID();

                if(RFID::tagAvailable.getEdgePos())
                {
                    const uint32_t uid = RFID::getUID();
                    const String uidPayload = "{\"uid\":\"" + String(uid) + "\"}";
                    server.sendHeader("Access-Control-Allow-Origin", "*"); // Add this line
                    server.send(200, "text/plain", uidPayload);
                    Signal::cardRead();
                    break;
                }

                if(timer.elapsed(10000))
                {
                    server.sendHeader("Access-Control-Allow-Origin", "*"); // Add this line
                    server.send(200, "text/plain", "timeout");
                    Signal::connectionError();
                    break;
                }

                delay(5);
            }
        });

        server.begin();
    }

    void handleClient()
    {
        server.handleClient();
    }
}