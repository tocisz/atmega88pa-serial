#ifndef NOKIA_DISPLAY_H
#define NOKIA_DISPLAY_H

#include <atmel_start.h>

#include <avr/pgmspace.h>
extern const uint8_t FONT[] PROGMEM;

#define LCDWIDTH 84
#define LCDHEIGHT 48

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

class NokiaTextDisplay {
public:
  void start() {
    N_SCK_set_dir(PORT_DIR_OUT);
  	N_MOSI_set_dir(PORT_DIR_OUT);
  	N_SCE_set_level(false);
  }

  void stop() {
    N_SCE_set_level(true);
  }

  void control() {
    N_D_C_set_level(false);
  }

  void data() {
    N_D_C_set_level(true);
  }

  void send(uint8_t b) {
    SPDR = b;
  	while(!(SPSR & (1<<SPIF)));
  }

  void goto_y_x(uint8_t y, uint8_t x) {
    control();
  	send(PCD8544_SETYADDR | y);
  	send(PCD8544_SETXADDR | x);
  }

  void clear() {
    goto_y_x(0, 0);
  	data();
  	for (uint16_t i = 0; i < LCDWIDTH*LCDHEIGHT/8; ++i) {
  		send(0);
  	}
  }

  void init(uint8_t bias, uint8_t contrast);
  void print(char c);
  void print_str(const char *s);

private:
  void _print(char c) {
    const uint8_t *ptr = FONT + (c - 32)*6;
  	for (uint8_t i = 0; i < 6; ++i) {
  		send(pgm_read_byte(ptr++));
  	}
  }
};

#endif
