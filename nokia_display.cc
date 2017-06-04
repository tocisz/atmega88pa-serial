#include "nokia_display.h"

#include "apple_font.h"

void NokiaDisplayBase::init(uint8_t bias, uint8_t contrast, bool vertical) {
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
	send(PCD8544_FUNCTIONSET | (vertical ? PCD8544_VERTICAL : 0));

	send(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}

void NokiaTextDisplay::print(char c) {
  hide_cursor();
  data();
	_print(c);
  show_cursor();
}

void NokiaTextDisplay::print(const char *s) {
  char c;
  hide_cursor();
	data();
	while ((c = *(s++)) != 0) {
		_print(c);
	}
  show_cursor();
}

void NokiaTextDisplay::redraw() {
  _goto_y_x(0, 0);
  data();
  for (uint8_t i = 0; i < buffer_length; ++i) {
    _print_raw(buffer[i]);
  }
}

void NokiaTextDisplay::do_scroll() {
  char *dst = buffer;
  char *src = buffer + 14;
  for (uint8_t i = 0; i < 5; ++i) {
    memcpy(dst, src, 14);
    dst = src;
    src = src + 14;
  }
  memset(dst, ' ', 14);
  redraw();
  _goto_y_x(5, 0);
  show_cursor();
}

void NokiaTextDisplay::redraw_char(
    unsigned char pos_y,
    unsigned char pos_x,
    bool inverse) {
  uint8_t old_y = y;
  uint8_t old_x = x;
  _goto_y_x(pos_y, pos_x);
  char c = buffer[y*14 + x];
  data();
  _print_raw(c, inverse);
  _goto_y_x(old_y, old_x);
}


void NokiaOscDisplay::draw_bar(uint8_t height) {
  data();
  height = 48 - height;
  for (int i = 0; i < 6; ++i) {
    if (height > 7) {
      send(0);
      height -= 8;
    } else {
      send(0xff << height);
      height = 0;
    }
  }
}


void NokiaGraphDisplay::redraw() {
  goto_y_x(0, 0);
  data();
  for (uint16_t i = 0; i < buffer_length; ++i) {
    send(buffer[i]);
  }
  // goto_y_x(0, 0);
}

void NokiaGraphDisplay::put_pixel(uint8_t color) {
  data();
  uint8_t bit = y % 8;
  uint16_t pos = (y / 8) * 84 + x;
  if (color == 0) {
    buffer[pos] &= ~(1 << bit);
  } else {
    buffer[pos] |= 1 << bit;
  }
  send(buffer[pos]);

  ++x;
  if (x > 84) {
    x = 0;
    ++y;
    if (y > 48) {
      scroll();
      goto_y_x(47, 0);
    } else {
      goto_y_x(y, x);
    }
  }
}

void NokiaGraphDisplay::scroll() {
  for (uint16_t i = 0; i < buffer_length; ++i) {
    buffer[i] >>= 1;
    // buffer[i] &=
    if (i < 84 * 5) { // last bank
      buffer[i] |= (buffer[i+84]&1) << 7;
    }
  }
  redraw();
}
