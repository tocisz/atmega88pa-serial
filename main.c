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

uint16_t min = 0xffff;
uint16_t max = 0;
char print_buffer[6];
void read_adc(void) {
	if (out_buf_length() > 0)
		return;

	uint16_t val = read_adcv();

	if (val > max) {
		max = val;
		itoa(val, print_buffer, 10);
		fputs("max:", stdout);
		puts(print_buffer);
	}
	if (val < min) {
		min = val;
		itoa(val, print_buffer, 10);
		fputs("min:", stdout);
		puts(print_buffer);
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
						// start_time = ctime;
					} else {
						// end_time = ctime;
						min = 0xffff;
						max = 0;
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
