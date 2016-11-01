//
// Fil: UART_metodar.c
// m.t.170913
//---------------------------------------

//---------------------------------------
// Inklusjonar og definisjonar
//---------------------------------------

#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_usart.h"

//---------------------------------------
// Globale variablar
//---------------------------------------

#include "extern_dekl_globale_variablar.h"

//---------------------------------------
// Funksjonsprototypar
//---------------------------------------

void USART1_oppstart(void);
void USART1_Put(uint8_t ch);
uint8_t USART1_Get(void);
void USART1_skriv(uint8_t ch);
uint8_t USART1_les(void);
void USART1_skriv_streng(uint8_t *streng);

void USART2_oppstart(void);
void USART2_Put(uint8_t ch);
uint8_t USART2_Get(void);
void USART2_skriv(uint8_t ch);
uint8_t USART2_les(void);
void USART2_skriv_streng(uint8_t *streng);
void USART2_send_tid8_og_data16(uint8_t tid, int16_t loggeverdi);
void USART2_send_tid8_og_data16x3(uint8_t tid,uint8_t sensor, int16_t loggeverdi1, int16_t loggeverdi2, int16_t loggeverdi3);
void USART2_handtering(uint8_t loggedata);
void USART2_handtering1(void);
void USART2_handtering2(uint16_t teljar);
void USART2andtering3(void);
// Funksjonsdeklarasjonar for USART1
//----------------------------------------------------------------------------
void USART1_oppstart(void)
{

    USART_InitTypeDef USART1_InitStructure;
    USART_ClockInitTypeDef  USART1_ClockInitStructure;

    // USART-delen må fiksast slik at den virkar her dvs. på STM32F3-kortet
    // der en bruker USART2 i staden for USART1 og med nye GPIO-pinnar, sjå manualen
    // på stm32f3 disc. shield samt brukarmanualen på f3-kortet.
    // Det kan også vera at strukturdefinisjonane er noko endra, sjå stm32f30x_usart.h

    //enable bus clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


    USART_ClockStructInit(&USART1_ClockInitStructure);
    USART_ClockInit(USART1, &USART1_ClockInitStructure);

    USART1_InitStructure.USART_BaudRate = 9600; //115200; //19200;//57600;//19200; //9600;
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;
    USART1_InitStructure.USART_Parity =  USART_Parity_No ; //USART_Parity_Odd;
    USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    //Write USART1 parameters
    USART_Init(USART1, &USART1_InitStructure);


// GPIO-delen må så konfigurerast
////------------------------------------------
//  //Deklarasjon av initialiseringsstrukturen.
//    GPIO_InitTypeDef GPIO_InitStructure_UART1;
//
//  //Slepp foerst til klokka paa GPIOA-modulen
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // Er alt gjort!
//
//  //Sett USART1 Tx (Pxx) som AlternativFunksjon og "push-pull" (vanleg totempaale)
//    GPIO_InitStructure_UART1.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure_UART1.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure_UART1.GPIO_Speed = GPIO_Speed_50MHz;
//
//  //Initialiser, dvs. last ned konfigurasjonen i modulen
//    GPIO_Init(GPIOA, &GPIO_InitStructure_UART1);
//
//  //Sett USART1 Rx (Pxx) som flytande inngang ("input floating")
//    GPIO_InitStructure_UART1.GPIO_Pin = GPIO_Pin_10;
//    GPIO_InitStructure_UART1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//
//  //Initialiser, dvs. last ned konfigurasjonen i modulen
//    GPIO_Init(GPIOA, &GPIO_InitStructure_UART1);
//
    //Aktiver så USART1
//    USART_Cmd(USART1, ENABLE);

    //Send til slutt her velkomst via UART/USB-modul
	USART1_skriv_streng((uint8_t *)"--\nVelkommen!\n\r");  // Ny linje og retur til linjestart etterpå.
}

void USART1_Put(uint8_t ch)
{
    USART_SendData(USART1, (uint8_t) ch); //Loop until the end of transmission
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        ;
        }

}

uint8_t USART1_Get(void)
{
    while ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {
        ;
        }
    return (uint8_t)USART_ReceiveData(USART1);
}


uint8_t USART1_les(void)
{
   if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
        {
	   return (uint8_t)USART_ReceiveData(USART1);
   }
   else {
       return (uint8_t)0x00;
   }
}

