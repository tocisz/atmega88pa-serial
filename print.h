#include "usart_util.h"

void print_param(const char *name, uint16_t val);
void print_align(uint16_t val);

static inline void print_pair(uint16_t a, uint16_t b) {
	print_align(a);
	print_align(b);
	putchar('\n');
	while (!out_buffer.is_empty()); //wait
}
