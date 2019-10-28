

#include <string.h>
#include "RTCC_fwk.h"
#include "USB_fwk.h"


bool RTC_getUserTime( struct tm* p_time )
{
    static enum RTC_GET_TIME_STATE state = GET_TIME_INIT;
    int8_t keyWord;
    
    switch( state )
    {
        case GET_TIME_INIT:
            USB_write("Ingrese Fecha y Hora \n");
            USB_write("Formato: \n");
            USB_write(" fecha:DD/MM/AAAA \n");
            USB_write(" hora:HH:MM:SS \n");
            state = GET_TIME_GET;
            break;
            
        case GET_TIME_GET:
            strcpy( USB_dummyBuffer, USB_read(0) );
            keyWord = strstr(USB_dummyBuffer,"fecha:");
            if( keyWord != 0 )
            {
                keyWord++; 
                p_time->tm_mday = (uint32_t)strtol(keyWord,&keyWord,10);
                //chequear que no haya error
                keyWord++;
                p_time->tm_mon = (uint32_t)strtol(keyWord,&keyWord,10);
                keyWord++;
                p_time->tm_year = (uint32_t)strtol(keyWord,&keyWord,10);
            }
            
    }
    return false;

}

