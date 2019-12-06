#include "Modem.h"
#include <stdio.h>
#include <string.h>
#include "../utils/Utils.h"
#include "../mcc_generated_files/uart1.h"
#include "../mcc_generated_files/pin_manager.h"

#include "../framework/USB_fwk.h"

#define SIM_PIN_JUAN "\"2222\""


uint8_t MDM_rxBuffer[ MDM_RX_BUFFER_SIZE ]; //guarda que no puede ser mayor que el largo del buffer de la uart1!!!
uint8_t MDM_txBuffer[ MDM_TX_BUFFER_SIZE ]; 
uint8_t MDM_cmdBuffer[20];  //solo para guardar los string con los comandos
uint8_t MDM_respBuffer[20];  //solo para guardar los string con los modelos de respuesta


static MDM_TASKS_t MDM_task;

void MDM_tasks()
{
    static uint8_t MDM_ESTADO = MDM_ESTADOS_INIT;
    static MDM_TASK_TASK_t mdm_task = MDM_TASK_UNDEF;
    
    switch( MDM_ESTADO )
    {
        case MDM_ESTADOS_INIT:
            if( MDM_Init() )
            {
                MDM_taskSetStatus( MDM_TASK_GET_GPS_FRAME, MDM_TASK_STATUS_UNDEF );
                MDM_taskSetStatus( MDM_TASK_SEND_SMS, MDM_TASK_STATUS_UNDEF );
                MDM_taskSetStatus( MDM_TASK_READ_SMS, MDM_TASK_STATUS_UNDEF );
                MDM_ESTADO = MDM_ESTADOS_WAIT;
            }
            break;
            
        case MDM_ESTADOS_WAIT:
            if( mdm_task == MDM_TASK_UNDEF )
            {
                if( MDM_taskGetStatus(MDM_TASK_GET_GPS_FRAME) == MDM_TASK_STATUS_NEW )
                {
                    mdm_task = MDM_TASK_GET_GPS_FRAME;
                    MDM_ESTADO = MDM_ESTADOS_EXECUTE;
                    break;
                }
                if( MDM_taskGetStatus(MDM_TASK_SEND_SMS) == MDM_TASK_STATUS_NEW )
                {
                    mdm_task = MDM_TASK_SEND_SMS;
                    MDM_ESTADO = MDM_ESTADOS_EXECUTE;
                    break;
                }
                if( MDM_taskGetStatus(MDM_TASK_READ_SMS) == MDM_TASK_STATUS_NEW )
                {
                    mdm_task = MDM_TASK_READ_SMS;
                    MDM_ESTADO = MDM_ESTADOS_EXECUTE;
                    break;
                }
            }
            break;
         
        case MDM_ESTADOS_EXECUTE:    
            switch( mdm_task )
            {
                case MDM_TASK_UNDEF: //podria hacerse al marcar el mdm task como undef, pero asi queda mas lindo
                    MDM_ESTADO = MDM_ESTADOS_WAIT;
                    break;
                
                case MDM_TASK_GET_GPS_FRAME:
                    MDM_taskSetStatus( MDM_TASK_GET_GPS_FRAME, MDM_TASK_STATUS_WORKING );
                    switch( MDM_GNSS_getInf( MDM_GNS_NMEA_RMC, true ) )
                    {
                        case MDM_AT_RESP_NAME_GNS_GET_INF:
                            MDM_taskSetStatus( MDM_TASK_GET_GPS_FRAME, MDM_TASK_STATUS_DONE );
                            mdm_task = MDM_TASK_UNDEF;
                            break;

                        case MDM_AT_RESP_NAME_ERROR:
                            MDM_taskSetStatus( MDM_TASK_GET_GPS_FRAME, MDM_TASK_STATUS_ERROR );
                            mdm_task = MDM_TASK_UNDEF;
                            break;

                        default: break; 

                    }
                    break;
                    
                        
                case MDM_TASK_READ_SMS:
                    break;
                    
                case MDM_TASK_SEND_SMS:
                    
                    
                    
                    MDM_taskSetStatus( MDM_TASK_SEND_SMS, MDM_TASK_STATUS_DONE );
                    mdm_task = MDM_TASK_UNDEF;
                    break;
                    
                    
                default: break;
            }
            break;
            
        default: break;
    }
    
}


