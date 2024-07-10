/*************************************************************************
** File Name:      mmisearch_export.h                                           *
** Author:         plum.peng                                             *
** Date:           16/03/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:                  *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 16/03/2010     plum.peng        Create.                              *
*************************************************************************/

#ifndef _MMISEARCH_H_
#define _MMISEARCH_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "mmk_type.h"
#include "guilistbox.h"

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
 #define MMISEARCH_MATCHED_NUM_MAX 2000
#define MMISEARCH_MAX_KEYWORD_LENS        60
//#define FMM_SD_DISK     0x0045
#define MMISEARCH_DETAIL_STR_MAX_LEN       30

#define MMIAPP_SEARCH_MAX_NUM               160
typedef void(*MMISEARCH_APP_OPEN_PFUNC)(uint32 user_data);//匹配项执行"打开"callback
typedef void(*MMISEARCH_APP_SETDATA_PFUNC)(GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr, uint32 user_data);//列表中的匹配项的单项显示callback
typedef void(*MMISEARCH_APP_SETCONTENT_PFUNC)(GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr, uint32 user_data);//列表中的匹配项的单项显示callback(动态图片)

typedef enum
{
    MMISEARCH_TYPE_APP,
    MMISEARCH_TYPE_CL,
    MMISEARCH_TYPE_PB,
    MMISEARCH_TYPE_SMS,
    MMISEARCH_TYPE_FILE,
    MMISEARCH_TYPE_ALL,
    MMISEARCH_TYPE_MAX,
    MMISEARCH_TYPE_WEB,
    MMISEARCH_TYPE_PROMPT_EMPTY,
//    MMISEARCH_TYPE_MMS,
} MMISEARCH_TYPE_E;

typedef enum
{
    MMISEARCH_STATUS_IDLE,//某类型搜索未启动
    MMISEARCH_STATUS_START,//某类型搜索已启动
    MMISEARCH_STATUS_END,//某类型搜索已结束
    MMISEARCH_STATUS_INVALID,//某类型搜索结果已无效
    MMISEARCH_STATUS_MAX,
} MMISEARCH_STATUS_E;

typedef struct
{
    MMISEARCH_TYPE_E match_item_type;
    GUIITEM_STYLE_E match_item_style;
    MMISEARCH_APP_OPEN_PFUNC open_pfunc;
    MMISEARCH_APP_SETDATA_PFUNC set_data_pfunc;
    MMISEARCH_APP_SETCONTENT_PFUNC set_content_pfunc;
    uint32 user_data;//open_pfunc、set_data_pfunc中用于回传的参数(指针/索引/数值，即应用可以识别该项的一个标识)
} MMISEARCH_MATCH_ITEM_T;

typedef struct
{
    uint32 match_num;
    MMISEARCH_MATCH_ITEM_T *match_item_ptr;
} MMISEARCH_DATA_T;

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init pic viewer module
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_InitModule(void);    

/*****************************************************************************/
//  Description : istyle search entry of local search
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_IstyleEntry(void);

/*****************************************************************************/
//  Description : stop search for i-style
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_IstyleStop(void);

/*****************************************************************************/
//  Description : leave i-style search
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_IstyleExit(void);

/*****************************************************************************/
//  Description : main entry of local search
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_Entry(void);

#ifdef MMISEARCH_SUPPORT_ONLYMMS
/*****************************************************************************/
//  Description : main entry of local search
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCHMMS_Entry(void);
#endif

/*****************************************************************************/
//  Description : async search call this func after search stopped 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_StopAsyncSearchCnf(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description : to stop search operation especially for asyn-search 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_StopSearching(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description : to get if async searching is runing (sms or file)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsAsyncSearching(void);

/*****************************************************************************/
//  Description : set search window handle and list handle
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_SetSearchWinHandle(MMI_HANDLE_T    win_handle);

/*****************************************************************************/
//  Description : handle search contrl msg 
//  Global resource dependence : 
//  Author: Xinhe.Yan
//  Note: search control window to separate view and mesaage flow
/*****************************************************************************/
PUBLIC MMI_RESULT_E  MMIAPISEARCH_HandleControlMsg(
                                               MMI_WIN_ID_T		win_id, 	
                                               MMI_MESSAGE_ID_E	msg_id,
                                               DPARAM				param
                                               );

/*****************************************************************************/
//  Description : 保存匹配数据至数据区(外部接口)
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_SaveMatchItems(MMISEARCH_DATA_T match_data);

/*****************************************************************************/
//  Description : 启动搜索(外部接口)
 //  Global resource dependence : 
 //  Author: hermann liu
 //  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_StartSearch(MMISEARCH_TYPE_E type, MMI_STRING_T *keyword_str);
/*****************************************************************************/
//  Description : to check if process of search is doing by type, especially for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsSearching(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description : to check if process of search is doing or done already by type, especially for asyn-search 
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsSearched(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_AppUpdateInd(MMISEARCH_TYPE_E type);
/*****************************************************************************/
//  Description : handle main search win msg 
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_ClearCurrFileMatchNum(void);
/*****************************************************************************/
//  Description : to exit search operation especially for asyn-search (stop)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_ExitSearching(MMISEARCH_TYPE_E type);

/*****************************************************************************/
//  Description : Get Edit Box String
//  Global resource dependence : 
//  Author: hermann liu
//  Note: search control window to separate view and mesaage flow
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_GetEditBoxString(MMI_STRING_T   *str_ptr );

/*****************************************************************************/
//  Description : to check if search is need to be restart, because some match item is invalid
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISEARCH_IsNeedSearchAgain(void);
/*****************************************************************************/
//  Description : set is need to be restart
//  Global resource dependence : 
//  Author: tao.xue
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISEARCH_ResetAllIsNeedRestart(void);
#endif
    

