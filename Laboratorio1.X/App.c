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

#define UYT -3 //zona horaria de Uruguay


APP_var_t APP_info;

APP_log_t APP_logBuffer[APP_LOG_BUFFER_SIZE];
uint32_t APP_logBufferHead;
bool APP_convertPot2RGB;
uint8_t APP_stringBuffer[APP_SHORT_STRING_SIZE];

bool APP_pot2RGBIsEnabled();
void APP_pot2RGBEnable( bool p_enable );
void APP_changePlantID( uint16_t p_newID );
APP_FUNC_STATUS_t APP_getNewPlantID();
bool APP_checkHumidityAlert();
void APP_clearHumidityAlert();
APP_FUNC_STATUS_t APP_setThresholds();
uint8_t* APP_threshold2String(APP_THRESHOLD_NAMES_t p_threshold );
APP_FUNC_STATUS_t APP_getNewThreshold( APP_THRESHOLD_NAMES_t p_threshold, int8_t *p_users_new_threshold );
APP_FUNC_STATUS_t APP_setNewPhone();
void APP_print_Buffer_Register ( uint8_t index_aux );
APP_FUNC_STATUS_t APP_LOG_Buffer_displayUSB ();
APP_FUNC_STATUS_t APP_celularConfig();


// THRESHOLDS Y PARAMETROS -------------------------------------------------------

void APP_THRESHOLD_initialize()
{
    APP_info.threshold.saturated=APP_THRESHOLD_SATURATED_DEFAULT;
    APP_info.threshold.slightly_saturated=APP_THRESHOLD_SLIGHTLY_SATURATED_DEFAULT;
    APP_info.threshold.slightly_dry=APP_THRESHOLD_SLIGHTLY_DRY_DEFAULT;
    APP_info.threshold.dry=APP_THRESHOLD_DRY_DEFAULT;
    APP_info.threshold.low_automatic=APP_THRESHOLD_LOW_AUTOMATIC_DEFAULT;
    APP_info.threshold.high_automatic=APP_THRESHOLD_HIGH_AUTOMATIC_DEFAULT;
    APP_info.threshold.manual=APP_THRESHOLD_MANUAL_DEFAULT;
}

void APP_THRESHOLD_set (APP_THRESHOLD_t p_threshold  )
{
    APP_info.threshold.saturated=p_threshold.saturated;
    APP_info.threshold.slightly_saturated= p_threshold.slightly_saturated;
    APP_info.threshold.slightly_dry=p_threshold.slightly_dry;
    APP_info.threshold.dry=p_threshold.dry;
    APP_info.threshold.low_automatic=p_threshold.low_automatic;
    APP_info.threshold.high_automatic=p_threshold.high_automatic;
    APP_info.threshold.manual=p_threshold.manual;
}

void APP_PARAM_initialize()
{
    APP_info.param.humiditySensePeriod=APP_HUMIDITY_SENSE_PERIOD_DEFAULT;
    APP_info.param.logRegisterPeriod=APP_LOG_REGISTRER_PERIOD_DEFAULT;
    APP_info.param.gpsGetPeriod=APP_GPS_GET_PERIOD_DEFAULT;
    APP_info.param.SMSalertPeriod=APP_SMS_ALERT_PERIOD_DEFAULT;
    APP_info.param.SMSalertCoolDown=APP_SMS_ALERT_COOL_DOWN_DEFAULT;
    
}

