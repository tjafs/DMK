//
// Fil: avbrots_metodar.c
// m.t.170913
//---------------------------------------

//---------------------------------------
// Inklusjonar og definisjonar
//---------------------------------------

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"

//---------------------------------------
// Globale variablar
//---------------------------------------

#include "extern_dekl_globale_variablar.h"

//---------------------------------------
// Funksjonsprototypar
//---------------------------------------

void avbrot_oppstart(void);
void SysTick_oppstart(void);
void SysTick_Handler(void);
void GPIO_sjekk_brytar(void);
int8_t USART2_les(void);
void PWM_sett_vidde_TIM4_k4(uint16_t vidde);
void GPIO_brytaravprelling(void);
//---------------------------------------
// Funksjonsdeklarasjonar
//---------------------------------------

// Oppsett av avbrotssystemet
//-------------------------------------
void avbrot_oppstart(void) {
//     __enable_irq(); // Ikkje n�dvendig. Avbrota er ope etter Reset.(PRIMASK=0)
	                   // Funksjon for stenging: __disable_irq();
     __set_BASEPRI(0x20); // Alle avbrot med prioritetsverdi 2 el. h�gare blir maskert vekk (dei 4 MSb gir pri-verdien).
                          // SysTick har f�tt avbrotsprioritet lik 1, sj� SysTick-fila.
//     __disable_irq();
}


// Oppsett av SysTick-taimeren som realiserer fast tikk-intervall
//----------------------------------------------------------------
void SysTick_oppstart(void) {

 // Oppsett av SysTick
  NVIC_SetPriority(SysTick_IRQn, 1); // 0-31 der 0 er h�gast
  SysTick->CTRL = 0;  // Stopp teljaren
  SysTick->LOAD = 72000;  // Startverdi gir 1 msek avbrotsintervall.
  SysTick->VAL = 0;  // Nullstill teljaren
  SysTick->CTRL = (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk) ;
                                               // (0x7), Start teljaren, avbrot og intern klokke.
}

// Avbrotsmetode ("Interrupt Service Routine", ISR)
// for avbrot fraa SysTick-taimeren
//----------------------------------------------------------------
void SysTick_Handler(void) {
	uint8_t kommando = 0;

	tikkteljar_avprelling++;
	if(tikkteljar_avprelling >= 10) { //Har det g�tt 10 x 1 millisek sidan siste
		                              // brytaravlesing, s� les brytarniv� og sjekk om
		GPIO_sjekk_brytar();          // det er eit nytt gyldig trykk.
		tikkteljar_avprelling = 0;
	}

	tikkteljar_sampling++;
	if(tikkteljar_sampling >= 10) { //Har det g�tt 10 millisek sidan sist, skal det utf�rast ny m�ling.

		ny_maaling = 1;
		tikkteljar_sampling = 0;
		  //N�r brukaren gir "k�yr"-signal fr� PC-sida, skal m�lingane sendast
												 //med tidsreferanse til UART/USB-grensesnittet.

		tikkteljar_uartkomm++;
		if(tikkteljar_uartkomm >= 10) { //Kvar tiande m�ling skal
					                    // sendast med tidsreferanse til UART/USB, s� gi melding til
			send_maaling = 1;          //hovudprogrammet om � k�yra ny sending.
			tikkteljar_uartkomm = 0;
		}

	}

	tikkteljar_diodar++;
	if(tikkteljar_diodar >= 200) { //Har det g�tt 200 x 1 millisek sidan siste
			                              // oppdatering av diodebitane), s� gi melding til
		oppdater_diodar = 1;          //tilstandsmaskinsmetoden.
		tikkteljar_diodar = 0;
	}

	GPIOC->ODR = GPIOC->ODR ^ GPIO_Pin_6; // Blinkesignal ut paa testpinne (PC6).


  //Sjekk om det er ny kommando fr� tastatur
	kommando = USART2_les();
	if( kommando == 'k')  {  //K�yr i gong sending av m�lingane til loggaren
		send_maalingar_til_loggar = 1;
		legg_til_meldingshovud = 1; //Aller f�rst skal hovudprogrammet leggja til STX

	}
	if( kommando == 's')  {  //Stopp sending av m�lingane til loggaren
		send_maalingar_til_loggar = 0;
		legg_til_meldingshale = 1;

	}


}


