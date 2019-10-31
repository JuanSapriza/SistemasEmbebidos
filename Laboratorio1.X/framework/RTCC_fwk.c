

#include <string.h>
#include "RTCC_fwk.h"
#include "USB_fwk.h"


bool RTC_getUserTime( struct tm* p_time )
{
    static enum RTC_GET_TIME_STATE state = GET_TIME_INIT;
    static bool msgShow = true;
    uint8_t* keyWord;
    
    
    switch( state )
    {
        case GET_TIME_INIT:
            if( *USB_read(0) != 0 ) //lamentablemente tengo que poner algo de esto
            {
                memset( p_time, 0, sizeof( struct tm ) );
                state = GET_TIME_GET_DATE;
            }
            break;
            
        case GET_TIME_GET_DATE:
            if( msgShow )
            {
                USB_write("Ingrese la FECHA\n");
                USB_write("Formato: \n");
                USB_write(" << fecha:DD/MM/AAAA >>\n");
                msgShow = false;
            }
            strcpy( USB_dummyBuffer, USB_read(0) );
            keyWord = strstr(USB_dummyBuffer,"fecha:"); //puntero al primer caracter de la palabra 'fecha' (si la encontro)
            if( keyWord != 0 )
            {
                keyWord += 6 ; //largo de "fecha:" 
                p_time->tm_mday = (uint32_t)strtol((char*)keyWord,(char**)(&keyWord),10);
                //chequear que no haya error
                keyWord ++; //strtol ya me deja en keyWord el puntero al siguiente elemento
                p_time->tm_mon = (uint32_t)strtol((char*)keyWord,(char**)(&keyWord),10);
                keyWord ++; 
                p_time->tm_year = (uint32_t)strtol((char*)keyWord,(char**)(&keyWord),10);
                
                msgShow = true;
                state = GET_TIME_GET_TIME;
            }
            break;
                
        case GET_TIME_GET_TIME:
            if( msgShow )
            {
                USB_write("Ingrese la HORA\n");
                USB_write("Formato: \n");
                USB_write(" << hora:HH:MM:SS >>\n");
                msgShow = false;
            }
            strcpy( USB_dummyBuffer, USB_read(0) );
            keyWord = strstr(USB_dummyBuffer,"hora:");
            if( keyWord != 0 )
            {
                keyWord += 5 ; //largo de "hora:" 
                p_time->tm_hour = (uint32_t)strtol((char*)keyWord,(char**)(&keyWord),10);
                //chequear que no haya error
                keyWord ++; //strtol ya me deja en keyWord el puntero al siguiente elemento
                p_time->tm_min = (uint32_t)strtol((char*)keyWord,(char**)(&keyWord),10);
                keyWord ++; 
                p_time->tm_sec = (uint32_t)strtol((char*)keyWord,(char**)(&keyWord),10);

                msgShow = true;
                state = GET_TIME_CHECK;
            }
            break;
                
        case GET_TIME_CHECK:
            //chequear error
            //si no hay error...
            USB_write("Listo :) \n");
            state = GET_TIME_INIT;
            return true;
            //...
            break;
            
            
        default: break;
            
    }
    return false;

}

