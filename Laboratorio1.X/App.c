
//<editor-fold defaultstate="collapsed" desc="Includes">

#include "App.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
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

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Variables">

APP_var_t APP_info;

//</editor-fold>

APP_log_t APP_logBuffer[APP_LOG_BUFFER_SIZE];
uint32_t APP_logBufferHead;
bool APP_convertPot2RGB;
uint8_t APP_stringBuffer[APP_SHORT_STRING_SIZE];

//<editor-fold defaultstate="collapsed" desc="Funciones">

uint8_t* APP_printDateTime( struct tm* p_time );

void APP_THRESHOLD_initialize( void );
void APP_PARAM_initialize( void );
APP_FUNC_STATUS_t APP_setThresholds( void );
APP_FUNC_STATUS_t APP_setParameters( void );
APP_FUNC_STATUS_t APP_getNewThreshold( APP_THRESHOLD_NAMES_t p_threshold, int8_t *p_users_new_threshold );
APP_FUNC_STATUS_t APP_getNewParameter( APP_PARAMETER_NAMES_t p_parameter, uint32_t *p_users_new_parameter );
uint8_t* APP_threshold2String(APP_THRESHOLD_NAMES_t p_threshold );
uint8_t* APP_parameter2String( APP_PARAMETER_NAMES_t p_parameter );
uint8_t* APP_typeDisplay2String( uint8_t p_parameter );

bool APP_getHumidity( void );
bool APP_checkHumidityAlert( void );
void APP_clearHumidityAlert( void );
APP_HUMIDITY_LEVEL_t APP_humidity2level( uint8_t humidity );
uint8_t* APP_humidityLevel2String( APP_HUMIDITY_LEVEL_t p_level );
void APP_RGB_humidity ( APP_HUMIDITY_LEVEL_t p_level );
void APP_RGB_humidityAnalog( uint8_t p_humidity );
void APP_pot2RGBEnable( bool p_enable );
bool APP_pot2RGBIsEnabled();
void APP_LEDA_irrigate ( uint8_t p_humidity );
void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad );

void APP_changePlantID( uint32_t p_newID );
APP_FUNC_STATUS_t APP_getNewPlantID( void );

uint32_t APP_LOG_BUFFER_HEAD_GetValue ( void );
void APP_LOG_data( APP_var_t* log_data );
void APP_print_Buffer_Register ( uint8_t index_aux );
APP_FUNC_STATUS_t APP_LOG_Buffer_displayUSB ();


APP_FUNC_STATUS_t APP_setNewPhone( void );
APP_FUNC_STATUS_t APP_GSMConfig( void );
APP_FUNC_STATUS_t APP_celularConfig( void );

MDM_smsInfo_t* APP_emergencySMS( void );
uint8_t* APP_location2GoogleMapsString( void );






//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Miscelánea">

