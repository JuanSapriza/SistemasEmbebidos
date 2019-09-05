
#ifndef _UTILS_H
#define _UTILS_H

#include "../platform/Timer.h"
#include <stdbool.h>

#define UTS_DELAY_CYCLES 1000

enum UTS_DELAY_ESTADOS
{
    UTS_DELAY_ESTADOS_INIT,
    UTS_DELAY_ESTADOS_CHECK,
};


//void UTS_delayms();

bool UTS_delayms( uint32_t p_tiempo, bool p_reiniciar );









#endif