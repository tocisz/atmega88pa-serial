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
#define PCD8544_VERTICAL 0x02
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

class NokiaTextDisplay {
public:
  NokiaTextDisplay() : x(0), y(0), scroll(true) {
    cursor_delay = 0;
    cursor_timer = 0;
    cursor_visible = false;
    cursor_enabled = false;
  }

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

  void clear() {
    memset(buffer, ' ', buffer_length);
    redraw();
    _goto_y_x(0, 0);
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

  void show_cursor() {
    cursor_enabled = true;
    cursor_visible = false;
    cursor_timer = 0; // redraw it in next animate_cursor() call
  }

  void hide_cursor() {
    cursor_enabled = false;
    if (cursor_visible) {
      // hide it immediatly
      redraw_char(y, x, false);
    }
  }

  void set_cursor_delay(uint16_t delay) {
    cursor_delay = delay;
    if (delay != 0) {
      show_cursor();
    } else {
      hide_cursor(); // and never show it
    }
  }

  void animate_cursor() {
    if (cursor_delay == 0 || !cursor_enabled)
      return;

    if (cursor_timer != 0) {
      --cursor_timer;
    } else {
      cursor_visible = !cursor_visible;
      redraw_char(y, x, cursor_visible);
      cursor_timer = cursor_delay;
    }
  }

  void init(uint8_t bias, uint8_t contrast);
  void print(char c);
  void print(const char *s);
  void redraw();
  void do_scroll();
  void goto_y_x(uint8_t _y, uint8_t _x) {
    hide_cursor();
    goto_y_x(_y, _x);
    show_cursor();
  }

private:
  // should be called with cursor hidden
  void _print(char c, bool inverse = false) {
    if (c == '\r') {
      _goto_y_x(y, 0);

    } else if ((c == '\n') && (y != 5)) {
      _goto_y_x(y+1, 0);

    } else if ((c == '\n') && (y == 5)) {
      scroll_or_wrap();

    } else if ((c >= 32) && (c <= 128)) {

      _print_raw(c, inverse);

      x++;
      if (x == 14) {
        x = 0;
        y++;
      }

      if (y == 6) {
        scroll_or_wrap();
      }

    }
  }

  void _print_raw(char c, bool inverse = false) {
    buffer[y*14 + x] = c;
    const uint8_t *ptr = FONT + (c - 32)*6;
    for (uint8_t i = 0; i < 6; ++i) {
      uint8_t pixels = pgm_read_byte(ptr++);
      send(inverse ? ~pixels : pixels);
    }
  }

  void scroll_or_wrap() {
    if (scroll) {
      do_scroll();
    } else {
      _goto_y_x(0, 0);
    }
  }

  // we need primitive like this
  // and public method that hides cursor
  void _goto_y_x(uint8_t _y, uint8_t _x) {
    control();
    send(PCD8544_SETYADDR | _y);
    send(PCD8544_SETXADDR | _x*6);

    y = _y;
    x = _x;
  }


  void redraw_char(uint8_t y, uint8_t x, bool inverse);

  // current position
  uint8_t x, y;

  // current buffer
  static const size_t buffer_length = 14*6;
  char buffer[buffer_length];

  bool scroll;

  uint16_t cursor_delay;
  uint16_t cursor_timer;
  bool cursor_visible;
  bool cursor_enabled;

};

class NokiaOscDisplay {
public:
  NokiaOscDisplay() : x(0) {}

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

  void clear() {
    goto_x(0);
    data();
    for (uint16_t i = 0; i < buffer_length; ++i) {
      send(0);
    }
    goto_x(0);
  }

  uint8_t get_x() {
    return x;
  }

  void goto_x(uint8_t _x) {
    control();
    send(PCD8544_SETYADDR);
    send(PCD8544_SETXADDR | _x*6);

    x = _x;
  }

  void init(uint8_t bias, uint8_t contrast);
  void draw_bar(uint8_t height);

private:
  // current position
  uint8_t x;

  static const size_t buffer_length = 84*6;

};

class NokiaGraphDisplay {
public:
  NokiaGraphDisplay() : x(0), y(0) {}

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
    _send_no_wait(b);
    _wait();
  }

  void clear() {
    memset(buffer, 0, buffer_length);
    redraw();
    goto_y_x(0, 0);
  }

  uint8_t get_x() {
    return x;
  }

  void goto_y_x(uint8_t _y, uint8_t _x) {
    control();
    send(PCD8544_SETYADDR | _y / 8);
    send(PCD8544_SETXADDR | _x);

    y = _y;
    x = _x;
  }

  void init(uint8_t bias, uint8_t contrast);
  void redraw();
  void put_pixel(uint8_t color);
  void scroll();

private:
  // current position
  uint8_t x, y;

  // current buffer
  static const size_t buffer_length = 84*6;
  uint8_t buffer[buffer_length];

  void _send_no_wait(uint8_t b) {
    SPDR = b;
  }

  void _wait() {
    while(!(SPSR & (1<<SPIF)));
  }

};

#endif
