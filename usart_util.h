#ifndef USART_UTIL_H
#define USART_UTIL_H

#include <atmel_start.h>

#define BUFLEN 16

#include "byte_buffer.h"

extern ByteBuffer<BUFLEN> in_buffer;
extern ByteBuffer<BUFLEN> out_buffer;

extern "C" {
  void USART_util_init(void);
}

static inline void USART_enable_udre() {
	UCSR0B |= (1 << UDRIE0);
}

static inline void USART_disable_udre() {
  UCSR0B &= ~(1 << UDRIE0);
}

#endif
