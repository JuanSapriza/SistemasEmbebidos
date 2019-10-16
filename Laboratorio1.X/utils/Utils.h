
#ifndef _UTILS_H
#define _UTILS_H

//#include "../platform/Timer.h"
#include <stdint.h>
#include <stdbool.h>

#define UTS_DELAY_CYCLES 1000
#define MAX_NUM_16_BITS     65535

enum UTS_DELAY_ESTADOS
{
    UTS_DELAY_ESTADOS_INIT,
    UTS_DELAY_ESTADOS_CHECK,
};



typedef enum UTS_DELAY_HANDLER
{
    UTS_DELAY_HANDLER_1,
    UTS_DELAY_HANDLER_2,
    UTS_DELAY_HANDLER_3,
    UTS_DELAY_HANDLER_4,
    UTS_DELAY_HANDLER_5,
    UTS_DELAY_HANDLER_COUNT,
}UTS_DELAY_HANDLER_t;


typedef struct UTS_DELAY_HANDLER_TYPE
{
    uint32_t initialTime;
    uint32_t countTime;
    bool active;
}UTS_delayHandler_t;

UTS_delayHandler_t UTS_delayHandler[UTS_DELAY_HANDLER_COUNT];





bool UTS_delayms( uint8_t p_handlerIndex, uint32_t p_tiempo, bool p_reiniciar );
//bool UTS_delayms( uint32_t p_tiempo, bool p_reiniciar );









#endif