#ifndef __SG_STRING__
#define __SG_STRING__
#include "wdp_SG_Def.h"	
extern void sg_wcs2str_s(SG_UINT16 *src, SG_INT8 *dest, SG_UINT len);

extern void sg_str2wcs_s(SG_UINT8 *src, SG_UINT16 *dest, SG_UINT len);

extern SG_UINT sg_strlen(SG_INT8 *src);

extern SG_UINT sg_wcslen(SG_UINT16 *src);

extern void sg_strcat_s(SG_INT8 *dest, SG_UINT len, SG_INT8 *src);

extern void sg_wcscat_s(SG_UINT16 *dest, SG_UINT len, SG_UINT16 *src);

extern void sg_strcpy_s(SG_INT8 *dest, SG_UINT len, SG_INT8 *src);

extern void sg_wcscpy_s(SG_UINT16 *dest, SG_UINT len, SG_UINT16 *src);

extern SG_INT sg_strcmp(SG_INT8 *str1, SG_INT8 *str2);

extern SG_INT sg_wcscmp(SG_UINT16 *str1, SG_UINT16 *str2);

extern SG_INT8* sg_strstr(SG_INT8 *str1, SG_INT8 *str2);

extern SG_UINT16* sg_wcsstr(SG_UINT16 *str1, SG_UINT16 *str2);

extern SG_UINT32 sg_str2Int(SG_INT8 *src);

extern SG_UINT32 sg_wcs2Int(SG_UINT16 *src);

extern SG_INT8* sg_strchr(const SG_INT8*src, SG_INT8 chr);

extern SG_UINT16* sg_wcschr(SG_UINT16*src, SG_UINT16 chr);
#endif
