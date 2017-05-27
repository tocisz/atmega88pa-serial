#include "events.h"
volatile EventFlags Events;
volatile uint16_t time = 0;

uint16_t capture_buffer[256];
volatile uint8_t capture_write_ptr = 0;
