

#include "RGB_fwk.h"
#include "../utils/Utils.h"


bool RGB_goRound( RGB_color p_color, uint32_t p_period, uint8_t p_times, uint8_t p_variant )
{
    static uint8_t ledCount = 0;
    static uint8_t periodCount = 0;
    static enum GO_ROUND_STATES state = GO_ROUND_INIT;
    uint8_t i;
    
    switch(state)
    {
        case GO_ROUND_INIT:
            RGB_setAll(OFF);
            ledCount = 0;
            state = GO_ROUND_WAIT;
            break;
            
        case GO_ROUND_WAIT:
            if( UTS_delayms( UTS_DELAY_HANDLER_GO_ROUND, p_period, false  ) )
            {
                state = GO_ROUND_CHANGE;
            }
            break;
            
        case GO_ROUND_CHANGE:
            if( ledCount == RGB_LEDS_COUNT && p_variant == GO_ROUND_VARIANT_LOOP )
            {
                if( ++periodCount == p_times )
                {
                    state = GO_ROUND_INIT;
                    periodCount = 0;
                    RGB_setAll(OFF);
                    return true;
                }
                ledCount = 0;
            }
            if( ledCount < RGB_LEDS_COUNT )
            {
                for( i = 0; i < RGB_LEDS_COUNT; i++ )
                {
                    if( i == ledCount )
                    {
                        RGB_setLed( i, p_color );
                    }
                    else
                    {
                        RGB_setLed( i, OFF );
                    }
                }
            }
            else if( ledCount == RGB_LEDS_COUNT )
            {
                switch( p_variant )
                {
                    default:    
                    case GO_ROUND_VARIANT_LAST_ALL_OFF:
                        RGB_setAll(OFF);
                        break;
                        
                    case GO_ROUND_VARIANT_LAST_ALL_ON:
                        RGB_setAll(p_color);
                        break;
                      
                }
            }
            state = GO_ROUND_WAIT;
            if( ++ledCount > RGB_LEDS_COUNT )
            {
                if( ++periodCount == p_times )
                {
                    state = GO_ROUND_INIT;
                    periodCount = 0;
                    RGB_setAll(OFF);
                    return true;
                }
                ledCount = 0;
            }
            break;
            
        default: break;
            
    }
    return false;
}


void RGB_setAll( RGB_color p_color)
{
    RGB_setLed( 0, p_color );
    RGB_setLed( 1, p_color );
    RGB_setLed( 2, p_color );
    RGB_setLed( 3, p_color );
    RGB_setLed( 4, p_color );
    RGB_setLed( 5, p_color );
    RGB_setLed( 6, p_color );
    RGB_setLed( 7, p_color );
}