void MDM_taskSetStatus( MDM_TASK_TASK_t p_task, MDM_TASK_STATUS_t p_status )
{
    switch( p_task )
    {
        case MDM_TASK_GET_GPS_FRAME:
            MDM_task.GPS_get.status = p_status;
            break;
    
        case MDM_TASK_READ_SMS:
            MDM_task.SMS_read.status = p_status;
            break;
    
        case MDM_TASK_SEND_SMS:
            MDM_task.SMS_send.status = p_status;
            break;
            
        default: break;
    }
}


bool MDM_taskSchedule( MDM_TASK_TASK_t p_task, void* p_taskPtr )
{
    switch( p_task )
    {
        case MDM_TASK_GET_GPS_FRAME:
            if( MDM_taskGetStatus(MDM_TASK_GET_GPS_FRAME) == MDM_TASK_STATUS_UNDEF )
            {
                MDM_taskSetStatus( MDM_TASK_GET_GPS_FRAME, MDM_TASK_STATUS_NEW );
                return true;
            }
            break;
            
        case MDM_TASK_READ_SMS:
            if( MDM_taskGetStatus(MDM_TASK_READ_SMS) == MDM_TASK_STATUS_UNDEF )
            {
                MDM_taskSetStatus( MDM_TASK_READ_SMS, MDM_TASK_STATUS_NEW );
                MDM_task.SMS_read.ptr = p_taskPtr;
                return true;
            }
            break;
            
        case MDM_TASK_SEND_SMS:
            if( MDM_taskGetStatus(MDM_TASK_SEND_SMS) == MDM_TASK_STATUS_UNDEF )
            {
                MDM_taskSetStatus( MDM_TASK_SEND_SMS, MDM_TASK_STATUS_NEW );
                MDM_task.SMS_send.ptr = p_taskPtr;
                return true;
            }
            break;
            
        default: break;
    }
    return false;
}

MDM_TASK_STATUS_t MDM_taskGetStatus( MDM_TASK_TASK_t p_task )
{
    switch( p_task )
    {
        case MDM_TASK_GET_GPS_FRAME:
            return MDM_task.GPS_get.status;
            
        case MDM_TASK_READ_SMS:
            return MDM_task.SMS_read.status;
            
        case MDM_TASK_SEND_SMS:
            return MDM_task.SMS_send.status;
            
        default: return MDM_TASK_STATUS_ERROR;
    }
}

//<editor-fold defaultstate="collapsed" desc="Init">

