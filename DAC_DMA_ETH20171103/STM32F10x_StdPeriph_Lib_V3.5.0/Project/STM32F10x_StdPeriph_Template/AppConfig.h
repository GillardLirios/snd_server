#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__

#define MAX_LANE_NUM 		6
#define MAX_LOOP_PER_LANE	2
#define MAX_LOOP_NUM 		MAX_LOOP_PER_LANE*MAX_LANE_NUM

#define NOT_FIRST_STARTUP	0x55
#define MAX_REGS	224

#define RUNNING_MODE_TEST 	0x88
#define RUNNING_MODE_NORMAL	0x66

// regs
// RO
#define MODBUS_RO_REG_ADDR					0
#define MODBUS_RO_REG_SIZE					131

// RW
#define MODBUS_RW_REG_ADDR					132
#define MODBUS_RW_REG_SIZE					76

// Lane info
#define MODBUS_REG_LANE_INFO_ADDR		0
#define MODBUS_REG_LANE_INFO_SIZE		44
// Loop info
#define MODBUS_REG_LOOP_INFO_ADDR		44
#define MODBUS_REG_LOOP_INFO_SIZE		88
// User config
#define MODBUS_REG_USER_CFG_ADDR		132
#define MODBUS_REG_USER_CFG_SIZE		56 
//40
// Sys config
#define MODBUS_REG_SYS_CFG_ADDR			(172+16)
#define MODBUS_REG_SYS_CFG_SIZE			36

// RO
#define MODBUS_RO_BUF_ADDR					(MODBUS_RO_REG_ADDR*2)
#define MODBUS_RO_BUF_SIZE					(MODBUS_RO_REG_SIZE*2)

// RW
#define MODBUS_RW_BUF_ADDR					(MODBUS_RW_REG_ADDR*2)
#define MODBUS_RW_BUF_SIZE					(MODBUS_RW_REG_SIZE*2)

// Lane info
#define MODBUS_BUF_LANE_INFO_ADDR		(MODBUS_REG_LANE_INFO_ADDR*2)
#define MODBUS_BUF_LANE_INFO_SIZE		(MODBUS_REG_LANE_INFO_SIZE*2)
// Loop info
#define MODBUS_BUF_LOOP_INFO_ADDR		(MODBUS_REG_LOOP_INFO_ADDR*2)
#define MODBUS_BUF_LOOP_INFO_SIZE		(MODBUS_REG_LOOP_INFO_SIZE*2)
// User config
#define MODBUS_BUF_USER_CFG_ADDR		(MODBUS_REG_USER_CFG_ADDR*2)
#define MODBUS_BUF_USER_CFG_SIZE		(MODBUS_REG_USER_CFG_SIZE*2)
// Sys config
#define MODBUS_BUF_SYS_CFG_ADDR			(MODBUS_REG_SYS_CFG_ADDR*2)
#define MODBUS_BUF_SYS_CFG_SIZE			(MODBUS_REG_SYS_CFG_SIZE*2)


// 车道信息
typedef struct
{
	unsigned short  u16flow[MAX_LANE_NUM];							// 0
	unsigned short 	u16speed[MAX_LANE_NUM];							// 2
	unsigned short	u16occupancy[MAX_LANE_NUM];					// 4
	unsigned short 	u16headway[MAX_LANE_NUM];						// 6
	unsigned short 	u16retrograde_flow[MAX_LANE_NUM];		// 8
	unsigned short 	u16retrograde_speed[MAX_LANE_NUM];	// 10
	short 			s16instant_speed[MAX_LANE_NUM];			// 12
	signed char	s8last_dectect_lane;
	unsigned char	u8flow_of_last_dectect_lane;
	short	s16inst_speed_of_last_dectect_lane;
}LaneInfoEntity;

// 线圈信息
typedef struct
{
	unsigned int u32instant_freq[MAX_LOOP_NUM];				// 当前频率
	unsigned int u32average_freq[MAX_LOOP_NUM];				// 平均频率（Hz）
	unsigned int u32base_freq[MAX_LOOP_NUM];	   			// 基频（Hz）
	unsigned short u16loop_status[MAX_LOOP_NUM];			// 通道状态：0 未探测到金属，1 已探测到   11未初始化	无线圈
	unsigned short s16reservd[4];
}LoopInfoEntity;


typedef struct
{
	unsigned long long u64device_time;
	unsigned char  u8device_name[24];					// 设备名称			32
	unsigned char	 u8ip[4];						// ip				4
	unsigned short u16modbus_port;
	unsigned short u16http_port;
	unsigned short u16udp_port;
	unsigned short u16uart_port;
	unsigned short u16rcv_timeout;
	unsigned short u16statistic_interval;	
	unsigned short u16loop_distance;
	unsigned short u16in_sensitivity[12];
	unsigned short u16out_sensitivity[12];
	unsigned char  u16scan_cycle;
	unsigned char  u16running_mode;
	unsigned short u16average_freq_stat_count;
	unsigned short u16base_freq_stat_count;
	unsigned short s16reservd[2];
	unsigned int	 u32unlock;
}UserCfgEntity;
 
typedef struct
{
	unsigned char	u8sw_version;
	unsigned char	u8hw_version;
	unsigned char	u8mac[6];						// 本机mac地址
	unsigned char u8device_id[12];
	unsigned char	u8device_sn[12];

	unsigned int 	u32manu_time_day;		// from 1601
	unsigned int 		u32auth_hour;
	unsigned int 	 	u32used_minutes;
	unsigned char		u8debug_sw;
	unsigned char		u8reservd;
	unsigned short  u16reservd[2];
	unsigned char 	s8oem_name[4];
	unsigned short	u16lane_num;	
	unsigned short  u16reservd2[5];
	unsigned int	  u32rand;
	unsigned int	  u32unlock;
}SysCfgEntity;


void InitConfigData(void);
char SaveConfig(void);
void InitSetting(void);
void ShowSetting(void);
unsigned char LoadSetting(void);
unsigned int GetRcvTimeout(void);
int get_lane_num(void);
unsigned char GetDebugSw(void);
void GetMacAddr(unsigned char* MacAddrBuf);
void GetIpAddr(unsigned char* IpAddrBuf);
unsigned short GetPort(void);
int Get1(void);
int Get0(void);

#endif
