/****************************************************************************
** File Name:      mmiikeybackup_wintab.c                                   *
** Author:         haiwu.chen                                               *
** Date:           12/25/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/25/2011     haiwu.chen        Create                                 *
****************************************************************************/
#include "mmi_app_ikeybackup_trc.h"
#ifdef IKEYBACKUP_SUPPORT


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "sci_types.h"
#include "os_api.h"
#include "guitab.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_app.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiikeybackup_wintab.h"
#include "mmiikeybackup_id.h"
#include "mmiikeybackup_api.h"
#include "mmiikeybackup_internal.h"
#include "mmiikeybackup_task.h"
#include "mmifmm_export.h"
#include "mmipub.h"
#include "guiform.h"
#include "mmitheme_pos.h"
#include "guiiconlist.h"
#include "mmiikeybackup_image.h"
#include "mmiikeybackup_menutable.h"
#include "mmisms_text.h"
#include "mmipb_text.h"
#include "mmiudisk_export.h"

#if 1
#include "tasks_id.h"
#include "mmi_signal.h"
#endif

#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif

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
#define MMIIKB_UI_SELECT_ALL_VALUE          (0xFF)
#define MMIIKB_UI_SETTING_LIST_NUM_MAX      (2)
#define MMIIKB_UI_PACK_NAME_BEGIN           (11)
#define MMIIKB_UI_PACK_NAME_LEN             (19)
#define MMIIKB_UI_TOTAL_APP_BUF_LEN         (40)//(20)
#define MMIIKB_BACKUP_PWD_MIN_LEN           (4)
#define MMIIKB_BACKUP_PWD_MAX_LEN           (8)
#define MMIIKB_PRO_TIPS_MAX_LEN             (50)

//version manage 0001.00001-> 0001.00003...->0012.00013
#define MMIIKB_VERSION_FIRST_NUM_LEN        (4)
#define MMIIKB_VERSION_SENCODE_NUM_LEN      (5)
#define MMIIKB_VERSION_NUM_LEN              (MMIIKB_VERSION_FIRST_NUM_LEN+\
                                            MMIIKB_VERSION_SENCODE_NUM_LEN+1)\

#define BACKUP_PWD_LEN_IS_IN_RANGE(_LEN)      \
    ((MMIIKB_BACKUP_PWD_MIN_LEN <= (_LEN))    \
    && (MMIIKB_BACKUP_PWD_MAX_LEN >= (_LEN))) \
    
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIIKB_UI_TAB_NONE,
    MMIIKB_UI_TAB_BACKUP,
    MMIIKB_UI_TAB_RESTORE,
    MMIIKB_UI_TAB_SETTING,
    
    MMIIKB_UI_TAB_MAX
}MMIIKB_UI_TAB_TYPE_E;


//backup ui
typedef struct
{
    uint32 user_data;
    MMI_IMAGE_ID_T image_id1;
    MMI_TEXT_ID_T text_id1;
    wchar text2_buf[MMIIKB_UI_TOTAL_APP_BUF_LEN];
    uint16 text2_len;
    uint32 total;
    BOOLEAN is_check;
    BOOLEAN is_gray;
}MMIIKB_UI_SEL_ITEM_LIST_T;

typedef struct
{
    uint16 intem_index;
    MMIIKB_TYPE_T types;
    MMIIKB_PASSWORD_INFO_T psw_info_t;    
}MMIIKB_CHECK_PSW_WIN_DATA_T;

typedef struct
{
    uint16 del_type;        //1: just delete one; 2: delete all pack
    uint16 del_index;       //used when del_type = 1
}MMIIKB_DEL_PACK_PARAM_T;

typedef struct
{
    uint32 txt_id;
    MMIIKB_STORE_POS_E pos;
}MMIIKB_POS_TEXT_T;



/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/
LOCAL const uint16 s_packinfo_name[] = {
    'i','b','p','h','o','n','e','_','h', 'e','a','d','.','i', 'n', 0};
LOCAL MMIIKB_UI_TAB_TYPE_E s_cur_ui_tab_type = MMIIKB_UI_TAB_BACKUP;
LOCAL MMIIKB_UI_SEL_ITEM_LIST_T s_ui_backup_list[] = 
{
    //{MMIIKB_UI_SELECT_ALL_VALUE,IMAGE_IKB_TEST_CC,TXT_IKB_SELECT_ALL, 0, FALSE},
    {MMIIKB_TYPE_CALLLOG,   IMAGE_SECMENU_ICON_CALL_LIST,       STXT_MAIN_CL,       {0}, 0, 0, FALSE, FALSE},
    {MMIIKB_TYPE_PHONEBOOK, IMAGE_SECMENU_ICON_TOOL_CL,         STXT_MAIN_PB,       {0}, 0, 0, FALSE, FALSE},
    {MMIIKB_TYPE_SMS,       IMAGE_SECMENU_ICON_MESSAGE_INBOX,   TXT_SMS,                    {0}, 0, 0, FALSE, FALSE},
    {MMIIKB_TYPE_SCHEDULE,  IMAGE_SECMENU_ICON_TOOL_CALENDAR,   TXT_IDLE_CALENDAR,          {0}, 0, 0, FALSE, FALSE},
/*     
#ifdef BROWSER_SUPPORT
    {MMIIKB_TYPE_BRO_LABEL, IMAGE_SECMENU_ICON_BROWSER,         TXT_COMMON_BOOKMARKS,{0}, 0, 0, FALSE, FALSE},
#endif
*/
};
LOCAL MMIIKB_UI_SEL_ITEM_LIST_T s_ui_restore_list[] = 
{
    {MMIIKB_TYPE_CALLLOG,   IMAGE_SECMENU_ICON_CALL_LIST,       STXT_MAIN_CL,       {0}, 0, 0, FALSE, FALSE},
    {MMIIKB_TYPE_PHONEBOOK, IMAGE_SECMENU_ICON_TOOL_CL,         STXT_MAIN_PB,       {0}, 0, 0, FALSE, FALSE},
    {MMIIKB_TYPE_SMS,       IMAGE_SECMENU_ICON_MESSAGE_INBOX,   TXT_SMS,                    {0}, 0, 0, FALSE, FALSE},
    {MMIIKB_TYPE_SCHEDULE,  IMAGE_SECMENU_ICON_TOOL_CALENDAR,   TXT_IDLE_CALENDAR,          {0}, 0, 0, FALSE, FALSE},
/*
#ifdef BROWSER_SUPPORT
    {MMIIKB_TYPE_BRO_LABEL, IMAGE_SECMENU_ICON_BROWSER,         TXT_COMMON_BOOKMARKS,{0}, 0, 0, FALSE, FALSE},
#endif
*/
};

//版本信息，奇数是小端，偶数是大端版本
#ifdef CHIP_ENDIAN_LITTLE
LOCAL char *s_str_ikb_version = "0001.00003";
#else
LOCAL char *s_str_ikb_version = "0001.00004";
#endif
//{'0','0','0','1','.','0','0','0','0', '1',0};
    
/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : IsCreateInstance
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 创建实例之前的检查
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IsCreateInstance(void);

/*****************************************************************************/
//  Description : AddTabsAndChildWindows
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AddIKBTabsAndChildWindows(void);

/*****************************************************************************/
//  Description : handle 1-key backup main window message
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIKeyBackupMainWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : ShowTips
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ShowTips(
    MMIIKB_TASK_TYPE_E task_type, 
    MMIIKB_ERROR_E error_code
);

//ui list common
/*****************************************************************************/
//  Description : InitListUIValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_TYPE_T InitListUIValue(
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
);

/*****************************************************************************/
//  Description : GetListItemSelectValues
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_TYPE_T GetListItemSelectValues(
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
);

/*****************************************************************************/
//  Description : GetOneListItemValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void GetOneListItemValue(
    const MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr, 
    GUILIST_ITEM_T *item_ptr,
    GUILIST_ITEM_DATA_T *item_data_ptr
);

/*****************************************************************************/
//  Description : AppendALLListItemCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AppendALLListItemCtrl(
    MMI_CTRL_ID_T ctrl_id, 
    const MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
);

/*****************************************************************************/
//  Description : OnClickCheckListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickCheckListItem(
    MMI_CTRL_ID_T ctrl_id,
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_backup_list_ptr,
    uint16 list_num
);

/*****************************************************************************/
//  Description : FormatTotalValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void FormatTotalValue(
    MMI_CTRL_ID_T ctrl_id,
    MMIIKB_UI_SEL_ITEM_LIST_T *item_list_ptr
);

/*****************************************************************************/
//  Description : GetFirstTypeTotalValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFirstTypeTotalValue(
    MMIIKB_TYPE_T types,
    const MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_ptr,
    uint16 list_num
);

/*****************************************************************************/
//  Description : UpdateProcessBar
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdateProcessBar(
    MMIIKB_TASK_SEQ_TABLE_T *seq_task_item_ptr
);

/*****************************************************************************/
//  Description : CancelCurAction
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CancelCurAction(MMIIKB_TASK_TYPE_E task_type);

/*****************************************************************************/
//  Description : handle backup progressbar event
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelActionQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : EndCancelAction
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void EndCancelAction(MMIIKB_SIG_CANCEL_RES_T *sig_cancel_res_ptr);

/*****************************************************************************/
//  Description : DeletePhysicalDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeletePhysicalDir(const wchar* pack_dir, uint16 path_len);


//backup list
/*****************************************************************************/
//  Description : set input password window control attribute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetEnterPswWinAttri(void);

/*****************************************************************************/
//  Description : OnClickBackupPswSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickBackupPswSoftkey(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id1,
    MMI_CTRL_ID_T ctrl_id2
);

/*****************************************************************************/
//  Description : handle add backup password window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterBackupPwd(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : handle backup progressbar event
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIKB_HandleBackupProWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : OptimizeSelTypes
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 优化选择的类别
/*****************************************************************************/
LOCAL MMIIKB_TYPE_T OptimizeBackupSelTypes(MMIIKB_TYPE_T types);

/*****************************************************************************/
//  Description : PreCheckBackupItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL BOOLEAN PreCheckBackupItems(
    MMIIKB_TYPE_T types,                //in
    MMIIKB_TYPE_T *opt_types_ptr        //out，过滤后的备份类型
);

/*****************************************************************************/
//  Description : IsNeedInputPsd
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 检查是否需要输入密码
/*****************************************************************************/
LOCAL BOOLEAN IsNeedInputPsd(
    MMIIKB_TYPE_T types
);

/*****************************************************************************/
//  Description : BackupSelItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL void BackupSelItems(MMIIKB_PRE_BACKUP_DATA_T *pre_backup_data_ptr);

/*****************************************************************************/
//  Description : StartBackupItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E StartBackupItems(
    MMIIKB_PRE_BACKUP_DATA_T *pre_backup_data_ptr
);

/*****************************************************************************/
//  Description : EndBackupItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 备份完成后的操作
/*****************************************************************************/
LOCAL void EndBackupItems(MMIIKB_SIG_BACKUP_RES_T *sig_backup_res_ptr);

/*****************************************************************************/
//  Description : InitBackupListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitBackupListCtrl(void);

/*****************************************************************************/
//  Description : OnClickBackupSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickBackupSoftkey(void);

/*****************************************************************************/
//  Description : HandleBackupChildWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBackupChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);


//restore list
/*****************************************************************************/
//  Description : set input password window control attribute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetEnterResPswWinAttri(void);

/*****************************************************************************/
//  Description : OnClickRestorePswSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickRestorePswSoftkey(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id1,
    MMIIKB_CHECK_PSW_WIN_DATA_T *psw_win_data_ptr
);

/*****************************************************************************/
//  Description : handle add restore password window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterRestorePwd(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : set restore result detail window control attribute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 动态创建各个栏目的提示信息，以后扩展就不需要修改了
/*****************************************************************************/
LOCAL void SetResResultDetailWinAttri(MMIIKB_TYPE_T types);

/*****************************************************************************/
//  Description : handle restore result detail window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResResultDetailMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : handle restore progressbar event
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIKB_HandleRestoreProWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : PreCheckRestoreItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL BOOLEAN PreCheckRestoreItems(uint16 item_index, MMIIKB_TYPE_T types);

/*****************************************************************************/
//  Description : RestoreSelItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL void RestoreSelItems(uint16 item_index, MMIIKB_TYPE_T types);

/*****************************************************************************/
//  Description : GetPackPswInfo
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E GetPackPswInfo(
    uint16 item_index,
    MMIIKB_PASSWORD_INFO_T *psw_info_ptr
);

/*****************************************************************************/
//  Description : StartRestoreItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E StartRestoreItems(uint16 item_index, MMIIKB_TYPE_T types);

/*****************************************************************************/
//  Description : EndRestoreItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void EndRestoreItems(MMIIKB_SIG_RESTORE_RES_T *sig_restore_res_ptr);

/*****************************************************************************/
//  Description : CreatePackListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreatePackListCtrl(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : ClearPackList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void ClearPackList(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : OpenPackListWin
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenPackListWin(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : StartLoadPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPack(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : FinishLoadPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPack(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : HandleLoadPackWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPackWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : UpdatePackList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdatePackList(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id,
    uint16 cur_index
);

/*****************************************************************************/
//  Description : AppendPackListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPackListItem(
    BOOLEAN is_text,
    uint16 item_index,
    uint16 content_index,
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : OnClickRestoreList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickRestoreList(
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : DeleteSelectedPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteSelectedPack(uint32 item_index);

/*****************************************************************************/
//  Description : DeleteAllPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteAllPack(void);

/*****************************************************************************/
//  Description : GetPackContentNumber
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPackContentNumber(
    const uint16 *pack_path_ptr,
    uint16 pack_path_len,
    MMIIKB_TYPE_E type
);

/*****************************************************************************/
//  Description : InitRestoreSelItemList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitRestoreSelItemList(
    uint16 index,
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
);

/*****************************************************************************/
//  Description : OnClickRestoreSelSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickRestoreSelSoftkey(uint16 item_index);

/*****************************************************************************/
//  Description : HandleRestoreItemSelWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreItemSelWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : handle delete selected package confirm message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void HandleDeletePackCnf(
    BOOLEAN is_success,             //in:delete is success
    uint16 pack_index,              //in:
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T list_ctrl_id      //in:
);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : HandleRestoreListLongOkOptMenuWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理长按还原列表，弹出菜单选项窗口
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreListLongOkOptMenuWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
#endif

/*****************************************************************************/
//  Description : handle package delete msg
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePackDelQueryWin(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : HandleRestoreListOptMenuWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理option列表，弹出菜单选项窗口
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreListOptMenuWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : HandleRestoreChildWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);


//setting list
/*****************************************************************************/
//  Description : InitSettingListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitSettingListCtrl(
    MMI_CTRL_ID_T ctrl_id,
    MMIIKB_CONFIG_T * cfg_ptr
);

/*****************************************************************************/
//  Description : InitBackupPositionListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitBackupPositionListCtrl(void);

/*****************************************************************************/
//  Description : SetBackupPosData
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetBackupPosData(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : HandleBackupPosWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBackupPosWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : InitAppItemDataListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitAppItemDataListCtrl(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : SetEncryptData
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetEncryptData(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : HandleEncryptDataWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEncryptDataWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : HandleSettingChildWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);



//version control
/*****************************************************************************/
//  Description : IsVersionLegal
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsVersionLegal(MMIIKB_PACK_INFO_T * pack_info_ptr);

/*****************************************************************************/
//  Description : AbortCurAction
//  Global resource dependence :
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void AbortCurAction(MMIIKB_TASK_TYPE_E task_type);

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIIKB_MAIN_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleIKeyBackupMainWinMsg),
    WIN_ID(MMIIKB_MAIN_WIN_ID),
#ifdef MMI_GUI_STYLE_TYPICAL
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMIIKB_TAB_CTRL_ID),
#else
    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMIIKB_TAB_CTRL_ID),
#endif

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIIKB_ENTER_PSW_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEnterBackupPwd),    
    WIN_ID(MMIIKB_ENTER_PSW_WIN_ID),
    WIN_TITLE(TXT_IKB_PLS_SET_PSW),
#ifndef MMI_PDA_SUPPORT
     WIN_SOFTKEY(TXT_IKB_BACKUP, TXT_NULL, STXT_RETURN),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIIKB_ENTER_PSW_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, 
            MMIIKB_PSW_LABEL_CTRL_ID, MMIIKB_ENTER_PSW_FORM_CTRL_ID),
        CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIIKB_BACKUP_PWD_MAX_LEN, 
            MMIIKB_SET_BACKUP_PSW_CTRL_ID, MMIIKB_ENTER_PSW_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, 
            MMIIKB_PSW_VERFY_LABEL_CTRL_ID, MMIIKB_ENTER_PSW_FORM_CTRL_ID),
        CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIIKB_BACKUP_PWD_MAX_LEN, 
            MMIIKB_SET_BACKUP_VERFY_PSW_CTRL_ID, MMIIKB_ENTER_PSW_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_SOFTKEY_CTRL(TXT_IKB_BACKUP, TXT_NULL, STXT_RETURN,
            MMICOMMON_SOFTKEY_CTRL_ID, MMIIKB_ENTER_PSW_FORM_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE(MMIIKB_BACKUP_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBackupChildWinMsg),
    WIN_ID(MMIIKB_BACKUP_CHILD_WIN_ID),
    
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIIKB_BACKUP_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIIKB_BACKUP_LIST_CTRL_ID, 
        MMIIKB_BACKUP_LIST_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_NEXT_STEP, TXT_NULL,
        MMICOMMON_SOFTKEY_CTRL_ID, MMIIKB_BACKUP_LIST_FORM_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIIKB_BACKUP_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_IKB_BACKUP, TXT_NULL, STXT_RETURN),
