/*
 * readkeys.h
 *
 *  Created on: 2012
 *      Author: Lars
 */
/*
 ============================================================================
 Name        : Motor
 Author      : Lars
 Version     :
 Copyright   : Your copyright notice
 Description : H_bridge control in C, Ansi-style
 ============================================================================
 */

#include "readkeys/readkeys.h"
#include "statemashine.h"

#include "drivers/rit128x96x4.h"
#include "drivers/control.h"
#include "drivers/motor.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 	)
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

// Function prototypes
void initHW(void);

//Globel variables
int event = NO_EVENT;
int potmeter;

// With this setup it would seem like main() must be the first function in this file, otherwise
// the wrong function gets called on reset.
int main(void)
{
	volatile unsigned long ulLoop;
	volatile int event;

	initHW();

	// Start the OLED display and write a message on it

	RIT128x96x4Init(ulSSI_FREQUENCY);
	RIT128x96x4StringDraw("Hi :)", 0, 0, mainFULL_SCALE);
	RIT128x96x4StringDraw("enter the code ...", 0, 5, mainFULL_SCALE);

	// Wait for the select key to be pressed
	while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
	// Wait for the select key to be pressed
	while (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0));
	// Wait for the select key to be pressed
	while (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1));
	// Wait for the select key to be pressed
	while (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2));
	// Wait for the select key to be pressed
	while (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3));

	//
	// Loop forever.
	//
	while (1)
	{
		// This is where a statemachine could be added


		// Statmashine function
		// This is where a statemachine could be added
		// event = GetKeyEvents();
		potmeter = statemashine(GetKeyEvents());

		motor(potmeter);
		//
		// Turn on the LED.
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
		// GPIO_PORTF_DATA_R |= 0x01;

		//
		// Delay for a bit.
		// This is BAD STYLE (tm) any embedded system should be either free-running or timer based
		for (ulLoop = 0; ulLoop < 200000; ulLoop++)
			{
			}

		//
		// Turn off the LED.
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
		//GPIO_PORTF_DATA_R &= ~(0x01);

		//
		// Delay for a bit.
		//
		for (ulLoop = 0; ulLoop < 200000; ulLoop++)
			{
			}
	}
}

void initHW(void)
{
	volatile unsigned long ulLoop;
	volatile int event;

  //initHW();
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  // Enable the ports
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  // Inputs :
  // PF1 : Select button
  // PE0 : Up button
  // PE1 : Down button
  // PE2 : Left button
  // PE3 : Right button
  //

  GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

  // Outputs:
  // PF0 : Status LED
  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);

  // a short delay to ensure stable IO before running the rest of the program
  for (ulLoop = 0; ulLoop < 200; ulLoop++)
    {
    }
}