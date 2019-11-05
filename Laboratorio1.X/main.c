//<editor-fold defaultstate="collapsed" desc="Versión">

#define COMMIT_VERSION Lab4_GPS_commandsFuncionando

//#define LABORATORIO_1
//#define LABORATORIO_2
//#define LABORATORIO_2_1
//#define LABORATORIO_3
//#define LABORATORIO_3_2
//#define LABORATORIO_3_3
//#define LABORATORIO_3_4
//#define LABORATORIO_3_5
#define LABORATORIO_4



//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Includes">


#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "App.h"
#include "framework/RGB_fwk.h"
#include "framework/USB_fwk.h"
#include "framework/RTCC_fwk.h"
#include "platform/Buttons.h"
#include "platform/RGB.h"
#include "platform/GPS.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/usb/usb.h"
#include "mcc_generated_files/uart1.h"
#include "utils/Utils.h"
#include "platform/Modem.h"
//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="Defines">

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Data">


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Functions">

static void MAIN_init()
{
    SYSTEM_Initialize();
    RGB_setLed(0,OFF);
    RGB_setLed(1,OFF);
    RGB_setLed(2,OFF);
    RGB_setLed(3,OFF);
    RGB_setLed(4,OFF);
    RGB_setLed(5,OFF);
    RGB_setLed(6,OFF);
    RGB_setLed(7,OFF);
    GPRS_PWR_SetDigitalInput();
    GPRS_RESET_SetHigh();
    
//    RGB_tasks();
   
}


//<editor-fold defaultstate="collapsed" desc="Main App Lab 3_5">
#ifdef LABORATORIO_3_5
void MAIN_app()
{

    switch( APP_info.state )
    {
        case APP_STATE_INIT:
            if( RGB_goRound( WHITE, 50, 3, GO_ROUND_VARIANT_LOOP ) )
            {
                USB_write("Bienvenido!\n");
                USB_write("Presione una tecla \n");
                APP_info.time = &RTC_time;
                APP_info.state = APP_STATE_WAIT;
            }
            break; 
            
        case APP_STATE_WAIT:
//            if( RTC_getUserTime( APP_info.time ) )
//            {
//                sprintf(USB_dummyBuffer,">>%02d - %02d - %04d \n",APP_info.time->tm_mday,APP_info.time->tm_mon, APP_info.time->tm_year);
//                USB_write( USB_dummyBuffer );
//                APP_info.state = 3;
//            }
            if( *USB_read(0) != 0 )
            {
                APP_info.state = APP_STATE_MAIN_MENU_CREATE;
            }
            break;
        
        case APP_STATE_MAIN_MENU_CREATE:
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Menu Principal. ¿Qué LED prendo?" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Led 1 Rojo" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Led 2 Verde" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Led 3 Azul" );
            APP_info.state = APP_STATE_MAIN_MENU_SHOW;
            break;
            
        case APP_STATE_MAIN_MENU_SHOW:
            switch( USB_showMenuAndGetAnswer(UTS_MENU_HANDLER_MENU_PRINCIPAL ) )
            {
                case 1:
                    RGB_setAll(OFF);
                    RGB_setLed(1,RED);
                    break;
                    
                case 2: 
                    RGB_setAll(OFF);
                    RGB_setLed(2,GREEN);
                    break;
                    
                case 3:
                    RGB_setAll(OFF);
                    RGB_setLed(3,BLUE);
                    break;
                    
                case -1:
                    RGB_setAll(OFF);
                    break;
                    
                default: break;
            
            }
            break;
            
            
            
            
        default: break;
    
    
    }
    
    
    

}
#endif
//</editor-fold>



//</editor-fold>
 


//<editor-fold defaultstate="collapsed" desc="Laboratorio 1">

