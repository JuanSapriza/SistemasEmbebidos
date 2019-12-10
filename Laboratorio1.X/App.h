
#ifndef APP_H
#define	APP_H

//<editor-fold defaultstate="collapsed" desc="Versión">

#define COMMIT_VERSION VERSION_ESTABLE_1

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
#include "platform/Modem.h"


#define APP_EMERGENCY_NUMBER_DEFAULT "\"+59891972950\""
#define APP_GOOGLE_MAPS_LOCATION_URL_LENGTH 70

#define APP_THRESHOLD_SATURATED_DEFAULT 5
#define APP_THRESHOLD_SLIGHTLY_SATURATED_DEFAULT 25
#define APP_THRESHOLD_SLIGHTLY_DRY_DEFAULT 35
#define APP_THRESHOLD_DRY_DEFAULT 55
#define APP_THRESHOLD_LOW_AUTOMATIC_DEFAULT 15
#define APP_THRESHOLD_HIGH_AUTOMATIC_DEFAULT 30
#define APP_THRESHOLD_MANUAL_DEFAULT 15

#define APP_HUMIDITY_SENSE_PERIOD_DEFAULT 5
#define APP_LOG_REGISTRER_PERIOD_DEFAULT 5000
#define APP_GPS_GET_PERIOD_DEFAULT 5000
#define APP_SMS_ALERT_COOL_DOWN_DEFAULT 10000
#define APP_DISPLAY_HUMIDITY_DEFAULT true
#define APP_LOG_BUFFER_SIZE 60

#define APP_LOG_BUFFER_SIZE_MAX 50000 //Calculado en base a la memoria de datos disponible y un valor razonable de registros (100 años guardando cada 12 hrs o un año guardando cada 10 minutos)

#define APP_PERIOD_IN_SECONDS_MAX 3600000

#define APP_PERIOD_MAX 604800 //Período máximo de 20 días en segundos

#define APP_HUMIDITY_DEFAULT_LEVEL 30

#define APP_4_DIGITS_MAX_NUM 9999
#define APP_DEFAULT_PLANT_ID 1234

#define APP_HUMIDITY_MAX_NUM 60
#define APP_HUMIDITY_MIN_NUM 1

#define APP_SHORT_STRING_SIZE 30
#define APP_SMS_LENGTH 100

#define APP_HUMIDITY_LEVEL_DESCRIPTION_LENGTH 20

#define UYT -3 //zona horaria de Uruguay

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
    APP_INIT_VARS,
    APP_INIT_MDM,
    APP_INIT_MDM_GSM,
};

enum APP_TASKS
{
    APP_TASK_POT,
    APP_TASK_POT_2_RGB,
    APP_TASK_GPS_GET,
    APP_TASK_SMS_SEND,
    APP_TASK_SMS_READ,
    APP_TASK_REGISTER_SAVE,
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
    
    APP_STATE_INIT,
    APP_STATE_TASKS,
    APP_STATE_WAIT,
    APP_STATE_CHECK,
    APP_STATE_CHECK_OK,
    APP_STATE_CHECK_ERROR,
    APP_STATE_GET,
    APP_STATE_SHOW,
    APP_STATE_ERROR_SHOW,
    APP_STATE_COOLDOWN,
    
    APP_STATE_MENU_CREATE,
    APP_STATE_MENU_SHOW,
    APP_STATE_MENU_OPTIONS,
    APP_STATE_GPS_GET,
    APP_STATE_GSM_SMS_INIT,
    APP_STATE_GSM_SMS_SEND,
    APP_STATE_GSM_SMS_GET,
    APP_STATE_PARSE_FRAME,
    APP_STATE_MAIN_MENU_CREATE,
    APP_STATE_MAIN_MENU_SHOW,
    APP_STATE_FINISH,
};

enum state_buffer
{
    STATE_BUFFER_INIT,
    STATE_BUFFER_PRINT,
    STATE_BUFFER_CHECK,
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
    APP_SET_THRESHOLDS_SHOW_HEADER,
    APP_SET_THRESHOLDS_SHOW,
    APP_SET_THRESHOLDS_FUNCTIONS,
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

typedef enum
{
    APP_HUMIDITY_SATURATED,
    APP_HUMIDITY_SLIGHTLY_SATURATED,
    APP_HUMIDITY_OPTIMAL,
    APP_HUMIDITY_SLIGHTLY_DRY,
    APP_HUMIDITY_DRY,
} APP_HUMIDITY_LEVEL_t;

//---------------------------
//Para setear parámetros:

enum APP_SET_PARAMETERS
{
    APP_SET_PARAMETERS_INIT,
    APP_SET_PARAMETERS_MENU,
    APP_SET_PARAMETERS_SHOW,
    APP_SET_PARAMETERS_FUNCTIONS,
};

enum APP_SET_NEW_PARAMETER
{
    APP_SET_NEW_PARAMETER_SHOW,
    APP_SET_NEW_PARAMETER_WAIT,
    APP_SET_NEW_PARAMETER_VALIDATE,
    APP_SET_NEW_PARAMETER_RESPONSE_OK,
    APP_SET_NEW_PARAMETER_RESPONSE_ERROR,
}; 


enum APP_DISPLAY_HUMIDITY
{
    APP_DISPLAY_HUMIDITY_OFF,
    APP_DISPLAY_HUMIDITY_DISCRETE,
    APP_DISPLAY_HUMIDITY_ANALOG,
}; 


typedef enum 
{
    APP_PARAMETER_UNDEF,
    APP_PARAMETER_HUMIDITY_PERIOD,
    APP_PARAMETER_LOG_PERIOD,
    APP_PARAMETER_GPS_PERIOD,
    APP_PARAMETER_DISPLAY_HUMIDITY,
    APP_PARAMETER_SMS_COOL_DOWN,

}APP_PARAMETER_NAMES_t;
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
    uint32_t humiditySensePeriod;
    uint32_t logRegisterPeriod;
    uint32_t gpsGetPeriod;
//    uint32_t SMSalertPeriod;
    uint8_t displayHumidity;
    uint32_t SMSalertCoolDown;
} APP_PARAMS_t;

typedef struct
{
    uint8_t level;
    bool alert;
    bool coolDown;
} APP_HUMIDITY_t;

typedef struct
{
    enum APP_STATES state;
    time_t time;
    bool position_validity;
    uint16_t plantID;
    uint8_t emergencyNum[MDM_SMS_PHONE_NUM_LENGTH];
    uint8_t simPin[MDM_SIM_PIN_SIZE];
    bool GSM_active;
    
    
    GPSPosition_t position;
    APP_HUMIDITY_t humidity;
    APP_PARAMS_t param;
    APP_THRESHOLD_t threshold;
    
}APP_var_t;

typedef struct
{
    uint8_t humidity;
    time_t time;
    GPSPosition_t position;
    bool position_validity;
    uint16_t plantID;  
    uint32_t logNum;
}APP_log_t;

extern APP_var_t APP_info;

extern APP_log_t APP_logBuffer[APP_LOG_BUFFER_SIZE];


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

void APP_UI();

bool APP_init();

void APP_THRESHOLD_initialize ();
//void APP_THRESHOLD_set ( APP_THRESHOLD_t p_threshold );
void APP_PARAM_initialize();
//void APP_PARAM_set ( APP_PARAMS_t p_param );
#endif


#endif	/* APP_H */

