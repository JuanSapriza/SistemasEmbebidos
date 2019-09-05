

#ifndef _HARDWARE_PROFILE
#define _HARDWARE_PROFILE

//INLCUDES
#include <xc.h>
#include <sys/attribs.h>

// LEDS
#define LEDA_SetDigitalOutput() (TRISAbits.TRISA7 = 0) //idem a hacer TRISAbits.w = 128
#define LEDB_SetDigitalOutput() (TRISBbits.TRISB14 = 0)

#define LEDA_setHigh()  (LATAbits.LATA7 = 1)
#define LEDA_setLow()   (LATAbits.LATA7 = 0)
#define LEDA_toggle()  (LATAbits.LATA7 = !LATAbits.LATA7)

#define LEDB_setHigh()  (LATBbits.LATB14 = 1)
#define LEDB_setLow()   (LATBbits.LATB14 = 0)
#define LEDB_toggle()  (LATBbits.LATB14 = !LATBbits.LATB14)


// BUTTONS
#define BTNB_setDigitalInput() { ANSELAbits.ANSA13 = 0 ; TRISAbits.TRISA13 = 1; }
#define BTNA_setDigitalInput() { ANSELBbits.ANSB15 = 0 ; TRISBbits.TRISB15 = 1; }

#define BTNB_setPullDown() (CNPDAbits.CNPDA13 = 1)
#define BTNA_setPullDown() (CNPDBbits.CNPDB15 = 1)

#define BTNB_getValue() PORTAbits.RA13
#define BTNA_getValue() PORTBbits.RB15

#define BTNB_setInt()           do{\
                                CNCONAbits.ON = 1;\
                                CNCONAbits.CNSTYLE = 1;\
                                } while(0);
                        
#define BTNA_setInt()           do{\
                                    CNCONBbits.ON = 1;\
                                    CNCONBbits.CNSTYLE = 1;\
                                } while(0);
                        
#define BTNB_setIntLowEdge()    { CNEN1Abits.CNIE1A13 = 1; CNEN0Abits.CNIE0A13 = 0; }
#define BTNB_setIntHighEdge()   { CNEN1Abits.CNIE1A13 = 0; CNEN0Abits.CNIE0A13 = 1; }
#define BTNB_setIntBothEdge()   { CNEN1Abits.CNIE1A13 = 1; CNEN0Abits.CNIE0A13 = 1; }
                       
#define BTNA_setIntLowEdge()    { CNEN1Bbits.CNIE1B15 = 1; CNEN0Bbits.CNIE0B15 = 0; }
#define BTNA_setIntHighEdge()   { CNEN1Bbits.CNIE1B15 = 0; CNEN0Bbits.CNIE0B15 = 1; }
#define BTNA_setIntBothEdge()   { CNEN1Bbits.CNIE1B15 = 1; CNEN0Bbits.CNIE0B15 = 1; }
                        
#define INT_init()              do{\
                                    INTCONbits.MVEC =   1;\
                                    IPC2bits.CNAIP =    2;\
                                    IPC2bits.CNAIS =    0;\
                                    IPC2bits.CNBIP =    2;\
                                    IPC2bits.CNBIS =    0;\
                                    IEC0bits.CNAIE =    1;\
                                    IEC0bits.CNBIE =    1;\
                                    __builtin_enable_interrupts();\
                                }while(0);                    
                        












#endif

