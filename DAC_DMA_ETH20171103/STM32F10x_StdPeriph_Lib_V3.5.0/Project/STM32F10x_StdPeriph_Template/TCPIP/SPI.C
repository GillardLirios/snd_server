#include "SPI.h"

//#include "stm32_eval.h"
#include "log.h"
//SPI1初始化
void SPI1_Init(void)
{
   SPI_InitTypeDef SPI_InitStructure;
#if 1
	GPIO_InitTypeDef GPIO_InitStructure;

   /* Enable SPI1 and GPIOA clocks */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
   //PA4 ---CS
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
	 //PC9 rst
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;   
   GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	 
	//	SPI1-
	//	PA5--SCK----SCK
	//	PA6--MISO---SO
	//	PA7--MOSI---SI 	 
   /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
   GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;   //上拉输入
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	 

#endif
   /* SPI1 configuration */ 
   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial = 7;
   SPI_Init(SPI1, &SPI_InitStructure);
   /***************************************
   PE5  --------  LED1
   PE6  --------  LED2 
   ****************************************/
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_Init(GPIOD, &GPIO_InitStructure);
   /* Enable SPI1  */
   SPI_Cmd(SPI1, ENABLE);
   syslog(DBG,"SPI1_Init  ends\r\n");
}

//SPI1读写一字节数据
#define MAX_RETRY	1000
unsigned char SPI1_ReadWrite(unsigned char writedat)
{
	int timeout = MAX_RETRY;
   /* Loop while DR register in not emplty */
   while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET)
	 {
		 timeout--;
		 if(timeout<=0)
		 {
			 syslog(DBG,"SPI_GetFlagStatus SPI_FLAG_TXE timeout"); 
			 break;
		 }
	 }
	 
	 timeout = MAX_RETRY;
   /* Send byte through the SPI1 peripheral */
   SPI_SendData(SPI1, writedat);

   /* Wait to receive a byte */
   while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET)
	{
		 timeout--;
		 if(timeout<=0)
		 {
			 syslog(DBG,"SPI_GetFlagStatus SPI_FLAG_RXNE timeout"); 
			 break;
		 }
	 }

   /* Return the byte read from the SPI bus */
   return SPI_ReceiveData(SPI1);
}


void SPI2_Init(void)
{
   SPI_InitTypeDef SPI_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   /* SPI2 Periph clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

   /* Configure SPI2 pins: NSS, SCK, MISO and MOSI */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* SPI1 configuration */ 
   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial = 7;

   SPI_Init(SPI2, &SPI_InitStructure);
   /* Enable SPI2  */
   SPI_Cmd(SPI2, ENABLE);
}

//SPI2读写一字节数据
unsigned char SPI2_ReadWrite(unsigned char writedat)
{
	
   /* Loop while DR register in not emplty */
   while (SPI_GetFlagStatus(SPI2, SPI_FLAG_TXE) == RESET );
	

   /* Send byte through the SPI1 peripheral */
   SPI_SendData(SPI2, writedat);

   /* Wait to receive a byte */
   while (SPI_GetFlagStatus(SPI2, SPI_FLAG_RXNE) == RESET);

   /* Return the byte read from the SPI bus */
   return SPI_ReceiveData(SPI2);
}