bool MDM_Init(void)
{
    static uint8_t MDM_ESTADO = MDM_ESTADOS_INIT;
    
    switch( MDM_ESTADO )
    {
        case MDM_ESTADOS_INIT:
            if( GPRS_STATUS_GetValue() )
            {
                return true;
            }
            GPRS_PWR_SetLow(); //PWR OFF
            GPRS_PWR_SetDigitalOutput();
            MDM_ESTADO = MDM_ESTADOS_WAIT; //No agregamos break para poder iniciar el delay
                        
        case MDM_ESTADOS_WAIT:
            if( UTS_delayms( UTS_DELAY_HANDLER_MDM_PWR, 2000, false ) )
            {
                GPRS_PWR_SetDigitalInput();
                MDM_ESTADO = MDM_ESTADOS_CHECK;
            }
            break;

        case MDM_ESTADOS_CHECK:
            
            if( UTS_delayms( UTS_DELAY_HANDLER_MDM_PWR, 3000, false ) )
            {
                MDM_ESTADO = MDM_ESTADOS_STATUS;
            }
            break;
            
        case MDM_ESTADOS_STATUS:
            if (GPRS_STATUS_GetValue())
            {
                MDM_ESTADO = MDM_ESTADOS_INIT;
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

uint8_t* MDM_whatsInReadBuffer()
{
    return MDM_rxBuffer;
}

bool MDM_writeChar( uint8_t p_char )
{
    if( p_char == 0 ) return false;  
//    USB_sniff( p_string, USB_SNIFF_TYPE_TX);
    UART1_Write( p_char );
    return true;
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
    static  uint8_t sendAndWaitState = MDM_ESTADOS_SEND;
    uint8_t* retPtr; 
    uint8_t i;
    switch( sendAndWaitState )
    {   
        case MDM_ESTADOS_SEND:
            UTS_delayms( UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_TIMEOUT, p_timeout, true );
            MDM_sendATCmd( MDM_commandString( p_cmd ), p_param );
            sendAndWaitState = MDM_ESTADOS_CHECK;
            break;
            
        case MDM_ESTADOS_CHECK:
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
                        sendAndWaitState = MDM_ESTADOS_TIMEOUT_CHECK;
                        break;
                    }
                    else
                    {
                        if( strstr( MDM_rxBuffer, retPtr ) != NULL )
                        {
                            sendAndWaitState = MDM_ESTADOS_SEND;
                            UTS_delayms( UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_TIMEOUT, p_timeout, true );
                            return MDM_responseName( p_cmd, i );
                        }
                    }
                    i++;
                } while( retPtr != NULL );
            }
            break;
            
        case MDM_ESTADOS_TIMEOUT_CHECK:
            if( UTS_delayms( UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_TIMEOUT, p_timeout, false ) )
            {
                sendAndWaitState = MDM_ESTADOS_SEND;
                return MDM_AT_RESP_NAME_TIMEOUT;
            }
            sendAndWaitState = MDM_ESTADOS_CHECK;
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
        case MDM_AT_CMD_NAME_ECHO_OFF:
        case MDM_AT_CMD_NAME_ECHO_ON:
        case MDM_AT_CMD_NAME_GNS_NMEA:
        case MDM_AT_CMD_NAME_GNS_URC:
        case MDM_AT_CMD_NAME_GSM_FUNCTIONALITY:
        case MDM_AT_CMD_NAME_GSM_SMS_FORMAT:
        case MDM_AT_CMD_NAME_GSM_SIM_PIN_SET:
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
                   
         case MDM_AT_CMD_NAME_GNS_GET_INFO:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_GNS_GET_INF;
                case 2:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN; 
            }
            
       case MDM_AT_CMD_NAME_GSM_SIM_PIN_ASK:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_GSM_SIM_PIN_NEEDED;
                case 2:
                    return MDM_AT_RESP_NAME_ERROR;    
                case 3:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN;       
            }     
            
        case MDM_AT_CMD_NAME_GSM_SMS_SEND_FOOTER:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_GSM_SMS_SENT;
                case 2:
                    return MDM_AT_RESP_NAME_ERROR;    
                case 3:
                    return MDM_AT_RESP_NAME_OK;
                default: return MDM_AT_RESP_NAME_UNKNOWN;       
            } 
          
         case MDM_AT_CMD_NAME_GSM_SMS_READ:
            switch ( p_index )
            {
                case 1:
                    return MDM_AT_RESP_NAME_GSM_SMS_READ;
                case 2:
                    return MDM_AT_RESP_NAME_ERROR;    
                case 3:
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
        
        case MDM_AT_CMD_NAME_ECHO_OFF:
            strcpy (MDM_cmdBuffer,"ATE0" );
            return MDM_cmdBuffer;
        
        case MDM_AT_CMD_NAME_ECHO_ON:
            strcpy (MDM_cmdBuffer,"ATE1" );
            return MDM_cmdBuffer;
        
        case MDM_AT_CMD_NAME_GNS_PWR:
            strcpy (MDM_cmdBuffer,"AT+CGNSPWR=" );
            return MDM_cmdBuffer;
            
        case MDM_AT_CMD_NAME_GNS_NMEA:
            strcpy (MDM_cmdBuffer,"AT+CGNSSEQ=" );
            return MDM_cmdBuffer; 
            
        case MDM_AT_CMD_NAME_GNS_GET_INFO:
            strcpy (MDM_cmdBuffer,"AT+CGNSINF" );
            return MDM_cmdBuffer;
            
        case MDM_AT_CMD_NAME_GSM_FUNCTIONALITY:
            strcpy (MDM_cmdBuffer,"AT+CFUN=" );
            return MDM_cmdBuffer;
            
        case MDM_AT_CMD_NAME_GSM_SIM_PIN_ASK:
            strcpy (MDM_cmdBuffer,"AT+CPIN?" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_SIM_PIN_SET:
            strcpy (MDM_cmdBuffer,"AT+CPIN=" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_SMS_FORMAT:
            strcpy (MDM_cmdBuffer,"AT+CMGF=" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_SMS_SEND_HEADER:
            strcpy (MDM_cmdBuffer,"AT+CMGS=" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_SMS_SEND_FOOTER:
            MDM_cmdBuffer[0]=0x1A;
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_SMS_READ:
            strcpy (MDM_cmdBuffer,"AT+CMGR=" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_SMS_LIST:
            strcpy (MDM_cmdBuffer,"AT+CMGL=\"ALL\"" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_CALL_MAKE:
            strcpy (MDM_cmdBuffer,"ATD" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_CALL_ANSWER:
            strcpy (MDM_cmdBuffer,"ATA" );
            return MDM_cmdBuffer;
                        
        case MDM_AT_CMD_NAME_GSM_CALL_HANG:
            strcpy (MDM_cmdBuffer,"ATH" );
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
        case MDM_AT_CMD_NAME_ECHO_ON:
        case MDM_AT_CMD_NAME_ECHO_OFF:
        case MDM_AT_CMD_NAME_GNS_NMEA:
        case MDM_AT_CMD_NAME_GNS_URC:
        case MDM_AT_CMD_NAME_GSM_FUNCTIONALITY:
        case MDM_AT_CMD_NAME_GSM_SMS_FORMAT:
        case MDM_AT_CMD_NAME_GSM_SIM_PIN_SET:
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
            
         case MDM_AT_CMD_NAME_GSM_SIM_PIN_ASK:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"+CPIN" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"ERROR" );
                    return MDM_respBuffer;    
                case 3:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                default: return NULL; 
            }
            
         case MDM_AT_CMD_NAME_GSM_SMS_SEND_FOOTER:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"+CMGS:" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"ERROR" );
                    return MDM_respBuffer;    
                case 3:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                default: return NULL; 
            }
            
         case MDM_AT_CMD_NAME_GSM_SMS_READ:
            switch ( p_index )
            {
                case 1:
                    strcpy (MDM_respBuffer,"+CMGR:" );
                    return MDM_respBuffer;
                case 2:
                    strcpy (MDM_respBuffer,"ERROR" );
                    return MDM_respBuffer;    
                case 3:
                    strcpy (MDM_respBuffer,"OK" );
                    return MDM_respBuffer;
                default: return NULL; 
            }   
            
           
        default: return NULL;              
    }


}

uint8_t* MDM_unsolicitedResponseString( MDM_AT_RESP_NAME_t p_response )
{
    switch( p_response )
    {
        case MDM_AT_RESP_NAME_GSM_SIM_PIN_READY:
            strcpy (MDM_respBuffer,"+CPIN: READY" );
            return MDM_respBuffer;    
            
        case MDM_AT_RESP_NAME_GSM_SIM_CALL_READY:
            strcpy (MDM_respBuffer,"Call Ready" );
            return MDM_respBuffer;    
            
        case MDM_AT_RESP_NAME_GSM_SIM_SMS_READY:
            strcpy (MDM_respBuffer,"SMS Ready" );
            return MDM_respBuffer;    
            
        case MDM_AT_RESP_NAME_GSM_CALL_INCOMING:
            strcpy (MDM_respBuffer,"RING" );
            return MDM_respBuffer;    
            
        case MDM_AT_RESP_NAME_GSM_SMS_INCOMING:
            strcpy (MDM_respBuffer,"+CMTI:" );
            return MDM_respBuffer;    

        default:
            return NULL;
    }
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Sequences">


//<editor-fold defaultstate="collapsed" desc="Gral">

bool MDM_sendInitialAT()
{
    static uint8_t initialATState = MDM_ESTADOS_INIT;
    
    switch( initialATState )
            {
                case MDM_ESTADOS_INIT:
                    MDM_write( "A\r" );
                    initialATState = MDM_ESTADOS_WAIT;
                    break;
                    
                case MDM_ESTADOS_WAIT:
                    if( UTS_delayms( UTS_DELAY_HANDLER_INITIAL_AT, 4000, false ) )
                    {
                        initialATState = MDM_ESTADOS_SEND;
                    }
                    break;
                    
                case MDM_ESTADOS_SEND:
                    switch( MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_AT, NULL, 10000 ) )
                    {
                        case MDM_AT_RESP_NAME_WORKING:
                            return false;
                            
                        case MDM_AT_RESP_NAME_OK:
                            initialATState = MDM_ESTADOS_INIT;
                            return true;
                            
                        case MDM_AT_RESP_NAME_TIMEOUT:
                            initialATState = MDM_ESTADOS_INIT;
                            return false;
                            
                        case MDM_AT_RESP_NAME_ERROR:
                            initialATState = MDM_ESTADOS_INIT;
                            return false;
                            
                        case MDM_AT_RESP_NAME_UNKNOWN:
                            initialATState = MDM_ESTADOS_INIT;
                            return false;
                        
                    }
                    break;
                
                default: break;
            
            }
    return false;
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="GPS">


uint8_t* MDM_GNSS_nmea2String( MDM_GNS_NMEA_t p_nmea );

MDM_AT_RESP_NAME_t MDM_GNSS_getInf( MDM_GNS_NMEA_t p_nmea, bool p_pwr )
{
    static MDM_AT_CMD_NAME_t state = MDM_AT_CMD_NAME_GNS_PWR;
    MDM_AT_RESP_NAME_t ret;
    
    state = ( !p_pwr ? MDM_AT_CMD_NAME_GNS_PWR : state );
    
    switch( state )
    {
        case MDM_AT_CMD_NAME_GNS_PWR:
            ret = ( p_pwr ? MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GNS_PWR, "1", MDM_COMMAND_DEFAULT_TIMEOUT ) : MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GNS_PWR, "0", MDM_COMMAND_DEFAULT_TIMEOUT ) );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    state = ( p_pwr ? MDM_AT_CMD_NAME_GNS_NMEA : state );
                    break;

                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                     break;
            }
            break;
            
        case MDM_AT_CMD_NAME_GNS_NMEA:
            ret = MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GNS_NMEA, MDM_GNSS_nmea2String( p_nmea ) , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    state = MDM_AT_CMD_NAME_GNS_GET_INFO;
                    break;
                    
                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                    break;
            }
            break;
            
        case MDM_AT_CMD_NAME_GNS_GET_INFO:
            ret =  MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GNS_GET_INFO, NULL , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_GNS_GET_INF:
                    return ret;

                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                     break;
            }
            break;
    }
    return MDM_AT_RESP_NAME_WORKING;
}

