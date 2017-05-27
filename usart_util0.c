#include <stdio.h>

int USART_putchar(char c, FILE *stream);
int USART_getchar(FILE *stream);

static FILE stdinout = FDEV_SETUP_STREAM(USART_putchar, USART_getchar, _FDEV_SETUP_RW);

void USART_util_init(void) {
  stdin = &stdinout;
  stdout = &stdinout;
}
