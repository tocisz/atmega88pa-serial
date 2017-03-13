#include <atmel_start.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

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

const PROGMEM char NL[] = "\r\n";
inline void append_nl(char *s, uint8_t slen) {
	uint8_t len = strlen(s);
	strlcpy_P(s+len, NL, slen-len);
}

#define BUFLEN 8
char print_buffer[BUFLEN];
void print_time(uint16_t val) {
	itoa(val, print_buffer, 10);
	append_nl(print_buffer, BUFLEN);
	USART_puts(print_buffer);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
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
					uint16_t ctime = read_time();
					if (button) {
						start_time = ctime;
					} else {
						end_time = ctime;
						print_time(end_time - start_time);
					}
				}
			}

			if (is_new_512hz_cycle()) {
				clear_new_512hz_cycle();
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
				}
			}
		}

		sleep_mode();

	}
}
