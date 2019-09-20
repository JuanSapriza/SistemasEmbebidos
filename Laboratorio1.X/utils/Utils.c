
#include "Utils.h"





//void UTS_delayms( uint32_t p_multiplo )
//{
//    static uint32_t contadorPrevio = TMR_2_contador; 
//    while (TMR_2_contador < contadorPrevio + p_multiplo)
//    {
//        Nop();
//    }
//}


bool UTS_delayms( uint32_t p_tiempo, bool p_reiniciar )
{
    static uint8_t DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
    static uint32_t tiempo;
    static uint32_t tiempoInicial;
    
    
    uint32_t esteTiempo;
    
    
    esteTiempo = TMR_2_getCount();
    
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
            tiempoInicial = TMR_2_getCount();
            DELAY_ESTADO = UTS_DELAY_ESTADOS_CHECK;
            break;
            
        case UTS_DELAY_ESTADOS_CHECK:
            if( !( ( TMR_2_getCount() - tiempoInicial )>=0 ) ) //overflow
            {
                if( (TMR_2_getCount() + MAX_NUM_32_BITS - tiempoInicial) >= tiempo )
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
                if( ( TMR_2_getCount() - tiempoInicial ) >= tiempo )
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