/****************************************************************************
** File Name:      mmimms_control_main.h                                   *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_CONTROL_MAIN_H
#define _MMIMMS_CONTROL_MAIN_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmimms_export.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_SECOND_1970_TO_1980    ((365*8 +366*2)*24*3600)

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: get the prefer disk
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMMS_GetPreferDisk(void);

/*****************************************************************************/
// 	Description: Check space is enough or not
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsEnoughSpace(uint32 size);

/*****************************************************************************/
//  Description : 获取彩信已占用的总大小total_size_ptr和能使用的空间大小all_space_ptr
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetTotalSizeAndAllSize(uint32 *total_size_ptr, 
                                            uint32 *all_space_ptr);

/*****************************************************************************/
// 	Description: get prefer device with enough space
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMMS_GetDeviceForSave(uint32 size);

/*****************************************************************************/
// 	Description: send msg from control to ui
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ControlToUIMsg(
                                  uint16   	    msg_id,
                                  DPARAM        param
                                  );

/*****************************************************************************/
// 	Description: 返回 1970年1月1日零点以来经过的秒数,
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentTime( void );

/*****************************************************************************/
// 	Description: 生成随机文件名
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GenRandomName(uint8 *name_ptr,uint16 name_maxlen);

/*****************************************************************************/
//  Description : MMS control init
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ControlInit(MMSCONTROL_CALLBACK callback);

/*****************************************************************************/
//  Description : Set systerm frequency for mmi mms
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_SetFreq(void);
 
/*****************************************************************************/
//  Description : Get systerm frequency for mmi mms
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/ 
PUBLIC uint32 MMIMMS_RestoreFreq(void);

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSLimitSize(uint32 mms_limit);
#endif

/*****************************************************************************/
//  Description : Get MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetMMSLimitSize(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_MAIN_H*/


