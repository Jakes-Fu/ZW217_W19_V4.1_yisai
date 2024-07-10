#ifdef HERO_ENGINE_SUPPORT

#ifndef __DSM_API_H__
#define __DSM_API_H__

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char U8;
//typedef int int32;
//typedef unsigned int uint32;
typedef unsigned char uint8;
typedef unsigned char kal_uint8;
typedef unsigned short int kal_uint16;
//typedef unsigned char BOOL;

typedef unsigned long int gunichar;
typedef char   gchar;
typedef unsigned int    guint;
typedef unsigned char   guchar;
typedef unsigned short  gushort;
typedef unsigned long   gulong;
typedef unsigned int    guint;

#define	VOID	void

#ifndef MAX
#define MAX(x, y)           (((x) > (y))?(x):(y))
#endif

#ifndef MIN
#define MIN(x, y)           (((x) < (y))?(x):(y))
#endif

#ifndef ABS
#define ABS(x)              (((x)<0) ? -(x) : (x))
#endif

#ifndef DIV
#define DIV(x, y)		((x) / (y))
#endif

#ifndef MOD
#define MOD(x, y)		((x) % (y))
#endif

typedef enum HERO_HTTP_TYPE
{
	HERO_HTTP_TYPE_CMNET,
	HERO_HTTP_TYPE_CMWAP,
}HERO_HTTP_TYPE;

typedef enum
{
	HERO_SOC_SUCCESS           = 0,     /* success */
	HERO_SOC_ERROR             = -1,    /* error */
	HERO_SOC_WOULDBLOCK        = -2,    /* not done yet */
}hero_socket_state;

typedef enum
{
	HERO_SOC_STATE_CONNECTING,
	HERO_SOC_STATE_CONNECTED,
	HERO_SOC_STATE_CLOSED,
	HERO_SOC_STATE_CONNECT_FALIED = -1,
	DSM_SOC_STATE_CONNECT_TIME_OUT = -2
}hero_socket_connect_state;

#define HERO_SIM_NONE       (0)
#define HERO_SIM_CARD1   (1<<0)
#define HERO_SIM_CARD2   (1<<1)
#define HERO_SIM_CARD3    (1<<2)
#define HERO_SIM_CARD4   (1<<3)
#define HERO_SIM_WIFI   (1<<5)

/*
* 初始化SIM信息
*/
typedef void(*init_sim_info_cb)(int ret);
void hero_init_sim_card_info(init_sim_info_cb cb);

/*
* 获取活动的SIM卡
*/
unsigned int hero_get_action_sim();

/*
* 获取活动的SIM卡IMSI号
*/
char *hero_get_action_sim_imsi();

/*
* 获取活动的SIM卡IMEI号
*/
char *hero_get_action_sim_imei();

void hero_get_manufactory(char *manufactory,int bufLen);

void hero_get_type(char *type,int bufLen);

/*
* 屏幕宽
*/
int hero_get_screen_width();

/*
* 屏幕高
*/
int hero_get_screen_height();


typedef void(*init_nwk_account_cb)(int ret);
/*
* 设置程序的网络账号
*/
BOOL hero_init_nwk_account(HERO_HTTP_TYPE conntype,init_nwk_account_cb cb);

/*
* 释放程序的网络账号
*/
void hero_free_nwk_account();

/*
* 创建一个socket
*/
signed int hero_socket_create();


typedef unsigned char(*socket_connect_cb)(hero_socket_connect_state state,int socId);
/*
* socket 连接
*/
int hero_socket_connect(int s, int ip, unsigned short port,socket_connect_cb cb);

/*
* socket 接收
*/
int hero_socket_recv(int s, const char *buf, int len);

/*
* socket 发送
*/
int hero_socket_send(int s, const char *buf, int len);

/*
* socket 关闭
*/
int hero_socket_close(int s);


typedef int(*hero_dsn_cb)(int result);
/*
* 获取域名IP
*/
int hero_get_host_by_name(const char *name, hero_dsn_cb cb);


//////////////////////////////////////////////////

typedef void(*sh_timer_cb)(void *p);

#define HERO_MAX_TIMER_COUNT   (20)

/*
* 创建一个timer
*/
int hero_create_timerId();

/*
*删除一个timer
*/
void hero_delete_timer(int timerId);

/*
*停止一个timer
*/
void hero_stop_timer(int timerId);

/*
*启动一个timer
*/
void hero_start_timer(int timerId,unsigned int delay,sh_timer_cb cb,void *userData);


/****************************************************
*					内存相关							*
*****************************************************/
//void *hero_malloc(unsigned int size);

//void hero_free(char **pptr);

int hero_get_mem_pool(char** mem_base, unsigned int* mem_len);

/****************************************************
*					短信相关							*
*****************************************************/

/*
* 发送短信
*/
enum{
	HERO_SMS_TYPE_7BIT = 0,
	HERO_SMS_TYPE_UCS2,
	HERO_SMS_TYPE_8BIT,
	HERO_SMS_TYPE_MAX
};
typedef void(*send_sms_cb)(int ret,void *userData);
int hero_send_sms(char *pNumber,char *pContent,int contentLen,int smsType,int active_sim,send_sms_cb cb,void *userData,int port);

void hero_sms_remove(unsigned short msg_id);


