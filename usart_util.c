#include <stdio.h>
#include "usart_util.h"

char USART_in_buffer[BUFLEN];
uint8_t USART_in_begin;
volatile uint8_t USART_in_end;

char USART_out_buffer[BUFLEN];
volatile uint8_t USART_out_begin;
uint8_t USART_out_end;

static int USART_putchar(char c, FILE *stream)
{
  USART_out_buffer[USART_out_end++] = c;
  if (USART_out_end == BUFLEN) USART_out_end = 0;
  USART_enable_udre();
  return 0; // TODO overflow detection
}

static int USART_getchar(FILE *stream)
{
  if (USART_in_begin == USART_in_end) {
    return _FDEV_EOF;
  }
  char c = USART_in_buffer[USART_in_begin++];
  if (USART_in_begin == BUFLEN) USART_in_begin = 0;
  return c;
}

static FILE stdinout = FDEV_SETUP_STREAM(USART_putchar, USART_getchar, _FDEV_SETUP_RW);

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
