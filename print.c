#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "usart_util.h"

char print_buffer[6];
void print_param(const char *name, uint16_t val) {
	// use long to have value unsigned
	ltoa(val, print_buffer, 10);
	fputs(name, stdout);
	puts(print_buffer);
	while (out_buf_length() > 0); //wait
}

void print_align(uint16_t val) {
	ltoa(val, print_buffer, 10);
	uint8_t len = strlen(print_buffer);
	for (int8_t i = 5-len; i > 0; --i) {
		putchar(' ');
	}
	putchar(' ');
	fputs(print_buffer, stdout);
}
