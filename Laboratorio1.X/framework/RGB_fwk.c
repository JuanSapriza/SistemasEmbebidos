

#include "RGB_fwk.h"
#include "../utils/Utils.h"



RGB_DISPLAY_TYPES_t RGB_displayType;
RGB_GO_ROUND_CONFIG_t RGB_goRoundConfig;


void RGB_goRoundSet( RGB_color p_color, uint32_t p_period, uint8_t p_times, uint8_t p_variant )
{
    RGB_goRoundConfig.color = p_color;
    RGB_goRoundConfig.period = p_period;
    RGB_goRoundConfig.times = p_times;
    RGB_goRoundConfig.variant = p_variant;
    RGB_displayType = RGB_DISPLAY_TYPE_GO_ROUND;
}

bool RGB_goRound( RGB_GO_ROUND_CONFIG_t p_config )
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
            RGB_displayType = RGB_DISPLAY_TYPE_GO_ROUND;
            state = GO_ROUND_WAIT;
            break;
            
        case GO_ROUND_WAIT:
            if( RGB_displayType != RGB_DISPLAY_TYPE_GO_ROUND )
            {
                state = GO_ROUND_INIT;
                periodCount = 0;
                return true;
            }
            if( UTS_delayms( UTS_DELAY_HANDLER_GO_ROUND, p_config.period, false  ) )
            {
                state = GO_ROUND_CHANGE;
            }
            break;
            
        case GO_ROUND_CHANGE:
            if( ledCount == RGB_LEDS_COUNT && p_config.variant == GO_ROUND_VARIANT_LOOP )
            {
                if( ++periodCount == p_config.times && p_config.times != 0)
                {
                    state = GO_ROUND_INIT;
                    periodCount = 0;
                    RGB_setAll(OFF);
                    RGB_displayType = RGB_DISPLAY_TYPE_UNDEF;
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
                        RGB_setLed( i, p_config.color );
                    }
                    else
                    {
                        RGB_setLed( i, OFF );
                    }
                }
            }
            else if( ledCount == RGB_LEDS_COUNT )
            {
                switch( p_config.variant  )
                {
                    case GO_ROUND_VARIANT_LAST_ALL_ON:
                        RGB_setAll(p_config.color);
                        break;
                    
                    default:    
                    case GO_ROUND_VARIANT_LAST_ALL_OFF:
                        RGB_setAll(OFF);
                        break;
                }
            }
            state = GO_ROUND_WAIT;
            if( ++ledCount > RGB_LEDS_COUNT )
            {
                if( ++periodCount == p_config.times )
                {
                    state = GO_ROUND_INIT;
                    periodCount = 0;
                    RGB_setAll(OFF);
                    RGB_displayType = RGB_DISPLAY_TYPE_UNDEF;
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
    RGB_displayType = RGB_DISPLAY_TYPE_ALL;
    
}

void RGB_setSingleLed( uint8_t p_led, RGB_color p_color  )
{
    RGB_setLed( p_led, p_color );
    RGB_displayType = RGB_DISPLAY_TYPE_1_BY_1;
}

