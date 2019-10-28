
#ifndef _RGB_FWK_H
#define _RGB_FWK_H

#include "../platform/RGB.h"
#include <stdbool.h>




enum GO_ROUND_STATES
{
    GO_ROUND_INIT,
    GO_ROUND_CHANGE,
    GO_ROUND_WAIT,
};

enum GO_ROUND_VARIANTS
{
    GO_ROUND_VARIANT_LAST_ALL_OFF,
    GO_ROUND_VARIANT_LAST_ALL_ON,
    GO_ROUND_VARIANT_LOOP,
};


bool RGB_goRound( RGB_color p_color, uint32_t p_period, uint8_t p_times, uint8_t p_variant );
void RGB_setAll( RGB_color p_color);


#endif