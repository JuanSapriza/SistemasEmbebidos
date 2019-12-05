#include "App.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/rtcc.h"
#include "mcc_generated_files/usb/usb.h"
#include "mcc_generated_files/uart1.h"
#include "platform/Buttons.h"
#include "platform/GPS.h"
#include "platform/Modem.h"
#include "platform/Potenciometro.h"
#include "framework/RGB_fwk.h"
#include "framework/USB_fwk.h"
#include "framework/RTCC_fwk.h"
#include "utils/Utils.h"

APP_var_t APP_info;

APP_var_t APP_logBuffer[APP_LOG_BUFFER_SIZE];
uint32_t APP_logBufferHead;
bool APP_convertPot2RGB;


bool APP_pot2RGBIsEnabled();
void APP_pot2RGBEnable( bool p_enable );
void APP_changePlantID( uint16_t p_newID );
APP_FUNC_STATUS_t APP_getNewPlantID();
APP_FUNC_STATUS_t APP_setThresholds();

void APP_THRESHOLD_initialize()
{
    APP_info.thresholds.saturated=APP_THRESHOLD_SATURATED_DEFAULT;
    APP_info.thresholds.slightly_saturated=APP_THRESHOLD_SLIGHTLY_SATURATED_DEFAULT;
    APP_info.thresholds.slightly_dry=APP_THRESHOLD_SLIGHTLY_DRY_DEFAULT;
    APP_info.thresholds.dry=APP_THRESHOLD_DRY_DEFAULT;
    APP_info.thresholds.low_automatic=APP_THRESHOLD_LOW_AUTOMATIC_DEFAULT;
    APP_info.thresholds.high_automatic=APP_THRESHOLD_HIGH_AUTOMATIC_DEFAULT;
    APP_info.thresholds.manual=APP_THRESHOLD_MANUAL_DEFAULT;
}

void APP_THRESHOLD_set (APP_THRESHOLD_t p_threshold  )
{
    APP_info.thresholds.saturated=p_threshold.saturated;
    APP_info.thresholds.slightly_saturated= p_threshold.slightly_saturated;
    APP_info.thresholds.slightly_dry=p_threshold.slightly_dry;
    APP_info.thresholds.dry=p_threshold.dry;
    APP_info.thresholds.low_automatic=p_threshold.low_automatic;
    APP_info.thresholds.high_automatic=p_threshold.high_automatic;
    APP_info.thresholds.manual=p_threshold.manual;
}

void APP_RGB_humidity ( uint8_t ADC_humedad )
{
    
    if( ( (ADC_humedad>=0) && (ADC_humedad<=APP_info.thresholds.saturated) )  )
    {
        RGB_setAll( BLUE );
    }            
     
    if( ( (ADC_humedad>APP_info.thresholds.saturated) && (ADC_humedad<=APP_info.thresholds.slightly_saturated) )  ) 
    {
        RGB_setAll( WATER_GREEN );
    }
    
    if( (ADC_humedad>APP_info.thresholds.slightly_saturated) && (ADC_humedad<=APP_info.thresholds.slightly_dry) ) 
    {
        RGB_setAll( GREEN );
    }
    
    if( ( (ADC_humedad>APP_info.thresholds.slightly_dry) && (ADC_humedad<=APP_info.thresholds.dry) )) 
    {
        RGB_setAll( YELLOW );
    }
    
    if( (ADC_humedad>APP_info.thresholds.dry) && (ADC_humedad<=60)  )
    {
        RGB_setAll( RED );
    }
        
}