/****************************************************
*					文件相关							*
*****************************************************/
#define HERO_FILE_RDONLY     1 //以只读的方式打开文件。
#define HERO_FILE_WRONLY     2 //以只写的方式打开文件。
#define HERO_FILE_RDWR       4 //以读写的方式打开文件。
#define HERO_FILE_CREATE     8 //如果文件不存在，创建该文件。
#define HERO_FILE_SHARD      16
#define HERO_FILE_COMMITTED  32

enum
{
   HERO_SEEK_SET,
   HERO_SEEK_CUR,
   HERO_SEEK_END
};

#define HERO_IS_FILE     1      //文件
#define HERO_IS_DIR      2      //目录
#define HERO_IS_INVALID  8  //无效(非文件、非目录)

void hero_init_file_drv();
int hero_file_create_dir(const char *dir,char isHidden);
int hero_file_open(const char* filename,unsigned int mode);
int hero_file_write(int f,void *p,unsigned int l);
int hero_file_read(int f,void *p,unsigned int l);
int hero_file_seek(int f,int pos,int method);
int hero_file_close(int f);
int hero_get_file_size(const char* filename);
int hero_get_file_info(const char* filename);
int hero_file_remove(const char* filename);

/*
* 背光灯是否开启
*/
char hero_is_back_light_on();

/*
* 获取系统时间
*/
typedef struct hero_mytimeSt
{
    short nYear;
    char nMonth;
    char nDay;
    char nHour;
    char nMin;
    char nSec;
    char DayIndex; /* 0=Sunday */
} hero_mytime;

void hero_get_date_time(hero_mytime *ptime);

/*
* 闹钟相关
*/
typedef enum
{
    HERO_REMINDER_NOTIFY_INIT,   //初始化(开机)
    HERO_REMINDER_NOTIFY_EXPIRY, // 事件触发
    HERO_REMINDER_NOTIFY_REINIT, // 重新初始化(比如:重置时间)
} hero_reminder_notify_enum;
/*
* 闹钟事件回调
*/
void hero_evt_cb(hero_reminder_notify_enum status,unsigned int usr_data);

/*
* 适配层接口
*/
int hero_event_set(const hero_mytime *expiry_time,unsigned int usr_data);

int hero_event_cance(unsigned int usr_data);

void hero_sand(unsigned int seed);

unsigned int hero_rand(void);

typedef struct tagSmsInputData{
	int appid;
	int appVersion;
	int fid;
	int fre;
	int isTest;
}smsInputData;

typedef struct tagSmsResultSt{
	int appid;
	int index;
	int count;
}smsResultSt;

void hero_start_dsm_app_req(smsInputData *inputData);

typedef struct tagAppInputData{
	int appid;
	int appVersion;
	int fid;
	int fre;
	int isTest;
	char *extData;
	int extDataLen;
}appInputData;

void hero_start_app_req(appInputData *inputData);

int hero_get_dsm_app_version();

void hero_dt_ticker();

void hero_send_sms_event(int ret, int retData);

void hero_send_heart_beat(int code);

int hero_set_homepage(char *homepage);

//modify 20181117 begin
#define NVRAM_HERO_APP_CACHE_SIZE		64

int32 hr_start_dsm_app_req_x(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hr_get_dsm_app_version_x(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hero_start_hero_app_req_ex(uint8 * input, int32 input_len, uint8 ** output, int32 * output_len, HR_PLAT_EX_CB * cb);

//modify 20181129
enum{
	HERO_APP_TEXT_VIEW_CANCEL,
	HERO_APP_TEXT_VIEW_OK,
	HERO_APP_TEXT_VIEW_UPDATE,
	HERO_APP_TEXT_VIEW_EXIT,
};
typedef void(*hero_app_text_view_callback)(int type, void *userData);

void hero_app_update_text_view(int handle, unsigned short *text, void *userData);

void hero_app_close_text_view(int handle);

int hero_app_can_show_tip();

int hero_app_open_text_view(int toolbarType, unsigned short *text, hero_app_text_view_callback callback, void *userData);
//modify 20181129

////////////////////////////////////////////////////////////////hero app////////////////////////////////////////////////////////////////
void hero_app_init_file_drv();

int hero_app_file_create_dir(const char *dir, char isHidden);

int hero_app_file_write(int f,void *p,unsigned int l);

int hero_app_file_read(int f,void *p,unsigned int l);

int hero_app_file_seek(int f, int pos, int method);

int hero_app_file_open(const char* filename, unsigned int mode);

int hero_app_file_close(int f);

int hero_app_get_file_info(const char* filename);

void hero_app_get_date_time(hero_mytime *ptime);

/**
*key list 数据必须为uncode编码格式
**/
int hero_app_add_key(void* input, int input_len);

int hero_app_check_sms(unsigned char *pContent, int nLen, char *pNum);

int hero_app_get_version();

int hero_app_notify_start();

int hero_app_set_cycle(void *input, int inputLen);

int hero_app_set_start_parameter(void *input, int inputLen);

void hero_app_send_heart_beat_event();

int32 hero_app_send_sms_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hero_app_get_available_sim_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hero_app_add_key_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hero_app_get_version_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hero_app_set_cycle_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hero_app_set_start_parameter_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

//modify 20181117 end

#endif // __DSM_API_H__

#endif // __MOSOFT_VVCAT_SUPPORT__
