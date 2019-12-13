
#ifndef _BUTTONS_H
#define _BUTTONS_H

#include <stdbool.h>

typedef enum 
{
    BTN_BUTTON_A,
    BTN_BUTTON_B,
}BTN_BUTTON_NAMES_t;


bool BTN_isButtonPressed( BTN_BUTTON_NAMES_t p_button );

bool BTN_switch( BTN_BUTTON_NAMES_t p_button );


#endif
