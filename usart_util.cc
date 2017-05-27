#include <stdio.h>
#include "usart_util.h"
#include "byte_buffer.h"

static ByteBuffer<BUFLEN> in_buffer;
static ByteBuffer<BUFLEN> out_buffer;

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

uint8_t in_buffer_read_byte() {
  return in_buffer.read_byte();
}

void in_buffer_write_byte(uint8_t b) {
  in_buffer.write_byte(b);
}

bool in_buffer_is_empty() {
  return in_buffer.is_empty();
}

uint8_t out_buffer_read_byte() {
  return out_buffer.read_byte();
}

void out_buffer_write_byte(uint8_t b) {
  out_buffer.write_byte(b);
}

bool out_buffer_is_empty() {
  return out_buffer.is_empty();
}

}
