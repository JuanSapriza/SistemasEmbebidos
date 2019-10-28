#include "USB_fwk.h"
#include <string.h>


static uint8_t readBuffer[USB_BUFFER_SIZE];
static uint8_t writeBuffer[USB_BUFFER_SIZE];
static bool sth2write;

//void MCC_USB_CDC_DemoTasks(void)
//{
//    if( USBGetDeviceState() < CONFIGURED_STATE )
//    {
//        return;
//    }
//
//    if( USBIsDeviceSuspended()== true )
//    {
//        return;
//    }
//
//    if( USBUSARTIsTxTrfReady() == true)
//    {
//        uint8_t i;
//        uint8_t numBytesRead;
//
//        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));
//
//        for(i=0; i<numBytesRead; i++)
//        {
//            switch(readBuffer[i])
//            {
//                /* echo line feeds and returns without modification. */
//                case 0x0A:
//                case 0x0D:
//                    writeBuffer[i] = readBuffer[i];
//                    break;
//
//                /* all other characters get +1 (e.g. 'a' -> 'b') */
//                default:
//                    writeBuffer[i] = readBuffer[i] + 1;
//                    break;
//            }
//        }
//
//        if(numBytesRead > 0)
//        {
//            putUSBUSART(writeBuffer,numBytesRead);
//        }
//    }
//
//    CDCTxService();
//}

//bool USB_CDC_tasks()
//{
//    if( USBGetDeviceState() < CONFIGURED_STATE )
//    {
//        return false;
//    }
//
//    if( USBIsDeviceSuspended()== true )
//    {
//        return false;
//    }
//    CDCTxService();
//    return true;
//}
//
//
//void USB_write( uint8_t *p_text )
//{
//    if( strlen(p_text) == 0 ) return;
//    
//    if( strlen( p_text ) < 64 )
//    {
//        putUSBUSART(p_text,strlen(p_text));
//    }
//    else
//    {
//        putUSBUSART(p_text, 64);
//    }
//}



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
    
    if( sth2write ) //hay algo para escribir
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