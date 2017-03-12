/**
 * \file
 *
 * \brief USART related functionality implementation.
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

#include <usart.h>
#include <utils.h>
#include <atmel_start_pins.h>

/**
 * \brief Initialize USART_0 interface
 */
int8_t USART_0_init()
{

	/* Enable USART0 */
	PRR &= ~(1 << PRUSART0);

#define BAUD 9600

#include <utils/setbaud.h>

	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	UCSR0A = (USE_2X << U2X0) | (0 << MPCM0); // Disable multi-processor communication mode

	UCSR0B = (1 << RXCIE0) | // Enable RX complete interrupt
	         (0 << TXCIE0) | // Disable TX complete interrupt
	         (0 << UDRIE0) | // Disable data register empty interrupt
	         (1 << RXEN0) |  // Enable receiver
	         (1 << TXEN0) |  // Enable transmitter
	         (0 << UCSZ02);  // Character size is 7-bit

	UCSR0C = (0 << UMSEL01) | (0 << UMSEL00) | // Module mode: UART
	         (0 << UPM01) | (0 << UPM00) |     // Parity mode: Disabled
	         (0 << USBS0) |                    // 1-bit stop bit
	         (1 << UCSZ01) | (0 << UCSZ00);    // Character size is 7-bit

	return 0;
}

/**
 * \brief Check if USART_0 transmitt buffer is empty
 */
int8_t USART_0_tx_empty()
{
	return UCSR0A & (1 << UDRE0);
}

/**
 * \brief Check if USART_0 receive buffer is full
 */
int8_t USART_0_rx_full()
{
	return UCSR0A & (1 << RXC0);
}

/**
 * \brief Check if USART_0 data is transmitted
 */
int8_t USART_0_data_transmitted()
{
	return UCSR0A & (1 << TXC0);
}

/**
 * \brief Read one character from USART_0
 */
uint8_t USART_0_getc()
{
	return UDR0;
}

/**
 * \brief Write one character on USART_0
 */
int8_t USART_0_putc(const uint8_t data)
{
	UDR0 = data;

	return 0;
}
