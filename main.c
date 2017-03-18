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

void print_align(uint16_t val) {
	itoa(val, print_buffer, 10);
	uint8_t len = strlen(print_buffer);
	for (uint8_t i = 4-len; i > 0; --i) {
		putchar(' ');
	}
	fputs(print_buffer, stdout);
}

static inline void print_result() {
	print_align(low_cnt);
	print_align(high_cnt);
	putchar('\n');
}

void read_adc(void) {
	uint16_t val = read_adcv();
	if (state_high) {
		if (val > MIDDLE_VAL) {
			++high_cnt;
		} else {
			print_result();
			state_high = false;
			low_cnt = 1;
		}
	} else { //state_low
		if (val < MIDDLE_VAL) {
			++low_cnt;
		} else {
			state_high = true;
			high_cnt = 1;
		}
	}
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
						on = !on;
					}
				}
			}

			if (is_new_512hz_cycle()) {
				clear_new_512hz_cycle();
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
					if (on)
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
