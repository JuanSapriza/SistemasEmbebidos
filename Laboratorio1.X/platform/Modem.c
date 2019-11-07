#include "Modem.h"
#include <stdio.h>
#include <string.h>
#include "../utils/Utils.h"
#include "../mcc_generated_files/uart1.h"
#include "../mcc_generated_files/pin_manager.h"

#include "../framework/USB_fwk.h"


uint8_t MDM_rxBuffer[ MDM_RX_BUFFER_SIZE ]; //guarda que no puede ser mayor que el largo del buffer de la uart1!!!
uint8_t MDM_txBuffer[ MDM_TX_BUFFER_SIZE ]; 
uint8_t MDM_cmdBuffer[20];  //solo para guardar los string con los comandos
uint8_t MDM_respBuffer[20];  //solo para guardar los string con los modelos de respuesta



//<editor-fold defaultstate="collapsed" desc="Init">

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


//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="Read Write">

void MDM_read( uint8_t* p_string )
{
    static uint8_t state = 1;
    bool finished = false;
    uint8_t dummyBlockBuffer[100];
    
    while( !finished )
    {
        switch( state )
        {
            case 1: 
                memset( dummyBlockBuffer, 0, sizeof( dummyBlockBuffer ) );
                UART1_ReadBuffer( dummyBlockBuffer, sizeof( dummyBlockBuffer ));
                strcat( p_string, dummyBlockBuffer );
                state = 2;
                //intentional breakthrough

            case 2:
                if( UTS_delayms( UTS_DELAY_HANDLER_USB_READ_FROM_MODEM_ACHIQUEN, 10, false  )) 
                {
                    if( UART1_ReceiveBufferSizeGet() != 0 )  
                    { //tengo algo pa leer todavia
                        state = 1;
                        break;
                    }
                    finished = true;
                }
                break;
        }
            
    }
    USB_sniff( p_string, USB_SNIFF_TYPE_RX );
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
    USB_sniff( p_string, USB_SNIFF_TYPE_TX);
    return UART1_WriteBuffer( p_string , strlen(p_string));
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Commands">

void MDM_sendATCmd( uint8_t* p_cmd, uint8_t* p_param )
{
    if( p_cmd == NULL || p_cmd[0] == 0 ) return;
    memset( MDM_txBuffer, 0, sizeof(MDM_txBuffer ));
    strcpy( MDM_txBuffer, p_cmd );
    if( p_param != NULL )
    {
        strcat( MDM_txBuffer, p_param );
    }
    strcat( MDM_txBuffer, "\r" );
    MDM_write( MDM_txBuffer );
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
            if( UTS_delayms( UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_ACHIQUEN, 1, false ) ) //si no no le da el tiempo a que lleguen todos los caracteres!!!!! 
            {
                memset( MDM_rxBuffer, 0, sizeof( MDM_rxBuffer ) ); //esto es necesario afuera para que no borre el buffer cada vez que voy a comparar con una respuesta
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
            }
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
        
        case MDM_AT_CMD_NAME_GNS_PWR:
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
                   
         case MDM_AT_CMD_NAME_GNS_NMEA:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_ERROR;
                case 2:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN;   
            }   
                               
         case MDM_AT_CMD_NAME_GNS_GET_INFO:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_GNS_GET_INF;
                case 2:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN; 
            }
            
                    
         case MDM_AT_CMD_NAME_GNS_URC:
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
    memset( MDM_cmdBuffer, 0, sizeof(MDM_cmdBuffer) );
    switch( p_cmd )
    {
        case MDM_AT_CMD_NAME_AT:
            strcpy (MDM_cmdBuffer,"AT" );
            return MDM_cmdBuffer;
        
        case MDM_AT_CMD_NAME_GNS_PWR:
            strcpy (MDM_cmdBuffer,"AT+CGNSPWR=" );
            return MDM_cmdBuffer;
            
        case MDM_AT_CMD_NAME_GNS_NMEA:
            strcpy (MDM_cmdBuffer,"AT+CGNSSEQ=RMC" );
            return MDM_cmdBuffer; 
            
        case MDM_AT_CMD_NAME_GNS_GET_INFO:
            strcpy (MDM_cmdBuffer,"AT+CGNSINF" );
            return MDM_cmdBuffer;
            
        case MDM_AT_CMD_NAME_GNS_URC:
            strcpy (MDM_cmdBuffer,"AT+CGNSURC=" );
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
        
        case MDM_AT_CMD_NAME_GNS_PWR:
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
                   
         case MDM_AT_CMD_NAME_GNS_NMEA:
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
                               
         case MDM_AT_CMD_NAME_GNS_GET_INFO:
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
            
                    
         case MDM_AT_CMD_NAME_GNS_URC:
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

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Sequences">


//<editor-fold defaultstate="collapsed" desc="Gral">

bool MDM_sendInitialAT()
{
    static uint8_t initialATState = MODEM_ESTADOS_INIT;
    
    switch( initialATState )
            {
                case MODEM_ESTADOS_INIT:
                    MDM_write( "A\r" );
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

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="GPS">

MDM_AT_RESP_NAME_t MDM_GNSS_init( uint8_t* p_nmea )
{
    static MDM_AT_CMD_NAME_t state = MDM_AT_CMD_NAME_GNS_PWR;
    MDM_AT_RESP_NAME_t ret;
    
    switch( state )
    {
        case MDM_AT_CMD_NAME_GNS_PWR:
            ret =  MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GNS_PWR, "1", MDM_COMMAND_DEFAULT_TIMEOUT ) ;
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    state = MDM_AT_CMD_NAME_GNS_NMEA;
                    break;

                default:
                    return ret;
            }
            break;
            
        case MDM_AT_CMD_NAME_GNS_NMEA:
            ret = MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GNS_NMEA, p_nmea , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    state = MDM_AT_CMD_NAME_GNS_GET_INFO;
                    break;;

                default:
                    return ret;
            }
            break;
            
        case MDM_AT_CMD_NAME_GNS_GET_INFO:
            ret =  MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GNS_GET_INFO, NULL , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_GNS_GET_INF:
                    state = MDM_AT_CMD_NAME_GNS_PWR;
                    return ret;

                default:
                    return ret;
            }
            break;
    
    
    }
    


}



//</editor-fold>




//</editor-fold>