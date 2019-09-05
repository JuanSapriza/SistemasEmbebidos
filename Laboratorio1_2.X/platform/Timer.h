#ifndef TIMER_H
#define	TIMER_H

// INCLUSIONES
#include "HardwareProfile.h"


// DEFINICIONES
#define MAX_NUM_32_BITS     42949672965


//////////////////////////////////////////////////////
// FUNCIONES
/////////////////////////////////////////////////////
void TMR_2_init();
uint32_t TMR_2_getCount();

#endif	/* TIMER_H */

