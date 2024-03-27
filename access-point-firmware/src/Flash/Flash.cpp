// =============================================================================
// Utils | FlashStorage
// =============================================================================

#include "Flash.h"
#include "Logging.h"

namespace Flash
{
    Layout flashLayout; // Definition of the global instance

    // add flash memory definition here
    // Example: RollStorage storage(&flashLayout.memory1[0], sizeof(flashLayout.memory1), sizeof(DataType));
    IDStorage configStore(&flashLayout.memory1[0], sizeof(flashLayout.memory1));

    // Add initializers here
    namespace
    {

    void initStorage()
    {
        // Example: storage.init();
        configStore.init();
    }

    }

    void clear()
    {
        // Example: storage.clear();
        configStore.clear();
    }


    void init()
    {
    #if defined(ESP8266) || defined(ESP32)
        EEPROM.begin(sizeof(Flash::Layout));
    #else
        EEPROM.begin();
    #endif

        initStorage();

        Logging::log("Flash memory initialized\n");
    }
} // namespace Flash
