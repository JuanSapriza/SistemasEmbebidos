#ifndef HEADER_TEMPLATE_H
#define	HEADER_TEMPLATE_H


//<editor-fold defaultstate="collapsed" desc="Includes">
#include <stdbool.h>
#include <stdint.h>
#include "Configuration_Bits.h"
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Defines">
#define HDT_UNA_CONSTANTE   12
#define HDT_UNA_ETIQUETA

//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="Enums">

enum HDT_UN_ENUM
{
    HDT_UN_ENUM_ESTO,                   // = 0
    HDT_UN_ENUM_ES,                     // = 1
    HDT_UN_ENUM_UN,                     // = 2
    HDT_UN_ENUM_NUMERO_DE_ELEMENTOS,    // = 3
};

enum HDT_OTRO_ENUM
{
    HDT_OTRO_ENUM_ESTE = 1,
    HDT_OTRO_ENUM_OTRO = 0,
    HDT_OTRO_ENUM_ENUM = -1,
};


//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="Structs">

struct HDT_nombreDeEstructura
{
    uint8_t unElemento;
    bool otroElemento;
    uint16_t unElementoQueEsUnVector[ UNA_CONSTANTE ];
} nombreDeMiVariableDelTipoDeEstaEstructura;

struct HDT_otraEstructura
{
    uint8_t *unElementoQueEsUnPuntero; 
};

typedef struct HDT_unaEstructuraQueSeraUnTipoDeVariable_t
{
    uint8_t unElemento;
    uint8_t otroElemento;
    uint8_t otroElementoMas;
}


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Global Variables">

uint8_t HDT_unaVariableGlobal;
bool HDT_otraVariableGlobal;
struct HDT_otraEstructura HDT_unaEstructuraDelTipoDeOtraEstructura;
HDT_unaEstructuraQueSeraUnTipoDeVariable_t HDT_yaMeCanseDeNombrarEstructuras;

extern uint8_t HDT_unaVariableDelPuntoC; 

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Declaración de Funciones Globales">
void HDT_unaFuncion();
uint8_t HDT_otraFuncion(bool p_parametro);


//</editor-fold>




#endif	/* HEADER_TEMPLATE_H */

