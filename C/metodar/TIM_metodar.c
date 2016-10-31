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

    TIM_OCInitTypeDef        TIM_OCInitStructure;

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = vidde;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}


// Frå Ørland 14

//NVIC_InitTypeDef NVIC_InitStructure;

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
