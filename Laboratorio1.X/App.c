#include "App.h"
#include <string.h>
#include "platform/RGB.h"
#include "framework/RGB_fwk.h"
#include "platform/Potenciometro.h"
#include <stdint.h>
#include "mcc_generated_files/pin_manager.h"

void APP_RGB_humidity ( uint8_t ADC_humedad )
{
    
    if( ( (ADC_humedad>=0) && (ADC_humedad<=5) ) || ( (ADC_humedad>=41) && (ADC_humedad<=60) ) ) 
    {
        //RGB_goRound( RED, 2000, 1, GO_ROUND_VARIANT_LAST_ALL_ON );
        RGB_setAll( RED );
    }            
     
    if( ( (ADC_humedad>=6) && (ADC_humedad<=9) ) || ( (ADC_humedad>=21) && (ADC_humedad<=40) ) )
    {
        RGB_setAll( YELLOW );
    }
    
    if( (ADC_humedad>=10) && (ADC_humedad<=20) ) 
    {
        RGB_setAll( GREEN );
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
    
                if (p_buffer=&APP_logBuffer[APP_LOG_BUFFER_SIZE-1])
                {
                    p_buffer=&APP_logBuffer[0];
                }
    
                else
                {
                    p_buffer++;
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
            { //HACERLO POR INTERRUPCIONES?
        
                APP_MANUAL_IRRIGATE = APP_MANUAL_IRRIGATE_BTN_PRESSED;
            }
                 //DELAY??????????    
            
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