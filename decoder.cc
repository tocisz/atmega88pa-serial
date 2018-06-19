#include <string.h>
#include <stdio.h>

#include "decoder.h"
#include "events.h"

const uint16_t LOW_COUNT = 621;
const uint16_t COUNT_STEP = 587;

decoder::decoder() {
  reset();
}

void decoder::reset() {
  last_nibble = 9; // out of range
  n0 = 9; // out of range
  n0_corrected = 9; // out of range

  odd_parity = false;
  repetitions = 0;
  empty_frames = decoder::max_empty_frames; // so it won't reset again
  #if DEBUG
  puts("{R}");
  #endif
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
  bool empty = true;
  uint8_t limit = capt_size / 2;

  #if DEBUG
  putchar('\n');
  for (uint8_t i = 0; i < HIST_SIZE; ++i) {
    printf("%hu\t", hist[i]);
  }
  putchar('\n');
  #endif

  for (uint8_t i = 0; i < HIST_SIZE; ++i) {
    if (hist[i] > limit) {
      // signal found
      empty = false;
      empty_frames = 0;

      #if DEBUG
      putchar('{');
      putchar('0'+i);
      putchar('}');
      #endif

      if (last_nibble == i) { // second time
        if (repetitions <= max_repetitions) {
          ++repetitions;
        }
        if (repetitions == max_repetitions) {
          got_nibble(i);
        }
      } else {
        repetitions = 0;
      }

      last_nibble = i;

      break; // can't be more over limit in hist
    }
  }

  if (empty) {
    if (empty_frames <= max_empty_frames) {
      ++empty_frames;
    }

    if (empty_frames == max_empty_frames) {
      reset();
    }
  }
}

void decoder::got_nibble(uint8_t n) {
  // correct forbidden repetitions rule
  uint8_t n_corrected = (n > n0) ? (n-1) : n;

  #if DEBUG
  putchar('[');
  putchar('0'+n_corrected);
  putchar(']');
  #endif

  if (odd_parity) { // we have a char
    _has_char = true;
    c = 32 + ( (n0_corrected<<3) | n_corrected );
  }

  n0 = n;
  n0_corrected = n_corrected;

  odd_parity = !odd_parity;
}

bool decoder::has_char() {
  return _has_char;
}

char decoder::get_char() {
  _has_char = false;
  return c;
}
