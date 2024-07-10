#ifndef _WDP_UPDATE_WINTAB_C_
#define _WDP_UPDATE_WINTAB_C_
#include "mmi_app_im_trc.h"
#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
#include "mmiim_sogou.h"
#include "wdp_update_wintab.h"
#include "wdp_update.h"
#include "mmidisplay_data.h"
#include "guimenu.h"
#include "guilistbox.h"
#include "mmiset_image.h"
#include "mmiim_text.h"
#include "mmipub.h"
#include "mmiidle_export.h"
#include "guitext.h"
#include "mmk_app.h"
#include "mmialarm_export.h"
#include "mmialarm_image.h"
#include "mmiim_nv.h"
#include "wdp_encrpty.h"
#include "wdp_SG_NetWork_Def.h"
#include "mmifmm_export.h"
#include "mmi_image.h"
#include "mmiim_image.h"
#include "jpeg_interface.h"
#include "block_mem.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmiudisk_export.h"
#include "guires.h"

#define ALERT_WIN_DURATION    5000
#define SKIN_FILEPATH_LEN 10
#define MIN_SKIN_FILE_BUF_SIZE (50 * 1024)
#define SG_MAX_MENU_ITEM_COUNT  49 // must <GUIMENU_DYNA_NODE_MAX_NUM

#define SOGOU_TRACE_OUT    SCI_TRACE_LOW
extern uint8 g_sg_save_position;//0:D盘,1:E盘,2:非D非E

// 皮肤列表结构
LOCAL SkinInf *s_skin_ptr = PNULL;
// 皮肤列表文件内容
LOCAL uint8 *s_skin_list_file = PNULL; 
// 获取服务器内容的类型
LOCAL SG_UPDATE_TYPE_E s_update_type = SG_UPDATE_TYPE_NONE;
// 更新设置
PUBLIC SYSTEM_UPDATE_SETTINGS_DATA_T g_sogou_update_settings_data = { 0 };
// 更新频率
PUBLIC VOCABULARY_UPDATE_FREQUENCY_T g_sogou_vocabulary_update_frequency_data = { 0 };
#ifdef SOGOU_USE_PDA_SKIN
// 下载皮肤全路径名
LOCAL wchar s_sg_download_skin_file_full_path[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
// 下载皮肤文件名
LOCAL wchar s_sg_download_skin_file_name[MMIFILE_FILE_NAME_MAX_LEN +1] = { 0 };
#endif
// 是否已下载皮肤文件
LOCAL BOOLEAN s_sg_is_download_skin_file = FALSE;
LOCAL BOOLEAN s_sg_is_skin_port_run = FALSE;
// 当前所选图片索引
LOCAL uint32 s_sg_def_pic_index = 0;
LOCAL uint16 s_sg_skin_select_menu_index = 0;
// 皮肤列表更新时所用sim卡ID
LOCAL MN_DUAL_SYS_E s_last_selected_sim_id = MN_DUAL_SYS_MAX;
// 是定时更新
LOCAL BOOLEAN s_sg_IsAlertUpdate = FALSE;
// 皮肤列表排序类型
PUBLIC MMISG_SORT_TYPE_E g_sg_pic_sort_type = MMISG_SORT_TYPE_TIME;
// 更新时是否通过WIFI进行更新
LOCAL BOOLEAN s_sg_is_over_wifi = FALSE;

/*****************************************************************************/
//  Description : 输入法设置菜单窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputSettingsMenuWindow(
                                                  MMI_WIN_ID_T    win_id,
                                                  MMI_MESSAGE_ID_E   msg_id,
                                                  DPARAM             param
                                                  );

/*****************************************************************************/
//  Description : 创建设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateInputSettingsMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectInputSettingsMenu(MMI_CTRL_ID_T ctrl_id);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 皮肤管理菜单窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinManagementMenuWindow(
                                                   MMI_WIN_ID_T    win_id,
                                                   MMI_MESSAGE_ID_E   msg_id,
                                                   DPARAM             param
                                                   );
#endif
/*****************************************************************************/
//  Description : 是否恢复出厂值窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetDefaultWindow(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 创建皮肤管理菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinManagementMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择皮肤管理菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinManagementMenu(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 皮肤选择窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinSelectMenuWindow(
                                                   MMI_WIN_ID_T    win_id,
                                                   MMI_MESSAGE_ID_E   msg_id,
                                                   DPARAM             param
                                                   );

/*****************************************************************************/
//  Description : 重新添加皮肤item
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void re_add_skin_item(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 创建皮肤选择菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinSelectMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择皮肤选择菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinSelectMenu(void);

/*****************************************************************************/
//  Description : 皮肤选项窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinOptionMenuWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description : 创建皮肤选项菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinOptionMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择皮肤选项菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinOptionMenu(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 删除皮肤确认窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleDeleteSkinConfirmWin(MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM           param);

/*****************************************************************************/
//  Description : 皮肤排序窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinSortWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description : 创建皮肤排序菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinSortMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择皮肤排序菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinSortMenu(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 皮肤预览窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinPreViewAnimWindow(
                                               MMI_WIN_ID_T    win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                                );

/*****************************************************************************/
//  Description : 皮肤预览窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinPreViewAnimWindow2(
                                                MMI_WIN_ID_T    win_id,
                                                MMI_MESSAGE_ID_E   msg_id,
                                                DPARAM             param
                                                );

/*****************************************************************************/
//  Description : 皮肤浏览窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSkinBrowseWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 创建皮肤浏览菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinBrowseMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 index_num);
#endif
/*****************************************************************************/
//  Description : 更新设置窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUpdateSettingsMenuWindow(
                                               MMI_WIN_ID_T    win_id,
                                               MMI_MESSAGE_ID_E   msg_id,
                                               DPARAM             param
                                                );

/*****************************************************************************/
//  Description : 创建更新设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateUpdateSettingsMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择更新设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectUpdateSettingsMenu(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 系统更新设置窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSystemUpdateSettingsListWindow(
                                                  MMI_WIN_ID_T    win_id,
                                                  MMI_MESSAGE_ID_E   msg_id,
                                                  DPARAM             param
                                                  );

/*****************************************************************************/
//  Description : 创建系统更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSystemUpdateSettingsList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择系统更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSystemUpdateSettingsList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 自动更新设置窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutomaticUpdateSettingsListWindow(
                                                  MMI_WIN_ID_T    win_id,
                                                  MMI_MESSAGE_ID_E   msg_id,
                                                  DPARAM             param
                                                  );

/*****************************************************************************/
//  Description : 创建自动更新设置list
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateAutomaticUpdateSettingsList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择自动更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectAutomaticUpdateSettingsList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 词库更新提示窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateNotificationListWindow(
                                                           MMI_WIN_ID_T    win_id,
                                                           MMI_MESSAGE_ID_E   msg_id,
                                                           DPARAM             param
                                                           );

/*****************************************************************************/
//  Description : 创建词库更新提示列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateVocabularyUpdateNotificationsList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择词库更新提示列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectVocabularyUpdateNotificationList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 词库更新提示窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateNotificationOpenAlertWinWindow(
                                                                MMI_WIN_ID_T    win_id,
                                                                MMI_MESSAGE_ID_E   msg_id,
                                                                DPARAM             param
                                                           );

/*****************************************************************************/
//  Description : 连接提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectRemindListWindow(
                                                 MMI_WIN_ID_T    win_id,
                                                 MMI_MESSAGE_ID_E   msg_id,
                                                 DPARAM             param
                                                 );

/*****************************************************************************/
//  Description : 创建连接提醒列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateConnectRemindList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择连接提醒
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectConnectRemindList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 连接提醒提示窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectRemindOpenAlertWinWindow(
                                                         MMI_WIN_ID_T    win_id,
                                                         MMI_MESSAGE_ID_E   msg_id,
                                                         DPARAM             param
                                                         );

/*****************************************************************************/
//  Description : 词库更新频率窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateFrequencyListWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       );

/*****************************************************************************/
//  Description : 创建词库更新频率列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateVocabularyUpdateFrequencyList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择词库更新频率列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectVocabularyUpdateFrequencyList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 词库更新频率提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateFrequencyOpenAlertWinWindow(
                                                                     MMI_WIN_ID_T    win_id,
                                                                     MMI_MESSAGE_ID_E   msg_id,
                                                                     DPARAM             param
                                                                     );

/*****************************************************************************/
//  Description : 词库更新时间窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateTimeWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       );

/*****************************************************************************/
//  Description : 选择词库更新时间
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectVocabularyUpdateTimeEdit(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 词库更新时间提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateTimeOpenAlertWinWindow(
                                                                MMI_WIN_ID_T    win_id,
                                                                MMI_MESSAGE_ID_E   msg_id,
                                                                DPARAM             param
                                                                );

/*****************************************************************************/
//  Description : SIM卡选择窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : GPRS提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkingRemindTextBoxWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       );

/*****************************************************************************/
//  Description : 自动更新词库提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoUpdateWordRemindWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       );

/*****************************************************************************/
//  Description : 进入自动更新窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterUpdateWindow(void);

/*****************************************************************************/
//  Description : 打开自动更新窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenUpdateWindow(MN_DUAL_SYS_E sim_id);

/*****************************************************************************/
//  Description : 关闭自动更新窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CloseUpdateWindow(void);

/*****************************************************************************/
//  Description : 更新监视窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUpdateWatchingWindow(
                                              MMI_WIN_ID_T    win_id,
                                              MMI_MESSAGE_ID_E   msg_id,
                                              DPARAM             param
                                              );

/*****************************************************************************/
//  Description : 是否取消更新窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCancelQueryWinMsg(
                                            MMI_WIN_ID_T    win_id,
                                            MMI_MESSAGE_ID_E   msg_id,
                                            DPARAM             param
                                            );

/*****************************************************************************/
//  Description : 超时提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTimeOutQueryWinMsg(
                                            MMI_WIN_ID_T    win_id,
                                            MMI_MESSAGE_ID_E   msg_id,
                                            DPARAM             param
                                            );

/*****************************************************************************/
//  Description : 退出更新窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUpdateExitWin(
                                        MMI_WIN_ID_T    win_id,
                                        MMI_MESSAGE_ID_E   msg_id,
                                        DPARAM             param
                                        );

/*****************************************************************************/
//  Description : 自动保存消息
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SGSaveAutoEvent(void);

/*****************************************************************************/
//  Description : 更新词库
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SGUpdateWordLib(void);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 删除选择的皮肤文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SGDeleteSelectedSkinFile(uint16* pfileName, uint16 nameLen);
#endif
/*****************************************************************************/
//  Description : 检查皮肤是否符合格式要求
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SG_Check_Skin_Is_valid(uint16* pfileName,uint16 nameLen);

// 搜狗输入法设置1
WINDOW_TABLE(SG_INPUT_SETTINGS_MENU_WIN_TAB) =
{
    WIN_FUNC( (uint32)HandleInputSettingsMenuWindow ),
    WIN_ID( SG_WIN_ID_SOGOU_SETTING ),
#ifdef MMI_IM_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_SETTING_MENU_FORM_ID),    
#ifdef SOGOU_USE_PDA_SKIN
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_SKIN_MANAGEMENT_FORM_ID, SG_CTRL_ID_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_SKIN_MANAGEMENT_LABEL_ID, SG_CTRL_ID_SKIN_MANAGEMENT_FORM_ID),
#endif
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_VOCABULARY_UPDATE_FORM_ID, SG_CTRL_ID_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_VOCABULARY_UPDATE_LABEL_ID, SG_CTRL_ID_VOCABULARY_UPDATE_FORM_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_UPDATE_SETTING_FORM_ID, SG_CTRL_ID_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_UPDATE_SETTING_LABEL_ID, SG_CTRL_ID_UPDATE_SETTING_FORM_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_RESET_DEFAULT_FORM_ID, SG_CTRL_ID_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_RESET_DEFAULT_LABEL_ID, SG_CTRL_ID_RESET_DEFAULT_FORM_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_NULL),
    END_WIN
};
#ifdef SOGOU_USE_PDA_SKIN
// 皮肤管理1.1
WINDOW_TABLE( SG_SKIN_MANAGEMENT_MENU_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSkinManagementMenuWindow ),
    WIN_ID( SG_WIN_ID_SKIN_MANAGEMENT ),
#ifdef MMI_IM_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_SKIN_MANAGEMENT_MENU_FORM_ID),    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_SKIN_SELECT_FORM_ID, SG_CTRL_ID_SKIN_MANAGEMENT_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_SKIN_SELECT_LABEL_ID, SG_CTRL_ID_SKIN_SELECT_FORM_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_MORE_SKIN_DOWNLOAD_FORM_ID, SG_CTRL_ID_SKIN_MANAGEMENT_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_MORE_SKIN_DOWNLOAD_LABEL_ID, SG_CTRL_ID_MORE_SKIN_DOWNLOAD_FORM_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_NULL),
    END_WIN
};

// 皮肤选择1.1.1
WINDOW_TABLE( SG_SKIN_SELECT_MENU_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSkinSelectMenuWindow ),
    WIN_ID( SG_WIN_ID_SKIN_SELECT ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_NULL),
    END_WIN
};

// 皮肤选项1.1.1.1
WINDOW_TABLE( SG_SKIN_OPTION_MENU_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSkinOptionMenuWindow ),
    WIN_ID( SG_WIN_ID_SKIN_OPTION ),
#ifdef MMI_IM_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID),    
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_NULL),
    END_WIN
};

// 皮肤排序
WINDOW_TABLE( SG_SKIN_SORT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSkinSortWindow ),
    WIN_ID( SG_WIN_ID_SKIN_SORT ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_NULL),
    END_WIN
};

// 皮肤预览1.1.1.1.1
WINDOW_TABLE( SG_SKIN_PREVIEW_ANIM_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSkinPreViewAnimWindow ),
    WIN_ID( SG_WIN_ID_SKIN_PREVIEW ),
    WIN_SOFTKEY(TXT_COMMON_STARTUP, TXT_NULL, STXT_RETURN),
    CREATE_ANIM_CTRL(SG_CTRL_ID_ANIM_SKIN_PREVIEW,SG_WIN_ID_SKIN_PREVIEW),
    END_WIN
};

// 默认皮肤预览
WINDOW_TABLE(SG_SKIN_PREVIEW_ANIM_WIN2_TAB) =
{
    WIN_FUNC( (uint32)HandleSkinPreViewAnimWindow2 ),
    WIN_ID( SG_WIN_ID_SKIN_PREVIEW ),
    WIN_SOFTKEY(TXT_COMMON_STARTUP, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 皮肤浏览界面
WINDOW_TABLE( SG_SKIN_BROWSE_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSkinBrowseWindow ),
    WIN_ID( SG_WIN_ID_SKIN_BROWSE ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_NULL),
    END_WIN
};
#endif
// 更新设置
WINDOW_TABLE( SG_UPDATE_SETTINGS_MENU_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleUpdateSettingsMenuWindow ),
    WIN_ID( SG_WIN_ID_UPDATE_SETTINGS ),
#ifdef MMI_IM_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_UPDATE_SETTING_MENU_FORM_ID),    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_SYSTEM_SETTING_FORM_ID, SG_CTRL_ID_UPDATE_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_SYSTEM_SETTING_LABEL_ID, SG_CTRL_ID_SYSTEM_SETTING_FORM_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_AUTOUPDATE_SETTING_FORM_ID, SG_CTRL_ID_UPDATE_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_AUTOUPDATE_SETTING_LABEL_ID, SG_CTRL_ID_AUTOUPDATE_SETTING_FORM_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_NULL),
    END_WIN
};

// 系统更新设置
WINDOW_TABLE( SG_SYSTEM_UPDATE_SETTINGS_LIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSystemUpdateSettingsListWindow ),
    WIN_ID( SG_WIN_ID_SYSTEM_UPDATE_SETTINGS ),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, SG_CTRL_ID_LIST_SYSTEM_UPDATE_SETTINGS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 自动更新设置
WINDOW_TABLE( SG_AUTOMATIC_UPDATE_SETTINGS_LIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAutomaticUpdateSettingsListWindow ),
    WIN_ID( SG_WIN_ID_AUTOMATIC_UPDATE_SETTINGS ),
#ifdef MMI_IM_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_AUTOUPDATE_SETTING_MENU_FORM_ID),    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_SETTING_FORM_ID, SG_CTRL_ID_AUTOUPDATE_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_LABEL_ID, SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_SETTING_FORM_ID),
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_VOCABULARY_UPDATE_TIME_MENU_FORM_ID, SG_CTRL_ID_AUTOUPDATE_SETTING_MENU_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, SG_CTRL_ID_VOCABULARY_UPDATE_TIME_LABEL_ID, SG_CTRL_ID_VOCABULARY_UPDATE_TIME_MENU_FORM_ID),
#endif
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 词库更新提示
WINDOW_TABLE( SG_VOCABULARY_UPDATE_NOTIFICATION_LIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleVocabularyUpdateNotificationListWindow ),
    WIN_ID( SG_WIN_ID_VOCABULARY_UPDATE_NOTIFICATION ),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, SG_CTRL_ID_LIST_VOCABULARY_UPDATE_NOTIFICATION),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 连接提醒
WINDOW_TABLE( SG_CONNECT_REMIND_LIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleConnectRemindListWindow ),
    WIN_ID( SG_WIN_ID_CONNECT_REMIND ),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, SG_CTRL_ID_LIST_CONNECT_REMIND),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 更新时sim卡选择窗口
