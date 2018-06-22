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
	BUTTON_set_dir(PORT_DIR_IN);

	BUTTON_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_UP);

	EXTERNAL_IRQ_0_init();
}

void PWM_0_initialization(void)
{

        // Set pin direction to output
        PD5_set_dir(PORT_DIR_OUT);

        PD5_set_level(
            // <y> Initial level
            // <id> pad_initial_level
            // <false"> Low
            // <true"> High
            false);

        PWM_0_init();
}

void tc8_2_init(void)
{

	// Set pin direction to output
	GLOW_set_dir(PORT_DIR_OUT);

	GLOW_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	TIMER_2_init();
}

void TIMER_1_initialization(void)
{

	// Set pin direction to input
	PB0_set_dir(PORT_DIR_IN);

	PB0_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_UP);

	TIMER_1_init();
}

void system_init()
{
	mcu_init();

	sysctrl_init();

	WDT_0_init();

	exint_0_init();

	PWM_0_initialization();

	tc8_2_init();

	TIMER_1_initialization();

	USART_init();
}
