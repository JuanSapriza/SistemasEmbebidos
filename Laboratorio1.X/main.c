//<editor-fold defaultstate="collapsed" desc="Versión">

#define COMMIT_VERSION USB_librerias

//#define LABORATORIO_1
//#define LABORATORIO_2
//#define LABORATORIO_2_1
//#define LABORATORIO_3
//#define LABORATORIO_3_2
//#define LABORATORIO_3_3
//#define LABORATORIO_3_4
#define LABORATORIO_3_5




//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Includes">


#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "App.h"
#include "framework/RGB_fwk.h"
#include "framework/USB_fwk.h"
#include "platform/Buttons.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h" //armar libreria de LEDS
#include "utils/Utils.h"
//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="Defines">

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Data">


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Functions">

static void MAIN_init()
{
    SYSTEM_Initialize();   
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
                USB_write("Bienvenido! \n");
                
                APP_info.time = &RTC_time;
                APP_info.state = APP_STATE_WAIT;
            }
            break; 
            
        case APP_STATE_WAIT:
            if( strstr( USB_read(0),"hola") != 0 )
            {
                RGB_setAll(RED);
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
