/***************************************************************************
** File Name:      mmikm_export.h                                          *
** Author:                                                                 *
** Date:           14/07/2011                                               *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011          tonny.chen          Create                              *
**																		   *
****************************************************************************/
#ifndef _MMIKM_MAIN_H_
#define _MMIKM_MAIN_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIKM_SaveLastPorcesstime       MMIAPIKM_SaveLastPorcesstime
#define MMIKM_GetLastPorcesstime        MMIAPIKM_GetLastPorcesstime
#define MMIKM_SetLastPlayOpenWin        MMIAPIKM_SetLastPlayOpenWin
#define MMIKM_ReleaseArray              MMIAPIKM_ReleaseArray

#endif //MMI_FUNC_COMPATIBLE_SUPPORT        

/******************************************************************************/
//  Description : set support kimgmovie file flag
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: TRUE: is support play file; FALSE:not support file
/*****************************************************************************/
PUBLIC void MMIAPIKM_SetSupportfileFlag(BOOLEAN is_kmv_file);

/******************************************************************************/
//  Description : is support kimgmovie file 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: TRUE: support play file; FALSE:not support
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIKM_IsSupportfile(void);
/******************************************************************************/
//  Description : is support kimgmovie file 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: TRUE: support play file; FALSE:not support
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPISM_IsSupportfile(void);//smv_support
#ifdef  MMI_KING_MOVIE_SUPPORT_HMV
/******************************************************************************/
//  Description : is support kimgmovie file 
//  Global resource dependence : 
//  Author: jie.hu
//  Note: TRUE: support play file; FALSE:not support
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIKM_IsHmvfile(void);

/******************************************************************************/
//  Description : set support kimgmovie file flag
//  Global resource dependence : 
//  Author: jie.hu
//  Note: TRUE: is support play file; FALSE:not support file
/*****************************************************************************/
PUBLIC void MMIAPIKM_SetHmvfileFlag(BOOLEAN is_hmv_file);
#endif

/******************************************************************************/
//  Description : just file type, check if support 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPIKM_CkeckSupportfile(wchar *full_file_name_ptr);

/*****************************************************************************/
//  Description : reset factory 
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_ResetFactory(void);

/******************************************************************************/
//  Description : init km module
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKM_InitModule(void);

/*****************************************************************************/
//  Description : init km
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_Init(void);

/*****************************************************************************/
//  Description : 进入km应用模块入口
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKM_MainEntry(void);

/*****************************************************************************/
//  Description : 保存上次播放文件的进度 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_SaveLastPorcesstime(uint32 process_time);//andy.he_cr118339

/*****************************************************************************/
//  Description : 获取上次播放文件的进度 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32  MMIAPIKM_GetLastPorcesstime(void);//andy.he_cr118339

/*****************************************************************************/
//  Description :设置是否进入上次播放模式
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPIKM_SetLastPlayOpenWin(BOOL isopened);

/*****************************************************************************/
//  Description : 是否以<<上次播放>> 形式打开 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKM_IsLastPlayOpenWin(void);//andy.he_cr118339

/*****************************************************************************/
//  Description :释放搜索所创建的ARRAY
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_ReleaseArray(void);

/*****************************************************************************/
//  Description : MMIAPIKM_IsKMmainWinOpen
//  Global resource dependence : 
//  Author:Glen.Li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKM_IsKMmainWinOpen(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif


#endif//_MMIKM_MAIN_H_

