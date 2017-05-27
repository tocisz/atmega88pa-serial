#include <atmel_start.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/////////////////// GOLW //////////////////
uint8_t power = 0;
int8_t dir = 1;

static inline void animate_glow(void) {
	TIMER_2_set_comp_b(power);
	power += dir;
	if (power == 0xff || (power == 0)) {
		dir = -dir;
	}
}
///////////////////////////////////////////

#include "events.h"
#include "usart_util.h"

// uint16_t start_time, end_time;

bool on = false;

#define MIDDLE_VAL 460
uint16_t low_cnt = 0;
uint16_t high_cnt = 0;

bool state_high = false;

static inline void reset_cnt() {
	low_cnt = 0;
	high_cnt = 0;
}

char print_buffer[6];
void print_param(char *name, uint16_t val) {
	itoa(val, print_buffer, 10);
	fputs(name, stdout);
	puts(print_buffer);
}

void read_adc(void) {
	print_param("ADC ", read_adcv());
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	/* Replace with your application code */
	for(;;) {

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			if (Events.button_change) {
				Events.button_change = false;
				bool button = Events.button_state_on;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					HEART_set_level(button);
					// uint16_t ctime = read_time();
					if (button) {
						read_adc();
					}
				}
			}

			if (Events.new_512hz_cycle) {
				Events.new_512hz_cycle = false;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
				}
			}

			while (in_buf_length() > 0) {
				putchar(getchar());
			}
		}

		sleep_mode();

	}
}
