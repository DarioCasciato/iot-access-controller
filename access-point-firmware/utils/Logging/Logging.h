// =============================================================================
// Utils | Logging
// =============================================================================

#include <Arduino.h>
#include <stdarg.h>

#ifndef UTILS_LOGGING_
#define UTILS_LOGGING_

namespace Logging
{
    /// @brief Prints a message to the serial monitor
    ///
    /// @param message The message that needs to be printed
    ///
    void log(const char *format, ...);
}

#endif // UTILS_LOGGING_
