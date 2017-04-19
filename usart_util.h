#include <atmel_start.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BUFLEN 16

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

void USART_util_init(void);

static inline void USART_enable_udre() {
	UCSR0B |= (1 << UDRIE0);
}

static inline void USART_disable_udre() {
  UCSR0B &= ~(1 << UDRIE0);
}

#ifdef __cplusplus
}
#endif
