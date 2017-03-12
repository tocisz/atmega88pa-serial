#include <atmel_start.h>

extern volatile char *USART_buffer;
void USART_puts(char *s);

static inline void USART_enable_udre() {
	UCSR0B |= (1 << UDRIE0);
}

static inline void USART_disable_udre() {
  UCSR0B &= ~(1 << UDRIE0);
}
