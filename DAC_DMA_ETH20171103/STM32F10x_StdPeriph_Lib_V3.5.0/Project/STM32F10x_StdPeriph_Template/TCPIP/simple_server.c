/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface 
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Title: Microchip ENC28J60 Ethernet Interface Driver
 * Chip type           : ATMEGA88 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/


/*********************************************
 * modified: 2007-08-08
 * Author  : awake
 * Copyright: GPL V2
 * http://www.icdev.com.cn/?2213/
 * Host chip: 
**********************************************/
#include<stdlib.h>
#include "simple_server.h"
#include "ip_arp_udp_tcp.h"
#include "ENC28J60.h"
#include "net.h"
#include <stdio.h>
#include <string.h>
#include <arr_fifo.h>
//#include "DataEntity.h"
//#include "rtc_calendar.h"
//#include "bsp.h"
//#include "protocol.h"
//#include "ADCDrv.h"
//#include "LoopDetectorBll.h"
//#include "AppConfig.h"
#include "log.h"
extern char g_bIsRecvTimeOut;
//simple_server.c
#define PSTR(s) (const unsigned char*)(s)
#define DEVICE_ID	1
// a DNS server (baseurl must end in "/"):
static char baseurl80[] = "http://192.168.1.100/";
//static char baseurl502[] = "http://192.168.1.100:502/";
//static u16 mywwwport = 80; // listen port for tcp/www (max range 1-254)
// or on a different port:
//static char baseurl[]="http://10.0.0.24:88/";
//static u16 mywwwport =88; // listen port for tcp/www (max range 1-254)
//
static u16 myudpport = 1200; // listen port for udp
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX

#define BUFFER_SIZE 1500 
//400
static u8 buf[BUFFER_SIZE];
uint8_t u8ip[4] = {192,168,22,100};
// the password string (only the first 5 char checked), (only a-z,0-9,_ characters):
static char password[] = "888"; // must not be longer than 9 char


// 
u8 verify_password(char* str)
{
   // the first characters of the received string are
   // a simple password/cookie:
   if (strncmp(password, str, sizeof(password)-1) == 0)
   {
      return(1);
   }
   return(0);
}

// takes a string of the form password/commandNumber and analyse it
// return values: -1 invalid password, otherwise command number
//                -2 no command given but password valid
u8 analyse_get_url(char* str)
{
   u8 i = 0;
   if (verify_password(str) == 0)
   {
      return((u8)-1);
   }
   // find first "/"
   // passw not longer than 9 char:
   while (*str && i<10 && *str>',' && *str < '{')
   {
      if (*str == '/')
      {
         str++;
         break;
      }
      i++;
      str++;
   }
   if (*str <0x3a && *str> 0x2f)
   {
      // is a ASCII number, return it
      return(*str - 0x30);
   }
   return((u8)-2);
}


