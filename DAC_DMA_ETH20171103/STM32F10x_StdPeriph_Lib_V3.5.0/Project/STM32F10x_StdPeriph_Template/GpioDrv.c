#include "GpioDrv.h"
#include "application.h"
#include "log.h"
#include "bsp.h"
#include "AppConfig.h"
#include <stdio.h>
extern UserCfgEntity* 	p_user_cfg;
#if(1==GPIO_SJ_VD12_2V0)

	const t_port	GPIO_PORT[MAX_IO_PORT] = {
	//id					port		pin						mode							speed							init_act
	{FLASH_NSS,		GPIOC,	GPIO_Pin_13,GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_SET},
	{RTS3,				GPIOA,	GPIO_Pin_0,	GPIO_Mode_IPU,		GPIO_Speed_50MHz, Bit_SET},
	{NIC_NRST,		GPIOA,	GPIO_Pin_1,	GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_RESET},
	{TX3,					GPIOA,	GPIO_Pin_2,	GPIO_Mode_AF_PP,	GPIO_Speed_50MHz, Bit_SET},
	{RX3,					GPIOA,	GPIO_Pin_3,	GPIO_Mode_IPU,		GPIO_Speed_50MHz, Bit_SET},
	{NIC_NSS,			GPIOA,	GPIO_Pin_4,	GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_SET},
	{SCK1,				GPIOA,	GPIO_Pin_5,	GPIO_Mode_AF_PP,	GPIO_Speed_50MHz, Bit_SET},
	{MISO1,				GPIOA,	GPIO_Pin_6,	GPIO_Mode_IPU,		GPIO_Speed_50MHz, Bit_SET},
	{MOSI1,				GPIOA,	GPIO_Pin_7,	GPIO_Mode_AF_PP,	GPIO_Speed_50MHz, Bit_SET},
	{CAP1_1,			GPIOB,	GPIO_Pin_0,	GPIO_Mode_IN_FLOATING,		GPIO_Speed_50MHz, Bit_SET},
	{CAP1_2,			GPIOB,	GPIO_Pin_1,	GPIO_Mode_IN_FLOATING,		GPIO_Speed_50MHz, Bit_SET},
	{RS485_T_R,		GPIOB,	GPIO_Pin_2,	GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_SET},
	{TX2,					GPIOB,	GPIO_Pin_10,GPIO_Mode_AF_PP,	GPIO_Speed_50MHz, Bit_SET},
	{RX2,					GPIOB,	GPIO_Pin_11,GPIO_Mode_IPU,		GPIO_Speed_50MHz, Bit_SET},
	{SOE,					GPIOB,	GPIO_Pin_12,GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_RESET},
	{SCLK,				GPIOB,	GPIO_Pin_13,GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_RESET},
	{SLAT,				GPIOB,	GPIO_Pin_14,GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_RESET},
	{SDAT,				GPIOB,	GPIO_Pin_15,GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_RESET},
	{CAP5_1,			GPIOA,	GPIO_Pin_8,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{TX1,					GPIOA,	GPIO_Pin_9,	GPIO_Mode_AF_PP,	GPIO_Speed_50MHz, Bit_SET},
	{RX1,					GPIOA,	GPIO_Pin_10,GPIO_Mode_IPU,		GPIO_Speed_50MHz, Bit_SET},
	{CAP5_2,			GPIOA,	GPIO_Pin_11,GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{KEY_IN,			GPIOA,	GPIO_Pin_12,GPIO_Mode_IPU,		GPIO_Speed_50MHz, Bit_SET},
//	{ETH_IRQ,		GPIOA,	GPIO_Pin_13,GPIO_Mode_Out_PP,	GPIO_Speed_50MHz, Bit_SET},
	{CAP4_1,			GPIOA,	GPIO_Pin_15,GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{CAP4_2,			GPIOB,	GPIO_Pin_3,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{CAP0_1,			GPIOB,	GPIO_Pin_4,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{CAP0_2,			GPIOB,	GPIO_Pin_5,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{CAP2_1,			GPIOB,	GPIO_Pin_6,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{CAP2_2,			GPIOB,	GPIO_Pin_7,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{CAP3_1,			GPIOB,	GPIO_Pin_8,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	{CAP3_2,			GPIOB,	GPIO_Pin_9,	GPIO_Mode_IPD,		GPIO_Speed_50MHz, Bit_SET},
	};
#else
	const t_port	GPIO_PORT[MAX_IO_PORT] = {
	//id								port		pin					mode										otype						speed							pupd					init_act
	{NC,								GPIOB,	GPIO_Pin_7,	GPIO_Mode_IN_FLOATING,	GPIO_Speed_50MHz, Bit_SET},
	};
#endif
	
	
void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	for(int i=0; i<MAX_IO_PORT; i++)
	{
		GPIO_InitStructure.GPIO_Pin 	= GPIO_PORT[i].pin;	
		GPIO_InitStructure.GPIO_Mode 	= GPIO_PORT[i].mode;
		GPIO_InitStructure.GPIO_Speed = GPIO_PORT[i].speed;
		GPIO_Init(GPIO_PORT[i].port, &GPIO_InitStructure);
		if(GPIO_Mode_Out_PP == GPIO_PORT[i].mode || GPIO_Mode_Out_OD==GPIO_PORT[i].mode)
			gpio_write(GPIO_PORT[i].id, GPIO_PORT[i].init_state);
	//	syslog(DBG,"%s",__FUNCTION__);
	}


	syslog(DBG,"%s ",__FUNCTION__);
}

uint8_t gpio_read(uint32_t port)
{
	if(port<MAX_IO_PORT)
	{
//		syslog(DBG,"%s,PORT[%d]:%d",__FUNCTION__,port, GPIO_ReadInputDataBit(GPIO_PORT[port],GPIO_PIN[port]));
	//	#if(DEV_TYPE==DEV_TYPE_BINDER||DEV_TYPE==DEV_TYPE_REMOTE_AUTO||DEV_TYPE==DEV_TYPE_BLINKER||DEV_TYPE==DEV_TYPE_TESTER||DEV_TYPE==DEV_TYPE_LED)
			return GPIO_ReadInputDataBit(GPIO_PORT[port].port,GPIO_PORT[port].pin);
	//	#else
//			return GPIO_ReadInputDataBit(GPIO_PORT[port],GPIO_PIN[port]);
//		#endif
	}
	else
		return 0xff;
}


void gpio_write(uint8_t port_id, uint8_t value)
{
	if(port_id<MAX_IO_PORT)
	{
//	#if(DEV_TYPE==DEV_TYPE_BINDER||DEV_TYPE==DEV_TYPE_REMOTE_AUTO||DEV_TYPE==DEV_TYPE_BLINKER||DEV_TYPE==DEV_TYPE_TESTER||DEV_TYPE==DEV_TYPE_LED)
		GPIO_WriteBit(GPIO_PORT[port_id].port,GPIO_PORT[port_id].pin,(BitAction)(value));
//	#else
//		GPIO_WriteBit(GPIO_PORT[port_id],GPIO_PIN[port_id],(BitAction)(value));
//	#endif
		
	//syslog(DBG,"%s,%d->%d",__FUNCTION__,port_id,value
	//	uart1_put_char('.');
	}
}

#define   STB_H      GPIOB->BSRR = GPIO_Pin_14
#define   STB_L      GPIOB->BRR = GPIO_Pin_14 

#define   CLK_H      GPIOB->BSRR = GPIO_Pin_13
#define   CLK_L      GPIOB->BRR = GPIO_Pin_13

#define   SOE_H      GPIOB->BSRR = GPIO_Pin_12
#define   SOE_L      GPIOB->BRR = GPIO_Pin_12

#define   SDA_H      GPIOB->BSRR = GPIO_Pin_15
#define   SDA_L      GPIOB->BRR = GPIO_Pin_15

void shift_data(unsigned char data)
{
	int i=0,j=0;
	SOE_L;
	for(i=7;i>=0;i--)
	{
		CLK_L;
		for(j=0;j<18;j++){;}
		if(data & (1<<i))
		{
			SDA_H;
			//putchar('^');
		}
		else 
		{
			SDA_L;
			//putchar('_');
		}
		CLK_H;
		for(j=0;j<18;j++){;}
	}
//	STB_L;
//	STB_H;	
	//syslog(DBG,"%x",data);
}

uint8_t sdata_buf[3]={0,0,0};
uint8_t sdata_buf_history[3]={0,0,0};
// byte 0
// 7	6 	5 	4 	3 	2 	1 	0 
// r8	r7	r6	r5	r4	r3	r2	r1
// byte 1
// 7	6 	5 	4 	3 	2 	1 	0 
// e4	e3	e2	e1	r12	r11	r10	r9
// byte 2
// 7	6 	5 	4 	3 	2 	1 	0 
// e12	e11	e10	e9	e8	e7	e6	e5
void SetLEDDisplay(void)
{
	int j;
	if(sdata_buf[0]==sdata_buf_history[0] &&sdata_buf[1]==sdata_buf_history[1] &&sdata_buf[2]==sdata_buf_history[2])
	{
	}
	else{
		//syslog(DBG,"%s,dat=%02X%02X%02X",__FUNCTION__,sdata_buf[0],sdata_buf[1],sdata_buf[2]);
		shift_data(sdata_buf[2]);
		shift_data(sdata_buf[1]);
		shift_data(sdata_buf[0]);
		STB_L;
		for(j=0;j<6;j++){;}
		STB_H;	
		for(j=0;j<6;j++){;}
		sdata_buf_history[0] = sdata_buf[0];
		sdata_buf_history[1] = sdata_buf[1];
		sdata_buf_history[2] = sdata_buf[2];
	}
}
void SetLEDData(T_LED_ID led_id, const char nState)
{
	if(led_id<24)
	{
		if(nState)
			sdata_buf[led_id/8] = bitset(sdata_buf[led_id/8],led_id%8);
		else
			sdata_buf[led_id/8] = bitclr(sdata_buf[led_id/8],led_id%8);
	}
}


// get_key
unsigned long long u64recently_pressed_tick_ms = 0;
unsigned char u8key_state = KEY_STATE_UP;
unsigned char get_key()
{

//	syslog(DBG,"KEY:%d",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1));
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))
	{
			if(KEY_STATE_UP == u8key_state)
			{
				return KEY_NULL;
			}
			else
			{
				u8key_state = KEY_STATE_UP;
				if(p_user_cfg->u64device_time-u64recently_pressed_tick_ms>4000)
				{
					syslog(DBG,"KEY_4S");
					return KEY_4S;
				}
				else if(p_user_cfg->u64device_time-u64recently_pressed_tick_ms>300)
				{
					syslog(DBG,"KEY_300ms");
					return KEY_300MS;
				}
				else
				{
					syslog(DBG,"KEY_INSTANT");
					return KEY_INSTANT;
				}
			}
	}
	else
	{
		if(KEY_STATE_UP == u8key_state)
		{
			u64recently_pressed_tick_ms = p_user_cfg->u64device_time;
			u8key_state = KEY_STATE_DOWN;
		}
		return KEY_NULL;
	}
}

#if 0

//******************************************************************
// 函数名称: GPIO_Configuration
// 功	 能: 配置GPIO
// 参 	 数: 	
// 返回类型：	
// 返 回 值: 
// 作 	 者: 龙丰 
// 电子邮箱: 
// 创建日期: 2012年2月8日
// 修改历史: 
//******************************************************************
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// PB0:TM3-3,PB1:TM3-4,PB6:TM4-1,PB7:TM4-2,PB8:TM4-3,PB9:TM4-4 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// PA0:PWM1
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	// PD3：sys STATE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
		

	

	// PG11:SCK, PG12:LAT, PG15:SDA, PG14:TXEN
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_14| GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_Init(GPIOG, &GPIO_InitStructure);



	syslog(DBG,"GPIO Configuration end");
}

#endif

