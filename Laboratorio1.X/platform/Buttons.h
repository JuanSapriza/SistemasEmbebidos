
#ifndef _BUTTONS_H
#define _BUTTONS_H

//#include "HardwareProfile.h"
#include <stdbool.h>




enum BTN_BUTTON_NAMES
{
    BTN_BUTTON_A,
    BTN_BUTTON_B,
};



//void BTN_init();
//void BTN_initInt();
bool BTN_isButtonPressed( enum BTN_BUTTON_NAMES p_button );










#endif