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

	// Set pin direction to output
	PD0_set_dir(PORT_DIR_OUT);

	PD0_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PD1

	// Set pin direction to output
	PD1_set_dir(PORT_DIR_OUT);

	PD1_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PD2

	// Set pin direction to output
	PD2_set_dir(PORT_DIR_OUT);

	PD2_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PD3

	// Set pin direction to output
	PD3_set_dir(PORT_DIR_OUT);

	PD3_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PD4

	// Set pin direction to output
	PD4_set_dir(PORT_DIR_OUT);

	PD4_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PD5

	// Set pin direction to output
	PD5_set_dir(PORT_DIR_OUT);

	PD5_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PD6

	// Set pin direction to output
	PD6_set_dir(PORT_DIR_OUT);

	PD6_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	sysctrl_init();

	WDT_0_init();

	tc8_0_init();
}