uint8_t* MDM_GNSS_nmea2String( MDM_GNS_NMEA_t p_nmea )
{
    static uint8_t MDM_nmeaBuffer[4];           //QUE TAN PROLIJO ES HACER ESTOOO???
    
    switch( p_nmea )
    {
        case MDM_GNS_NMEA_RMC:
            strcpy(MDM_nmeaBuffer,"RMC");
            break;
        case MDM_GNS_NMEA_GGA:
            strcpy(MDM_nmeaBuffer,"GGA");
            break;
            
        case MDM_GNS_NMEA_GSV:
            strcpy(MDM_nmeaBuffer,"GSV");
            break;
            
        case MDM_GNS_NMEA_GSA:
            strcpy(MDM_nmeaBuffer,"GSA");
            break;
            
        default:
            memset( MDM_nmeaBuffer,0,sizeof(MDM_nmeaBuffer) );
            break;
    }
    return MDM_nmeaBuffer;
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="GSM">

MDM_AT_RESP_NAME_t MDM_GSM_init()
{
    static MDM_AT_CMD_NAME_t state = MDM_AT_CMD_NAME_GSM_FUNCTIONALITY;
    MDM_AT_RESP_NAME_t ret;
    
    switch( state )
    {
        case MDM_AT_CMD_NAME_GSM_FUNCTIONALITY:
            ret = MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GSM_FUNCTIONALITY, "1,1" , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    state = MDM_AT_CMD_NAME_ECHO_OFF;
                    break;

                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                     break;
            }
            break;
            
        case MDM_AT_CMD_NAME_ECHO_OFF:
            ret = MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_ECHO_OFF, NULL , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    state = MDM_AT_CMD_NAME_GSM_SIM_PIN_ASK;
                    break;

                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                     break;
            }
            break;
            
        case MDM_AT_CMD_NAME_GSM_SIM_PIN_ASK:
            ret = MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GSM_SIM_PIN_ASK, NULL , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_GSM_SIM_PIN_NEEDED:
                    state = MDM_AT_CMD_NAME_GSM_SIM_PIN_SET;
                    break;
                
                case MDM_AT_RESP_NAME_OK:
                    state = MDM_COMMAND_SEQ_WAIT_4_TIMEOUT;
                    break;
                    
                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                    break;
            }
            break;
            
        case MDM_AT_CMD_NAME_GSM_SIM_PIN_SET:
            ret =  MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GSM_SIM_PIN_SET, SIM_PIN_JUAN , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    state = MDM_COMMAND_SEQ_WAIT_4_TIMEOUT;
                    break;
                    
                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                     break;
            }
            break;
            
        case MDM_COMMAND_SEQ_WAIT_4_TIMEOUT:
            if( strstr( MDM_readString(), "SMS Ready" ) != 0 )
            {
                state = MDM_AT_CMD_NAME_GSM_SMS_FORMAT;
            }
            //            if( UTS_delayms(UTS_DELAY_HANDLER_MDM_WAIT, MDM_SIM_GET_READY_TIMEOUT, false ) )