u16 PrintDeviceStatus(u8* buf, u8 on_off)
{
	u16 plen;
	int i;
	char szBuf[256];	
	
	#if 1
	//GetDeviceID(&g_eConfig.device_id);
	plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	sprintf(szBuf,"<html><head><title>VD[%d.%d.%d.%d]</title><meta http-equiv=\"refresh\" content=\"1\"></head><body>",
					u8ip[0],u8ip[1],u8ip[2],u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888\"><b>[首页]</b></a>",
				u8ip[0],u8ip[1],u8ip[2],u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
  plen = fill_tcp_data_p(buf, plen, PSTR("</table></body></html>"));
	#endif
	syslog(DBG,"%s,len=%d",__FUNCTION__,plen);
   return(plen);
}
#if 0
u16 PrintLoopStatus(u8* buf)
{
	u16 plen;
	int i;
	char szBuf[256];	
	rtc_time  tm;
	
	#if 1
	plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	sprintf(szBuf,"<html><head><title>VD[%d.%d.%d.%d]</title><meta http-equiv=\"refresh\" content=\"1\"></head><body>",
					p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888\"><b>[首页]</b></a>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
//	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888/0\"><b>[线圈]</b></a>",
//				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
//   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
//		sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888/1\"><b>[灵敏度]</b></a><p>",
//				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
//   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	// 时间
	to_tm(p_user_cfg->u64device_time/1000,&tm);
	sprintf(szBuf,"<p>线圈状态[%02d:%02d:%02d]:<br/>",
						tm.tm_hour,tm.tm_min,tm.tm_sec);
   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
   plen = fill_tcp_data_p(buf, plen, PSTR("<table border=\"1\"><tr><td>线圈</td><td>基准(Hz)</td><td>平均(Hz)</td><td>进灵敏度(1/1000)</td><td>出灵敏度(1/1000)</td><td>线圈状态</td></tr>"));
   for(i=0; i<p_sys_cfg->u16lane_num*2; i++)
  {
	   sprintf(szBuf,"<tr><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>%s</td></tr>",
	   		i+1,GetBaseFreq(i),GetAverageFreq(i),p_user_cfg->u16in_sensitivity[i],p_user_cfg->u16out_sensitivity[i],(GetLoopState(i)!=NO_LOOP) ? "正常" :"断开");
	   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	}
//	sprintf(szBuf,"</table><a href=\"http://%d.%d.%d.%d/888\">[back]</a><p>",
//				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	//plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
   plen = fill_tcp_data_p(buf, plen, PSTR("</body></html>"));
	#endif
	syslog(DBG,"%s,len=%d",__FUNCTION__,plen);
   return(plen);
}

u16 PrintLoopSetting(u8* buf,uint16_t page)
{
	u16 plen;
	int i;
	char szBuf[256];	
//	rtc_time  tm;
	#if 1
	plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	sprintf(szBuf,"<html><head><title>VD[%d.%d.%d.%d]</title></head><body>",
					p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888\"><b>[首页]</b></a>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888/0\"><b>[线圈]</b></a>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888/1\"><b>[灵敏度]</b></a><p>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	// 时间
//	to_tm(GetTickCount(),&tm);
//	sprintf(szBuf,"Loop[%02d:%02d:%02d]:<br/>",
//						tm.tm_hour,tm.tm_min,tm.tm_sec);
//   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
		sprintf(szBuf,"<form name=\"sens_set\" action=\"%d\" method=\"get\">",1==page?3:4);
		plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
   plen = fill_tcp_data_p(buf, plen, PSTR("<table border=\"1\"><tr><td>线圈</td><td>进灵敏度(1/1000)</td><td>出灵敏度(1/1000)</td></tr>"));
   if(page==2 && p_sys_cfg->u16lane_num*2>6)
		 i = 6;
	 else
		 i = 0;
	 for(; i<(page==1?p_sys_cfg->u16lane_num*2/2:p_sys_cfg->u16lane_num*2); i++)
  {
	   sprintf(szBuf,"<tr><td>%d</td><td><input type=\"text\" name=\"si%d\" value=\"%d\" /></td><td><input type=\"text\" name=\"so%d\" value=\"%d\" /></td></tr>",
	   		i+1,i,p_user_cfg->u16in_sensitivity[i],i,p_user_cfg->u16out_sensitivity[i]);
	   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	}
	sprintf(szBuf,"<input type=\"submit\" value=\"保存\"/></form><br/>\r\n");
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"</table><a href=\"http://%d.%d.%d.%d/888/%d\">[%s]</a><p>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3],
				page==1?2:1,page==1?"下一页":"上一页");
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
   plen = fill_tcp_data_p(buf, plen, PSTR("</body></html>"));
	#endif
	syslog(DBG,"%s,len=%d",__FUNCTION__,plen);
	
   return(plen);
}

u16 PrintSysSetting(u8* buf)
{
	u16 plen;
//	int i;
	char szBuf[256];	
	rtc_time  tm;
	syslog(DBG,"%s, in",__FUNCTION__);
	#if 1
	plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	sprintf(szBuf,"<html><head><title>VD[%d.%d.%d.%d]</title></head><body>",
					p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888\"><b>[首页]</b></a>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888/0\"><b>[线圈]</b></a>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	sprintf(szBuf,"<a href=\"http://%d.%d.%d.%d/888/1\"><b>[灵敏度]</b></a><p>",
				p_user_cfg->u8ip[0],p_user_cfg->u8ip[1],p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
	// 时间
	to_tm(p_user_cfg->u64device_time/1000,&tm);
	sprintf(szBuf,"系统设置[%02d:%02d:%02d]:<br/>",
						tm.tm_hour,tm.tm_min,tm.tm_sec);
   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
		sprintf(szBuf,"<form name=\"sys_set\" action=\"6\" method=\"get\">");
		plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
   plen = fill_tcp_data_p(buf, plen, PSTR("<table border=\"1\">"));
 
	   sprintf(szBuf,"<tr><td>本机IP</td><td><input type=\"text\" name=\"ip0\" value=\"%d\" /></td><td><input type=\"text\" name=\"ip1\" value=\"%d\" /></td>",
	   		p_user_cfg->u8ip[0],p_user_cfg->u8ip[1]);
	   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
		 sprintf(szBuf,"<td><input type=\"text\" name=\"ip2\" value=\"%d\" /></td><td><input type=\"text\" name=\"ip3\" value=\"%d\" /></td></tr></table>",
	   		p_user_cfg->u8ip[2],p_user_cfg->u8ip[3]);
	   plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));
		 //<td><input type=\"text\" name=\"so%d\" value=\"%d\" /></td>
	
	sprintf(szBuf,"<input type=\"submit\" value=\"保存\"/></form><br/>\r\n");
	plen = fill_tcp_data_p(buf, plen, PSTR(szBuf));

   plen = fill_tcp_data_p(buf, plen, PSTR("</body></html>"));
	#endif
	syslog(DBG,"%s,len=%d",__FUNCTION__,plen);
	
   return(plen);
}
#endif
int InitServer(void)
{
   /*initialize enc28j60*/
	 uint8_t u8device_id[12];
	
	uint16_t u16http_port = 80;
	memcpy(u8device_id,(void*)(0x1ffff7e8),12);
	uint8_t u8mac[6];
	u8mac[0] = 0x00;
	u8mac[1] = 0x04;
	u8mac[2] = 0xa3;
	u8mac[3] = u8device_id[9];
	u8mac[4] = u8device_id[10];
	u8mac[5] = u8device_id[11];
  enc28j60Init(u8mac);
	
   enc28j60getrev();
   
   init_ip_arp_udp_tcp(u8mac, u8ip, u16http_port);
   //指示灯状态:0x476 is PHLCON LEDA(绿)=links status, LEDB(红)=receive/transmit
//   enc28j60PhyWrite(PHLCON, 0x7a4); 
   enc28j60PhyWrite(PHLCON, 0x476);
 //  enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
	syslog(DBG,"");
   return 0;
}

#if PCM8U
extern uint16_t g_u32dac12bit_buf0[640]; 	
extern uint16_t g_u32dac12bit_buf1[640]; 	
extern uint16_t g_u32dac12bit_buf3[640]; 	
#else
extern uint32_t g_u32dac12bit_buf0[320]; 	
extern uint32_t g_u32dac12bit_buf1[320]; 	
extern uint32_t g_u32dac12bit_buf3[320]; 	
#endif
extern uint32_t buf_index;
extern __IO uint32_t buf_ready[2];
uint8_t pkt_idx=0;
extern fifo_t pcm_fifo;
uint32_t sdz = 0;
int rcv_pkt_cnt=0;
int lost_pkt_cnt= 0;
extern int32_t g_rcv_time_out;
int RunNetSevice(void)
{
	u16 plen,payloadlen;
	u8 cmd;
	u8 buf1[256];
	// get the next new packet:
	plen = enc28j60PacketReceive(BUFFER_SIZE, buf);

	/*plen will ne unequal to zero if there is a valid packet (without crc error) */
	if (plen == 0)
	{
	 	return 0;
	}

	// arp is broadcast if unknown but a host may also
	// verify the mac address by sending it to 
	// a unicast address.
	if (eth_type_is_arp_and_my_ip(buf, plen))
	{
	//	syslog(DBG,"recv ARP:%d",plen);
		make_arp_answer_from_request(buf);
		syslog(DBG,"send ARP");
		return 0;
	}
	
	// check if ip packets are for us:
	if (eth_type_is_ip_and_my_ip(buf, plen) == 0)
	{
	//	syslog(DBG,"recv other ip data:%d",plen);
		return 0;
	}
//	syslog(DBG,"net recv:%d",plen);	
	
	if (buf[IP_PROTO_P] == IP_PROTO_ICMP_V &&
	 buf[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V)
	{
		syslog(DBG,"recv ICMP:%d",plen);
		// a ping packet, let's send pong	
		make_echo_reply_from_request(buf, plen);
//		syslog(DBG,"send ICMP");
		//USART_DMASendText(USART1,"make_echo_reply_from_request\n");
		return 0;
	}
	#if 1
	u16 dat_p;
	if (buf[IP_PROTO_P] == IP_PROTO_TCP_V &&
	 buf[TCP_DST_PORT_H_P] == 0 &&
	 buf[TCP_DST_PORT_L_P] == 80)
	{
			syslog(DBG,"tcp recv:%d",plen);
		if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
		{
			make_tcp_synack_from_syn(buf);
			// make_tcp_synack_from_syn does already send the syn,ack
			return 0;
		}
		if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
		{
			init_len_info(buf); // init some data structures
			// we can possibly have no data, just ack:
			dat_p = get_tcp_data_pointer();
			if (dat_p == 0)
			{
				if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
				{
					make_tcp_ack_from_any(buf);
				}
				 // just an ack with no data, wait for next packet
				 return 0;
			}
			if (strncmp("GET ", (char *) &(buf[dat_p]), 4) != 0)
			{
				 plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
				 goto SENDTCP;
			}
			if (strncmp("/ ", (char *) &(buf[dat_p + 4]), 2) == 0)
			{
				 plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
				 plen = fill_tcp_data_p(buf, plen, PSTR("<p>Usage: "));
				 plen = fill_tcp_data(buf, plen, baseurl80);
				 plen = fill_tcp_data_p(buf, plen, PSTR("password</p>"));
				 goto SENDTCP;
			}
			cmd = analyse_get_url((char *) &(buf[dat_p + 5]));
			// for possible status codes see:	
			if (cmd ==(u8)-1)
			{
				 plen = fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
				 goto SENDTCP;
			}

				plen = PrintDeviceStatus(buf, 0);
			
			
SENDTCP : 
			make_tcp_ack_from_any(buf); // send ack for http get
			make_http_ack_with_data(buf, plen); // send data
			return 0;
		}
	}
	if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0)
	{
		g_rcv_time_out = 0;
		payloadlen=	  buf[UDP_LEN_H_P];
		payloadlen=payloadlen<<8;
		payloadlen=(payloadlen+buf[UDP_LEN_L_P])-UDP_HEADER_LEN;	
//	syslog(DBG,"recv UDP(1200):%d",payloadlen);		
		if(1314== payloadlen && buf[UDP_DATA_P+0]==0x5a)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
//			if(buf[UDP_DATA_P+1] == pkt_idx ||
//					( buf[UDP_DATA_P+1] == pkt_idx-1) ||
//					( buf[UDP_DATA_P+1] == 255 && pkt_idx==0 )
//			)
//			{
//				putchar('.');
//				return 0;
//			}
//			else
			
			if(buf[UDP_DATA_P+1] != ((pkt_idx+1)&0xff))
			{
				if(buf[UDP_DATA_P+1]>pkt_idx)
				{
					lost_pkt_cnt = buf[UDP_DATA_P+1]-pkt_idx-1;	
				}
				else
				{
					lost_pkt_cnt =255+buf[UDP_DATA_P+1]-pkt_idx-1;
				}
				syslog(DBG,"pkt lost:%d, last=%d,current=%d",
						lost_pkt_cnt, pkt_idx, buf[UDP_DATA_P+1]);
				
			}
			pkt_idx = buf[UDP_DATA_P+1];
			rcv_pkt_cnt++;
			if(buf[UDP_DATA_P+DEVICE_ID/8+2] & (1<<(DEVICE_ID%8)))
			{
				//putchar('~');
				if(sdz == 1)
				{
					GPIO_ResetBits(GPIOC,GPIO_Pin_6);
					sdz = 0;
					printf("PA on\r\n");
				}
				#if 1
				if(FIFO_ERROR==fifo_push_multiple(&pcm_fifo, buf+UDP_DATA_P+34, 1280))
				{
					printf("fifo full\r\n");
				}
				
				#else
				if(buf_index)
				{
					memcpy((unsigned char*)g_u32dac12bit_buf0, buf+UDP_DATA_P+34, 1280);
					printf("w0");
				}
				else
				{
					memcpy((unsigned char*)g_u32dac12bit_buf1, buf+UDP_DATA_P+34, 1280);
					printf("w1");
				}
				#endif
			}
			else
			{
				putchar('X');
				if(sdz == 0)
				{
					GPIO_SetBits(GPIOC,GPIO_Pin_6);
					sdz = 1;
					printf("PA off\r\n");
				}
				#if 0
				memcpy((unsigned char*)g_u32dac12bit_buf0, buf+UDP_DATA_P+34, 1280);
				#else
				if(buf_index)
				{
					memcpy((unsigned char*)g_u32dac12bit_buf0, g_u32dac12bit_buf3, 1280);
				}
				else
				{
					memcpy((unsigned char*)g_u32dac12bit_buf1, g_u32dac12bit_buf3, 1280);
				}
				#endif
			}	
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			//sprintf(buf1,"reply 1314\r\n");
			//make_udp_reply_from_request(buf,buf1,strlen(buf1),myudpport);
			return 1280;
		}
		else
		{
			sprintf(buf1,"{DEV_ID:%d},{LOST:%0.2f%%}\0",DEVICE_ID,lost_pkt_cnt*100.0/(lost_pkt_cnt+rcv_pkt_cnt));
			lost_pkt_cnt = 0;
			rcv_pkt_cnt = 0;
			syslog(DBG,"recv UDP(1200):%d, reply:%s",plen,buf1);		
			make_udp_reply_from_request(buf,buf1,strlen(buf1),myudpport);
			return payloadlen;
		}
	}
	else
	{
		//syslog(DBG,"unkown data,dst port:%x,%x",buf[TCP_DST_PORT_H_P],buf[TCP_DST_PORT_L_P]);	
	}
	#endif
	return 0;
}