uint8_t* APP_printDateTime( struct tm* p_time )
{
    static uint8_t dummyBuffer[20]; //largo de DD/MM/AAAA_hh:mm:ss
    memset( dummyBuffer,0, sizeof(dummyBuffer) );
    sprintf(dummyBuffer,"%02d/%02d/%04d %2d:%02d:%02d",p_time->tm_mday,p_time->tm_mon+1, p_time->tm_year+1900,p_time->tm_hour, p_time->tm_min, p_time->tm_sec );
    return dummyBuffer;
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Parámteros y Umbrales">

void APP_THRESHOLD_initialize( void )
{
    APP_info.threshold.saturated=APP_THRESHOLD_SATURATED_DEFAULT;
    APP_info.threshold.slightly_saturated=APP_THRESHOLD_SLIGHTLY_SATURATED_DEFAULT;
    APP_info.threshold.slightly_dry=APP_THRESHOLD_SLIGHTLY_DRY_DEFAULT;
    APP_info.threshold.dry=APP_THRESHOLD_DRY_DEFAULT;
    APP_info.threshold.low_automatic=APP_THRESHOLD_LOW_AUTOMATIC_DEFAULT;
    APP_info.threshold.high_automatic=APP_THRESHOLD_HIGH_AUTOMATIC_DEFAULT;
    APP_info.threshold.manual=APP_THRESHOLD_MANUAL_DEFAULT;
}

void APP_PARAM_initialize( void )
{
    APP_info.param.humiditySensePeriod=APP_HUMIDITY_SENSE_PERIOD_DEFAULT;
    APP_info.param.logRegisterPeriod=APP_LOG_REGISTRER_PERIOD_DEFAULT;
    APP_info.param.gpsGetPeriod=APP_GPS_GET_PERIOD_DEFAULT;
    APP_info.param.SMSalertCoolDown=APP_SMS_ALERT_COOL_DOWN_DEFAULT;
    APP_info.param.displayHumidity=APP_DISPLAY_HUMIDITY_DEFAULT;       
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
            
            state_thresholds = APP_SET_THRESHOLDS_SHOW;
            //intentional breakthrough
            
            case APP_SET_THRESHOLDS_SHOW_HEADER:
                USB_write("Los cambios no son efectivos hasta salir.");
                state_thresholds = APP_SET_THRESHOLDS_SHOW;
            //intentional breakthrough

            case APP_SET_THRESHOLDS_SHOW:
            retMenu_thresholds = UTS_showMenuAndGetAnswer( UTS_MENU_HANDLER_MENU_THRESHOLDS, true );
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
     
APP_FUNC_STATUS_t APP_setParameters( void )
{
    static uint8_t state_parameters = APP_SET_PARAMETERS_MENU;
    static int8_t retMenu_parameters; 
    static APP_PARAMETER_NAMES_t selectedParameter; 
    
    uint32_t aux_user_parameter;
    
    switch( state_parameters )
    {
        case APP_SET_PARAMETERS_MENU:
            memset(USB_dummyBuffer,0,sizeof(USB_dummyBuffer));
            
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, "Configuración de parámetros. ¿Qué parámetro desea modificar?" );
            
            if(APP_info.param.humiditySensePeriod>APP_PERIOD_IN_SECONDS_MAX)
            {
                sprintf( USB_dummyBuffer, "Período de sensado de humedad: %d min",APP_info.param.humiditySensePeriod/60000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );
            }
            else
            {
                sprintf( USB_dummyBuffer, "Período de sensado de humedad: %d.%d s",APP_info.param.humiditySensePeriod/1000,APP_info.param.humiditySensePeriod%1000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );
            }
            
            if(APP_info.param.logRegisterPeriod>APP_PERIOD_IN_SECONDS_MAX)
            {
                sprintf( USB_dummyBuffer, "Período de guardado de registros: %d min",APP_info.param.logRegisterPeriod/60000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );
            }
            else
            {
                sprintf( USB_dummyBuffer, "Período de guardado de registros: %d.%d s",APP_info.param.logRegisterPeriod/1000,APP_info.param.logRegisterPeriod%1000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );
            }

            if(APP_info.param.gpsGetPeriod>APP_PERIOD_IN_SECONDS_MAX)
            {
                sprintf( USB_dummyBuffer, "Período de obtención de trama de GPS: %d min",APP_info.param.gpsGetPeriod/60000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );
            }
            else
            {
                sprintf( USB_dummyBuffer, "Período de obtención de trama de GPS: %d.%d s",APP_info.param.gpsGetPeriod/1000,APP_info.param.gpsGetPeriod%1000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );  
            }
            
            sprintf( USB_dummyBuffer, "Tipo de display: %s",APP_typeDisplay2String( APP_info.param.displayHumidity ));
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );

            
            if(APP_info.param.SMSalertCoolDown>APP_PERIOD_IN_SECONDS_MAX)
            {
                sprintf( USB_dummyBuffer, "Período de alertas de SMS: %d min",APP_info.param.SMSalertCoolDown/60000,APP_info.param.SMSalertCoolDown%1000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );  
            }
            else
            {
                sprintf( USB_dummyBuffer, "Período de alertas de SMS: %d.%d s",APP_info.param.SMSalertCoolDown/1000,APP_info.param.SMSalertCoolDown%1000);
                UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PARAMETER, USB_dummyBuffer );
            }

            // configurar parametros 
            state_parameters = APP_SET_PARAMETERS_SHOW;
            //intentional breakthrough
            

        case APP_SET_PARAMETERS_SHOW:
            retMenu_parameters = UTS_showMenuAndGetAnswer( UTS_MENU_HANDLER_MENU_PARAMETER, true );
            state_parameters = APP_SET_PARAMETERS_FUNCTIONS;
            //intentional breakthrough
            
            
        case APP_SET_PARAMETERS_FUNCTIONS:
            switch( retMenu_parameters )
            {
                case 0: //working
                    state_parameters = APP_SET_PARAMETERS_SHOW;
                    selectedParameter = APP_PARAMETER_UNDEF;
                    break;
                    
                case -1: //return                  
                    state_parameters = APP_SET_PARAMETERS_MENU;
                    return APP_FUNC_RETURN;
                    break;
                    
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:

                    selectedParameter = (APP_PARAMETER_NAMES_t) retMenu_parameters;
                    break;
                    
                default: break;
            }
            
            if( selectedParameter != APP_PARAMETER_UNDEF )
            {
                switch( APP_getNewParameter( selectedParameter, &aux_user_parameter ) )
                    {
                        case APP_FUNC_RETURN:
                            state_parameters = APP_SET_PARAMETERS_MENU;
                            break;
                            
                        case APP_FUNC_DONE:
                            switch( selectedParameter )
                            {
                                case APP_PARAMETER_HUMIDITY_PERIOD:
                                    APP_info.param.humiditySensePeriod = aux_user_parameter;
                                    break;
                                    
                                case APP_PARAMETER_LOG_PERIOD:
                                    APP_info.param.logRegisterPeriod = aux_user_parameter;
                                    break;
                                    
                                case APP_PARAMETER_GPS_PERIOD:
                                    APP_info.param.gpsGetPeriod = aux_user_parameter;
                                    break;
                                    
                                case APP_PARAMETER_DISPLAY_HUMIDITY:
                                    APP_info.param.displayHumidity = aux_user_parameter;
                                    APP_pot2RGBEnable( APP_info.param.displayHumidity );
                                    break;
                                    
                                case APP_PARAMETER_SMS_COOL_DOWN:
                                    APP_info.param.SMSalertCoolDown = aux_user_parameter;
                                    break;
               
                                default: break;
                                    
                            }
                            selectedParameter = APP_PARAMETER_UNDEF;
                            state_parameters = APP_SET_PARAMETERS_MENU;
                            break;
                            
                        default: break;
                    }
            }
            break;
            
        default: break;
    
    }

    return APP_FUNC_WORKING;

}  