//            {
//                state = MDM_AT_CMD_NAME_GSM_SMS_FORMAT;
//            }
            break;
            
        case MDM_COMMAND_SEQ_WAIT_4_RESPONSE:
            if( strstr( MDM_readString(), MDM_unsolicitedResponseString( MDM_AT_RESP_NAME_GSM_SIM_SMS_READY )) != NULL)
            {
                state = MDM_COMMAND_SEQ_WAIT_4_TIMEOUT;
            }
            break;
            
        case MDM_AT_CMD_NAME_GSM_SMS_FORMAT:
            ret =  MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GSM_SMS_FORMAT, "1" , MDM_COMMAND_DEFAULT_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_OK:
                    return MDM_AT_RESP_NAME_OK;
                    break;
                    
                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    
                default:
                     break;
            }
            break;            
            
        
    }
    return MDM_AT_RESP_NAME_WORKING;
}



MDM_AT_RESP_NAME_t MDM_sendSMS( uint8_t* p_phoneNr, uint8_t* p_string )
{
    static MDM_AT_CMD_NAME_t state = MDM_AT_CMD_NAME_GSM_SMS_SEND_HEADER;
    MDM_AT_RESP_NAME_t ret;
    
    switch( state )
    {
        case MDM_AT_CMD_NAME_GSM_SMS_SEND_HEADER:
            MDM_sendATCmd( MDM_commandString( MDM_AT_CMD_NAME_GSM_SMS_SEND_HEADER ), p_phoneNr );
            state = MDM_COMMAND_SEQ_WRITE;
            break;   
            
        case MDM_COMMAND_SEQ_WRITE:
            MDM_write( p_string );
            state = MDM_COMMAND_SEQ_WAIT_4_TIMEOUT;
            break;
            
        case MDM_COMMAND_SEQ_WAIT_4_TIMEOUT:
            if( UTS_delayms(UTS_DELAY_HANDLER_MDM_WAIT, MDM_SMS_WAIT_TIME, false ) )
            {
                state = MDM_AT_CMD_NAME_GSM_SMS_SEND_FOOTER;
            }
            break;
            
        case MDM_AT_CMD_NAME_GSM_SMS_SEND_FOOTER:
//            USB_send2Modem();
            ret =  MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_GSM_SMS_SEND_FOOTER, NULL , MDM_COMMAND_SUPERLONG_TIMEOUT );
            switch( ret )
            {
                case MDM_AT_RESP_NAME_GSM_SMS_SENT:
                case MDM_AT_RESP_NAME_ERROR:
                    return ret;
                    break;

                default:
                     break;
            }
            break;
        
        default: break;
    }
    return MDM_AT_RESP_NAME_WORKING;
}


//</editor-fold>


//</editor-fold>