#endif
    
    END_WIN
};

WINDOW_TABLE(MMIIKB_RESTORE_ITEM_SEL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleRestoreItemSelWinMsg),
    WIN_ID(MMIIKB_RESTORE_ITEM_SEL_WIN_ID),
    WIN_TITLE(TXT_IKB_SELECT_RESTORE_ITEM),

#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIIKB_RESTORE_ITEM_SEL_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIIKB_RESTORE_ITEM_SEL_CTRL_ID, 
        MMIIKB_RESTORE_ITEM_SEL_FORM_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_IKB_LAST_STEP, TXT_NULL, TXT_NEXT_STEP,
        MMICOMMON_SOFTKEY_CTRL_ID, MMIIKB_RESTORE_ITEM_SEL_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_IKB_RESTORE, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIIKB_RESTORE_ITEM_SEL_CTRL_ID),
#endif

    END_WIN
};

WINDOW_TABLE(MMIIKB_ENTER_RES_PSW_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEnterRestorePwd),    
    WIN_ID(MMIIKB_ENTER_RES_PSW_WIN_ID),
    WIN_TITLE(TXT_PLS_INPUT_PWD),
#ifndef MMI_PDA_SUPPORT
     WIN_SOFTKEY(TXT_IKB_RESTORE, TXT_NULL, STXT_RETURN),
#endif

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIIKB_ENTER_RES_PSW_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, 
            MMIIKB_RES_PSW_LABEL_CTRL_ID, MMIIKB_ENTER_RES_PSW_FORM_CTRL_ID),
        CHILD_EDIT_PASSWORD_CTRL(TRUE, MMIIKB_BACKUP_PWD_MAX_LEN, 
            MMIIKB_SET_RESTORE_PSW_CTRL_ID, MMIIKB_ENTER_RES_PSW_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_SOFTKEY_CTRL(TXT_IKB_RESTORE, TXT_NULL, STXT_RETURN,
            MMICOMMON_SOFTKEY_CTRL_ID, MMIIKB_ENTER_RES_PSW_FORM_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE(MMIIKB_RES_RESULT_DETAIL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleResResultDetailMsg),    
    WIN_ID(MMIIKB_RES_RESULT_DETAIL_WIN_ID),
    WIN_TITLE(TXT_IKB_RESTORE_DETAIL),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIIKB_RES_DETAIL_FORM_CTRL_ID),
    
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
//备份包列表长按选项窗口
WINDOW_TABLE(MMIIKB_RESTORE_LIST_LONGOK_POPMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleRestoreListLongOkOptMenuWinMsg),
    WIN_ID(MMIIKB_RESTORE_LIST_LONGOK_POPMENU_WIN_ID),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MMIIKB_RESTORE_LIST_LONGOK_OPT_MENU, 
        MMIIKB_RESTORE_LIST_LONGOK_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

//备份包列表左软键选项窗口
WINDOW_TABLE(MMIIKB_RESTORE_LIST_OPT_POPMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleRestoreListOptMenuWinMsg),
    WIN_ID(MMIIKB_RESTORE_LIST_OPT_POPMENU_WIN_ID),
    //WIN_TITLE(STXT_OPTION),
    WIN_STYLE(WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MMIIKB_RESTORE_LIST_OPT_MENU, 
        MMIIKB_RESTORE_LIST_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIIKB_RESTORE_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleRestoreChildWinMsg),
    WIN_ID(MMIIKB_RESTORE_CHILD_WIN_ID),

#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIIKB_RESTORE_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIIKB_RESTORE_LIST_CTRL_ID, 
        MMIIKB_RESTORE_LIST_FORM_CTRL_ID),
    //CHILD_SOFTKEY_CTRL(TXT_NULL, TXT_IKB_START_RESTORE, TXT_NULL,
    //    MMICOMMON_SOFTKEY_CTRL_ID, MMIIKB_RESTORE_LIST_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIIKB_RESTORE_LIST_CTRL_ID),
#endif

    END_WIN
};

WINDOW_TABLE(MMIIKB_SETTING_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleSettingChildWinMsg),
    WIN_ID(MMIIKB_SETTING_CHILD_WIN_ID),

#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIIKB_SETTING_LIST_FORM_CTRL_ID),
    CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, MMIIKB_SETTING_LIST_CTRL_ID, 
        MMIIKB_SETTING_LIST_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIIKB_SETTING_LIST_CTRL_ID),
#endif

    END_WIN
};

WINDOW_TABLE(MMIIKB_BACKUP_POS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBackupPosWinMsg),
    WIN_ID(MMIIKB_BACKUP_POS_WIN_ID),
    WIN_TITLE(TXT_IKB_CUR_BACKUP_POSITION),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_FINISH),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIIKB_BACKUP_POS_LIST_CTRL_ID),
    
    END_WIN
};

WINDOW_TABLE(MMIIKB_ENCRYPT_DATA_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEncryptDataWinMsg),
    WIN_ID(MMIIKB_ENCRYPT_DATA_WIN_ID),

    WIN_TITLE(TXT_IKB_ENCRYPT_DATA),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIIKB_ENCRYPT_DATA_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, TXT_COMPLETE),
    END_WIN
};



/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DEFINITION                     *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : 创建主窗口
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 本函数算是一键备份还原的UI层实例创建
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIIKB_OpenMainWin(void)
{
    MMI_HANDLE_T win_handle = 0;
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;

    //入口条件检查，成功则创建实例，否则退出
    error_code = IsCreateInstance();
    if (MMIIKB_STATUS_SUCCESS != error_code)
    {
        ShowTips(MMIIKB_TASK_TYPE_BACKUP, error_code);
        return win_handle;
    }

    MMK_CloseParentWin(MMIIKB_MAIN_WIN_ID);
   
    MMK_CreateParentWin((uint32 *)MMIIKB_MAIN_WIN_TAB, PNULL);
    
    MMK_CreateChildWin(MMIIKB_MAIN_WIN_ID, 
        (uint32*)MMIIKB_BACKUP_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMIIKB_MAIN_WIN_ID, 
        (uint32*)MMIIKB_RESTORE_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(MMIIKB_MAIN_WIN_ID, 
        (uint32*)MMIIKB_SETTING_CHILD_WIN_TAB, NULL);
    
    switch (s_cur_ui_tab_type)
    {
    case MMIIKB_UI_TAB_BACKUP:
        MMK_SetChildWinFocus(MMIIKB_MAIN_WIN_ID, MMIIKB_BACKUP_CHILD_WIN_ID);
        break;
        
    case MMIIKB_UI_TAB_RESTORE:
        MMK_SetChildWinFocus(MMIIKB_MAIN_WIN_ID, MMIIKB_RESTORE_CHILD_WIN_ID);
        break;
        
    case MMIIKB_UI_TAB_SETTING:
        MMK_SetChildWinFocus(MMIIKB_MAIN_WIN_ID, MMIIKB_SETTING_CHILD_WIN_ID);
        break;
        
    default:
        MMK_SetChildWinFocus(MMIIKB_MAIN_WIN_ID, MMIIKB_BACKUP_CHILD_WIN_ID);
        break;
    }

    MMIAPIIKB_PrintRegTable();

    return win_handle;
}

/*****************************************************************************/
//  Description : 销毁主窗口，退出实例
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 本函数算是一键备份还原的UI层实例退出
/*****************************************************************************/
PUBLIC void MMIIKB_UITerminate(void)
{
    s_cur_ui_tab_type = MMIIKB_UI_TAB_BACKUP;
}

/*****************************************************************************/
//  Description : MMIAPIIKB_HandleAppSigRes
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理1-key backup反馈消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIKB_HandleAppSigRes(MMI_MESSAGE_ID_E msg_id, void *param)
{
    void *original_param = PNULL;
    
    if (PNULL == param)
    {
        //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_HandleAppSigRes: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_995_112_2_18_2_24_36_147,(uint8*)"");
        return FALSE;
    }    
    original_param = (void*)((uint32)param);
    
    //SCI_TRACE_LOW:"[MMIIKB] MMIAPIIKB_HandleAppSigRes msg_id=%d, param=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1001_112_2_18_2_24_36_148,(uint8*)"dd", msg_id, original_param);
    
    switch (msg_id)
    {
        case MSG_IKB_BACKUP_DATA_RES:
            {
                MMIIKB_SIG_BACKUP_RES_T *sig_backup_res_ptr = PNULL;
                sig_backup_res_ptr = (MMIIKB_SIG_BACKUP_RES_T *)original_param;
                
                EndBackupItems(sig_backup_res_ptr);
            }
            break;

        case MSG_IKB_RESTORE_DATA_RES:
            {
                MMIIKB_SIG_RESTORE_RES_T *sig_restore_res_ptr = PNULL;
                sig_restore_res_ptr = (MMIIKB_SIG_RESTORE_RES_T *)original_param;
                
                EndRestoreItems(sig_restore_res_ptr);
            }
            break;

        case MSG_IKB_PROGRESS_VALUE_RES:
            {
                MMIIKB_SIG_PROGRESS_RES_T *sig_progress_res_ptr = PNULL;
                
                sig_progress_res_ptr = 
                    (MMIIKB_SIG_PROGRESS_RES_T *)original_param;
                    UpdateProcessBar(&sig_progress_res_ptr->seq_task_item);
            }
            break;

        case MSG_IKB_CANCEL_ACTION_RES:
            {
                MMIIKB_SIG_CANCEL_RES_T *sig_cancel_res_ptr = PNULL;

                sig_cancel_res_ptr = 
                    (MMIIKB_SIG_CANCEL_RES_T *)original_param;
                    EndCancelAction(sig_cancel_res_ptr);
            }
            break;

        default:
            break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : MMIIKB_AppendRadioListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIKB_AppendRadioListItem(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T text_1,
    uint32 user_data
)
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : MMIIKB_AppendCheckBoxListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIIKB_AppendCheckBoxListItem(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T  text_1,
    uint32 user_data
)
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_CHECK;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : MMIIKB_SetListMarkable
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIKB_SetListMarkable(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_markable
)
{
    BOOLEAN result = FALSE;
    
    result = GUILIST_SetTextListMarkable(ctrl_id, is_markable);

    return (result);
}


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINITION                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : IsCreateInstance
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 创建实例之前的检查
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E IsCreateInstance(void)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_SUCCESS;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_SDCARD;

    //1: 先检查是否存在sd卡，若无则退出
    device_type = MMIAPIFMM_GetFirstValidSD();
    if(device_type >= MMI_DEVICE_NUM)
    {
        error_code = MMIIKB_STATUS_SD_NOT_EXIST;
    }

    //2: 检查U盘是否使用中u
    if (MMIAPIUDISK_UdiskIsRun())
    {
        error_code = MMIIKB_STATUS_UDISK_USING;
    }
    
    //2: 空间是否需要预留50K
    /*
    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        
    }
    */

    //3: 找到SD卡，是否需要做一些配置，后续思考

    return error_code;
}

/*****************************************************************************/
//  Description : AddTabsAndChildWindows
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AddIKBTabsAndChildWindows(void)
{
    MMI_STRING_T str_text = {0};
    MMI_IMAGE_ID_T ikb_pda_tab[3][2] = {
        {IMAGE_IKB_TAB_BACKUP_SELECT, IMAGE_IKB_TAB_BACKUP_RELEASED},
        {IMAGE_IKB_TAB_RESTORE_SELECT, IMAGE_IKB_TAB_RESTORE_RELEASED},
        {IMAGE_IKB_TAB_SETTING_SELECT, IMAGE_IKB_TAB_SETTING_RELEASED}
        };
    
    GUITAB_SetMaxItemNum(MMIIKB_TAB_CTRL_ID, 3);
#ifdef MMI_GUI_STYLE_MINISCREEN
    GUITAB_SetState(MMIIKB_TAB_CTRL_ID, GUITAB_STATE_SINGLE_LINE, FALSE);
#endif
    MMI_GetLabelTextByLang(TXT_IKB_BACKUP, &str_text);
    GUITAB_AppendSimpleItem(
        MMIIKB_TAB_CTRL_ID,
        &str_text,
        ikb_pda_tab[0][0],
        ikb_pda_tab[0][1]);
    
    GUITAB_AddChildWindow(
        MMIIKB_TAB_CTRL_ID,
        MMIIKB_BACKUP_CHILD_WIN_ID,
        0
        );

   MMI_GetLabelTextByLang(TXT_IKB_RESTORE, &str_text);
    GUITAB_AppendSimpleItem(
       MMIIKB_TAB_CTRL_ID,
       &str_text,
        ikb_pda_tab[1][0],
        ikb_pda_tab[1][1]);
   
   GUITAB_AddChildWindow(
       MMIIKB_TAB_CTRL_ID,
       MMIIKB_RESTORE_CHILD_WIN_ID,
       1
       );
   
   MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &str_text);
   GUITAB_AppendSimpleItem(
       MMIIKB_TAB_CTRL_ID,
       &str_text,
        ikb_pda_tab[2][0],
        ikb_pda_tab[2][1]);
   
   GUITAB_AddChildWindow(
       MMIIKB_TAB_CTRL_ID,
       MMIIKB_SETTING_CHILD_WIN_ID,
       2
       );

   switch (s_cur_ui_tab_type)
   {
    case MMIIKB_UI_TAB_BACKUP:
        GUITAB_SetCurSel(MMIIKB_TAB_CTRL_ID, 0);
        break;
        
    case MMIIKB_UI_TAB_RESTORE:
        GUITAB_SetCurSel(MMIIKB_TAB_CTRL_ID, 1);
        break;
        
    case MMIIKB_UI_TAB_SETTING:
        GUITAB_SetCurSel(MMIIKB_TAB_CTRL_ID, 2);
        break;
        
    default:
        GUITAB_SetCurSel(MMIIKB_TAB_CTRL_ID, 0);
        break;
   }
}

/*****************************************************************************/
//  Description : handle 1-key backup main window message
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIKeyBackupMainWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIIKB_TAB_CTRL_ID);
        AddIKBTabsAndChildWindows();
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:        
        MMIIKB_UITerminate();
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : ShowTips
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ShowTips(
    MMIIKB_TASK_TYPE_E task_type, 
    MMIIKB_ERROR_E error_code
)
{
    BOOLEAN is_ret = FALSE;
    MMI_IMAGE_ID_T image_id = 0;
    MMI_TEXT_ID_T text_id1 = TXT_NULL;
    MMI_TEXT_ID_T text_id2 = TXT_NULL;
    
    //SCI_TRACE_LOW:"[MMIIKB] ShowTips: error_code=%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1279_112_2_18_2_24_36_149,(uint8*)"d", error_code);

    if (MMIIKB_TASK_TYPE_BACKUP == task_type)
    {
        text_id1 = TXT_IKB_TIPS_BACKUP;
    }
    else if (MMIIKB_TASK_TYPE_RESTORE == task_type)
    {
        text_id1 = TXT_IKB_TIPS_RESTORE;
    }
    
    image_id = IMAGE_PUBWIN_FAIL;    
    switch (error_code)
    {
    case MMIIKB_STATUS_SUCCESS:
        image_id = IMAGE_PUBWIN_SUCCESS;
        text_id2 = TXT_SUCCESS;
        break;
    case MMIIKB_STATUS_FAILED:
        text_id2 = TXT_ERROR;
        break;
    case MMIIKB_STATUS_SYSTEM_ERROR:
        text_id2 = TXT_ERROR;
        break;
    case MMIIKB_STATUS_NO_MEMORY:
        text_id2 = TXT_MEMORY_FULL;
        break;
    case MMIIKB_STATUS_INVALID_PARAM:
        text_id2 = TXT_IKB_TIPS_INVALID_PARAM;
        break;
    case MMIIKB_STATUS_DISK_NO_SPACE:
        text_id2 = TXT_NO_SPACE;
        break;
    case MMIIKB_STATUS_NO_INIT_TASK:
        text_id2 = TXT_IKB_TIPS_NO_INIT_TASK;
        break;
    case MMIIKB_STATUS_WRONG_TASK:
        text_id2 = TXT_IKB_TIPS_WRONG_TASK;
        break;
    case MMIIKB_STATUS_WRONG_OP_TYPE:
        text_id2 = TXT_IKB_TIPS_WRONG_OP_TYPE;
        break;
    case MMIIKB_STATUS_CRC_ERROR:
        text_id2 = TXT_IKB_TIPS_WRONG_CRC;
        break;
    case MMIIKB_STATUS_FILE_ERROR:
        text_id2 = TXT_IKB_TIPS_FILE_OPERATE_ERROR;
        break;
    case MMIIKB_STATUS_SD_OPERATE_FAIL:
        text_id2 = TXT_IKB_TIPS_SD_OPERATE_FAIL;
        break;
    case MMIIKB_STATUS_SD_NOT_READY:
        text_id2 = TXT_COMMON_SD_FAIL;
        break;
    case MMIIKB_STATUS_SD_NOT_EXIST:
        text_id2 = TXT_PLEASE_INSERT_SD;
        break;
    case MMIIKB_STATUS_READ_PACK_ERROR:
        text_id2 = TXT_IKB_TIPS_READ_PACK_ERROR;
        break;
    case MMIIKB_STATUS_CANCEL_ACTION:
        text_id2 = TXT_USER_CANCEL;
        break;
    case MMIIKB_STATUS_BACKUP_TIME_OUT:
    case MMIIKB_STATUS_RESTORE_TIME_OUT:
        text_id2 = TXT_IKB_TIPS_EXIT_BY_TIMEOUT;
        break;
    case MMIIKB_STATUS_PB_FULL:
        text_id2 = TXT_PB_NV_FULL;
        break;
    case MMIIKB_STATUS_ILLEGAL_VERSION:
        text_id2 = TXT_IKB_TIPS_ILLEGAL_VER;
        break;
    case MMIIKB_STATUS_UDISK_USING:
        image_id = IMAGE_PUBWIN_WARNING;
        text_id2 = TXT_COMMON_UDISK_USING;
        break;
        
    default://MMIIKB_STATUS_UNKNOWN_ERROR
        text_id2 = TXT_UNKNOWN_ERROR;
        break;
    }

    if (error_code == MMIIKB_STATUS_SD_NOT_READY
        || error_code == MMIIKB_STATUS_FILE_ERROR
        || error_code == MMIIKB_STATUS_SD_OPERATE_FAIL)
    {
        MMIIKB_CONFIG_T *cfg_ptr = MMIAPIIKB_GetConfigInfo();
        MMIFILE_DEVICE_E device_id = 0;
        uint32 free_high_word = 0;
        uint32 free_low_word  = 0;
        BOOLEAN is_space_enough = FALSE;
        
        if (MMIIKB_STORE_POS_SD_1 == cfg_ptr->store_pos)
        {
            device_id = MMI_DEVICE_SDCARD;
        }
        else if (MMIIKB_STORE_POS_SD_2 == cfg_ptr->store_pos)
        {
            device_id = MMI_DEVICE_SDCARD_1;
        }
        
        if(!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_id), MMIAPIFMM_GetDevicePathLen(device_id)))
        {
            error_code = MMIIKB_STATUS_SD_NOT_EXIST;
        }
        else
        {
            if (MMIAPIFMM_GetDeviceFreeSpace(
                    MMIAPIFMM_GetDevicePath(device_id), 
                    MMIAPIFMM_GetDevicePathLen(device_id),
                    &free_high_word, 
                    &free_low_word))
            {
                if (!(0 == free_high_word && MMIIKB_SPACE_LIMIT_VALUE > free_low_word))
                {
                    is_space_enough = TRUE;
                }
            }
            
            if (!is_space_enough)
            {
                text_id2 = TXT_NO_SPACE;
            }
        }   
    }

    if (MMIIKB_STATUS_SD_NOT_EXIST == error_code)
    {
        MMIPUB_OpenAlertWarningWin(TXT_PLEASE_INSERT_SD);
    }
    else
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, text_id1, text_id2, image_id, 
            PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }
    
    is_ret = TRUE;

    return is_ret;
}


