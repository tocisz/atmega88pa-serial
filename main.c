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

#define READ_CNT_MAX 32;
uint8_t read_cnt = READ_CNT_MAX;

uint16_t min = 0xffff;
uint16_t max = 0;

static inline void reset_min_max() {
	min = 0xffff;
	max = 0;
}

char print_buffer[6];
void print_param(char *name, uint16_t val) {
	itoa(val, print_buffer, 10);
	fputs(name, stdout);
	puts(print_buffer);
}

void read_adc(void) {
	uint16_t val = read_adcv();
	if (val > max) {
		max=val;
	}
	if (val < min) {
		min=val;
	}
	if (--read_cnt > 0)
		return;

	print_param("min:", min);
	print_param("max:", max);
	puts("");

	read_cnt = READ_CNT_MAX;
	reset_min_max();
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
			if (is_button_change()) {
				clear_button_change();
				bool button = button_state_on;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					HEART_set_level(button);
					// uint16_t ctime = read_time();
					if (button) {
						// start_time = ctime;
					} else {
						// end_time = ctime;
					}
				}
			}

			if (is_new_512hz_cycle()) {
				clear_new_512hz_cycle();
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
					read_adc();
				}
			}

			while (in_buf_length() > 0) {
				putchar(getchar());
			}
		}

		sleep_mode();

	}
}
