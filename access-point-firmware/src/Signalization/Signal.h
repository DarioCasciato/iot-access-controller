// =============================================================================
// Template-Project | Signal
// =============================================================================

#ifndef _ACCESS_POINT_SIGNAL_
#define _ACCESS_POINT_SIGNAL_

#include <stdint.h>
#include <Arduino.h>


namespace Signal
{
    /// @brief Signalization for granted access
    ///
    void granted();

    /// @brief Signalization for denied access
    ///
    void denied();

    /// @brief Signalization for connection established
    ///
    void connectionEstablished();

    /// @brief Signalization for connection error
    ///
    void connectionError();

    /// @brief signalization for ping
    ///
    void ping();

    /// @brief signalization for card read
    ///
    void cardRead();
}


#endif // _ACCESS_POINT_SIGNAL_