//ui list common
/*****************************************************************************/
//  Description : InitListUIValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_TYPE_T InitListUIValue(
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
)
{
    uint16 i = 0;
    MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    MMIIKB_TYPE_T types = 0;

    if (PNULL == ui_list_head_ptr
        || 0 == list_num)
    {
        //SCI_TRACE_LOW:"[MMIIKB] InitListUIValue: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1418_112_2_18_2_24_36_150,(uint8*)"");
        return types;
    }

    //第一行全选不取，从第二行开始
    for (i = 0; i < list_num; i++)
    {
        cur_list_ptr = ui_list_head_ptr + i;

        cur_list_ptr->total = 0;
        cur_list_ptr->is_check = FALSE;
        cur_list_ptr->is_gray = FALSE;
        SCI_MEMSET(cur_list_ptr->text2_buf, 0x0, 
            sizeof(cur_list_ptr->text2_buf));
        cur_list_ptr->text2_len = 0;
    }

    return types;
}

/*****************************************************************************/
//  Description : GetListItemSelectValues
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_TYPE_T GetListItemSelectValues(
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
)
{
    uint16 i = 0;
    uint32 user_data = 0;
    const MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    MMIIKB_TYPE_T types = 0;

    if (PNULL == ui_list_head_ptr
        || 0 == list_num)
    {
        //SCI_TRACE_LOW:"[MMIIKB] GetListItemSelectValues: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1455_112_2_18_2_24_37_151,(uint8*)"");
        return types;
    }

    //第一行全选不取，从第二行开始
    for (i = 0; i < list_num; i++)
    {
        cur_list_ptr = ui_list_head_ptr + i;
        user_data = cur_list_ptr->user_data;

        if (cur_list_ptr->is_check
            && !cur_list_ptr->is_gray)
        {
            types |= (0x01<<user_data);
        }
    }

    return types;
}

/*****************************************************************************/
//  Description : GetOneListItemValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void GetOneListItemValue(
    const MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr, 
    GUILIST_ITEM_T *item_ptr,
    GUILIST_ITEM_DATA_T *item_data_ptr
)
{
    MMI_IMAGE_ID_T image_id2 = 0;
    
    if (PNULL == cur_list_ptr
        || PNULL == item_ptr
        || PNULL == item_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] GetOneListItemValue: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1491_112_2_18_2_24_37_152,(uint8*)"");
        return;
    }

    if (cur_list_ptr->is_check)
    {
        item_ptr->item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM; 
    }
    else
    {
        item_ptr->item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON;       
    }
    item_ptr->item_data_ptr = item_data_ptr;
    item_ptr->user_data = cur_list_ptr->user_data;
    
    item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data_ptr->item_content[0].item_data.image_id = cur_list_ptr->image_id1;
    
    item_data_ptr->item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data_ptr->item_content[1].item_data.text_id = cur_list_ptr->text_id1;

    image_id2 = cur_list_ptr->is_check ? IMAGE_CHECK_SELECTED_ICON 
        : IMAGE_CHECK_UNSELECTED_ICON;
    item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data_ptr->item_content[2].item_data.image_id = image_id2;

    if (cur_list_ptr->is_check)
    {
        if(0 != cur_list_ptr->text2_len)
        {
            item_data_ptr->item_content[3].item_data_type 
                = GUIITEM_DATA_TEXT_BUFFER;
            item_data_ptr->item_content[3].item_data.text_buffer.wstr_ptr 
                = (wchar*)cur_list_ptr->text2_buf;
            item_data_ptr->item_content[3].item_data.text_buffer.wstr_len 
                = cur_list_ptr->text2_len;
        }
    }
    
    //item_data_ptr->softkey_id[0] = STXT_SELECT;
    //item_data_ptr->softkey_id[1] = TXT_NULL;
    //item_data_ptr->softkey_id[2] = STXT_RETURN;
}

/*****************************************************************************/
//  Description : AppendALLListItemCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void AppendALLListItemCtrl(
    MMI_CTRL_ID_T ctrl_id, 
    const MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
)
{
    uint16 i = 0;
    const MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;

    if (PNULL == ui_list_head_ptr
        || 0 == list_num)
    {
        //SCI_TRACE_LOW:"[MMIIKB] AppendALLListItemCtrl: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1551_112_2_18_2_24_37_153,(uint8*)"");
        return;
    }

    for (i = 0; i < list_num; i++)
    {
        GUILIST_ITEM_T item_t = {0};        /*lint !e64*/
        GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
        cur_list_ptr = ui_list_head_ptr + i;

        GetOneListItemValue(cur_list_ptr, &item_t, &item_data);
        
        GUILIST_AppendItem(ctrl_id, &item_t);

        if (cur_list_ptr->is_gray)
        {
            GUILIST_SetItemGray(ctrl_id, i, TRUE);
        }
    }
}

/*****************************************************************************/
//  Description : OnClickCheckListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickCheckListItem(
    MMI_CTRL_ID_T ctrl_id,
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_backup_list_ptr,
    uint16 list_num
)
{
    uint16 index = 0;
    uint32 user_data = 0;
    MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    BOOLEAN is_cur_line_state = FALSE;
    GUILIST_ITEM_T item_t = {0};        /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/

    if (PNULL == ui_backup_list_ptr
        || 0 == list_num)
    {
        //SCI_TRACE_LOW:"[MMIIKB] OnClickCheckListItem: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1592_112_2_18_2_24_37_154,(uint8*)"");
        return;
    }
    
    index = GUILIST_GetCurItemIndex(ctrl_id);
    GUILIST_GetItemData(ctrl_id, index, &user_data);
    
    SCI_MEMSET(&item_t, 0x0, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x0, sizeof(item_data));
    
    cur_list_ptr = ui_backup_list_ptr + user_data;

    if (!cur_list_ptr->is_gray)
    {
        is_cur_line_state = cur_list_ptr->is_check;
        
        cur_list_ptr->is_check = !is_cur_line_state;
        FormatTotalValue(ctrl_id, cur_list_ptr);

        GetOneListItemValue(cur_list_ptr, &item_t, &item_data);

        GUILIST_ReplaceItem(ctrl_id, &item_t, index);
    }
    
    return;
}

/*****************************************************************************/
//  Description : FormatTotalValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void FormatTotalValue(
    MMI_CTRL_ID_T ctrl_id,
    MMIIKB_UI_SEL_ITEM_LIST_T *item_list_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    uint32 total_size = 0;
    char c_text[MMIIKB_UI_TOTAL_APP_BUF_LEN] = {0};
    uint16 text_len = 0;
    MMI_STRING_T text_str = {0};
    MMIIKB_GET_TOTAL_OUT_T param_out = {0};
    
    if (PNULL == item_list_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] FormatTotalValue: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1638_112_2_18_2_24_37_155,(uint8*)"");
        return;
    }

    type = (MMIIKB_TYPE_E)(item_list_ptr->user_data);

    if (item_list_ptr->is_check)
    {
        //backup list
        if (MMIIKB_BACKUP_LIST_CTRL_ID == ctrl_id)
        {
            error_code = IKeyBackup_GetTotalSizeByType(type, &param_out);
            if (MMIIKB_STATUS_SUCCESS == error_code)
            {
                total_size = param_out.total;
            }
            else
            {
                total_size = 0;
            }
            item_list_ptr->total = total_size;
            SCI_MEMSET(item_list_ptr->text2_buf, 0x0, 
                sizeof(item_list_ptr->text2_buf));            

            if (MMIIKB_TOTAL_TYPE_NUMBER == param_out.total_type)
            {
                SCI_MEMSET(c_text, 0x0, sizeof(c_text));
                SCI_MEMSET(item_list_ptr->text2_buf, 0x0, 
                    sizeof(item_list_ptr->text2_buf));
                
                sprintf(c_text, "%d", item_list_ptr->total);
                MMIAPICOM_StrToWstr((uint8 *)c_text, item_list_ptr->text2_buf);
                text_len = MMIAPICOM_Wstrlen(item_list_ptr->text2_buf);
                MMI_GetLabelTextByLang(TXT_ITEMS, &text_str);
                MMIAPICOM_Wstrncpy(item_list_ptr->text2_buf + text_len, 
                    text_str.wstr_ptr, MIN(text_str.wstr_len, 
                    MMIIKB_UI_TOTAL_APP_BUF_LEN-text_len));
            }
            else
            {
                MMIAPIFMM_GetFileSizeString(total_size, item_list_ptr->text2_buf, 
                    MMIIKB_UI_TOTAL_APP_BUF_LEN, FALSE);
            }
            
            text_len = MMIAPICOM_Wstrlen(item_list_ptr->text2_buf);
            item_list_ptr->text2_len = text_len;
        }
    }
    else
    {
        if (MMIIKB_BACKUP_LIST_CTRL_ID == ctrl_id)
        {
            SCI_MEMSET(item_list_ptr->text2_buf, 0x0, 
                sizeof(item_list_ptr->text2_buf));
            item_list_ptr->text2_len = 0;
        }
    }

    return;
}

/*****************************************************************************/
//  Description : GetFirstTypeTotalValue
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint16 GetFirstTypeTotalValue(
    MMIIKB_TYPE_T types,
    const MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_ptr,
    uint16 list_num
)
{
    uint32 total = 0;
    const MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    uint16 i = 0;

    if (PNULL == ui_list_ptr
        || 0 == list_num
        || 0 == types)
    {
        //SCI_TRACE_LOW:"[MMIIKB] GetFirstTypeTotalValue: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1717_112_2_18_2_24_37_156,(uint8*)"");
        return 0;
    }

    for (i = 0; i < list_num; i++)
    {
        if (0 != (types & (0x01<<i)))
        {
            cur_list_ptr = ui_list_ptr + i;
            total = cur_list_ptr->total;
            break;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] GetFirstTypeTotalValue types=%d, first total=%d, type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1732_112_2_18_2_24_37_157,(uint8*)"ddd", types, total, i);

    return total;
}

/*****************************************************************************/
//  Description : UpdateProcessBar
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdateProcessBar(
    MMIIKB_TASK_SEQ_TABLE_T *seq_task_item_ptr
)
{
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;
    MMI_WIN_ID_T win_id = 0;
    uint32 finished_size = 0;
    uint32 total_size = 0;
    wchar units[MMIIKB_PRO_TIPS_MAX_LEN] = {0};
    MMI_STRING_T text_str = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T win_text_str = {0};
    uint16 advanced = 0;
    static BOOLEAN is_new_item = TRUE;
    
    if (PNULL == seq_task_item_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] UpdateProcessBar: param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1758_112_2_18_2_24_37_158,(uint8*)"");
        return;
    }
    
    type = seq_task_item_ptr->type;

    if (MMIIKB_TASK_TYPE_BACKUP == seq_task_item_ptr->task_type)
    {
        win_id = MMIIKB_BACKUP_PRG_WAIT_WIN_ID;
    }
    else if (MMIIKB_TASK_TYPE_RESTORE == seq_task_item_ptr->task_type)
    {
        win_id = MMIIKB_RESTORE_PRG_WAIT_WIN_ID;
    }

    if(0 == win_id || !MMK_IsFocusWin(win_id))
    {
        //SCI_TRACE_LOW:"[MMIIKB] UpdateProcessBar: win_id=%04x."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1775_112_2_18_2_24_37_159,(uint8*)"d", win_id);
        return;
    }
    
    switch (type)
    {
    case MMIIKB_TYPE_CALLLOG:
        finished_size = seq_task_item_ptr->trans_number;
        total_size = seq_task_item_ptr->total_number;
        text_id = STXT_MAIN_CL;
        break;
    case MMIIKB_TYPE_PHONEBOOK:
        finished_size = seq_task_item_ptr->trans_number;
        total_size = seq_task_item_ptr->total_number;
        text_id = STXT_MAIN_PB;
        break;
    case MMIIKB_TYPE_SMS:
        finished_size = seq_task_item_ptr->trans_number;
        total_size = seq_task_item_ptr->total_number;
        text_id = TXT_COMMUNICATION_MESSAGE;
        break;
    case MMIIKB_TYPE_SCHEDULE:
        finished_size = seq_task_item_ptr->trans_number;
        total_size = seq_task_item_ptr->total_number;
        text_id = TXT_TOOLS_CALENDAR;
        break;
        
#ifdef BROWSER_SUPPORT
    case MMIIKB_TYPE_BRO_LABEL:
        finished_size = seq_task_item_ptr->trans_size;
        total_size = seq_task_item_ptr->total_size;
        text_id = TXT_COMMUNICATION_BROWSER;
        break;
#endif

    default:
        break;
    }

    if (MMIIKB_TASK_STATE_END == seq_task_item_ptr->task_state)
    {
        advanced = 0;
        MMI_GetLabelTextByLang(text_id, &text_str);
        SCI_MEMSET(units, 0x0, sizeof(units));
        MMIAPICOM_Wstrncpy(units, text_str.wstr_ptr, 
            MIN(text_str.wstr_len, MMIIKB_PRO_TIPS_MAX_LEN-1));
        advanced += text_str.wstr_len;

        if (advanced < MMIIKB_PRO_TIPS_MAX_LEN)
        {
            MMIAPICOM_Wstrcat(units + advanced, L":");
            advanced++;

            if (advanced < MMIIKB_PRO_TIPS_MAX_LEN)
            {
                SCI_MEMSET(&text_str, 0x00, sizeof(text_str));    
                MMI_GetLabelTextByLang(STXT_FINISH, &text_str);
                MMIAPICOM_Wstrncpy(units + advanced,
                    text_str.wstr_ptr, 
                    MIN(text_str.wstr_len, MMIIKB_PRO_TIPS_MAX_LEN-advanced-1));
            }
        }
                
        win_text_str.wstr_ptr = units;
        win_text_str.wstr_len = MMIAPICOM_Wstrlen(win_text_str.wstr_ptr);
        MMIPUB_SetProgressBarTextByPtr(win_id, &win_text_str, FALSE);
        is_new_item = TRUE;
    }
    else
    {
        if (is_new_item)
        {
            advanced = 0;
            MMI_GetLabelTextByLang(TXT_IKB_CUR_ITEM , &text_str);
            SCI_MEMSET(units, 0x0, sizeof(units));
            MMIAPICOM_Wstrncpy(units, text_str.wstr_ptr, 
                MIN(text_str.wstr_len, MMIIKB_PRO_TIPS_MAX_LEN-1));
            advanced += text_str.wstr_len;

            if (advanced < MMIIKB_PRO_TIPS_MAX_LEN)
            {
                SCI_MEMSET(&text_str, 0x00, sizeof(text_str));
                MMI_GetLabelTextByLang(text_id, &text_str);
                MMIAPICOM_Wstrncpy(units + advanced, text_str.wstr_ptr, 
                    MIN(text_str.wstr_len, MMIIKB_PRO_TIPS_MAX_LEN-advanced-1));
            }
        }
            
        win_text_str.wstr_ptr = units;
        win_text_str.wstr_len = MMIAPICOM_Wstrlen(win_text_str.wstr_ptr);
        MMIPUB_SetProgressBarTextByPtr(win_id, &text_str, FALSE);
        is_new_item = FALSE;
    }
    
    MMIPUB_SetProgressTotalSize(&win_id, total_size);
    MMIPUB_UpdateProgressBarEx(&win_id, finished_size, TRUE);
        
    return;
}