void APP_PARAM_set ( APP_PARAMS_t p_param )
{
    APP_info.param.humiditySensePeriod=p_param.humiditySensePeriod;
    APP_info.param.logRegisterPeriod=p_param.logRegisterPeriod;
    APP_info.param.gpsGetPeriod=p_param.gpsGetPeriod;
    APP_info.param.SMSalertPeriod=p_param.SMSalertPeriod;
    APP_info.param.SMSalertCoolDown=p_param.SMSalertCoolDown;
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
    static APP_THRESHOLD_NAMES_t selectedThreshold; 
    
    int8_t aux_user_threshold;
    
    switch( state_thresholds )
    {
        case APP_SET_THRESHOLDS_INIT:
            
            aux_threshold_saturated=APP_info.threshold.saturated;
            aux_threshold_slightly_saturated=APP_info.threshold.slightly_saturated;
            aux_threshold_dry=APP_info.threshold.dry;
            aux_threshold_slightly_dry=APP_info.threshold.slightly_dry;
            aux_threshold_automatic_low=APP_info.threshold.low_automatic;
            aux_threshold_automatic_high=APP_info.threshold.high_automatic;    
            aux_threshold_manual=APP_info.threshold.manual; 
        
            state_thresholds = APP_SET_THRESHOLDS_MENU;
            
            //intentional breakthrough  
    
        case APP_SET_THRESHOLDS_MENU:
            memset(USB_dummyBuffer,0,sizeof(USB_dummyBuffer));
            
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, "Configuración de umbrales. ¿Qué umbral desea modificar?" );
            
            sprintf( USB_dummyBuffer, "Umbral de saturación: %d",aux_threshold_saturated);
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, USB_dummyBuffer );
            
            sprintf( USB_dummyBuffer, "Umbral de riesgo de saturación: %d",aux_threshold_slightly_saturated);
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, USB_dummyBuffer );
            
            sprintf( USB_dummyBuffer, "Umbral de riesgo de sequía: %d",aux_threshold_slightly_dry);
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, USB_dummyBuffer );
            
            sprintf( USB_dummyBuffer, "Umbral de sequía: %d",aux_threshold_dry);
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, USB_dummyBuffer );
            
            sprintf( USB_dummyBuffer, "Punto óptimo para riego automático: %d",aux_threshold_automatic_low);
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, USB_dummyBuffer );
            
            sprintf( USB_dummyBuffer, "Umbral de sequía para riego automático: %d",aux_threshold_automatic_high);
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, USB_dummyBuffer );
            
            sprintf( USB_dummyBuffer, "Punto óptimo para riego manual: %d",aux_threshold_manual);
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_THRESHOLDS, USB_dummyBuffer );
            
            // configurar parametros 
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
                    selectedThreshold = APP_THRESHOLD_UNDEF;
                    break;
                    
                case -1: //return                
                           
                    if ( ( aux_threshold_saturated < aux_threshold_slightly_saturated  ) && ( aux_threshold_slightly_saturated < aux_threshold_slightly_dry ) && ( aux_threshold_slightly_dry < aux_threshold_dry ) && ( aux_threshold_automatic_low < aux_threshold_automatic_high ) )
                    {
                        APP_info.threshold.saturated=aux_threshold_saturated;
                        APP_info.threshold.slightly_saturated=aux_threshold_slightly_saturated;
                        APP_info.threshold.dry=aux_threshold_dry;
                        APP_info.threshold.slightly_dry=aux_threshold_slightly_dry;
                        APP_info.threshold.low_automatic=aux_threshold_automatic_low;
                        APP_info.threshold.high_automatic=aux_threshold_automatic_high;
                        APP_info.threshold.manual=aux_threshold_manual; 

                        state_thresholds = APP_SET_THRESHOLDS_INIT;   
                        return APP_FUNC_RETURN;
                    }  

                    else
                    {
                        USB_write("\n\n ERROR \n");
                        USB_write("\n\n Valores inconsistentes. Reingresar umbrales. \n");
                        state_thresholds = APP_SET_THRESHOLDS_MENU;
                    }
                    selectedThreshold = APP_THRESHOLD_UNDEF;           
                    break;
                   
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    selectedThreshold = (APP_THRESHOLD_NAMES_t) retMenu_thresholds;
                    break;
                    
                default: break;
            }
            
            if( selectedThreshold != APP_THRESHOLD_UNDEF )
            {
                switch( APP_getNewThreshold( selectedThreshold, &aux_user_threshold ) )
                    {
                        case APP_FUNC_RETURN:
                            state_thresholds = APP_SET_THRESHOLDS_MENU;
                            break;
                            
                        case APP_FUNC_DONE:
                            switch( selectedThreshold )
                            {
                                case APP_THRESHOLD_SATURATED:
                                    aux_threshold_saturated = aux_user_threshold;
                                    break;
                                    
                                case APP_THRESHOLD_SLIGHTLY_SATURATED:
                                    aux_threshold_slightly_saturated = aux_user_threshold;
                                    break;
                                    
                                case APP_THRESHOLD_SLIGHTLY_DRY:
                                    aux_threshold_slightly_dry = aux_user_threshold;
                                    break;
                                    
                                case APP_THRESHOLD_DRY:
                                    aux_threshold_dry = aux_user_threshold;
                                    break;
                                    
                                case APP_THRESHOLD_AUTO_LOW:
                                    aux_threshold_automatic_low = aux_user_threshold;
                                    break;
                                    
                                case APP_THRESHOLD_AUTO_HIGH:
                                    aux_threshold_automatic_high = aux_user_threshold;
                                    break;
                                    
                                case APP_THRESHOLD_MANUAL:
                                    aux_threshold_manual = aux_user_threshold;
                                    break;
                                    
                                default: break;
                                    
                            }
                            selectedThreshold = APP_THRESHOLD_UNDEF;
                            state_thresholds = APP_SET_THRESHOLDS_MENU;
                            break;
                            
                        default: break;
                    }
            }
            break;
            
        default: break;
    
    }

    return APP_FUNC_WORKING;

}
     
