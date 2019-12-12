
//<editor-fold defaultstate="collapsed" desc="Includes">

#include "USB_fwk.h"
#include <string.h>
#include <stdio.h>
#include "../utils/Utils.h" 

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Variables">

static uint8_t USB_rxBuffer[USB_BUFFER_SIZE];
static uint8_t USB_txBuffer[USB_BUFFER_SIZE];
static uint8_t USB_txAuxBuffer[USB_AUX_BUFFER_SIZE];

static bool sth2write;
static USB_SNIFF_TYPE_t sniffType = USB_SNIFF_TYPE_NONE;
//static USB_SNIFF_TYPE_t sniffType = USB_SNIFF_TYPE_BOTH;

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="USB Gral">

bool USB_sth2Write()
{
    return sth2write;
}

bool USB_sth2Read()
{
    return (*USB_read(0) != 0);
}

void USB_write( uint8_t *p_text )
{
    if( strlen(p_text) == 0 ) return;
    
    if( strlen( p_text ) < sizeof(USB_txAuxBuffer) -strlen(USB_txAuxBuffer)  )
    {
        strcat(USB_txAuxBuffer, p_text);
    }
    else
    {
        strncat(USB_txAuxBuffer, p_text, sizeof(USB_txAuxBuffer) - strlen(USB_txAuxBuffer) );
    }
    sth2write = true;
}

uint8_t *USB_read( uint8_t p_length )
{
    memset( USB_rxBuffer, 0, sizeof(USB_rxBuffer) ); 
    
    if ( p_length <= sizeof(USB_rxBuffer) && 0 < p_length )
    {
         getsUSBUSART(USB_rxBuffer, p_length);
    }
    else
    {
        getsUSBUSART(USB_rxBuffer, sizeof(USB_rxBuffer));
    }
        if( strstr(USB_rxBuffer, "/*-") != NULL )
        {
            USB_sniffSetType( !(bool)(USB_sniffType()) );
        }
    return ( USB_rxBuffer );
}

uint8_t* USB_whatsInReadBuffer()
{
    return USB_rxBuffer;
}

void USB_CDC_tasks()
{
    static bool sending = false;
    
    //<editor-fold defaultstate="collapsed" desc="Chequeo Inicial">
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    if( USBIsDeviceSuspended()== true )
    {
        return;
    }
    //</editor-fold>
    
    if( sending )
    {
        if(  USBUSARTIsTxTrfReady() ) 
        {
           memset( USB_txBuffer, 0, sizeof(USB_txBuffer));
           if( USB_txAuxBuffer[0] != 0 ) 
           {
                memcpy(USB_txBuffer,USB_txAuxBuffer,sizeof(USB_txBuffer)-1);
    
                memcpy(USB_txAuxBuffer,USB_txAuxBuffer+sizeof(USB_txBuffer)-1, sizeof(USB_txAuxBuffer)-sizeof(USB_txBuffer)+1);
                memset(USB_txAuxBuffer+sizeof(USB_txAuxBuffer)-sizeof(USB_txBuffer)+1, 0, sizeof(USB_txBuffer)-1 );
               putUSBUSART(USB_txBuffer,strlen(USB_txBuffer)); 
           }
           else
           {
               sth2write = false;
               sending = false;
           }
        }
    }
    else
    {
       if( sth2write )
       {
           sending = true;
       }
    }
    
    CDCTxService();
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Modem">

void USB_sniff( uint8_t* p_string, USB_SNIFF_TYPE_t p_type )
{
    if( *p_string == 0 ) return;
    if( USB_sniffType() == p_type || USB_sniffType() == USB_SNIFF_TYPE_BOTH )
    {
        switch( p_type )
        {
            case USB_SNIFF_TYPE_RX:
                USB_write("\n<");
                break;
                
            case USB_SNIFF_TYPE_TX:
                USB_write("\n>");
                break;
        }
        USB_write( p_string );  
        USB_write("\n");
    }
}

void USB_sniffSetType( USB_SNIFF_TYPE_t p_type )
{
    sniffType = p_type;
}

USB_SNIFF_TYPE_t USB_sniffType()
{
    return sniffType;
}

void USB_send2Modem()
{
    USB_SNIFF_TYPE_t previousSniffType; 
            
    if( UTS_delayms( UTS_DELAY_HANDLER_USB_SEND_TO_MODEM_ACHIQUEN, 10, false) ) 
    {
        if( *USB_read(0) !=  0 ) // si se envía algo 
        {
//            previousSniffType = USB_sniffType(); 
//            USB_sniffSetType( USB_SNIFF_TYPE_RX ); 
        
            MDM_sendATCmd( USB_rxBuffer, NULL);
            
//            USB_sniffSetType( previousSniffType ); 
        }
        MDM_readString(); //para comerme el eco? cuidado! 
    }
}


//</editor-fold>
