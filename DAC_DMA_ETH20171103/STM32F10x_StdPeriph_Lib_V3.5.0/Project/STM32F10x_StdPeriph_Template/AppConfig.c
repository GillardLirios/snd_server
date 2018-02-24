#include "AppConfig.h"
#include "bsp.h"
#include <stdio.h>
#include <string.h>
//#include "stm32_eval.h"
#include "log.h"
#include "Application.h"
//#include "SpiFlashDrv.h"
#include "eeprom.h"
#include "encrypt.h"


#define CONFIG_SECTOR 30
#define CONFIG_ADDR CONFIG_SECTOR*0x1000  // 4096bytes per sector

unsigned char  g_u8ArrModbusRegs[MAX_REGS*2];

LoopInfoEntity* p_loop_info	= (LoopInfoEntity*)(g_u8ArrModbusRegs+MODBUS_BUF_LOOP_INFO_ADDR);
LaneInfoEntity*	p_lane_info	= (LaneInfoEntity*)(g_u8ArrModbusRegs+MODBUS_BUF_LANE_INFO_ADDR);
UserCfgEntity* 	p_user_cfg	= (UserCfgEntity*)(g_u8ArrModbusRegs+MODBUS_BUF_USER_CFG_ADDR);
SysCfgEntity*		p_sys_cfg		= (SysCfgEntity*)(g_u8ArrModbusRegs+MODBUS_BUF_SYS_CFG_ADDR);





//typedef struct
//{
//	unsigned long long u64device_time;
//	unsigned char  u8device_name[24];					// 设备名称			32
//	unsigned char	 u8ip[4];						// ip				4
//	unsigned short u16modbus_port;
//	unsigned short u16http_port;
//	unsigned short u16udp_port;
//	unsigned short u16uart_port;
//	unsigned short u16rcv_timeout;
//	unsigned short u16statistic_interval;	
//	unsigned short u16loop_distance;
//	unsigned short u16in_sensitivity[4];
//	unsigned short u16out_sensitivity[4];
//	unsigned char  u16scan_cycle;
//	unsigned char  u16running_mode;
//	unsigned short u16average_freq_stat_count;
//	unsigned short u16base_freq_stat_count;
//	unsigned short s16reservd[2];
//	unsigned int	 u32unlock;
//}UserCfgEntity;



void InitConfigData(void)
{
	int i;
	#if 1
//	unsigned char u8ArrDeviceId[12]={0};
//	GetDeviceID(u8ArrDeviceId);
//	memset((void*)&p_user_cfg,0,sizeof(UserCfgEntity));
//	memset((void*)&p_sys_cfg,0,sizeof(UserCfgEntity));
	memset((void*)g_u8ArrModbusRegs,0,MAX_REGS*2);
	p_sys_cfg->u8sw_version		= 20;	
	p_sys_cfg->u8hw_version		= 30;
	GetDeviceID(p_sys_cfg->u8device_id);
		// 00-0F-E2	   H3c
	// 00-0f-f7	   cisio
	//00-80-E1   STMICROELECTRONICS
	//00-04-A3   Microchip Technology, Inc. (enc28j60)
	p_sys_cfg->u8mac[0] = 0x00;
	p_sys_cfg->u8mac[1] = 0x04;
	p_sys_cfg->u8mac[2] = 0xa3;
	p_sys_cfg->u8mac[3] = p_sys_cfg->u8device_id[9];
	p_sys_cfg->u8mac[4] = p_sys_cfg->u8device_id[10];
	p_sys_cfg->u8mac[5] = p_sys_cfg->u8device_id[11];
	GetDeviceID(p_sys_cfg->u8device_sn);
	p_sys_cfg->u32manu_time_day 	= (2017-1601)*12+6;
	p_sys_cfg->u32auth_hour				= 24*365*100;
	p_sys_cfg->u32used_minutes		= 0;
	p_sys_cfg->u8debug_sw					= 0xff;
	p_sys_cfg->s8oem_name[0]				= 's';
	p_sys_cfg->s8oem_name[1]				= 'j';
	p_sys_cfg->s8oem_name[2]				= '2';
	p_sys_cfg->s8oem_name[3]				= 0;
	p_sys_cfg->u16lane_num				= 6;

	p_user_cfg->u64device_time		= 0;
	sprintf(p_user_cfg->u8device_name,"VD4-%02x%02x%02x",p_sys_cfg->u8mac[3],p_sys_cfg->u8mac[4],p_sys_cfg->u8mac[5]);
	p_user_cfg->u8ip[0]				= 192;	// 服务器ip	   58.251.86.247:8008
	p_user_cfg->u8ip[1]				= 168;
	p_user_cfg->u8ip[2]				= 100;
	p_user_cfg->u8ip[3]				= 100;
	p_user_cfg->u16modbus_port			= 502;	// 端口
	p_user_cfg->u16http_port				= 80;
	p_user_cfg->u16udp_port					= 1200;
	p_user_cfg->u16uart_port				= 8001;
	p_user_cfg->u16rcv_timeout			= 300;	// 通信接收超时s
	p_user_cfg->u16statistic_interval	= 60;	// 统计周期	s
	p_user_cfg->u16loop_distance		= 300;	// 线圈距离m
	for(i=0;i<MAX_LOOP_NUM;i++)
	{
		p_user_cfg->u16in_sensitivity[i] 	= 20;
		p_user_cfg->u16out_sensitivity[i]	= 15;
	}
	
	p_user_cfg->u16scan_cycle			= 1;
	p_user_cfg->u16running_mode		= RUNNING_MODE_NORMAL;
	p_user_cfg->u16average_freq_stat_count	= 80;
	p_user_cfg->u16base_freq_stat_count			= 5000;
//	syslog(INFO,"init cfg data");
	#endif
}	




