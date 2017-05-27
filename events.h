#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <util/atomic.h>

struct EventFlags {
  bool button_change : 1;
  bool new_512hz_cycle : 1;
  bool button_state_on : 1; // see driver_isr.c
};
extern volatile EventFlags Events;

extern volatile uint16_t time;
extern volatile uint16_t adcv;

static inline uint16_t read_time(void) {
  uint16_t ret;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    ret = time;
  }
  return ret;
}

static inline uint16_t read_adcv(void) {
  uint16_t ret;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    ret = adcv;
  }
  return ret;
}

#endif
