// =============================================================================
// Template-Project | OLED u8g2
// =============================================================================

#ifdef U8G2_H

#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define FONT u8g2_font_6x10_tf

#ifndef _TEMPLATE_PROJECT_OLED_
#define _TEMPLATE_PROJECT_OLED_

// This Class is used (mostly probably) for the ESP8266 with the onboard OLED u8g2
class OLED
{
    public:
        /// @brief Construct a new OLED object
        ///
        /// @param pin_scl pin number for the SCL pin
        /// @param pin_sda pin number for the SDA pin
        OLED(uint8_t pin_scl, uint8_t pin_sda);

        /// @brief Clear the OLED screen
        void clear();

        /// @brief Write a message to the OLED screen
        ///
        /// @param format The message that needs to be printed
        void write(uint8_t line, const char *format, ...);

    private:
        U8G2_SSD1306_128X64_NONAME_F_SW_I2C screen;
};

#endif // _TEMPLATE_PROJECT_OLED_

#endif // U8G2_H