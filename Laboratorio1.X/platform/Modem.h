#ifndef MODEM_H
#define	MODEM_H

#include <stdint.h>
#include <stdbool.h>

#define MDM_RX_BUFFER_SIZE 128
#define MDM_TX_BUFFER_SIZE 128

#define MDM_COMMAND_DEFAULT_TIMEOUT 1000

//Comandos AT para GPS

//#define MDM_AT_CMD_PWR_ON "AT+CGNSPWR=1"
//#define MDM_AT_CMD_PWR_OFF "AT+CGNSPWR=0"
//#define MDM_AT_CMD_AT "AT"
//#define MDM_AT_CMD_PWR "AT+CGNSPWR="
//#define MDM_AT_CMD_NMAE "AT+CGNSSEQ=RMC"
//#define MDM_AT_CMD_GET_INFO "AT+CGNSINF"
//#define MDM_AT_CMD_REPORTING_OFF "AT+CGNSURC=0"

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
void MDM_sendATCmd( uint8_t* p_cmd, uint8_t* p_param );
MDM_AT_RESP_NAME_t MDM_sendAndWaitResponse( MDM_AT_CMD_NAME_t p_cmd, uint8_t* p_param, uint32_t p_timeout );
MDM_AT_RESP_NAME_t MDM_responseName(MDM_AT_CMD_NAME_t p_cmd, uint8_t p_index);
uint8_t* MDM_commandString( MDM_AT_CMD_NAME_t p_cmd );
uint8_t* MDM_responseString(MDM_AT_CMD_NAME_t p_cmd, uint8_t p_index);

bool MDM_sendInitialAT();

#endif
