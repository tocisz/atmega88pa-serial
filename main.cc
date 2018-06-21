#include <atmel_start.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

#include "events.h"
#include "usart_util.h"
#include "print.h"
#include "decoder.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	// eni();

	decoder decoder;

	/* Replace with your application code */
	for(;;) {

		if (ci == capt_size) {
			decoder.capture_frame();
			ci = 0; // resume PWM capture
			decoder.decode_frame();
			//printf("%hu\n", decoder.get_average());
		}

		// ATOMIC_BLOCK(ATOMIC_FORCEON) {
		// 	if (Events.new_1s_cycle) {
		// 		Events.new_1s_cycle = false;
		//     NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
		// 			puts("tick");
		//     }
		// 	}
		// }

		while(decoder.has_char()) {
			putchar(decoder.get_char());
		}

		while (!in_buffer.is_empty()) {
			putchar(getchar());
		}

		wdt_reset();

	}
}
