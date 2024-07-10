/*****************************************************************************
** File Name:      mmidc_conflict.c                                          *
** Author:                                                                   *
** Date:           22/05/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used between dc and app manager              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2012       robert.wang       Create                                    *
******************************************************************************/

#define _MMIDC_CONFLICT_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
 #ifdef CAMERA_SUPPORT
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"

//other module
#include "mmiset_display.h"


#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_text.h"
#include "mmiqbtheme_export.h"
#endif

#ifdef MMI_WIFI_SUPPORT
#include "mmiwlan_text.h"
#include "mmiwifi_export.h"
#endif

#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#endif

#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "mmipicview_export.h"
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
#include "mmimp3_export.h"
#endif

#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
        
#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif

#ifdef MMS_SUPPORT
#include "mmimms_export.h"
#endif
        
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif
        
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
        
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#include "mmidl_text.h"
#endif

#ifdef MMIUZONE_SUPPORT
#include "mmiuzone_export.h"
#endif

//local module
#include "mmidc_export.h"
#include "mmidc_camera_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_camera_image.h"
#include "mmidc_camera_anim.h"
#include "mmidc_main.h"
#include "mmidc_conflict.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DC_APP_CLOSE_DELAY_TIME_UNIT 500
#define DC_APP_CLOSE_DELAY_TIME_MIN 200
#define DC_APP_CLOSE_DELAY_TIME_MAX 5000

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    uint32 delay_time;
}MMIDC_DELAY_TIME_T;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//  Description : check wifi 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDC_APP_CheckWifi(void);

/*****************************************************************************/
//  Description : close wifi 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseWifi(void** param, //out
                                       uint32* param_size //out
                                       );

/*****************************************************************************/
//  Description : start wifi 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIDC_APP_StartWifi(void** param, //in,out
                                      uint32* param_size //in,out
                                      );
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
/*****************************************************************************/
//  Description : close mp3
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseMP3(void** param, //out
                                        uint32* param_size //out
                                       );

/*****************************************************************************/
//  Description : start mp3
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartMP3(void** param, //in,out
                                       uint32* param_size //in,out
                                       );
#endif

#ifdef EBOOK_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close ebook
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseEbook(void** param, //out
                                          uint32* param_size //out
                                          );

/*****************************************************************************/
//  Description : mmidc app start ebook
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartEbook(void** param, //in,out
                                         uint32* param_size //in,out
                                         );
#endif

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close browser
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseBrowser(void** param, //out
                                             uint32* param_size //out
                                            );

/*****************************************************************************/
//  Description : mmidc app start browser
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartBrowser(void** param, //in,out
                                            uint32* param_size //in,out
                                           );
#endif

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close bluetooth
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseBlueTooth(void** param, //out
                                               uint32* param_size //out
                                               );

/*****************************************************************************/
//  Description : mmidc app start bluetooth
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartBlueTooth(void** param, //in,out
                                              uint32* param_size //in,out
                                              );
#endif

#ifdef DL_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close download
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseDL(void** param, //out
                                      uint32* param_size //out
                                      );

/*****************************************************************************/
//  Description : mmidc app start download
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartDL(void** param, //in,out
                                     uint32* param_size //in,out
                                     );
#endif

#ifdef QBTHEME_SUPPORT
/*****************************************************************************/
//  Description : mmidc app check qbtheme
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIDC_APP_CheckQbTheme(void);

/*****************************************************************************/
//  Description : mmidc app close qbtheme
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseQbTheme(void** param, //out
                                              uint32* param_size //out
                                              );
                   
/*****************************************************************************/
//  Description : mmidc app start qbtheme
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartQbTheme(void** param, //in,out
                                             uint32* param_size //in,out
                                             );
#endif

#ifdef MMIEMAIL_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close email
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseEmail(void** param, //out
                                         uint32* param_size //out
                                         );

/*****************************************************************************/
//  Description : mmidc app start email
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartEmail(void** param, //in,out
                                        uint32* param_size //in,out
                                        );
#endif

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close dynamic mainmenu
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseDyMainmenu(void** param, //out
                                                  uint32* param_size //out
                                                  );
#endif

#ifdef PIC_VIEWER_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close pic view
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_ClosePicview(void** param, //out
                                            uint32* param_size //out
                                           );

/*****************************************************************************/
//  Description : mmidc app start picview
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartPicview(void** param, //in,out
                                           uint32* param_size //in,out
                                          );
#endif

#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close auto down mms
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseAutodownMMS(void** param, //out
                                                    uint32* param_size //out
                                                    );

/*****************************************************************************/
//  Description : mmidc app start auto down mms
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartAutodownMMS(void** param, //in,out
                                                    uint32* param_size //in,out
                                                   );
#endif

#ifdef MOBILE_VIDEO_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close mv
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseMV(void** param, //out
                                       uint32* param_size //out
                                      );

/*****************************************************************************/
//  Description : mmidc app start mv
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartMV(void** param, //in,out
                                      uint32* param_size //in,out
                                     );
#endif

#ifdef MMIUZONE_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close uzone
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseUZone(void** param, //out
                                          uint32* param_size //out
                                          );

/*****************************************************************************/
//  Description : mmidc app start uzone
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartUZone(void** param, //in,out
                                         uint32* param_size //in,out
                                         );
#endif

/*****************************************************************************/
//  Description : get conflict dc app handle
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDCAppConflictID( MMIDC_APP_E *app_id_ptr );

/*****************************************************************************/
//  Description : get dc app text id
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetDCAppTextIdByAppID(MMIDC_APP_E app_id);

/*****************************************************************************/
//  Description : recheck app conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void RecheckAppConflict(void);