void APP_LEDA_irrigate ( uint8_t ADC_humedad )
{
    static uint8_t APP_IRRIGATE = APP_IRRIGATE_OFF;
    
    static uint8_t APP_LEDA = APP_LEDA_OFF;
        
    
    switch ( APP_IRRIGATE )
    {
        case APP_IRRIGATE_OFF:        
            if( (ADC_humedad)>APP_info.thresholds.high_automatic )
            {
                APP_LEDA=APP_LEDA_ON;
                APP_IRRIGATE = APP_IRRIGATE_ON;
            }
            break;
            
        case APP_IRRIGATE_ON:   
            if( (ADC_humedad)<APP_info.thresholds.low_automatic )
            {
                APP_LEDA=APP_LEDA_OFF;
                APP_IRRIGATE = APP_IRRIGATE_OFF;
            }
            break;
            
        default: break;
            
    }    
    
    switch ( APP_LEDA )
    {
        case APP_LEDA_ON:
            LED_A_SetHigh();
            break;    
    
        case APP_LEDA_OFF:
            LED_A_SetLow();
            break;  
            
        default: break;
    
    }

    
}    
    
void APP_changePlantID( uint16_t p_newID )
{
    APP_info.plantID = p_newID;
}
   

APP_FUNC_STATUS_t APP_getNewPlantID()
{
    static uint8_t state = APP_GET_NEW_ID_SHOW;
    static int32_t aux; 
    
    switch( state )
    {
        case APP_GET_NEW_ID_SHOW:
            USB_write("\n\n Ingrese el Identificador de la Planta \n");
            USB_write(    "         - máximo 4 dígitos -  \n        ");
            state = APP_GET_NEW_ID_WAIT;
            //intentional breakthrough
            
        case APP_GET_NEW_ID_WAIT:
            if( USB_sth2Read() )
            {
                state = APP_GET_NEW_ID_VALIDATE;
            }
            else
            {
                break;
            }
            //intentional breakthrough
            
        case APP_GET_NEW_ID_VALIDATE:
            if( strstr( USB_whatsInReadBuffer(),USB_FWK_RETURN_CHAR ) != NULL )
            {
                state = APP_GET_NEW_ID_SHOW;
                return APP_FUNC_RETURN;
            }
            aux = (int32_t) atoi( USB_whatsInReadBuffer() );
            if( aux > 0 && aux <= APP_PLANT_ID_MAX_NUM )
            {
                state = APP_GET_NEW_ID_RESPONSE_OK;
            }
            else
            {
                state = APP_GET_NEW_ID_RESPONSE_ERROR;
            }
            break;
            
        case APP_GET_NEW_ID_RESPONSE_OK:
            USB_write("\n\n ID configurado correctamente! \n");
            APP_changePlantID( (uint16_t) aux);
            state = APP_GET_NEW_ID_SHOW;
            return APP_FUNC_DONE;
            
        case APP_GET_NEW_ID_RESPONSE_ERROR:
            USB_write("\n\n ERROR \n");
            sprintf( USB_dummyBuffer, "\n\n ingrese un número entre 0 y %d \n",APP_PLANT_ID_MAX_NUM );
            USB_write(USB_dummyBuffer);
            state = APP_GET_NEW_ID_WAIT;
            break;
    
        default: break;
    }
    
    return APP_FUNC_WORKING;
}

void APP_LOG_data ( APP_var_t* log_data )
{
    static APP_var_t* p_buffer;
    
    static uint8_t APP_LOG_STATE = APP_LOG_PTR_INIT;
    
    switch ( APP_LOG_STATE )
    {
        case APP_LOG_PTR_INIT:
            APP_logBufferHead = 0;
            p_buffer = &APP_logBuffer[APP_logBufferHead];
            APP_LOG_STATE = APP_LOG_PTR_OK;
            //intentional breakthrough
        case APP_LOG_PTR_OK:
            
//            *p_buffer = log_data;
            memcpy( p_buffer, log_data, sizeof( *log_data ) );
    
                if (p_buffer == &APP_logBuffer[APP_LOG_BUFFER_SIZE-1])
                {
                    p_buffer=&APP_logBuffer[0];
                    APP_logBufferHead = 0;
                }
                else
                {
                    p_buffer++;
                    APP_logBufferHead++;
                }
            
            break;
            
    }
        
}

