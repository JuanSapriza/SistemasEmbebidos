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

void APP_RGB_humidity ( uint8_t ADC_humedad )
{
    
    if( ( (ADC_humedad>=0) && (ADC_humedad<=5) )  )
    {
        RGB_setAll( RED );
    }            
     
    if( ( (ADC_humedad>=6) && (ADC_humedad<=9) )  ) 
    {
        RGB_setAll( YELLOW );
    }
    
    if( (ADC_humedad>=10) && (ADC_humedad<=20) ) 
    {
        RGB_setAll( GREEN );
    }
    
    if( ( (ADC_humedad>=21) && (ADC_humedad<=40) )) 
    {
        RGB_setAll( WATER_GREEN );
    }
    
    if( (ADC_humedad>=41) && (ADC_humedad<=60)  )
    {
        RGB_setAll( BLUE );
    }
        
}

void APP_LEDA_irrigate ( uint8_t ADC_humedad )
{
    static uint8_t APP_IRRIGATE = APP_IRRIGATE_OFF;
    
    static uint8_t APP_LEDA = APP_LEDA_OFF;
        
    
    switch ( APP_IRRIGATE )
    {
        case APP_IRRIGATE_OFF:        
            if( (ADC_humedad)>30 )
            {
                APP_LEDA=APP_LEDA_ON;
                APP_IRRIGATE = APP_IRRIGATE_ON;
            }
            break;
            
        case APP_IRRIGATE_ON:   
            if( (ADC_humedad)<15 )
            {
                APP_LEDA=APP_LEDA_OFF;
                APP_IRRIGATE = APP_IRRIGATE_OFF;
            }
            break;
            
    }    
    
    switch ( APP_LEDA )
    {
        case APP_LEDA_ON:
            
            LED_A_SetHigh();
        
        break;    
    
        case APP_LEDA_OFF:
            
            LED_A_SetLow();
        
        break;    
    
    }

    
}

void APP_LOG_data ( APP_var_t log_data )
{
    static APP_var_t *p_buffer;
    
    static uint8_t APP_LOG_STATE = APP_LOG_PTR_INIT;
    
    switch ( APP_LOG_STATE )
    {
        case APP_LOG_PTR_INIT:
            p_buffer = &APP_logBuffer[0];
            APP_LOG_STATE = APP_LOG_PTR_OK;
            
        case APP_LOG_PTR_OK:
            
            *p_buffer=log_data;
    
                if (p_buffer==&APP_logBuffer[APP_LOG_BUFFER_SIZE-1])
                {
                    p_buffer=&APP_logBuffer[0];
                }
    
                else
                {
                    p_buffer=p_buffer+1;
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
           
            
            if ( ADC_humedad > 15 )
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
            
            if ( ADC_humedad <= 15 )
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
//                UTS_ledBlink( 500, 500 );
                if( MDM_sendInitialAT() )
                {
//                    RGB_setLed( 7, WHITE);
//                    USB_write( MDM_whatsInReadBuffer() );
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
//                    RGB_setLed( 2, GREEN );
                    state = APP_STATE_PARSE_FRAME;
                    break;

                case MDM_AT_RESP_NAME_ERROR:
//                    RGB_setLed( 3, RED );
                    state = APP_STATE_WAIT;
                    break;

                    case MDM_AT_RESP_NAME_WORKING:
//                    RGB_setLed( 2, BLUE );
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