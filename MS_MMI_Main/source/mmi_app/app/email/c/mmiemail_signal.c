/***************************************************************************************
** File Name:      mmiemail_signal.c                                                     *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    This File will gather functions that all events to this app are     *
**                 handled. These functions don't be changed by project changed.       *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_comm.c                              *
**                                                                                     *
***************************************************************************************/

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "os_api.h"
#include "tasks_id.h"
#include "mmiemail_signal.h"
#include "mmiemail_utils.h"
#include "mmiemail_edit.h"
#include "mmiemail_export.h"
#include "priority_app.h"
#define TASK_EMAIL_APP_PRI    PRI_EMAIL_APP_TASK  //30

/**------------------------------------------------------------------------------------*
**                         Macro Declaration    						               *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT    				 	                   *
**------------------------------------------------------------------------------------*/
typedef struct
{
    SIGNAL_VARS
    void    *pMsg;
}MMIEMAIL_APP_SIG_T;

/**------------------------------------------------------------------------------------*
**                         Constant Variables                                          *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         STATIC DEFINITION                                           *
**------------------------------------------------------------------------------------*/
BLOCK_ID g_email_app_task_id = 0;
static SCI_MUTEX_PTR g_email_app_mutex = (void*)0;

static BOOLEAN g_email_enable = TRUE; 
/*-------------------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                                     *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : email app process func
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEmail_ThreadEnable(void)
{
   g_email_enable = TRUE;
}

PUBLIC void MMIEmail_ThreadDisable(void)
{
   g_email_enable = FALSE;
}

PUBLIC void MMIEMAIL_GetMutex(void)
{
   SCI_GetMutex(g_email_app_mutex, SCI_WAIT_FOREVER);
}

PUBLIC void MMIEMAIL_PutMutex(void)
{
   SCI_PutMutex(g_email_app_mutex);
}

static void EmailAppProc(uint32 argc, void *argv)
{
   MMIEMAIL_APP_SIG_T *pstSig = NULL;
   int32   result = 0;
   
   while(TRUE)/*lint !e716*/
   {
      pstSig = (MMIEMAIL_APP_SIG_T*)SCI_GetSignal(SCI_IdentifyThread());
      if(pstSig)
      {
          MMIEMAIL_GetMutex();
          
          if(g_email_enable)
          {
              switch(pstSig->SignalCode)
              {
              case MMIEMAIL_EVT_SAVE_EMAIL:
                  result = MMIEMAIL_EditSaveAsDraft();
                  MMIEMAIL_EmailEventInterface(MMIEMAIL_MMI_APP, EMAIL_SIG_SAVE_EMAIL_RET, (void*)result, PNULL);
                  break;
              case MMIEMAIL_EVT_SAVE_AND_SEND_EMAIL:
                  result = MMIEMAIL_EditSendEmail();
                  MMIEMAIL_EmailEventInterface(MMIEMAIL_MMI_APP, EMAIL_SIG_SEND_EMAIL_RET, (void*)result, PNULL);
                  break;
              default:
                  break;
              }
          }
          
          SCI_FREE(pstSig);
          MMIEMAIL_PutMutex();
      }
   }
   
   return;/*lint !e527*/
}

/**************************************************************************************/
// Description : create email task thread
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/



unsigned int MMIEMAIL_CreateEmailTaskThread(void)
{
   if(0 == g_email_app_task_id)
   {  
      g_email_app_task_id = SCI_CreateThread("TS_Email_App_Task",
         "TS_Email_App_Queue",
         EmailAppProc,
         g_email_app_task_id,
         0,
         20480,
         32,
         TASK_EMAIL_APP_PRI/*SCI_PRIORITY_IDLE*/,
         SCI_PREEMPT,
         SCI_DONT_START);
      
      if(SCI_INVALID_BLOCK_ID == g_email_app_task_id || !g_email_app_task_id)
      {
         return EMAIL_ERROR;
      }

	  SCI_ResumeThread(g_email_app_task_id);
   }
   
   g_email_app_mutex = SCI_CreateMutex("TS_Email_App_Mutex", SCI_INHERIT);

   return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : delete email task thread
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
unsigned int MMIEMAIL_DeleteEmailTaskThread(void)
{
   if(g_email_app_task_id)
   {
      SCI_DeleteThread(g_email_app_task_id);
      g_email_app_task_id = 0;
   }

   if(g_email_app_mutex)
   {
      SCI_DeleteMutex(g_email_app_mutex);
      g_email_app_mutex = 0;
   }

   return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : email event interface
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
unsigned int MMIEMAIL_EmailEventInterface(int vAppType , int vEventType, void* pMsg, void *hWnd)
{
   if(0 == g_email_app_task_id || !g_email_enable)
   {
      return FALSE;
   }

   EMA_UTILS_TRACE("MMIEMAIL_EmailEventInterface--: vAppType = %d, EventType = %d", vAppType, vEventType);
   
   switch (vAppType)
   {
   case MMIEMAIL_MMI_APP:
      {
         MMIEMAIL_APP_SIG_T *pstSig = SCI_ALLOC_APP(sizeof(MMIEMAIL_APP_SIG_T));
         if(!pstSig)
            return FALSE;
         EMA_UTILS_MEMSET(pstSig, 0x00, sizeof(MMIEMAIL_APP_SIG_T));

         pstSig->SignalSize = sizeof(MMIEMAIL_APP_SIG_T);
         pstSig->SignalCode = vEventType;
         pstSig->pMsg = pMsg;
         
         if(SCI_SendSignal((xSignalHeader)pstSig, P_APP) == SCI_SUCCESS)
            return TRUE;
         else
            return FALSE;
      }
   case MMIEMAIL_EMAIL_APP:
      {
         MMIEMAIL_APP_SIG_T *pstSig = SCI_ALLOCA(sizeof(MMIEMAIL_APP_SIG_T));
         if(!pstSig)
            return FALSE;

         EMA_UTILS_MEMSET(pstSig, 0x00, sizeof(MMIEMAIL_APP_SIG_T));
         
         pstSig->SignalSize  = sizeof(MMIEMAIL_APP_SIG_T);
         pstSig->SignalCode  = vEventType;
         pstSig->pMsg        = pMsg;

         if(SCI_SendSignal((xSignalHeader)pstSig, g_email_app_task_id) == SCI_SUCCESS)
         {
            return TRUE;
         }
         else
         {
            return FALSE;
         }
      }
   default:
      return FALSE;
   }
   
   //return FALSE;
}