void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad ) {
    
    static uint8_t APP_MANUAL_IRRIGATE = APP_MANUAL_IRRIGATE_INIT;
    
    switch ( APP_MANUAL_IRRIGATE )
    {  
    
        case APP_MANUAL_IRRIGATE_INIT:
            
            if(BTN_A_GetValue())
            {
        
                APP_MANUAL_IRRIGATE = APP_MANUAL_IRRIGATE_BTN_PRESSED;
            }  
            
        break;
            
        case APP_MANUAL_IRRIGATE_BTN_PRESSED:
           
            
            if ( ADC_humedad > APP_info.thresholds.manual )
            {
                APP_MANUAL_IRRIGATE = APP_MANUAL_IRRIGATE_LEDA_ON;
            }
            
            else
            {
                APP_MANUAL_IRRIGATE = APP_MANUAL_IRRIGATE_LEDA_OFF;
            }
                       
        break;
        
        case APP_MANUAL_IRRIGATE_LEDA_ON:
            
            LED_A_SetHigh();
            
            if ( ADC_humedad <= APP_info.thresholds.manual )
            {
                APP_MANUAL_IRRIGATE = APP_MANUAL_IRRIGATE_LEDA_OFF;
            }
            
        break;
        
        case APP_MANUAL_IRRIGATE_LEDA_OFF:
            
            LED_A_SetLow();
            APP_MANUAL_IRRIGATE = APP_MANUAL_IRRIGATE_INIT;
            
        break;
                   
    }
 
}

void APP_MDM_tasks()
{
    static uint8_t state = APP_STATE_INIT;
    uint8_t* ret;
    
    
    switch( state )
    {
        case APP_STATE_INIT:
            if( MDM_Init() )
            {
                if( MDM_sendInitialAT() )
                {
                    state = APP_STATE_GSM_SMS_INIT;
                }
            }
            break;

        case APP_STATE_GSM_SMS_INIT:
            if( MDM_GSM_init() == MDM_AT_RESP_NAME_OK )
            {
                state = APP_STATE_GPS_GET;
//                state = APP_STATE_GSM_SMS_GET;
            }
            break;

        case APP_STATE_GPS_GET:
            switch( MDM_GNSS_getInf( MDM_GNS_NMEA_RMC, true ) )
            {
                case MDM_AT_RESP_NAME_GNS_GET_INF:
                    state = APP_STATE_PARSE_FRAME;
                    break;

                case MDM_AT_RESP_NAME_ERROR:
                    state = APP_STATE_WAIT;
                    break;

                    case MDM_AT_RESP_NAME_WORKING:
                    break;

                default:
                    break;
            }
            break;    

        case APP_STATE_PARSE_FRAME:


            break;
            
        case APP_STATE_GSM_SMS_GET:
            if( BTN_isButtonPressed( BTN_BUTTON_A ) )
            {
                ret = USB_read(0);
                if( ret[0] != 0 )
                {
                    state = APP_STATE_GSM_SMS_SEND;
                }
            }
            break;

        case APP_STATE_GSM_SMS_SEND:
            if( MDM_sendSMS( NUMERO_VICKY, ret ) )
            {
                state = APP_STATE_GSM_SMS_GET;
            }
            break;    
            
        default: break;
    
    }
}

void APP_RGB_tasks()
{
    switch( RGB_displayType )
    {
        case RGB_DISPLAY_TYPE_UNDEF:
        case RGB_DISPLAY_TYPE_ALL:
        case RGB_DISPLAY_TYPE_1_BY_1:
            break;
            
        case RGB_DISPLAY_TYPE_GO_ROUND:
            RGB_goRound( RGB_goRoundConfig );
            break;
            
        default: break;
    }
    RGB_tasks();
}


uint32_t APP_LOG_BUFFER_HEAD_GetValue ( void )
{
    return APP_logBufferHead; //APP_logBufferHead da un entero que indica el indice dentro del buffer, no da la direccion de memoria del ultimo registro sino la posicion dentro del buffer
}

