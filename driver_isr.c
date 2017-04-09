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

//#define BUTTON
#define CAPTURE

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
	USART_in_buffer[USART_in_end++] = USART_getc();
	if (USART_in_end == BUFLEN) USART_in_end = 0;
	HEART_toggle_level();
}

ISR(USART_UDRE_vect)
{
	if (USART_out_begin != USART_out_end) {
		USART_putc(USART_out_buffer[USART_out_begin++]);
		if (USART_out_begin == BUFLEN) USART_out_begin = 0;
	} else {
		USART_disable_udre();
	}
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

#ifdef CAPTURE
int16_t last_capture;
#endif

ISR(PCINT1_vect)
{
	#ifdef BUTTON
		if (button_block)
			return;

		handle_button_state_change();

		// 1 can give arbitrary small delay (we don't reset timer)
		button_block = 2;
	#endif
}

ISR(INT0_vect)
{
#ifdef CAPTURE
  uint16_t time = TCNT1;
	uint8_t lvl = PD2_get_level();
	uint16_t timediff = time - last_capture;
	last_capture = time;
	if ((capture_write_ptr%2) != (lvl?1:0)) {
		// we want state high on even indices (so why it's reversed?)
		capture[capture_write_ptr] = 0;
		capture_write_ptr = (capture_write_ptr+1)%256;
	}
	capture[capture_write_ptr] = timediff;
	capture_write_ptr = (capture_write_ptr+1)%256;
#endif
}