void USART1_skriv(uint8_t data)
{
    USART_SendData(USART1, (uint8_t) data); //Loop until the end of transmission
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        ;
        }

}
void USART1_skriv_streng(uint8_t *streng)
{
    while( *streng != 0) {   // Skriv ut ein 0-terminert tekststreng
    	USART1_skriv(*streng);
    	streng++;
    }
}

// Funksjonsdeklarasjonar for USART2
//----------------------------------------------------------------------------
void USART2_oppstart(void)
{
  //Deklarasjon av initialiseringsstrukturane.
    USART_InitTypeDef USART2_InitStructure;
    USART_ClockInitTypeDef  USART2_ClockInitStructure;

    // USART-delen må fiksast slik at den virkar her dvs. på STM32F3-kortet
    // der en bruker USART2 i staden for USART1 og med nye GPIO-pinnar, sjå manualen
    // på stm32f3 disc. shield samt brukarmanualen på f3-kortet.
    // Det kan også vera at strukturdefinisjonane er noko endra, sjå stm32f30x_usart.h

    //Slepp til klokka
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_ClockStructInit(&USART2_ClockInitStructure);
    USART_ClockInit(USART2, &USART2_ClockInitStructure);

    USART2_InitStructure.USART_BaudRate = 115200; //19200;//57600;//19200; //9600;
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;
    USART2_InitStructure.USART_Parity =  USART_Parity_No ; //USART_Parity_Odd;
    USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    //Legg inn konfigurasjonen i modulen
    USART_Init(USART2, &USART2_InitStructure);


   //GPIO-delen må fiksast slik at den virkar her dvs. på STM32F3-kortet
   //På STM32F3: GPIO-pinnane PA2 og 3 brukt mot intern USART2-modul
   //------------------------------------------
  //Deklarasjon av initialiseringsstrukturen.
    GPIO_InitTypeDef GPIO_InitStructure_UART2;

  //Slepp foerst til klokka paa GPIOA-modulen
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  //Sett USART2 Tx (PA2) som AlternativFunksjon og "push-pull" (vanleg totempaale)
    GPIO_InitStructure_UART2.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure_UART2.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure_UART2.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_InitStructure_UART2.GPIO_OType = GPIO_OType_PP;

  //Initialiser, dvs. last ned konfigurasjonen i modulen
    GPIO_Init(GPIOA, &GPIO_InitStructure_UART2);

  //Knytt pinnen til AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);// Sjå stm32f30x_gpio.h

  //Sett USART2 Rx (PA3) som flytande inngang ("input floating")
    GPIO_InitStructure_UART2.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure_UART2.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure_UART2.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  //Initialiser, dvs. last ned konfigurasjonen i modulen
    GPIO_Init(GPIOA, &GPIO_InitStructure_UART2);

  //Knytt pinnen til AF */
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);

  //Aktiver så USART1
    USART_Cmd(USART2, ENABLE);

  //Send til slutt her velkomst via UART/USB-modul
	USART2_skriv_streng((uint8_t *)"--\nSTM32F3 er klar!\n\r");  // Ny linje og retur til linjestart etterpå.
}

void USART2_Put(uint8_t ch)
{
    USART_SendData(USART2, (uint8_t) ch); //Loop until the end of transmission
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
        ;
        }

}

