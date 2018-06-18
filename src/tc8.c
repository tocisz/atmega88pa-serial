/**
 * \file
 *
 * \brief TC8 related functionality implementation.
*
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <tc8.h>
#include <utils.h>

/**
 * \brief Initialize TIMER_0 interface
 */
int8_t TIMER_0_init()
{
	/* Enable TC0 */
	PRR &= ~(1 << PRTIM0);
	TCCR0A = (0 << COM0A1) | (0 << COM0A0) | // Normal port operation, OCA disconnected
	         (0 << COM0B1) | (0 << COM0B0) | // Normal port operation, OCA disconnected
	         (0 << WGM01) | (0 << WGM00);    // Mode 0 Normal

	TCCR0B = (0 << WGM02) |                           // Mode 0 Normal
	         (0 << CS02) | (1 << CS01) | (0 << CS00); // IO clock divided by 8

	TIMSK0 = (0 << OCIE0B) | // Disable output compare match B interrupt
	         (0 << OCIE0A) | // Disable output compare match A interrupt
	         (1 << TOIE0);   // Enable overflow interrupt

	return 0;
}

/**
 * \brief Initialize TIMER_2 interface
 */
int8_t TIMER_2_init()
{
	/* Enable TC2 */
	PRR &= ~(1 << PRTIM2);

	// TCCR2A = (0 << COM2A1) | (0 << COM2A0) /* Normal port operation, OCA disconnected */
	//		 | (0 << COM2B1) | (0 << COM2B0) /* Normal port operation, OCB disconnected */
	//		 | (0 << WGM21) | (0 << WGM20); /* TC8 Mode 0 Normal */

	TCCR2B = 0                                          /* TC8 Mode 0 Normal */
	         | (1 << CS22) | (0 << CS21) | (1 << CS20); /* IO clock divided by 128 */

	TIMSK2 = 0 << OCIE2B   /* Output Compare B Match Interrupt Enable: disabled */
	         | 0 << OCIE2A /* Output Compare A Match Interrupt Enable: disabled */
	         | 1 << TOIE2; /* Overflow Interrupt Enable: enabled */

	// GTCCR = 0 << TSM /* Timer/Counter Synchronization Mode: disabled */
	//		 | 0 << PSRASY /* Prescaler Reset Timer/Counter2: disabled */
	//		 | 0 << PSRSYNC; /* Prescaler Reset: disabled */

	ASSR = (1 << AS2); // Enable asynchronous mode

	return 0;
}