void ShowSetting()
{
#if 1
	int i;
	syslog(INFO,"sw ver:%d",p_sys_cfg->u8sw_version);
	syslog(INFO,"hw ver:%d",p_sys_cfg->u8hw_version);
	syslog(INFO,"device id:%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X",
		p_sys_cfg->u8device_id[0],p_sys_cfg->u8device_id[1],p_sys_cfg->u8device_id[2],p_sys_cfg->u8device_id[3],
		p_sys_cfg->u8device_id[4],p_sys_cfg->u8device_id[5],p_sys_cfg->u8device_id[6],p_sys_cfg->u8device_id[7],
		p_sys_cfg->u8device_id[8],p_sys_cfg->u8device_id[9],p_sys_cfg->u8device_id[10],p_sys_cfg->u8device_id[11]);
	syslog(INFO,"mac addr:%02X-%02X-%02X-%02X-%02X-%02X",
			p_sys_cfg->u8mac[0],p_sys_cfg->u8mac[1],p_sys_cfg->u8mac[2],
			p_sys_cfg->u8mac[3],p_sys_cfg->u8mac[4],p_sys_cfg->u8mac[5]);
	
	syslog(INFO,"sn:%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X",
		p_sys_cfg->u8device_sn[0],p_sys_cfg->u8device_sn[1],p_sys_cfg->u8device_sn[2],p_sys_cfg->u8device_sn[3],
		p_sys_cfg->u8device_sn[4],p_sys_cfg->u8device_sn[5],p_sys_cfg->u8device_sn[6],p_sys_cfg->u8device_sn[7],
		p_sys_cfg->u8device_sn[8],p_sys_cfg->u8device_sn[9],p_sys_cfg->u8device_sn[10],p_sys_cfg->u8device_sn[11]);
	
	syslog(INFO,"manu day:%u", p_sys_cfg->u32manu_time_day);
	syslog(INFO,"auth hour:%u", p_sys_cfg->u32auth_hour);
	syslog(INFO,"used time:%uminutes", p_sys_cfg->u32used_minutes);
	syslog(INFO,"dbg sw:%02X", p_sys_cfg->u8debug_sw);
	syslog(INFO,"oem name:%s", p_sys_cfg->s8oem_name);
	syslog(INFO,"lane num:%u", p_sys_cfg->u16lane_num);
	
	syslog(INFO,"device time:%u", p_user_cfg->u64device_time);	
	syslog(INFO,"device name:%s",p_user_cfg->u8device_name);
	syslog(INFO,"local ip:%u.%u.%u.%u:%u",p_user_cfg->u8ip[0], p_user_cfg->u8ip[1], 
				p_user_cfg->u8ip[2], p_user_cfg->u8ip[3],p_user_cfg->u16modbus_port);
	syslog(INFO,"modbus port: %u", p_user_cfg->u16modbus_port);
	syslog(INFO,"http port:%u", p_user_cfg->u16http_port);
	syslog(INFO,"udp port:%u", p_user_cfg->u16udp_port);
	syslog(INFO,"uart port:%u", p_user_cfg->u16uart_port);
	syslog(INFO,"rcv timeout:%useconds",p_user_cfg->u16rcv_timeout);
	syslog(INFO,"stat interval:%useconds", p_user_cfg->u16statistic_interval);
	syslog(INFO,"loop distance:%ucm", p_user_cfg->u16loop_distance);	
	for(i=0;i<MAX_LOOP_NUM;i++)
	{
		syslog(INFO,"sens[%d]: (%u,%u)",i,p_user_cfg->u16in_sensitivity[i],p_user_cfg->u16out_sensitivity[i]);
	}
	syslog(INFO,"scan cycle:%u", p_user_cfg->u16scan_cycle);						
	syslog(INFO,"run mode :%u", p_user_cfg->u16running_mode);						
	syslog(INFO,"average count:%u", p_user_cfg->u16average_freq_stat_count);						
	syslog(INFO,"base count:%u", p_user_cfg->u16base_freq_stat_count);				
#endif
}

