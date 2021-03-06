
#ifndef _UTILS_H
#define _UTILS_H


//<editor-fold defaultstate="collapsed" desc="Includes">

#include <stdint.h>
#include <stdbool.h>
#include "../mcc_generated_files/tmr2.h"

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Defines">

#define UTS_DELAY_CYCLES 1000

#define UTS_MENU_ITEM_TITLE_LENGTH 80
#define UTS_MENU_MAX_OPTIONS 15
#define UTS_MENU_MAX_OPTION_NAME_LENGTH 60

#define UTS_MENU_RETURN_CHAR "*"

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enums">

enum UTS_DELAY_ESTADOS
{
    UTS_DELAY_ESTADOS_INIT,
    UTS_DELAY_ESTADOS_CHECK,
};

enum USB_SHOW_MENU_STATES
{
    USB_SHOW_MENU_STATES_INIT,
    USB_SHOW_MENU_STATES_SHOW,
    USB_SHOW_MENU_STATES_WAIT,
};

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Data Types">

typedef enum UTS_DELAY_HANDLER
{
    UTS_DELAY_HANDLER_LED_A,
    UTS_DELAY_HANDLER_INITIAL_AT,
    UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_TIMEOUT,
    UTS_DELAY_HANDLER_AT_SEND_AND_WAIT_ACHIQUEN,
    UTS_DELAY_HANDLER_USB_SEND_TO_MODEM_ACHIQUEN,
    UTS_DELAY_HANDLER_READ_FROM_MODEM_ACHIQUEN,  
    UTS_DELAY_HANDLER_RGB_SEND,
    UTS_DELAY_HANDLER_MDM_PWR,
    UTS_DELAY_HANDLER_MDM_WAIT,
    UTS_DELAY_HANDLER_AD_CONVERT,
    UTS_DELAY_HANDLER_GO_ROUND,
    
    UTS_DELAY_HANDLER_LOG_DATA, //Para guardar los registros en el historial
            
    UTS_DELAY_HANDLER_APP,
            
    UTS_DELAY_HANDLER_DUMMY_1,
    UTS_DELAY_HANDLER_SENSE_HUMIDITY,
    UTS_DELAY_HANDLER_DUMMY_3,
    
    UTS_DELAY_HANDLER_MDM_ERROR_TIMEOUT,
    UTS_DELAY_HANDLER_TIME_SYNC,
    UTS_DELAY_HANDLER_HUMIDITY_SENSE,
    UTS_DELAY_HANDLER_HUMIDITY_COOLDOWN,
    UTS_DELAY_HANDLER_REGISTRY_LOG,
    UTS_DELAY_HANDLER_GPS_GET,
            
    UTS_DELAY_HANDLER_COUNT,
}UTS_DELAY_HANDLER_t;

    // D�NDE LA FUNCI�N DE DELAY ALMACENA LA INFORMACI�N DE CADA HANDLER 
typedef struct UTS_DELAY_HANDLER_TYPE
{
    uint32_t initialTime;
    uint32_t countTime;
    bool active;
}UTS_delayHandler_t;

typedef enum 
{
    UTS_MENU_HANDLER_MENU_PRINCIPAL,
    UTS_MENU_HANDLER_MENU_THRESHOLDS,
    UTS_MENU_HANDLER_MENU_PARAMETER,
    UTS_MENU_HANDLER_MENU_CEL_CONFIG,
    UTS_MENU_HANDLER_COUNT,
} UTS_MENU_HANDLER_t;

typedef struct 
{
    uint8_t title[ UTS_MENU_ITEM_TITLE_LENGTH ];
    uint8_t optionsNumber;
    uint8_t optionNames[UTS_MENU_MAX_OPTIONS][UTS_MENU_MAX_OPTION_NAME_LENGTH];
    UTS_MENU_HANDLER_t handler;
}UTS_MENU_ITEM_t;

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Delay">

bool UTS_delayms( UTS_DELAY_HANDLER_t p_handlerIndex, uint32_t p_tiempo, bool p_reiniciar );

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="LEDS">

void UTS_ledBlink( uint32_t p_ON, uint32_t p_OFF );

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="MEN�">
bool UTS_addOption2Menu( UTS_MENU_HANDLER_t p_menu, uint8_t* p_optionName );
bool UTS_addTitle2Menu( UTS_MENU_HANDLER_t p_menu, uint8_t* p_title );
uint8_t* UTS_getMenuTitle( UTS_MENU_HANDLER_t p_menu );
uint8_t* UTS_getMenuOption( UTS_MENU_HANDLER_t p_menu, uint8_t p_option );
uint8_t UTS_getmenuOptionsNumber( UTS_MENU_HANDLER_t p_menu );
int8_t UTS_showMenuAndGetAnswer( UTS_MENU_HANDLER_t p_menu, bool p_return );
//</editor-fold>

#endif