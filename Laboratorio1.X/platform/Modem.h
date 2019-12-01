#ifndef MODEM_H
#define	MODEM_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define MDM_RX_BUFFER_SIZE 200
#define MDM_TX_BUFFER_SIZE 200

#define MDM_COMMAND_DEFAULT_TIMEOUT 1000

#define MDM_SIM_GET_READY_TIMEOUT 3000

#define MDM_COMMAND_SYNTAX_PARAM_ASK "=?"
#define MDM_COMMAND_SYNTAX_STATUS_ASK "?"


enum MODEM_ESTADO
{
    MODEM_ESTADOS_INIT,
    MODEM_ESTADOS_WAIT,
    MODEM_ESTADOS_CHECK,
    MODEM_ESTADOS_STATUS,
    MODEM_ESTADOS_SEND,
    MODEM_ESTADOS_TIMEOUT_CHECK,
};

typedef enum 
{
    MDM_AT_CMD_NAME_AT,
            
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
            
            
    MDM_COMMAND_SEQ_WAIT,        
} MDM_AT_CMD_NAME_t;

typedef enum
{
    MDM_AT_RESP_NAME_WORKING,
    MDM_AT_RESP_NAME_TIMEOUT,
    MDM_AT_RESP_NAME_UNKNOWN,
    MDM_AT_RESP_NAME_OK,
    MDM_AT_RESP_NAME_ERROR,
            
    MDM_AT_RESP_NAME_GNS_PWR,
    MDM_AT_RESP_NAME_GNS_GET_INF,
            
    MDM_AT_RESP_NAME_GSM_SIM_PIN_NEEDED,
    MDM_AT_RESP_NAME_GSM_SMS_READ,
    MDM_AT_RESP_NAME_GSM_SMS_SENT,
           
    // RESPUESTAS SIN SOLICITUD PREVIA        
    MDM_AT_RESP_NAME_GSM_SIM_PIN_READY,
    MDM_AT_RESP_NAME_GSM_SIM_CALL_READY,
    MDM_AT_RESP_NAME_GSM_SIM_SMS_READY,
    MDM_AT_RESP_NAME_GSM_CALL_INCOMING,
    MDM_AT_RESP_NAME_GSM_SMS_INCOMING,
} MDM_AT_RESP_NAME_t;

typedef enum
{
    MDM_GNS_NMEA_RMC,
    MDM_GNS_NMEA_GGA,
    MDM_GNS_NMEA_GSV,
    MDM_GNS_NMEA_GSA,
} MDM_GNS_NMEA_t;

struct MDM_GNS_INFO
{
    bool pwr;
    uint8_t urc;
    MDM_GNS_NMEA_t nmea;
    uint8_t status;
    uint32_t lat;
    uint32_t lon;
    struct tm time;
} gMDM_gnsInfo;


bool MDM_Init(void);
void MDM_read( uint8_t* p_string );
uint8_t* MDM_readString();
uint8_t MDM_write(uint8_t *p_string);
uint8_t* MDM_whatsInReadBuffer();
void MDM_sendATCmd( uint8_t* p_cmd, uint8_t* p_param );
MDM_AT_RESP_NAME_t MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_t p_cmd, uint8_t* p_param, uint32_t p_timeout );
MDM_AT_RESP_NAME_t MDM_responseName(MDM_AT_CMD_NAME_t p_cmd, uint8_t p_index);
uint8_t* MDM_commandString( MDM_AT_CMD_NAME_t p_cmd );
uint8_t* MDM_responseString(MDM_AT_CMD_NAME_t p_cmd, uint8_t p_index);

bool MDM_sendInitialAT();
MDM_AT_RESP_NAME_t MDM_GNSS_getInf( MDM_GNS_NMEA_t p_nmea, bool p_pwr );

#endif
