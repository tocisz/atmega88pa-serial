/**
 * \file
 *
 * \brief USART related functionality declaration.
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

#ifndef _USART_H_INCLUDED
#define _USART_H_INCLUDED

#include <compiler.h>
#include <driver_init.h>

/**
 * \addtogroup usart USART driver
 *
 * \section usart_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#ifdef __cplusplus
extern "C" {
#endif

int8_t USART_init();

/**
 * \brief Check if USART transmitt buffer is empty
 */
static inline int8_t USART_tx_empty()
{
	return UCSR0A & (1 << UDRE0);
}

/**
 * \brief Check if USART receive buffer is full
 */
static inline int8_t USART_rx_full()
{
	return UCSR0A & (1 << RXC0);
}

/**
 * \brief Check if USART data is transmitted
 */
static inline int8_t USART_data_transmitted()
{
	return UCSR0A & (1 << TXC0);
}

/**
 * \brief Read one character from USART
 */
static inline uint8_t USART_getc()
{
	return UDR0;
}

/**
 * \brief Write one character on USART
 */
static inline int8_t USART_putc(const uint8_t data)
{
	UDR0 = data;

	return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _USART_H_INCLUDED */
