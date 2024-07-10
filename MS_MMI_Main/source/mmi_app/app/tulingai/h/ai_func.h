
#ifndef __ZDT_HYHL_FUNC_H__
#define __ZDT_HYHL_FUNC_H__
/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "ai_common.h"
#include "cjson.h"
#include "ai_ase.h"

/*
extern uint16 HYHL_Func_GetCrc16(const uint8* pData, int nLength);
extern uint8 HYHL_Func_GetTime(uint8 * pData);
extern uint8 HYHL_Func_GetPhoneInfo(uint8 * pData);
extern uint8 HYHL_Func_GetBatLevel(uint8 * pData);
extern uint8 HYHL_Func_GetBatPercent(uint8 * pData);
extern uint8 HYHL_Func_GetRssiLevel(uint8 * pData);
extern uint8 HYHL_Func_GetJPStatus(uint8 * pData);
extern uint8 HYHL_Func_GetJPNum(uint8 * pData);
#ifdef ZDT_PLAT_HYHL_FRIEND_SUPPORT
extern uint8 HYHL_Func_GetJPNum2(uint8 * pData);
#endif
extern uint16 HYHL_Func_GetWIFIInfo(uint8 * pData,uint8 loc_ok);
extern uint8 HYHL_Func_GetGPSLat(uint8 * pData,uint32 Latitude);
extern uint8 HYHL_Func_GetGPSLong(uint8 * pData,uint32 Longitude);
extern uint16 HYHL_Func_GetSSInfo(uint8 * pData);
extern uint8 HYHL_Func_GetGPSSateNum(uint8 * pData,uint8 sate_num);
extern uint8 HYHL_Func_GetGPSSpeed(uint8 * pData,uint32 speed);
extern uint8 HYHL_Func_GetGPSDir(uint8 * pData,uint8 is_ok,uint8 Lat_Dir,uint8 long_Dir,uint16 cog);
extern int HYHL_Func_SOSGetNextPara(uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen);
extern int  HYHL_Func_LinkManGetNextPara(char mark,uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen);
extern int  HYHL_Func_ClockGetNextPara(uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen);
extern int  HYHL_Func_HiddenGetNextPara(uint8** pCmd, uint16* pLen, uint8* pStr, uint16 wMaxStrLen);
*/
void get_rand_str(char s[], int number);
int CTAIHttp_WritePostBody(uint8 * ptr);



#endif
