
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



#define APP_LOG_BUFFER_SIZE 5

#define NUMERO_VICKY "\"+59891972950\""

#define APP_THRESHOLD_SATURATED_DEFAULT 5
#define APP_THRESHOLD_SLIGHTLY_SATURATED_DEFAULT 9
#define APP_THRESHOLD_SLIGHTLY_DRY_DEFAULT 20
#define APP_THRESHOLD_DRY_DEFAULT 40
#define APP_THRESHOLD_LOW_AUTOMATIC_DEFAULT 15
#define APP_THRESHOLD_HIGH_AUTOMATIC_DEFAULT 30
#define APP_THRESHOLD_MANUAL_DEFAULT 15

#define APP_PLANT_ID_MAX_NUM 9999


#define APP_HUMIDITY_MAX_NUM 60
#define APP_HUMIDITY_MIN_NUM 1

#define APP_SHORT_STRING_SIZE 30

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


//struct APP_info_t
//{
//    enum APP_STATES state;
//    uint8_t humidity;
//    time_t time;
//    GPSPosition_t position;
//    bool position_validity;
//}


typedef struct
{
    enum APP_STATES state;
    uint8_t humidity;
    struct tm* time;
    GPSPosition_t position;
}APP_var_t;

extern APP_var_t APP_info;

extern APP_var_t APP_logBuffer[APP_LOG_BUFFER_SIZE];




//Funcion para mostrar el nivel de humedad con los leds RGB:
void APP_RGB_humidity ( uint8_t ADC_linearized );
//Funcion para indicar riego con led A:
void APP_LEDA_irrigate ( uint8_t ADC_humedad);
//Funcion que actualiza el registro historico
void APP_LOG_data ( APP_var_t* log_data );
//Funcion para riego a demanda con botón A
void APP_BTNA_manual_irrigate ( uint8_t ADC_humedad );
//Funcion para obtener el valor del indice que indica el head del buffer
uint32_t APP_LOG_BUFFER_HEAD_GetValue ( void );
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
void APP_LOG_data ( APP_var_t* log_data );
//Funcion para riego a demanda con botón A
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
    APP_UI_STATE_PRINT_HEADER,
    APP_UI_STATE_MENU_FUNCTIONS,
    

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

enum APP_GET_NEW_ID
{
    APP_GET_NEW_ID_SHOW,
    APP_GET_NEW_ID_WAIT,
    APP_GET_NEW_ID_VALIDATE,
    APP_GET_NEW_ID_RESPONSE_OK,
    APP_GET_NEW_ID_RESPONSE_ERROR,
};

//---------------------------
//Para setear umbrales:

enum APP_SET_THRESHOLDS
{
    APP_SET_THRESHOLDS_INIT,
    APP_SET_THRESHOLDS_MENU,
    APP_SET_THRESHOLDS_HEADERS,
    APP_SET_THRESHOLDS_SHOW,
    APP_SET_THRESHOLDS_FUNCTIONS,
//    APP_SET_THRESHOLDS_WAIT,
//    APP_SET_THRESHOLDS_VALIDATE,
//    APP_SET_THRESHOLDS_RESPONSE_OK,
//    APP_SET_THRESHOLDS_RESPONSE_ERROR,
};

enum APP_SET_NEW_THRESHOLD
{
    APP_SET_NEW_THRESHOLD_SHOW,
    APP_SET_NEW_THRESHOLD_WAIT,
    APP_SET_NEW_THRESHOLD_VALIDATE,
    APP_SET_NEW_THRESHOLD_RESPONSE_OK,
    APP_SET_NEW_THRESHOLD_RESPONSE_ERROR,
};            
            
typedef enum 
{
    APP_THRESHOLD_UNDEF,
    APP_THRESHOLD_SATURATED,
    APP_THRESHOLD_SLIGHTLY_SATURATED,
    APP_THRESHOLD_SLIGHTLY_DRY,
    APP_THRESHOLD_DRY,
    APP_THRESHOLD_AUTO_LOW,
    APP_THRESHOLD_AUTO_HIGH,
    APP_THRESHOLD_MANUAL,
}APP_THRESHOLD_NAMES_t;

//---------------------------

typedef enum
{
    APP_FUNC_WORKING = 0,
    APP_FUNC_DONE,
    APP_FUNC_ERROR,
    APP_FUNC_RETURN,
} APP_FUNC_STATUS_t;

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


typedef struct
{
    enum APP_STATES state;
    uint8_t humidity;
    time_t time;
    GPSPosition_t position;
    bool position_validity;
    uint16_t plantID;
    uint32_t humidity_sampling_time;
    uint32_t log_sampling_time;
    APP_THRESHOLD_t thresholds;
    
}APP_var_t;

extern APP_var_t APP_info;

extern APP_var_t APP_logBuffer[APP_LOG_BUFFER_SIZE];


//Funcion para mostrar el nivel de humedad con los leds RGB:
void APP_RGB_humidity ( uint8_t ADC_linearized );
//Funcion para indicar riego con led A:
void APP_LEDA_irrigate ( uint8_t ADC_humedad);
//Funcion que actualiza el registro historico
void APP_LOG_data ( APP_var_t* log_data );
//Funcion para riego a demanda con botón A
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

