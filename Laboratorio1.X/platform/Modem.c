#include "Modem.h"
#include <stdio.h>
#include <string.h>
#include "../utils/Utils.h"
#include "../mcc_generated_files/uart1.h"
#include "../mcc_generated_files/pin_manager.h"

#include "../framework/USB_fwk.h"



bool MDM_Init(void)
{
    static uint8_t MODEM_ESTADO = MODEM_ESTADOS_INIT;
    
    switch( MODEM_ESTADO )
    {
        case MODEM_ESTADOS_INIT:
            if( GPRS_STATUS_GetValue() )
            {
                return true;
            }
            GPRS_PWR_SetLow(); //PWR OFF
            GPRS_PWR_SetDigitalOutput();
            MODEM_ESTADO = MODEM_ESTADOS_WAIT; //No agregamos break para poder iniciar el delay
                        
        case MODEM_ESTADOS_WAIT:
            if( UTS_delayms( UTS_DELAY_HANDLER_MDM_PWR, 2000, false ) )
            {
                GPRS_PWR_SetDigitalInput();
                MODEM_ESTADO = MODEM_ESTADOS_CHECK;
            }
            break;

        case MODEM_ESTADOS_CHECK:
            
            if( UTS_delayms( UTS_DELAY_HANDLER_MDM_PWR, 3000, false ) )
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
            //@ToDo: y si no agarro????????
            break;
    }
    
    return false;
}

void MDM_read( uint8_t* p_string )
{
    UART1_ReadBuffer( p_string, sizeof( p_string ) );
    USB_write( p_string );  //debug!
}

uint8_t* MDM_readString()
{
    memset( MDM_rxBuffer, 0, sizeof( MDM_rxBuffer ) );
    MDM_read( MDM_rxBuffer );
    return MDM_rxBuffer;
}

uint8_t MDM_write(uint8_t *p_string)
{
    if( strlen(p_string) == 0 ) return 0;  
    USB_write( p_string );  //debug!
    return UART1_WriteBuffer( p_string , strlen(p_string));
}

void MDM_sendATCmd( uint8_t* p_cmd, uint8_t* p_param )
{
    uint8_t dummyBuffer[ MDM_TX_BUFFER_SIZE ];
    
    strcpy( dummyBuffer, p_cmd );
    if( p_param != NULL )
    {
        strcat( dummyBuffer, p_param );
    }
    strcat( dummyBuffer, "\r" );
    MDM_write( dummyBuffer );
    
}

MDM_AT_RESP_NAME_t MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_t p_cmd, uint8_t* p_param, uint32_t p_timeout )
{
    static  uint8_t sendAndWaitState = MODEM_ESTADOS_SEND;
    uint8_t* retPtr; 
    uint8_t i;
    switch( sendAndWaitState )
    {   
        case MODEM_ESTADOS_SEND:
            UTS_delayms( UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_TIMEOUT, p_timeout, true );
            MDM_sendATCmd( MDM_commandString( p_cmd ), p_param );
            sendAndWaitState = MODEM_ESTADOS_CHECK;
            break;
            
        case MODEM_ESTADOS_CHECK:
            MDM_read( MDM_rxBuffer );
            i = 1;
            do
            {
                retPtr = MDM_responseString( p_cmd, i );
                if( retPtr == NULL )
                {
                    sendAndWaitState = MODEM_ESTADOS_TIMEOUT_CHECK;
                    break;
                }
                else
                {
                    if( strstr( MDM_rxBuffer, retPtr ) != NULL )
                    {
                        sendAndWaitState = MODEM_ESTADOS_SEND;
                        UTS_delayms( UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_TIMEOUT, p_timeout, true );
                        return MDM_responseName( p_cmd, i );
                    }
                }
                i++;
            } while( retPtr != NULL );
            break;
            
        case MODEM_ESTADOS_TIMEOUT_CHECK:
            if( UTS_delayms( UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_TIMEOUT, p_timeout, false ) )
            {
                sendAndWaitState = MODEM_ESTADOS_SEND;
                return MDM_AT_RESP_NAME_TIMEOUT;
            }
            sendAndWaitState = MODEM_ESTADOS_CHECK;
            break;
        
        default: break;
    }
    
    return MDM_AT_RESP_NAME_WORKING;

}

