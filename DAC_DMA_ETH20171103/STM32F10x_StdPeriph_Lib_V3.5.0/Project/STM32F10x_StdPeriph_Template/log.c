#include "log.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "STM32F10x.h"
#include "AppConfig.h"
//#include "Application.h"
///#include "core_cm3.h"
#include "UartDrv.h"
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000
		
//__STATIC_INLINE uint32_t ITM_TraceChar(uint32_t ch,int port)
//{
//	if (DEMCR & TRCENA) {
//        while (ITM_Port32(port) == 0);
//        ITM_Port8(port) = ch;
//      }
//      return(ch);
//}

#define BIT_SET(x,n) (x=x | (0x01<<n))  	// ?x?n??1
#define BIT_TEST(x,n) ((x & (0x01<<n))!=0)  // ??x?n????1
#define BIT_CLEAR(x,n) (x=x & ~(0x01<<n))  	// ?x?n???

#if 0
void itm_write(const char* buf,int port)
{
	return ;
	do
	{
		ITM_SendChar(*buf++);
	}while(*buf);
	
}
    struct __FILE { int handle; /* Add whatever you need here */ };
    FILE __stdout;
    FILE __stdin;

  
    int fputc(int ch, FILE *f) {
      if (DEMCR & TRCENA) {
        while (ITM_Port32(0) == 0);
        ITM_Port8(0) = ch;
      }
      return(ch);
    }
#endif
//PUTCHAR_PROTOTYPE
//{
//	if (DEMCR & TRCENA) {
//		while (ITM_Port32(0) == 0);
//		ITM_Port8(0) = ch;
//	}
//	return(ch);
//}
		
//static unsigned int LogMask = 0xff;        /* mask of priorities to be logged */
const char LOG_PRI_STR[][8]	= {"[err]\t","[warn]\t","[info]\t", "[debug]\t"};

uint8_t GetDebugSw()
{
	return 0xff;
}
void PrintDebugInfo(char* strLog,char type)   
{

#if 1
	if(DBG == type)
	{
		if(BIT_TEST(GetDebugSw(),DBG))
		{
			SendString1("D\t");
	//		while(1);
		}
		else
			return;
	}
	else if(INFO == type)
	{
		if(BIT_TEST(GetDebugSw(),INFO))
			SendString1("P\t");
		else
			return;
	}
	else if(WRN == type)
	{
		if(BIT_TEST(GetDebugSw(),WRN))
			SendString1("W\t");
		else
			return;
	}
	else if(ERR == type)
	{
		if(BIT_TEST(GetDebugSw(),ERR))
			SendString1("E\t");
		else
			return;
	}
	SendString1(strLog);
	SendString1("\r\n");
#endif
//	printf("%s\r\n",strLog);
}
char etxt[DEBUG_MSG_SIZE]; /* text buffer */ 
void syslog(char type, const char *fmt, ...)
//void DebugOut(char type,char* fmt,...) 
{
	va_list arg_ptr;
	va_start (arg_ptr, fmt); /* format string */
	vsprintf (etxt, fmt, arg_ptr);
	va_end (arg_ptr);
	PrintDebugInfo(etxt,type);   
}
