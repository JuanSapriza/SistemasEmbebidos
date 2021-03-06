
#ifndef _RTCC_FWK_H
#define _RTCC_FWK_H

#include "../mcc_generated_files/rtcc.h"

struct tm RTC_time;

enum RTC_TM_ELEMENTS
{
    SECOND,
    MINUTE,
    HOUR,
    MDAY,
    MONTH,
    YEAR,
    WDAY,
    YDAY,
};

enum RTC_GET_TIME_STATE
{
    GET_TIME_INIT,
    GET_TIME_GET_DATE,
    GET_TIME_GET_TIME,
    GET_TIME_CHECK,
};

bool RTC_getUserTime( struct tm* p_time );

#endif