/*****************************************************************************/
//  Description : CancelCurAction
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CancelCurAction(MMIIKB_TASK_TYPE_E task_type)
{
    MMI_WIN_ID_T win_cancel_id = MMIIKB_CANCEL_WAIT_WIN_ID;
    
    if(MMIIKB_TASK_TYPE_NONE >= task_type
        || MMIIKB_TASK_TYPE_MAX <= task_type)
    {
        //SCI_TRACE_LOW:"[MMIIKB] CancelCurAction param is 0, task_type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1887_112_2_18_2_24_37_160,(uint8*)"d",task_type);
        
        return;
    }
        
    if(MMK_IsOpenWin(win_cancel_id))
    {
        MMK_CloseWin(win_cancel_id);
    }
    
    //MMIAPIIKB_SuspendTask();
    
    MMIPUB_OpenQueryWinByTextIdEx(
        MMK_GetFirstAppletHandle(),
        TXT_IKB_CANCEL_QUERY,
        IMAGE_PUBWIN_QUERY,
        &win_cancel_id,
        HandleCancelActionQueryMsg,
        (uint32)task_type
        );

    return;
}

/*****************************************************************************/
//  Description : handle backup progressbar event
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelActionQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMIIKB_TASK_TYPE_E task_type = MMIIKB_TASK_TYPE_NONE;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL != pubwin_info_ptr)
    {
        task_type = (MMIIKB_TASK_TYPE_E)pubwin_info_ptr->user_data;
    }
    
    if(MMIIKB_TASK_TYPE_NONE >= task_type
        || MMIIKB_TASK_TYPE_MAX <= task_type)
    {
        //SCI_TRACE_LOW:"[MMIIKB] HandleCancelActionQueryMsg param is 0, task_type=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_1933_112_2_18_2_24_37_161,(uint8*)"d", task_type);
        
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        IKeyBackup_SuspendTask();
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (MMIIKB_TASK_TYPE_BACKUP == task_type)
        {
            MMK_CloseWin(MMIIKB_BACKUP_PRG_WAIT_WIN_ID);
        }
        else if (MMIIKB_TASK_TYPE_RESTORE == task_type)
        {
            MMK_CloseWin(MMIIKB_RESTORE_PRG_WAIT_WIN_ID);
        }        
        
        MMIAPIIKB_CancelActionReq(task_type);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        IKeyBackup_ResumeTask();
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id,param);
        //MMIAPIIKB_SuspendTask();
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : EndCancelAction
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void EndCancelAction(MMIIKB_SIG_CANCEL_RES_T *sig_cancel_res_ptr)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint16 path_len = 0;

    if (PNULL == sig_cancel_res_ptr)
    {
        return;
    }

    //handle result
    error_code = sig_cancel_res_ptr->error_code;

    if (MMIIKB_TASK_TYPE_BACKUP == sig_cancel_res_ptr->task_type)
    {
        if (0 != sig_cancel_res_ptr->pack_dir[0])
        {
            path_len = MMIAPICOM_Wstrlen(sig_cancel_res_ptr->pack_dir);
            if (MMIAPIFMM_IsFolderExist(sig_cancel_res_ptr->pack_dir, path_len))
            {
                DeletePhysicalDir(sig_cancel_res_ptr->pack_dir, path_len);
            }
        }
    }

    //针对结果的提示
    ShowTips(sig_cancel_res_ptr->task_type, error_code);

    return;
}

/*****************************************************************************/
//  Description : DeletePhysicalDir
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeletePhysicalDir(const wchar* pack_dir, uint16 path_len)
{
    wchar *str_ibphone_dir = PNULL;
    uint16 dir_len = path_len;
    wchar *str_ibphone_name = PNULL;
    uint16 name_len = 0;
    BOOLEAN is_ret = FALSE;

    if (PNULL == pack_dir
        || 0 == path_len)
    {
        //SCI_TRACE_LOW:"[MMIIKB] DeletePhysicalDir param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2016_112_2_18_2_24_38_162,(uint8*)"");
        return is_ret;        
    }
    
    str_ibphone_dir = (wchar *)SCI_ALLOCA(sizeof(wchar)*(path_len+1));
    if (PNULL == str_ibphone_dir)
    {
        //SCI_TRACE_LOW:"[MMIIKB] DeletePhysicalDir no memory."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2023_112_2_18_2_24_38_163,(uint8*)"");
        return is_ret;
    }
    SCI_MEMSET(str_ibphone_dir, 0x0, sizeof(wchar)*(path_len+1));
    
    str_ibphone_name = (wchar *)SCI_ALLOCA(sizeof(wchar)*(path_len+1));
    if (PNULL == str_ibphone_name)
    {
        //SCI_TRACE_LOW:"[MMIIKB] DeletePhysicalDir no memory."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2031_112_2_18_2_24_38_164,(uint8*)"");
        SCI_FREE(str_ibphone_dir);
        return is_ret;
    }
    SCI_MEMSET(str_ibphone_name, 0x0, sizeof(wchar)*(path_len+1));
    
    MMIAPICOM_Wstrcpy(str_ibphone_dir, pack_dir);

    is_ret = MMIAPIFMM_SplitLastName(str_ibphone_dir, &dir_len, 
        str_ibphone_name, &name_len);
    
    if (is_ret && SFS_NO_ERROR == MMIAPIFMM_DeleteTree(
        str_ibphone_dir, str_ibphone_name))
    {
        is_ret = TRUE;
    }
    else
    {
        is_ret = FALSE;            
    }
    
    SCI_FREE(str_ibphone_dir);    
    SCI_FREE(str_ibphone_name);

    return is_ret;
}




//backup list
/*****************************************************************************/
//  Description : set input password window control attribute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetEnterPswWinAttri(void)
{
    MMI_STRING_T psw_str = {0};
    MMI_STRING_T psw_verfy_str = {0};

    MMI_GetLabelTextByLang(TXT_COMN_PASSWORD_INPUT, &psw_str);
    MMI_GetLabelTextByLang(TXT_IKB_PLS_INPUT_VERFY_PSW, &psw_verfy_str);
    
    GUILABEL_SetText(MMIIKB_PSW_LABEL_CTRL_ID, &psw_str, FALSE);
    GUILABEL_SetText(MMIIKB_PSW_VERFY_LABEL_CTRL_ID, &psw_verfy_str, FALSE);
}

/*****************************************************************************/
//  Description : OnClickBackupPswSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickBackupPswSoftkey(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id1,
    MMI_CTRL_ID_T ctrl_id2
)
{
    MMI_STRING_T pwd_value1 = {0};
    MMI_STRING_T pwd_value2 = {0};
    
    GUIEDIT_GetString(ctrl_id1, &pwd_value1);
    GUIEDIT_GetString(ctrl_id2, &pwd_value2);

    if (0 == pwd_value1.wstr_len)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_PLS_INPUT_PWD,
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
            MMIPUB_SOFTKEY_ONE, PNULL);
        GUIEDIT_ClearAllStr(ctrl_id1);
        return;
    }

    if (0 == pwd_value2.wstr_len)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_PLS_INPUT_PWD,
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
            MMIPUB_SOFTKEY_ONE, PNULL);
        GUIEDIT_ClearAllStr(ctrl_id2);
        return;
    }
    
    if (BACKUP_PWD_LEN_IS_IN_RANGE(pwd_value1.wstr_len)
        && BACKUP_PWD_LEN_IS_IN_RANGE(pwd_value2.wstr_len))
    {
        if (pwd_value1.wstr_len != pwd_value2.wstr_len)
        {
            //长度不相等
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_IKB_UNEQUAL_PSW_LEN,
                TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
                MMIPUB_SOFTKEY_ONE, PNULL);
        }
        else
        {
            if (0 == MMIAPICOM_Wstrncmp(pwd_value1.wstr_ptr, 
                pwd_value2.wstr_ptr, pwd_value1.wstr_len))
            {
                MMIIKB_PRE_BACKUP_DATA_T pre_backup_data = {0};
                SCI_MEMSET(&pre_backup_data, 0x0, 
                    sizeof(pre_backup_data));
                
                pre_backup_data.types = 
                    ((MMIIKB_TYPE_T)(uint32)MMK_GetWinAddDataPtr(
                        win_id));
                pre_backup_data.psw_info.is_password 
                    = TRUE;
                pre_backup_data.psw_info.password_len 
                    = pwd_value1.wstr_len;
                MMIAPICOM_Wstrncpy(
                    pre_backup_data.psw_info.password, 
                    pwd_value1.wstr_ptr, pwd_value1.wstr_len);
                
                MMK_CloseWin(win_id);
                
                BackupSelItems(&pre_backup_data);
            }
            else
            {
                //提示密码不匹配
                MMIPUB_OpenAlertWinByTextId(PNULL, 
                    TXT_MSGBOX_PWD_CONFLICT, TXT_NULL, 
                    IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
                    MMIPUB_SOFTKEY_ONE, PNULL);
                //GUIEDIT_ClearAllStr(ctrl_id);
            }
        }
    }
    else
    {
        //wrong length
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_IKB_WRONG_PSW_LEN,
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
            MMIPUB_SOFTKEY_ONE, PNULL);
    }

    return;
}

/*****************************************************************************/
//  Description : handle add backup password window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterBackupPwd(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id1 = MMIIKB_SET_BACKUP_PSW_CTRL_ID;
    MMI_CTRL_ID_T ctrl_id2 = MMIIKB_SET_BACKUP_VERFY_PSW_CTRL_ID;
    MMI_CTRL_ID_T btn_ctrl_id = 0;
    //GUI_BOTH_RECT_T edit_both_rect = {0};
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetEnterPswWinAttri();
        GUIEDIT_SetImTag(ctrl_id1, GUIIM_TAG_PURE_NUM);
        GUIEDIT_SetImTag(ctrl_id2, GUIIM_TAG_PURE_NUM);
        MMK_SetAtvCtrl(win_id, ctrl_id1);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef MMI_PDA_SUPPORT
            if (PNULL != param)
            {
                btn_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
                if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    OnClickBackupPswSoftkey(win_id, ctrl_id1, ctrl_id2);                    
                    break;
                }
                else if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_CloseWin(win_id);
                    break;
                }
            }
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        OnClickBackupPswSoftkey(win_id, ctrl_id1, ctrl_id2);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle backup progressbar event
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIKB_HandleBackupProWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            //打开一个询问窗口，是否确认取消
            CancelCurAction(MMIIKB_TASK_TYPE_BACKUP);
        }
        break;

		//Bug 151383
		/*一键备份过程中，在按power键或home键退出备份操作时，需要提示是否中止，若要回到idle界面下，需要按两次power键或home键*/
#ifdef MMI_PDA_SUPPORT
	case MSG_KEYUP_RED:
#else		
	case MSG_APP_RED:
#endif 
        {
            //打开一个询问窗口，是否确认中止
            AbortCurAction(MMIIKB_TASK_TYPE_BACKUP);
        }
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : OptimizeSelTypes
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 优化选择的类别
/*****************************************************************************/
LOCAL MMIIKB_TYPE_T OptimizeBackupSelTypes(MMIIKB_TYPE_T types)
{
    MMIIKB_TYPE_T ret_types = types;
    const MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    uint16 i = 0;

    for (i = 0; i < ARR_SIZE(s_ui_backup_list); i++)
    {
        cur_list_ptr = s_ui_backup_list + i;
        
        if (cur_list_ptr->is_check
            && !cur_list_ptr->is_gray
            && 0 == cur_list_ptr->total)
        {
            ret_types &= ~(0x01<<cur_list_ptr->user_data);/*lint !e502*/
        }
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] OptimizeBackupSelTypes types=%d, opt_types=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2269_112_2_18_2_24_38_165,(uint8*)"dd", types, ret_types);

    return ret_types;
}

/*****************************************************************************/
//  Description : PreCheckBackupItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL BOOLEAN PreCheckBackupItems(
    MMIIKB_TYPE_T types,                //in
    MMIIKB_TYPE_T *opt_types_ptr        //out，过滤后的备份类型
)
{
    //FIXME: Address alerting for PDA style is not ready recently.
    BOOLEAN is_ret = FALSE;

    if (PNULL == opt_types_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] PreCheckBackupItems param is null, types=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2289_112_2_18_2_24_38_166,(uint8*)"d",types);
        return is_ret;
    }
                    
    if (0 == types)
    {
        //无内容提示
        MMIPUB_OpenAlertWarningWin(TXT_PLS_SELECT_ONE_ITEM);
    }
    else if (0 != (types & (0x01 << MMIIKB_TYPE_BRO_LABEL)))
    {
        //暂不支持浏览器书签
        MMIPUB_OpenAlertWarningWin(TXT_IKB_NOT_SUPPORT_LABLE);
    }
    else
    {
        //过滤掉０条的栏目
        *opt_types_ptr = OptimizeBackupSelTypes(types);
        //SCI_TRACE_LOW:"[MMIIKB] PreCheckBackupItems types, opt_types=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2308_112_2_18_2_24_38_167,(uint8*)"d", types, *opt_types_ptr);
        if (0 == *opt_types_ptr)
        {
            //选中的各个项目都是０条，则需要跳过去
            MMIPUB_OpenAlertWarningWin(TXT_IKB_ALL_ZERO_ITEM);
            
            return is_ret;
        }
        
        if(MMIAPIIKB_IsBusy())
        {
            MMIPUB_OpenAlertWarningWin(TXT_IKB_TASK_IS_BUSY);
            
            return is_ret;
        }
        
        
        //开始备份
        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : IsNeedInputPsd
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 检查是否需要输入密码
/*****************************************************************************/
LOCAL BOOLEAN IsNeedInputPsd(
    MMIIKB_TYPE_T types
)
{
    BOOLEAN is_ret = FALSE;
    MMIIKB_CONFIG_T * cfg_ptr = MMIAPIIKB_GetConfigInfo();

    if (0 == types)
    {
        //SCI_TRACE_LOW:"[MMIIKB] IsNeedInputPsd param is null, types=0."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2345_112_2_18_2_24_38_168,(uint8*)"");
        return is_ret;
    }

    is_ret = (0 != (cfg_ptr->types&types));

    return is_ret;
}

/*****************************************************************************/
//  Description : BackupSelItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL void BackupSelItems(MMIIKB_PRE_BACKUP_DATA_T *pre_backup_data_ptr)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMI_WIN_ID_T progress_win_id = MMIIKB_BACKUP_PRG_WAIT_WIN_ID;
    uint32 total = 0;

    if (PNULL == pre_backup_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] BackupSelItems param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2366_112_2_18_2_24_38_169,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"[MMIIKB] BackupSelItems types=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2371_112_2_18_2_24_38_170,(uint8*)"d", pre_backup_data_ptr->types);
    
    //开始备份    
    error_code = StartBackupItems(pre_backup_data_ptr);

    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        if(MMK_IsOpenWin(progress_win_id))
        {
            MMK_CloseWin(progress_win_id);
        }
        
        MMIPUB_OpenProgressWinByTextId(TXT_IKB_BEING_BACKUP, 
            &progress_win_id, PNULL, MMIPUB_SOFTKEY_ONE, 
            MMIIKB_HandleBackupProWinMsg);
        total = GetFirstTypeTotalValue(pre_backup_data_ptr->types,
            s_ui_backup_list, ARR_SIZE(s_ui_backup_list));
        MMIPUB_SetProgressTotalSize(&progress_win_id, total);
    }
    else
    {
        ShowTips(MMIIKB_TASK_TYPE_BACKUP, error_code);
    }
}

/*****************************************************************************/
//  Description : StartBackupItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E StartBackupItems(
    MMIIKB_PRE_BACKUP_DATA_T *pre_backup_data_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_BACKUP_REQ_T backup_req = {0};
    char c_pack_dir_name[MMIIKB_PACK_NAME_LEN] = {0};
    wchar w_pack_dir_name[MMIIKB_PACK_NAME_LEN] = {0};
    SCI_DATE_T cur_date = {0};
    SCI_TIME_T cur_time = {0};
    uint16 path_len  = 0;
    BOOLEAN is_find_dir = FALSE;
    BOOLEAN is_ret = FALSE;

    if (PNULL == pre_backup_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] StartBackupItems param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2416_112_2_18_2_24_38_171,(uint8*)"");
        return error_code;
    }    

    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

    //1: create pack dir
    sprintf(c_pack_dir_name, "%04d-%02d-%02d_%02d-%02d-%02d",
        cur_date.year, cur_date.mon, cur_date.mday,
        cur_time.hour, cur_time.min, cur_time.sec);

    MMIAPICOM_StrToWstr((uint8*)c_pack_dir_name, w_pack_dir_name);
    
    SCI_MEMSET(backup_req.pack_dir, 0x0, sizeof(backup_req.pack_dir));
    is_find_dir = MMIIKB_GetCurBaseDir(backup_req.pack_dir, &path_len);

    if (is_find_dir)
    {
        if (!MMIAPIFMM_IsFolderExist(backup_req.pack_dir, path_len))
        {
            is_ret = MMIAPIFMM_CreateDir(backup_req.pack_dir, path_len);
        }

        //set hide dir attribute
        MMIAPIFMM_SetAttr(backup_req.pack_dir, path_len, 
            TRUE, FALSE, FALSE, FALSE);        

        MMIAPICOM_Wstrcat(backup_req.pack_dir, L"\\");
        MMIAPICOM_Wstrcat(backup_req.pack_dir, w_pack_dir_name);
        path_len = MMIAPICOM_Wstrlen(backup_req.pack_dir);

        if (MMIAPIFMM_IsFolderExist(backup_req.pack_dir, path_len))
        {
            DeletePhysicalDir(backup_req.pack_dir, path_len);            
        }
        is_ret = MMIAPIFMM_CreateDir(backup_req.pack_dir, path_len);

        if (is_ret)
        {
            //2: set backup types value
            backup_req.types = pre_backup_data_ptr->types;

            //3: set config value
            backup_req.cfg_ptr = MMIAPIIKB_GetConfigInfo();

            //4: copy password info
            SCI_MEMCPY(&backup_req.psw_info, &pre_backup_data_ptr->psw_info, 
                sizeof(MMIIKB_PASSWORD_INFO_T));

            //SCI_TRACE_LOW:"[MMIIKB] StartBackupItems send request, types=%d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2467_112_2_18_2_24_39_172,(uint8*)"d", pre_backup_data_ptr->types);
            
            error_code = MMIAPIIKB_BackupAllItemReq(&backup_req);
        }
        else
        {
            error_code = MMIIKB_STATUS_SD_OPERATE_FAIL;
        }
    }
    else
    {
        error_code = MMIIKB_STATUS_SD_NOT_READY;
    }

    return error_code;
}

