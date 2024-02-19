// =============================================================================
// Template-Project | ESP-WiFi
// =============================================================================

#ifndef UTILS_CONNECTIVITY_ESPWIFI_
#define UTILS_CONNECTIVITY_ESPWIFI_

#include <Arduino.h>

#if defined(ESP8266) || defined(ESP32)
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

namespace Wifi
{
    /// @brief Establishes a connection to the Wi-Fi network with the credentials specified in the espWiFi.cpp.
    /// @return True if the connection was successful, false otherwise.
    ///
    bool establish();

    /// @brief Establishes a connection to the Wi-Fi network.
    /// @param ssid The SSID of the Wi-Fi network.
    /// @param password The password of the Wi-Fi network.
    /// @return True if the connection was successful, false otherwise.
    ///
    bool establish(String ssid, String password);

    /// @brief Establishes a connection to the Wi-Fi network.
    /// @param ssid The SSID of the Wi-Fi network.
    /// @param password The password of the Wi-Fi network.
    /// @return True if the connection was successful, false otherwise.
    ///
    bool establish(char* ssid, char* password);

    /// @brief Disconnects from the Wi-Fi network.
    ///
    void disconnect();

    /// @brief Checks if the device is connected to the Wi-Fi network.
    /// @return True if the device is connected to the Wi-Fi network, false otherwise.
    ///
    bool isConnected();
} // namespace WiFi

#endif // defined(ESP8266) || defined(ESP32)

#endif // UTILS_CONNECTIVITY_ESPWIFI_