/*****************************************************************************/
//  Description : resolve dc app conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void ResolveDCAppConflict( 
                                 MMIPUB_INFO_T  *win_info_ptr
                                 );

/*****************************************************************************/
//  Description : handle conflict win msg
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCAppConflictWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        );

/*****************************************************************************/
//  Description : create conflict ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void CreateDCAppConflictPtr( 
                             const MMIDC_APP_CONFLICT_T* conflict_ptr
                             );

/*****************************************************************************/
//  Description : destroy conflict ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void DestroyDCAppConflictPtr( void );

/*****************************************************************************/
//  Description : notify resolve conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyCaller( BOOLEAN is_do_ok );

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void OpenDCAppQueryWin(MMIDC_APP_E app_id);


/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL MMIDC_APP_CONFLICT_T* s_mmidc_app_conflict_ptr = PNULL;
LOCAL MMIDC_DELAY_TIME_T s_mmidc_delay_time = {0};
LOCAL uint8 s_mmidc_app_close_timer = 0;
/**
Note:!!! app list has sorted
*/
LOCAL MMIDC_APP_ITEM_T s_mmidc_app_array[] =
{
    #ifdef MMI_WIFI_SUPPORT
    {MMIDC_APP_WIFI, MMIDC_APP_TYPE_QUERY, TXT_WIFI, MMIDC_APP_CheckWifi, MMIDC_APP_CloseWifi, MMIDC_APP_StartWifi, PNULL, 0, FALSE},
    #endif

    #ifdef BROWSER_SUPPORT
    {MMIDC_APP_BROWSER, MMIDC_APP_TYPE_QUERY, TXT_COMMUNICATION_BROWSER, MMIAPIBROWSER_IsRunning, MMIDC_APP_CloseBrowser, MMIDC_APP_StartBrowser, PNULL, 0, FALSE},
    #endif
        
    #ifdef MMIEMAIL_SUPPORT
    {MMIDC_APP_EMAIL, MMIDC_APP_TYPE_QUERY, TXT_EMAIL, MMIAPIEMAIL_IsRunning, MMIDC_APP_CloseEmail, MMIDC_APP_StartEmail, PNULL, 0, FALSE},
    #endif

    #ifdef EBOOK_SUPPORT
    {MMIDC_APP_EBOOK, MMIDC_APP_TYPE_CLOSE, TXT_COMM_EBOOK, MMIAPIEBOOK_IsRunning, MMIDC_APP_CloseEbook, MMIDC_APP_StartEbook, PNULL, 0, FALSE},
    #endif

    #ifdef DYNAMIC_MAINMENU_SUPPORT
    {MMIDC_APP_DYNAMIC_MAINMENU, MMIDC_APP_TYPE_CLOSE,TXT_NULL, PNULL, MMIDC_APP_CloseDyMainmenu, PNULL, PNULL, 0, FALSE},
    #endif
    
    #ifdef PIC_VIEWER_SUPPORT
    {MMIDC_APP_PIC_VIEWER, MMIDC_APP_TYPE_RESTORE, TXT_ENTERTAIMENT_PIC, MMIAPIPICVIEW_IsPicFolderWinOpen, MMIDC_APP_ClosePicview, MMIDC_APP_StartPicview, PNULL, 0, FALSE},
    #endif

    #ifdef MMI_AUDIO_PLAYER_SUPPORT
    //{MMIDC_APP_AUDIO_PLAYER, MMIDC_APP_TYPE_RESTORE, TXT_AUDIO, MMIAPIAP_IsOpened, MMIDC_APP_CloseMP3, MMIDC_APP_StartMP3, PNULL, 0, FALSE},
    #endif

    #ifdef MMS_SUPPORT
    {MMIDC_APP_MMS, MMIDC_APP_TYPE_RESTORE, TXT_MMS, MMIAPIMMS_IsSendingOrRecving, MMIDC_APP_CloseAutodownMMS, MMIDC_APP_StartAutodownMMS, PNULL, 0, FALSE},
    #endif
        
    #ifdef BLUETOOTH_SUPPORT
    {MMIDC_APP_BLUETOOTH, MMIDC_APP_TYPE_RESTORE, TXT_BLUETOOTH, MMIAPIBT_IsBtOn, MMIDC_APP_CloseBlueTooth, MMIDC_APP_StartBlueTooth, PNULL, 0, FALSE},
    #endif
        
    #ifdef MOBILE_VIDEO_SUPPORT
    {MMIDC_APP_MOBILE_VIDEO, MMIDC_APP_TYPE_RESTORE, TXT_ENTERTAIMENT_MV, MMIAPIMV_IsOpenMobileVideo, MMIDC_APP_CloseMV, MMIDC_APP_StartMV, PNULL, 0, FALSE},
    #endif

    #ifdef DL_SUPPORT
    {MMIDC_APP_DL, MMIDC_APP_TYPE_RESTORE, TXT_DL_DOWNLOAD_MANAGER, MMIAPIDL_GetDownloadStatus, MMIDC_APP_CloseDL, MMIDC_APP_StartDL, PNULL, 0, FALSE},
    #endif

    #ifdef MMIUZONE_SUPPORT
    {MMIDC_APP_UZONE, MMIDC_APP_TYPE_RESTORE, TXT_UZONE,MMIAPIUZONE_IsOpened, MMIDC_APP_CloseUZone, MMIDC_APP_StartUZone, PNULL, 0, FALSE},
    #endif

    #ifdef QBTHEME_SUPPORT
    {MMIDC_APP_QBTHEME, MMIDC_APP_TYPE_RESTORE, TXT_QB_QBTHEME_IDLE, MMIDC_APP_CheckQbTheme, MMIDC_APP_CloseQbTheme, MMIDC_APP_StartQbTheme, PNULL, 0, FALSE},
    #endif
    {MMIDC_APP_MAX, MMIDC_APP_TYPE_NONE, TXT_NULL, PNULL, PNULL, PNULL, PNULL, 0, FALSE},
};

