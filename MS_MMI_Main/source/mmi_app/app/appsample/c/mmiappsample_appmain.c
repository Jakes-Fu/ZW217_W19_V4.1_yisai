
/*****************************************************************************
** File Name:      mmiappsample_appmain.c                                               *
** Author:                                                          *
** Date:                                                          *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
                                *
******************************************************************************/

#include "mmiappsample_internal.h"
#include "mmi_event_api.h"
#include "mmk_app.h"
#include "mmi_applet_table.h"


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


typedef struct
{
    MMI_HANDLE_T event_win_handle[MAX_APPSAMPLE_EVENT_WIN_NUM];
    BOOLEAN      event_registered[MAX_APPSAMPLE_EVENT_WIN_NUM];
    MMI_HANDLE_T event_process_handle;
    MMI_HANDLE_T read_completed_handle;
    BOOLEAN      isProcessRegisted;
} MMIAPPSAMPLE_EVENT_T;

typedef struct
{
    CAF_APPLET_T caf_applet;
    uint32       test_num;

} APPSAMPLE_APPLET_T;


/**--------------------------------------------------------------------------*
**                         FUNCTION DECALARION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register sample module applet info
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void);
/*****************************************************************************/
//  Description : the process message function of applet
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  AppletSampleHandleEvent(
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);


/********************************************************************
//  Description :sample app event handler
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL void MMIAPPSAMPLE_HandleEvent(MMISAMPLE_APP_EVENT_E event, uint32 param_ptr, uint32 size);


/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**--------------------------------------------------------------------------*/
LOCAL MMIAPPSAMPLE_EVENT_T appsample_event_data = {0};
MMI_APPLICATION_T           g_appsample_app;

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmiappsample_applet_array[] =
{
    { GUID_NAME_DEF(SPRD_APPSAMPLE_APPLET_ID), sizeof(APPSAMPLE_APPLET_T), AppletSampleHandleEvent, 0,0 },
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T s_mmiappsample_applet_info =
{
    (CAF_STATIC_APPLET_T*)s_mmiappsample_applet_array,
    ARR_SIZE(s_mmiappsample_applet_array)
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/********************************************************************
//  Description :sample app init
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPIAPPSAMPLE_InitModule(void)
{
    MMIAPPSAMPLE_RegMenuGroup();
    MMIAPPSAMPLE_RegNv();
    MMIAPPSAMPLE_RegWinIdNameArr();
    RegAppletInfo();
}


/*****************************************************************************/
//  Description : register sample module applet info
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_APPSAMPLE, &s_mmiappsample_applet_info );
}

/*****************************************************************************/
//  Description : the process message function of applet
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  AppletSampleHandleEvent(
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E     result     = MMI_RESULT_TRUE;
    APPSAMPLE_APPLET_T* applet_ptr = (APPSAMPLE_APPLET_T*)iapplet_ptr;

    switch (msg_id)
    {
        //applet启动
        case MSG_APPLET_START:
        {
            MMK_SampleRegisterEvent(MMIAPPSAMPLE_HandleEvent);
            EnterAppSampleMainWin(applet_ptr->caf_applet.app_handle);
        }
        break;

        case MSG_APPLET_STOP:
            MMK_SampleUnregisterEvent(MMIAPPSAMPLE_HandleEvent);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

/********************************************************************
//  Description :sample app error/warning handler
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_RaiseError(uint32 errorcode)
{
    SCI_TRACE_LOW("MMIAPPSAMPLE_TRACE error code =0x%x", errorcode);
    return;
}



/********************************************************************
//  Description :sample app event handler
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
LOCAL void MMIAPPSAMPLE_HandleEvent(MMISAMPLE_APP_EVENT_E event, uint32 param_ptr, uint32 size)
{
    int i = 0;

    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE MMIAPPSAMPLE_HandleEvent event id = %d", event);

    if (event == MMISAMPLE_APP_UPDATE)
    {
        for (i = 0; i < MAX_APPSAMPLE_EVENT_WIN_NUM; i++)
        {
            if (appsample_event_data.event_registered[i])
            {
                //Notify it
                MMK_SetWinDirty(appsample_event_data.event_win_handle[i], TRUE);
                MMK_PostMsg(appsample_event_data.event_win_handle[i],
                            MSG_FULL_PAINT, 0, 0);
            }
        }
    }
    else if (event == MMISAMPLE_APP_CHANGE_COMPLETED)
    {
        if ( appsample_event_data.isProcessRegisted)
        {
            MMK_PostMsg(appsample_event_data.event_win_handle[i],
                        MSG_FULL_PAINT, 0, 0);
            appsample_event_data.isProcessRegisted = FALSE;
        }
    }
    else if (event == MMISAMPLE_APP_READ_COMPLETED)
    {
        if ( appsample_event_data.read_completed_handle != PNULL)
        {
            //Call readcompleted
            appsample_event_data.read_completed_handle = PNULL;
        }
    }
    else
    {
        MMIAPPSAMPLE_RaiseError(0x0000);
    }

    return;
}

/********************************************************************
//  Description :sample app data changed event Register
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_RegisterDateChangeEvent(MMI_HANDLE_T win_handle, uint32 index)
{
    appsample_event_data.event_win_handle[index] = win_handle;
    appsample_event_data.event_registered[index] = TRUE;
}

/********************************************************************
//  Description :sample app data changed event Unregister
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_UnRegisterDateChangeEvent(uint32 index)
{
    appsample_event_data.event_registered[index] = FALSE;

}

/********************************************************************
//  Description :sample app process event register
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_RegisterProcessEvent(MMI_HANDLE_T win_handle, uint32 index)
{
    appsample_event_data.event_process_handle = win_handle;
    appsample_event_data.isProcessRegisted = TRUE;
}

/********************************************************************
//  Description :sample app process event unregister
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_UnRegisterProcessEvent(uint32 index)
{
    appsample_event_data.isProcessRegisted = FALSE;

}

/*****************************************************************************/
//  Description : Start app sample applet
//  Global resource dependence :
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL void MMIAPPSAMPLE_StartApplet(void)
{
    MMI_APPLET_START_T start = {0};
    uint32 user_data = 0;

    MMIAPPSAMPLE_TRACE("MMIAPPSAMPLE MMIAPPSAMPLE_StartApplet");
    start.guid = SPRD_APPSAMPLE_APPLET_ID;
    start.state = MMI_APPLET_STATE_CLOSE_WITH_WIN;
    start.param_ptr = &user_data;
    start.size_of_param = sizeof(uint32);
    MMK_StartApplet( &start );
}


/********************************************************************
//  Description :sample app Entry
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPIAPPSAMPLE_Enter(void)
{
    MMIAPPSAMPLE_StartApplet();
    return;
}



