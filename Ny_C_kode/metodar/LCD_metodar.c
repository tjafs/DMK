//
// Fil: LCD_metodar.c
// m.t.171013
//---------------------------------------

//---------------------------------------
// Inklusjonar og definisjonar
//---------------------------------------

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_spi.h"
#include "stm32f30x_rcc.h"


//---------------------------------------
// Globale variablar
//---------------------------------------

#include "extern_dekl_globale_variablar.h"

//---------------------------------------
// Funksjonsprototypar
//---------------------------------------
void LCD_init(void);
void LCD_kommando(uint8_t kommando);
void LCD_tekst(int8_t *tekst);
void LCD_data(uint8_t data);

void LCD_tal2siff_4(uint16_t tal);

void LCD_skrivesyklus_data(uint8_t databussverdi);
void LCD_skrivesyklus_kommando(uint8_t databussverdi);

void vent_400nsek(void);
void vent_100usek(void);
void vent_400msek(void);
//---------------------------------------
// Funksjonsdeklarasjonar
//---------------------------------------

//---------------------------------------------------------------------------
// Initialiseringsrutiner for gpio-modulane
//---------------------------------------------------------------------------

void LCD_init(void) {
	    volatile uint32_t j;
//   Oppsett av skjerm rett etter oppskrifta i databladet
//----------------------------------------------------------
        LCD_kommando(0x38);       // 8bit, 2 linjer (her.2x16),5x7pkt.

// Vent så  40msek, krav: minst 30msek
//---------------------------------------------
        j = 400;
        while(j-- > 0){
        	vent_100usek();
        }
//---------------------------------------------
        LCD_kommando(0x38);       // 8bit, 2 linjer (her.2x16),5x7pkt.

        //Vent i 200 usek
//---------------------------------------------
        j = 2;
        while(j-- > 0){
        	vent_100usek();
        }
//---------------------------------------------

        LCD_kommando(0x08);       // Skjerm av
        LCD_kommando(0x0E);       // Skjerm og visar på, blink av
        LCD_kommando(0x01);       // Slett skjerm og køyr heim visaren

        //  Vent så 4msek, krav: 1.53msek
//---------------------------------------------
        j = 40;
        while(j-- > 0){
        	vent_100usek();
        }
//---------------------------------------------

        LCD_kommando(0x06);       // Sett visar til inkr. (entry mode)
        LCD_tekst("STM32F3 er klar!!!!!");
        LCD_kommando(0xC0);        // Flytt peikar ("cursor") til pos.1, linje 2
}


//-----------------------------------------------------------------------
// Ymse små metodar for LCD-handtering
//-----------------------------------------------------------------------

void LCD_kommando(uint8_t kommando) { // Gi ein kommando til LCD

        LCD_skrivesyklus_kommando(kommando);

// Etter å ha sendt data el.kommando til LCD, må ein venta i > 40usek, bør i praksis venta 100usek
         vent_100usek();
}


void LCD_tekst(int8_t *tekst) {

        while( *tekst != 0) {   // Skriv ut ein 0-terminert tekststreng

             LCD_skrivesyklus_data(*tekst++);

// Etter å ha sendt data el.kommando til LCD, må ein venta i > 40usek, bør i praksis venta 100usek
             vent_100usek();
        }
}

void LCD_data(uint8_t data) {  // Skriv ut eit ASCII-teikn

        LCD_skrivesyklus_data( data );

// Etter å ha sendt data el.kommando til LCD, må ein venta i > 40usek, bør i praksis venta 100usek
        vent_100usek();
}



// Rutine som skriv ut eit 4-sifra tal til skjermen

void LCD_tal2siff_4( uint16_t tal ) {

        uint16_t skyv=0x30;    // Denne rutina splittar eit
        uint16_t t=1000;        // 4-sifra tal inn i enkle
        uint16_t i=0;          // siffer og skriv desse
        uint16_t siffer;       // sifra ut på skjermen
        uint8_t teikn;

        while( i < 4 ) {
             siffer=tal/t;
             teikn = ( uint8_t )(siffer + skyv ); // Gjer om til ASCII-verdi
             LCD_data(teikn);

             if ( tal >= t ) {
                tal=tal%t;
             }

             t=t/10;
             i++;
        }
}

// Rutinene som realiserer dei 2 typane skrivesyklus mot LCD
//----------------------------------------------------------
void LCD_skrivesyklus_data(uint8_t databussverdi) {

// Sett nå opp sekvens iflg. tidsdiagram for LCD for skriving av kommando (RS = 0)


    GPIOC->ODR = (0xFFF0 | LCD_Write | LCD_RS_data);
//
    GPIOC->ODR = (0xFFF0 | LCD_Write | LCD_RS_data | LCD_Enable);

    GPIOA->ODR = databussverdi;  //Legg ut kommando på databussen

        vent_400nsek();  //Må vera med

    GPIOC->ODR = (0xFFF0 | LCD_Write | LCD_RS_data);

}

void LCD_skrivesyklus_kommando(uint8_t databussverdi) {

	// Sett nå opp sekvens iflg. tidsdiagram for LCD for skriving av kommando (RS = 0)

    GPIOC->ODR = (0xFFF0 | LCD_Write | LCD_RS_kommando);
	//
    GPIOC->ODR = (0xFFF0 | LCD_Write | LCD_RS_kommando | LCD_Enable);

    GPIOA->ODR = databussverdi; //Legg ut ascii-teikn på databussen

      vent_400nsek();  //Må vera med

    GPIOC->ODR = (0xFFF0 | LCD_Write | LCD_RS_kommando);

}
//void LCD_skrivesyklus_data(uint8_t databussverdi) {
//
//// Sett nå opp sekvens iflg. tidsdiagram for LCD for skriving av kommando (RS = 0)
//    GPIOA->ODR &= (0xFF00 | databussverdi);  //Legg ut kommando på databussen
//
//    GPIOC->ODR &= (0xFFF0 | LCD_Write | LCD_RS_data);
////
//    GPIOC->ODR &= (0xFFF0 | LCD_Write | LCD_RS_data | LCD_Enable);
//
//        vent_400nsek();  //Må vera med
//
//    GPIOC->ODR &= (0xFFF0 | LCD_Write | LCD_RS_data);
////    GPIOC->ODR ^= GPIO_Pin_3;
//}
//
//void LCD_skrivesyklus_kommando(uint8_t databussverdi) {
//
//	// Sett nå opp sekvens iflg. tidsdiagram for LCD for skriving av kommando (RS = 0)
//    GPIOA->ODR &= (0xFF00 | databussverdi); //Legg ut ascii-teikn på databussen
//
//    GPIOC->ODR &= (0xFFF0 | LCD_Write | LCD_RS_kommando);
//	//
//    GPIOC->ODR &= (0xFFF0 | LCD_Write | LCD_RS_kommando | LCD_Enable);
//
//      vent_400nsek();  //Må vera med
//
//    GPIOC->ODR &= (0xFFF0 | LCD_Write | LCD_RS_kommando);
//
//}

void vent_400nsek(void) { //For optimaliseringsnivaa=2
	 volatile uint32_t i;
	 for(i=0;i<1;i++);
}
void vent_100usek(void) { //For optimaliseringsnivaa=2
	 volatile uint32_t i;
	 for(i=0;i<235;i++);
}
void vent_400msek(void) {
	 volatile uint32_t j = 4000;
     while(j-- >0) {
    		 vent_100usek();
     }
}