/*****************************************************************************/
//  Description : EndBackupItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 备份完成后的操作
/*****************************************************************************/
LOCAL void EndBackupItems(MMIIKB_SIG_BACKUP_RES_T *sig_backup_res_ptr)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    uint16 path_len = 0;
    uint16 path_ptr[MMIIKB_CON_PATH_LEN] = {0};
    MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E file_error = SFS_ERROR_NONE;

    MMIIKB_PACK_INFO_T pack_info = {0};
    uint32 i = 0;
    uint32 write_len = 0;
    MMI_WIN_ID_T progress_win_id = MMIIKB_BACKUP_PRG_WAIT_WIN_ID;
    MMI_WIN_ID_T win_cancel_id = MMIIKB_CANCEL_WAIT_WIN_ID;

    SCI_MEMSET(&pack_info, 0x0, sizeof(MMIIKB_PACK_INFO_T));
    
    //handle result
    if (PNULL != sig_backup_res_ptr)
    {
        error_code = sig_backup_res_ptr->error_code;
        
        if (0 != sig_backup_res_ptr->pack_dir[0])
        {
            MMIAPICOM_Wstrcat(path_ptr, sig_backup_res_ptr->pack_dir);
            path_len = MMIAPICOM_Wstrlen(path_ptr);
            if (MMIIKB_STATUS_SUCCESS != sig_backup_res_ptr->error_code)
            {
                //fail
                if (MMIAPIFMM_IsFolderExist(path_ptr, path_len))
                {
                    DeletePhysicalDir(path_ptr, path_len);
                }
            }
            else
            {
                MMIAPICOM_Wstrncpy(pack_info.mod_name, MMIIKB_MODU_NAME, MMIIKB_MODULE_LEN);
                error_code = sig_backup_res_ptr->error_code;
                pack_info.types = sig_backup_res_ptr->types;               
                SCI_MEMCPY(&pack_info.psw_info, 
                    &sig_backup_res_ptr->psw_info,
                    sizeof(MMIIKB_PASSWORD_INFO_T));
            
                MMIAPICOM_Wstrcpy(pack_info.pack_name, 
                    &(sig_backup_res_ptr->pack_dir[MMIIKB_UI_PACK_NAME_BEGIN]));
                
                MMIAPICOM_Wstrcpy(pack_info.pack_dir, 
                    sig_backup_res_ptr->pack_dir);
                
                pack_info.size = sig_backup_res_ptr->size;
                for (i = 0; i < MMIIKB_TYPE_MAX_CNT; i++)
                {
                    pack_info.crcs[i] = sig_backup_res_ptr->crcs[i];
                }

                //write version info
                SCI_MEMCPY(pack_info.version, s_str_ikb_version, 
                    MMIIKB_VERSION_NUM_LEN);
                
                //success                            
                MMIAPICOM_Wstrcat(path_ptr, L"\\");
                MMIAPICOM_Wstrcat(path_ptr, s_packinfo_name);
                path_len = MMIAPICOM_Wstrlen(path_ptr);

                if (MMIAPIFMM_IsFileExist(path_ptr, path_len))
                {
                    MMIAPIFMM_DeleteFile(path_ptr, PNULL);
                }
                
                file_handle = MMIAPIFMM_CreateFile(path_ptr, 
                    SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, 0, 0);

                if (SFS_INVALID_HANDLE != file_handle)
                {
                    //write content head to occupy position
                    i = sizeof(MMIIKB_PACK_INFO_T);
                    file_error = MMIAPIFMM_WriteFile(file_handle, 
                        &pack_info, i, &write_len, PNULL);                                
                }
                MMIAPIFMM_CloseFile(file_handle);
            }
        }
    }

    //无论如何先关闭进度条窗口        
    if(MMK_IsOpenWin(progress_win_id))
    {
        MMK_CloseWin(progress_win_id);
    }

    //如果取消询问窗口打开的话，则关闭      
    if(MMK_IsOpenWin(win_cancel_id))
    {
        MMK_CloseWin(win_cancel_id);
    }

    //针对结果的提示
    ShowTips(MMIIKB_TASK_TYPE_BACKUP, error_code);
}

/*****************************************************************************/
//  Description : InitBackupListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitBackupListCtrl(void)
{
    //uint16 i = 0;
    //MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    
    return;
}

/*****************************************************************************/
//  Description : OnClickBackupSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickBackupSoftkey(void)
{
    MMIIKB_TYPE_T types = GetListItemSelectValues(
        s_ui_backup_list, ARR_SIZE(s_ui_backup_list));
    MMIIKB_TYPE_T opt_types = 0;

    if (PreCheckBackupItems(types, &opt_types))
    {
        if (IsNeedInputPsd(opt_types))
        {
            //打开设置密码窗口
            MMK_CreateWin((uint32*)MMIIKB_ENTER_PSW_WIN_TAB, 
                (ADD_DATA)opt_types);
        }
        else
        {
            MMIIKB_PRE_BACKUP_DATA_T pre_backup_data = {0};
            SCI_MEMSET(&pre_backup_data, 0x0, 
                sizeof(pre_backup_data));
            
            pre_backup_data.types = opt_types;
            pre_backup_data.psw_info.is_password 
                = FALSE;
            pre_backup_data.psw_info.password_len 
                = 0;
            
            BackupSelItems(&pre_backup_data);
        }
        
    }

    return;
}

/*****************************************************************************/
//  Description : HandleBackupChildWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBackupChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl_id = MMIIKB_BACKUP_LIST_CTRL_ID;
    MMI_CTRL_ID_T btn_ctrl_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            InitListUIValue(s_ui_backup_list, ARR_SIZE(s_ui_backup_list));
            GUILIST_SetMaxItem(list_ctrl_id, ARR_SIZE(s_ui_backup_list), FALSE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id,ARR_SIZE(s_ui_backup_list));
            AppendALLListItemCtrl(list_ctrl_id, s_ui_backup_list, 
                ARR_SIZE(s_ui_backup_list));
            
#ifdef MMI_PDA_SUPPORT
            {
                GUIFORM_CHILD_HEIGHT_T child_height = {0};
                child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
                GUILIST_PermitBorder(list_ctrl_id, FALSE);
                GUIFORM_IsSlide(MMIIKB_BACKUP_LIST_FORM_CTRL_ID, FALSE);
                GUIFORM_SetChildHeight(MMIIKB_BACKUP_LIST_FORM_CTRL_ID, 
                    list_ctrl_id, &child_height);
                GUIFORM_PermitChildBg(MMIIKB_BACKUP_LIST_FORM_CTRL_ID, FALSE);
                GUILIST_SetUserBg(list_ctrl_id, FALSE);
            }
#endif
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
/*#if defined(MMI_PDA_SUPPORT)
    case MSG_APP_MENU:
#endif*/
        {
            OnClickBackupSoftkey();
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:        
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT)
        if (MSG_CTL_PENOK == msg_id && PNULL != param)
        {
            btn_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                OnClickBackupSoftkey();
                break;
            }
        }
#endif

        OnClickCheckListItem(list_ctrl_id, s_ui_backup_list,
            ARR_SIZE(s_ui_backup_list));
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_CTL_TAB_SWITCH:
        s_cur_ui_tab_type = MMIIKB_UI_TAB_BACKUP;
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIIKB_MAIN_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}



//restore list
/*****************************************************************************/
//  Description : set input password window control attribute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetEnterResPswWinAttri(void)
{
    MMI_STRING_T psw_str = {0};

    MMI_GetLabelTextByLang(TXT_COMN_PASSWORD_INPUT, &psw_str);
    
    GUILABEL_SetText(MMIIKB_RES_PSW_LABEL_CTRL_ID, &psw_str, FALSE);
}

/*****************************************************************************/
//  Description : OnClickRestorePswSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickRestorePswSoftkey(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id1,
    MMIIKB_CHECK_PSW_WIN_DATA_T *psw_win_data_ptr
)
{
    MMI_STRING_T pwd_value1 = {0};

    if (PNULL == psw_win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] OnClickRestorePswSoftkey win_data is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2744_112_2_18_2_24_39_173,(uint8*)"");
        return;
    }
    
    GUIEDIT_GetString(ctrl_id1, &pwd_value1);

    if (0 == pwd_value1.wstr_len)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_PLS_INPUT_PWD,
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
            MMIPUB_SOFTKEY_ONE, PNULL);
        GUIEDIT_ClearAllStr(ctrl_id1);
        return;
    }
    
    if (BACKUP_PWD_LEN_IS_IN_RANGE(pwd_value1.wstr_len))
    {
        if (pwd_value1.wstr_len == 
                psw_win_data_ptr->psw_info_t.password_len
            && 0 == MMIAPICOM_Wstrncmp(pwd_value1.wstr_ptr, 
                psw_win_data_ptr->psw_info_t.password, 
                pwd_value1.wstr_len))
        {
            uint16 intem_index = psw_win_data_ptr->intem_index;
            MMIIKB_TYPE_T types = psw_win_data_ptr->types;
            
            MMK_CloseWin(win_id);
            RestoreSelItems(intem_index, types);
        }
        else
        {
            //提示密码不匹配
            MMIPUB_OpenAlertWinByTextId(PNULL, 
                TXT_MSGBOX_PWD_CONFLICT, TXT_NULL, 
                IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
                MMIPUB_SOFTKEY_ONE, PNULL);
            GUIEDIT_ClearAllStr(ctrl_id1);
        }
    }
    else
    {
        //wrong length
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_IKB_WRONG_PSW_LEN,
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
            MMIPUB_SOFTKEY_ONE, PNULL);
        GUIEDIT_ClearAllStr(ctrl_id1);
    }

    return;
}

/*****************************************************************************/
//  Description : handle add restore password window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnterRestorePwd(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id1 = MMIIKB_SET_RESTORE_PSW_CTRL_ID;
    MMI_CTRL_ID_T btn_ctrl_id = 0;
    MMIIKB_CHECK_PSW_WIN_DATA_T *psw_win_data_ptr = 
        (MMIIKB_CHECK_PSW_WIN_DATA_T *)MMK_GetWinAddDataPtr(win_id);

    if (PNULL == psw_win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] HandleEnterRestorePwd win_data is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_2804_112_2_18_2_24_39_174,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetEnterResPswWinAttri();
        GUIEDIT_SetImTag(ctrl_id1, GUIIM_TAG_PURE_NUM);
        MMK_SetAtvCtrl(win_id, ctrl_id1);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef MMI_PDA_SUPPORT
            if (PNULL != param)
            {
                btn_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
                if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
                {
                    OnClickRestorePswSoftkey(win_id, ctrl_id1, psw_win_data_ptr);
                    break;
                }
                else if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_CloseWin(win_id);
                    break;
                }
            }
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        OnClickRestorePswSoftkey(win_id, ctrl_id1, psw_win_data_ptr);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        SCI_FREE(psw_win_data_ptr);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : set restore result detail window control attribute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 动态创建各个栏目的提示信息，以后扩展就不需要修改了
/*****************************************************************************/
LOCAL void SetResResultDetailWinAttri(MMIIKB_TYPE_T types)
{
    MMI_STRING_T label_str = {0};
    MMI_WIN_ID_T win_id = MMIIKB_RES_RESULT_DETAIL_WIN_ID;
    MMI_CTRL_ID_T form_id = MMIIKB_RES_DETAIL_FORM_CTRL_ID;
    MMI_CTRL_ID_T item_label_ctrl_id = 0;
    GUIFORM_DYNA_CHILD_T form_child_ctrl= {0};
    GUILABEL_INIT_DATA_T label_init = {0};
    uint16 i = 0;
    wchar item_str[100] = {0};
    char c_text[MMIIKB_UI_TOTAL_APP_BUF_LEN] = {0};
    wchar w_text[MMIIKB_UI_TOTAL_APP_BUF_LEN] = {0};
    MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    const MMIIKB_TASK_SEQ_TABLE_T * task_seq_ptr = PNULL;
    uint16 text_len = 0;

    //set restore finish label
    label_init.align = GUILABEL_ALIGN_LEFT;
    form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    form_child_ctrl.is_bg = FALSE;
    form_child_ctrl.is_get_active = FALSE;
    form_child_ctrl.init_data_ptr = &label_init;

    form_child_ctrl.child_handle = MMIIKB_RES_DETAIL_FINISH_LABEL_CTRL_ID;
    GUIFORM_CreatDynaChildCtrl(win_id, form_id, &form_child_ctrl);
    MMI_GetLabelTextByLang(TXT_IKB_RESTORE_FINISH, &label_str);
    GUILABEL_SetText(MMIIKB_RES_DETAIL_FINISH_LABEL_CTRL_ID, &label_str, FALSE);

    //set restore item information label
    for (i = 0; i < ARR_SIZE(s_ui_restore_list); i++)
    {
        SCI_MEMSET(item_str, 0x0, sizeof(item_str));
        cur_list_ptr = &(s_ui_restore_list[i]);

        if (0 == (types & (0x01<<i))) continue;

        MMI_GetLabelTextByLang(cur_list_ptr->text_id1, &label_str);
        MMIAPICOM_Wstrncpy(item_str, label_str.wstr_ptr, label_str.wstr_len);

        MMIAPICOM_Wstrcat(item_str, L" : ");

        task_seq_ptr = MMIIKB_GetTaskItemByType((MMIIKB_TYPE_E)i);
        if (PNULL == task_seq_ptr) continue;
        
        SCI_MEMSET(c_text, 0x0, sizeof(c_text));        
        sprintf(c_text, "%d", task_seq_ptr->trans_number);
        
        MMIAPICOM_StrToWstr((uint8 *)c_text, w_text);
        MMIAPICOM_Wstrcat(item_str, w_text);

		MMIAPICOM_Wstrcat(item_str, L" ");
        
        MMI_GetLabelTextByLang(TXT_ITEMS, &label_str);
        MMIAPICOM_Wstrncat(item_str, label_str.wstr_ptr, label_str.wstr_len);
        
        text_len = MMIAPICOM_Wstrlen(item_str);
        label_str.wstr_ptr = item_str;
        label_str.wstr_len = text_len;
            
        label_init.align = GUILABEL_ALIGN_LEFT;
        form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;
        form_child_ctrl.init_data_ptr = &label_init;
        item_label_ctrl_id = MMIIKB_RES_DETAIL_ITEM_LABEL_CTRL_ID_START + i;
        form_child_ctrl.child_handle = item_label_ctrl_id;
        GUIFORM_CreatDynaChildCtrl(win_id, form_id, &form_child_ctrl);
        
        GUILABEL_SetText(item_label_ctrl_id, &label_str, FALSE);
    }
}

/*****************************************************************************/
//  Description : handle restore result detail window
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResResultDetailMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id1 = MMIIKB_RES_DETAIL_FORM_CTRL_ID;
    MMI_CTRL_ID_T btn_ctrl_id = 0;
    MMIIKB_TYPE_T * type_ptr = (MMIIKB_TYPE_T *)MMK_GetWinAddDataPtr(win_id);
    if (PNULL == type_ptr)
    {
        return (result);
    }
        
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetResResultDetailWinAttri(*type_ptr);
        MMK_SetAtvCtrl(win_id, ctrl_id1);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef MMI_PDA_SUPPORT
            if (PNULL != param)
            {
                btn_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
                if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
                {
                    MMK_CloseWin(win_id);
                    break;
                }
            }
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        SCI_FREE(type_ptr);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);    
}

