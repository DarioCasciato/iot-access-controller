// =============================================================================
// Template-Project | State
// =============================================================================

#ifndef _ACCESS_POINT_STATE_
#define _ACCESS_POINT_STATE_

#include "hardware.h"

namespace State
{
    enum States : uint8_t
    {
        st_idle,
        st_error
    };

    /// @brief State driver function
    ///
    void stateDriver();

    /// @brief Handler for the Idle state
    ///
    void stateIdle();

    /// @brief Handler for the Error state
    ///
    void stateError();
} // namespace State

#endif // _ACCESS_POINT_STATE_
