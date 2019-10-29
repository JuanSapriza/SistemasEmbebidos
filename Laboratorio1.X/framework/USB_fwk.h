#ifndef _USB_FWK_H
#define _USB_FWK_H


#include "../mcc_generated_files/usb/usb.h"


#define USB_BUFFER_SIZE 128

uint8_t USB_dummyBuffer[USB_BUFFER_SIZE];

uint8_t *USB_read( uint8_t p_length );
void USB_write( uint8_t *p_text );
bool USB_CDC_tasks();




#endif