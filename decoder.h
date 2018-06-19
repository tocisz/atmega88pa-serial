#ifndef DECODER_H
#define DECODER_H

#include <atmel_start.h>

const int HIST_SIZE = 9;

const uint8_t START_MARKER = 0;

class decoder {
public:
  decoder();
  void capture_frame();
  void decode_frame();
  bool has_char();
  char get_char();

private:
  static const uint8_t max_empty_frames = 5; // actually it can go one more...
  static const uint8_t max_repetitions = 1; // actually it can go one more...

  uint8_t hist[HIST_SIZE];

  bool _has_char :1;
  uint8_t odd_parity :1;
  uint8_t repetitions :2;
  uint8_t empty_frames :4;

  uint8_t last_nibble;
  uint8_t n0;
  uint8_t n0_corrected;

  char c;

  void got_nibble(uint8_t n);
  void reset();
};

#endif
