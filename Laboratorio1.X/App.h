
#ifndef APP_H
#define	APP_H

//<editor-fold defaultstate="collapsed" desc="Versión">

#define COMMIT_VERSION Lab4_GPS_Y_POTENCIOMETRO

//#define LABORATORIO_1
//#define LABORATORIO_2
//#define LABORATORIO_2_1
//#define LABORATORIO_3
//#define LABORATORIO_3_2
//#define LABORATORIO_3_3
//#define LABORATORIO_3_4
//#define LABORATORIO_3_5
//#define LABORATORIO_4
//#define LABORATORIO_4_2
#define PROYECTO_1


//</editor-fold>

#include "mcc_generated_files/rtcc.h"
#include <stdint.h>
#include "platform/GPS.h"
#include <time.h>
#include <stdbool.h>

#define APP_LOG_BUFFER_SIZE 3

#ifdef LABORATORIO_3_5

enum APP_STATES
{
    APP_STATE_INIT,
    APP_STATE_WAIT,
    APP_STATE_MAIN_MENU_CREATE,
    APP_STATE_MAIN_MENU_SHOW,
    APP_STATE_FINISH,
    APP_STATE_CHECK,
    APP_STATE_TASKS,
};



struct APP_info_t
{
    enum APP_STATES state;
    struct tm* time;

}APP_info;


#endif

#ifdef LABORATORIO_4


//Funcion para mostrar el nivel de humedad con los leds RGB:

void APP_RGB_humidity ( uint8_t ADC_linearized );



//Funcion para indicar riego con led A:

enum APP_IRRIGATE
{
    APP_IRRIGATE_OFF,
    APP_IRRIGATE_ON,
};

enum APP_LEDA
{
    APP_LEDA_OFF,
    APP_LEDA_ON,
};

void APP_LEDA_irrigate ( uint8_t ADC_humedad);




//Funcion que actualiza el registro historico


typedef struct
{
    uint8_t humidity;
//    time_t time;
//    GPSPosition_t position;
}APP_var_t;

APP_var_t APP_logBuffer[APP_LOG_BUFFER_SIZE];

enum APP_LOG
{
    APP_LOG_PTR_INIT,
    APP_LOG_PTR_OK,
};

void APP_LOG_data ( APP_var_t log_data );




//Funcion para riego a demanda con botón A

void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad );

enum APP_MANUAL_IRRIGATE
{
    APP_MANUAL_IRRIGATE_INIT,
    APP_MANUAL_IRRIGATE_BTN_PRESSED,
    APP_MANUAL_IRRIGATE_LEDA_OFF,
    APP_MANUAL_IRRIGATE_LEDA_ON,
};

#endif

#ifdef LABORATORIO_4_2

enum APP_STATES
{
    APP_STATE_INIT,
    APP_STATE_GPS_GET,
    APP_STATE_WAIT,
    APP_STATE_PARSE_FRAME,
    APP_STATE_MAIN_MENU_CREATE,
    APP_STATE_MAIN_MENU_SHOW,
    APP_STATE_FINISH,
    APP_STATE_CHECK,
    APP_STATE_TASKS,
};



struct APP_info_t
{
    enum APP_STATES state;
    struct tm* time;
//    GPSPosition_t position;
}APP_info;

#endif

#ifdef PROYECTO_1

enum APP_STATES
{
    APP_STATE_INIT,
    APP_STATE_GPS_GET,
    APP_STATE_WAIT,
    APP_STATE_PARSE_FRAME,
    APP_STATE_MAIN_MENU_CREATE,
    APP_STATE_MAIN_MENU_SHOW,
    APP_STATE_FINISH,
    APP_STATE_CHECK,
    APP_STATE_TASKS,
};

enum APP_IRRIGATE
{
    APP_IRRIGATE_OFF,
    APP_IRRIGATE_ON,
};

enum APP_LEDA
{
    APP_LEDA_OFF,
    APP_LEDA_ON,
};

enum APP_LOG
{
    APP_LOG_PTR_INIT,
    APP_LOG_PTR_OK,
};

enum APP_MANUAL_IRRIGATE
{
    APP_MANUAL_IRRIGATE_INIT,
    APP_MANUAL_IRRIGATE_BTN_PRESSED,
    APP_MANUAL_IRRIGATE_LEDA_OFF,
    APP_MANUAL_IRRIGATE_LEDA_ON,
};




struct APP_info_t
{
    enum APP_STATES state;
    time_t time;
    GPSPosition_t position;
    bool position_validity;
}APP_info;

typedef struct
{
    uint8_t humidity;
    time_t time;
    GPSPosition_t position;
    bool position_validity;
}APP_var_t;

APP_var_t APP_logBuffer[APP_LOG_BUFFER_SIZE];




//Funcion para mostrar el nivel de humedad con los leds RGB:
void APP_RGB_humidity ( uint8_t ADC_linearized );
//Funcion para indicar riego con led A:
void APP_LEDA_irrigate ( uint8_t ADC_humedad);
//Funcion que actualiza el registro historico
void APP_LOG_data ( APP_var_t log_data );
//Funcion para riego a demanda con botón A
void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad );

#endif


#endif	/* APP_H */

