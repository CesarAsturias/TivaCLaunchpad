//**********************************************************
//!  @file buttons.h
//!  Defines for the hardware resources used by the pushbuttons.
//!  The switches are on the following ports/pins:
//!
//!  PF4 - Left Button
//!  PF0 - Right Button
//!
//!  The switches tie to the GPIO ground, so the GPIOs need to be configured with
//!  pull-ups, and a value of 0 means the switch is pressed.
//!
//
//  *********************************************************

#ifndef __BUTTONS_H__
#define __BUTTONS_H__

//**********************************************************
//
// If building with a C++ compiler, make all of the 
// definitions in this header have a C binding
//
// *********************************************************

#ifdef __cplusplus
extern "C"
{
#endif

//*********************************************

#define BUTTONS_GPIO_PERIPH	SYSCTL_PERIPH_GPIOF
#define BUTTONS_GPIO_BASE	GPIO_PORTF_BASE

#define NUM_BUTTONS		2
#define LEFT_BUTTON		GPIO_PIN_4
#define RIGHT_BUTTON 		GPIO_PIN_0

#define ALL_BUTTONS		(LEFT_BUTTON | RIGHT_BUTTON)
	
//***************************************
// 
// Useful macros for detecting button events
//
// *****************************************
#define BUTTON_PRESSED(button, buttons, changed)	\
	(((button) & (changed)) && ((button) & (buttons)))

#define BUTTON_RELEASED(button, buttons, changed)	\
	(((button) & (changed)) && ((button) & (buttons)))

//***************************************************
// If building with a C++ compiler, make all of the 
// definitions in this header have a C binding
//
// *********************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************
//
// Functions exported from buttons.c
//
// ****************************************************
extern void ButtonsInit(void);
extern uint8_t ButtonsPoll(uint8_t *pui8Delta, uint8_t *pui8Raw);

//********************************************
//
// Mark the end of the C bindings section for C++ compilers
//
// *******************************************

#ifdef __cplusplus
}
#endif

#endif // __BUTTONS_H_
