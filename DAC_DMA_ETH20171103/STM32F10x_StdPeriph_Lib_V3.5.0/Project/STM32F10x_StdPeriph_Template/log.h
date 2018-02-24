#ifndef LOG_H__
#define LOG_H__

#define ERR		0
#define WRN		1
#define INFO	2
#define DBG		3
void itm_write(const char* buf,int port);
void syslog(char pri, const char *fmt, ...);
#endif
