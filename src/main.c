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

#define LED_BLUE GPIO_PIN_2

unsigned long In; // Input from PF4
unsigned long Out; // Output (PF2), blue LED

int main(void)
{
	// Init PORTF
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable PORT F
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_BLUE) // PF2 as output (blue led)
}



