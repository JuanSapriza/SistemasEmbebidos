#include "USB_fwk.h"
#include <string.h>
#include <stdio.h>
#include "../utils/Utils.h" 


static uint8_t USB_rxBuffer[USB_BUFFER_SIZE];
static uint8_t USB_txBuffer[USB_BUFFER_SIZE];
static uint8_t USB_txAuxBuffer[USB_AUX_BUFFER_SIZE];

static bool sth2write;
//static USB_SNIFF_TYPE_t sniffType = USB_SNIFF_TYPE_BOTH;
static USB_SNIFF_TYPE_t sniffType = USB_SNIFF_TYPE_NONE;

//<editor-fold defaultstate="collapsed" desc="USB Gral">

void USB_CDC_tasks()
{
    static bool sending = false;
    
    //<editor-fold defaultstate="collapsed" desc="Chequeo Inicial">
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
//        return false;
    }

    if( USBIsDeviceSuspended()== true )
    {
//        return false;
    }
    //</editor-fold>
    
//    if( sth2write && USBUSARTIsTxTrfReady() ) //hay algo para escribir
    if( sending )
    {
        if(  USBUSARTIsTxTrfReady() ) 
        {
           memset( USB_txBuffer, 0, sizeof(USB_txBuffer));
           if( USB_txAuxBuffer[0] != 0 ) //no es lo mismo que sth2write?? para que esta styh2write?? 
           {
                memcpy(USB_txBuffer,USB_txAuxBuffer,sizeof(USB_txBuffer)-1);
    
                memcpy(USB_txAuxBuffer,USB_txAuxBuffer+sizeof(USB_txBuffer)-1, sizeof(USB_txAuxBuffer)-sizeof(USB_txBuffer)+1);
                memset(USB_txAuxBuffer+sizeof(USB_txAuxBuffer)-sizeof(USB_txBuffer)+1, 0, sizeof(USB_txBuffer)-1 );
//                memcpy( USB_txBuffer, USB_txAuxBuffer, sizeof(USB_txBuffer) );
//                memset( USB_txAuxBuffer, 0, sizeof( USB_txAuxBuffer ) );                  //si son del mismo tamanio!!!!
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

bool USB_sth2Read()
{
    return (*USB_read(0) != 0);
}

void USB_write( uint8_t *p_text )
{
    if( strlen(p_text) == 0 ) return;
    
//    if( !sth2write )
//    {
//        memset(USB_txBuffer, 0, sizeof(USB_txBuffer));
//    }
    
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
    return ( USB_rxBuffer );
}

uint8_t* USB_whatsInReadBuffer()
{
    return USB_rxBuffer;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Menu">

int8_t USB_showMenuAndGetAnswer( UTS_MENU_HANDLER_t p_menu )
{
    static enum USB_SHOW_MENU_STATES state[UTS_MENU_HANDLER_COUNT] = {USB_SHOW_MENU_STATES_INIT};
    uint8_t i;
    uint8_t selectedOption; 
    
    if( p_menu < 0 || p_menu >= UTS_MENU_HANDLER_COUNT ) return -1;
    
    switch( state[p_menu] )
    {
        case USB_SHOW_MENU_STATES_INIT:
            USB_read(0); //para limpiar el buffer de lectura
            sprintf( USB_dummyBuffer,"\n%s \n", UTS_getMenuTitle( p_menu ) );
            USB_write( USB_dummyBuffer );
            for( i=0; i < UTS_getmenuOptionsNumber(p_menu) ; i++ )
            {
                memset(USB_dummyBuffer,0,sizeof(USB_dummyBuffer));
                if(UTS_getMenuOption(p_menu,i) != NULL)
                {
                    sprintf( USB_dummyBuffer,"%d. %s \n",i+1, UTS_getMenuOption( p_menu, i ) );
                    USB_write( USB_dummyBuffer );
                }
            }        
            sprintf( USB_dummyBuffer,"Presione %s para volver \n >", USB_FWK_RETURN_CHAR );
            USB_write(USB_dummyBuffer);
            memset( USB_dummyBuffer, 0, sizeof( USB_dummyBuffer ) );
            state[p_menu] = USB_SHOW_MENU_STATES_WAIT;
            break;
            
        case USB_SHOW_MENU_STATES_WAIT:
            strcpy( USB_dummyBuffer, USB_read(0) );
            selectedOption = (uint8_t)strtol((char*)USB_dummyBuffer,NULL,10);
            if( selectedOption > 0 && selectedOption <= UTS_getmenuOptionsNumber(p_menu) )
            {
                state[p_menu] = USB_SHOW_MENU_STATES_INIT;
                return (int8_t)selectedOption; 
            }
            else if( strstr(USB_dummyBuffer, USB_FWK_RETURN_CHAR) != NULL )
            {
                state[p_menu] = USB_SHOW_MENU_STATES_INIT;
                return (int8_t)-1; //return 
            }
            break;
            
        default: break;
    }
    
    return 0; //working
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
            
    if( UTS_delayms( UTS_DELAY_HANDLER_USB_SEND_TO_MODEM_ACHIQUEN, 10, false) ) //VER DE ACHICAR ESTE DELAY
    {
        if( *USB_read(0) !=  0 ) //mandamos algo
        {
            previousSniffType = USB_sniffType(); 
            USB_sniffSetType( USB_SNIFF_TYPE_RX ); 
        
            MDM_sendATCmd( USB_rxBuffer, NULL);
            
            USB_sniffSetType( previousSniffType ); 
        }
        MDM_readString();
    }
}


//</editor-fold>
