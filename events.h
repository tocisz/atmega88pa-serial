#ifndef EVENTS_H
#define EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <util/atomic.h>

extern volatile uint8_t EVENT0;

// see driver_isr.c
extern volatile bool button_state_on;

extern volatile uint16_t time;

extern volatile uint16_t adcv;

#define BTN_CHG 0
#define NEW_512HZ_CYCLE 1

static inline bool is_button_change(void) {
  return EVENT0 & (1 << BTN_CHG);
}

static inline void set_button_change(void) {
  EVENT0 |= (1 << BTN_CHG);
}

static inline void clear_button_change(void) {
    EVENT0 &= ~(1 << BTN_CHG);
}


static inline bool is_new_512hz_cycle(void) {
  return EVENT0 & (1 << NEW_512HZ_CYCLE);
}

static inline void set_new_512hz_cycle(void) {
  EVENT0 |= (1 << NEW_512HZ_CYCLE);
}

static inline void clear_new_512hz_cycle(void) {
    EVENT0 &= ~(1 << NEW_512HZ_CYCLE);
}

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

#ifdef __cplusplus
}
#endif

#endif
