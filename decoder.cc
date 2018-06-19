#include <string.h>
#include <stdio.h>

#include "decoder.h"
#include "events.h"

const uint16_t LOW_COUNT = 621;
const uint16_t COUNT_STEP = 587;

decoder::decoder() {
}

void decoder::capture_frame() {
  memset(hist, 0, HIST_SIZE);
  for (uint8_t i = 0; i < capt_size; ++i) {
    uint16_t val = capt[i];
    if (val >= LOW_COUNT) {
      uint16_t idx = (val - LOW_COUNT) / COUNT_STEP;
      if (idx < HIST_SIZE) {
        ++hist[idx];
      }
    }
  }
}

void decoder::decode_frame() {
  uint8_t limit = capt_size / 2;
  for (uint8_t i = 0; i < HIST_SIZE; ++i) {
    if (hist[i] > limit) {
      // signal found
      if (last_nibble == i) { // second time
        got_nibble(i);
      }

      last_nibble = i;
    }
  }
}

void decoder::got_nibble(uint8_t n) {
  // putchar('[');
  // putchar('0'+n);
  // putchar(']');
  switch (state) {
    case START:
    if (n == START_MARKER) {
      state = BEFORE0;
    }
    break;

    case BEFORE0:
    if (n != START_MARKER) {
      n0 = n-1;
      state = AFTER0;
    }
    break;

    case AFTER0:
    if (n == START_MARKER) {
      state = BEFORE1;
    }
    break;

    case BEFORE1:
    if (n != START_MARKER) {
      c = 32 + ( (n0<<3) | (n-1) );
      _has_char = true;
      state = START;
    }
    break;
  }
}

bool decoder::has_char() {
  return _has_char;
}

char decoder::get_char() {
  _has_char = false;
  return c;
}