/*****************************************************************************/
//  Description : get dc app conflict info ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMIDC_APP_CONFLICT_T *GetDCAppConflictInfoPtr(void)
{
    return s_mmidc_app_conflict_ptr;
}

/*****************************************************************************/
//  Description : get dc app conflict list ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMIDC_APP_LIST_T *GetDCAppListPtr(void)
{
    MMIDC_APP_CONFLICT_T *info_ptr = GetDCAppConflictInfoPtr();
    MMIDC_APP_LIST_T *app_list_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        app_list_ptr = &info_ptr->app_list_info;
    }

    return app_list_ptr;
}

/*****************************************************************************/
//  Description : get dc app ok func ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMIDC_APP_OK_FUNC_T *GetDCAppOKFuncPtr(void)
{
    MMIDC_APP_CONFLICT_T *info_ptr = GetDCAppConflictInfoPtr();
    MMIDC_APP_OK_FUNC_T *ok_func_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        ok_func_ptr = &info_ptr->ok_func_info;
    }

    return ok_func_ptr;
}

/*****************************************************************************/
//  Description : get dc app cancel func ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMIDC_APP_CANCEL_FUNC_T *GetDCAppCancelFuncPtr(void)
{
    MMIDC_APP_CONFLICT_T *info_ptr = GetDCAppConflictInfoPtr();
    MMIDC_APP_CANCEL_FUNC_T *cancel_func_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        cancel_func_ptr = &info_ptr->cancel_func_info;
    }

    return cancel_func_ptr;
}

/*****************************************************************************/
//  Description : get dc app total number
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetDCAppTotalNumber(void)
{
    uint32 total_num = 0;
    MMIDC_APP_CONFLICT_T *info_ptr = GetDCAppConflictInfoPtr();

    if (PNULL != info_ptr)
    {
        total_num = info_ptr->app_list_info.total_num;
    }

    SCI_TRACE_LOW("[MMIDC] GetDCAppTotalNumber total_num=%d",
                 total_num); 
                 
    return total_num;
}

/*****************************************************************************/
//  Description : get dc app item by app id
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_APP_ITEM_T *GetDCAppItemByAppID(MMIDC_APP_E app_id)
{
    MMIDC_APP_ITEM_T *item_ptr = PNULL;
    MMIDC_APP_LIST_T *info_ptr = GetDCAppListPtr();
    uint32 total_num = 0;
    uint32 i = 0;

    if ((PNULL != info_ptr)
        && (PNULL != info_ptr->array_ptr)
       )
    {        
        if (app_id < MMIDC_APP_MAX)
        {
            total_num = GetDCAppTotalNumber();
            
            for (i = 0; i < total_num; i++)
            {
                item_ptr = &info_ptr->array_ptr[i];

                if (app_id == item_ptr->app_id)
                {
                    break;
                }
            }
        }        
    }
    
    SCI_TRACE_LOW("[MMIDC] GetDCAppItem app_id=%d",\
                 app_id);
    
    return item_ptr;
}

