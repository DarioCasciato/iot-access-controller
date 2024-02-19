// =============================================================================
// Utils | GPIO
// =============================================================================

#include <stdint.h>
#include <Arduino.h>

#ifndef UTILS_GPIO_
#define UTILS_GPIO_

#include "../../src/hardware.h"


namespace GPIO
{
    /// Enumeration of digital and analog ports.
    ///
    /// This enum represents the available digital and analog ports.
    /// Digital ports are labeled as D0, D1, etc., while analog ports
    /// are labeled as A0, A1, etc.
#ifdef ARDUINO_AVR_NANO
    enum class Port : uint8_t
    {
        D0 = 0,
        D1 = 1,
        D2 = 2,
        D3 = 3,
        D4 = 4,
        D5 = 5,
        D6 = 6,
        D7 = 7,
        D8 = 8,
        D9 = 9,
        D10 = 10,
        D11 = 11,
        D12 = 12,
        D13 = 13,
        D14 = 14,
        D15 = 15,
        D16 = 16,
        D17 = 17,
        D18 = 18,
        D19 = 19,
        A0 = A0,
        A1 = A1,
        A2 = A2,
        A3 = A3,
        A4 = A4,
        A5 = A5
    };
#endif

#ifdef ARDUINO_AVR_UNO
    enum class Port : uint8_t
    {
        D0 = 0,
        D1 = 1,
        D2 = 2,
        D3 = 3,
        D4 = 4,
        D5 = 5,
        D6 = 6,
        D7 = 7,
        D8 = 8,
        D9 = 9,
        D10 = 10,
        D11 = 11,
        D12 = 12,
        D13 = 13,
        D14 = 14,
        D15 = 15,
        D16 = 16,
        D17 = 17,
        D18 = 18,
        D19 = 19,
        A0 = A0,
        A1 = A1,
        A2 = A2,
        A3 = A3,
        A4 = A4,
        A5 = A5
    };
#endif

#ifdef ARDUINO_AVR_MEGA2560
    enum class Port : uint8_t
    {
        D0 = 0,
        D1 = 1,
        D2 = 2,
        D3 = 3,
        D4 = 4,
        D5 = 5,
        D6 = 6,
        D7 = 7,
        D8 = 8,
        D9 = 9,
        D10 = 10,
        D11 = 11,
        D12 = 12,
        D13 = 13,
        D14 = 14,
        D15 = 15,
        D16 = 16,
        D17 = 17,
        D18 = 18,
        D19 = 19,
        D20 = 20,
        D21 = 21,
        D22 = 22,
        D23 = 23,
        D24 = 24,
        D25 = 25,
        D26 = 26,
        D27 = 27,
        D28 = 28,
        D29 = 29,
        D30 = 30,
        D31 = 31,
        D32 = 32,
        D33 = 33,
        D34 = 34,
        D35 = 35,
        D36 = 36,
        D37 = 37,
        D38 = 38,
        D39 = 39,
        D40 = 40,
        D41 = 41,
        D42 = 42,
        D43 = 43,
        D44 = 44,
        D45 = 45,
        D46 = 46,
        D47 = 47,
        D48 = 48,
        D49 = 49,
        D50 = 50,
        D51 = 51,
        D52 = 52,
        D53 = 53,
        A0 = A0,
        A1 = A1,
        A2 = A2,
        A3 = A3,
        A4 = A4,
        A5 = A5,
        A6 = A6,
        A7 = A7,
        A8 = A8,
        A9 = A9,
        A10 = A10,
        A11 = A11,
        A12 = A12,
        A13 = A13,
        A14 = A14,
        A15 = A15
    };
#endif

#ifdef ESP8266
    enum class Port : uint8_t
    {
        D0 = 16,
        D1 = 5,
        D2 = 4,
        D3 = 0,
        D4 = 2,
        D5 = 14,
        D6 = 12,
        D7 = 13,
        D8 = 15,
        D9 = 3,
        D10 = 1,
        A0 = A0
    };
#endif

} // namespace GPIO


#endif // UTILS_GPIO_
