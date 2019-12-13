
#ifndef _RGB_FWK_H
#define _RGB_FWK_H

#include "../platform/RGB.h"
#include <stdbool.h>

typedef enum 
{
    RGB_DISPLAY_TYPE_1_BY_1,
    RGB_DISPLAY_TYPE_ALL,
    RGB_DISPLAY_TYPE_GO_ROUND,
    RGB_DISPLAY_TYPE_UNDEF,
}RGB_DISPLAY_TYPES_t;

typedef struct 
{
    RGB_color color;
    uint32_t  period;
    uint8_t   times;
    uint8_t   variant;
}RGB_GO_ROUND_CONFIG_t;

extern RGB_DISPLAY_TYPES_t RGB_displayType;
extern RGB_GO_ROUND_CONFIG_t RGB_goRoundConfig;

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

RGB_GO_ROUND_CONFIG_t RGB_goRoundSet( RGB_color p_color, uint32_t p_period, uint8_t p_times, uint8_t p_variant );
bool RGB_goRound( RGB_GO_ROUND_CONFIG_t p_config );
void RGB_setAll( RGB_color p_color);


#endif