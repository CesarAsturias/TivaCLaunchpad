/**
 *
 * @file buttons.c
 *
 * Buttons driver
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "buttons.h"

/*****************************
 *
 * Holds the current, debounced, state of each button.
 * A 0 in a bit indicates that that button is currently
 * pressed, otherwise it is released.
 * 
 **************************/
static uint8_t g_ui8ButtonStates = ALL_BUTTONS;

/**
 *
 * Polls the current state of the buttons and determines which have changed
 *
 * @param pui8Delta Points to a character that will be written to indicate
 * which button states changed since the last time this function was called
 * This value is derived from the debounced state of the button.
 * @param pui8RawState Points to a location where the raw button state
 * will be stored
 *
 * This function should be called periodically by the application to poll
 * the pushbuttons. It determines both the current debounced state of 
 * the buttons and also which buttons have changed state since the last 
 * time the function was called.
 *
 * In order for button debouncing to work properly, this function should
 * be called at a regular interval, even if the state of the buttons
 * is not needed that often.
 *
 * If button debouncing is not required, then the caller can pass a pointer
 * for the @e pui8RawState parameter in order to get the raw state of the
 * buttons. The value returned in @e pui8RawState wll be a bit mask where
 * a 1 indicates the buttons is pressed
 *
 * @return Returns the current debounced state of the buttons where a 1
 * in the button ID's position indicates that the button is pressed and 
 * a 0 indicates that it is released.
 *
 */
uint8_t ButtonsPoll(uint8_t *pui8Delta, uint8_t *pui8RawState)
{
	uint32_t ui32Delta;
	uint32_t ui32Data;
	static uint8_t ui8SwitchClockA = 0;
	static uint8_t ui8SwitchClockB = 0;

	//
	// Read the raw state of the push buttons. Save the
	// raw state (inverting the bit sense) if the caller 
	// supplied storage for the raw value.
	//
	ui32Data = (ROM_GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS));
	if(pui8RawState)
	{
		*pui8RawState = (uint8_t)~ui32Data; // Flip bits with ~
	}

	// 
	// Determine the switches that are at a different state than
	// the debounced state
	//
	ui32Delta = ui32Data ^ g_ui8ButtonStates; // XOR
	
	//
	// Increment the clocks by one
	//
	ui8SwitchClockA ^= ui8SwitchClockB;
	ui8SwitchClockB = ~ui8SwitchClockB;

	//
	// Reset the clocks corresopnding to switches that have not changed state
	//
	ui8SwitchClockA &= ui32Delta;
	ui8SwitchClockB &= ui32Delta;

	//
	// Get the new debounced state.
	//
	g_ui8ButtonStates &= ui8SwitchClockA | ui8SwitchClockB;
	g_ui8ButtonStates |= (~(ui8SwitchClockA | ui8SwitchClockB)) & ui32Data;

	//
	// Determine the switches that just changed debounced state.
	//
	ui32Delta ^= (ui8SwitchClockA | ui8SwitchClockB);

	// 
	// Store the bit mask for the buttons that have changed for
	// return to caller
	//
	if(pui8Delta)
	{
		*pui8Delta = (uint8_t)ui32Delta;
	}

	//
	// Return the debounced buttons states to the caller. Invert
	// the bit sense so that a '1' indicates the button is pressed,
	// which is a sensible way to interprete the return value.
	//
	return(~g_ui8ButtonStates);
}

/**
 * Initializes the GPIO pins used by the pushbuttons
 *
 * This function must be called during application initializatin to
 * configure the GPIO pins to which the pushbuttons are attached.
 * It enables the port used by the buttons and configures each button
 * GPIO as an input with a weak pull-up
 *
 * @return None
 *
 */
void ButtonsInit(void)
{
	//
	// Enable the GPIO port to which the pushbuttons are connected
	//
	ROM_SysCtlPeripheralEnable(BUTTONS_GPIO_PERIPH);

	// 
	// Unlock PF0 so we can change it to a GPIO Input.
	// First, we write 0x4C4F434B to the GPIOLOCK register, 
	// enabling access to the GPIOCR register. This key is
	// defined in inc/hw_gpio.h.
	// Then, set the first bit in the GPIOCR register (see
	// page 685 of the datasheet) so we can commit our
	// changes to the corresponding bit of the GPIOAFSEL,
	// GPIOPUR, GPIOPDR of GPIODEN registers is commited to
	// the register and reflects the new value.
	// Once we have enabled (unlocked) the commit register
	// then re-lock it to prevent further changes. PF0 is
	// muxed with NMI thus a special case
	//
	HWREG(BUTTONS_GPIO_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(BUTTONS_GPIO_BASE + GPIO_O_CR) |= 0x01;
	HWREG(BUTTONS_GPIO_BASE + GPIO_O_LOCK) = 0;

	//
	// Set each of the button GPIO pins as an input with a pull-up
	//
	ROM_GPIODirModeSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_DIR_MODE_IN);
	MAP_GPIOPadConfigSet(BUTTONS_GPIO_BASE, ALL_BUTTONS,
			     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//
	// Initialize the debounced button state with the current state read 
	// from the GPIO bank
	//
	g_ui8ButtonStates = ROM_GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS);
}
