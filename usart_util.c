#include "usart_util.h"

volatile char *USART_buffer = NULL;

void USART_puts(char *s) {
  // wait for previous puts to finish
  while (USART_buffer != NULL) {
    sleep_mode();
  }
	USART_buffer = s;
	USART_enable_udre();
}