/*****************************************************************************/
//  Description : get dc app item by index
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIDC_APP_ITEM_T *GetDCAppItemByIndex(uint32 index)
{
    MMIDC_APP_ITEM_T *item_ptr = PNULL;
    MMIDC_APP_LIST_T *info_ptr = GetDCAppListPtr();

    if ((PNULL != info_ptr)
        && (PNULL != info_ptr->array_ptr)
       )
    {        
        if (index < MMIDC_APP_MAX)
        {
            item_ptr = &info_ptr->array_ptr[index];
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] GetDCAppItemByIndex index=%d",\
                 index);
    
    return item_ptr;
}

/*****************************************************************************/
//  Description : set dc app item running State
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetDCAppRunningStateByAppID(MMIDC_APP_E app_id, BOOLEAN is_running)
{
    MMIDC_APP_ITEM_T *item_ptr = PNULL;

    
    item_ptr = GetDCAppItemByAppID(app_id);
    if (PNULL != item_ptr)
    {
        item_ptr->is_running = is_running;
    }

    SCI_TRACE_LOW("[MMIDC] SetDCAppRunningState app_id=%d, is_running=%d",\
                 app_id, is_running);
}

/*****************************************************************************/
//  Description : get conflict id
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDCAppConflictID( MMIDC_APP_E *app_id_ptr )
{
    BOOLEAN is_query = FALSE;
    uint32 i = 0;
    uint32 total_num = GetDCAppTotalNumber();
    MMIDC_APP_E app_id = 0;
    MMIDC_APP_ITEM_T *app_item_ptr = PNULL;
    MMIDC_APP_CHECK_FUNC check_func_ptr = PNULL;
    MMIDC_APP_CLOSE_FUNC close_func_ptr = PNULL;
    MMIDC_APP_TYPE_E do_type = 0;    

    for (i = 0; i < total_num; i++)
    {
        app_item_ptr = GetDCAppItemByIndex(i);

        if (PNULL != app_item_ptr)
        {
            app_id = app_item_ptr->app_id;
            check_func_ptr = app_item_ptr->check_func_ptr;
            close_func_ptr = app_item_ptr->close_func_ptr;

            do_type = app_item_ptr->do_type;


            SCI_TRACE_LOW("[MMIDC] GetDCAppConflictID app_id=%d",\
                         app_id);
            SCI_TRACE_LOW("[MMIDC] GetDCAppConflictID do_type=%d",\
                         do_type);
            SCI_TRACE_LOW("[MMIDC] GetDCAppConflictID check_func_ptr=0x%X",\
                         check_func_ptr);                          
            SCI_TRACE_LOW("[MMIDC] GetDCAppConflictID close_func_ptr=0x%X",\
                         close_func_ptr);

                         
            switch(do_type)
            {
            case MMIDC_APP_TYPE_QUERY:
                if (PNULL != check_func_ptr)
                {
                    if (check_func_ptr())
                    {
                        is_query = TRUE;
                    }
                }
                break;
            case MMIDC_APP_TYPE_CLOSE:
                if (PNULL != check_func_ptr)
                {
                    if (check_func_ptr())
                    {
                        if (PNULL != close_func_ptr)
                        {
                            close_func_ptr(&app_item_ptr->param, &app_item_ptr->param_size);
                        }
                    }
                }
                else
                {
                    if (PNULL != close_func_ptr)
                    {
                        close_func_ptr(&app_item_ptr->param, &app_item_ptr->param_size);
                    }
                }
                break;
            case MMIDC_APP_TYPE_RESTORE:
                if (PNULL != check_func_ptr)
                {
                    if (check_func_ptr())
                    {
                        SetDCAppRunningStateByAppID(app_id, TRUE);

                        if (PNULL != close_func_ptr)
                        {
                            close_func_ptr(&app_item_ptr->param, &app_item_ptr->param_size);
                        }
                    }
                }
                break;
            default:
                break;
            }
        }

        if (is_query)
        {
            break;
        }
    }

    *app_id_ptr = app_id;


    SCI_TRACE_LOW("[MMIDC] GetDCAppConflictID app_id=%d",\
                 app_id);
    SCI_TRACE_LOW("[MMIDC] GetDCAppConflictID is_query=%d",\
                 is_query);
    
    return is_query;
}

/*****************************************************************************/
//  Description : get app  text id by app id
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetDCAppTextIdByAppID(MMIDC_APP_E app_id)
{
    MMI_TEXT_ID_T text_id = 0;
    MMIDC_APP_ITEM_T *item_ptr = GetDCAppItemByAppID(app_id);


    if (PNULL != item_ptr)
    {
        text_id = item_ptr->app_name_id;
    }

    return text_id;
}

/*****************************************************************************/
//  Description : init app close time
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void InitAppCloseTime(void)
{
    s_mmidc_delay_time.delay_time = DC_APP_CLOSE_DELAY_TIME_MIN;
}

/*****************************************************************************/
//  Description : start app close timer
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void StartAppCloseTimer(uint32 time)
{
    if(0 != s_mmidc_app_close_timer)
    {
        MMK_StopTimer(s_mmidc_app_close_timer);
    }
    
    s_mmidc_app_close_timer = MMK_CreateTimer(time, TRUE);

    SCI_TRACE_LOW("[MMIDC] StartAppCloseTimer time=%d",\
                 time);
}

/*****************************************************************************/
//  Description : close app close timer
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void CloseAppCloseTimer(void)
{
    if(0 != s_mmidc_app_close_timer)
    {
        MMK_StopTimer(s_mmidc_app_close_timer);
        s_mmidc_app_close_timer = 0;
    }
}

/*****************************************************************************/
//  Description : destroy dc app
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ProcessConflictTimer(uint8 timer_id, MMIDC_APP_E app_id)
{
    MMIDC_APP_ITEM_T *app_item_ptr = GetDCAppItemByAppID(app_id);
    MMIDC_APP_CHECK_FUNC check_func_ptr = PNULL;
    MMIDC_APP_CLOSE_FUNC close_func_ptr = PNULL;
    MMIDC_APP_TYPE_E do_type = 0;

    uint32 unit_time = DC_APP_CLOSE_DELAY_TIME_UNIT;
    //uint32 max_time = DC_APP_CLOSE_DELAY_TIME_MAX;
    uint32 delay_time = s_mmidc_delay_time.delay_time;

    BOOLEAN ret = FALSE;
    

    SCI_TRACE_LOW("[MMIDC] ProcessConflictTimer timer_id=%d",\
                 timer_id);
    SCI_TRACE_LOW("[MMIDC] ProcessConflictTimer app_id=%d",\
                 app_id);
                 
    if (s_mmidc_app_close_timer == timer_id)
    {
        ret = TRUE;
        
        CloseAppCloseTimer();
        
        if ((PNULL != app_item_ptr)
            && (app_id == app_item_ptr->app_id)
           )
        {
            check_func_ptr = app_item_ptr->check_func_ptr;
            close_func_ptr = app_item_ptr->close_func_ptr;

            do_type = app_item_ptr->do_type;


            SCI_TRACE_LOW("[MMIDC] ProcessConflictTimer do_type=%d",\
                         do_type);
            
            if (MMIDC_APP_TYPE_QUERY == do_type)
            {
                if (PNULL != check_func_ptr)
                {
                    //wait to close
                    if (check_func_ptr())
                    {
                        #if 0
                        if (delay_time > max_time)
                        {
                            break;
                        }
                        #endif
                        delay_time += unit_time;
                        StartAppCloseTimer(delay_time);
                        
                        s_mmidc_delay_time.delay_time = delay_time;
                    }
                    else
                    {
                        RecheckAppConflict();
                    }
                }
             }
        }
    }


    SCI_TRACE_LOW("[MMIDC] ProcessConflictTimer ret=%d",
                 ret);

    return ret;
}

/*****************************************************************************/
//  Description : destroy dc app
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DestroyDCApp(MMIDC_APP_E app_id)
{
    MMIDC_APP_ITEM_T *app_item_ptr = GetDCAppItemByAppID(app_id);
    MMIDC_APP_CHECK_FUNC check_func_ptr = PNULL;
    MMIDC_APP_CLOSE_FUNC close_func_ptr = PNULL;
    MMIDC_APP_TYPE_E do_type = 0;

    BOOLEAN ret = TRUE;
    uint32 delay_time = s_mmidc_delay_time.delay_time;
    

    SCI_TRACE_LOW("[MMIDC] DestroyDCApp app_id=%d",\
                 app_id);  

    if ((PNULL != app_item_ptr)
        && (app_id == app_item_ptr->app_id)
       )
    {
        check_func_ptr = app_item_ptr->check_func_ptr;
        close_func_ptr = app_item_ptr->close_func_ptr;

        do_type = app_item_ptr->do_type;


        SCI_TRACE_LOW("[MMIDC] DestroyDCApp do_type=%d",\
                     do_type);                         
        
        if (MMIDC_APP_TYPE_QUERY == do_type)
        {
            if (PNULL != check_func_ptr)
            {
                if (check_func_ptr())
                {
                    SCI_TRACE_LOW("[MMIDC] DestroyDCApp close func"); 
                
                    close_func_ptr(&app_item_ptr->param, &app_item_ptr->param_size);
                }

                if (check_func_ptr())
                {
                    SCI_TRACE_LOW("[MMIDC] DestroyDCApp start timer");
                    
                    StartAppCloseTimer(delay_time);

                    ret = FALSE;
                }
            }

        }
    }

    SCI_TRACE_LOW("[MMIDC] DestroyDCApp ret=%d", ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : recheck app conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void RecheckAppConflict(void)
{
    MMI_WIN_ID_T win_id = MMIDC_STORAGE_OPT_WIN_ID;
    MMIDC_APP_E app_id = 0;


    SCI_TRACE_LOW("[MMIDC] RecheckAppConflict");
    
    //仍然有产生冲突的app
    if (GetDCAppConflictID(&app_id))
    {
        OpenDCAppQueryWin(app_id);
    }
    else
    {
        NotifyCaller(TRUE);

        MMK_CloseWin(win_id);
    }
}

/*****************************************************************************/
//  Description : resolve app  conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void ResolveDCAppConflict( 
                                 MMIPUB_INFO_T  *win_info_ptr
                                 )
{
    MMIDC_APP_E app_id = 0;
    BOOLEAN is_ok = FALSE;

    if ( PNULL != win_info_ptr )
    {   
        //init
        InitAppCloseTime();
    
        //close app
        app_id = win_info_ptr->user_data;
        is_ok = DestroyDCApp(app_id);

        if (is_ok)
        {
            //仍然有产生冲突的app
            RecheckAppConflict();
        }


        SCI_TRACE_LOW("[MMIDC] ResolveDCAppConflict app_id=%d",\
                      app_id);
                      
        SCI_TRACE_LOW("[MMIDC] ResolveDCAppConflict is_ok=%d",\
                              is_ok);                      
    }
}

/*****************************************************************************/
//  Description : create conflict ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void CreateDCAppConflictPtr( 
                             const MMIDC_APP_CONFLICT_T* conflict_ptr
                             )
{
    uint32 conflict_size = sizeof(MMIDC_APP_CONFLICT_T);
    MMIDC_APP_OK_FUNC_T *in_ok_func_ptr = PNULL;
    uint32 in_ok_param_size = 0;
    MMIDC_APP_CANCEL_FUNC_T *in_cancel_func_ptr = PNULL;
    uint32 in_cancel_param_size = 0;


    
    if ( PNULL != conflict_ptr )
    {
        DestroyDCAppConflictPtr();

        in_ok_func_ptr = (MMIDC_APP_OK_FUNC_T*)&conflict_ptr->ok_func_info;
        in_ok_param_size = in_ok_func_ptr->size_of_param;
        
        in_cancel_func_ptr = (MMIDC_APP_CANCEL_FUNC_T*)&conflict_ptr->cancel_func_info;
        in_cancel_param_size = in_cancel_func_ptr->size_of_param;

        
        s_mmidc_app_conflict_ptr = SCI_ALLOC_APP( conflict_size\
                         +  in_ok_param_size + in_cancel_param_size);

        if (PNULL != s_mmidc_app_conflict_ptr)
        {
            SCI_MEMCPY(s_mmidc_app_conflict_ptr, conflict_ptr, conflict_size);
            
            if (in_ok_param_size > 0)
            {
                s_mmidc_app_conflict_ptr->ok_func_info.param = (uint8*)s_mmidc_app_conflict_ptr + conflict_size;
                SCI_MEMCPY(s_mmidc_app_conflict_ptr->ok_func_info.param, \
                               in_ok_func_ptr->param,\
                               in_ok_param_size);
            }
            
            if (in_cancel_param_size > 0)
            {
                s_mmidc_app_conflict_ptr->cancel_func_info.param = (uint8*)s_mmidc_app_conflict_ptr \
                                                                 + conflict_size + in_ok_param_size;
                SCI_MEMCPY(s_mmidc_app_conflict_ptr->cancel_func_info.param,\
                              in_cancel_func_ptr->param,\
                              in_cancel_param_size);
            }
        }
    }
}

/*****************************************************************************/
//  Description : destroy conflict ptr
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void DestroyDCAppConflictPtr( void )
{
    SCI_TRACE_LOW("[MMIDC] DestroyDCAppConflictPtr");

    if ( PNULL != s_mmidc_app_conflict_ptr )
    {
        SCI_FREE(s_mmidc_app_conflict_ptr);
    }
}

/*****************************************************************************/
//  Description : notify resolve conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyCaller(BOOLEAN is_do_ok)
{
    MMIDC_APP_OK_FUNC_T *ok_func_ptr = GetDCAppOKFuncPtr();
    MMIDC_APP_CANCEL_FUNC_T *cancel_func_ptr = GetDCAppCancelFuncPtr();


    if (is_do_ok)
    {
        if ((PNULL != ok_func_ptr)
             && (PNULL != ok_func_ptr->ok_func)
           )
        {
            ok_func_ptr->ok_func(0, 0, ok_func_ptr->param);
        }
    }
    else
    {
        if ((PNULL != cancel_func_ptr)
            && (PNULL != cancel_func_ptr->cancel_func)
            )
        {
            cancel_func_ptr->cancel_func(cancel_func_ptr->param);
        }
    }


    SCI_TRACE_LOW("[MMIDC] NotifyCaller is_do_ok=%d",\
                 is_do_ok);    
}

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void OpenDCAppQueryWin(MMIDC_APP_E app_id)
{
    MMI_WIN_ID_T  win_id = MMIDC_STORAGE_OPT_WIN_ID;

    MMI_TEXT_ID_T title_text_id = TXT_COMMON_NO_MEMORY;
    MMI_TEXT_ID_T has_open_text_id = TXT_COMM_APP_OPENED;
    MMI_TEXT_ID_T query_text_id = TXT_COMM_IS_CLOSE;
    MMI_TEXT_ID_T app_text_id = GetDCAppTextIdByAppID(app_id);

    wchar tip_text[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMI_STRING_T  tip_text_str = {0};
    MMI_STRING_T  query_text_str = {0};
    MMI_STRING_T  app_text_str = {0};


    SCI_TRACE_LOW("[MMIDC] OpenDCAppQueryWin app_id=%d",
                  app_id);
    
    //get app name
    if ((TXT_NULL != app_text_id)
        && (0 != app_text_id)
       )
    {
        tip_text_str.wstr_ptr = tip_text;
        tip_text_str.wstr_len = MMIFILE_FILE_NAME_MAX_LEN;
        

        MMI_GetLabelTextByLang(app_text_id, &app_text_str);
        MMI_WSTRNCPY(tip_text_str.wstr_ptr, tip_text_str.wstr_len,
                    app_text_str.wstr_ptr, app_text_str.wstr_len,
                    app_text_str.wstr_len);
        tip_text_str.wstr_len = app_text_str.wstr_len;

        //padding space
        MMIAPICOM_Wstrncat(tip_text_str.wstr_ptr, L" ",1);
    }
    
    //get opened string                    
    MMI_GetLabelTextByLang(has_open_text_id, &app_text_str);
    MMIAPICOM_Wstrncat(tip_text_str.wstr_ptr, app_text_str.wstr_ptr,
                       app_text_str.wstr_len);
    tip_text_str.wstr_len = MMIAPICOM_Wstrlen(tip_text_str.wstr_ptr);

    //get query string
    MMI_GetLabelTextByLang(query_text_id, &query_text_str);

    
    if ( !MMK_IsOpenWin( win_id ) )
    {    
        MMIPUB_OpenQueryWinByTextPtrEx(MMK_GetFirstAppletHandle(),
                                           &tip_text_str, 
                                           &query_text_str, 
                                           IMAGE_PUBWIN_QUERY, 
                                           &win_id, 
                                           HandleDCAppConflictWinMsg,
                                           app_id
                                           );
        MMIPUB_SetWinTitleTextId(win_id, title_text_id, FALSE);                                      
    }
    else
    {
        MMIPUB_SetWinAddData(win_id, app_id);
        MMIPUB_SetAlertWinTextByPtr( win_id, &tip_text_str, &query_text_str, TRUE );
    }


    MMIPUB_SetWinSoftkeyEx(win_id, TXT_COMMON_OK, TXT_NULL, STXT_CANCEL, TRUE);
}

/*****************************************************************************/
//  Description : handle conflict win msg
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDCAppConflictWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        )
{
    MMI_RESULT_E   result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T  *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_TRACE_LOW("[MMIDC] HandleDCAppConflictWinMsg msg_id=0x%X",\
                 msg_id);

    switch (msg_id)
    {   
    case MSG_APP_MENU:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_TEXT_ID_T exit_text_id = STXT_EXIT;
            MMI_TEXT_ID_T tip_text_id = TXT_COMMON_WAITING;
            MMI_STRING_T  tip_text_str = {0};
            
            
            MMIPUB_SetWinSoftkeyEx(win_id, TXT_NULL, TXT_NULL, exit_text_id, FALSE);
            
            MMI_GetLabelTextByLang(tip_text_id, &tip_text_str);
            MMIPUB_SetAlertWinTextByPtr( win_id, &tip_text_str, PNULL, FALSE );

            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); 
            MMITHEME_SetUpdateDelayCount(0);
            MMITHEME_UpdateRect();
        }
        
        ResolveDCAppConflict( win_info_ptr );
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        NotifyCaller(FALSE);
        MMK_CloseWin( win_id );
        break;
    case MSG_TIMER:
         result = ProcessConflictTimer(*(uint8*)param, win_info_ptr->user_data);
         break;
    case MSG_CLOSE_WINDOW:
        CloseAppCloseTimer();
        
        DestroyDCAppConflictPtr();
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : check dc app conflict
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CheckAppConflict(
                                            const MMIDC_APP_CONFLICT_T* conflict_ptr
                                            )
{
    BOOLEAN ret = TRUE;
    MMIDC_APP_E app_id = 0;

    if ( PNULL == conflict_ptr )
    {
        return ret;
    }

    CreateDCAppConflictPtr( conflict_ptr );
    
    if (GetDCAppConflictID(&app_id))
    {
        OpenDCAppQueryWin(app_id);
    }
    else
    {        
        NotifyCaller(ret);

        DestroyDCAppConflictPtr();
    }


    SCI_TRACE_LOW("[MMIDC] MMIDC_CheckAppConflict ret=%d",\
                  ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : restore conflict app
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_RestoreConflictApp(MMIDC_APP_LIST_T *app_list_ptr)
{
    int32 i = 0;
    int32 total_num = 0;
    MMIDC_APP_ITEM_T *app_item_ptr = PNULL;
    MMIDC_APP_START_FUNC start_func_ptr = PNULL;
    BOOLEAN is_running = FALSE;    

    if ((PNULL != app_list_ptr)
        && (PNULL != app_list_ptr->array_ptr)
       )
    {
        total_num = app_list_ptr->total_num;

        SCI_TRACE_LOW("[MMIDC] MMIDC_RestoreConflictApp total_num=%d",\
                     total_num);
                     
        for (i = total_num - 1; i >= 0; i--)
        {
            app_item_ptr = &app_list_ptr->array_ptr[i];

            if (PNULL != app_item_ptr) /*lint !e774*/
            {
                start_func_ptr = app_item_ptr->start_func_ptr;
                is_running = app_item_ptr->is_running;


                SCI_TRACE_LOW("[MMIDC] MMIDC_RestoreConflictApp i=%d",i);
                SCI_TRACE_LOW("[MMIDC] MMIDC_RestoreConflictApp app_id=%d",\
                             app_item_ptr->app_id);
                SCI_TRACE_LOW("[MMIDC] MMIDC_RestoreConflictApp is_running=%d",\
                            is_running);
                
                if (is_running)
                {
                    app_item_ptr->is_running = FALSE;
                    
                    if (PNULL != start_func_ptr)
                    {
                        start_func_ptr(&app_item_ptr->param, &app_item_ptr->param_size);
                    }
                }
            }
        }
    }
}

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//  Description : check wifi 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDC_APP_CheckWifi(void)
{
    BOOLEAN ret = FALSE;

    
    if (MMIWIFI_STATUS_OFF != MMIAPIWIFI_GetStatus())
    {
        ret = TRUE;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_CheckWifi ret=%d",
                 ret);

    return ret;    
}

/*****************************************************************************/
//  Description : close wifi 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseWifi(void** param, //out
                                       uint32* param_size //out
                                       )
{
    MMIAPIWIFI_Off();
}
/*****************************************************************************/
//  Description : start wifi 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL void MMIDC_APP_StartWifi(void** param, //in,out
                                      uint32* param_size //in,out
                                      )
{
    MMIAPIWIFI_On(FALSE);
}
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT

/*****************************************************************************/
//  Description : close mp3
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseMP3(void** param, //out
                                        uint32* param_size //out
                                       )
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_CloseMP3");
    
    MMIAPIAP_DestroyByBG();
}

