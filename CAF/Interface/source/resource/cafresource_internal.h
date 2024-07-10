/*****************************************************************************
** File Name:      cafresource_internal.h                                    *
** Author:         andrew.zhang                                              *
** Date:           09/30/2009                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to control mmi module                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2009        andrew.zhang         Creat
******************************************************************************/

#ifndef  _CAFRESOURCE_INTERNAL_H_    
#define  _CAFRESOURCE_INTERNAL_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

#define CAF_MMI_MAX_MODU_NUM                      64  

// CAF资源类型枚举
typedef enum _CAF_MMI_RES_TYPE_E
{
	CAF_MMI_RT_IMAGE = 0,
    CAF_MMI_RT_ANIM,
	CAF_MMI_RT_RING,
	CAF_MMI_RT_TEXT,
	CAF_MMI_RT_FONT,
	CAF_MMI_RT_DATA,
	CAF_MMI_RT_MAX_NUM
} CAF_MMI_RES_TYPE_E;

/* every unit resource data, for example, a image, a txt */
typedef struct 
{
    uint32 data_addr;
    uint32 data_size;
//#ifdef MULTI_THEME_SUPPORT
#if 1
    uint8  cur_theme;
#endif
}CAF_RES_UNIT_INFO_T;

typedef struct 
{   
    uint32          common_total;
    CAF_RES_UNIT_INFO_T *common_info_arry;
}CAF_RES_COMMON_INFO_T;

//#ifdef MULTI_THEME_SUPPORT
#if 1
#define CAF_MAX_ADDT_RESOURCE_NUM	32
#define CAF_MAX_RES_VERSION_LENGTH  128
#define CAF_MAX_RES_NAME_LENGTH 	128 

typedef struct  
{
    uint32 resource_offset;
    uint32 resource_size;
}CAF_RESOURCE_INFO;

typedef struct 
{
    CAF_RESOURCE_INFO default_resource_info;
    CAF_RESOURCE_INFO addition_resource_info[CAF_MAX_ADDT_RESOURCE_NUM];
    uint8  addition_resource_num;
}CAF_RESOURCE_HEADER_INFO, *PCAF_RESOURCE_HEADER_INFO;
#endif

//CAF 资源工具模块信息
typedef struct
{
    uint32 total_usedsize;    //所有资源所用空间
    uint32 mmi_module_adrr[CAF_MMI_MAX_MODU_NUM]; //各模块在数据里的头指针
//#ifdef MULTI_THEME_SUPPORT
#if 1
    uint8 resource_version_array[CAF_MAX_RES_VERSION_LENGTH];
    uint8 resource_name_array[CAF_MAX_RES_NAME_LENGTH];
#endif
}CAF_MMI_MODULE_TOTAL_INFO_T,*PCAF_MMI_MODULE_TOTAL_INFO_T;

typedef struct _CAF_TOOL_USED_INFO_T
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
} CAF_TOOL_USED_INFO_T, * PCAF_TOOL_USED_INFO_T;

/* project module info */
typedef struct 
{
    CAF_TOOL_USED_INFO_T        ToolUsedInfo;       
    MMI_RES_DATA_T              mmi_data_table[ CAF_MMI_RT_MAX_NUM ];
}CAF_MMIRES_MODULE_INFO_T;


//caf资源接口函数
LOCAL void CAF_LoadTextRes(CAF_RES_T *pRes );


LOCAL const wchar* CAFRES_GetText(
                                   MMI_TEXT_ID_T      text_id,     //[in] the text id 
                                   CAF_HANDLE_T       handle_id,
                                   MMI_STRING_T       *str_ptr
                                   );

LOCAL BOOLEAN CAF_LoadResourceData(CAF_RES_T * res_info_ptr);

LOCAL uint8* CAFRES_GetImage(
                              MMI_IMAGE_ID_T image_id, 
                              CAF_HANDLE_T handle_id, 
                              uint32 *size_ptr
                              );

LOCAL uint8* CAFRES_GetAnim(MMI_ANIM_ID_T anim_id, CAF_HANDLE_T handle_id, uint32 *size_ptr);

LOCAL BOOLEAN CAFRES_GetRingInfo(
                                  MMI_RING_ID_T ring_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIAUD_RING_DATA_INFO_T * pRingInfo
                                  );

RES_NODE_DATA_T* CAF_LoadResNode(const CAF_RES_T * com_res_info_ptr, uint32 res_id, CAF_HANDLE_T handle_id, CAF_MMI_RES_TYPE_E res_type);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif

