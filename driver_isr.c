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

// contdown to limit button instability
uint8_t button_block = 0;

static inline void handle_button_state_change(void) {
	bool level = !BUTTON_get_level();
	if (button_state_on != level) {
		button_state_on = level;
		set_button_change();
	}
}

ISR(USART0_RX_vect)
{

	/* Insert your USART_0 reception complete interrupt handling code here */
}

ISR(USART0_UDRE_vect)
{

	/* Insert your USART_0 data register empty interrupt handling code here */
}

ISR(TIMER0_OVF_vect)
{
	set_new_512hz_cycle();

	++time;

	if (button_block == 1) { // check state at the end of blocked period
		handle_button_state_change();
	}
	if (button_block > 0) {
		--button_block;
	}
}

ISR(PCINT1_vect)
{
	if (button_block)
		return;

	handle_button_state_change();

	// 1 can give arbitrary small delay (we don't reset timer)
	button_block = 2;
}