WINDOW_TABLE(SG_SIMCARD_SELECT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSIMSelectWinMsg),
    WIN_ID(SG_WIN_ID_SIMCARDSELECT),
    WIN_TITLE(TXT_SIM_SEL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, SG_CTRL_ID_LIST_SIMCARDSELECT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 词库更新频率
WINDOW_TABLE( SG_VOCABULARY_UPDATE_FREQUENCY_LIST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleVocabularyUpdateFrequencyListWindow ),
    WIN_ID( SG_WIN_ID_VOCABULARY_UPDATE_FREQUENCY ),
    WIN_TITLE(TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, SG_CTRL_ID_LIST_VOCABULARY_UPDATE_FREQUENCY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// 词库更新时间
WINDOW_TABLE( SG_VOCABULARY_UPDATE_TIME_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleVocabularyUpdateTimeWindow ),
    WIN_ID( SG_WIN_ID_VOCABULARY_UPDATE_TIME ),
    WIN_TITLE(TXT_NULL),
#ifdef MMI_IM_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_VOCABULARY_UPDATE_TIME_FORM_ID),
    CHILD_EDIT_TOUCH_TIME_CTRL(TRUE, SG_CTRL_ID_TIMEEDITBOX_VOCABULARY_UPDATE_TIME, SG_CTRL_ID_VOCABULARY_UPDATE_TIME_FORM_ID),
#else
    CREATE_EDIT_TIME_CTRL(SG_CTRL_ID_TIMEEDITBOX_VOCABULARY_UPDATE_TIME),
#endif

//#ifdef MMI_PDA_SUPPORT	//PDA风格及TK版本，不需要ok键		//Bug 85724 - [ST][6530][64X64_240X400PDA]搜狗输入法设置中，更新设置选项中的自动更新设置中的词库更新时间只能选择无法保存
//    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
//#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//#endif

    END_WIN
};

// GPRS提醒
WINDOW_TABLE( SG_NETWORKING_REMIND_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleNetworkingRemindTextBoxWindow ),
    WIN_ID( SG_WIN_ID_NETWORKING_REMIND ),
    WIN_TITLE(TXT_SOGOU_CONNECT_REMIND),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_TEXT_CTRL(SG_CTRL_ID_TEXTBOX_NETWORKING_REMIND),
    END_WIN
};

// 自动更新词库提醒
WINDOW_TABLE( SG_AUTOUPDATEWORD_REMIND_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAutoUpdateWordRemindWindow ),
    WIN_ID( SG_WIN_ID_AUTOUPDATEWORD_REMIND ),
    WIN_TITLE(TXT_SOGOU_AUTOUPDATE),
#ifdef MMI_PDA_SUPPORT	//PDA风格及TK版本，不需要ok键
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif

#ifdef MMI_IM_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, SG_CTRL_ID_AUTOUPDATEWORD_REMIND_FORM_ID),
#else
    CREATE_TEXT_CTRL(SG_CTRL_ID_TEXTBOX_AUTOUPDATEWORD_REMIND),
#endif
    END_WIN
};

/*****************************************************************************/
//  Description : 打开输入模式窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_OpenSogouIMSetInputModeWin(void)
{
    MMK_CloseMiddleWin(SG_WIN_ID_START, SG_WIN_ID_MAX);
	//开机后直接从英文输入进入此处需要设g_sg_save_position值
	//因为没有走中文的Sogou_ImeInit初始化没有设过g_sg_save_position值
	if(1==SG_Check_D_Space_Is_Enough())
	{
		SG_setBaseFolderPath(L"D:\\@sogou\\");
		g_sg_save_position=1;//皮肤放到E盘//2012_11_6
	}
	else if(1==SG_Check_E_Space_Is_Enough())
	{
		SG_setBaseFolderPath(L"E:\\@sogou\\");
		g_sg_save_position=1;
	}
	else
	{
		SCI_TraceLow("Sogou_ImeInit 4 g_sg_save_position=2");
		g_sg_save_position=2;
	}
    MMK_CreateWin((uint32*)SG_INPUT_SETTINGS_MENU_WIN_TAB, PNULL);//搜狗输入法设置1
    return TRUE;
}

/*****************************************************************************/
//  Description : 输入法设置菜单窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInputSettingsMenuWindow(
                                              MMI_WIN_ID_T    win_id,
                                              MMI_MESSAGE_ID_E   msg_id,
                                              DPARAM             param
                                              )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = SG_CTRL_ID_MENU_SOGOU_SETTING;
    MMI_STRING_T title = { 0 };
    
    //SOGOU_TRACE_OUT:"HandleInputSettingsMenuWindow 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_786_112_2_18_2_29_36_320,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        GUIWIN_SetTitleTextId(win_id, TXT_SOGOU_SETTING, FALSE);
        GUIFORM_SetType(SG_CTRL_ID_SETTING_MENU_FORM_ID, GUIFORM_TYPE_TP);

#ifdef SOGOU_USE_PDA_SKIN
        GUIFORM_SetStyle(SG_CTRL_ID_SKIN_MANAGEMENT_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_MANAGEMENT, &title);
        GUILABEL_SetText(SG_CTRL_ID_SKIN_MANAGEMENT_LABEL_ID, &title, FALSE);
#endif

        GUIFORM_SetStyle(SG_CTRL_ID_VOCABULARY_UPDATE_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE, &title);
        GUILABEL_SetText(SG_CTRL_ID_VOCABULARY_UPDATE_LABEL_ID, &title, FALSE);

        GUIFORM_SetStyle(SG_CTRL_ID_UPDATE_SETTING_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_UPDATE_SETTINGS, &title);
        GUILABEL_SetText(SG_CTRL_ID_UPDATE_SETTING_LABEL_ID, &title, FALSE);
        
        GUIFORM_SetStyle(SG_CTRL_ID_RESET_DEFAULT_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_RESET_DEFAULT, &title);
        GUILABEL_SetText(SG_CTRL_ID_RESET_DEFAULT_LABEL_ID, &title, FALSE);
#ifdef SOGOU_USE_PDA_SKIN
        MMK_SetAtvCtrl(win_id, SG_CTRL_ID_SKIN_MANAGEMENT_FORM_ID);
#else
        MMK_SetAtvCtrl(win_id, SG_CTRL_ID_VOCABULARY_UPDATE_FORM_ID);
#endif
#else
        s_sg_IsAlertUpdate = FALSE;
        CreateInputSettingsMenu(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_IM_PDA_SUPPORT
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
#endif
        SelectInputSettingsMenu(ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    case MSG_CLOSE_WINDOW:
        MMIAPIIM_SGUpdateRelease();
#ifdef SOGOU_USE_PDA_SKIN
        MMIAPIIM_SGFreeSkinFileList();
#endif
        if (s_skin_ptr)
        {
            SCI_FREE(s_skin_ptr);
        }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateInputSettingsMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    MMI_STRING_T  str= {0};
    uint16 node_index = 0;
    GUI_BOTH_RECT_T      both_rect = MMITHEME_GetWinClientBothRect(win_id);

    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num = 4;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl( &create );

    MMI_GetLabelTextByLang(TXT_SOGOU_SETTING, &str);
    GUIMENU_SetMenuTitle(&str,ctrl_id);

    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    node_item.select_icon_id = 0;

#ifdef SOGOU_USE_PDA_SKIN
    MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_MANAGEMENT, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SOGOU_SETTING_MENU_SKIN_MANAGEMENT+1, 0, &node_item, ctrl_id);//6_7_1
#endif

    node_index++;
    MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SOGOU_SETTING_MENU_VOCABULARY_UPDATE+1, 0, &node_item, ctrl_id);//6_7_1

    node_index++;
    MMI_GetLabelTextByLang(TXT_SOGOU_UPDATE_SETTINGS, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SOGOU_SETTING_MENU_UPDATE_SETTINGS+1, 0, &node_item, ctrl_id);//6_7_1
    
    node_index++;
    MMI_GetLabelTextByLang(TXT_RESET_DEFAULT, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SOGOU_SETTING_MENU_RESET_DEFAULT+1, 0, &node_item, ctrl_id);//6_7_1
}

/*****************************************************************************/
//  Description : 是否恢复出厂值窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleResetDefaultWindow(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMIAPIIM_SGResetNv ();
        MMIAPIIM_SGResetDefault();
        MMK_CloseWin(win_id);
#if 0      //非长时间等待提示，不需要  
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SOGOU_RESET_SUCCESS, TXT_NULL,IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_NONE,PNULL);
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 选择设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectInputSettingsMenu(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN result = TRUE;

#ifdef MMI_IM_PDA_SUPPORT
    switch (ctrl_id)
#else
    switch (GUIMENU_GetCurNodeId(ctrl_id) - 1)
#endif
    {
#ifdef SOGOU_USE_PDA_SKIN    
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_SKIN_MANAGEMENT_LABEL_ID:
#else
    case SOGOU_SETTING_MENU_SKIN_MANAGEMENT://6_7_1
#endif
        MMK_CreateWin((uint32*)SG_SKIN_MANAGEMENT_MENU_WIN_TAB,PNULL);  //皮肤管理1.1//6_7_1

        break;
#endif
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_VOCABULARY_UPDATE_LABEL_ID:
#else
    case SOGOU_SETTING_MENU_VOCABULARY_UPDATE://6_7_1
#endif
    {
        uint32 free_high_word = 0;
        uint32 free_low_word = 0;
        BOOLEAN is_space_enough = FALSE;

        if (MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
                                       MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                                       &free_high_word, &free_low_word))
        {
            if (!(0 == free_high_word && SG_MINIMIZE_FREESPACE > free_low_word))
            {
                is_space_enough = TRUE;
            }
        }
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        else if (is_space_enough == FALSE)
        {
            MMIPUB_OpenAlertWarningWin( TXT_SOGOU_INSERT_SD_CARD);//TXT_NO_SPACE
        }
        else
        {
            SGUpdateWordLib();
        }
        break;
    }

#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_UPDATE_SETTING_LABEL_ID:
#else
    case SOGOU_SETTING_MENU_UPDATE_SETTINGS://6_7_1
#endif
        MMK_CreateWin((uint32*)SG_UPDATE_SETTINGS_MENU_WIN_TAB,PNULL);  //更新设置
        break;

#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_RESET_DEFAULT_LABEL_ID:
#else
    case SOGOU_SETTING_MENU_RESET_DEFAULT://6_7_1
#endif
        MMIPUB_OpenQueryWinByTextId(TXT_SOGOU_RESET_DEFAULT_QUERY, IMAGE_PUBWIN_QUERY, 
            PNULL, HandleResetDefaultWindow);
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
}

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 皮肤管理菜单窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinManagementMenuWindow(
                                                   MMI_WIN_ID_T    win_id,
                                                   MMI_MESSAGE_ID_E   msg_id,
                                                   DPARAM             param
                                                  )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_MENU_SKIN_MANAGEMENT;
    MMI_STRING_T title = { 0 };

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        GUIWIN_SetTitleTextId(win_id, TXT_SOGOU_SKIN_MANAGEMENT, FALSE);
        GUIFORM_SetType(SG_CTRL_ID_SKIN_MANAGEMENT_MENU_FORM_ID, GUIFORM_TYPE_TP);
        
        GUIFORM_SetStyle(SG_CTRL_ID_SKIN_SELECT_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_SELECT, &title);
        GUILABEL_SetText(SG_CTRL_ID_SKIN_SELECT_LABEL_ID, &title, FALSE);
        
        GUIFORM_SetStyle(SG_CTRL_ID_MORE_SKIN_DOWNLOAD_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_MORE_SKIN_DOWNLOAD, &title);
        GUILABEL_SetText(SG_CTRL_ID_MORE_SKIN_DOWNLOAD_LABEL_ID, &title, FALSE);

        MMK_SetAtvCtrl(win_id, SG_CTRL_ID_SKIN_SELECT_FORM_ID);

#else
        CreateSkinManagementMenu(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_IM_PDA_SUPPORT
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
#endif
        SelectSkinManagementMenu(ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 皮肤管理菜单窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinManagementMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    MMI_STRING_T  str= {0};
    uint16 node_index = 0;
    GUI_BOTH_RECT_T      both_rect = MMITHEME_GetWinClientBothRect(win_id);

    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num=2;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl( &create );

    MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_MANAGEMENT, &str);
    GUIMENU_SetMenuTitle(&str,ctrl_id);

    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    node_item.select_icon_id = 0;

    MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_SELECT, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SKIN_MANAGEMENT_MENU_SKIN_SELECT+1, 0, &node_item, ctrl_id);//6_7_1

    node_index++;
    MMI_GetLabelTextByLang(TXT_SOGOU_MORE_SKIN_DOWNLOAD, &str);
    node_item.item_text_ptr = &str;

    GUIMENU_InsertNode(node_index, SKIN_MANAGEMENT_MENU_DOWNLOAD_SKIN+1, 0, &node_item, ctrl_id);//6_7_1
}

/*****************************************************************************/
//  Description : 选择皮肤管理菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinManagementMenu(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN result = TRUE;
    MMI_STRING_T note = {0};

#ifdef MMI_IM_PDA_SUPPORT
    switch (ctrl_id)
#else
    switch (GUIMENU_GetCurNodeId(ctrl_id) - 1)
#endif
    {
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_SKIN_SELECT_LABEL_ID:
#else
    case SKIN_MANAGEMENT_MENU_SKIN_SELECT://6_7_1
#endif
        if (MMIAPIFMM_GetDeviceStatus((uint16*)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
                                    MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)))
        {
            MMK_CreateWin((uint32*)SG_SKIN_SELECT_MENU_WIN_TAB, PNULL);  //皮肤选择1.1.1
        }
        else
        {
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMI_GetLabelTextByLang(TXT_COMMON_UDISK_USING, &note);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_SOGOU_INSERT_SD_CARD, &note);
            }
            
            MMIPUB_OpenAlertWinByTextPtr(PNULL, &note, PNULL, IMAGE_PUBWIN_WARNING,
                PNULL, PNULL, MMIPUB_SOFTKEY_NONE, 0);
            result = FALSE;
        }
        break;

#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_MORE_SKIN_DOWNLOAD_LABEL_ID:
#else
    case SKIN_MANAGEMENT_MENU_DOWNLOAD_SKIN://6_7_1
