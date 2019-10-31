#ifndef MODEM_H
#define	MODEM_H

#include <stdint.h>
#include <stdbool.h>

#define MDM_RX_BUFFER_SIZE 128

//Comandos AT para GPS

#define MDM_AT_CMD_PWR_ON "AT+CGNSPWR=1"
#define MDM_AT_CMD_PWR_OFF "AT+CGNSPWR=0"
#define MDM_AT_CMD_NMAE "AT+CGNSSEQ=RMC"
#define MDM_AT_CMD_GET_INFO "AT+CGNSINF"
#define MDM_AT_CMD_REPORTING_OFF "AT+CGNSURC=0"

enum MODEM_ESTADO
{
    MODEM_ESTADOS_INIT,
    MODEM_ESTADOS_WAIT,
    MODEM_ESTADOS_CHECK,
    MODEM_ESTADOS_STATUS,
};


uint8_t MDM_rxBuffer[ MDM_RX_BUFFER_SIZE ];

void MDM_read( uint8_t* p_string );
uint8_t* MDM_readString();

bool MDM_Init(void);
uint8_t MDM_write(uint8_t *p_string);

#endif
