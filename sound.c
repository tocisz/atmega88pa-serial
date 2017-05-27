#include <atmel_start.h>
#include <avr/eeprom.h>
#include <util/atomic.h>

#include "freq.h"
#include "events.h"
#include "eeprom.h"

uint16_t freq_table[] = {
  C3_FREQ,
  Cs3_FREQ,
  D3_FREQ,
  Ds3_FREQ,
  E3_FREQ,
  F3_FREQ,
  Fs3_FREQ,
  G3_FREQ,
  Gs3_FREQ,
  A3_FREQ,
  As3_FREQ,
  B3_FREQ,
  C4_FREQ,
  Cs4_FREQ,
  D4_FREQ,
  Ds4_FREQ,
  E4_FREQ,
  F4_FREQ,
  Fs4_FREQ,
  G4_FREQ,
  Gs4_FREQ,
  A4_FREQ,
  As4_FREQ,
  B4_FREQ,
  C5_FREQ,
  Cs5_FREQ,
  D5_FREQ,
  Ds5_FREQ,
  E5_FREQ,
  F5_FREQ,
  Fs5_FREQ,
  G5_FREQ,
  Gs5_FREQ, // not used -- can't be encoded...
  A5_FREQ,
  As5_FREQ,
  B5_FREQ
};

#define FREQ_MASK 31
inline uint16_t decode_freq(uint8_t freq) {
  freq &= FREQ_MASK;
  return freq_table[freq];
}

uint16_t length_base = 512;

inline uint16_t decode_length(uint8_t len) {
  len >>= 5;
  if (len == 7) {
    len = 8;
  }
  if (len > 0) {
    return length_base * len / 8;
  } else {
    return length_base / 16;
  }
}

uint8_t melodies_cnt;
uint8_t current_melody;
struct melody_general *mptr;
uint8_t melody_len;
uint8_t song_buffer[128];
uint8_t note_index;

// After init_sound() is called, it's necessary to run:
// next_melody();
// before runnig start_play()
void init_sound() {
  TIMER_1_set_comp_a(0xffff);
  melodies_cnt = eeprom_read_byte(&eeprom.melodies_cnt);
  current_melody = 0xff;
}

void read_next_note();
void next_melody() {
  note_index = 0;
  ++current_melody;
  if (current_melody == melodies_cnt) {
    current_melody = 0;
  }
  if (current_melody == 0) {
    mptr = (struct melody_general *)&eeprom.melody1;
  } else {
    void *p = mptr;
    p += melody_len + 1; // extra byte for length
    mptr = (struct melody_general *)p;
  }
  uint8_t *len_ptr = &mptr->melody_len;
  melody_len = eeprom_read_byte(len_ptr);

  eeprom_read_block(song_buffer, mptr->melody, melody_len);

  // to be ready
  read_next_note();
}

#define MODE_OFF 0
#define MODE_SIMPLE 1
#define MODE_TABULAR 2

uint8_t mode = MODE_OFF;

uint16_t next_note_length = 128;
uint16_t mode_simple_freq = A3_FREQ;
// uint8_t mode_tabular_idx = 0;

#define SCHEDULED_SOUND_STOP 0
#define SCHEDULED_SOUND_START 1

bool scheduler_on = false; // scheduler is running when true
bool last_note = false; // we have reached last note

uint8_t scheduler_op = SCHEDULED_SOUND_START;
const uint8_t note_gap_time = 10;
uint16_t next_note_time = 0;

void start_generator(void) {
	switch (mode) {
		case MODE_SIMPLE:
			TIMER_1_set_comp_a(mode_simple_freq);
			break;
		// case MODE_TABULAR:
		// 	TIMER_1_set_comp_a(freq_seq[0]);
		// 	break;
	}

  TCCR1A |= (0 << COM1A1) | (1 << COM1A0); // Toggle OCA on Compare Match
  TCNT1 = 0;
  TCCR1B |= (1 << CS10); // Start timer
}

void stop_generator(void) {
  TCCR1B &= ~(1 << CS10); // Stop timer
  TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0)); // Disconnect OCA
  SPEAKER_set_level(false); // Set level to LOW
}

void sound_on(void) {
  /* Enable TC1 */
	PRR &= ~(1 << PRTIM1);

	mode = MODE_SIMPLE;
	start_generator();
}

void sound_off(void) {
	mode = MODE_OFF;
  stop_generator();

  /* Disable TC1 */
	PRR |= (1 << PRTIM1);
}

inline void schedule_gap(uint16_t scheduler_time) {
  next_note_time = scheduler_time + next_note_length - note_gap_time;
  scheduler_op = SCHEDULED_SOUND_STOP;
}

inline void schedule_note(uint16_t scheduler_time) {
  next_note_time = scheduler_time + note_gap_time;
  scheduler_op = SCHEDULED_SOUND_START;
}

void start_play(void) {
  last_note = false;
  sound_on();
  uint16_t start_time;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    start_time = time;
  }
  schedule_gap(start_time);
  scheduler_on = true;

  // to be ready
  read_next_note();
}

void stop_play(void) {
  scheduler_on = false;
  sound_off();
}

void start_new_cycle(void) {
	switch (mode) {
		case MODE_SIMPLE:
			break; // just play
		// case MODE_TABULAR:
		// 	TIMER_1_set_comp_a(freq_seq[mode_tabular_idx]);
		// 	mode_tabular_idx = (mode_tabular_idx+1) % freq_seq_len;
		// 	break;
		case MODE_OFF:
			stop_generator();
			break;
	}
}


/////////////// MELODY ////////////////
void read_next_note() {
  if (note_index == melody_len) {
    note_index = 0;
    last_note = true;
    return;
  }

  uint8_t data = song_buffer[note_index++];
  mode_simple_freq = decode_freq(data);
  next_note_length = decode_length(data);
}

void sound_scheduler() {
  if (!scheduler_on)
    return;

  uint16_t scheduler_time;
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    scheduler_time = time;
  }

  if (scheduler_time == next_note_time) {
    switch (scheduler_op) {
      case SCHEDULED_SOUND_STOP:
        if (last_note) {
          stop_play();
          Events.finished_playing = true;
        } else {
          sound_off();
          schedule_note(scheduler_time);
        }
        break;
      case SCHEDULED_SOUND_START:
        sound_on();
        schedule_gap(scheduler_time);
        read_next_note();
        break;
    }
  }
}
