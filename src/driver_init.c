/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <system.h>

void tc8_0_init(void)
{

	TIMER_0_init();
}

void system_init()
{
	mcu_init();

	// PORT on PD0
	DDRD = 255;

	sysctrl_init();

	WDT_0_init();

	tc8_0_init();
}
