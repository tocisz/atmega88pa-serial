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
#include "print.h"
#include "nokia_display.h"

NokiaOscDisplay display;

void read_adc(void) {
	// print_param("ADC ", read_adcv());
	ADCBufferT::ByteBufferT &captured = adc_values.capture();
	// puts("ADC");
	// uint8_t i = 0;
	display.goto_x(0);
	while(!captured.is_empty()) {
		uint16_t v = captured.read_short();
		uint8_t vs = v / 21;
		// print_pair(vs, v);
		display.draw_bar(vs);
	}
	display.goto_x(0); // without it last byte is not visible
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

/*
	NokiaTextDisplay display;

	display.init(4, 60);
	display.print("Ready\n");
	display.set_cursor_delay(F_CPU/3906);
*/
	display.init(4, 60);

	bool on = false;
	uint8_t draw_cnt = 0;

	/* Replace with your application code */
	for(;;) {

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			if (Events.button_change) {
				Events.button_change = false;
				bool button = Events.button_state_on;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					HEART_set_level(button);
					if (button) {
						on = !on;
					}
				}
			}

			if (Events.new_512hz_cycle) {
				Events.new_512hz_cycle = false;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
					// display.animate_cursor();
					if (on && !draw_cnt) {
						draw_cnt = 16;
						read_adc();
					}
					--draw_cnt;
				}
			}

			while (!in_buffer.is_empty()) {
				putchar(getchar());
			}
		}

		sleep_mode();

	}
}
