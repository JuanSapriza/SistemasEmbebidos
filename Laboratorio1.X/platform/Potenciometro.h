
#ifndef _POTENCIOMETRO_H
#define _POTENCIOMETRO_H

#include <stdbool.h>
#include "../mcc_generated_files/adc1.h"
#include <stdint.h>

enum POT_ESTADO
{
    POT_ESTADOS_START,
    POT_ESTADOS_WAIT,
    POT_ESTADOS_COMPLETE,
};

bool POT_Convert( uint16_t* p_conversion );


#endif
