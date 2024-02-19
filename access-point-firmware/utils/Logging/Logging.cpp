// =============================================================================
// Utils | Logging
// =============================================================================

#include "logging.h"
#include <Arduino.h>
#include <stdarg.h>  // For va_list and va_start


void Logging::log(const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}
