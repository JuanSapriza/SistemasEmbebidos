
#ifndef _POTENCIOMETRO_H
#define _POTENCIOMETRO_H

#include <stdbool.h>
#include "../mcc_generated_files/adc1.h"
#include <stdint.h>

enum POT_ESTADO
{
    POT_ESTADOS_START,
    POT_ESTADOS_WAIT_1,
    POT_ESTADOS_WAIT_2,
    POT_ESTADOS_COMPLETE,
};

bool POT_Convert( uint16_t* p_conversion );

//bool POT_Convert( uint16_t* p_conversion )

uint8_t POT_Linearized ( uint16_t ADC_result );


#endif
