#ifndef MODEM_H
#define	MODEM_H


enum MODEM_ESTADO
{
    MODEM_ESTADOS_INIT,
    MODEM_ESTADOS_WAIT,
    MODEM_ESTADOS_CHECK,
    MODEM_ESTADOS_STATUS,
};

bool MDM_Init(void);
uint8_t MDM_write(uint8_t *p_string);

#endif