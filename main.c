#include <atmel_start.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/////////////////// GOLW //////////////////
#ifdef GLOW
uint8_t power = 0;
int8_t dir = 1;

static inline void animate_glow(void) {
	TIMER_2_set_comp_b(power);
	power += dir;
	if (power == 0xff || (power == 0)) {
		dir = -dir;
	}
}
#endif
///////////////////////////////////////////

#include "events.h"
#include "usart_util.h"

// bool on = false;
bool capture_on = false;
bool print_on = false;

uint16_t low_cnt = 0;
uint16_t high_cnt = 0;

static inline void reset_cnt() {
	low_cnt = 0;
	high_cnt = 0;
}

char print_buffer[6];
void print_param(char *name, uint16_t val) {
	// use long to have value unsigned
	ltoa(val, print_buffer, 10);
	fputs(name, stdout);
	puts(print_buffer);
}

void print_align(uint16_t val) {
	ltoa(val, print_buffer, 10);
	uint8_t len = strlen(print_buffer);
	for (int8_t i = 5-len; i > 0; --i) {
		putchar(' ');
	}
	putchar(' ');
	fputs(print_buffer, stdout);
}

static inline void print_result() {
	print_align(low_cnt);
	print_align(high_cnt);
	putchar('\n');
}

void start_capture(void) {
	capture_ptr = 0;
}

void capture_print(void) {
	uint8_t print_ptr = 0;
	while (print_ptr < 254) {
		low_cnt = capture[print_ptr++];
		high_cnt = capture[print_ptr++];

		while (out_buf_length() > 0); //wait

		print_result();
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

			if (is_new_512hz_cycle()) {
				clear_new_512hz_cycle();
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					#ifdef GLOW
					animate_glow();
					#endif
				}
			}

			NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
				while (in_buf_length() > 0) {
					char c = getchar();
					if (c == 'c') {
						start_capture();
					} else if (c == 'p') {
						capture_print();
					} else {
						putchar(c);
					}
				}
			}
		}

		wdt_reset();
		sleep_mode();

	}
}
