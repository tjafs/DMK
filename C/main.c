//
// Hovudprogram
// Fil: main.c
// m.t.150715

// Prosjektet saman med eit passande python-skript, gir ein dataloggar for tre 16-bits
// akselerasjonsdata med tidsreferanse
//---------------------------------------

//---------------------------------------
// Inklusjonar og definisjonar
//---------------------------------------
#include "stm32f30x.h"
#include "stm32f3_discovery_lsm303dlhc.h"

//---------------------------------------
// Globale variablar
//---------------------------------------

#include "dekl_globale_variablar.h"

//---------------------------------------
// Funksjonsprototypar
//---------------------------------------

void maskinvare_init(void);
//void tilstandsmaskin(void);
void GPIO_sjekk_brytar(void);
void GPIO_sett_kompassmoenster(int8_t verdi);
void GPIO_blink(void);
void GPIO_lys_av(void);
void GPIO_lys_paa(void);
void USART2_skriv(uint8_t ch);
void USART2_send_tid8_og_data16(uint8_t tid, int16_t loggeverdi);
void USART2_send_tid8_og_data16x3(uint8_t tid, int16_t loggeverdi1, int16_t loggeverdi2, int16_t loggeverdi3);
int8_t SPI2_SendByte_Sokkel1(int8_t data);
int8_t SPI1_SendByte_gyro(int8_t data);
int8_t les_gyrotemp(void);
void vent_400nsek(void);

//---------------------------------------
// Funksjonsdeklarasjonar
//---------------------------------------

int main(void)  {
    uint16_t a;

    maskinvare_init();

	while(1) {


		if(gyldig_trykk_av_USERbrytar) { //Er brytaren trykt ned sidan sist?
                                         // Skal då laga ei ekstramelding.
		    uint8_t data0, data1;

            data0 = diode_moenster;
            data1 = data0 >> 4;

        	USART2_skriv('S');    //Viser at det er ei brytarmelding (S for svitsj)
        	USART2_skriv((uint8_t)(hex2ascii_tabell[(data1 & 0x0F)]));   // Send MS Hex-siffer av ein tidsbyten
        	USART2_skriv((uint8_t)(hex2ascii_tabell[(data0 & 0x0F)])); // Send LS Hex-siffer av ein tidsbyten

        	gyldig_trykk_av_USERbrytar = 0;

        	diode_moenster = 0; 		  // Nullstill diodemønsteret etter eit brytartrykk
        }

	 // Her er hovudoppgåva til dette prosjektet, nemleg avlesing av 3D akselerasjonsdata og sending av dette
	 // til PC.
		if(send_maalingar_til_loggar)  {   // Viss brukaren har sett i gong logging ved å trykkja 'k' på
			                               // tastaturet.
        	if(legg_til_meldingshovud)  {
        		USART2_skriv((uint8_t) STX); // Meldingsstart aller først
        		legg_til_meldingshovud = 0;
        	}

        	if(ny_maaling) { // Det skal utførast ny måling med 100Hz samplefrekvens (ordna i fila avbrotsmetodar)

		        LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, buffer, 6); // OUT_X_L_A ligg på lågaste adresse
		        																   // i XYZ-registerblokka i kretsen
			 // Sett saman akselerometerdata
                a   = (buffer[1] << 8) | buffer[0]; //Buffer 1 er MSByte i flg. databladet.
		        a_x = ((int16_t)(a)) >> 4;          //Gjer om til int der bare dei 12 teljande bitane er med
				a   = (buffer[3] << 8) | buffer[2];	//Dette gir området -2048 til 2047. Oppløysinga er
				a_y = ((int16_t)(a)) >> 4;			// 1mg pr. LSb ved +/-2g område i flg. databladet.
				a   = (buffer[5] << 8) | buffer[4];	// Verdien 1000 gir då 1 g.
				a_z = ((int16_t)(a)) >> 4;

			 // Filtrer målingane
				a_xf_k = (a1*a_xf_k_1 + b1*a_x)/100; //Nedsamplingsfilter, sjå deklarasjonsfila
				a_xf_k_1 = a_xf_k;

				a_yf_k = (a1*a_yf_k_1 + b1*a_y)/100; //Nedsamplingsfilter, sjå deklarasjonsfila
				a_yf_k_1 = a_yf_k;

				a_zf_k = (a1*a_zf_k_1 + b1*a_z)/100; //Nedsamplingsfilter, sjå deklarasjonsfila
				a_zf_k_1 = a_zf_k;

			 // Send kvar tiande måling
				if(send_maaling) {

                	samplenr++;                      // Oppdatering av tidsreferanse

    		        USART2_send_tid8_og_data16x3(samplenr, (int16_t) a_xf_k, (int16_t) a_yf_k, (int16_t) a_zf_k);

    		        send_maaling = 0; // Sendinga er nå utført.
                }

    		    ny_maaling = 0; // Målinga er nå utført
			 }

        }
		else if(!send_maalingar_til_loggar) { // Her har brukaren trykt ein 's' på tastaturet, eller
											  // logginga har aldri blitt starta.
			if(legg_til_meldingshale)  {
			     USART2_skriv((uint8_t) ETX); // Meldingshale for å avslutta loggemeldingane etter stopping.
			     legg_til_meldingshale = 0;
			}
		}

     }
}