//void APP_LOG_Buffer_displayUSB ( uint32_t APP_logBufferHead )
//{
//    uint32_t index_aux;
//    for (index_aux = 0; index_aux <= APP_LOG_BUFFER_SIZE-1; index_aux++)
//    
//    {   
//        //Definir USB_dummyBuffer e incluir USB framework
//        sprintf(USB_dummyBuffer,"La humedad del %d° registro es: %d \n",1+index_aux,(APP_logBufferHead*sizeof()+APP_logBuffer[0]).humidity),;
//        USB_write(USB_dummyBuffer);
//        sprintf(USB_dummyBuffer,"Latitud: %f \n",APP_logBuffer[0].position.latitude);
//        USB_write(USB_dummyBuffer);
//    }
//}
void APP_pot2RGBEnable( bool p_enable )
{
    APP_convertPot2RGB = p_enable;
}

bool APP_pot2RGBIsEnabled()
{
    return APP_convertPot2RGB;
}

bool APP_getHumidity()
{
    uint16_t datos_potenciometro;
    
    if(POT_Convert( &datos_potenciometro ) )
    {
        APP_info.humidity  = POT_Linearized ( datos_potenciometro ); 
        return true;
    }
    return false;
}

void APP_pot2RGB( uint8_t p_humidity )
{
    APP_RGB_humidity ( p_humidity );
    APP_LEDA_irrigate ( p_humidity );
    APP_BTNA_manual_irrigate ( p_humidity );
}

