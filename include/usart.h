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

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * \brief Initialize USART_0 interface
 *
 * \return Initialization status.
 */
int8_t USART_0_init();

/**
 * \brief Check if the USART_0 transmitt buffer is empty
 *
 * \return Transmitter ready statua
 * \retval false Transmitt buffer is full, no new data can be written
 * \retval true  Transmitt buffer is empty, new data can be written
 */
int8_t USART_0_tx_empty();

/**
 * \brief Check if the USART_0 receive buffer is full
 *
 * \return Receiver ready status
 * \retval false  No new data in receive buffer
 * \retval true   Receive buffer has new data
 */
int8_t USART_0_rx_full();

/**
 * \brief Check if USART_0 data is transmitted
 *
 * \return Receiver ready status
 * \retval false  Data is not completely shifted out of the shift register
 * \retval true   Data completely shifted out if the USART shift register
 */
int8_t USART_0_data_transmitted();

/**
 * \brief Read one character from USART_0
 *
 * \return Data read from the USART_0 module
 */
uint8_t USART_0_getc();

/**
 * \brief Write one character on USART_0
 *
 * \param[in] data The character to write to the USART bus
 *
 * \return Character write status
 */
int8_t USART_0_putc(const uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* _USART_H_INCLUDED */
