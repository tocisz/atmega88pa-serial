/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <port.h>

/**
 * \brief Set HEART pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void HEART_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(PORTB0, pull_mode);
}

/**
 * \brief Set HEART data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void HEART_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(PORTB0, dir);
}

/**
 * \brief Set HEART level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void HEART_set_level(const bool level)
{
	PORTB_set_pin_level(PORTB0, level);
}

/**
 * \brief Toggle output level on HEART
 *
 * Toggle the pin level
 */
static inline void HEART_toggle_level()
{
	PORTB_toggle_pin_level(PORTB0);
}

/**
 * \brief Get level on HEART
 *
 * Reads the level on a pin
 */
static inline bool HEART_get_level()
{
	return PORTB_get_pin_level(PORTB0);
}

/**
 * \brief Set Analog pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void Analog_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(PORTC4, pull_mode);
}

/**
 * \brief Set Analog data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void Analog_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(PORTC4, dir);
}

/**
 * \brief Set Analog level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void Analog_set_level(const bool level)
{
	PORTC_set_pin_level(PORTC4, level);
}

/**
 * \brief Toggle output level on Analog
 *
 * Toggle the pin level
 */
static inline void Analog_toggle_level()
{
	PORTC_toggle_pin_level(PORTC4);
}

/**
 * \brief Get level on Analog
 *
 * Reads the level on a pin
 */
static inline bool Analog_get_level()
{
	return PORTC_get_pin_level(PORTC4);
}

/**
 * \brief Set BUTTON pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void BUTTON_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(PORTC5, pull_mode);
}

/**
 * \brief Set BUTTON data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void BUTTON_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(PORTC5, dir);
}

/**
 * \brief Set BUTTON level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void BUTTON_set_level(const bool level)
{
	PORTC_set_pin_level(PORTC5, level);
}

/**
 * \brief Toggle output level on BUTTON
 *
 * Toggle the pin level
 */
static inline void BUTTON_toggle_level()
{
	PORTC_toggle_pin_level(PORTC5);
}

/**
 * \brief Get level on BUTTON
 *
 * Reads the level on a pin
 */
static inline bool BUTTON_get_level()
{
	return PORTC_get_pin_level(PORTC5);
}

/**
 * \brief Set PD0 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PD0_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(PORTD0, pull_mode);
}

/**
 * \brief Set PD0 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PD0_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(PORTD0, dir);
}

/**
 * \brief Set PD0 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PD0_set_level(const bool level)
{
	PORTD_set_pin_level(PORTD0, level);
}

/**
 * \brief Toggle output level on PD0
 *
 * Toggle the pin level
 */
static inline void PD0_toggle_level()
{
	PORTD_toggle_pin_level(PORTD0);
}

/**
 * \brief Get level on PD0
 *
 * Reads the level on a pin
 */
static inline bool PD0_get_level()
{
	return PORTD_get_pin_level(PORTD0);
}

/**
 * \brief Set PD1 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PD1_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(PORTD1, pull_mode);
}

/**
 * \brief Set PD1 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PD1_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(PORTD1, dir);
}

/**
 * \brief Set PD1 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PD1_set_level(const bool level)
{
	PORTD_set_pin_level(PORTD1, level);
}

/**
 * \brief Toggle output level on PD1
 *
 * Toggle the pin level
 */
static inline void PD1_toggle_level()
{
	PORTD_toggle_pin_level(PORTD1);
}

/**
 * \brief Get level on PD1
 *
 * Reads the level on a pin
 */
static inline bool PD1_get_level()
{
	return PORTD_get_pin_level(PORTD1);
}

/**
 * \brief Set PD2 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PD2_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(PORTD2, pull_mode);
}

/**
 * \brief Set PD2 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PD2_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(PORTD2, dir);
}

/**
 * \brief Set PD2 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PD2_set_level(const bool level)
{
	PORTD_set_pin_level(PORTD2, level);
}

/**
 * \brief Toggle output level on PD2
 *
 * Toggle the pin level
 */
static inline void PD2_toggle_level()
{
	PORTD_toggle_pin_level(PORTD2);
}

/**
 * \brief Get level on PD2
 *
 * Reads the level on a pin
 */
static inline bool PD2_get_level()
{
	return PORTD_get_pin_level(PORTD2);
}

#endif // ATMEL_START_PINS_H_INCLUDED