APP_FUNC_STATUS_t APP_setThresholds( void )
{
    static uint8_t state_thresholds = APP_SET_THRESHOLDS_INIT;
    static int8_t retMenu_thresholds; 
    
    static int8_t aux_threshold_saturated;
    static int8_t aux_threshold_slightly_saturated;
    static int8_t aux_threshold_dry;
    static int8_t aux_threshold_slightly_dry;
    static int8_t aux_threshold_automatic_low;
    static int8_t aux_threshold_automatic_high;    
    static int8_t aux_threshold_manual;        
    
    int8_t aux_user_threshold;
    
    switch( state_thresholds )
    {
        case APP_SET_THRESHOLDS_INIT:
            
            aux_threshold_saturated=APP_info.thresholds.saturated;
            aux_threshold_slightly_saturated=APP_info.thresholds.slightly_saturated;
            aux_threshold_dry=APP_info.thresholds.dry;
            aux_threshold_slightly_dry=APP_info.thresholds.slightly_dry;
            aux_threshold_automatic_low=APP_info.thresholds.low_automatic;
            aux_threshold_automatic_high=APP_info.thresholds.high_automatic;    
            aux_threshold_manual=APP_info.thresholds.manual; 
        
            state_thresholds = APP_SET_THRESHOLDS_MENU;
            
            break;    
    
        case APP_SET_THRESHOLDS_MENU:
            
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Configuración de umbrales. ¿Qué umbral desea modificar?" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Umbral de saturación" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Umbral de riesgo de saturación" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Umbral de riesgo de sequedad" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Umbral de sequedad" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Punto óptimo para riego automático" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Umbral de sequedad para riego automático" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Punto óptimo para riego manual" );
            
            // configurar parametros 
            state_thresholds = APP_SET_THRESHOLDS_HEADERS;
            break;
            
        case APP_SET_THRESHOLDS_HEADERS:
            
            USB_write("\n Los valores de los umbrales actuales son: \n");
//            sprintf( USB_dummyBuffer, "\n    Umbral de saturación: %d \n",aux_threshold_saturated);
//            USB_write( USB_dummyBuffer );
//            sprintf( USB_dummyBuffer, "\n    Umbral de riesgo de saturación: %d \n",aux_threshold_slightly_saturated);
//            USB_write( USB_dummyBuffer );
//            sprintf( USB_dummyBuffer, "\n    Umbral de riesgo de sequedad: %d \n",aux_threshold_slightly_dry);
//            USB_write( USB_dummyBuffer );
//            sprintf( USB_dummyBuffer, "\n    Umbral de sequedad: %02d \n",aux_threshold_dry);
//            USB_write( USB_dummyBuffer );
//            sprintf( USB_dummyBuffer, "\n    Punto óptimo para riego automático: %d \n",aux_threshold_automatic_high);
//            USB_write( USB_dummyBuffer );
//            sprintf( USB_dummyBuffer, "\n    Umbral de sequedad para riego automático: %d \n",aux_threshold_automatic_low);
//            USB_write( USB_dummyBuffer );
//            sprintf( USB_dummyBuffer, "\n    Punto óptimo para riego manual: %d \n",aux_threshold_manual);
//            USB_write( USB_dummyBuffer );
            
            
            state_thresholds = APP_SET_THRESHOLDS_SHOW;
            //intentional breakthrough
            

            case APP_SET_THRESHOLDS_SHOW:
            retMenu_thresholds = USB_showMenuAndGetAnswer( UTS_MENU_HANDLER_MENU_THRESHOLDS );
            state_thresholds = APP_SET_THRESHOLDS_FUNCTIONS;
            //intentional breakthrough
            
            
            case APP_SET_THRESHOLDS_FUNCTIONS:
            switch( retMenu_thresholds )
            {
                case 0: //working
                    state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    break;
                    
                case -1: //return                
                           
                if ( ( aux_threshold_saturated < aux_threshold_slightly_saturated  ) && ( aux_threshold_slightly_saturated < aux_threshold_slightly_dry ) && ( aux_threshold_slightly_dry < aux_threshold_dry ) && ( aux_threshold_automatic_low < aux_threshold_automatic_high ) )
                {
                    APP_info.thresholds.saturated=aux_threshold_saturated;
                    APP_info.thresholds.slightly_saturated=aux_threshold_slightly_saturated;
                    APP_info.thresholds.dry=aux_threshold_dry;
                    APP_info.thresholds.slightly_dry=aux_threshold_slightly_dry;
                    APP_info.thresholds.low_automatic=aux_threshold_automatic_low;
                    APP_info.thresholds.high_automatic=aux_threshold_automatic_high;
                    APP_info.thresholds.manual=aux_threshold_manual; 
                    
                    state_thresholds = APP_SET_THRESHOLDS_INIT;   
                    return APP_FUNC_RETURN;
                }  
                
                else
                {
                    USB_write("\n\n ERROR \n");
                    USB_write("\n\n Valores inconsistentes. Reingresar umbrales. \n");
                    state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                }
                           
                break;
                   
                case 1: //SETEAR UMBRAL 1
                    if( APP_getNewThreshold( &aux_user_threshold ) )
                    {              
                        aux_threshold_saturated=aux_user_threshold;
                        state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                    }
                                       
                    if ( APP_getNewThreshold( &aux_user_threshold )==APP_FUNC_RETURN )
                    {
                        state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    }
                    
                    break;
                    
                case 2: //SETEAR UMBRAL 2
                    if( APP_getNewThreshold( &aux_user_threshold ) )
                    {              
                        aux_threshold_slightly_saturated=aux_user_threshold;
                        state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                    }
                                       
                    if ( APP_getNewThreshold( &aux_user_threshold )==APP_FUNC_RETURN )
                    {
                        state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    }
                   
                    break;
                
                case 3: //SETEAR UMBRAL 3
                    if( APP_getNewThreshold( &aux_user_threshold ) )
                    {              
                        aux_threshold_dry=aux_user_threshold;
                        state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                    }
                                       
                    if ( APP_getNewThreshold( &aux_user_threshold )==APP_FUNC_RETURN )
                    {
                        state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    }
                    
                    break;
                    
                case 4: //SETEAR UMBRAL 4
                    if( APP_getNewThreshold( &aux_user_threshold ) )
                    {              
                        aux_threshold_slightly_dry=aux_user_threshold;
                        state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                    }
                                       
                    if ( APP_getNewThreshold( &aux_user_threshold )==APP_FUNC_RETURN )
                    {
                        state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    }
                    
                    break;
                    
                case 5: //SETEAR UMBRAL 5
                    if( APP_getNewThreshold( &aux_user_threshold ) )
                    {              
                        aux_threshold_automatic_low=aux_user_threshold;
                        state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                    }
                                       
                    if ( APP_getNewThreshold( &aux_user_threshold )==APP_FUNC_RETURN )
                    {
                        state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    }
                    
                    break;
                
                case 6: //SETEAR UMBRAL 6
                    if( APP_getNewThreshold( &aux_user_threshold ) )
                    {              
                        aux_threshold_automatic_high=aux_user_threshold;
                        state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                    }
                                       
                    if ( APP_getNewThreshold( &aux_user_threshold )==APP_FUNC_RETURN )
                    {
                        state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    }
                    
                    break;    
                
                case 7: //SETEAR UMBRAL 7
                    if( APP_getNewThreshold( &aux_user_threshold ) )
                    {              
                        aux_threshold_manual=aux_user_threshold;
                        state_thresholds = APP_SET_THRESHOLDS_HEADERS;
                    }
                                       
                    if ( APP_getNewThreshold( &aux_user_threshold )==APP_FUNC_RETURN )
                    {
                        state_thresholds = APP_SET_THRESHOLDS_SHOW;
                    }
                    
                    break;                    
                                      
                default: break;
            
            }
            break;
            
        default: break;
    
    }


}
            
