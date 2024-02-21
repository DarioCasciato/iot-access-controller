// =============================================================================
// Arduino OLED Testing | API Handler
// =============================================================================

#ifndef UTILS_CONNECTIVITY_APIHANDLER_
#define UTILS_CONNECTIVITY_APIHANDLER_

#if defined(ESP8266) || defined(ESP32)
#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

namespace APIHandler
{
    /// @brief Sends a GET request to the specified URL.
    /// @param url The URL to send the request to.
    /// @param response The response from the server.
    /// @return True if the request was successful, false otherwise.
    ///
    bool get(String url, String& response);

    /// @brief Sends a GET request to the specified URL.
    /// @param url The URL to send the request to.
    /// @param response The response from the server.
    /// @return True if the request was successful, false otherwise.
    ///
    bool get(const char* url, String& response);

    /// @brief Sends a POST request to the specified URL.
    /// @param url The URL to send the request to.
    /// @param payload The payload to send.
    /// @param response The response from the server.
    /// @return True if the request was successful, false otherwise.
    ///
    bool post(String url, String payload, String& response);

    /// @brief Sends a POST request to the specified URL.
    /// @param url The URL to send the request to.
    /// @param payload The payload to send.
    /// @param response The response from the server.
    /// @return True if the request was successful, false otherwise.
    ///
    bool post(const char* url, String payload, String& response);


    namespace JSON
    {
        /// @brief Extracts the value at the specified path from the JSON response.
        /// @param jsonResponse The JSON response.
        /// @param path The path to the value. (Example: "value/innerValue")
        /// @param result Where to store the result.
        /// @return True if the value was extracted successfully, false otherwise.
        ///
        template <typename T>
        bool extract(const String& jsonResponse, const String& path, T* result);
    }
}

#endif // defined(ESP8266) || defined(ESP32)

#endif // UTILS_CONNECTIVITY_APIHANDLER_