/*****************************************************************************/
//  Description : start mp3
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartMP3(void** param, //in,out
                                       uint32* param_size //in,out
                                       )
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_StartMP3");

    MMIAPIAP_StartByBG();
}
#endif

#ifdef EBOOK_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close ebook
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseEbook(void** param, //out
                                          uint32* param_size //out
                                          )
{
    MMIAPIEBOOK_WaitExit();
}

/*****************************************************************************/
//  Description : mmidc app start ebook
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartEbook(void** param, //in,out
                                         uint32* param_size //in,out
                                         )
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_StartEbook");


    MMIAPIEBOOK_ManagerWinMsg(TRUE);
}    
#endif

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close browser
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseBrowser(void** param, //out
                                             uint32* param_size //out
                                            )
{
    MMIAPIBROWSER_Close();
}

/*****************************************************************************/
//  Description : mmidc app start browser
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartBrowser(void** param, //in,out
                                            uint32* param_size //in,out
                                           )
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0};


    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_StartBrowser");
    
    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    
    MMIAPIBROWSER_Entry(&entry_param);
}
#endif

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close bluetooth
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseBlueTooth(void** param, //out
                                               uint32* param_size //out
                                               )
{
    MMIAPIBT_OpenBlueTooth(FALSE);
}

/*****************************************************************************/
//  Description : mmidc app start bluetooth
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartBlueTooth(void** param, //in,out
                                              uint32* param_size //in,out
                                              )
{
    MMIAPIBT_OpenBlueTooth(TRUE);
}
#endif

