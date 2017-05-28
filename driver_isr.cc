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

#define BUTTON

// contdown to limit button instability
uint8_t button_block = 0;

static inline void handle_button_state_change(void) {
	bool level = !BUTTON_get_level();
	if (Events.button_state_on != level) {
		Events.button_state_on = level;
		Events.button_change = true;
	}
}

ISR(USART_RX_vect)
{
	in_buffer.write_byte(USART_getc());
	HEART_toggle_level();
}

ISR(USART_UDRE_vect)
{
	if (!out_buffer.is_empty()) {
		USART_putc(out_buffer.read_byte());
	} else {
		USART_disable_udre();
	}
}

ISR(TIMER0_OVF_vect)
{
	Events.new_512hz_cycle = true;

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

ISR(ADC_vect)
{
	uint8_t l = ADCL;
	uint8_t h = ADCH;
	// adcv = (h<<8)|l;
	adc_values.store2(h,l);
}
