#ifndef MODEM_H
#define	MODEM_H

#include "../utils/Utils.h"
#include "../mcc_generated_files/pin_manager.h"


#define MDM_RX_BUFFER_SIZE 128

enum MODEM_ESTADO
{
    MODEM_ESTADOS_INIT,
    MODEM_ESTADOS_WAIT,
    MODEM_ESTADOS_CHECK,
    MODEM_ESTADOS_STATUS,
};


uint8_t MDM_rxBuffer[ MDM_RX_BUFFER_SIZE ];


bool MODEM_Init(void);
void MDM_read( uint8_t* p_string );
uint8_t* MDM_readString();



#endif