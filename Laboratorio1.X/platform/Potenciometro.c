#include "Potenciometro.h"
#include <stdio.h>
#include <string.h>
#include "../mcc_generated_files/pin_manager.h"

bool POT_Convert( uint16_t* p_conversion )
{
    static uint8_t POT_ESTADO = POT_ESTADOS_START;
    
    switch( POT_ESTADO )
    {
        case POT_ESTADOS_START:
            ADC1_Start();
            POT_ESTADO = POT_ESTADOS_WAIT; //No agregamos break para poder iniciar el delay
                        
        case POT_ESTADOS_WAIT:
            if( ADC1_IsConversionComplete() )
            {
                POT_ESTADO = POT_ESTADOS_COMPLETE;
            }
            break;

        case POT_ESTADOS_COMPLETE:
            *p_conversion=ADC1_ConversionResultGet();
            POT_ESTADO = POT_ESTADOS_START;
            
            return true;

    }
    
    return false;
}