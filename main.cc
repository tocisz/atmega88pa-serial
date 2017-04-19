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

void print_param(const char *name, uint16_t val);
static inline void print_pair(uint16_t a, uint16_t b);

/////////////// CAPTURE ///////////////////

class Capture {
public:
	uint8_t capture_read_ptr = 0;

	uint8_t state;

	uint16_t s_short;
	uint16_t s_long;
	uint16_t s_cycle;

	uint16_t avg_short;
	uint16_t avg_long;
	uint16_t avg_cycle;

	uint16_t cycle_1st_low;
	uint16_t cycle_1st_high;

	Capture() {
		init_caputre();
	}

	void init_caputre();
	void process_capture();

};


#define BETWEEN(a,x,b) ((a<=x)&&(x<=b))

void Capture::init_caputre() {
	state = 0;
	s_short = 0;
	s_long = 0;
	s_cycle = 0;
}

void Capture::process_capture() {
	while ((capture_write_ptr&~1) != (capture_read_ptr&~1)) {
		uint8_t i_up = capture_read_ptr;
		uint8_t i_down = (capture_read_ptr+1)%256;
		uint16_t l_up = capture_buffer[i_up];
		uint16_t l_down = capture_buffer[i_down];
		uint16_t l_sum = l_up + l_down;
		if (BETWEEN(2500, l_up, 5000) && BETWEEN(2500, l_down, 5000)) {
			cycle_1st_low = l_sum / 3;
			cycle_1st_high = l_sum / 2;
			state = 1;
		} else if ((state == 1 || state == 2 || state == 5)
			&& (BETWEEN(250, l_up, 1000) && BETWEEN(cycle_1st_low, l_sum, cycle_1st_high))) {
				s_short += l_up;
				s_cycle += l_sum;
				++state;
		} else if ((state == 3 || state == 4 || state == 6)
			&& (BETWEEN(1000, l_up, 2000) && BETWEEN(cycle_1st_low, l_sum, cycle_1st_high))) {
				s_long += l_up;
				s_cycle += l_sum;
				++state;
		} else if (state > 0) {
			putchar('F'); putchar('0'+state);
			print_pair(l_up, l_down);
			print_pair(cycle_1st_low, cycle_1st_high);
			init_caputre();
		}

		capture_read_ptr = (capture_read_ptr+2)%256;
	}
	if (state == 6) {
		avg_short = s_short / 3;
		avg_long = s_long / 3;
		avg_cycle = s_cycle / 6;
		print_param("LO", avg_short);
		print_param("HI", avg_long);
		print_param("CY", avg_cycle);
		init_caputre();
	}
}
///////////////////////////////////////////

char print_buffer[6];
void print_param(const char *name, uint16_t val) {
	// use long to have value unsigned
	ltoa(val, print_buffer, 10);
	fputs(name, stdout);
	puts(print_buffer);
	while (out_buf_length() > 0); //wait
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

static inline void print_pair(uint16_t a, uint16_t b) {
	print_align(a);
	print_align(b);
	putchar('\n');
	while (out_buf_length() > 0); //wait
}

void capture_print(void) {
	uint16_t low_cnt;
	uint16_t high_cnt;

	// even number > capture_write_ptr
	uint8_t print_ptr = ((capture_write_ptr & ~1) + 2)%256;

	while ((print_ptr & ~1) != (capture_write_ptr & ~1)) {

		high_cnt = capture_buffer[print_ptr++];
		low_cnt = capture_buffer[print_ptr++];

		print_pair(high_cnt, low_cnt);
	}
}


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	Capture capture;

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
					if (c == 'p') {
						capture_print();
					} else if (c == 's') {
						print_param("STA", capture.state);
					} else if (c == 'w') {
						print_param("WP ", capture_write_ptr);
					} else if (c == 'r') {
						print_param("RP ", capture.capture_read_ptr);
					} else {
						putchar(c);
					}
				}
			}

			if ((capture_write_ptr&~1) != (capture.capture_read_ptr&~1)) {
			NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					capture.process_capture();
				}
			}

		}

		wdt_reset();
		sleep_mode();

	}
}
