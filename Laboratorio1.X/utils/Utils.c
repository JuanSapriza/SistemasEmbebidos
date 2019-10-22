
#include "Utils.h"
#include "../mcc_generated_files/tmr2.h"
#include <limits.h>



static UTS_delayHandler_t UTS_delayHandler[UTS_DELAY_HANDLER_COUNT];


//bool UTS_delayms( uint32_t p_tiempo, bool p_reiniciar )
//{
//    static uint8_t DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
//    static uint32_t tiempo;
//    static uint32_t tiempoInicial = 0 ;
//    
//    
//    uint32_t esteTiempo;
//    
//    
//    esteTiempo = TMR2_SoftwareCounterGet();
//    
//    if( p_reiniciar )
//    {
//        tiempo = 0;
//        tiempoInicial = 0;
//        DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
//        return true;
//    }
//    
//    switch( DELAY_ESTADO )
//    {
//        case UTS_DELAY_ESTADOS_INIT:
//            tiempo = p_tiempo;
//            tiempoInicial = TMR2_SoftwareCounterGet();
//            DELAY_ESTADO = UTS_DELAY_ESTADOS_CHECK;
//            break;
//            
//        case UTS_DELAY_ESTADOS_CHECK:
//            if( ( ( esteTiempo - tiempoInicial ) < 0 ) ) //overflow
//            {
//                if( (esteTiempo + MAX_NUM_16_BITS+1 - tiempoInicial) >= tiempo )
//                {   // [                                                       ] 32bits
//                    //        ^tiempo Actual           ^ tiempo Inicial   
//                    //                                 |---------------------->
//                    //  |----->
//                    //                     Tiempo Transcurrido
//                    
//                    tiempo = 0;
//                    tiempoInicial = 0;
//                    DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
//                    return true;
//                }
//            }
//            else
//            {
//                if( ( esteTiempo - tiempoInicial ) >= tiempo )
//                {
//                    tiempo = 0;
//                    tiempoInicial = 0;
//                    DELAY_ESTADO = UTS_DELAY_ESTADOS_INIT;
//                    return true;
//                }
//            }
//    }
//    
//    return false;
//
//}




bool UTS_delayms( UTS_DELAY_HANDLER_t p_handlerIndex, uint32_t p_tiempo, bool p_reiniciar )
{
    uint32_t esteTiempo;
    
    if( !UTS_delayHandler[p_handlerIndex].active )
    {
        UTS_delayHandler[p_handlerIndex].initialTime = TMR2_SoftwareCounterGet();
        UTS_delayHandler[p_handlerIndex].countTime = p_tiempo;
        UTS_delayHandler[p_handlerIndex].active = true;
    }
    
    if( p_reiniciar )
    {
        UTS_delayHandler[p_handlerIndex].initialTime = 0;
        UTS_delayHandler[p_handlerIndex].countTime = 0;
        UTS_delayHandler[p_handlerIndex].active = false;
        return true;
    }
    
    esteTiempo = TMR2_SoftwareCounterGet();
    
    if( TMR2_SoftwareCounterGet() >= UTS_delayHandler[p_handlerIndex].initialTime + UTS_delayHandler[p_handlerIndex].countTime )
    {
        UTS_delayHandler[p_handlerIndex].initialTime = 0;
        UTS_delayHandler[p_handlerIndex].countTime = 0;
        UTS_delayHandler[p_handlerIndex].active = false;
        return true;
    }

//    if( ( ( esteTiempo - UTS_delayHandler[p_handlerIndex].initialTime ) < 0 ) ) //overflow
//    {
//        if( (esteTiempo + 4294967296  - UTS_delayHandler[p_handlerIndex].initialTime) >= UTS_delayHandler[p_handlerIndex].countTime )
//        {   // [                                                       ] 32bits
//            //        ^tiempo Actual           ^ tiempo Inicial   
//            //                                 |---------------------->
//            //  |----->
//            //                     Tiempo Transcurrido
//
//            UTS_delayHandler[p_handlerIndex].initialTime = 0;
//            UTS_delayHandler[p_handlerIndex].countTime = 0;
//            UTS_delayHandler[p_handlerIndex].active = false;
//            return true;
//        }
//    }
//    else
//    {
//        if( ( esteTiempo - UTS_delayHandler[p_handlerIndex].initialTime ) >= UTS_delayHandler[p_handlerIndex].countTime )
//        {
//            UTS_delayHandler[p_handlerIndex].initialTime = 0;
//            UTS_delayHandler[p_handlerIndex].countTime = 0;
//            UTS_delayHandler[p_handlerIndex].active = false;
//            return true;
//        }
//    }
    
    return false;

}
