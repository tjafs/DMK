#include "stm32f30x_adc.h"
#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"


// Funksjoner
void ADC_oppstart(void);
uint8_t ADc_verdi;

void ADC_oppstart(void);


// initialiseringsstruktur
GPIO_InitTypeDef		GPIO_InitStructure;
ADC_InitTypeDef 		ADC_InitStructure;
ADC_CommonInitTypeDef	ADC_commonInitStructure;
void ADC_oppstart(void){

// Konfigurer ADC3-klokken. 72Mhz / 6 = 12 MHz
	RCC_ADCCLKConfig(RCC_ADC34PLLCLK_Div6);

// aktiver klokken til ADC3 og 4
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

	ADC_StructInit(&ADC_InitStructure);

// kalibrerings prosedyre
	ADC_VoltageRegulatorCmd(ADC3, ENABLE);

// starter kalibrering
	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1) != RESET );
	int32_t calibration_value = ADC_GetCalibrationValue(ADC3);




// setter strukturverdier for ADC3
	ADC_commonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_commonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
	ADC_commonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_commonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
	ADC_commonInitStructure.ADC_TwoSamplingDelay = 0;
	ADC_CommonInit(ADC1, &ADC_commonInitStructure);



	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
	ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
	ADC_InitStructure.ADC_NbrOfRegChannel = 1;

// Laster ned konfigurasjonen til modulen
	ADC_Init(ADC3, &ADC_InitStructure);

// setter ADC3 kanal 1, og smpletid
	ADC_RegularChannelConfig(ADC3,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);

// aktiverer ADC3
	ADC_Cmd(ADC3, ENABLE);

// Venter på at ADC3 er klar
	while(!ADC_GetFlagStatus(ADC3,ADC_FLAG_RDY));

// Aktivere klokken til GPIOB
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

// Konfigurering av PB1 som analog inngang
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

// Laster ned konfigurasjon til modulen
	GPIO_Init(GPIOB, &GPIO_InitStructure);

// Starter ADC-avlesning
	ADC_StartConversion(ADC3);



}

uint8_t ADCVerdi(void){ // hent ut avlest bitmønster fra ADC3
	while(! (ADC3->ISR & ADC_ISR_EOC)); // Test EOC flag
	uint8_t digitalverdi = ADC_GetConversionValue(ADC3);
	return digitalverdi;
}

