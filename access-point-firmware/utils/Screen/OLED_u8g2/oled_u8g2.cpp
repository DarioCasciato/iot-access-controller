// =============================================================================
// Template-Project | OLED u8g2
// =============================================================================

#include "oled_u8g2.h"

#define LINE_HEIGHT 12

OLED::OLED(uint8_t pin_scl, uint8_t pin_sda) : screen(U8G2_R0, pin_scl, pin_sda, U8X8_PIN_NONE)
{
    screen.begin();
    screen.setFont(FONT);
}

void OLED::clear()
{
    screen.clear();
}

void OLED::write(uint8_t line, const char *format, ...)
{
    char buffer[64];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    screen.drawStr(0, line * LINE_HEIGHT + 12, buffer); // 12 is the offset for the first line
    screen.sendBuffer();
}