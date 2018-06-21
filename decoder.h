#ifndef DECODER_H
#define DECODER_H

#include <atmel_start.h>

const int HIST_SIZE = 5;

const uint8_t START_MARKER = 0;

class decoder {
public:
  decoder();
  void capture_frame();
  void decode_frame();
  bool has_char();
  char get_char();
  uint16_t get_average();

private:
  static const uint8_t max_empty_frames = 5; // actually it can go one more...
  static const uint8_t max_repetitions = 1; // actually it can go one more...
  static const uint8_t chunks_limit = 2; // chunk: 0,1,2

  uint8_t hist[HIST_SIZE];

  bool _has_char;
  uint8_t chunk;
  uint8_t repetitions;
  uint8_t empty_frames;

  uint8_t last_nibble;
  uint8_t n0;
  uint8_t c0;

  char c;

  void got_chunk(uint8_t n);
  void reset();
};

#endif