/*****************************************************************************/
//  Description : handle restore progressbar event
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIIKB_HandleRestoreProWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            //打开一个询问窗口，是否确认取消
            CancelCurAction(MMIIKB_TASK_TYPE_RESTORE);
        }
        break;
        
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : PreCheckRestoreItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL BOOLEAN PreCheckRestoreItems(uint16 item_index, MMIIKB_TYPE_T types)
{
    BOOLEAN is_ret = FALSE;
                    
    if (0 == types)
    {
        //无内容提示
        MMIPUB_OpenAlertWarningWin(TXT_PLS_SELECT_ONE_ITEM);
    }
    else if (0 != (types & (0x01 << MMIIKB_TYPE_BRO_LABEL)))
    {
        //暂不支持浏览器书签
        MMIPUB_OpenAlertWarningWin(TXT_IKB_NOT_SUPPORT_LABLE);
    }
    else
    {
        if(MMIAPIIKB_IsBusy())
        {
            MMIPUB_OpenAlertWarningWin(TXT_IKB_TASK_IS_BUSY);
            
            return is_ret;
        }
        
        //开始还原
        //SCI_TRACE_LOW:"[MMIIKB] PreCheckRestoreItems types=%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3052_112_2_18_2_24_40_175,(uint8*)"d", types);

        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : RestoreSelItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL void RestoreSelItems(uint16 item_index, MMIIKB_TYPE_T types)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMI_WIN_ID_T progress_win_id = MMIIKB_RESTORE_PRG_WAIT_WIN_ID;
    uint32 total = 0;
    
    //SCI_TRACE_LOW:"[MMIIKB] RestoreSelItems types=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3070_112_2_18_2_24_40_176,(uint8*)"d", types);

    if (0 == types)
    {
        return;
    }
        
    //开始还原    
    error_code = StartRestoreItems(item_index, types);

    if (MMIIKB_STATUS_SUCCESS == error_code)
    {
        if(MMK_IsOpenWin(progress_win_id))
        {
            MMK_CloseWin(progress_win_id);
        }
        
        MMIPUB_OpenProgressWinByTextId(TXT_IKB_BEING_RESTORE, 
            &progress_win_id, PNULL, MMIPUB_SOFTKEY_ONE, 
            MMIIKB_HandleRestoreProWinMsg);
        total = GetFirstTypeTotalValue(types, 
            s_ui_restore_list, ARR_SIZE(s_ui_restore_list));
        MMIPUB_SetProgressTotalSize(&progress_win_id, total);
    }
    else
    {
        ShowTips(MMIIKB_TASK_TYPE_RESTORE, error_code);
    }
}

/*****************************************************************************/
//  Description : GetPackPswInfo
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 会打开UI相关的接口
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E GetPackPswInfo(
    uint16 item_index,
    MMIIKB_PASSWORD_INFO_T *psw_info_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    FILEARRAY_DATA_T file_info = {0};
    MMIIKB_PACK_INFO_T *pack_info_ptr = PNULL;
    
    if (PNULL == psw_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] GetPackPswInfo param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3115_112_2_18_2_24_40_177,(uint8*)"");
        return error_code;
    }
    
    if (MMIIKB_GetPackFileInfo(item_index, &file_info))
    {
        pack_info_ptr = (MMIIKB_PACK_INFO_T *)SCI_ALLOCA(
            sizeof(MMIIKB_PACK_INFO_T));
        if (PNULL == pack_info_ptr)
        {
            error_code = MMIIKB_STATUS_NO_MEMORY;
            return error_code;
        }
        SCI_MEMSET(pack_info_ptr, 0x0, sizeof(MMIIKB_PACK_INFO_T));

        if (MMIIKB_ReadPackFileInfo(file_info.filename, 
            file_info.name_len, pack_info_ptr))
        {
            SCI_MEMCPY(psw_info_ptr, &pack_info_ptr->psw_info, 
                sizeof(MMIIKB_PASSWORD_INFO_T));
            
            error_code = MMIIKB_STATUS_SUCCESS;
        }
        else
        {
            error_code = MMIIKB_STATUS_READ_PACK_ERROR;
        }

        SCI_FREE(pack_info_ptr);
    }
    else
    {
        error_code = MMIIKB_STATUS_READ_PACK_ERROR;
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : StartRestoreItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMIIKB_ERROR_E StartRestoreItems(uint16 item_index, MMIIKB_TYPE_T types)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_RESTORE_REQ_T restore_req = {0};
    FILEARRAY_DATA_T file_info = {0};
    MMIIKB_PACK_INFO_T *pack_info_ptr = PNULL;
    
    if (MMIIKB_GetPackFileInfo(item_index, &file_info))
    {
        pack_info_ptr = (MMIIKB_PACK_INFO_T *)SCI_ALLOCA(
            sizeof(MMIIKB_PACK_INFO_T));
        if (PNULL == pack_info_ptr)
        {
            error_code = MMIIKB_STATUS_NO_MEMORY;
            return error_code;
        }
        SCI_MEMSET(pack_info_ptr, 0x0, sizeof(MMIIKB_PACK_INFO_T));

        if (MMIIKB_ReadPackFileInfo(file_info.filename, 
            file_info.name_len, pack_info_ptr))
        {
            if (IsVersionLegal(pack_info_ptr))
            {
                restore_req.cfg_ptr = MMIAPIIKB_GetConfigInfo();
                restore_req.pack_info_ptr = pack_info_ptr;
                restore_req.types = pack_info_ptr->types & types;

                //SCI_TRACE_LOW:"[MMIIKB] StartRestoreItems send request, pack_types=%d,sel_types=%d, res_types=%d, index=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3184_112_2_18_2_24_40_178,(uint8*)"dddd", pack_info_ptr->types,types, restore_req.types, item_index);
                
                error_code = MMIAPIIKB_RestoreAllItemReq(&restore_req);
            }
            else
            {
                error_code = MMIIKB_STATUS_ILLEGAL_VERSION;
            }
        }
        else
        {
            error_code = MMIIKB_STATUS_READ_PACK_ERROR;
        }

        SCI_FREE(pack_info_ptr);
    }
    else
    {
        error_code = MMIIKB_STATUS_READ_PACK_ERROR;
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : EndRestoreItems
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void EndRestoreItems(MMIIKB_SIG_RESTORE_RES_T *sig_restore_res_ptr)
{
    MMI_WIN_ID_T progress_win_id = MMIIKB_RESTORE_PRG_WAIT_WIN_ID;
    MMI_WIN_ID_T restore_sel_win_id = MMIIKB_RESTORE_ITEM_SEL_WIN_ID;
    MMI_WIN_ID_T win_cancel_id = MMIIKB_CANCEL_WAIT_WIN_ID;
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIIKB_TYPE_T suc_types = 0;
    MMIIKB_TYPE_T err_types = 0;
    MMIIKB_TYPE_T * type_ptr = PNULL;

    //handle result
    if (PNULL != sig_restore_res_ptr)
    {
        error_code = sig_restore_res_ptr->error_code;
    }

    //无论如何先关闭进度条窗口        
    if(MMK_IsOpenWin(progress_win_id))
    {
        MMK_CloseWin(progress_win_id);
    }

    //如果取消询问窗口打开的话，则关闭      
    if(MMK_IsOpenWin(win_cancel_id))
    {
        MMK_CloseWin(win_cancel_id);
    }
    
    //如何恢复完成后，则回退到备份包列表窗口
    if(MMK_IsOpenWin(restore_sel_win_id))
    {
        MMK_CloseWin(restore_sel_win_id);
    }

    //需求针对还原最后结果不是简单的一个成功失败提示，而是需要列出还原清单
    //弹出还原清单窗口
    type_ptr = (MMIIKB_TYPE_T *)SCI_ALLOCA(sizeof(MMIIKB_TYPE_T));
    if (PNULL != sig_restore_res_ptr)
    {
        suc_types = sig_restore_res_ptr->suc_types;
        err_types = sig_restore_res_ptr->err_types;
        *type_ptr = suc_types | err_types;
    }
    else
    {
        *type_ptr = 0;
    }
    
    SCI_TRACE_LOW("[MMIIKB] EndRestoreItems suc=%d, err=%d, types=%d.",
        suc_types, err_types, *type_ptr);

    MMK_CreateWin((uint32*)MMIIKB_RES_RESULT_DETAIL_WIN_TAB, 
        (ADD_DATA)type_ptr);
    

    //针对结果的提示
    //ShowTips(MMIIKB_TASK_TYPE_RESTORE, error_code);
}

/*****************************************************************************/
//  Description : CreatePackListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreatePackListCtrl(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id
)
{
    GUILIST_INIT_DATA_T list_init_data = {0};
    MMI_CONTROL_CREATE_T create = {0};

    //set init data
    list_init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    list_init_data.type = GUILIST_TEXTLIST_E;

    create.guid = SPRD_GUI_LIST_ID;
    create.init_data_ptr = &list_init_data;

    //creat control
    create.ctrl_id = ctrl_id;
    create.parent_win_handle = win_id;

    MMK_CreateControl(&create);

    //set active
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

/*****************************************************************************/
//  Description : ClearPackList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void ClearPackList(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id
)
{
    //set list enable mark
    MMIIKB_SetListMarkable(ctrl_id, FALSE);
    
    GUILIST_SetMaxItem(ctrl_id, 0, TRUE);
    
    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
}

/*****************************************************************************/
//  Description : OpenPackListWin
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OpenPackListWin(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id
)
{
    //creat list control
    CreatePackListCtrl(win_id, ctrl_id);
    
    //open waiting win
    //MMK_SendMsg(win_id, MSG_IKB_LOAD_PACK_DATA_REQ, PNULL);
}

/*****************************************************************************/
//  Description : StartLoadPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPack(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    BOOLEAN is_find_dir = FALSE;
    wchar pack_dir[MMIIKB_PACK_DIR_LEN] = {0};
    uint16 pack_len = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T file_filter = {"*.in"};

    SCI_MEMSET(pack_dir, 0x0, sizeof(pack_dir));
    is_find_dir = MMIIKB_GetCurBaseDir(pack_dir, &pack_len);
    if (is_find_dir)
    {
        MMIAPIFMM_SearchFileInPathBySort(
            pack_dir,
            pack_len,
            &file_filter,
            TRUE,
            FUNC_FIND_FILE,
            FILEARRAY_SORT_TIME_DESCEND,
            MMIIKB_GetAllPackArray(),
            MMIIKB_WAIT_WIN_ID,
            MSG_IKB_GET_ALL_PACK_DATA_CNF
            );

        //set softkey
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);

        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
    }

    return (result);
}

/*****************************************************************************/
//  Description : FinishLoadPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPack(MMI_WIN_ID_T win_id)
{
    uint16 cur_index = 0;
    uint32 num = 0;

    num = MMIIKB_GetAllPackNum();

    //cur_index 需要决定当前第一个

    //need update list
    MMK_PostMsg(MMIIKB_RESTORE_CHILD_WIN_ID, MSG_IKB_UPDATE_PACK_LIST, 
        &cur_index, sizeof(uint16));

    MMK_CloseWin(win_id);
}

/*****************************************************************************/
//  Description : HandleLoadPackWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPackWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = StartLoadPack(win_id, msg_id, param);
        break;

    case MSG_IKB_GET_ALL_PACK_DATA_CNF:
        FinishLoadPack(win_id);
        break;

    case MSG_APP_CANCEL:
        MMIAPIFMM_SearchFileStop();
        break;

    case MSG_APP_RED:
    case MSG_APP_FLIP:
        MMIAPIFMM_SearchFileStop();
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : UpdatePackList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdatePackList(
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T ctrl_id,
    uint16 cur_index
)
{
    uint16 i = 0;
    uint16 total_num = MMIIKB_GetAllPackNum();
    GUILIST_ITEM_T item = {0};

    //set max item
    GUILIST_SetMaxItem(ctrl_id, total_num, TRUE);

    if (0 < total_num)
    {
        //set title index
        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);

        //set item
        item.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT;
        for (i = 0; i < total_num; i++)
        {
            GUILIST_AppendItem(ctrl_id, &item);
        }

        //set current index
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
    }
}

