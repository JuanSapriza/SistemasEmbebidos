#ifndef _USB_FWK_H
#define _USB_FWK_H


#include "../mcc_generated_files/usb/usb.h"


#define USB_BUFFER_SIZE 255 // porque putUSBUSART y el de read toman como parametro de largo un uint8_t, por lo que este largo no podria ser mas de 255
                            // se podria modificar todo, pero seria alto laburo y no hay garantias de que eso no sea intencional. 
#define USB_AUX_BUFFER_SIZE 512

#define USB_FWK_RETURN_CHAR "*"

uint8_t USB_dummyBuffer[USB_BUFFER_SIZE];

//<editor-fold defaultstate="collapsed" desc="USB Gral">
bool USB_isSth2Write();
uint8_t *USB_read( uint8_t p_length );
void USB_write( uint8_t *p_text );
void USB_CDC_tasks();
bool USB_sth2Read();
uint8_t* USB_whatsInReadBuffer();
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Menu">

enum USB_SHOW_MENU_STATES
{
    USB_SHOW_MENU_STATES_INIT,
    USB_SHOW_MENU_STATES_SHOW,
    USB_SHOW_MENU_STATES_WAIT,
};



//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Modem">

typedef enum 
{
    USB_SNIFF_TYPE_TX,
    USB_SNIFF_TYPE_RX,
    USB_SNIFF_TYPE_BOTH,
    USB_SNIFF_TYPE_NONE,
} USB_SNIFF_TYPE_t;

void USB_sniff( uint8_t* p_string, USB_SNIFF_TYPE_t p_type );
void USB_sniffSetType( USB_SNIFF_TYPE_t p_type );
USB_SNIFF_TYPE_t USB_sniffType();
void USB_send2Modem();

//</editor-fold>

#endif