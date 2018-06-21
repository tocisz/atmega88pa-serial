#include <string.h>
#include <stdio.h>

#include "decoder.h"
#include "events.h"

const uint16_t LOW_COUNT = 979;
const uint16_t COUNT_STEP = 909;

decoder::decoder() {
  reset();
}

void decoder::reset() {
  last_nibble = HIST_SIZE; // out of range
  n0 = HIST_SIZE; // out of range
  c0 = 0;

  chunk = 0;
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
          got_chunk(i);
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

uint16_t decoder::get_average() {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < capt_size; ++i) {
    sum += capt[i];
  }
  return sum / capt_size;
}

void decoder::got_chunk(uint8_t n) {
  // correct forbidden repetitions rule
  uint8_t n_corrected = (n > n0) ? (n-1) : n;
  n0 = n;

  #if DEBUG
  putchar('[');
  putchar('0'+n_corrected);
  putchar(']');
  #endif

  c0 = (c0 << 2) | n_corrected;

  if (chunk == chunks_limit) { // we have a char
    _has_char = true;
    c = 32 + c0;
    chunk = 0;
    c0 = 0;
  } else {
    ++chunk;
  }

}

bool decoder::has_char() {
  return _has_char;
}

char decoder::get_char() {
  _has_char = false;
  return c;
}
