//<editor-fold defaultstate="collapsed" desc="Versión">

#define COMMIT_VERSION USB_librerias

//#define LABORATORIO_1
//#define LABORATORIO_2
//#define LABORATORIO_2_1
//#define LABORATORIO_3
#define LABORATORIO_3_2




//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Includes">


#include <stdbool.h>
#include <stdint.h>
#include "platform/Buttons.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/usb/usb.h"
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

