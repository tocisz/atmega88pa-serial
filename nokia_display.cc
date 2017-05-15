#include "nokia_display.h"

#include "apple_font.h"

void NokiaTextDisplay::init(uint8_t bias, uint8_t contrast) {
  start();
	// RST is set LOW in system_init()
	// N_RST_set_level(false);
	// _delay_ms(100);
	N_RST_set_level(true);
	control();
	send(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	send(PCD8544_SETBIAS | bias);
	send(PCD8544_FUNCTIONSET);

	send(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	if (contrast > 0x7f) contrast = 0x7f;
	send(PCD8544_SETVOP | contrast);
	send(PCD8544_FUNCTIONSET);

	send(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
	clear();
}

void NokiaTextDisplay::print(char c) {
  data();
	_print(c);
}

void NokiaTextDisplay::print(const char *s) {
  char c;
	data();
	while ((c = *(s++)) != 0) {
		_print(c);
	}
}
