#include <stdio.h>
#include "usart_util.h"
#include "byte_buffer.h"

ByteBuffer<BUFLEN> in_buffer;
ByteBuffer<BUFLEN> out_buffer;

extern "C" {

int USART_putchar(char c, FILE *stream)
{
  out_buffer.write_byte(c);
  USART_enable_udre();
  return 0; // TODO overflow detection
}

int USART_getchar(FILE *stream)
{
  if (in_buffer.is_empty()) {
    return _FDEV_EOF;
  }
  return in_buffer.read_byte();
}

}
