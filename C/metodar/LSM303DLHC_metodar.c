//
// Fil: LSM303DLHC_metodar.c
// m.t.140715
// Metoden under konfigurerer I2C1-modulen i mikrokontrolleren samt
// akselerometer- og magnetometerkretsen LSM303DLHC på STM32F3-Discovery-kortet.
//---------------------------------------

//---------------------------------------
// Inklusjonar og definisjonar
//---------------------------------------

#include "stm32f30x.h"
#include "stm32f3_discovery_lsm303dlhc.h"


//---------------------------------------
// Funksjonsprototypar
//---------------------------------------
void aks_oppstart(void);

//---------------------------------------
// Funksjonsdeklarasjonar
//---------------------------------------

void aks_oppstart(void)  {

	LSM303DLHCMag_InitTypeDef magnetoMeter;
	magnetoMeter.MagFull_Scale = LSM303DLHC_FS_8_1_GA;
	magnetoMeter.MagOutput_DataRate = LSM303DLHC_ODR_15_HZ;
	magnetoMeter.Temperature_Sensor = LSM303DLHC_TEMPSENSOR_DISABLE;
	magnetoMeter.Working_Mode = LSM303DLHC_CONTINUOS_CONVERSION;
	LSM303DLHC_MagInit(&magnetoMeter);


	LSM303DLHCAcc_InitTypeDef acc;
	acc.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
	acc.AccOutput_DataRate = LSM303DLHC_ODR_100_HZ;
	acc.Axes_Enable = LSM303DLHC_AXES_ENABLE;
	acc.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
	acc.Endianness = LSM303DLHC_BLE_LSB;
	acc.High_Resolution = LSM303DLHC_HR_ENABLE;  //12bit oppløysing
	acc.Power_Mode = LSM303DLHC_NORMAL_MODE;     //Dvs. ikkje lågeffektsmodus
	LSM303DLHC_AccInit(&acc);  // Denne funksjonen utfører først "Low level Init", som
							   // set opp I2C-modulen. Innhaldet i denne er vist under:
}
	/* Frå stm32f3_discovery_lsm303dlhc:

	* @brief  Initializes the low level interface used to drive the LSM303DLHC
	* @param  None
	* @retval None
	*/
//	static void LSM303DLHC_LowLevel_Init(void)
//	{
//	  GPIO_InitTypeDef GPIO_InitStructure;
//	  EXTI_InitTypeDef EXTI_InitStructure;
//	  I2C_InitTypeDef  I2C_InitStructure;
//
//	  /* Enable the I2C periph */
//	  RCC_APB1PeriphClockCmd(LSM303DLHC_I2C_CLK, ENABLE);
//
//	  /* Enable SCK and SDA GPIO clocks */
//	  RCC_AHBPeriphClockCmd(LSM303DLHC_I2C_SCK_GPIO_CLK | LSM303DLHC_I2C_SDA_GPIO_CLK , ENABLE);
//
//	  /* Enable INT1 GPIO clock */
//	  RCC_AHBPeriphClockCmd(LSM303DLHC_I2C_INT1_GPIO_CLK, ENABLE);
//
//	  /* Enable INT2 GPIO clock */
//	  RCC_AHBPeriphClockCmd(LSM303DLHC_I2C_INT2_GPIO_CLK, ENABLE);
//
//	  /* Enable DRDY clock */
//	  RCC_AHBPeriphClockCmd(LSM303DLHC_DRDY_GPIO_CLK, ENABLE);
//
//	  GPIO_PinAFConfig(LSM303DLHC_I2C_SCK_GPIO_PORT, LSM303DLHC_I2C_SCK_SOURCE, LSM303DLHC_I2C_SCK_AF);
//	  GPIO_PinAFConfig(LSM303DLHC_I2C_SDA_GPIO_PORT, LSM303DLHC_I2C_SDA_SOURCE, LSM303DLHC_I2C_SDA_AF);
//
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //Opprinneleg 50MHz
//
//	  /* I2C SCK pin configuration */
//	  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_I2C_SCK_PIN;
//	  GPIO_Init(LSM303DLHC_I2C_SCK_GPIO_PORT, &GPIO_InitStructure);
//
//	  /* I2C SDA pin configuration */
//	  GPIO_InitStructure.GPIO_Pin =  LSM303DLHC_I2C_SDA_PIN;
//	  GPIO_Init(LSM303DLHC_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
//
//	  /* Mems DRDY */
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //Opprinneleg 50MHz
//
//	  /* Mems DRDY pin configuration */
//	  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_DRDY_PIN;
//	  GPIO_Init(LSM303DLHC_DRDY_GPIO_PORT, &GPIO_InitStructure);
//
//	  /* Connect EXTI Line to Mems DRDY Pin */
//	  SYSCFG_EXTILineConfig(LSM303DLHC_DRDY_EXTI_PORT_SOURCE, LSM303DLHC_DRDY_EXTI_PIN_SOURCE);
//
//	  EXTI_InitStructure.EXTI_Line = LSM303DLHC_DRDY_EXTI_LINE;
//	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//
//	  EXTI_Init(&EXTI_InitStructure);
//
//	  /* I2C configuration -------------------------------------------------------*/
//	  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//	  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
//	  I2C_InitStructure.I2C_DigitalFilter = 0x00;
//	  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
//	  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//	  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	  I2C_InitStructure.I2C_Timing = 0x00902025;
//
//	  /* Apply LSM303DLHC_I2C configuration after enabling it */
//	  I2C_Init(LSM303DLHC_I2C, &I2C_InitStructure);
//
//	  /* LSM303DLHC_I2C Peripheral Enable */
//	  I2C_Cmd(LSM303DLHC_I2C, ENABLE);
//
//	  /* Configure GPIO PINs to detect Interrupts */
//	  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_I2C_INT1_PIN;
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //Opprinneleg 50MHz
//	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//	  GPIO_Init(LSM303DLHC_I2C_INT1_GPIO_PORT, &GPIO_InitStructure);
//
//	  GPIO_InitStructure.GPIO_Pin = LSM303DLHC_I2C_INT2_PIN;
//	  GPIO_Init(LSM303DLHC_I2C_INT2_GPIO_PORT, &GPIO_InitStructure);
//	}


