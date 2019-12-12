#ifndef _USB_FWK_H
#define _USB_FWK_H

//<editor-fold defaultstate="collapsed" desc="Includes">

#include "../mcc_generated_files/usb/usb.h"

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Defines">

#define USB_BUFFER_SIZE 255  
#define USB_AUX_BUFFER_SIZE 512

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Variables">

uint8_t USB_dummyBuffer[USB_BUFFER_SIZE];

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Data Types">

        // TIPOS DE SNIFFEO
typedef enum 
{
    USB_SNIFF_TYPE_NONE,
    USB_SNIFF_TYPE_RX,
    USB_SNIFF_TYPE_BOTH,
    USB_SNIFF_TYPE_TX,
} USB_SNIFF_TYPE_t;

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="USB Gral">
bool USB_sth2Write();
bool USB_sth2Read();
uint8_t *USB_read( uint8_t p_length );
void USB_write( uint8_t *p_text );
uint8_t* USB_whatsInReadBuffer();
void USB_CDC_tasks();
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Modem">

void USB_sniff( uint8_t* p_string, USB_SNIFF_TYPE_t p_type );
void USB_sniffSetType( USB_SNIFF_TYPE_t p_type );
USB_SNIFF_TYPE_t USB_sniffType();
void USB_send2Modem();

//</editor-fold>

#endif