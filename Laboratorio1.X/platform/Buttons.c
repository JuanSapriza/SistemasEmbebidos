
#include "Buttons.h"
#include "../mcc_generated_files/pin_manager.h"


bool BTN_isButtonPressed( BTN_BUTTON_NAMES_t p_button )
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

bool BTN_switch( BTN_BUTTON_NAMES_t p_button )
{
    static bool btnState = false;
    static bool swState = false;
    
    if( BTN_isButtonPressed( p_button ) && !btnState )
    {
        btnState = true;
        swState = !swState;
    }
    else if( !BTN_isButtonPressed( p_button ) )
    {
        btnState = false;
    }
    return swState;
}