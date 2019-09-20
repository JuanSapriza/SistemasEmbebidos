//<editor-fold defaultstate="collapsed" desc="Versi�n">

#define COMMIT_VERSION Aca podemos cagar todo igual

//#define LABORATORIO_1
//#define LABORATORIO_2
#define LABORATORIO_2_1



//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Includes">


#include <stdbool.h>
#include <stdint.h>
#include "Configuration_Bits.h"
#include "utils/Utils.h"
#include "platform/HardwareProfile.h"
#include "platform/Buttons.h"
#include "platform/Leds.h"
#include "platform/Timer.h"
#include "platform/Clock.h"


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Defines">
#define ARRAY_SIZE      12


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Data">


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Functions">

static void MAIN_init()
{
    CLK_Initialize();     
    BTN_init();
    BTN_initInt();
    
    LEDA_SetDigitalOutput();
    LEDB_SetDigitalOutput();

    TMR_2_init();
    INT_init();
    
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
      
    LEDA_setLow();
    LEDB_setLow();
    
    while( 1 )
    {
        if( BTN_isButtonPressed( BTN_BUTTON_A ) )
        {
            btnAWasPressed = true;
        }
        
        if( btnAWasPressed && UTS_delayms( 2000, false )  ) 
        {
            LEDA_setHigh();
        }
        
    }
    return 0;
}
#endif
//</editor-fold>
