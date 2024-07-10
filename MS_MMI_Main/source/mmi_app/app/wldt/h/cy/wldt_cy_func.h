#ifndef __WLDT_CY_FUNC_H__
#define __WLDT_CY_FUNC_H__
#include "wldt_common.h"
extern uint8 CY_Func_HexCharToInt(uint8 hexchar);
extern uint8 CY_Func_IntToHexChar(uint8 bNibble);
extern uint8 CY_Func_IntToHexChar_One(uint8 data,uint8 * dest_buf);
extern uint32 CY_Func_IntToHexChar_Array(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len);
extern int  CY_Func_GetCmdPosPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen,uint16 pos);
extern int  CY_Func_GetNextPara(uint8** pCmd, uint32* pLen, char* pStr, uint32 wMaxStrLen);

extern char* CY_Func_strupr(char* src);
extern char* CY_Func_strlwr(char* src);
#endif
