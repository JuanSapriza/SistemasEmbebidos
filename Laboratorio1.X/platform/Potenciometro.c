#include "Potenciometro.h"
#include <stdio.h>
#include <string.h>
#include "../utils/Utils.h"
#include "../mcc_generated_files/pin_manager.h"

bool POT_Convert( uint16_t* p_conversion )
{
    static uint8_t POT_ESTADO = POT_ESTADOS_START;
    
    switch( POT_ESTADO )
    {
        case POT_ESTADOS_START:
//            ADC1_ChannelSelect( ADC1_POT );
            ADC1_Start();
            POT_ESTADO = POT_ESTADOS_WAIT_1; //No agregamos break para poder iniciar el delay
            
            
        case POT_ESTADOS_WAIT_1:
            if( UTS_delayms( UTS_DELAY_HANDLER_AD_CONVERT, 1, false ) )
            {
                ADC1_Stop();
                POT_ESTADO = POT_ESTADOS_WAIT_2;
            }
            break;

            
        case POT_ESTADOS_WAIT_2:
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

uint8_t POT_Linearized ( uint16_t ADC_result )
{
    uint16_t aux1=ADC_result*60/1023;
    uint8_t aux2;
    aux2=(uint8_t)aux1;
    return aux2;
}
