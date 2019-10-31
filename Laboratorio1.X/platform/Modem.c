#include "Modem.h"
#include <stdio.h>
#include <string.h>
#include "../utils/Utils.h"
#include "../mcc_generated_files/uart1.h"
#include "../mcc_generated_files/pin_manager.h"


bool MDM_Init(void)
{
    
    static UTS_DELAY_HANDLER_t MODEM_power_timer_handler = UTS_DELAY_HANDLER_3;
    static uint8_t MODEM_ESTADO = MODEM_ESTADOS_INIT;
    
    switch( MODEM_ESTADO )
    {
        case MODEM_ESTADOS_INIT:
            
            GPRS_PWR_SetLow(); //PWR OFF
            GPRS_PWR_SetDigitalOutput();
            MODEM_ESTADO = MODEM_ESTADOS_WAIT; //No agregamos break para poder iniciar el delay
                        
        case MODEM_ESTADOS_WAIT:
            if( UTS_delayms( MODEM_power_timer_handler, 2000, false ) )
            {
                GPRS_PWR_SetDigitalInput();
                MODEM_ESTADO = MODEM_ESTADOS_CHECK;
            }
            break;

        case MODEM_ESTADOS_CHECK:
            
            if( UTS_delayms( MODEM_power_timer_handler, 3000, false ) )
            {
                MODEM_ESTADO = MODEM_ESTADOS_STATUS;
            }
            break;
            
        case MODEM_ESTADOS_STATUS:
            if (GPRS_STATUS_GetValue())
            {
                MODEM_ESTADO = MODEM_ESTADOS_INIT;
                return true;
            }
//            else 
//            {
//                if ( UTS_delayms( MODEM_power_timer_handler, 3000, false ) )
//                {
//                    if (GPRS_STATUS_GetValue())
//                    {
//                        MODEM_ESTADO = MODEM_ESTADOS_INIT;
//                        return true;
//                    }
//                }
            break;
    }
    
    return false;
}


void MDM_read( uint8_t* p_string )
{
    UART1_ReadBuffer( p_string, sizeof( p_string ) );
}

uint8_t* MDM_readString()
{
    MDM_read( MDM_rxBuffer );
    return MDM_rxBuffer;
}

uint8_t MDM_write(uint8_t *p_string)
{
    if( strlen(p_string) == 0 ) return 0;  
    return UART1_WriteBuffer(*p_string,strlen(p_string));
}

