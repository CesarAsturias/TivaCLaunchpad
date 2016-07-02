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

#define LED_BLUE GPIO_PIN_2

unsigned long In; // Input from PF4
unsigned long mask=4; // Mask

int main(void)
{
	// Device configuration
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); // Configure the clocking of the device: System clock divider = 4, 
											      // PLL is used, crystal of 16 MHz, main clock is the source clock									      
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable PORT F
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_BLUE); // PF2 as output (blue led)
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0); // PF0 as input (switch 2)

	// Loop
	while(1)
	{
		In = ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0); // Read PORTF 0
		In = In>>2; // Shift into position PF2
		if (In & mask){
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 1); // Set PORTF 2
		}
		else {
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // Reset PORTF 2
		}
		

	}
}
