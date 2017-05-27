#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <util/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

struct EventFlags {
  uint8_t button_change : 1;
  uint8_t new_512hz_cycle : 1;
};
extern volatile struct EventFlags Events;

// see driver_isr.c
extern volatile bool button_state_on;
extern volatile uint16_t time;

static inline uint16_t read_time(void) {
  uint16_t ret;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    ret = time;
  }
  return ret;
}

#ifdef __cplusplus
}
#endif

#endif