uint8_t USART2_Get(void)
{
    while ( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        {
        ;
        }
    return (uint8_t)USART_ReceiveData(USART2);
}


uint8_t USART2_les(void)
{
   if ( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
        {
	   return (uint8_t)USART_ReceiveData(USART2);
   }
   else {
       return (uint8_t)0x00;
   }
}

void USART2_skriv(uint8_t data)
{
    USART_SendData(USART2, (uint8_t) data); //Loop until the end of transmission
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
        ;
        }

}
void USART2_skriv_streng(uint8_t *streng)
{
    while( *streng != 0) {   // Skriv ut ein 0-terminert tekststreng
    	USART2_skriv(*streng);
    	streng++;
    }
}

void USART2_send_tid8_og_data16(uint8_t tid, int16_t loggeverdi)  {
	uint8_t tid0, tid1;
    int16_t data0, data1, data2, data3;

    tid0 = tid;
    tid1 = tid0 >> 4;

	USART2_skriv('T');
	USART2_skriv((uint8_t)(hex2ascii_tabell[(tid1 & 0x0F)]));   // Send MS Hex-siffer av ein tidsbyten
	USART2_skriv((uint8_t)(hex2ascii_tabell[(tid0 & 0x0F)])); // Send LS Hex-siffer av ein tidsbyten

	data0 = loggeverdi; //
	data1 = data0 >> 4; // Under skifting er det viktig at forteiknet blir med, difor int.
	data2 = data1 >> 4;
	data3 = data2 >> 4;

	USART2_skriv('L');            // L for loggedata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data3 & 0x000F)])); // Send MS Hex-siffer av 16-bitsdata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data2 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data1 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data0 & 0x000F)])); // Send LS Hex-siffer av dei 16 bitane
}

void USART2_send_tid8_og_data16x3(uint8_t tid, uint8_t sensor,int16_t loggeverdi1, int16_t loggeverdi2, int16_t loggeverdi3)  {
	uint8_t tid0, tid1,sensor0,sensor1;
    int16_t data0, data1, data2, data3;

    tid0 = tid;
    tid1 = tid0 >> 4;

	USART2_skriv('T');
	USART2_skriv((uint8_t)(hex2ascii_tabell[(tid1 & 0x0F)]));   // Send MS Hex-siffer av ein tidsbyten
	USART2_skriv((uint8_t)(hex2ascii_tabell[(tid0 & 0x0F)])); // Send LS Hex-siffer av ein tidsbyten

    sensor0 = sensor;
    sensor1 = sensor0 >> 4;

	USART2_skriv('S');
	USART2_skriv((uint8_t)(hex2ascii_tabell[(sensor1 & 0x0F)]));   // Send MS Hex-siffer av ein tidsbyten
	USART2_skriv((uint8_t)(hex2ascii_tabell[(sensor0 & 0x0F)])); // Send LS Hex-siffer av ein tidsbyten

	data0 = loggeverdi1; //
	data1 = data0 >> 4; // Under skifting er det viktig at forteiknet blir med, difor int.
	data2 = data1 >> 4;
	data3 = data2 >> 4;

	USART2_skriv('X');            // L for loggedata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data3 & 0x000F)])); // Send MS Hex-siffer av 16-bitsdata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data2 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data1 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data0 & 0x000F)])); // Send LS Hex-siffer av dei 16 bitane

	data0 = loggeverdi2; //
	data1 = data0 >> 4; // Under skifting er det viktig at forteiknet blir med, difor int.
	data2 = data1 >> 4;
	data3 = data2 >> 4;

	USART2_skriv('Y');            // L for loggedata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data3 & 0x000F)])); // Send MS Hex-siffer av 16-bitsdata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data2 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data1 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data0 & 0x000F)])); // Send LS Hex-siffer av dei 16 bitane

	data0 = loggeverdi3; //
	data1 = data0 >> 4; // Under skifting er det viktig at forteiknet blir med, difor int.
	data2 = data1 >> 4;
	data3 = data2 >> 4;

	USART2_skriv('Z');            // L for loggedata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data3 & 0x000F)])); // Send MS Hex-siffer av 16-bitsdata
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data2 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data1 & 0x000F)]));
	USART2_skriv((uint8_t)(hex2ascii_tabell[(data0 & 0x000F)])); // Send LS Hex-siffer av dei 16 bitane


}




