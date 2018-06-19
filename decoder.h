#ifndef DECODER_H
#define DECODER_H

#include <atmel_start.h>

const int HIST_SIZE = 9;

/*
 * START -0-> BEFORE0 -A-> AFTER0 -0-> BEFORE1 -B-> START(emit A,B)
 */
enum decoder_state {
  START, BEFORE0, AFTER0, BEFORE1
};

const uint8_t START_MARKER = 0;

class decoder {
public:
  decoder();
  void capture_frame();
  void decode_frame();
  bool has_char();
  char get_char();

private:
  uint8_t hist[HIST_SIZE];
  bool _has_char :1;
  decoder_state state :2, :5;
  uint8_t last_nibble :4;
  uint8_t n0 :4;
  char c;

  void got_nibble(uint8_t n);
};

#endif
