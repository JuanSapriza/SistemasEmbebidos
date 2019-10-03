//<editor-fold defaultstate="collapsed" desc="Versión">

#define COMMIT_VERSION MCC_branch_prueba_3

//#define LABORATORIO_1
//#define LABORATORIO_2
//#define LABORATORIO_2_1
#define LABORATORIO_3




//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Includes">


#include <stdbool.h>
#include <stdint.h>
#include "platform/Buttons.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/usb/usb.h"
//</editor-fold>


////<editor-fold defaultstate="collapsed" desc="Configuration bits">
//
//// PIC32MM0256GPM064 Configuration Bit Settings
//
//// 'C' source line config statements
//
//// FDEVOPT
//#pragma config SOSCHP = OFF             // Secondary Oscillator High Power Enable bit (SOSC oprerates in normal power mode.)
//#pragma config ALTI2C = OFF             // Alternate I2C1 Pins Location Enable bit (Primary I2C1 pins are used)
//#pragma config FUSBIDIO = OFF           // USBID pin control (USBID pin is controlled by the USB module)
//#pragma config FVBUSIO = ON             // VBUS Pin Control (VBUS pin is controlled by port function)
//#pragma config USERID = 0xFFFF          // User ID bits (Enter Hexadecimal value)
//
//// FICD
//#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
//#pragma config ICS = PGx1               // ICE/ICD Communication Channel Selection bits (Communicate on PGEC1/PGED1)
//
//// FPOR
//#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
//#pragma config RETVR = OFF              // Retention Voltage Regulator Enable bit (Retention regulator is disabled)
//#pragma config LPBOREN = ON             // Downside Voltage Protection Enable bit (Low power BOR is enabled, when main BOR is disabled)
//
//// FWDT
//#pragma config SWDTPS = PS1048576       // Sleep Mode Watchdog Timer Postscale Selection bits (1:1048576)
//#pragma config FWDTWINSZ = PS25_0       // Watchdog Timer Window Size bits (Watchdog timer window size is 25%)
//#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Watchdog timer is in non-window mode)
//#pragma config RWDTPS = PS1048576       // Run Mode Watchdog Timer Postscale Selection bits (1:1048576)
//#pragma config RCLKSEL = LPRC           // Run Mode Watchdog Timer Clock Source Selection bits (Clock source is LPRC (same as for sleep mode))
//#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT is disabled)
//
//// FOSCSEL
//#pragma config FNOSC = FRCDIV           // Oscillator Selection bits (Fast RC oscillator (FRC) with divide-by-N)
//#pragma config PLLSRC = PRI             // System PLL Input Clock Selection bit (Primary oscillator is selected as PLL reference input on device reset)
//#pragma config SOSCEN = OFF             // Secondary Oscillator Enable bit (Secondary oscillator is disabled)
//#pragma config IESO = ON                // Two Speed Startup Enable bit (Two speed startup is enabled)
//#pragma config POSCMOD = XT             // Primary Oscillator Selection bit (XT oscillator mode is selected)
//#pragma config OSCIOFNC = ON            // System Clock on CLKO Pin Enable bit (System clock is connected to CLKO/OSC2 pin)
//#pragma config SOSCSEL = OFF            // Secondary Oscillator External Clock Enable bit (SOSC pins configured for Crystal mode)
//#pragma config FCKSM = CSECME           // Clock Switching and Fail-Safe Clock Monitor Enable bits (Clock switching is enabled; Fail-safe clock monitor is enabled)
//
//// FSEC
//#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)
//
//// #pragma config statements should precede project file includes.
//// Use project enums instead of #define for ON and OFF.
//
//
//
////</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Defines">
#define ARRAY_SIZE      12


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Data">


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Functions">

static void MAIN_init()
{
    SYSTEM_Initialize();
    
//    CLK_Initialize();     
//    BTN_init();
//    BTN_initInt();
    
//    LEDA_SetDigitalOutput();
//    LEDB_SetDigitalOutput();

//    TMR_2_init();
//    INT_init();
    
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