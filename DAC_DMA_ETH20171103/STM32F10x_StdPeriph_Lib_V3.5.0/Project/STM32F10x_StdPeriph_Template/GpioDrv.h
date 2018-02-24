#ifndef _GPIO_H_
#define _GPIO_H_
#include "stm32f10x_gpio.h"
void gpio_init(void);
uint8_t gpio_read(uint32_t port);
void gpio_write(uint8_t port_id, uint8_t value);

typedef struct{
	unsigned char id;
	GPIO_TypeDef* port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
	GPIOSpeed_TypeDef speed;
	BitAction init_state;
}t_port;



#define GPIO_SJ_VD12_2V0 	1
#if(1==GPIO_SJ_VD12_2V0)
	typedef enum{
		FLASH_NSS = 0,
		RTS3,
		NIC_NRST,
		TX3,
		RX3,
		NIC_NSS,
		SCK1,
		MISO1,
		MOSI1,
		CAP1_1,
		CAP1_2,
		RS485_T_R,
		TX2,
		RX2,
		SOE,
		SCLK,
		SLAT,
		SDAT,
		CAP5_1,
		TX1,
		RX1,
		CAP5_2,
		KEY_IN,
		//ETH_IRQ,
		CAP4_1,
		CAP4_2,
		CAP0_1,
		CAP0_2,
		CAP2_1,
		CAP2_2,
		CAP3_1,
		CAP3_2,
	}PORT;
	#define MAX_IO_PORT	31
	
	typedef enum{
		RUN1 = 0,
		RUN2,
		RUN3,
		RUN4,
		RUN5,
		RUN6,
		RUN7,
		RUN8,
		RUN9,
		RUN10,
		RUN11,
		RUN12,
		ERR1,
		ERR2,
		ERR3,
		ERR4,
		ERR5,
		ERR6,
		ERR7,
		ERR8,
		ERR9,
		ERR10,
		ERR11,
		ERR12,
	}T_LED_ID;
	#define LED_CNT	24
#else
	typedef enum{
		NC = 0,
	}PORT;
	#define MAX_IO_PORT 1
#endif
	
void GPIO_Configuration(void);
	
#define ON	1
#define OFF	0
#define START 0
#define STOP 1
#define LOW	 0
#define HIGH 1	
void SetLEDData(T_LED_ID led_id, const char nState);	
void SetLEDDisplay(void);	
void SetAlarmLED(const char nLedNum, const char nState);
// key
#define KEY_STATE_UP		1
#define KEY_STATE_DOWN	0
#define KEY_300MS	2
#define KEY_400MS	3
#define KEY_4S		4
#define KEY_INSTANT	5
#define KEY_NULL	6
unsigned char get_key(void);
// key end
#endif
