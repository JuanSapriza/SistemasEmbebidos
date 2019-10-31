#ifndef MODEM_H
#define	MODEM_H

#include "../utils/Utils.h"
#include "../mcc_generated_files/pin_manager.h"

enum MODEM_ESTADO
{
    MODEM_ESTADOS_INIT,
    MODEM_ESTADOS_WAIT,
    MODEM_ESTADOS_CHECK,
    MODEM_ESTADOS_STATUS,
};

bool MODEM_Init(void);

#endif