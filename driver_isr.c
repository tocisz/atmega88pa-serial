/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <atmel_start.h>
#include <compiler.h>

#include "events.h"
#include "usart_util.h"

// contdown to limit button instability
uint8_t button_block = 0;

static inline void handle_button_state_change(void) {
	bool level = !BUTTON_get_level();
	if (button_state_on != level) {
		button_state_on = level;
		set_button_change();
	}
}

ISR(USART_RX_vect)
{
	char c = USART_getc();
	USART_putc(c);
	HEART_toggle_level();
}

ISR(USART_UDRE_vect)
{
	char c = *(USART_buffer++);
	if (c != 0) {
		USART_putc(c);
	} else {
		USART_disable_udre();
		USART_buffer = NULL;
	}
}

ISR(TIMER0_OVF_vect)
{
	set_new_512hz_cycle();

	++time;

	if (button_block == 1) { // check state at the end of blocked period
		PCMSK1 |= (1 << PCINT13); // Pin change enable mask 13
		handle_button_state_change();
	}
	if (button_block > 0) {
		--button_block;
	}
}

ISR(PCINT1_vect)
{
	handle_button_state_change();
	PCMSK1 &= ~(1 << PCINT13); // Pin change enable mask 13
	// disable button interrupt and schedule it to be enabled after 2-4ms
	button_block = 2;
}
