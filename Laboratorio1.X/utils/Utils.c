
#include <limits.h>
#include <string.h>
#include "Utils.h"
#include "../mcc_generated_files/tmr2.h"



static UTS_delayHandler_t UTS_delayHandler[UTS_DELAY_HANDLER_COUNT];

//<editor-fold defaultstate="collapsed" desc="DELAY">
bool UTS_delayms( UTS_DELAY_HANDLER_t p_handlerIndex, uint32_t p_tiempo, bool p_reiniciar )
{
    uint32_t esteTiempo;
    
    if( !UTS_delayHandler[p_handlerIndex].active )
    {
        UTS_delayHandler[p_handlerIndex].initialTime = TMR2_SoftwareCounterGet();
        UTS_delayHandler[p_handlerIndex].countTime = p_tiempo;
        UTS_delayHandler[p_handlerIndex].active = true;
    }
    
    if( p_reiniciar )
    {
        UTS_delayHandler[p_handlerIndex].initialTime = 0;
        UTS_delayHandler[p_handlerIndex].countTime = 0;
        UTS_delayHandler[p_handlerIndex].active = false;
        return true;
    }
    
    esteTiempo = TMR2_SoftwareCounterGet();
    
    if( TMR2_SoftwareCounterGet() >= UTS_delayHandler[p_handlerIndex].initialTime + UTS_delayHandler[p_handlerIndex].countTime )
    {
        UTS_delayHandler[p_handlerIndex].initialTime = 0;
        UTS_delayHandler[p_handlerIndex].countTime = 0;
        UTS_delayHandler[p_handlerIndex].active = false;
        return true;
    }

//    if( ( ( esteTiempo - UTS_delayHandler[p_handlerIndex].initialTime ) < 0 ) ) //overflow
//    {
//        if( (esteTiempo + 4294967296  - UTS_delayHandler[p_handlerIndex].initialTime) >= UTS_delayHandler[p_handlerIndex].countTime )
//        {   // [                                                       ] 32bits
//            //        ^tiempo Actual           ^ tiempo Inicial   
//            //                                 |---------------------->
//            //  |----->
//            //                     Tiempo Transcurrido
//
//            UTS_delayHandler[p_handlerIndex].initialTime = 0;
//            UTS_delayHandler[p_handlerIndex].countTime = 0;
//            UTS_delayHandler[p_handlerIndex].active = false;
//            return true;
//        }
//    }
//    else
//    {
//        if( ( esteTiempo - UTS_delayHandler[p_handlerIndex].initialTime ) >= UTS_delayHandler[p_handlerIndex].countTime )
//        {
//            UTS_delayHandler[p_handlerIndex].initialTime = 0;
//            UTS_delayHandler[p_handlerIndex].countTime = 0;
//            UTS_delayHandler[p_handlerIndex].active = false;
//            return true;
//        }
//    }
    
    return false;

}
//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="MEN�">

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


//</editor-fold>