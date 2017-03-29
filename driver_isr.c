/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <atmel_start.h>
#include <compiler.h>

bool going_up = true;

ISR(TIMER0_OVF_vect)
{
	uint8_t d = PORTD;
	if (going_up) {
		d <<= 1;
		if (d == 128) {
			going_up = false;
		}
	} else {
		d >>= 1;
		if (d == 1) {
			going_up = true;
		}
	}
	PORTD = d;
}
