// =============================================================================
// Template-Project | Signal
// =============================================================================

#ifndef _TEMPLATE_PROJECT_SIGNAL_
#define _TEMPLATE_PROJECT_SIGNAL_

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
}


#endif // _TEMPLATE_PROJECT_SIGNAL_