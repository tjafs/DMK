//
// Fil: TIM_metodar.c
// m.t.311013
//---------------------------------------

//---------------------------------------
// Inklusjonar og definisjonar
//---------------------------------------

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_tim.h"

//---------------------------------------
// Globale variablar
//---------------------------------------

#include "extern_dekl_globale_variablar.h"

//---------------------------------------
// Funksjonsprototypar
//---------------------------------------
void TIM_oppstart(void);
void PWM_sett_vidde_TIM2_CH2(uint32_t vidde);


void TIM4_oppstart_reg(void);
void TIM4_aktiver_reg(void);
void TIM4_deaktiver_reg(void);

//---------------------------------------
// Funksjonsdeklarasjonar
//---------------------------------------

void TIM_oppstart(void) { // Koden er ikkje heilt i mål
//
// Her er det nå TIM2 CH2 som skal brukast, og går ut på PA1.

 //Deklarasjon av initialiseringsstrukturane.
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef        TIM_OCInitStructure;

// Oppsett av TIM2 CH2 som PWM-utgang

 //Slepp først til klokka paa TIM2.
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


   /* Time base configuration */
   TIM_TimeBaseStructure.TIM_Period = PWM_periode; //
   TIM_TimeBaseStructure.TIM_Prescaler = PWM_preskalering; // 0
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

   /* PWM1 Mode configuration: Channel2 */
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   TIM_OCInitStructure.TIM_Pulse = PWM_vidde;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

   TIM_OC2Init(TIM2, &TIM_OCInitStructure);

//   TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);



//Så oppsett av GPIO-pinnen PA1 som blir brukt av TIM2-modulen
//------------------------------------------
//Deklarasjon av initialiseringsstrukturen.
	GPIO_InitTypeDef GPIO_InitStructure_TIM2;

  //Slepp til klokka paa GPIO-portA.
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); // | RCC_AHBPeriph_AFIO

  //Konfigurer PA1.
	GPIO_InitStructure_TIM2.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure_TIM2.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure_TIM2.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStructure_TIM2.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  //Initialiser, dvs. last ned konfigurasjonen i modulen
	GPIO_Init(GPIOA, &GPIO_InitStructure_TIM2);

  //Knytt TIM2-pinnane til AF */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1); //!!!! Ikkje AF_2 som det og står i dok.



// Aktiver til slutt TIM2

    TIM_Cmd(TIM2, ENABLE);
}


//
void PWM_sett_vidde_TIM2_CH2(uint32_t vidde)  {

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_OCInitTypeDef        TIM_OCInitStructure;

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = vidde*0.5;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = vidde; //
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_preskalering; // 0
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}


// Frå Ørland 14

//NVIC_InitTypeDef NVIC_InitStructure;
//
//NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//NVIC_Init(&NVIC_InitStructure);
//
//NVIC_SetPriority(TIM2_IRQn, 2);
//}
//
//
//extern uint16_t diode_moenster;
//extern int32_t pulsteller;
//extern uint8_t bytt_retning;
//
//uint8_t first = 1;
//
//void TIM2_IRQHandler()
//{
//if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//{
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//
//    diode_moenster = diode_moenster + 0x100;
//
//    pulsteller++;
//
//    if(pulsteller > 100000){
//    	LinMot_bytt_retning();
//    	pulsteller = 0;
//    }
//}
//}
//
////
//void PWM_sett_vidde_TIM2_CH2(uint32_t vidde)  {
//
//TIM_OCInitTypeDef        TIM_OCInitStructure;
//
///* PWM1 Mode configuration: Channel2 */
//TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//TIM_OCInitStructure.TIM_Pulse = vidde;
//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//
//TIM_OC2Init(TIM2, &TIM_OCInitStructure);
//
//TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
//
///* TIM2 enable counter */
//TIM_Cmd(TIM2, ENABLE);
//}
//
//

/*
 * Setter opp TIM4 til å lage er 4kHz 50% duty klokkesignal til SCF-filteret.
 * Dette skal ut på pinne PD12. Bruker kanal 1, CH1, i TIM4 for å lage dette.
 * I denne fuksjonen skriver vi direkte til regristrene i TIM4.
 * Vi bruker driverfuksjonene/bibliotekene for å sette opp GPIO PD12.
 */

void TIM4_oppstart_reg(void){

	 //Slepp først til klokka paa TIM4.
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	 // Setter hvilken modus TIM4 skal være i ved å bruke capture compare mode register 1 (CCMR1)
	 // Bit 6:4 skal være 110 (for PWM mode 1, up-count)
	 // Bit 3 settes til 1 (for at capture compare verdi 1 bare oppdateres ved start av ny periode)
	  TIM4->CCMR1 = 0x68;			// = 0b 01101000

	 // Setter hvilken capture compare kanal som skal være aktiv.
	 // Vi vil sammenligne telleren med capture compare verdi nr. 1 og at kanal 1 skal være output.
	  TIM4->CCER = 0x01;

	 // Setter prescaler for klokke, grunnklokka på 72 MHz deles på dette tallet.
	 // Timerens klokkefrekvens blir da resultatet av denne delingen. f_TIM4 = f / (PSC+1)
	  TIM4->PSC = 0x47;				// = 71, altså 72. Dette gir f_TIM4 = 1 MHz og T_TIM4 = 1 us.

	 // Setter auto-reload, aka. hvor langt skal vi telle.
	  TIM4->ARR = 0xFA;				// = 250.	(Altså til 250 us , f = 4 kHz)

	 // Setter capture compare verdi nr 1. for TIM4. Dette er hvor langt timeren skal telle mens
	 // utgangen er høy. ( Setter Duty Cycle)
	  TIM4->CCR1 = 0x7D;			// = 125.	(Altså til 125, halvparten siden D = 50%)




	 // Så oppsett av GPIO-pinnen PD12 som blir brukt av TIM4-modulen
	 // Her bruker vi driverfunksjonene (enkelt å bare kopiere og tilpasse koden ovenfra)

	 // Deklarasjon av initialiseringsstrukturen.
	  GPIO_InitTypeDef GPIO_InitStructure_TIM4;

	 // Slepp til klokka paa GPIO-portD.
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE); // | RCC_AHBPeriph_AFIO

	 // Konfigurer PD12.
	  GPIO_InitStructure_TIM4.GPIO_Pin = GPIO_Pin_12;
	  GPIO_InitStructure_TIM4.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure_TIM4.GPIO_Speed = GPIO_Speed_Level_1;
	  GPIO_InitStructure_TIM4.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	 // Initialiser, dvs. last ned konfigurasjonen i modulen
	  GPIO_Init(GPIOD, &GPIO_InitStructure_TIM4);

	 // Knytt TIM4-pinnane til AF */
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_2);

	 // Aktiverer så timeren ved å skrive til Enable bit i CR1 registeret.
	  TIM4->CR1 |= 0x01;

}

void TIM4_aktiver_reg(void){
		TIM4->CR1 |= 0x01;		// Aktiverer ved å skrive 1 til Enable bit i CR1 registeret
}

void TIM4_deaktiver_reg(void){
		TIM4->CR1 &= 0xFE;		// Deaktiverer ved å slette Enable bittet i CR1 registeret.
}