uint8_t* APP_threshold2String(APP_THRESHOLD_NAMES_t p_threshold )
{
    switch( p_threshold )
    {
        case APP_THRESHOLD_SATURATED:
            strcpy( APP_stringBuffer, "saturación" );
            break;
    
        case APP_THRESHOLD_SLIGHTLY_SATURATED:
            strcpy( APP_stringBuffer, "riesgo de saturación" );
            break;
    
        case APP_THRESHOLD_SLIGHTLY_DRY:
            strcpy( APP_stringBuffer, "riesgo de sequía" );
            break;
    
        case APP_THRESHOLD_DRY:
            strcpy( APP_stringBuffer, "sequía" );
            break;
    
    
        case APP_THRESHOLD_AUTO_LOW:
            strcpy( APP_stringBuffer, "punto óptimo para riego automático" );
            break;
    
    
        case APP_THRESHOLD_AUTO_HIGH:
            strcpy( APP_stringBuffer, "sequía para riego automático" );
            break;
    
    
        case APP_THRESHOLD_MANUAL:
            strcpy( APP_stringBuffer, "riego manual" );
            break;
    
        default: return NULL;
    
    }
    return APP_stringBuffer;
}

APP_FUNC_STATUS_t APP_getNewThreshold( APP_THRESHOLD_NAMES_t p_threshold, int8_t *p_users_new_threshold )
{
    static uint8_t state = APP_SET_NEW_THRESHOLD_SHOW;
    static int8_t aux; 
    
    switch( state )
    {
        case APP_SET_NEW_THRESHOLD_SHOW:
            
            sprintf(USB_dummyBuffer, "\nIngrese el nuevo valor para el umbral de %s \n", APP_threshold2String( p_threshold ) );
            USB_write(USB_dummyBuffer);
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
    return APP_FUNC_WORKING;

}

// CONTROL DE HUMEDAD    -------------------------------------------------------

void APP_RGB_humidity ( uint8_t ADC_humedad )
{
    
    if( ( (ADC_humedad>=0) && (ADC_humedad<=APP_info.threshold.saturated) )  )
    {
        RGB_setAll( BLUE );
    }            
     
    if( ( (ADC_humedad>APP_info.threshold.saturated) && (ADC_humedad<=APP_info.threshold.slightly_saturated) )  ) 
    {
        RGB_setAll( WATER_GREEN );
    }
    
    if( (ADC_humedad>APP_info.threshold.slightly_saturated) && (ADC_humedad<=APP_info.threshold.slightly_dry) ) 
    {
        RGB_setAll( GREEN );
    }
    
    if( ( (ADC_humedad>APP_info.threshold.slightly_dry) && (ADC_humedad<=APP_info.threshold.dry) )) 
    {
        RGB_setAll( YELLOW );
    }
    
    if( (ADC_humedad>APP_info.threshold.dry) && (ADC_humedad<=60)  )
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
            if( (ADC_humedad)>APP_info.threshold.high_automatic )
            {
                APP_LEDA=APP_LEDA_ON;
                APP_IRRIGATE = APP_IRRIGATE_ON;
            }
            break;
            
        case APP_IRRIGATE_ON:   
            if( (ADC_humedad)<APP_info.threshold.low_automatic )
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
           
            
            if ( ADC_humedad > APP_info.threshold.manual )
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
            
            if ( ADC_humedad <= APP_info.threshold.manual )
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
        APP_info.humidity.level  = POT_Linearized ( datos_potenciometro ); 
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

bool APP_checkHumidityAlert()
{
    if( APP_info.humidity.level < APP_info.threshold.saturated || APP_info.humidity.level > APP_info.threshold.dry )
    {
        APP_info.humidity.alert = true;
    }
    return APP_info.humidity.alert;
}

void APP_clearHumidityAlert()
{
    APP_info.humidity.alert = false;
}
    
// ID DE LA PLANTA -------------------------------------------------------------

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

// LOGGEO DE INFO  -------------------------------------------------------------

uint32_t APP_LOG_BUFFER_HEAD_GetValue ( void )
{
    return APP_logBufferHead; //APP_logBufferHead da un entero que indica el indice dentro del buffer, no da la direccion de memoria del ultimo registro sino la posicion dentro del buffer
}


 void APP_print_Buffer_Register ( uint8_t index_aux )
{
        struct tm * time_to_display;
        
        sprintf(USB_dummyBuffer,"\nDatos del registro %d correspondiente a la planta %04d \n",APP_logBuffer[index_aux].logNum,APP_logBuffer[index_aux].plantID);
        USB_write(USB_dummyBuffer);
		time_to_display = localtime(&(APP_logBuffer[index_aux].time));
        sprintf(USB_dummyBuffer,"Fecha y hora: %02d/%02d/%04d %2d:%02d:%02d \n",time_to_display->tm_mday,time_to_display->tm_mon,time_to_display->tm_year,(time_to_display->tm_hour+UYT)%24,time_to_display->tm_min,time_to_display->tm_sec);
        USB_write(USB_dummyBuffer);
        sprintf(USB_dummyBuffer,"Humedad: %d \n",APP_logBuffer[index_aux].humidity);
        USB_write(USB_dummyBuffer);
        
        if ( APP_logBuffer[index_aux].position_validity == 0 )
        {
            USB_write("Posicion no disponible \n");
            USB_write("Ulitma posicion conocida: \n");            
        }
        
        sprintf(USB_dummyBuffer,"Latitud: %f \n",APP_logBuffer[index_aux].position.latitude);
        USB_write(USB_dummyBuffer);
        sprintf(USB_dummyBuffer,"Longitud: %f \n",APP_logBuffer[index_aux].position.longitude);
        USB_write(USB_dummyBuffer);

}

APP_FUNC_STATUS_t APP_LOG_Buffer_displayUSB ()
{
    static uint8_t index_buffer;
    static uint8_t state_buffer;
    
    switch ( state_buffer )
    {
        case STATE_BUFFER_INIT:
            if ( APP_LOG_BUFFER_HEAD_GetValue() == 0 )
            {
                index_buffer = APP_LOG_BUFFER_SIZE-1 ;  
            }
            else
            {
                index_buffer = APP_LOG_BUFFER_HEAD_GetValue()-1; //porque APP_LOG_BUFFER_HEAD_GetValue() es el siguente elemento a escribir en el log y APP_LOG_BUFFER_HEAD_GetValue()-1 es el último registro
            }
            state_buffer = STATE_BUFFER_PRINT;
        //intentional breakthrough
        
        case STATE_BUFFER_PRINT:
            if (APP_logBuffer[index_buffer].plantID != 0)
            {
                APP_print_Buffer_Register ( index_buffer );
                state_buffer = STATE_BUFFER_CHECK;
                return APP_FUNC_WORKING;
            }
            state_buffer = STATE_BUFFER_CHECK;
            break;
         
        case STATE_BUFFER_CHECK:
            if ( USB_isSth2Write() == 0 )
            {
                if ( index_buffer == 0 )
                {
                    index_buffer = APP_LOG_BUFFER_SIZE-1 ;
                }
                
                else
                {
                    index_buffer--;
                }
                
                if ( ( index_buffer == APP_LOG_BUFFER_HEAD_GetValue()-1 ) || (APP_LOG_BUFFER_HEAD_GetValue() == 0 && index_buffer == APP_LOG_BUFFER_SIZE-1 ) )
                {
                    state_buffer=STATE_BUFFER_INIT;
                    return APP_FUNC_DONE;
                }   
                
                state_buffer=STATE_BUFFER_PRINT;
            }
        break;
            
        default: break;
    }
    
 return APP_FUNC_WORKING;

}
    
void APP_LOG_data( APP_var_t* log_data )
{
    static APP_log_t* ptr_buffer;
    static uint8_t APP_LOG_STATE = APP_LOG_PTR_INIT;
    static double latitude_prev=0;
    static double longitude_prev=0;
    static uint32_t logCount = 0; 
    
    switch ( APP_LOG_STATE )
    {
        case APP_LOG_PTR_INIT:
            APP_logBufferHead = 0;
            memset(APP_logBuffer,0,sizeof(APP_logBuffer));
            ptr_buffer = &APP_logBuffer[APP_logBufferHead];
            APP_LOG_STATE = APP_LOG_PTR_OK;
            //intentional breakthrough
            
        case APP_LOG_PTR_OK:
            ptr_buffer->logNum = ++logCount;
            ptr_buffer->humidity = log_data->humidity.level;
            ptr_buffer->plantID = log_data->plantID;
            ptr_buffer->position.latitude = log_data->position.latitude;
            ptr_buffer->position.longitude = log_data->position.longitude;
            ptr_buffer->position_validity = log_data->position_validity;
            ptr_buffer->time = log_data->time;
            
            if ( log_data->position_validity ) 
            {
                latitude_prev = log_data->position.latitude;
                longitude_prev  = log_data->position.longitude;      
            }
            
            else 
            {
                if ( latitude_prev!=0 && longitude_prev!=0 ) 
                {
                    ptr_buffer->position.latitude = latitude_prev;
                    ptr_buffer->position.longitude = longitude_prev;
                }
                
            }
            
            // RAJAR A LA MIERDA
            sprintf(USB_dummyBuffer, "hum: %d - lat: %d \n  ", ptr_buffer->humidity, ptr_buffer->position.latitude);
            USB_write( USB_dummyBuffer );
            // ESTO 
            
            if (ptr_buffer == &APP_logBuffer[APP_LOG_BUFFER_SIZE-1])
            {
                ptr_buffer=&APP_logBuffer[0];
                APP_logBufferHead = 0;
            }
            
            else
            {
                ptr_buffer++;
                APP_logBufferHead++;
            }
            
            break;
    }     
}

// CELULAR         -------------------------------------------------------------

APP_FUNC_STATUS_t APP_setNewPhone()
{
    static uint8_t state = APP_STATE_INIT;
    uint8_t auxBuffer[ APP_PHONE_NUM_SIZE +2 ]; //por las comillas de los bordes
    
    switch( state )
    {
        case APP_STATE_INIT:
            
            sprintf(USB_dummyBuffer, "\nIngrese el nuevo número de emergencia (con código país )\n" );
            USB_write(USB_dummyBuffer);
            sprintf( USB_dummyBuffer,"         - o presione %s para regresar -  \n", USB_FWK_RETURN_CHAR);
            USB_write(USB_dummyBuffer);
            state = APP_STATE_WAIT;
            //intentional breakthrough
            
        case APP_STATE_WAIT:
            if( USB_sth2Read() )
            {
                state = APP_STATE_CHECK;
            }
            else
            {
                break;
            }
            //intentional breakthrough
            
        case APP_STATE_CHECK:
            if( strstr( USB_whatsInReadBuffer(),USB_FWK_RETURN_CHAR ) != NULL )
            {                          
                state = APP_STATE_INIT;
                return APP_FUNC_RETURN;
            }
            if( strlen( USB_whatsInReadBuffer() -1 ) == APP_PHONE_NUM_SIZE ) //por el /n
            {
                state = APP_STATE_CHECK_OK;
            }
            else
            {
                state = APP_STATE_CHECK_ERROR;
            }
            break;            
            
        case APP_STATE_CHECK_OK:
            USB_write("\n\n El valor ha sido ingresado \n");
            memset( APP_info.emergencyNum, 0, sizeof(APP_info.emergencyNum) );
            strcat(APP_info.emergencyNum,"\"");
            strncat(APP_info.emergencyNum,USB_whatsInReadBuffer(),strlen(USB_whatsInReadBuffer())-1);
            strcat(APP_info.emergencyNum,"\"");
            state = APP_STATE_INIT;
            return APP_FUNC_DONE;
            
        case APP_STATE_CHECK_ERROR:
            USB_write("\n ERROR \n");
            sprintf( USB_dummyBuffer, "\n\n Ingrese un número en formato +XXXYYYYYYYY \n");
            USB_write(USB_dummyBuffer);
            state = APP_STATE_WAIT;
            break;
    
        default: break;
    }
    return APP_FUNC_WORKING;
    

}

APP_FUNC_STATUS_t APP_celularConfig()
{
    static uint8_t state_celConfig = APP_STATE_INIT;
    static int8_t retMenu_celConfig;
    
    switch( state_celConfig )
    {
        case APP_STATE_INIT:
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_CEL_CONFIG, "Configuración Celular" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_CEL_CONFIG, "Configurar GSM" );
            sprintf(USB_dummyBuffer, "Nro de Emergencia: %s", APP_info.emergencyNum );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_CEL_CONFIG, USB_dummyBuffer );
            state_celConfig = APP_STATE_MENU_SHOW;
            //break; intentional breakthrough
    
        case APP_STATE_MENU_SHOW:
            retMenu_celConfig = USB_showMenuAndGetAnswer( UTS_MENU_HANDLER_MENU_CEL_CONFIG );
            state_celConfig = APP_STATE_MENU_OPTIONS;
            //break; intentional breakthrough
            
        case APP_STATE_MENU_OPTIONS:
            switch( retMenu_celConfig )
            {
                    
                case -1: //return 
                    return APP_FUNC_RETURN;
                    
                case 1:
                    //configurar GSM
                    break;
                    
                case 2: //setear nuevo numero
                    if( APP_setNewPhone() ) //solo distingue entre working y return/done
                    {
                        state_celConfig = APP_STATE_INIT;
                    }
                    break;
                    
                default: 
                case 0: //working
                    state_celConfig = APP_STATE_INIT;
                    break;
            }
            break;
    
    }

    return APP_FUNC_WORKING;
}


