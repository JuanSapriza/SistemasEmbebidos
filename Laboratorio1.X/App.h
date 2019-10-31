
#ifndef APP_H
#define	APP_H

#include "mcc_generated_files/rtcc.h"

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

enum APP_STATES
{
    APP_STATE_INIT,
    APP_STATE_PREVIO,
    APP_STATE_INTERMEDIO,
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

#endif	/* APP_H */

