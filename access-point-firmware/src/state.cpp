// =============================================================================
// Template-Project | State
// =============================================================================

#include <inttypes.h>
#include "state.h"
#include "hardware.h"
#include "configurations.h"
#include "Timer.h"
#include "Flash.h"
#include "Logging.h"
#include "RFID.h"

bool tagPresent()
{
    static bool lastState = false;
    bool currentState = Hardware::reader.PICC_IsNewCardPresent() &&
                        Hardware::reader.PICC_ReadCardSerial();

    bool diff = currentState ^ lastState;
    lastState = currentState;
    return diff;
}

//------------------------------------------------------------------------------

namespace State
{
    States state = States::st_idle;

    void stateDriver()
    {
        switch (State::state)
        {
        case State::st_idle: stateIdle(); break;
        case State::st_end: stateEnd(); break;

        default:    // catch invalid state (implement safety backup)
        goto exception;
            break;
        }

        return;

        exception:
            for(;;) { delay(1000); }
    }


    // State implementations (can also be moved to separate files)
    void stateIdle()
    {

    }

    void stateEnd()
    {
        delay(1000);
    }
} // namespace State

//------------------------------------------------------------------------------

// Other Functions
