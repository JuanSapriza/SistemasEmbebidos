//<editor-fold defaultstate="collapsed" desc="Includes">



#include "App.h"
#include <string.h>
#include <stdio.h>
#include "framework/USB_fwk.h"
#include "platform/Buttons.h"
#include "platform/Modem.h"
#include "utils/Utils.h"

//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="Defines">

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Data">


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Local Functions">

static void MAIN_init()
{
    SYSTEM_Initialize();
    RGB_setLed(0,OFF);
    RGB_setLed(1,OFF);
    RGB_setLed(2,OFF);
    RGB_setLed(3,OFF);
    RGB_setLed(4,OFF);
    RGB_setLed(5,OFF);
    RGB_setLed(6,OFF);
    RGB_setLed(7,OFF);
    GPRS_PWR_SetDigitalInput();
    GPRS_RESET_SetHigh();
    ADC1_ChannelSelect( ADC1_POT );
    
//    RGB_tasks();
   
}


//<editor-fold defaultstate="collapsed" desc="Main App Lab 3_5">
#ifdef LABORATORIO_3_5
void MAIN_app()
{

    switch( APP_info.state )
    {
        case APP_STATE_INIT:
            if( RGB_goRound( WHITE, 50, 3, GO_ROUND_VARIANT_LOOP ) )
            {
                USB_write("Bienvenido!\n");
                USB_write("Presione una tecla \n");
                APP_info.time = &RTC_time;
                APP_info.state = APP_STATE_WAIT;
            }
            break; 
            
        case APP_STATE_WAIT:
//            if( RTC_getUserTime( APP_info.time ) )
//            {
//                sprintf(USB_dummyBuffer,">>%02d - %02d - %04d \n",APP_info.time->tm_mday,APP_info.time->tm_mon, APP_info.time->tm_year);
//                USB_write( USB_dummyBuffer );
//                APP_info.state = 3;
//            }
            if( *USB_read(0) != 0 )
            {
                APP_info.state = APP_STATE_MAIN_MENU_CREATE;
            }
            break;
        
        case APP_STATE_MAIN_MENU_CREATE:
            UTS_addTitle2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Menu Principal. ¿Qué LED prendo?" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Led 1 Rojo" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Led 2 Verde" );
            UTS_addOption2Menu( UTS_MENU_HANDLER_MENU_PRINCIPAL, "Led 3 Azul" );
            APP_info.state = APP_STATE_MAIN_MENU_SHOW;
            break;
            
        case APP_STATE_MAIN_MENU_SHOW:
            switch( USB_showMenuAndGetAnswer(UTS_MENU_HANDLER_MENU_PRINCIPAL ) )
            {
                case 1:
                    RGB_setAll(OFF);
                    RGB_setLed(1,RED);
                    break;
                    
                case 2: 
                    RGB_setAll(OFF);
                    RGB_setLed(2,GREEN);
                    break;
                    
                case 3:
                    RGB_setAll(OFF);
                    RGB_setLed(3,BLUE);
                    break;
                    
                case -1:
                    RGB_setAll(OFF);
                    break;
                    
                default: break;
            
            }
            break;
            
            
            
            
        default: break;
    
    
    }
    
    
    

}
#endif
//</editor-fold>



//</editor-fold>
 


//<editor-fold defaultstate="collapsed" desc="Laboratorio 1">

