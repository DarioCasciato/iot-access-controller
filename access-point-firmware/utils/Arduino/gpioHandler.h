// =============================================================================
// Utils | GPIO Handler
// =============================================================================

#include <stdint.h>

#ifndef UTILS_GPIO_HANDLER_
#define UTILS_GPIO_HANDLER_

// Forward declaration
namespace Hardware { enum class Port : uint8_t; }

namespace GPIO
{
    /// Get the state of a digital port.
    ///
    /// @param port The digital port to read.
    /// @return true if the port is high, false if low.
    ///
    bool getPort(Hardware::Port port);

    /// Get the analog value of a port.
    ///
    /// @param port The analog port to read.
    /// @return The analog value in the range 0-1023.
    ///
    uint16_t getAnalogPort(Hardware::Port port);

    /// Set the state of a digital port.
    ///
    /// @param port The digital port to set.
    /// @param value The value to set (true for high, false for low).
    ///
    void setPort(Hardware::Port port, bool value);

    /// Set the analog value of a port.
    ///
    /// @param port The analog port to set.
    /// @param value The analog value in the range 0-255.
    ///
    void setAnalogPort(Hardware::Port port, uint8_t value);
} // namespace GPIO

#endif // UTILS_GPIO_HANDLER_
