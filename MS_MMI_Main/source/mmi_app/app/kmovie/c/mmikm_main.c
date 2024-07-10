/***************************************************************************
** File Name:      mmikm_main.c                                            *
** Author:                                                                 *
** Date:           07/14/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011         tonny.chen      Create                                  *
**																		   *
****************************************************************************/
/**-------------------------------------------------------------------------*/
/**                         Include Files                                   */
/**-------------------------------------------------------------------------*/
#include "mmi_app_kmovie_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "window_parse.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#include "mmikm_menutable.h"
#include "mmikm_id.h"
#include "mmikm_text.h"
#include "mmikm_internal.h"
#include "mmikm_export.h"
#include "mmikm_playlist.h"
#include "mmikm_text.h"
#include "mmicc_export.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                  */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_kmv_file = TRUE;

LOCAL BOOLEAN s_is_smv_file = TRUE;//smv_support

#ifdef MMI_KING_MOVIE_SUPPORT_HMV
LOCAL BOOLEAN s_is_hmv_file = TRUE;
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
extern void MMIAPICOM_GetFileNameAndSuffix( 
                                           const wchar  *ucs_full_name_ptr,     //IN
                                           wchar        *ucs_suffix_ptr,        //OUT
                                           wchar        *ucs_name_ptr           //OUT
                                           );
/******************************************************************************/
//  Description : set support kimgmovie file flag
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: TRUE: is support play file; FALSE:not support file
/*****************************************************************************/
PUBLIC void MMIAPIKM_SetSupportfileFlag(BOOLEAN is_kmv_file)
{
     s_is_kmv_file = is_kmv_file;
}

/******************************************************************************/
//  Description : is support kimgmovie file 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: TRUE: support play file; FALSE:not support
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIKM_IsSupportfile(void)
{
    //SCI_TRACE_LOW:"[MMIKM] MMIAPIKM_IsSupportfile:s_is_kmv_file = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_MAIN_67_112_2_18_2_31_15_0,(uint8*)"d", s_is_kmv_file);
     return s_is_kmv_file;
}

#ifdef MMI_KING_MOVIE_SUPPORT_HMV
/******************************************************************************/
//  Description : set support kimgmovie file flag
//  Global resource dependence : 
//  Author: jie.hu
//  Note: TRUE: is support play file; FALSE:not support file
/*****************************************************************************/
PUBLIC void MMIAPIKM_SetHmvfileFlag(BOOLEAN is_hmv_file)
{
     s_is_hmv_file = is_hmv_file;
}

/******************************************************************************/
//  Description : is support kimgmovie file 
//  Global resource dependence : 
//  Author: jie.hu
//  Note: TRUE: support play file; FALSE:not support
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIKM_IsHmvfile(void)
{
    //SCI_TRACE_LOW:"[MMIKM] MMIAPIKM_IsSupportfile:s_is_hmv_file = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_MAIN_87_112_2_18_2_31_15_1,(uint8*)"d", s_is_hmv_file);
     return s_is_hmv_file;
}
#endif

/******************************************************************************/
//  Description : is support kimgmovie file 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: TRUE: support play file; FALSE:not support
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPISM_IsSupportfile(void)//smv_support
{
    //SCI_TRACE_LOW:"[MMIKM] MMIAPIKM_IsSupportfile:s_is_kmv_file = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_MAIN_98_112_2_18_2_31_15_2,(uint8*)"d", s_is_kmv_file);
     return s_is_smv_file;
}

/******************************************************************************/
//  Description : just file type, check if support 
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPIKM_CkeckSupportfile(wchar *full_file_name_ptr)
{
    wchar file_suffix_type[KM_FILE_SUFFIX_MAX_LEN] = {0}; 
#ifdef KM_DEBUG_SWITCH_ENABLE
    uint8 char_filename[FILEARRAY_FILENAME_MAX_LEN+1] = {0}; 
    MMIAPICOM_WstrToStr((const wchar*)full_file_name_ptr, char_filename);
#endif
     
    s_is_kmv_file = FALSE;
    s_is_smv_file=FALSE;//smv_support
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    s_is_hmv_file = FALSE;
#endif

    if(PNULL == full_file_name_ptr)
    {
        return;
    }

    MMIAPICOM_GetFileNameAndSuffix(full_file_name_ptr,file_suffix_type, PNULL);

    if(0 == MMIAPICOM_Wstrlen(file_suffix_type))
    {
        return;
    }
        
#ifdef KM_DEBUG_SWITCH_ENABLE
    memset(char_filename, 0x00, FILEARRAY_FILENAME_MAX_LEN+1);
    MMIAPICOM_WstrToStr((const wchar*)file_suffix_type, char_filename);
#endif
    if(0 == MMIAPICOM_Wstrncmp(file_suffix_type, KM_FILE_SUFFIX_STR, 
                MMIAPICOM_Wstrlen(file_suffix_type)) )
    {
        s_is_kmv_file = TRUE;
        SCI_TRACE_LOW("[MMIKM] MMIAPIKM_CkeckSupportfile:s_is_kmv_file = %d",s_is_kmv_file);
    }

#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    if(0 == MMIAPICOM_Wstrncmp(file_suffix_type, HMV_FILE_SUFFIX_STR, 
                MMIAPICOM_Wstrlen(file_suffix_type)) )
    {
        s_is_hmv_file = TRUE;
        SCI_TRACE_LOW("[MMIKM] MMIAPIKM_CkeckSupportfile:s_is_Hmv_file = %d",s_is_hmv_file);
    }
