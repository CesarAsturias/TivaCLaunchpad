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
#include "tm4c123gh6pm.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

uint8_t In; // Input from PF0

// Function prototype
void PortF_Init(void);

int main(void)
{
	PortF_Init();

	// Loop
	while(1)
	{
		In = GPIO_PORTF_DATA_R & 0x10; // Read PORTF 4 (left button)
		if (In==0){
			GPIO_PORTF_DATA_R = LED_BLUE; // Turn on LED BLUE
		}
		else {
			GPIO_PORTF_DATA_R = LED_RED; // Turn on LED RED
		}
	}
}

/**
 * Initialization of the Port F
 *
 * This function must be called during application initialization to
 * configure the GPIO pins to which the pushbuttons are attached.
 * It enables the port used by the buttons and configures each 
 * button GPIO as an input with weak pull-up resistors.
 *
 * @return None
 */

void PortF_Init(void)
{
	volatile unsigned long delay;

	SYSCTL_RCGC2_R |= 0x00000020; // Activate PortF clock
	delay = SYSCTL_RCGC2_R; // Allow time for clock to start
	GPIO_PORTF_LOCK_R = 0x4C4F4343; // Unlock the Port
	GPIO_PORTF_CR_R = 0x01; // Enable commits to PortF[0]
	GPIO_PORTF_DIR_R = 0x0E; // PF1-3 as outputs, PF0 and PF4 inputs
	GPIO_PORTF_AFSEL_R = 0x00; // PF0-4 controlled by the GPIO registers
	GPIO_PORTF_PCTL_R = 0x00; // Ensure GPIO control by clearing the GPIOPCTL register
	GPIO_PORTF_DR2R_R = 0x1F; // Set drive strength to 2mA for all pins
	GPIO_PORTF_PUR_R = 0x11; // Enable pull-up resistors on PF0-4
	GPIO_PORTF_DEN_R = 0x1F; // Enable digital I/O on PF0-4
}
