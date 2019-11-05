#ifndef _USB_FWK_H
#define _USB_FWK_H


#include "../mcc_generated_files/usb/usb.h"


#define USB_BUFFER_SIZE 128

uint8_t USB_dummyBuffer[USB_BUFFER_SIZE];

//<editor-fold defaultstate="collapsed" desc="USB Gral">
uint8_t *USB_read( uint8_t p_length );
void USB_write( uint8_t *p_text );
bool USB_CDC_tasks();
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Menu">

enum USB_SHOW_MENU_STATES
{
    USB_SHOW_MENU_STATES_INIT,
    USB_SHOW_MENU_STATES_SHOW,
    USB_SHOW_MENU_STATES_WAIT,
};



//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Others">

void USB_sniff( uint8_t* p_string, bool p_direction );

//</editor-fold>

#endif