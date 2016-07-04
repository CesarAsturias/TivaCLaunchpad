/**
 * @file switch_led.c
 * Control of the led using Port F
 */

// Includes
#include <stdbool.h>
#include <stdint.h>

#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "driverlib/pin_map.h"
#include "buttons.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

uint8_t In; // Input from PF0

int main(void)
{
	// 
	// Device configuration: PLL is used, crystal of 16 MHz, main clock is the source clock
	// System clock divider = 4
	//
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//
	// PF2 as output (blue led)
	// 
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN);
	ButtonsInit(); // Init buttons

	// Loop
	while(1)
	{
		In = ROM_GPIOPinRead(BUTTONS_GPIO_BASE, RIGHT_BUTTON); // Read PORTF 0
		if (In==0){
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, LED_BLUE); // Turn on LED BLUE
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, 0);
		}
		else {
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, LED_RED); // Turn on LED RED
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, 0);
		}
	}
}
