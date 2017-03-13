#include <atmel_start.h>

#define BUFLEN 8

extern char USART_in_buffer[BUFLEN];
extern volatile uint8_t in_begin;
extern uint8_t in_end;

static inline uint8_t in_buf_length() {
	return in_end-in_begin;
}

extern char USART_out_buffer[BUFLEN];
extern uint8_t out_begin;
extern volatile uint8_t out_end;

static inline uint8_t out_buf_length() {
	return out_end-out_begin;
}

void USART_util_init(void);

static inline void USART_enable_udre() {
	UCSR0B |= (1 << UDRIE0);
}

static inline void USART_disable_udre() {
  UCSR0B &= ~(1 << UDRIE0);
}
