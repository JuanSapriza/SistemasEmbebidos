
#include "Utils.h"
#include "../mcc_generated_files/tmr2.h"




bool UTS_delayms( uint32_t p_tiempo, bool p_reiniciar )
{
    static uint8_t DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
    static uint32_t tiempo;
    static uint32_t tiempoInicial = 0 ;
    
    
    uint32_t esteTiempo;
    
    
    esteTiempo = TMR2_SoftwareCounterGet();
    
    if( p_reiniciar )
    {
        tiempo = 0;
        tiempoInicial = 0;
        DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
        return true;
    }
    
    switch( DELAY_ESTADO )
    {
        case UTS_DELAY_ESTADOS_INIT:
            tiempo = p_tiempo;
            tiempoInicial = TMR2_SoftwareCounterGet();
            DELAY_ESTADO = UTS_DELAY_ESTADOS_CHECK;
            break;
            
        case UTS_DELAY_ESTADOS_CHECK:
            if( ( ( esteTiempo - tiempoInicial ) < 0 ) ) //overflow
            {
                if( (esteTiempo + MAX_NUM_16_BITS+1 - tiempoInicial) >= tiempo )
                {   // [                                                       ] 32bits
                    //        ^tiempo Actual           ^ tiempo Inicial   
                    //                                 |---------------------->
                    //  |----->
                    //                     Tiempo Transcurrido
                    
                    tiempo = 0;
                    tiempoInicial = 0;
                    DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
                    return true;
                }
            }
            else
            {
                if( ( esteTiempo - tiempoInicial ) >= tiempo )
                {
                    tiempo = 0;
                    tiempoInicial = 0;
                    DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
                    return true;
                }
            }
    }
    
    return false;

}