#endif
        if(MMIAPIFMM_GetDeviceStatus((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)))
        {
            s_update_type = SG_UPDATE_TYPE_SKIN_DIR;
            if (PNULL == s_skin_list_file)
            {
                if (g_sogou_update_settings_data.is_need_connect_remind == TRUE)
                {
#ifdef MMI_IM_PDA_SUPPORT
                    MMI_WIN_ID_T alert_win_id = SG_WIN_ID_NETWORKING_REMIND;
                    MMI_STRING_T text_str = { 0 };
                    if (s_update_type == SG_UPDATE_TYPE_VOCABULARY)
                    {
                        MMI_GetLabelTextByLang(TXT_SOGOU_DOWNLOAD_VOCABULARY, &text_str);
                    }
                    else
                    {
                        MMI_GetLabelTextByLang(TXT_SOGOU_DOWNLOAD_SKIN, &text_str);
                    }
                    MMIPUB_OpenQueryWinByTextPtr(&text_str, PNULL, IMAGE_PUBWIN_QUERY, 
                        &alert_win_id, HandleNetworkingRemindTextBoxWindow);
#else
                    MMK_CreateWin((uint32*)SG_NETWORKING_REMIND_WIN_TAB,PNULL);
#endif
                }
                else
                {
                    EnterUpdateWindow();
                }
            }
            else
            {
                MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_NONE);
                MMIAPIIM_SoGouOpenSkinBrowseWin();
            }
        }
        else
        {
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMI_GetLabelTextByLang(TXT_COMMON_UDISK_USING, &note);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_SOGOU_INSERT_SD_CARD, &note);
            }

            MMIPUB_OpenAlertWinByTextPtr(
                PNULL,
                &note,
                PNULL,
                IMAGE_PUBWIN_WARNING,
                PNULL,PNULL,
                MMIPUB_SOFTKEY_NONE,
                0);
            result = FALSE;
        }
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 皮肤选择窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSkinSelectMenuWindow(
                                                   MMI_WIN_ID_T    win_id,
                                                   MMI_MESSAGE_ID_E   msg_id,
                                                   DPARAM             param
                                                  )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_MENU_SKIN_SELECT;
    uint16 selected_index = 0;
    //SOGOU_TRACE_OUT:"HandleSkinSelectMenuWindow win_id=0x%04x,msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_1248_112_2_18_2_29_37_321,(uint8*)"dd",win_id,msg_id);
    SCI_TraceLow("[SOGOU]HandleSkinSelectMenuWindow1 selected_index =%d",selected_index); //yl add 20120920
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_sg_skin_select_menu_index = 0;
        CreateSkinSelectMenu(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        SCI_TraceLow("[SOGOU]HandleSkinSelectMenuWindow case MSG_APP_OK selected_index =%d",selected_index); //yl add 20120920
        selected_index = GUIMENU_GetCurNodeId(ctrl_id)-1;
        SCI_TraceLow("[SOGOU]HandleSkinSelectMenuWindow case MSG_APP_OK end selected_index =%d",selected_index); //yl add 20120920
        s_sg_skin_select_menu_index = selected_index;
        SelectSkinSelectMenu();//这里不能close窗口
        break;
    case MSG_GET_FOCUS:
        re_add_skin_item(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 重新添加皮肤item
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void re_add_skin_item(MMI_WIN_ID_T win_id ,MMI_CTRL_ID_T ctrl_id)
{
    MMK_DestroyDynaCtrl(ctrl_id);
    CreateSkinSelectMenu(win_id,ctrl_id);
}

LOCAL int SGCompareFileTime(const void *i1_data, const void *i2_data)
{
    MMISG_PIC_List_T *data1_ptr;
    MMISG_PIC_List_T *data2_ptr;
    int32             result;

    //SCI_ASSERT(PNULL != i1_data && PNULL != i2_data); /*assert verified*/
    if(PNULL == i1_data || PNULL == i2_data )
    {
        return -1;
    }
    
    data1_ptr = (MMISG_PIC_List_T *) i1_data;
    data2_ptr = (MMISG_PIC_List_T *) i2_data;
    if (data1_ptr->time > data2_ptr->time )
    {
        result = -1;
    }
    else if (data1_ptr->time < data2_ptr->time)
    {
        result = 1;
    }
    else
    {
        result = MMIAPICOM_CompareTwoWstr( data1_ptr->filename, data1_ptr->filename_len,
            data2_ptr->filename, data2_ptr->filename_len );
    }

    return (result);
}

#define WDP_IS_ALPHA(CH) ((CH >= 'a' && CH <= 'z') || (CH >= 'A' &&  CH <='Z'))
#define WDP_IS_UPPER(CH) (CH >= 'A' &&  CH <='Z')

LOCAL int WDP_CompareFilename(const wchar * string1, const wchar * string2)
{
    int ret = 0 ;

    if( string1 == PNULL && string2 == PNULL )
    {
        return 0;
    }
    else if( string1 == PNULL && string2 != PNULL )
    {
        return -1;
    }
    else if( string1 != PNULL && string2 == PNULL )
    {
        return 1;
    }
    else
    {
        /*lint -save -e613 */
        wchar char1 = *(wchar *)string1;
        wchar char2 = *(wchar *)string2;

        if (WDP_IS_ALPHA(char1) && WDP_IS_ALPHA(char2))
        {
            if (WDP_IS_UPPER(char1))
            {
                char1 += 32;
            }
            if (WDP_IS_UPPER(char2))
            {
                char2 += 32;
            }

            ret = char1 - char2;
        }
        else
        {
            ret = *(wchar *)string1 - *(wchar *)string2;/*lint !e613 */
        }

        while( (!ret) && *string2 && *string1)/*lint !e613 */
        {
            ++string1;/*lint !e613 */
            ++string2;
            char1 = *(wchar *)string1;
            char2 = *(wchar *)string2;
            if (WDP_IS_ALPHA(char1) && WDP_IS_ALPHA(char2))
            {
                if (WDP_IS_UPPER(char1))
                {
                    char1 += 32;
                }
                if (WDP_IS_UPPER(char2))
                {
                    char2 += 32;
                }

                ret = char1 - char2;
            }
            else
            {
                ret = *(wchar *)string1 - *(wchar *)string2;
            }
        }
        /*lint -restore */
        return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
    }
}

LOCAL int SGCompareFileName(const void *i1_data, const void *i2_data)
{
    int               result;
    MMISG_PIC_List_T *data1_ptr;
    MMISG_PIC_List_T *data2_ptr;

    //SCI_ASSERT(PNULL != i1_data && PNULL != i2_data); /*assert verified*/
    if(PNULL == i1_data || PNULL == i2_data )
    {
        return -1;
    }
    
    data1_ptr = (MMISG_PIC_List_T *) i1_data;
    data2_ptr = (MMISG_PIC_List_T *) i2_data;

    result = WDP_CompareFilename(data1_ptr->filename,  data2_ptr->filename);

    return (result);
}

LOCAL int SGFileDataSort(MMISG_PIC_List_T *list_data_ptr, MMISG_SORT_TYPE_E sort, uint16 file_num)
{
    //SCI_ASSERT(PNULL != list_data_ptr); /*assert verified*/
    if(PNULL == list_data_ptr )
    {
        return -1;
    }
    
    if (sort == MMISG_SORT_TYPE_NAME)
    {
        qsort((void*)list_data_ptr,file_num, sizeof(MMISG_PIC_List_T), SGCompareFileName);
    }
    else
    {
        qsort((void*)list_data_ptr,file_num, sizeof(MMISG_PIC_List_T), SGCompareFileTime);
    }
    return (1);
}

/*****************************************************************************/
//  Description : 创建皮肤选择菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinSelectMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    MMI_STRING_T  str_default= {0};
    MMI_STRING_T  str= {0};
    uint16 node_index = 0;
    uint32 temp_num1=0;
    uint32 temp_num2=0;
    uint32 readed;
    uint32 filehandle;
    BOOLEAN have_find_pic_file=FALSE;
    uint32 temp=0;
    uint16 i=0;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    LOCAL MMISG_PIC_List_T s_sg_pic_list_data[SG_MAX_MENU_ITEM_COUNT - 1] = { 0 }; //不包括默认皮肤的皮肤列表
	LOCAL wchar s_sg_temp_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = { 0 };
    uint16 def_pic_filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = { 0 };
    wchar find_path[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint16 find_path_len = MMIFILE_FILE_NAME_MAX_LEN;
    MMIFILE_FIND_DATA_T find_data = {0};
    MMIFILE_HANDLE find_handle = 0;    

	if(0==g_sg_save_position)
    {
    	filehandle= MMIAPIFMM_CreateFile((const wchar*)SG_D_SKIN_CONFIG_FILE, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);
	}
	else if(1==g_sg_save_position)
	{
		filehandle= MMIAPIFMM_CreateFile((const wchar*)SG_E_SKIN_CONFIG_FILE, SFS_MODE_OPEN_EXISTING|SFS_MODE_WRITE|SFS_MODE_READ, 0, 0);
	}
	else
    {
    	return;
	}
	
    SCI_MEMSET(def_pic_filename, 0, sizeof(def_pic_filename));
    if (filehandle)
    {
        MMIAPIFMM_ReadFile(filehandle, (void*)def_pic_filename, MMIFILE_FILE_NAME_MAX_LEN * 2, &readed,0);
        MMIAPIFMM_CloseFile(filehandle);
        temp = MMIAPICOM_Wstrlen((const wchar*)def_pic_filename);
        SCI_MEMSET(def_pic_filename + temp - 4, 0, 8);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_DEFAULT, &str);
        SCI_MEMCPY(def_pic_filename, str.wstr_ptr, 2 * str.wstr_len);
    }
    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num=SG_MAX_MENU_ITEM_COUNT;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl( &create );

    MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_SELECT, &str);
    GUIMENU_SetMenuTitle(&str,ctrl_id);

    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

    MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_DEFAULT, &str_default);
    node_item.item_text_ptr = &str_default;
    if (0 != MMIAPICOM_CompareTwoWstr(str_default.wstr_ptr, str_default.wstr_len, def_pic_filename, 
                                      MMIAPICOM_Wstrlen((const wchar*)def_pic_filename)))
    {
        node_item.select_icon_id = IMAGE_CHECK_UNSELECTED_ICON;
        have_find_pic_file=FALSE;
        s_sg_def_pic_index = 0;
    }
    else
    {
        node_item.select_icon_id = IMAGE_CHECK_SELECTED_ICON;
        have_find_pic_file=TRUE;
        s_sg_def_pic_index = node_index;
    }
    if (MMIAPIIM_SGIsUseDefaultSkin())
    {
        node_item.select_icon_id = IMAGE_CHECK_SELECTED_ICON;
    }
    if (s_sg_is_skin_port_run)
    {
        SCI_MEMSET(s_sg_temp_file_name, 0, sizeof(s_sg_temp_file_name));
        SCI_MEMCPY(s_sg_temp_file_name,
                   s_sg_pic_list_data[s_sg_skin_select_menu_index - 1].filename,
                   2 * s_sg_pic_list_data[s_sg_skin_select_menu_index - 1].filename_len);
    }

    GUIMENU_InsertNode(node_index, node_index+1, 0, &node_item, ctrl_id);

    if(MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
                               MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                               (const wchar*)SG_SKIN_DIR,
                               MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_DIR),
                               (const wchar*)SG_SKIN_FIND_SUFFIX,
                               MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_FIND_SUFFIX),
                               find_path,
                               &find_path_len))
    {
        find_handle =  MMIAPIFMM_FindFirstFile(find_path,find_path_len, &find_data);
        if (SFS_INVALID_HANDLE != find_handle)
        {
            temp_num1=MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_FIND_SUFFIX);
            do
            {
                if(!(SFS_ATTR_DIR&find_data.attr))
                {
                    temp_num2 = (uint32)(MMIAPICOM_Wstrlen((const wchar*)find_data.name) - temp_num1 + 1);
                    if (0 != MMIAPICOM_CompareTwoWstr(str_default.wstr_ptr,str_default.wstr_len,find_data.name,temp_num2))
                    {
                        node_index++;
                        SCI_MEMSET( s_sg_pic_list_data[node_index-1].filename,0,((MMIFMM_FULL_FILENAME_LEN+1)*sizeof(wchar)));
                        s_sg_pic_list_data[node_index-1].filename_len=temp_num2;
                        SCI_MEMCPY(s_sg_pic_list_data[node_index-1].filename,find_data.name,2*temp_num2);
                        s_sg_pic_list_data[node_index-1].time = MMIAPICOM_Tm2Second(find_data.modify_time.sec, find_data.modify_time.min, find_data.modify_time.hour,
                                                        find_data.modify_Date.mday, find_data.modify_Date.mon, find_data.modify_Date.year);

                    }
                }
            }
            while ((node_index < SG_MAX_MENU_ITEM_COUNT-1) && (SFS_NO_ERROR == MMIAPIFMM_FindNextFile(find_handle, &find_data)));

            MMIAPIFMM_FindClose(find_handle);
        }
        SGFileDataSort((MMISG_PIC_List_T*)s_sg_pic_list_data,g_sg_pic_sort_type,node_index);
        for(i=0;i<node_index;i++)
        {
            str.wstr_len = s_sg_pic_list_data[i].filename_len;
            str.wstr_ptr = s_sg_pic_list_data[i].filename;
            node_item.item_text_ptr = &str;
            if(s_sg_is_download_skin_file)
            {
                if (0 == MMIAPICOM_CompareTwoWstr(str.wstr_ptr, str.wstr_len, s_sg_download_skin_file_name, 
                                                  MMIAPICOM_Wstrlen((const wchar*)s_sg_download_skin_file_name) - 4))
                {
                    s_sg_skin_select_menu_index = i + 1;
                    s_sg_is_download_skin_file = FALSE;
                }
            }
            else if(s_sg_is_skin_port_run)
            {
                if (0 == MMIAPICOM_CompareTwoWstr(str.wstr_ptr,str.wstr_len, s_sg_temp_file_name, MMIAPICOM_Wstrlen((const wchar*)s_sg_temp_file_name)))
                {
                    s_sg_skin_select_menu_index = i + 1;
                    s_sg_is_skin_port_run = FALSE;
                }
            }
            if(have_find_pic_file)
            {
                node_item.select_icon_id = IMAGE_CHECK_UNSELECTED_ICON;
            }
            else
            {
                if (MMIAPIIM_SGIsUseDefaultSkin() ||
                    0 != MMIAPICOM_CompareTwoWstr(str.wstr_ptr,str.wstr_len, def_pic_filename, MMIAPICOM_Wstrlen((const wchar*)def_pic_filename)))
                {
                    node_item.select_icon_id = IMAGE_CHECK_UNSELECTED_ICON;
                    have_find_pic_file=FALSE;
                }
                else
                {
                    node_item.select_icon_id = IMAGE_CHECK_SELECTED_ICON;
                    have_find_pic_file=TRUE;
                    s_sg_def_pic_index = i + 1;
                }
            }
            GUIMENU_InsertNode(i+1, i+2, 0, &node_item, ctrl_id);
        }
        s_sg_is_download_skin_file = FALSE;
    }
    GUIMENU_SetFirstAndSelectedItem(0, s_sg_skin_select_menu_index, ctrl_id);
}

/*****************************************************************************/
//  Description : 选择皮肤选择菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinSelectMenu(void)
{
    MMK_CreateWin((uint32*)SG_SKIN_OPTION_MENU_WIN_TAB, PNULL);//皮肤选项1.1.1.1
    return TRUE;
}

/*****************************************************************************/
//  Description : 皮肤选项窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinOptionMenuWindow(
                                                   MMI_WIN_ID_T    win_id,
                                                   MMI_MESSAGE_ID_E   msg_id,
                                                   DPARAM             param
                                                  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifndef MMI_IM_PDA_SUPPORT    
    LOCAL uint16 sg_OptionWin_index = 0;
#endif
    MMI_CTRL_ID_T ctrl_id = SG_CTRL_ID_MENU_SKIN_OPTION;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        {
            uint16 selected_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT) - 1;
            MMI_STRING_T title = { 0 };
            GUIFORM_INIT_DATA_T form_init_data = { 0 };
            GUILABEL_INIT_DATA_T label_init_data = { 0 };
            GUIFORM_DYNA_CHILD_T dyna_form = { 0 };
            GUIFORM_DYNA_CHILD_T dyna_label = { 0 };
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            SCI_TraceLow("[SOGOU]HandleSkinOptionMenuWindow #ifdef MMI_IM_PDA_SUPPORT end selected_index =%d",selected_index); //yl add 20120920
            
            // windows title
            GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&title);
            GUIWIN_SetTitleText(win_id, title.wstr_ptr, title.wstr_len, FALSE);
            GUIFORM_SetType(SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID, GUIFORM_TYPE_TP);

            form_init_data.layout_type = GUIFORM_LAYOUT_ORDER;
            form_init_data.both_rect = both_rect;
            
            label_init_data.align = GUILABEL_ALIGN_LEFT;
            label_init_data.both_rect = both_rect;
            
            dyna_form.child_handle = SG_CTRL_ID_VIEW_FORM_ID;
            dyna_form.guid = SPRD_GUI_FORM_ID;
            dyna_form.init_data_ptr = &form_init_data;
            dyna_form.is_bg = FALSE;
            dyna_form.is_get_active = TRUE;

            dyna_label.child_handle = SG_CTRL_ID_VIEW_LABEL_ID;
            dyna_label.guid = SPRD_GUI_LABEL_ID;
            dyna_label.init_data_ptr = &label_init_data;
            dyna_label.is_bg = FALSE;
            dyna_label.is_get_active = TRUE;

            // view label
            GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID, &dyna_form);
            GUIFORM_SetStyle(SG_CTRL_ID_VIEW_FORM_ID, GUIFORM_STYLE_UNIT);
            MMI_GetLabelTextByLang(TXT_VIEW, &title);
            GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_VIEW_FORM_ID, &dyna_label);
            GUILABEL_SetText(SG_CTRL_ID_VIEW_LABEL_ID, &title, FALSE);

            if (0 != selected_index)
            {
                // open / close label
                if (selected_index == s_sg_def_pic_index)
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &title);
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &title);
                }
                dyna_form.child_handle = SG_CTRL_ID_SETUP_FORM_ID;
                dyna_label.child_handle = SG_CTRL_ID_SETUP_LABEL_ID;
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID, &dyna_form);
                GUIFORM_SetStyle(SG_CTRL_ID_SETUP_FORM_ID, GUIFORM_STYLE_UNIT);
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SETUP_FORM_ID, &dyna_label);
                GUILABEL_SetText(SG_CTRL_ID_SETUP_LABEL_ID, &title, FALSE);

                // delete label
                dyna_form.child_handle = SG_CTRL_ID_DELETE_FORM_ID;
                dyna_label.child_handle = SG_CTRL_ID_DELETE_LABEL_ID;
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID, &dyna_form);
                GUIFORM_SetStyle(SG_CTRL_ID_DELETE_FORM_ID, GUIFORM_STYLE_UNIT);
                MMI_GetLabelTextByLang(TXT_DELETE, &title);
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_DELETE_FORM_ID, &dyna_label);
                GUILABEL_SetText(SG_CTRL_ID_DELETE_LABEL_ID, &title, FALSE);
                
                // sort label
                dyna_form.child_handle = SG_CTRL_ID_SORT_FORM_ID;
                dyna_label.child_handle = SG_CTRL_ID_SORT_LABEL_ID;
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID, &dyna_form);
                GUIFORM_SetStyle(SG_CTRL_ID_SORT_FORM_ID, GUIFORM_STYLE_UNIT);
                MMI_GetLabelTextByLang(TXT_COMMON_SORT, &title);
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SORT_FORM_ID, &dyna_label);
                GUILABEL_SetText(SG_CTRL_ID_SORT_LABEL_ID, &title, FALSE);
            }
            else if (MMIAPIIM_SGIsUseDefaultSkin() == FALSE)
            {
                // open / close label
                dyna_form.child_handle = SG_CTRL_ID_SETUP_FORM_ID;
                dyna_label.child_handle = SG_CTRL_ID_SETUP_LABEL_ID;
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID, &dyna_form);
                GUIFORM_SetStyle(SG_CTRL_ID_SETUP_FORM_ID, GUIFORM_STYLE_UNIT);
                MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &title);
                GUIFORM_CreatDynaChildCtrl(win_id, SG_CTRL_ID_SETUP_FORM_ID, &dyna_label);
                GUILABEL_SetText(SG_CTRL_ID_SETUP_LABEL_ID, &title, FALSE);
            }

            MMK_SetAtvCtrl(win_id, SG_CTRL_ID_VIEW_FORM_ID);
        }
#else
        sg_OptionWin_index=0;
        CreateSkinOptionMenu(win_id, SG_CTRL_ID_MENU_SKIN_OPTION);
        MMK_SetAtvCtrl(win_id, SG_CTRL_ID_MENU_SKIN_OPTION);
#endif
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_IM_PDA_SUPPORT
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
#else
        sg_OptionWin_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_OPTION) - 1;
#endif
        SelectSkinOptionMenu(ctrl_id);
        break;
#ifndef MMI_IM_PDA_SUPPORT
    case MSG_GET_FOCUS:
        MMK_DestroyDynaCtrl(SG_CTRL_ID_MENU_SKIN_OPTION);
        CreateSkinOptionMenu(win_id, SG_CTRL_ID_MENU_SKIN_OPTION);
        if (GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT) == 1 && MMIAPIIM_SGIsUseDefaultSkin())
        {
            sg_OptionWin_index = 0;
        }
        GUIMENU_SetFirstAndSelectedItem(0, sg_OptionWin_index, SG_CTRL_ID_MENU_SKIN_OPTION);
        MMK_SetAtvCtrl(win_id, SG_CTRL_ID_MENU_SKIN_OPTION);
        break;
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建皮肤选项菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinOptionMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    uint16 selected_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT)-1;
    MMI_STRING_T  str= {0};
    uint16 node_index = 0;
    GUI_BOTH_RECT_T      both_rect = MMITHEME_GetWinClientBothRect(win_id);

    SCI_TraceLow("[SOGOU]CreateSkinOptionMenu end selected_index =%d",selected_index); //yl add 20120920
    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num=4;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl( &create );

    GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&str);
    GUIMENU_SetMenuTitle(&str,ctrl_id);

    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

    MMI_GetLabelTextByLang(TXT_VIEW, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SKIN_OPTION_MENU_VIEW+1, 0, &node_item, ctrl_id);//6_7_1
    node_item.select_icon_id = 0;

    if(0!=selected_index)
    {
        node_index++;
        if (selected_index == s_sg_def_pic_index)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &str);
        }
        node_item.item_text_ptr = &str;
        GUIMENU_InsertNode(node_index, SKIN_OPTION_MENU_2+1, 0, &node_item, ctrl_id);//6_7_1

        node_index++;
        MMI_GetLabelTextByLang(TXT_DELETE, &str);
        node_item.item_text_ptr = &str;
        GUIMENU_InsertNode(node_index, SKIN_OPTION_MENU_DELETE+1, 0, &node_item, ctrl_id);//6_7_1

        node_index++;
        MMI_GetLabelTextByLang(TXT_COMMON_SORT, &str);
        node_item.item_text_ptr = &str;
        GUIMENU_InsertNode(node_index, SKIN_OPTION_MENU_SORT+1, 0, &node_item, ctrl_id);//6_7_1
    }
    else if (MMIAPIIM_SGIsUseDefaultSkin() == FALSE)
    {
        node_index++;
        MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &str);
        node_item.item_text_ptr = &str;
        GUIMENU_InsertNode(node_index, SKIN_OPTION_MENU_2+1, 0, &node_item, ctrl_id);//6_7_1
    }
}