APP_FUNC_STATUS_t APP_getNewThreshold( int8_t *p_users_new_threshold )
{
    static uint8_t state = APP_SET_NEW_THRESHOLD_SHOW;
    static int8_t aux; 
    
    switch( state )
    {
        case APP_SET_NEW_THRESHOLD_SHOW:
            
            
            USB_write("\n\n Ingrese el nuevo valor para el umbral \n");
            sprintf( USB_dummyBuffer,"         - entre %d y %d cB -  \n",APP_HUMIDITY_MIN_NUM,APP_HUMIDITY_MAX_NUM);
            USB_write(USB_dummyBuffer);
            
            
            state = APP_SET_NEW_THRESHOLD_WAIT;
            //intentional breakthrough
            
        case APP_SET_NEW_THRESHOLD_WAIT:
            if( USB_sth2Read() )
            {
                state = APP_SET_NEW_THRESHOLD_VALIDATE;
            }
            else
            {
                break;
            }
            //intentional breakthrough
            
        case APP_SET_NEW_THRESHOLD_VALIDATE:
            if( strstr( USB_whatsInReadBuffer(),USB_FWK_RETURN_CHAR ) != NULL )
            {                          
                state = APP_SET_NEW_THRESHOLD_SHOW;
                return APP_FUNC_RETURN;
            }
            
            aux = (int8_t) atoi( USB_whatsInReadBuffer() );
            if( aux >= APP_HUMIDITY_MIN_NUM && aux <= APP_HUMIDITY_MAX_NUM )
            {
                state = APP_SET_NEW_THRESHOLD_RESPONSE_OK;
            }
            else
            {
                state = APP_SET_NEW_THRESHOLD_RESPONSE_ERROR;
            }
            break;
            
        case APP_SET_NEW_THRESHOLD_RESPONSE_OK:
            USB_write("\n\n El valor ha sido ingresado \n");
            *p_users_new_threshold= aux;
            state = APP_SET_NEW_THRESHOLD_SHOW;
            return APP_FUNC_DONE;
            
        case APP_SET_NEW_THRESHOLD_RESPONSE_ERROR:
            USB_write("\n\n ERROR \n");
            sprintf( USB_dummyBuffer, "\n\n Ingrese un número entre %d y %d \n",APP_HUMIDITY_MIN_NUM,APP_HUMIDITY_MAX_NUM);
            USB_write(USB_dummyBuffer);
            state = APP_SET_NEW_THRESHOLD_WAIT;
            break;
    
        default: break;
    }

}


bool APP_init()  //inicializacion de cosas propias de nuestra aplicacion 
{
    static uint8_t APP_INIT_STATE = APP_INIT_VARS;
    
    switch( APP_INIT_STATE )
    {
        case APP_INIT_VARS:
            APP_pot2RGBEnable( false );
            APP_info.humidity = 0;
            APP_info.plantID = 1234;
            APP_THRESHOLD_initialize();
            APP_INIT_STATE = APP_INIT_MDM;
            //intentional breakthrough
            
        case APP_INIT_MDM:
            if( MDM_Init() )
            {
                if( MDM_sendInitialAT() )
                {
                    APP_INIT_STATE = APP_INIT_VARS;
                    return true;
                }
            }
            break;
    }
    return false;    
}