void USART2_handtering(uint8_t loggedata)  {
    uint8_t data;
	USART2_skriv('D');
	USART2_skriv(':');
    data = loggedata >> 4;
	USART2_skriv((uint8_t)(hex2ascii_tabell[data]));   // Send MSB av maalinga
    data = loggedata & 0x0F;
	USART2_skriv((uint8_t)(hex2ascii_tabell[data])); // Send LSB
	USART2_skriv(' ');
}
void USART2_handtering1(void)  {


//	uint8_t ny_kommando;

	//	USART1_skriv(((maaling_teljar-1)/100)+0x30);    // Skriving av målingsnr. via UART/USB-modulen.


//	    ny_kommando = USART1_les();  // Sjekk om det er ny kommando frå tastatur
//		if(ny_kommando != 0)  {      // I så fall, oppdater kommando, ellers kommando som før
//			gyldig_trykk_av_USERbrytar = 1;
//		}


//		USART1_skriv(teikn++);    // Skriving av ASCII-koda teikn via UART/USB-modulen.
//		USART1_skriv('\r');       // retur til same felt.
//		if(teikn >'z')
//		   {
//		   teikn = 0x30;
//		   }

//	USART1_skriv(((maaling_teljar-1)/100)+0x30);    // Skriving av målingsnr. via UART/USB-modulen.


//	USART2_skriv((uint8_t)(loggedata >> 8));   // Send MSB av maalinga
//	USART2_skriv((uint8_t)(loggedata & 0xFF)); // Send LSB
//
//	USART1_skriv((uint8_t)(testmaaling >> 8));   // Send MSB av maalinga
//	USART1_skriv((uint8_t)(testmaaling & 0xFF)); // Send LSB


    testmaaling += 10;
    if(testmaaling >= 500) {
    	testmaaling = 0;
    }

    maaling_teljar++;
    if(maaling_teljar > 2000) {

    	maaling_teljar = 1;
//    	send_ny_tidsserie_1000 = 0;

    }

    USART1_skriv((uint8_t)(maaling_teljar >> 8));   // Send MSB av maalinga
    	USART1_skriv((uint8_t)(maaling_teljar & 0xFF)); // Send LSB

    	USART1_skriv((uint8_t)(testmaaling >> 8));   // Send MSB av maalinga
    	USART1_skriv((uint8_t)(testmaaling & 0xFF)); // Send LSB


        testmaaling += 10;
        if(testmaaling >= 500) {
        	testmaaling = 0;
        }

        maaling_teljar++;
        if(maaling_teljar > 2000) {

        	maaling_teljar = 1;
//       	send_ny_tidsserie_1000 = 0;

        }
//	USART1_skriv((uint8_t)(maaling_teljar >> 8));   // Send MSB av maalinga
//		USART1_skriv((uint8_t)(maaling_teljar & 0xFF)); // Send LSB
//
//		USART1_skriv((uint8_t)(testmaaling >> 8));   // Send MSB av maalinga
//		USART1_skriv((uint8_t)(testmaaling & 0xFF)); // Send LSB
//
//
//		testmaaling += 10;
//		if(testmaaling >= 500) {
//			testmaaling = 0;
//		}
//
//		maaling_teljar++;
//		if(maaling_teljar > 1000) {
//
//			maaling_teljar = 1;
//			send_ny_tidsserie_1000 = 0;
//
//		}
//	USART1_skriv((uint8_t)(maaling_teljar >> 8));   // Send MSB av maalinga
//		USART1_skriv((uint8_t)(maaling_teljar & 0xFF)); // Send LSB
//
//		USART1_skriv((uint8_t)(testmaaling >> 8));   // Send MSB av maalinga
//		USART1_skriv((uint8_t)(testmaaling & 0xFF)); // Send LSB
//
//
//		testmaaling += 10;
//		if(testmaaling >= 500) {
//			testmaaling = 0;
//		}
//
//		maaling_teljar++;
//		if(maaling_teljar > 1000) {
//
//			maaling_teljar = 1;
//			send_ny_tidsserie_1000 = 0;
//
//		}

}

void USART2_handtering2(uint16_t teljar)  {


	uint8_t data, i = 98;

    data = teljar;   // sender LSB av teljaren først
	USART1_skriv(data);  // Skriving av data via UART/USB-modulen.

    data = (teljar>>8);   // sender så MSB av teljaren
	USART1_skriv(data);  // Skriving av data via UART/USB-modulen.

	data = 0;
	while(i>0)  // Så skriving av 98 nullar.
		{
		USART1_skriv(data);
		i--;
	    }

}

void USART2_handtering3(void)  {

	uint8_t teikn = 0x30;
	uint8_t ny_kommando;
	uint8_t kommando ='2';


    //Send velkomst via UART/USB-modul
	USART1_skriv_streng((uint8_t *)"--\nVelkommen!\n\r");  // Ny linje og retur til linjestart etterpå.

	while(1)   // Endelaus løkkje
		{
	    ny_kommando = USART1_les();  // Sjekk om det er ny kommando frå tastatur
		if(ny_kommando != 0)  {      // I så fall, oppdater kommando, ellers kommando som før
			kommando = ny_kommando;
		}

		switch(kommando) {           // Tolk kommando
		case 'g': //GPIO_snu_PC9();
			break; // Grøn blink
		case 'b': //GPIO_snu_PC8();
			break; // Blå blink
		}

//		venting_vhja_nedteljing(1000000);  // Det må gå litt tid mellom blinka

		USART1_skriv(teikn++);    // Skriving av ASCII-koda teikn via UART/USB-modulen.
		USART1_skriv('\r');       // retur til same felt.
		if(teikn >'z')
		   {
		   teikn = 0x30;
		   }

    }
}

