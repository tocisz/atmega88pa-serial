#include <atmel_start.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/////////////////// GOLW //////////////////
uint8_t power = 0;
int8_t dir = 1;

static inline void animate_glow(void) {
	TIMER_2_set_comp_b(power);
	power += dir;
	if (power == 0xff || (power == 0)) {
		dir = -dir;
	}
}
///////////////////////////////////////////

#include "events.h"
#include "usart_util.h"

uint16_t start_time, end_time;

char print_buffer[6];
void print_time(uint16_t val) {
	itoa(val, print_buffer, 10);
	puts(print_buffer);
}

#include <util/delay.h>
#include "apple_font.h"

#define LCDWIDTH 84
#define LCDHEIGHT 48

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80


inline static void nokia_start() {
	N_SCK_set_dir(PORT_DIR_OUT);
	N_MOSI_set_dir(PORT_DIR_OUT);
	N_SCE_set_level(false);
}

inline static void nokia_stop() {
	N_SCE_set_level(true);
}

inline static void nokia_control() {
	N_D_C_set_level(false);
}

inline static void nokia_data() {
	N_D_C_set_level(true);
}

void nokia_send(uint8_t b) {
	SPDR = b;
	while(!(SPSR & (1<<SPIF)));
}

void nokia_goto(uint8_t y, uint8_t x) {
	nokia_control();
	nokia_send(PCD8544_SETYADDR | y);
	nokia_send(PCD8544_SETXADDR | x);
}

void nokia_clear() {
	nokia_goto(0, 0);
	nokia_data();
	for (uint16_t i = 0; i < LCDWIDTH*LCDHEIGHT/8; ++i) {
		nokia_send(0);
	}
}

inline static void nokia_init(uint8_t bias, uint8_t contrast) {
	nokia_start();
	// RST is set LOW in system_init()
	// N_RST_set_level(false);
	// _delay_ms(100);
	N_RST_set_level(true);
	nokia_control();
	nokia_send(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	nokia_send(PCD8544_SETBIAS | bias);
	nokia_send(PCD8544_FUNCTIONSET);

	nokia_send(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	if (contrast > 0x7f) contrast = 0x7f;
	nokia_send(PCD8544_SETVOP | contrast);
	nokia_send(PCD8544_FUNCTIONSET);

	nokia_send(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
	nokia_clear();
}

inline static void _nokia_print(char c) {
	const uint8_t *ptr = FONT + (c - 32)*6;
	for (uint8_t i = 0; i < 6; ++i) {
		nokia_send(pgm_read_byte(ptr++));
	}
}

void nokia_print(char c) {
	nokia_data();
	_nokia_print(c);
}

void nokia_print_str(const char *s) {
	char c;
	nokia_data();
	while ((c = *(s++)) != 0) {
		_nokia_print(c);
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_util_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	cpu_irq_enable();

	nokia_init(4, 60);

	nokia_goto(2,6);
	nokia_print_str("Hello world!");
	nokia_goto(4, LCDWIDTH/2 - 6);
	nokia_print_str(":)");
	nokia_data();

	// for (uint16_t i = 0; i < LCDWIDTH*LCDHEIGHT/8; ++i) {
	// 	nokia_send(pgm_read_byte(FONT+i));
	// }
	nokia_stop();

	/* Replace with your application code */
	for(;;) {

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			if (is_button_change()) {
				clear_button_change();
				bool button = button_state_on;
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					HEART_set_level(button);
					uint16_t ctime = read_time();
					if (button) {
						start_time = ctime;
					} else {
						end_time = ctime;
						print_time(end_time - start_time);
					}
				}
			}

			if (is_new_512hz_cycle()) {
				clear_new_512hz_cycle();
				NONATOMIC_BLOCK(NONATOMIC_FORCEOFF) {
					wdt_reset();
					animate_glow();
				}
			}

			while (in_buf_length() > 0) {
				putchar(getchar());
			}
		}

		sleep_mode();

	}
}
