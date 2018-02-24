#include "UartDrv.h"
//#include "stm32_eval.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
//#include "DataEntity.h"
//#include "Application.h"
#include "AppConfig.h"
#include "log.h"
unsigned char Uart3_Send_Buffer1[MAX_SEND_BUF];
volatile unsigned char g_szRevBuf[MAX_REV_BUF] = {0};

unsigned char g_bUart2SendLen	= 0;
unsigned char* g_pszUart2SendBuf = 0;
char g_bIsRevPackageOver;

unsigned char g_bRev	= 0;
unsigned long g_ulLastRecvTickMs = 0;
char g_flagUart3Sending = 0;


//unsigned char g_flagSendEnable = 0;
RecvEntity g_eRecv;
extern LoopInfoEntity* p_loop_info;
extern LaneInfoEntity*	p_lane_info;
extern UserCfgEntity* 	p_user_cfg;
extern SysCfgEntity*		p_sys_cfg;

 //******************************************************************
// 函数名称: InitUart
// 功 能: 初始化UART功能
// 参 数: 	
// 返回类型: int 
// 返 回 值：S_OK-成功 ，E_FAIL-出错;
// 作 者:龙丰 
// 电子邮箱: 
// 创建日期: 2011年12月4日
// 修改历史: 
//******************************************************************
int InitUart(unsigned int baud)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	DMA_InitTypeDef DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	g_flagUart3Sending = 0;

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);;
	printf("%s\r\n",__FUNCTION__);
	syslog(DBG,"Uart init");	

	return 0;		 
}

void DMA1_Channel2_IRQHandler(void)
{
	DMA_ClearFlag(DMA1_FLAG_TC2);
	DMA_Cmd(DMA1_Channel2,DISABLE);
	g_flagUart3Sending = 0;
}



//******************************************************************
// 函数名称: SendByte
// 功	 能: 通过串口2发送数据
// 参 	 数: 	
// 			 unsigned char ch		发送的数据
// 返回类型：unsigned char	
// 返 回 值: 已发送的数据
// 作 	 者: 龙丰 
// 电子邮箱: 
// 创建日期: 2012年2月8日
// 修改历史: 
//******************************************************************
// 只能由SendString调用
unsigned char SendByte(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  
//  USART_SendData(EVAL_COM2, (uint8_t) ch);
//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM2, USART_FLAG_TC) == RESET)
//  {}
  return ch;
}

//******************************************************************
// 函数名称: SendByte3
// 功	 能: 通过串口3发送数据
// 参 	 数: 	
// 			 unsigned char ch		发送的数据
// 返回类型：unsigned char	
// 返 回 值: 已发送的数据
// 作 	 者: 龙丰 
// 电子邮箱: 
// 创建日期: 2012年2月8日
// 修改历史: 
//******************************************************************
// 只能由SendString3调用
unsigned char SendByte3(char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  
//  USART_SendData(EVAL_COM3, (uint8_t) ch);
//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM3, USART_FLAG_TC) == RESET)
//  {}
  return ch;
}

//******************************************************************
// 函数名称: SendByte1
// 功	 能: 通过串口1发送数据
// 参 	 数: 	
// 			 unsigned char ch		发送的数据
// 返回类型：unsigned char	
// 返 回 值: 已发送的数据
// 作 	 者: 龙丰 
// 电子邮箱: 
// 创建日期: 2012年2月8日
// 修改历史: 
//******************************************************************
// 只能由SendString1调用
unsigned char SendByte1(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
 // 	STM_EVAL_LEDToggle(LED1);
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
  return ch;
}


int SendString1(const char* pString)
{
	while(*pString)
	{
		SendByte1(*pString++);	
	}
	return 0;
}

