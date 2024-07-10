/*****************************************************************************
** File Name:      mmirecord_api.c                                               *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMIRECORD_API_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmirecord_export.h"
#include "mmi_filemgr.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmirecord_setting.h"
#include "mmirecord_main.h"
#include "mmirecord_appcontrol.h"
#include "mmirecord_nv.h"
#include "mmirecord_id.h"
#include "mmirecord_wintablel.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIRECORD_GetRecordStorage(void)
{
    return MMIRECORD_GetRecordStorage();
}


/*****************************************************************************/
//  Description : to whether there is recoding
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIRECORD_IsRecording(void)
{
    return MMIRECORD_IsRecord();
}

/*****************************************************************************/
//  FUNCTION:      MMIAPIRECORD_InitModule
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_InitModule(void)
{
    MMIRECORD_RegMenuGroup();
    MMIRECORD_RegNv();
    MMIRECORD_RegWinIdNameArr();
    MMIRECORD_RegAppletInfo();
}

/*****************************************************************************/
//  FUNCTION:      MMIAPIRECORD_Init
//  Description :
//  Global resource dependence :
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_Init(void)
{
    MMIRECORD_SettingInit();
}

/*****************************************************************************/
//  Description : recorder exit
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_Exit(void)
{
    MMIRECORD_Exit();
}

/*****************************************************************************/
//  Description : open record main win
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_WORK_ERROR_E MMIAPIRECORD_OpenMainWin(MMIRECORD_WORK_PARAM_T *work_param_ptr)
{
    return MMIRECORD_OpenMainWin(work_param_ptr);

}

/*****************************************************************************/
//  Description : Whether the record module is busy with working.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIRECORD_IsOpened(void) //TRUE: have opened
{

    return MMIRECORD_IsOpened();
}

/*****************************************************************************/
//  Description : stop record and stop play
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_StopRecordOrPlay(void)
{
    MMIRECORD_StopRecordOrPlay();
}

/*****************************************************************************/
//  Description : Close Menu
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_CloseMenu(void)
{
    MMIRECORD_CloseMenu();
}

/*****************************************************************************/
//  Description : MMIAPIRECORD_GetRecordFmt
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RECORD_FMT_E MMIAPIRECORD_GetRecordFmt(void)
{
    return MMIRECORD_ConvertWorkFMTToSRVFMT(MMIRECORD_GetRecordFileFormatType());
}
