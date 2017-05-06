class Capture {
public:
	uint8_t capture_read_ptr = 0;
	uint8_t state;

private:
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
	Capture() {
		init_capture();
	}

	void process_capture();

private:
	void init_capture();
	void capture_init_sequence();
	void capture_bit();
	void start_new_bit();
};

void capture_print(void);