MDM_AT_RESP_NAME_t MDM_responseName(MDM_AT_CMD_NAME_t p_cmd, uint8_t p_index)
{
    switch ( p_cmd )
    {
        case MDM_AT_CMD_NAME_AT:
            switch( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_ERROR;    
                case 2:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN;
            }
        
        case MDM_AT_CMD_NAME_PWR:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_GNS_PWR;
                case 2:
                    return MDM_AT_RESP_NAME_ERROR;    
                case 3:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN;       
            }            
                   
         case MDM_AT_CMD_NAME_NMAE:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_ERROR;
                case 2:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN;   
            }   
                               
         case MDM_AT_CMD_NAME_GET_INFO:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_GNS_INF;
                case 2:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN; 
            }
            
                    
         case MDM_AT_CMD_NAME_REPORTING_OFF:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_ERROR;
                case 2:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN; 
            } 
           
        default: return MDM_AT_RESP_NAME_UNKNOWN;              
    }


}

uint8_t* MDM_commandString( MDM_AT_CMD_NAME_t p_cmd )
{
    switch( p_cmd )
    {
        case MDM_AT_CMD_NAME_AT:
            strcpy (MDM_cmdBuffer,"AT" );
            return MDM_cmdBuffer;
        
        case MDM_AT_CMD_NAME_PWR:
            strcpy (MDM_cmdBuffer,"AT+CGNSPWR=" );
            return MDM_cmdBuffer;
            
        case MDM_AT_CMD_NAME_NMAE:
            strcpy (MDM_cmdBuffer,"AT+CGNSSEQ=RMC" );
            return MDM_cmdBuffer; 
            
        case MDM_AT_CMD_NAME_GET_INFO:
            strcpy (MDM_cmdBuffer,"AT+CGNSINF" );
            return MDM_cmdBuffer;
            
        case MDM_AT_CMD_NAME_REPORTING_OFF:
            strcpy (MDM_cmdBuffer,"AT+CGNSURC=0" );
            return MDM_cmdBuffer;
            
        default: return NULL;
    }
}

uint8_t* MDM_responseString(MDM_AT_CMD_NAME_t p_cmd, uint8_t p_index)
{
    memset( MDM_respBuffer, 0, sizeof( MDM_respBuffer ) );
    switch ( p_cmd )
    {
        case MDM_AT_CMD_NAME_AT:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"ERROR" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                default: return NULL;   
            }
        
        case MDM_AT_CMD_NAME_PWR:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"+CGNSPWR:" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"ERROR" );
                    return MDM_respBuffer;    
                case 3:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                default: return NULL;       
            }            
                   
         case MDM_AT_CMD_NAME_NMAE:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"ERROR" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                default: return NULL;   
            }   
                               
         case MDM_AT_CMD_NAME_GET_INFO:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"+CGNSINF:" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                default: return NULL; 
            }
            
                    
         case MDM_AT_CMD_NAME_REPORTING_OFF:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"ERROR" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                    
                default: return NULL;          
            } 
           
        default: return NULL;              
    }


}

bool MDM_sendInitialAT()
{
    static uint8_t initialATState = MODEM_ESTADOS_INIT;
    
    switch( initialATState )
            {
                case MODEM_ESTADOS_INIT:
                    MDM_write( "A" );
                    initialATState = MODEM_ESTADOS_WAIT;
                    break;
                    
                case MODEM_ESTADOS_WAIT:
                    if( UTS_delayms( UTS_DELAY_HANDLER_INITIAL_AT, 4000, false ) )
                    {
                        initialATState = MODEM_ESTADOS_SEND;
                    }
                    break;
                    
                case MODEM_ESTADOS_SEND:
                    switch( MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_AT, NULL, 10000 ) )
                    {
                        case MDM_AT_RESP_NAME_WORKING:
                            return false;
                            
                        case MDM_AT_RESP_NAME_OK:
                            initialATState = MODEM_ESTADOS_INIT;
                            return true;
                            
                        case MDM_AT_RESP_NAME_TIMEOUT:
                            initialATState = MODEM_ESTADOS_INIT;
                            return false;
                        case MDM_AT_RESP_NAME_ERROR:
                            initialATState = MODEM_ESTADOS_INIT;
                            return false;
                        case MDM_AT_RESP_NAME_UNKNOWN:
                            initialATState = MODEM_ESTADOS_INIT;
                            return false;
                        
                    }
                    break;
                
                default: break;
            
            }
    return false;
}



