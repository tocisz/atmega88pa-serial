#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <util/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

struct EventFlags {
  bool button_change : 1;
  bool new_512hz_cycle : 1;
  bool button_state_on : 1; // see driver_isr.c
  bool capture_finished : 1;
};
extern volatile struct EventFlags Events;

extern volatile uint16_t time;

extern uint16_t capture_buffer[256];
extern volatile uint8_t capture_write_ptr;

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
