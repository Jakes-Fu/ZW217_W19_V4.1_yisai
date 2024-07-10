#define _MV_SMS_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "_sms.h"
#include "mn_type.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv_sms_api.h"
#include "mmicom_string.h"
#include "mmi_id.h"
#include "mmimv_export.h"
#include "mmimv_id.h"
#include "mv_mm_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

//LOCAL SMSCallbackProc s_mv_SMS_callbackfunc = PNULL;
//LOCAL _VOID*          s_mv_SMS_callbackpara = PNULL;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : CM_SMS_AppendCallbackFunc
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
#if 0
_HANDLE CM_SMS_AppendCallbackFunc(
									_INT nPriority,
									SMSCallbackProc pSMSCallbackProc,
									_VOID* pSMSCallbackProcParam
)
{
    // 不用关心优先级
    s_mv_SMS_callbackfunc = pSMSCallbackProc;
    s_mv_SMS_callbackpara = pSMSCallbackProcParam;

    return (_HANDLE)1;
}

/*****************************************************************************/
//  Description : CM_SMS_RemoveCallbackFunc
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_SMS_RemoveCallbackFunc(_HANDLE hSMSHandle)
{
    s_mv_SMS_callbackfunc = PNULL;
    s_mv_SMS_callbackpara = PNULL;
    	
    return _TRUE;
}
#endif
/*****************************************************************************/
//  Description : CM_IsMvSms
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_IsMvSms(APP_SMS_USER_DATA_T* sms_user_data_ptr)
{
    BOOLEAN check_sms = FALSE;
    uint8 ascii_vaild[MN_SMS_MAX_USER_VALID_DATA_LENGTH] = {0};
    _WCHAR conten[MN_SMS_MAX_USER_VALID_DATA_LENGTH] = {0};
    uint8 sms_head[10]={0};
    _WCHAR wchar_sms_head[10]={0};
    uint8 content_len = 0;
    MN_SMS_USER_DATA_HEAD_T * pHead = PNULL;
    MN_SMS_USER_VALID_DATA_T* pData = PNULL;
    
    pHead = &sms_user_data_ptr->user_data_head_t;
    pData = &sms_user_data_ptr->user_valid_data_t;

#ifdef DEBUG_SMS
    SCI_TRACE_BUF("[MV] CM_IsMvSms user_header_arr =", pHead->user_header_arr, pHead->length);
#endif
    
    if (0x6 == pHead->length
        && MN_SMS_APP_PORT_16_BIT_ADDR == pHead->user_header_arr[0]
        && 4 == pHead->user_header_arr[1]
        && 0x40 == pHead->user_header_arr[2]
        && 0x74 == pHead->user_header_arr[3])
    {
    
        check_sms = TRUE;
        content_len = MMIAPICOM_Default2Ascii(
            (uint8 *)(pData->user_valid_data_arr),
            (uint8 *)ascii_vaild,
            (uint16)(pData->length));
            
        MMIAPICOM_StrToWstr((uint8*)ascii_vaild, (wchar *)conten);
    #ifdef DEBUG_SMS
        //MV_TRACE_LOW:"[MV] MMISMS_HandleSMSMTInd ascii_vaild = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_SMS_API_110_112_2_18_2_39_21_184,(uint8*)"s", ascii_vaild);
    #endif
        SCI_MEMSET(sms_head, 0, sizeof(sms_head));
        SCI_STRCPY((char*)sms_head, (char *)"16500");
        MMIAPICOM_StrToWstr((uint8*)sms_head, (wchar *)wchar_sms_head);

        if(MMK_IsOpenWin(MMIMV_MAIN_WIN_ID))
        {
            // s_mv_SMS_callbackfunc(ESMEEvent_NewMessage,(_UINT32)conten,
            //     (_UINT32)wchar_sms_head,s_mv_SMS_callbackpara);
        #ifndef WIN32
            CM_Check_SMS((_UINT32)conten,(_UINT32)wchar_sms_head,0);
        #endif
        }
        else
        {
            MV_CreatMemHandle();
        #ifndef WIN32
            check_sms = CM_Check_SMS((_UINT32)conten, (_UINT32)wchar_sms_head, 0);
        #endif
            if(check_sms && MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
            {            
                if(!MMIAPIMV_Entry())
                {
                    MV_DeleteMemHandle();
                }
            }
            else
            {
                MV_DeleteMemHandle();
            }
        }

    }

#ifdef DEBUG_SMS    
    //MV_TRACE_LOW:"[MV] MMISMS_HandleSMSMTInd check_sms = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_SMS_API_146_112_2_18_2_39_21_185,(uint8*)"d", check_sms);
#endif

    if(check_sms)
    {
        return _TRUE;
    }

    return _FALSE;
}