/*****************************************************************************/
//  Description : AppendPackListItem
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPackListItem(
    BOOLEAN is_text,
    uint16 item_index,
    uint16 content_index,
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN result = FALSE;
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar time_wstr[8] = {0};
    FILEARRAY_DATA_T file_info = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    //MMI_IMAGE_ID_T image_id = IMAGE_SECMENU_ICON_PHONESET_RESET;
    MMIIKB_PACK_INFO_T *pack_buf_ptr = PNULL;
    uint16 pack_name_len = 0;
    uint32 user_data = 0;
    MMI_TEXT_ID_T text_pos_id = 0;
    MMI_TEXT_ID_T text_arr_id[3]=
    {
#ifndef DUAL_TCARD_SUPPORT
        TXT_COMMON_SD_CARD,
#else
        TXT_COMMON_SD_CARD0,
#endif
        TXT_COMMON_SD_CARD1,
        TXT_IKB_SERVER,
    };

    //get picture info
    if (MMIIKB_GetPackFileInfo(item_index, &file_info))
    {
        pack_buf_ptr = (MMIIKB_PACK_INFO_T *)SCI_ALLOCA(
            sizeof(MMIIKB_PACK_INFO_T));
        if (PNULL == pack_buf_ptr)
        {
            return result;
        }
        SCI_MEMSET(pack_buf_ptr, 0x0, sizeof(MMIIKB_PACK_INFO_T));

        if (MMIIKB_ReadPackFileInfo(file_info.filename, file_info.name_len,
            pack_buf_ptr))
        {
            if(MMIAPICOM_Wstrcmp(pack_buf_ptr->mod_name, MMIIKB_MODU_NAME))
            {
                return result;
            }
            //set softkey
            item_data.softkey_id[0] = STXT_OPTION;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;

            //text1 file name
            //item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            //item_data.item_content[0].item_data.image_id = image_id;

            pack_name_len = MMIAPICOM_Wstrlen(pack_buf_ptr->pack_name);
            
            //text1,file name date
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = 
                pack_buf_ptr->pack_name;
            item_data.item_content[0].item_data.text_buffer.wstr_len = 
                MIN(pack_name_len, 10);
            
            //text2 file name time
            SCI_MEMSET(time_wstr, 0x0, sizeof(time_wstr));
            MMIAPICOM_Wstrncpy(time_wstr, &(pack_buf_ptr->pack_name[11]), 
                pack_name_len - 11);
            time_wstr[2] = ':';
            time_wstr[5] = ':';
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = 
                time_wstr;
            item_data.item_content[1].item_data.text_buffer.wstr_len = 
                (uint16)MMIAPICOM_Wstrlen(
                item_data.item_content[1].item_data.text_buffer.wstr_ptr);

            //text3 file name path
            if (pack_buf_ptr->pack_dir[0] == 'E'
                || pack_buf_ptr->pack_dir[0] == 'e')
            {
                text_pos_id = text_arr_id[0];
            }
            else if (pack_buf_ptr->pack_dir[0] == 'F'
                || pack_buf_ptr->pack_dir[0] == 'f')
            {
                text_pos_id = text_arr_id[1];
            }
            else
            {
                text_pos_id = text_arr_id[2];
            }
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[2].item_data.text_id = text_pos_id;

            //text4, file size
            SCI_MEMSET(temp_wstr, 0x0, sizeof(temp_wstr));
            item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            
            MMIAPIFMM_GetFileSizeString(pack_buf_ptr->size, temp_wstr, 
                GUILIST_STRING_MAX_NUM, FALSE);
            
            item_data.item_content[3].item_data.text_buffer.wstr_ptr = 
                temp_wstr;
            item_data.item_content[3].item_data.text_buffer.wstr_len = 
                (uint16)MMIAPICOM_Wstrlen(
                item_data.item_content[3].item_data.text_buffer.wstr_ptr);
            
            //SCI_TRACE_LOW:"[MMIIKB] AppendPackListItem ctrl_id=%d, item_index=%d, types=%d, ptr=%d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3526_112_2_18_2_24_41_179,(uint8*)"dddd", ctrl_id, item_index,pack_buf_ptr->types, pack_buf_ptr->types);
            
            user_data = (uint32)(pack_buf_ptr->types);            
            GUILIST_SetItemUserData(ctrl_id, item_index, &user_data);
            
            if (is_text)
            {
                //add item text etc.
                result = GUILIST_SetItemData(ctrl_id, &item_data, item_index);
            }
            else
            {
                result = GUILIST_SetItemContent(ctrl_id,
                    &item_data.item_content[content_index],
                    item_index, content_index);
            }
        }

        SCI_FREE(pack_buf_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : OnClickRestoreList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickRestoreList(
    MMI_CTRL_ID_T ctrl_id
)
{
    uint16 item_index = 0;

    if (0 < MMIIKB_GetAllPackNum())
    {
        item_index = GUILIST_GetCurItemIndex(ctrl_id);
        
        MMK_CreateWin((uint32 *)MMIIKB_RESTORE_ITEM_SEL_WIN_TAB, 
            (ADD_DATA)item_index);
    }
    
    return;
}

/*****************************************************************************/
//  Description : DeleteSelectedPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteSelectedPack(uint32 item_index)
{
    BOOLEAN result = FALSE;
    FILEARRAY_DATA_T file_info = {0};
    
    if (MMIIKB_GetPackFileInfo(item_index, &file_info))
    {
        MMI_STRING_T cur_file_s = {0};
        wchar *full_path_ptr = PNULL;
        uint16 path_len = 0;

        cur_file_s.wstr_ptr = SCI_ALLOC_APP(
            (MMIIKB_PACK_NAME_LEN + 1)* sizeof(wchar));
        SCI_MEMSET(cur_file_s.wstr_ptr, 0, (
            (MMIIKB_PACK_NAME_LEN + 1)* sizeof(wchar)));
        full_path_ptr = SCI_ALLOC_APP(
            (MMIIKB_CON_PATH_LEN + 1)* sizeof(wchar));
        SCI_MEMSET(full_path_ptr, 0, (
            (MMIIKB_CON_PATH_LEN + 1)* sizeof(wchar)));

        MMIAPICOM_Wstrcpy(full_path_ptr, file_info.filename);
        
        path_len = MMIAPICOM_Wstrlen(full_path_ptr);

        MMIAPIFMM_SplitLastName(full_path_ptr, &path_len, 
            cur_file_s.wstr_ptr, &cur_file_s.wstr_len);
        
        result = DeletePhysicalDir(full_path_ptr, path_len);

        if (PNULL != cur_file_s.wstr_ptr)
        {
            SCI_FREE(cur_file_s.wstr_ptr);
            cur_file_s.wstr_ptr = PNULL;
        }

        SCI_FREE(full_path_ptr);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : DeleteAllPack
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteAllPack(void)
{
    BOOLEAN result = FALSE;
    uint16 num = 0;
    uint16 i = 0;
    MMI_CTRL_ID_T list_ctrl_id = MMIIKB_RESTORE_LIST_CTRL_ID;
    num = GUILIST_GetTotalItemNum(list_ctrl_id);
    
    if (num > 0)
    {
        result = TRUE;
        
        for (i = 0; i < num && result; i++)
        {
            result = DeleteSelectedPack(i);
        }
    }
    else
    {
        result = FALSE;
    }    
    
    return result;
}

/*****************************************************************************/
//  Description : GetPackContentNumber
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPackContentNumber(
    const uint16 *pack_path_ptr,
    uint16 pack_path_len,
    MMIIKB_TYPE_E type
)
{
    uint32 total = 0;
    wchar * pack_dir_ptr = PNULL;
    uint16 path_len = 0;
    wchar *str_ibphone_name = PNULL;
    uint16 str_name_len = 0;

    if (PNULL == pack_path_ptr
        || 0 == pack_path_len
        || MMIIKB_TYPE_MAX <= type)
    {
        return total;
    }

    pack_dir_ptr = (wchar *)SCI_ALLOCA(sizeof(wchar)*(pack_path_len+1));
    if (PNULL == pack_dir_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] GetPackContentNumber no memory."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3671_112_2_18_2_24_41_180,(uint8*)"");
        return total;
    }
    SCI_MEMSET(pack_dir_ptr, 0x0, sizeof(wchar)*(pack_path_len+1));
    
    str_ibphone_name = (wchar *)SCI_ALLOCA(sizeof(wchar)*(pack_path_len+1));
    if (PNULL == str_ibphone_name)
    {
        //SCI_TRACE_LOW:"[MMIIKB] GetPackContentNumber no memory."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3679_112_2_18_2_24_41_181,(uint8*)"");
        SCI_FREE(pack_dir_ptr);
        return total;
    }
    SCI_MEMSET(str_ibphone_name, 0x0, sizeof(wchar)*(pack_path_len+1));
    
    MMIAPICOM_Wstrcpy(pack_dir_ptr, pack_path_ptr);
        
    path_len = MMIAPICOM_Wstrlen(pack_dir_ptr);

    MMIAPIFMM_SplitLastName(pack_dir_ptr, &path_len, 
        str_ibphone_name, &str_name_len);
    
    SCI_FREE(str_ibphone_name);
    str_ibphone_name = PNULL;

    total = MMIIKB_GetPackContentNumber(pack_dir_ptr, path_len, type);

    SCI_FREE(pack_dir_ptr);

    return (total);
}

/*****************************************************************************/
//  Description : InitRestoreSelItemList
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitRestoreSelItemList(
    uint16 index,
    MMIIKB_UI_SEL_ITEM_LIST_T *ui_list_head_ptr,
    uint16 list_num
)
{
    uint16 i = 0;
    MMIIKB_UI_SEL_ITEM_LIST_T *cur_list_ptr = PNULL;
    MMIIKB_TYPE_T types = 0;
    FILEARRAY_DATA_T file_info = {0};
    MMIIKB_PACK_INFO_T * pack_info_ptr = PNULL;
    BOOLEAN result = FALSE;
    uint32 total_size = 0;
    MMIIKB_TYPE_E type = MMIIKB_TYPE_MAX;

    if (PNULL == ui_list_head_ptr
        || 0 == list_num)
    {
        //SCI_TRACE_LOW:"[MMIIKB] InitRestoreSelItemList param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_3724_112_2_18_2_24_41_182,(uint8*)"");
        return result;
    }

    if (MMIIKB_GetPackFileInfo(index, &file_info))
    {
        pack_info_ptr = (MMIIKB_PACK_INFO_T *)SCI_ALLOCA(
            sizeof(MMIIKB_PACK_INFO_T));
        if (PNULL == pack_info_ptr)
        {
            return result;
        }
        SCI_MEMSET(pack_info_ptr, 0x0, sizeof(MMIIKB_PACK_INFO_T));

        if (MMIIKB_ReadPackFileInfo(file_info.filename,
            file_info.name_len, pack_info_ptr))
        {
            types = pack_info_ptr->types;
        }

        SCI_FREE(pack_info_ptr);
    }

    //第一行全选不取，从第二行开始
    for (i = 0; i < list_num; i++)
    {
        cur_list_ptr = ui_list_head_ptr + i;

        if (0 != (types & (0x01 << i)))
        {
            char c_text[MMIIKB_UI_TOTAL_APP_BUF_LEN] = {0};
            uint16 text_len = 0;
            MMI_STRING_T text_str = {0};
    
            type = (MMIIKB_TYPE_E)i;
            total_size = GetPackContentNumber(file_info.filename, 
                file_info.name_len, type);

            cur_list_ptr->total = total_size;
            SCI_MEMSET(cur_list_ptr->text2_buf, 0x0, 
                sizeof(cur_list_ptr->text2_buf));            

            if (MMIIKB_TYPE_CALLLOG == type
                || MMIIKB_TYPE_PHONEBOOK == type
                || MMIIKB_TYPE_SMS == type
                || MMIIKB_TYPE_SCHEDULE == type)
            {
                SCI_MEMSET(c_text, 0x0, sizeof(c_text));
                SCI_MEMSET(cur_list_ptr->text2_buf, 0x0, 
                    sizeof(cur_list_ptr->text2_buf));
                
                sprintf(c_text, "%d", cur_list_ptr->total);
                MMIAPICOM_StrToWstr((uint8 *)c_text, cur_list_ptr->text2_buf);
                text_len = MMIAPICOM_Wstrlen(cur_list_ptr->text2_buf);
                MMI_GetLabelTextByLang(TXT_ITEMS, &text_str);
                MMIAPICOM_Wstrncpy(cur_list_ptr->text2_buf + text_len, 
                    text_str.wstr_ptr, MIN(text_str.wstr_len, 
                    MMIIKB_UI_TOTAL_APP_BUF_LEN-text_len));
            }
            
            text_len = MMIAPICOM_Wstrlen(cur_list_ptr->text2_buf);
            cur_list_ptr->text2_len = text_len;
            
            cur_list_ptr->is_check = TRUE;
            cur_list_ptr->is_gray = FALSE;
        }
        else
        {
            cur_list_ptr->is_check = FALSE;
            cur_list_ptr->is_gray = TRUE;
        }
    }

    result = TRUE;

    return result;
}

/*****************************************************************************/
//  Description : OnClickRestoreSelSoftkey
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnClickRestoreSelSoftkey(uint16 item_index)
{
    uint16 types = 0;
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    
    types = GetListItemSelectValues(
        s_ui_restore_list, ARR_SIZE(s_ui_restore_list));

    if (PreCheckRestoreItems(item_index, types))
    {
        MMIIKB_CHECK_PSW_WIN_DATA_T *psw_win_data_ptr = PNULL;
        BOOLEAN is_free = TRUE;
        psw_win_data_ptr = 
            (MMIIKB_CHECK_PSW_WIN_DATA_T *)SCI_ALLOCA(
            sizeof(MMIIKB_CHECK_PSW_WIN_DATA_T));
        if (PNULL != psw_win_data_ptr)
        {
            SCI_MEMSET(psw_win_data_ptr, 0x0, 
                sizeof(MMIIKB_CHECK_PSW_WIN_DATA_T));

            error_code = GetPackPswInfo(
                item_index, &psw_win_data_ptr->psw_info_t);
            psw_win_data_ptr->intem_index = item_index;
            psw_win_data_ptr->types = types;                            

            if (MMIIKB_STATUS_SUCCESS == error_code)
            {
                if (psw_win_data_ptr->psw_info_t.is_password)
                {
                    is_free = FALSE;
                    //打开设置密码窗口
                    MMK_CreateWin(
                        (uint32*)MMIIKB_ENTER_RES_PSW_WIN_TAB, 
                        (ADD_DATA)psw_win_data_ptr);
                }
                else
                {
                    SCI_FREE(psw_win_data_ptr);
                    RestoreSelItems(item_index, types);
                }
            }

            if (is_free)
            {
                SCI_FREE(psw_win_data_ptr);
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Description : HandleRestoreItemSelWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreItemSelWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIIKB_RESTORE_ITEM_SEL_CTRL_ID;
    uint16 item_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    MMI_CTRL_ID_T btn_ctrl_id = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_SetMaxItem(ctrl_id, ARR_SIZE(s_ui_restore_list), FALSE);
            GUILIST_SetMaxSelectedItem(ctrl_id, ARR_SIZE(s_ui_restore_list));

            InitRestoreSelItemList(item_index, s_ui_restore_list, 
                ARR_SIZE(s_ui_restore_list));
    
            AppendALLListItemCtrl(ctrl_id, s_ui_restore_list, 
                ARR_SIZE(s_ui_restore_list));
            
#ifdef MMI_PDA_SUPPORT
            {
                GUIFORM_CHILD_HEIGHT_T child_height = {0};
                child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
                GUILIST_PermitBorder(ctrl_id, FALSE);
                GUIFORM_IsSlide(MMIIKB_RESTORE_ITEM_SEL_FORM_CTRL_ID, FALSE);
                GUIFORM_SetChildHeight(MMIIKB_RESTORE_ITEM_SEL_FORM_CTRL_ID, 
                    ctrl_id, &child_height);
                GUIFORM_PermitChildBg(MMIIKB_RESTORE_ITEM_SEL_FORM_CTRL_ID, 
                    FALSE);
                GUILIST_SetUserBg(ctrl_id, FALSE);
            }
#endif            
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
#if defined(MMI_PDA_SUPPORT)
    case MSG_APP_MENU:
#endif
        {
            OnClickRestoreSelSoftkey(item_index);
        }
        break;
        
    //case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:        
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT)
        if (MSG_CTL_PENOK == msg_id && PNULL != param)
        {
            btn_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                MMK_CloseWin(win_id);
                break;
            }
            else if (btn_ctrl_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                OnClickRestoreSelSoftkey(item_index);   
                break;
            }
        }
#endif
        OnClickCheckListItem(ctrl_id, s_ui_restore_list, 
            ARR_SIZE(s_ui_restore_list));
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : handle delete selected package confirm message
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void HandleDeletePackCnf(
    BOOLEAN is_success,             //in:delete is success
    uint16 pack_index,              //in:
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T list_ctrl_id      //in:
)
{
    uint16 cur_index = 0;
    
    if (is_success)
    {
        //update list
        GUILIST_RemoveItem(list_ctrl_id, pack_index);

        //update picture array
        MMIAPIFILEARRAY_Delete(MMIIKB_GetAllPackArray(), pack_index);
        
        /*成功不提示
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SUCCESS, TXT_NULL,
            IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        */

        cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        MMK_PostMsg(win_id, MSG_IKB_UPDATE_PACK_LIST, 
            &cur_index, sizeof(uint16));
    }
    else
    {
        //prompt fail
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
            IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : HandleRestoreListLongOkOptMenuWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理长按还原列表，弹出菜单选项窗口
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreListLongOkOptMenuWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIIKB_RESTORE_LIST_LONGOK_OPT_CTRL_ID;
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = MMIIKB_RESTORE_LIST_LONGOK_OPT_MENU;
    uint16 item_index = 0;
    MMI_WIN_ID_T query_win_id = MMIIKB_DEL_QUERY_WIN_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T title_str = {0};
            MMI_GetLabelTextByLang(STXT_OPTION, &title_str);
            GUIMENU_SetMenuTitle(&title_str, ctrl_id);
        }
        break;
        
    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        item_index = (uint32)MMK_GetWinAddDataPtr(win_id);
        
        //SCI_TRACE_LOW:"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg menu_id=%x, item_index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_4022_112_2_18_2_24_42_183,(uint8*)"dd", menu_id, item_index);
        
        switch (menu_id)
        {
        case MMIIKB_ID_MENU_DELETE_PACK:
            {
                MMIIKB_DEL_PACK_PARAM_T *del_param_ptr 
                    = (MMIIKB_DEL_PACK_PARAM_T *)SCI_ALLOCA(
                    sizeof(MMIIKB_DEL_PACK_PARAM_T));
                if (PNULL == del_param_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_4033_112_2_18_2_24_42_184,(uint8*)"");
                    return result;
                }
                SCI_MEMSET(del_param_ptr, 0x0, sizeof(MMIIKB_DEL_PACK_PARAM_T));
    
                del_param_ptr->del_type = 1;
                del_param_ptr->del_index = item_index;
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETE,
                    IMAGE_PUBWIN_QUERY, &query_win_id, 
                    HandlePackDelQueryWin, (uint32)del_param_ptr);
            }
            break;
     
        default:
            break;
        } 
        MMK_CloseWin(win_id);
        break;        
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif

/*****************************************************************************/
//  Description : handle package delete msg
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePackDelQueryWin(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    uint32 time_out = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T wait_win_id = MMIIKB_DEL_WAIT_WIN_ID;
    MMI_WIN_ID_T query_win_id = MMIIKB_DEL_QUERY_WIN_ID;
    MMI_WIN_ID_T restore_win_id = MMIIKB_RESTORE_CHILD_WIN_ID;
    MMI_CTRL_ID_T list_ctrl_id = MMIIKB_RESTORE_LIST_CTRL_ID;
    BOOLEAN is_delete = FALSE;
    uint32 del_type = 1;
    uint16 item_index = 0;
    MMIIKB_DEL_PACK_PARAM_T *del_param_ptr = PNULL;    
    MMIPUB_INFO_T *win_info_ptr = PNULL;
    
    win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);    
    if (PNULL == win_info_ptr || 0 == win_info_ptr->user_data)
    {
        return MMI_RESULT_FALSE;
    }    
    del_param_ptr = (MMIIKB_DEL_PACK_PARAM_T *)(win_info_ptr->user_data);
    if (PNULL == del_param_ptr)
    {
        return MMI_RESULT_FALSE;        
    }
    
    switch (msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
        {
            del_type = del_param_ptr->del_type;
            item_index = del_param_ptr->del_index;
            
            MMIPUB_CloseQuerytWin(&query_win_id);
            
            if (1 == del_type)
            {
                //open wainting
                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING, TXT_NULL,
                    IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, 
                    MMIPUB_SOFTKEY_NONE, PNULL);

                //del one package
                GUILIST_GetCurItemIndex(list_ctrl_id);
                is_delete = DeleteSelectedPack(item_index);
                HandleDeletePackCnf(is_delete, item_index, 
                    restore_win_id, list_ctrl_id);
                
                //close wait win
                MMK_CloseWin(wait_win_id);
            }
            else if (2 == del_type)
            {
                //open wainting
                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING, TXT_NULL,
                    IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, 
                    MMIPUB_SOFTKEY_NONE, PNULL);
                
                //del all package
                is_delete = DeleteAllPack();
                if (is_delete)
                {
                    /*成功不提示
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SUCCESS, TXT_NULL,
                        IMAGE_PUBWIN_FAIL, PNULL, PNULL, 
                        MMIPUB_SOFTKEY_ONE, PNULL);
                    */
                    
                    MMK_PostMsg(MMIIKB_RESTORE_CHILD_WIN_ID, 
                        MSG_IKB_LOAD_PACK_DATA_REQ, PNULL, PNULL);
                }
                else
                {
                    //prompt fail
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL,
                        IMAGE_PUBWIN_FAIL, PNULL, PNULL, 
                        MMIPUB_SOFTKEY_ONE, PNULL);
                }
                
                //close wait win
                MMK_CloseWin(wait_win_id);
            }
        }
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        SCI_FREE(del_param_ptr);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : HandleRestoreListOptMenuWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理option列表，弹出菜单选项窗口
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreListOptMenuWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIIKB_RESTORE_LIST_OPT_CTRL_ID;
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = MMIIKB_RESTORE_LIST_OPT_MENU;
    MMI_WIN_ID_T query_win_id = MMIIKB_DEL_QUERY_WIN_ID;
    uint16 item_index = 0;

    switch (msg_id)
    {    
    case MSG_OPEN_WINDOW:
        if(0==GUILIST_GetTotalItemNum(MMIIKB_RESTORE_LIST_CTRL_ID))
        {
            GUIMENU_SetItemGrayed(MMIIKB_RESTORE_LIST_OPT_CTRL_ID,
                group_id, MMIIKB_ID_MENU_LEFT_OPT_RESTORE, TRUE);

            GUIMENU_SetItemGrayed(MMIIKB_RESTORE_LIST_OPT_CTRL_ID,
                group_id, MMIIKB_ID_MENU_LEFT_OPT_DELETE_PACK, TRUE);
        
            GUIMENU_SetItemGrayed(MMIIKB_RESTORE_LIST_OPT_CTRL_ID,
                group_id, MMIIKB_ID_MENU_LEFT_OPT_DELETE_ALL_PACK, TRUE);
        }
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        item_index = (uint32)MMK_GetWinAddDataPtr(win_id);
        
        //SCI_TRACE_LOW:"[MMIIKB] HandleRestoreListOptMenuWinMsg menu_id=%x, item_index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_4200_112_2_18_2_24_42_185,(uint8*)"dd", menu_id, item_index);
        
        switch (menu_id)
        {
        case MMIIKB_ID_MENU_LEFT_OPT_RESTORE:
            MMK_CreateWin((uint32 *)MMIIKB_RESTORE_ITEM_SEL_WIN_TAB, 
                (ADD_DATA)item_index);
            break;
            
        case MMIIKB_ID_MENU_LEFT_OPT_DELETE_PACK:
            {
                MMIIKB_DEL_PACK_PARAM_T *del_param_ptr 
                    = (MMIIKB_DEL_PACK_PARAM_T *)SCI_ALLOCA(
                    sizeof(MMIIKB_DEL_PACK_PARAM_T));
                if (PNULL == del_param_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_4216_112_2_18_2_24_42_186,(uint8*)"");
                    return result;
                }
                SCI_MEMSET(del_param_ptr, 0x0, sizeof(MMIIKB_DEL_PACK_PARAM_T));
    
                del_param_ptr->del_type = 1;
                del_param_ptr->del_index = item_index;
                
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETE,
                    IMAGE_PUBWIN_QUERY, &query_win_id, 
                    HandlePackDelQueryWin, (uint32)del_param_ptr);
            }
            break;
            
        case MMIIKB_ID_MENU_LEFT_OPT_DELETE_ALL_PACK:
            {
                MMIIKB_DEL_PACK_PARAM_T *del_param_ptr 
                    = (MMIIKB_DEL_PACK_PARAM_T *)SCI_ALLOCA(
                    sizeof(MMIIKB_DEL_PACK_PARAM_T));
                if (PNULL == del_param_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIIKB] HandleRestoreListLongOkOptMenuWinMsg no memory."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_4237_112_2_18_2_24_42_187,(uint8*)"");
                    return result;
                }
                SCI_MEMSET(del_param_ptr, 0x0, sizeof(MMIIKB_DEL_PACK_PARAM_T));
    
                del_param_ptr->del_type = 2;
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID, TXT_DELETE_ALL_QUERY,
                    IMAGE_PUBWIN_QUERY, &query_win_id, 
                    HandlePackDelQueryWin, (uint32)del_param_ptr);
            }
            break;
     
        default:
            break;
        } 
        MMK_CloseWin(win_id);
        break;        
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleRestoreChildWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRestoreChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl_id = MMIIKB_RESTORE_LIST_CTRL_ID; 
    uint32 time_out = 0;
    MMI_WIN_ID_T wait_win_id = MMIIKB_WAIT_WIN_ID;
    GUILIST_NEED_ITEM_DATA_T *item_data_ptr = PNULL; 
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    uint16 item_index = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            OpenPackListWin(win_id, list_ctrl_id);
            
