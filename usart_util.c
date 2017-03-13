#include <stdio.h>
#include "usart_util.h"

char USART_in_buffer[BUFLEN];
volatile uint8_t in_begin;
uint8_t in_end;

char USART_out_buffer[BUFLEN];
uint8_t out_begin;
volatile uint8_t out_end;

int uart_putchar(char c, FILE *stream)
{
  USART_out_buffer[out_end++] = c;
  if (out_end == BUFLEN) out_end = 0;
  USART_enable_udre();
  return 0; // TODO overflow detection
}

int uart_getchar(FILE *stream)
{
  if (in_begin == in_end) {
    return _FDEV_EOF;
  }
  char c = USART_in_buffer[in_begin++];
  if (in_begin == BUFLEN) in_begin = 0;
  return c;
}

static FILE stdinout = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

void USART_util_init(void) {
  stdin = &stdinout;
  stdout = &stdinout;
}

// uint8_t USART_puts(char *s) {
//   uint8_t cnt = 0;
//   while (s != )
//   // wait for previous puts to finish
//   while (USART_buffer != NULL) {
//     sleep_mode();
//   }
// 	USART_buffer = s;
// 	USART_enable_udre();
// }
