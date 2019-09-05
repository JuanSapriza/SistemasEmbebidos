
//<editor-fold defaultstate="collapsed" desc="Includes">
#include <stdbool.h>
#include <stdint.h>
#include "../Laboratorio1.X/Configuration_Bits.h" //solo si voy a usar una función de ahí
#include "FileTemplate.h"
//</editor-fold>



//<editor-fold defaultstate="collapsed" desc="Global Variables">

//Variables que solo van a ser usadas en este archivo.C

uint8_t unaVariable;
uint8_t unaVariableInicializada = HDT_OTRO_ENUM_ESTE;
uint32_t unaVariableInicializadaPeroMasGrande = HDT_OTRO_ENUM_ESTE;
uint8_t HDT_unaVariableDelPuntoC = 14;  //muy mal no haber definido esto como un DEFINE 

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Funciones Locales">
void HDT_unaFuncion();
uint8_t HDT_otraFuncion(bool p_parametro);


//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Definición de Funciones Globales">
void HDT_unaFuncion()
{
    static uint8_t estadoDeMiFuncion = HDT_UN_ENUM_ESTO;
    uint8_t unaVariableDeEstaFuncion = 0; 
    
    HDT_unaFuncion();
    
    switch( estadoDeMiFuncion )
    {
        case HDT_UN_ENUM_ESTO:
            unaVariableDeEstaFuncion = 1;
            estadoDeMiFuncion = HDT_UN_ENUM_ES;
            break;
            
        case HDT_UN_ENUM_ES:
            HDT_otraFuncion( unaVariableDeEstaFuncion );
            estadoDeMiFuncion = HDT_UN_ENUM_ESTO;
            break;
            
        default: break;
    }
}

uint8_t HDT_otraFuncion(bool p_parametro)
{




}


//</editor-fold>