void InitSetting()
{
	// 读出配置文件
   unsigned char buRet;
//	int i;
	ee_int_used_time_write_addr();
	buRet = LoadSetting();
	if(buRet == E_FAIL)
	{	
		// 初次启动		  tick	ip:port			  stat dist	vdt lcdt ver lane flag
		ShowSetting();
		InitConfigData();
		SaveConfig();
#if TRAFFIC_STAT_DBG
		PrintDebugInfo("First startup,init config ok.",INFO);
#endif
		buRet = LoadSetting();
	}
	p_sys_cfg->u32used_minutes = EE_ReadUsedTime();
  ShowSetting(); 
//	if(p_sys_cfg->u8mac[3]!=0x50 || p_sys_cfg->u8mac[4] != 0x33 || p_sys_cfg->u8mac[5]!=0x43)
//	{
//		 FLASH_Unlock();
//		FLASH_EraseAllPages();
//		while(1);
//		return;
//		while(1);
//		return;
//		while(1);
//		return;
//		while(1);
//		return;
//	}
}

unsigned char LoadSetting()
{
#if 1
	unsigned short nLen,crc,i;
	unsigned char* pBuf;
	unsigned char szBuffer[384] = {0};
	pBuf = g_u8ArrModbusRegs+MODBUS_RW_BUF_ADDR;
	nLen = MODBUS_RW_BUF_SIZE;//sizeof(g_u8ArrModbusRegs);
	syslog(INFO,"config file size 0x%X", nLen);

//	SPI_FLASH_BufferRead(szBuffer,CONFIG_ADDR,nLen+2);
 //	 FLASH_Unlock();

	EE_ReadBuf(szBuffer,nLen+2);
	syslog(DBG,"read from eeprom:%d",nLen+2);
	syslog(DBG,"copy buf");
	memcpy(pBuf,szBuffer,nLen);
//	p_sys_cfg->u8debug_sw = 0xff;
	syslog(DBG,"cale crc");
	i = CRC16(szBuffer,nLen);
	crc = szBuffer[nLen]<<8 | szBuffer[nLen+1];
	if( crc != i)
	{
#if 1
		syslog(INFO,"config crc err:%X->%X",crc,i);
		//PrintDebugInfo(g_strLog,INFO);
#endif
		return E_FAIL;
	}
	else
	{
#if 1
		syslog(INFO,"config crc OK",INFO);
#endif
		return S_OK;
	}
#endif
	return S_OK;
}

//unsigned int GetRcvTimeout(void)
//{
//	return p_user_cfg->u16rcv_timeout;	
//}

unsigned char GetDebugSw(void)
{
	 return p_sys_cfg->u8debug_sw;
}

void  GetMacAddr(unsigned char* MacAddrBuf)
{
	 	memcpy(MacAddrBuf,p_sys_cfg->u8mac,6);
}

void  GetIpAddr(unsigned char* IpAddrBuf)
{
		memcpy(IpAddrBuf,p_user_cfg->u8ip,4); 
}
//void  SetIpAddr(unsigned char* IpAddrBuf)
//{
//	// 	memcpy(p_user_cfg->u8ip,IpAddrBuf,4); 
//}
//unsigned short GetPort(void)
//{
//	return p_user_cfg->u16modbus_port;
//}


char SaveConfig(void)
{  
#if 1
	unsigned short nLen,crc;
//	int i;
	unsigned char* pBuf = g_u8ArrModbusRegs+MODBUS_RW_BUF_ADDR;
	unsigned char szBuffer[384] = {0};
	nLen = MODBUS_RW_BUF_SIZE;//sizeof(g_u8ArrModbusRegs);
	 

	crc = CRC16(pBuf,nLen);
	memcpy(szBuffer,pBuf,MODBUS_RW_BUF_SIZE);
	szBuffer[nLen] = crc>>8;
	szBuffer[nLen+1] = crc&0xff;   
  FLASH_Unlock();
	EE_WriteBuf(szBuffer,nLen+2);
	FLASH_Lock();
//	  printf("save to eeprom: \r\n");
//	for(i=0;i<nLen+2;)
//	{
//		printf("%02X ", szBuffer[i]);
//		i++;
//		if(0==i%8)
//			printf("\r\n");
//	}
//	printf("\r\n");
#endif
	syslog(INFO,"Save config,Len:%u, CRC:%X",nLen,crc);
	return 1;
}

int Get1(void)
{
	#if 0
	char buf[12];
	int i=0;
	Compute_SN(p_sys_cfg->device_id,buf);
	for(i=0;i<12;i++)
	{
		if(p_sys_cfg->device_sn[i]-buf[i])
			return 0;
	}
	#endif
	return 1;

}
int Get0(void)
{
	return 0;
}