/*****************************************************************************/
//  Description : 删除选择的皮肤文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SGDeleteSelectedSkinFile(uint16* pfileName,uint16 nameLen)
{
    uint16 file_path[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint16 file_path_len = MMIFILE_FILE_NAME_MAX_LEN;
    BOOLEAN result = FALSE;
    uint16 fullNameLen = nameLen + MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_FIND_SUFFIX) - 1;   //*.skin
    uint16* pSuffix = (uint16*)SG_SKIN_FIND_SUFFIX;
    uint16 *pfullName = SCI_ALLOC(fullNameLen*2);
	uint16 temp_size;

    SCI_MEMCPY(pfullName, pfileName, nameLen * 2);
	temp_size=(MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_FIND_SUFFIX) - 1) * 2;
    SCI_MEMCPY(pfullName + nameLen, pSuffix + 1, temp_size);
    if(MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
                               MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                               (const wchar*)(SG_SKIN_DIR),
                               MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_DIR),
                               pfullName,
                               fullNameLen,
                               file_path,
                               &file_path_len))
    {
        if(MMIAPIFMM_IsFileExist(file_path, file_path_len))
        {
            if(SFS_NO_ERROR == MMIAPIFMM_DeleteFile(file_path,PNULL))
            {
                result = TRUE;
            }
        }
    }
    SCI_FREE(pfullName);
    return result; /*lint !e438*/
}

/*****************************************************************************/
//  Description : 删除皮肤确认窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleDeleteSkinConfirmWin(MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id,
                                         DPARAM           param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    {
        uint16 selected_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT) - 1;
        MMI_STRING_T node_str = { 0 };
        MMI_STRING_T note1 = { 0 };
        MMI_STRING_T note2 = { 0 };
        MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_SUCCESS;
		wchar filename[MMIFMM_FULL_FILENAME_LEN +1];
		uint16 filelen = 0;

        GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);

		//MMK_CloseWin(SG_WIN_ID_SKIN_OPTION); caused the "selectedNode->xxx" value changed, should save this value before del skin
		SCI_MEMSET (filename, 0, sizeof (filename));
		filelen = MIN (MMIFMM_FULL_FILENAME_LEN, node_str.wstr_len);
		SCI_MEMCPY (filename, node_str.wstr_ptr, filelen * sizeof (wchar));
			
        MMI_GetLabelTextByLang(TXT_DELETE, &note1);
        MMK_CloseWin(win_id);
        MMK_CloseWin(SG_WIN_ID_SKIN_OPTION);	
        if (SGDeleteSelectedSkinFile(node_str.wstr_ptr, node_str.wstr_len))
        {
            MMI_GetLabelTextByLang(TXT_SUCCESS, &note2);
            
            if (selected_index == s_sg_def_pic_index)
            {
                selected_index = 0;
                GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);
                MMIAPIIM_SGSetupSelectedSkinFile(node_str.wstr_ptr, node_str.wstr_len);
            }
            
            s_sg_skin_select_menu_index = 0;
            MMIAPIIM_SGGetImeSkin();
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_ERROR, &note2);
            image_id = IMAGE_PUBWIN_FAIL;
        }
        MMIPUB_OpenAlertWinByTextPtr(PNULL,
                                     &note1,
                                     &note2,
                                     image_id,
                                     PNULL,
                                     PNULL,
                                     MMIPUB_SOFTKEY_NONE,
                                     PNULL);
        break;
    }

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 选择皮肤选项菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinOptionMenu(MMI_CTRL_ID_T ctrl_id)
{
    uint16 selected_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT)-1;
    BOOLEAN result = FALSE;
    MMI_STRING_T node_str = {0};
    MMI_STRING_T note1 = {0};
    MMI_STRING_T note2 = {0};

    GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);

    SCI_TraceLow("[SOGOU]SelectSkinOptionMenu end selected_index =%d",selected_index); //yl add 20120920
#ifdef MMI_IM_PDA_SUPPORT
    switch (ctrl_id)
#else
    switch (GUIMENU_GetCurNodeId(ctrl_id) - 1)
#endif
    {
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_VIEW_LABEL_ID:
#else
    case SKIN_OPTION_MENU_VIEW: //查看//6_7_1
#endif
        if(0!=selected_index)
        {
            MMK_CreateWin((uint32*)SG_SKIN_PREVIEW_ANIM_WIN_TAB,PNULL);//皮肤预览1.1.1.1.1
        }
        else
        {
            MMK_CreateWin((uint32*)SG_SKIN_PREVIEW_ANIM_WIN2_TAB,PNULL);//皮肤预览1.1.1.1.1
        }
        break;
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_SETUP_LABEL_ID:
#else
    case SKIN_OPTION_MENU_2://6_7_1
#endif
        if (0 == selected_index)
        {
            MMIAPIIM_SGSetupSelectedSkinFile(node_str.wstr_ptr,node_str.wstr_len);
            MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &note1);
            MMI_GetLabelTextByLang(TXT_SUCCESS, &note2);
            s_sg_def_pic_index = selected_index;

            MMIPUB_OpenAlertWinByTextPtr(PNULL, &note1, &note2, IMAGE_PUBWIN_SUCCESS,
                                         PNULL, PNULL, MMIPUB_SOFTKEY_NONE, 0);
            MMIAPIIM_SGGetImeSkin();
            MMK_CloseWin(SG_WIN_ID_SKIN_OPTION);
        }
        else
        {
            if (selected_index != s_sg_def_pic_index)//启用
            {
                if (MMIAPIIM_SGSetupSelectedSkinFile(node_str.wstr_ptr,node_str.wstr_len))
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &note1);
                    MMI_GetLabelTextByLang(TXT_SUCCESS, &note2);
                    s_sg_def_pic_index = selected_index;

                    MMIPUB_OpenAlertWinByTextPtr(PNULL, &note1, &note2, IMAGE_PUBWIN_SUCCESS,
                                                 PNULL, PNULL, MMIPUB_SOFTKEY_NONE, 0);
                    MMIAPIIM_SGGetImeSkin();
                    MMK_CloseWin(SG_WIN_ID_SKIN_OPTION);
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_NO_SUPPORT, &note2);

                    MMIPUB_OpenAlertWinByTextPtr(PNULL, PNULL, &note2, IMAGE_PUBWIN_FAIL,
                                                 PNULL, PNULL, MMIPUB_SOFTKEY_NONE, 0);
                }
            }
            else //关闭
            {
                selected_index = 0;
                GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);
                if (MMIAPIIM_SGSetupSelectedSkinFile(node_str.wstr_ptr,node_str.wstr_len))
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &note1);
                    MMI_GetLabelTextByLang(TXT_SUCCESS, &note2);
                    s_sg_def_pic_index = 0;

                    MMIPUB_OpenAlertWinByTextPtr(PNULL, &note1, &note2, IMAGE_PUBWIN_SUCCESS,
                                                 PNULL, PNULL, MMIPUB_SOFTKEY_NONE, 0);
                    MMIAPIIM_SGGetImeSkin();
                    MMK_CloseWin(SG_WIN_ID_SKIN_OPTION);
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &note1);
                    MMI_GetLabelTextByLang(TXT_ERROR, &note2);

                    MMIPUB_OpenAlertWinByTextPtr(PNULL, &note1, &note2, IMAGE_PUBWIN_FAIL,
                                                 PNULL, PNULL, MMIPUB_SOFTKEY_NONE, 0);
                }
            }
        }
        break;
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_DELETE_LABEL_ID:
#else
    case SKIN_OPTION_MENU_DELETE: //删除//6_7_1
#endif
        MMIPUB_OpenQueryWinByTextId(TXT_SOGOU_DELETE_CONFIRM, IMAGE_PUBWIN_QUERY, PNULL, (MMIPUB_HANDLE_FUNC)HandleDeleteSkinConfirmWin);
        break;
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_SORT_LABEL_ID:
#else
    case SKIN_OPTION_MENU_SORT: //排序//6_7_1
#endif
        MMK_CreateWin((uint32*)SG_SKIN_SORT_WIN_TAB,PNULL);//皮肤排序
        break;
    default:
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 皮肤排序窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinSortWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_SKIN_SORT;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateSkinSortMenu(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        SelectSkinSortMenu(ctrl_id);
        s_sg_is_skin_port_run = TRUE;
        MMK_CloseWin( win_id );
        MMK_CloseWin( SG_WIN_ID_SKIN_OPTION );
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建皮肤排序菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinSortMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    uint16 selected_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT)-1;
    MMI_STRING_T  str= {0};
    uint16 node_index = 0;
    GUI_BOTH_RECT_T      both_rect = MMITHEME_GetWinClientBothRect(win_id);

    SCI_TraceLow("[SOGOU]CreateSkinSortMenu GUIMENU_GetCurNodeId end selected_index =%d",selected_index); //yl add 20120920
    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num=2;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl( &create );

    GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&str);
    GUIMENU_SetMenuTitle(&str,ctrl_id);

    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);

    if (0 != selected_index)
    {
        MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_SORT_BY_TIME, &str);
        node_item.item_text_ptr = &str;
        if (g_sg_pic_sort_type == MMISG_SORT_TYPE_TIME)
        {
            node_item.select_icon_id = IMAGE_RADIO_SELECTED_ICON;
        }
        else
        {
            node_item.select_icon_id = IMAGE_RADIO_UNSELECTED_ICON;
        }
        GUIMENU_InsertNode(node_index, node_index+1, 0, &node_item, ctrl_id);

        node_index++;
        MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_SORT_BY_NAME, &str);
        node_item.item_text_ptr = &str;
        if (g_sg_pic_sort_type == MMISG_SORT_TYPE_NAME)
        {
            node_item.select_icon_id = IMAGE_RADIO_SELECTED_ICON;
        }
        else
        {
            node_item.select_icon_id = IMAGE_RADIO_UNSELECTED_ICON;
        }
        GUIMENU_InsertNode(node_index, node_index+1, 0, &node_item, ctrl_id);
    }
}

/*****************************************************************************/
//  Description : 选择皮肤排序菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSkinSortMenu(MMI_CTRL_ID_T ctrl_id)
{
    if (GUIMENU_GetCurNodeId(ctrl_id) == 1)
    {
        g_sg_pic_sort_type = MMISG_SORT_TYPE_TIME;
    }
    else
    {
        g_sg_pic_sort_type = MMISG_SORT_TYPE_NAME;
    }
    MMI_WriteNVItem(IMNV_SOGOU_SKIN_SORT_TYPE, &g_sg_pic_sort_type);

    return TRUE;
}

