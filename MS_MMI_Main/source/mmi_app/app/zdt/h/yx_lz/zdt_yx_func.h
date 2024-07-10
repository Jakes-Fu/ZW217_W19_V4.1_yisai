#ifndef __ZDT_YX_FUNC_H__
#define __ZDT_YX_FUNC_H__
/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "zdt_common.h"
char *YX_Func_UrlEncode(const char *pcIn, int *pnInLen, 
                       char *pcOut, int *pnOutLen);
char * YX_Func_UrlDecode(const char *pcIn, int *pnInLen,
                       char *pcOut, int *pnOutLen);

extern void YX_Func_MD5_Generate(uint8 * input, unsigned int input_len,char out_str[32]);
extern void YX_Func_HMAC_SHA256_Generate(uint8 * input, int input_len,unsigned char out_str[64]);
extern uint8 * YX_Func_Encrypt(uint8 *data, uint32 data_len,uint32 * pOutlen);
extern uint8 * YX_Func_Decrypt(uint8 *data, uint32 data_len,uint32 * pOutlen);
extern uint8 YX_Func_HexCharToInt(uint8 hexchar);
extern uint8 YX_Func_IntToHexChar(uint8 bNibble);
extern uint8 YX_Func_IntToHexChar_One(uint8 data,uint8 * dest_buf);
extern uint32 YX_Func_IntToHexChar_Array(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len);
extern uint8 YX_Func_GetIndexWater(uint8 * pData);
extern uint8 YX_Func_GetICCID(uint8 * pData);
extern uint8 YX_Func_GetAlertType(uint8 * pData,uint16 alert_type);
extern uint8 YX_Func_Get220VStatus(uint8 * pData);
extern uint8 YX_Func_GetGPSStatus(uint8 * pData);
extern uint8 YX_Func_GetDeviceType(uint8 * pData);
extern uint8 YX_Func_GetNetType(uint8 * pData);
extern uint8 YX_Func_GetDeviceModel(uint8 * pData);
extern uint8 YX_Func_GetDeviceVer(uint8 * pData);
extern uint8 YX_Func_GetDeviceModelVer(uint8 * pData);
extern uint8 YX_Func_GetFirstLandTime(uint8 * pData);
extern uint8 YX_Func_SetRepeatNum(uint8 rpt_num);
extern uint8 YX_Func_GetRepeatNum(uint8 * pData);



extern int  YX_Func_GetCmdPosPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos);
extern int  YX_Func_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
extern int  YX_Func_GetNextParaM(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
extern int  YX_Func_GetNextParaF(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
extern int  YX_Func_GetNextParaJ(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
extern int  YX_Func_GetNextParaD(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
extern int  YX_Func_GetNextParaStar(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
extern int  YX_Func_GetNextParaHush(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);
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
extern uint16 YX_Func_GetGPSTime(uint8 * pData,uint32 gps_date,uint32 gps_time);
extern uint16 YX_Func_GetGPSInfo(uint8 * pData,uint8 loc_ok);
extern int  YX_Func_ClockGetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen);
extern int  YX_Func_TimerGetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen);
extern char* YX_Func_strupr(char* src);
extern char* YX_Func_strlwr(char* src);
extern uint16 YX_Func_GetFileName(char *pFullPath,char *pFileName);
extern uint8 YX_Func_GetNoteMsgID(uint8 * pData,uint16 msg_id);
extern uint32 YX_Func_StrTimeToInt(char *time_str,uint32 time_len);
extern uint32 YX_Func_StrDateToInt(char *date_str,uint32 date_len);
extern int  YX_Func_CR_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint16 wMaxStrLen);
extern BOOLEAN YX_Func_Regional_IsInPoly(double x, double y, int poly_sides,double *poly_X,double *poly_Y);
extern BOOLEAN YX_Func_Regional_IsInRound(double src_longitude, double src_latitude, uint16 radius,double new_longitude,double new_latitude);

#endif
