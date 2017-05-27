#ifndef ADC_BUFFER_H
#define ADC_BUFFER_H

#include "byte_buffer.h"

template <size_t S>
class ADCBuffer {
public:
  typedef ByteBuffer<S> ByteBufferT;

  void store(uint16_t v) {
    bbs[active_bb].write_byte(convert(v));
  }

  void capture() {
    uint8_t new_bb = (active_bb+1)%2;
    bbs[new_bb].reset();
    active_bb = new_bb;
  }

  ByteBufferT &get_captured() {
    return bbs[(active_bb+1)%2];
  }

  void set_middle(uint16_t m) {
    middle = m;
  }

private:
  ByteBufferT bbs[2];
  uint8_t active_bb = 0;
  uint16_t middle = 512;

  uint8_t convert(uint16_t v) {
    return (v-middle);
  }
};

#endif
