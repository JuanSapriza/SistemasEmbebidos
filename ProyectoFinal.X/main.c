
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "framework/USB_fwk.h"
#include "platform/Buttons.h"
#include "platform/Modem.h"
#include "utils/Utils.h"
#include "framework/RGB_fwk.h"

#include "App.h"

static void MAIN_init() 
{
    SYSTEM_Initialize();
    RGB_setAll(OFF);
    GPRS_PWR_SetDigitalInput();
    GPRS_RESET_SetHigh();
    ADC1_ChannelSelect( ADC1_POT );
}

int main ()
{
    uint8_t state_main = APP_STATE_APP_INIT;

    MAIN_init();
    while(1)
    {
        if( !BTN_switch( BTN_BUTTON_B ) ) //pausa el programa
        {
            switch( state_main )
            {
                case APP_STATE_APP_INIT:
                    if( APP_init() )
                    {
                        UTS_ledBlink( 500, 500 );
                        state_main = APP_STATE_TASKS;
                    }
                    break;
                case APP_STATE_TASKS:
                    APP_UI();
                    APP_tasks();
                    break;
                
                default: break;
            }
            MDM_tasks();
            RGB_tasks();    
            USB_CDC_tasks();
        }
    }
    return 0;
}



