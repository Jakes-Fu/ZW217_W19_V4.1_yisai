/*************************************************************************
** File Name:      mmisearch_internal.h                                 *
** Author:         tao.xue                                              *
** Date:           16/01/2012                                           *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.   *
** Description:                                                         *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 16/01/2012     tao.xue          Create.                              *
*************************************************************************/

#ifndef _MMISEARCH_INTERNAL_H_
#define _MMISEARCH_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "mmk_type.h"
#include "mmisearch_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMISEARCH_SUFFIX_MAX_LEN           10  //include dot
#define MMISEARCH_DETAIL_STR_MAX_LEN       30

#define MMISEARCH_KEYWORD_LENGTH_MAX 30

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start searching with type
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_EntrySearching(MMI_STRING_T *keyword_str_ptr);

/*****************************************************************************/
//  Description : ���ƥ���������е�һ��
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetSearchType(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description : �������
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
PUBLIC MMISEARCH_TYPE_E MMISEARCH_GetSearchType(void);

/*****************************************************************************/
//  Description : ���ƥ���������е�һ��
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC MMISEARCH_MATCH_ITEM_T* MMISEARCH_GetOneMatchItemPtr(uint32 index);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_ResetAppendFlag(void);

/*****************************************************************************/
//  Description : to stop update timer of search window for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_IsAsyncUpdataTimerStopped(void);

/*****************************************************************************/
//  Description : to stop update timer of search window for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_StopAsyncUpdataTimer(void);
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
/*****************************************************************************/
//  Description : to stop update timer of search window for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_StopPbAsyncUpdataTimer(void);
#endif
/*****************************************************************************/
// 	Description : MMISEARCH_OpenMainWin
//	Global resource dependence :
//  Author:        tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_OpenMainWin(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_StartFileSearching(void);

/*****************************************************************************/
//  Description : ��ȡƥ�����ݸ���
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISEARCH_GetMatchItemsNum(void);

/*****************************************************************************/
//  Description :���� ���ڽ�������ʾ��ƥ�����ݸ���
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetDisplayItemsNum(uint32 displayed_num);

/*****************************************************************************/
//  Description :��� ���ڽ�������ʾ��ƥ�����ݸ���
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISEARCH_GetDisplayItemsNum(void);

/*****************************************************************************/
//  Description :�Ƿ�ƥ����������ʾ���?
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_IsDisplayItemsEnd(void);

/*****************************************************************************/
//  Description : free search module data
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_Free(void);

/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_FreeLocalSearchResource(void);


/*****************************************************************************/
//  Description : to set search status by type 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetSearchStatus(MMISEARCH_TYPE_E type, MMISEARCH_STATUS_E search_status);

/*****************************************************************************/
//  Description : to get search status by type 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC MMISEARCH_STATUS_E MMISEARCH_GetSearchStatus(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description : set is need to be restart
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetIsNeedRestart(MMISEARCH_TYPE_E type, BOOLEAN is_need_restart);

/*****************************************************************************/
//  Description : get is need to be restart
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_GetIsNeedRestart(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description : set search window handle and list handle
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMISEARCH_GetSearchWinHandle(void);

/*****************************************************************************/
// 	Description : ���������˳���appģ���ͷ�ģ���ڲ���Դ
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIAPP_FreeLocalSearchResource(void);

/*****************************************************************************/
// 	Description : ����������appӦ�������ӿ�
//	Global resource dependence : 
//  Author:Xinhe.Yan
//	Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_LocalSearchAPP(
                                  const MMI_STRING_T *key_word_str, 
                                  MMISEARCH_DATA_T  *match_data_ptr,
                                  uint32 max_num
                                  );

/*****************************************************************************/
//  Description : Is Enter From BrwSearch Item
//  Global resource dependence : 
//  Author: hermann liu
//  Note: search control window to separate view and mesaage flow
/*****************************************************************************/
// PUBLIC BOOLEAN MMISEARCH_IsEnterFromBrwSearchItem(void);

/*****************************************************************************/
//  Description : set callback func after async search stopped
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_SetSearchStoppedCallBack(MMISEARCH_APP_OPEN_PFUNC open_pfunc, uint32 user_data);

/*****************************************************************************/
//  Description : run callback func after async search stopped
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_RunSearchStoppedCallBack(void);

/*****************************************************************************/
//  Description : to get if search process is end  
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_IsSearchEnd(void);

/*****************************************************************************/
//  Description : to stop async searching(sms and file)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_StopAsyncSearch(void); 

/*****************************************************************************/
//  Description : insert item of " No matches"
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
PUBLIC void MMISEARCH_InsertPromptNoMatchesItem(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:        tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMISEARCH_ResetSplitNum(void);

/*****************************************************************************/
//  Description : use notification item
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMISearch_AddNotificationItemToFile(MMI_TEXT_ID_T text_id);


#ifdef MMISEARCH_SUPPORT_ONLYMMS     //if mms only,couldn't changed type
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISEARCH_isOnlyMMSSearch(void);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_STRING_T MMISEARCH_GetKeyWord();
#endif
    

