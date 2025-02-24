/********************************************************************************
* File Name: main.c
*
* Version : 1.10
*
* Description: 
*  This is a source code for example project of 16 bit Timer UDB Block.
*
* Test Result:
*  Watch capture output on P0_0, tc output on P0_1, and Period, Compare and 
*  Count value on LCD.
*
********************************************************************************
* Copyright 2012-2017, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/
#include <project.h>
#include<stdio.h>

volatile uint8 InterruptCnt;
volatile uint32 vrijeme = 0;

char str[100];

volatile uint32 capturedValue = 0;
uint32 timerPeriod = 4294967295;

// u boju otkucaja sata Timera u mikrosekundi
uint8 glitchFilterDelay = 2;
   

/*******************************************************************************
* Define Interrupt service routine and allocate an vector to the Interrupt
********************************************************************************/
CY_ISR(InterruptHandler)
{
	/* Read Status register in order to clear the sticky Terminal Count (TC) bit 
	 * in the status register. Note that the function is not called, but rather 
	 * the status is read directly.
	 */
    
    // Returns the contents of the capture register or the output of the FIFO
    // dobivenu vrijednost oduzmi od maksimalne, pridruži odgodu signala zbog Glitch Filter-a
    capturedValue = timerPeriod - Timer_1_ReadCapture() + glitchFilterDelay;
    
    // pretvaranje u mikrosekunde:
    capturedValue = capturedValue / 2;
    
   	Timer_1_STATUS;
    Timer_1_ReadStatusRegister();
    
	/* Increment the Counter to indicate the keep track of the number of 
     * interrupts received */
    InterruptCnt++;    
}


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
* Main function performs following functions:
* 1: Enables the clock
* 2: Start the Timer
* 3: Initializes the LCD
* 4: Print Period, capture, counter value on LCD
* 
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{   
    // Enable the global interrupt */
    CyGlobalIntEnable;
    
    // Enable the Interrupt component connected to Timer interrupt */
    TimerISR_StartEx(InterruptHandler);

	// Start the components */

    USBUART_Start(0, USBUART_5V_OPERATION);
    while(USBUART_GetConfiguration() == 0){}
    
    // Enable the Timer
    Timer_1_Start();
     
    for(;;)
    { 
        sprintf( str, "\nVrijeme:  %lu\r\n", vrijeme );
        USBUART_PutString(str);
 
        CyDelay(100);
        vrijeme += 100;
        
        sprintf( str, "Period:  %lu\r\n", Timer_1_ReadPeriod() );
        USBUART_PutString(str);
        
        CyDelay(100);
        vrijeme += 100;
        
        /*
        // To display Capture value 
        sprintf( str, "Capture: %lu\r\n", Timer_1_ReadCapture() );
        USBUART_PutString(str);
        CyDelay(100);
        */
        
        /*
        // Forces a capture, and then returns the capture value
        sprintf( str, "Counter: %lu\r\n", Timer_1_ReadCounter() );
        USBUART_PutString(str);
        
        CyDelay(100);
        vrijeme += 100;
        */
        sprintf( str, "capturedValue: %lu\r\n", capturedValue );
        USBUART_PutString(str);
        
        CyDelay(100);
        vrijeme += 100;
        
        // To display Interrupt count 
        sprintf( str, "IntCnt: %d\r\n", InterruptCnt );
        USBUART_PutString(str);
		
        CyDelay(5000);
        vrijeme += 5000;
    }
}
