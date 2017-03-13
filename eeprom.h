#include <stdint.h>
#include <stdbool.h>

#define MELODY_LEN_1 51
#define MELODY_LEN_2 62

struct melody_general {
  uint8_t melody_len;
  uint8_t melody[];
};

struct melody_1 {
  uint8_t melody_len;
  uint8_t melody[MELODY_LEN_1];
};

struct melody_2 {
  uint8_t melody_len;
  uint8_t melody[MELODY_LEN_2];
};

struct eeprom {
  uint8_t melodies_cnt;
  struct melody_1 melody1;
  struct melody_2 melody2;
};

extern struct eeprom EEMEM eeprom;
