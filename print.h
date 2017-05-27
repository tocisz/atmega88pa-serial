#include "usart_util.h"

#ifdef __cplusplus
extern "C" {
#endif

void print_param(const char *name, uint16_t val);
void print_align(uint16_t val);

static inline void print_pair(uint16_t a, uint16_t b) {
	print_align(a);
	print_align(b);
	putchar('\n');
	while (!out_buffer_is_empty()); //wait
}

#ifdef __cplusplus
}
#endif