/*****************************************************************************/
//  Description : 皮肤预览窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinPreViewAnimWindow(
                                                MMI_WIN_ID_T    win_id,
                                                MMI_MESSAGE_ID_E   msg_id,
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_ANIM_SKIN_PREVIEW;
    uint16 selected_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT)-1;
    uint16 temp_size;
    
    SCI_TraceLow("[SOGOU]HandleSkinPreViewAnimWindow GUIMENU_GetCurNodeId end selected_index =%d",selected_index); //yl add 20120920

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 file_path[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
            uint16 file_path_len = MMIFILE_FILE_NAME_MAX_LEN;
            uint16 fullNameLen = 0;
            uint16* pSuffix = (uint16*)SG_SKIN_FIND_SUFFIX;
            uint16 *pfullName = PNULL;
            MMI_STRING_T node_str = {0};

            GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);
            fullNameLen = node_str.wstr_len + MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_FIND_SUFFIX)-1;  //*.skin
            pfullName = SCI_ALLOC(fullNameLen*2);

            SCI_TraceLow("[SOGOU]HandleSkinPreViewAnimWindow case MSG_OPEN_WINDOW: end selected_index =%d",selected_index); //yl add 20120920
            if(0!=selected_index)
            {
                SCI_MEMCPY(pfullName, node_str.wstr_ptr, node_str.wstr_len * 2);
				temp_size=(MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_FIND_SUFFIX) - 1) * 2;
                SCI_MEMCPY(pfullName + node_str.wstr_len, pSuffix + 1, temp_size);
                MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
                                        MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                                        (const wchar*)SG_SKIN_DIR,
                                        MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_DIR),
                                        pfullName,
                                        fullNameLen,
                                        file_path,
                                        &file_path_len);
                MMIAPIFMM_SetPreviewPicParam(FALSE,file_path,file_path_len,ctrl_id);
                //result = MMIAPIFMM_SetPreviewPicParam(FALSE,full_path_name_ptr,full_path_len,MMIFMM_SEL_PICTURE_PREVIEW_CTRL_ID);
            }
            if(0!=selected_index)
            {
                if (selected_index != s_sg_def_pic_index)
                {
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_COMMON_STARTUP,NULL,STXT_RETURN,TRUE);
                }
                else
                {
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_CLOSE,NULL,STXT_RETURN,TRUE);
                }
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,NULL,NULL,STXT_RETURN,TRUE);
            }
            MMK_SetAtvCtrl(win_id,ctrl_id);
            SCI_FREE(pfullName);
        } /*lint !e438*/
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            MMI_STRING_T node_str = {0};
            MMI_STRING_T note1 = {0};
            MMI_STRING_T note2 = {0};

            GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);

            if(0!=selected_index)
            {
                if (selected_index != s_sg_def_pic_index)//启用
                {
                    if (MMIAPIIM_SGSetupSelectedSkinFile(node_str.wstr_ptr,node_str.wstr_len))
                    {
                        MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &note1);
                        MMI_GetLabelTextByLang(TXT_SUCCESS, &note2);
                        s_sg_def_pic_index = selected_index;

                        MMIPUB_OpenAlertWinByTextPtr(
                            PNULL,
                            &note1,
                            &note2,
                            IMAGE_PUBWIN_SUCCESS,
                            PNULL,PNULL,
                            MMIPUB_SOFTKEY_NONE,
                            0);
                        MMIAPIIM_SGGetImeSkin();
                    }
                    else
                    {
                        MMI_GetLabelTextByLang(TXT_COMMON_NO_SUPPORT, &note2);
                        MMIPUB_OpenAlertWinByTextPtr(
                            PNULL,
                            PNULL,
                            &note2,
                            IMAGE_PUBWIN_FAIL,
                            PNULL,PNULL,
                            MMIPUB_SOFTKEY_NONE,
                            0);
                    }
                }
                else
                {
                    selected_index = 0;
                    GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);
                    if (MMIAPIIM_SGSetupSelectedSkinFile(node_str.wstr_ptr,node_str.wstr_len))
                    {
                        MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &note1);
                        MMI_GetLabelTextByLang(TXT_SUCCESS, &note2);
                        s_sg_def_pic_index = 0;

                        MMIPUB_OpenAlertWinByTextPtr(
                            PNULL,
                            &note1,
                            &note2,
                            IMAGE_PUBWIN_SUCCESS,
                            PNULL,PNULL,
                            MMIPUB_SOFTKEY_NONE,
                            0);
                        MMIAPIIM_SGGetImeSkin();
                    }
                    else
                    {
                        MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &note1);
                        MMI_GetLabelTextByLang(TXT_ERROR, &note2);

                        MMIPUB_OpenAlertWinByTextPtr(
                            PNULL,
                            &note1,
                            &note2,
                            IMAGE_PUBWIN_FAIL,
                            PNULL,PNULL,
                            MMIPUB_SOFTKEY_NONE,
                            0);
                    }
                }
            }
        }
        MMK_CloseWin( win_id );
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 皮肤预览窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinPreViewAnimWindow2(
                                                MMI_WIN_ID_T    win_id,
                                                MMI_MESSAGE_ID_E   msg_id,
                                                DPARAM             param
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T         dis_point = {0};
    uint16 selected_index = GUIMENU_GetCurNodeId(SG_CTRL_ID_MENU_SKIN_SELECT)-1;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRectEx(win_id);
    MMI_STRING_T    node_str = {0};

    GUIMENU_GetNodeStr(SG_CTRL_ID_MENU_SKIN_SELECT,selected_index,&node_str);

    SCI_TraceLow("[SOGOU]HandleSkinPreViewAnimWindow2 GUIMENU_GetCurNodeId end selected_index =%d",selected_index); //yl add 20120920
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        if(0==selected_index)
        {
            uint16 width;
            uint16 height;
            GUIRES_GetImgWidthHeight(&width, &height, IMAGE_SOGOU_DEFAULT_SKIN, SG_WIN_ID_SKIN_PREVIEW);
            rect.bottom = rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
            LCD_FillRect(&lcd_dev_info, rect, 0);
            dis_point.x = 0;
            dis_point.y = (rect.bottom - height) / 2;
            GUIRES_DisplayImg(&dis_point, PNULL, PNULL, win_id, IMAGE_SOGOU_DEFAULT_SKIN, &lcd_dev_info);
            GUIWIN_SetSoftkeyTextId(win_id, (MMIAPIIM_SGIsUseDefaultSkin() == FALSE) ? TXT_COMMON_STARTUP : NULL,
                                    NULL, STXT_RETURN, TRUE);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (MMIAPIIM_SGIsUseDefaultSkin() == FALSE)
        {
            MMI_STRING_T note1 = {0};
            MMI_STRING_T note2 = {0};

            MMIAPIIM_SGSetupSelectedSkinFile(node_str.wstr_ptr,node_str.wstr_len);
            MMI_GetLabelTextByLang(TXT_COMMON_STARTUP, &note1);
            MMI_GetLabelTextByLang(TXT_SUCCESS, &note2);
            s_sg_def_pic_index = selected_index;

            MMIPUB_OpenAlertWinByTextPtr(
                PNULL,
                &note1,
                &note2,
                IMAGE_PUBWIN_SUCCESS,
                PNULL,PNULL,
                MMIPUB_SOFTKEY_NONE,
                0
            );
            MMIAPIIM_SGGetImeSkin();
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 皮肤浏览窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSkinBrowseWindow(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_MENU_SKIN_BROWSE;
    uint16 selected_index =0;
    LOCAL BOOLEAN is_open = FALSE;

    //SOGOU_TRACE_OUT:"HandleSkinBrowseWindow win_id=0x%04x,msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_2432_112_2_18_2_29_39_322,(uint8*)"dd",win_id,msg_id);

    //参数校验
    if(s_skin_ptr == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] HandleSkinBrowseWindow ERR s_skin_ptr");
        return MMI_RESULT_FALSE;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (s_skin_ptr)
        {
            SCI_MEMSET(s_skin_ptr, 0, sizeof(SkinInf));
        }
        CreateSkinBrowseMenu(win_id,ctrl_id,0);
        is_open=TRUE;
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        SCI_TraceLow("[SOGOU]HandleSkinBrowseWindow case MSG_APP_OK: begin selected_index =%d",selected_index); //yl add 20120920
        selected_index = GUIMENU_GetCurNodeId(ctrl_id)-1;
        SCI_TraceLow("[SOGOU]HandleSkinBrowseWindow case MSG_APP_OK: end selected_index =%d",selected_index); //yl add 20120920
        if (0 == MMIAPIIM_parse_skinbin_get_type(s_skin_ptr->startpos + selected_index, s_skin_list_file))
        {
            uint16 skin_file_fullpath_len;
            uint8 temp_char[40] = { 0 };

            SCI_MEMSET(s_skin_ptr->pic_name,0,sizeof(s_skin_ptr->pic_name));
            MMIAPIIM_parse_skinbin_get_string(s_skin_ptr->startpos + selected_index,
                                     (unsigned short*)s_skin_ptr->pic_name,
                                     (unsigned short*)temp_char, 
                                     s_skin_list_file);
            MMIAPICOM_WstrToStr((wchar*)temp_char, (uint8*)s_skin_ptr->picID);
            MMIAPICOM_Wstrcat((wchar*)s_skin_ptr->pic_name, (const wchar*)(L".jpg"));
            //MMK_CloseWin( win_id );
            //MMK_SendMsg( SG_WIN_ID_UPDATE_WATCHING, (MMI_MESSAGE_ID_E)MSG_FULL_PAINT, NULL);
            SCI_MEMSET(s_sg_download_skin_file_full_path,0,sizeof(s_sg_download_skin_file_full_path));
            skin_file_fullpath_len = MMIFILE_FILE_NAME_MAX_LEN+1;

            if(MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD),
                                    MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                                    (const wchar*)SG_SKIN_DIR,
                                    MMIAPICOM_Wstrlen((const wchar*)SG_SKIN_DIR),
                                    s_skin_ptr->pic_name,
                                    MMIAPICOM_Wstrlen((const wchar*)s_skin_ptr->pic_name),
                                    s_sg_download_skin_file_full_path,
                                    &skin_file_fullpath_len))
            {
                if(!MMIAPIFMM_IsFileExist(s_sg_download_skin_file_full_path, skin_file_fullpath_len))
				{
					uint16 pic_name_len = 2 * MMIAPICOM_Wstrlen((const wchar*)s_skin_ptr->pic_name);
					SCI_MEMSET(s_sg_download_skin_file_name, 0, sizeof(s_sg_download_skin_file_name));
					SCI_MEMCPY(s_sg_download_skin_file_name,
							   s_skin_ptr->pic_name,
							   pic_name_len);
					s_sg_is_download_skin_file = TRUE;
					s_update_type = SG_UPDATE_TYPE_SKIN_FILE;
					if(MMK_IsOpenWin(SG_WIN_ID_UPDATE_WATCHING))
					{
						MMK_WinGrabFocus(SG_WIN_ID_UPDATE_WATCHING);
						MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_CREATING_SOCKET);
						MMIAPIIM_SGUpdateCallBackFunc();
					}
					else
					{
						EnterUpdateWindow();
					}
	            }
				else
				{
                    MMI_STRING_T note = { 0 };
					MMI_GetLabelTextByLang(TXT_SOGOU_ALREADY_EXISTING, &note);
					MMIPUB_OpenAlertWinByTextPtr(
						PNULL,
						&note,
						PNULL,
						IMAGE_PUBWIN_WARNING,
						PNULL,PNULL,
						MMIPUB_SOFTKEY_NONE,
						0);
				}
	         }
        }
        else
        {
            if (is_open)
            {
                s_skin_ptr->index = selected_index + 1;
            }
            else
            {
                s_skin_ptr->index = s_skin_ptr->startpos + selected_index;
            }
            MMK_DestroyDynaCtrl(ctrl_id);
            s_skin_ptr->dirLevelIndex[s_skin_ptr->level] = selected_index;
            s_skin_ptr->level++;
            CreateSkinBrowseMenu(win_id, ctrl_id, s_skin_ptr->index);
            is_open = FALSE;
            s_skin_ptr->dirLevel[s_skin_ptr->level] = s_skin_ptr->index;
        }
        //MMK_CloseWin( win_id );
        break;
    case MSG_GET_FOCUS:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(0==s_skin_ptr->index)
        {
            s_skin_ptr->dirLevelIndex[s_skin_ptr->level] = 0;
            MMK_CloseWin(win_id);
            MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_NONE);
            MMK_CloseWin(SG_WIN_ID_UPDATE_WATCHING);
        }
        else
        {
            s_skin_ptr->dirLevel[s_skin_ptr->level] = 0;
            s_skin_ptr->dirLevelIndex[s_skin_ptr->level] = 0;
            s_skin_ptr->level--;
            s_skin_ptr->index = s_skin_ptr->dirLevel[s_skin_ptr->level];
            MMK_DestroyDynaCtrl(ctrl_id);
            CreateSkinBrowseMenu(win_id,ctrl_id,s_skin_ptr->index);
        }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建皮肤浏览菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSkinBrowseMenu(
                                   MMI_WIN_ID_T win_id ,
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint16 index_num //第几个索引查找
                                   )
{
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    MMI_STRING_T  str= {0};
    uint16 node_index = 0;
    uint8 i=0;
    uint8 itemN;
    wchar itemStr[SG_MAX_MENU_ITEM_COUNT][50]={0};
    GUI_BOTH_RECT_T      both_rect = MMITHEME_GetWinClientBothRect(win_id);

    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num=SG_MAX_MENU_ITEM_COUNT;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl( &create );
    s_skin_ptr->index=index_num;
    itemN = MMIAPIIM_parse_skinbin_get_count(s_skin_ptr->index, &s_skin_ptr->startpos, s_skin_list_file);
    if (0 < itemN && itemN < SG_MAX_MENU_ITEM_COUNT)
    {
        if (0 == s_skin_ptr->index)
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_DIR, &str);
        }
        else if (1 == MMIAPIIM_parse_skinbin_get_type(s_skin_ptr->startpos, s_skin_list_file))
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_SUB_DIR, &str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_LIST, &str);
        }
        GUIMENU_SetMenuTitle(&str,ctrl_id);
        GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#ifdef MMI_IM_PDA_SUPPORT
        node_item.select_icon_id = IMAGE_NULL;
#else
        node_item.select_icon_id = 0;
#endif
        for (i = 0; i < itemN; i++)
        {
            MMIAPIIM_parse_skinbin_get_string(s_skin_ptr->startpos + i, itemStr[i], NULL, s_skin_list_file);
            str.wstr_len = MMIAPICOM_Wstrlen((const wchar*)itemStr[i]);//*.jpg
            str.wstr_ptr = itemStr[i];
            node_item.item_text_ptr = &str;
            GUIMENU_InsertNode(node_index, node_index+1, 0, &node_item, ctrl_id);
            node_index++;
        }
        i = s_skin_ptr->dirLevelIndex[s_skin_ptr->level];
        if (i < itemN)
        {
            GUIMENU_SetFirstAndSelectedItem(0,i,ctrl_id);
        }
        else
        {
            GUIMENU_SetFirstAndSelectedItem(0,0,ctrl_id);
        }
        MMK_SendMsg( win_id, (MMI_MESSAGE_ID_E)MSG_FULL_PAINT, NULL);
    }
}
#endif
/*****************************************************************************/
//  Description : 更新设置窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUpdateSettingsMenuWindow(
                                                  MMI_WIN_ID_T    win_id,
                                                  MMI_MESSAGE_ID_E   msg_id,
                                                  DPARAM             param
                                                  )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_MENU_UPDATE_SETTINGS;
    MMI_STRING_T title = { 0 };

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        GUIWIN_SetTitleTextId(win_id, TXT_SOGOU_UPDATE_SETTINGS, FALSE);
        GUIFORM_SetType(SG_CTRL_ID_UPDATE_SETTING_MENU_FORM_ID, GUIFORM_TYPE_TP);
        
        GUIFORM_SetStyle(SG_CTRL_ID_SYSTEM_SETTING_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_SYSTEM_UPDATE_SETTINGS, &title);
        GUILABEL_SetText(SG_CTRL_ID_SYSTEM_SETTING_LABEL_ID, &title, FALSE);
        
        GUIFORM_SetStyle(SG_CTRL_ID_AUTOUPDATE_SETTING_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_AUTOMATIC_UPDATE_SETTINGS, &title);
        GUILABEL_SetText(SG_CTRL_ID_AUTOUPDATE_SETTING_LABEL_ID, &title, FALSE);

        MMK_SetAtvCtrl(win_id, SG_CTRL_ID_SYSTEM_SETTING_FORM_ID);

