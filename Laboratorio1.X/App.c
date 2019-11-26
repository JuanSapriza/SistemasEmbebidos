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
    
    APP_LEDA_irrigate ( 4 );
        
}

void APP_LEDA_irrigate ( uint8_t ADC_humedad )
{
    static uint8_t APP_IRRIGATE = APP_IRRIGATE_OFF;
    
    switch ( APP_IRRIGATE )
    {
        case APP_IRRIGATE_OFF:        
            if( (ADC_humedad)>30 )
            {
                LED_A_SetHigh();
                APP_IRRIGATE = APP_IRRIGATE_ON;
            }
            break;
            
        case APP_IRRIGATE_ON:        
            if( (ADC_humedad)<15 )
            {
                LED_A_SetLow();
                APP_IRRIGATE = APP_IRRIGATE_OFF;
            }
            break;
            
    }    
    
}