/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <system.h>

void exint_0_init(void)
{
	// Set pin direction to input
	PD2_set_dir(PORT_DIR_IN);

	PD2_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	// Set pin direction to input
	BUTTON_set_dir(PORT_DIR_IN);

	BUTTON_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_UP);

	EXTERNAL_IRQ_0_init();
}

void tc8_0_init(void)
{

	TIMER_0_init();
}

void tc8_2_init(void)
{
	TIMER_2_init();
}

void tc16_1_init(void)
{
	TIMER_1_init();
}

void system_init()
{
	mcu_init();

	// PORT on PB0

	// Set pin direction to output
	HEART_set_dir(PORT_DIR_OUT);

	HEART_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PC4

	// Disable digital pin circuitry
	Analog_set_dir(PORT_DIR_OFF);
	// PORT on PB1

	// Set pin direction to output
	N_D_C_set_dir(PORT_DIR_OUT);

	N_D_C_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PB2

	// Set pin direction to output
	N_RST_set_dir(PORT_DIR_OUT);

	N_RST_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	// PORT on PB4

	// Set pin direction to output
	N_SCE_set_dir(PORT_DIR_OUT);

	N_SCE_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	sysctrl_init();

	WDT_0_init();

	exint_0_init();

	SPI_0_init();

	tc8_0_init();

	tc8_2_init();

	tc16_1_init();

	USART_init();
}