#ifdef DL_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close download
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseDL(void** param, //out
                                      uint32* param_size //out
                                      )
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_CloseDL");

    MMIAPIDL_StopDownload(0, TRUE);
}

/*****************************************************************************/
//  Description : mmidc app start download
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartDL(void** param, //in,out
                                      uint32* param_size //in,out
                                     )
{
    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_StartDL");
    
    MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
}        
#endif

#ifdef QBTHEME_SUPPORT
/*****************************************************************************/
//  Description : mmidc app check qbtheme
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIDC_APP_CheckQbTheme(void)
{
    BOOLEAN ret = FALSE;


    if(STYLE_IS_SUPPORT_QBTHEME)
    {
        ret = TRUE;
    }
    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_CheckQbTheme ret=%d",
                  ret);
    
    return ret;
}

/*****************************************************************************/
//  Description : mmidc app close qbtheme
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseQbTheme(void** param, //out
                                              uint32* param_size //out
                                              )
{
    MMISET_IDLE_STYLE_E idle_style = 0;
    MMISET_IDLE_STYLE_E cur_style = 0;
    void* param_ptr = PNULL;

    //search idle style
    #if !defined(MMI_ANDROID_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
         idle_style = MMISET_IDLE_ISTYLE;
    #endif
    
    #if defined MMI_GRID_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_GRID;
    #endif
    
    #if defined MMIWIDGET_SUPPORT
        idle_style = MMISET_IDLE_STYLE_WIDGET;
    #endif
        
    #ifdef MMI_COMMON_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_COMMON;
    #endif
        
    #ifdef DPHONE_SUPPORT 
    idle_style = MMISET_IDLE_STYLE_COMMON;
    #endif

    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_CloseQbTheme idle_style=%d",\
              idle_style);
    
    cur_style = MMIAPISET_GetIdleStyle();
    if(PNULL != param 
        && PNULL == *param 
        && PNULL != param_size)
    {
        param_ptr = SCI_ALLOC_APP(sizeof(cur_style));
        if(PNULL != param_ptr)
        {
            SCI_MEMCPY(param_ptr, &cur_style, sizeof(cur_style));
            *param = param_ptr;
            *param_size = sizeof(cur_style);
        }
    }
    //退出千变主题，需要释放内存
    MMIAPIQBTHEME_StyleStop();

    //MMISET_SetIdleStyle(idle_style);
}

