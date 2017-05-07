#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include <atmel_start.h>

template <size_t S>
class ByteBuffer {

public:
  ByteBuffer() {
    read_ptr = 0;
    write_ptr = 0;
  }

  bool is_empty() {
    return read_ptr == write_ptr;
  }

  uint8_t read_byte() {
    uint8_t b = buffer[read_ptr++];
    if (read_ptr == S) {
      read_ptr = 0;
    }
    return b;
  }

  void write_byte(uint8_t b) {
    buffer[write_ptr++] = b;
    if (write_ptr == S) {
      write_ptr = 0;
    }
  }

private:
  uint8_t buffer[S];
  volatile uint8_t read_ptr;
  volatile uint8_t write_ptr;

};

#endif
