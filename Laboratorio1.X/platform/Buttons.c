
#include "Buttons.h"
#include "../mcc_generated_files/pin_manager.h"


//volatile bool BTN_A_pressed; 
//volatile bool BTN_B_pressed; 



//void __ISR ( _CHANGE_NOTICE_B_VECTOR, IPL2SOFT ) _CHANGE_NOTICE_B( void ) 
//{
//    
//    if (IFS0bits.CNBIF == 1) 
//    {
//        IFS0CLR= 1 << _IFS0_CNBIF_POSITION;
//        if (CNFBbits.CNFB15 == 1)
//        {
//            CNFBCLR=0x8000;
//            BTN_A_pressed = BTNA_getValue(); 
//        }
//    }
//    
//    
//}
//
//void __ISR ( _CHANGE_NOTICE_A_VECTOR, IPL2SOFT ) _CHANGE_NOTICE_A( void ) 
//{
//    if (IFS0bits.CNAIF == 1) 
//    {
//        IFS0CLR= 1 << _IFS0_CNAIF_POSITION;
//        if (CNFAbits.CNFA13 == 1)
//        {
//            CNFACLR=0x2000;
//            BTN_B_pressed = BTNB_getValue(); 
//        }
//    }
//}

//void BTN_init()
//{
//    BTNA_setDigitalInput();
//    BTNB_setDigitalInput();
//    BTNA_setPullDown();
//    BTNB_setPullDown();
//}


//void BTN_initInt()
//{
//    BTN_A_pressed = false;
//    BTN_B_pressed = false;
//    BTNA_setInt();
//    BTNB_setInt();
//    BTNA_setIntBothEdge();  
//    BTNB_setIntBothEdge();  
//}

bool BTN_isButtonPressed( enum BTN_BUTTON_NAMES p_button )
{
    switch( p_button )
    {
        case BTN_BUTTON_A:
            return BTN_A_pressed;
            
        case BTN_BUTTON_B:
            return BTN_B_pressed;
            
        default: 
            return false;
    }
}