#include "byte_buffer.h"

typedef ByteBuffer<16> SmallByteBuffer;

class BitBuffer {
private:
  SmallByteBuffer &byte_buf;
  uint8_t byte;
  uint8_t position;

public:
  BitBuffer(SmallByteBuffer &byte_buf)
  : byte_buf(byte_buf)
  {
    reset();
  }

  void write_bit(bool bit) {
    byte <<= 1;
    byte |= bit ? 1 : 0;
    if (++position == 8) {
      byte_buf.write_byte(byte);
      reset();
    }
  }

  void flush() {
    if (position == 0)
      return;
    byte <<= (8 - position);
    byte_buf.write_byte(byte);
    reset();
  }

private:
  void reset() {
    byte = 0;
    position = 0;
  }
};

class Capture {
public:
	uint8_t capture_read_ptr = 0;
	uint8_t state;

private:
  uint16_t *capture_buffer;
  volatile uint8_t &capture_write_ptr;

  BitBuffer bit_buffer;

	uint16_t s_short;
	uint16_t s_long;
	uint16_t s_cycle;

	uint16_t avg_cycle;
	uint16_t bit_threshold;

	uint16_t cycle_1st_low;
	uint16_t cycle_1st_high;

	//uint8_t current_bit_start;
public: // XXX
	int16_t bit_sequence_length;
	uint16_t bit_sequence_high;
	bool going_back_allowed;

public:
	Capture(
    uint16_t *capture_buffer,
    volatile uint8_t &capture_write_ptr,
    SmallByteBuffer &output
  )
   : capture_buffer(capture_buffer),
     capture_write_ptr(capture_write_ptr),
     bit_buffer(output)
  {
		init_capture();
	}

	void process_capture();

private:
	void init_capture();
	void capture_init_sequence();
	void capture_bit();
	void start_new_bit();
  void emit_bit(uint8_t bit);
};


void capture_print(void);
