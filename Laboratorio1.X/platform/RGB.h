/* ************************************************************************** */
/** WS2812

  @Company
    BQN

  @File Name
    WS2812.h

  @Summary
    Declaraciones para el manejo de los leds WS2812.

  @Description
    Contiene la estructura que almacena los datos RGB de un led y la declaración
    de la función de escritura en el led.
 */
/* ************************************************************************** */

#ifndef _RGB_H    /* Guard against multiple inclusion */
#define _RGB_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdint.h>





#define RGB_LEDS_COUNT 8


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

/** ws2812_t

  @Summary
    Estructura que contiene el valor para un led WS2812

  @Description
    Estructura que contiene el valor para un led WS2812
    <p>
    Contiene 3 bytes con el valor RGB de cada led.
    <p>

  @Remarks
    None
 */
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}RGB_color;


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */
extern RGB_color WHITE;
extern RGB_color RED;
extern RGB_color GREEN;
extern RGB_color BLUE;
extern RGB_color VIOLET;
extern RGB_color CYAN;
extern RGB_color YELLOW;
extern RGB_color OFF;

RGB_color RGB_leds[RGB_LEDS_COUNT];


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/**
  @Function
    void WS2812_send( ws2812_t* p_led );

  @Summary
    Transmite un valor rgb a un led.

  @Description
    Transmite un valor rgb a un led.

  @Precondition
    None

  @Parameters
    @param p_led Puntero a la estrucutra que contiene los valores para configurar un led individual.


  @Returns
    void

  @Remarks
    None.
 */
void RGB_send( RGB_color *p_leds, uint8_t p_length );

void RGB_setLed( uint8_t p_led, RGB_color p_color );

void RGB_tasks();

#endif /* _WS2812_H */

/* *****************************************************************************
 End of File
 */
