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

private:
	uint16_t s_short;
	uint16_t s_long;
	uint16_t s_cycle;

	uint16_t avg_cycle;
	uint16_t bit_threshold;

	uint16_t cycle_1st_low;
	uint16_t cycle_1st_high;

	//uint8_t current_bit_start;
public: // XXX
	int16_t bit_sequence_length;
	uint16_t bit_sequence_high;
	bool going_back_allowed;

public:
	Capture() {
		init_capture();
	}

	void process_capture();

private:
	void init_capture();
	void capture_init_sequence();
	void capture_bit();
	void start_new_bit();
};


#define BETWEEN(a,x,b) ((a<=x)&&(x<=b))

void Capture::init_capture() {
	state = 0;
	s_short = 0;
	s_long = 0;
	s_cycle = 0;
}

void Capture::process_capture() {
	while ((capture_write_ptr&~1) != (capture_read_ptr&~1)) {
		if (state == 9) {
			capture_bit();
		} else {
			capture_init_sequence();
		}
	}
}

inline void read_pair(uint16_t &l_up, uint16_t &l_down, uint16_t &l_sum, uint8_t i) {
	l_up = capture_buffer[i];
	l_down = capture_buffer[(i+1)%256];
	l_sum = l_up + l_down;
}

inline void read_pair(uint16_t &l_up, uint16_t &l_sum, uint8_t i) {
	l_up = capture_buffer[i];
	uint16_t l_down = capture_buffer[(i+1)%256];
	l_sum = l_up + l_down;
}

void Capture::start_new_bit() {
	//current_bit_start = capture_read_ptr;
	bit_sequence_length = 0;
	bit_sequence_high = 0;
	going_back_allowed = false;
}

void emit_bit(uint8_t bit) {
	putchar('0'+bit);
	while (out_buf_length() > 0); //wait
}

void Capture::capture_bit() {
	uint16_t l_up, l_sum;
	int16_t prv_bit_sequence_length;
	bool terminate = false;
	while ((capture_write_ptr&~1) != (capture_read_ptr&~1)) {
		read_pair(l_up, l_sum, capture_read_ptr);

		if (l_sum > 4*avg_cycle) {
			terminate = true;
			l_sum = avg_cycle + avg_cycle/8; // cut off rest
		}

		bit_sequence_high += l_up;
		prv_bit_sequence_length = bit_sequence_length;
		bit_sequence_length += l_sum;

		if (bit_sequence_length > (int16_t)avg_cycle) {
			uint16_t deviation;
			uint16_t deviation_plus = bit_sequence_length - avg_cycle;
			uint16_t deviation_minus = avg_cycle - prv_bit_sequence_length;
			// what if bit_sequence_length > 2*avg_cycle ?
			// we will use deviation_minus and go into infinite loop?
			if (going_back_allowed
					&& (deviation_minus < deviation_plus)) { // go back one pair
				putchar('-');
				capture_read_ptr = (capture_read_ptr-2)%256;
				deviation = deviation_minus;
				bit_sequence_high -= l_up;
				terminate = false;
			} else {
				putchar('+');
				deviation = deviation_plus;
			}

			if (terminate) {
				emit_bit((bit_sequence_high > bit_threshold) ? 1 : 0);
				puts("T\n");
				init_capture();
				return;
			}

			if (deviation < avg_cycle / 8) {
				emit_bit((bit_sequence_high > bit_threshold) ? 1 : 0);
				start_new_bit();
			} else {
				// lengh is way too long
				while (bit_sequence_length > (int16_t)(2*avg_cycle-avg_cycle/2)) {
					emit_bit((bit_sequence_high > bit_threshold) ? 5 : 6);
					bit_sequence_length -= avg_cycle;
					if (bit_sequence_high > avg_cycle) {
						bit_sequence_high -= avg_cycle;
					} else {
						bit_sequence_high = 0;
					}
				}
				// 0.5*avg_cycle < bit_sequence_length < 1.5*avg_cycle

				if (bit_sequence_length > (int16_t)avg_cycle) {
					deviation = bit_sequence_length - avg_cycle;
				} else {
					deviation = avg_cycle - bit_sequence_length;
				}

				if (deviation < avg_cycle / 8) {
					// we are back on track
					emit_bit((bit_sequence_high > bit_threshold) ? 1 : 0);
					start_new_bit();
				} else {
					// try to correct timing
					emit_bit((bit_sequence_high > bit_threshold) ? 3 : 2);
					// current_bit_start = capture_read_ptr;
					if (bit_sequence_length < (int16_t)avg_cycle) { // cycle too short
						bit_sequence_length = -deviation;
						bit_sequence_high = 0;
					} else { // cycle too long
						// yes, we start from negative value to correct cycle boundary
						bit_sequence_length = deviation;
						// and assume that high state was delayed
						bit_sequence_high = deviation;
					}
				}
			}

		} else {
			going_back_allowed = true;
		}

		capture_read_ptr = (capture_read_ptr+2)%256;
	}
}

void Capture::capture_init_sequence() {
	while ((capture_write_ptr&~1) != (capture_read_ptr&~1)) {
		uint16_t l_up, l_down, l_sum;
		read_pair(l_up, l_down, l_sum, capture_read_ptr);
		if (BETWEEN(2500, l_up, 5000) && BETWEEN(2500, l_down, 5000)) {
			cycle_1st_high = l_sum / 2; // 1/2 * (2/3 * 5/4 + 1/6);
			cycle_1st_low = cycle_1st_high / 2; // 1/2 * (2/3 * 3/4);
			state = 1;
		} else if ((state == 1 || state == 2 || state == 5 || state == 7) // ((1<<(state-1))&0b01010011)
			&& (BETWEEN(250, l_up, 1000) && BETWEEN(cycle_1st_low, l_sum, cycle_1st_high))) {
				s_short += l_up;
				s_cycle += l_sum;
				++state;
		} else if ((state == 3 || state == 4 || state == 6 || state == 8) //((1<<(state-1))&~0b01010011)
			&& (BETWEEN(1000, l_up, 2000) && BETWEEN(cycle_1st_low, l_sum, cycle_1st_high))) {
				s_long += l_up;
				s_cycle += l_sum;
				++state;
		} else if (state > 0) {
			putchar('F'); putchar('0'+state);
			print_pair(l_up, l_down);
			print_pair(cycle_1st_low, cycle_1st_high);
			init_capture(); // go back to state 0
			break; // break to reprocess
			// pair that caused error may be start of a new init sequence
		}

		capture_read_ptr = (capture_read_ptr+2)%256;

		if (state == 9) {
			uint16_t avg_short = s_short / 4;
			uint16_t avg_long = s_long / 4;
			bit_threshold = (avg_short + avg_long) / 2;
			avg_cycle = s_cycle / 8;
			print_param("LO ", avg_short);
			print_param("HI ", avg_long);
			print_param("CY ", avg_cycle);
			start_new_bit();
			break;
		}
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
					} else if (c == 'l') {
						print_param("BL ", capture.bit_sequence_length);
					} else if (c == 'h') {
						print_param("BH ", capture.bit_sequence_high);
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