#ifdef MMI_PDA_SUPPORT
            {
                GUIFORM_CHILD_HEIGHT_T child_height = {0};
                
                //GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, 
                //    TITLE_BUTTON_NORMAL, FALSE);
            
                child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
                GUILIST_PermitBorder(list_ctrl_id, FALSE);
                GUIFORM_IsSlide(MMIIKB_RESTORE_LIST_FORM_CTRL_ID, FALSE);
                GUIFORM_SetChildHeight(MMIIKB_RESTORE_LIST_FORM_CTRL_ID, 
                    list_ctrl_id, &child_height);
                GUIFORM_PermitChildBg(MMIIKB_RESTORE_LIST_FORM_CTRL_ID, FALSE);
                GUILIST_SetUserBg(list_ctrl_id, FALSE);
            }
#endif
        }
        break;

    case MSG_CTL_TAB_SWITCH:
        s_cur_ui_tab_type = MMIIKB_UI_TAB_RESTORE;
        MMK_PostMsg(win_id, MSG_IKB_LOAD_PACK_DATA_REQ, PNULL, 0);
        break;

    case MSG_IKB_LOAD_PACK_DATA_REQ:
        {
            //load file
            MMIIKB_DestroyAllPackArray();
            MMIIKB_CreateAllPackArray();
            
            //clear pack list
            ClearPackList(win_id, list_ctrl_id);

            MMIPUB_OpenAlertWinByTextId(&time_out, TXT_COMMON_WAITING, TXT_NULL, 
                IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, 
                HandleLoadPackWinMsg);
        }
        break;

    case MSG_IKB_UPDATE_PACK_LIST:
        UpdatePackList(win_id, list_ctrl_id, *(uint16*)param);
        GUIWIN_SetTitleTextId(MMIIKB_MAIN_WIN_ID, TXT_IKB_RESTORE, TRUE);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //add item text etc.
        item_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
        if (PNULL != item_data_ptr)
        {        
            AppendPackListItem(TRUE, item_data_ptr->item_index, 
                0, list_ctrl_id);
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        //add item anim
        item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
        if (PNULL != item_content_ptr)
        {
            AppendPackListItem(FALSE, item_content_ptr->item_index, 
                item_content_ptr->item_content_index, list_ctrl_id);
        }
        break;

    case MSG_CTL_LIST_MARK_ITEM:
        //get mark num
        if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0))
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    if(0!=GUILIST_GetTotalItemNum(list_ctrl_id))
    {

#ifdef MMI_PDA_SUPPORT
            OnClickRestoreList(list_ctrl_id);
#else
        
            item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            MMK_CreateWin((uint32*)MMIIKB_RESTORE_LIST_OPT_POPMENU_WIN_TAB, 
                (ADD_DATA)item_index);
#endif
    }
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        MMK_CreateWin((uint32*)MMIIKB_RESTORE_LIST_LONGOK_POPMENU_WIN_TAB, 
            (ADD_DATA)item_index);
        break;
#endif
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseParentWin(MMIIKB_MAIN_WIN_ID);
        break;
    case MSG_CLOSE_WINDOW:
        MMIIKB_DestroyAllPackArray();
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}


//setting list
/*****************************************************************************/
//  Description : InitSettingListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitSettingListCtrl(
    MMI_CTRL_ID_T ctrl_id,
    MMIIKB_CONFIG_T * cfg_ptr
)
{
    GUILIST_ITEM_T item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_TEXT_ID_T txt_id2 = 0;

    if (PNULL == cfg_ptr)
    {
        //SCI_TRACE_LOW:"[MMIIKB] InitSettingListCtrl param is null."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIKEYBACKUP_WINTAB_4417_112_2_18_2_24_43_188,(uint8*)"");
        return;
    }
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    //item_t.user_data = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_IKB_CUR_BACKUP_POSITION;
    
    //text 2
    if (MMIIKB_STORE_POS_SD_1 == cfg_ptr->store_pos)
    {
#ifndef DUAL_TCARD_SUPPORT
        txt_id2 = TXT_COMMON_SD_CARD;
#else
        txt_id2 = TXT_COMMON_SD_CARD0;
#endif
    }
    else if (MMIIKB_STORE_POS_SD_2 == cfg_ptr->store_pos)
    {
        txt_id2 = TXT_COMMON_SD_CARD1;
    }
    else if (MMIIKB_STORE_POS_SERVER == cfg_ptr->store_pos)
    {
        txt_id2 = TXT_IKB_SERVER;
    }
    
    if (0 != txt_id2)
    {
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = txt_id2;
    }
    
    //anim 3
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = IMAGE_COMMON_RIGHT_ARROW;
    GUILIST_AppendItem(ctrl_id, &item_t);

    //encrypted data
    SCI_MEMSET(&item_t, 0x0, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x0, sizeof(item_data));
    
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    //item_t.user_data = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_IKB_ENCRYPT_DATA;
    
    //text 2    
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data.text_id = TXT_IKB_ENCRYPT_DATA_TIPS;
    
    //anim 3
    item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = IMAGE_COMMON_RIGHT_ARROW;
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : InitBackupPositionListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitBackupPositionListCtrl(void)
{
    uint16 i = 0;
    MMI_CTRL_ID_T ctrl_id = MMIIKB_BACKUP_POS_LIST_CTRL_ID;
    MMIIKB_POS_TEXT_T text_arr_id[]=
    {
#ifndef DUAL_TCARD_SUPPORT
        {TXT_COMMON_SD_CARD, MMIIKB_STORE_POS_SD_1},
#else
		{TXT_COMMON_SD_CARD0, MMIIKB_STORE_POS_SD_1},
        {TXT_COMMON_SD_CARD1, MMIIKB_STORE_POS_SD_2},
#endif
#if 0//云端备份
        {TXT_IKB_SERVER, MMIIKB_STORE_POS_SERVER}
#endif
    };
    
    uint16 list_num = ARR_SIZE(text_arr_id);

    GUILIST_SetMaxItem(ctrl_id, list_num, FALSE);   

    for(i = 0; i < list_num; i++)
    {
        MMIIKB_AppendRadioListItem(ctrl_id, text_arr_id[i].txt_id, 
            (uint32)text_arr_id[i].pos);
    }

    if (!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), 
        MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)))
    {
        GUILIST_SetItemGray(ctrl_id, 0, TRUE);
    }
#ifdef DUAL_TCARD_SUPPORT
    if (!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1), 
        MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)))
    {
        GUILIST_SetItemGray(ctrl_id, 1, TRUE);
    }
#endif    

    #if 0
    GUILIST_SetItemGray(ctrl_id, 2, TRUE);
    #endif
}

/*****************************************************************************/
//  Description : SetBackupPosData
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetBackupPosData(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = 0;
    MMIIKB_CONFIG_T * cfg_ptr = MMIAPIIKB_GetConfigInfo();
    
    index = GUILIST_GetCurItemIndex(ctrl_id);
    cfg_ptr->store_pos = (MMIIKB_STORE_POS_E)(index+1);

    //save config nv values.
    MMIIKB_SetConfigNVInfo(cfg_ptr);
}

/*****************************************************************************/
//  Description : HandleBackupPosWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBackupPosWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIIKB_BACKUP_POS_LIST_CTRL_ID;
    uint16 index = 0;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIIKB_CONFIG_T * cfg_ptr = MMIAPIIKB_GetConfigInfo();
            
            InitBackupPositionListCtrl();

            if (MMIIKB_STORE_POS_SD_1 == cfg_ptr->store_pos)
            {
                index = 0;
            }
            else if (MMIIKB_STORE_POS_SD_2 == cfg_ptr->store_pos)
            {
                index = 1;
            }
            else if (MMIIKB_STORE_POS_SERVER == cfg_ptr->store_pos)
            {
                index = 2;
            }
            
            GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id, index);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef MMI_PDA_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
#endif
        {
            SetBackupPosData(ctrl_id);
    
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : InitAppItemDataListCtrl
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InitAppItemDataListCtrl(MMI_CTRL_ID_T ctrl_id)
{
    uint16 i = 0;
    uint16 list_num = ARR_SIZE(s_ui_backup_list);

    GUILIST_SetMaxItem(ctrl_id, list_num, FALSE);  
    GUILIST_SetMaxSelectedItem(ctrl_id, list_num);

    for(i = 0; i < list_num; i++)
    {
        MMIIKB_AppendCheckBoxListItem(ctrl_id, 
            s_ui_backup_list[i].text_id1, (uint32)0);
    }
}

/*****************************************************************************/
//  Description : SetEncryptData
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void SetEncryptData(MMI_CTRL_ID_T ctrl_id)
{
    uint16 i = 0;
    uint16  selected_num = 0;
    uint16 list_num = ARR_SIZE(s_ui_backup_list);
    uint16  selected_item[ARR_SIZE(s_ui_backup_list)] = {0};
    uint16 types = 0;
    MMIIKB_CONFIG_T * cfg_ptr = MMIAPIIKB_GetConfigInfo();
            
    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id, 
        selected_item, list_num);

    for (i = 0; i < selected_num; i++)
    {
        types |= (0x1 << selected_item[i]);
    }

    //set types
    cfg_ptr->types = types;

    //save config nv values.
    MMIIKB_SetConfigNVInfo(cfg_ptr);
}

/*****************************************************************************/
//  Description : HandleEncryptDataWinMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEncryptDataWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIIKB_ENCRYPT_DATA_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 i = 0;
            MMIIKB_CONFIG_T * cfg_ptr = MMIAPIIKB_GetConfigInfo();
            uint16 list_num = ARR_SIZE(s_ui_backup_list);
            
            InitAppItemDataListCtrl(ctrl_id);
            
            for (i = 0; i < MMIIKB_TYPE_MAX && i < list_num; i++)
            {
                if (0x0 != (cfg_ptr->types & (0x1 << i)))
                {
                    GUILIST_SetSelectedItem(ctrl_id, i, TRUE);
                }
            }
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        SetEncryptData(ctrl_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : HandleSettingChildWinMsg
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSettingChildWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T list_ctrl_id = MMIIKB_SETTING_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_SetMaxItem(list_ctrl_id, 
                MMIIKB_UI_SETTING_LIST_NUM_MAX, FALSE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id, 
                MMIIKB_UI_SETTING_LIST_NUM_MAX);
            
            InitSettingListCtrl(list_ctrl_id, MMIAPIIKB_GetConfigInfo());
            
#ifdef MMI_PDA_SUPPORT
            {
                GUIFORM_CHILD_HEIGHT_T child_height = {0};
                child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
                GUILIST_PermitBorder(list_ctrl_id, FALSE);
                GUIFORM_IsSlide(MMIIKB_SETTING_LIST_FORM_CTRL_ID, FALSE);
                GUIFORM_SetChildHeight(MMIIKB_SETTING_LIST_FORM_CTRL_ID, 
                    list_ctrl_id, &child_height);
                GUIFORM_PermitChildBg(MMIIKB_SETTING_LIST_FORM_CTRL_ID, FALSE);
                GUILIST_SetUserBg(list_ctrl_id, FALSE);
            }
#endif
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint16 index = 0;
            index = GUILIST_GetCurItemIndex(list_ctrl_id);

            if (0 == index)
            {
                //打开设置保存路径窗口
                MMK_CreatePubListWin((uint32*)MMIIKB_BACKUP_POS_WIN_TAB, PNULL);
            }
            else if (1 == index)
            {
                //打开设置加密数据窗口
                MMK_CreateWin((uint32*)MMIIKB_ENCRYPT_DATA_WIN_TAB, PNULL);
            }
        }
        break;
    
    case MSG_GET_FOCUS:
        //本页面数据比较简单，先移除再添加
        GUILIST_RemoveAllItems(list_ctrl_id);
        InitSettingListCtrl(list_ctrl_id, MMIAPIIKB_GetConfigInfo());
        break;

    case MSG_CTL_TAB_SWITCH:
        s_cur_ui_tab_type = MMIIKB_UI_TAB_SETTING;
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMIIKB_MAIN_WIN_ID);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}


//version control
/*****************************************************************************/
//  Description : IsVersionLegal
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsVersionLegal(MMIIKB_PACK_INFO_T * pack_info_ptr)
{
    BOOLEAN is_legal = FALSE;
    char ver1_10[MMIIKB_VERSION_NUM_LEN] = {0};
    char ver10 = 0;
    uint8 mod = 0;

    if (PNULL == pack_info_ptr)
    {
        SCI_TRACE_LOW("[MMIIKB] IsVersionLegal param is null.");
        return is_legal;
    }

    SCI_MEMCPY(ver1_10, pack_info_ptr->version, MMIIKB_VERSION_NUM_LEN);
    ver10 = pack_info_ptr->version[MMIIKB_VERSION_NUM_LEN - 1];
    
    //检查大小端
    //最的五个字符肯定是数字，1为49,2为50，只做奇偶就可以检查出来
    //版本信息，奇数是小端，偶数是大商版本
    mod = ver10 % 2;
    
#ifdef CHIP_ENDIAN_LITTLE
    if (1 == mod)
    {
        is_legal = TRUE;
    }
#else
    if (0 == mod)
    {
        is_legal = TRUE;
    }
#endif
    
    if (!is_legal)
    {
        SCI_TRACE_LOW("[MMIIKB] IsVersionLegal endian inlegal version=%s.",
            ver1_10);
        return is_legal;
    }

    //比较版本号，小的则不合法，包取出的版本比现在程序版本小，则不合法
    if (MMIAPICOM_Stricmp(ver1_10, s_str_ikb_version) >= 0)/*lint !e64*/
    {
        is_legal = TRUE;
    }
    else
    {
        is_legal = FALSE;
    }
    
    SCI_TRACE_LOW("[MMIIKB] IsVersionLegal is_legal=%d, version=%s", 
        is_legal, ver1_10);
    
    return is_legal;
}

/*****************************************************************************/
//  Description : AbortCurAction
//  Global resource dependence :
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void AbortCurAction(MMIIKB_TASK_TYPE_E task_type)
{
    MMI_WIN_ID_T win_cancel_id = MMIIKB_CANCEL_WAIT_WIN_ID;
    
    if(MMIIKB_TASK_TYPE_NONE >= task_type
        || MMIIKB_TASK_TYPE_MAX <= task_type)
    {
        SCI_TRACE_LOW("[MMIIKB] AbortCurAction task_type:%d", task_type); 
        return;
    }
        
    if(MMK_IsOpenWin(win_cancel_id))
    {
        MMK_CloseWin(win_cancel_id);
    }
        
    MMIPUB_OpenQueryWinByTextIdEx(
        MMK_GetFirstAppletHandle(),
        TXT_IKB_ABORT_QUERY,
        IMAGE_PUBWIN_QUERY,
        &win_cancel_id,
        HandleCancelActionQueryMsg,
        (uint32)task_type
        );

    return;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(IKEYBACKUP_SUPPORT)



/*Edit by script, ignore 11 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
