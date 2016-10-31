//Knut Ørland, hausten 2014
//
#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"

#include "extern_dekl_globale_variablar.h"

void LinMot_oppstart(void);
void LinMot_bytt_retning(void);

void LinMot_oppstart(void){

	// Sett opp outputs

	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

    // DIR
	#ifdef BGA_KORT
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	#else
		// D9 (TX på adapter-click, velger retning)
		 GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
	#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void LinMot_bytt_retning(void){
	#ifdef BGA_KORT
		GPIOD->ODR ^= (1<<8);
	#else
		GPIOD->ODR ^= (1<<9);
	#endif
}