APP_FUNC_STATUS_t APP_getNewThreshold( APP_THRESHOLD_NAMES_t p_threshold, int8_t *p_users_new_threshold )
{
    static uint8_t state = APP_SET_NEW_THRESHOLD_SHOW;
    static int8_t aux;
    static int aux_buffer;
    
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
            if( strstr( USB_whatsInReadBuffer(),UTS_MENU_RETURN_CHAR ) != NULL )
            {                          
                state = APP_SET_NEW_THRESHOLD_SHOW;
                return APP_FUNC_RETURN;
            }
            
            aux_buffer = atoi( USB_whatsInReadBuffer() );
            aux = (int8_t) aux_buffer;
            if( aux_buffer >= APP_HUMIDITY_MIN_NUM && aux_buffer <= APP_HUMIDITY_MAX_NUM )
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

APP_FUNC_STATUS_t APP_getNewParameter( APP_PARAMETER_NAMES_t p_parameter, uint32_t *p_users_new_parameter )
{
    static uint8_t state_parameter = APP_SET_NEW_PARAMETER_SHOW;
    static uint32_t aux_parameter; 
    double aux_parameter_double;
    
    switch( state_parameter )
    {
        case APP_SET_NEW_PARAMETER_SHOW:
            
            if ( p_parameter == APP_PARAMETER_DISPLAY_HUMIDITY )
            {
                sprintf(USB_dummyBuffer, "\n Ingrese el nuevo valor para %s \n", APP_parameter2String( p_parameter ) );
                USB_write(USB_dummyBuffer);
            }
            else
            {
                sprintf(USB_dummyBuffer, "\n Ingrese el nuevo valor para %s en segundos \n", APP_parameter2String( p_parameter ) );
                USB_write(USB_dummyBuffer);                
            }
            state_parameter = APP_SET_NEW_PARAMETER_WAIT;
            //intentional breakthrough
            
        case APP_SET_NEW_PARAMETER_WAIT:
            if( USB_sth2Read() )
            {
                state_parameter = APP_SET_NEW_PARAMETER_VALIDATE;
            }
            else
            {
                break;
            }
            //intentional breakthrough
            
        case APP_SET_NEW_PARAMETER_VALIDATE:
            if( strstr( USB_whatsInReadBuffer(),UTS_MENU_RETURN_CHAR ) != NULL )
            {                          
                state_parameter = APP_SET_NEW_PARAMETER_SHOW;
                return APP_FUNC_RETURN;
            }
 
            aux_parameter_double = ( strtod( USB_whatsInReadBuffer() , NULL));
            
            if ( p_parameter == APP_PARAMETER_DISPLAY_HUMIDITY )
            {
                aux_parameter = (uint32_t) aux_parameter_double;
                if ( ( aux_parameter >= APP_DISPLAY_HUMIDITY_OFF ) && ( aux_parameter <= APP_DISPLAY_HUMIDITY_ANALOG ) )
                {
                    state_parameter = APP_SET_NEW_PARAMETER_RESPONSE_OK;
                }
                else
                {
                    state_parameter = APP_SET_NEW_PARAMETER_RESPONSE_ERROR;
                }
            }
                
            else
            {
                aux_parameter_double = aux_parameter_double*1000;
                aux_parameter = (uint32_t) aux_parameter_double;
                if ( ( aux_parameter > 0 ) && ( aux_parameter < APP_PERIOD_MAX *1000 ) )
                {
                    state_parameter = APP_SET_NEW_PARAMETER_RESPONSE_OK;
                }
                else
                {
                    state_parameter = APP_SET_NEW_PARAMETER_RESPONSE_ERROR;
                }
            }
            break;
            
        case APP_SET_NEW_PARAMETER_RESPONSE_OK:
            USB_write("\n\n El valor ha sido ingresado \n");
            *p_users_new_parameter= aux_parameter;
            state_parameter = APP_SET_NEW_PARAMETER_SHOW;
            return APP_FUNC_DONE;
            
        case APP_SET_NEW_PARAMETER_RESPONSE_ERROR:
            USB_write("\n\n ERROR \n");
            
            if ( p_parameter == APP_PARAMETER_DISPLAY_HUMIDITY )
            {
                USB_write("\n\n ERROR \n");
                sprintf( USB_dummyBuffer, "\n\n Ingrese un número entre %d y %d \n",APP_DISPLAY_HUMIDITY_OFF,APP_DISPLAY_HUMIDITY_ANALOG);
                USB_write(USB_dummyBuffer);
                state_parameter = APP_SET_NEW_PARAMETER_WAIT;
            }
            
            else
            {
                USB_write("\n\n ERROR \n");
                sprintf( USB_dummyBuffer, "\n\n Ingrese un número mayor a 0 s y menor a %d s (7 días) \n",APP_PERIOD_MAX);
                USB_write(USB_dummyBuffer);
                state_parameter = APP_SET_NEW_PARAMETER_WAIT;
                
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

uint8_t* APP_parameter2String( APP_PARAMETER_NAMES_t p_parameter )
{
    switch( p_parameter )
    {
        case APP_PARAMETER_HUMIDITY_PERIOD:
            strcpy( APP_stringBuffer, "el período de sensado de humedad" );
            break;
    
        case APP_PARAMETER_LOG_PERIOD:
            strcpy( APP_stringBuffer, "el período de guardado de registros" );
            break;
    
        case APP_PARAMETER_GPS_PERIOD:
            strcpy( APP_stringBuffer, "el período de obtención de trama de GPS" );
            break;
    
        case APP_PARAMETER_DISPLAY_HUMIDITY:
            strcpy( APP_stringBuffer, "el tipo de display de humedad (0-OFF 1-Discreto 2-Analógico)" );
            break;
    
    
        case APP_PARAMETER_SMS_COOL_DOWN:
            strcpy( APP_stringBuffer, "el período de alertas de SMS" );
            break;
   
    
        default: return NULL;
    
    }
    return APP_stringBuffer;
}

uint8_t* APP_typeDisplay2String( uint8_t p_parameter )
{
    switch( p_parameter )
    {
        case APP_DISPLAY_HUMIDITY_OFF:
            strcpy( APP_stringBuffer, "OFF" );
            break;
    
        case APP_DISPLAY_HUMIDITY_DISCRETE:
            strcpy( APP_stringBuffer, "discreto" );
            break;
    
        case APP_DISPLAY_HUMIDITY_ANALOG:
            strcpy( APP_stringBuffer, "analógico" );
            break;
    
        default: return NULL;
    
    }
    return APP_stringBuffer;
}
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Humedad">

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

APP_HUMIDITY_LEVEL_t APP_humidity2level( uint8_t p_humidity )
{
    if( ( (p_humidity>=0) && (p_humidity<=APP_info.threshold.saturated) )  )
    {
        return APP_HUMIDITY_SATURATED;
    }            
     
    if( ( (p_humidity>APP_info.threshold.saturated) && (p_humidity<=APP_info.threshold.slightly_saturated) )  ) 
    {
        return APP_HUMIDITY_SLIGHTLY_SATURATED;
    }
    
    if( (p_humidity>APP_info.threshold.slightly_saturated) && (p_humidity<=APP_info.threshold.slightly_dry) ) 
    {
        return APP_HUMIDITY_OPTIMAL;
    }
    
    if( ( (p_humidity>APP_info.threshold.slightly_dry) && (p_humidity<=APP_info.threshold.dry) )) 
    {
        return APP_HUMIDITY_SLIGHTLY_DRY;
    }
    
    if( (p_humidity>APP_info.threshold.dry) && (p_humidity<=60)  )
    {
        return APP_HUMIDITY_DRY;
    }
}

uint8_t* APP_humidityLevel2String( APP_HUMIDITY_LEVEL_t p_level )
{
    static uint8_t dummyBuffer[APP_HUMIDITY_LEVEL_DESCRIPTION_LENGTH];
    
    memset( dummyBuffer, 0, sizeof( dummyBuffer ) );
    switch( p_level )
    {
        case APP_HUMIDITY_SATURATED:
            strcpy(dummyBuffer,"saturado");
            break;
            
        case APP_HUMIDITY_SLIGHTLY_SATURATED:
            strcpy(dummyBuffer,"levemente saturado");
            break;
            
        case APP_HUMIDITY_OPTIMAL:
            strcpy(dummyBuffer,"óptimo");
            break;
            
        case APP_HUMIDITY_SLIGHTLY_DRY:
            strcpy(dummyBuffer,"levemente seco");
            break;
            
        case APP_HUMIDITY_DRY:
            strcpy(dummyBuffer,"muy seco");
            break;
            
        default: break;
    }
    return dummyBuffer;

}

void APP_RGB_humidity ( APP_HUMIDITY_LEVEL_t p_level )
{
    switch( p_level )
    {
        case APP_HUMIDITY_SATURATED:
            RGB_setAll( BLUE );
            break;
            
        case APP_HUMIDITY_SLIGHTLY_SATURATED:
            RGB_setAll( WATER_GREEN );
            break;
            
        case APP_HUMIDITY_OPTIMAL:
            RGB_setAll( GREEN );
            break;
            
        case APP_HUMIDITY_SLIGHTLY_DRY:
            RGB_setAll( YELLOW );
            break;
            
        case APP_HUMIDITY_DRY:
            RGB_setAll( RED );
            break;
            
        default: break;
    }
}

void APP_RGB_humidityAnalog( uint8_t p_humidity )
{
    RGB_color color;
    uint8_t R, G, B;
    
    if( p_humidity <= 20 )
    {
        color.r = 0;
        color.g = p_humidity;
        color.b = 25;
        RGB_setAll( color );
        return;
    }
    if( p_humidity <= 30 )
    {
        color.r = 0;
        color.g = 25;
        color.b = -2*p_humidity +60; 
        RGB_setAll( color );
        return;
    }
    if( p_humidity <= 40 )
    {
        color.r = 2*p_humidity - 60;
        color.g = 25;
        color.b = 0; 
        RGB_setAll( color );
        return;
    }
    color.r = 25;
    color.g = -p_humidity +60;
    color.b = 0; 
    RGB_setAll( color );
    return;
}

void APP_pot2RGBEnable( bool p_enable )
{
    APP_convertPot2RGB = p_enable;
    if ( !p_enable )
    {
        RGB_setAll(OFF);
    }    
}

bool APP_pot2RGBIsEnabled()
{
    return APP_convertPot2RGB;
}

void APP_LEDA_irrigate ( uint8_t p_humidity )
{
    static uint8_t APP_IRRIGATE = APP_IRRIGATE_OFF;
    static uint8_t APP_LEDA = APP_LEDA_OFF;

    switch ( APP_IRRIGATE )
    {
        case APP_IRRIGATE_OFF:        
            if( (p_humidity)>APP_info.threshold.high_automatic )
            {
                APP_LEDA=APP_LEDA_ON;
                APP_IRRIGATE = APP_IRRIGATE_ON;
            }
            break;
            
        case APP_IRRIGATE_ON:   
            if( (p_humidity)<APP_info.threshold.low_automatic )
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

void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad ) 
{
    
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

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="ID de la Planta">

void APP_changePlantID( uint32_t p_newID )
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
            if( strstr( USB_whatsInReadBuffer(),UTS_MENU_RETURN_CHAR ) != NULL )
            {
                state = APP_GET_NEW_ID_SHOW;
                return APP_FUNC_RETURN;
            }
            aux = (int32_t) atoi( USB_whatsInReadBuffer() );
            if( aux > 0 && aux <= APP_4_DIGITS_MAX_NUM )
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
            APP_changePlantID( (uint32_t) aux);
            state = APP_GET_NEW_ID_SHOW;
            return APP_FUNC_DONE;
            
        case APP_GET_NEW_ID_RESPONSE_ERROR:
            USB_write("\n\n ERROR \n");
            sprintf( USB_dummyBuffer, "\n\n ingrese un número entre 0 y %d \n",APP_4_DIGITS_MAX_NUM );
            USB_write(USB_dummyBuffer);
            state = APP_GET_NEW_ID_WAIT;
            break;
    
        default: break;
    }
    
    return APP_FUNC_WORKING;
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Loggeo">

uint32_t APP_LOG_BUFFER_HEAD_GetValue ( void )
{
    return APP_logBufferHead; //APP_logBufferHead da un entero que indica el indice dentro del buffer
}

void APP_LOG_data( APP_var_t* log_data )
{
    static APP_log_t* ptr_buffer;
    static uint8_t APP_LOG_STATE = APP_LOG_PTR_INIT;
    static bool firstTime_buffer = false;
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
                firstTime_buffer = true;
            }
            
            else 
            {
                if ( latitude_prev!=0 && longitude_prev!=0 ) 
                {
                    ptr_buffer->position.latitude = latitude_prev;
                    ptr_buffer->position.longitude = longitude_prev;
                }
                else 
                {
                    ptr_buffer->position.latitude = 0;
                    ptr_buffer->position.longitude = 0;
                }
                
            }
            
            ptr_buffer->firstTime = firstTime_buffer;
            
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

 void APP_print_Buffer_Register ( uint8_t index_aux )
{
        struct tm * time_to_display;
        
        sprintf(USB_dummyBuffer,"\n  | Datos del registro %d correspondiente a la planta %04d \n",APP_logBuffer[index_aux].logNum,APP_logBuffer[index_aux].plantID);
        USB_write(USB_dummyBuffer);
        
		time_to_display = localtime(&(APP_logBuffer[index_aux].time));
        
        
        if( APP_logBuffer[index_aux].firstTime == false )
        {
            USB_write("  | Fecha y hora: - \n");
            sprintf(USB_dummyBuffer,"  | Humedad: %dCb: %s \n",APP_logBuffer[index_aux].humidity, APP_humidityLevel2String( APP_humidity2level( APP_logBuffer[index_aux].humidity ) ));
            USB_write(USB_dummyBuffer);
            USB_write("  | Posición no disponible \n");
            USB_write("  | Latitud: - \n");
            USB_write("  | Longitud: - \n");
        }
        else
        {
            sprintf(USB_dummyBuffer,"  | Fecha y hora: %s \n",APP_printDateTime( time_to_display ));
            USB_write(USB_dummyBuffer);
            sprintf(USB_dummyBuffer,"  | Humedad: %dCb: %s \n",APP_logBuffer[index_aux].humidity, APP_humidityLevel2String( APP_humidity2level( APP_logBuffer[index_aux].humidity ) ));
            USB_write(USB_dummyBuffer);
         
            if ( APP_logBuffer[index_aux].position_validity == 0 )
            {
                USB_write("  | Posición no disponible \n");
                USB_write("  | Última posición conocida: \n");            
            }

            sprintf(USB_dummyBuffer,"  | Latitud: %f \n",APP_logBuffer[index_aux].position.latitude);
            USB_write(USB_dummyBuffer);
            sprintf(USB_dummyBuffer,"  | Longitud: %f \n",APP_logBuffer[index_aux].position.longitude);
            USB_write(USB_dummyBuffer);
        }
}

APP_FUNC_STATUS_t APP_LOG_Buffer_displayUSB()
{
    static uint8_t index_buffer = 0;
    static uint8_t state_buffer = STATE_BUFFER_INIT;
    
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
            if ( USB_sth2Write() == 0 )
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

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Celular">

APP_FUNC_STATUS_t APP_setNewPhone()
{
    static uint8_t state = APP_STATE_INIT;
    uint8_t auxBuffer[ MDM_SMS_PHONE_NUM_LENGTH +2 ]; //por las comillas de los bordes
    
    switch( state )
    {
        case APP_STATE_INIT:
            
            sprintf(USB_dummyBuffer, "\nIngrese el nuevo número de emergencia (con código país )\n" );
            USB_write(USB_dummyBuffer);
            sprintf( USB_dummyBuffer,"         - o presione %s para regresar -  \n", UTS_MENU_RETURN_CHAR);
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
            if( strstr( USB_whatsInReadBuffer(),UTS_MENU_RETURN_CHAR ) != NULL )
            {                          
                state = APP_STATE_INIT;
                return APP_FUNC_RETURN;
            }
            if( strlen( USB_whatsInReadBuffer() )-1 == MDM_SMS_PHONE_NUM_LENGTH ) //por el /n
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

APP_FUNC_STATUS_t APP_GSMConfig()
{
    static uint8_t state_gsmConfig = APP_STATE_INIT;
    static uint16_t tempPIN = 0; 
    
    switch( state_gsmConfig )
    {
        case APP_STATE_INIT:
            MDM_taskSchedule( MDM_TASK_CONFIG, NULL );
            state_gsmConfig = APP_STATE_TASKS;
            USB_write("\nAguarde por favor...\n");
            //intentional breakthrough
        
        case APP_STATE_TASKS:
            switch( MDM_GSM_init( tempPIN ) )
            {
                case MDM_AT_RESP_NAME_GSM_SIM_ERROR:
                    USB_write("\nRevise la SIM y vuelva a intentar\n");
                    state_gsmConfig = APP_STATE_INIT;
                    APP_info.GSM_active = false;
                    return APP_FUNC_ERROR;
                    break;
                    
                case MDM_AT_RESP_NAME_ERROR:
                    USB_write("\nError en PIN");
                    tempPIN = 0;
                    APP_info.GSM_active = false;
                    state_gsmConfig = APP_STATE_INIT;
                    break;

                case MDM_AT_RESP_NAME_GSM_SIM_PIN_NEEDED:
                    state_gsmConfig = APP_STATE_SHOW;
                    break;
                    
                case MDM_AT_RESP_NAME_OK:
                    tempPIN = 0;
                    APP_info.GSM_active = true;
                    state_gsmConfig = APP_STATE_INIT;
                    MDM_taskSetStatus( MDM_TASK_CONFIG, MDM_TASK_STATUS_DONE );
                    return APP_FUNC_DONE;
                    break;

                default: break;
            }
            break;
            
        case APP_STATE_SHOW:
            USB_write("\nIngrese el PIN: \n");
            state_gsmConfig = APP_STATE_GET;
            //intentional breakthrough
            
        case APP_STATE_GET:
            if( USB_sth2Read() )
            {
                state_gsmConfig = APP_STATE_CHECK;  
            }
            else
            {
                break;
            }
            //intentional breakthrough

        case APP_STATE_CHECK:
            if( strstr( USB_whatsInReadBuffer(), UTS_MENU_RETURN_CHAR ) != NULL )
            {
                tempPIN = 0;
                state_gsmConfig = APP_STATE_INIT;
                return APP_FUNC_RETURN;
            }
            tempPIN = (uint32_t) atoi( USB_whatsInReadBuffer() );
            if( tempPIN <= APP_4_DIGITS_MAX_NUM )
            {
                USB_write("\nAguarde por favor...");
                state_gsmConfig = APP_STATE_TASKS;
            }
            else
            {
                tempPIN = 0;
                USB_write("\nIngrese un PIN válido\n");
                state_gsmConfig = APP_STATE_GET;
            }
            break;
            
        case APP_STATE_COOLDOWN:
            if( UTS_delayms( UTS_DELAY_HANDLER_USB_SEND_TO_MODEM_ACHIQUEN, MDM_COMMAND_DEFAULT_TIMEOUT, false ) )
            {
                state_gsmConfig = APP_STATE_TASKS;
            }
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
            // intentional breakthrough
    
        case APP_STATE_MENU_SHOW:
            retMenu_celConfig = UTS_showMenuAndGetAnswer( UTS_MENU_HANDLER_MENU_CEL_CONFIG, true );
            state_celConfig = APP_STATE_MENU_OPTIONS;
            // intentional breakthrough
            
        case APP_STATE_MENU_OPTIONS:
            switch( retMenu_celConfig )
            {
                    
                case -1: //return 
                    state_celConfig = APP_STATE_INIT;
                    return APP_FUNC_RETURN;
                    
                case 1: //configurar GSM
                    if( APP_GSMConfig() )
                    {
                        state_celConfig = APP_STATE_INIT;
                    }
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

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="SMS de Emergencia">

MDM_smsInfo_t* APP_emergencySMS()
{
    static MDM_smsInfo_t smsInfo;
    struct tm * time_to_display;
    static bool RTCC_initialized = false;
    
    if ( APP_info.position_validity )
    {
        RTCC_initialized = true;
    }
    
    if ( RTCC_initialized )
    {
        time_to_display = localtime(&(APP_info.time));

        memset( &smsInfo, 0, sizeof( MDM_smsInfo_t ) );
        strcpy( smsInfo.num, APP_info.emergencyNum );
        sprintf( smsInfo.text, "%04d-suelo %s-%s-%s", APP_info.plantID, APP_humidityLevel2String( APP_humidity2level( APP_info.humidity.level ) ), APP_printDateTime( time_to_display ), APP_location2GoogleMapsString() );
        return &smsInfo;
    }
   
    else
    {
        
        memset( &smsInfo, 0, sizeof( MDM_smsInfo_t ) );
        strcpy( smsInfo.num, APP_info.emergencyNum );
        sprintf( smsInfo.text, "%04d-suelo %s-%s-%s", APP_info.plantID, APP_humidityLevel2String( APP_humidity2level( APP_info.humidity.level ) ), "Hora no disponible", APP_location2GoogleMapsString() );
        return &smsInfo;
    }
    
}

uint8_t* APP_location2GoogleMapsString()
{
    static uint8_t dummyBuffer[APP_GOOGLE_MAPS_LOCATION_URL_LENGTH];
    memset(dummyBuffer,0,sizeof(dummyBuffer));
    if( APP_info.position_validity )
    {
        sprintf(dummyBuffer,"https://www.google.com/maps/@%+02.6lf,%+02.6lf,17z \n",APP_info.position.latitude,APP_info.position.longitude);
    }
    else
    {
        sprintf( dummyBuffer,"Ubicación no disponible" );
    }
    return dummyBuffer;
}

//</editor-fold>

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
            APP_info.GSM_active = false;
            strcpy( APP_info.emergencyNum, APP_EMERGENCY_NUMBER_DEFAULT );
            memset(APP_logBuffer, 0, sizeof(APP_logBuffer));
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
    struct tm aux_tm;
    static bool sense = false;
    
    // ACTUALIZACION DE LA HORA
    if( UTS_delayms( UTS_DELAY_HANDLER_TIME_SYNC, 1000, false ) )
    {
      if (RTCC_TimeGet(&aux_tm) == true)
      {
          APP_info.time = mktime( &aux_tm );
          APP_info.time += UYT*3600;
      }
    }
    
    
    // SENSADO DE HUMEDAD
    if( !sense && UTS_delayms( UTS_DELAY_HANDLER_HUMIDITY_SENSE, APP_info.param.humiditySensePeriod, false ) )
    {
        sense = true;
    }
    if( sense )
    {
    if( APP_getHumidity() )
        {
            if( APP_pot2RGBIsEnabled() )
            {
                if( APP_info.param.displayHumidity == APP_DISPLAY_HUMIDITY_DISCRETE )
                {
                    APP_RGB_humidity ( APP_humidity2level( APP_info.humidity.level) );
                }
                else if( APP_info.param.displayHumidity == APP_DISPLAY_HUMIDITY_ANALOG )
                {
                    APP_RGB_humidityAnalog( APP_info.humidity.level );
                }
            }
            sense = false;
        }
    }
    APP_LEDA_irrigate ( APP_info.humidity.level );
    APP_BTNA_manual_irrigate ( APP_info.humidity.level );
    
    
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

        default: break;
    } 
    
    // ENVIO DE ALERTA POR SMS 
    if( !APP_info.humidity.coolDown && APP_checkHumidityAlert() )
    {
        if( APP_info.GSM_active )
        {
            switch( MDM_taskGetStatus( MDM_TASK_SEND_SMS ) )
            {
                case MDM_TASK_STATUS_UNDEF:
                    MDM_taskSchedule( MDM_TASK_SEND_SMS, (MDM_smsInfo_t*) APP_emergencySMS() );
                    break;

                case MDM_TASK_STATUS_DONE:
                    MDM_taskSetStatus( MDM_TASK_SEND_SMS, MDM_TASK_STATUS_UNDEF );
                    APP_clearHumidityAlert();   
                    APP_info.humidity.coolDown = true;
                    break;

                default: break;
            } 
        }
        else
        { 
            APP_clearHumidityAlert();
        }
    }
    else if( APP_info.humidity.coolDown )
    {
        if( UTS_delayms( UTS_DELAY_HANDLER_HUMIDITY_COOLDOWN, APP_info.param.SMSalertCoolDown, false ) )
        {
            APP_info.humidity.coolDown = false;
        }
    }
     
    if( UTS_delayms( UTS_DELAY_HANDLER_REGISTRY_LOG, APP_info.param.logRegisterPeriod, false ) )
    {//GUARDAR EN EL BUFFER
        APP_LOG_data( &APP_info );
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
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Menú Principal. ¿Qué desea hacer?" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Setear ID de Planta" ); //1
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configurar Umbrales" ); //2
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configurar Parámetros" ); //3
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Acceso al Registro" ); //4
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Configuración Celular" ); //5
            UI_STATE = APP_UI_STATE_PRINT_HEADER;
            break;   
            
        case APP_UI_STATE_PRINT_HEADER:
            sprintf( USB_dummyBuffer, "\n    Nro ID: %04d ",APP_info.plantID );
            USB_write( USB_dummyBuffer );
            UI_STATE = APP_UI_STATE_MENU_SHOW;
            //intentional breakthrough
            
        case APP_UI_STATE_MENU_SHOW:
            retMenu = UTS_showMenuAndGetAnswer( UTS_MENU_HANDLER_MENU_PRINCIPAL, false );
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
                    
                case 3: //CONFIGURAR TPARÁMETROS
                    if( APP_setParameters() ) //Se utiliza un if porque se distingue entre working (0) y return
                    {
                        UI_STATE = APP_UI_STATE_PRINT_HEADER;
                    }
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