#ifdef LABORATORIO_1
int main ()
{
    LEDA_SetDigitalOutput();
    LEDB_SetDigitalOutput();

    LEDA_setHigh();
    LEDB_setLow();
    
    while( 1 )
    {
        LEDA_toggle();
        LEDB_toggle();
        UTS_delayms();
    }
    
    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 2">
#ifdef LABORATORIO_2
int main ()
{
    MAIN_init();
    
    bool btnAWasPressed = false;
    bool btnBWasPressed = false;
      
    LEDA_setLow();
    LEDB_setLow();
    
    while( 1 )
    {
        if( BTN_isButtonPressed( BTN_BUTTON_A ) && !btnAWasPressed )
        {
            btnAWasPressed = true;
        }
        else if( !BTN_isButtonPressed( BTN_BUTTON_A ) && btnAWasPressed )
        {
            btnAWasPressed = false;
            LEDA_toggle();
        }
             
        if( BTN_isButtonPressed( BTN_BUTTON_B ) && !btnBWasPressed )
        {
            btnBWasPressed = true;
        }
        else if( !BTN_isButtonPressed( BTN_BUTTON_B ) && btnBWasPressed )
        {
            btnBWasPressed = false;
            LEDB_toggle();
        }
        
        while( ! UTS_delayms( 100, false ) )
        {           
        }
        
    }
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 2_1">
#ifdef LABORATORIO_2_1
int main ()
{
    MAIN_init();
       
    bool btnAWasPressed = false;
    bool btnBWasPressed = false;
      
    LED_A_SetLow();
    LED_B_SetHigh();
    
    while( 1 )
    {
        if( BTN_isButtonPressed( BTN_BUTTON_A ) )
        {
            btnAWasPressed = true;
        }
        
        if( btnAWasPressed ) 
        {
            if( UTS_delayms( 2000, false ) )
            {
                LED_A_SetHigh();
            }
        }
        
    }
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3">
#ifdef LABORATORIO_3
int main ()
{
    MAIN_init();
    
    while( 1 )
    {
       MCC_USB_CDC_DemoTasks();
    }
    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_2">
#ifdef LABORATORIO_3_2
int main ()
{
    
    MAIN_init();
 
    while(1)
    {  
        //echo USB
       if( USB_CDC_tasks() )
       {
           USB_write( USB_read( 0 ) );
       }
       
    }    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_3">
#ifdef LABORATORIO_3_3
int main ()
{
    
    MAIN_init();
 
    memset( RGB_leds, 0, sizeof(RGB_leds));
//    RGB_leds[0] = YELLOW;
//    RGB_leds[1] = VIOLET;
//    RGB_leds[2] = RED;
//    RGB_leds[3] = WHITE;
//    RGB_leds[4] = GREEN;
//    RGB_leds[5] = BLUE;
//    RGB_leds[6] = RED;
//    RGB_leds[7] = CYAN;
    RGB_setLed(2,CYAN);
    while(1)
    {  
//        RGB_send( RGB_leds, RGB_LEDS_COUNT );
       
    }    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_4">
#ifdef LABORATORIO_3_4
    bool led_1 = false;
    bool led_2 = false;
int main ()
{
    UTS_DELAY_HANDLER_t LED_1_delay = UTS_DELAY_HANDLER_1;
    UTS_DELAY_HANDLER_t LED_2_delay = UTS_DELAY_HANDLER_2;
   
    MAIN_init();
    UTS_delayms( LED_1_delay , 0, true );
    UTS_delayms( LED_2_delay , 0, true );
 
    while(1)
    {  
        
        if( UTS_delayms( LED_1_delay, 1000, false ) )
        {
            if( led_1 )
            {
                RGB_setLed(2,WHITE);
                USB_write("hola \n");
                led_1 = false;
            }
            else
            {
                USB_write("chau \n");
                RGB_setLed(2,BLUE);
                led_1 = true;
            }
        }
        
        if( UTS_delayms( LED_2_delay, 1200, false ) )
        {
            if( led_2 )
            {
                RGB_setLed(7,VIOLET);
                USB_write("mundo \n");
                led_2 = false;
            }
            else
            {
                RGB_setLed(7,RED);
                USB_write("pajarito \n");
                led_2 = true;
            }    
        }
        
        USB_CDC_tasks();        
        RGB_tasks();

    }    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 3_5">
#ifdef LABORATORIO_3_5


int main ()
{
    MAIN_init();
    while(1)
    {
        MAIN_app();
        USB_CDC_tasks();
        RGB_tasks();
    }
    
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 4">
#ifdef LABORATORIO_4

int main ()
{
//    uint8_t dummyBuffer[ 64 ];
    uint16_t datos_potenciometro;
    uint16_t *p_datos_potenciometro;  
    //uint16_t aux3=0;
    
    uint8_t aux3=0;
    APP_var_t data_to_log;
    p_datos_potenciometro=&datos_potenciometro;
    MAIN_init();
    
    while (1)
        
    {
        
        if( UTS_DELAY_HANDLER_SENSE_HUMIDITY, 5000, false )
        {
            if(POT_Convert( p_datos_potenciometro ))
            {
                Nop();
                aux3 = POT_Linearized ( *p_datos_potenciometro );
                APP_RGB_humidity ( aux3 );
                Nop();
                APP_LEDA_irrigate ( aux3 );
                APP_BTNA_manual_irrigate ( aux3 );
            }
        }
        
        if( UTS_delayms( UTS_DELAY_HANDLER_LOG_DATA, 10000, false ) )
        {
            Nop();
            data_to_log.humidity=aux3;
            data_to_log.time=APP_info.time;
            data_to_log.position.latitude=APP_info.position.latitude;
            data_to_log.position.longitude=APP_info.position.longitude;
            data_to_log.position_validity=APP_info.position_validity;
            APP_LOG_data ( data_to_log );
            
            Nop();            
            sprintf(USB_dummyBuffer,"el nivel de humedad es %d \n",APP_logBuffer[0]);
            USB_write(USB_dummyBuffer);
            Nop();
            sprintf(USB_dummyBuffer,"el nivel de humedad es %d \n",APP_logBuffer[1]);
            USB_write(USB_dummyBuffer);
            Nop();
            sprintf(USB_dummyBuffer,"el nivel de humedad es %d \n\n\n",APP_logBuffer[2]);
            USB_write(USB_dummyBuffer);
        }
        
        
        
        USB_CDC_tasks();
        RGB_tasks();
        
    }    
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Laboratorio 4_2">
#ifdef LABORATORIO_4_2

int main ()
{
    uint8_t dummyBuffer[ 64 ];
    MAIN_init();
    APP_info.state = APP_STATE_INIT;
    
    while(1)
    {
        if( !BTN_switch( BTN_BUTTON_B ) )
        {
            switch( APP_info.state )
            {
                case APP_STATE_INIT:
                    if( MDM_Init() )
                    {
                        UTS_ledBlink( 500, 500 );
                        if( MDM_sendInitialAT() )
                        {
                            RGB_setLed( 7, WHITE);
                            APP_info.state = APP_STATE_GPS_GET;
                        }
                    }
                    break;

                case APP_STATE_GPS_GET:
                    switch( MDM_GNSS_getInf( MDM_GNS_NMEA_RMC, true ) )
                    {
                        case MDM_AT_RESP_NAME_GNS_GET_INF:
                            RGB_setLed( 2, GREEN );
                            APP_info.state = APP_STATE_WAIT;
                            break;
                            
                        case MDM_AT_RESP_NAME_ERROR:
                            RGB_setLed( 3, RED );
                            APP_info.state = APP_STATE_WAIT;
                            break;
                            
                            case MDM_AT_RESP_NAME_WORKING:
                            RGB_setLed( 2, BLUE );
                            break;
                            
                        default:
                            break;
                    }
                    break;
                    
               case APP_STATE_PARSE_FRAME:
                   GPS_parseFrame( MDM_whatsInReadBuffer(), *, *APP_info.position );
                   *APP_info.time = 
                   //hacer algo con esta nueva información
                   break; 
                    
              case APP_STATE_WAIT: 
                  RGB_setLed( 2, BLUE );
                  if( UTS_delayms(UTS_DELAY_HANDLER_DUMMY_1, 2000, false ) )
                  {
                      USB_write( MDM_whatsInReadBuffer() );
                      APP_info.state = APP_STATE_GPS_GET;
                  }
                  break;

            }
            RGB_tasks();
            USB_CDC_tasks();
        }
    }
    return 0;
}
#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="PRUEBAS GSM">
#ifdef PRUEBAS_GSM

int main ()
{
    struct tm p_time_aux;
    uint8_t* index = 0;
    uint8_t dummyBuffer[ 64 ];
    MAIN_init();
    APP_info.state = APP_STATE_INIT;
    
    USB_sniffSetType( USB_SNIFF_TYPE_BOTH );
    
    while(1)
    {
        if( !BTN_switch( BTN_BUTTON_B ) )
        {

        // FUNCIONES ESPECÍFICAS DE LA APLICACIÓN
            APP_MDM_tasks();
            APP_RGB_tasks();
            
            
        // MÁQUINA DE ESTADOS PRINCIPAL ( Solo testing )
            switch( APP_info.state )
            {
                case APP_STATE_INIT:
                    break;

                case APP_STATE_GPS_GET:
                    //USB_send2Modem();
                    switch( MDM_GNSS_getInf( MDM_GNS_NMEA_RMC, true ) )
                    {
                        case MDM_AT_RESP_NAME_GNS_GET_INF:
                            RGB_setLed( 2, GREEN );
                            APP_info.state = APP_STATE_PARSE_FRAME;
                            break;
                            
                        case MDM_AT_RESP_NAME_ERROR:
                            RGB_setLed( 3, RED );
                            APP_info.state = APP_STATE_WAIT;
                            break;
                            
                            case MDM_AT_RESP_NAME_WORKING:
                            RGB_setLed( 2, BLUE );
                            break;
                            
                        default:
                            break;
                    }
                    break;
                    
               
                    
               case APP_STATE_PARSE_FRAME:
                   
                   
                   
  
                   index = strstr( MDM_whatsInReadBuffer(), MDM_responseString( MDM_AT_CMD_NAME_GNS_GET_INFO, 1 ) );  
                   //  voy hasta el final de +CGNSINF: 
                   index += strlen( MDM_responseString( MDM_AT_CMD_NAME_GNS_GET_INFO, 1 ) +3);                 
                   
                   if( *index == 0x30 )
                   {
                       APP_info.position_validity = false;
                   }
                   
                   else
                   {
                        GPS_parseFrame( MDM_whatsInReadBuffer(), &p_time_aux, &APP_info.position );
                        APP_info.time = mktime( &p_time_aux );
                        
                        sprintf(USB_dummyBuffer,"tiempo time_t %d \n",APP_info.time);
                        USB_write(USB_dummyBuffer);
                        APP_info.state = APP_STATE_WAIT;
                        sprintf(USB_dummyBuffer,"estado %d \n",APP_info.state);
                        USB_write(USB_dummyBuffer);
                        APP_info.position_validity = true;
                        sprintf(USB_dummyBuffer,"validez %d \n",APP_info.position_validity);
                        USB_write(USB_dummyBuffer);
                        sprintf(USB_dummyBuffer,"latitud %f \n",APP_info.position.latitude);
                        USB_write(USB_dummyBuffer);
                        sprintf(USB_dummyBuffer,"longitud %f \n",APP_info.position.longitude);
                        USB_write(USB_dummyBuffer);
                        RGB_setLed( 4, VIOLET );
                    
                   }
                   
                   break;
                    
              case APP_STATE_WAIT: 
                  RGB_setLed( 2, BLUE );
                  if( UTS_delayms(UTS_DELAY_HANDLER_DUMMY_1, 5000, false ) )
                  {
//                      USB_write( MDM_whatsInReadBuffer() );
                      APP_info.state = APP_STATE_GPS_GET;
                  }
                  break;
                    
            }
            
            
            
            
            RGB_tasks();
            USB_CDC_tasks();
            
        }
    }
    return 0;
}

#endif
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="PROYECTO 1">
#ifdef PROYECTO_1

int main ()
{
    struct tm p_time_aux;
    uint8_t* index = 0;
    uint8_t dummyBuffer[ 64 ];
    MAIN_init();
    APP_info.state = APP_STATE_INIT;
    
    while(1)
    {
        if( !BTN_switch( BTN_BUTTON_B ) )
        {
            switch( APP_info.state )
            {
                case APP_STATE_INIT:
                    if( MDM_Init() )
                    {
                        UTS_ledBlink( 500, 500 );
                        if( MDM_sendInitialAT() )
                        {
                            RGB_setLed( 7, WHITE);
                            APP_info.state = APP_STATE_GPS_GET;
                        }
                    }
                    break;

                case APP_STATE_GPS_GET:
                    //USB_send2Modem();
                    switch( MDM_GNSS_getInf( MDM_GNS_NMEA_RMC, true ) )
                    {
                        case MDM_AT_RESP_NAME_GNS_GET_INF:
                            RGB_setLed( 2, GREEN );
                            APP_info.state = APP_STATE_PARSE_FRAME;
                            break;
                            
                        case MDM_AT_RESP_NAME_ERROR:
                            RGB_setLed( 3, RED );
                            APP_info.state = APP_STATE_WAIT;
                            break;
                            
                            case MDM_AT_RESP_NAME_WORKING:
                            RGB_setLed( 2, BLUE );
                            break;
                            
                        default:
                            break;
                    }
                    break;
                    
               
                    
               case APP_STATE_PARSE_FRAME:
                   
                   
                   
  
                   index = strstr( MDM_whatsInReadBuffer(), MDM_responseString( MDM_AT_CMD_NAME_GNS_GET_INFO, 1 ) );  
                   //  voy hasta el final de +CGNSINF: 
                   index += strlen( MDM_responseString( MDM_AT_CMD_NAME_GNS_GET_INFO, 1 ) +3);                 
                   
                   if( *index == 0x30 )
                   {
                       APP_info.position_validity = false;
                   }
                   
                   else
                   {
                        GPS_parseFrame( MDM_whatsInReadBuffer(), &p_time_aux, &APP_info.position );
                        APP_info.time = mktime( &p_time_aux );
                        
                        sprintf(USB_dummyBuffer,"tiempo time_t %d \n",APP_info.time);
                        USB_write(USB_dummyBuffer);
                        APP_info.state = APP_STATE_WAIT;
                        sprintf(USB_dummyBuffer,"estado %d \n",APP_info.state);
                        USB_write(USB_dummyBuffer);
                        APP_info.position_validity = true;
                        sprintf(USB_dummyBuffer,"validez %d \n",APP_info.position_validity);
                        USB_write(USB_dummyBuffer);
                        sprintf(USB_dummyBuffer,"latitud %f \n",APP_info.position.latitude);
                        USB_write(USB_dummyBuffer);
                        sprintf(USB_dummyBuffer,"longitud %f \n",APP_info.position.longitude);
                        USB_write(USB_dummyBuffer);
                        RGB_setLed( 4, VIOLET );
                    
                   }
                   
                   break;
                    
              case APP_STATE_WAIT: 
                  RGB_setLed( 2, BLUE );
                  if( UTS_delayms(UTS_DELAY_HANDLER_DUMMY_1, 5000, false ) )
                  {
//                      USB_write( MDM_whatsInReadBuffer() );
                      APP_info.state = APP_STATE_GPS_GET;
                  }
                  break;
                    
            }
            
            
            
            
            RGB_tasks();
            USB_CDC_tasks();
        }
    }
    return 0;
}

#endif
//</editor-fold>


