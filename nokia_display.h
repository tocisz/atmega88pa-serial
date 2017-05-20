#ifndef NOKIA_DISPLAY_H
#define NOKIA_DISPLAY_H

#include <atmel_start.h>
#include <string.h>
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
  NokiaTextDisplay() : x(0), y(0), scroll(true) {}

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

  void goto_y_x(uint8_t _y, uint8_t _x) {
    control();
    send(PCD8544_SETYADDR | _y);
    send(PCD8544_SETXADDR | _x*6);

    y = _y;
    x = _x;
  }

  void clear() {
    memset(buffer, ' ', buffer_length);
    redraw();
    goto_y_x(0, 0);
  }

  uint8_t get_x() {
    return x;
  }

  uint8_t get_y() {
    return y;
  }

  void set_scroll(bool _scroll) {
    scroll = _scroll;
  }

  void init(uint8_t bias, uint8_t contrast);
  void print(char c);
  void print(const char *s);
  void redraw();
  void do_scroll();

private:
  void _print(char c) {
    if (c == '\r') {
      goto_y_x(y, 0);

    } else if ((c == '\n') && (y != 5)) {
      goto_y_x(y+1, 0);

    } else if ((c == '\n') && (y == 5)) {
      if (scroll) {
        do_scroll();
      } else {
        goto_y_x(0, 0);
      }

    } else if ((c >= 32) && (c <= 128)) {

      buffer[y*14 + x] = c;
      const uint8_t *ptr = FONT + (c - 32)*6;
      for (uint8_t i = 0; i < 6; ++i) {
        send(pgm_read_byte(ptr++));
      }

      x++;
      if (x == 14) {
        x = 0;
        y++;
      }

      if (y == 6) {
        goto_y_x(0, 0);
      }

    }
  }

  // current position
  uint8_t x, y;

  // current buffer
  static const size_t buffer_length = 14*6;
  char buffer[buffer_length];

  bool scroll;

};

#endif
