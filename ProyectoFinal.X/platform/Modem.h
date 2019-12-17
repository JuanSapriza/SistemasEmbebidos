#ifndef MODEM_H
#define	MODEM_H

//<editor-fold defaultstate="collapsed" desc="Includes">

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Defines">

#define MDM_RX_BUFFER_SIZE 200
#define MDM_TX_BUFFER_SIZE 200

#define MDM_AT_CMD_HEADER_LENGTH 20
#define MDM_AT_RESP_HEADER_LENGTH 20


#define MDM_COMMAND_DEFAULT_TIMEOUT 2000
#define MDM_COMMAND_SUPERLONG_TIMEOUT 5000
#define MDM_COMMAND_MEGALONG_TIMEOUT 10000

#define MDM_SIM_MAX_RETRIES 5

#define MDM_SMS_WAIT_TIME 1000
#define MDM_SMS_LENGTH 100
#define MDM_SMS_PHONE_NUM_LENGTH 12
#define MDM_SIM_PIN_SIZE 4

#define MDM_GPS_MAX_RETRIES 5

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enums">

enum MDM_ESTADO
{
    MDM_ESTADOS_INIT,
    MDM_ESTADOS_WAIT,
    MDM_ESTADOS_CHECK,
    MDM_ESTADOS_STATUS,
    MDM_ESTADOS_SEND,
    MDM_ESTADOS_TIMEOUT_CHECK,
    MDM_ESTADOS_EXECUTE,
};

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Data Types">

//<editor-fold defaultstate="collapsed" desc="Tasks">

        // NOMBRE DE TAREAS
typedef enum
{
    MDM_TASK_CONFIG,
    MDM_TASK_GET_GPS_FRAME,
    MDM_TASK_SEND_SMS,
    MDM_TASK_READ_SMS,
    MDM_TASK_COUNT,
    MDM_TASK_UNDEF,
} MDM_TASK_NAME_t;

        // ESTADOS DE UNA TAREA
typedef enum
{
    MDM_TASK_STATUS_UNDEF,
    MDM_TASK_STATUS_DONE,
    MDM_TASK_STATUS_WORKING,
    MDM_TASK_STATUS_NEW,
    MDM_TASK_STATUS_ERROR,
}MDM_TASK_STATUS_t;

        // ESTRUCTURA DE UNA TAREA
typedef struct
{
    MDM_TASK_STATUS_t status;
    void* ptr;
} MDM_TASK_ELEMENT_t;

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Commands">


        // NOMBRES DE COMANDOS
typedef enum 
{
    MDM_AT_CMD_NAME_AT,
    MDM_AT_CMD_NAME_ECHO_OFF,
    MDM_AT_CMD_NAME_ECHO_ON,
            
    MDM_AT_CMD_NAME_GNS_PWR,
    MDM_AT_CMD_NAME_GNS_NMEA,
    MDM_AT_CMD_NAME_GNS_GET_INFO,
    MDM_AT_CMD_NAME_GNS_URC,
            
    MDM_AT_CMD_NAME_GSM_FUNCTIONALITY,
    MDM_AT_CMD_NAME_GSM_SIM_PIN_ASK,
    MDM_AT_CMD_NAME_GSM_SIM_PIN_SET,
    MDM_AT_CMD_NAME_GSM_SMS_FORMAT,
    MDM_AT_CMD_NAME_GSM_SMS_SEND_HEADER,
    MDM_AT_CMD_NAME_GSM_SMS_SEND_FOOTER,
    MDM_AT_CMD_NAME_GSM_SMS_READ,
    MDM_AT_CMD_NAME_GSM_SMS_LIST,
    MDM_AT_CMD_NAME_GSM_CALL_MAKE,
    MDM_AT_CMD_NAME_GSM_CALL_ANSWER,
    MDM_AT_CMD_NAME_GSM_CALL_HANG,
            
            
    MDM_COMMAND_SEQ_WAIT_4_READY,        
    MDM_COMMAND_SEQ_WAIT_4_RESPONSE,
    MDM_COMMAND_SEQ_WRITE,
    DEBUG,
    
} MDM_AT_CMD_NAME_t;

        // NOMBRES DE RESPUESTAS

typedef enum
{
    MDM_AT_RESP_NAME_WORKING,
    MDM_AT_RESP_NAME_TIMEOUT,
    MDM_AT_RESP_NAME_UNKNOWN,
    MDM_AT_RESP_NAME_OK,
    MDM_AT_RESP_NAME_ERROR,
            
    MDM_AT_RESP_NAME_GNS_PWR,
    MDM_AT_RESP_NAME_GNS_GET_INF,
    MDM_AT_RESP_NAME_GNS_GET_INF_NO_SIGNAL,
            
    MDM_AT_RESP_NAME_GSM_SIM_ERROR,
    MDM_AT_RESP_NAME_GSM_SMS_READ,
    MDM_AT_RESP_NAME_GSM_SMS_SENT,
           
    // RESPUESTAS SIN SOLICITUD PREVIA        
    MDM_AT_RESP_NAME_GSM_SIM_PIN_NEEDED, //tambien llega por solicitud, se pone aca porque a veces llega luego de error
    MDM_AT_RESP_NAME_GSM_SIM_PIN_READY,
    MDM_AT_RESP_NAME_GSM_SIM_CALL_READY,
    MDM_AT_RESP_NAME_GSM_SIM_SMS_READY,
    MDM_AT_RESP_NAME_GSM_CALL_INCOMING,
    MDM_AT_RESP_NAME_GSM_SMS_INCOMING,
} MDM_AT_RESP_NAME_t;


    // NOMBRES DE TIPOS DE TRAMA GNSS
typedef enum
{
    MDM_GNS_NMEA_RMC,
    MDM_GNS_NMEA_GGA,
    MDM_GNS_NMEA_GSV,
    MDM_GNS_NMEA_GSA,
} MDM_GNS_NMEA_t;

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Info">

        // ESTRUCTURA CON INFO DE SMS
typedef struct
{
    uint8_t num[MDM_SMS_PHONE_NUM_LENGTH+3];
    uint8_t text[MDM_SMS_LENGTH];
} MDM_smsInfo_t;

//</editor-fold>

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Functions">

void MDM_tasks();
void MDM_taskSchedule( MDM_TASK_NAME_t p_task, void* p_taskPtr );
void MDM_taskSetStatus( MDM_TASK_NAME_t p_task, MDM_TASK_STATUS_t p_status );
MDM_TASK_STATUS_t MDM_taskGetStatus( MDM_TASK_NAME_t p_task );
bool MDM_Init(void);
uint8_t* MDM_readString();
uint8_t* MDM_whatsInReadBuffer();
uint8_t* MDM_responseString(MDM_AT_CMD_NAME_t p_cmd, uint8_t p_index);
bool MDM_sendInitialAT();
MDM_AT_RESP_NAME_t MDM_GSM_init( uint16_t p_pin );

//</editor-fold>

#endif
