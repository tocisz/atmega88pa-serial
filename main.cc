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
#include "print.h"
#include "capture.h"
#include "byte_buffer.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	SmallByteBuffer captured_bytes;
	Capture capture(capture_buffer, capture_write_ptr, captured_bytes);

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
				while (!in_buffer_is_empty()) {
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

					if (is_capture_finished()) {
						clear_capture_finished();
						puts("RADIO:");
						while (!captured_bytes.is_empty()) {
							while(!out_buffer_is_empty());
							putchar(captured_bytes.read_byte());
						}
						putchar('\n');
					}
				}
			}

		}

		wdt_reset();
		sleep_mode();

	}
}
