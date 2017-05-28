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

void read_adc(void) {
	// print_param("ADC ", read_adcv());
	adc_values.capture();
	ADCBufferT::ByteBufferT &captured = adc_values.get_captured();
	puts("ADC");
	uint8_t i = 0;
	while(!captured.is_empty()) {
		print_pair(++i, captured.read_short());
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	NokiaTextDisplay display;

	display.init(4, 60);
	display.print("Ready\n");
	display.set_cursor_delay(F_CPU/3906);

	/* Replace with your application code */
	for(;;) {

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			if (Events.button_change) {
				Events.button_change = false;
				bool button = Events.button_state_on;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					HEART_set_level(button);
					if (button) {
						read_adc();
					}
				}
			}

			if (Events.new_512hz_cycle) {
				Events.new_512hz_cycle = false;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
					display.animate_cursor();
				}
			}

			while (!in_buffer.is_empty()) {
				putchar(getchar());
			}
		}

		sleep_mode();

	}
}
