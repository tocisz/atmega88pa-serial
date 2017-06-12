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

#include "nokia_display.h"

////////////////////////////////////////////

class Automaton30 {
	static const size_t line_size = 11;
	uint8_t line[2][line_size];
	uint8_t gen;

public:
	Automaton30() {
		gen = 0;
		memset(line[gen], 0, line_size);
		line[gen][5] = 1;
	}

	void generate() {
		uint8_t nextgen = !gen;
		uint8_t im = line_size-1;
		uint8_t i = 0;
		uint8_t ip = 1;
		while(i < line_size) {
			uint8_t vm = (((line[gen][im]&1) << 7) | (line[gen][i]>>1));
			uint8_t v = line[gen][i];
			uint8_t vp = (line[gen][i]<<1) | (line[gen][ip]>>7);
			line[nextgen][i] = vm ^ (v | vp);

			im = i;
			++i;
			ip = i+1;
			if (ip == line_size) {
				ip = 0;
			}
		}

		gen = nextgen;
	}

	uint8_t* get_generation() {
		return line[gen];
	}

	void draw_generation(NokiaGraphDisplay &display) {
		uint8_t bit = 0;
		uint8_t *ptr = line[gen];
		uint8_t val;
		for (uint8_t i = 0; i < 84; ++i) {
			if (bit == 0) {
				val = *(ptr++);
				bit = 8;
			}
			display.put_pixel((val & 0b10000000) ? 1 : 0);
			val <<= 1;
			--bit;
		}
	}
};

////////////////////////////////////////////

const uint8_t draw_delay = 1;
uint8_t draw_cnt = draw_delay;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	NokiaGraphDisplay display;

	display.init(4, 60);

	Automaton30 automaton;

	/* Replace with your application code */
	for(;;) {

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			if (Events.button_change) {
				Events.button_change = false;
				bool button = Events.button_state_on;
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
				Events.new_512hz_cycle = false;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();

					--draw_cnt;
					if (draw_cnt == 0) {
						automaton.generate();
						automaton.draw_generation(display);
						draw_cnt = draw_delay;
					}
				}
			}

			while (in_buf_length() > 0) {
				char c = getchar();
				putchar(c);
			}
		}

		sleep_mode();

	}
}
