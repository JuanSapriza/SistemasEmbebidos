#include "USB_fwk.h"
#include <string.h>
#include <stdio.h>
#include "../utils/Utils.h" 


static uint8_t readBuffer[USB_BUFFER_SIZE];
static uint8_t writeBuffer[USB_BUFFER_SIZE];
static bool sth2write;

//<editor-fold defaultstate="collapsed" desc="USB Gral">

bool USB_CDC_tasks()
{
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return false;
    }

    if( USBIsDeviceSuspended()== true )
    {
        return false;
    }
    
    if( sth2write && USBUSARTIsTxTrfReady() ) //hay algo para escribir
    {
       putUSBUSART(writeBuffer,strlen(writeBuffer)); 
       sth2write = false;
    }
    
    CDCTxService();
    return true;
}

void USB_write( uint8_t *p_text )
{
    if( strlen(p_text) == 0 ) return;
    
    if( !sth2write )
    {
        memset(writeBuffer, 0, sizeof(writeBuffer));
    }
    
    if( strlen( p_text ) < USB_BUFFER_SIZE -strlen(writeBuffer)  )
    {
        strcat(writeBuffer, p_text);
    }
    else
    {
        strncat(writeBuffer, p_text, USB_BUFFER_SIZE - strlen(writeBuffer) );
    }
    sth2write = true;
}

uint8_t *USB_read( uint8_t p_length )
{
    memset( readBuffer, 0, sizeof(readBuffer) ); 
    
    if ( p_length <= sizeof(readBuffer) && 0 < p_length )
    {
         getsUSBUSART(readBuffer, p_length);
    }
    else
    {
        getsUSBUSART(readBuffer, sizeof(readBuffer));
    }
    return ( readBuffer );
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
            sprintf( USB_dummyBuffer,"%s \n", UTS_getMenuTitle( p_menu ) );
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
            USB_write("Presione '-' para volver ");
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
            else if( strstr(USB_dummyBuffer,"-") != NULL )
            {
                state[p_menu] = USB_SHOW_MENU_STATES_INIT;
                return (int8_t)-1; 
            }
            break;
            
        default: break;
    }
    
    return 0;
}



//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Others">
void USB_sniff( uint8_t* p_string, bool p_direction )
{
    if( p_direction )
    {
        USB_write("\n>");
    }
    else
    {
        USB_write("\n<");
    }
    USB_write( p_string );  
    USB_write("\n");
}

//</editor-fold>