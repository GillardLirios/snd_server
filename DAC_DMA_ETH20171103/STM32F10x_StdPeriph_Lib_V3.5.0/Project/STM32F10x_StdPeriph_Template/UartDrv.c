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
// ��������: InitUart
// �� ��: ��ʼ��UART����
// �� ��: 	
// ��������: int 
// �� �� ֵ��S_OK-�ɹ� ��E_FAIL-����;
// �� ��:���� 
// ��������: 
// ��������: 2011��12��4��
// �޸���ʷ: 
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
// ��������: SendByte
// ��	 ��: ͨ������2��������
// �� 	 ��: 	
// 			 unsigned char ch		���͵�����
// �������ͣ�unsigned char	
// �� �� ֵ: �ѷ��͵�����
// �� 	 ��: ���� 
// ��������: 
// ��������: 2012��2��8��
// �޸���ʷ: 
//******************************************************************
// ֻ����SendString����
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
// ��������: SendByte3
// ��	 ��: ͨ������3��������
// �� 	 ��: 	
// 			 unsigned char ch		���͵�����
// �������ͣ�unsigned char	
// �� �� ֵ: �ѷ��͵�����
// �� 	 ��: ���� 
// ��������: 
// ��������: 2012��2��8��
// �޸���ʷ: 
//******************************************************************
// ֻ����SendString3����
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
// ��������: SendByte1
// ��	 ��: ͨ������1��������
// �� 	 ��: 	
// 			 unsigned char ch		���͵�����
// �������ͣ�unsigned char	
// �� �� ֵ: �ѷ��͵�����
// �� 	 ��: ���� 
// ��������: 
// ��������: 2012��2��8��
// �޸���ʷ: 
//******************************************************************
// ֻ����SendString1����
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
