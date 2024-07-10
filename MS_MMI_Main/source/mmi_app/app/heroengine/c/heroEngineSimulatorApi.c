#ifdef HERO_ENGINE_SUPPORT
#ifdef WIN32
//#include "heroAppApi.h"

typedef struct tagSmsInputData{
	int appid;
	int appVersion;
	int fid;
	int fre;
	int isTest;
}smsInputData;

typedef struct tagAppInputData{
	int appid;
	int appVersion;
	int fid;
	int fre;
	int isTest;
	char *extData;
	int extDataLen;
}appInputData;

typedef struct
{
	char *NetId;
	char *apn;
	char *userName;
	char *passWord;
	unsigned char dns[4];
	unsigned char authentication_type;
}T_HR_APN;


#ifdef HERO_ENGINE_TTS_SUPPORT
#include "mmitts_export.h"
#include "hero_tts_api.h"

typedef struct TxtParam_
{
    uint8* str;
    uint32 len;
    BOOLEAN is_unicode;
    uint32 position;
}TxtParam;

void hero_tts_check_str(uint8* pStr, uint16* pStrLen){}
void TTS_SynthText(TxtParam* txtPar,char*HeapBuffer,uint32 HEAP_BUFFER_SIZE,void *ResData){}
BOOLEAN hero_tts_switch_PreCheck_in(TTS_ALL_RING_TYPE_E ring_type){}
void hero_tts_auto_time_broadcast_in(void){}
void hero_tts_akey_time_broadcast_in(void){}
void hero_tts_new_sms_broadcast_in(wchar* wstr_ptr, TTS_ALL_RING_TYPE_E ring_type){}
void hero_tts_lock_screen_broadcast_in(wchar* wstr_ptr, uint16 wstr_len, TTS_ALL_RING_TYPE_E ring_type){}
void hero_tts_akey_power_level_broadcast_in(int bat_value){}
BOOLEAN TTS_IsRingCallRepeate_in(void){}
#endif

int hr_start_up_plat_app(unsigned char* input, int input_len, unsigned char** output, unsigned char* output_len, void *cb){}
T_HR_APN *hr_getAPN(unsigned char * netId){}
char hero_check_ussd(unsigned char *pContent, int nLen){}
void hero_start_dsm_app_req(smsInputData *inputData){}
int hero_get_dsm_app_version(){}
void hero_start_app_req(appInputData *inputData){}
int hero_app_add_key(void* input, int input_len){}
int hero_app_get_version(){}
int hero_app_set_cycle(void *input, int inputLen){}
int hero_app_set_start_parameter(void *input, int inputLen){}
int GetLocation_ex( char *area, int size, char *tel ){}
void LocationData_Init_ex(void){}
void LocationData_deInit_ex(void){}
void hccall_printf(const char *format,...){}
void hero_tts_charge_opt_broadcast_in(HERO_TTS_CHARGE_OPT_E charge_type){}
int hero_app_update_ext_data(char* input, int inputLen){}
int hero_app_http_stop_request(char* input, int inputLen, char** output, int* outputLen){}
int hero_app_http_post_request(char* input, int inputLen, char** output, int* outputLen){}
int hero_app_http_get_request(char* input, int inputLen, char** output, int* outputLen){}
unsigned char hero_app_is_operate_tts(){}

#endif
#endif
