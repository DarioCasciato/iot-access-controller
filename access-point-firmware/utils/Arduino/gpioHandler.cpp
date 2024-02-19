// =============================================================================
// Utils | GPIO Handler
// =============================================================================

#include "gpioHandler.h"
#include "../../src/hardware.h"

bool GPIO::getPort(Hardware::Port port)
{
    return digitalRead((uint8_t)port);
}

uint16_t GPIO::getAnalogPort(Hardware::Port port)
{
    return analogRead((uint8_t)port);
}

void GPIO::setPort(Hardware::Port port, bool value)
{
    digitalWrite((uint8_t)port, value);
}

void GPIO::setAnalogPort(Hardware::Port port, uint8_t value)
{
    analogWrite((uint8_t)port, value);
}
