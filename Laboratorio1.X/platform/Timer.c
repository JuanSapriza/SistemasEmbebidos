
#include <proc/p32mm0256gpm064.h>
#include <xc.h>
#include <sys/attribs.h>
#include "Timer.h"

volatile uint32_t TMR_2_contador;


void TMR_2_init()
{

    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC4bits.T2IP = 1;
    IPC4bits.T2IS = 0;
    T2CONbits.TCS = 0;
    T2CONbits.TCKPS0 = 0;
    T2CONbits.TCKPS1 = 1;
    T2CONbits.TCKPS2 = 1;
    TMR2 = 0;
    T2CONbits.ON = 1;
//    PR2 = 374;
    PR2 = 124;  //porque nos quedaba mal jejeje
    
    // f_cpu_teorica = 24MHz
    // f_cpu_nosotros_creemos = 8MHz
    // f_1_ms = 1kHz
    // Razon_creemos = 8kHz
    // 8kHz = PS . ( PRI +1 )
    // PS = 64 . ( PRI + 1 )
    // PRI = 124
    
    TMR_2_contador = 0;
    
}

void __ISR(_TIMER_2_VECTOR, IPL1SOFT) TMR2_ISR( void )
{
   IFS0bits.T2IF = 0;  // Clear interrupt flag (timer 2)
   TMR_2_contador = ( TMR_2_contador < MAX_NUM_32_BITS ? (TMR_2_contador +1) : 0 );
}


uint32_t TMR_2_getCount()
{
    return TMR_2_contador;
}