APP_tasks()
{
    static uint8_t APP_TASK_STATE = APP_TASK_POT;
    
    switch( APP_TASK_STATE )
    {
        case APP_TASK_POT:
            if( APP_getHumidity() )
            {
                APP_TASK_STATE = APP_TASK_POT_2_RGB;
            }
            break;
            
        case APP_TASK_POT_2_RGB:
            if( APP_pot2RGBIsEnabled() )
            {
                APP_pot2RGB( APP_info.humidity );
            }
            APP_TASK_STATE = APP_TASK_POT;
            break;
            
            // obtener trama gps
            
            // armar un registro 
            
            // control ( SI ME VOY DE MAMBO ENVIAR UN SMS!! )
            
            // esperar un comando por sms
            
        default: break;
    }
}

void APP_UI() //interfaz de usuario
{
    static uint8_t UI_STATE = APP_UI_STATE_INIT;
    static int8_t retMenu;
    
    switch( UI_STATE )
    {
        case APP_UI_STATE_INIT:
            
            if( RGB_goRound( RGB_goRoundSet( WHITE, 50, 3, GO_ROUND_VARIANT_LOOP)  ) )
            {
                USB_write("Bienvenido!\n");
                USB_write("Presione una tecla \n");
                APP_info.time = mktime(&RTC_time);
                UI_STATE = APP_UI_STATE_WAIT_4_KEY;
            }
            break;
            
        case APP_UI_STATE_WAIT_4_KEY:
            if( *USB_read(0) != 0 )
            {
                APP_pot2RGBEnable( true );
                UI_STATE = APP_UI_STATE_MENU_CREATE;
            }
            break;
            
        case APP_UI_STATE_MENU_CREATE:
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Menu Principal. ¿Qué desea hacer?" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Setear ID de Planta" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configurar Umbrales" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configurar Teléfono" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro2" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro3" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro4" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro5" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro6" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro7" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro8" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro9" );
            // configurar parametros 
            UI_STATE = APP_UI_STATE_PRINT_HEADER;
            break;   
            
        case APP_UI_STATE_PRINT_HEADER:
            sprintf( USB_dummyBuffer, "\n    Nro ID: %04d \n",APP_info.plantID );
            USB_write( USB_dummyBuffer );
            UI_STATE = APP_UI_STATE_MENU_SHOW;
            //intentional breakthrough
            
        case APP_UI_STATE_MENU_SHOW:
            retMenu = USB_showMenuAndGetAnswer( UTS_MENU_HANDLER_MENU_PRINCIPAL );
            UI_STATE = APP_UI_STATE_MENU_FUNCTIONS;
            //intentional breakthrough
            
            
        case APP_UI_STATE_MENU_FUNCTIONS:
            switch( retMenu )
            {
                case 0: //working
                case -1: //return
                   UI_STATE = APP_UI_STATE_MENU_SHOW;
                   break;
                   
                case 1: //SETEAR ID DE PLANTA
                    if( APP_getNewPlantID() ) //Se utiliza un if porque se distingue entre working (0) y done
                    {
                        UI_STATE = APP_UI_STATE_PRINT_HEADER;
                    }
                    break;
                    
                case 2: // CONFIGURAR UMBRALES
                    if( APP_setThresholds() ) //Se utiliza un if porque se distingue entre working (0) y return
                    {
                        UI_STATE = APP_UI_STATE_PRINT_HEADER;
                    }
                    break;
                    
                case 3: //CONFIGURAR TELÉFONO
                    break;
                    
                case 4: //ACCESO AL REGISTRO
                    break;
                    
                
                    
                default: break;
            
            }
            break;
            
        default: break;
    
    }


}