#else
        CreateUpdateSettingsMenu(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_IM_PDA_SUPPORT
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
#endif
        SelectUpdateSettingsMenu(ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建更新设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateUpdateSettingsMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_CONTROL_CREATE_T create = {0};
    GUIMENU_INIT_DATA_T init_data = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    MMI_STRING_T  str= {0};
    uint16 node_index = 0;
    GUI_BOTH_RECT_T      both_rect = MMITHEME_GetWinClientBothRect(win_id);

    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num=2;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl( &create );

    MMI_GetLabelTextByLang(TXT_SOGOU_UPDATE_SETTINGS, &str);
    GUIMENU_SetMenuTitle(&str,ctrl_id);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    node_item.select_icon_id = 0;

    MMI_GetLabelTextByLang(TXT_SOGOU_SYSTEM_UPDATE_SETTINGS, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SG_UPDATE_SETTINGS_MENU_SYSTEM_SETTINGS+1, 0, &node_item, ctrl_id);//系统更新设置//6_7_1

    node_index++;
    MMI_GetLabelTextByLang(TXT_SOGOU_AUTOMATIC_UPDATE_SETTINGS, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SG_UPDATE_SETTINGS_MENU_AUTOMATIC_SETTINGS+1, 0, &node_item, ctrl_id);//自动更新设置//6_7_1
}

/*****************************************************************************/
//  Description : 选择更新设置菜单
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectUpdateSettingsMenu(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN result = TRUE;

#ifdef MMI_IM_PDA_SUPPORT
    switch (ctrl_id)
#else
    switch (GUIMENU_GetCurNodeId(ctrl_id) - 1)
#endif
    {
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_SYSTEM_SETTING_LABEL_ID:
#else
    case SG_UPDATE_SETTINGS_MENU_SYSTEM_SETTINGS:     //系统更新设置//6_7_1
#endif
        MMK_CreateWin((uint32*)SG_SYSTEM_UPDATE_SETTINGS_LIST_WIN_TAB,PNULL);
        break;

#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_AUTOUPDATE_SETTING_LABEL_ID:
#else
    case SG_UPDATE_SETTINGS_MENU_AUTOMATIC_SETTINGS:     //自动更新设置//6_7_1
#endif
        MMK_CreateWin((uint32*)SG_AUTOMATIC_UPDATE_SETTINGS_LIST_WIN_TAB,PNULL);
        break;
    
    default:
        result = FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : 系统更新设置窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSystemUpdateSettingsListWindow(
                                                        MMI_WIN_ID_T    win_id,
                                                        MMI_MESSAGE_ID_E   msg_id,
                                                        DPARAM             param
                                                        )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_LIST_SYSTEM_UPDATE_SETTINGS;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateSystemUpdateSettingsList(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

	//not close this window, because when return from "open/close" need to reserve, so when return, the value need to refresh	
	//yaoguang.chen@sprd 2012-11-08
	case MSG_GET_FOCUS:
	{
		GUILIST_ITEM_T* item_ptr = PNULL;

		//here, the index is binded with "CreateSystemUpdateSettingsList"
		//update notify
		item_ptr = GUILIST_GetItemPtrByIndex(SG_CTRL_ID_LIST_SYSTEM_UPDATE_SETTINGS, 0);
		if (item_ptr && item_ptr->item_data_ptr)
		{
            if (GUIITEM_DATA_TEXT_BUFFER == item_ptr->item_data_ptr->item_content[2].item_data_type)
            {
                SCI_FREE(item_ptr->item_data_ptr->item_content[2].item_data.text_buffer.wstr_ptr);
            }

			if (g_sogou_update_settings_data.is_need_update_notif)  
		    {
                item_ptr->item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_ptr->item_data_ptr->item_content[2].item_data.text_id = TXT_COMMON_OPEN;
		        //MMI_GetLabelTextByLang(TXT_COMMON_OPEN, &(item_ptr->item_data_ptr->item_content[2].item_data.text_buffer));

		        item_ptr->item_data_ptr->item_content[0].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
		    }
		    else
		    {
                item_ptr->item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_ptr->item_data_ptr->item_content[2].item_data.text_id = TXT_COMMON_CLOSE;
		        //MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &(item_ptr->item_data_ptr->item_content[2].item_data.text_buffer));

		        item_ptr->item_data_ptr->item_content[0].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
		    }
		}

		//connect remind
		item_ptr = GUILIST_GetItemPtrByIndex(SG_CTRL_ID_LIST_SYSTEM_UPDATE_SETTINGS, 1);
		if (item_ptr && item_ptr->item_data_ptr)
	    {
            if (GUIITEM_DATA_TEXT_BUFFER == item_ptr->item_data_ptr->item_content[2].item_data_type)
            {
                SCI_FREE(item_ptr->item_data_ptr->item_content[2].item_data.text_buffer.wstr_ptr);
            }
		    if (g_sogou_update_settings_data.is_need_connect_remind)    
		    {
                item_ptr->item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_ptr->item_data_ptr->item_content[2].item_data.text_id = TXT_COMMON_OPEN;
		        //MMI_GetLabelTextByLang(TXT_COMMON_OPEN, &(item_ptr->item_data_ptr->item_content[2].item_data.text_buffer));

                item_ptr->item_data_ptr->item_content[0].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
		    }
		    else
		    {
                item_ptr->item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;
                item_ptr->item_data_ptr->item_content[2].item_data.text_id = TXT_COMMON_CLOSE;
		        //MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &(item_ptr->item_data_ptr->item_content[2].item_data.text_buffer));

		        item_ptr->item_data_ptr->item_content[0].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
		    }
		}

		break;
	}
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_KEYUP_WEB:
        SelectSystemUpdateSettingsList(ctrl_id);
        //MMK_CloseWin(win_id);			//not close this window, because when return from "open/close" need to reserve.
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建系统更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateSystemUpdateSettingsList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_INIT_DATA_T init_data = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.type = GUILIST_TEXTLIST_E;
    init_data.is_dynamic_item_data = TRUE;

    GUIWIN_SetTitleTextId( win_id, TXT_SOGOU_SYSTEM_UPDATE_SETTINGS, FALSE );

    GUILIST_SetMaxItem(ctrl_id,2, FALSE);

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;//IMAGE_CHECK_UNSELECTED_ICON;//IMAGE_SELECT_TICK_OFF_PIC;
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;

    //词库更新提示
    item_data.item_content[1].item_data.text_id = TXT_SOGOU_VOCABULARY_UPDATE_NOTIFICATION;

    if (g_sogou_update_settings_data.is_need_update_notif)  //词库更新提示
    {
        item_data.item_content[2].item_data.text_id = TXT_COMMON_OPEN;
        item_data.item_content[0].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        item_data.item_content[2].item_data.text_id = TXT_COMMON_CLOSE;
        item_data.item_content[0].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    GUILIST_AppendItem( ctrl_id, &item_t );

    //连接提醒
    item_data.item_content[1].item_data.text_id = TXT_SOGOU_CONNECT_REMIND;

    if (g_sogou_update_settings_data.is_need_connect_remind)    //连接提醒
    {
        item_data.item_content[2].item_data.text_id = TXT_COMMON_OPEN;
        item_data.item_content[0].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        item_data.item_content[2].item_data.text_id = TXT_COMMON_CLOSE;
        item_data.item_content[0].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    GUILIST_AppendItem( ctrl_id, &item_t );
}

/*****************************************************************************/
//  Description : 选择系统更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectSystemUpdateSettingsList(MMI_CTRL_ID_T ctrl_id)
{
    switch (GUILIST_GetCurItemIndex(ctrl_id))
    {
    case 0:             //词库更新提示
        MMK_CreateWin((uint32*)SG_VOCABULARY_UPDATE_NOTIFICATION_LIST_WIN_TAB,PNULL);//系统更新设置
        break;
    case 1:             //连接提醒
        MMK_CreateWin((uint32*)SG_CONNECT_REMIND_LIST_WIN_TAB,PNULL);//系统更新设置
        break;
    default:
        break;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 词库更新提示窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateNotificationListWindow(
                                                                MMI_WIN_ID_T    win_id,
                                                                MMI_MESSAGE_ID_E   msg_id,
                                                                DPARAM             param
                                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_LIST_VOCABULARY_UPDATE_NOTIFICATION;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateVocabularyUpdateNotificationsList(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        SelectVocabularyUpdateNotificationList(ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建词库更新提示列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateVocabularyUpdateNotificationsList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_INIT_DATA_T init_data = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.type = GUILIST_RADIOLIST_E;
    init_data.is_dynamic_item_data = TRUE;

    GUILIST_SetMaxItem(ctrl_id,2, FALSE);//max item 2

    GUIWIN_SetTitleTextId( win_id, TXT_SOGOU_VOCABULARY_UPDATE_NOTIFICATION, FALSE );

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    //开启
    MMI_GetLabelTextByLang(TXT_COMMON_OPEN, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );

    //关闭
    MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );

    if (g_sogou_update_settings_data.is_need_update_notif)
    {
        GUILIST_SetSelectedItem(ctrl_id,0,1);
    }
    else
    {
        GUILIST_SetSelectedItem(ctrl_id,1,1);
    }
}

/*****************************************************************************/
//  Description : 选择词库更新提示列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectVocabularyUpdateNotificationList(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
    MMI_STRING_T note1 = {0};
    MMI_STRING_T note2 = {0};

    if (index==0) //开启
    {
        MMI_GetLabelTextByLang(TXT_COMMON_OPEN, &note1);
        g_sogou_update_settings_data.is_need_update_notif = TRUE;
    }
    else//关闭
    {
        MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &note1);
        g_sogou_update_settings_data.is_need_update_notif = FALSE;
    }
    MMI_WriteNVItem(IMNV_SOGOU_UPDATE_SETTING, &g_sogou_update_settings_data);

    MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_NOTIFICATION, &note2);

    MMIPUB_OpenAlertWinByTextPtr(
        PNULL,
        &note1,
        &note2,
        IMAGE_PUBWIN_SUCCESS,
        PNULL,PNULL,
        MMIPUB_SOFTKEY_NONE,
        HandleVocabularyUpdateNotificationOpenAlertWinWindow
        );

    return TRUE;
}

/*****************************************************************************/
//  Description : 词库更新提示窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateNotificationOpenAlertWinWindow(
                                                                        MMI_WIN_ID_T    win_id,
                                                                        MMI_MESSAGE_ID_E   msg_id,
                                                                        DPARAM             param
                                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {

    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(SG_WIN_ID_VOCABULARY_UPDATE_NOTIFICATION);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : 连接提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectRemindListWindow(
                                                 MMI_WIN_ID_T    win_id,
                                                 MMI_MESSAGE_ID_E   msg_id,
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_LIST_CONNECT_REMIND;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateConnectRemindList(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        SelectConnectRemindList(ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建连接提醒列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateConnectRemindList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_INIT_DATA_T init_data = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.type = GUILIST_RADIOLIST_E;
    init_data.is_dynamic_item_data = TRUE;

    GUILIST_SetMaxItem(ctrl_id,2, FALSE);

    GUIWIN_SetTitleTextId(win_id,TXT_SOGOU_CONNECT_REMIND,FALSE);

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    //开启
    MMI_GetLabelTextByLang(TXT_COMMON_OPEN, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );

    //关闭
    MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );

    if (g_sogou_update_settings_data.is_need_connect_remind)
    {
        GUILIST_SetSelectedItem(ctrl_id,0,1);
    }
    else
    {
        GUILIST_SetSelectedItem(ctrl_id,1,1);
    }
}

/*****************************************************************************/
//  Description : 选择连接提醒
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectConnectRemindList(MMI_CTRL_ID_T ctrl_id)
{
    uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
    MMI_STRING_T note1 = {0};
    MMI_STRING_T note2 = {0};

    if (index==0) //开启
    {
        MMI_GetLabelTextByLang(TXT_COMMON_OPEN, &note1);
        g_sogou_update_settings_data.is_need_connect_remind = TRUE;
    }
    else//关闭
    {
        MMI_GetLabelTextByLang(TXT_COMMON_CLOSE, &note1);
        g_sogou_update_settings_data.is_need_connect_remind = FALSE;
    }
    MMI_WriteNVItem(IMNV_SOGOU_UPDATE_SETTING, &g_sogou_update_settings_data);

    MMI_GetLabelTextByLang(TXT_SOGOU_CONNECT_REMIND, &note2);

    MMIPUB_OpenAlertWinByTextPtr(
        PNULL,
        &note1,
        &note2,
        IMAGE_PUBWIN_SUCCESS,
        PNULL,PNULL,
        MMIPUB_SOFTKEY_NONE,
        HandleConnectRemindOpenAlertWinWindow
        );

    return TRUE;
}

/*****************************************************************************/
//  Description : 连接提醒提示窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConnectRemindOpenAlertWinWindow(
                                                         MMI_WIN_ID_T    win_id,
                                                         MMI_MESSAGE_ID_E   msg_id,
                                                         DPARAM             param
                                                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(SG_WIN_ID_CONNECT_REMIND);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : 自动更新设置窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutomaticUpdateSettingsListWindow(
                                                           MMI_WIN_ID_T    win_id,
                                                           MMI_MESSAGE_ID_E   msg_id,
                                                           DPARAM             param
                                                           )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_LIST_AUTOMATIC_UPDATE_SETTINGS;
    MMI_STRING_T title = { 0 };

    //SOGOU_TRACE_OUT:"HandleAutomaticUpdateSettingsListWindow msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_3198_112_2_18_2_29_41_324,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        GUIWIN_SetTitleTextId(win_id, TXT_SOGOU_AUTOMATIC_UPDATE_SETTINGS, FALSE);
        GUIFORM_SetType(SG_CTRL_ID_AUTOUPDATE_SETTING_MENU_FORM_ID, GUIFORM_TYPE_TP);
        
        GUIFORM_SetStyle(SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_SETTING_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_FREQUENCY, &title);
        GUILABEL_SetText(SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_LABEL_ID, &title, FALSE);
        
        GUIFORM_SetStyle(SG_CTRL_ID_VOCABULARY_UPDATE_TIME_MENU_FORM_ID, GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_TIME, &title);
        GUILABEL_SetText(SG_CTRL_ID_VOCABULARY_UPDATE_TIME_LABEL_ID, &title, FALSE);

        MMK_SetAtvCtrl(win_id, SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_SETTING_FORM_ID);

#else
        CreateAutomaticUpdateSettingsList(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef MMI_IM_PDA_SUPPORT
        if (MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
        }
        else
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
#endif
        SelectAutomaticUpdateSettingsList(ctrl_id);
        //MMK_CloseWin(win_id);
        break;
    case MSG_GET_FOCUS:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建自动更新设置list
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateAutomaticUpdateSettingsList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    MMI_CONTROL_CREATE_T create = { 0 };
    GUIMENU_INIT_DATA_T init_data = { 0 };
    GUIMENU_DYNA_ITEM_T node_item = { 0 };
    MMI_STRING_T  str= { 0 };
    uint16 node_index = 0;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    init_data.is_static = FALSE;
    init_data.both_rect = both_rect;
    init_data.node_max_num = 2;
    init_data.menu_style = GUIMENU_STYLE_SECOND;

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_MENU_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    MMK_CreateControl(&create);

    MMI_GetLabelTextByLang(TXT_SOGOU_AUTOMATIC_UPDATE_SETTINGS, &str);
    GUIMENU_SetMenuTitle(&str, ctrl_id);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
    node_item.select_icon_id = 0;

    MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_FREQUENCY, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SG_UPDATE_AUTOMATIC_SETTINGS_MENU_VOCABULARY_UPDATE_FREQUENCY + 1, 0, &node_item, ctrl_id);//6_7_1

    node_index++;
    MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_TIME, &str);
    node_item.item_text_ptr = &str;
    GUIMENU_InsertNode(node_index, SG_UPDATE_AUTOMATIC_SETTINGS_MENU_VOCABULARY_UPDATE_TIME + 1, 0, &node_item, ctrl_id);//6_7_1
}

/*****************************************************************************/
//  Description : 选择自动更新设置
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectAutomaticUpdateSettingsList(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN result = TRUE;

#ifdef MMI_IM_PDA_SUPPORT
    switch (ctrl_id)
#else
    switch (GUIMENU_GetCurNodeId(ctrl_id) - 1)
#endif
    {
     // 词库更新频率
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_LABEL_ID:
#else
    case SG_UPDATE_AUTOMATIC_SETTINGS_MENU_VOCABULARY_UPDATE_FREQUENCY://6_7_1
#endif
        MMK_CreateWin((uint32*)SG_VOCABULARY_UPDATE_FREQUENCY_LIST_WIN_TAB,PNULL);
        break;

     // 自动更新设置
#ifdef MMI_IM_PDA_SUPPORT
    case SG_CTRL_ID_VOCABULARY_UPDATE_TIME_LABEL_ID:
        MMK_CreatePubFormWin((uint32*)SG_VOCABULARY_UPDATE_TIME_WIN_TAB, PNULL);
#else
    case SG_UPDATE_AUTOMATIC_SETTINGS_MENU_VOCABULARY_UPDATE_TIME://6_7_1
        MMK_CreateWin((uint32*)SG_VOCABULARY_UPDATE_TIME_WIN_TAB,PNULL);
#endif
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 词库更新频率窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateFrequencyListWindow(
                                                              MMI_WIN_ID_T    win_id,
                                                              MMI_MESSAGE_ID_E   msg_id,
                                                              DPARAM             param
                                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_LIST_VOCABULARY_UPDATE_FREQUENCY;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateVocabularyUpdateFrequencyList(win_id,ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        SelectVocabularyUpdateFrequencyList(ctrl_id);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 创建词库更新频率列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateVocabularyUpdateFrequencyList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_INIT_DATA_T init_data = {0};
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.type = GUILIST_RADIOLIST_E;
    init_data.is_dynamic_item_data = TRUE;

    GUILIST_SetMaxItem(ctrl_id,VOCABULARY_UPDATE_FREQUENCY_MAX, FALSE);

    GUIWIN_SetTitleTextId( win_id, TXT_SOGOU_VOCABULARY_UPDATE_FREQUENCY, FALSE );

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    //每天
    MMI_GetLabelTextByLang(TXT_ALARM_MODE_EVERYDAY, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );

    //每7天
    MMI_GetLabelTextByLang(TXT_ALARM_MODE_WEEK, &item_data.item_content[0].item_data.text_buffer);
    GUILIST_AppendItem( ctrl_id, &item_t );

    //每15天
    MMI_GetLabelTextByLang(TXT_SCH_AWAKE_EVERY15DAYS, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );

    //每30天
    MMI_GetLabelTextByLang(TXT_ALARM_MODE_MONTH, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );


    //从不
    MMI_GetLabelTextByLang(TXT_COMM_NEVER, &item_data.item_content[0].item_data.text_buffer);

    GUILIST_AppendItem( ctrl_id, &item_t );

    //先将数据读出
    MMI_ReadNVItem (IMNV_SOGOU_UPDATE_FREQUENCY, &g_sogou_vocabulary_update_frequency_data.frequency);
    switch(g_sogou_vocabulary_update_frequency_data.frequency)
    {
    case VOCABULARY_UPDATE_FREQUENCY_E_ONE:
        GUILIST_SetSelectedItem(ctrl_id,VOCABULARY_UPDATE_FREQUENCY_E_ONE, TRUE);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_SEVEN:
        GUILIST_SetSelectedItem(ctrl_id,VOCABULARY_UPDATE_FREQUENCY_E_SEVEN, TRUE);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN:
        GUILIST_SetSelectedItem(ctrl_id,VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN, TRUE);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_THIRTY:
        GUILIST_SetSelectedItem(ctrl_id,VOCABULARY_UPDATE_FREQUENCY_E_THIRTY, TRUE);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_NEVER:
    default:
        GUILIST_SetSelectedItem(ctrl_id,VOCABULARY_UPDATE_FREQUENCY_E_NEVER, TRUE);
        break;
    }
}

/*****************************************************************************/
//  Description : 选择词库更新频率列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectVocabularyUpdateFrequencyList(MMI_CTRL_ID_T ctrl_id)
{
    //MMI_STRING_T note1 = {0};
    //MMI_STRING_T note2 = {0};

    //MMI_GetLabelTextByLang(TXT_SOGOU_SETTING_VOCABULARY_UPDATE_FREQUENCY, &note1);

    switch (GUILIST_GetCurItemIndex(ctrl_id))
    {
    case VOCABULARY_UPDATE_FREQUENCY_E_ONE:
        g_sogou_vocabulary_update_frequency_data.frequency = VOCABULARY_UPDATE_FREQUENCY_E_ONE;
        //MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_FREQUENCY_ONE, &note2);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_SEVEN:
        g_sogou_vocabulary_update_frequency_data.frequency = VOCABULARY_UPDATE_FREQUENCY_E_SEVEN;
        //MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_FREQUENCY_SEVEN, &note2);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN:
        g_sogou_vocabulary_update_frequency_data.frequency = VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN;
        //MMI_GetLabelTextByLang(TXT_SCH_AWAKE_EVERY15DAYS, &note2);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_THIRTY:
        g_sogou_vocabulary_update_frequency_data.frequency = VOCABULARY_UPDATE_FREQUENCY_E_THIRTY;
        //MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_FREQUENCY_THIRTY, &note2);
        break;
    case VOCABULARY_UPDATE_FREQUENCY_E_NEVER:
    default:
        g_sogou_vocabulary_update_frequency_data.frequency = VOCABULARY_UPDATE_FREQUENCY_E_NEVER;
        //MMI_GetLabelTextByLang(TXT_COMM_NEVER, &note2);
        break;
    }
    MMI_WriteNVItem(IMNV_SOGOU_UPDATE_FREQUENCY, &g_sogou_vocabulary_update_frequency_data.frequency);
    SGSaveAutoEvent();

#if 0    //非长时间等待提示，不需要
    MMIPUB_OpenAlertWinByTextPtr(
        PNULL,
        &note1,
        &note2,
        IMAGE_PUBWIN_SUCCESS,
        PNULL,PNULL,
        MMIPUB_SOFTKEY_NONE,
        HandleVocabularyUpdateFrequencyOpenAlertWinWindow
        );
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : 词库更新频率提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateFrequencyOpenAlertWinWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(SG_WIN_ID_VOCABULARY_UPDATE_FREQUENCY);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description : 词库更新时间窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateTimeWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   edit_ctrl_id = SG_CTRL_ID_TIMEEDITBOX_VOCABULARY_UPDATE_TIME;
    //GUI_RECT_T         dis_rect = {MMI_SET_TIME_DATE_BG_LEFT, MMI_SET_TIME_DATE_BG_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT};
    GUIEDIT_TIME_STYLE_E        time_style = GUIEDIT_TIME_STYLE_NULL;
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_12HOURS;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        GUIFORM_IsSlide(SG_CTRL_ID_VOCABULARY_UPDATE_TIME_FORM_ID, FALSE);
#endif
        time_type = MMIAPISET_GetTimeDisplayType();
        if (MMISET_TIME_12HOURS == time_type)
        {
            time_style = GUIEDIT_TIME_STYLE_12;
        }
        else
        {
            time_style = GUIEDIT_TIME_STYLE_24;
        }

        //set time display style
        GUIEDIT_SetTimeStyle(edit_ctrl_id,PNULL,&time_style,PNULL,FALSE);
        GUIEDIT_SetFont(edit_ctrl_id, MMI_SET_TIME_FONT);

        //先将数据读出
        MMI_ReadNVItem (IMNV_SOGOU_UPDATE_TIME, &g_sogou_vocabulary_update_frequency_data.time);
        GUIEDIT_SetTime(
        edit_ctrl_id,
        g_sogou_vocabulary_update_frequency_data.time.hour,
        g_sogou_vocabulary_update_frequency_data.time.min,
        0);
        MMK_SetAtvCtrl(win_id,edit_ctrl_id);
        GUIWIN_SetTitleTextId( win_id, TXT_SOGOU_VOCABULARY_UPDATE_TIME, FALSE );
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        MMK_SetAtvCtrl(win_id, edit_ctrl_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        SelectVocabularyUpdateTimeEdit(edit_ctrl_id);
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef MMI_IM_PDA_SUPPORT
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                SelectVocabularyUpdateTimeEdit(edit_ctrl_id);
                MMK_CloseWin(win_id);
                break;

            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_CloseWin(win_id);      
                break;

            default:
        break;
            }
        }
#endif
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_FULL_PAINT:
    case MSG_LOSE_FOCUS:
    case MSG_GET_FOCUS:
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 词库更新时间提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVocabularyUpdateTimeOpenAlertWinWindow(
                                                                MMI_WIN_ID_T    win_id,
                                                                MMI_MESSAGE_ID_E   msg_id,
                                                                DPARAM             param
                                                                )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(SG_WIN_ID_VOCABULARY_UPDATE_TIME);
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : append selection into sim select window
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SgConstructSimList(MMI_CTRL_ID_T ctrl_id)
{
#if defined MMI_WIFI_SUPPORT || defined MMI_MULTI_SIM_SYS_SINGLE    
    MMI_STRING_T string = { 0 };
    GUILIST_ITEM_T item_t = { 0 };
    GUILIST_ITEM_DATA_T item_data = { 0 };
#endif

#ifdef MMI_WIFI_SUPPORT
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX + 1, FALSE);
#else
    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE);
#endif
    
    // append sim card selection
#ifdef MMI_MULTI_SIM_SYS_SINGLE 
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MN_DUAL_SYS_1;
    
    MMI_GetLabelTextByLang(TXT_GPRS, &string);
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string.wstr_ptr;
    
    // set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#else
    MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
#endif
    
#ifdef MMI_WIFI_SUPPORT
    // append wifi selection
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;//WIFI
    MMI_GetLabelTextByLang(TXT_WIFI, &string);
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string.wstr_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string.wstr_ptr;
    
    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif
    
    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : SIM卡选择窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMSelectWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E  recode                  = MMI_RESULT_TRUE;
    uint16  list_index                    = 0;
    uint32 item_data = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_WIFI_SUPPORT
        GUITITLE_SetTextId(win_id, MMITHEME_GetTitleCtrlId(), TXT_SIM_SEL, FALSE);
#else
        GUITITLE_SetTextId(win_id, MMITHEME_GetTitleCtrlId(), TXT_NETWORK_SELECT, FALSE);
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
#endif
        MMK_SetAtvCtrl(SG_WIN_ID_SIMCARDSELECT, SG_CTRL_ID_LIST_SIMCARDSELECT);
        break;

    case MSG_FULL_PAINT:
        list_index = GUILIST_GetCurItemIndex(SG_CTRL_ID_LIST_SIMCARDSELECT);
        SgConstructSimList(SG_CTRL_ID_LIST_SIMCARDSELECT);
        GUILIST_SetCurItemIndex(SG_CTRL_ID_LIST_SIMCARDSELECT, list_index);
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MSG_GET_FOCUS:
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;
#endif

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        list_index = GUILIST_GetCurItemIndex(SG_CTRL_ID_LIST_SIMCARDSELECT);
        
        if (GUILIST_IsCurrItemInvalid(SG_CTRL_ID_LIST_SIMCARDSELECT))
        {
            break;
        }
        
        GUILIST_GetItemData(SG_CTRL_ID_LIST_SIMCARDSELECT, list_index, &item_data);

        if (item_data < MMI_DUAL_SYS_MAX) 
            {
            // select sim card
            s_last_selected_sim_id = item_data;
            s_sg_is_over_wifi = FALSE;
        }    
        else
        {
            // wifi
            s_sg_is_over_wifi = TRUE;
        }
#ifdef MMI_WIFI_SUPPORT
        if (s_sg_is_over_wifi && MMIWIFI_STATUS_CONNECTED != MMIAPIWIFI_GetStatus())
        {
            MMIAPIWIFI_OpenListWin();
        }
        else
#endif
        {
            MMK_CloseWin(win_id);
            OpenUpdateWindow(s_last_selected_sim_id);
		}
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 选择词库更新时间
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SelectVocabularyUpdateTimeEdit(MMI_CTRL_ID_T ctrl_id)
{
    //MMI_STRING_T note1 = {0};
    //MMI_STRING_T note2 = {0};

    //GUIEDITBOX_GetTimeEditBoxInfo(ctrl_id,&(g_sogou_vocabulary_update_frequency_data.time.hour),&(g_sogou_vocabulary_update_frequency_data.time.min));
    GUIEDIT_GetTime(ctrl_id, &(g_sogou_vocabulary_update_frequency_data.time.hour), &(g_sogou_vocabulary_update_frequency_data.time.min), PNULL);
    MMI_WriteNVItem(IMNV_SOGOU_UPDATE_TIME, &g_sogou_vocabulary_update_frequency_data.time);
    SGSaveAutoEvent();

#if 0    //非长时间等待提示，不需要
    MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE_TIME, &note1);
    MMI_GetLabelTextByLang(TXT_SET_SUCCESS, &note2);
    MMIPUB_OpenAlertWinByTextPtr(
        PNULL,
        &note1,
        &note2,
        IMAGE_PUBWIN_SUCCESS,
        PNULL,PNULL,
        MMIPUB_SOFTKEY_NONE,
        HandleVocabularyUpdateTimeOpenAlertWinWindow
        );
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : 自动更新词库提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAutoUpdateWordRemindWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
#ifndef MMI_IM_PDA_SUPPORT    
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_TEXTBOX_AUTOUPDATEWORD_REMIND;
    MMI_STRING_T string = {0};
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        GUIFORM_IsSlide(SG_CTRL_ID_AUTOUPDATEWORD_REMIND_FORM_ID, FALSE);
        GUIWIN_SetTitleTextId(win_id, TXT_SOGOU_AUTOUPDATEWORD_REMIND, FALSE);
#else
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMI_GetLabelTextByLang(TXT_SOGOU_AUTOUPDATEWORD_REMIND, &string);
        GUITEXT_SetString(ctrl_id , string.wstr_ptr, string.wstr_len, FALSE);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMK_CloseWin( win_id );
        s_sg_IsAlertUpdate = TRUE;
        SGUpdateWordLib();
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : GPRS提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkingRemindTextBoxWindow(
                                                       MMI_WIN_ID_T    win_id,
                                                       MMI_MESSAGE_ID_E   msg_id,
                                                       DPARAM             param
                                                       )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
#ifndef MMI_IM_PDA_SUPPORT    
    MMI_CTRL_ID_T   ctrl_id = SG_CTRL_ID_TEXTBOX_NETWORKING_REMIND;
    MMI_STRING_T string = {0};
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_IM_PDA_SUPPORT
        MMIPUB_SetWinTitleTextId(win_id, TXT_SOGOU_CONNECT_REMIND, FALSE);
#else
        MMK_SetAtvCtrl(win_id, ctrl_id);
        if (s_update_type == SG_UPDATE_TYPE_VOCABULARY)
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_DOWNLOAD_VOCABULARY, &string);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_DOWNLOAD_SKIN, &string);
        }
        GUITEXT_SetString(ctrl_id , string.wstr_ptr, string.wstr_len, FALSE);
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMK_CloseWin( win_id );
        EnterUpdateWindow();
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
#ifdef MMI_IM_PDA_SUPPORT
        recode = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
#else
        recode = MMI_RESULT_FALSE;
#endif
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 更新词库
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SGUpdateWordLib(void)
{
    s_update_type = SG_UPDATE_TYPE_VOCABULARY;
	//SOGOU_TRACE_OUT:"SOGOU_AutoUpdateWordRemind g_sogou_update_settings_data.is_need_connect_remind=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_3936_112_2_18_2_29_42_325,(uint8*)"d",g_sogou_update_settings_data.is_need_connect_remind);
    if (g_sogou_update_settings_data.is_need_connect_remind == TRUE)
    {
#ifdef MMI_IM_PDA_SUPPORT
        MMI_WIN_ID_T alert_win_id = SG_WIN_ID_NETWORKING_REMIND;
        MMI_STRING_T text_str = { 0 };
        if (s_update_type == SG_UPDATE_TYPE_VOCABULARY)
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_DOWNLOAD_VOCABULARY, &text_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_DOWNLOAD_SKIN, &text_str);
        }
        MMIPUB_OpenQueryWinByTextPtr(&text_str, PNULL, IMAGE_PUBWIN_QUERY, 
            &alert_win_id, HandleNetworkingRemindTextBoxWindow);
#else
        MMK_CreateWin((uint32*)SG_NETWORKING_REMIND_WIN_TAB,PNULL);
#endif
    }
    else
    {
        EnterUpdateWindow();
    }
}

/*****************************************************************************/
//  Description : 自动更新提醒
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SogouAutoUpdateWordRemind(void)
{
	//SOGOU_TRACE_OUT:"SOGOU_AutoUpdateWordRemind g_sogou_update_settings_data.is_need_update_notif=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_3968_112_2_18_2_29_42_326,(uint8*)"d",g_sogou_update_settings_data.is_need_update_notif);
    if(g_sogou_update_settings_data.is_need_update_notif)
    {
#ifdef MMI_IM_PDA_SUPPORT
        MMK_CreatePubFormWin((uint32*)SG_AUTOUPDATEWORD_REMIND_WIN_TAB, (ADD_DATA)PNULL);
#else
        MMK_CreateWin((uint32*)SG_AUTOUPDATEWORD_REMIND_WIN_TAB,PNULL);
#endif
    }
    else
    {
        s_sg_IsAlertUpdate = TRUE;
        SGUpdateWordLib();
    }
}

/*****************************************************************************/
//  Description : 自动保存消息
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SGSaveAutoEvent(void)
{
    MMIACC_SMART_EVENT_T    event = {0};
    SCI_DATE_T              cur_date = {0};
    MMI_STRING_T string = {0};

    TM_GetSysDate(&cur_date);
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    event.event_fast_info.is_valid = TRUE;
    event.event_fast_info.type = SMART_EVENT_SOGOU_UPDATE;

    event.event_fast_info.is_on = TRUE;

    switch(g_sogou_vocabulary_update_frequency_data.frequency)
    {
    case VOCABULARY_UPDATE_FREQUENCY_E_ONE:
        event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
        break;

    case VOCABULARY_UPDATE_FREQUENCY_E_SEVEN:
        event.event_fast_info.fre |= s_week_mask[cur_date.wday];
        event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
        break;

    case VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN:
        event.event_fast_info.fre_mode = ALM_MODE_EVERY15DAYS;
        break;

    case VOCABULARY_UPDATE_FREQUENCY_E_THIRTY:
        event.event_fast_info.fre_mode = ALM_MODE_EVERYMONTH;
        break;

    case VOCABULARY_UPDATE_FREQUENCY_E_NEVER:
        event.event_fast_info.is_on = FALSE;
        break;

    default:
        event.event_fast_info.is_on = FALSE;
        SCI_TRACE_LOW (0, ("[MMIIM] SGSaveAutoEvent g_sogou_vocabulary_update_frequency_data.frequency=%d",g_sogou_vocabulary_update_frequency_data.frequency)); /*assert verified*/
        //SCI_PASSERT(0, ("SGSaveAutoEvent g_sogou_vocabulary_update_frequency_data.frequency=%d",g_sogou_vocabulary_update_frequency_data.frequency)); /*assert verified*/
        break;
    }

    event.event_fast_info.start_year = cur_date.year;
    event.event_fast_info.start_mon = cur_date.mon;
    event.event_fast_info.start_day = cur_date.mday;
    event.event_fast_info.hour = g_sogou_vocabulary_update_frequency_data.time.hour;
    event.event_fast_info.minute = g_sogou_vocabulary_update_frequency_data.time.min;
    event.event_fast_info.year = cur_date.year;
    event.event_fast_info.mon = cur_date.mon;
    event.event_fast_info.day = cur_date.mday;
    event.event_fast_info.end_year = cur_date.year+1;
    event.event_fast_info.end_mon = cur_date.mon;
    event.event_fast_info.end_day = cur_date.mday;

    MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATE, &string);
    event.event_content_info.w_length = string.wstr_len;
    SCI_MEMCPY(event.event_content_info.w_content,string.wstr_ptr,2*string.wstr_len);
    event.event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
    event.event_content_info.ring_type = ALM_FIX_RING;
    event.event_content_info.ring = 1;

    MMIAPIALM_SaveInteEventInfo(EVENT_SOGOU_UPDATE, &event);

    return TRUE;
}
PUBLIC BOOLEAN MMIAPIIM_SGDelAutoEvent(void)//6_12_1
{
    MMIACC_SMART_EVENT_T    event = {0};
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    event.event_fast_info.is_valid = TRUE;
    event.event_fast_info.type = SMART_EVENT_SOGOU_UPDATE;
    event.event_fast_info.is_on = FALSE;
    MMIAPIALM_SaveInteEventInfo(EVENT_SOGOU_UPDATE, &event);
    return TRUE;
}

/*****************************************************************************/
//  Description : 进入自动更新窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EnterUpdateWindow(void)
{
    uint8 connection_num = 0;
    
    //飞行模式，不能进入，返回
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPISET_AlertFlyMode();
        return;
    }

#ifdef MMI_WIFI_SUPPORT
    connection_num++;
#endif
    
    connection_num += MMIAPIPHONE_GetSimAvailableNum(PNULL, 0);
    
	SCI_TraceLow("EnterUpdateWindow connection_num = %d",connection_num);
    // 下载词库,或者皮肤目录时,弹出连接选择窗口
    if ((connection_num > 1) && 
		(!MMK_IsOpenWin(SG_WIN_ID_SIMCARDSELECT))   &&
		(SG_UPDATE_TYPE_SKIN_FILE != s_update_type))
    {
        s_sg_is_over_wifi = FALSE;
        MMK_CreateWin((uint32*)SG_SIMCARD_SELECT_WIN_TAB, PNULL);
    }
#ifdef MMI_WIFI_SUPPORT
    // 此时无SIM卡
    else if (connection_num == 1)
    {
        s_sg_is_over_wifi = TRUE;
        if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
        {
            OpenUpdateWindow(MN_DUAL_SYS_MAX);
        }
        else
        {
            MMIAPIWIFI_OpenListWin();
        }
    }
#endif
    else
    {
        // 连接选择大于1,并且下载皮肤文件是,使用原先选择的信道
        if (SG_UPDATE_TYPE_SKIN_FILE == s_update_type && connection_num > 1)
		{
			OpenUpdateWindow(s_last_selected_sim_id);
		}
		else
		{
            // 其他情况下只有一种方式
#ifdef MMI_WIFI_SUPPORT
            s_sg_is_over_wifi = TRUE;
#else
            s_sg_is_over_wifi = FALSE;
#endif
			OpenUpdateWindow(MN_DUAL_SYS_MAX);
		}
    }
}

