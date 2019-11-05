/* ************************************************************************** */
/** WS2812

  @Company
    BQN

  @File Name
    WS2812.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "RGB.h"
#include <string.h>
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/interrupt_manager.h"
#include "../utils/Utils.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
RGB_color WHITE     = {25, 25, 25};
RGB_color RED       = {25, 0, 0};
RGB_color GREEN     = {0, 25, 0};
RGB_color BLUE      = {0, 0, 25};
RGB_color VIOLET    = {27, 10, 36};
RGB_color CYAN      = {32, 91, 92};
RGB_color YELLOW    = {21, 20, 6};
RGB_color OFF       = {00, 00, 00};

bool RGB_newConfig = false;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */

/** 
  @Function
    static uint32_t WS2812_bitFlip( uint8_t p_b )

  @Summary
    Invierte el orden de los bits en un byte.

  @Description
    Invierte el orden de los bits en un byte.
    <p>
    Se utilizan rotaciones para ser m�s eficiente
    <p>

  @Precondition
    None

  @Parameters
    @param p_b Byte a ser modificado.

  @Returns
    uint32_t Devuelve el valor del byte en 32 bits

  @Remarks
    None
 */

static uint32_t RGB_bitFlip( uint8_t p_b )
{
    p_b = (p_b & 0xF0) >> 4 | (p_b & 0x0F) << 4;
    p_b = (p_b & 0xCC) >> 2 | (p_b & 0x33) << 2;
    p_b = (p_b & 0xAA) >> 1 | (p_b & 0x55) << 1;
    return (uint32_t)p_b;
}

static void RGB_sendSingle( RGB_color* p_led )
{
    uint8_t j;
    uint32_t val;
    // the WS2812 wants bits in the order of:
    // GGGGGGGGRRRRRRRRBBBBBBBB
    // but I want to work in the opposite order. so i'm going to flip
    // the bits around and do some shifting so my order is
    // BBBBBBBBRRRRRRRRGGGGGGGG
    // with the most significant bit on the far right. so the RGB value
    // of 128 64 32, which normally would be:
    // R : 0b10000000
    // G : 0b01000000
    // B : 0b00100000
    // will become:
    // BBBBBBBBRRRRRRRRGGGGGGGG
    // 000001000000000100000010
    val = (RGB_bitFlip(p_led->b) << 16) + (RGB_bitFlip(p_led->r) << 8) + (RGB_bitFlip(p_led->g));

    // now begin shifting them over one at a time
    for( j = 0; j < 24; j++ )
    {
        // depending on if the currently viewed bit is 1 or 0
        // the pin will stay high for different times
        if(val&1){
            // if it is a 1, let it stay higher a bit longer
            LED_CTRL_SetHigh( ); // 0.90 us
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            LED_CTRL_SetLow( ); // 0.35 us
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
        }else{
            // but a 0 should go high and then low as fast as possible
            LED_CTRL_SetHigh( ); // 0.35 us
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            LED_CTRL_SetLow( ); // 0.90 us
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
            _nop( );
        }

        // and then right shift to get the next bit
        val=val>>1;
    }
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
void RGB_send( RGB_color *p_leds, uint8_t p_length )
{
    uint8_t i;
    INTERRUPT_GlobalDisable();
    
    //WS2812_sendMultiple( p_leds, p_length );
    for( i=0; i<p_length; i++ )
    {
        RGB_sendSingle( (p_leds+i) );
    }
    
    INTERRUPT_GlobalEnable();
    while(!UTS_delayms( UTS_DELAY_HANDLER_RGB_SEND, 1, false ));
}


void RGB_setLed( uint8_t p_led, RGB_color p_color )
{
    if( p_led >= RGB_LEDS_COUNT ) return;
    RGB_leds[ p_led ] = p_color;  
    RGB_newConfig = true;
}

void RGB_tasks()
{
    if( RGB_newConfig )
    {
        RGB_send( RGB_leds, RGB_LEDS_COUNT ); 
        RGB_newConfig = false;    
    }
}