#include <avr/eeprom.h>

#include "eeprom.h"
#include "freq.h"

struct eeprom EEMEM eeprom = {
  .melodies_cnt = 2,
  .melody1 = {
    .melody_len = MELODY_LEN_1,
    .melody = {
      (E4|L_QUART), (E4|L_QUART), (E4|L_HALF),
      (E4|L_QUART), (E4|L_QUART), (E4|L_HALF),
      (E4|L_QUART), (G4|L_QUART), (C4|L_QUART_D), (D4|L_EIGHT),
      (E4|L_FULL),
      (F4|L_QUART), (F4|L_QUART), (F4|L_QUART_D), (F4|L_EIGHT),
      (F4|L_QUART), (E4|L_QUART), (E4|L_QUART), (E4|L_EIGHT), (E4|L_EIGHT),

      (E4|L_QUART), (D4|L_QUART), (D4|L_QUART), /**/(E4|L_QUART),
      (D4|L_HALF), (G4|L_HALF),

      (E4|L_QUART), (E4|L_QUART), (E4|L_HALF),
      (E4|L_QUART), (E4|L_QUART), (E4|L_HALF),
      (E4|L_QUART), (G4|L_QUART), (C4|L_QUART_D), /**/(D4|L_EIGHT),
      (E4|L_FULL),
      (F4|L_QUART), (F4|L_QUART), (F4|L_QUART_D), (F4|L_EIGHT),
      (F4|L_QUART), (E4|L_QUART), (E4|L_QUART), (E4|L_EIGHT), (E4|L_EIGHT),

      (G4|L_QUART), (G4|L_QUART), (F4|L_QUART), (D4|L_QUART),
      (C4|L_FULL)
    }
  },
  .melody2 = {
    .melody_len = MELODY_LEN_2,
    .melody = {
      (E4|L_QUART), (E4|L_QUART), (F4|L_QUART), (G4|L_QUART),
      (G4|L_QUART), (F4|L_QUART), (E4|L_QUART), (D4|L_QUART),
      (C4|L_QUART), (C4|L_QUART), (D4|L_QUART), (E4|L_QUART),
      (E4|L_QUART_D), (D4|L_EIGHT), (D4|L_HALF), // 15

      (E4|L_QUART), (E4|L_QUART), (F4|L_QUART), (G4|L_QUART),
      (G4|L_QUART), (F4|L_QUART), (E4|L_QUART), (D4|L_QUART),
      (C4|L_QUART), (C4|L_QUART), (D4|L_QUART), (E4|L_QUART),
      (D4|L_QUART_D), (C4|L_EIGHT), (C4|L_HALF), // 15

      (D4|L_QUART), (D4|L_QUART), (E4|L_QUART), (C4|L_QUART),
      (D4|L_QUART), (E4|L_EIGHT), /**/(F4|L_EIGHT), (E4|L_QUART), (C4|L_QUART),
      (D4|L_QUART), (E4|L_EIGHT), (F4|L_EIGHT), (E4|L_QUART), (D4|L_QUART),
      (C4|L_QUART), (D4|L_QUART), (G3|L_HALF), // 17

      (E4|L_QUART)/**/, (E4|L_QUART), (F4|L_QUART), (G4|L_QUART),
      (G4|L_QUART), (F4|L_QUART), (E4|L_QUART), (D4|L_QUART),
      (C4|L_QUART), (C4|L_QUART), (D4|L_QUART), (E4|L_QUART),
      (D4|L_QUART_D), (C4|L_EIGHT), (C4|L_HALF) // 15
    }
  }
};
