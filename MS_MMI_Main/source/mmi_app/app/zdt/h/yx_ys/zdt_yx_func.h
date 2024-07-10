#ifndef __ZDT_YX_FUNC_H__
#define __ZDT_YX_FUNC_H__
/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "zdt_common.h"
extern void YX_Func_MD5_Generate(uint8 * input, unsigned int input_len,char out_str[32]);
extern uint8 YX_Func_HexCharToInt(uint8 hexchar);
extern uint8 YX_Func_IntToHexChar(uint8 bNibble);
extern uint8 YX_Func_IntToHexChar_One(uint8 data,uint8 * dest_buf);
extern uint32 YX_Func_IntToHexChar_Array(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len);
extern int  YX_Func_GetCmdPosPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos);
extern int  YX_Func_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
extern int  YX_Func_GetPosPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos);

extern uint8 YX_Func_GetDate(uint8 * pData);
extern uint8 YX_Func_GetTime(uint8 * pData);
extern uint8 YX_Func_GetDateTime(uint8 * pData);
extern uint8 YX_Func_GetDateTimeEx(uint8 * pData);
extern uint8 YX_Func_GetTimeDateTime(uint8 * pData);
extern uint8 YX_Func_GetBatPercent(uint8 * pData);
extern uint8 YX_Func_GetRssiPercent(uint8 * pData);
extern uint8 YX_Func_GetStatus(uint8 * pData,uint16 type);
extern uint8 YX_Func_GetJPNum(uint8 * pData);
extern uint8 YX_Func_GetFGNum(uint8 * pData);
extern uint16 YX_Func_GetWIFIInfo(uint8 * pData,uint8 loc_ok);
extern uint8 YX_Func_GetGPSLat(uint8 * pData,uint32 Latitude,uint32 Lat_m,uint8 Lat_Dir);
extern uint8 YX_Func_GetGPSLong(uint8 * pData,uint32 Longitude,uint8 long_Dir);
extern uint16 YX_Func_GetSSInfo(uint8 * pData);
extern uint8 YX_Func_GetGPSSateNum(uint8 * pData,uint8 sate_num);
extern uint8 YX_Func_GetGPSSpeed(uint8 * pData,uint32 speed);
extern uint8 YX_Func_GetGPSCog(uint8 * pData,uint16 cog);
extern uint8 YX_Func_GetGPSAlt(uint8 * pData,uint32 alt, uint8 alt_dir);
extern int  YX_Func_ClockGetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen);
extern int  YX_Func_TimerGetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen);
extern char* YX_Func_strupr(char* src);
extern char* YX_Func_strlwr(char* src);
extern uint16 YX_Func_GetFileName(char *pFullPath,char *pFileName);

#endif
