// =============================================================================
// Template-Project | Configurations
// =============================================================================

#include "user_interface.h"
#include "Flash/Flash.h"

#ifndef _ACCESS_POINT_CONFIGURATIONS_
#define _ACCESS_POINT_CONFIGURATIONS_

namespace
{
    String serverIP = "";
    const String deviceID = String(system_get_chip_id(), HEX);
}


String getDeviceID()
{
    return deviceID;
}

void setServerIP(const String& ip)
{
    Flash::configStore.write((uint8_t)Flash::ID::ServerIP, ip);
}

String getServerIP()
{
    String ip;
    Flash::configStore.read((uint8_t)Flash::ID::ServerIP, ip);
    return ip;
}


#endif // _ACCESS_POINT_CONFIGURATIONS_
