//<editor-fold defaultstate="collapsed" desc="Versión">

#define COMMIT_VERSION MCC_branch_3

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
#include "platform/Leds.h"
#include "mcc_generated_files/usb/usb.h"

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
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
    bool btnAWasPressed = false;
    bool btnBWasPressed = false;
    
    MAIN_init();
    
    LED_A_SetHigh();
    LED_B_SetHigh();
    
    while( 1 )
    {
        if( BTN_isButtonPressed( BTN_BUTTON_A ) && !btnAWasPressed )
        {
            btnAWasPressed = true;
        }
        else if( !BTN_isButtonPressed( BTN_BUTTON_A ) && btnAWasPressed )
        {
            btnAWasPressed = false;
            LED_A_Toggle();
        }
             
        if( BTN_isButtonPressed( BTN_BUTTON_B ) && !btnBWasPressed )
        {
            btnBWasPressed = true;
        }
        else if( !BTN_isButtonPressed( BTN_BUTTON_B ) && btnBWasPressed )
        {
            btnBWasPressed = false;
            LED_B_Toggle();
        }
        
        while( ! UTS_delayms( 100, false ) )
        {           
        }
        
    }
    
    return 0;
}
#endif
//</editor-fold>


static uint8_t readBuffer[64];
static uint8_t writeBuffer[64];

void MCC_USB_CDC_DemoTasks(void)
{
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    if( USBIsDeviceSuspended()== true )
    {
        return;
    }

    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        for(i=0; i<numBytesRead; i++)
        {
            switch(readBuffer[i])
            {
                /* echo line feeds and returns without modification. */
                case 0x0A:
                case 0x0D:
                    writeBuffer[i] = readBuffer[i];
                    break;

                /* all other characters get +1 (e.g. 'a' -> 'b') */
                default:
                    writeBuffer[i] = readBuffer[i] + 1;
                    break;
            }
        }

        if(numBytesRead > 0)
        {
            putUSBUSART(writeBuffer,numBytesRead);
        }
    }

    CDCTxService();
}
















//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_2">
#ifdef LABORATORIO_3_2
int main ()
{

    static uint8_t readBuffer[64];
    static uint8_t writeBuffer[64];
    uint8_t i;
    uint8_t numBytesRead;
    
    MAIN_init();
 
    while(1)
    {   
         if( USBGetDeviceState() < CONFIGURED_STATE )
        {
            break;
        }

        if( USBIsDeviceSuspended()== true )
        {
            break;
        }

        if( USBUSARTIsTxTrfReady() == true)
        {

            numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

            for(i=0; i<numBytesRead; i++)
            {
                switch(readBuffer[i])
                {
                    /* echo line feeds and returns without modification. */
                    case 0x0A:
                    case 0x0D:
                        writeBuffer[i] = readBuffer[i];
                        break;

                    /* all other characters get +1 (e.g. 'a' -> 'b') */
                    default:
                        writeBuffer[i] = readBuffer[i] + 1;
                        break;
                }
            }

            if(numBytesRead > 0)
            {
                putUSBUSART(writeBuffer,numBytesRead);
            }
        }

        CDCTxService();

    }    
    return 0;
}
#endif
//</editor-fold>


/*   
//    MCC_USB_CDC_DemoTasks();
    
    
    
    
//    
//    LED_A_SetLow();    
//    LED_B_SetLow();    
//    
    USB_DEVICE_STATE estado;
    bool estado2;
//    
    while(1)
        {
            USBDeviceTasks(); 
            
            
            
            
//            if( UTS_delayms(1000,false) )
//            {
//                putsUSBUSART("hola mundo");
//            }
            
            
            
            
            estado = USBGetDeviceState();
            estado2 = USBIsDeviceSuspended(); 
            
            switch(estado)
            {
                case ATTACHED_STATE:
//                    LED_A_SetLow();
//                    LED_B_SetLow();
                    break;
                
                case DEFAULT_STATE:
//                    LED_A_SetHigh();
//                    LED_B_SetLow();
                    break;
                    
                case ADR_PENDING_STATE:
//                    LED_A_SetHigh();
//                    LED_B_SetHigh();
                    break;
                    
                case CONFIGURED_STATE:
//                    LED_A_SetHigh();
//                    LED_B_SetHigh();
                    break;
                    
                default:
                    break;
            }
//            
//         if(( estado < CONFIGURED_STATE ) || (estado2 == true))
//            {
//                // no esta configurado aun o esta suspendido
//                LED_A_SetHigh();    
//                
//            }
//            else 
//            {
//                //esta configurado Y habilitado
//                LED_B_SetHigh();    
//            }
        }
    */