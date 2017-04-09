#include "events.h"
volatile uint8_t EVENT0 = 0;
volatile uint16_t time = 0;
volatile bool button_state_on = false;
// volatile uint16_t adcv = 0;

uint16_t capture[256];
volatile uint8_t capture_write_ptr = 0;
