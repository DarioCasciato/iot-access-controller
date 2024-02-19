// =============================================================================
// arduino-access-system | Dario Casciato
// =============================================================================

#ifndef ARDUINO_ACCESS_SYSTEM_SK6812_
#define ARDUINO_ACCESS_SYSTEM_SK6812_

#include <util/delay.h>
#include <stdint.h>
#include <Arduino.h>

#ifndef F_CPU
#define  F_CPU 16000000UL
#endif

// SK6812 LED structure representing RGBW values
struct RGBW {
	uint8_t g; // 0
	uint8_t r; // 1
	uint8_t b; // 2
	uint8_t w; // 3
};

class SK6812 {
public:
	/// @brief SK6812 constructor
	///
	/// @param num_led The number of LEDs in the SK6812 chain
	SK6812(uint16_t num_led);

	/// @brief SK6812 destructor
	~SK6812();

	/// @brief Set the output pin for the SK6812 LEDs
	///
	/// @param pin The pin number to which the SK6812 LEDs are connected
	void set_output(uint8_t pin);

	/// @brief Get the RGBW value of a specific LED
	///
	/// @param index The index of the LED
	/// @return The RGBW value of the LED
	RGBW get_rgbw(uint16_t index);

	/// @brief Set the RGBW value of a specific LED
	///
	/// @param index The index of the LED
	/// @param px_value The RGBW value to be set
	/// @return 0 if successful, 1 if the index is out of range
	uint8_t set_rgbw(uint16_t index, RGBW px_value);

	/// @brief Set the RGBW value of a specific LED and sync it
	///
	/// @param index The index of the LED
	/// @param color The RGBW value to be set
	void set_color(uint16_t index, RGBW color);

	/// @brief Synchronize the LED data to the physical LEDs
	void sync();

private:
	uint16_t _count_led;
	RGBW *_pixels;

	const volatile uint8_t *_port;
	volatile uint8_t *_port_reg;
	uint8_t _pin_mask;

	void sendarray_mask(uint8_t *array, uint16_t length, uint8_t pinmask, uint8_t *port, uint8_t *portreg);

};

#endif // ARDUINO_ACCESS_SYSTEM_SK6812_