
/**
  TMR2 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr2.c

  @Summary
    This is the generated source file for the TMR2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for TMR2. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.145.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.20
        MPLAB 	          :  MPLAB X v5.25
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <stdio.h>
#include "tmr2.h"

/**
 Section: File specific functions
*/
void (*TMR2_InterruptHandler)(void) = NULL;
void TMR2_CallBack(void);

/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

  @Description
    This defines the object required for the maintenance of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    volatile bool           timerElapsed;
    /*Software Counter value*/
    volatile uint32_t        count;

} TMR_OBJ;

static TMR_OBJ tmr2_obj;

/**
  Section: Driver Interface
*/

void TMR2_Initialize (void)
{
    // Period = 0.001 s; Frequency = 24000000 Hz; PR2 374; 
    PR2 = 0x176;
    //  TCKPS 1:64; T32 16 Bit; TCS PBCLK; SIDL disabled; TGATE disabled; ON enabled; 
    T2CON = 0x8060;    

    if(TMR2_InterruptHandler == NULL)
    {
        TMR2_SetInterruptHandler(&TMR2_CallBack);
    }

    IFS0CLR = _IFS0_T2IF_MASK;
    IEC0SET = _IEC0_T2IE_MASK;

    tmr2_obj.timerElapsed = false;

}



void __attribute__ ((vector(_TIMER_2_VECTOR), interrupt(IPL1SOFT))) TMR2_ISR()
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin
    static volatile unsigned int CountCallBack = 0;

    // callback function - called every 374th pass
    if (++CountCallBack >= TMR2_INTERRUPT_TICKER_FACTOR)
    {
        // ticker function call
        if(TMR2_InterruptHandler) 
        { 
            TMR2_InterruptHandler(); 
        }

        // reset ticker counter
        CountCallBack = 0;
    }

    //***User Area End

    tmr2_obj.count++;
    tmr2_obj.timerElapsed = true;
    IFS0CLR = _IFS0_T2IF_MASK;
}

void TMR2_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR2 = value;
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;
}

uint16_t TMR2_Period16BitGet( void )
{
    return( PR2 );
}

void TMR2_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR2 = value;
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;
}

uint16_t TMR2_Counter16BitGet( void )
{
    return( TMR2 );
}


void __attribute__ ((weak)) TMR2_CallBack(void)
{
    // Add your custom callback code here
}

void  TMR2_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.T2IE = false;
    TMR2_InterruptHandler = InterruptHandler; 
    IEC0bits.T2IE = true;
}

void TMR2_Start( void )
{
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC0SET = _IEC0_T2IE_MASK;

    /* Start the Timer */
    T2CONSET = _T2CON_ON_MASK;
}

void TMR2_Stop( void )
{
    /* Stop the Timer */
    T2CONCLR = _T2CON_ON_MASK;

    /*Disable the interrupt*/
    IEC0CLR = _IEC0_T2IE_MASK;
}

bool TMR2_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr2_obj.timerElapsed;

    if(status == true)
    {
        tmr2_obj.timerElapsed = false;
    }
    return status;
}

uint32_t TMR2_SoftwareCounterGet(void)
{
    return tmr2_obj.count;
}

void TMR2_SoftwareCounterClear(void)
{
    tmr2_obj.count = 0; 
}

/**
 End of File
*/