/*****************************************************************************/
//  Description : mmidc app start qbtheme
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartQbTheme(void** param, //in,out
                                             uint32* param_size //in,out
                                             )
{
    MMISET_IDLE_STYLE_E idle_style = MMISET_IDLE_QBTHEME;

    if(PNULL != param 
        && PNULL != *param
        && PNULL != param_size)
    {
        SCI_MEMCPY(&idle_style, *param, sizeof(idle_style));
        SCI_FREE(*param);
        *param = PNULL;
        *param_size = 0;
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_APP_StartQbTheme");
    MMISET_SetIdleStyle(idle_style);
    

    //千变主题需要做一些初始化
    MMIAPIQBTHEME_StyleStart();
}
#endif

#ifdef MMIEMAIL_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close email
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseEmail(void** param, //out
                                         uint32* param_size //out
                                         )
{
    MMIAPIEMAIL_SyncEmail();
}

/*****************************************************************************/
//  Description : mmidc app start email
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartEmail(void** param, //in,out
                                        uint32* param_size //in,out
                                        )
{
    MMIAPIEMAIL_Entry();
}
#endif

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close dynamic mainmenu
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseDyMainmenu(void** param, //out
                                                  uint32* param_size //out
                                                  )
{
    MMIMENU_DyDestroy();
}
#endif

#ifdef PIC_VIEWER_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close pic view
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_ClosePicview(void** param, //out
                                            uint32* param_size //out
                                           )
{
    MMIAPIPICVIEW_ClosePicViewer();
}

/*****************************************************************************/
//  Description : mmidc app start picview
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartPicview(void** param, //in,out
                                           uint32* param_size //in,out
                                          )
{
    MMIAPIPICVIEW_OpenPicViewer();
}
#endif

#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close auto down mms
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseAutodownMMS(void** param, //out
                                                     uint32* param_size //out
                                                    )
{
    MMIAPIMMS_StopAutodownMMS();
}

/*****************************************************************************/
//  Description : mmidc app start auto down mms
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartAutodownMMS(void** param, //in,out
                                                    uint32* param_size //in,out
                                                   )
{
    MMIAPIMMS_RestartAutodownMMS();
}
#endif
    
#ifdef MOBILE_VIDEO_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close mv
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseMV(void** param, //out
                                       uint32* param_size //out
                                      )
{
    MMIAPIMV_Exit();
}

/*****************************************************************************/
//  Description : mmidc app start mv
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartMV(void** param, //in,out
                                      uint32* param_size //in,out
                                     )
{
    MMIAPIMV_Entry();
}
#endif

#ifdef MMIUZONE_SUPPORT
/*****************************************************************************/
//  Description : mmidc app close uzone
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_CloseUZone(void** param, //out
                                          uint32* param_size //out
                                          )
{
    MMIAPIUZONE_Exit();
}

/*****************************************************************************/
//  Description : mmidc app start uzone
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void MMIDC_APP_StartUZone(void** param, //in,out
                                          uint32* param_size //in,out
                                         )
{
    MMIAPIUZONE_Entry();
}
#endif

/*****************************************************************************/
//  Description : get dc app total number
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetDCAppTotalNumber(void)
{
    uint32 total_num = ARR_SIZE(s_mmidc_app_array);

    
    return total_num;
}

/*****************************************************************************/
//  Description : get dc app array
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_APP_ITEM_T* MMIDC_GetDCAppArrayPtr(void)
{
    return (MMIDC_APP_ITEM_T*)s_mmidc_app_array;
}

/*****************************************************************************/
//  Description : restore 2m sensor environment
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_Restore2MSensorEnvironment(BOOLEAN is_check)
{
    MMIDC_APP_LIST_T app_list_info = {0};

    
    //sensor 2M
    if (is_check)
    {
        app_list_info.total_num = MMIDC_GetDCAppTotalNumber();
        app_list_info.array_ptr = MMIDC_GetDCAppArrayPtr();
        
        MMIDC_RestoreConflictApp(&app_list_info);
    }

    SCI_TRACE_LOW("[MMIDC] MMIDC_Restore2MSensorEnvironment is_check=%d",\
                 is_check);
}
#endif  //#ifdef  CAMERA_SUPPORT

