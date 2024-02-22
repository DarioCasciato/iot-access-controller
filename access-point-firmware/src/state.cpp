// =============================================================================
// Template-Project | State
// =============================================================================

#include <inttypes.h>
#include "state.h"
#include "hardware.h"
#include "Logging.h"
#include "RFID.h"
#include "Signal.h"
#include "Access.h"


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
        if(RFID::tagAvailable.getEdgePos())
        {
            Logging::log("\nTag detected! UID: %x", RFID::getUID());

            Logging::log("Checking access...");
            Access::ResponseStatus response = Access::request(String(RFID::getUID()));

            switch (response)
            {
            case Access::ResponseStatus::Ok:
                Logging::log("Access granted!");
                Signal::granted();
                break;

            case Access::ResponseStatus::Denied:
                Logging::log("Access denied!");
                Signal::denied();
                break;

            case Access::ResponseStatus::ConnectionFailed:
                Logging::log("Connection failed!");
                Signal::connectionError();
                break;

            case Access::ResponseStatus::ParseError:
                Logging::log("Parse error!");
                Signal::connectionError();
                break;
            }
        }
    }

    void stateEnd()
    {
        delay(1000);
    }
} // namespace State

//------------------------------------------------------------------------------

// Other Functions

