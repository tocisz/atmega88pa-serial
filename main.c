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

uint16_t start_time, end_time;

char print_buffer[6];
void print_time(uint16_t val) {
	itoa(val, print_buffer, 10);
	puts(print_buffer);
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
				Events.button_change = 0;
				bool button = button_state_on;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					HEART_set_level(button);
					uint16_t ctime = read_time();
					if (button) {
						start_time = ctime;
					} else {
						end_time = ctime;
						print_time(end_time - start_time);
					}
				}
			}

			if (Events.new_512hz_cycle) {
				Events.new_512hz_cycle = 0;
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
