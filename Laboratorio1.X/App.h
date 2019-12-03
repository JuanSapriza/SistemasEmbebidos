
#ifndef APP_H
#define	APP_H

//<editor-fold defaultstate="collapsed" desc="Versi�n">

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
//#define PRUEBAS_GSM
//#define PROYECTO_1
#define PROYECTO_2


//</editor-fold>


#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/adc1.h"
#include "platform/GPS.h"
#include "platform/RGB.h"



#define APP_LOG_BUFFER_SIZE 60

#define NUMERO_VICKY "\"+59891972950\""

#define APP_THRESHOLD_SATURATED_DEFAULT 5
#define AAPP_THRESHOLD_SLIGHTLY_SATURATED_DEFAULT 9
#define APP_THRESHOLD_SLIGHTLY_DRY_DEFAULT 20
#define APP_THRESHOLD_DRY_DEFAULT 40
#define APP_THRESHOLD_LOW_AUTOMATIC_DEFAULT 15
#define APP_THRESHOLD_HIGH_AUTOMATIC_DEFAULT 30
#define APP_THRESHOLD_MANUAL_DEFAULT 15


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




//Funcion para riego a demanda con bot�n A

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

#ifdef PRUEBAS_GSM


enum APP_STATES
{
    APP_STATE_INIT,
    APP_STATE_GPS_GET,
    APP_STATE_GSM_SMS_INIT,
    APP_STATE_GSM_SMS_SEND,
    APP_STATE_GSM_SMS_GET,
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
    struct tm* time;
    GPSPosition_t position;
}APP_info;

typedef struct
{
    uint8_t humidity;
    struct tm* time;
    GPSPosition_t position;
}APP_var_t;

APP_var_t APP_logBuffer[APP_LOG_BUFFER_SIZE];




//Funcion para mostrar el nivel de humedad con los leds RGB:
void APP_RGB_humidity ( uint8_t ADC_linearized );
//Funcion para indicar riego con led A:
void APP_LEDA_irrigate ( uint8_t ADC_humedad);
//Funcion que actualiza el registro historico
void APP_LOG_data ( APP_var_t log_data );
//Funcion para riego a demanda con bot�n A
void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad );
//Funcion que se encarga de todas las operaciones relativas al Modem
void APP_MDM_tasks();
//Funcion que se encarga de todas las operaciones relativas a los LEDs RGB
void APP_RGB_tasks()

#endif

#ifdef PROYECTO_1

enum APP_STATES
{
    APP_STATE_INIT,
    APP_STATE_GPS_GET,
    APP_STATE_GSM_SMS_INIT,
    APP_STATE_GSM_SMS_SEND,
    APP_STATE_GSM_SMS_GET,
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
//Funcion para riego a demanda con bot�n A
void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad );
//Funcion que se encarga de todas las operaciones relativas al Modem
void APP_MDM_tasks();
//Funcion que se encarga de todas las operaciones relativas a los LEDs RGB
void APP_RGB_tasks();

#endif


#ifdef PROYECTO_2

enum APP_INIT_STATES
{
    APP_INIT_MDM,
    APP_INIT_VARS,
};

enum APP_TASKS
{
    APP_TASK_POT,
    APP_TASK_POT_2_RGB,
//    APP_TASK_POT,
};

enum APP_UI_STATES
{
    APP_UI_STATE_INIT,
    APP_UI_STATE_WAIT_4_KEY,
    APP_UI_STATE_MENU_CREATE,
    APP_UI_STATE_MENU_SHOW,
    

};

enum APP_STATES
{
    APP_STATE_APP_INIT,
    APP_STATE_TASKS,
    APP_STATE_WAIT,
    
    
    APP_STATE_INIT,
    APP_STATE_GPS_GET,
    APP_STATE_GSM_SMS_INIT,
    APP_STATE_GSM_SMS_SEND,
    APP_STATE_GSM_SMS_GET,
    APP_STATE_PARSE_FRAME,
    APP_STATE_MAIN_MENU_CREATE,
    APP_STATE_MAIN_MENU_SHOW,
    APP_STATE_FINISH,
    APP_STATE_CHECK,
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

typedef struct 
{
    uint8_t saturated;
    uint8_t slightly_saturated;
    uint8_t slightly_dry;
    uint8_t dry; 
    uint8_t low_automatic;
    uint8_t high_automatic;
    uint8_t manual;
   
}APP_THRESHOLD_t;

extern APP_THRESHOLD_t APP_threshold;


typedef struct
{
    enum APP_STATES state;
    uint8_t humidity;
    time_t time;
    GPSPosition_t position;
    bool position_validity;
}APP_var_t;

extern APP_var_t APP_info;

extern APP_var_t APP_logBuffer[APP_LOG_BUFFER_SIZE];



//Funcion para mostrar el nivel de humedad con los leds RGB:
void APP_RGB_humidity ( uint8_t ADC_linearized );
//Funcion para indicar riego con led A:
void APP_LEDA_irrigate ( uint8_t ADC_humedad);
//Funcion que actualiza el registro historico
void APP_LOG_data ( APP_var_t log_data );
//Funcion para riego a demanda con bot�n A
void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad );
//Funcion que se encarga de todas las operaciones relativas al Modem
void APP_MDM_tasks();
//Funcion que se encarga de todas las operaciones relativas a los LEDs RGB
void APP_RGB_tasks();
//Funcion que obtiene info del potenciometro y prende los leds segun corresponda
void APP_pot2RGB( uint8_t p_humidity );
void APP_UI();

bool APP_init();

void APP_THRESHOLD_initialize ();
void APP_THRESHOLD_set ( APP_THRESHOLD_t p_threshold );

#endif


#endif	/* APP_H */

