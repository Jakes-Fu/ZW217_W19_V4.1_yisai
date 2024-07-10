/*****************************************************************************
** File Name:      mmischedule.c                                                
** Author:         liqing.peng   
** Date:           2006/07/04
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to implement schedule function 
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/06/04     liqing.peng      Create                                    *
******************************************************************************/


/**---------------------------------------------------------------------------*
**                         INCLUDE FILES                                     *
**---------------------------------------------------------------------------*/
#ifdef CALENDAR_SUPPORT
#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
//#include "win.def"
#include "mmk_app.h"
#include "mmi_text.h"
//#include "sci_types.h"
#include "guisoftkey.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmi_theme.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guilistbox.h"
#include "mmischedule_export.h"
#include "guilabel.h"
#include "mmialarm_export.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmimp3_export.h"
#include "mmi_solarlunar.h"
#include "mmialarm_id.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmialarm_menutable.h"
//#include "mmialarm_nv.h"
#include "guidropdownlist.h"
#include "mmiacc_text.h"
#include "mmiacc_image.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmiacc_event.h"
#include "mmicalendar_export.h"
//#include "mmischedule_position.h"
//#include "mmialarm_position.h"
#include "mmi_mainmenu_export.h"
#include "guitext.h"
//#include "mmiset_wintab.h"
#include "guibutton.h"
#include "guirichtext.h"
#include "guires.h"
#include "guiedit.h"
//#include "mmiset_display.h"
#include "guiform.h"
#include "guisetlist.h"
//#include "mmiset_set_ring.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#ifdef MMI_VCALENDAR_SUPPORT
#include "mmivcalendar.h"
#ifdef MMS_SUPPORT
#include "Mmimms_export.h"
#endif
#include "Mmisms_export.h"
#include "Mmisms_text.h" 
#include "Mmiset_text.h"
#include "Mmipb_text.h"
#include "Mmisms_id.h"
#include "mmifilearray_export.h"
#endif
#include "mmi_text.h"
#include "mmiudisk_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget.h"
#include "mmiwidget_export.h"
#endif
#include "guictrl_api.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget_id.h"
#endif

#include "mmifmm.h"
#include "mmifmm_export.h"

#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef FM_SUPPORT//cr132759
#include "mmifm_internal.h"
#endif
/**---------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define SCH_DROP_NUM  					ALM_MODE_MAX_TYPE
#define SCH_WEEK_DAY_NUM           		7
#define MMISCH_RECT1_LEFT               12
#define MMISCH_RECT1_TOP                MMI_TITLE_HEIGHT
#define MMISCH_RECT1_RIGHT              (MMISCH_RECT1_LEFT+151)
#define MMISCH_RECT1_BOTTOM             (MMISCH_RECT1_TOP+70)

#define MMISCH_RECT2_LEFT               MMISCH_RECT1_LEFT
#define MMISCH_RECT2_TOP                (MMISCH_RECT1_BOTTOM+4)
#define MMISCH_RECT2_RIGHT              MMISCH_RECT1_RIGHT
#define MMISCH_RECT2_BOTTOM             (MMISCH_RECT2_TOP+78)

#define MMISCH_CONT_LEFT                MMISCH_TIME_TITLE_LEFT
#define MMISCH_CONT_TOP                 (MMISCH_RECT2_TOP+3)
#define MMISCH_CONT_RIGHT               MMISCH_TIME_RIGHT
#define MMISCH_CONT_BOTTOM              (MMISCH_RECT2_BOTTOM-2)

#define MMISCH_EDITTIME_TITLE_LEFT      15
#define MMISCH_EDITTIME_TITLE_TOP       (MMI_TITLE_HEIGHT+5)
#define MMISCH_EDITTIME_TITLE_RIGHT     (MMISCH_EDITTIME_TITLE_LEFT+100)
#define MMISCH_EDITTIME_TITLE_BOTTOM    (MMISCH_EDITTIME_TITLE_TOP+20)

#define MMISCH_EDITTIME_LEFT            (MMISCH_EDITTIME_TITLE_LEFT+47)
#define MMISCH_EDITTIME_TOP             (MMISCH_EDITTIME_TITLE_BOTTOM+5)
#define MMISCH_EDITTIME_RIGHT           (MMISCH_EDITTIME_LEFT+48)
#define MMISCH_EDITTIME_BOTTOM          (MMISCH_EDITTIME_TOP+17)

#define MMISCH_EDITDATE_TITLE_LEFT      MMISCH_EDITTIME_TITLE_LEFT
#define MMISCH_EDITDATE_TITLE_TOP       (MMISCH_EDITTIME_BOTTOM+20)
#define MMISCH_EDITDATE_TITLE_RIGHT     MMISCH_EDITTIME_TITLE_RIGHT
#define MMISCH_EDITDATE_TITLE_BOTTOM    (MMISCH_EDITDATE_TITLE_TOP+20)

#define MMISCH_EDITDATE_LEFT            (MMISCH_EDITTIME_TITLE_LEFT+27)
#define MMISCH_EDITDATE_TOP             (MMISCH_EDITDATE_TITLE_BOTTOM+5)
#define MMISCH_EDITDATE_RIGHT           (MMISCH_EDITDATE_LEFT+83)
#define MMISCH_EDITDATE_BOTTOM          (MMISCH_EDITDATE_TOP+17)

#define MMISCH_ALL_ICON_START_X         15//(10)
#define MMISCH_ALL_ICON_START_Y         (MMI_TITLE_HEIGHT+15)
#define MMISCH_ICON_WIDTH               28
#define MMISCH_ALL_ICON_RECT			{(MMISCH_ALL_ICON_START_X - 7), (MMISCH_ALL_ICON_START_Y - 7), 224, 263}

#define MMISCH_ALL_LIST_BOTTOM          (MMI_MAINSCREEN_HEIGHT-MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT-22)

#define SCH_DECEMBER                    12
#define MMISCH_DEADLINE_TITLE_HEIGHT    (MMISCH_DEADLINE_TITLE_BOTTOM - MMISCH_DEADLINE_TITLE_TOP + 1)
#define MMISCH_DEADLINE_HEIGHT          (MMISCH_DEADLINE_BOTTOM - MMISCH_DEADLINE_TOP + 1)

#define WEEKS_STR_BUFF_LEN  100

#define MMISCH_SMS_VCALENDAR_MAX_8BIT_SPLIT_LEN     128

#define MMISCH_VCALENDAR_DETAIL_MAX_LEN  1024

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/


extern MMI_CALENDAR_T g_calendar_show;
/*---------------------------------------------------------------------------*/
/*                          LOCAL VARIABLE DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMIACC_SCH_LIST_T	   s_user_list = {0};  

LOCAL BOOLEAN s_weeks_is_selected[SCH_WEEK_DAY_NUM]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};

LOCAL MMIACC_SCH_LIST_T	   s_unRead_event_list = {0}; 

LOCAL MMISET_DATE_T        s_all_list_cur_date = {0};

LOCAL MMISCH_LIST_WIN_PARAM_T   *s_list_param = PNULL;

#ifdef MMI_VCALENDAR_SUPPORT
LOCAL MMIVCALENDAR_INFO_T  s_vcal_info={0};
LOCAL  wchar   *s_vcalendar_export_folder_ptr = PNULL;
LOCAL MMISCH_OPERATE_RECORDS_INFO_T s_operate_sch_info={0};
LOCAL MMI_CTRL_ID_T  s_cur_vcal_list_id=0;
LOCAL BOOLEAN s_vcalendar_is_processing=FALSE;
LOCAL FILEARRAY  s_vcalendar_array = PNULL;
//LOCAL BOOLEAN    s_is_vcal_loading = FALSE;
#endif

#ifdef MMI_GUI_STYLE_MINISCREEN
LOCAL THEMELIST_ITEM_STYLE_T          s_my_style = {0};
LOCAL THEMELIST_ITEM_STYLE_T          s_my_style1 = {0};
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : view the list of current day's schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchViewListWinMsg( 
                                           MMI_WIN_ID_T		win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           );

/*****************************************************************************/
// 	Description : view the list of all schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchViewAllListWinMsg( 
                                              MMI_WIN_ID_T		win_id, 
                                              MMI_MESSAGE_ID_E	msg_id, 
                                              DPARAM				param
                                              );

/*****************************************************************************/
// 	Description : schedule list option win
//	Global resource dependence : 
//  Author: xiangjie
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchListOptionWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );

/*****************************************************************************/
// 	Description : build event list
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL void BuildEventUserList(                        
                              BOOLEAN     is_all,
                              uint16      year,
                              uint8       month,
                              uint8       day                   
                              );

/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUserListBoxItem(
                                 MMI_CTRL_ID_T	ctrl_id,
								 BOOLEAN is_have_event
                                 );

/*****************************************************************************/
// 	Description : sort event list
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL void SortEventUserList(void);

/*****************************************************************************/
// 	Description : MapListItemToUserItem
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL uint16 MapListItemToUserItem(
                                   uint16 index
                                   );

/*****************************************************************************/
// 	Description : view and edit schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSchWinMsg( 
                                       MMI_WIN_ID_T		win_id, 
                                       MMI_MESSAGE_ID_E	msg_id, 
                                       DPARAM				param
                                       );

/*****************************************************************************/
// 	Description : handle SCH weeks list windows messages
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSCHSelectweeksWinMsg(
                                          MMI_WIN_ID_T      win_id, 	
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );

/*****************************************************************************/
// 	Description : view and edit schedule
//	Global resource dependence : 
//  Author:  xiangjie
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewSchDetailWinMsg( 
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );
#if 0
/*****************************************************************************/
// 	Description : edit schedule text content
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchTextEditorWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );
#endif
/*****************************************************************************/
// 	Description : CompareDateTime
//         compare a date to another , find smaller(earlier)
//	Global resource dependence : 
//  Author:        xiangjie
//	Note:   
/*****************************************************************************/
LOCAL uint16 CompareDate(
                         SCI_DATE_T   first, 
                         SCI_DATE_T   second
                         );

/*****************************************************************************/
// 	Description : if the deadline is earlier then the date ,it is not a valid deadline
//	Global resource dependence : 
//  Author:        xiangjie
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsValidDeadline(
                              SCI_DATE_T   deadline, 
                              SCI_DATE_T   date
                              );

/*****************************************************************************/
// 	Description : schedule option win
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchOptionWinMsg(
                                         MMI_WIN_ID_T		win_id, 
                                         MMI_MESSAGE_ID_E	msg_id, 
                                         DPARAM				param
                                         );

/*****************************************************************************/
// 	Description : select schedule ring
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchRingWinMsg(
                                        MMI_WIN_ID_T		win_id, 
                                        MMI_MESSAGE_ID_E	msg_id, 
                                        DPARAM				param
                                        );

/*****************************************************************************/
// 	Description : select fixed ring
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchFixedRingWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );

#if 0
/*****************************************************************************/
// 	Description : HandleSchEditWinWebKeyMsg
//	Global resource dependence :
//  Author:  xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void HandleSchEditWinWebKeyMsg(
                                     MMI_WIN_ID_T	    win_id,
                                     MMISCH_DETAIL_WIN_PARAM_T   *win_param
                                    // MMIACC_SMART_EVENT_T        *event
                                     );
#endif
/*****************************************************************************/
// 	Description : schedule  list option win
//	Global resource dependence : 
//  Author: xiaoqing
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchAllListOptWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );

/*****************************************************************************/
// 	Description : schedule  list option win when list be marked
//	Global resource dependence : 
//  Author: xiaoqing
//	Note:   
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E HandleSchListMarkOptWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );                                             
#endif

/*****************************************************************************/
// 	Description : add menu item of schedule list option win 
//	Global resource dependence : 
//  Author: xiaoqing
//	Note:   
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL int32 AddSchListMarkOptMenuItem(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_CTRL_ID_T      ctrl_id, 
                                             MMI_CTRL_ID_T 		list_ctrl_id
                                             );
#endif

/*****************************************************************************/
// 	Description : get one date's schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetInvalidSchByDate(
                            uint16              year,
                            uint8               month,
                            uint8               day,
                            uint16              *sche_num_ptr
                            );

/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendAllUserListBoxItem(
                                 MMI_CTRL_ID_T	ctrl_id,
					             BOOLEAN is_have_event,
					             BOOLEAN is_widget_call
                                 );

/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void MoveNextDay(
                         MMISET_DATE_T  date_in,
                         MMISET_DATE_T  *date_out_ptr
                       );
                       
/*****************************************************************************/
//    Description : move one day backward
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void MovePrevDay(
                         MMISET_DATE_T  date_in,
                         MMISET_DATE_T  *date_out_ptr
                       );

/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendAllUserListBoxItemData(
                                 MMI_CTRL_ID_T	ctrl_id,
								 BOOLEAN is_have_event,
								 uint16 index
                                 );
/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUserListBoxItemData(
                                 MMI_CTRL_ID_T	ctrl_id,
								 BOOLEAN is_have_event,
								 uint16 entry_id,
								 uint32	index
                                 );

/*****************************************************************************/
// 	Description : set schedule detail win
//	Global resource dependence : 
//  Author:  yongwei.he
//	Note:   
/*****************************************************************************/
LOCAL void InitViewSchDetail_se( 
                                MMIACC_SMART_EVENT_T*	event_ptr,
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id
                                );

/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void AddOneDetailItem(
                            MMI_CTRL_ID_T   ctrl_id,
                            MMI_TEXT_ID_T   label_title,
                            MMI_TEXT_ID_T   label_name,
                            uint16          *index_ptr,
                            wchar           *detail_ptr,
                            uint32          detail_len
                            );

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:jian.ma
// Note: 
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item );  

/*****************************************************************************/
//    Description : SetTitleIndexType
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void SetTitleIndexType( MMI_CTRL_ID_T	ctrl_id, BOOLEAN is_list_has_item );

/*****************************************************************************/
// 	Description : enablel menu item
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
LOCAL void EnableListOptionMenuItem(
									 BOOLEAN					is_all,
									 uint16                   year,
									 uint8                    month,
									 uint8                    day
									 );

/*****************************************************************************/
// 	Description : HandleDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllListDelQueryWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );
/*****************************************************************************/
// 	Description : HandleDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllListDelAllQueryWin(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               );

/*****************************************************************************/
// 	Description : HandleDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleListDelQueryWin(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );
/*****************************************************************************/
// 	Description : HandleDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleListDelAllQueryWin(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
// 	Description : set name param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinNameParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         );

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDateParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         );
#ifdef CMCC_UI_STYLE

/*****************************************************************************/
// 	Description : set date end param
//	Global resource dependence : 
//  Author:  PANVS
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDateStopParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         );

#endif
//#ifdef FM_SUPPORT//CR132759
/*****************************************************************************/
//  Description : 频率编辑窗口消息处理函数
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
//LOCAL MMI_RESULT_E  HandleFMEditWinMsg(
// 									   MMI_WIN_ID_T      win_id,     
// 									   MMI_MESSAGE_ID_E  msg_id,
// 									   DPARAM            param
//                                           );

//#endif
/*****************************************************************************/
// 	Description : set time param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinTimeParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         );

/*****************************************************************************/
// 	Description : set ring param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinRingParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         );

/*****************************************************************************/
// 	Description : set freq type param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinFreqTypeParam(void);

/*****************************************************************************/
// 	Description : set deadline  param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDeadlineParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         );

/*****************************************************************************/
// 	Description : set week list param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinWeeklistParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr, //in
					                         MMI_WIN_ID_T	    win_id
					                         );

/*****************************************************************************/
// 	Description : Append the item of weeks list
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:
/*****************************************************************************/
LOCAL void AppendWeeksListItem(
                                               MMI_CTRL_ID_T    ctrl_id ,//in
                                               MMIACC_SMART_EVENT_T*	event_ptr //in
                                               );

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinWeeklistDisplay(void );

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDeadlineDisplay(void );

/*****************************************************************************/
//  Description : 设置SETLIST 的数据
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetFreqSetList(
                              MMIACC_SMART_EVENT_T*	event_ptr  //in
                              );

/*****************************************************************************/
//  Description : save the frequency mode
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SaveSchEditWinFreqMode(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              );

/*****************************************************************************/
//  Description : save the name
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinName(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              );

/*****************************************************************************/
//  Description : save the date
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinDate(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              );

/*****************************************************************************/
//  Description : save the time
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinTime(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              );
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : save the date
//  Global resource dependence : 
//  Author:panvs
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinStopDateTime(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              );

#endif
/*****************************************************************************/
//  Description : save the deadline time
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinDeadlineDate(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              );

/*****************************************************************************/
// 	Description : save the select weeks
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:
/*****************************************************************************/
LOCAL uint16 SaveSelectWeeks(
                                               MMI_CTRL_ID_T    ctrl_id ,//in
                                               MMIACC_SMART_EVENT_T*	event_ptr
                                               );

/*****************************************************************************/
// 	Description : handle SCH edit windows FormSwitchActive messages
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void  HandleSchEditFormSwitchActiveMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id
                                          );

/*****************************************************************************/
// 	Description : Set EditWin Button bg
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SCHSetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id);

/*****************************************************************************/
// 	Description : save fre
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SCHSaveFreEditWin(MMIACC_SMART_EVENT_T*	event_ptr );

/*****************************************************************************/
// 	Description : handle SCH edit windows penok messages
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void  HandleSCHEditPENOKMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id,
                                          	MMISCH_DETAIL_WIN_PARAM_T   *win_param
                                          );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SCHButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void setSCHEditSelectWeeksSoftkey(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id,
                                          	BOOLEAN is_update);

/*****************************************************************************/
// 	Description : append unread event list to ctrl
//	Global resource dependence : 
//  Author:  renyi.HU
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUnreadEventListBoxItem(
                                 MMI_CTRL_ID_T	ctrl_id
                                 );

/*****************************************************************************/
// 	Description : view the list of unread schedule
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchViewUnreadListWinMsg( 
                                           MMI_WIN_ID_T		win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           );

/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  renyi.hu
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUnreadEventListBoxItemData(
                                 MMI_CTRL_ID_T	ctrl_id,
								 uint16 entry_id,
								 uint32	index
                                 );

/*****************************************************************************/
// 	Description : view unread sch detail
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewSchUnReadDetailWinMsg( 
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );
#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
//  Description :Send Vcalendar By BT
//  Global resource dependence : none
//  Author: 
//  Note: renyi.hu
/***************************************************************************/
LOCAL void SendVcalendarByBT(uint16* event_id,uint16 num);

/*****************************************************************************/
//  Description :Send Vcalendar By MMS
//  Global resource dependence : none
//  Author: 
//  Note: renyi.hu
/***************************************************************************/
LOCAL void SendVcalendarByMMS(uint16 event_id);

/*****************************************************************************/
//  Description :Send Vcalendar By SMS
//  Global resource dependence : none
//  Author: 
//  Note: renyi.hu
/***************************************************************************/
LOCAL void SendVcalendarBySMS(uint16 event_id);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewVcalendarDetailWinMsg( 
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void  SaveVcalendarprompt(MMIVCAL_ERROR_TYPE_E error_tyep);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL uint16  GetVcalendarDetailStr(MMIVCALENDAR_INFO_T *vcal_info,wchar* buff,uint16 max_buff_len);

/*****************************************************************************/
//  Description : 选择导出的目录
//  Global resource dependence : none
//  Note: 
/***************************************************************************/
LOCAL void SelectVcalendarExportFolder(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : 选择导出的目录 callback
//  Global resource dependence : none
//  Note: 
/***************************************************************************/
LOCAL void VcalendarFolderSelectionCallBack(BOOLEAN is_success, FILEARRAY file_array);

/*****************************************************************************/
//  Description : 处理保存VCalendar
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSaveVCalendar(MMISCH_OPERATE_RECORDS_INFO_T *info_ptr, const wchar *path_name_ptr);

/*****************************************************************************/
// Description : 将选定日程条目保存为VCalendar格式的文件
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveOneVCalendar(uint16 event_id, const wchar *path_name_ptr);

/*****************************************************************************/
//  Description : HandleSchWaitWinMsg  
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : SchOpenWaitWin 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void SchOpenWaitWin(MMIPUB_HANDLE_FUNC handle_func);

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchProcessWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : 导入vcalendar到日程
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void SchLoadAllVCalendar(void);

/*****************************************************************************/
//  Description : Call back function for adding file to sch.
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void SchLoadVcalendarCallBack(
                    BOOLEAN     is_success,
                    FILEARRAY   file_array
                    );

/*****************************************************************************/
//  Description : 导入VCalendar到sch
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL MMI_RESULT_E  HandleLoadVCalendarWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
/*****************************************************************************/
//  Description : 导入VCalendar到sch
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void  HandleSaveSchCNF
(
    MMI_CTRL_ID_T current_ctrl_id,
    DPARAM        param
);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void  HandleDeleteSchCNF
(
    MMI_CTRL_ID_T current_ctrl_id,
    DPARAM        param
);

/*****************************************************************************/
//  Description : HandleSchDelWaitWinMsg  
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchDelWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchDelProcessWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DelOneSch(void);

/*****************************************************************************/
//  Description : 删除sch
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void DeleteSch(MMISCH_OPERATE_RECORDS_INFO_T *info_ptr,MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : HandleListDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelMarkQueryWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description : HandleListDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllDelMarkQueryWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

/*****************************************************************************/
//  Description : HandleEditTimeMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/*****************************************************************************/
//  Description : HandleEditDateMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditDateMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

/*****************************************************************************/
//  Description : HandleEditDeadlineMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditDeadlineMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

#endif

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :录音 后 的 处 理  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AlarmRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr);
#endif
#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description : Record button CallBack
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E  RecordCalendarRingCallBack(void);
#endif
/**---------------------------------------------------------------------------*
**                         CONSTANT DEFINITION                               *
**---------------------------------------------------------------------------*/
// #ifdef FM_SUPPORT //cr132759
// WINDOW_TABLE( SCH_FM_EDITWIN_TAB ) = 
// {
//     WIN_FUNC((uint32) MMIAPIALM_HandleFMEditWinMsg), 
// 		WIN_ID(MMIALM_FM_EDIT_WIN_ID),
// 		WIN_STATUSBAR,
// 		WIN_TITLE(TXT_ALARM_FM_RING),
// 		WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
// #ifdef MMI_PDA_SUPPORT
// 		CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_SELECT_FM_LIST_CTRL_ID),
// #else
// 		CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_FORM_CTRL_ID),
//         CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_LIST_FORM_CTRL_ID,MMI_FM_EDITWIN_FORM_CTRL_ID),
// 		CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID,MMI_FM_EDITWIN_LIST_FORM_CTRL_ID),
//         CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID,MMI_FM_EDITWIN_FORM_CTRL_ID),
// 		CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMI_FM_EDITWIN_FRE_LABEL_CTRL_ID,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID),
// 		CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_FM_MAX_LEN,MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID,MMI_FM_EDITWIN_FRE_FORM_CTRL_ID),          
// #endif
// 		END_WIN
// };
// #endif
//日程表的浏览列表
WINDOW_TABLE( SCH_VIEW_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSchViewListWinMsg ),    
    WIN_ID( MMISCH_VIEW_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE),
#endif
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,      //junxue zhu
#endif
    WIN_TITLE(TXT_NULL),

#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_SCH_PREVIOUS_DAY, TXT_NULL, TXT_SCH_NEXT_DAY),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISCH_VIEW_LIST_CTRL_ID),
    END_WIN
}; 

// 所有日程表的浏览列表
WINDOW_TABLE( SCH_VIEW_ALL_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSchViewAllListWinMsg ),    
    WIN_ID( MMISCH_VIEW_ALL_WIN_ID ),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,      //junxue zhu
#endif
    WIN_TITLE(TXT_CALENDAR_ALL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISCH_VIEW_ALL_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

WINDOW_TABLE( SCH_LIST_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSchListOptionWinMsg),
	WIN_ID(MMISCH_LIST_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SCH_OPT,MMISCH_LIST_OPTION_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
	END_WIN
 }; 

WINDOW_TABLE( SCH_ALL_LIST_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSchAllListOptWinMsg),
    WIN_ID(MMISCH_ALL_LIST_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SCH_ALL_OPT,MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
}; 

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( SCH_LIST_MARK_OPTION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSchListMarkOptWinMsg),
    WIN_ID(MMISCH_LIST_MARK_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    //CREATE_POPMENU_CTRL(MENU_SCH_ALL_OPT,MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
}; 
#endif

//查看日程表的详细内容
WINDOW_TABLE( SCH_VIEW_DETAIL_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleViewSchDetailWinMsg ),    
    WIN_ID( MMISCH_VIEW_DETAIL_WIN_ID ), 
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_IDLE_CALENDAR),
    WIN_BACKGROUND_ID((MMI_IMAGE_ID_T)IMAGE_COMMON_BG),         
    CREATE_RICHTEXT_CTRL(MMISCH_VIEW_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
}; 
#ifdef MMI_VCALENDAR_SUPPORT
WINDOW_TABLE( MMISCH_VCALENDAR_DETAIL_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleViewVcalendarDetailWinMsg ),    
    WIN_ID( MMISCH_VCALENDAR_DETAIL_WIN_ID ), 
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_TITLE(TXT_IDLE_CALENDAR),
    WIN_BACKGROUND_ID((MMI_IMAGE_ID_T)IMAGE_COMMON_BG),         
    CREATE_TEXT_CTRL(MMISCH_VCALENDAR_DETAIL_TEXT_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, STXT_SAVE, TXT_NULL),
#else
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
}; 
#endif
//编辑日程表的内容

WINDOW_TABLE( SCH_EDIT_WIN_TAB ) = 
{
	WIN_FUNC((uint32) HandleEditSchWinMsg ),    
	WIN_ID( MMISCH_EDIT_WIN_ID ), 
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_TITLE(TXT_IDLE_CALENDAR),

	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISCH_EDIT_WIN_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_NAME_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_NAME_TITLE_LABEL_CTRL_ID,MMISCH_NAME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISCH_NAME_EDIT_LABEL_CTRL_ID,MMISCH_NAME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIACC_EVENT_TEXT_MAX_LEN,MMISCH_NAME_EDIT_CTRL_ID,MMISCH_NAME_FORM_CTRL_ID),
#endif

#ifdef CMCC_UI_STYLE
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_TEXT_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_TEXT_TITLE_LABEL_CTRL_ID,MMISCH_TEXT_FORM_CTRL_ID),
	    CHILD_EDIT_TEXT_CTRL(TRUE,MMIACC_EVENT_TEXT_MAX_LEN,MMISCH_TEXT_EDIT_CTRL_ID,MMISCH_TEXT_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_DATE_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_DATE_TITLE_LABEL_CTRL_ID,MMISCH_DATE_FORM_CTRL_ID),
	    CHILD_EDIT_DATE_CTRL(TRUE,MMISCH_DATE_EDIT_CTRL_ID,MMISCH_DATE_FORM_CTRL_ID),
	    CHILD_EDIT_TIME_CTRL(TRUE,MMISCH_TIME_EDIT_CTRL_ID,MMISCH_DATE_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_DATE_STOP_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_DATE_STOP_TITLE_LABEL_CTRL_ID,MMISCH_DATE_STOP_FORM_CTRL_ID),
	    CHILD_EDIT_DATE_CTRL(TRUE,MMISCH_DATE_STOP_EDIT_CTRL_ID,MMISCH_DATE_STOP_FORM_CTRL_ID),
	    CHILD_EDIT_TIME_CTRL(TRUE,MMISCH_TIME_STOP_EDIT_CTRL_ID,MMISCH_DATE_STOP_FORM_CTRL_ID),
#else
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_DATE_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_DATE_TITLE_LABEL_CTRL_ID,MMISCH_DATE_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISCH_DATE_EDIT_LABEL_CTRL_ID,MMISCH_DATE_FORM_CTRL_ID),
#else
        CHILD_EDIT_DATE_CTRL(TRUE,MMISCH_DATE_EDIT_CTRL_ID,MMISCH_DATE_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_TIME_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_TIME_TITLE_LABEL_CTRL_ID,MMISCH_TIME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISCH_TIME_EDIT_LABEL_CTRL_ID,MMISCH_TIME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TIME_CTRL(TRUE,MMISCH_TIME_EDIT_CTRL_ID,MMISCH_TIME_FORM_CTRL_ID),
#endif

#endif

//#ifdef MMI_PDA_SUPPORT

     CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISCH_RING_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
     CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_RECORD_RING_FORM_CTRL_ID,MMISCH_RING_FORM_CTRL_ID),
       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_RING_TITLE_LABEL_CTRL_ID,MMISCH_RECORD_RING_FORM_CTRL_ID),
       CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISCH_RING_LABEL_CTRL_ID,MMISCH_RECORD_RING_FORM_CTRL_ID),
#ifndef MMI_RECORD_SUPPORT
	   CHILD_BUTTON_CTRL(FALSE, IMAGE_DROPDOWNLIST_ARROW_PR, MMISCH_RECORD_RING_BUTTON_CTRL_ID, MMISCH_RING_FORM_CTRL_ID),
#else
       CHILD_BUTTON_CTRL(TRUE, IMG_SCH_RECORD_ICON, MMISCH_RECORD_RING_BUTTON_CTRL_ID, MMISCH_RING_FORM_CTRL_ID),
#endif
//#else
//    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_RING_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
//	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_RING_TITLE_LABEL_CTRL_ID,MMISCH_RING_FORM_CTRL_ID),
//        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMISCH_RING_LABEL_CTRL_ID,MMISCH_RING_FORM_CTRL_ID),
//#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISCH_FREQ_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_FREQ_TYPE_FORM_CTRL_ID,MMISCH_FREQ_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_FREQ_TITLE_LABEL_CTRL_ID,MMISCH_FREQ_TYPE_FORM_CTRL_ID),
	    CHILD_SETLIST_CTRL(TRUE,MMISCH_FREQ_SETLIST_CTRL_ID,MMISCH_FREQ_TYPE_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(FALSE, IMAGE_DROPDOWNLIST_ARROW_PR, MMISCH_FREQ_TYPE_BUTTON_CTRL_ID, MMISCH_FREQ_FORM_CTRL_ID),
#else
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_FREQ_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_FREQ_TITLE_LABEL_CTRL_ID,MMISCH_FREQ_FORM_CTRL_ID),
	    CHILD_SETLIST_CTRL(TRUE,MMISCH_FREQ_SETLIST_CTRL_ID,MMISCH_FREQ_FORM_CTRL_ID),

#endif

	//CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMISCH_WEEK_LIST_LABEL_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISCH_EDITWIN_BUTTON0_CTRL_ID,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISCH_EDITWIN_BUTTON1_CTRL_ID,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISCH_EDITWIN_BUTTON2_CTRL_ID,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISCH_EDITWIN_BUTTON3_CTRL_ID,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISCH_EDITWIN_BUTTON4_CTRL_ID,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISCH_EDITWIN_BUTTON5_CTRL_ID,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMISCH_EDITWIN_BUTTON6_CTRL_ID,MMISCH_EDITWIN_CHILD_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_DEADLINE_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_DEADLINE_TITLE_LABEL_CTRL_ID,MMISCH_DEADLINE_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMISCH_DEADLINE_EDIT_LABEL_CTRL_ID,MMISCH_DEADLINE_FORM_CTRL_ID),
#else
        CHILD_EDIT_DATE_CTRL(TRUE,MMISCH_DEADLINE_EDIT_CTRL_ID,MMISCH_DEADLINE_FORM_CTRL_ID),
#endif

#ifdef CMCC_UI_STYLE
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISCH_REMIND_FORM_CTRL_ID,MMISCH_EDIT_WIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMISCH_REMIND_TITLE_LABLE_CTRL_ID,MMISCH_REMIND_FORM_CTRL_ID),
	    CHILD_SETLIST_CTRL(TRUE,MMISCH_REMIND_SETLIST_CTRL_ID,MMISCH_REMIND_FORM_CTRL_ID),
#endif

#ifdef MMI_PDA_SUPPORT
    //WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
#endif
	END_WIN

}; 

//select the weeks
WINDOW_TABLE( SCH_EDITWIN_SELECT_WEEKS_TAB ) = 
{
	WIN_FUNC((uint32) HandleSCHSelectweeksWinMsg),  
	WIN_TITLE(TXT_SCH_AWAKE_EVERYWEEK),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	
	CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMISCH_EDITWIN_WEEKS_LIST_CTRL_ID),
	
    WIN_ID(MMISCH_SELECT_WEEKS_WIN_ID),
        
    END_WIN
}; 


WINDOW_TABLE(SCH_OPTION_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleSchOptionWinMsg),
    WIN_ID(MMISCH_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_SCH,MMISCH_OPTION_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( SCH_EDIT_RING_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSchRingWinMsg ),    
    WIN_ID( MMISCH_RINGLIST_WIN_ID ),
//#ifdef MMI_PDA_SUPPORT
    //WIN_STATUSBAR,
//#endif
    //WIN_TITLE(TXT_RING),
    CREATE_POPMENU_CTRL(MENU_SCH_SETTING_RING, MMISCH_RINGLIST_CTRL_ID),
    END_WIN
};
#else
// set ring
WINDOW_TABLE( SCH_EDIT_RING_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSchRingWinMsg ),    
    WIN_ID( MMISCH_RINGLIST_WIN_ID ),
    WIN_TITLE(TXT_RING),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISCH_RINGLIST_CTRL_ID),
    END_WIN
}; 
#endif
// set fixed ring
WINDOW_TABLE( SCH_FIXEDRINGWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSchFixedRingWinMsg ),    
    WIN_ID( MMISCH_FIXEDRINGLIST_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_TITLE(TXT_RING_FIXED),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISCH_FIXEDRINGLIST_CTRL_ID),
    END_WIN
}; 

//键盘锁后未处理的日程表的浏览列表
WINDOW_TABLE( SCH_VIEW_UNREAD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSchViewUnreadListWinMsg),    
    WIN_ID( MMISCH_VIEW_UNREAD_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_CALENDAR_UNREAD_SCH),
    WIN_SOFTKEY(TXT_VIEW, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISCH_VIEW_UNREAD_LIST_CTRL_ID),
    END_WIN
}; 

//查看未处理的日程表的详细内容
WINDOW_TABLE( SCH_VIEW_UNREAD_DETAIL_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleViewSchUnReadDetailWinMsg ),    
    WIN_ID( MMISCH_VIEW_UNREAD_DETAIL_WIN_ID ), 
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_IDLE_CALENDAR),
    WIN_BACKGROUND_ID((MMI_IMAGE_ID_T)IMAGE_COMMON_BG),         
    CREATE_RICHTEXT_CTRL(MMISCH_VIEW_DETAIL_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMISCH_NAMEWIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditNameMsg),    
    WIN_ID(MMISCH_NAMEWIN_ID),
    WIN_TITLE(TXT_COMMON_SUBJECT),
    CREATE_EDIT_TEXT_CTRL(MMIACC_EVENT_TEXT_MAX_LEN,MMISCH_NAME_EDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};
WINDOW_TABLE( MMISCH_TIMEWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditTimeMsg ),    
    WIN_ID( MMISCH_TIMEWIN_ID ),
    WIN_TITLE( TXT_TIME ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISCH_EDIT_TIME_FORM_CTRL_ID),
    CHILD_EDIT_TOUCH_TIME_CTRL(TRUE,MMISCH_TIME_EDIT_CTRL_ID,MMISCH_EDIT_TIME_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
WINDOW_TABLE( MMISCH_DATEWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditDateMsg ),    
    WIN_ID( MMISCH_DATEWIN_ID ),
    WIN_TITLE( TXT_DATE ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISCH_EDIT_DATE_FORM_CTRL_ID),
    CHILD_EDIT_TOUCH_DATE_CTRL(TRUE,MMISCH_DATE_EDIT_CTRL_ID,MMISCH_EDIT_DATE_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMISCH_DEADLINEWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditDeadlineMsg ),    
    WIN_ID( MMISCH_DEADLINEWIN_ID ),
    WIN_TITLE( TXT_CALENDAR_EDIT_DEADLINE ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISCH_EDIT_DEADLINE_FORM_CTRL_ID),
    CHILD_EDIT_TOUCH_DATE_CTRL(TRUE,MMISCH_DEADLINE_EDIT_CTRL_ID,MMISCH_EDIT_DEADLINE_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

/**---------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : open schedule view list win
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenViewListWin(ADD_DATA win_param)
{    
// 	s_list_param = SCI_ALLOC_APP(sizeof(MMISCH_LIST_WIN_PARAM_T));
// 	SCI_MEMSET(s_list_param, 0, sizeof(MMISCH_LIST_WIN_PARAM_T));
    if (MMK_IsOpenWin(MMISCH_VIEW_WIN_ID))
    {
    	MMK_CloseWin(MMISCH_VIEW_WIN_ID);
    }
    
    if (PNULL == s_list_param)
    {
        s_list_param = SCI_ALLOC_APP(sizeof(MMISCH_LIST_WIN_PARAM_T));
    }
    if (PNULL!=s_list_param)
    {
        SCI_MEMSET(s_list_param, 0, sizeof(MMISCH_LIST_WIN_PARAM_T));
    }
    else
    {
        return;
    }
    s_list_param->day = ((MMISCH_LIST_WIN_PARAM_T *)win_param)->day;
    s_list_param->year = ((MMISCH_LIST_WIN_PARAM_T *)win_param)->year;
    s_list_param->month = ((MMISCH_LIST_WIN_PARAM_T *)win_param)->month;
		
    MMK_CreateWin((uint32 *)SCH_VIEW_WIN_TAB, s_list_param);
}
/*****************************************************************************/
// 	Description : open child schedule view list win
//	Global resource dependence : none
//  Author: maryxiao
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenChildViewAllListWin(MMI_WIN_ID_T win_id , ADD_DATA win_param)
{
    if(MMK_IsOpenWin(MMISCH_VIEW_ALL_WIN_ID))
    {
        MMK_CloseWin(MMISCH_VIEW_ALL_WIN_ID);
    }	
    MMK_CreateChildWin(win_id, (uint32 *)SCH_VIEW_ALL_WIN_TAB, win_param);
}
/*****************************************************************************/
// 	Description : open schedule view list all win
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenViewListAllWin(ADD_DATA win_param)
{
    MMK_CreateWin((uint32 *)SCH_VIEW_ALL_WIN_TAB, win_param);
}

/*****************************************************************************/
// 	Description : open schedule view detail win
//	Global resource dependence : none
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenEditWin(ADD_DATA win_param)
{
    MMK_CreateWin((uint32 *)SCH_EDIT_WIN_TAB, win_param);
}

/*****************************************************************************/
// 	Description : open event list
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_OpenEventList(
                            BOOLEAN         is_all,
                            uint16          year,
                            uint8           month,
                            uint8           day                
                            )
{
    BuildEventUserList(is_all, year, month, day);     
    
    if (0 == s_user_list.total)      //  list empty       
    {
        return FALSE;
    }
    else       
    {
        SortEventUserList();
        return TRUE;
    }       
}

/*****************************************************************************/
// 	Description : build event list
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL void BuildEventUserList( 
                              BOOLEAN     is_all,
                              uint16      year,
                              uint8       month,
                              uint8       day
                              )
{
    uint16                      i           = 0; 
    uint16                      start_id    = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T  *event_ptr  = NULL;
    SCI_DATE_T		event_start_date                        = {0};
    SCI_DATE_T		event_end_date                          = {0};
    SCI_DATE_T      cur_date                                = {0};
    uint8           week_day = 0;

    cur_date.year = year;
    cur_date.mon= month;
    cur_date.mday= day;    
	
    SCI_MEMSET(((uint8 *)&s_user_list), 0, sizeof(MMIACC_SCH_LIST_T));
	
    start_id = EVENT_SCH_0;
    
    // read all of the content of Event to local RAM
    for (i = 0; i<MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(start_id + i));
        
        // if event is ON, build it to view list
        if (!is_all)
        {
            event_start_date.year = event_ptr->start_year;
            event_start_date.mon  = event_ptr->start_mon;
            event_start_date.mday = event_ptr->start_day;
            event_end_date.year = event_ptr->end_year;
            event_end_date.mon  = event_ptr->end_mon;
            event_end_date.mday = event_ptr->end_day;	

            switch (event_ptr->fre_mode)
            {
            case ALM_MODE_ONCE:
            case ALM_MODE_NO:
                if( event_ptr->is_valid &&
                    year == event_ptr->year &&
                    month == event_ptr->mon &&
                    day == event_ptr->day
                    )
                {
                    s_user_list.entry_id[s_user_list.total] = start_id + i;
                    s_user_list.total++;
                }
                break;

            case ALM_MODE_EVERYDAY:
                if (event_ptr->is_valid &&
                MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)
                )
                {
                    s_user_list.entry_id[s_user_list.total] = start_id + i;
                    s_user_list.total++;
                }
                break;

            case ALM_MODE_EVERYWEEK:
                week_day = MMICALENDAR_GetWeekday(year,month,day);
                if(event_ptr->is_valid &&
                    MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                    ((event_ptr->fre & s_week_mask[week_day]) != 0)
                    )
                {
                    s_user_list.entry_id[s_user_list.total] = start_id + i;
                    s_user_list.total++;
                }					
                break;
				
            case ALM_MODE_EVERYMONTH:
                if (event_ptr->is_valid &&
                    MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                    day == event_ptr->day 
                )
                {
                    s_user_list.entry_id[s_user_list.total] = start_id + i;
                    s_user_list.total++;
                }
                break;

            case ALM_MODE_EVERYYEAR:
                if (event_ptr->is_valid &&
                    MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                    month == event_ptr->mon &&
                    day == event_ptr->day 
                )
                {
                    s_user_list.entry_id[s_user_list.total] = start_id + i;
                    s_user_list.total++;
                }
                break;

            default:
                break;
            }	
        }
        else
        {
            if (event_ptr->is_valid)
            {   
                s_user_list.entry_id[s_user_list.total] = start_id + i;
                s_user_list.total++;
            }
        }
    }
    
    return;
}

/*****************************************************************************/
// 	Description : sort event list
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL void SortEventUserList(void)
{
    uint32                  i = 0;
    uint32                  j = 0;
    uint16                  change = 0;
    MMIACC_SMART_EVENT_FAST_TABLE_T    event1 = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T    event2 = {0};
    
    if(s_user_list.total==0)
    {
    	return;
    }
    
    for(i=0;i<s_user_list.total-1; i++)/*lint !e574*/
    {
        for(j=i+1; j<s_user_list.total; j++)
        {
            SCI_MEMCPY(&event1,  MMIALM_GeEvent(s_user_list.entry_id[i]), sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
            SCI_MEMCPY(&event2,  MMIALM_GeEvent(s_user_list.entry_id[j]), sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
            if (MMIAPIALM_CompareTwoEvent(&event1, &event2) == CMP_SECOND_SMALL)
            {
                change = s_user_list.entry_id[i];
                s_user_list.entry_id[i] = s_user_list.entry_id[j];
                s_user_list.entry_id[j] = change;
            }
        }
    }
    
    return;
}

/*****************************************************************************/
// 	Description : sort event list
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SortEventUserListEx(MMIACC_SCH_LIST_T *user_list)
{
    uint32                  i = 0;
    uint32                  j = 0;
    uint16                  change = 0;
    MMIACC_SMART_EVENT_FAST_TABLE_T    event1 = {0};
    MMIACC_SMART_EVENT_FAST_TABLE_T    event2 = {0};
    
    if(user_list->total==0)
    {
    	return;
    }
    
    for(i=0;i<user_list->total-1; i++)/*lint !e574*/
    {
        for(j=i+1; j<user_list->total; j++)
        {
            SCI_MEMCPY(&event1,  MMIALM_GeEvent(user_list->entry_id[i]), sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
            SCI_MEMCPY(&event2,  MMIALM_GeEvent(user_list->entry_id[j]), sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
            if (MMIAPIALM_CompareTwoEvent(&event1, &event2) == CMP_SECOND_SMALL)
            {
                change = user_list->entry_id[i];
                user_list->entry_id[i] = user_list->entry_id[j];
                user_list->entry_id[j] = change;
            }
        }
    }
    
    return;
}

/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUserListBoxItem(
                                 MMI_CTRL_ID_T	ctrl_id,
								 BOOLEAN is_have_event
                                 )
{
    int16 i = 0;
    GUILIST_ITEM_T			item_t= {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data= {0};/*lint !e64*/
#if defined(MMI_GUI_STYLE_MINISCREEN) || defined(MAINLCD_SIZE_128X160)
	THEMELIST_ITEM_STYLE_T  *pitem_style = PNULL;
	int scroll_index = 0;
#endif	
    //SCI_ASSERT( ctrl_id !=0 );/*assert verified*/
	if(ctrl_id == 0)
	{
	    return;
    }
    
    if(!is_have_event)
	{
		/*GUILIST_SetMaxItem( ctrl_id, 1, FALSE );//max item 1
		//item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
		item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
        
        item_data.item_content[0].item_data.text_id = TXT_BLANK;
		item_t.item_data_ptr = &item_data;
        GUILIST_AppendItem(ctrl_id, &item_t); */
		return;
	}
	else
	{
		GUILIST_SetMaxItem( ctrl_id, s_user_list.total, TRUE );//max item 100

#ifdef MMI_GUI_STYLE_MINISCREEN	
		item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
#else
		item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
#endif
        item_t.item_data_ptr = &item_data;
#ifdef MAINLCD_SIZE_128X160 //只有128X160用简化方式(无icon)
		pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT);
		scroll_index = 0;
#else
#ifdef MMI_GUI_STYLE_MINISCREEN
		pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT);
		scroll_index = 1;
#endif
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
		if (pitem_style)
		{	
//cr128967
// 			MMI_STRING_T        *time_string = SCI_ALLOC_APPZ(sizeof(MMI_STRING_T));
// 			if (PNULL != time_string)
// 			{
// 				time_string->wstr_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*GUILIST_STRING_MAX_NUM);
// 				if (PNULL != time_string->wstr_ptr)
// 				{
				// 	GUISTR_STYLE_T      font_style = {0};
				// 	GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN|GUISTR_STATE_ANGLE;
				// 	uint16              time_width = 0;
				// 	MMIACC_SMART_EVENT_T    smart_event = {0};
				// 	uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
					SCI_MEMCPY(&s_my_style, pitem_style, sizeof(THEMELIST_ITEM_STYLE_T));
					
				// 	MMIAPIALM_FormInteEventInfo(s_user_list.entry_id[0],&smart_event);
					//time
// 					SCI_MEMSET(tmp_item,0,strlen((char*)tmp_item));/*lint !e666*/
// 					MMIAPISET_FormatTimeStrByTime(
// 						smart_event.event_fast_info.hour,
// 						smart_event.event_fast_info.minute,
// 						tmp_item,
// 						GUILIST_STRING_MAX_NUM
// 						);
// 					MMI_STRNTOWSTR( time_string->wstr_ptr,
// 						GUILIST_STRING_MAX_NUM,   //2
// 						tmp_item,
// 						strlen((char*)tmp_item),
// 						strlen((char*)tmp_item));
// 
// 					time_string->wstr_len = strlen((char*)tmp_item);
// 					
// 					font_style.font = s_my_style.content[scroll_index].font_focus;
// 					time_width = GUISTR_GetStringWidth(&font_style, time_string, state);
// 					
					s_my_style.content[scroll_index].rect.right = MMI_LIST_ITEM_RIGHT*3/5;// s_my_style.content[scroll_index].rect.left + time_width;
					s_my_style.content[scroll_index].rect_focus.right = s_my_style.content[scroll_index].rect.right;
					s_my_style.content[scroll_index+2].rect.right = s_my_style.content[scroll_index+1].rect.right * 4/5;;
					s_my_style.content[scroll_index+2].rect_focus.right = s_my_style.content[scroll_index+2].rect.right;

					s_my_style.content[scroll_index].scale_left = s_my_style.content[scroll_index].scale_right = 0;
					s_my_style.content[scroll_index+2].scale_left = s_my_style.content[scroll_index+2].scale_right = 0;

				    s_my_style.main_index = scroll_index+2;
// 					if (PNULL != time_string->wstr_ptr)
// 					{
// 						SCI_FREE(time_string->wstr_ptr);
// 						time_string->wstr_ptr = PNULL;
// 					}
// 					if (PNULL != time_string)/*lint !e774*/
// 					{
// 						SCI_FREE(time_string);
// 						time_string = PNULL;
// 					}
// 				}
// 				else
// 				{
// 					if (PNULL != time_string)/*lint !e774*/
// 					{
// 						SCI_FREE(time_string);
// 						time_string = PNULL;
// 					}
// 				}
		 // 	}	
		}
#endif
		for( i = 0; i < s_user_list.total; i++ )
		{
			item_t.user_data = s_user_list.entry_id[i];
			GUILIST_AppendItem(ctrl_id, &item_t);
#ifdef MMI_GUI_STYLE_MINISCREEN
			GUILIST_SetItemStyleEx(ctrl_id, i, &s_my_style);
#endif
		}
	}
 
    return;
}
/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUserListBoxItemData(
                                 MMI_CTRL_ID_T	ctrl_id,
								 BOOLEAN is_have_event,
								 uint16 entry_id,
								 uint32	index
                                 )
{
    GUILIST_ITEM_T			item_t= {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data= {0};  /*lint !e64*/
//    const MMIACC_SMART_EVENT_T* event_ptr = MMIALM_GeEvent(entry_id);
    uint8  item_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar  item_wstr[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    MMIACC_SMART_EVENT_T    smart_event = {0};

    MMIAPIALM_FormInteEventInfo(entry_id,&smart_event);
#ifdef MAINLCD_DEV_SIZE_128X160
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type
        = item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

	MMIAPISET_FormatTimeStrByTime(smart_event.event_fast_info.hour,smart_event.event_fast_info.minute,item_str,MMIALM_TIME_STR_12HOURS_LEN + 1);

    item_data.item_content[0].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)item_str);

    item_data.item_content[0].item_data.text_buffer.wstr_ptr = item_wstr;

    MMI_STRNTOWSTR( item_wstr,
        item_data.item_content[0].item_data.text_buffer.wstr_len,
        item_str,
        strlen((char*)item_str),
        strlen((char*)item_str));
    

    if (0 !=smart_event.event_content_info.w_length)
    {
        item_data.item_content[2].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
    }
    else
    {   
        item_data.item_content[2].item_data_type= GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = TXT_BLANK;
    }


#else
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    if (0 == smart_event.event_content_info.image_id )
    {
        smart_event.event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
    }
    item_data.item_content[0].item_data.image_id = smart_event.event_content_info.image_id;
    
    item_data.item_content[1].item_data_type 
        = item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

	MMIAPISET_FormatTimeStrByTime(smart_event.event_fast_info.hour,smart_event.event_fast_info.minute,item_str,MMIALM_TIME_STR_12HOURS_LEN + 1);

    item_data.item_content[1].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)item_str);

    item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_wstr;

    MMI_STRNTOWSTR( item_wstr,
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        item_str,
        strlen((char*)item_str),
        strlen((char*)item_str));
    

    if (0 !=smart_event.event_content_info.w_length)
    {
        item_data.item_content[3].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
    }
    else
    {   
        item_data.item_content[3].item_data_type= GUIITEM_DATA_TEXT_ID;
        item_data.item_content[3].item_data.text_id = TXT_BLANK;
    }
#endif

    item_data.softkey_id[0] = STXT_OPTION;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#else
    item_data.softkey_id[1] = TXT_NULL;
#endif
    item_data.softkey_id[2] = STXT_RETURN;
    
	GUILIST_SetItemData( ctrl_id, &item_data, index );      
	
}
/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendAllUserListBoxItem(
                                    MMI_CTRL_ID_T	ctrl_id,
					                BOOLEAN is_have_event,
					                BOOLEAN is_widget_call
                                 )
{
    uint16 i = 0;
    GUILIST_ITEM_T			item_t= {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data= {0}; /*lint !e64*/  
#if defined(MMI_GUI_STYLE_MINISCREEN) || defined(MAINLCD_SIZE_128X160)
    THEMELIST_ITEM_STYLE_T  *pitem_style = PNULL;
    int scroll_index = 0;
#endif	
    //SCI_ASSERT( ctrl_id !=0 );/*assert verified*/
    if(ctrl_id == 0)
    {
        return;
    }
    
    if(!is_have_event)
    {
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
        GUILIST_RemoveAllItems(MMIWIDGET_SCHEDULE_LIST_CTRL_ID);
#endif        
        return;
    }    
    else
    {
        GUILIST_SetMaxItem( ctrl_id, s_user_list.total, TRUE ); //max item 100
        if(is_widget_call)
        {
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL;
        }
        else
        {

#ifdef MMI_GUI_STYLE_MINISCREEN
            item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
#else
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
#endif
            item_t.item_data_ptr = &item_data;
        }
#ifdef MAINLCD_SIZE_128X160 //只有128X160用简化方式(无icon)
        pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT);
        scroll_index = 0;
#else
#ifdef MMI_GUI_STYLE_MINISCREEN
        pitem_style = THEMELIST_GetStyle(GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT);
        scroll_index = 1;
#endif
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
        if (pitem_style)
        {	//CR128967
// 			MMI_STRING_T        *time_string = SCI_ALLOC_APPZ(sizeof(MMI_STRING_T));
// 			if (PNULL != time_string)
// 			{
// 				time_string->wstr_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*GUILIST_STRING_MAX_NUM);
// 				if (PNULL != time_string->wstr_ptr)
// 				{
// 					GUISTR_STYLE_T      font_style = {0};
// 					GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN|GUISTR_STATE_ANGLE;
// 					uint16              year_width = 0;
// 					uint16              time_width = 0;
// 					uint16              vis_lcd_width = 0;
// 					MMIACC_SMART_EVENT_T    smart_event = {0};
// 					uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
                    SCI_MEMCPY(&s_my_style1, pitem_style, sizeof(THEMELIST_ITEM_STYLE_T));
// 					
// 					MMIAPIALM_FormInteEventInfo(s_user_list.entry_id[0],&smart_event);
// 					//year
// 					SCI_MEMSET(tmp_item,0,strlen((char*)tmp_item));/*lint !e666*/
// 					MMIAPISET_FormatDateStrByDateStyle(smart_event.event_fast_info.start_year,
// 						smart_event.event_fast_info.start_mon,smart_event.event_fast_info.start_day,
// 						'/',tmp_item,GUILIST_STRING_MAX_NUM);
// 					
// 					MMI_STRNTOWSTR( time_string->wstr_ptr,
// 						GUILIST_STRING_MAX_NUM,   
// 						tmp_item,
// 						strlen((char*)tmp_item),
// 						strlen((char*)tmp_item));
// 
// 					time_string->wstr_len = strlen(tmp_item);
// 					font_style.font = s_my_style1.content[scroll_index].font_focus;
// 					year_width = GUISTR_GetStringWidth(&font_style, time_string, state);
// 
// 					//time
// 					SCI_MEMSET(tmp_item,0,strlen((char*)tmp_item));/*lint !e666*/
// 					MMIAPISET_FormatTimeStrByTime(
// 						smart_event.event_fast_info.hour,
// 						smart_event.event_fast_info.minute,
// 						tmp_item,
// 						GUILIST_STRING_MAX_NUM
// 							 );
// 					MMI_STRNTOWSTR( time_string->wstr_ptr,
// 						GUILIST_STRING_MAX_NUM,   //2
// 						tmp_item,
// 						strlen((char*)tmp_item),
// 						strlen((char*)tmp_item));
// 
// 					time_string->wstr_len = strlen(tmp_item);
// 					font_style.font = s_my_style1.content[scroll_index+1].font_focus;
// 					time_width = GUISTR_GetStringWidth(&font_style, time_string, state);
// 					
// 					vis_lcd_width = MMITHEME_GetMainScreenRightPixel() - SCROLL_BAR_WIDTH - 1;

                    s_my_style1.content[scroll_index].rect.right = MMITHEME_GetMainScreenRightPixel()*3/5;
                    s_my_style1.content[scroll_index].rect_focus.right = s_my_style1.content[scroll_index].rect.right;

                    s_my_style1.content[scroll_index+1].rect.left = s_my_style1.content[scroll_index].rect_focus.right;
                    s_my_style1.content[scroll_index+1].rect_focus.left = s_my_style1.content[scroll_index+1].rect.left;

                    s_my_style1.content[scroll_index+1].rect.right = MMI_LIST_ITEM_RIGHT;
                    s_my_style1.content[scroll_index+1].rect_focus.right = s_my_style1.content[scroll_index+1].rect.right;

                    s_my_style1.content[scroll_index+2].rect.right = MMI_LIST_ITEM_RIGHT*4/5;
                    s_my_style1.content[scroll_index+2].rect_focus.right = s_my_style1.content[scroll_index+2].rect.right;
                    s_my_style1.content[scroll_index].scale_left = s_my_style1.content[scroll_index].scale_right = 0;
                    s_my_style1.content[scroll_index+1].scale_left = s_my_style1.content[scroll_index+1].scale_right = 0;
                    s_my_style1.content[scroll_index+2].scale_left = s_my_style1.content[scroll_index+2].scale_right = 0;
                    s_my_style1.content[scroll_index+1].state = GUIITEM_CONTENT_STATE_TEXT_R_ALIGN;

                    s_my_style1.main_index = scroll_index+2;
// 					if (PNULL != time_string->wstr_ptr)
// 					{
// 						SCI_FREE(time_string->wstr_ptr);
// 						time_string->wstr_ptr = PNULL;
// 					}
// 					if (PNULL != time_string)/*lint !e774*/
// 					{
// 						SCI_FREE(time_string);
// 						time_string = PNULL;
// 					}
// 				}
// 				else
// 				{
// 					if (PNULL != time_string)/*lint !e774*/
// 					{
// 						SCI_FREE(time_string);
// 						time_string = PNULL;
// 					}
// 				}
// 			}	
        }
#endif
        for( i = 0; i < s_user_list.total; i++ )
        {
            GUILIST_AppendItem(ctrl_id, &item_t); 
#ifdef MMI_GUI_STYLE_MINISCREEN
            GUILIST_SetItemStyleEx(ctrl_id, i, &s_my_style1);
#endif			
        }
    }
    return;
}
/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  liqing.peng
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendAllUserListBoxItemData(
                                 MMI_CTRL_ID_T	ctrl_id,
								 BOOLEAN is_have_event,
								 uint16 index
                                 )
{
	GUILIST_ITEM_T			item_t= {0}; /*lint !e64*/
	GUILIST_ITEM_DATA_T     item_data= {0};/*lint !e64*/
    uint8 tmp_item[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_1[GUILIST_STRING_MAX_NUM] = {0};
    wchar tmp_wstr_2[GUILIST_STRING_MAX_NUM] = {0};
    MMIACC_SMART_EVENT_T    smart_event = {0};
    
    MMIAPIALM_FormInteEventInfo(s_user_list.entry_id[index],&smart_event);

// #if defined MAINLCD_SIZE_128X160 && (defined MAINLCD_LOGIC_ANGLE_0 || defined MAINLCD_LOGIC_ANGLE_180)
// 	item_t.item_style = GUIITEM_STYLE_CUSTOMIZE;
// #else
// 	item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
// #endif

    item_t.item_data_ptr = &item_data;
#ifdef MAINLCD_DEV_SIZE_128X160
    item_data.item_content[0].item_data_type = item_data.item_content[1].item_data_type   //junxue zhu
        = item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
#else
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = smart_event.event_content_info.image_id;
    item_data.item_content[1].item_data_type = item_data.item_content[2].item_data_type
        = item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
#endif

    MMIAPISET_FormatDateStrByDateStyle(smart_event.event_fast_info.start_year,
        smart_event.event_fast_info.start_mon,smart_event.event_fast_info.start_day,
        '/',tmp_item,GUILIST_STRING_MAX_NUM);
#ifdef MAINLCD_DEV_SIZE_128X160
    item_data.item_content[0].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);  //junxue zhu
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
#else
    item_data.item_content[1].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = tmp_wstr_1;
#endif
#ifdef MAINLCD_DEV_SIZE_128X160
    MMI_STRNTOWSTR( tmp_wstr_1,
        item_data.item_content[0].item_data.text_buffer.wstr_len,   //junxue zhu
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
#else
   MMI_STRNTOWSTR( tmp_wstr_1,
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
#endif

	//time
    SCI_MEMSET(tmp_item,0,strlen((char*)tmp_item));/*lint !e666*/
	MMIAPISET_FormatTimeStrByTime(
							 smart_event.event_fast_info.hour,
							 smart_event.event_fast_info.minute,
							 tmp_item,
							 GUILIST_STRING_MAX_NUM
							 );
#ifdef MAINLCD_DEV_SIZE_128X160
    item_data.item_content[1].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);  //2

    item_data.item_content[1].item_data.text_buffer.wstr_ptr = tmp_wstr_2;   //2

    MMI_STRNTOWSTR( tmp_wstr_2,
        item_data.item_content[1].item_data.text_buffer.wstr_len,   //2
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    if (0!= smart_event.event_content_info.w_length)
    {
        item_data.item_content[2].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;  //3

        item_data.item_content[2].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;   //3
    }
    else
    {
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_ID;  //3
        item_data.item_content[2].item_data.text_id = TXT_BLANK;   //3
    }

#else
    item_data.item_content[2].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)tmp_item);

    item_data.item_content[2].item_data.text_buffer.wstr_ptr = tmp_wstr_2;

    MMI_STRNTOWSTR( tmp_wstr_2,
        item_data.item_content[2].item_data.text_buffer.wstr_len,
        tmp_item,
        strlen((char*)tmp_item),
        strlen((char*)tmp_item));
    if (0!= smart_event.event_content_info.w_length)
    {
        item_data.item_content[3].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;

        item_data.item_content[3].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
    }
    else
    {
        item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[3].item_data.text_id = TXT_BLANK;
    }
#endif
    item_data.softkey_id[0] = STXT_OPTION;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#else
    item_data.softkey_id[1] = TXT_NULL;
#endif
    item_data.softkey_id[2] = STXT_RETURN;
	GUILIST_SetItemData( ctrl_id, &item_data, index );
}
    
/*****************************************************************************/
// 	Description : MapListItemToUserItem
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL uint16 MapListItemToUserItem(
                                   uint16 index
                                   )
{
    uint16 i = 0;
    
    //SCI_PASSERT(MMIACC_SCH_NUM>index,("MapListItemToUserItem: index is %d",index)); /*assert verified*/  

    if(MMIACC_SCH_NUM <= index)
	{
	    return i;
    }
    
    i = s_user_list.entry_id[index];
    
    return i;
}

/*****************************************************************************/
// 	Description : set schedule date title
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL void SchSetDateTitle(
                           uint16           year,
                           uint8            month,
                           uint8            day,
                           MMI_STRING_T     *string_ptr)
{
    uint8						datestr[MMISET_DATE_STR_LEN] = {0};
    uint16						len = {0}; 

    MMIAPISET_FormatDateStrByDateStyle(year,month,day,'-',datestr,MMISET_DATE_STR_LEN);

    len = strlen((char *)datestr);

    //SCI_ASSERT(len < string_ptr->wstr_len);/*assert verified*/
    if(len >= string_ptr->wstr_len)
	{
	    return;
    }
    
    MMI_STRNTOWSTR(string_ptr->wstr_ptr, string_ptr->wstr_len,datestr, len,len);
    string_ptr->wstr_len = len;

    return;
}


/*****************************************************************************/
// 	Description : view the list of current day's schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchViewListWinMsg( 
                                           MMI_WIN_ID_T		win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           )
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    uint16                      i = 0;
    uint16                      index = 0;
    MMI_CTRL_ID_T               current_ctrl_id = MMISCH_VIEW_LIST_CTRL_ID;
    MMISCH_LIST_WIN_PARAM_T     *win_param = NULL;
    MMI_STRING_T                title = {0};
    wchar                       title_str[30] = {0};
    static MMISCH_DETAIL_WIN_PARAM_T    *detail_win_param = NULL;
    static MMISET_DATE_T data = {0};
    MMI_CALENDAR_T                 *calendar_ctrl_ptr  = &g_calendar_show;
    uint16 entry_id = 0;
    const GUILIST_ITEM_T* item_ptr = PNULL; 
    BOOLEAN is_list_has_item = TRUE;

    SCI_MEMSET(title_str, 0,sizeof(title_str));
    title.wstr_ptr = title_str;
    title.wstr_len = sizeof(title_str);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        win_param = (MMISCH_LIST_WIN_PARAM_T*)MMK_GetWinAddDataPtr(win_id);
        //SCI_ASSERT(NULL != win_param);/*assert verified*/
        if(NULL == win_param)
        {
            break;
        }
        data.date_day = win_param->day;
        data.date_month = win_param->month;
        data.date_year = win_param->year;       
        {
            is_list_has_item = MMIAPISCH_OpenEventList(FALSE, data.date_year, data.date_month, data.date_day);
            AppendUserListBoxItem(current_ctrl_id,is_list_has_item); 
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);      
#ifdef MMI_GUI_STYLE_MINISCREEN
            SetTitleIndexType(current_ctrl_id,is_list_has_item);
#endif
            SchSetDateTitle(data.date_year, data.date_month, data.date_day, &title);
            GUIWIN_SetTitleText(win_id, title_str, title.wstr_len, FALSE);
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);	
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);	
#endif
#endif
        }
#ifdef MMI_PDA_SUPPORT
         GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#else
        GUIWIN_SetTitleRollArrow(win_id,FALSE);
#endif
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            //SCI_ASSERT( PNULL != need_item_data_ptr );/*assert verified*/
            if(PNULL == need_item_data_ptr)
            {
                break;
            }
            item_ptr = GUILIST_GetItemPtrByIndex(current_ctrl_id,need_item_data_ptr->item_index);
            //SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
            if(PNULL == item_ptr)
            {
                break;
            }
            entry_id = item_ptr->user_data;

            AppendUserListBoxItemData(current_ctrl_id,MMIAPISCH_OpenEventList(FALSE,data.date_year, data.date_month, data.date_day),entry_id,need_item_data_ptr->item_index);
        }
        break;  

    case MSG_GET_FOCUS:
#ifdef MMI_VCALENDAR_SUPPORT
        if(!GUILIST_GetListStateById(current_ctrl_id,GUILIST_STATE_ENABLE_MARK))
#endif
        {
            MMK_PostMsg(win_id, MSG_SCH_LIST_UPDATE, PNULL, 0);
        }
        break;

    case MSG_APP_LEFT:
    case MSG_CTL_TITLE_MOVE_PREV:    	
        MovePrevDay(data, &data);
        MMK_PostMsg(win_id, MSG_SCH_LIST_UPDATE_DATE, PNULL, 0);		
        break;
    
    case  MSG_APP_RIGHT:  //移到下一天    
    case MSG_CTL_TITLE_MOVE_NEXT:    
        MoveNextDay(data, &data);
        MMK_PostMsg(win_id, MSG_SCH_LIST_UPDATE_DATE, PNULL, 0);	
        break;

    case MSG_SCH_LIST_UPDATE_DATE:
        calendar_ctrl_ptr->solar_year = data.date_year;
        calendar_ctrl_ptr->solar_month = data.date_month;
        calendar_ctrl_ptr->solar_date= data.date_day;		
        GUILIST_RemoveAllItems( current_ctrl_id);
        is_list_has_item = MMIAPISCH_OpenEventList(FALSE, data.date_year, data.date_month, data.date_day);
        AppendUserListBoxItem(current_ctrl_id,is_list_has_item);  
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        SchSetDateTitle(data.date_year, data.date_month, data.date_day, &title);
        GUIWIN_SetTitleText(win_id, title_str, title.wstr_len, FALSE);
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);	
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
#endif
#endif
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);		
        break;

    case MSG_SCH_LIST_UPDATE:
        i = GUILIST_GetCurItemIndex(current_ctrl_id);	       
        GUILIST_RemoveAllItems( current_ctrl_id);
        {
            is_list_has_item = MMIAPISCH_OpenEventList(FALSE, data.date_year, data.date_month, data.date_day);
            AppendUserListBoxItem(current_ctrl_id,is_list_has_item);
            GUILIST_SetCurItemIndex(current_ctrl_id,i); 
        }
#ifdef MMI_GUI_STYLE_MINISCREEN
        SetTitleIndexType(current_ctrl_id,is_list_has_item);
#endif
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        MMK_CloseWin(win_id);
        break;
#ifdef MMI_PDA_SUPPORT    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id,MSG_APP_RIGHT ,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_LEFT,PNULL);
                break; 

             case MMISCH_VIEW_LIST_CTRL_ID:
                MMK_SendMsg(win_id, MSG_CTL_MIDSK,PNULL);
                break;
            default:
                break;
            }

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 

    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_WEB:
        if(0==GUILIST_GetTotalItemNum(current_ctrl_id))
        {
            break; 
        }
        item_ptr = GUILIST_GetItemPtrByIndex(current_ctrl_id,GUILIST_GetCurItemIndex(current_ctrl_id));
        //SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
        if(PNULL == item_ptr)
        {
            break;
        }
        
        index = item_ptr->user_data;
        if(!MMIAPISCH_OpenEventList(FALSE,data.date_year, data.date_month, data.date_day))
        {
            break;
        }
        if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
        }
        detail_win_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
        if(detail_win_param != PNULL)
        {
            SCI_MEMSET(detail_win_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            
            detail_win_param->win_id = win_id;
            detail_win_param->index = index;

            MMIAPIALM_FormInteEventInfo(index,&detail_win_param->event);
            if(!MMIAPIALM_GetRingInfo(index,&detail_win_param->event))
            {
                detail_win_param->event.event_content_info.ring_type = ALM_FIX_RING;
                detail_win_param->event.event_content_info.ring = 1;
            }
            MMK_CreateWin((uint32 *)SCH_VIEW_DETAIL_WIN_TAB, (ADD_DATA)detail_win_param);
            MMIAPISCH_DeleteUnreadEvent(index);
        }
        break;
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if(0<GUILIST_GetTotalItemNum(current_ctrl_id))
        {
            item_ptr = GUILIST_GetItemPtrByIndex(current_ctrl_id,GUILIST_GetCurItemIndex(current_ctrl_id));
            //SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
            if(PNULL != item_ptr)
            {
                index = item_ptr->user_data;
            }
        }

        if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
        }
        detail_win_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
        if(detail_win_param != PNULL)
        {
            SCI_MEMSET(detail_win_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            detail_win_param->win_id = win_id;
            detail_win_param->index = index;
            if(0<GUILIST_GetTotalItemNum(current_ctrl_id))
            {
                MMIAPIALM_FormInteEventInfo(index,&detail_win_param->event);
                if(!MMIAPIALM_GetRingInfo(index,&detail_win_param->event))
                {
                    detail_win_param->event.event_content_info.ring_type = ALM_FIX_RING;
                    detail_win_param->event.event_content_info.ring = 1;
                }
            }
#ifdef MMI_PDA_SUPPORT
            if( 0 ==GUILIST_GetSelectedItemIndex(current_ctrl_id,PNULL,0))
            {
                MMK_CreateWin((uint32 *)SCH_LIST_OPTION_WIN_TAB, (ADD_DATA)detail_win_param);
            }
            else
            {
                MMK_CreateWin((uint32 *)SCH_LIST_MARK_OPTION_WIN_TAB, (ADD_DATA)detail_win_param);
            }
#else
            MMK_CreateWin((uint32 *)SCH_LIST_OPTION_WIN_TAB, (ADD_DATA)detail_win_param);
#endif
        }
        break;
    case MSG_SCH_LIST_DEL_ONE:
        {
            MMI_WIN_ID_T    query_win_id = MMISCH_DELETE_ANIM_WIN_ID;
            MMIPUB_CloseQuerytWin(&query_win_id);
            item_ptr = GUILIST_GetItemPtrByIndex(current_ctrl_id,GUILIST_GetCurItemIndex(current_ctrl_id));
            //SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
            if(PNULL != item_ptr)
            {
                index = item_ptr->user_data;
            }
            if (MMIALM_DelOneEvent(index))
            {
                GUILIST_RemoveItem(current_ctrl_id, GUILIST_GetCurItemIndex(current_ctrl_id));
            
                // 当控件中的item为空时，不需要返回list窗口
                if(GUILIST_GetTotalItemNum(current_ctrl_id) == 0)
                {
                    MMK_CloseWin(win_id);
                }
//#ifndef  MMI_PDA_SUPPORT
//                MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
//#endif
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        if ( PNULL != s_list_param)
        {
            SCI_FREE(s_list_param);
            s_list_param = PNULL;
        }
        if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
        }
        MMK_CloseWin(MMICALENDAR_OPTION_WIN_ID);
        if (!MMK_IsOpenWin(MMICALENDAR_WIN_ID))//从 idle入口的情况
        {
            MMIAPICALENDAR_FreeRes();
        }
#ifdef MMI_VCALENDAR_SUPPORT    
        s_vcalendar_is_processing = FALSE;
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
#endif
        break;
#ifdef MMI_VCALENDAR_SUPPORT
    case MSG_VCALENDAR_SAVE_SCH_CNF:
        HandleSaveSchCNF(current_ctrl_id,param);
        break;
    case MSG_SCH_DELETE_CNF:
        HandleDeleteSchCNF(current_ctrl_id,param);
        break;
#endif 
    case MSG_CTL_LIST_MARK_ITEM:
        if (0 == GUILIST_GetSelectedItemIndex(current_ctrl_id,PNULL,0))
        {
            GUILIST_SetTextListMarkable(current_ctrl_id,FALSE);
        }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//    Description : SetTitleIndexType
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void SetTitleIndexType( MMI_CTRL_ID_T	ctrl_id, BOOLEAN is_list_has_item )
{
    if (is_list_has_item) 
    {
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
    }         
    else
    {
        GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_NONE );
    }   
}

/*****************************************************************************/
//    Description : move one day backward
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void MovePrevDay(
                         MMISET_DATE_T  date_in,
                         MMISET_DATE_T  *date_out_ptr
                       )
{
    uint16  year     = date_in.date_year;
    uint8   month    = date_in.date_month;
    uint8   date     = date_in.date_day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }

    if (1 != date)   //不是第一天，移到前一天
    {
	    date--;
    }
    else if (1 != month) //第一天且不是一月，移到上月最后一天
    {
        month--;
        date = MMICALENDAR_GetMonthday(year, month);
    }
    else if (QUERY_YEAR_BOTTOM < year)   //一月一日且年没超出范围，移到上年最后一天
    {
        year--;
        month = SCH_DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month);
    }
    else if (QUERY_YEAR_BOTTOM == year)
    {
        year = QUERY_YEAR_TOP;
        month = SCH_DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month);
    }

    date_out_ptr->date_year = year;
    date_out_ptr->date_month = month;
    date_out_ptr->date_day = date;

    return;
}


/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author: zhaohui
//    Note:
/*****************************************************************************/
LOCAL void MoveNextDay(
                         MMISET_DATE_T  date_in,
                         MMISET_DATE_T  *date_out_ptr
                       )
{
    uint16      year   = date_in.date_year;
    uint8       month   = date_in.date_month;
    uint8       date    = date_in.date_day;

    //SCI_ASSERT(NULL != date_out_ptr);/*assert verified*/
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (MMICALENDAR_GetMonthday(year, month) != date)   //不是最后一天, 移到后一天
    {
	    date++;
    }
    else if (SCH_DECEMBER !=month) //最后一天且不是十二月，移到下月第一天
    {
        month++;
        date = 1;
    }
    else if (QUERY_YEAR_TOP > year)   //十二月最后一天且年不超出范围，移到下年第一天
    {
        year++;
        month = 1;
        date = 1;
    }
    else if (QUERY_YEAR_TOP == year)
    {
        year = QUERY_YEAR_BOTTOM;
        month = 1;
        date = 1;
    }

    date_out_ptr->date_year = year;
    date_out_ptr->date_month = month;
    date_out_ptr->date_day = date;

    return;
}

/*****************************************************************************/
// 	Description : view the list of all schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchViewAllListWinMsg( 
                                              MMI_WIN_ID_T		win_id, 
                                              MMI_MESSAGE_ID_E	msg_id, 
                                              DPARAM				param
                                              )
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    uint16                      i = 0; 
    uint16                      index = 0;
    MMI_CTRL_ID_T               current_ctrl_id = MMISCH_VIEW_ALL_LIST_CTRL_ID;
    //MMISCH_LIST_WIN_PARAM_T     *win_param = NULL;
    static MMISCH_DETAIL_WIN_PARAM_T    *detail_win_param = NULL;
	//static MMISET_DATE_T data = {0};
    BOOLEAN is_list_has_item = TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		//win_param = (MMISCH_LIST_WIN_PARAM_T*)MMK_GetWinAddDataPtr(win_id);
		//SCI_ASSERT(NULL != win_param);/*assert verified*/
        //if(PNULL == win_param)
        //{
        //    break;
        //}
		//data.date_day = win_param->day;
		//data.date_month = win_param->month;
		//data.date_year = win_param->year;
		{
            is_list_has_item = MMIAPISCH_OpenEventList(TRUE,0, 0, 0);
			AppendAllUserListBoxItem(current_ctrl_id,is_list_has_item,FALSE);
			SetTitleIndexType(current_ctrl_id,is_list_has_item);
			MMK_SetAtvCtrl( win_id,  current_ctrl_id);
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);	
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
#endif
		}
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;
	case MSG_CTL_LIST_NEED_ITEM_DATA:
		{
			uint16 list_index = 0;
			GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

			if( PNULL != need_item_data_ptr )
			{
				list_index = need_item_data_ptr->item_index;
				AppendAllUserListBoxItemData(current_ctrl_id,MMIAPISCH_OpenEventList(TRUE,0, 0, 0),list_index);
			}
		}
        break;

    case MSG_GET_FOCUS:
#ifdef MMI_VCALENDAR_SUPPORT
        if(!GUILIST_GetListStateById(current_ctrl_id,GUILIST_STATE_ENABLE_MARK))
#endif
        {
            MMK_PostMsg(win_id, MSG_SCH_LIST_UPDATE, PNULL, 0);
        }
        break;
        
    case MSG_SCH_LIST_UPDATE:
	    i = GUILIST_GetCurItemIndex(current_ctrl_id);       
        GUILIST_RemoveAllItems( current_ctrl_id);
        {
            is_list_has_item = MMIAPISCH_OpenEventList(TRUE,0, 0, 0);
            AppendAllUserListBoxItem(current_ctrl_id,is_list_has_item,FALSE);
            SetTitleIndexType(current_ctrl_id,is_list_has_item);
 			GUILIST_SetCurItemIndex(current_ctrl_id,i);  
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
		if(0==GUILIST_GetTotalItemNum(current_ctrl_id))
		{
		   break; 
		}
		i = GUILIST_GetCurItemIndex(current_ctrl_id);
		index = MapListItemToUserItem(i);
		if(!MMIAPISCH_OpenEventList(TRUE,0, 0, 0))
		{
		    break;
		}
		if(NULL != detail_win_param)
		{
		    SCI_FREE(detail_win_param);
		}
		detail_win_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
		if(detail_win_param != PNULL)
		{
		SCI_MEMSET(detail_win_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));

		detail_win_param->win_id = win_id;
		detail_win_param->index = index;

		MMIAPIALM_FormInteEventInfo(index,&detail_win_param->event);
		//MMIAPIALM_GetRingInfo(index,&detail_win_param->event);
		if(!MMIAPIALM_GetRingInfo(index,&detail_win_param->event))
		{
		 	detail_win_param->event.event_content_info.ring_type = ALM_FIX_RING;
			detail_win_param->event.event_content_info.ring = 1;
		}
		MMK_CreateWin((uint32 *)SCH_VIEW_DETAIL_WIN_TAB, (ADD_DATA)detail_win_param);
		MMIAPISCH_DeleteUnreadEvent(index);
		}
		break;
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_APP_MENU:
        if(0<GUILIST_GetTotalItemNum(current_ctrl_id))
        {
            i = GUILIST_GetCurItemIndex(current_ctrl_id);
            index = MapListItemToUserItem(i);
        }
        if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
        }
        detail_win_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
        if(detail_win_param != PNULL)
        {
	        SCI_MEMSET(detail_win_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
	        
	        detail_win_param->win_id = win_id;
	        detail_win_param->index = index;
            if(0<GUILIST_GetTotalItemNum(current_ctrl_id))
            {
    	        MMIAPIALM_FormInteEventInfo(index,&detail_win_param->event);
    			//MMIAPIALM_GetRingInfo(index,&detail_win_param->event);
    			if(!MMIAPIALM_GetRingInfo(index,&detail_win_param->event))
    			{
    	         	detail_win_param->event.event_content_info.ring_type = ALM_FIX_RING;
    				detail_win_param->event.event_content_info.ring = 1;
    	        }
            }
#ifdef MMI_PDA_SUPPORT
            if( 0 ==GUILIST_GetSelectedItemIndex(current_ctrl_id,PNULL,0))
            {
                MMK_CreateWin((uint32 *)SCH_ALL_LIST_OPTION_WIN_TAB, (ADD_DATA)detail_win_param);
            }
            else
            {
                MMK_CreateWin((uint32 *)SCH_LIST_MARK_OPTION_WIN_TAB, (ADD_DATA)detail_win_param);
            }
#else
            MMK_CreateWin((uint32 *)SCH_ALL_LIST_OPTION_WIN_TAB, (ADD_DATA)detail_win_param);
#endif
            
        }
        break;
    case MSG_SCH_LIST_DEL_ONE:
        {
            MMI_WIN_ID_T    query_win_id = MMISCH_DELETE_ANIM_WIN_ID;
            MMIPUB_CloseQuerytWin(&query_win_id);
            i = GUILIST_GetCurItemIndex(current_ctrl_id);
            index = MapListItemToUserItem(i);
            if (MMIALM_DelOneEvent((uint16)(index)))
            {
                GUILIST_RemoveItem(current_ctrl_id, GUILIST_GetCurItemIndex(current_ctrl_id));
            
                // 当控件中的item为空时，不需要返回list窗口
                if(GUILIST_GetTotalItemNum(current_ctrl_id) == 0)
                {
                    MMK_CloseWin(win_id);
                }
//#ifndef  MMI_PDA_SUPPORT
//                MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
//#endif
            }
            else
            {         
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }	
        }
        break;

    case MSG_CLOSE_WINDOW:
        if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
        }
        MMK_CloseWin(MMICALENDAR_OPTION_WIN_ID);
        MMK_CloseWin(MMISCH_ALL_LIST_OPTION_WIN_ID);
#ifdef MMI_VCALENDAR_SUPPORT
        s_vcalendar_is_processing = FALSE;
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
#endif 
        break;
#ifdef MMI_VCALENDAR_SUPPORT
    case MSG_VCALENDAR_SAVE_SCH_CNF:
        HandleSaveSchCNF(current_ctrl_id,param);
        break;
    case MSG_SCH_DELETE_CNF:
        HandleDeleteSchCNF(current_ctrl_id,param);
        break;
#endif
    case MSG_CTL_LIST_MARK_ITEM:
        if (0 == GUILIST_GetSelectedItemIndex(current_ctrl_id,PNULL,0))
        {
            GUILIST_SetTextListMarkable(current_ctrl_id,FALSE);
        }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : HandleListDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleListDelQueryWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_WIN_ID_T                query_win_id = MMISCH_DELETE_ANIM_WIN_ID;
    uint32                      time_out = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&query_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            MMK_PostMsg(MMISCH_VIEW_WIN_ID, MSG_SCH_LIST_DEL_ONE, PNULL, 0);

            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
    return result;

}

/*****************************************************************************/
// 	Description : HandleListDelAllQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleListDelAllQueryWin(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
//     MMI_WIN_ID_T                query_win_id = MMISCH_DELETE_ANIM_WIN_ID;
//     uint32                      time_out = 0;

    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
						MMIALM_OpenDeleteWaittingWin(MSG_SCH_LIST_DEL_CURDATE);
			 			if (MMIAPICALENDAR_GetCurDayHasPeriodSch())
            {
                MMIAPICALENDAR_SetIfDelPeriodSch(TRUE);
            }
            MMK_CloseWin(win_id);
            break;		

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
	      				if(MMIAPICALENDAR_IsCurDayHasAppointSch(CALENDAR_OPT_SCH_NOT_PERIOD) && MMIAPICALENDAR_GetCurDayHasPeriodSch())
                {
                   	MMIALM_OpenDeleteWaittingWin(MSG_SCH_LIST_DEL_CURDATE);
                    MMIAPICALENDAR_SetIfDelPeriodSch(FALSE);
                }
                MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
    return result;
}


/*****************************************************************************/
// 	Description : HandleAllListDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllListDelQueryWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_WIN_ID_T                query_win_id = MMISCH_DELETE_ANIM_WIN_ID;
    uint32                      time_out = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&query_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            MMK_PostMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_SCH_LIST_DEL_ONE, PNULL, 0);
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
     return result;

}

/*****************************************************************************/
// 	Description : HandleAllListDelAllQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllListDelAllQueryWin(
                                        MMI_WIN_ID_T win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
//     MMI_WIN_ID_T                query_win_id = MMISCH_DELETE_ANIM_WIN_ID;
//     uint32                      time_out = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
			MMIALM_OpenDeleteWaittingWin(MSG_SCH_LIST_DEL_ALL);
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
     return result;
}

/*****************************************************************************/
// 	Description : enablel menu item
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
LOCAL void EnableListOptionMenuItem(
									 BOOLEAN					is_all,
									 uint16                   year,
									 uint8                    month,
									 uint8                    day
									 )
{
#ifdef MMI_VCALENDAR_SUPPORT
    uint16 selected_num=0;
    uint16 cur_selection=0;
    const GUILIST_ITEM_T *item_ptr=PNULL;
#endif
	if (is_all)
	{
		if(!MMIAPISCH_OpenEventList(TRUE, year, month, day))
		{
#ifndef MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_ALL_LIST_VIEW, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_ALL_LIST_EDIT, TRUE);
#endif
#ifndef  MMI_VCALENDAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_ALL_LIST_DEL, TRUE);
#endif
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_ALL_LIST_DEL_ALL, TRUE);
#endif
#ifdef MMI_VCALENDAR_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_MARK, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_SEND, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_DEL_MARK, TRUE);
#endif
        }
#ifdef MMI_VCALENDAR_SUPPORT
        if(GUILIST_GetListStateById(MMISCH_VIEW_ALL_LIST_CTRL_ID,GUILIST_STATE_ENABLE_MARK))
        {
            selected_num=GUILIST_GetSelectedItemIndex(MMISCH_VIEW_ALL_LIST_CTRL_ID,PNULL,0);
#ifdef MMI_PDA_SUPPORT
            if(selected_num==0)
            {
                GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
                GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_SEND, TRUE);
                GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_DEL_MARK, TRUE);
            }
#endif
            if(selected_num>1)
            {
                GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_SEND, TRUE);
            }

            cur_selection=GUILIST_GetCurItemIndex(MMISCH_VIEW_ALL_LIST_CTRL_ID);
            item_ptr = GUILIST_GetItemPtrByIndex(MMISCH_VIEW_ALL_LIST_CTRL_ID,cur_selection);
            if(GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
            {
               GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_MARK, TRUE); 
               GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_CANCEL_MARK, FALSE); 
            }
            else
            {
               GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_MARK, FALSE); 
               GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_CANCEL_MARK, TRUE);  
            }
            
            if(selected_num==s_user_list.total)
            {
               GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_MARK_ALL, TRUE); 
            }
            else
            {
               GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_CANCEL_MARK_ALL, TRUE); 
            }
        }
        else
        {
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_CANCEL_MARK, TRUE); 
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_CANCEL_MARK_ALL, TRUE);
#ifdef MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_SEND, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_DEL_MARK, TRUE);
#endif
        }
        
        if(!(MMIAPIFMM_GetFirstValidDevice() < MMI_DEVICE_NUM))
        {
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_IMPORT, TRUE);
        }
#endif
	}
	else
	{
        if(!MMIAPISCH_OpenEventList(FALSE, year, month, day))
		{
#ifndef MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_VIEW, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_EDIT, TRUE);
#endif
#ifndef  MMI_VCALENDAR_SUPPORT
#ifndef MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_DEL, TRUE);
#endif
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_DEL_ALL, TRUE);
#endif
#ifdef MMI_VCALENDAR_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_MARK, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_SEND, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_DEL_MARK, TRUE);
#endif
        }
#ifdef MMI_VCALENDAR_SUPPORT
        if(GUILIST_GetListStateById(MMISCH_VIEW_LIST_CTRL_ID,GUILIST_STATE_ENABLE_MARK))
        {
            selected_num=GUILIST_GetSelectedItemIndex(MMISCH_VIEW_LIST_CTRL_ID,PNULL,0);
#ifdef MMI_PDA_SUPPORT
            if(selected_num==0)
            {
                GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
                GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_SEND, TRUE);
                GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_DEL_MARK, TRUE);
            }
#endif
            
            if(selected_num>1)
            {
                GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_SEND, TRUE);
            }
            
            cur_selection=GUILIST_GetCurItemIndex(MMISCH_VIEW_LIST_CTRL_ID);
            item_ptr = GUILIST_GetItemPtrByIndex(MMISCH_VIEW_LIST_CTRL_ID,cur_selection);
            if(GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
            {
               GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_MARK, TRUE); 
               GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_CANCEL_MARK, FALSE); 
            }
            else
            {
               GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_MARK, FALSE); 
               GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,ID_SCH_LIST_SUB_CANCEL_MARK, TRUE);  
            }

            if(selected_num==s_user_list.total)
            {
               GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_MARK_ALL, TRUE); 
            }
            else 
            {
               GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_CANCEL_MARK_ALL, TRUE); 
            }
        }
        else
        {
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_CANCEL_MARK, TRUE); 
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_MARK,    ID_SCH_LIST_SUB_CANCEL_MARK_ALL, TRUE); 
#ifdef MMI_PDA_SUPPORT
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_SEND, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_OPT, ID_SCH_LIST_DEL_MARK, TRUE);
#endif

        }

        if(!(MMIAPIFMM_GetFirstValidDevice() < MMI_DEVICE_NUM))
        {
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_EXPORT, TRUE);
            GUIMENU_SetItemGrayed(MMISCH_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_VCALENDAR, ID_SCH_VCALENDAR_IMPORT, TRUE);
        }
#endif 
	}
    
}

/*****************************************************************************/
// 	Description : schedule  list option win
//	Global resource dependence : 
//  Author: xiangjie
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchListOptionWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E                        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id = MMISCH_LIST_OPTION_MENU_CTRL_ID;
    static MMISCH_DETAIL_WIN_PARAM_T    *win_param = NULL;
    MMI_MENU_GROUP_ID_T	                group_id = 0; 
    MMI_MENU_ID_T		                menu_id  = 0;
    static MMISCH_DETAIL_WIN_PARAM_T 	*win_new_param = PNULL;
    MMI_CALENDAR_T                 *calendar_ctrl_ptr = &g_calendar_show;
    static MMISET_DATE_T   *detail_win_param = PNULL;
    MMISET_LANGUAGE_TYPE_E lang_type=MMISET_LANGUAGE_ENGLISH;
    static uint32 *to_date_param=PNULL;
#ifdef MMI_VCALENDAR_SUPPORT
    MMI_CTRL_ID_T list_ctrl_id=MMISCH_VIEW_LIST_CTRL_ID;
    uint16        cur_selection=0;
    //GUILIST_ITEM_T* item_ptr=PNULL;
    uint16 event_id=0;
    uint16 event_index[MMIACC_SCH_NUM]={0};
    uint16 event_num=0;
    int i=0;
    uint32 user_data=0;
    uint16 _event_id[MMIACC_SCH_NUM]={0};
#endif

    win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != win_param);/*assert verified*/
    if(PNULL == win_param)
    {
        return recode;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
    //CR163242
        {
            MMI_STRING_T kstring = {0};
            MMI_GetLabelTextByLang(STXT_MARK, &kstring);
            GUIMENU_SetMenuTitle(&kstring,MMISCH_LIST_OPTION_MENU_CTRL_ID);
        }  
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);	
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#endif
        MMK_SetAtvCtrl( win_id,  ctrl_id);
        EnableListOptionMenuItem(
            FALSE,
            calendar_ctrl_ptr->solar_year,
            calendar_ctrl_ptr->solar_month,
            calendar_ctrl_ptr->solar_date
            );
        MMIAPISET_GetLanguageType(&lang_type);
        if(MMISET_LANGUAGE_SIMP_CHINESE!=lang_type&&MMISET_LANGUAGE_TRAD_CHINESE!=lang_type)
        {
            GUIMENU_SetItemVisible(ctrl_id,MENU_SCH_OPT,ID_SCH_LIST_LUNAR_DETAIL,FALSE);
        }
        else
        {
            GUIMENU_SetItemVisible(ctrl_id,MENU_SCH_OPT,ID_SCH_LIST_LUNAR_DETAIL,TRUE);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch( menu_id)
        {
        case ID_SCH_LIST_VIEW:	
            MMK_CreateWin((uint32 *)SCH_VIEW_DETAIL_WIN_TAB, (ADD_DATA)win_param);
            MMIAPISCH_DeleteUnreadEvent(win_param->index);
#ifdef MMI_VCALENDAR_SUPPORT
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
#endif
            break;
            
        case ID_SCH_LIST_NEW:
#ifdef MMI_VCALENDAR_SUPPORT
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
#endif
            if (PNULL != win_new_param)
            {
                SCI_FREE(win_new_param);
            }
            win_new_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            if(win_new_param != PNULL)
            {
                SCI_MEMSET(win_new_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
                win_new_param->win_id = win_id;
                MMIAPISCH_InitNewSch(calendar_ctrl_ptr->solar_year,
                    calendar_ctrl_ptr->solar_month,
                    calendar_ctrl_ptr->solar_date,
                    &(win_new_param->event));	
                MMK_CreateWin((uint32 *)SCH_EDIT_WIN_TAB, (ADD_DATA)win_new_param);	
            }
            break;
            
        case ID_SCH_LIST_EDIT:	
            win_param->win_id = win_id;
            MMK_CreateWin((uint32 *)SCH_EDIT_WIN_TAB, (ADD_DATA)win_param);
            MMIAPISCH_DeleteUnreadEvent(win_param->index);
#ifdef MMI_VCALENDAR_SUPPORT
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
#endif
            break;
#ifndef MMI_VCALENDAR_SUPPORT        
        case ID_SCH_LIST_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleListDelQueryWin);
            MMK_CloseWin(win_id);
            break;
            
        case ID_SCH_LIST_DEL_ALL:
            MMIAPICALENDAR_SetCurDayHasPeriodSch(MMIAPICALENDAR_IsCurDayHasAppointSch(CALENDAR_OPT_SCH_PERIOD));
            if ( MMIAPICALENDAR_GetCurDayHasPeriodSch() )
            {
                MMIPUB_OpenQueryWinByTextId(TXT_CALENDAR_DEL_CDR_DAY_PERIODSCH,IMAGE_PUBWIN_QUERY,PNULL,HandleListDelAllQueryWin);//当日日程有重复项,是否也删除?
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,HandleListDelAllQueryWin);
            }
            MMK_CloseWin(win_id);
            break;
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
        case ID_SCH_LIST_LUNAR_DETAIL:
            if(NULL != detail_win_param)
            {
                SCI_FREE(detail_win_param);
                detail_win_param=PNULL;
            }
            detail_win_param = SCI_ALLOC_APP(sizeof(MMISET_DATE_T));
            if(detail_win_param != PNULL)
            {
                SCI_MEMSET(detail_win_param, 0, sizeof(MMISET_DATE_T));
                
                detail_win_param->date_year= calendar_ctrl_ptr->solar_year;
                detail_win_param->date_month= calendar_ctrl_ptr->solar_month;
                detail_win_param->date_day= calendar_ctrl_ptr->solar_date;
                MMIAPICALENDAR_OpenLunarDetailWin((ADD_DATA)detail_win_param);
            }
            break;
#endif
        case ID_SCH_TO_DATE:
            if(PNULL!=to_date_param)
            {
                SCI_FREE(to_date_param);
                to_date_param=PNULL;
            }
            to_date_param=SCI_ALLOC_APP(sizeof(uint32));
            *to_date_param=CALENDAR_TO_DATE_SCH_LIST;
            MMIAPICALEND_OpenCalendarQueryByDate((ADD_DATA)to_date_param);
            break;
#ifdef MMI_VCALENDAR_SUPPORT
        case ID_SCH_LIST_SEND_SMS:
            event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
            if(1==event_num)
            {
                cur_selection=event_index[0];
            }
            else
            {
                cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            }
            //item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_selection);
            //event_id = item_ptr->user_data;
            GUILIST_GetItemData(list_ctrl_id,cur_selection,&user_data);
            event_id=user_data;
            SendVcalendarBySMS(event_id);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_MMS:
            event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
            if(1==event_num)
            {
                cur_selection=event_index[0];
            }
            else
            {
                cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            }
            //item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_selection);
            //event_id = item_ptr->user_data;
            GUILIST_GetItemData(list_ctrl_id,cur_selection,&user_data);
            event_id=user_data;
            SendVcalendarByMMS(event_id);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_BT:		
            if(!GUILIST_GetListStateById(list_ctrl_id,GUILIST_STATE_ENABLE_MARK))
            {
                event_num = 1;
                cur_selection = GUILIST_GetCurItemIndex(list_ctrl_id);
                GUILIST_GetItemData(list_ctrl_id,cur_selection,&user_data);
                _event_id[0]=user_data;
            }
            else
            {
                event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);			
                for(i=0;i<event_num;i++)
                {
                    _event_id[i] = MapListItemToUserItem(event_index[i]);
                }
            }
            //GUILIST_GetItemData(list_ctrl_id,cur_selection,&user_data);
            //event_id=user_data;
            SendVcalendarByBT(_event_id,event_num);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_EMAIL:
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SUB_MARK:
            cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id, s_user_list.total);
            GUILIST_SetSelectedItem(list_ctrl_id,cur_selection,TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_SCH_LIST_SUB_CANCEL_MARK:
            cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            GUILIST_SetSelectedItem(list_ctrl_id,cur_selection,FALSE);
            if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
            {
                GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_SCH_LIST_SUB_MARK_ALL:          
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id, s_user_list.total);
            GUILIST_SetAllSelected(list_ctrl_id,TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_SCH_LIST_SUB_CANCEL_MARK_ALL:
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_VCALENDAR_EXPORT:
        case ID_SCH_LIST_DEL_MARK:
            SCI_MEMSET(&s_operate_sch_info,0,sizeof(MMISCH_OPERATE_RECORDS_INFO_T));
            event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
            if(event_num>0)
            {
                s_operate_sch_info.total_count=event_num;
                for(i=0;i<event_num;i++)
                {
                    //item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,event_index[i]);
                    GUILIST_GetItemData(list_ctrl_id,event_index[i],&user_data);
                    s_operate_sch_info.event_id[i] = user_data;
                }
            }
            else
            {
                s_operate_sch_info.total_count=1;
                cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
                //item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,cur_selection);
                GUILIST_GetItemData(list_ctrl_id,cur_selection,&user_data);
                s_operate_sch_info.event_id[0] = user_data;
            }
            if(ID_SCH_VCALENDAR_EXPORT==menu_id)
            {
                SelectVcalendarExportFolder(list_ctrl_id);
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleDelMarkQueryWin);
                //DeleteSch(&s_operate_sch_info,list_ctrl_id);
            }
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_VCALENDAR_IMPORT:
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
            SchLoadAllVCalendar();
            MMK_CloseWin(win_id);
            break;
#endif     
        default:
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
     
        case MSG_SCH_EVENT_EDIT_FINISH:		
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            switch( menu_id)
            {
            case ID_SCH_LIST_EDIT:
                if (MMIAPIALM_SaveInteEventInfo((uint16)(win_param->index),&(win_param->event)))
                {
//#ifndef  MMI_PDA_SUPPORT
//                    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
                } 
                break;
                
            case ID_SCH_LIST_NEW:			
                if (MMIALM_AddOneEvent(&win_new_param->event,NULL))
                {
//#ifndef  MMI_PDA_SUPPORT
//                    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
                }
                else
                {
                    if(MMIACC_SCH_NUM<=(MMIAPISCH_GetAllSchNumber()))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SCHEDULE_FULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
                    }
                }            
                break;
                
            default:
                break;            
            }
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            if (win_new_param != NULL)
            {
                SCI_FREE(win_new_param);
            }
            if(NULL != detail_win_param)
            {
                SCI_FREE(detail_win_param);
                detail_win_param=PNULL;
            }
            if(PNULL!=to_date_param)
            {
                SCI_FREE(to_date_param);
                to_date_param=PNULL;
            }
            break;
            
        default:
	        recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#if 0
/*****************************************************************************/
// 	Description : HandleSchEditWinWebKeyMsg
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void HandleSchEditWinWebKeyMsg(
                                     MMI_WIN_ID_T	    win_id, 
                                     MMISCH_DETAIL_WIN_PARAM_T   *win_param
                                     //MMIACC_SMART_EVENT_T        *event
                                     )
{
//  	uint32 setlist_index=0;
	MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
	
	switch(ctrl_id)
	{
	case MMISCH_RING_LABEL_CTRL_ID:
		MMK_CreateWin((uint32 *)SCH_EDIT_RING_WIN_TAB, (ADD_DATA)win_param);
		break;
	case MMISCH_EDITWIN_BUTTON0_CTRL_ID:
	case MMISCH_EDITWIN_BUTTON1_CTRL_ID:
	case MMISCH_EDITWIN_BUTTON2_CTRL_ID:
	case MMISCH_EDITWIN_BUTTON3_CTRL_ID:
	case MMISCH_EDITWIN_BUTTON4_CTRL_ID:
	case MMISCH_EDITWIN_BUTTON5_CTRL_ID:
	case MMISCH_EDITWIN_BUTTON6_CTRL_ID:
			if(s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID])
			{
				s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
			}
			else
			{
				s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
			}
#ifndef MMI_PDA_SUPPORT
			setSCHEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
			SCHSetEditWinButtonBg(win_id,ctrl_id);
			break;
	default:
		break;
	}
}
#endif
/*****************************************************************************/
// 	Description : set name param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinNameParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         )
{
	MMI_STRING_T    name_str  = {0};
    //wchar                               forbid_char_value[]= {0x0d, 0x0a};		//NEWMS00214305 [6530][64X64_240X400PDA2]搜狗输入法，点击日历-创建新日程-点击主题，任意输入法，输入多行键码点击回车键无效

	//SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return;
    }
   
    GUIFORM_SetStyle(MMISCH_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	GUILABEL_SetTextById(MMISCH_NAME_TITLE_LABEL_CTRL_ID, TXT_COMMON_SUBJECT,FALSE);
    //GUIEDIT_ConfigDisableChar(MMISCH_NAME_EDIT_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
	name_str.wstr_ptr = event_ptr->event_content_info.w_content;
	name_str.wstr_len = event_ptr->event_content_info.w_length;
#ifdef MMI_PDA_SUPPORT
    GUILABEL_SetText(MMISCH_NAME_EDIT_LABEL_CTRL_ID, &name_str,FALSE);
#else
	GUIEDIT_SetString(MMISCH_NAME_EDIT_CTRL_ID, name_str.wstr_ptr, name_str.wstr_len );
#endif
}

#ifndef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDateParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         )
{	
#ifndef MMI_PDA_SUPPORT
	MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
	GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;
#endif
#ifdef MMI_GUI_STYLE_MINISCREEN
    GUIEDIT_SEPARATOR_E date_show_style=GUIEDIT_SEPARATOR_SOLIDUS;
#endif
	//SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return;
    }
    
    GUIFORM_SetStyle(MMISCH_DATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	
	GUILABEL_SetTextById(MMISCH_DATE_TITLE_LABEL_CTRL_ID, TXT_DATE,FALSE);
#ifdef MMI_PDA_SUPPORT
    {
        MMI_STRING_T date_str= {0};
        uint8      date[MMISET_DATE_STR_LEN]= {0};	
        MMIAPISET_FormatDateStrByDateStyle(
            event_ptr->event_fast_info.year, 
            event_ptr->event_fast_info.mon,
            event_ptr->event_fast_info.day,
            '.',date,MMISET_DATE_STR_LEN);
        date_str.wstr_len = SCI_STRLEN((char*)date);
        date_str.wstr_ptr = (wchar*)SCI_ALLOC_APPZ((date_str.wstr_len +1)*sizeof(wchar));
        MMI_STRNTOWSTR(date_str.wstr_ptr,date_str.wstr_len,date,strlen((char*)date),strlen((char*)date));
        GUILABEL_SetText(MMISCH_DATE_EDIT_LABEL_CTRL_ID, &date_str, FALSE);	
        if (PNULL != date_str.wstr_ptr)
        {
            SCI_FREE(date_str.wstr_ptr);
            date_str.wstr_ptr = PNULL;
        }
    }
#else
	date_display_type = MMIAPISET_GetDateDisplayType();
	if(MMISET_DATE_YMD == date_display_type)
	{
		date_display_style = GUIEDIT_DATE_STYLE_YMD;
	}
	else if(MMISET_DATE_MDY == date_display_type)
	{
		date_display_style = GUIEDIT_DATE_STYLE_MDY;
	}
	else if(MMISET_DATE_DMY == date_display_type)
	{
		date_display_style = GUIEDIT_DATE_STYLE_DMY;
	}
#ifdef MMI_GUI_STYLE_MINISCREEN
	GUIEDIT_SetDateStyle(MMISCH_DATE_EDIT_CTRL_ID,&date_show_style,&date_display_style,FALSE);
#else
    GUIEDIT_SetDateStyle(MMISCH_DATE_EDIT_CTRL_ID,NULL,&date_display_style,FALSE);
#endif
	GUIEDIT_SetDate(MMISCH_DATE_EDIT_CTRL_ID,
		            event_ptr->event_fast_info.year,
		            event_ptr->event_fast_info.mon,
		            event_ptr->event_fast_info.day);
#endif
}
#endif
/*****************************************************************************/
// 	Description : set time param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinTimeParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         )
{	
#ifndef MMI_PDA_SUPPORT
	GUIEDIT_TIME_STYLE_E			time_style = GUIEDIT_TIME_STYLE_NULL;
	MMISET_TIME_DISPLAY_TYPE_E		time_type = MMISET_TIME_12HOURS;
#endif
	//SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
	if(PNULL == event_ptr)
    {
        return;
    }
    
    GUIFORM_SetStyle(MMISCH_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	GUILABEL_SetTextById(MMISCH_TIME_TITLE_LABEL_CTRL_ID, TXT_TIME,FALSE);
#ifdef MMI_PDA_SUPPORT
    {
        uint8               time[MMISET_TIME_STR_12HOURS_LEN+1] = {0};
        MMI_STRING_T        time_str= {0};
        MMIAPISET_FormatTimeStrByTime(event_ptr->event_fast_info.hour, event_ptr->event_fast_info.minute,time,MMISET_TIME_STR_12HOURS_LEN+1);
        time_str.wstr_len = SCI_STRLEN((char*)time);
        time_str.wstr_ptr = SCI_ALLOC_APPZ((time_str.wstr_len +1)*sizeof(wchar));
        MMI_STRNTOWSTR(time_str.wstr_ptr,time_str.wstr_len,time,strlen((char*)time),strlen((char*)time));
        GUILABEL_SetText(MMISCH_TIME_EDIT_LABEL_CTRL_ID, &time_str, FALSE);
        if (PNULL != time_str.wstr_ptr)
        {
            SCI_FREE(time_str.wstr_ptr);
            time_str.wstr_ptr = PNULL;
        }

    }
#else
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
	GUIEDIT_SetTimeStyle(MMISCH_TIME_EDIT_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
	//GUIEDIT_SetFont(MMISCH_TIME_EDIT_CTRL_ID, MMI_SET_TIME_FONT);
	GUIEDIT_SetTime(
				MMISCH_TIME_EDIT_CTRL_ID,
				event_ptr->event_fast_info.hour,
				event_ptr->event_fast_info.minute,
				0);
		
#endif
}
/*****************************************************************************/
//  Description : change float string to integer value
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author://CR132759
//  Note: COPY FROM ALARM
/*****************************************************************************/
// LOCAL int16 AlarmConvertFloatStrToInt(const MMI_STRING_T *str_ptr, int32 mul_scale)
// {
//     BOOLEAN result = TRUE;
//     int32  i = 0;
//     int16  ret =0;
//     uint16 num_len = 0;
//     int16  point_index =0;
//     uint8  num_buf[MMIALM_CHANNEL_NAME_LEN + 1] = {0};
//     uint8  decimal_buf[MMIALM_CHANNEL_NAME_LEN + 1] = {0};
//     
//     if(str_ptr != PNULL && str_ptr->wstr_ptr != PNULL)
//     {
//         num_len = MIN(MMIALM_CHANNEL_NAME_LEN, str_ptr->wstr_len);/*lint !e64*/
//         MMI_WSTRNTOSTR((char *)num_buf, MMIALM_CHANNEL_NAME_LEN, str_ptr->wstr_ptr, MMIALM_CHANNEL_NAME_LEN, num_len);/*lint !e64*/
//         //get point postion
//         for(i =0; i< num_len; i++)
//         {
//             if('.' == num_buf[i])
//             {
//                 point_index = i;
//                 break;
//             }
//         }
//         
//         for(i = point_index +1; i < num_len; i++)
//         {
// 			if( ( '0' > num_buf[i] ) || ( '9' < num_buf[i] )) 
// 			{
// 				result = FALSE;
// 				break;
// 			}
//         }
//         
//         if(result)
//         {
//             if(0 != point_index && (point_index + 1 < num_len))
//             {
//                 strncpy(decimal_buf, &num_buf[point_index +1], 1);/*lint !e64*/
//             }
//             if(point_index > 0)
//             {
//                 //no point
//                 num_buf[point_index] = 0;
//             }
//             ret = atoi(num_buf)*mul_scale;/*lint !e64*/
//             ret += atoi(decimal_buf);/*lint !e64*/
//         }
//         else
//         {
//             ret = 0;
//         }
//     }
//     //SCI_TRACE_LOW:"[MMIFM] ConvertFloatStrToInt ret %d"
//     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_10395_112_2_18_2_1_11_56,(uint8*)"d", ret);
//     return ret;
// }
/*****************************************************************************/
// 	Description : set ring param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinRingParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         )
{
	MMI_STRING_T    ring = {0};
	uint16		     fixed_ringid  = 0;//coverity 12689
// 	MMI_TEXT_ID_T   ring_text      = TXT_NULL;
	wchar              file_name[MMIFILE_FILE_NAME_MAX_LEN+1]   = {0};
    	uint16              file_name_len = 0;
	
	//SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
	if(PNULL == event_ptr)
    {
        return;
    }
	ring.wstr_ptr = file_name; //CR132759

    fixed_ringid  = event_ptr->event_content_info.ring;
    GUIFORM_SetStyle(MMISCH_RING_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	
	GUILABEL_SetTextById(MMISCH_RING_TITLE_LABEL_CTRL_ID, TXT_RING,FALSE);

	//if sch ring is download ring, check whether file exists
	if((ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type||ALM_RECORD_RING == event_ptr->event_content_info.ring_type) 
		&&!MMIAPIFMM_IsFileExist(event_ptr->event_ring_info.dring_name_arr,event_ptr->event_ring_info.dring_name_len))
	{
		event_ptr->event_content_info.ring_type = ALM_FIX_RING;
		event_ptr->event_content_info.ring = 1;
	}	

	//@zhaohui,cr112262,应只有ALM_DOWNLOAD_RING，ALM_FIX_RING两种情况
	if(ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type
     ||ALM_RECORD_RING == event_ptr->event_content_info.ring_type)
	{
		MMIAPIFMM_SplitFullPath(event_ptr->event_ring_info.dring_name_arr, event_ptr->event_ring_info.dring_name_len,
		NULL, NULL, NULL, NULL,
		file_name, &file_name_len);

		ring.wstr_ptr = file_name; //(uint8 *)ring_str;  
		ring.wstr_len = file_name_len;

		GUILABEL_SetText(MMISCH_RING_LABEL_CTRL_ID, &ring, FALSE);
	}
#ifdef FM_SUPPORT//CR132759
	else if(ALM_FM_RING== event_ptr->event_content_info.ring_type)
	{
		wchar fm[3]={'F','M',0};
		MMI_STRING_T ring_str={0};	
		wchar*channel_name = PNULL;

		ring_str.wstr_ptr = event_ptr->event_ring_info.dring_name_arr;
        ring_str.wstr_len = event_ptr->event_ring_info.dring_name_len;
		channel_name = MMIAPIFM_GetChannelName(MMIAPIALM_ConvertFloatStrToInt(&ring_str,10));

		if(PNULL!=channel_name)
		{
			ring.wstr_len = MMIAPICOM_Wstrlen(channel_name);
			ring.wstr_ptr = channel_name; 
        }
		else
		{
            MMIAPICOM_Wstrncpy(ring.wstr_ptr, fm, 2);
            MMIAPICOM_Wstrcat(ring.wstr_ptr, ring_str.wstr_ptr);
            ring.wstr_len=MMIAPICOM_Wstrlen(ring.wstr_ptr);
        }
		GUILABEL_SetText(MMISCH_RING_LABEL_CTRL_ID, &ring, FALSE);
	}
#endif
	else//ALM_FIX_RING
	{
            ring.wstr_len = MMIAPISET_GetRingName(fixed_ringid, file_name,MMIFILE_FILE_NAME_MAX_LEN);
            ring.wstr_ptr = file_name;
            GUILABEL_SetText(MMISCH_RING_LABEL_CTRL_ID, &ring, FALSE);
	}
	
}

/*****************************************************************************/
// 	Description : set freq type param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinFreqTypeParam(void)
{		
	MMI_STRING_T        text            = {0};
//  将repeat mode与alarm的统一
	GUIFORM_SetStyle(MMISCH_FREQ_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMI_GUI_STYLE_MINISCREEN
    GUIFORM_SetStyle(MMISCH_FREQ_TYPE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif
    
    MMI_GetLabelTextByLang(TXT_ALARM_EDIT_MODE, &text);
    GUILABEL_SetText(MMISCH_FREQ_TITLE_LABEL_CTRL_ID, &text, FALSE);
    
    GUISETLIST_SetTitleTextId(MMISCH_FREQ_SETLIST_CTRL_ID, TXT_ALARM_EDIT_MODE);
	///
//     GUIFORM_SetStyle(MMISCH_FREQ_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
// 
//     GUILABEL_SetTextById(MMISCH_FREQ_TITLE_LABEL_CTRL_ID, TXT_ALARM_EDIT_MODE,FALSE);
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : set the Remind Time Param
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetSchEditWinRemindParam(MMIACC_SMART_EVENT_T *event_ptr)
{		
    uint32	i = 0;
    MMI_TEXT_ID_T text_arr[] = {
                                TXT_SCH_REMIND_ONTIME,
                                TXT_SCH_REMIND_FIVE_MIN,
                                TXT_SCH_REMIND_FIFTEEN_MIN,
                                TXT_SCH_REMIND_ONE_HOUR,
                                TXT_SCH_REMIND_ONE_DAY,
                                TXT_SCH_REMIND_ONE_WEEK
                                };
    
    if(PNULL == event_ptr)
    {
        SCI_TRACE_LOW("SetSchEditWinRemindParam PNULL == event_ptr");
        return;
    }
    
    GUIFORM_SetStyle(MMISCH_REMIND_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISCH_REMIND_TITLE_LABLE_CTRL_ID, TXT_SCH_AWAKE_TIME,FALSE);

    for(i = 0; i < ARR_SIZE(text_arr); i++)
    {
        GUISETLIST_AddItemById(MMISCH_REMIND_SETLIST_CTRL_ID, text_arr[i]);
    }
    
    if(event_ptr->event_fast_info.remind_type > MMISCH_REMIND_ONE_WEEK)
    {
        event_ptr->event_fast_info.remind_type = MMISCH_REMIND_ONTIME;
    }
    GUISETLIST_SetCurIndex(MMISCH_REMIND_SETLIST_CTRL_ID, (uint32)event_ptr->event_fast_info.remind_type);
    GUISETLIST_SetTitleTextId(MMISCH_REMIND_SETLIST_CTRL_ID, TXT_SCH_AWAKE_TIME);
}

/*****************************************************************************/
//  Description : save the Remind Time
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SaveSchEditWinRemindTime(MMIACC_SMART_EVENT_T *event_ptr)
{
    //SCI_ASSERT(event_ptr != PNULL);
    if(PNULL == event_ptr)
    {
        SCI_TRACE_LOW("SaveSchEditWinRemindTime PNULL == event_ptr");
        return;
    }

    event_ptr->event_fast_info.remind_type = (MMISCH_REMIND_TYPE_E)GUISETLIST_GetCurIndex(MMISCH_REMIND_SETLIST_CTRL_ID);

    if(event_ptr->event_fast_info.remind_type > MMISCH_REMIND_ONE_WEEK)
    {
        event_ptr->event_fast_info.remind_type = MMISCH_REMIND_ONTIME;
    }
}
#endif

/*****************************************************************************/
// 	Description : set deadline  param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDeadlineParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         )
{	
	SCI_DATE_T	deadline= {0};
	SCI_DATE_T    second_date = {0};
#ifndef MMI_PDA_SUPPORT
	MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
	GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;
#endif
	//SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
	if(PNULL == event_ptr)
    {
        return;
    }
	GUIFORM_SetStyle(MMISCH_DEADLINE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    
	GUILABEL_SetTextById(MMISCH_DEADLINE_TITLE_LABEL_CTRL_ID, TXT_CALENDAR_EDIT_DEADLINE,FALSE);

	deadline.year = event_ptr->event_fast_info.end_year;
	deadline.mon= event_ptr->event_fast_info.end_mon;
	deadline.mday= event_ptr->event_fast_info.end_day;

#ifdef CMCC_UI_STYLE
	second_date.year = event_ptr->event_fast_info.stop_year;
	second_date.mon  = event_ptr->event_fast_info.stop_mon;
	second_date.mday = event_ptr->event_fast_info.stop_day;
#else
	second_date.year = event_ptr->event_fast_info.year;
	second_date.mon  = event_ptr->event_fast_info.mon;
	second_date.mday = event_ptr->event_fast_info.day;	
#endif
	if (IsValidDeadline(second_date, deadline) 
		|| ALM_MODE_ONCE == event_ptr->event_fast_info.fre_mode
		||ALM_MODE_NO== event_ptr->event_fast_info.fre_mode)
	{
#ifdef CMCC_UI_STYLE
        event_ptr->event_fast_info.end_year = event_ptr->event_fast_info.stop_year;
        event_ptr->event_fast_info.end_mon = event_ptr->event_fast_info.stop_mon;
        event_ptr->event_fast_info.end_day = event_ptr->event_fast_info.stop_day;

#else
        event_ptr->event_fast_info.end_year = event_ptr->event_fast_info.year;
        event_ptr->event_fast_info.end_mon = event_ptr->event_fast_info.mon;
        event_ptr->event_fast_info.end_day = event_ptr->event_fast_info.day;
#endif
	}
	//GUIEDIT_SetDate(MMISCH_DEADLINE_EDIT_CTRL_ID, event_ptr->event_fast_info.end_year, event_ptr->event_fast_info.end_mon, event_ptr->event_fast_info.end_day);	
#ifdef MMI_PDA_SUPPORT
    {
        MMI_STRING_T        date_str                                = {0};
        uint8               date[MMISET_DATE_STR_LEN]                                = {0};
        MMIAPISET_FormatDateStrByDateStyle(
                        event_ptr->event_fast_info.end_year, 
                        event_ptr->event_fast_info.end_mon, 
                        event_ptr->event_fast_info.end_day,
                        '.',date,MMISET_DATE_STR_LEN);

        date_str.wstr_len = SCI_STRLEN((char*)date);
        date_str.wstr_ptr = SCI_ALLOC_APPZ((date_str.wstr_len +1)*sizeof(wchar));
        MMI_STRNTOWSTR(date_str.wstr_ptr,date_str.wstr_len,date,strlen((char*)date),strlen((char*)date));
        GUILABEL_SetText(MMISCH_DEADLINE_EDIT_LABEL_CTRL_ID, &date_str, FALSE);		

        if (PNULL != date_str.wstr_ptr)
        {
            SCI_FREE(date_str.wstr_ptr);
            date_str.wstr_ptr = PNULL;
        }
    }
#else
	date_display_type = MMIAPISET_GetDateDisplayType();
	if(MMISET_DATE_YMD == date_display_type)
	{
		date_display_style = GUIEDIT_DATE_STYLE_YMD;
	}
	else if(MMISET_DATE_MDY == date_display_type)
	{
		date_display_style = GUIEDIT_DATE_STYLE_MDY;
	}
	else if(MMISET_DATE_DMY == date_display_type)
	{
		date_display_style = GUIEDIT_DATE_STYLE_DMY;
	}
	GUIEDIT_SetDateStyle(MMISCH_DEADLINE_EDIT_CTRL_ID,PNULL,&date_display_style,FALSE);       
	GUIEDIT_SetDate(MMISCH_DEADLINE_EDIT_CTRL_ID,
				event_ptr->event_fast_info.end_year,
				event_ptr->event_fast_info.end_mon,
				event_ptr->event_fast_info.end_day); 
#endif
}

/*****************************************************************************/
// 	Description : set week list param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinWeeklistParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr, //in
					                         MMI_WIN_ID_T	    win_id
					                         )
{
    uint8 day_num=0;
    uint8   week_day =0;
    // 	MMI_STRING_T string={0};
    uint32 ctrl_id[]={	MMISCH_EDITWIN_BUTTON0_CTRL_ID,
      	MMISCH_EDITWIN_BUTTON1_CTRL_ID,
      	MMISCH_EDITWIN_BUTTON2_CTRL_ID,
      	MMISCH_EDITWIN_BUTTON3_CTRL_ID,
      	MMISCH_EDITWIN_BUTTON4_CTRL_ID,
      	MMISCH_EDITWIN_BUTTON5_CTRL_ID,
      	MMISCH_EDITWIN_BUTTON6_CTRL_ID,
    };
    GUIFORM_CHILD_WIDTH_T  width={0}; 
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};
    uint16 hor_space = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;
    GUI_BORDER_T border = {0};
    uint8   week_day_image_adjust = 0;
    bg.bg_type=GUI_BG_IMG;
    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif

	GUIFORM_SetStyle(MMISCH_EDITWIN_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);	
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_CLOCK_ALARM_WEEK_ENABLED, win_id);

    width.type=GUIFORM_CHILD_WIDTH_FIXED;
   width.add_data = (MMI_MAINSCREEN_WIDTH-3*3-1)/ALM_WEEK_DAY_NUM;//左边空距+滚动条+右边空距+1
       GUIFORM_SetSpace(MMISCH_EDITWIN_CHILD_FORM_CTRL_ID,&hor_space,PNULL);
    GUIFORM_SetMargin(MMISCH_EDITWIN_CHILD_FORM_CTRL_ID,0);
    GUIFORM_PermitChildFont(MMISCH_EDITWIN_CHILD_FORM_CTRL_ID, FALSE);    
    border.type = GUI_BORDER_NONE;
    IGUICTRL_SetBorder(MMK_GetCtrlPtr(MMISCH_EDITWIN_CHILD_FORM_CTRL_ID), &border);

    for(day_num = 0; day_num < SCH_WEEK_DAY_NUM; day_num++)   
    {//对weekly显示的icon微调
		if ((width.add_data * ALM_WEEK_DAY_NUM + week_day_image_adjust) < (MMI_MAINSCREEN_WIDTH-3*3-1))
		{
			width.add_data = width.add_data + 1;
			week_day_image_adjust++;
		}
        week_day = MMIAPISET_GetWeekDayByOffset(day_num );
#ifdef MAINLCD_DEV_SIZE_128X160
        GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_TOP);
#else
        GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_HVMIDDLE);
#endif
        GUIBUTTON_SetTextId(ctrl_id[day_num],s_alm_week_text_id[ week_day ]);
        if(0!=(event_ptr->event_fast_info.fre & s_week_mask[week_day]) ) 
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;
            s_weeks_is_selected[day_num]=TRUE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;
            s_weeks_is_selected[day_num]=FALSE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &disabled_font);
        }
        GUIBUTTON_SetBg(ctrl_id[day_num],&bg);
        GUIFORM_SetChildWidth(MMISCH_EDITWIN_CHILD_FORM_CTRL_ID,ctrl_id[day_num],&width);
        GUIBUTTON_PermitBorder(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetOwnerDraw(ctrl_id[day_num],SCHButtonOwnerDrawFun);
        bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        GUIBUTTON_SetRunSheen(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetPressedBg(ctrl_id[day_num],&bg);
		width.add_data = (MMI_MAINSCREEN_WIDTH-3*3-1)/ALM_WEEK_DAY_NUM;
    }
}

/*****************************************************************************/
// 	Description : Append the item of weeks list
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:
/*****************************************************************************/
LOCAL void AppendWeeksListItem(
                                               MMI_CTRL_ID_T    ctrl_id ,//in
                                               MMIACC_SMART_EVENT_T*	event_ptr //in
                                               )
{
    	uint8 day_num=0;
	uint8   week_day =0;
	GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
       GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/

	//add items to listbox
	for(day_num = 0; day_num < SCH_WEEK_DAY_NUM; day_num++)   
	{
		week_day = MMIAPISET_GetWeekDayByOffset(day_num );
    
		item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
		item_t.item_data_ptr = &item_data;

		item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
		item_data.item_content[0].item_data.text_id = s_alm_week_text_id[ week_day ];
    
    		GUILIST_AppendItem( ctrl_id, &item_t );  

		if ((event_ptr->event_fast_info.fre & s_week_mask[week_day]) != 0) 
		{
			GUILIST_SetSelectedItem(ctrl_id, day_num, TRUE);
		}
	}  
}

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinWeeklistDisplay(void )
{
	uint32 setlist_index=0;
    
    GUIFORM_SetCircularHandleLeftRight(MMISCH_EDITWIN_CHILD_FORM_CTRL_ID,TRUE);

	setlist_index=GUISETLIST_GetCurIndex(MMISCH_FREQ_SETLIST_CTRL_ID);
	if(MMISCH_MODE_EVERYWEEK!=setlist_index)
	{
		
		GUIFORM_SetChildDisplay(MMISCH_EDIT_WIN_FORM_CTRL_ID, 
							MMISCH_EDITWIN_CHILD_FORM_CTRL_ID, 
							GUIFORM_CHILD_DISP_HIDE);
	}
	else
	{
		
		GUIFORM_SetChildDisplay(MMISCH_EDIT_WIN_FORM_CTRL_ID, 
							MMISCH_EDITWIN_CHILD_FORM_CTRL_ID, 
							GUIFORM_CHILD_DISP_NORMAL);
	}
}

/*****************************************************************************/
// 	Description : set week list display
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDeadlineDisplay(void )
{
	uint32 setlist_index=0;
	setlist_index=GUISETLIST_GetCurIndex(MMISCH_FREQ_SETLIST_CTRL_ID);

    GUIFORM_SetStyle(MMISCH_DEADLINE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	if(MMISCH_MODE_AWAKE_NO==setlist_index ||MMISCH_MODE_ONCE==setlist_index)
	{
		
		GUIFORM_SetChildDisplay(MMISCH_EDIT_WIN_FORM_CTRL_ID, 
            MMISCH_DEADLINE_FORM_CTRL_ID, 
            GUIFORM_CHILD_DISP_HIDE);
	}
	else
	{
		
		GUIFORM_SetChildDisplay(MMISCH_EDIT_WIN_FORM_CTRL_ID, 
            MMISCH_DEADLINE_FORM_CTRL_ID, 
            GUIFORM_CHILD_DISP_NORMAL);
	}
}

/*****************************************************************************/
//  Description : 设置SETLIST 的数据
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetFreqSetList(
                              MMIACC_SMART_EVENT_T*	event_ptr  //in
                              )
{
	uint32	i = 0;
	MMISCH_MODE_TYPE_E          index = MMISCH_MODE_ONCE;
	MMI_TEXT_ID_T			text_arr[SCH_DROP_NUM] = 
	{	TXT_ALARM_MODE_ONCE,//@zhaohui,cr108959
		TXT_SCH_AWAKE_EVERYDAY,
		TXT_SCH_AWAKE_EVERYWEEK,
		TXT_SCH_AWAKE_EVERYMONTH,
		TXT_SCH_AWAKE_EVERYYEAR,
		TXT_SCH_AWAKE_NO
	};

	//SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
	if(PNULL == event_ptr)
    {
        return;
    }
    
	//初始化下拉列表
	if(FALSE == event_ptr->event_fast_info.is_on)
	{
		index = MMISCH_MODE_AWAKE_NO;
	}
	else
	{
		switch(event_ptr->event_fast_info.fre_mode)
		{
		case ALM_MODE_ONCE:
			index = MMISCH_MODE_ONCE;
			break;

		case ALM_MODE_EVERYDAY:
			index = MMISCH_MODE_EVERYDAY;
			break;

		case ALM_MODE_EVERYWEEK:
			index = MMISCH_MODE_EVERYWEEK;
			break;

		case ALM_MODE_EVERYMONTH:
			index = MMISCH_MODE_EVERYMONTH;;
			break;

		case ALM_MODE_EVERYYEAR:
			index = MMISCH_MODE_EVERYYEAR;
			break;

		default:
			break;
		}
	}

	for(i = 0; i < SCH_DROP_NUM; i++)
	{
		GUISETLIST_AddItemById(MMISCH_FREQ_SETLIST_CTRL_ID, text_arr[i]);
	}
	GUISETLIST_SetCurIndex(MMISCH_FREQ_SETLIST_CTRL_ID, (uint32)index );
//    GUISETLIST_SetTitleTextId(MMISCH_FREQ_SETLIST_CTRL_ID, TXT_COMMON_TYPE);
}

/*****************************************************************************/
//  Description : save the frequency mode
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SaveSchEditWinFreqMode(
					                              MMIACC_SMART_EVENT_T*	event_ptr  //in
					                              )
{
// 	int32        loop=0;
// 	uint16      mode = 0;
	
	//SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
	if(PNULL == event_ptr)
    {
        return;
    }
    
	event_ptr->event_fast_info.is_on = TRUE; 
	switch(GUISETLIST_GetCurIndex(MMISCH_FREQ_SETLIST_CTRL_ID))
	{
	case MMISCH_MODE_ONCE:
		event_ptr->event_fast_info.fre_mode = ALM_MODE_ONCE;
		break;

	case MMISCH_MODE_EVERYDAY:
		event_ptr->event_fast_info.fre_mode  = ALM_MODE_EVERYDAY;
		break;

	case MMISCH_MODE_EVERYWEEK:
		event_ptr->event_fast_info.fre_mode  = ALM_MODE_EVERYWEEK;
		break;

	case MMISCH_MODE_EVERYMONTH:
		event_ptr->event_fast_info.fre_mode  = ALM_MODE_EVERYMONTH;
		break;

	case MMISCH_MODE_EVERYYEAR:
		event_ptr->event_fast_info.fre_mode  = ALM_MODE_EVERYYEAR;
		break;

	case MMISCH_MODE_AWAKE_NO:
		event_ptr->event_fast_info.fre_mode = ALM_MODE_NO;
		event_ptr->event_fast_info.is_on = FALSE;
		break;

	default:
		break;
	}  
}
/*****************************************************************************/
//  Description : save the name
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinName(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              )
{
    MMI_STRING_T  str={0};

    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return FALSE;
    }	
    
    GUIEDIT_GetString(MMISCH_NAME_EDIT_CTRL_ID, &str);
#if 0		//NEWMS00214305 [6530][64X64_240X400PDA2]搜狗输入法，点击日历-创建新日程-点击主题，任意输入法，输入多行键码点击回车键无效
	if (MMIAPISET_IsIncludeReturnChar(str.wstr_ptr,str.wstr_len))
	{
		MMIPUB_OpenAlertFailWin(TXT_SCH_INVALID_TITLE);
		return FALSE;
	}
#endif	
    event_ptr->event_content_info.w_length = str.wstr_len;

    if (!str.wstr_len)//if str.str_len is 0,should clean event_ptr->content
    {
        str.wstr_len = MMIACC_EVENT_TEXT_MAX_LEN;
    }
    SCI_MEMSET(event_ptr->event_content_info.w_content,0,sizeof(wchar)*(MMIACC_EVENT_TEXT_MAX_LEN+1));
    MMI_WSTRNCPY(
            event_ptr->event_content_info.w_content, MMIACC_EVENT_TEXT_MAX_LEN,
            str.wstr_ptr, str.wstr_len,
            str.wstr_len);

    return TRUE;
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : save the text
//  Global resource dependence : 
//  Author:panvs
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinText(
	                              MMIACC_SMART_EVENT_T*	event_ptr  //in
	                              )
{
    MMI_STRING_T  str={0};

    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        SCI_TRACE_LOW("SaveSchEditWinText PNULL == event_ptr");
        return FALSE;
    }    

    GUIEDIT_GetString(MMISCH_TEXT_EDIT_CTRL_ID, &str);
    if (MMIAPISET_IsIncludeReturnChar(str.wstr_ptr,str.wstr_len))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SCH_INVALID_TITLE, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL,PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        return FALSE;
    }	        
    event_ptr->event_content_info.w_text_length = str.wstr_len;

    if (0 == str.wstr_len)//if str.str_len is 0,should clean event_ptr->content
    {
        str.wstr_len = MMIACC_EVENT_TEXT_MAX_LEN;
    }

    MMI_WSTRNCPY(
        event_ptr->event_content_info.w_text, 
        MMIACC_EVENT_TEXT_MAX_LEN,
        str.wstr_ptr, 
        str.wstr_len,
        str.wstr_len);

    return TRUE;
}

/*****************************************************************************/
// 	Description : set text param
//	Global resource dependence : 
//  Author:  panvs
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinTextParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         )
{
    MMI_STRING_T    text_str  = {0};

    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        SCI_TRACE_LOW("SetSchEditWinTextParam PNULL == event_ptr");
        return;
    }

    GUIFORM_SetStyle(MMISCH_TEXT_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMISCH_TEXT_TITLE_LABEL_CTRL_ID, TXT_SCH_TITLE_CONTENT,FALSE);

    text_str.wstr_ptr = event_ptr->event_content_info.w_text;
    text_str.wstr_len = event_ptr->event_content_info.w_text_length;
    GUIEDIT_SetString(MMISCH_TEXT_EDIT_CTRL_ID, text_str.wstr_ptr, text_str.wstr_len );
}

/*****************************************************************************/
// 	Description : set date param
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDateParam(
		                         MMIACC_SMART_EVENT_T*	event_ptr //in
		                         )
{	
    MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
    GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;
    GUIEDIT_TIME_STYLE_E time_display_style = GUIEDIT_TIME_STYLE_NULL;

    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        SCI_TRACE_LOW("SetSchEditWinDateParam PNULL == event_ptr");
        return;
    }

    GUIFORM_SetStyle(MMISCH_DATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMISCH_DATE_TITLE_LABEL_CTRL_ID, TXT_COMMON_START,FALSE);

    //set date
    date_display_type = MMIAPISET_GetDateDisplayType();
    if(MMISET_DATE_YMD == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_YMD;
    }
    else if(MMISET_DATE_MDY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_MDY;
    }
    else if(MMISET_DATE_DMY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_DMY;
    }
    GUIEDIT_SetDateStyle(MMISCH_DATE_EDIT_CTRL_ID,PNULL,&date_display_style,FALSE);
    GUIEDIT_SetDate(
        MMISCH_DATE_EDIT_CTRL_ID,
        event_ptr->event_fast_info.year,
        event_ptr->event_fast_info.mon,
        event_ptr->event_fast_info.day);

    //set time	
    if (MMISET_TIME_12HOURS == MMIAPISET_GetTimeDisplayType())
    {
        time_display_style = GUIEDIT_TIME_STYLE_12;
    }
    else
    {
        time_display_style = GUIEDIT_TIME_STYLE_24;
    }	    
    GUIEDIT_SetTimeStyle(MMISCH_TIME_EDIT_CTRL_ID,PNULL,&time_display_style,PNULL,FALSE);
    GUIEDIT_SetTime(
        MMISCH_TIME_EDIT_CTRL_ID,
        event_ptr->event_fast_info.hour,
        event_ptr->event_fast_info.minute,
        0);
}

/*****************************************************************************/
// 	Description : set date end param
//	Global resource dependence : 
//  Author:  PANVS
//	Note:   
/*****************************************************************************/
LOCAL void SetSchEditWinDateStopParam(
					                         MMIACC_SMART_EVENT_T*	event_ptr //in
					                         )
{	
    MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
    GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;
    GUIEDIT_TIME_STYLE_E time_display_style = GUIEDIT_TIME_STYLE_NULL;

    SCI_ASSERT(event_ptr != PNULL);/*assert verified*/

    GUIFORM_SetStyle(MMISCH_DATE_STOP_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    GUILABEL_SetTextById(MMISCH_DATE_STOP_TITLE_LABEL_CTRL_ID, STXT_STOP,FALSE);

    if(event_ptr->event_fast_info.stop_year == 0)
    {
        event_ptr->event_fast_info.stop_year = event_ptr->event_fast_info.year;
        event_ptr->event_fast_info.stop_mon = event_ptr->event_fast_info.mon;
        event_ptr->event_fast_info.stop_day = event_ptr->event_fast_info.day;
        event_ptr->event_fast_info.stop_hour = event_ptr->event_fast_info.hour;
        event_ptr->event_fast_info.stop_minute = event_ptr->event_fast_info.minute;
    }
    
    //set date
    date_display_type = MMIAPISET_GetDateDisplayType();
    if(MMISET_DATE_YMD == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_YMD;
    }
    else if(MMISET_DATE_MDY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_MDY;
    }
    else if(MMISET_DATE_DMY == date_display_type)
    {
        date_display_style = GUIEDIT_DATE_STYLE_DMY;
    }
    GUIEDIT_SetDateStyle(MMISCH_DATE_STOP_EDIT_CTRL_ID,PNULL,&date_display_style,FALSE);
    GUIEDIT_SetDate(
        MMISCH_DATE_STOP_EDIT_CTRL_ID,
        event_ptr->event_fast_info.stop_year,
        event_ptr->event_fast_info.stop_mon,
        event_ptr->event_fast_info.stop_day);

    //set time	
    if (MMISET_TIME_12HOURS == MMIAPISET_GetTimeDisplayType())
    {
        time_display_style = GUIEDIT_TIME_STYLE_12;
    }
    else
    {
        time_display_style = GUIEDIT_TIME_STYLE_24;
    }	    
    GUIEDIT_SetTimeStyle(MMISCH_TIME_STOP_EDIT_CTRL_ID,PNULL,&time_display_style,PNULL,FALSE);
    GUIEDIT_SetTime(
        MMISCH_TIME_STOP_EDIT_CTRL_ID,
        event_ptr->event_fast_info.stop_hour,
        event_ptr->event_fast_info.stop_minute,
        0);
}

/*****************************************************************************/
//  Description : save the date
//  Global resource dependence : 
//  Author:panvs
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinStopDateTime(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              )
{
    SCI_DATE_T                  start_date        = {0};
    SCI_TIME_T                  start_time        = {0};
    SCI_DATE_T                  stop_date        = {0};
    SCI_TIME_T                  stop_time        = {0};
    MMISET_DATE_T     date = {0};
    MMISET_TIME_T       time = {0};

    SCI_ASSERT(event_ptr != PNULL);/*assert verified*/

    GUIEDIT_GetDate(MMISCH_DATE_STOP_EDIT_CTRL_ID, &date.date_year, &date.date_month, &date.date_day);
    if (!MMIAPICOM_CheckDateIsValid(date))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_INVALID_DATE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    
    GUIEDIT_GetTime(MMISCH_TIME_STOP_EDIT_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
    if (!MMIAPICOM_CheckTimeIsValid(time))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_INVALID_TIME,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return FALSE;
    }
    
    start_date.year = event_ptr->event_fast_info.year;
    start_date.mon = event_ptr->event_fast_info.mon;
    start_date.mday = event_ptr->event_fast_info.day;
    start_time.hour = event_ptr->event_fast_info.hour;
    start_time.min = event_ptr->event_fast_info.minute;
    start_time.sec = 0;
    stop_date.year = date.date_year;
    stop_date.mon = date.date_month;
    stop_date.mday = date.date_day;
    stop_time.hour = time.time_hour;
    stop_time.min = time.time_minute;
    stop_time.sec = 0;

    if(CMP_SECOND_SMALL != MMIAPIALM_CompareDateAndTime(start_date, start_time, stop_date, stop_time))
    {
        event_ptr->event_fast_info.stop_year = date.date_year;
        event_ptr->event_fast_info.stop_mon = date.date_month;
        event_ptr->event_fast_info.stop_day = date.date_day;
        event_ptr->event_fast_info.stop_hour = time.time_hour;
        event_ptr->event_fast_info.stop_minute = time.time_minute;        
    }
    else
    {
        event_ptr->event_fast_info.stop_year = event_ptr->event_fast_info.year;
        event_ptr->event_fast_info.stop_mon = event_ptr->event_fast_info.mon;
        event_ptr->event_fast_info.stop_day = event_ptr->event_fast_info.day;
        event_ptr->event_fast_info.stop_hour = event_ptr->event_fast_info.hour;
        event_ptr->event_fast_info.stop_minute = event_ptr->event_fast_info.minute;  
    }
    
    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : save the date
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinDate(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              )
{
    MMISET_DATE_T     date = {0};

    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return FALSE;
    }
    
    GUIEDIT_GetDate(MMISCH_DATE_EDIT_CTRL_ID, 
                    &date.date_year,
                    &date.date_month,
                    &date.date_day);
    if (MMIAPICOM_CheckDateIsValid(date))
    {
        event_ptr->event_fast_info.start_year = date.date_year;
        event_ptr->event_fast_info.start_mon = date.date_month;
        event_ptr->event_fast_info.start_day = date.date_day;
        event_ptr->event_fast_info.year = date.date_year;
        event_ptr->event_fast_info.mon = date.date_month;
        event_ptr->event_fast_info.day = date.date_day;
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : save the time
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinTime(
                                              MMIACC_SMART_EVENT_T*	event_ptr  //in
                                              )
{
    MMISET_TIME_T       time = {0};
    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return FALSE;
    }
    
    GUIEDIT_GetTime(MMISCH_TIME_EDIT_CTRL_ID, 
                    &time.time_hour, 
                    &time.time_minute,
                    PNULL);

    if (MMIAPICOM_CheckTimeIsValid(time))
    {
        event_ptr->event_fast_info.hour = time.time_hour;
        event_ptr->event_fast_info.minute = time.time_minute;
    }
    else 
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : save the deadline time
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSchEditWinDeadlineDate(
				                              MMIACC_SMART_EVENT_T*	event_ptr  //in
				                              )
{
    MMISET_DATE_T     date = {0};
    SCI_DATE_T      second_date = {0};
    SCI_DATE_T      deadline = {0};

    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return FALSE;
    }
    
    GUIEDIT_GetDate(MMISCH_DEADLINE_EDIT_CTRL_ID, 
                    &date.date_year,
                    &date.date_month,
                    &date.date_day);

    if (MMIAPICOM_CheckDateIsValid(date))
    {
        event_ptr->event_fast_info.end_year = date.date_year;
        event_ptr->event_fast_info.end_mon = date.date_month;
        event_ptr->event_fast_info.end_day = date.date_day;		    

        //when deadline earlier than event time,alert!
#ifdef CMCC_UI_STYLE
        second_date.year = event_ptr->event_fast_info.stop_year;
        second_date.mon  = event_ptr->event_fast_info.stop_mon;
        second_date.mday = event_ptr->event_fast_info.stop_day;

        event_ptr->event_fast_info.end_year = MAX(event_ptr->event_fast_info.end_year, second_date.year);
        event_ptr->event_fast_info.end_mon = MAX(event_ptr->event_fast_info.end_mon, second_date.mon);
        event_ptr->event_fast_info.end_day = MAX(event_ptr->event_fast_info.end_day, second_date.mday);
#else
        second_date.year = event_ptr->event_fast_info.year;
        second_date.mon  = event_ptr->event_fast_info.mon;
        second_date.mday = event_ptr->event_fast_info.day;	
#endif
        deadline.year = event_ptr->event_fast_info.end_year;
        deadline.mon= event_ptr->event_fast_info.end_mon;
        deadline.mday= event_ptr->event_fast_info.end_day;
        if (!IsValidDeadline(deadline, second_date))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
            return FALSE;
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : save the select weeks
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:
/*****************************************************************************/
LOCAL uint16 SaveSelectWeeks(
                                               MMI_CTRL_ID_T    ctrl_id ,//in
                                               MMIACC_SMART_EVENT_T*	event_ptr
                                               )
{
	uint16      cur_selection[SCH_WEEK_DAY_NUM]   =  {0};
	uint16      cur_num       =   0;
	uint8   week_day =0;
	int32 loop=0;
// 	uint32      i = 0; 
	uint16      mode = 0;
	
	cur_num=GUILIST_GetSelectedItemIndex(ctrl_id, cur_selection, SCH_WEEK_DAY_NUM);

	if(0==cur_num)//不允许一项也不选，否则保存闹钟时会死机
	{
		return cur_num;
	}

	for(loop=0;loop<cur_num;loop++)
	{
		week_day = MMIAPISET_GetWeekDayByOffset(cur_selection[loop]);
		mode |= s_week_mask[week_day];
	}

	if (mode != event_ptr->event_fast_info.fre) 
	{
		event_ptr->event_fast_info.fre = mode & 0x00ff;
	}
	
	return cur_num;
}

/*****************************************************************************/
// 	Description : view and edit schedule
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditSchWinMsg( 
                                       MMI_WIN_ID_T	    win_id, 
                                       MMI_MESSAGE_ID_E	msg_id, 
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E	            recode          = MMI_RESULT_TRUE;
    MMISCH_DETAIL_WIN_PARAM_T   *win_param      = NULL;
    MMIACC_SMART_EVENT_T        *event          = NULL;
    SCI_DATE_T                  cur_date        = {0};
    SCI_TIME_T                  cur_time        = {0};
    SCI_DATE_T                  second_date     = {0};
    SCI_TIME_T                  second_time     = {0};
    SCI_DATE_T					deadline        = {0};
    GUI_LCD_DEV_INFO			lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // 	GUI_RECT_T                  bk_rect         = MMITHEME_GetClientRect();
    MMI_CTRL_ID_T               ctrl_id         = 0;
    //static GUI_POINT_T          s_last_point = {0};  
    SCI_DATE_T                  start_date        = {0};
    SCI_TIME_T                  start_time        = {0};
    SCI_DATE_T                  min_date        = {0};
    SCI_TIME_T                  min_time        = {0};
    //#ifdef MMI_PDA_SUPPORT
    uint16                      img_width  = 0;
    uint16                      img_height = 0;
    GUIFORM_CHILD_WIDTH_T       button_width = {0};
    GUIFORM_CHILD_HEIGHT_T      button_height = {0};
    //#endif
#ifdef MMI_RECORD_SUPPORT
    GUI_BG_T bg={0};
#endif

    min_date.year =MMICOM_DATE_MIN_YEAR;
    min_date.mon = 1;
    min_date.mday = 1;

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;	

    win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != win_param);/*assert verified*/
    if(PNULL == win_param)
    {
        return recode;
    }
    event = &win_param->event;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:	
//#ifdef MMI_PDA_SUPPORT
        CTRLFORM_SetCircularHandleUpDown(MMISCH_EDIT_WIN_FORM_CTRL_ID,TRUE);
        GUIFORM_SetType(MMISCH_EDIT_WIN_FORM_CTRL_ID,GUIFORM_TYPE_TP);
        //GUIEDIT_SetAlign(MMISCH_DATE_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        //GUIEDIT_SetAlign(MMISCH_TIME_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        //GUIEDIT_SetAlign(MMISCH_DEADLINE_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMISCH_FREQ_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
//#endif
        event->event_fast_info.year = event->event_fast_info.start_year;
        event->event_fast_info.mon  = event->event_fast_info.start_mon;
        event->event_fast_info.day = event->event_fast_info.start_day;
        SetFreqSetList(event);
//#ifdef MMI_PDA_SUPPORT
        GUIFORM_SetStyle(MMISCH_RECORD_RING_FORM_CTRL_ID, GUIFORM_STYLE_UNIT);
        GUIAPICTRL_SetState( MMISCH_RECORD_RING_FORM_CTRL_ID, GUICTRL_STATE_DISABLE_TP,TRUE );

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DROPDOWNLIST_ARROW_PR, win_id);

        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;

        GUIFORM_SetChildWidth(MMISCH_FREQ_FORM_CTRL_ID, MMISCH_FREQ_TYPE_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMISCH_FREQ_FORM_CTRL_ID, MMISCH_FREQ_TYPE_BUTTON_CTRL_ID, &button_height);
#ifndef MMI_RECORD_SUPPORT
        GUIFORM_SetChildWidth(MMISCH_RING_FORM_CTRL_ID, MMISCH_RECORD_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMISCH_RING_FORM_CTRL_ID, MMISCH_RECORD_RING_BUTTON_CTRL_ID, &button_height);
#else
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMG_SCH_RECORD_ICON, win_id);
        button_width.add_data = img_width;
        button_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        button_height.add_data = img_height;
        button_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
        GUIFORM_SetChildWidth(MMISCH_RING_FORM_CTRL_ID, MMISCH_RECORD_RING_BUTTON_CTRL_ID, &button_width);
        GUIFORM_SetChildHeight(MMISCH_RING_FORM_CTRL_ID, MMISCH_RECORD_RING_BUTTON_CTRL_ID, &button_height);

        GUIBUTTON_SetCallBackFunc(MMISCH_RECORD_RING_BUTTON_CTRL_ID, RecordCalendarRingCallBack);
        GUIBUTTON_SetRunSheen(MMISCH_RECORD_RING_BUTTON_CTRL_ID,FALSE);
        bg.bg_type=GUI_BG_IMG;
        bg.img_id=IMG_SCH_RECORD_ICON;
        GUIBUTTON_SetPressedBg(MMISCH_RECORD_RING_BUTTON_CTRL_ID,&bg);
#endif
#ifndef MMI_PDA_SUPPORT
		SetSchEditWinNameParam(event);
		SetSchEditWinDateParam(event);
#ifdef CMCC_UI_STYLE
        SetSchEditWinTextParam(event);
        SetSchEditWinDateStopParam(event);
        SetSchEditWinRemindParam(event);
        GUIEDIT_SetAlign(MMISCH_DATE_STOP_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMISCH_TIME_STOP_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        GUISETLIST_SetFontAlign(MMISCH_REMIND_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#else
		SetSchEditWinTimeParam(event);
#endif        
		SetSchEditWinDeadlineParam(event);
        GUIEDIT_SetAlign(MMISCH_DATE_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMISCH_TIME_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        GUIEDIT_SetAlign(MMISCH_DEADLINE_EDIT_CTRL_ID,ALIGN_LVMIDDLE);
        
#endif
        SetSchEditWinFreqTypeParam();
		SetSchEditWinRingParam(event);
		SetSchEditWinWeeklistParam(event, win_id);
		SetSchEditWinDeadlineDisplay();
		SetSchEditWinWeeklistDisplay();
		MMK_SetAtvCtrl(win_id, MMISCH_NAME_FORM_CTRL_ID);
		break;
	    
	case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        SetSchEditWinNameParam(event);
		SetSchEditWinDateParam(event);
		SetSchEditWinTimeParam(event);
		SetSchEditWinDeadlineParam(event);
         if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMISCH_FREQ_SETLIST_CTRL_ID))
         {
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
            MMI_STRING_T str={0};
            str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,event->event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
            str.wstr_ptr=buff;
            GUISETLIST_SetDispText(MMISCH_FREQ_SETLIST_CTRL_ID,&str,FALSE);
         }
#endif
		break;
	case MSG_GET_FOCUS:
		SetSchEditWinRingParam(event);
		break;
	    
	case MSG_KEYDOWN_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(MMICALENDAR_OPTION_WIN_ID);
		MMK_CloseWin(MMISCH_LIST_OPTION_WIN_ID);
		MMK_CloseWin(MMISCH_ALL_LIST_OPTION_WIN_ID);
		MMK_CloseWin(MMISCH_OPTION_WIN_ID);
		MMK_CloseWin(win_id);
	    	break;
			
	case MSG_CTL_SETLIST_SWITCH:
		SetSchEditWinDeadlineDisplay();
		SetSchEditWinWeeklistDisplay();
#ifdef MMI_PDA_SUPPORT
        SaveSchEditWinFreqMode(event);
#endif
		break;
		
	case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
		ctrl_id = MMK_GetActiveCtrlId(win_id);
		HandleSchEditFormSwitchActiveMsg(win_id,ctrl_id);
		break;
    
	case MSG_CTL_OK:
	case MSG_APP_OK:
#ifndef MMI_PDA_SUPPORT
		if(!SaveSchEditWinName(event))
		{
			break;
		}
		if(!SaveSchEditWinDate(event))
		{
			break;
		}
		if(!SaveSchEditWinTime(event))
		{
			break;
		}
#ifdef CMCC_UI_STYLE
   	    if(!SaveSchEditWinStopDateTime(event))
		{
			break;
		}
		if(!SaveSchEditWinText(event))
		{
			break;
		}
        SaveSchEditWinRemindTime(event);
#endif		
#endif	
		SaveSchEditWinFreqMode(event);
		
		if(event->event_fast_info.fre_mode == ALM_MODE_EVERYWEEK)
		{
			if(!SCHSaveFreEditWin(event))
			{
				break;
			}
		}
		
		if(ALM_MODE_ONCE != event->event_fast_info.fre_mode && ALM_MODE_NO != event->event_fast_info.fre_mode)
		{
#ifndef MMI_PDA_SUPPORT
			if(!SaveSchEditWinDeadlineDate(event))
			{
				break;
			}
#endif
		}
		else
		{
#ifdef CMCC_UI_STYLE
    		event->event_fast_info.end_year = event->event_fast_info.stop_year;
			event->event_fast_info.end_mon = event->event_fast_info.stop_mon;
			event->event_fast_info.end_day = event->event_fast_info.stop_day;
#else            
			event->event_fast_info.end_year = event->event_fast_info.year;
			event->event_fast_info.end_mon = event->event_fast_info.mon;
			event->event_fast_info.end_day = event->event_fast_info.day;
#endif
		}
		
		event->event_fast_info.start_year = event->event_fast_info.year;
		event->event_fast_info.start_mon  = event->event_fast_info.mon;
		event->event_fast_info.start_day = event->event_fast_info.day;		
		TM_GetSysDate(&cur_date);
		TM_GetSysTime(&cur_time);
		event->event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
		second_date.year = event->event_fast_info.year;
		second_date.mon  = event->event_fast_info.mon;
		second_date.mday = event->event_fast_info.day;
		second_time.hour = event->event_fast_info.hour;
		second_time.min = event->event_fast_info.minute;
		second_time.sec = 0;
		start_date.year = event->event_fast_info.start_year;
		start_date.mon = event->event_fast_info.start_mon;
		start_date.mday = event->event_fast_info.start_day;

		if(CMP_FIRST_SMALL == MMIAPIALM_CompareDateAndTime(start_date,start_time,min_date,min_time))
		{
			MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
			break;
		}
		if(ALM_MODE_ONCE == event->event_fast_info.fre_mode)
		{
			// 判断时间是否过期
			if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
			{
				deadline.year = event->event_fast_info.end_year;
				deadline.mon= event->event_fast_info.end_mon;
				deadline.mday= event->event_fast_info.end_day;
				second_date.year = event->event_fast_info.year;
				second_date.mon  = event->event_fast_info.mon;
				second_date.mday = event->event_fast_info.day;			
				if (IsValidDeadline(deadline, second_date))
				{			
					MMK_PostMsg(win_param->win_id, MSG_SCH_EVENT_EDIT_FINISH, PNULL, 0);
					MMK_PostMsg(MMISCH_VIEW_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0);
					MMK_PostMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0);
#ifdef SCH_ASSISTANT_WIDGET
                    if(!MMIAPIWIDGET_GetScheduleWidgetFlag())
                    {   
#ifdef MMI_GRID_IDLE_SUPPORT
                        if(MMISET_IDLE_STYLE_GRID==MMIAPISET_GetIdleStyle())
                        { 
							MMK_CloseWin(win_id);
                            MMIPUB_OpenAlertWarningWin(TXT_SCH_ADD_SCH_ASSIITANT);
                            MMIAPIGRID_AddNewWidget(MMIWIDGET_SCHEDULE_ID);
							
                        }
#endif
#ifdef QBTHEME_SUPPORT
                        //if(MMISET_IDLE_QBTHEME==MMIAPISET_GetIdleStyle())
                        //{
                        //    MMIPUB_OpenAlertWarningWin(TXT_SCH_ADD_SCH_ASSIITANT);
                        //    MMIQBIDLE_AddNewWidget(MMIWIDGET_SCHEDULE_ID);
                        //}
#endif
                        MMIAPIWIDGET_SetScheduleWidgetFlag(TRUE);
                    }
#endif
					if (MMK_IsOpenWin(win_id))
					{
						MMK_CloseWin(win_id);
					}
					
				}
				else
				{
					MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
				}
			}
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
			}
		}
		else if (ALM_MODE_EVERYWEEK == event->event_fast_info.fre_mode)
		{
			uint8  i = 0;
			uint8  offset_day = 0;
// 			uint16 sel_count = 0;

			if (ALM_MODE_EVERYWEEK == event->event_fast_info.fre_mode)
			{
				if (0 == event->event_fast_info.fre)
				{
					MMIPUB_OpenAlertWarningWin(TXT_ALARM_SELECT_ONE_DAY);
					return recode;
				}
			}
            second_date.wday=MMICALENDAR_GetWeekday(second_date.year,second_date.mon,second_date.mday);
			if (MMIAPIALM_CompareDateAndTime(second_date, cur_time, second_date, second_time) <= CMP_FIRST_SMALL)
			{
				offset_day = MMIAPIALM_RoundWeek(second_date.wday, event->event_fast_info.fre, CMP_FIRST_SMALL);
			}
			else
			{
				offset_day = MMIAPIALM_RoundWeek(second_date.wday, event->event_fast_info.fre, CMP_SECOND_SMALL);
			}

			for (i=0; i <offset_day; i++)
			{
				if ((QUERY_YEAR_TOP == second_date.year) && (SCH_DECEMBER == second_date.mon) && (BIG_MONTH_DAY == second_date.mday))
				{
					break;
				}
				MMIAPIALM_IncreaseDateByOne(&second_date);
			}
			// 计算后的日期
			event->event_fast_info.day = second_date.mday;
			event->event_fast_info.year = second_date.year;
			event->event_fast_info.mon = second_date.mon;
			
			event->event_fast_info.start_year = event->event_fast_info.year;
			event->event_fast_info.start_mon  = event->event_fast_info.mon;
			event->event_fast_info.start_day = event->event_fast_info.day;
			// 比较的日期
			second_date.year = event->event_fast_info.year;
			second_date.mon  = event->event_fast_info.mon;
			second_date.mday = event->event_fast_info.day;
			second_time.hour = event->event_fast_info.hour;
			second_time.min = event->event_fast_info.minute;
			// 判断时间是否过期
			TM_GetSysDate(&cur_date); 
			if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
			{
				deadline.year = event->event_fast_info.end_year;
				deadline.mon= event->event_fast_info.end_mon;
				deadline.mday= event->event_fast_info.end_day;
				second_date.year = event->event_fast_info.year;
				second_date.mon  = event->event_fast_info.mon;
				second_date.mday = event->event_fast_info.day;			
				if (IsValidDeadline(deadline, second_date))
				{			
					MMK_PostMsg(win_param->win_id, MSG_SCH_EVENT_EDIT_FINISH, PNULL, 0);
					MMK_PostMsg(MMISCH_VIEW_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0); 
					MMK_PostMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0);
#ifdef SCH_ASSISTANT_WIDGET
                    if(!MMIAPIWIDGET_GetScheduleWidgetFlag())
                    {   

#ifdef MMI_GRID_IDLE_SUPPORT
                        if(MMISET_IDLE_STYLE_GRID==MMIAPISET_GetIdleStyle())
                        {
							MMK_CloseWin(win_id);
                            MMIPUB_OpenAlertWarningWin(TXT_SCH_ADD_SCH_ASSIITANT);
                            MMIAPIGRID_AddNewWidget(MMIWIDGET_SCHEDULE_ID);
                        }
#endif
#ifdef QBTHEME_SUPPORT
                        //if(MMISET_IDLE_QBTHEME==MMIAPISET_GetIdleStyle())
                        //{
                        //    MMIPUB_OpenAlertWarningWin(TXT_SCH_ADD_SCH_ASSIITANT);
                        //    MMIQBIDLE_AddNewWidget(MMIWIDGET_SCHEDULE_ID);
                        //}
#endif
                        MMIAPIWIDGET_SetScheduleWidgetFlag(TRUE);
                    }
#endif

					if (MMK_IsOpenWin(win_id))
					{
						MMK_CloseWin(win_id);
					}
				}
				else
				{
					MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
				}
			}
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
			}
		}
		else
		{
			deadline.year = event->event_fast_info.end_year;
			deadline.mon= event->event_fast_info.end_mon;
			deadline.mday= event->event_fast_info.end_day;
			second_date.year = event->event_fast_info.year;
			second_date.mon  = event->event_fast_info.mon;
			second_date.mday = event->event_fast_info.day;			
			if (CMP_SECOND_SMALL == CompareDate(deadline, second_date))
			{	
				MMK_PostMsg(win_param->win_id, MSG_SCH_EVENT_EDIT_FINISH, PNULL, 0);
				MMK_PostMsg(MMISCH_VIEW_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0);
				MMK_PostMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0);
				MMK_CloseWin(win_id);
			}
			else if (CMP_EQUAL == CompareDate(deadline, second_date))
			{
    			if(MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) == CMP_FIRST_SMALL)
    			{
    				MMK_PostMsg(win_param->win_id, MSG_SCH_EVENT_EDIT_FINISH, PNULL, 0);
    				MMK_PostMsg(MMISCH_VIEW_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0);
    				MMK_PostMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL, 0);
#ifdef SCH_ASSISTANT_WIDGET
                    if(!MMIAPIWIDGET_GetScheduleWidgetFlag())
                    {   
#ifdef MMIWIDGET_SUPPORT                      
                     // MMIAPIWIDGET_AddDynamicItem(MMMIWIDGET_CTRL_IDLE_ID,
                      //                  MMIWIDGET_SCHEDULE_ID,FALSE,FALSE);
#endif
#ifdef MMI_GRID_IDLE_SUPPORT
                        if(MMISET_IDLE_STYLE_GRID==MMIAPISET_GetIdleStyle())
                        {
							MMK_CloseWin(win_id);
                            MMIPUB_OpenAlertWarningWin(TXT_SCH_ADD_SCH_ASSIITANT);
                            MMIAPIGRID_AddNewWidget(MMIWIDGET_SCHEDULE_ID);
                        }
#endif
#ifdef QBTHEME_SUPPORT
                        //if(MMISET_IDLE_QBTHEME==MMIAPISET_GetIdleStyle())
                        //{
                        //    MMIPUB_OpenAlertWarningWin(TXT_SCH_ADD_SCH_ASSIITANT);
                        //    MMIQBIDLE_AddNewWidget(MMIWIDGET_SCHEDULE_ID);
                        //}
#endif

                        MMIAPIWIDGET_SetScheduleWidgetFlag(TRUE);
                    }
#endif
					if (MMK_IsOpenWin(win_id))
					{
						MMK_CloseWin(win_id);
					}
    			}                
    			else
    			{
    				MMIPUB_OpenAlertWarningWin(TXT_DATE_IS_PAST);
    			}
			}
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_SCH_INVALID_DEADLINE);
			}
		}
#if (defined FM_SUPPORT) && (defined MMI_FM_NEED_HEADSET)//cr132759
		if(ALM_FM_RING == event->event_content_info.ring_type)
		{
			MMIPUB_OpenAlertWarningWin(TXT_FM_TIMER_SAVE_PROMPT);
        }
#endif
		break;
		
	case MSG_CTL_MIDSK:          
	case MSG_APP_WEB:
		//HandleSchEditWinWebKeyMsg(win_id, win_param);+
        ctrl_id= MMK_GetActiveCtrlId(win_id);
        HandleSCHEditPENOKMsg(win_id,ctrl_id,win_param);
		break;
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
		ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
		HandleSCHEditPENOKMsg(win_id,ctrl_id,win_param);
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	case MMI_MSG_RECORDTORING:
        {
			MMI_CTRL_ID_T       label_ctrl_id = MMISCH_RING_LABEL_CTRL_ID;
			MMI_STRING_T        ring_name = {0};
			if(PNULL != param)
			{       
                ring_name.wstr_ptr = (wchar*)param;
                ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
                SCI_MEMSET(event->event_ring_info.dring_name_arr,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
                event->event_ring_info.dring_name_len = ring_name.wstr_len;
                MMIAPICOM_Wstrncpy(event->event_ring_info.dring_name_arr, ring_name.wstr_ptr, ring_name.wstr_len);
                SCI_MEMSET(ring_name.wstr_ptr,0,ring_name.wstr_len * sizeof(wchar));
                
                MMIAPIFMM_SplitFullPath(event->event_ring_info.dring_name_arr, event->event_ring_info.dring_name_len, PNULL, PNULL, PNULL, PNULL, ring_name.wstr_ptr, &ring_name.wstr_len);

                GUILABEL_SetText(label_ctrl_id, &ring_name, FALSE);                
                event->event_content_info.ring_type = ALM_RECORD_RING;
			}
            MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
		}
	    break;
	    
	case MSG_CLOSE_WINDOW:
#ifndef MMI_GUI_STYLE_MINISCREEN
		if(MMK_IsOpenWin(MMISCH_VIEW_DETAIL_WIN_ID))
		{
        	MMK_CloseWin(MMISCH_VIEW_DETAIL_WIN_ID);
		}
#endif
        break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
    }
    
    return recode;
}


/*****************************************************************************/
// 	Description : handle SCH weeks list windows messages
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSCHSelectweeksWinMsg(
                                          MMI_WIN_ID_T      win_id, 	
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E    recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     =   MMISCH_EDITWIN_WEEKS_LIST_CTRL_ID;
    MMISCH_DETAIL_WIN_PARAM_T       *win_param = NULL;
    MMIACC_SMART_EVENT_T            *event_ptr = NULL;
    win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != win_param);/*assert verified*/
    if(PNULL == win_param)
    {
        return recode;
    }
    
    event_ptr = &win_param->event;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:                   
        GUILIST_SetMaxItem(ctrl_id, SCH_WEEK_DAY_NUM, FALSE);
        GUILIST_SetMaxSelectedItem(ctrl_id, SCH_WEEK_DAY_NUM);
        AppendWeeksListItem(ctrl_id,event_ptr);
        MMK_SetAtvCtrl(win_id,ctrl_id);
            break; 
			
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
         break;
	    
    case MSG_CTL_CANCEL:
        if(0==SaveSelectWeeks(ctrl_id,event_ptr))
        {
            MMIPUB_OpenAlertFailWin(TXT_ALARM_SELECT_ONE_DAY);
        }
        else
        {
            MMK_CloseWin( win_id );   
        }      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
// 	Description : set schedule detail win
//	Global resource dependence : 
//  Author:  yongwei.he
//	Note:   
/*****************************************************************************/
LOCAL void InitViewSchDetail_se( 
                                MMIACC_SMART_EVENT_T*	event_ptr,
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id
                                )
{
    MMI_STRING_T        name_str = {0};
    MMI_STRING_T        content_str = {0};
    uint8			    temp_str[MMISET_DATE_STR_LEN] = {0};
    wchar               wchar_temp_str[MMISET_DATE_STR_LEN + MMISET_TIME_STR_12HOURS_LEN + 2] = {0};
    uint16                 wchar_temp_len = 0;
    wchar               wchar_str[MMISET_DATE_STR_LEN + 1] = {0};
    MMI_TEXT_ID_T       text_id = TXT_NULL;
    MMI_STRING_T        ring = {0};
    wchar               file_name[MMIFILE_FILE_NAME_MAX_LEN+1]  = {0};        
    uint16              file_name_len = 0;
    uint16              nIndex = 0;

    SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
   
    //name:
    name_str.wstr_ptr = event_ptr->event_content_info.w_content;
    name_str.wstr_len = event_ptr->event_content_info.w_length;  
    if(name_str.wstr_len == 0)
    {
        MMI_GetLabelTextByLang(TXT_BLANK, &name_str);
    }
    AddOneDetailItem(ctrl_id,TXT_CALENDAR_NAME,PNULL,&nIndex,name_str.wstr_ptr,name_str.wstr_len);	

    //content:
    content_str.wstr_ptr = event_ptr->event_content_info.w_text;
    content_str.wstr_len = event_ptr->event_content_info.w_text_length;  
    if(content_str.wstr_len == 0)
    {
        MMI_GetLabelTextByLang(TXT_BLANK, &content_str);
    }
    AddOneDetailItem(ctrl_id,TXT_SCH_TITLE_CONTENT,PNULL,&nIndex,content_str.wstr_ptr,content_str.wstr_len);	

    //date:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));   
    MMIAPISET_FormatDateStrByDateStyle(event_ptr->event_fast_info.start_year, 
        event_ptr->event_fast_info.start_mon, event_ptr->event_fast_info.start_day,
        '.',temp_str,MMISET_DATE_STR_LEN);

    MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
    wchar_temp_len = (uint16)strlen((char*)temp_str);
    wchar_temp_str[wchar_temp_len] = 0x0020;
    wchar_temp_len++;

    //time:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_str,0,sizeof(wchar_str));    
	MMIAPISET_FormatTimeStrByTime(event_ptr->event_fast_info.hour,event_ptr->event_fast_info.minute,temp_str,MMISET_DATE_STR_LEN);
    MMIAPICOM_StrToWstr(temp_str,wchar_str );

    MMI_WSTRNCPY(
        &wchar_temp_str[wchar_temp_len] , 
        MMISET_DATE_STR_LEN + MMISET_TIME_STR_12HOURS_LEN + 2-wchar_temp_len, 
        wchar_str, 
        (uint16)strlen((char*)temp_str), 
        (uint16)strlen((char*)temp_str));
    wchar_temp_len += (uint16)strlen((char*)temp_str);
    
    AddOneDetailItem(ctrl_id,TXT_COMMON_START,PNULL,&nIndex,wchar_temp_str,wchar_temp_len);    
    
    //stop date:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));   
    MMIAPISET_FormatDateStrByDateStyle(event_ptr->event_fast_info.stop_year, 
        event_ptr->event_fast_info.stop_mon, event_ptr->event_fast_info.stop_day,
        '.',temp_str,MMISET_DATE_STR_LEN);

    MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
    wchar_temp_len = (uint16)strlen((char*)temp_str);
    wchar_temp_str[wchar_temp_len] = 0x0020;
    wchar_temp_len++;

    //stop time:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_str,0,sizeof(wchar_str));    
	MMIAPISET_FormatTimeStrByTime(event_ptr->event_fast_info.stop_hour,event_ptr->event_fast_info.stop_minute,temp_str,MMISET_DATE_STR_LEN);
    MMIAPICOM_StrToWstr(temp_str,wchar_str );

    MMI_WSTRNCPY(
        &wchar_temp_str[wchar_temp_len] , 
        MMISET_DATE_STR_LEN + MMISET_TIME_STR_12HOURS_LEN + 2-wchar_temp_len, 
        wchar_str, 
        (uint16)strlen((char*)temp_str), 
        (uint16)strlen((char*)temp_str));
    wchar_temp_len += (uint16)strlen((char*)temp_str);
    
    AddOneDetailItem(ctrl_id,STXT_STOP,PNULL,&nIndex,wchar_temp_str,wchar_temp_len);    
    
    //ring:
   //if sch ring is download ring, check whether file exists
    if(ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type &&
        !MMIFILE_IsFileExist(event_ptr->event_ring_info.dring_name_arr,
        event_ptr->event_ring_info.dring_name_len))
    {
        event_ptr->event_content_info.ring_type = ALM_FIX_RING;
        event_ptr->event_content_info.ring = 1;
    }	    
    //@zhaohui,cr112262,应只有ALM_DOWNLOAD_RING和ALM_FIX_RING两种情况
    if(ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type)
    {        
        MMIAPIFMM_SplitFullPath(event_ptr->event_ring_info.dring_name_arr, event_ptr->event_ring_info.dring_name_len,
            NULL, NULL, NULL, NULL,
            file_name, &file_name_len);
        
        ring.wstr_ptr = file_name;  	
        ring.wstr_len = file_name_len;
    }
    else//ALM_FIX_RING
    {
        uint16 fixed_ringid = event_ptr->event_content_info.ring;
        ring.wstr_len = MMIAPISET_GetRingName(fixed_ringid, file_name,MMIFILE_FILE_NAME_MAX_LEN);
        ring.wstr_ptr = file_name;
    }
    AddOneDetailItem(ctrl_id,TXT_CALENDAR_RING,PNULL,&nIndex,ring.wstr_ptr,ring.wstr_len);  
    
    //freq:
    if(FALSE == event_ptr->event_fast_info.is_on)
    {
        text_id = TXT_SCH_AWAKE_NO;

    }
    else
    {
        switch(event_ptr->event_fast_info.fre_mode)
        {
        case ALM_MODE_ONCE:
            text_id = TXT_ALARM_MODE_ONCE;

            break;
            
        case ALM_MODE_EVERYDAY:
            text_id = TXT_SCH_AWAKE_EVERYDAY;

            break;
            
        case ALM_MODE_EVERYWEEK:
            text_id = TXT_SCH_AWAKE_EVERYWEEK;

            break;
            
        case ALM_MODE_EVERYMONTH:
            text_id = TXT_SCH_AWAKE_EVERYMONTH;

            break;
            
        case ALM_MODE_EVERYYEAR:
            text_id = TXT_SCH_AWAKE_EVERYYEAR;

            break;
            
        default:
            break;
        }
    }   
    AddOneDetailItem(ctrl_id,TXT_TYPE,text_id,&nIndex,PNULL,0); 
    
    //deadline:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));   
    MMIAPISET_FormatDateStrByDateStyle(event_ptr->event_fast_info.end_year, 
        event_ptr->event_fast_info.end_mon, event_ptr->event_fast_info.end_day,
        '.',temp_str,MMISET_DATE_STR_LEN);

    MMIAPICOM_StrToWstr( temp_str,wchar_temp_str );
    AddOneDetailItem(ctrl_id,TXT_CALENDAR_DEADLINE,PNULL,&nIndex,wchar_temp_str,(uint16)strlen((char*)temp_str));  

    {
        MMI_TEXT_ID_T text_arr[6] = {
                                    TXT_SCH_REMIND_ONTIME,
                                    TXT_SCH_REMIND_FIVE_MIN,
                                    TXT_SCH_REMIND_FIFTEEN_MIN,
                                    TXT_SCH_REMIND_ONE_HOUR,
                                    TXT_SCH_REMIND_ONE_DAY,
                                    TXT_SCH_REMIND_ONE_WEEK
                                    };
        
        if(event_ptr->event_fast_info.remind_type > MMISCH_REMIND_ONE_WEEK)
        {
            event_ptr->event_fast_info.remind_type = MMISCH_REMIND_ONTIME;
        }
        
        AddOneDetailItem(ctrl_id,TXT_SCH_AWAKE_TIME,text_arr[event_ptr->event_fast_info.remind_type],&nIndex,PNULL,0); 
    }
}
#else
/*****************************************************************************/
// 	Description : set schedule detail win
//	Global resource dependence : 
//  Author:  yongwei.he
//	Note:   
/*****************************************************************************/
LOCAL void InitViewSchDetail_se( 
                                MMIACC_SMART_EVENT_T*	event_ptr,
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id
                                )
{
    MMI_STRING_T        name_str = {0};
    uint8			    temp_str[MMISET_DATE_STR_LEN] = {0};
    wchar               wchar_temp_str[MMISET_DATE_STR_LEN +1] = {0};
    MMI_TEXT_ID_T       text_id = TXT_NULL;
    MMI_STRING_T        ring = {0};
    wchar               file_name[MMIFILE_FILE_NAME_MAX_LEN+1]  = {0};        
    uint16              file_name_len = 0;
    uint16              nIndex = 0;

    //SCI_ASSERT(event_ptr != PNULL);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return;
    }
    ring.wstr_ptr = file_name; //CR132759 
    //name:
    name_str.wstr_ptr = event_ptr->event_content_info.w_content;
    name_str.wstr_len = event_ptr->event_content_info.w_length;  
    AddOneDetailItem(ctrl_id,TXT_CALENDAR_NAME,PNULL,&nIndex,name_str.wstr_ptr,name_str.wstr_len);	
    
    //date:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));
#ifdef MMI_GUI_STYLE_MINISCREEN
    MMIAPISET_FormatDateStrByDateStyle(event_ptr->event_fast_info.start_year,
        event_ptr->event_fast_info.start_mon, event_ptr->event_fast_info.start_day,
        '/',temp_str,MMISET_DATE_STR_LEN);
#else
    MMIAPISET_FormatDateStrByDateStyle(event_ptr->event_fast_info.start_year,
        event_ptr->event_fast_info.start_mon, event_ptr->event_fast_info.start_day,
        '.',temp_str,MMISET_DATE_STR_LEN);
#endif

    MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
    AddOneDetailItem(ctrl_id,TXT_COMMON_DATE,PNULL,&nIndex,wchar_temp_str,(uint16)strlen((char*)temp_str));    
    
    //time:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));    
	MMIAPISET_FormatTimeStrByTime(event_ptr->event_fast_info.hour,event_ptr->event_fast_info.minute,temp_str,MMISET_DATE_STR_LEN);
    MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
    AddOneDetailItem(ctrl_id,TXT_COMMON_TIME,PNULL,&nIndex,wchar_temp_str,(uint16)strlen((char*)temp_str));  
    
    //ring:
   //if sch ring is download ring, check whether file exists
    if((ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type||ALM_RECORD_RING == event_ptr->event_content_info.ring_type) 
        &&!MMIAPIFMM_IsFileExist(event_ptr->event_ring_info.dring_name_arr,
        event_ptr->event_ring_info.dring_name_len))
    {
        event_ptr->event_content_info.ring_type = ALM_FIX_RING;
        event_ptr->event_content_info.ring = 1;
    }	    
    //@zhaohui,cr112262,应只有ALM_DOWNLOAD_RING和ALM_FIX_RING两种情况
    if(ALM_DOWNLOAD_RING == event_ptr->event_content_info.ring_type
        ||ALM_RECORD_RING == event_ptr->event_content_info.ring_type)
    {        
        MMIAPIFMM_SplitFullPath(event_ptr->event_ring_info.dring_name_arr, event_ptr->event_ring_info.dring_name_len,
            NULL, NULL, NULL, NULL,
            file_name, &file_name_len);
        
        ring.wstr_ptr = file_name;  	
        ring.wstr_len = file_name_len;
    }
#ifdef FM_SUPPORT//CR132759
	else if(ALM_FM_RING== event_ptr->event_content_info.ring_type)
	{
		wchar fm[3]={'F','M',0};
		MMI_STRING_T ring_str={0};	
		wchar*channel_name = PNULL;
		
		ring_str.wstr_ptr = event_ptr->event_ring_info.dring_name_arr;
        ring_str.wstr_len = event_ptr->event_ring_info.dring_name_len;
		channel_name = MMIAPIFM_GetChannelName(MMIAPIALM_ConvertFloatStrToInt(&ring_str,10));
		
		if(PNULL!=channel_name)
		{
			ring.wstr_len = MMIAPICOM_Wstrlen(channel_name);
			ring.wstr_ptr = channel_name; 
        }
		else
		{
            MMIAPICOM_Wstrncpy(ring.wstr_ptr, fm, 2);
            MMIAPICOM_Wstrcat(ring.wstr_ptr, ring_str.wstr_ptr);
            ring.wstr_len=MMIAPICOM_Wstrlen(ring.wstr_ptr);
        }
	}
#endif
    else//ALM_FIX_RING
    {
        uint16 fixed_ringid = event_ptr->event_content_info.ring;
        ring.wstr_len = MMIAPISET_GetRingName(fixed_ringid, file_name,MMIFILE_FILE_NAME_MAX_LEN);
        ring.wstr_ptr = file_name;
    }
    AddOneDetailItem(ctrl_id,TXT_CALENDAR_RING,PNULL,&nIndex,ring.wstr_ptr,ring.wstr_len);  
    
    //freq:
    if(FALSE == event_ptr->event_fast_info.is_on)
    {
        text_id = TXT_SCH_AWAKE_NO;

    }
    else
    {
        switch(event_ptr->event_fast_info.fre_mode)
        {
        case ALM_MODE_ONCE:
            text_id = TXT_ALARM_MODE_ONCE;

            break;
            
        case ALM_MODE_EVERYDAY:
            text_id = TXT_SCH_AWAKE_EVERYDAY;

            break;
            
        case ALM_MODE_EVERYWEEK:
            text_id = TXT_SCH_AWAKE_EVERYWEEK;

            break;
            
        case ALM_MODE_EVERYMONTH:
            text_id = TXT_SCH_AWAKE_EVERYMONTH;

            break;
            
        case ALM_MODE_EVERYYEAR:
            text_id = TXT_SCH_AWAKE_EVERYYEAR;

            break;
            
        default:
            break;
        }
    }   
    AddOneDetailItem(ctrl_id,TXT_TYPE,text_id,&nIndex,PNULL,0); 
    
    //deadline:
    SCI_MEMSET(temp_str,0,sizeof(temp_str));
    SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));   
    MMIAPISET_FormatDateStrByDateStyle(event_ptr->event_fast_info.end_year, 
        event_ptr->event_fast_info.end_mon, event_ptr->event_fast_info.end_day,
        '.',temp_str,MMISET_DATE_STR_LEN);

    MMIAPICOM_StrToWstr( temp_str,wchar_temp_str );
    AddOneDetailItem(ctrl_id,TXT_CALENDAR_DEADLINE,PNULL,&nIndex,wchar_temp_str,(uint16)strlen((char*)temp_str));  
}
#endif

/*****************************************************************************/
// 	Description : view and edit schedule
//	Global resource dependence : 
//  Author:  xiangjie
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewSchDetailWinMsg( 
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    uint32 i                                    = 0;
//     uint16 index                                = 0;
    MMI_RESULT_E	            recode          = MMI_RESULT_TRUE;
static   MMISCH_DETAIL_WIN_PARAM_T  *s_win_param = PNULL;
#ifndef MMI_GUI_STYLE_MINISCREEN
static    MMISCH_DETAIL_WIN_PARAM_T   *s_pass_win_param = PNULL;
#endif
//     GUI_LCD_DEV_INFO	        lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

#ifdef MMI_GUI_STYLE_MINISCREEN
static     MMISCH_DETAIL_WIN_PARAM_T    *win_param = PNULL;
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
        if(PNULL == s_win_param)
        {
        return FALSE;
        }
#ifdef MMI_GUI_STYLE_MINISCREEN
        win_param = s_win_param;
#endif
        MMK_SetAtvCtrl(win_id, MMISCH_VIEW_DETAIL_TEXT_CTRL_ID);
        InitViewSchDetail_se(&(s_win_param->event), win_id,MMISCH_VIEW_DETAIL_TEXT_CTRL_ID);
        GUIRICHTEXT_SetFocusMode(MMISCH_VIEW_DETAIL_TEXT_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_NONE);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;		
        
    case MSG_APP_OK:	
    case MSG_CTL_OK:
	case MSG_APP_MENU:
#ifdef MMI_GUI_STYLE_MINISCREEN
        win_param->win_id = win_id;
        MMK_CreateWin((uint32 *)SCH_OPTION_WIN_TAB, (ADD_DATA)win_param);
#else
       s_pass_win_param = s_win_param;
       s_pass_win_param->win_id = win_id;
       MMK_CreateWin((uint32 *)SCH_OPTION_WIN_TAB,(ADD_DATA)s_pass_win_param);
#endif
        break;

    case MSG_APP_WEB:
#ifdef MMI_GUI_STYLE_MINISCREEN
    case MSG_CTL_MIDSK:
       win_param->win_id = win_id;
       MMK_CreateWin((uint32 *)SCH_EDIT_WIN_TAB, (ADD_DATA)win_param);
#endif
        break;

#ifdef MMI_GUI_STYLE_MINISCREEN
    case MSG_SCH_EVENT_EDIT_FINISH:
            if (MMIAPIALM_SaveInteEventInfo((uint16)(win_param->index),&(win_param->event)))
            {
            }
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
			}
            MMK_CloseWin(win_id);
            break;
#endif

    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(MMISCH_LIST_OPTION_WIN_ID);
        MMK_CloseWin(MMISCH_ALL_LIST_OPTION_WIN_ID);
        break;
        
    case MSG_APP_LEFT:
        i = 0;
        MMK_SendMsg(win_id, MSG_SK_SKB_ICON_PRESSED, (DPARAM)&i);
        break;
        
    case MSG_APP_RIGHT:
        i = 2;
        MMK_SendMsg(win_id, MSG_SK_SKB_ICON_PRESSED, (DPARAM)&i);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
// 	Description : init a schedule record
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISCH_InitNewSch(
                       uint16                   year,
                       uint8                    month,
                       uint8                    day,
                       MMIACC_SMART_EVENT_T     *event_ptr
                       )
{
    SCI_TIME_T cur_time = {0};
    SCI_DATE_T date = {0};  
    //SCI_ASSERT(NULL != event_ptr);/*assert verified*/
    if(PNULL == event_ptr)
    {
        return;
    }
    
    TM_GetSysTime(&cur_time);
    
    event_ptr->event_fast_info.is_valid = TRUE;
    event_ptr->event_fast_info.is_on = TRUE;

    date.year = year;
    date.mon = month;
    date.mday = day;

    //新建日程时,默认时间是当前时间+1小时
    if (MMICOM_24HOUR_MAX_HOUR-1 == cur_time.hour )
    {
        cur_time.hour = 0;

        MMIAPIALM_IncreaseDateByOne(&date);//date ++
    }
    else
    {
        cur_time.hour ++;
    }

    event_ptr->event_fast_info.start_year = date.year;
    event_ptr->event_fast_info.start_mon = date.mon;
    event_ptr->event_fast_info.start_day = date.mday; 
    event_ptr->event_fast_info.year = date.year;
    event_ptr->event_fast_info.mon = date.mon;
    event_ptr->event_fast_info.day = date.mday;
    event_ptr->event_fast_info.end_year = date.year;
    event_ptr->event_fast_info.end_mon = date.mon;
    event_ptr->event_fast_info.end_day = date.mday;
    event_ptr->event_fast_info.fre = 0x3e; // 表示周一到周五选中
    
    event_ptr->event_fast_info.hour = cur_time.hour;
    event_ptr->event_fast_info.minute = cur_time.min;
    
    event_ptr->event_fast_info.type = SMART_EVENT_SCH;
    event_ptr->event_fast_info.fre_mode = ALM_MODE_ONCE;
    
    event_ptr->event_content_info.w_length = 0;
    
    event_ptr->event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
    
    event_ptr->event_content_info.ring_type = ALM_FIX_RING;
    event_ptr->event_content_info.ring = 1;
    
    return;
} 

#if 0
/*****************************************************************************/
// 	Description : edit schedule text content
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchTextEditorWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E                    recode = MMI_RESULT_TRUE;
    MMI_STRING_T          str = {0}; 
    MMI_CTRL_ID_T                   current_ctrl_id = MMIALM_TEXTEDIT_CTRL_ID;
    MMISCH_DETAIL_WIN_PARAM_T       *win_param = NULL;
    MMIACC_SMART_EVENT_T            *event_ptr = NULL;
    
    win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != win_param);/*assert verified*/
    if(PNULL == win_param)
    {
        return  recode;
    }
    event_ptr = &win_param->event;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIEDIT_SetString(
            current_ctrl_id,
            event_ptr->event_content_info.w_content,
            event_ptr->event_content_info.w_length
            );
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;
    
    case MSG_APP_OK:
    case MSG_CTL_OK:        
	case MSG_CTL_MIDSK:
        // get string info
        GUIEDIT_GetString(current_ctrl_id, &str);
        if (MMIAPISET_IsIncludeReturnChar(str.wstr_ptr,str.wstr_len))
        {
            MMIPUB_OpenAlertFailWin(TXT_SCH_INVALID_TITLE);
            break;
        }	        
        event_ptr->event_content_info.w_length = str.wstr_len;
  
	  	if (!str.wstr_len)//if str.str_len is 0,should clean event_ptr->content
		{
			str.wstr_len = MMIACC_EVENT_TEXT_MAX_LEN;
		}
        
        MMI_WSTRNCPY(
            event_ptr->event_content_info.w_content, MMIACC_EVENT_TEXT_MAX_LEN,
            str.wstr_ptr, str.wstr_len,
            str.wstr_len);
        
        MMK_CloseWin(win_id);
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif
/*****************************************************************************/
// 	Description : CompareDateTime
//         compare a date to another , find smaller(earlier)
//	Global resource dependence : 
//  Author:        xiangjie
//	Note:   
/*****************************************************************************/
LOCAL uint16 CompareDate(
                         SCI_DATE_T   first, 
                         SCI_DATE_T   second
                         )
{
    ALM_CMP_E result = CMP_EQUAL;
    
    if(first.year > second.year)
    {
        result = CMP_SECOND_SMALL;
    }
    else if(first.year < second.year)
    {
        result = CMP_FIRST_SMALL;
    }
    else
    {
        if(first.mon> second.mon)
        {
            result = CMP_SECOND_SMALL;
        }
        else if(first.mon < second.mon)
        {
            result = CMP_FIRST_SMALL;
        }
        else
        {
            if(first.mday > second.mday)
            {
                result = CMP_SECOND_SMALL;
            }
            else if(first.mday < second.mday)
            {
                result = CMP_FIRST_SMALL;
            }	
            else
            {
                result = CMP_EQUAL;
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : if the deadline is earlier then the date ,it is not a valid deadline
//	Global resource dependence : 
//  Author:        xiangjie
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN IsValidDeadline(
                              SCI_DATE_T   deadline, 
                              SCI_DATE_T   date
                              )
{
    BOOLEAN	result = TRUE;
    
    if (CMP_FIRST_SMALL == CompareDate(deadline, date))
    {
        result = FALSE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : schedule option win
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchOptionWinMsg(
                                         MMI_WIN_ID_T		win_id, 
                                         MMI_MESSAGE_ID_E	msg_id, 
                                         DPARAM				param
                                         )
{
    MMI_RESULT_E                        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       current_ctrl_id = MMISCH_OPTION_MENU_CTRL_ID;
    static MMISCH_DETAIL_WIN_PARAM_T    *win_param = NULL;
    MMI_MENU_GROUP_ID_T	                group_id = 0; 
    MMI_MENU_ID_T		                menu_id  = 0;
	static uint32 *to_date_param=PNULL;
    win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    if(PNULL == win_param)
    {
        return  recode;
    }   
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		{//CR163242
			MMI_STRING_T kstring = {0};
			MMI_GetLabelTextByLang(TXT_SEND, &kstring);
			GUIMENU_SetMenuTitle(&kstring,MMISCH_OPTION_MENU_CTRL_ID);
		}
        //SCI_ASSERT(NULL != win_param);/*assert verified*/
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        GUIMENU_GetId(current_ctrl_id, &group_id, &menu_id);
        switch( menu_id)
        {
        case ID_SCH_EDIT:
            win_param->win_id = win_id;
            MMK_CreateWin((uint32 *)SCH_EDIT_WIN_TAB, (ADD_DATA)win_param);
            break;
            
        case ID_SCH_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        /*case ID_SCH_TO_DATE:
			if(PNULL!=to_date_param)
			{
				SCI_FREE(to_date_param);
				to_date_param=PNULL;
			}
			to_date_param=SCI_ALLOC_APP(sizeof(uint32));
			*to_date_param=CALENDAR_TO_DATE_CALENDAR;
            MMIAPICALEND_OpenCalendarQueryByDate((ADD_DATA)to_date_param);
            break;*/
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_VCALENDAR_SUPPORT
    case ID_SCH_VCALENDAR_EXPORT:
        {
            MMI_CTRL_ID_T list_ctrl_id=MMISCH_VIEW_LIST_CTRL_ID;
            SCI_MEMSET(&s_operate_sch_info,0,sizeof(MMISCH_OPERATE_RECORDS_INFO_T));
            s_operate_sch_info.total_count=1;
            s_operate_sch_info.event_id[0] = win_param->index;
            if(MMK_IsOpenWin(MMISCH_VIEW_WIN_ID))
            {
                list_ctrl_id=MMISCH_VIEW_LIST_CTRL_ID;
            }
            else if(MMK_IsOpenWin(MMISCH_VIEW_ALL_WIN_ID))
            {
                list_ctrl_id=MMISCH_VIEW_ALL_LIST_CTRL_ID;
            }
            SelectVcalendarExportFolder(list_ctrl_id);

            MMK_CloseWin(win_id);
        }
        break;
   case ID_SCH_LIST_SEND_SMS:
            SendVcalendarBySMS(win_param->index);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_MMS:
            SendVcalendarByMMS(win_param->index);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_BT:
            {
                //uint16 _event_id[MMIACC_SCH_NUM]={0};
                //int i=0;
                //int event_num = 0;
                //event_num=GUILIST_GetSelectedItemIndex(MMISCH_VIEW_LIST_CTRL_ID, win_param->index, MMIACC_SCH_NUM);
                //for(i=0;i<event_num;i++)
                //{
                //    _event_id[i] = MapListItemToUserItem(win_param->index);
                //}
                uint16 _event_id[1]={0};
                _event_id[0]=win_param->index;
                SendVcalendarByBT(_event_id,1); 
                MMK_CloseWin(win_id);
            }
            break;
        case ID_SCH_LIST_SEND_EMAIL:
            MMK_CloseWin(win_id);
            break;     
        
#endif
#endif
            
        default:
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_PROMPTWIN_OK: 
            MMIPUB_CloseQuerytWin(PNULL);
            if(MMIALM_DelOneEvent((uint16)(win_param->index)))
            {
                MMK_CloseWin(win_param->win_id);
                MMK_CloseWin(MMISCH_VIEW_DETAIL_WIN_ID);
                MMK_CloseWin(MMISCH_LIST_OPTION_WIN_ID);
                MMK_CloseWin(MMISCH_ALL_LIST_OPTION_WIN_ID);
//#ifndef  MMI_PDA_SUPPORT
//                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
                if (MMK_IsOpenWin(MMISCH_VIEW_ALL_WIN_ID))
                {
                    MMK_PostMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL,0);
                }
                if (MMK_IsOpenWin(MMISCH_VIEW_WIN_ID))
                {
                    MMK_PostMsg(MMISCH_VIEW_WIN_ID, MSG_SCH_LIST_UPDATE, PNULL,0);
                }
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMK_CloseWin(win_id);
            break;
            
        case  MSG_PROMPTWIN_CANCEL: //MSG_ACC_MSGBOX_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;
            
        case MSG_SCH_EVENT_EDIT_FINISH:
            if (MMIAPIALM_SaveInteEventInfo((uint16)(win_param->index),&(win_param->event)))
            {
//#ifndef  MMI_PDA_SUPPORT
//                MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
            }  
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
			} 
#ifdef MMI_GUI_STYLE_MINISCREEN
		    if(MMK_IsOpenWin(MMISCH_VIEW_DETAIL_WIN_ID))
		    {
        	    MMK_CloseWin(MMISCH_VIEW_DETAIL_WIN_ID);
		    }
#endif
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
			if(PNULL!=to_date_param)
			{
				SCI_FREE(to_date_param);
				to_date_param=PNULL;
			}
            break;
            
        default:
		recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
// #ifdef FM_SUPPORT//CR132759
/*****************************************************************************/
//  Description : 频率编辑窗口消息处理函数
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:   
/*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleFMEditWinMsg(
//                                           MMI_WIN_ID_T      win_id,     
//                                           MMI_MESSAGE_ID_E  msg_id,
//                                           DPARAM            param
//                                           )
// {
//     MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
// #ifdef FM_SUPPORT
// #ifdef MMI_PDA_SUPPORT
//     uint16 s_cur_list_index=0;
// #else
//     MMI_STRING_T str={0};
//     MMI_CTRL_ID_T ctrl_id =MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID;
// #endif
// 	MMISCH_DETAIL_WIN_PARAM_T   *detail_win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
//     switch(msg_id)
//     {
//     case MSG_OPEN_WINDOW:
//         MMIAPIALM_SetFMEditCtrlParam();
// #ifdef MMI_PDA_SUPPORT
//         MMK_SetAtvCtrl(win_id, MMIALM_SELECT_FM_LIST_CTRL_ID);
// #else
//         MMK_SetAtvCtrl(win_id, MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID);
// #endif
//         break;
//     case MSG_CTL_PENOK:
// #ifdef MMI_PDA_SUPPORT
//         s_cur_list_index = GUILIST_GetCurItemIndex(MMIALM_SELECT_FM_LIST_CTRL_ID);
//         if(0==s_cur_list_index)
//         {
//             MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
//             *w_id = MMIALM_FM_EDIT_WIN_ID;
//             MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
//         }
//         else if(1==s_cur_list_index)
//         {
//            MMK_CreatePubEditWin((uint32 *)MMIALM_EDITFMFRE_WIN_TAB, PNULL); 
//         }
// #else
//         ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
//         if(ctrl_id==MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID)
//         {
//             MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
//             *w_id = MMIALM_FM_EDIT_WIN_ID;
//             MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
//         }
// #endif
//         break;
// #ifndef MMI_PDA_SUPPORT
//     case MSG_CTL_OK:
//     case MSG_APP_OK:
//     case MSG_KEYDOWN_WEB:
//     case MSG_CTL_MIDSK:
//     case MSG_APP_MENU:
//         ctrl_id = MMK_GetActiveCtrlId(win_id);
//         if(ctrl_id==MMI_FM_EDITWIN_LIST_LABEL_CTRL_ID)
//         {
//             MMI_WIN_ID_T *w_id = SCI_ALLOC_APP(sizeof(MMI_WIN_ID_T));
//             *w_id = MMIALM_FM_EDIT_WIN_ID;
//             MMIAPIFM_CreateAlarmChannelList((ADD_DATA)w_id);
//         }
//         else if(ctrl_id==MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID)
//         {
//             GUIEDIT_GetString(MMI_FM_EDITWIN_FRE_EDIT_CTRL_ID, &str);
//             if(MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))
//             {
//                 int32 iloop = 0, dot_index = 0;
//                 BOOLEAN exist_dot = FALSE;
// 
//                 //因存在点号而点号后面没有数据，导致数据非法，在后面添加0
//                 //其实应该在函数MMIALM_IsValidFM中处理的，但是这样就修改了原函数
//                 //的功能，此处有一个地方注意，在点号后直接添加0，
//                 //等于将编辑器添加了一个字符0，但这里应该不会有什么影响吧
//                 for(iloop = 0; iloop < str.wstr_len && iloop < MMIALM_FM_MAX_LEN; iloop ++)
//                 {
//                     if((uint16)str.wstr_ptr[iloop] == '.')
//                     {
//                         exist_dot = TRUE;
//                         dot_index = iloop;
//                         //break;        //防止存在点号和后续数据，依然被处理了
//                     }
//                 }
// 
//                 if(exist_dot && (dot_index + 1 == iloop))   //点号为最后一位，添加0
//                 {
//                     str.wstr_ptr[dot_index + 1] = '0';
//                     str.wstr_len += 1;
//                 }
//                 else if(exist_dot == FALSE)     //没有点号，需要添加点号和0
//                 {
//                     str.wstr_ptr[str.wstr_len] = '.';
//                     str.wstr_ptr[str.wstr_len + 1] = '0';
//                     str.wstr_len += 2;
//                 }
// 				if(PNULL == detail_win_param)
// 				{
// 					break;
// 				}
//                 detail_win_param->event.event_content_info.ring_type = ALM_FM_RING;
//                 detail_win_param->event.event_ring_info.dring_name_len = str.wstr_len;
// 
//                 SCI_MEMSET(detail_win_param->event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
//                 MMI_WSTRNCPY(detail_win_param->event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
//                 str.wstr_ptr, str.wstr_len, str.wstr_len);
//                 detail_win_param->event.event_content_info.ring = MMISET_MAX_RING_ID;
//                 
//                 MMK_CloseWin(win_id);
//                 MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
//             }
//             else
//             {
//                 MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
//             }
//         }
//         break;
// #endif
//     case MSG_ALM_SELECTED_FM_RETURN:
//         {
//             char freq_str[MMIALM_FREQ_MAX_LEN+1]={0};
//             MMI_STRING_T str_t = {0};
//             wchar wfreq_str[MMIALM_FREQ_MAX_LEN+1]={0};
//             MMIFM_CHANNEL_T *win_param = (MMIFM_CHANNEL_T *)param;
//             sprintf((char *)freq_str,"%d.%d",win_param->freqency/10, win_param->freqency%10);
//             str_t.wstr_len = MIN(strlen(freq_str), MMIALM_FREQ_MAX_LEN);    /*lint !e666*/
//             str_t.wstr_ptr = wfreq_str;
//             MMI_STRNTOWSTR(str_t.wstr_ptr, MMIALM_FREQ_MAX_LEN, freq_str, MMIALM_FREQ_MAX_LEN, str_t.wstr_len);/*lint !e64*/
//             if(MMIAPIALM_IsValidFM(str_t.wstr_ptr, str_t.wstr_len))
//             {
// 				if(PNULL == detail_win_param)
// 				{
// 					break;
// 				}
//                 detail_win_param->event.event_content_info.ring_type = ALM_FM_RING;
//                 detail_win_param->event.event_ring_info.dring_name_len = str_t.wstr_len;
// 
//                 SCI_MEMSET(detail_win_param->event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
//                 MMI_WSTRNCPY(detail_win_param->event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
//                 str_t.wstr_ptr, str_t.wstr_len, str_t.wstr_len);
//                 detail_win_param->event.event_content_info.ring = MMISET_MAX_RING_ID;
//                 
//                 MMK_CloseWin(win_id);
//                 MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
//             }
//             else
//             {
//                 MMIPUB_OpenAlertWarningWin(TXT_ALARM_FM_INVALID_PROMPT);
//             }
//         }
//         break;
// 
//     case MSG_CTL_CANCEL:
//     case MSG_KEYDOWN_CANCEL:
//         MMK_CloseWin(win_id);
//         break;
// 
//     case MSG_CLOSE_WINDOW:
//         break;
//         
//     default:
//         recode = MMI_RESULT_FALSE;
//         break;
//     }
// #endif
//     return recode;
// }
// 
// #endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : select schedule ring
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchRingWinMsg(
                                        MMI_WIN_ID_T		win_id, 
                                        MMI_MESSAGE_ID_E	msg_id, 
                                        DPARAM				param
                                        )
{
    MMI_RESULT_E	                recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                   ctrl_id = MMISCH_RINGLIST_CTRL_ID;
    uint32			                item_id = 0;
    MULTIM_SELECT_RETURN_T          *win_param = NULL;
    MMISCH_DETAIL_WIN_PARAM_T       *detail_win_param = NULL;
    MMIFMM_SELECT_INFO_T            select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    GUIMENU_BUTTON_INFO_T    button_info={0};
    GUIMENU_TITLE_INFO_T      title_info={0};
    uint32 group_id=MENU_SCH_SETTING_RING;
    MMI_STRING_T title={0};
    
    detail_win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != detail_win_param);/*assert verified*/
    if(PNULL == detail_win_param)
    {
        return  recode;
    }
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {                
                button_info.is_static=TRUE;
                button_info.group_id=group_id;
                button_info.button_style=GUIMENU_BUTTON_STYLE_CANCEL;
                GUIMENU_SetButtonStyle(ctrl_id,&button_info);

                title_info.group_id=group_id;
                title_info.is_static=TRUE;
                MMI_GetLabelTextByLang(TXT_RING,&title);
                title_info.title_ptr=&title;
                GUIMENU_SetMenuTitleEx(ctrl_id,&title_info);
                    
                if(ALM_DOWNLOAD_RING == detail_win_param->event.event_content_info.ring_type)
                {
                    pop_item_info.menu_id=ID_ALARM_RING_DOWNLOAD;
                }
                else if(ALM_RECORD_RING == detail_win_param->event.event_content_info.ring_type)
                {
                    pop_item_info.menu_id=ID_ALARM_RING_RECORD;
                }
                else
                {
                     pop_item_info.menu_id=ID_ALARM_RING_FIX;
                }
                //set pop-up menu status
                pop_item_info.is_static = TRUE;
                pop_item_info.ctrl_id   = ctrl_id;
                pop_item_info.group_id  = group_id;
                GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
                MMK_SetAtvCtrl(win_id, ctrl_id);
            }
            break;

        case MSG_GET_FOCUS:
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB: 
            GUIMENU_GetId(ctrl_id, &group_id, &item_id);
            switch( item_id)
            {
                case ID_ALARM_RING_FIX:
                    MMK_CreatePubListWin((uint32 *)SCH_FIXEDRINGWIN_TAB, (ADD_DATA)detail_win_param);
                    break;

                case ID_ALARM_RING_RECORD:
                    RecordCalendarRingCallBack();
                    break;

                case ID_ALARM_RING_DOWNLOAD:					
                    select_info.ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
                    if (0 == select_info.ring_vol)
                    {
                        select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
                    }					                 
#ifdef DRM_SUPPORT
            		select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif
                    if(ALM_DOWNLOAD_RING == detail_win_param->event.event_content_info.ring_type)
                    {
                        select_info.ticked_file_wstr = detail_win_param->event.event_ring_info.dring_name_arr;
                        select_info.ticked_file_len= detail_win_param->event.event_ring_info.dring_name_len;
				        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMusicWin(&select_info);
                    }
                    else
                    {
                        select_info.ticked_file_wstr = PNULL;
                        select_info.ticked_file_len= 0;
				        select_info.win_id = win_id;
				        MMIAPIFMM_OpenSelectMusicWin(&select_info);
                    }
#ifdef DRM_SUPPORT
                    {
                        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
                        limit_value.is_rights_valid = 1;
                        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
                    }
#endif 

                    break;

                default:
                    break;
            }
            break;

        case MSG_MULTIM_SELECTED_RETURN:
            win_param = (MULTIM_SELECT_RETURN_T *)param;

            detail_win_param->event.event_content_info.ring_type = ALM_DOWNLOAD_RING;
            detail_win_param->event.event_ring_info.dring_name_len = win_param->file_name_len;

            SCI_MEMSET(detail_win_param->event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));
            MMI_WSTRNCPY(detail_win_param->event.event_ring_info.dring_name_arr, (MMIFILE_FULL_PATH_MAX_LEN+1),
            win_param->file_name_ptr, win_param->file_name_len, win_param->file_name_len);

            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;     
}
#else

/*****************************************************************************/
// 	Description : select schedule ring
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchRingWinMsg(
                                        MMI_WIN_ID_T		win_id, 
                                        MMI_MESSAGE_ID_E	msg_id, 
                                        DPARAM				param
                                        )
{
    MMI_RESULT_E                    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                   ctrl_id = MMISCH_RINGLIST_CTRL_ID;
    uint16                          item_id = 0;
    MULTIM_SELECT_RETURN_T          *win_param = NULL;
    MMISCH_DETAIL_WIN_PARAM_T       *detail_win_param = NULL;
    MMIFMM_SELECT_INFO_T            select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
    detail_win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != detail_win_param);/*assert verified*/
    if(PNULL == detail_win_param)
    {
        return  recode;
    }
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            {
                uint16 current_index = ALM_FIX_RING ;//CR132759
                if( ALM_DOWNLOAD_RING == detail_win_param->event.event_content_info.ring_type )
                {
                    current_index=ALM_DOWNLOAD_RING;
                }
                else if( ALM_RECORD_RING == detail_win_param->event.event_content_info.ring_type )
                {
                    current_index=ALM_RECORD_RING;
                }
#ifdef FM_SUPPORT
                else if( ALM_FM_RING == detail_win_param->event.event_content_info.ring_type )
                {
                    current_index=ALM_FM_RING;
                }
#endif

                GUILIST_SetMaxItem(ctrl_id, ALM_RING_TYPE_MAX, FALSE );
                GUILIST_SetOwnSofterKey( ctrl_id, TRUE );

                MMIAPISET_AppendListItemByTextId( TXT_RING_FIXED, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
                MMIAPISET_AppendListItemByTextId( TXT_COMMON_MORE_RING, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#ifdef FM_SUPPORT
                MMIAPISET_AppendListItemByTextId(TXT_ALARM_FM_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
#ifdef MMI_RECORD_SUPPORT
                MMIAPISET_AppendListItemByTextId( TXT_ALARM_RECORD_RING_TYPE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif
                GUILIST_SetCurItemIndex( ctrl_id, current_index );
                GUILIST_SetSelectedItem( ctrl_id, current_index, TRUE );

                MMK_SetAtvCtrl(win_id, ctrl_id);
            }
            break;

        case MSG_GET_FOCUS://CR132759
            item_id = detail_win_param->event.event_content_info.ring_type;
            if(ALM_RING_TYPE_MAX<=item_id)
            {
                item_id = 0;
            }
            GUILIST_SetCurItemIndex(ctrl_id, item_id);
            GUILIST_SetSelectedItem(ctrl_id, item_id, TRUE);
//             if( ALM_DOWNLOAD_RING == detail_win_param->event.event_content_info.ring_type )
//             {
//                 GUILIST_SetSelectedItem( ctrl_id, 0, FALSE );
//                 GUILIST_SetSelectedItem( ctrl_id, 1, TRUE );
//                 GUILIST_SetSelectedItem( ctrl_id, 2, FALSE );


//             }
//             else if( ALM_RECORD_RING == detail_win_param->event.event_content_info.ring_type )
//             {
//                 GUILIST_SetSelectedItem( ctrl_id, 0, FALSE );
//                 GUILIST_SetSelectedItem( ctrl_id, 2, TRUE );
//                 GUILIST_SetSelectedItem( ctrl_id, 1, FALSE );

//             }

//             else
//             {
//                 GUILIST_SetSelectedItem( ctrl_id, 1, FALSE );
//                 GUILIST_SetSelectedItem( ctrl_id, 0, TRUE );
//                 GUILIST_SetSelectedItem( ctrl_id, 2, FALSE );


 //            }
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB: 
            item_id = GUILIST_GetCurItemIndex(ctrl_id);
            switch( item_id)
            {//CR132759
                case ALM_FIX_RING:
                    MMK_CreateWin((uint32 *)SCH_FIXEDRINGWIN_TAB, (ADD_DATA)detail_win_param);
                    break;
#ifdef MMI_RECORD_SUPPORT
                case ALM_RECORD_RING:
                    RecordCalendarRingCallBack();
                    break;
#endif
#ifdef FM_SUPPORT
                case ALM_FM_RING:
                {
                    MMIAPIALM_OpenFMEditWin((ADD_DATA)&(detail_win_param->event));
                }
                    break;
#endif
                case ALM_DOWNLOAD_RING:
                    select_info.ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
                    if (0 == select_info.ring_vol)
                    {
                        select_info.ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
                    }
#ifdef DRM_SUPPORT
                    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
                    if(ALM_DOWNLOAD_RING == detail_win_param->event.event_content_info.ring_type)
                    {
                        select_info.ticked_file_wstr = detail_win_param->event.event_ring_info.dring_name_arr;
                        select_info.ticked_file_len= detail_win_param->event.event_ring_info.dring_name_len;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMusicWin(&select_info);
                    }
                    else
                    {
                        select_info.ticked_file_wstr = PNULL;
                        select_info.ticked_file_len= 0;
                        select_info.win_id = win_id;
                        MMIAPIFMM_OpenSelectMusicWin(&select_info);
                    }
#ifdef DRM_SUPPORT
                    {
                        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
                        limit_value.is_rights_valid = 1;
                        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
                    }
#endif 

                    break;

                default:
                    break;
            }
            break;

        case MSG_MULTIM_SELECTED_RETURN:
            win_param = (MULTIM_SELECT_RETURN_T *)param;

            detail_win_param->event.event_content_info.ring_type = ALM_DOWNLOAD_RING;
            detail_win_param->event.event_ring_info.dring_name_len = win_param->file_name_len;

            SCI_MEMSET(detail_win_param->event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));
            MMI_WSTRNCPY(detail_win_param->event.event_ring_info.dring_name_arr, (MMIFILE_FULL_PATH_MAX_LEN+1),
            win_param->file_name_ptr, win_param->file_name_len, win_param->file_name_len);

            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;     
}

#endif
/*****************************************************************************/
// 	Description : select fixed ring
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchFixedRingWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E	            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMISCH_FIXEDRINGLIST_CTRL_ID;
    
    uint16                      cur_select = 0;
    uint16                      ring_id = 0;
    MMISCH_DETAIL_WIN_PARAM_T   *detail_win_param = NULL;
	uint8 ring_vol=0;

    detail_win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != detail_win_param);/*assert verified*/
    if(PNULL == detail_win_param)
    {
        return  recode;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        GUILIST_SetMaxItem( ctrl_id, MMISET_RING_FIXED_DEFAUT_NUM, FALSE );//max item 8
        
        MMIAPISET_AppendFixedItems(
                                        TXT_RING,
                                        MMISET_RING_FIXED_DEFAUT_NUM,
                                        TXT_COMMON_OK,
                                        TXT_NULL,
                                        STXT_RETURN,
                                        ctrl_id,
                                        GUIITEM_STYLE_ONE_LINE_RADIO
                                        );
    
        if( ALM_FIX_RING == detail_win_param->event.event_content_info.ring_type )
        {
            ring_id = detail_win_param->event.event_content_info.ring;
            cur_select = ring_id - 1;
            
            GUILIST_SetSelectedItem( ctrl_id, cur_select, TRUE );
        }
        else
        {
            ring_id = 1;
            cur_select = 0;
        }
        
        GUILIST_SetCurItemIndex(ctrl_id,cur_select);	
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
		ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMISCH_FIXEDRINGLIST_CTRL_ID:
                MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
                cur_select = GUILIST_GetCurItemIndex(ctrl_id);
                
                ring_id = cur_select+1;
        		ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
                if (0 == ring_vol)
                {
                    ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
                }
                MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            default:
                break;
            }

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#else   
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        cur_select = GUILIST_GetCurItemIndex(ctrl_id);
        
        ring_id = cur_select+1;
		ring_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_ALARM);
        if (0 == ring_vol)
        {
            ring_vol = MMISET_PLAY_RING_DEFAULT_VOL;
        }
        MMIAPISET_PlayRingByIdEx(FALSE,ring_id,ring_vol,1,MMISET_RING_TYPE_ALARM,PNULL);
        break;
#endif   
    case MSG_FULL_PAINT:
        break;
        
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if ( MMK_IsSingleClick(msg_id, param))
        {
            break;
        }
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
        
        cur_select = GUILIST_GetCurItemIndex(ctrl_id);
        ring_id = cur_select +1;
        detail_win_param->event.event_content_info.ring_type = ALM_FIX_RING;
        detail_win_param->event.event_content_info.ring = ring_id;
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin(MMISCH_RINGLIST_WIN_ID);
        MMK_CloseWin( win_id );
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_LOSE_FOCUS:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        break;        
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode; 
}
                                             
                                             
/*****************************************************************************/
// 	Description : check whether the date has schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsOneDateHasSche(
                                uint16      year,
                                uint8       month,
                                uint8       day
                                )
{
    uint16                      i = 0;
    uint16                      start_id = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T  *event_ptr = NULL;
    BOOLEAN                     has_sche = FALSE;
    SCI_DATE_T      event_start_date                         = {0};
    SCI_DATE_T      event_end_date                  = {0};
    SCI_DATE_T          	cur_date                             = {0};
    uint8     week_day = 0;

    cur_date.year = year;
    cur_date.mon= month;
    cur_date.mday= day;    
    start_id = EVENT_SCH_0;
    for (i = 0; i<MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(start_id+i));

        event_start_date.year = event_ptr->start_year;
        event_start_date.mon  = event_ptr->start_mon;
        event_start_date.mday = event_ptr->start_day;
        event_end_date.year = event_ptr->end_year;
        event_end_date.mon  = event_ptr->end_mon;
        event_end_date.mday = event_ptr->end_day;	

    switch (event_ptr->fre_mode)
        {
        case ALM_MODE_ONCE:
        case ALM_MODE_NO:
            if(event_ptr->is_valid &&
                    year == event_ptr->year &&
                    month == event_ptr->mon &&
                    day == event_ptr->day
                    )
                {
                    return TRUE;
                }
            break;

        case ALM_MODE_EVERYDAY:
            if (event_ptr->is_valid &&
                MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)
                )
                {
                    return TRUE;
                }
            break;

        case ALM_MODE_EVERYWEEK:
            week_day = MMICALENDAR_GetWeekday(year,month,day);
            if(event_ptr->is_valid &&
                 MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                ((event_ptr->fre & s_week_mask[week_day]) != 0)
                )
                {
                    return TRUE;
                }
            break;

        case ALM_MODE_EVERYMONTH:
            if (event_ptr->is_valid &&
                    MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                day == event_ptr->day
                )
                {
                    return TRUE;
                }
            break;

        case ALM_MODE_EVERYYEAR:
            if (event_ptr->is_valid &&
                MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                month == event_ptr->mon &&
                day == event_ptr->day
                )
                {
                    return TRUE;
                }
            break;

        default:
            break;
        }
    }
    
    return has_sche;
}


/*****************************************************************************/
// 	Description : get one date's schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_GetOneDateSche(
                              uint16              year,
                              uint8               month,
                              uint8               day,
                              uint16              *sche_num_ptr,
                              MMI_IMAGE_ID_T      *sche_icon_1_ptr,
                              MMI_IMAGE_ID_T      *sche_icon_2_ptr
                              )
{
    uint16                          i           = 0;
    uint16                          start_id    = 0;
    BOOLEAN                         has_sche    = FALSE;
    const MMIACC_SMART_EVENT_FAST_TABLE_T      *event_ptr  = NULL;

    SCI_DATE_T		event_start_date                         = {0};
    SCI_DATE_T		event_end_date                  = {0};
    SCI_DATE_T          	cur_date                             = {0};
    uint8     week_day = 0;

    cur_date.year = year;
    cur_date.mon= month;
    cur_date.mday= day;

    //SCI_ASSERT(NULL != sche_num_ptr);/*assert verified*/
    //SCI_ASSERT(NULL != sche_icon_1_ptr);/*assert verified*/
    //SCI_ASSERT(NULL != sche_icon_2_ptr);/*assert verified*/
    if(PNULL == sche_num_ptr || PNULL == sche_icon_1_ptr ||PNULL == sche_icon_2_ptr)
    {
        return  FALSE;
    }
    
    *sche_num_ptr = 0;
    
    start_id = EVENT_SCH_0;
    for (i = 0; i<MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(start_id+i));

        event_start_date.year = event_ptr->start_year;
        event_start_date.mon  = event_ptr->start_mon;
        event_start_date.mday = event_ptr->start_day;
        event_end_date.year = event_ptr->end_year;
        event_end_date.mon  = event_ptr->end_mon;
        event_end_date.mday = event_ptr->end_day;	

        switch (event_ptr->fre_mode)
        {
        case ALM_MODE_ONCE:
        case ALM_MODE_NO:
            if(event_ptr->is_valid &&
                    year == event_ptr->year &&
                    month == event_ptr->mon &&
                    day == event_ptr->day
                    )
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_1_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_2_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        case ALM_MODE_EVERYDAY:

            if (event_ptr->is_valid &&
                MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)
                )
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_1_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_2_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        case ALM_MODE_EVERYWEEK:
            week_day = MMICALENDAR_GetWeekday(year,month,day);
            if(event_ptr->is_valid &&
                 MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                ((event_ptr->fre & s_week_mask[week_day]) != 0)
                    )
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_1_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_2_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else
                {
                    (*sche_num_ptr)++;
                }	
            }
            break;
				
        case ALM_MODE_EVERYMONTH:
            if (event_ptr->is_valid &&
                    MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                    day == event_ptr->day
                )
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_1_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_2_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        case ALM_MODE_EVERYYEAR:
            if (event_ptr->is_valid &&
                    MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
                    month == event_ptr->mon &&
                    day == event_ptr->day
                )
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_1_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                    *sche_icon_2_ptr = IMAGE_SCHEDULE_ARRANGE_01;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        default:
            break;
        }
    }
    
    return has_sche;
}

/*****************************************************************************/
// 	Description : To get all schedule number
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_GetAllSchNumber(void)
{
    uint16                          i          = 0;
    uint16                         	sche_num   = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T      *event_ptr = NULL;
    
    for (i = 0; i < MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0 + i));
        if (event_ptr->is_valid)
        {
            sche_num++;
        }
    }
    
    return sche_num;	
}

/*****************************************************************************/
// 	Description : To get today's schedule number
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_GetTodaySchNumber(SCI_DATE_T *date)
{
    // SCI_DATE_T		date = {0};
    uint16 			sch_num = 0;
    MMI_IMAGE_ID_T	sche_icon = IMAGE_NULL;
    
    //SCI_ASSERT(NULL != date);/*assert verified*/
    if(PNULL == date )
    {
        return  sch_num;
    }
        
    MMIAPISCH_GetOneDateSche(date->year, date->mon, date->mday,
        &sch_num, &sche_icon, &sche_icon);
    
    return sch_num;
}

/*****************************************************************************/
// 	Description : schedule  list option win
//	Global resource dependence : 
//  Author: xiaoqing
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchAllListOptWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E                        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id = MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID;
    static MMISCH_DETAIL_WIN_PARAM_T    *win_param = NULL;
    MMI_MENU_GROUP_ID_T	                group_id = 0; 
    MMI_MENU_ID_T		                menu_id  = 0;
    static MMISCH_DETAIL_WIN_PARAM_T 	*win_new_param = PNULL;
    MMI_CALENDAR_T                 *calendar_ctrl_ptr = &g_calendar_show;
	static MMISET_DATE_T   *detail_win_param = PNULL;
    MMISET_LANGUAGE_TYPE_E lang_type=MMISET_LANGUAGE_ENGLISH;
	static uint32 *to_date_param=PNULL;
	const MMIACC_SMART_EVENT_FAST_TABLE_T *event=PNULL;
#ifdef MMI_VCALENDAR_SUPPORT
    MMI_CTRL_ID_T list_ctrl_id=MMISCH_VIEW_ALL_LIST_CTRL_ID;
    uint16        cur_selection=0;
    //GUILIST_ITEM_T* item_ptr=PNULL;
    uint16 event_id=0;
    uint16 event_index[MMIACC_SCH_NUM]={0};
    uint16 event_num=0;
    int i=0;
	uint16 _event_id[MMIACC_SCH_NUM]={0};
#endif

    win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
    //SCI_ASSERT(NULL != win_param);/*assert verified*/
    if(PNULL == win_param )
    {
        return  recode;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
//CR163242
		{
			MMI_STRING_T kstring = {0};
			MMI_GetLabelTextByLang(STXT_MARK, &kstring);
			GUIMENU_SetMenuTitle(&kstring,MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID);
		}  
        
        MMK_SetAtvCtrl( win_id,  ctrl_id);
		EnableListOptionMenuItem(
			TRUE,
			calendar_ctrl_ptr->solar_year,
			calendar_ctrl_ptr->solar_month,
			calendar_ctrl_ptr->solar_date
			);
		MMIAPISET_GetLanguageType(&lang_type);
		if(MMISET_LANGUAGE_SIMP_CHINESE!=lang_type&&MMISET_LANGUAGE_TRAD_CHINESE!=lang_type)
		{
			GUIMENU_SetItemVisible(ctrl_id,MENU_SCH_ALL_OPT,ID_SCH_LIST_LUNAR_DETAIL,FALSE);
			if(!MMIAPISCH_OpenEventList(TRUE, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date))//maryxiao cr230753
			{
	            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_TO_DATE, TRUE);
			}
		}
		else
		{
			GUIMENU_SetItemVisible(ctrl_id,MENU_SCH_ALL_OPT,ID_SCH_LIST_LUNAR_DETAIL,TRUE);
			if(!MMIAPISCH_OpenEventList(TRUE, calendar_ctrl_ptr->solar_year, calendar_ctrl_ptr->solar_month, calendar_ctrl_ptr->solar_date))
			{
	            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_LIST_LUNAR_DETAIL, TRUE);
	            GUIMENU_SetItemGrayed(MMISCH_ALL_LIST_OPTION_MENU_CTRL_ID,MENU_SCH_ALL_OPT, ID_SCH_TO_DATE, TRUE);
			}
		}
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch( menu_id)
        {
        case ID_SCH_ALL_LIST_VIEW:	
            MMK_CreateWin((uint32 *)SCH_VIEW_DETAIL_WIN_TAB, (ADD_DATA)win_param);	
			MMIAPISCH_DeleteUnreadEvent(win_param->index);
#ifdef MMI_VCALENDAR_SUPPORT
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
#endif
            break;
            
        case ID_SCH_ALL_LIST_NEW:
#ifdef MMI_VCALENDAR_SUPPORT
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
#endif
            if(NULL != win_new_param)
            {
                SCI_FREE(win_new_param);
            }
            win_new_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            if(win_new_param != PNULL)
            {
                MMISET_DATE_T  date={0};
                SCI_DATE_T  cur_date={0};
                SCI_MEMSET(win_new_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
                win_new_param->win_id = win_id;
                date.date_year=calendar_ctrl_ptr->solar_year;
                date.date_month=calendar_ctrl_ptr->solar_month;
                date.date_day=calendar_ctrl_ptr->solar_date;
                if((!MMIAPICOM_CheckDateIsValid(date))
                    ||(!MMIAPICALENDAR_IsOpenCalendar()))
                {
                    TM_GetSysDate(&cur_date);
                    calendar_ctrl_ptr->solar_year=cur_date.year;
                    calendar_ctrl_ptr->solar_month=cur_date.mon;
                    calendar_ctrl_ptr->solar_date=cur_date.mday;
                }
                MMIAPISCH_InitNewSch(calendar_ctrl_ptr->solar_year,
                    calendar_ctrl_ptr->solar_month,
                    calendar_ctrl_ptr->solar_date,
                    &(win_new_param->event));
                MMK_CreateWin((uint32 *)SCH_EDIT_WIN_TAB, (ADD_DATA)win_new_param);	
            }
            break;
            
        case ID_SCH_ALL_LIST_EDIT:	
            win_param->win_id = win_id;
            MMK_CreateWin((uint32 *)SCH_EDIT_WIN_TAB, (ADD_DATA)win_param);	
            MMIAPISCH_DeleteUnreadEvent(win_param->index);
#ifdef MMI_VCALENDAR_SUPPORT
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
#endif
            break;
#ifndef MMI_VCALENDAR_SUPPORT     
        case ID_SCH_ALL_LIST_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleAllListDelQueryWin);
            MMK_CloseWin(win_id);
            break;
            
        case ID_SCH_ALL_LIST_DEL_ALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,HandleAllListDelAllQueryWin);
            MMK_CloseWin(win_id);
            break;
#endif
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
        case ID_SCH_LIST_LUNAR_DETAIL:
            if(NULL != detail_win_param)
            {
                SCI_FREE(detail_win_param);
                detail_win_param=PNULL;
            }
            detail_win_param = SCI_ALLOC_APP(sizeof(MMISET_DATE_T));
            if(detail_win_param != PNULL)
            {
                SCI_MEMSET(detail_win_param, 0, sizeof(MMISET_DATE_T));

                if (PNULL != win_param)
                {
                    event=MMIALM_GeEvent(win_param->index);
                    if(win_param->index>=EVENT_SCH_0&&win_param->index<=EVENT_SCH_MAX)
                    {
                        detail_win_param->date_year= event->start_year;
                        detail_win_param->date_month= event->start_mon;
                        detail_win_param->date_day= event->start_day;
                    }
                    else
                    {
                        detail_win_param->date_year= calendar_ctrl_ptr->solar_year;
                        detail_win_param->date_month= calendar_ctrl_ptr->solar_month;
                        detail_win_param->date_day= calendar_ctrl_ptr->solar_date;
                    }
                    MMIAPICALENDAR_OpenLunarDetailWin((ADD_DATA)detail_win_param);
                }
            }
            break;
#endif
        case ID_SCH_TO_DATE:
            if(PNULL!=to_date_param)
            {
                SCI_FREE(to_date_param);
                to_date_param=PNULL;
            }
            to_date_param=SCI_ALLOC_APP(sizeof(uint32));
            *to_date_param=CALENDAR_TO_DATE_ALL_SCH_LIST;
            event=MMIALM_GeEvent(win_param->index);
            if(win_param->index>=EVENT_SCH_0&&win_param->index<=EVENT_SCH_MAX)
            {
                MMIAPISCH_SetAllListCurData(event->start_day,event->start_mon,event->start_year);
            }
            else
            {
                MMIAPISCH_SetAllListCurData(calendar_ctrl_ptr->solar_date,calendar_ctrl_ptr->solar_month,calendar_ctrl_ptr->solar_year);
            }
            MMIAPICALEND_OpenCalendarQueryByDate((ADD_DATA)to_date_param);
            break;
#ifdef MMI_VCALENDAR_SUPPORT
        case ID_SCH_LIST_SEND_SMS:
            event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
            if(1==event_num)
            {
                cur_selection=event_index[0];
            }
            else
            {
                cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            }
		    event_id = MapListItemToUserItem(cur_selection);
            SendVcalendarBySMS(event_id);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_MMS:
            event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
            if(1==event_num)
            {
                cur_selection=event_index[0];
            }
            else
            {
                cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            }
		    event_id = MapListItemToUserItem(cur_selection);
            SendVcalendarByMMS(event_id);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_BT:		
    		event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
            if(0==event_num)
            {
                cur_selection = GUILIST_GetCurItemIndex(list_ctrl_id);
                _event_id[0] = MapListItemToUserItem(cur_selection);
                event_num = 1;
            }
            else
            {
    		    for(i=0;i<event_num;i++)
                {
            	    _event_id[i] = MapListItemToUserItem(event_index[i]);
		        }
            }
		    //event_id = MapListItemToUserItem(cur_selection);
            SendVcalendarByBT(_event_id,event_num);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SEND_EMAIL:
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_LIST_SUB_MARK:
            cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id, s_user_list.total);
            GUILIST_SetSelectedItem(list_ctrl_id,cur_selection,TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_SCH_LIST_SUB_CANCEL_MARK:
            cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
            GUILIST_SetSelectedItem(list_ctrl_id,cur_selection,FALSE);
            if (0 == GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0))
            {
                GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            }
            MMK_CloseWin(win_id);
            break;

        case ID_SCH_LIST_SUB_MARK_ALL:          
            GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
            GUILIST_SetMaxSelectedItem(list_ctrl_id, s_user_list.total);
            GUILIST_SetAllSelected(list_ctrl_id,TRUE);
            MMK_CloseWin(win_id);
            break;

        case ID_SCH_LIST_SUB_CANCEL_MARK_ALL:
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_VCALENDAR_EXPORT:
        case ID_SCH_LIST_DEL_MARK:
            SCI_MEMSET(&s_operate_sch_info,0,sizeof(MMISCH_OPERATE_RECORDS_INFO_T));
            event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
            if(event_num>0)
            {
                s_operate_sch_info.total_count=event_num;
                for(i=0;i<event_num;i++)
                {
                    //item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id,event_index[i]);
		            s_operate_sch_info.event_id[i] = s_user_list.entry_id[event_index[i]];
                }
            }
            else
            {
                s_operate_sch_info.total_count=1;
                cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
                s_operate_sch_info.event_id[0] = s_user_list.entry_id[cur_selection];
            }
            
            if(ID_SCH_VCALENDAR_EXPORT==menu_id)
            {
                SelectVcalendarExportFolder(list_ctrl_id);
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleAllDelMarkQueryWin);
                //DeleteSch(&s_operate_sch_info,list_ctrl_id);
            }
            MMK_CloseWin(win_id);
            break;
        case ID_SCH_VCALENDAR_IMPORT:
            GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
            GUILIST_SetAllSelected(list_ctrl_id,FALSE);
            SchLoadAllVCalendar();
            MMK_CloseWin(win_id);
            break;
#endif
        default:
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
 
        case MSG_SCH_EVENT_EDIT_FINISH:		
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            switch( menu_id)
            {
            case ID_SCH_ALL_LIST_EDIT:
                if (MMIAPIALM_SaveInteEventInfo((uint16)(win_param->index),&(win_param->event)))
                {
//#ifndef  MMI_PDA_SUPPORT
//                    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
                } 
                break;
                
            case ID_SCH_ALL_LIST_NEW:			
                if (MMIALM_AddOneEvent(&win_new_param->event,NULL))
                {
//#ifndef  MMI_PDA_SUPPORT
//                    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
                }
                else
                {
			if(MMIACC_SCH_NUM<=(MMIAPISCH_GetAllSchNumber()))
			{
			    MMIPUB_OpenAlertWarningWin(TXT_SCHEDULE_FULL);
			}
			else
			{
			    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
			}                
		 }            
                break;
                
            default:
                break;            
            }
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            if (PNULL != win_new_param)
            {
                SCI_FREE(win_new_param);
            }
            if(NULL != detail_win_param)
            {
                SCI_FREE(detail_win_param);
                detail_win_param=PNULL;
            }
            if(PNULL!=to_date_param)
            {
                SCI_FREE(to_date_param);
                to_date_param=PNULL;
            }
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : schedule  list option win when list be marked
//	Global resource dependence : 
//  Author: xiaoqing
//	Note:   
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E HandleSchListMarkOptWinMsg(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
        MMI_RESULT_E                        recode = MMI_RESULT_TRUE;
        MMI_CTRL_ID_T                       ctrl_id = MMISCH_LIST_MARK_OPTION_DYNAMIC_MENU_CTRL_ID;
        MMI_CTRL_ID_T list_ctrl_id = 0;
#ifdef MMI_VCALENDAR_SUPPORT
        uint32 node_id = 0;
        MMISCH_DETAIL_WIN_PARAM_T    *win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id);
        uint16        cur_selection=0;
        uint16 event_id=0;
        uint16 event_index[MMIACC_SCH_NUM]={0};
        uint16 event_num=0;
        int i=0;
	uint16 _event_id[MMIACC_SCH_NUM]={0};
        uint32 user_data = 0;
        if( win_param->win_id == MMISCH_VIEW_ALL_WIN_ID)
        {
            list_ctrl_id = MMISCH_VIEW_ALL_LIST_CTRL_ID;
        }
        else if( win_param->win_id == MMISCH_VIEW_WIN_ID)
        {
            list_ctrl_id = MMISCH_VIEW_LIST_CTRL_ID;
        }
#endif
        switch(msg_id)
        {
        case MSG_OPEN_WINDOW:
            AddSchListMarkOptMenuItem(win_id,ctrl_id, list_ctrl_id);
            MMK_SetAtvCtrl( win_id,  ctrl_id);
    
            break;
#ifdef MMI_VCALENDAR_SUPPORT            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
			
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:                   
            node_id = GUIMENU_GetCurNodeId(ctrl_id);
            switch( node_id)
            {            

            case ID_SCH_LIST_SEND_SMS:
                event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
                if(1==event_num)
                {
                    cur_selection=event_index[0];
                }
                else
                {
                    cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
                }
                event_id = MapListItemToUserItem(cur_selection);
                SendVcalendarBySMS(event_id);
                MMK_CloseWin(win_id);
                break;
            case ID_SCH_LIST_SEND_MMS:
                event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
                if(1==event_num)
                {
                    cur_selection=event_index[0];
                }
                else
                {
                    cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
                }
                event_id = MapListItemToUserItem(cur_selection);
                SendVcalendarByMMS(event_id);
                MMK_CloseWin(win_id);
                break;
            case ID_SCH_LIST_SEND_BT:
        		{
        			event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
        			for(i=0;i<event_num;i++)
        	        {
        	            _event_id[i] = MapListItemToUserItem(event_index[i]);
        			}
        			SendVcalendarByBT(_event_id,event_num);
        			MMK_CloseWin(win_id);
            	}
                break;
            case ID_SCH_LIST_SEND_EMAIL:
                MMK_CloseWin(win_id);
                break;
    
            case ID_SCH_LIST_SUB_MARK_ALL:          
                GUILIST_SetTextListMarkable(list_ctrl_id,TRUE);
                GUILIST_SetMaxSelectedItem(list_ctrl_id, s_user_list.total);
                GUILIST_SetAllSelected(list_ctrl_id,TRUE);
                MMK_CloseWin(win_id);
                break;
    
            case ID_SCH_LIST_SUB_CANCEL_MARK_ALL:
                GUILIST_SetTextListMarkable(list_ctrl_id,FALSE);
                MMK_CloseWin(win_id);
                break;
            case ID_SCH_VCALENDAR_EXPORT:
            case ID_SCH_LIST_DEL_MARK:
                SCI_MEMSET(&s_operate_sch_info,0,sizeof(MMISCH_OPERATE_RECORDS_INFO_T));
                event_num=GUILIST_GetSelectedItemIndex(list_ctrl_id, event_index, MMIACC_SCH_NUM);
                if(event_num>0)
                {
                    s_operate_sch_info.total_count=event_num;
                    for(i=0;i<event_num;i++)
                    {
                        if( win_param->win_id == MMISCH_VIEW_ALL_WIN_ID)
                        {
                            s_operate_sch_info.event_id[i] = s_user_list.entry_id[event_index[i]];
                        }
                        else if( win_param->win_id == MMISCH_VIEW_WIN_ID)
                        {
                            GUILIST_GetItemData(list_ctrl_id,event_index[i],&user_data);
                            s_operate_sch_info.event_id[i] = (uint16)user_data;
                        }
                    }
                }
                else
                {
                    s_operate_sch_info.total_count=1;
                    cur_selection=GUILIST_GetCurItemIndex(list_ctrl_id);
                    
                    if( win_param->win_id == MMISCH_VIEW_ALL_WIN_ID)
                    {
                        s_operate_sch_info.event_id[0] = s_user_list.entry_id[cur_selection];
                    }
                    else if( win_param->win_id == MMISCH_VIEW_WIN_ID)
                    {
                        GUILIST_GetItemData(list_ctrl_id,cur_selection,&user_data);
                        s_operate_sch_info.event_id[0] = (uint16)user_data;
                    }                    
                }
                
                if(ID_SCH_VCALENDAR_EXPORT==node_id)
                {
                    SelectVcalendarExportFolder(list_ctrl_id);
                }
                else
                {
                    if( win_param->win_id == MMISCH_VIEW_ALL_WIN_ID)
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleAllDelMarkQueryWin);
                    }
                    else if( win_param->win_id == MMISCH_VIEW_WIN_ID)
                    {
                        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,HandleDelMarkQueryWin);
                    }                    
                }
                MMK_CloseWin(win_id);
                break;

            default:
                break;
            }
            break;
#endif            
            case MSG_APP_CANCEL:
            case MSG_CTL_CANCEL:
                MMK_CloseWin(win_id);
                break;
                
            case MSG_CLOSE_WINDOW:
                break;
                
            default:
                recode = MMI_RESULT_FALSE;
                break;
        }
        
        return recode;
}
#endif

/*****************************************************************************/
// 	Description : add menu item of schedule list option win 
//	Global resource dependence : 
//  Author: xiaoqing
//	Note:   
/*****************************************************************************/
#ifdef MMI_PDA_SUPPORT
LOCAL int32 AddSchListMarkOptMenuItem(
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_CTRL_ID_T      ctrl_id, 
                                             MMI_CTRL_ID_T 	    list_ctrl_id
                                             )
{
#ifdef MMI_VCALENDAR_SUPPORT 
    MMI_STRING_T kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    uint16       selected_num =0;
#endif
    uint16       index = 0;

    GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP_AUTO);    
#ifdef MMI_VCALENDAR_SUPPORT    
    //MMI_GetLabelTextByLang(STXT_MARK, &kstring);
    //GUIMENU_SetMenuTitle(&kstring,ctrl_id);   
    node_item.item_text_ptr = &kstring;    
//CR163242
	{
		MMI_GetLabelTextByLang(STXT_OPTION, &kstring);
		GUIMENU_SetMenuTitle(&kstring,ctrl_id);
	}  

#ifdef MMI_VCALENDAR_SUPPORT
    MMI_GetLabelTextByLang(STXT_MARK, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_LIST_MARK, 0, &node_item, ctrl_id);    
#endif
    selected_num=GUILIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if(selected_num!=s_user_list.total)
    {
        MMI_GetLabelTextByLang(TXT_MARK_ALL, &kstring);
        GUIMENU_InsertNode(index++, ID_SCH_LIST_SUB_MARK_ALL, ID_SCH_LIST_MARK, &node_item, ctrl_id); 
    }

    MMI_GetLabelTextByLang(TXT_CANCEL_MARK_ALL, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_LIST_SUB_CANCEL_MARK_ALL, ID_SCH_LIST_MARK, &node_item, ctrl_id);  

    MMI_GetLabelTextByLang(TXT_DELETE, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_LIST_DEL_MARK, 0, &node_item, ctrl_id);   

//#ifdef MMI_VCALENDAR_SUPPORT
    MMI_GetLabelTextByLang(TXT_SEND, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_LIST_SEND, 0, &node_item, ctrl_id);      

    MMI_GetLabelTextByLang(TXT_SMS, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_LIST_SEND_SMS, ID_SCH_LIST_SEND, &node_item, ctrl_id);
    
#ifdef MMS_SUPPORT
    MMI_GetLabelTextByLang(TXT_MMS, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_LIST_SEND_MMS, ID_SCH_LIST_SEND, &node_item, ctrl_id); 
#endif

#ifdef BLUETOOTH_SUPPORT
    MMI_GetLabelTextByLang(TXT_BLUETOOTH, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_LIST_SEND_BT, ID_SCH_LIST_SEND, &node_item, ctrl_id);    
#endif

    MMI_GetLabelTextByLang(TXT_COMM_EXPORT, &kstring);
    GUIMENU_InsertNode(index++, ID_SCH_VCALENDAR_EXPORT, 0, &node_item, ctrl_id);   

#endif

    return (int32)index;
}
#endif
/*****************************************************************************/
// 	Description : get if start_date <= cur_date <= iend_date
//	Global resource dependence : 
//  Author:  zhaohui
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsInDateRange(     
				            SCI_DATE_T   start_date, 
                            SCI_DATE_T   end_date,
                            SCI_DATE_T   cur_date
                         )
{
		if (
		    ((CMP_FIRST_SMALL == CompareDate(start_date, cur_date)) 
		    &&(CMP_FIRST_SMALL == CompareDate(cur_date, end_date)) )||
		    (CMP_EQUAL == CompareDate(start_date, cur_date)
		    ||CMP_EQUAL == CompareDate(cur_date, end_date))
		)
		{
			return TRUE;
		}

		return FALSE;
}

/*****************************************************************************/
// 	Description : get one date's schedule
//	Global resource dependence : 
//  Author:  liqing.peng
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN GetInvalidSchByDate(
                            uint16              year,
                            uint8               month,
                            uint8               day,
                            uint16              *sche_num_ptr
                            )
{
    uint16                          i           = 0;
    uint16                          start_id    = 0;
    BOOLEAN                         has_sche    = FALSE;
    const MMIACC_SMART_EVENT_FAST_TABLE_T      *event_ptr  = NULL;
    SCI_DATE_T		event_start_date                = {0};
    SCI_DATE_T		event_end_date                  = {0};
    SCI_DATE_T      cur_date                        = {0};
    uint8           week_day = 0;

    cur_date.year = year;
    cur_date.mon= month;
    cur_date.mday= day;
	
    //SCI_ASSERT(NULL != sche_num_ptr);/*assert verified*/
    if(PNULL == sche_num_ptr )
    {
        return  FALSE;
    }
    
    *sche_num_ptr = 0;
    
    start_id = EVENT_SCH_0;
    for (i = 0; i<MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(start_id+i));

        event_start_date.year = event_ptr->start_year;
        event_start_date.mon  = event_ptr->start_mon;
        event_start_date.mday = event_ptr->start_day;
        event_end_date.year = event_ptr->end_year;
        event_end_date.mon  = event_ptr->end_mon;
        event_end_date.mday = event_ptr->end_day;	

        switch (event_ptr->fre_mode)
        {
        case ALM_MODE_ONCE:
        case ALM_MODE_NO:
            if(event_ptr->is_valid &&
                year == event_ptr->year &&
                month == event_ptr->mon &&
                day == event_ptr->day
                )
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        case ALM_MODE_EVERYDAY:
            if (event_ptr->is_valid &&
               MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)
                )
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        case ALM_MODE_EVERYWEEK:
            week_day = MMICALENDAR_GetWeekday(year,month,day);
            if(event_ptr->is_valid &&
            	MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
            	((event_ptr->fre & s_week_mask[week_day]) != 0)
            		)
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;
				
        case ALM_MODE_EVERYMONTH:
            if (event_ptr->is_valid &&
            	MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
            	    day == event_ptr->day
            	)
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        case ALM_MODE_EVERYYEAR:
            if (event_ptr->is_valid &&
            	MMIAPISCH_IsInDateRange(event_start_date,event_end_date,cur_date)&&
            	    month == event_ptr->mon &&
            	    day == event_ptr->day
            	)
            {
                has_sche = TRUE;
                if(0 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else if(1 == *sche_num_ptr)
                {
                    (*sche_num_ptr)++;
                }
                else
                {
                    (*sche_num_ptr)++;
                }
            }
            break;

        default:
            break;
        }
    }

    return has_sche;
}

/*****************************************************************************/
// 	Description : To get today's schedule number
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_GetInvalidSchByDate(SCI_DATE_T *date)
{
    uint16 			sch_num = 0;
    
    //SCI_ASSERT(NULL != date);/*assert verified*/
    if(PNULL == date )
    {
        return  sch_num;
    }
    
    GetInvalidSchByDate(date->year, date->mon, date->mday,
        &sch_num);
    
    return sch_num;
}

/*****************************************************************************/
// 	Description : to open today's schedule event for idle use
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISCH_OpenEventToday(uint16* event_id_ptr)
{
      SCI_DATE_T                          cur_date            = {0}; 
  
      TM_GetSysDate(&cur_date);
     
            
	if (MMIAPISCH_OpenEventList(FALSE, cur_date.year, cur_date.mon, cur_date.mday))
	{   
        
        *event_id_ptr = s_user_list.entry_id[0];        
		return TRUE;
	}
	else
	{
		return FALSE;
	}
} 

/*****************************************************************************/
// 	Description : if today's event is all past
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsAllPastEventToday(uint16* event_id_ptr)//如果没有都过期，则返回第一个未过齐的event_id
{
	uint16	i = 0;
	BOOLEAN		result = TRUE;
	SCI_DATE_T      cur_date = {0};
    SCI_TIME_T      cur_time = {0};
	MMIACC_SMART_EVENT_FAST_TABLE_T const * event_ptr = PNULL;  
      
	TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

	for (;i<s_user_list.total;i++)
	{
		//通过外部接口获得event,
		event_ptr = MMIALM_GeEvent(s_user_list.entry_id[i]);  
#ifdef CMCC_UI_STYLE
        if ((event_ptr->hour>cur_time.hour)||((event_ptr->hour == cur_time.hour)&&(event_ptr->minute > cur_time.min)))
        {
            *event_id_ptr = s_user_list.entry_id[i];		
            result = FALSE;
            break;
        }  
#else
		if ((event_ptr->year == cur_date.year&&
			event_ptr->mon == cur_date.mon&&
			event_ptr->day == cur_date.mday)&&
            ((event_ptr->hour>cur_time.hour)||((event_ptr->hour == cur_time.hour)&&(event_ptr->minute > cur_time.min))))
		{
			*event_id_ptr = s_user_list.entry_id[i];		
			result = FALSE;
			break;
		}
#endif
	}

	return result;
}

/*****************************************************************************/
// 	Description : 今日是否有日程
//	Global resource dependence : 
//  Author:applezhang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_IsEventToday(void)
{
    uint16 event_id = 0;
    //SCI_TRACE_LOW:"MMIAPISCH_IsEventToday s_user_list.total = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_6723_112_2_18_2_2_10_84,(uint8*)"d", s_user_list.total);
    if (s_user_list.total > 0)
    {
        if (!MMIAPISCH_IsAllPastEventToday(&event_id))
        {
             return TRUE;
        }
       
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : add one detail item
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void AddOneDetailItem(
                            MMI_CTRL_ID_T   ctrl_id,
                            MMI_TEXT_ID_T   label_title,
                            MMI_TEXT_ID_T   label_name,
                            uint16          *index_ptr,
                            wchar           *detail_ptr,
                            uint32          detail_len
                            )
{
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    MMI_STRING_T detail_str = {0};
    
    //set XXX title
    SetCommonCharacter(&item_data);
    MMI_GetLabelTextByLang( label_title, &detail_str );
    item_data.text_data.buf.len = detail_str.wstr_len;
    item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
    GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);
    
    //set XXX name
    if ( PNULL == label_name && PNULL!=detail_ptr )
    {
        SCI_MEMSET(&item_data,0,sizeof(item_data));
        SetCommonCharacter(&item_data);
        item_data.text_data.buf.len = detail_len;
        item_data.text_data.buf.str_ptr = detail_ptr;
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);
    }
    else if (PNULL != label_name)
    {
        SCI_MEMSET(&item_data,0,sizeof(item_data));
        SCI_MEMSET(&detail_str,0,sizeof(detail_str));
        SetCommonCharacter(&item_data);
        MMI_GetLabelTextByLang( label_name, &detail_str );
        item_data.text_data.buf.len = detail_str.wstr_len;
        item_data.text_data.buf.str_ptr = detail_str.wstr_ptr;
        GUIRICHTEXT_AddItem (ctrl_id,&item_data,index_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"AddOneDetailItem fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_6779_112_2_18_2_2_10_85,(uint8*)"");
    }
    
}

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:jian.ma
// Note: 
/*****************************************************************************/
LOCAL void SetCommonCharacter( GUIRICHTEXT_ITEM_T* p_item )
{  
    if(PNULL == p_item )
    {
        return;
    }
    
    p_item->img_type = GUIRICHTEXT_IMAGE_NONE;
    p_item->text_type = GUIRICHTEXT_TEXT_BUF;
}

/*****************************************************************************/
// 	Description : handle SCH edit windows FormSwitchActive messages
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void  HandleSchEditFormSwitchActiveMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id
                                          )
{
	switch(ctrl_id)
	{
		case MMISCH_RING_LABEL_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_SELECTED_ICON,1,TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_SELECT, STXT_RETURN, TRUE);
#endif
#endif
			break;

        case MMISCH_FREQ_SETLIST_CTRL_ID:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, TRUE);
            GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_SELECTED_ICON,1,TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_SELECT, STXT_RETURN, TRUE);
#endif			
            break;

		case MMISCH_EDITWIN_BUTTON0_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON1_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON2_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON3_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON4_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON5_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON6_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
			setSCHEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
			SCHSetEditWinButtonBg(win_id,ctrl_id);
			break;
		case MMISCH_NAME_EDIT_CTRL_ID:
#ifdef CMCC_UI_STYLE
		case MMISCH_TEXT_EDIT_CTRL_ID:
#endif
			break;
		default:
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, TRUE);	
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, TRUE);
#endif
#endif
			SCHSetEditWinButtonBg(win_id,ctrl_id);
			break;
	}
}

/*****************************************************************************/
// 	Description : Set EditWin Button bg
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SCHSetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id)
{	
    uint32 i=0;
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};

    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif    
    bg.bg_type=GUI_BG_IMG;
    for(i=0;i<SCH_WEEK_DAY_NUM;i++)
    {
        if(s_weeks_is_selected[i])
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;            
            GUIBUTTON_SetFont(MMISCH_EDITWIN_BUTTON0_CTRL_ID+i, &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;            
            GUIBUTTON_SetFont(MMISCH_EDITWIN_BUTTON0_CTRL_ID+i, &disabled_font);
        }
#ifndef MMI_PDA_SUPPORT
        if(ctrl_id==MMISCH_EDITWIN_BUTTON0_CTRL_ID+i)
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        }
#endif
        GUIBUTTON_SetBg(MMISCH_EDITWIN_BUTTON0_CTRL_ID+i, &bg);
        GUIBUTTON_Update(MMISCH_EDITWIN_BUTTON0_CTRL_ID+i);
    }	
}

/*****************************************************************************/
// 	Description : save fre
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SCHSaveFreEditWin(MMIACC_SMART_EVENT_T*	event_ptr )
{
// 	uint16      cur_num       =   0;
	uint8   week_day =0;
	int32 loop=0;
// 	uint32      i = 0; 
	uint16      mode = 0;
	
	for(loop=0;loop<SCH_WEEK_DAY_NUM;loop++)
	{
		week_day = MMIAPISET_GetWeekDayByOffset(loop);
		if(s_weeks_is_selected[loop])
		{
			mode |= s_week_mask[week_day];
		}
	}

	if (mode != event_ptr->event_fast_info.fre) 
	{
		event_ptr->event_fast_info.fre = mode & 0x00ff;
	}

	if(0==event_ptr->event_fast_info.fre)
	{
#ifdef MMI_PDA_SUPPORT
        MMIPUB_DisplayTipsIDEx(PNULL, TXT_ALARM_SELECT_ONE_DAY, MMIPUB_TIPS_DIS_MIDDLE, PNULL);
#else
		MMIPUB_OpenAlertFailWin(TXT_ALARM_SELECT_ONE_DAY);
#endif
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
// 	Description : handle SCH edit windows penok messages
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void  HandleSCHEditPENOKMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id,
                                          	MMISCH_DETAIL_WIN_PARAM_T   *win_param
                                          )
{
#ifdef MMI_PDA_SUPPORT
    MMIACC_SMART_EVENT_T *event = &win_param->event;
#endif
	switch(ctrl_id)
	{
		case MMISCH_RING_LABEL_CTRL_ID:
			MMK_CreateWin((uint32 *)SCH_EDIT_RING_WIN_TAB, (ADD_DATA)win_param);
			break;
#ifdef MMI_PDA_SUPPORT
        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
			MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
			break;
        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
			MMK_SendMsg(win_id, MSG_APP_CANCEL,  PNULL);
			break;
        case MMISCH_NAME_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubEditWin((uint32 *)MMISCH_NAMEWIN_TAB, (ADD_DATA)event);
			break;
        case MMISCH_DATE_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubFormWin((uint32 *)MMISCH_DATEWIN_TAB, (ADD_DATA)event);
			break;
        case MMISCH_TIME_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubFormWin((uint32 *)MMISCH_TIMEWIN_TAB, (ADD_DATA)event);
			break;
        case MMISCH_DEADLINE_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubFormWin((uint32 *)MMISCH_DEADLINEWIN_TAB, (ADD_DATA)event);
			break;
#endif
		case MMISCH_EDITWIN_BUTTON0_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON1_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON2_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON3_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON4_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON5_CTRL_ID:
		case MMISCH_EDITWIN_BUTTON6_CTRL_ID:
			if(s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID])
			{
				s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
			}
			else
			{
				s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
			}
#ifndef MMI_PDA_SUPPORT
			setSCHEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
			SCHSetEditWinButtonBg(win_id,ctrl_id);
#ifdef MMI_PDA_SUPPORT
            SCHSaveFreEditWin(event);
            if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMISCH_FREQ_SETLIST_CTRL_ID))
            {
                wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
                MMI_STRING_T str={0};
                str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,event->event_fast_info.fre,buff,GUILIST_STRING_MAX_NUM+1);
                str.wstr_ptr=buff;
                GUISETLIST_SetDispText(MMISCH_FREQ_SETLIST_CTRL_ID,&str,TRUE);
            }
#endif
			break;
	    
		default:
			break;
	}
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void SCHButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
	uint32 ctrl_id=MMISCH_EDITWIN_BUTTON0_CTRL_ID;
	GUI_RECT_T rect={0};//max_rect与owner_draw_ptr->display_rect相交区域，也即图片最后显示区域
	GUI_RECT_T max_rect={0};//图片可以显示的最大区域
	GUI_RECT_T img_rect={0};//图片裁剪区
	MMI_HANDLE_T win_handle = 0;
	MMI_WIN_ID_T    win_id=0;
	uint16 image_w=0;
	uint16 iamge_h=0;

	ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
	win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
	win_id=MMK_GetWinId(win_handle);
	GUIRES_GetImgWidthHeight(&image_w, &iamge_h, IMAGE_CLOCK_ALARM_WEEK_MARK, win_id);

	max_rect.left=owner_draw_ptr->display_x;
	max_rect.top=owner_draw_ptr->display_y;
	max_rect.right=max_rect.left+image_w;
	max_rect.bottom=max_rect.top+iamge_h;
	
	if(!GUI_IntersectRect(&rect,max_rect,owner_draw_ptr->display_rect))
	{//没有相交区域，返回
		return ;
	}

	img_rect.left=rect.left-max_rect.left;
	img_rect.top=rect.top-max_rect.top;
	img_rect.right=img_rect.left+(rect.right-rect.left);
	img_rect.bottom=img_rect.top+(rect.bottom-rect.top);

	if(GUI_IsInvalidRect(img_rect)||GUI_IsInvalidRect(rect))//无效矩形不要画了
	{
		return ;
	}
	
	if(s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID])
	{
		GUIRES_DisplayImg(PNULL, &rect, &img_rect, win_handle, IMAGE_CLOCK_ALARM_WEEK_MARK, &owner_draw_ptr->lcd_dev);
	}
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void setSCHEditSelectWeeksSoftkey(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id,
                                          	BOOLEAN is_update)
{
	if(ctrl_id>=MMISCH_EDITWIN_BUTTON0_CTRL_ID && ctrl_id<=MMISCH_EDITWIN_BUTTON6_CTRL_ID)
	{
		if(s_weeks_is_selected[ctrl_id-MMISCH_EDITWIN_BUTTON0_CTRL_ID])
		{
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_CANCEL_ICON,1,is_update);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_CANCEL, STXT_RETURN, is_update);
#endif
		}
		else
		{
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, TXT_NULL, STXT_RETURN, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_SELECTED_ICON,1,is_update);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SAVE, STXT_SELECT, STXT_RETURN, is_update);
#endif
		}
	}
}

/*****************************************************************************/
// 	Description : view the list of unread schedule
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSchViewUnreadListWinMsg( 
                                           MMI_WIN_ID_T		win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           )
{
    MMI_RESULT_E                recode = MMI_RESULT_TRUE;
    uint16                      i = 0;
    uint16                      index = 0;
    MMI_CTRL_ID_T               current_ctrl_id = MMISCH_VIEW_UNREAD_LIST_CTRL_ID;
    static MMISCH_DETAIL_WIN_PARAM_T    *detail_win_param = NULL;
    uint16 entry_id = 0;
    const GUILIST_ITEM_T* item_ptr = PNULL; 

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:     
        { 
            AppendUnreadEventListBoxItem(current_ctrl_id); 
            MMK_SetAtvCtrl( win_id,  current_ctrl_id);      
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            //SCI_ASSERT( PNULL != need_item_data_ptr );/*assert verified*/
            if(PNULL == need_item_data_ptr )
            {
                break;
            }
            
            item_ptr = GUILIST_GetItemPtrByIndex(current_ctrl_id,need_item_data_ptr->item_index);
            //SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
            if(PNULL == item_ptr )
            {
                break;
            }
            entry_id = item_ptr->user_data;

            AppendUnreadEventListBoxItemData(current_ctrl_id,entry_id,need_item_data_ptr->item_index);
        }
        break;  
      
    case MSG_GET_FOCUS:
        MMK_PostMsg(win_id, MSG_SCH_LIST_UPDATE, PNULL, 0);
        break;
	
    case MSG_SCH_LIST_UPDATE:
        i = GUILIST_GetCurItemIndex(current_ctrl_id);	       
        GUILIST_RemoveAllItems( current_ctrl_id);
        AppendUnreadEventListBoxItem(current_ctrl_id);
        GUILIST_SetCurItemIndex(current_ctrl_id,i); 
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
       
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        item_ptr = GUILIST_GetItemPtrByIndex(current_ctrl_id,GUILIST_GetCurItemIndex(current_ctrl_id));
        //SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
        if(PNULL == item_ptr )
        {
            break;
        }
        
        index = item_ptr->user_data;
        if(s_unRead_event_list.total==0)
        {
            break;
        }
        if(index<=EVENT_ALARM_MAX)
        {
            MMIAPIALARM_OpenEditAlarmWinbyEventid(index);
        }
        else
        {
            if(NULL != detail_win_param)
            {
                SCI_FREE(detail_win_param);
            }
            detail_win_param = SCI_ALLOC_APP(sizeof(MMISCH_DETAIL_WIN_PARAM_T));
            if(detail_win_param != PNULL)
            {
                SCI_MEMSET(detail_win_param, 0, sizeof(MMISCH_DETAIL_WIN_PARAM_T));
                
                detail_win_param->win_id = win_id;
                detail_win_param->index = index;

                MMIAPIALM_FormInteEventInfo(index,&detail_win_param->event);
                //MMIAPIALM_GetRingInfo(index,&detail_win_param->event);
                if(!MMIAPIALM_GetRingInfo(index,&detail_win_param->event))
                {
                    detail_win_param->event.event_content_info.ring_type = ALM_FIX_RING;
                    detail_win_param->event.event_content_info.ring = 1;
                    }
                MMK_CreateWin((uint32 *)SCH_VIEW_UNREAD_DETAIL_WIN_TAB, (ADD_DATA)detail_win_param);
                MMIAPISCH_DeleteUnreadEvent(index);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        if(NULL != detail_win_param)
        {
            SCI_FREE(detail_win_param);
            detail_win_param=PNULL;
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// 	Description : append unread event list to ctrl
//	Global resource dependence : 
//  Author:  renyi.HU
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUnreadEventListBoxItem(
                                 MMI_CTRL_ID_T	ctrl_id
                                 )
{
    int16 i = 0;
    GUILIST_ITEM_T			item_t= {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data= {0};/*lint !e64*/
		
    if(s_unRead_event_list.total==0)
	{
		GUILIST_SetMaxItem( ctrl_id, 1, FALSE );//max item 1
		//item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
		item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
        //set softkey
        item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT  
		item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#else
        item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
#endif
        item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;

        item_data.item_content[0].item_data.text_id = TXT_BLANK;
		item_t.item_data_ptr = &item_data;
        GUILIST_AppendItem(ctrl_id, &item_t);
		return;
	}
	else
	{
		GUILIST_SetMaxItem( ctrl_id, s_unRead_event_list.total, TRUE );//max item 100
        
		item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
        item_t.item_data_ptr = &item_data;
		
		for( i = 0; i < s_unRead_event_list.total; i++ )
		{
			item_t.user_data = s_unRead_event_list.entry_id[i];
			GUILIST_AppendItem(ctrl_id, &item_t);  
		}
	}
 
    return;
}
/*****************************************************************************/
// 	Description : append event list to ctrl
//	Global resource dependence : 
//  Author:  renyi.hu
//  RETRUN:   
//	Note:   
/*****************************************************************************/
LOCAL void AppendUnreadEventListBoxItemData(
                                 MMI_CTRL_ID_T	ctrl_id,
								 uint16 entry_id,
								 uint32	index
                                 )
{
    GUILIST_ITEM_T			item_t= {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data= {0};  /*lint !e64*/
//    const MMIACC_SMART_EVENT_T* event_ptr = MMIALM_GeEvent(entry_id);
    uint8  item_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    wchar  item_wstr[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    MMIACC_SMART_EVENT_T    smart_event = {0};
    uint32 image_id=IMAGE_SCHEDULE_ARRANGE_01;    
    wchar	str_content[MMIACC_EVENT_TEXT_MAX_LEN+1] = {0};	

    if(entry_id<=EVENT_ALARM_MAX)
    {
        image_id=IMAGE_ALARM_ARRANGE;
    }

    MMIAPIALM_FormInteEventInfo(entry_id,&smart_event);
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    if (0 == smart_event.event_content_info.image_id )
    {
        smart_event.event_content_info.image_id = image_id;
    }
    item_data.item_content[0].item_data.image_id = smart_event.event_content_info.image_id;
    
    item_data.item_content[1].item_data_type 
        = item_data.item_content[3].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

	MMIAPISET_FormatTimeStrByTime(smart_event.event_fast_info.hour,smart_event.event_fast_info.minute,item_str,MMIALM_TIME_STR_12HOURS_LEN + 1);

    item_data.item_content[1].item_data.text_buffer.wstr_len = SCI_STRLEN((char*)item_str);

    item_data.item_content[1].item_data.text_buffer.wstr_ptr = item_wstr;

    MMI_STRNTOWSTR( item_wstr,
        item_data.item_content[1].item_data.text_buffer.wstr_len,
        item_str,
        strlen((char*)item_str),
        strlen((char*)item_str));
    
    if(entry_id<=EVENT_ALARM_MAX)
    {
        item_data.item_content[3].item_data.text_buffer.wstr_ptr = str_content;
        item_data.item_content[3].item_data.text_buffer.wstr_len= MMIACC_EVENT_TEXT_MAX_LEN;
        MMIAPIALM_GetAlarmName(entry_id,&(item_data.item_content[3].item_data.text_buffer));
    }
    else
    {
        if (0 !=smart_event.event_content_info.w_length)
        {
            item_data.item_content[3].item_data.text_buffer.wstr_len = smart_event.event_content_info.w_length;
            item_data.item_content[3].item_data.text_buffer.wstr_ptr = smart_event.event_content_info.w_content;
        }
        else
        {   
            item_data.item_content[3].item_data_type= GUIITEM_DATA_TEXT_ID;
            item_data.item_content[3].item_data.text_id = TXT_BLANK;
        }
    }

    
    item_data.softkey_id[0] = TXT_VIEW;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
	item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#else
    item_data.softkey_id[1] = TXT_NULL;
#endif
    item_data.softkey_id[2] = STXT_RETURN;
    
	GUILIST_SetItemData( ctrl_id, &item_data, index );      
	
}

/*****************************************************************************/
// 	Description : view unread sch detail
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewSchUnReadDetailWinMsg( 
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
//     uint32 i                                    = 0;
//     uint16 index                                = 0;
    MMI_RESULT_E	            recode          = MMI_RESULT_TRUE;
static    MMISCH_DETAIL_WIN_PARAM_T   *s_win_param = PNULL;
//     static MMISCH_DETAIL_WIN_PARAM_T   s_pass_win_param = {0};    
//     GUI_LCD_DEV_INFO	        lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_win_param = (MMISCH_DETAIL_WIN_PARAM_T *) MMK_GetWinAddDataPtr(win_id); 
        if(PNULL == s_win_param)
        {
        return FALSE;
        }              
        MMK_SetAtvCtrl(win_id, MMISCH_VIEW_DETAIL_TEXT_CTRL_ID);
        InitViewSchDetail_se(&(s_win_param->event), win_id,MMISCH_VIEW_DETAIL_TEXT_CTRL_ID);
        GUIRICHTEXT_SetFocusMode(MMISCH_VIEW_DETAIL_TEXT_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_NONE);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
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
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISCH_AddUnreadEvent(uint16 event_id)
{
	uint16 i=0;

	if ((event_id>EVENT_ALARM_MAX && event_id <EVENT_SCH_0)||(event_id >EVENT_SCH_MAX))
	{
		return ;
	}
	if(s_unRead_event_list.total>=MMIACC_SCH_NUM+MMIACC_ALARM_NUM)
	{
        return ;
    }
	for(i=0;i<s_unRead_event_list.total;i++)
	{
		if(s_unRead_event_list.entry_id[i]==event_id)
		{
			return;
		}
	}
    s_unRead_event_list.entry_id[s_unRead_event_list.total]=event_id;
    s_unRead_event_list.total++;

#ifdef PDA_UI_DROPDOWN_WIN
    if (event_id>=EVENT_SCH_0 && event_id<=EVENT_SCH_MAX )
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_UNREAD_CALENDAR);
    }
    else
    {
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_UNREAD_ALARM);        
    }	
#endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPISCH_DeleteUnreadEvent(uint16 event_id)
{
    uint16 i=0;
    int index=-1;

    SCI_TRACE_LOW("MMIAPISCH_DeleteUnreadEvent in:event_id =%d,total =%d",event_id,s_unRead_event_list.total);

    if(0==s_unRead_event_list.total)
    {
        return ;
    }

    if ((event_id>EVENT_ALARM_MAX && event_id <EVENT_SCH_0)||(event_id >EVENT_SCH_MAX))
    {
        return ;
    }
	
    for(i=0;i<s_unRead_event_list.total;i++)
    {
        if(s_unRead_event_list.entry_id[i] == event_id)
        {
            index=i;
            break;
        }
    }

    if(index!=-1)
    {
        for(i=index;i<s_unRead_event_list.total-1;i++)
        {
            s_unRead_event_list.entry_id[i]=s_unRead_event_list.entry_id[i+1];
        }

        s_unRead_event_list.total--;
    }
    
    SCI_TRACE_LOW("MMIAPISCH_DeleteUnreadEvent out:index =%d,total =%d",index,s_unRead_event_list.total);

}

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC uint32 MMIAPISCH_GetTotalTimesOfUnreadEvent(void)
{
    return s_unRead_event_list.total;
}

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenUnreadEventWin(void)
{
    MMK_CreateWin((uint32 *)SCH_VIEW_UNREAD_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC MMISET_DATE_T* MMIAPISCH_GetAllListCurData(void)
{
    return &s_all_list_cur_date;
}

/*****************************************************************************/
// 	Description :  
//	Global resource dependence : 
//  Author:      renyi.hu
//	Note:            
/*****************************************************************************/
PUBLIC void MMIAPISCH_SetAllListCurData(uint8 date_day, uint8  date_month,uint16  date_year)
{
    s_all_list_cur_date.date_day=date_day;
	s_all_list_cur_date.date_month=date_month;
	s_all_list_cur_date.date_year=date_year;
}
#ifdef MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : 判断保存文件所需空间是否不足
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckDiskSpaceForSaveVCalendar(MMIFILE_DEVICE_E file_dev)
{
    BOOLEAN    bRet = TRUE;
    uint32     space = 0;
    if(MMI_DEVICE_NUM > file_dev)
    {
        space = MMI_GetFreeSpace(file_dev);
        if ((MMIVCAL_MIN_NEED_SPACE >> 10) > space)
        {
            bRet = FALSE;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}
/*****************************************************************************/
//  Description :Send Vcalendar By BT
//  Global resource dependence : none
//  Author: 
//  Note: renyi.hu
/***************************************************************************/
LOCAL void SendVcalendarByBT(uint16* event_id,uint16 num)
{
#ifdef BLUETOOTH_SUPPORT
    MMIACC_SMART_EVENT_T event={0};
    //MMIFILE_FILE_INFO_T      file_info = {0};
    MMIFMM_BT_SENDFILE_INFO_T *send_file_info = PNULL;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
	SFS_ERROR_E  file_ret  = SFS_ERROR_NONE;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_WIN_ID_T  alert_id =MMISCH_ALART_WIN_ID;
    int i=0;
	//int j=0;
    
	if(num==0)
	{
		return;
	}

	send_file_info=(MMIFMM_BT_SENDFILE_INFO_T*)SCI_ALLOC_APP(num*sizeof(MMIFMM_BT_SENDFILE_INFO_T));    	
	if(PNULL==send_file_info)
	{
		return ;  
	}
    
    //send by Bluetooth
    if (MMIAPIUDISK_UdiskIsRun())
    {
        //udis is run
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id, PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
       
        file_dev = MMIAPIFMM_GetFirstValidDevice();
        // 判断保存文件所需空间是否不足
        if (!CheckDiskSpaceForSaveVCalendar(file_dev))
        {
            if(!MMIAPIFMM_GetDeviceTypeStatus((file_dev)))
            {
                //device selected doesn't exist!
                text_id = MMIAPIFMM_GetAlertTextIDByDev((file_dev));
            }
            else
            {
                text_id = TXT_NO_SPACE;
            }

            MMIPUB_OpenAlertWinByTextId(PNULL,
                                        text_id,
                                        TXT_NULL,
                                        IMAGE_PUBWIN_WARNING,
										&alert_id,
                                        PNULL,
                                        MMIPUB_SOFTKEY_ONE,
                                        PNULL);
        }
        else
        {
    		for(i=0;i<num;i++)
    		{
    			SCI_MEMSET(&event,0,sizeof(MMIACC_SMART_EVENT_T));
    			MMIAPIALM_FormInteEventInfo(event_id[i],&event);
    			if(MMIVCAL_VcalBuildInit())
    			{
    				MMIVCAL_BuildVcalendarByEvent(&event,MMIVCAL_OBJECT_TYPE_VEVENT);
    			}
    			if(MMIVCAL_InitFileNamePtr())
    			{
    				MMIVCAL_GetVcalendarFileName(&event);
    			}
    			SCI_MEMSET(send_file_info[i].filepath_name , 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));/*lint !e605*/
                
    			file_ret = MMIVCAL_SaveVcalFileSync(file_dev, send_file_info[i].filepath_name);/*lint !e605*/
    			MMIVCAL_VcalBuildExit();
    			MMIVCAL_FreeFileNamePtr();
    		}
            if(SFS_ERROR_NONE != file_ret)
            {
                // 提示信息
		        MMIAPIFMM_PromptFileErrorType(PNULL, file_ret, file_dev);
            }
            else
            {
                //save success
    			for(i=0;i<num;i++)
    			{
    				send_file_info[i].filepath_len = MMIAPICOM_Wstrlen(send_file_info[i].filepath_name);
    				send_file_info[i].is_temp_file = TRUE;
    				MMIAPIFMM_GetFileInfo( send_file_info[i].filepath_name,
    				send_file_info[i].filepath_len,
    				&send_file_info[i].file_size,
    				NULL, NULL
    				);
    			}
			    MMIAPIBT_SendMultiFile(send_file_info,num); 
           }          
       }
    }
	
    SCI_FREE(send_file_info);  
    		

#endif
}

/*****************************************************************************/
//  Description :Send Vcalendar By MMS
//  Global resource dependence : none
//  Author: 
//  Note: renyi.hu
/***************************************************************************/
LOCAL void SendVcalendarByMMS(uint16 event_id)
{
#ifdef MMS_SUPPORT
    MMIACC_SMART_EVENT_T event={0};
    MMIFILE_FILE_INFO_T      file_info = {0};
    wchar* full_file_name= PNULL;
    //wchar *vcard_name_ptr = PNULL;
    uint16 full_file_len = 0;
   // static BOOLEAN  is_vcard = FALSE;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
	SFS_ERROR_E  file_ret  = SFS_ERROR_NONE;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_WIN_ID_T  alert_id =MMISCH_ALART_WIN_ID ;

    full_file_name=(wchar*)SCI_ALLOC_APP((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
    if(PNULL==full_file_name)
    {
      return ;  
    }
    //send by Bluetooth
    if (MMIAPIUDISK_UdiskIsRun())
    {
        //udis is run
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id, PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
       
        file_dev = MMIAPIFMM_GetFirstValidDevice();
        // 判断保存文件所需空间是否不足
        if (!CheckDiskSpaceForSaveVCalendar(file_dev))
        {
            if(!MMIAPIFMM_GetDeviceTypeStatus((file_dev)))
            {
                //device selected doesn't exist!
                text_id = MMIAPIFMM_GetAlertTextIDByDev((file_dev));
            }
            else
            {
                text_id = TXT_NO_SPACE;
            }

            MMIPUB_OpenAlertWinByTextId(PNULL,
                                        text_id,
                                        TXT_NULL,
                                        IMAGE_PUBWIN_WARNING,
										&alert_id,
                                        PNULL,
                                        MMIPUB_SOFTKEY_ONE,
                                        PNULL);
        }
        else
        {
            MMIAPIALM_FormInteEventInfo(event_id,&event);
    		if(MMIVCAL_VcalBuildInit())
    		{
    			MMIVCAL_BuildVcalendarByEvent(&event,MMIVCAL_OBJECT_TYPE_VEVENT);
    		}
            if(MMIVCAL_InitFileNamePtr())
            {
                MMIVCAL_GetVcalendarFileName(&event);
            }
            SCI_MEMSET(full_file_name , 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));
			
			file_ret = MMIVCAL_SaveVcalFileSync(file_dev, full_file_name);
            MMIVCAL_VcalBuildExit();
            MMIVCAL_FreeFileNamePtr();
            if(SFS_ERROR_NONE != file_ret)
            {
                // 提示信息
				MMIAPIFMM_PromptFileErrorType(PNULL, file_ret, file_dev);
            }
            else
            {
                //save success
                full_file_len = MMIAPICOM_Wstrlen(full_file_name);


                MMIAPIFMM_GetFileInfoFormFullPath(full_file_name, full_file_len, &file_info);
                MMIAPIMMS_EditMMSFromFile(&file_info, MMIFMM_FILE_TYPE_VCALENDAR);/*lint !e64*/
            }          
         }
    }

    SCI_FREE(full_file_name);

#endif
}
/*****************************************************************************/
//  Description :Send Vcalendar By SMS 
//  Global resource dependence : none
//  Author: 
//  Note: renyi.hu
/***************************************************************************/
LOCAL void SendVcalendarBySMS(uint16 event_id)
{
    uint32 vcal_len = 0; 
    MMIACC_SMART_EVENT_T event={0};
    static MMIVC_SMS_SEND_TYPE_E  send_type = MMIVC_SMS_SEND_NORMAL;
    MMI_WIN_ID_T  alert_id =MMISCH_ALART_WIN_ID ;

    MMIAPIALM_FormInteEventInfo(event_id,&event);

    MMIAPISMS_ClearDestNum();
    if(MMIVCAL_VcalBuildInit())
	{
		MMIVCAL_BuildVcalendarByEvent(&event,MMIVCAL_OBJECT_TYPE_VEVENT);
	}
    vcal_len = strlen((char*)MMIVCAL_GetBuildData());
    if((MMISCH_SMS_VCALENDAR_MAX_8BIT_SPLIT_LEN*MMISMS_SPLIT_MAX_NUM) >= vcal_len)
    {
        send_type = MMIVC_SMS_SEND_VCALENDAR;
        MMIAPISMS_EnterSendOptionMenu((uint32)send_type);/*lint !e746 !e718*/        
    }
    else
    {
        //data too large
        MMIPUB_OpenAlertWinByTextId(PNULL,
                                    TXT_COMMON_BEYOND_LEN,
                                    TXT_NULL,
                                    IMAGE_PUBWIN_WARNING,
                                    &alert_id,
                                    PNULL,
                                    MMIPUB_SOFTKEY_ONE,
                                    PNULL);
        MMIVCAL_VcalBuildExit();          
    }
}
/*****************************************************************************/
// Description : 解析VCalendar文件,并保存到SCH中
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISCH_ReadVCalendarData(uint8  *data_ptr, uint32 data_size)
{
    BOOLEAN ret = FALSE;
    VF_OBJECT_T *p_object = NULL;
    VF_OBJECT_T *pTmp;

    //SCI_TRACE_LOW:"MMIAPISCH_ReadVCalendarData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_7743_112_2_18_2_2_12_86,(uint8*)"");
    if( PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPISCH_ReadVCalendarData PNULL == data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_7746_112_2_18_2_2_12_87,(uint8*)"");
        return;
    }                       
    //开始解析
    //SCI_TRACE_LOW:"MMIAPISCH_ReadVCalendarData: start parser"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_7750_112_2_18_2_2_12_88,(uint8*)"");
    ret = MMIVCAL_parse_by_data(&p_object, data_ptr, data_size);
    if (ret && PNULL != p_object)
    {
        pTmp = p_object;
        if (!vf_get_next_object(&pTmp))
        {
            if (!MMIVCAL_VcalendarIsEmpty(p_object))
            {
                MMIVCAL_ObjectToVcalInfo(p_object, &s_vcal_info);
				MMK_CreateWin((uint32*)MMISCH_VCALENDAR_DETAIL_WIN_TAB, &s_vcal_info);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        }
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
        vf_delete_object(p_object, TRUE);
#endif
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
    }

    return;
}

/*****************************************************************************/
// Description : 解析VCalendar文件,并保存到SCH中
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISCH_ReadVcalendarFile(uint16  *name_ptr)
{
    MMIVCAL_ERROR_TYPE_E ret = MMIVCAL_NO_ERROR;
    VF_OBJECT_T *p_object = NULL;
    VF_OBJECT_T *pTmp;
    //MMIVCALENDAR_INFO_T vcal_info={0};
	MMI_TEXT_ID_T    alert_text = TXT_NULL; 
    //MMIPB_CONTACTS_DETAILS *detail_handle = PNULL;
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;

    //SCI_TRACE_LOW:"mmivcalendar: MMIAPISCH_ReadVcalendarFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_7797_112_2_18_2_2_12_89,(uint8*)"");
    if( PNULL == name_ptr)
    {
        //SCI_TRACE_LOW:"mmivcalendar MMIAPISCH_ReadVcalendarFile PNULL == name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_7800_112_2_18_2_2_12_90,(uint8*)"");
        return;
    }

	file_dev = MMIAPIFMM_GetDeviceTypeByPath(name_ptr, MMIFILE_DEVICE_LEN);
	if(!MMIAPIFMM_CheckMemoryCard(file_dev))
	{
		return;
	}                      

    //SCI_TRACE_LOW:"MMIAPISCH_ReadVcalendarFile: start parser"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_7810_112_2_18_2_2_12_91,(uint8*)"");
    ret = MMIVCAL_ReadFile(&p_object, name_ptr);
    if ((MMIVCAL_NO_ERROR == ret) && PNULL != p_object)
    {
        pTmp = p_object;
        if (!vf_get_next_object(&pTmp))
        {
            if (!MMIVCAL_VcalendarIsEmpty(p_object))
            {
                MMIVCAL_ObjectToVcalInfo(p_object, &s_vcal_info);
				MMK_CreateWin((uint32*)MMISCH_VCALENDAR_DETAIL_WIN_TAB, &s_vcal_info);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        }
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
        vf_delete_object(p_object, TRUE);
#endif
    }
    else
    {
        //解析失败
		if(MMIVCAL_IS_TOOBIG== ret)
		{
			alert_text = TXT_FILE_TOO_LARGE;
		}
		else if(MMIVCAL_NO_SUPPORT == ret)
		{
			alert_text = TXT_COMMON_NO_SUPPORT;
		}
		else if(MMIVCAL_IS_EMPTY == ret)
		{
			alert_text = TXT_EMPTY_FILE;
		}
		else
		{
			alert_text = TXT_ERROR;
		}
        MMIPUB_OpenAlertWarningWin(alert_text);
    }

    return;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:  renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewVcalendarDetailWinMsg( 
                                             MMI_WIN_ID_T		win_id, 
                                             MMI_MESSAGE_ID_E	msg_id, 
                                             DPARAM				param
                                             )
{
    //uint32 i                                    = 0;
    MMI_RESULT_E	            recode          = MMI_RESULT_TRUE;
    static MMIVCALENDAR_INFO_T *vcal_info  = PNULL; 
    MMIACC_SMART_EVENT_T event_info={0};
    MMIVCAL_ERROR_TYPE_E result=MMIVCAL_NO_ERROR;
    uint16 index=0;
    wchar *buff=PNULL;


    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        vcal_info =(MMIVCALENDAR_INFO_T *) MMK_GetWinAddDataPtr(win_id); 
        buff=SCI_ALLOC_APP(MMISCH_VCALENDAR_DETAIL_MAX_LEN*sizeof(wchar));
        if(buff!=PNULL)
        {
            SCI_MEMSET(buff,0,MMISCH_VCALENDAR_DETAIL_MAX_LEN*sizeof(wchar));
            GetVcalendarDetailStr(vcal_info,buff,MMISCH_VCALENDAR_DETAIL_MAX_LEN);
            GUITEXT_SetString(MMISCH_VCALENDAR_DETAIL_TEXT_CTRL_ID, buff, MMIAPICOM_Wstrlen(buff), FALSE);
            SCI_FREE(buff);
        }
        MMK_SetAtvCtrl(win_id, MMISCH_VCALENDAR_DETAIL_TEXT_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;		
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    if (PNULL != param)
    {
        uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
        
        switch (src_id)
        {
        case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
            MMK_SendMsg(win_id, MSG_CTL_OK, PNULL);
            break;          
        default:
            break;
        }

    }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif        
    case MSG_APP_OK:	
    case MSG_CTL_OK:
        result=MMIVCAL_VcalInfoToSchedule(vcal_info,&event_info);
        if(MMIVCAL_NO_ERROR==result)
    	{
    		MMIALM_AddOneEvent(&event_info,&index);
            if(0==index)
        	{
        		result=MMIVCAL_SCH_IS_FULL;
        	}
    	}
        SaveVcalendarprompt(result);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIVCAL_FreeVcalInfo(vcal_info);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void  SaveVcalendarprompt(MMIVCAL_ERROR_TYPE_E error_tyep)
{
    MMI_TEXT_ID_T    alert_text = TXT_NULL;
    MMI_IMAGE_ID_T    image_id=IMAGE_PUBWIN_WARNING;
    
    switch(error_tyep)
    {
        case MMIVCAL_NO_ERROR:
            alert_text =TXT_COMM_SAVE_SUCCESS;
            image_id=IMAGE_PUBWIN_SUCCESS;
            break;
        case MMIVCAL_IS_TOOBIG:
            alert_text = TXT_FILE_TOO_LARGE;
            break;
        case MMIVCAL_NO_SUPPORT:
        case MMIVCAL_INVALID_DATETIME:
        case MMIVCAL_PARSE_FAIL:
        case MMIVCAL_IS_NESTED:
            alert_text = TXT_COMMON_NO_SUPPORT;
            break;
        case MMIVCAL_IS_EMPTY:
            alert_text = TXT_EMPTY_FILE;
            break;
        case MMIVCAL_SCH_IS_FULL:
            alert_text = TXT_SCHEDULE_FULL;
            break;
        default:
            alert_text = TXT_ERROR;
            image_id=IMAGE_PUBWIN_FAIL;
            break;
    }
#ifdef  MMI_PDA_SUPPORT
    if(MMIVCAL_NO_ERROR==error_tyep)
    {
        return;
    }
#endif
    MMIPUB_OpenAlertWinByTextId(PNULL,alert_text,TXT_NULL,
                                    image_id,PNULL,PNULL,
                                    MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL uint16  GetVcalendarDetailStr(MMIVCALENDAR_INFO_T *vcal_info,wchar* buff,uint16 max_buff_len)
{
    MMI_STRING_T string={0};
    wchar enter[]={0x0d,0x0a,0};
    wchar colon[]={0x3a,0};//:
    uint16 buff_len=0;
    MMISET_TIME_T time={0};
    uint8	temp_str[MMISET_DATE_STR_LEN] = {0};
    wchar   wchar_temp_str[MMISET_DATE_STR_LEN +1] = {0};
    
    if(vcal_info==PNULL || buff==PNULL)
    {
        return buff_len;
    }
    //类型
    if(vcal_info->categories.wstr_len>0)
    {
        buff_len=MMIAPICOM_Wstrlen(buff);
        MMI_GetLabelTextByLang( TXT_TYPE, &string );
        MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, vcal_info->categories.wstr);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, enter);
    }

    //类别
    if(vcal_info->classification.wstr_len>0)
    {
        buff_len=MMIAPICOM_Wstrlen(buff);
        SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_SCH_CLASSIFICATION, &string );
        if(buff_len+string.wstr_len+vcal_info->classification.wstr_len+6>max_buff_len)
        {
            return buff_len;
        }
        MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, vcal_info->classification.wstr);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, enter);
    }
    
    //主题
    if(vcal_info->summary.wstr_len>0)
    {
        buff_len=MMIAPICOM_Wstrlen(buff);
        SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_SET_THEME, &string );
        if(buff_len+string.wstr_len+vcal_info->summary.wstr_len+7>max_buff_len)
        {
            return buff_len;
        }
        //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
        MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
        MMIAPICOM_Wstrcat(buff, colon);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, vcal_info->summary.wstr);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, enter);
    }
    
    //描述
    if(vcal_info->description.wstr_len>0)
    {
        buff_len=MMIAPICOM_Wstrlen(buff);
        SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_SCH_DESCRIPTION, &string );
        if(buff_len+string.wstr_len+vcal_info->description.wstr_len+6>max_buff_len)
        {
            return buff_len;
        }
        //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
        MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, vcal_info->description.wstr);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, enter);
    }

    //状态
    if(vcal_info->status.wstr_len>0)
    {
        buff_len=MMIAPICOM_Wstrlen(buff);
        SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
        MMI_GetLabelTextByLang(TXT_COMMON_STATE, &string );
        if(buff_len+string.wstr_len+vcal_info->status.wstr_len+7>max_buff_len)
        {
            return buff_len;
        }
        //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
        MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
        MMIAPICOM_Wstrcat(buff, colon);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, vcal_info->status.wstr);
        MMIAPICOM_Wstrcat(buff, enter);
        MMIAPICOM_Wstrcat(buff, enter);
    }
    
    //时间
    if(MMIVCAL_OBJECT_TYPE_VEVENT==vcal_info->object_type)
    {
        time.time_hour=vcal_info->dtstart.date_time.hour;
        time.time_minute=vcal_info->dtstart.date_time.min;
        if(MMICALENDAR_IsDateValid(vcal_info->dtstart.date_time.year,vcal_info->dtstart.date_time.mon,vcal_info->dtstart.date_time.mday)
            &&MMIAPICOM_CheckTimeIsValid(time))
        {
            //开始日期
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_SCH_START_DATE, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str)); 
            MMIAPISET_FormatDateStrByDateStyle(vcal_info->dtstart.date_time.year,vcal_info->dtstart.date_time.mon,vcal_info->dtstart.date_time.mday,
            '.',temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
           // MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);
            
            //开始时间
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_SCH_START_TIME, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));    
        	MMIAPISET_FormatTimeStrByTime(vcal_info->dtstart.date_time.hour,vcal_info->dtstart.date_time.min,temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
            //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);

            
        }

        time.time_hour=vcal_info->dtend.date_time.hour;
        time.time_minute=vcal_info->dtend.date_time.min;
        if(MMICALENDAR_IsDateValid(vcal_info->dtend.date_time.year,vcal_info->dtend.date_time.mon,vcal_info->dtend.date_time.mday)
            &&MMIAPICOM_CheckTimeIsValid(time))
        {
            //截止日期
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALENDAR_DEADLINE, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str)); 
            MMIAPISET_FormatDateStrByDateStyle(vcal_info->dtend.date_time.year,vcal_info->dtend.date_time.mon,vcal_info->dtend.date_time.mday,
            '.',temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
            //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);
            
            //截止时间
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALENDAR_DEADLINE_TIME, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));    
        	MMIAPISET_FormatTimeStrByTime(vcal_info->dtend.date_time.hour,vcal_info->dtend.date_time.min,temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
            //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);

            
        }
    }
    else if(MMIVCAL_OBJECT_TYPE_VTODO==vcal_info->object_type)
    {
        time.time_hour=vcal_info->due.date_time.hour;
        time.time_minute=vcal_info->due.date_time.min;
        if(MMICALENDAR_IsDateValid(vcal_info->due.date_time.year,vcal_info->due.date_time.mon,vcal_info->due.date_time.mday)
            &&MMIAPICOM_CheckTimeIsValid(time))
        {
            //预期日期
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALENDAR_DUE_DATE, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str)); 
            MMIAPISET_FormatDateStrByDateStyle(vcal_info->due.date_time.year,vcal_info->due.date_time.mon,vcal_info->due.date_time.mday,
            '.',temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
            //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);
            
            //预期时间
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALENDAR_DUE_TIME, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));    
        	MMIAPISET_FormatTimeStrByTime(vcal_info->due.date_time.hour,vcal_info->due.date_time.min,temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
            //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);

            
        }

        time.time_hour=vcal_info->completed.date_time.hour;
        time.time_minute=vcal_info->completed.date_time.min;
        if(MMICALENDAR_IsDateValid(vcal_info->completed.date_time.year,vcal_info->completed.date_time.mon,vcal_info->completed.date_time.mday)
            &&MMIAPICOM_CheckTimeIsValid(time))
        {
            //实际日期
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALENDAR_COMPLETED_DATE, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str)); 
            MMIAPISET_FormatDateStrByDateStyle(vcal_info->completed.date_time.year,vcal_info->completed.date_time.mon,vcal_info->completed.date_time.mday,
            '.',temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
            //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);
            
            //实际时间
            buff_len=MMIAPICOM_Wstrlen(buff);
            
            SCI_MEMSET(&string,0,sizeof(MMI_STRING_T));
            MMI_GetLabelTextByLang(TXT_CALENDAR_COMPLETED_TIME, &string );
            
            SCI_MEMSET(temp_str,0,sizeof(temp_str));
            SCI_MEMSET(wchar_temp_str,0,sizeof(wchar_temp_str));    
        	MMIAPISET_FormatTimeStrByTime(vcal_info->completed.date_time.hour,vcal_info->completed.date_time.min,temp_str,MMISET_DATE_STR_LEN);
            MMIAPICOM_StrToWstr(temp_str,wchar_temp_str );
            if(buff_len+MMIAPICOM_Wstrlen(wchar_temp_str)+string.wstr_len+6>max_buff_len)
            {
                return buff_len;
            }
            //MMIAPICOM_Wstrcat(buff, string.wstr_ptr);
            MMIAPICOM_Wstrncpy(buff+buff_len, string.wstr_ptr,string.wstr_len);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, wchar_temp_str);
            MMIAPICOM_Wstrcat(buff, enter);
            MMIAPICOM_Wstrcat(buff, enter);

            
        }
    }
    
    buff_len=MMIAPICOM_Wstrlen(buff);
    return buff_len;
}

/*****************************************************************************/
//  Description : 选择导出的目录
//  Global resource dependence : none
//  Note: 
/***************************************************************************/
LOCAL void SelectVcalendarExportFolder(MMI_CTRL_ID_T ctrl_id)
{
	FILEARRAY ret_array = {0};
	MMIFMM_FILTER_T filter = {0};

    s_cur_vcal_list_id=ctrl_id;

	SCI_MEMCPY(filter.filter_str, "*", strlen("*")); /*lint !e605 !e666*/
	//get directory from FMM
	ret_array = MMIAPIFILEARRAY_Create();          
	if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL, 0, &filter, FUNC_FIND_FOLDER, FALSE, NULL, ret_array, VcalendarFolderSelectionCallBack,FALSE))
	{
        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
    	GUILIST_SetAllSelected(ctrl_id, FALSE);
		MMIAPIFILEARRAY_Destroy(&ret_array);
	}
}

/*****************************************************************************/
//  Description : 选择导出的目录 callback
//  Global resource dependence : none
//  Note: 
/***************************************************************************/
LOCAL void VcalendarFolderSelectionCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
    uint32  file_num = 0;
    BOOLEAN is_saved = FALSE;
    FILEARRAY_DATA_T *file_array_data_ptr = PNULL;
	if(is_success)
	{
		file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        //SCI_TRACE_LOW:"VcalendarFolderSelectionCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_8344_112_2_18_2_2_13_92,(uint8*)"d", file_num);
        if(file_num > 0)
        {
            //read user selected dir
			file_array_data_ptr = (FILEARRAY_DATA_T *)SCI_ALLOC_APP(sizeof(FILEARRAY_DATA_T));
            if(PNULL!=file_array_data_ptr)
            {
                SCI_MEMSET(file_array_data_ptr,0,sizeof(FILEARRAY_DATA_T));
                if(MMIAPIFILEARRAY_Read(file_array, 0, file_array_data_ptr))
    			{
                      if(s_vcalendar_export_folder_ptr != PNULL)
    				  {
    					  SCI_FREE(s_vcalendar_export_folder_ptr);
                          s_vcalendar_export_folder_ptr=PNULL;
    				  }
                      
                      s_vcalendar_export_folder_ptr = (wchar *)SCI_ALLOC_APP(MMIFMM_PATHNAME_LEN*sizeof(wchar));
                      
    				  if(s_vcalendar_export_folder_ptr != PNULL)
    				  {
    					  SCI_MEMSET(s_vcalendar_export_folder_ptr, 0x00, MMIFMM_PATHNAME_LEN*sizeof(wchar));
    					  MMIAPICOM_Wstrncpy(s_vcalendar_export_folder_ptr, file_array_data_ptr->filename, file_array_data_ptr->name_len);
    					  HandleSaveVCalendar(&s_operate_sch_info, s_vcalendar_export_folder_ptr);
    					  is_saved= TRUE;
    				  }
    			}
    			SCI_FREE(file_array_data_ptr);
            }
		}
	}
	if(!is_saved)
	{
        GUILIST_SetTextListMarkable(s_cur_vcal_list_id, FALSE);
    	GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
	}
	MMIAPIFILEARRAY_Destroy(&file_array);
}

/*****************************************************************************/
//  Description : 处理保存VCalendar
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSaveVCalendar(MMISCH_OPERATE_RECORDS_INFO_T *info_ptr, const wchar *path_name_ptr)
{
    MMI_WIN_ID_T win_id=MMISCH_VCALENDAR_PROGRESS_WIN_ID;
    MMIPUB_HANDLE_FUNC handle_func=HandleSchWaitWinMsg;
    //info_ptr->total_count = MMIPB_GetTotalOperateCount();
    info_ptr->done_count = 0;
	info_ptr->failed_count = 0;
    //g_mmipb_process_t.is_add        = TRUE;
    //g_mmipb_process_t.is_processing = TRUE;
    s_vcalendar_is_processing=TRUE;
    if(info_ptr->total_count > 1)
    {
        if(MMK_IsOpenWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID))
        {
            MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
        }
        MMIPUB_OpenProgressWinByTextId(TXT_PUB_SAVING, &win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleSchProcessWinMsg);
        MMIPUB_SetProgressTotalSize(&win_id, info_ptr->total_count);
    }
    else
    {
        SchOpenWaitWin(handle_func);
    }
	
    MMIAPISCH_SaveVcalendar(path_name_ptr);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_SaveVcalendar(const wchar *path_name_ptr)
{
	BOOLEAN result = FALSE;
	//MMIPB_PHONEBOOK_CONT_T entry_idx = {0};
    
    if(s_operate_sch_info.done_count < s_operate_sch_info.total_count)
    {
		if(!SaveOneVCalendar(s_operate_sch_info.event_id[s_operate_sch_info.done_count], path_name_ptr))
		{
			GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
            GUILIST_SetTextListMarkable(s_cur_vcal_list_id, FALSE);
		}
		else
		{
			result = TRUE;
		}		
    }
    if(!result || s_operate_sch_info.done_count >= s_operate_sch_info.total_count)
    {
        //save end
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        GUILIST_SetTextListMarkable(s_cur_vcal_list_id, FALSE);
        //turn on the back light
        MMIDEFAULT_SetBackLight(TRUE);
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);

        s_vcalendar_is_processing=FALSE;

		if(!result && (s_operate_sch_info.done_count < s_operate_sch_info.total_count))
		{
			MMIPUB_OpenAlertFailWin(TXT_COMM_ERROR);
		}
		else
		{
//#ifndef  MMI_PDA_SUPPORT
//			MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
        }
    }
	return result;
}
/*****************************************************************************/
// Description : 将选定日程条目保存为VCalendar格式的文件
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN SaveOneVCalendar(uint16 event_id, const wchar *path_name_ptr)
{
    //MMIPB_PHONEBOOK_ENTRY_T   detail_entry = {0};
    // MMI_TEXT_ID_T             first_text = TXT_COMMON_UDISK;
    // MMI_TEXT_ID_T             second_text = TXT_NULL;
    MMIACC_SMART_EVENT_T event_info={0};
	//MMI_STRING_T              name_str = {0};
    BOOLEAN                   is_using = MMIAPIUDISK_UdiskIsRun();
    BOOLEAN                   is_success = FALSE;
	SFS_ERROR_E               file_ret = SFS_ERROR_NONE;
    //PHONEBOOK_STORAGE_E       storage = PHONEBOOK_SIM_STORAGE;
	MMIFILE_DEVICE_E              fs_dev = MMI_DEVICE_UDISK;
	
    if (is_using) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        MMIDEFAULT_SetBackLight(TRUE);
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        GUILIST_SetTextListMarkable(s_cur_vcal_list_id, FALSE);
        return is_success;
    }

    MMIAPIALM_FormInteEventInfo(event_id,&event_info);

    MMIVCAL_VcalBuildInit();
    MMIVCAL_InitFileNamePtr();
	MMIVCAL_BuildVcalendarByEvent(&event_info,MMIVCAL_OBJECT_TYPE_VEVENT);
	MMIVCAL_GetVcalendarFileName(&event_info);
	
    file_ret = MMIVCAL_SaveVcalendarFileToAssignFolder(path_name_ptr, PNULL);
	if((SFS_ERROR_NONE == file_ret) || (SFS_ERROR_IO_PENDING == file_ret))
	{
		is_success = TRUE;
	}

    
    if(!is_success)
    {
        if(PNULL != path_name_ptr)
		{
			fs_dev = MMIAPIFMM_GetDeviceTypeByPath(path_name_ptr, MMIFILE_DEVICE_LEN);
		}
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        GUILIST_SetTextListMarkable(s_cur_vcal_list_id, FALSE);
		MMIAPIFMM_PromptFileErrorType(PNULL, file_ret, fs_dev);
        MMIDEFAULT_SetBackLight(TRUE);
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
    }

    //SCI_TRACE_LOW:"SaveOneVCalendar is_success %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_8514_112_2_18_2_2_13_93,(uint8*)"d",is_success);
    return is_success;
}

/*****************************************************************************/
//  Description : HandleSchWaitWinMsg  
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
   
    switch (msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        s_vcalendar_is_processing = FALSE;
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:  // 不响应翻盖
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
	case MSG_KEYREPEAT_RED:
        s_vcalendar_is_processing = FALSE;
        result  = MMI_RESULT_FALSE;
		break;
    case MSG_CLOSE_WINDOW:
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        s_vcalendar_is_processing = FALSE;
		if(s_vcalendar_export_folder_ptr != PNULL)
		{
			SCI_FREE(s_vcalendar_export_folder_ptr);
			s_vcalendar_export_folder_ptr = 0;
		}
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : SchOpenWaitWin 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void SchOpenWaitWin(MMIPUB_HANDLE_FUNC handle_func)
{
    MMI_STRING_T    prompt_str = {0};

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    if(MMK_IsOpenWin(MMISCH_WAITING_ALERT_WIN_ID))
    {
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
    }
    MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,MMISCH_WAITING_ALERT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,handle_func);
}
/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchProcessWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);	
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    //case MSG_APP_RED:   
    case MSG_APP_CANCEL: 	
        if(!s_vcalendar_is_processing)
		{
			
			MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
		    MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
		}
		s_vcalendar_is_processing = FALSE;
        break;

    case MSG_CLOSE_WINDOW:
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        s_vcalendar_is_processing = FALSE;
		if(s_vcalendar_export_folder_ptr != PNULL)
		{
			SCI_FREE(s_vcalendar_export_folder_ptr);
			s_vcalendar_export_folder_ptr = 0;
		}
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : notify vcalendar saved
//  Global resource dependence : g_ready_times
//  Author: 
//RETURN: void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_VCalendarSavedNotify(BOOLEAN is_ok)
{   
	BOOLEAN save_ret = is_ok;

	if(MMK_IsOpenWin(MMISCH_VIEW_WIN_ID ))
	{
		MMK_SendMsg(MMISCH_VIEW_WIN_ID, MSG_VCALENDAR_SAVE_SCH_CNF, (DPARAM) &save_ret);
	}
	else if(MMK_IsOpenWin(MMISCH_VIEW_ALL_WIN_ID ))
	{
		MMK_SendMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_VCALENDAR_SAVE_SCH_CNF, (DPARAM) &save_ret);
	}
}

/*****************************************************************************/
//  Description : 导入vcalendar到日程
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void SchLoadAllVCalendar(void)
{
	//FILEARRAY ret_array = NULL;
	MMIFMM_FILTER_T filter = {{"*.vcs"}};
	//s_loading_delay = FALSE;
	//s_is_vcal_loading = TRUE;
	s_vcalendar_is_processing= TRUE;
    if(PNULL!=s_vcalendar_array)
	{
		MMIAPIFILEARRAY_Destroy(&s_vcalendar_array);
	} 
	s_vcalendar_array = MMIAPIFILEARRAY_Create();
	
	if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(
		PNULL,
		0,
		&filter,
		FUNC_FIND_FILE_AND_ALLFOLER,
		TRUE,
		PNULL,
		s_vcalendar_array,
		SchLoadVcalendarCallBack,
		FALSE
		))
	{
	    s_vcalendar_is_processing = FALSE;
		if(PNULL!=s_vcalendar_array)
		{
    		MMIAPIFILEARRAY_Destroy(&s_vcalendar_array);
    		s_vcalendar_array = PNULL;
		} 
	}
}

/*****************************************************************************/
//  Description : Call back function for adding file to sch.
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void SchLoadVcalendarCallBack(
                    BOOLEAN     is_success,
                    FILEARRAY   file_array
                    )
{
	MMI_WIN_ID_T    win_id = MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID;

    if(MMIAPIFILEARRAY_GetArraySize(file_array)>0)
    {    
	    MMIPUB_OpenProgressWinByTextId(TXT_COMMON_IMPORTING, &win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleLoadVCalendarWaitWinMsg);
	    MMIPUB_SetProgressTotalSize(&win_id, MMIAPIFILEARRAY_GetArraySize(file_array));	
	    MMIPUB_SetWinSoftkey(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID,TXT_NULL,STXT_CANCEL,FALSE);
    
        if(!MMK_IsOpenWin(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID))
        {
            if(PNULL!=s_vcalendar_array)
    		{
        		MMIAPIFILEARRAY_Destroy(&s_vcalendar_array);
        		s_vcalendar_array = PNULL;
    		}
            return ;
        }
        
        if( is_success)
        {
            MmiSignalS *signal = PNULL;
	        s_operate_sch_info.done_count = 0;
	        s_operate_sch_info.total_count = 0;
		    s_operate_sch_info.failed_count = 0;
		    s_vcalendar_is_processing  = TRUE;
            //MMK_PostMsg(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID, MSG_SCH_GET_ALL_FILE_FINISH,PNULL, 0);
            //MMI_CREATE_SIGNAL(signal,(uint16)MSG_VCALENDAR_SAVE_IND,sizeof(MmiSignalS),P_APP);
            MMI_CREATE_SIGNAL(signal,(uint16)MSG_SCH_GET_ALL_FILE_FINISH,sizeof(MmiSignalS),P_APP);
            MMI_SEND_SIGNAL(signal, P_APP);
        }
	    else
	    {
		    MMK_CloseWin(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID);
		    //s_is_vcal_loading = FALSE;
	    }
    }
    else
    {
        if(PNULL!=s_vcalendar_array)
		{
    		MMIAPIFILEARRAY_Destroy(&s_vcalendar_array);
    		s_vcalendar_array = PNULL;
		} 
        //MMIPUB_OpenAlertWarningWin(TXT_PB_FOLDER_EMPTY);
    }
    return;
}

/*****************************************************************************/
//  Description : 导入VCalendar到sch
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL MMI_RESULT_E  HandleLoadVCalendarWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)

{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
	FILEARRAY_DATA_T file_array_data = {0};
	//LOCAL FILEARRAY file_array = PNULL;
	LOCAL uint32 s_read_index = 0;
	uint32   num = 0; 
	BOOLEAN  save_vcard_ret = FALSE;
    MmiSignalS *signal = PNULL;

    switch (msg_id)
    {
		
	case MSG_SCH_GET_ALL_FILE_FINISH:
		{

			if(s_vcalendar_is_processing)
			{
				num = MMIAPIFILEARRAY_GetArraySize(s_vcalendar_array);
				s_operate_sch_info.total_count = num;
	            if(num == 0)
				{
					MMIPUB_OpenAlertWarningWin(TXT_PB_FOLDER_EMPTY);
					MMK_CloseWin(win_id);
				}
				else
				{
					MMK_PostMsg(win_id, MSG_SCH_LOAD_NEXT_VCALENDAR, PNULL, 0);
				}
			}
		}
		break;
		
	case MSG_SCH_LOAD_NEXT_VCALENDAR:
		{
            if(!s_vcalendar_is_processing)
			{
				//MMIPB_UpdateCurrentList();
				MMK_CloseWin(win_id);
				break;
			}
			
			num = MMIAPIFILEARRAY_GetArraySize(s_vcalendar_array);
			if(s_read_index < num)
			{	
				MMIAPIFILEARRAY_Read(s_vcalendar_array, s_read_index, &file_array_data);
				save_vcard_ret = MMIAPISCH_OpenVCalendarFile(file_array_data.filename);
				s_read_index ++;
				if(!save_vcard_ret)
				{
                    s_operate_sch_info.failed_count++;
				}
				//MMIPB_SendSignal(MMI_PB_LOAD_NEXT);
                //MMK_PostMsg(win_id, MSG_SCH_LOAD_NEXT_VCALENDAR, PNULL,0);
                MMI_CREATE_SIGNAL(signal,(uint16)MSG_SCH_LOAD_NEXT_VCALENDAR,sizeof(MmiSignalS),P_APP);
                MMI_SEND_SIGNAL(signal, P_APP);
			}
			else
			{
				//MMIPB_UpdateCurrentList();
				if(s_vcalendar_is_processing)
				{
					if(s_operate_sch_info.failed_count < num)
					{
#ifndef  MMI_PDA_SUPPORT
						MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif
                    }
					else
					{
						MMIPUB_OpenAlertFailWin(TXT_COMM_ERROR);	
					}
				}
				
				MMK_CloseWin(win_id);
			}
			MMIPUB_UpdateProgressBar(&win_id, s_read_index);
		}
		break;
		
//	case MSG_LOSE_FOCUS:
//		{
//			if(s_vcalendar_array != PNULL)
//			{
//				s_vcalendar_is_processing = FALSE;
//			}
//			result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
//		}
// 		break;
		
/*	case MSG_GET_FOCUS:
		{
			if(s_vcalendar_array != PNULL)
			{
				s_vcalendar_is_processing = TRUE;
				MMISCH_LoadNextVcalendar();
			}
			result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
		}
		break;*/
		
    //case MSG_APP_RED:  
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //SCI_TRACE_LOW:"HandleLoadVCalendarWaitWinMsg red key pressed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_8839_112_2_18_2_2_14_94,(uint8*)"");
        //MMIPB_UpdateCurrentList();
		MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.		
    case MSG_APP_FLIP:  // 不响应翻盖
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
		
    case MSG_CLOSE_WINDOW:
        s_read_index = 0;

		s_vcalendar_is_processing = FALSE;
		//s_is_vcal_loading = FALSE;
		if(PNULL!=s_vcalendar_array)
		{
    		MMIAPIFILEARRAY_Destroy(&s_vcalendar_array);
    		s_vcalendar_array = PNULL;
		}
        //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_PBVIEW);
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
		
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : load next vcard
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
/*PUBLIC void  MMISCH_LoadNextVcalendar(void)
{
	if(MMK_IsOpenWin(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID))
	{
         MMK_SendMsg(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID, MSG_SCH_LOAD_NEXT_VCALENDAR, PNULL);
	}
	else
	{
       s_vcalendar_is_processing = FALSE;
	}
}*/

/*****************************************************************************/
// Description : 解析VCalendar文件,并保存到sch中
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISCH_OpenVCalendarFile(uint16  *name_ptr)
{
    MMIVCAL_ERROR_TYPE_E read_file_ret = MMIVCAL_NO_ERROR;
	BOOLEAN ret = FALSE;
    VF_OBJECT_T *p_object = NULL;
    VF_OBJECT_T *pTmp;
    //MMI_TEXT_ID_T    alert_text = TXT_NULL;
    MMIVCALENDAR_INFO_T vcal_info={0};
    MMIACC_SMART_EVENT_T event_info={0};
    //uint16 index=0;
	
    //SCI_TRACE_LOW:"MMIAPISCH_OpenVCalendarFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_8904_112_2_18_2_2_15_95,(uint8*)"");

    if(NULL == name_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPISCH_OpenVCalendarFile NULL == name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_8908_112_2_18_2_2_15_96,(uint8*)"");
        return ret;
    }

    read_file_ret = MMIVCAL_ReadFile(&p_object, name_ptr);
    if ((MMIVCAL_NO_ERROR == read_file_ret) && PNULL != p_object)
    {
        pTmp = p_object;
        if (!vf_get_next_object(&pTmp))
        {
            if (!MMIVCAL_VcalendarIsEmpty(p_object))
            {
                MMIVCAL_ObjectToVcalInfo(p_object,&vcal_info);
				read_file_ret=MMIVCAL_VcalInfoToSchedule(&vcal_info,&event_info);
				MMIVCAL_FreeVcalInfo(&vcal_info);
                if(MMIVCAL_NO_ERROR == read_file_ret)
                {
	                if (MMIALM_AddOneEvent(&event_info,NULL))
	                {
				        ret = TRUE;
	                }
                	else
                	{
				        if(MMIACC_SCH_NUM<=(MMIAPISCH_GetAllSchNumber()))
				        {
				            MMIPUB_OpenAlertWarningWin(TXT_SCHEDULE_FULL);
				        }
				        else
				        {
				            MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
				        }                
		 	}  

                }

            }
        }
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
        vf_delete_object(p_object, TRUE);
#endif
    }
    return ret;
}
/*****************************************************************************/
//  Description : 导入VCalendar到sch
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void  HandleSaveSchCNF
(
    MMI_CTRL_ID_T current_ctrl_id,
    DPARAM        param
)
{
    BOOLEAN is_save_success = FALSE;
    MMI_WIN_ID_T win_id=MMISCH_VCALENDAR_PROGRESS_WIN_ID;
    
    if(PNULL!=param)
    {
        is_save_success = *(BOOLEAN *)param;
    }
    else
    {
        s_vcalendar_is_processing = FALSE;

        GUILIST_SetAllSelected(current_ctrl_id, FALSE);
        GUILIST_SetTextListMarkable(current_ctrl_id, FALSE);
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
    }

    //SCI_TRACE_LOW:"MSG_VCALENDAR_SAVE_SCH_CNF is_save_success %d param %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_8979_112_2_18_2_2_15_97,(uint8*)"dd",is_save_success, param);
    s_operate_sch_info.done_count++;
    if(s_vcalendar_is_processing)
    {
        MMIPUB_UpdateProgressBar(&win_id, s_operate_sch_info.done_count);
    }
    if(is_save_success)
    { 
        if(s_vcalendar_is_processing)
        {
            MMIAPISCH_SaveVcalendar(s_vcalendar_export_folder_ptr);
        }
        else					 
        {
            GUILIST_SetAllSelected(current_ctrl_id, FALSE);
            GUILIST_SetTextListMarkable(current_ctrl_id, FALSE);
            MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
            MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
        }
    }
    else
    {
        s_vcalendar_is_processing = FALSE;

        GUILIST_SetAllSelected(current_ctrl_id, FALSE);
        GUILIST_SetTextListMarkable(current_ctrl_id, FALSE);
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
    }
}
/*****************************************************************************/
// Description : 中断vcalendar导入导出
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPISCH_InterruptVCalendar(void)
{
    if(s_vcalendar_is_processing)
    {
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
        s_vcalendar_is_processing=FALSE;
    }
}

/*****************************************************************************/
// Description : MMIAPISCH_CreateFileArray
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC FILEARRAY  MMIAPISCH_CreateFileArray(void)
{
    if(PNULL==s_vcalendar_array)
    {
        s_vcalendar_array = MMIAPIFILEARRAY_Create();
        return s_vcalendar_array;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : MMIAPISCH_LoadVcalendar
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_LoadVcalendar(FILEARRAY   file_array)
{
	MMI_WIN_ID_T    win_id = MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID;

	MMIPUB_OpenProgressWinByTextId(TXT_COMMON_IMPORTING, &win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleLoadVCalendarWaitWinMsg);
	MMIPUB_SetProgressTotalSize(&win_id, MMIAPIFILEARRAY_GetArraySize(file_array));	
	MMIPUB_SetWinSoftkey(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID,TXT_NULL,STXT_CANCEL,FALSE);

    if(!MMK_IsOpenWin(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID))
    {
        if(PNULL!=s_vcalendar_array)
		{
    		MMIAPIFILEARRAY_Destroy(&s_vcalendar_array);
    		s_vcalendar_array = PNULL;
		}
        return ;
    }
    
    if( !s_vcalendar_is_processing)
    {
        MmiSignalS *signal = PNULL;
	    s_operate_sch_info.done_count = 0;
	    s_operate_sch_info.total_count = 0;
		s_operate_sch_info.failed_count = 0;
		s_vcalendar_is_processing  = TRUE;
        //MMK_PostMsg(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID, MSG_SCH_GET_ALL_FILE_FINISH,PNULL, 0);
        //MMI_CREATE_SIGNAL(signal,(uint16)MSG_VCALENDAR_SAVE_IND,sizeof(MmiSignalS),P_APP);
        MMI_CREATE_SIGNAL(signal,(uint16)MSG_SCH_GET_ALL_FILE_FINISH,sizeof(MmiSignalS),P_APP);
        MMI_SEND_SIGNAL(signal, P_APP);
    }
	else
	{
		MMK_CloseWin(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID);
		//s_is_vcal_loading = FALSE;
	}
    return;
}

/*****************************************************************************/
//  Description : 删除sch
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void DeleteSch(MMISCH_OPERATE_RECORDS_INFO_T *info_ptr,MMI_CTRL_ID_T ctrl_id)
{
    MMI_WIN_ID_T win_id=MMISCH_VCALENDAR_PROGRESS_WIN_ID;
    MMIPUB_HANDLE_FUNC handle_func=HandleSchDelWaitWinMsg;
    s_cur_vcal_list_id=ctrl_id;
    info_ptr->done_count = 0;
	info_ptr->failed_count = 0;
    s_vcalendar_is_processing=TRUE;
    if(info_ptr->total_count > 1)
    {
        if(MMK_IsOpenWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID))
        {
            MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
        }
        MMIPUB_OpenProgressWinByTextId(TXT_PUB_DELETING, &win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleSchDelProcessWinMsg);
        MMIPUB_SetProgressTotalSize(&win_id, info_ptr->total_count);
    }
    else
    {
        SchOpenWaitWin(handle_func);
    }
	
    DelOneSch();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DelOneSch(void)
{
	BOOLEAN result = FALSE;
    
    if(s_operate_sch_info.done_count < s_operate_sch_info.total_count)
    {
		if(!MMIALM_DelOneEvent(s_operate_sch_info.event_id[s_operate_sch_info.done_count]))
		{
			GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
            GUILIST_SetTextListMarkable(s_cur_vcal_list_id, FALSE);
		}
		else
		{
			result = TRUE;
		}		
    }
    if(!result || s_operate_sch_info.done_count >= s_operate_sch_info.total_count)
    {
        //save end
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        GUILIST_SetTextListMarkable(s_cur_vcal_list_id, FALSE);
        //turn on the back light
        MMIDEFAULT_SetBackLight(TRUE);
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);

        s_vcalendar_is_processing=FALSE;

		if(!result && (s_operate_sch_info.done_count < s_operate_sch_info.total_count))
		{
			MMIPUB_OpenAlertFailWin(TXT_COMM_ERROR);
		}
		else
		{
//#ifndef  MMI_PDA_SUPPORT
//			MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
//#endif
        }
    }
    else
    {
        MmiSignalS *signal = PNULL;
       
        MMI_CREATE_SIGNAL(signal,(uint16)MSG_SCH_DELETE_IND,sizeof(MmiSignalS),P_APP);
        MMI_SEND_SIGNAL(signal, P_APP);
    }
	return result;
}

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchDelProcessWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);	
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    //case MSG_APP_RED:   
    case MSG_APP_CANCEL: 	
        if(!s_vcalendar_is_processing)
		{
			
			MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
		    MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
		}
		s_vcalendar_is_processing = FALSE;
        break;

    case MSG_CLOSE_WINDOW:
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        s_vcalendar_is_processing = FALSE;
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : HandleSchDelWaitWinMsg  
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSchDelWaitWinMsg
(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
)
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
   
    switch (msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        s_vcalendar_is_processing = FALSE;
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:  // 不响应翻盖
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
	case MSG_KEYREPEAT_RED:
        s_vcalendar_is_processing = FALSE;
        result  = MMI_RESULT_FALSE;
		break;
    case MSG_CLOSE_WINDOW:
		GUILIST_SetAllSelected(s_cur_vcal_list_id, FALSE);
        s_vcalendar_is_processing = FALSE;
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : notify vcalendar saved
//  Global resource dependence : g_ready_times
//  Author: 
//RETURN: void
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_DelSchNotify(void)
{   

	if(MMK_IsOpenWin(MMISCH_VIEW_WIN_ID ))
	{
		MMK_SendMsg(MMISCH_VIEW_WIN_ID, MSG_SCH_DELETE_CNF, PNULL);
	}
	else if(MMK_IsOpenWin(MMISCH_VIEW_ALL_WIN_ID ))
	{
		MMK_SendMsg(MMISCH_VIEW_ALL_WIN_ID, MSG_SCH_DELETE_CNF, PNULL);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void  HandleDeleteSchCNF
(
    MMI_CTRL_ID_T current_ctrl_id,
    DPARAM        param
)
{
    MMI_WIN_ID_T win_id=MMISCH_VCALENDAR_PROGRESS_WIN_ID;
    s_operate_sch_info.done_count++;
    if(s_vcalendar_is_processing)
    {
        MMIPUB_UpdateProgressBar(&win_id, s_operate_sch_info.done_count);
    }
    
    if(s_vcalendar_is_processing)
    {
        DelOneSch();
    }
    else					 
    {
        GUILIST_SetAllSelected(current_ctrl_id, FALSE);
        GUILIST_SetTextListMarkable(current_ctrl_id, FALSE);
        MMK_CloseWin(MMISCH_WAITING_ALERT_WIN_ID);
        MMK_CloseWin(MMISCH_VCALENDAR_PROGRESS_WIN_ID);
    }
}

/*****************************************************************************/
// 	Description : HandleListDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelMarkQueryWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            DeleteSch(&s_operate_sch_info,MMISCH_VIEW_LIST_CTRL_ID);
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            SCI_MEMSET(&s_operate_sch_info,0,sizeof(MMISCH_OPERATE_RECORDS_INFO_T));
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
    return result;

}

/*****************************************************************************/
// 	Description : HandleListDelQueryWin
//	Global resource dependence :                                
//      Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllDelMarkQueryWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            DeleteSch(&s_operate_sch_info,MMISCH_VIEW_ALL_LIST_CTRL_ID);
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            SCI_MEMSET(&s_operate_sch_info,0,sizeof(MMISCH_OPERATE_RECORDS_INFO_T));
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
    return result;

}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMISCH_NAME_EDIT_CTRL_ID;
    MMIACC_SMART_EVENT_T*	event_ptr = (MMIACC_SMART_EVENT_T *) MMK_GetWinAddDataPtr(win_id);

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
         {
        	MMI_STRING_T    name_str  = {0};
            //wchar           forbid_char_value[]= {0x0d, 0x0a};		//NEWMS00214305 [6530][64X64_240X400PDA2]搜狗输入法，点击日历-创建新日程-点击主题，任意输入法，输入多行键码点击回车键无效
        	name_str.wstr_ptr = event_ptr->event_content_info.w_content;
        	name_str.wstr_len = event_ptr->event_content_info.w_length;
        	//GUIEDIT_SetStyle(MMISCH_NAME_EDIT_CTRL_ID,GUIEDIT_STYLE_SINGLE);
        	GUIEDIT_SetString(ctrl_id, name_str.wstr_ptr, name_str.wstr_len );
            //GUIEDIT_ConfigDisableChar(MMISCH_NAME_EDIT_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveSchEditWinName(event_ptr))
		{
			MMK_CloseWin(win_id);
		}
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : HandleEditTimeMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
	MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;
    MMIACC_SMART_EVENT_T*	event_ptr = (MMIACC_SMART_EVENT_T *) MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIFORM_IsSlide(MMISCH_EDIT_TIME_FORM_CTRL_ID,FALSE);
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
        	GUIEDIT_SetTimeStyle(MMISCH_TIME_EDIT_CTRL_ID,PNULL,&time_style,PNULL,FALSE);
        	//GUIEDIT_SetFont(MMISCH_TIME_EDIT_CTRL_ID, MMI_SET_TIME_FONT);
        	GUIEDIT_SetTime(
        				MMISCH_TIME_EDIT_CTRL_ID,
        				event_ptr->event_fast_info.hour,
        				event_ptr->event_fast_info.minute,
        				0);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveSchEditWinTime(event_ptr))
		{
			MMK_CloseWin(win_id);
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
//  Description : HandleEditDateMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditDateMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    MMIACC_SMART_EVENT_T*	event_ptr = (MMIACC_SMART_EVENT_T *) MMK_GetWinAddDataPtr(win_id);
    MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
	GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIFORM_IsSlide(MMISCH_EDIT_DATE_FORM_CTRL_ID,FALSE);
            date_display_type = MMIAPISET_GetDateDisplayType();
        	if(MMISET_DATE_YMD == date_display_type)
        	{
        		date_display_style = GUIEDIT_DATE_STYLE_YMD;
        	}
        	else if(MMISET_DATE_MDY == date_display_type)
        	{
        		date_display_style = GUIEDIT_DATE_STYLE_MDY;
        	}
        	else if(MMISET_DATE_DMY == date_display_type)
        	{
        		date_display_style = GUIEDIT_DATE_STYLE_DMY;
        	}
        	GUIEDIT_SetDateStyle(MMISCH_DATE_EDIT_CTRL_ID,PNULL,&date_display_style,FALSE);
        	GUIEDIT_SetDate(MMISCH_DATE_EDIT_CTRL_ID,
        		            event_ptr->event_fast_info.year,
        		            event_ptr->event_fast_info.mon,
        		            event_ptr->event_fast_info.day);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveSchEditWinDate(event_ptr))
		{
			MMK_CloseWin(win_id);
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
//  Description : HandleEditDeadlineMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditDeadlineMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    MMIACC_SMART_EVENT_T*	event_ptr = (MMIACC_SMART_EVENT_T *) MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {	
        	SCI_DATE_T	deadline= {0};
        	SCI_DATE_T    second_date = {0};
        	MMISET_DATE_DISPLAY_TYPE_E date_display_type=MMISET_DATE_YMD;
        	GUIEDIT_DATE_STYLE_E date_display_style=GUIEDIT_DATE_STYLE_YMD;

            GUIFORM_IsSlide(MMISCH_EDIT_DEADLINE_FORM_CTRL_ID,FALSE);
            
        	deadline.year = event_ptr->event_fast_info.end_year;
        	deadline.mon= event_ptr->event_fast_info.end_mon;
        	deadline.mday= event_ptr->event_fast_info.end_day;
        	second_date.year = event_ptr->event_fast_info.year;
        	second_date.mon  = event_ptr->event_fast_info.mon;
        	second_date.mday = event_ptr->event_fast_info.day;	
        	if (IsValidDeadline(second_date, deadline) 
        		|| ALM_MODE_ONCE == event_ptr->event_fast_info.fre_mode
        		||ALM_MODE_NO== event_ptr->event_fast_info.fre_mode)
        	{
        		event_ptr->event_fast_info.end_year = event_ptr->event_fast_info.year;
        		event_ptr->event_fast_info.end_mon = event_ptr->event_fast_info.mon;
        		event_ptr->event_fast_info.end_day = event_ptr->event_fast_info.day;
        	}

        	date_display_type = MMIAPISET_GetDateDisplayType();
        	if(MMISET_DATE_YMD == date_display_type)
        	{
        		date_display_style = GUIEDIT_DATE_STYLE_YMD;
        	}
        	else if(MMISET_DATE_MDY == date_display_type)
        	{
        		date_display_style = GUIEDIT_DATE_STYLE_MDY;
        	}
        	else if(MMISET_DATE_DMY == date_display_type)
        	{
        		date_display_style = GUIEDIT_DATE_STYLE_DMY;
        	}
        	GUIEDIT_SetDateStyle(MMISCH_DEADLINE_EDIT_CTRL_ID,PNULL,&date_display_style,FALSE);       
        	GUIEDIT_SetDate(MMISCH_DEADLINE_EDIT_CTRL_ID,
        				event_ptr->event_fast_info.end_year,
        				event_ptr->event_fast_info.end_mon,
        				event_ptr->event_fast_info.end_day); 
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        /*SaveSchEditWinFreqMode(event_ptr);
		
		if(event_ptr->event_fast_info.fre_mode == ALM_MODE_EVERYWEEK)
		{
			SCHSaveFreEditWin(event_ptr)
		}*/
        
        if(SaveSchEditWinDeadlineDate(event_ptr))
		{
			MMK_CloseWin(win_id);
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

#endif
#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :录音 后 的 处 理  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AlarmRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr)
{
    //MMIFILE_FILE_INFO_T file_info = {0};
    static MMI_STRING_T    str_record_file_path = {0};
    uint32          size_of_param= 0;

    if (PNULL == data_info_ptr) 
    {
        SCI_TRACE_LOW("[MMIsch]OpenRecord data_info_ptr is pnull");
        return; 
    } 

    if (MMIRECORD_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            str_record_file_path.wstr_ptr=(wchar*)SCI_ALLOCA((data_info_ptr->file_name_len+1)*sizeof(wchar));
            if(str_record_file_path.wstr_ptr==PNULL)
            {
                return ;
            }
            SCI_MEMSET(str_record_file_path.wstr_ptr,0,(data_info_ptr->file_name_len+1)*sizeof(wchar));
            MMIAPICOM_Wstrncpy(str_record_file_path.wstr_ptr, data_info_ptr->file_name, data_info_ptr->file_name_len);
            str_record_file_path.wstr_len = data_info_ptr->file_name_len;
            size_of_param = (str_record_file_path.wstr_len+1)*sizeof(wchar);
            MMK_PostMsg(MMISCH_EDIT_WIN_ID,  MMI_MSG_RECORDTORING, (DPARAM)str_record_file_path.wstr_ptr,size_of_param);
            SCI_FREE(str_record_file_path.wstr_ptr);
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIsch]OpenRecord result is not succ");
    }

    return ;
}
#endif
#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description : Record button CallBack
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL  MMI_RESULT_E  RecordCalendarRingCallBack(void)
{

    MMIRECORD_WORK_PARAM_T parameter={0};

    parameter.max_file_size = 0;
    parameter.max_file_time = 0;
    parameter.record_support_type = MMIRECORD_WORK_FORMAT_WAV;
    parameter.work_callback = AlarmRecordWorkCallBack;
    if(MMIAPIRECORD_OpenMainWin(&parameter) != MMIRECORD_WORK_SUCCESS)
    {
        SCI_TRACE_LOW("[MMIsch]OpenRecord alarm record error!");
    }

    return MMI_RESULT_TRUE;
}
#endif
/*****************************************************************************/
//  Description : MMIAPIACCSCH_AppendAllUserListBoxItem 供外部使用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
/*PUBLIC void  MMIAPIACCSCH_AppendAllUserListBoxItem(MMI_CTRL_ID_T ctrl_id,BOOLEAN is_widget_call)
{
    BuildEventUserList(TRUE,0,0,0);
    AppendAllUserListBoxItem(ctrl_id,MMIAPIACCSCH_IsHaveEvent(),is_widget_call);
}*/

/*****************************************************************************/
//  Description : MMIAPIACCSCH_IsHaveEvent 供外部使用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIAPIACCSCH_IsHaveEvent(void)
{
    if (0 == s_user_list.total)      //  list empty       
    {
        return FALSE;
    }
    else       
    {
        SortEventUserList();
        return TRUE;
    }       
}*/
/*****************************************************************************/
//  Description : MMIAPIACCSCH_AppendAllUserListBoxItemData 供外部使用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
/*PUBLIC void MMIAPIACCSCH_AppendAllUserListBoxItemData(
                                                    MMI_CTRL_ID_T    ctrl_id ,  
                                                    uint16 list_index
                                                    )
{
    AppendAllUserListBoxItemData(ctrl_id,MMIAPISCH_OpenEventList(TRUE,0,0,0),list_index);
}*/
/*****************************************************************************/
//  Description : MMIAPIACCSCH_GetEventInfoByIndex 供外部使用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 根据list当前的index获得event的信息
/*****************************************************************************/
/*PUBLIC void MMIAPIACCSCH_GetEventInfoByIndex(uint16 cur_index,MMIACC_SMART_EVENT_T *event_ptr)
{
    MMIAPIALM_FormInteEventInfo(s_user_list.entry_id[cur_index],event_ptr);
}*/
/*****************************************************************************/
//  Description : MMIAPIACCSCH_GetEventEntryIdByIndex 供外部使用
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 根据list当前的index获得event的信息
/*****************************************************************************/
/*PUBLIC uint16 MMIAPIACCSCH_GetEventEntryIdByIndex(uint16 cur_index)
{
    return s_user_list.entry_id[cur_index];
}*/

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : 判断是否有相同日程
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN IsExitAccordantSch(MMIACC_SMART_EVENT_T* integrity_event_ptr)
{
    uint16                          i          = 0;
    uint16                          j          = 0;
    //const MMIACC_SMART_EVENT_FAST_TABLE_T      *event_ptr = NULL;
    MMIACC_SMART_EVENT_T *event=PNULL;

    event=(MMIACC_SMART_EVENT_T*)SCI_ALLOC_APP(sizeof(MMIACC_SMART_EVENT_T));
    if(PNULL==event)
    {
        return FALSE;
    }
    
    for (i = 0; i < MMIACC_SCH_NUM; i++)
    {
        SCI_MEMSET(event,0,sizeof(MMIACC_SMART_EVENT_T));
        MMIAPIALM_FormInteEventInfo((uint16)(EVENT_SCH_0 + i),event);
        if (event->event_fast_info.is_valid)
        {
            if(event->event_fast_info.start_day==integrity_event_ptr->event_fast_info.start_day
                &&event->event_fast_info.start_mon==integrity_event_ptr->event_fast_info.start_mon
                &&event->event_fast_info.start_year==integrity_event_ptr->event_fast_info.start_year
                &&event->event_fast_info.end_day==integrity_event_ptr->event_fast_info.end_day
                &&event->event_fast_info.end_mon==integrity_event_ptr->event_fast_info.end_mon
                &&event->event_fast_info.end_year==integrity_event_ptr->event_fast_info.end_year
                &&event->event_fast_info.fre==integrity_event_ptr->event_fast_info.fre
                &&event->event_fast_info.fre_mode==integrity_event_ptr->event_fast_info.fre_mode
                &&event->event_content_info.w_length==integrity_event_ptr->event_content_info.w_length)
            {
                for(j=0;j<event->event_content_info.w_length;j++)
                {
                    if(event->event_content_info.w_content[j]!=integrity_event_ptr->event_content_info.w_content[j])
                    {
                        break;
                    }
                }
                if(j==event->event_content_info.w_length)
                {
                    return TRUE;
                }
                else
                {
                    continue;
                }
            }
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : 一键还原备份函数
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void sch_backup_callback(
    MMIIKB_BACKUP_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    //void *buf_ptr = PNULL;
    static BOOLEAN is_build_list=FALSE;
    MMIACC_SMART_EVENT_T* integrity_event_ptr=PNULL;
    
    integrity_event_ptr=(MMIACC_SMART_EVENT_T*)SCI_ALLOC_APP(sizeof(MMIACC_SMART_EVENT_T));
    
    if (PNULL == param_in_ptr
        || PNULL == param_out_ptr
        || PNULL == integrity_event_ptr)
    {
        //log pls.
        is_build_list=FALSE;
        return;
    }
    
    if (MMIIKB_TYPE_SCHEDULE!= param_in_ptr->type
        || MMIIKB_OPERATE_TYPE_BUF != param_in_ptr->op_type)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        is_build_list=FALSE;
    }
    else
    {
        if(!is_build_list)
        {
           BuildEventUserList(TRUE,0,0,0);
           is_build_list=TRUE; 
        }
        param_out_ptr->type = param_in_ptr->type;
        param_out_ptr->op_type = param_in_ptr->op_type;
        //set three seg for testing
        if (param_in_ptr->number >=s_user_list.total)
        {
            error_code = MMIIKB_STATUS_INVALID_PARAM;
            is_build_list=FALSE;
        }
        else
        {
            SCI_MEMSET(integrity_event_ptr,0,sizeof(MMIACC_SMART_EVENT_T));
            MMIAPIALM_FormInteEventInfo(s_user_list.entry_id[param_in_ptr->number],integrity_event_ptr);
            MMIAPIALM_GetRingInfo(s_user_list.entry_id[param_in_ptr->number],integrity_event_ptr);
            param_out_ptr->buf_ptr=(void*)integrity_event_ptr;
            param_out_ptr->is_need_free=TRUE;
            param_out_ptr->len=sizeof(MMIACC_SMART_EVENT_T);
            param_out_ptr->number=1;
            param_out_ptr->is_finished=FALSE;
            if((param_in_ptr->number+1)==s_user_list.total)
            {
                param_out_ptr->is_finished=TRUE;
                is_build_list=FALSE; 
            }
            error_code = MMIIKB_STATUS_NONE;
        }
    }
    
    param_out_ptr->error_code = error_code;

    return;   
}
/*****************************************************************************/
//  Description : 获得备份总数
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void sch_get_total_callback(
    MMIIKB_GET_TOTAL_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_SUCCESS;

    param_out_ptr->total = MMIAPISCH_GetAllSchNumber();    
    param_out_ptr->total_type = MMIIKB_TOTAL_TYPE_NUMBER;
    
    param_out_ptr->error_code = error_code;

    return;
}
/*****************************************************************************/
//  Description : 还原函数
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
LOCAL void sch_restore_callback(
    MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *param_in_ptr,
    MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIACC_SMART_EVENT_T* integrity_event_ptr=PNULL;
    uint16 index=0;
    
    if (PNULL == param_in_ptr
        || PNULL == param_out_ptr)
    {
        //log pls.
        return;
    }

    param_out_ptr->type=param_in_ptr->type;
    param_out_ptr->op_type=param_in_ptr->op_type;
    
    if (MMIIKB_TYPE_SCHEDULE!= param_in_ptr->type
        || MMIIKB_OPERATE_TYPE_BUF != param_in_ptr->op_type
        || 1 != param_in_ptr->number
        || sizeof(MMIACC_SMART_EVENT_T)!=param_in_ptr->len)
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        param_out_ptr->suc_cnt = 0;
        param_out_ptr->err_cnt = 1;
    }
    else
    {
        integrity_event_ptr=(MMIACC_SMART_EVENT_T*)param_in_ptr->buf_ptr;
        if(!IsExitAccordantSch(integrity_event_ptr))
        {
            MMIALM_AddOneEvent(integrity_event_ptr,&index);
        }
        error_code = MMIIKB_STATUS_SUCCESS;
        param_out_ptr->suc_cnt = 1;
        param_out_ptr->err_cnt = 0;
    }
    
    param_out_ptr->error_code = error_code;

    return;
}
/*****************************************************************************/
//  Description : 一键还原注册函数
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_ikeybackup_register(void)
{
    BOOLEAN is_ret = FALSE;
    MMIIKB_TYPE_PROCESS_PARAM_T ikb_param = {0};
    
    ikb_param.type = MMIIKB_TYPE_SCHEDULE;
    ikb_param.op_type = MMIIKB_OPERATE_TYPE_BUF;
    ikb_param.backup_call_back = sch_backup_callback;
    ikb_param.is_backup_asyn = FALSE;
    ikb_param.restore_call_back = sch_restore_callback;
    ikb_param.is_restore_asyn = FALSE;
    ikb_param.get_total_call_back = sch_get_total_callback;
    is_ret = MMIAPIIKB_RegTypeFunc(&ikb_param);
    
    return is_ret;
}
#endif

/*****************************************************************************/
// 	Description : CompareDateTime
//         compare a time to another , find smaller(earlier)
//	Global resource dependence : 
//  Author:        renyi.hu
//	Note:   
/*****************************************************************************/
PUBLIC uint16 MMIAPISCH_CompareTime(
                         SCI_TIME_T   first, 
                         SCI_TIME_T   second
                         )
{
    ALM_CMP_E result = CMP_EQUAL;
    
    if(first.hour> second.hour)
    {
        result = CMP_SECOND_SMALL;
    }
    else if(first.hour< second.hour)
    {
        result = CMP_FIRST_SMALL;
    }
    else
    {
        if(first.min> second.min)
        {
            result = CMP_SECOND_SMALL;
        }
        else if(first.min< second.min)
        {
            result = CMP_FIRST_SMALL;
        }
        else
        {
            if(first.sec> second.sec)
            {
                result = CMP_SECOND_SMALL;
            }
            else if(first.sec< second.sec)
            {
                result = CMP_FIRST_SMALL;
            }	
            else
            {
                result = CMP_EQUAL;
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 根据list当前的index获得event的信息
/*****************************************************************************/
PUBLIC MMIACC_SCH_LIST_T* MMIAPISCH_GetUserList(void)
{
    return &s_user_list;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISCH_OpenDetailWin(ADD_DATA detail_win_param )
{
    MMK_CreateWin((uint32 *)SCH_VIEW_DETAIL_WIN_TAB, detail_win_param);

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32  MMIAPISCH_GetNextActiveSchEventId(void)
{
    uint32 id=0;
    uint32 i=0;
    BOOLEAN is_find=FALSE;
    SCI_DATE_T first_date={0};
    SCI_TIME_T first_time={0};
    SCI_DATE_T second_date={0};
    SCI_TIME_T second_time={0};
    MMIACC_SCH_LIST_T*user_list=PNULL;
    MMIACC_SMART_EVENT_FAST_TABLE_T * event=PNULL;
    
    if(MMIAPISCH_OpenEventList(TRUE,0,0,0))
    {
        TM_GetSysTime(&second_time);
        TM_GetSysDate(&second_date);
        user_list=MMIAPISCH_GetUserList();
        for(i=0;i<user_list->total;i++)
        {
            event=MMIALM_GeEvent(user_list->entry_id[i]);/*lint !e605*/
            if(PNULL==event)
            {
                return id;
            }
            first_date.year=event->year;
            first_date.mon=event->mon;
            first_date.mday=event->day;
            first_time.hour=event->hour;
            first_time.min=event->minute;
            first_time.sec=second_time.sec;//不比较秒，设为相等
            if(MMIAPIALM_CompareDateAndTime(first_date,first_time, second_date,second_time)==CMP_SECOND_SMALL
                ||MMIAPIALM_CompareDateAndTime(first_date,first_time, second_date,second_time)==CMP_EQUAL)
            {
                id=user_list->entry_id[i];
                is_find=TRUE;
                break;
            }
        }
        if(!is_find)
        {
            id=user_list->entry_id[user_list->total-1];
        }
    }
    else
    {
        id=0;
    }

    return id;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISCH_GetAllUnreadSchNum(void)
{
	uint16 i=0;
	uint32  num=0;
	if(0==s_unRead_event_list.total)
	{
		return 0;
	}
	
	for(i=0;i<s_unRead_event_list.total;i++)
	{
		if(s_unRead_event_list.entry_id[i]>=EVENT_SCH_0 && s_unRead_event_list.entry_id[i]<=EVENT_SCH_MAX )
		{
			num++;
		}
	}

	return num;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_GetFirstUnreadSch(MMIACC_SMART_EVENT_T * event_ptr)
{
    uint16 i=0;
	MMIACC_SCH_LIST_T	   user_list = {0}; 

	if( PNULL == event_ptr)
	{
		return FALSE;
	}
	
	for(i=0;i<s_unRead_event_list.total;i++)
	{
		if(s_unRead_event_list.entry_id[i]>=EVENT_SCH_0 && s_unRead_event_list.entry_id[i]<=EVENT_SCH_MAX)
		{
			user_list.entry_id[user_list.total]=s_unRead_event_list.entry_id[i];
			user_list.total++;
		}
	}

	if(0==user_list.total)
	{
		return FALSE;
	}
	else
	{
		SortEventUserListEx(&user_list);
	}

	MMIAPIALM_FormInteEventInfo(user_list.entry_id[user_list.total-1],event_ptr);

	return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPISCH_GetAllUnreadAlarmNum(void)
{
    uint16 i=0;
    uint32  num=0;
    SCI_TRACE_LOW("MMIAPISCH_GetAllUnreadAlarmNum:s_unRead_event_list.total=%d",s_unRead_event_list.total);

    if(0==s_unRead_event_list.total)
    {
        return 0;
    }

    for(i=0;i<s_unRead_event_list.total;i++)
    {
        if(s_unRead_event_list.entry_id[i]>=EVENT_ALARM_0 && s_unRead_event_list.entry_id[i]<=EVENT_ALARM_MAX )/*lint !e685 !e568*/
        {
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISCH_GetFirstUnreadAlarm(MMIACC_SMART_EVENT_T * event_ptr)
{
    uint16 i=0;
	MMIACC_SCH_LIST_T	   user_list = {0}; 

	if( PNULL == event_ptr)
	{
		return FALSE;
	}
	
	for(i=0;i<s_unRead_event_list.total;i++)
	{
		if(s_unRead_event_list.entry_id[i]>=EVENT_ALARM_0 && s_unRead_event_list.entry_id[i]<=EVENT_ALARM_MAX)/*lint !e685 !e568*/
		{
			user_list.entry_id[user_list.total]=s_unRead_event_list.entry_id[i];
			user_list.total++;
		}
	}
    
    SCI_TRACE_LOW("MMIAPISCH_GetFirstUnreadAlarm in:user_list.total,=%d,s_unRead_event_list.total =%d",user_list.total,s_unRead_event_list.total);

	if(0==user_list.total)
	{
		return FALSE;
	}
	else
	{
		SortEventUserListEx(&user_list);
	}

	MMIAPIALM_FormInteEventInfo(user_list.entry_id[user_list.total-1],event_ptr);

	return TRUE;
}




/*Edit by script, ignore 25 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
#endif
