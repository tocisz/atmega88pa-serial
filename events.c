#include "events.h"
volatile struct EventFlags Events;
volatile uint16_t time = 0;
volatile bool button_state_on = false;
