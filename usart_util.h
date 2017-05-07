#ifndef USART_UTIL_H
#define USART_UTIL_H

#include <atmel_start.h>

#define BUFLEN 16

/*
extern ByteBuffer<BUFLEN> in_buffer;
extern ByteBuffer<BUFLEN> out_buffer;

extern char USART_in_buffer[BUFLEN];
extern uint8_t USART_in_begin;
extern volatile uint8_t USART_in_end;

static inline uint8_t in_buf_length() {
	return USART_in_end-USART_in_begin;
}

extern char USART_out_buffer[BUFLEN];
extern volatile uint8_t USART_out_begin;
extern uint8_t USART_out_end;

static inline uint8_t out_buf_length() {
	return USART_out_end-USART_out_begin;
}

*/

#ifdef __cplusplus
extern "C" {
#endif

void USART_util_init(void);

uint8_t in_buffer_read_byte();
void in_buffer_write_byte(uint8_t);
bool in_buffer_is_empty();

uint8_t out_buffer_read_byte();
void out_buffer_write_byte(uint8_t);
bool out_buffer_is_empty();

static inline void USART_enable_udre() {
	UCSR0B |= (1 << UDRIE0);
}

static inline void USART_disable_udre() {
  UCSR0B &= ~(1 << UDRIE0);
}

#ifdef __cplusplus
}
#endif

#endif