/*****************************************************************************/
//  Description : 打开自动更新窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void OpenUpdateWindow(MN_DUAL_SYS_E sim_id)
{
    MMI_STRING_T prompt_str = {0};
    
    if (s_sg_is_over_wifi)
    {
        //SOGOU_TRACE_OUT:"wdp_update_wintab.c OpenUpdateWindow: over wifi!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_4128_112_2_18_2_29_43_327,(uint8*)"");
        MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_SETTING_NETWORK);
    } 
    else if (MMIAPIIM_SGGetIsSetNetwork() && SG_UPDATE_TYPE_SKIN_FILE == s_update_type)
    {
        //SOGOU_TRACE_OUT:"wdp_update_wintab.c OpenUpdateWindow: over sim, status = SG_IM_UPD_S_CREATING_SOCKET"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_4133_112_2_18_2_29_43_328,(uint8*)"");
        MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_CREATING_SOCKET);
    }
    else
    {
        //SOGOU_TRACE_OUT:"wdp_update_wintab.c OpenUpdateWindow: over sim, status = SG_IM_UPD_S_START"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_4138_112_2_18_2_29_43_329,(uint8*)"");
        MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_START);
    }
    MMIAPIIM_SGSetSelectedSIM(sim_id);
    MMI_GetLabelTextByLang(TXT_NETWORK_CONNECTING, &prompt_str);
    MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, SG_WIN_ID_UPDATE_WATCHING,
                       NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleUpdateWatchingWindow);
    MMIAPIIM_SGUpdateCallBackFunc();
}

/*****************************************************************************/
//  Description : 关闭自动更新窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CloseUpdateWindow(void)
{
    if ( SG_UPDATE_TYPE_VOCABULARY == s_update_type && s_sg_IsAlertUpdate == FALSE)
    {
        if(!MMK_IsOpenWin(SG_WIN_ID_SOGOU_SETTING))
        {
            MMK_CreateWin((uint32*)SG_INPUT_SETTINGS_MENU_WIN_TAB,PNULL);//搜狗输入法设置1
        }
        else
        {
            MMK_SendMsg( SG_WIN_ID_SOGOU_SETTING, MSG_FULL_PAINT, NULL);
        }
    }
#ifdef SOGOU_USE_PDA_SKIN
    else if (SG_UPDATE_TYPE_SKIN_DIR == s_update_type)
    {
        if(!MMK_IsOpenWin(SG_WIN_ID_SKIN_MANAGEMENT))
        {
            MMK_CreateWin((uint32*)SG_SKIN_MANAGEMENT_MENU_WIN_TAB,PNULL);//皮肤管理1.1
        }
        else
        {
            MMK_SendMsg( SG_WIN_ID_SKIN_MANAGEMENT, MSG_FULL_PAINT, NULL);
        }
    }
    else if (SG_UPDATE_TYPE_SKIN_FILE == s_update_type)
    {
        if(!MMK_IsOpenWin(SG_WIN_ID_SKIN_BROWSE))
        {
            MMK_CreateWin((uint32*)SG_SKIN_BROWSE_WIN_TAB,PNULL);//搜狗皮肤浏览界面
        }
        else
        {
            MMK_WinGrabFocus(SG_WIN_ID_SKIN_BROWSE);
        }
    }
#endif
    s_update_type = SG_UPDATE_TYPE_NONE;
}

