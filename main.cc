#include <atmel_start.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

#include "events.h"
#include "usart_util.h"
#include "print.h"

uint16_t start_time, end_time;

const uint16_t LOW_COUNT = 621;
const uint16_t COUNT_STEP = 587;
#define HIST_SIZE 9

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	// eni();

	uint8_t hist[HIST_SIZE];

	/* Replace with your application code */
	for(;;) {

		if (ci == 100) {
			uint32_t sum = 0;
			memset(hist, 0, HIST_SIZE);
			for (uint8_t i = 0; i < 100; ++i) {
				uint16_t val = capt[i];
				sum += val;
				if (val >= LOW_COUNT) {
					uint16_t idx = (val - LOW_COUNT) / COUNT_STEP;
					if (idx < HIST_SIZE) {
						++hist[idx];
					}
				}
			}
			sum /= 100;
			while (!out_buffer.is_empty());
			printf("avg %u\r\n", (uint16_t)sum);

			while (!out_buffer.is_empty());
			// puts("his\t");
			for (uint16_t i = 0; i < HIST_SIZE; ++i) {
				while (!out_buffer.is_empty());
				printf("%u\t", hist[i]);
			}
			puts("");

		ci = 0;
	}

		// ATOMIC_BLOCK(ATOMIC_FORCEON) {
		// 	if (Events.new_1s_cycle) {
		// 		Events.new_1s_cycle = false;
		//     NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
		// 			puts("tick");
		//     }
		// 	}
		// }

		while (!in_buffer.is_empty()) {
			putchar(getchar());
		}

		wdt_reset();

	}
}
