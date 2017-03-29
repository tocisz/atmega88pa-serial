#include <atmel_start.h>
#include <avr/wdt.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	PORTD = 1;
	sei();

	while (true) {
		sleep_mode();
		wdt_reset();
	}
}
