#include <atmel_start.h>
#include <avr/wdt.h>

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

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	/* Replace with your application code */
	for(;;) {

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			if (is_button_change()) {
				clear_button_change();
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					HEART_set_level(button_state_on);
					USART_puts(button_state_on ? "ON\r\n" : "OFF\r\n");
				}
			}

			if (is_new_512hz_cycle()) {
				clear_new_512hz_cycle();
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
				}
			}
		}

		sleep_mode();

	}
}
