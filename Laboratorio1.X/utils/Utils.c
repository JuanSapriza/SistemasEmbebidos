
//<editor-fold defaultstate="collapsed" desc="Includes">

#include "Utils.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include "../mcc_generated_files/tmr2.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../framework/USB_fwk.h"

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Variables">

static UTS_delayHandler_t UTS_delayHandlerVector[UTS_DELAY_HANDLER_COUNT];
static UTS_MENU_ITEM_t UTS_menusVector[UTS_MENU_HANDLER_COUNT];
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="DELAY">

bool UTS_delayms( UTS_DELAY_HANDLER_t p_handlerIndex, uint32_t p_tiempo, bool p_reiniciar )
{
    if( !UTS_delayHandlerVector[p_handlerIndex].active )
    {
        UTS_delayHandlerVector[p_handlerIndex].initialTime = TMR2_SoftwareCounterGet();
        UTS_delayHandlerVector[p_handlerIndex].countTime = p_tiempo;
        UTS_delayHandlerVector[p_handlerIndex].active = true;
    }
    
    if( p_reiniciar )
    {
        UTS_delayHandlerVector[p_handlerIndex].initialTime = 0;
        UTS_delayHandlerVector[p_handlerIndex].countTime = 0;
        UTS_delayHandlerVector[p_handlerIndex].active = false;
        return true;
    }
    
    if( TMR2_SoftwareCounterGet() >= UTS_delayHandlerVector[p_handlerIndex].initialTime + UTS_delayHandlerVector[p_handlerIndex].countTime )
    {
        UTS_delayHandlerVector[p_handlerIndex].initialTime = 0;
        UTS_delayHandlerVector[p_handlerIndex].countTime = 0;
        UTS_delayHandlerVector[p_handlerIndex].active = false;
        return true;
    }

    return false;

}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="LEDS">

void UTS_ledBlink( uint32_t p_ON, uint32_t p_OFF )
{
    static UTS_DELAY_HANDLER_t LED_1_delay = UTS_DELAY_HANDLER_LED_A;    
    static bool ledState = false;
    
    if( ledState )
    {
        if( UTS_delayms( LED_1_delay, p_OFF, false ) )
        {
            LED_A_SetHigh(); 
            ledState = false;
        }    
    }
    else
    {
        if( UTS_delayms( LED_1_delay, p_ON, false ) )
        {
            LED_A_SetLow();
            ledState = true;
        }
    }
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="MENÚ">

bool UTS_addOption2Menu( UTS_MENU_HANDLER_t p_menu, uint8_t* p_optionName )
{
    uint8_t i;
    
    if( p_menu < 0 || p_menu >= UTS_MENU_HANDLER_COUNT ) return false;
    if( strlen(p_optionName) >= UTS_MENU_MAX_OPTION_NAME_LENGTH ) return false;
    
    for( i = 0; i < UTS_MENU_MAX_OPTIONS; i++ )
    {
        if( UTS_menusVector[p_menu].optionNames[i][0] == 0 )
        {
            strcpy(UTS_menusVector[p_menu].optionNames[i], p_optionName );
            UTS_menusVector[p_menu].optionsNumber ++; 
            break;
        }
    }
    return true;
}

bool UTS_addTitle2Menu( UTS_MENU_HANDLER_t p_menu, uint8_t* p_title )
{
    if( p_menu < 0 || p_menu >= UTS_MENU_HANDLER_COUNT ) return false;
    if( strlen(p_title) >= UTS_MENU_ITEM_TITLE_LENGTH ) return false;  
    
    strcpy( UTS_menusVector[p_menu].title, p_title);
    memset( UTS_menusVector[p_menu].optionNames, 0, sizeof(UTS_menusVector[p_menu].optionNames) );
    UTS_menusVector[p_menu].optionsNumber = 0; 
    return true;
}

uint8_t* UTS_getMenuTitle( UTS_MENU_HANDLER_t p_menu )
{
    if( p_menu < 0 || p_menu >= UTS_MENU_HANDLER_COUNT ) return NULL;
    return &(UTS_menusVector[p_menu].title[0]); 
}

uint8_t* UTS_getMenuOption( UTS_MENU_HANDLER_t p_menu, uint8_t p_option )
{
    if( p_menu < 0 || p_menu >= UTS_MENU_HANDLER_COUNT ) return NULL;
    if( p_option < 0 || p_option >= UTS_menusVector[p_menu].optionsNumber ) return NULL;
    return &(UTS_menusVector[p_menu].optionNames[p_option][0]); 
}

uint8_t UTS_getmenuOptionsNumber( UTS_MENU_HANDLER_t p_menu )
{
    if( p_menu < 0 || p_menu >= UTS_MENU_HANDLER_COUNT ) return 0;
    return UTS_menusVector[p_menu].optionsNumber; 
}

int8_t UTS_showMenuAndGetAnswer( UTS_MENU_HANDLER_t p_menu, bool p_return )
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
            if( p_return )
            {
                sprintf( USB_dummyBuffer,"Presione %s para volver \n >", UTS_MENU_RETURN_CHAR );
                USB_write(USB_dummyBuffer);
            }
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
            else if( strstr(USB_dummyBuffer, UTS_MENU_RETURN_CHAR) != NULL )
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

