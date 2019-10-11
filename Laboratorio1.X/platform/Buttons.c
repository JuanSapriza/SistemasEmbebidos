
#include "Buttons.h"
#include "../mcc_generated_files/pin_manager.h"


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