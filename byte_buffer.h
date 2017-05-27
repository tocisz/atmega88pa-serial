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

  void reset() {
    read_ptr = write_ptr;
  }

  bool is_empty() {
    return read_ptr == write_ptr;
  }

  uint8_t length() {
    return (write_ptr - read_ptr) % S;
  }

  bool is_full() {
    return length() == S-1;
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
    wrap(write_ptr);
    if (write_ptr == read_ptr) {
      // lose last byte
      ++read_ptr;
      wrap(read_ptr);
    }
  }

  void wrap(volatile uint8_t &num) {
    if (num == S) {
      num = 0;
    }
  }

private:
  uint8_t buffer[S];
  volatile uint8_t read_ptr;
  volatile uint8_t write_ptr;

};

#endif