#endif

    if(0 == MMIAPICOM_Wstrncmp(file_suffix_type, SMV_FILE_SUFFIX_STR, 
                MMIAPICOM_Wstrlen(file_suffix_type)) )//smv_support
    {
        s_is_smv_file = TRUE;
        SCI_TRACE_LOW("[MMIKM] MMIAPIKM_CkeckSupportfile:s_is_smv_file = %d",s_is_smv_file);
    }

    //SCI_TRACE_LOW:"[MMIKM] MMIAPIKM_CkeckSupportfile:s_is_kmv_file = %d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_MAIN_150_112_2_18_2_31_15_3,(uint8*)"d", s_is_kmv_file);
}                                                                            

/*****************************************************************************/
//  Description : reset factory 
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_ResetFactory(void)
{
    MMIKM_DeleteFile();
    MMIKM_ResetAllInfo();
}
/******************************************************************************/
//  Description : init km module
//  Global resource dependence : 
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIKM_InitModule(void)
{
    //register menu
    MMIKM_RegMenuGroup();
     
}
/*****************************************************************************/
//  Description : init km
//  Global resource dependence : none
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKM_Init(void)
{

}
/*****************************************************************************/
//  Description : mmikm app window entrance
//  Global resource dependence : 
//  Author: tonny.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKM_MainEntry(void)
{
    BOOLEAN recode = FALSE;    
    BOOLEAN is_need_search = FALSE;
#ifndef MMI_KM_NEED_SEARCH_FIRST_TIME
    MMIKM_PLAY_INFO_T       km_play_info = {0};
#endif

    //MMIFILE_DEVICE_E km_dev =  MMIAPIFMM_GetFirstValidSD();
    //check sdcard1 and sdcard2

#ifdef MMI_CALL_MINIMIZE_SUPPORT
    //SCI_TRACE_LOW:"[MMIKM] MMIAPIKM_MainEntry call running =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKM_MAIN_192_112_2_18_2_31_15_4,(uint8*)"d", MMIAPICC_IsInState(CC_IN_CALL_STATE));
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin( TXT_CALL_USING);
        return FALSE;
    }
#endif


    //check sdcard status
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
        return FALSE;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        return FALSE;
    }

#ifdef MMI_KM_NEED_SEARCH_FIRST_TIME//search files when need
    //check playlist file in filesystem
    if (MMIKM_CheckFile())
    {
        if (MMIKM_AlloclMemoForPlaylistInfo())
        {
            //check file ok, to read information from filesystem
            if(MMIKM_ReadFile())
            {
                if (MMIKM_IsPlayListEmpty())
                {
                    is_need_search = TRUE;
                    MMIKM_ReleaseMemoForPlaylistInfo();
                    SCI_TRACE_LOW("MMIKM] MMIAPIKM_MainEntry play list is empty");
                }
                else
                {
                    //creat km app window
                    MMIKM_CreateMainWin();
                    recode = TRUE;
                }
            }
            else
            {
                is_need_search = TRUE;
                MMIKM_ReleaseMemoForPlaylistInfo();
                SCI_TRACE_LOW("MMIKM] MMIAPIKM_MainEntry Read data file fail");
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_KM_ALLOC_MEMORY_FAIL);
        }
    }
    else
    {
        is_need_search = TRUE;
        SCI_TRACE_LOW("MMIKM] MMIAPIKM_MainEntry check data file fail");
    }

    if (is_need_search)

#endif//MMI_KM_NEED_SEARCH_FIRST_TIME search when need
    {
#ifndef MMI_KM_NEED_SEARCH_FIRST_TIME
	    if(MMIKM_CheckPlayInfoFile())
	    {
	        MMIKM_LoadPlayInfoFromFile(&km_play_info);
	    }
#endif
        MMIAPIKM_ResetFactory();

        if (MMIKM_AlloclMemoForPlaylistInfo())
        {
            if (MMIKM_CreateInforFile())
            {
                //if read play list file fail,
                //to search kmv file in the dir of SDCard KingMovie
                MMIKM_CreateSearchWaitWin();
            #ifndef MMI_KM_NEED_SEARCH_FIRST_TIME
                //make sure last play info can be display 
                MMIKM_SetLastPlayInfor(km_play_info.cur_file_full_path_name, 
                                        km_play_info.cur_file_full_path_len,
                                        km_play_info.played_time
                                        );
            #endif
                recode = TRUE;
            }
            else
            {
                MMIKM_CreateMainWin();
                recode = TRUE;
                SCI_TRACE_LOW("MMIKM] MMIAPIKM_MainEntry creat empty data file fail");
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_KM_ALLOC_MEMORY_FAIL);
        }


    }
      
    return recode;
}

