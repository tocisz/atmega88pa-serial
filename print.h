#include "usart_util.h"

void print_param(const char *name, uint16_t val);
void print_align(uint16_t val, uint8_t align);

static inline void print_pair(uint16_t a, uint16_t b) {
	print_align(a, 2);
	print_align(b, 4);
	putchar('\n');
}