#ifdef LABORATORIO_1
int main ()
{
    LEDA_SetDigitalOutput();
    LEDB_SetDigitalOutput();

    LEDA_setHigh();
    LEDB_setLow();
    
    while( 1 )
    {
        LEDA_toggle();
        LEDB_toggle();
        UTS_delayms();
    }
    
    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 2">
#ifdef LABORATORIO_2
int main ()
{
    MAIN_init();
    
    bool btnAWasPressed = false;
    bool btnBWasPressed = false;
      
    LEDA_setLow();
    LEDB_setLow();
    
    while( 1 )
    {
        if( BTN_isButtonPressed( BTN_BUTTON_A ) && !btnAWasPressed )
        {
            btnAWasPressed = true;
        }
        else if( !BTN_isButtonPressed( BTN_BUTTON_A ) && btnAWasPressed )
        {
            btnAWasPressed = false;
            LEDA_toggle();
        }
             
        if( BTN_isButtonPressed( BTN_BUTTON_B ) && !btnBWasPressed )
        {
            btnBWasPressed = true;
        }
        else if( !BTN_isButtonPressed( BTN_BUTTON_B ) && btnBWasPressed )
        {
            btnBWasPressed = false;
            LEDB_toggle();
        }
        
        while( ! UTS_delayms( 100, false ) )
        {           
        }
        
    }
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 2_1">
#ifdef LABORATORIO_2_1
int main ()
{
    MAIN_init();
       
    bool btnAWasPressed = false;
    bool btnBWasPressed = false;
      
    LED_A_SetLow();
    LED_B_SetHigh();
    
    while( 1 )
    {
        if( BTN_isButtonPressed( BTN_BUTTON_A ) )
        {
            btnAWasPressed = true;
        }
        
        if( btnAWasPressed ) 
        {
            if( UTS_delayms( 2000, false ) )
            {
                LED_A_SetHigh();
            }
        }
        
    }
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3">
#ifdef LABORATORIO_3
int main ()
{
    MAIN_init();
    
    while( 1 )
    {
       MCC_USB_CDC_DemoTasks();
    }
    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_2">
#ifdef LABORATORIO_3_2
int main ()
{
    
    MAIN_init();
 
    while(1)
    {  
        //echo USB
       if( USB_CDC_tasks() )
       {
           USB_write( USB_read( 0 ) );
       }
       
    }    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_3">
#ifdef LABORATORIO_3_3
int main ()
{
    
    MAIN_init();
 
    memset( RGB_leds, 0, sizeof(RGB_leds));
//    RGB_leds[0] = YELLOW;
//    RGB_leds[1] = VIOLET;
//    RGB_leds[2] = RED;
//    RGB_leds[3] = WHITE;
//    RGB_leds[4] = GREEN;
//    RGB_leds[5] = BLUE;
//    RGB_leds[6] = RED;
//    RGB_leds[7] = CYAN;
    RGB_setLed(2,CYAN);
    while(1)
    {  
//        RGB_send( RGB_leds, RGB_LEDS_COUNT );
       
    }    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_4">
#ifdef LABORATORIO_3_4
    bool led_1 = false;
    bool led_2 = false;
int main ()
{
    UTS_DELAY_HANDLER_t LED_1_delay = UTS_DELAY_HANDLER_1;
    UTS_DELAY_HANDLER_t LED_2_delay = UTS_DELAY_HANDLER_2;
   
    MAIN_init();
    UTS_delayms( LED_1_delay , 0, true );
    UTS_delayms( LED_2_delay , 0, true );
 
    while(1)
    {  
        
        if( UTS_delayms( LED_1_delay, 1000, false ) )
        {
            if( led_1 )
            {
                RGB_setLed(2,WHITE);
                USB_write("hola \n");
                led_1 = false;
            }
            else
            {
                USB_write("chau \n");
                RGB_setLed(2,BLUE);
                led_1 = true;
            }
        }
        
        if( UTS_delayms( LED_2_delay, 1200, false ) )
        {
            if( led_2 )
            {
                RGB_setLed(7,VIOLET);
                USB_write("mundo \n");
                led_2 = false;
            }
            else
            {
                RGB_setLed(7,RED);
                USB_write("pajarito \n");
                led_2 = true;
            }    
        }
        
        USB_CDC_tasks();        
        RGB_tasks();

    }    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_5">
#ifdef LABORATORIO_3_5


int main ()
{
    MAIN_init();
    while(1)
    {
        MAIN_app();
        USB_CDC_tasks();
        RGB_tasks();
    }
    
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 4">
#ifdef LABORATORIO_4

int main ()
{
    uint8_t dummyBuffer[ 64 ];
    MAIN_init();
    APP_info.state = APP_STATE_INIT;
    
    while(1)
    {
        if( !BTN_switch( BTN_BUTTON_B ) )
        {
            switch( APP_info.state )
            {
                case APP_STATE_INIT:
                    if( MDM_Init() )
                    {
                        UTS_ledBlink( 500, 500 );
                        if( MDM_sendInitialAT() )
                        {
                            RGB_setLed( 3, GREEN );
                            APP_info.state = APP_STATE_CHECK;
                        }
                    }
                    break;

                case APP_STATE_GPS_GET:

                    break;


            }
            RGB_tasks();
            USB_CDC_tasks();
        }
    }
    return 0;
}
#endif
//</editor-fold>