// FUNCIONES GRANDES------------------------------------------------------------

bool APP_init()  //inicializacion de cosas propias de nuestra aplicacion 
{
    static uint8_t APP_INIT_STATE = APP_INIT_VARS;
    
    switch( APP_INIT_STATE )
    {
        case APP_INIT_VARS:
            APP_pot2RGBEnable( false );
            APP_info.humidity.level = APP_HUMIDITY_DEFAULT_LEVEL;
            APP_info.plantID = APP_DEFAULT_PLANT_ID;
            APP_info.humidity.alert = false;
            APP_info.humidity.coolDown = false;
            strcpy( APP_info.emergencyNum, APP_EMERGENCY_NUMBER_DEFAULT );
            APP_THRESHOLD_initialize();
            APP_PARAM_initialize();
            APP_INIT_STATE = APP_INIT_MDM;
            //intentional breakthrough
            
        case APP_INIT_MDM:
            if( MDM_Init() )
            {
                if( MDM_sendInitialAT() )
                {
                    USB_write("+ Modem Configurado\n");
                    APP_INIT_STATE = APP_INIT_VARS;
                    return true;
                }
            }
            break;
            
        default: break;
    }
    return false;    
}

void APP_tasks()
{
    static uint8_t APP_smsBuffer[APP_SMS_LENGTH]; //larog maximo de un sms
    struct tm aux_tm;
    
    //ACTUALIZACION DE LA HORA
    if( UTS_delayms( UTS_DELAY_HANDLER_TIME_SYNC, 1000, false ) )
    {
        RTCC_TimeGet(&aux_tm);
        APP_info.time = mktime( &aux_tm );
    }
    
    
    // SENSADO DE HUMEDAD
    if( UTS_delayms( UTS_DELAY_HANDLER_HUMIDITY_SENSE, APP_info.param.humiditySensePeriod, false ) )
    {
        if( APP_getHumidity() )
        {
            if( APP_pot2RGBIsEnabled() )
            {
                APP_pot2RGB( APP_info.humidity.level );
            }
        }
    }
    
    
    // OBTENCION DE TRAMA GPS 
    switch( MDM_taskGetStatus( MDM_TASK_GET_GPS_FRAME ) )
    {
        case MDM_TASK_STATUS_UNDEF:
            if( UTS_delayms( UTS_DELAY_HANDLER_GPS_GET, APP_info.param.gpsGetPeriod, false ) )
            {
                MDM_taskSchedule( MDM_TASK_GET_GPS_FRAME, NULL );
            }
            break;

        case MDM_TASK_STATUS_DONE:
            GPS_parseFrame( MDM_whatsInReadBuffer(), &aux_tm, &APP_info.position, &APP_info.position_validity );
            MDM_taskSetStatus( MDM_TASK_GET_GPS_FRAME, MDM_TASK_STATUS_UNDEF );
            if(APP_info.position_validity)
            {
               RTCC_TimeSet(&aux_tm);
            }
            break;

        default: 
            //que hago?
            break;
    } 
    
    // ENVIO DE ALERTA POR SMS 
    if( !APP_info.humidity.coolDown && APP_checkHumidityAlert() )
    {
                                                                    //TENER ARMADO EL SMS!!
        switch( MDM_taskGetStatus( MDM_TASK_SEND_SMS ) )
        {
            case MDM_TASK_STATUS_UNDEF:
                MDM_taskSchedule( MDM_TASK_SEND_SMS, (uint8_t*) APP_smsBuffer );
                break;

            case MDM_TASK_STATUS_DONE:
                MDM_taskSetStatus( MDM_TASK_SEND_SMS, MDM_TASK_STATUS_UNDEF );
                APP_clearHumidityAlert();   
                APP_info.humidity.coolDown = true;
                break;

            default: 
                //que hago?
                break;
        } 
    }
    else if( APP_info.humidity.coolDown )
    {
        if( UTS_delayms( UTS_DELAY_HANDLER_HUMIDITY_COOLDOWN, APP_info.param.SMSalertCoolDown, false ) )
        {
            APP_info.humidity.coolDown = false;
        }
    }
     
    if( UTS_delayms( UTS_DELAY_HANDLER_REGISTRY_LOG, APP_info.param.logRegisterPeriod, false ) && APP_info.position_validity )
    {
        //GUARDAR EN EL BUFFER
        APP_LOG_data( &APP_info );
    }
            

        // esperar un comando por sms
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
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Setear ID de Planta" ); //1
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configurar Umbrales" ); //2
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configurar Teléfono" ); //3
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro" ); //4
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configuración Celular" ); //5
            // configurar parametros 
            UI_STATE = APP_UI_STATE_PRINT_HEADER;
            break;   
            
        case APP_UI_STATE_PRINT_HEADER:
            sprintf( USB_dummyBuffer, "\n    Nro ID: %04d ",APP_info.plantID );
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
                    if( APP_LOG_Buffer_displayUSB () ) //Se utiliza un if porque se distingue entre working (0) y return
                    {
                        UI_STATE = APP_UI_STATE_PRINT_HEADER;
                    }                    
                    break;
                    
                case 5: // configuraion celular
                    if( APP_celularConfig() ) 
                    {
                        UI_STATE = APP_UI_STATE_PRINT_HEADER;
                    }
                    break;
                    
                
                    
                default: break;
            
            }
            break;
            
        default: break;
    
    }


}

//#include "rtcc.h"
//void RTC_update_from_GPS ( void )
//{
//    
//}