//-------------------------------------------------------------------------------
//#include <math.h> //Hugs å velja "use base C library" i config/link
//					//samt FPU Hard under "Compile"
//#include "stm32f30x.h"
//#include "stm32f30x_gpio.h"
//#include "stm32f30x_rcc.h"

//void GPIO_init(void);
//void GPIO_blink(void);
//void USART_Printf_Example(void);
//void GPIO_Toggle_Example(void);
//volatile float acc = 0;
//uint32_t *p;
//volatile uint16_t a = 256;
//volatile uint8_t  b;
//
//int main(void)
//{
//volatile float x = 1.57,y;
//volatile float z;
//
//GPIO_init();
//    //automatically added by CoIDE
////	GPIO_Toggle_Example();
//
//	//automatically added by CoIDE
////	USART_Printf_Example();
//    b = (a>>8);

//	while(1)
//    {
//    GPIO_blink();
//
//	  p= (uint32_t *)&acc;
//	  y = sinf(x);  //funksjonf er for enkelpresisjon dvs. "float"
//	  z = 1 - powf(cosf(x),2);  //FPU-en er laga for float. Double vil vera mykje mindre effektivt
//
//	  acc = acc + y*z- 2.0f; // For å tvinga "float" som std.
//    }
//}



//	     tilstandsmaskin(); // Styring av sykkellykttilstand
//	     if(send_ny_tidsserie_1000) { // sending kvart 10. msek, sjå SysTick_oppstart() og
//	    	 if(send_1maaling) { // sending kvart 10. msek, sjå SysTick_oppstart() og
//	    	                // SysTick_Handler() i fila avbrotsmetodar.c
//
//	    		 USART_handtering();
//	    		 //send_1maaling = 0; Sender nå løpande sidan ein lagar testdata i USART-rutina
//	    	 }
//
//	     }
//void tilstandsmaskin(void) {
//
//	if (gyldig_trykk_av_USERbrytar) {
//
//		gyldig_trykk_av_USERbrytar = 0;
//        tilstand++;
//
//	    if (tilstand > 2) { // Skal ha 3 tilstandar, tilstand0-2
//		   tilstand = 0;
//	    }
//	}
//
//	if (oppdater_diodar) {
//
//		switch(tilstand) {
//		case 0:
//			GPIO_lys_av();
//			break;
//		case 1:
//			GPIO_blink();
//			break;
//		case 2:
//			GPIO_lys_paa();
//			break;
//		}
//	    oppdater_diodar = 0;
//	}
//}
//---------------------------------------------------------------------------------
// Fraa oerland
//----------------------------------------------------------------------------------
//---------------------------------------
// Globale variablar
//---------------------------------------
//
//#include "dekl_globale_variablar.h"
//extern uint16_t diode_moenster;
//uint8_t  mottatt_data;
//extern uint8_t bytt_retning;
//
////---------------------------------------
//// Funksjonsprototypar
////---------------------------------------
//
//extern uint8_t leddbuss;
//
//void maskinvare_init(void);
////void tilstandsmaskin(void);
//void GPIO_sjekk_brytar(void);
//void GPIO_sett_kompassmoenster(uint16_t verdi);
//void GPIO_blink(void);
//void GPIO_lys_av(void);
//void GPIO_lys_paa(void);
//void USART_handtering(void);
//int8_t SPI2_SendByte_Sokkel1(int8_t data);
//void Exp_click_sokkel1_sett_retningAB(int8_t moenster_A,int8_t moenster_B);
//void Exp_click_sokkel1_skriv_til_AB(int8_t moenster_A,int8_t moenster_B);
//void vent_400nsek(void);
//
//void LinMot_bytt_retning(void);
//
////---------------------------------------
//// Funksjonsdeklarasjonar
////---------------------------------------
//char* itoa(int val, int base){
//		static char buf[32] = {0};
//		int i = 30;
//		for(; val && i ; --i, val /= base)
//			buf[i] = "0123456789abcdef"[val % base];
//		return &buf[i+1];
//	}
//
//extern int32_t pulsteller;
//
//uint16_t lcd_wait = 0;
//
//int main(void)
//{
//	volatile uint32_t i;
//	volatile uint8_t data_A = 0;
//	volatile uint32_t data_B = 72000;
//	data_B = 72000/10;
//	data_B = 72;
//	data_B = 360;
//
//    maskinvare_init();
//
//    i = 200;
//	while(i--) {
//	  vent_400nsek();
//	}
//
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//
//
//	while(1) {
//
//		i = 20;
//        while(i--) {
//	      vent_400nsek();
//        }
//
//		if(oppdater_diodar) {
//			GPIO_sett_kompassmoenster(diode_moenster);
//			//diode_moenster = diode_moenster + 0x100;
//			oppdater_diodar = 0;
//		}
//
//
//        // Les accelerometer
//        uint8_t buffer[6];
//        LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_H_A, buffer, 6);
//        //LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, buffer, 6);
//
//		// Hent ut accelerometerdata
//		int16_t x = (buffer[0] << 8) | buffer[1];
//		int16_t y = (buffer[4] << 8) | buffer[5];
//		int16_t z = (buffer[2] << 8) | buffer[3];
//
//
//        // Skriv LCD
//		if(lcd_wait++ > 50){
//			lcd_wait = 0;
//
//			LCD_kommando(0x01);
//
//			i = 40;
//			while(i-- > 0){
//				vent_100usek();
//			}
//
//			// Skriv data til LCD
//			LCD_pos(0, 0);
//			char xs[10];
//			sprintf(xs, "%i", x);
//			LCD_tekst(xs);
//
//			LCD_pos(1, 0);
//			char ys[10];
//			sprintf(ys, "%i", y);
//			LCD_tekst(ys);
//
//			LCD_pos(2, 0);
//			char zs[10];
//			sprintf(zs, "%i", z);
//			LCD_tekst(zs);
//		}
//
//		char log[30];
//		sprintf(log, "%i\n", z);
//		USART2_skriv_streng(log);
//
//
//		#ifdef BGA_KORT
//			// Blink LEDs på BGA-kort
//			leddbuss ^= (1<<4) | (1<<5) | (1<<6) | (1<<7);
//		#endif
//
//
//		//data_B = 72;
//		//data_B += 1000;
//		//if(data_B > 72000) data_B = 7200;
//
//		// Frekvens til LinMot
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//		TIM_OCInitTypeDef        TIM_OCInitStructure;
//
//		/* Time base configuration */
//		TIM_TimeBaseStructure.TIM_Period = data_B; //
//		TIM_TimeBaseStructure.TIM_Prescaler = PWM_preskalering; // 0
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//
//		/* PWM1 Mode configuration: Channel2 */
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//		TIM_OCInitStructure.TIM_Pulse = data_B/2;
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//		TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//
//
//		 // Endre frekvens
//		 if((GPIOC->IDR & (1<<2)) == 0 && lcd_wait == 0){
//			 data_B -= 1;
//			 if(data_B < 7){
//				 data_B = 360;
//			 }
//		 }
//
//		GPIO_sjekk_brytar();
//        if(gyldig_trykk_av_USERbrytar) { //Er brytaren trykt ned sidan sist?
//        	diode_moenster = 0x100;
//
//        	LinMot_bytt_retning();
//
//        	gyldig_trykk_av_USERbrytar = 0;
//        }
//	}
//}