/*****************************************************************************/
//  Description : 更新监视窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUpdateWatchingWindow(
                                              MMI_WIN_ID_T    win_id,
                                              MMI_MESSAGE_ID_E   msg_id,
                                              DPARAM             param
                                              )
{
    LOCAL MMI_MESSAGE_ID_E s_prev_msg = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

	SCI_TRACE_LOW("[MMIIM] HandleUpdateWatchingWindow msg_id = 0x%04x ,s_update_type= %d",msg_id, s_update_type);
    if (msg_id >= MSG_SG_UPD_WAIT_SETING_NETWORK && msg_id <= MSG_SG_UPD_WAIT_UNPACKING)
    {
        MMI_STRING_T note = {0};

        if (s_prev_msg == msg_id)
        {
            return MMI_RESULT_TRUE;
        }
        else
        {
            s_prev_msg = msg_id;
        }
        switch(msg_id)
        {
        case MSG_SG_UPD_WAIT_SETING_NETWORK:
        case MSG_SG_UPD_WAIT_CONNECTING:
            MMI_GetLabelTextByLang(TXT_NETWORK_CONNECTING, &note);
            break;

        case MSG_SG_UPD_WAIT_SENDING:
            MMI_GetLabelTextByLang(TXT_SOGOU_SENDING, &note);
            break;
        case MSG_SG_UPD_WAIT_RECEIVING:
        case MSG_SG_UPD_WAIT_UNPACKING:
            if(SG_UPDATE_TYPE_VOCABULARY==s_update_type)
            {
                MMI_GetLabelTextByLang(TXT_SOGOU_VOCABULARY_UPDATING, &note);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_SOGOU_DOWNING, &note);
            }
            break;
        default:
            break;
        }
        if (SG_UPDATE_TYPE_SKIN_DIR!=s_update_type)
        {
            MMIPUB_SetWaitWinText(TRUE, 1, &note, PNULL, PNULL, win_id);
        }
    }
    else if (msg_id >= MSG_SG_UPD_WAIT_SET_NETWORK_FALED && msg_id <= MSG_SG_UPD_WAIT_UNPACK_FAILED)
    {
        MMI_STRING_T note = {0};

        switch(msg_id)
        {
        case MSG_SG_UPD_WAIT_SET_NETWORK_FALED:
            MMI_GetLabelTextByLang(TXT_SOGOU_SET_NETWORK_FAILED, &note);
            break;
        case MSG_SG_UPD_WAIT_RECEIVE_FAILED:
            MMI_GetLabelTextByLang(TXT_COMMON_RECEIVE_FAIL, &note);
            break;
        case MSG_SG_UPD_WAIT_UNPACK_FAILED:
            MMI_GetLabelTextByLang(TXT_SOGOU_UNPACK_FAILED, &note);
            break;
        default:
            break;
        }
        MMIPUB_OpenAlertWinByTextPtr(PNULL,&note,PNULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,
            MMIPUB_SOFTKEY_NONE,HandleUpdateExitWin);
    }
    else if (msg_id == MSG_SG_UPD_WAIT_CONNECTING_TIME_OUT)
    {
        MMI_STRING_T note1 = {0};
        MMI_STRING_T note2 = {0};
        MMI_WIN_ID_T query_win_id = SG_WIN_ID_UPDATE_TIME_OUT_QUERY;

        MMI_GetLabelTextByLang(TXT_SOGOU_CONNECTING_TIME_OUT, &note1);
        MMI_GetLabelTextByLang(TXT_SOGOU_TIME_OUT_CONTINUE, &note2);
        MMIPUB_OpenQueryWinByTextPtr(&note1,&note2,IMAGE_PUBWIN_QUERY,&query_win_id,HandleTimeOutQueryWinMsg);
    }
    else if (msg_id == MSG_SG_UPD_WAIT_SUCCEED)
    {
        MMI_STRING_T note = {0};
        MMI_WIN_ID_T query_win_id = SG_WIN_ID_UPDATE_CANCEL_QUERY;
        uint32 time_period = ALERT_WIN_DURATION;

        if(MMK_IsOpenWin(query_win_id))
        {
            MMIPUB_CloseQuerytWin(&query_win_id);
        }
        if(SG_UPDATE_TYPE_VOCABULARY==s_update_type)
        {
            uint8 str_len = MMIAPICOM_Wstrlen((const wchar*)MMIAPIIM_GetDisplayStr());

            if (4 < str_len)
            {
				note.wstr_ptr = MMIAPIIM_GetDisplayStr();
				note.wstr_len = MIN(DISPLAY_TO_USER_STRING_LEN, str_len);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_SUCCESS, &note);
            }
        }
        else if(SG_UPDATE_TYPE_SKIN_FILE==s_update_type)
        {
            MMI_GetLabelTextByLang(TXT_DOWNLOAD_SUCCEED, &note);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_UPDATE_SUCCEED, &note);
        }
        MMIPUB_OpenAlertWinByTextPtr(&time_period, &note, PNULL, IMAGE_PUBWIN_SUCCESS,
                                     PNULL, PNULL, MMIPUB_SOFTKEY_ONE, HandleUpdateExitWin);
    }
    else if (msg_id == MSG_TIMER)
    {
        if((*(uint8*)param == MMIAPIIM_SGGetLibTimerId()) && 0 != MMIAPIIM_SGGetLibTimerId())
        {
#ifndef WIN32
            SG_soc_excuteTimer();
            MMIAPIIM_SGUpDateTimeout();
#endif
        }
        // 解析主机地址定时器
        else if((*(uint8*)param == MMIAPIIM_GetServerDelayTimerId()) && 0 != MMIAPIIM_GetServerDelayTimerId())
        {
            MMIAPIIM_SGGetSkinServerAdd();
        }
        else if ((*(uint8*)param == MMIAPIIM_SGPdpTimerId()) && 0 != MMIAPIIM_SGPdpTimerId())
        {
            MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_PDPACTIVE_TIMEOUT);
            MMIAPIIM_SGUpdateCallBackFunc();
        }
        else
        {
            MMIAPIIM_SGUpdateCallBackFunc();
        }
    }
    else if (msg_id == MSG_CTL_CANCEL || msg_id == MSG_APP_CANCEL)
    {
        MMI_STRING_T note = {0};
        MMI_WIN_ID_T query_win_id = SG_WIN_ID_UPDATE_CANCEL_QUERY;

        if(SG_UPDATE_TYPE_SKIN_FILE==s_update_type)
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_IS_CANCEL_DOWNLOAD, &note);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SOGOU_IS_CANCEL, &note);
        }

        MMIPUB_OpenQueryWinByTextPtr(&note, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, HandleCancelQueryWinMsg);
    }
    else if(msg_id == MSG_CTL_LOSE_FOCUS)
    {
        MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_END);
        MMIAPIIM_SGUpdateCallBackFunc();
        MMK_CloseWin(win_id);
    }
    else
    {
		//SOGOU_TRACE_OUT:"HandleUpdateWatchingWindow MMIPUB_HandleWaitWinMsg begin win_id=0x%04x,msg_id=0x%04x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_4358_112_2_18_2_29_43_330,(uint8*)"dd",win_id,msg_id);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
    }

    return recode;
}

/*****************************************************************************/
//  Description : 退出更新窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleUpdateExitWin(
                                        MMI_WIN_ID_T    win_id,
                                        MMI_MESSAGE_ID_E   msg_id,
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    SG_UPDATE_TYPE_E temp_sogou_update_type = SG_UPDATE_TYPE_NONE;
    MMI_WIN_ID_T query_win_id = SG_WIN_ID_UPDATE_CANCEL_QUERY;
    //SOGOU_TRACE_OUT:"HandleUpdateExitWin win_id=0x%04x,msg_id=0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WDP_UPDATE_WINTAB_4378_112_2_18_2_29_43_331,(uint8*)"dd",win_id,msg_id);
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        temp_sogou_update_type=s_update_type;
        MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_END);
        MMIAPIIM_SGUpdateCallBackFunc();
        MMK_CloseWin(SG_WIN_ID_UPDATE_WATCHING);
        CloseUpdateWindow();
#ifdef SOGOU_USE_PDA_SKIN
        if(temp_sogou_update_type==SG_UPDATE_TYPE_SKIN_FILE)
        {
            if(MMK_IsOpenWin(SG_WIN_ID_SKIN_BROWSE))
            {
                MMK_CloseWin(SG_WIN_ID_SKIN_BROWSE);
            }

            if(MMK_IsOpenWin(SG_WIN_ID_SKIN_SELECT))
            {
                MMK_CloseWin(SG_WIN_ID_SKIN_SELECT);
            }

            if(MMK_IsOpenWin(query_win_id))
            {
                MMIPUB_CloseQuerytWin(&query_win_id);
            }
            MMK_CreateWin((uint32*)SG_SKIN_SELECT_MENU_WIN_TAB,PNULL);
        }
#endif
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : 是否取消更新窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCancelQueryWinMsg(
                                            MMI_WIN_ID_T    win_id,
                                            MMI_MESSAGE_ID_E   msg_id,
                                            DPARAM             param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
	case MSG_APP_RED:
        MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_END);
        MMIAPIIM_SGUpdateCallBackFunc();
        MMK_CloseWin(win_id);
        MMK_CloseWin(SG_WIN_ID_UPDATE_WATCHING);
        CloseUpdateWindow();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        MMIAPIIM_SGUpdateCallBackFunc();
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : 超时提醒窗口函数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeOutQueryWinMsg(
                                             MMI_WIN_ID_T    win_id,
                                             MMI_MESSAGE_ID_E   msg_id,
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        if (s_sg_is_over_wifi)
        {
            MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_SETTING_NETWORK);
        } 
        else if (MMIAPIIM_SGGetIsSetNetwork() && SG_UPDATE_TYPE_SKIN_FILE == s_update_type)
        {
            MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_CREATING_SOCKET);
        }
        else
        {
            MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_START);
        }
        MMIAPIIM_SGUpdateCallBackFunc();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_END);
        MMIAPIIM_SGUpdateCallBackFunc();
        MMK_CloseWin(SG_WIN_ID_UPDATE_WATCHING);
        CloseUpdateWindow();
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 打开皮肤浏览窗口
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SoGouOpenSkinBrowseWin(void)
{
    MMK_CreateWin((uint32*)SG_SKIN_BROWSE_WIN_TAB,PNULL);//搜狗皮肤浏览界面
    return TRUE;
}

/*****************************************************************************/
//  Description : 检查皮肤是否符合格式要求
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SG_Check_Skin_Is_valid(uint16* pfileName,uint16 nameLen)
{
    WD_UINT16 pic_filename[MMIFILE_FULL_PATH_MAX_LEN + 1] = { 0 };
    WD_UINT32 full_path_len;
    WD_UINT32 file_size;
    WD_UINT32 data_size = 0;
    WD_UINT8 *pic_data_ptr=NULL;
    JINF_GET_INFO_IN_T jpg_input;
    JINF_INFO_T jpg_info;
    WD_UINT16 dest_width = 0;
    WD_UINT16 dest_height = 0;
    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_DEC_OUT_T    img_output = {0};
    BOOLEAN             result = FALSE;

    if (pfileName == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] SG_Check_Skin_Is_valid pic_filename");
        return FALSE;
    }

    if (nameLen > MMIFILE_FULL_PATH_MAX_LEN - SKIN_FILEPATH_LEN - 4)        //file name.jpg
    {
        SCI_TRACE_LOW ("[MMIIM] SG_Check_Skin_Is_valid namelen");
        return FALSE;
    }
    
	if(0==g_sg_save_position)
    	MMIAPICOM_Wstrncpy(pic_filename, (const wchar*)SG_D_SKIN_CONFIG_PATH, MMIFILE_FULL_PATH_MAX_LEN);
	else if(1==g_sg_save_position)
    	MMIAPICOM_Wstrncpy(pic_filename, (const wchar*)SG_E_SKIN_CONFIG_PATH, MMIFILE_FULL_PATH_MAX_LEN);
	else
		return FALSE;
    SCI_MEMCPY(pic_filename + SKIN_FILEPATH_LEN,pfileName,2*nameLen);
    SCI_MEMCPY(pic_filename + SKIN_FILEPATH_LEN+nameLen,L".jpg",8);

    if (MMIAPIFMM_IsFileExist(pic_filename,MMIAPICOM_Wstrlen(pic_filename)))
    {
        if(MMIAPIFMM_GetFileInfo(pic_filename, MMIAPICOM_Wstrlen(pic_filename), &file_size, NULL, NULL))
        {
            full_path_len = MMIAPICOM_Wstrlen(pic_filename);
            if((file_size > 0) && (file_size < MIN_SKIN_FILE_BUF_SIZE))
            {
                //读图片文件头70，足以得到其长和宽
                if (MIN_SKIN_FILE_BUF_SIZE < file_size)
                {
                    data_size = MIN_SKIN_FILE_BUF_SIZE;
                }
                else
                {
                    data_size = file_size;
                }
                pic_data_ptr = (uint8*)MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);/*lint !e64*/
                if (pic_data_ptr == PNULL)
                {
                    SCI_TRACE_LOW ("[MMIIM] SG_Check_Skin_Is_valid MMI_BL_MALLOC");
                    return FALSE;
                }
                
                if (MMIAPIFMM_ReadFilesDataSyn(pic_filename,full_path_len,pic_data_ptr,data_size))
                {
                    jpg_input.jpeg_buf_ptr = pic_data_ptr;
                    jpg_input.jpeg_buf_size = data_size;
                    jpg_input.read_file_func = 0;
                    if (JINF_SUCCESS == IMGJPEG_GetInfo(&jpg_input,&jpg_info))
                    {
                        dest_width = jpg_info.image_width;
                        dest_height = jpg_info.image_height;
                    }
                    else
                    {
                        MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                        return FALSE;
                    }
                }

                if (SG_SKIN_SCREEN_P_WIDTH != dest_width ||  SG_SKIN_HEIGHT != dest_height)
                {
                    MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                    return FALSE;
                }

                img_src.src_buf_ptr = pic_data_ptr;
                img_src.src_data_size = data_size;
                //set image dest width and height
                img_display.dest_width = dest_width;
                img_display.dest_height = dest_height;
                //set image decode output data buffer
                img_output.decode_data_ptr = (uint8*)MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                if (PNULL == img_output.decode_data_ptr)
                {
                    SCI_TRACE_LOW ("[MMIIM] SG_Check_Skin_Is_valid BLOCK_MEM_POOL_PIC_UNDO");
                    MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);

                    return FALSE;
                }
                img_output.decode_data_size = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                //decode jpg
                result = GUIIMG_Decode(&img_src, &img_display, &img_output);
                if (result)
                {
                    if (SG_SKIN_SCREEN_P_WIDTH < img_output.actual_width || SG_SKIN_HEIGHT < img_output.actual_height)
                    {
                        return FALSE;
                    }
                }
                //free file data buffer
                MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_PROCESS);
                //free vpbits
                MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_PIC_UNDO);
                img_output.decode_data_ptr = PNULL;

                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE;    
}

/*****************************************************************************/
//  Description : 设置选择的皮肤文件
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGSetupSelectedSkinFile(uint16* pfileName,uint16 nameLen)
{
    WD_UINT16 txt_filename[MMIFILE_FULL_PATH_MAX_LEN];
    SFS_HANDLE file_handle = 0;
    uint32  write_size  = 0;
    uint16 temp_name[MMIFILE_FULL_PATH_MAX_LEN + 1]={0};
    MMI_STRING_T  str_default= {0};

	SCI_TraceLow("MMIAPIIM_SGSetupSelectedSkinFile g_sg_save_position =%d",g_sg_save_position);
	if(0==g_sg_save_position)
    {
    	MMIAPICOM_Wstrncpy(txt_filename, (const wchar*)SG_D_SKIN_CONFIG_FILE, MMIFILE_FULL_PATH_MAX_LEN - 1);
	}
	else if(1==g_sg_save_position)
    {
    	MMIAPICOM_Wstrncpy(txt_filename, (const wchar*)SG_E_SKIN_CONFIG_FILE, MMIFILE_FULL_PATH_MAX_LEN - 1);
	}
	else
    {
    	return FALSE;
	}
	
    MMI_GetLabelTextByLang(TXT_SOGOU_SKIN_DEFAULT, &str_default);
    if (0 != MMIAPICOM_CompareTwoWstr(str_default.wstr_ptr,str_default.wstr_len,pfileName,nameLen))
    {
        if(FALSE==SG_Check_Skin_Is_valid(pfileName,nameLen))
        {
			SCI_TraceLow("MMIAPIIM_SGSetupSelectedSkinFile error 1");
            return FALSE;
        }
    }
    if(!MMIAPIFMM_IsFileExist(txt_filename, MMIAPICOM_Wstrlen(txt_filename)))
    {
        file_handle = SFS_CreateFile(txt_filename, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }
    else
    {
        SFS_DeleteFile(txt_filename, PNULL);
        file_handle = SFS_CreateFile(txt_filename, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);
    }
    if(file_handle == 0)
    {
		SCI_TraceLow("MMIAPIIM_SGSetupSelectedSkinFile error 2");
        return FALSE;
    }
    SCI_MEMSET(temp_name,0,sizeof(temp_name));
    SCI_MEMCPY(temp_name,pfileName,2*nameLen);
    MMIAPICOM_Wstrcat((wchar*)temp_name, (const wchar*)(L".jpg"));
    SFS_WriteFile(file_handle, temp_name, 2*nameLen+8, &write_size, NULL);
    SFS_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
//  Description : 释放皮肤文件列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGFreeSkinFileList(void)
{
    if (s_skin_list_file)
    {
        SCI_FREE(s_skin_list_file);
    }
}

/*****************************************************************************/
//  Description : 获取皮肤文件列表
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPIIM_GetSkinFileList(void)
{
    return s_skin_list_file;
}

/*****************************************************************************/
//  Description : 分配皮肤文件列表空间
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGAllocSkinFileList(uint16 size)
{
    s_skin_list_file = SCI_ALLOCA(size);
    if (s_skin_list_file == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] MMIAPIIM_SGAllocSkinFileList");
        return;
    }
    
    SCI_MEMSET(s_skin_list_file, 0, size);
}

/*****************************************************************************/
//  Description : 获取皮肤文件id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPIIM_SGGetSkinInfPicId(void)
{
    return &s_skin_ptr->picID[0];
}

/*****************************************************************************/
//  Description : 分配皮肤结构空间
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGAllocSkinInf(void)
{
    if (s_skin_ptr)
    {
        SCI_FREE(s_skin_ptr);
    }
    
    s_skin_ptr = SCI_ALLOCA(sizeof(SkinInf));
    if (s_skin_ptr == PNULL)
    {
        SCI_TRACE_LOW ("[MMIIM] MMIAPIIM_SGAllocSkinInf");
        return;
    }
    
    SCI_MEMSET(s_skin_ptr, 0, sizeof(SkinInf));
}
#endif
/*****************************************************************************/
//  Description : 设置更新类型
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGSetUpdateType(SG_UPDATE_TYPE_E type)
{
    s_update_type = type;
}

/*****************************************************************************/
//  Description : 获取更新类型
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SG_UPDATE_TYPE_E MMIAPIIM_SGGetUpdateType(void)
{
    return s_update_type;
}

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : 获取下载皮肤的全路径
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPIIM_SGGetDownloadSkinFullPath(void)
{
    return &s_sg_download_skin_file_full_path[0];
}
#endif
/*****************************************************************************/
//  Description : 是否通过WIFI进行更新
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGGetIsOverWifi(void)
{
    return s_sg_is_over_wifi;
}
#endif // #if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
#endif //_WDP_UPDATE_WINTAB_C_


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527


/*Edit by script, ignore 20 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
