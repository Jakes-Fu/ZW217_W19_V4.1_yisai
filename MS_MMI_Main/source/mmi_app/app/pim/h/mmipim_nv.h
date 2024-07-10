/******************************************************************************
文件名称:mmipim_nvl.h
文件描述:
文件说明:
开发人员:王晓林
首次开发日期:2007-9.25
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2007-09-25            王晓林                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/

#ifndef _MMIPIM_NV_H_
#define _MMIPIM_NV_H_

#include "sci_types.h"
#include "mmi_module.h"
#include "mmipim.h"
#include "pimdatasynctask.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
	MMI_PIM_AUTH_TYPE= MMI_MODULE_PIM<< 16,
	MMI_PIM_ENCODE_TYPE,
	MMIPIM_LAST_ANCHOR,
	MMINV_PIM_NET_INFO, // chenxiang 20071101 pim_setting
	MMINV_PIM_PROXY_INFO,
	MMI_PIM_SERVER_INFO,
	MMINV_PIM_WAP_URL_INFO, // chenxiang 20071101 pim_setting
	MMI_PIM_PB_LOG_INFO,
#ifdef HS_PIM_UA_SETTING // chenxiang 20080318
    MMINV_PIM_UA_TYPE,
    MMINV_PIM_UA_INFO,
#endif
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
MMINV_PIM_SYNC_TYPE,
#endif  // HS_PIM_SUPPORT_OTHER_SYNC_TYPE 
#ifdef HS_PIM_VCAL  //Added by wangzhefeng  2009.9.4
	MMI_PIM_VCAL_LOG_INFO,
	MMIPIM_VCAL_LAST_ANCHOR,//added by wangzhefeng 20091208 for 日程更新的时间戳
#endif /* HS_PIM_VCAL */
	MMINV_PIM_NEXT_ANCHOR
	
} MMIPIM_NV_ITEM_E;

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif





