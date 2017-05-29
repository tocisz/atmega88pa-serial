#ifndef ADC_BUFFER_H
#define ADC_BUFFER_H

#include "byte_buffer.h"

template <size_t S>
class ADCBuffer {
public:
  typedef ByteBuffer<2*S+1> ByteBufferT;

  void store(uint16_t v) {
    bbs[active_bb].write_byte(convert(v));
  }

  void store2(uint8_t h, uint8_t l) {
    bbs[active_bb].write_byte(h);
    bbs[active_bb].write_byte(l);
  }

  ByteBufferT &capture() {
    uint8_t g1_active = active_bb;
    uint8_t g2_active = (~active_bb)&1;
    bbs[g2_active].reset();
    active_bb = g2_active;
    fix_inactive_parity(g1_active);
    return bbs[g1_active];
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

  void fix_inactive_parity(uint8_t inactive) {
    if (bbs[inactive].length() % 2) { // ensure parity
      bbs[inactive].read_byte();
    }
  }

};

#endif
