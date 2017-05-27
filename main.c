#include <atmel_start.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sound.h"

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

bool playing = false;

#include "usart_util.h"
// uint16_t start_time, end_time;
// char print_buffer[6];
// void print_time(uint16_t val) {
// 	itoa(val, print_buffer, 10);
// 	puts(print_buffer);
// }

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	init_sound();
	cpu_irq_enable();

	for(;;) {

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			if (Events.button_change) {
				Events.button_change = false;
				if (Events.button_state_on) {
					NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
						if (!playing) {
							next_melody();
							start_play();
						} else {
							stop_play();
						}
						playing = !playing;
						HEART_set_level(playing);
					}
				}
			}

			if (Events.new_512hz_cycle) {
				Events.new_512hz_cycle = false;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
					sound_scheduler();
				}
			}

			if (Events.new_sound_cycle) {
				Events.new_sound_cycle = false;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					start_new_cycle();
				}
			}

			if (Events.finished_playing) {
				Events.finished_playing = false;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					playing = false;
					HEART_set_level(playing);
				}
			}

			while (in_buf_length() > 0) {
				putchar(getchar());
			}
		}

		sleep_mode();

	}
}
