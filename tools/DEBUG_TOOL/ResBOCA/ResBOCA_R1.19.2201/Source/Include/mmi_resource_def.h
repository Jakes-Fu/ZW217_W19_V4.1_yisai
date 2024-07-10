
#ifndef _MMI_RESOURCE_DEF_H__
#define _MMI_RESOURCE_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif
///////////////////////////////////////////////////////////////////////////////
// 整个资源的信息
///////////////////////////////////////////////////////////////////////////////
#define MAX_LANG_NAME 100
// 工具使用的信息
typedef struct _TOOL_USED_INFO_T
{
	uint16 nMajorVersion; 
    uint16 nMinorVersion; 
	uint16 nBuildNumber; 
	uint8  bAddrIsOffset;
    uint8  bBigEndian;

	uint32 nStartAddress;   // MMI资源的起始地址
	uint32 nMaxLimitSize;   // 整个MMI资源的最大限制
	uint32 nUsedSize;       // 当前已经使用的空间

	char   szOtherInfo[24]; // 其他信息，比如标识等
	                        // 前两个字符 szOtherInfo[0-1] 存放语言种类数 //x
} TOOL_USED_INFO_T, * PTOOL_USED_INFO_T;

#define FLAG_STR_NO_FLAG  0x1
#define FLAG_LANG_ID      0x2

typedef struct _RES_HDR_T
{
	uint8   flag[4];
	uint8   version[4];
	uint32  size;
	uint8   bigedn_flag;
	uint8   reserved1; //ext count
	uint8   ext_flag;
	uint8   lang_id;
	_RES_HDR_T(){
		flag[0]='s';
		flag[1]='r';
		flag[2]='e';
		flag[3]='s';
		version[0]='0';
		version[1]='0';
		version[2]='0';
		version[3]='1';
		size = sizeof(_RES_HDR_T);
		bigedn_flag = 1;
		reserved1 = 0 ;
		ext_flag = FLAG_STR_NO_FLAG;
		lang_id  = 0;
	}
} RES_HDR_T, * PRES_HDR_T;


typedef struct _LANG_INFO_T
{
	_TCHAR  szLangName[MAX_LANG_NAME];
	uint32  nLangProperty;       //语言文字编码的字节数
}LANG_INFO_T, *PLANG_INFO_T;

typedef struct _LANG_TOTAL_INFO_T
{
	uint32       nUsedLang;
	LANG_INFO_T  arrLangInfo[32];
}LANG_TOTAL_INFO_T,*PLANG_TOTAL_INFO_T;

// 资源类型枚举
typedef enum _MMI_RES_TYPE_E
{
	MMI_RT_IMAGE = 0,
    MMI_RT_ANIM,
	MMI_RT_RING,
	MMI_RT_TEXT,
	MMI_RT_FONT,
#ifndef PRJ_OLD_VER
	MMI_RT_OTHER,
#endif
	//[[ @hongliang 2009-6-19
	MMI_RT_COLOR,
	MMI_RT_RECT,
	//]]

	MMI_RT_MAX_NUM
} MMI_RES_TYPE_E;

// MMI资源数据
typedef struct _MMI_RES_DATA_T
{
	const void *  pData; // 该类型资源的存放地址
 	uint32  uCount;      // 该类型资源的数量
	uint32  uSize;       // 该类型资源的长度
} MMI_RES_DATA_T, * PMMI_RES_DATA_T;

// MMI的资源结构
typedef struct _MMI_RESOURCE_T
{
	TOOL_USED_INFO_T ToolUsedInfo;
	MMI_RES_DATA_T   mmi_data_table[ MMI_RT_MAX_NUM ];
} MMI_RESOURCE_T, * PMMI_RESOURCE_T;


typedef struct _MMI_RESOURCE_EX_T
{
	MMI_RES_DATA_T   mmi_data_table[ MMI_RT_MAX_NUM ];
} MMI_RESOURCE_EX_T, * PMMI_RESOURCE_EX_T;


typedef struct
{	
	uint32 total_usedsize;    //所有资源所用空间
    // Note:
    // Because different platform maybe have a diffefrent max 
    // module count, so we use a pointer to save the addresses
    // the definition of the struct is different from mmi code
    // from 1.3.8
	uint32* lp_mmi_module_adrr; //各模块在数据里的头指针
}MMI_MODULE_TOTAL_INFO_T,*PMMI_MODULE_TOTAL_INFO_T;


#define MAX_ADDT_RESOURCE_NUM	32
#define MAX_RES_VERSION_LENGTH  128
#define MAX_RES_NAME_LENGTH 	128 

typedef struct 
{
	DWORD  resource_offset;
	DWORD  resource_size;
} RESOURCE_INFO;

typedef struct 
{ 
	RESOURCE_INFO default_resource_info;
	RESOURCE_INFO addition_resource_info_array[MAX_ADDT_RESOURCE_NUM];
	BYTE addtion_resource_num;
}RESOURCE_HEADER_INFO;

typedef struct  
{
	BYTE resource_version_array[MAX_RES_VERSION_LENGTH];
	BYTE resource_name_array[MAX_RES_NAME_LENGTH];
	DWORD resource_name_id;
}MMI_MODULE_VER_INFO_T;


extern uint8 * MMI_RES_PTR;

#ifdef __cplusplus
}
#endif

#endif // _MMI_RESOURCE_DEF_H__