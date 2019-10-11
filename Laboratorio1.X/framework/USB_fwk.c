#include "USB_fwk.h"
#include <string.h>


static uint8_t readBuffer[64];
//static uint8_t writeBuffer[64];

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
    CDCTxService();
    return true;
}


void USB_write( uint8_t *p_text )
{
    if( strlen(p_text) == 0 ) return;
    
    if( strlen( p_text ) < 64 )
    {
        putUSBUSART(p_text,strlen(p_text));
    }
    else
    {
        putUSBUSART(p_text, 64);
    }
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