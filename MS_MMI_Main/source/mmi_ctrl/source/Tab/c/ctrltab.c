/*****************************************************************************
** File Name:      ctrltab.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "ctrltab.h"
#include "guilcd.h"
#include "guicommon.h"
#include "mmitheme_tab.h"
#include "guifont.h"
#include "guistring.h"
#include "ctrltitle.h"
#include "..//..//mmi_gui//source//title//h//guititle_internal.h"
#include "mmk_tp.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUITAB_MIN_BAR_LENGTH               5
#define GUITAB_TEXT_MARGIN                  3
#define GUITAB_SHOW_TEXT_RELATIVE_TOP       0X0005      // 显示文字与Tab顶端的间隔
#define INNERTAB_STATE_CUSTOMER_WIDTH       0x10000000  // 用户自设置宽度

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : append a new tab item to tail.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendSimpleItem(
                             CTRLTAB_OBJ_T *	tab_ptr,
                             MMI_STRING_T *		text_ptr,
                             MMI_IMAGE_ID_T		sel_image_id,
                             MMI_IMAGE_ID_T		unsel_image_id
                             );

/*****************************************************************************/
//  Description : insert a new tab item to the given position.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertTabItem(
                            CTRLTAB_OBJ_T      *tab_ptr, 
                            const GUITAB_ITEM_T* add_item_ptr, // 指定的Item
                            uint32             pos           // 指定的位置, from 0 to total_item_num
                            );

/*****************************************************************************/
//  Description : decide whether tab items need scroll.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemScroll(
                           CTRLTAB_OBJ_T * tab_ptr
                           );

/*****************************************************************************/
//  Description : draw back ground image or color.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawBackGround(
                          CTRLTAB_OBJ_T		*tab_ptr,
                          GUI_RECT_T disp_rect
                          );

/*****************************************************************************/
//  Description : draw left scroll image and right scroll image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawScroll(
                      CTRLTAB_OBJ_T * tab_ptr
                      );

/*****************************************************************************/
// 	Description : display progress
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DrawPrg(
				   CTRLTAB_OBJ_T	*tab_ptr
				   );

/*****************************************************************************/
//  Description : display item description information.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawItemText(
                        CTRLTAB_OBJ_T * tab_ptr, 
                        uint32 index
                        );

/*****************************************************************************/
// 	Description : get item text rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemTextRect(//is need to draw
							  CTRLTAB_OBJ_T		*tab_ptr,	//in:
							  GUITAB_ITEM_T		*item_ptr,	//in:
							  GUI_RECT_T		*rect_ptr	//in/out:
							  );

/*****************************************************************************/
// 	Description : get item text string
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GetItemTextStr(
						  CTRLTAB_OBJ_T		*tab_ptr,	//in:
						  GUITAB_ITEM_T		*item_ptr,	//in:
						  MMI_STRING_T		*str_ptr	//in/out:
						  );

/*****************************************************************************/
// 	Description : item string is need scroll
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemTextScroll(
							   CTRLTAB_OBJ_T	*tab_ptr,	//in:
							   wchar			*str_ptr,	//in:
							   uint16			str_len,	//in:
							   GUI_RECT_T		str_rect	//in:
							   );

/*****************************************************************************/
// 	Description : start item text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartItemTextScrollTimer(
									CTRLTAB_OBJ_T	*tab_ptr
									);

/*****************************************************************************/
// 	Description : stop item text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StopItemTextScrollTimer(
								   CTRLTAB_OBJ_T	*tab_ptr
								   );

/*****************************************************************************/
//  Description : handle item text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleItemTextScrollTimer(
									 CTRLTAB_OBJ_T	*tab_ptr
									 );

/*****************************************************************************/
//  Description : display additional string.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawAddedString(
                           CTRLTAB_OBJ_T * tab_ptr
                           );

/*****************************************************************************/
//  Description : draw all tab items.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawAllTabItems(
                           CTRLTAB_OBJ_T * tab_ptr
                           );

/*****************************************************************************/
//  Description : draw current tab text.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawTabCurText(
                          CTRLTAB_OBJ_T * tab_ptr
                          );

/*****************************************************************************/
//  Description : draw child window.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawChildWindow(
                           CTRLTAB_OBJ_T * tab_ptr
                           );

/*****************************************************************************/
//  Description : draw the whole control.
//  Global resource dependence :
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawCtrl(
                    CTRLTAB_OBJ_T *tab_ptr
                    );

/*****************************************************************************/
//  Description : handle tab tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabTpDown(
								   CTRLTAB_OBJ_T	*tab_ptr,
								   DPARAM			param
								   );

/*****************************************************************************/
//  Description : handle tab item tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleItemTpDown(
									CTRLTAB_OBJ_T	*tab_ptr,
									GUI_POINT_T		*point_ptr
									);

/*****************************************************************************/
//  Description : handle tab tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabTpMove(
								   CTRLTAB_OBJ_T	*tab_ptr,
								   DPARAM			param
								   );

/*****************************************************************************/
//  Description : tp move item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpMoveItem(
						 CTRLTAB_OBJ_T	*tab_ptr,
						 int16			move_x
						 );

/*****************************************************************************/
//  Description : handle tab tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabTpUp(
								 CTRLTAB_OBJ_T	*tab_ptr,
								 DPARAM			param
								 );

/*****************************************************************************/
//  Description : handle MSG_KEYDOWN_LEFT message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLeftKeyDown(
                                     CTRLTAB_OBJ_T * tab_ptr
                                     );

/*****************************************************************************/
//  Description : handle MSG_KEYDOWN_RIGHT message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRightKeyDown(
                                      CTRLTAB_OBJ_T * tab_ptr
                                      );

/*****************************************************************************/
//  Description : handle MSG_KEYUP_LEFT and MSG_KEYUP_RIGHT message.
//  Global resource dependence : 
//  Author:  hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKeyUp(
                               CTRLTAB_OBJ_T *tab_ptr  // [in] tab handle
                               );

/*****************************************************************************/
//  Description : handle MSG_TIMER message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimer(
                               CTRLTAB_OBJ_T * tab_ptr
                               );

/*****************************************************************************/
//  Description : reset the item pointer of list 
//  Global resource dependence : 
//  Author:Lin.Lin
//  Note:
/*****************************************************************************/
LOCAL void ResetTabItem(
                        CTRLTAB_OBJ_T* tab_ptr  // the list control pointer
                        );


/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyTabItemData(//is destroy anim ctrl
                                 CTRLTAB_OBJ_T*      tab_ptr,  // the tab control pointer
                                 GUITAB_ITEM_T*      item_ptr,  // 指定的Item
                                 BOOLEAN              is_page
                                 );


/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL GUITAB_ITEM_T* GetItemPtr(
                                 const CTRLTAB_OBJ_T* tab_ptr,
                                 uint32              index  //from 0 to total_item_num-1
                                 );

/*****************************************************************************/
// 	Description : 终止当前正在解码的item
//	Global resource dependence : 
//  Author: Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL void DestroyTabItemDecode(
                                CTRLTAB_OBJ_T *tab_ptr   //in
                                );

/*****************************************************************************/
//  Description : set the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void SetState( 
                    uint32* dst_state_ptr,
                    uint32  src_state,
                    BOOLEAN is_true
                    );

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState(
                       uint32 dst_state,
                       uint32 src_state
                       );

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyAllITabtemData(
                                    CTRLTAB_OBJ_T*       tab_ptr,  // the list control pointer
                                    BOOLEAN              is_page
                                    );

/*****************************************************************************/
//  Description : alloc all items memory of the tab control
//  Global resource dependence : 
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocAllItemsSpace(
                                 CTRLTAB_OBJ_T* tab_ptr  // the list control pointer
                                 );

/*****************************************************************************/
//  Description : stop the tab control timer.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint8 StartTabTimer(
                          CTRLTAB_OBJ_T *tab_ptr,
                          uint32 timer_out
                          );

/*****************************************************************************/
//  Description : Get show num
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetShowItemNum(
                            CTRLTAB_OBJ_T *tab_ptr
                            );

/*****************************************************************************/
//  Description : draw item dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DrawDividingLine(
							CTRLTAB_OBJ_T	*tab_ptr,
							uint32			index
							);

/*****************************************************************************/
//  Description : draw item anim
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawItemAnim(
                           CTRLTAB_OBJ_T	*tab_ptr,
                           uint32			index
                           );

/*****************************************************************************/
//  Description : Set Scroll state
//  Global resource dependence :
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void ResetScrollState(
                             CTRLTAB_OBJ_T *tab_ptr
                             );


/*****************************************************************************/
//  Description : stop the tab control timer.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopTabTimer(CTRLTAB_OBJ_T *tab_ptr);


/*****************************************************************************/
//  Description : move item
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void MoveToNextValidItem(
                               CTRLTAB_OBJ_T *tab_ptr,
                               GUITAB_SWITCH_TYPE_E move_type
                               );

/*****************************************************************************/
//  Description : move to pre item
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ToSelPreItem(
                        CTRLTAB_OBJ_T * tab_ptr
                        );

/*****************************************************************************/
//  Description : move to next item
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ToSelNextItem(
                         CTRLTAB_OBJ_T * tab_ptr
                         );

/*****************************************************************************/
//  Description : Set Scroll Card State
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetScrollCardState(
                              CTRLTAB_OBJ_T *tab_ptr,
                              GUITAB_SWITCH_TYPE_E type
                              );

/*****************************************************************************/
//  Description : update item rectangle.TAB控件显示item的有效区域
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateTabItemTotalRect(
                                  CTRLTAB_OBJ_T *tab_ptr
                                  );

/*****************************************************************************/
//  Description : update item height.TAB控件显示item的高度完全相同
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateTabItemHeight(
                               CTRLTAB_OBJ_T *tab_ptr
                               );

/*****************************************************************************/
//  Description : 更新当前页显示的item各自的区域,并且返回当前页显示个数.
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurPageItemRect(
								 CTRLTAB_OBJ_T	*tab_ptr
								 );

/*****************************************************************************/
//  Description : Get tab item rect
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateItemRect(
                                CTRLTAB_OBJ_T *tab_ptr, 
                                GUI_RECT_T pre_item_rect,
                                GUI_RECT_T *cur_item_rect_ptr,
                                uint32 index
                                );

/*****************************************************************************/
//  Description : 更新当前页显示的item各自的区域,并且返回当前页显示个数.
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 InitCurPageItemRect(
                                 CTRLTAB_OBJ_T *tab_ptr
                                 );

/*****************************************************************************/
//  Description : 计算item的宽度，并设置item宽度数值
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 CalculateItemWidth(
                                CTRLTAB_OBJ_T *tab_ptr,
                                GUITAB_ITEM_T *item_ptr
                                );

/*****************************************************************************/
//  Description : 获取text的长度，计算到每个象素
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetTextLength(
                           CTRLTAB_OBJ_T *tab_ptr, 
                           GUITAB_CAPTION_T text_data
                           );

/*****************************************************************************/
//  Description : create item data
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTabItemData(
                                CTRLTAB_OBJ_T*      tab_ptr,  // the list control pointer
                                GUITAB_ITEM_T*      item_ptr,     // 指定的Item
                                const GUITAB_ITEM_CONTENT_T* item_data_ptr
                                );

/*****************************************************************************/
//  Description : display item description information. while not GUITAB_STATE_SINGLE_LINE
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawTabTitleText(
                            CTRLTAB_OBJ_T * tab_ptr, 
                            uint32 index
                            );

/*****************************************************************************/
//  Description : display item description information.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawTabItem(
                       CTRLTAB_OBJ_T *tab_ptr, 
                       uint32 index
                       );

/*****************************************************************************/
//  Description : 计算item的宽度，并设置item宽度数值
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateAllItemWidth(
                              CTRLTAB_OBJ_T *tab_ptr
                              );

/*****************************************************************************/
//  Description : handle MSG_KEYDOWN_RIGHT message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetMoveItemNum(
                            CTRLTAB_OBJ_T *tab_ptr, 
                            GUITAB_SWITCH_TYPE_E type
                            );

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabConstruct(
                           CTRLBASE_OBJ_T       *base_ctrl_ptr,
                           CTRLTAB_INIT_PARAM_T *init_data_ptr
                           );

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabDestruct(
                          CTRLBASE_OBJ_T *base_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : handle message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabMsg(
                                CTRLBASE_OBJ_T *  base_ctrl_ptr,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                );

/*****************************************************************************/
//  Description : get image rectangle,return true/false
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:true:需要画图片，false：不需要画图片
/*****************************************************************************/
LOCAL BOOLEAN GetItemImageRect(
                               CTRLTAB_OBJ_T *tab_ptr, 
                               uint32 index,
                               GUI_RECT_T *rect_ptr
                               );

/*****************************************************************************/
//  Description : 判断item是否在当前也显示
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemInCurrentPage(
                                  CTRLTAB_OBJ_T *tab_ptr, 
                                  uint32 item_index
                                  );

/*****************************************************************************/
//  Description : 删除不在当前页的动画,释放动画控件
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void DeleteInvalidItemData(
                                 CTRLTAB_OBJ_T *tab_ptr
                                 );

/*****************************************************************************/
//  Description : display item icon
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawItemIcon(
                        CTRLTAB_OBJ_T *tab_ptr, 
                        GUI_RECT_T disp_rect, 
                        MMI_IMAGE_ID_T image_id
                        );

/*****************************************************************************/
//  Description : draw select bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DrawSelectBg(
						CTRLTAB_OBJ_T	*tab_ptr,	//in:
						uint32			index		//in:
						);

/*****************************************************************************/
//  Description : draw tp down bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DrawTpDownBg(
						CTRLTAB_OBJ_T	*tab_ptr,	//in:
						uint32			index		//in:
						);

/*****************************************************************************/
//  Description : is item visible
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemValid(
                          GUITAB_ITEM_T *item_ptr
                          );

/*****************************************************************************/
//  Description : 设置有效的item列表
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ResetValidItemList(
                              CTRLTAB_OBJ_T *tab_ptr
                              );

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL GUITAB_ITEM_T* GetValidItemPtr(
                                     const CTRLTAB_OBJ_T* tab_ptr,
                                     uint32              index  //from 0 to total_item_num-1
                                     );

/*****************************************************************************/
//  Description :  把一个Item插入有效的Item链表中
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertValidItem(
                              CTRLTAB_OBJ_T *tab_ptr, 
                              GUITAB_ITEM_T *item_ptr,
                              uint32 pos
                              );

/*****************************************************************************/
//  Description :  把一个Item插入有效的Item链表中
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteValidItem(
                              CTRLTAB_OBJ_T *tab_ptr, 
                              GUITAB_ITEM_T *item_ptr
                              );

/*****************************************************************************/
//  Description : 重新调整区域
//  Global resource dependence : 
//  Author:  xiaoqinglu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T ReadjustAnimRect(
                                  GUI_RECT_T rect, 
                                  int16 width, 
                                  int16 height
                                  );

/*****************************************************************************/
//  Description : 通过原始列表的索引号，查找有效item列表对应的索引号
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidIndexByItemIndex(
                                       CTRLTAB_OBJ_T *tab_ptr, 
                                       uint32 item_index,
                                       uint32 *valid_index_ptr // out
                                       );

/*****************************************************************************/
//  Description : 通过原始列表的索引号，查找有效item列表对应的索引号
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemIndexByValidIndex(
                                       CTRLTAB_OBJ_T *tab_ptr, 
                                       uint32 valid_index,
                                       uint32 *index_ptr // out
                                       );

/*****************************************************************************/
//  Description : handle MSG_TIMER message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void CreateTabTitleCtrl(
                              CTRLTAB_OBJ_T * tab_ptr
                              );

/*****************************************************************************/
//  Description : 重新设置title
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void ResetTabTitleText(
                             CTRLTAB_OBJ_T * tab_ptr, 
                             uint32 index,
                             BOOLEAN is_update
                             );

/*****************************************************************************/
//  Description : 发送图片解析结果给应用窗口
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SendAnimDispResultToWin(
                                   CTRLTAB_OBJ_T    *tab_ptr,
                                   MMI_MESSAGE_ID_E msg_id,
                                   DPARAM            param
                                   );

/*****************************************************************************/
// 	Description : send msg to win, notify it append the next item content
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void NotifyAppendNextItemContent(
                                       CTRLTAB_OBJ_T   *tab_ptr
                                       );

/*****************************************************************************/
//  Description : tab set rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabCtrlSetRect( 
                             CTRLBASE_OBJ_T *   base_ctrl_ptr, 
                             const GUI_RECT_T * rect_ptr
                             );

/*****************************************************************************/
// Description : release title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL void ReleaseTabCommonTitleInfo(
                                     CTRLTAB_OBJ_T* tab_ptr
                                     );

/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: if is_set = TRUE, set title; else if is_set = FALSE, release title
/*****************************************************************************/
LOCAL BOOLEAN SetTabCommonTitleInfo(
                                    CTRLTAB_OBJ_T* tab_ptr,
                                    MMI_STRING_T *str_ptr,
                                    BOOLEAN is_set
                                    );

/*****************************************************************************/
//  Description : send need item data message
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SendTabNotifyMessage(
                                CTRLTAB_OBJ_T    *tab_ptr,
                                uint32           index,
                                uint32           pre_index,
                                MMI_MESSAGE_ID_E msg_id,
                                BOOLEAN          is_child,
                                MMI_HANDLE_T     handle
                                );

/*****************************************************************************/
//  Description : tab set rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateScrollCardRect( 
                                CTRLTAB_OBJ_T  *tab_ptr
                                );

/*****************************************************************************/
//  Description : 更新当前页显示的item各自的区域,并且返回当前页显示个数.
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void AdjustValidItemWidth(CTRLTAB_OBJ_T *tab_ptr);

/*****************************************************************************/
//  Description : init tab class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabCtrlInitVtbl(
    CTRLTAB_VTBL_T        *tab_vtbl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TabCtrlPackInitParam (
    GUITAB_INIT_DATA_T     *tab_init_ptr,   //in
    CTRLTAB_INIT_PARAM_T   *tab_param_ptr   //out
);

/*****************************************************************************/
//  Description : get tab pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTAB_OBJ_T* GetTabPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TabTypeOf (
    CTRLBASE_OBJ_T     *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : set the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void SetStatePtr( 
                       CTRLTAB_OBJ_T* tab_ptr,
                       GUITAB_STATE_T src_state,
                       BOOLEAN is_true
                       );

/*****************************************************************************/
//  Description : get the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStatePtr(
                          const CTRLTAB_OBJ_T* tab_ptr,
                          GUITAB_STATE_T    src_state
                          );

/*****************************************************************************/
//  Description : get the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemStatePtr(
                              const GUITAB_ITEM_T* item_ptr,
                              GUITAB_ITEM_STATE_T src_state
                              );

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetItemStatePtr( 
                           GUITAB_ITEM_T* item_ptr,
                           GUITAB_ITEM_STATE_T  src_state,
                           BOOLEAN is_true
                           );

/*****************************************************************************/
//  Description : Set the max item of list
//  Global resource dependence : 
//  Author:Lin.Lin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMaxItemByPtr(
                               CTRLTAB_OBJ_T *tab_ptr,
                               uint32        max_item_num
                               );

/*****************************************************************************/
//  Description : Add the item to tab list
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertItemByPtr(
                               CTRLTAB_OBJ_T*       tab_ptr,
                               const GUITAB_ITEM_T* item_ptr,
                               uint32               pos
                               );

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetItemContentByPtr(
                                  CTRLTAB_OBJ_T               *tab_ptr,
                                  const GUITAB_ITEM_CONTENT_T *item_data_ptr,
                                  uint32                      index
                                  );

/*****************************************************************************/
//  Description : set item image
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:设置非选中状态下的图标
/*****************************************************************************/
LOCAL BOOLEAN SetItemImagePtr(
                              CTRLTAB_OBJ_T *tab_ptr,   // 控件ID
                              GUITAB_IMAGE_DATA_T *img_ptr,  // 设置数据
                              GUI_POINT_T start_point,
                              uint32 index             // 索引号
                              );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get tab type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TAB_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "tab",                                         // control name
                       sizeof (CTRLTAB_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) TabConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) TabDestruct,      // control destructor function
                       sizeof (CTRLTAB_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) TabCtrlPackInitParam,     // convert init param
                       sizeof (CTRLTAB_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) TabCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init tab class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabCtrlInitVtbl (
    CTRLTAB_VTBL_T        *tab_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) tab_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = HandleTabMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->SetRect = TabCtrlSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void TabCtrlPackInitParam (
    GUITAB_INIT_DATA_T     *tab_init_ptr,   //in
    CTRLTAB_INIT_PARAM_T   *tab_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) tab_param_ptr;

    if (PNULL == tab_init_ptr || PNULL == tab_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = tab_init_ptr->both_rect;
    
    tab_param_ptr->state           = tab_init_ptr->state;
    tab_param_ptr->is_active_item  = tab_init_ptr->is_active_item;
    tab_param_ptr->max_item_num    = tab_init_ptr->max_item_num;
    tab_param_ptr->style           = tab_init_ptr->style;
}

/*****************************************************************************/
//  Description : construct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabConstruct(
                           CTRLBASE_OBJ_T       *base_ctrl_ptr,
                           CTRLTAB_INIT_PARAM_T *tab_param_ptr
                           )
{
    BOOLEAN         result = TRUE;
	CTRLTAB_OBJ_T   *tab_ptr = (CTRLTAB_OBJ_T*)base_ctrl_ptr;

    if ( PNULL == tab_ptr || PNULL == tab_param_ptr )
    {
        //SCI_TRACE_LOW:"GUITAB_Init init param null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_3443_112_2_18_3_21_55_353,(uint8*)"");
        return FALSE;
    }

	tab_ptr->tp_down_item_index = 0xFFFFFFFF;

    tab_ptr->tab_state = tab_param_ptr->state;
    tab_ptr->style = tab_param_ptr->style;
    tab_ptr->max_item_num = tab_param_ptr->max_item_num;

    SetMaxItemByPtr(tab_ptr, tab_param_ptr->max_item_num);

    base_ctrl_ptr->lcd_dev_info = *(MMITHEME_GetDefaultLcdDev());
    base_ctrl_ptr->bg = MMITHEME_GetCommonBg();

	// get theme style
	if (MMITHEME_GetTabStyle(&tab_ptr->tab_style))
    {
        tab_ptr->left_scroll_card.pressed_image_id = tab_ptr->tab_style.left_scroll_pressed_image_id;
		tab_ptr->left_scroll_card.disabled_image_id = tab_ptr->tab_style.left_scroll_disabled_image_id;
        tab_ptr->left_scroll_card.released_image_id = tab_ptr->tab_style.left_scroll_released_image_id;

        tab_ptr->right_scroll_card.pressed_image_id = tab_ptr->tab_style.right_scroll_pressed_image_id;
		tab_ptr->right_scroll_card.released_image_id = tab_ptr->tab_style.right_scroll_released_image_id;
		tab_ptr->right_scroll_card.disabled_image_id = tab_ptr->tab_style.right_scroll_disabled_image_id;

        tab_ptr->item_width = tab_ptr->tab_style.item_default_width;
    }

	// scroll
	tab_ptr->is_scroll = FALSE;
    tab_ptr->is_rotative = FALSE;
    tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
    tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
    tab_ptr->item_space = 0;

	// timer
	tab_ptr->timeout = GUITAB_DEFAULT_TIMEOUT;
	tab_ptr->timer_status = GUITAB_TIMER_NULL;
	tab_ptr->timer_id = 0;

    ResetTabItem(tab_ptr);

	// item other information
	SCI_MEMSET(&tab_ptr->item_rect, 0, sizeof(GUI_RECT_T));

    tab_ptr->is_reset_title = TRUE;
    tab_ptr->title_ptr = PNULL;

    //title
    if (MMITHEME_IsIstyle())
    {
        CreateTabTitleCtrl(tab_ptr);
    }

    tab_ptr->tab_state |= GUITAB_STATE_SYNC_DECODE_ANIM | GUITAB_STATE_IS_ADAPT_WIDTH;

    // additional string length is set to 0
    tab_ptr->added_string.text.text_buffer.wstr_len = 0;

	SetStatePtr(tab_ptr, GUITAB_STATE_SWITCH_ITEM, TRUE);
    
    return result;
}

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabDestruct(
                          CTRLBASE_OBJ_T *base_ctrl_ptr
                          )
{
    BOOLEAN result = TRUE;
    CTRLTAB_OBJ_T *tab_ptr = (CTRLTAB_OBJ_T *)base_ctrl_ptr;

    if(PNULL != tab_ptr)
    {
        StopTabTimer(tab_ptr);

		//stop item text timer
		StopItemTextScrollTimer(tab_ptr);
		tab_ptr->str_index = 0;

        if (PNULL != tab_ptr->title_ptr)
        {
            MMK_DestroyControl( GUICTRL_GetCtrlHandle( tab_ptr->title_ptr) );
            tab_ptr->title_ptr = PNULL;
        }

        DestroyAllITabtemData( tab_ptr, FALSE );

        if (PNULL != tab_ptr->item_ptr)
        {
            SCI_FREE(tab_ptr->item_ptr);
        }
        
        ReleaseTabCommonTitleInfo(tab_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"TabDestruct input param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_882_112_2_18_3_21_50_350,(uint8*)"");
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle msg.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabMsg(
                                CTRLBASE_OBJ_T *  base_ctrl_ptr,
                                MMI_MESSAGE_ID_E  msg_id,
                                DPARAM            param
                                )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
	CTRLTAB_OBJ_T   *tab_ptr = (CTRLTAB_OBJ_T *)base_ctrl_ptr;

	if (PNULL == base_ctrl_ptr || !TabTypeOf( base_ctrl_ptr ))
    {
        //SCI_TRACE_LOW:"HandleTabMsg error ctrl type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_4480_112_2_18_3_21_57_358,(uint8*)"");

        return MMI_RESULT_FALSE;
    }

	switch (msg_id)
	{
	case MSG_CTL_OPEN:
        CreateTabTitleCtrl(tab_ptr);
        SetStatePtr(tab_ptr, GUITAB_STATE_FOCUS, TRUE);
		break;

	case MSG_CTL_LOSE_FOCUS:
        if (GetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT)) // 当刷新过一次，才可以把is_reset_title设置为FALSE
        {
            tab_ptr->is_reset_title = FALSE;
        }

        SetStatePtr(tab_ptr, GUITAB_STATE_FOCUS, FALSE);

        StopTabTimer(tab_ptr);

		//stop item text timer
		StopItemTextScrollTimer(tab_ptr);
		tab_ptr->str_index = 0;
		break;

	case MSG_CTL_GET_FOCUS:
		SetStatePtr(tab_ptr, GUITAB_STATE_FOCUS, TRUE);
		break;

	case MSG_CTL_PAINT:
        if (!GetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT)) // 永远保持TRUE，知道被销毁
        {
            SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, TRUE);
        }

		DrawCtrl(tab_ptr);
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
        //MMK_DelayTpMove();
		result = HandleTabTpDown(tab_ptr,param);
		break;

	case MSG_TP_PRESS_MOVE:
		result = HandleTabTpMove(tab_ptr,param);
		break;

	case MSG_TP_PRESS_UP:
		result = HandleTabTpUp(tab_ptr,param);
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	case MSG_KEYDOWN_LEFT:
    case MSG_KEYREPEAT_LEFT:
		result = HandleLeftKeyDown(tab_ptr);
		break;

	case MSG_KEYDOWN_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
		result = HandleRightKeyDown(tab_ptr);
		break;

	case MSG_KEYUP_LEFT:
	case MSG_KEYUP_RIGHT:
	case MSG_KEYPRESSUP_LEFT:
	case MSG_KEYPRESSUP_RIGHT:
        result = HandleKeyUp(tab_ptr);
		break;

	case MSG_TIMER:
        if (*(uint8*)param == tab_ptr->timer_id)
		{
			result = HandleTimer(tab_ptr);
		}
		else if (tab_ptr->text_timer_id == *(uint8*)param)
        {
            HandleItemTextScrollTimer(tab_ptr);
        }
		else
		{
			result = MMI_RESULT_FALSE;
		}
		break;

	case MSG_CTL_CLOSE:
        StopTabTimer(tab_ptr);
		break;

    case MSG_CTL_ANIM_DISPLAY_IND:
        {
            GUIANIM_DISPLAY_IND_T anim_display_ind = *(GUIANIM_DISPLAY_IND_T *)param;

            SendAnimDispResultToWin(tab_ptr, MSG_NOTIFY_TAB_ANIM_DISPLAY_IND, param);

            if( GUIANIM_RESULT_INTERRUPT != anim_display_ind.result )
            {
                //send msg to win,append the next item content
                NotifyAppendNextItemContent(tab_ptr);
            }
        }
        break;

	default:
		result = MMI_RESULT_FALSE;
		break;
	}

	return(result);     
}

/*****************************************************************************/
//  Description : tab set rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TabCtrlSetRect( 
                             CTRLBASE_OBJ_T *   base_ctrl_ptr, 
                             const GUI_RECT_T * rect_ptr
                             )
{
    BOOLEAN       result = FALSE;
    CTRLTAB_OBJ_T *tab_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL!= base_ctrl_ptr && PNULL != rect_ptr ); /*assert verified*/
    if ( (PNULL== base_ctrl_ptr) || (PNULL == rect_ptr) )
    {
        return FALSE;
    }

    tab_ptr = (CTRLTAB_OBJ_T *)base_ctrl_ptr;

    if (!GUI_EqualRect(base_ctrl_ptr->rect, *rect_ptr))
    {
        base_ctrl_ptr->rect = *rect_ptr;
		base_ctrl_ptr->display_rect = *rect_ptr;
        
        MMITHEME_GetTabStyle(&tab_ptr->tab_style);

        GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));

		//stop item text timer
		StopItemTextScrollTimer(tab_ptr);
		tab_ptr->str_index = 0;

        // set scroll card rect
        UpdateScrollCardRect(tab_ptr);

        // update heght & total item rect
        UpdateTabItemHeight(tab_ptr);

        // update item total rect
        UpdateTabItemTotalRect(tab_ptr);

        // update all items width
        UpdateAllItemWidth(tab_ptr);

        // 这里要重新设置valid list
        ResetValidItemList(tab_ptr); // 更新tab项

        InitCurPageItemRect(tab_ptr);

        // 删除无效的item数据
        DeleteInvalidItemData(tab_ptr);

        // reset scroll state
        ResetScrollState(tab_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get tab pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTAB_OBJ_T* GetTabPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != base_ctrl_ptr)
    {
        SCI_ASSERT (TabTypeOf (base_ctrl_ptr)); /*assert verified*/
    }

    return (CTRLTAB_OBJ_T*) base_ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TabTypeOf (
    CTRLBASE_OBJ_T     *base_ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)base_ctrl_ptr, CTRL_TAB_TYPE);
}

/*****************************************************************************/
//  Description : tab set rect
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateScrollCardRect( 
                                CTRLTAB_OBJ_T  *tab_ptr
                                )
{
    uint16 width = 0;
    uint16 height = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

	if (PNULL == tab_ptr)
	{
        return;
	}

    if (GUI_IsInvalidRect(base_ctrl_ptr->rect)) // 区域无效时
    {
        return;
    }

    width  = (uint16)MIN(tab_ptr->tab_style.scroll_card_def_width, (uint16)(base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1));
    height = (uint16)MIN(tab_ptr->tab_style.item_default_height, (uint16)(base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1));    
    
    if (MMITHEME_CheckImageID(tab_ptr->left_scroll_card.pressed_image_id))
    {
        // left scroll rectangle
        tab_ptr->left_scroll_card.card_rect.left = base_ctrl_ptr->rect.left;
        tab_ptr->left_scroll_card.card_rect.right = (int16)(base_ctrl_ptr->rect.left + width - 1);
        tab_ptr->left_scroll_card.card_rect.top = (int16)(base_ctrl_ptr->rect.bottom - height + 1);
        tab_ptr->left_scroll_card.card_rect.bottom = base_ctrl_ptr->rect.bottom;
    }
    else
    {
        SCI_MEMSET(&tab_ptr->left_scroll_card.card_rect, 0, sizeof(GUI_RECT_T));
    }
    
    if (MMITHEME_CheckImageID(tab_ptr->right_scroll_card.pressed_image_id))
    {
        // right scroll rectangle
        tab_ptr->right_scroll_card.card_rect.left = (int16)(base_ctrl_ptr->rect.right - width + 1);
        tab_ptr->right_scroll_card.card_rect.right = base_ctrl_ptr->rect.right;
        tab_ptr->right_scroll_card.card_rect.top = (int16)(base_ctrl_ptr->rect.bottom - height + 1);
        tab_ptr->right_scroll_card.card_rect.bottom = base_ctrl_ptr->rect.bottom;
    }
    else
    {
        SCI_MEMSET(&tab_ptr->right_scroll_card.card_rect, 0, sizeof(GUI_RECT_T));
    }

    return;
}

/*****************************************************************************/
//  Description : update item rectangle.TAB控件显示item的有效区域
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateTabItemTotalRect(
                                  CTRLTAB_OBJ_T *tab_ptr
                                  )
{
    uint16		left_scroll_width = 0;
    uint16		right_scroll_width = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    tab_ptr->item_rect = base_ctrl_ptr->rect;

    // 如果有滚动块，扣除左右的区域
    if (!GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL))
    {
        left_scroll_width = tab_ptr->left_scroll_card.card_rect.right - tab_ptr->left_scroll_card.card_rect.left + 1;
        right_scroll_width = tab_ptr->right_scroll_card.card_rect.right - tab_ptr->right_scroll_card.card_rect.left + 1;

		tab_ptr->item_rect.left += left_scroll_width;
		tab_ptr->item_rect.right -= right_scroll_width;
	}

	tab_ptr->item_rect.top = (int16)(base_ctrl_ptr->rect.bottom - tab_ptr->item_height + 1);/*lint !e737*/

    // 设置滚动块的top, bottom
	if (!GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL))
	{
        tab_ptr->left_scroll_card.card_rect.top = tab_ptr->item_rect.top;
        tab_ptr->left_scroll_card.card_rect.bottom = tab_ptr->item_rect.bottom;
        
        tab_ptr->right_scroll_card.card_rect.top = tab_ptr->item_rect.top;
        tab_ptr->right_scroll_card.card_rect.bottom = tab_ptr->item_rect.bottom;
    }

	return;
}

/*****************************************************************************/
//  Description : 更新当前页显示的item各自的区域,并且返回当前页显示个数.
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 InitCurPageItemRect(
                                 CTRLTAB_OBJ_T *tab_ptr
                                 )
{
    uint32 i = 0;
    uint32 total_width = 0;
    uint32 item_width = 0;
    uint32 cur_page_item_num = 0;
    uint32 start_item_index = 0;
    uint32 item_index = 0;
    GUI_RECT_T rect = {0};
    GUITAB_ITEM_T *item_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return 0;
    }

    if (tab_ptr->valid_item_list.item_num == 0)
    {
        return 0;
    }

    AdjustValidItemWidth(tab_ptr);

    total_width = tab_ptr->item_rect.right - tab_ptr->item_rect.left + 1;

    // 先向后计算
    start_item_index = tab_ptr->cur_sel_item_index; // 设置起始显示的item索引号
    item_index = tab_ptr->cur_sel_item_index;
    item_ptr = GetValidItemPtr(tab_ptr, item_index);
    while (item_ptr != PNULL)
    {
		if (item_width < total_width)
		{
			item_width += item_ptr->item_width;

			cur_page_item_num++;
            item_index++;
            item_ptr = GetValidItemPtr(tab_ptr, item_index);
		}
		else
		{
			break;
		}
    }

    if (tab_ptr->cur_sel_item_index > 0)
    {
        // 再向前计算
        item_index = tab_ptr->cur_sel_item_index - 1;
        item_ptr = GetValidItemPtr(tab_ptr, item_index);
        while (item_ptr != PNULL)
        {
			if (item_width < total_width)
			{
				item_width += item_ptr->item_width;

				start_item_index = item_index; // 重新设置起始显示的item索引号

				cur_page_item_num++;
                if ((int32)(item_index - 1) < 0)
                {
                    break;
                }
                item_index--;
                item_ptr = GetValidItemPtr(tab_ptr, item_index);
			}
			else
			{
				break;
			}
        }
    }
    
    tab_ptr->start_show_item_index = start_item_index;
    tab_ptr->show_item_num = cur_page_item_num;

	//set offset
	if (start_item_index == tab_ptr->cur_sel_item_index)
	{
		item_width = 0;
		item_ptr = tab_ptr->valid_item_list.item_ptr;
		
		for (i=0; i<start_item_index; i++)
		{
			item_width += item_ptr->item_width;
			item_ptr = item_ptr->next_ptr;
		}

		tab_ptr->offset_x = 0-item_width;
	}
	else
	{
		tab_ptr->offset_x = total_width - tab_ptr->valid_item_list.total_width;
	}
	if (0 < tab_ptr->offset_x)
	{
		tab_ptr->offset_x = 0;
	}

    // tab_ptr->offset_x += tab_ptr->item_rect.left;

    rect = tab_ptr->item_rect;
    rect.left = (int16)(tab_ptr->item_rect.left + tab_ptr->offset_x); // 初始化rect
    rect.right = rect.left;

    for (i = 0; i<tab_ptr->valid_item_list.item_num; i++)
    {
        CalculateItemRect(tab_ptr, rect, &rect, i);
    }

    tab_ptr->cur_sel_item_index = (uint32)((tab_ptr->cur_sel_item_index) % tab_ptr->valid_item_list.item_num);
    tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;

    return cur_page_item_num;
}

/*****************************************************************************/
//  Description : 更新当前页显示的item各自的区域,并且返回当前页显示个数.
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurPageItemRect(
								 CTRLTAB_OBJ_T	*tab_ptr
								 )
{
	BOOLEAN			result = FALSE;
	uint32			i = 0;
	uint32			item_index = 0;
	uint32			total_width = 0;
	uint32			item_width = 0;
	uint32			start_item_index = 0;
	uint32			cur_page_item_num = 0;
	GUI_RECT_T		rect = {0};
	GUITAB_ITEM_T	*item_ptr = PNULL;

	//get current item
	item_ptr = GetValidItemPtr(tab_ptr,tab_ptr->cur_sel_item_index);

	total_width = tab_ptr->item_rect.right - tab_ptr->item_rect.left + 1;

	//current item is display all
	if (item_ptr->rect.left < tab_ptr->item_rect.left)
	{
		start_item_index = tab_ptr->cur_sel_item_index;

		//get item number in current page
		//从当前往后找
		while (item_ptr != PNULL)
		{
			if (item_width < total_width)
			{
				item_width += item_ptr->item_width;

				cur_page_item_num++;
				item_ptr = item_ptr->next_ptr;
			}
			else
			{
				break;
			}
		}

		//set offset
		item_width = 0;
		item_ptr = tab_ptr->valid_item_list.item_ptr;
		
		for (i=0; i<start_item_index; i++)
		{
			item_width += item_ptr->item_width;
			item_ptr = item_ptr->next_ptr;
		}

		tab_ptr->offset_x = 0-item_width;

		result = TRUE;
	}
	else if (item_ptr->rect.right > tab_ptr->item_rect.right)
	{
		//get item number in current page
		//从当前往前找
		item_index = tab_ptr->cur_sel_item_index;
        while (item_ptr != PNULL)
        {
			if (item_width < total_width)
			{
				item_width += item_ptr->item_width;

				//set start item index
				start_item_index = item_index;

				cur_page_item_num++;
				if (0 == item_index)
                {
                    break;
                }
                item_index--;
                item_ptr = GetValidItemPtr(tab_ptr,item_index);
			}
			else
			{
				break;
			}
        }

		//set offset
		item_width = 0;
		if (PNULL != item_ptr) item_ptr = item_ptr->next_ptr;
		
		for (i=(tab_ptr->cur_sel_item_index+1); i<tab_ptr->valid_item_list.item_num; i++)
		{
            if (PNULL == item_ptr) break;
            
			item_width += item_ptr->item_width;
			item_ptr = item_ptr->next_ptr;
		}

		tab_ptr->offset_x = total_width + item_width - tab_ptr->valid_item_list.total_width;
		if (0 < tab_ptr->offset_x)
		{
			tab_ptr->offset_x = 0;
		}
		
		result = TRUE;
	}

    // tab_ptr->offset_x += tab_ptr->item_rect.left;

	if (result)
	{
		tab_ptr->start_show_item_index = start_item_index;
		tab_ptr->show_item_num = cur_page_item_num;

		//calculate all item rect
		rect = tab_ptr->item_rect;
		rect.left = (int16)(tab_ptr->item_rect.left + tab_ptr->offset_x);
		rect.right = rect.left;

		for (i = 0; i<tab_ptr->valid_item_list.item_num; i++)
		{
			CalculateItemRect(tab_ptr, rect, &rect, i);
		}
	}
}

/*****************************************************************************/
//  Description : update item height.TAB控件显示item的高度完全相同
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateTabItemHeight(
                               CTRLTAB_OBJ_T *tab_ptr
                               )
{
	CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    if (GetStatePtr(tab_ptr, GUITAB_STATE_SINGLE_LINE))
	{
		tab_ptr->item_height = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1;
	}
	else
    {
        tab_ptr->item_height = tab_ptr->tab_style.item_default_height;
    }
}

/*****************************************************************************/
//  Description : 计算item的宽度，并设置item宽度数值
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateAllItemWidth(
                              CTRLTAB_OBJ_T *tab_ptr
                              )
{
    uint32 i = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    for (i = 0; i < tab_ptr->total_item_num; i++)
    {
        item_ptr = GetItemPtr(tab_ptr, i);
        CalculateItemWidth(tab_ptr, item_ptr);
    }
    
    AdjustValidItemWidth(tab_ptr);
}

/*****************************************************************************/
//  Description : 计算item的宽度，并设置item宽度数值
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 CalculateItemWidth(
                                CTRLTAB_OBJ_T *tab_ptr,
                                GUITAB_ITEM_T *item_ptr
                                )
{
    uint32 item_width = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr && PNULL != item_ptr); /*assert verified*/
    if ((PNULL == tab_ptr)||(PNULL == item_ptr))
    {
        return 0;
    }

	if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_UIMG_DTEXT))
	{
		if (GetStatePtr(tab_ptr, INNERTAB_STATE_CUSTOMER_WIDTH)) // 如果自设置
        {
            item_width = tab_ptr->item_width;
        }
        else
        {
            item_width = tab_ptr->tab_style.item_default_width;
        }
	}
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_IMG|GUITAB_ITEMSTATE_SHOW_TEXT))
    {
        SCI_ASSERT(PNULL != item_ptr->item_data_ptr); /*assert verified*/
        
        item_width = tab_ptr->item_width;
        item_width += GetTextLength(tab_ptr, item_ptr->item_data_ptr->text_data);
    }
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_IMG))
    {
        if (GetStatePtr(tab_ptr, INNERTAB_STATE_CUSTOMER_WIDTH)) // 如果自设置
        {
            item_width = tab_ptr->item_width;
        }
        else
        {
            item_width = tab_ptr->tab_style.item_default_width;
        }
    }
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_TEXT))
    {
        if (GetStatePtr(tab_ptr, INNERTAB_STATE_CUSTOMER_WIDTH)) // 如果自设置
        {
            item_width = tab_ptr->item_width;
        }
        else
        {
            SCI_ASSERT(PNULL != item_ptr->item_data_ptr); /*assert verified*/
            item_width = GetTextLength(tab_ptr, item_ptr->item_data_ptr->text_data);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    item_ptr->item_width = MAX(item_width + tab_ptr->item_space, tab_ptr->item_width);
    item_ptr->item_width = MIN((int32)item_ptr->item_width,tab_ptr->item_rect.right - tab_ptr->item_rect.left + 1);

    return item_ptr->item_width;
}

/*****************************************************************************/
//  Description : Get tab item rect
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CalculateItemRect(
                                CTRLTAB_OBJ_T   *tab_ptr, 
                                GUI_RECT_T      pre_item_rect,
                                GUI_RECT_T      *cur_item_rect_ptr,
                                uint32          index
                                )
{
    BOOLEAN result = FALSE;
    GUI_RECT_T rect = {0};
    uint32 item_width = 0;
    GUITAB_ITEM_T *item_ptr = GetValidItemPtr(tab_ptr, index);

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == tab_ptr)||(PNULL == item_ptr))
    {
        return FALSE;
    }

    if (index < tab_ptr->valid_item_list.item_num)
    {
        item_width = item_ptr->item_width;
        if(0 != item_width)
        {
			rect = pre_item_rect;
            if (pre_item_rect.right == pre_item_rect.left)
            {
                rect.left = pre_item_rect.right;
            }
            else
            {
                rect.left = (int16)(pre_item_rect.right + 1);
            }
            rect.right = (int16)(rect.left + item_width - 1);/*lint !e737*/
        }
        
        result = TRUE;
        item_ptr->rect = rect;
    }

    if (cur_item_rect_ptr != PNULL)
    {
        *cur_item_rect_ptr = rect;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : 更新当前页显示的item各自的区域,并且返回当前页显示个数.
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void AdjustValidItemWidth(
                                CTRLTAB_OBJ_T *tab_ptr
                                )
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 total_width = 0;
    uint32 left_width = 0;
    uint32 add_width = 0;
    uint32 item_width = 0;
    uint32 red_width = 0;
    uint32 item_num = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;

    if (GetStatePtr(tab_ptr, GUITAB_STATE_IS_ADAPT_WIDTH)
        && 0 < tab_ptr->valid_item_list.item_num)
    {
        item_width = tab_ptr->item_rect.right - tab_ptr->item_rect.left + 1;

        // 当总宽度小于tab宽度的时候才自适应区域
        if (tab_ptr->valid_item_list.total_width < item_width)
        {
            left_width = item_width - tab_ptr->valid_item_list.total_width;
            
            item_num = MIN(tab_ptr->valid_item_list.item_num, tab_ptr->tab_style.default_item_num);
                        
            while (i < tab_ptr->valid_item_list.item_num)
            {
                for (j = 0; (j < item_num) && (i+j < tab_ptr->valid_item_list.item_num); j++)
                {
                    red_width = left_width - total_width; // 剩余
                    add_width = red_width/(item_num - j);
                    
                    item_ptr = GetItemPtr(tab_ptr, i+j);
                    SCI_ASSERT(PNULL != item_ptr);  /*assert verified*/
                    
                    item_ptr->item_width += add_width;
                    total_width += add_width;
                }

                i += item_num;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 获取text的长度，计算到每个象素
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetTextLength(
                           CTRLTAB_OBJ_T *tab_ptr, 
                           GUITAB_CAPTION_T text_data
                           )
{
    uint32   text_len = 0;
    wchar*   str_ptr  = PNULL;
    uint16   str_len  = 0;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return 0;
    }

    if( TABITEM_DATA_TEXT_ID == text_data.text_type)
    {
        MMI_STRING_T temp_str = {0};
        
        MMITHEME_GetLabelTextByLang(text_data.text.text_id, &temp_str );
        
        str_ptr = temp_str.wstr_ptr;
        str_len = temp_str.wstr_len;
    }
    else
    {
        str_ptr = text_data.text.text_buffer.wstr;
        str_len = text_data.text.text_buffer.wstr_len;
    }

    text_len = GUI_CalculateStringPiexlNum(
        str_ptr, 
        str_len,
        tab_ptr->tab_style.font, 
        tab_ptr->tab_style.font_space
        );

    text_len += 2*GUITAB_TEXT_MARGIN;

    return text_len;
}

/*****************************************************************************/
// 	Description : get item text string
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void GetItemTextStr(
						  CTRLTAB_OBJ_T		*tab_ptr,	//in:
						  GUITAB_ITEM_T		*item_ptr,	//in:
						  MMI_STRING_T		*str_ptr	//in/out:
						  )
{
	MMI_STRING_T	text_str = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)tab_ptr;

	if (TABITEM_DATA_TEXT_ID == item_ptr->item_data_ptr->text_data.text_type)
	{
		MMITHEME_GetResText(item_ptr->item_data_ptr->text_data.text.text_id,base_ctrl_ptr->win_handle,&text_str);
	}
	else
	{
		text_str.wstr_ptr = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr;
        text_str.wstr_len = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr_len;
	}

	if (PNULL != str_ptr)
	{
		str_ptr->wstr_ptr = text_str.wstr_ptr;
		str_ptr->wstr_len = text_str.wstr_len;
	}
}

/*****************************************************************************/
// 	Description : get item text rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemTextRect(//is need to draw
							  CTRLTAB_OBJ_T		*tab_ptr,	//in:
							  GUITAB_ITEM_T		*item_ptr,	//in:
							  GUI_RECT_T		*rect_ptr	//in/out:
							  )
{
	BOOLEAN		result = FALSE;
	uint16		font_height = 0;
	GUI_RECT_T	text_rect = {0};
	
    text_rect = item_ptr->rect;
	if (tab_ptr->tab_style.is_display_prg)
	{
		text_rect.bottom = (int16)(text_rect.bottom - tab_ptr->tab_style.prg_height + 1);
	}

	if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_UIMG_DTEXT))
	{
		result = TRUE;

		//get font height
		font_height   = GUIFONT_GetHeight(tab_ptr->tab_style.font);
		text_rect.top = (int16)(text_rect.bottom - font_height);
	}
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_IMG|GUITAB_ITEMSTATE_SHOW_TEXT))
    { // 即显示图片也显示文字，注意，这个判断不能放在后面
        result = TRUE;

        text_rect.left = (int16)(text_rect.left + tab_ptr->item_width);/*lint !e737*/
    }
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_IMG))
    { 
		//只显示图片
    }
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_TEXT))
    { // 只显示文字
		result = TRUE;
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

	if (PNULL != rect_ptr)
	{
		rect_ptr->left   = text_rect.left;
		rect_ptr->top    = text_rect.top;
		rect_ptr->right  = text_rect.right;
		rect_ptr->bottom = text_rect.bottom;
	}

	return (result);
}

/*****************************************************************************/
//  Description : get image rectangle,return true/false
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:true:需要画图片，false：不需要画图片
/*****************************************************************************/
LOCAL BOOLEAN GetItemImageRect(
                               CTRLTAB_OBJ_T *tab_ptr, 
                               uint32 index,
                               GUI_RECT_T *rect_ptr
                               )
{
    BOOLEAN             is_need_to_draw = FALSE;
	uint16				font_height		= 0;
    GUI_RECT_T          disp_rect       = {0};
    GUITAB_ITEM_T       *item_ptr       = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return FALSE;
    }
    
    item_ptr = GetValidItemPtr(tab_ptr, index);

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return FALSE;
    }
    
    disp_rect = item_ptr->rect;
    disp_rect.right = (int16)(item_ptr->rect.left + item_ptr->item_width + (tab_ptr->item_space/2) - 1);/*lint !e737*/
    disp_rect.left = (int16)(item_ptr->rect.left + (tab_ptr->item_space/2));/*lint !e737*/

	if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_UIMG_DTEXT))
	{
		is_need_to_draw = TRUE;
		
		//get font height
		font_height = GUIFONT_GetHeight(tab_ptr->tab_style.font);
		disp_rect.bottom = (int16)(disp_rect.bottom - font_height);
	}
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_IMG|GUITAB_ITEMSTATE_SHOW_TEXT))
    {
        is_need_to_draw = TRUE;
    }
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_IMG))
    {
        is_need_to_draw = TRUE;
    }
    else if (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_TEXT))
    {
        is_need_to_draw = FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"item state = %d, item index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_5436_112_2_18_3_21_59_364,(uint8*)"dd", item_ptr->item_state, index);
        SCI_ASSERT(0); /*assert verified*/
    }

    SCI_ASSERT(rect_ptr != PNULL); /*assert verified*/
    *rect_ptr = disp_rect;

    return is_need_to_draw;
}

/*****************************************************************************/
//  Description : Get show num
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetShowItemNum(
                            CTRLTAB_OBJ_T *tab_ptr
                            )
{
    uint32 show_item_num = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return 0;
    }

    show_item_num = MIN(tab_ptr->show_item_num, tab_ptr->valid_item_list.item_num);

    return show_item_num;
}


/*****************************************************************************/
//  Description : 重新调整区域
//  Global resource dependence : 
//  Author:  xiaoqinglu
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T ReadjustAnimRect(
                                  GUI_RECT_T    rect,
                                  int16         width,
                                  int16         height
                                  )
{
	GUI_RECT_T disp_rect = {0};

	if (0 == width || 0 == height)
	{
		disp_rect = rect;
		return disp_rect;
	}

	if (rect.right - rect.left + 1 <= width)
	{
		disp_rect.left = rect.left;
		disp_rect.right = rect.right;
	}
	else
	{
		disp_rect.left  = (int16)(rect.left + ((rect.right - rect.left + 1 - width)/2));
		disp_rect.right = (int16)(disp_rect.left + width -1);
	}

	if (rect.bottom - rect.top + 1 <= height)
	{
		disp_rect.top = rect.top;
		disp_rect.bottom = rect.bottom;
	}
	else
	{
        // 从下面计算
		disp_rect.bottom = (int16)(rect.bottom - ((rect.bottom - rect.top + 1 - height)/2));
		disp_rect.top	 = (int16)(disp_rect.bottom - height + 1);
	}

	return disp_rect;

}

/*****************************************************************************/
//  Description : 判断item是否在当前也显示
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemInCurrentPage(
                                  CTRLTAB_OBJ_T*    tab_ptr,
                                  uint32            item_index
                                  )
{
    BOOLEAN is_true = TRUE;

    if (item_index < tab_ptr->start_show_item_index 
        || item_index >= tab_ptr->start_show_item_index + tab_ptr->show_item_num)
    {
        is_true = FALSE;
    }

    return is_true;
}

/*****************************************************************************/
//  Description : 删除不在当前页的动画,释放动画控件
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void DeleteInvalidItemData(
                                 CTRLTAB_OBJ_T *tab_ptr
                                 )
{
    uint32 item_index = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;

    for (item_index = 0; item_index < tab_ptr->valid_item_list.item_num; item_index++)
    {
        if (!IsItemInCurrentPage(tab_ptr, item_index))
        {
            item_ptr = GetValidItemPtr(tab_ptr, item_index);

            if (DestroyTabItemData(tab_ptr, item_ptr, TRUE))
            {
                DestroyTabItemDecode(tab_ptr);// 终止解码
            }
        }
    }
}

/*****************************************************************************/
// 	Description : 终止当前正在解码的item
//	Global resource dependence : 
//  Author: Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL void DestroyTabItemDecode(
                                CTRLTAB_OBJ_T *tab_ptr   //in
                                )
{
    GUITAB_ITEM_T  *item_ptr = PNULL;
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T*)tab_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    //正在加载图片
    if (GetStatePtr(tab_ptr,GUITAB_STATE_SETTING_ITEM_DATA))
    {
        //获得当前正在解码的item指针
        item_ptr = GetItemPtr(tab_ptr,tab_ptr->cur_object_index);
        if (PNULL != item_ptr)
        {
            //judge item is need item content
            if (PNULL != item_ptr->item_data_ptr)
            {
                if (0 != item_ptr->add_data_ptr)
                {
                    switch (item_ptr->item_data_ptr->image_data.image_data_type)
                    {
                    case TABITEM_DATA_ANIM_DATA:
                    case TABITEM_DATA_ANIM_PATH:
                    case TABITEM_DATA_IMAGE_ID:
                    case TABITEM_DATA_ANIM_ID:    
                        //终止当前的解码
                        MMK_DestroyControl( GUICTRL_GetCtrlHandle( item_ptr->add_data_ptr) );
                        SetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_NEED_CONTENT, FALSE);
                        item_ptr->add_data_ptr = 0;
                        break;

                    default:
                        SCI_PASSERT(FALSE,("DestroyTabItemDecode:item_data_type %d is error!", item_ptr->item_data_ptr->image_data.image_data_type));/*assert verified*/
                        break;
                    }
                }//end if
            }
        }

        //delete display ind msg
        MMK_DeleteMSGByHwndAndMsg(base_ctrl_ptr->win_handle,MSG_CTL_ANIM_DISPLAY_IND);

        //set list is not content
        SetStatePtr(tab_ptr,GUITAB_STATE_SETTING_ITEM_DATA,FALSE);
    }
}

/*****************************************************************************/
//  Description : 重新设置title
//  Global resource dependence : 
//  Author:  
//  Note:
/*****************************************************************************/
LOCAL void ResetTabTitleText(
                             CTRLTAB_OBJ_T * tab_ptr, 
                             uint32 index,
                             BOOLEAN is_update
                             )
{
    GUITAB_ITEM_T *item_ptr = PNULL;
    wchar           * str_ptr   = PNULL;
    uint16          str_len     = 0;
    MMI_STRING_T    str_info    = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)tab_ptr;
    
    if (tab_ptr->valid_item_list.item_num > 0 && PNULL != tab_ptr->title_ptr)
    {
        
        item_ptr = GetValidItemPtr(tab_ptr, index);
        
        if(PNULL != item_ptr)
        {
            if( TABITEM_DATA_TEXT_ID == item_ptr->item_data_ptr->text_data.text_type)
            {
                MMI_STRING_T temp_str = {0};
            
                MMITHEME_GetResText(
                        item_ptr->item_data_ptr->text_data.text.text_id,
                        base_ctrl_ptr->win_handle, &temp_str );
            
                str_ptr = temp_str.wstr_ptr;
                str_len = temp_str.wstr_len;
            }
            else
            {
                str_ptr = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr;
                str_len = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr_len;
            }
        
            str_info.wstr_len = str_len;
            str_info.wstr_ptr = str_ptr;

            GUITITLE_SetTextByPtr((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr, str_ptr, str_len, is_update);
        }
	}
}

/*****************************************************************************/
//  Description : reset the item pointer of list 
//  Global resource dependence : 
//  Author:Lin.Lin
//  Note:
/*****************************************************************************/
LOCAL void ResetTabItem(
                        CTRLTAB_OBJ_T* tab_ptr  // the list control pointer
                        )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }
    
    // item information
	tab_ptr->total_item_num = 0;
	tab_ptr->show_item_num = 0;
	tab_ptr->cur_sel_item_index = 0;
	tab_ptr->start_show_item_index = 0;
    tab_ptr->cur_object_index = -1;

    // 这里要重新设置valid list
    ResetValidItemList(tab_ptr);

    SetStatePtr(tab_ptr, GUITAB_STATE_SETTING_ITEM_DATA, FALSE );
}

/*****************************************************************************/
//  Description : Set Scroll state
//  Global resource dependence :
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void ResetScrollState(
                             CTRLTAB_OBJ_T *tab_ptr
                             )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }
    
    tab_ptr->is_scroll = IsItemScroll(tab_ptr);

    if(tab_ptr->is_scroll
        && !GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL))
    {
        if (tab_ptr->cur_sel_item_index == 0 || 
            (!tab_ptr->is_rotative && tab_ptr->start_show_item_index == 0))
        {
            tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
            tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
        }
        else if (tab_ptr->cur_sel_item_index >= (tab_ptr->valid_item_list.item_num - 1)
            || (!tab_ptr->is_rotative && (tab_ptr->valid_item_list.item_num - tab_ptr->start_show_item_index) == tab_ptr->show_item_num))
        {
            tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
            tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
        }
        else
        {
            tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
            tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
        }
    }
    else
    {
        tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
        tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
    }
}

/*****************************************************************************/
//  Description : Set Scroll Card State
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetScrollCardState(
                              CTRLTAB_OBJ_T *tab_ptr,
                              GUITAB_SWITCH_TYPE_E type
                              )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    tab_ptr->is_scroll = IsItemScroll(tab_ptr);

    switch(type) 
    {
    case GUITAB_SWITCH_NEXT:
        if (tab_ptr->is_scroll
            && !GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL))
        {
            if (tab_ptr->cur_sel_item_index == 0 
                || (!tab_ptr->is_rotative && tab_ptr->start_show_item_index == 0))
            {
                tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
            }
            else
            {
                tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
            }
            tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
        }
        else
        {
            tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
            tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
        }
    	break;

    case GUITAB_SWITCH_PRE:
        if (tab_ptr->is_scroll
            && !GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL))
        {
            if (tab_ptr->cur_sel_item_index == (tab_ptr->valid_item_list.item_num - 1)
                || (!tab_ptr->is_rotative && (tab_ptr->valid_item_list.item_num - tab_ptr->start_show_item_index) == tab_ptr->show_item_num))
            {
                tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
            }
            else
            {
                tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
            }
            tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_RELEASED;
        }
        else
        {
            tab_ptr->left_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
            tab_ptr->right_scroll_card.card_state = GUITAB_SCROLL_CARD_DISABLED;
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"SetScrollCardState type error! %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_6211_112_2_18_3_22_1_366,(uint8*)"d", type);
        break;
    }
}

/*****************************************************************************/
//  Description : decide whether tab items need scroll.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemScroll(
                           CTRLTAB_OBJ_T *tab_ptr
                           )
{
	BOOLEAN		result = FALSE;

    if (tab_ptr->valid_item_list.item_num > tab_ptr->show_item_num)
    {
        result = TRUE;
    }

	return(result);
}

/*****************************************************************************/
//  Description : 通过原始列表的索引号，查找有效item列表对应的索引号
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidIndexByItemIndex(
                                       CTRLTAB_OBJ_T *tab_ptr, 
                                       uint32 item_index,
                                       uint32 *valid_index_ptr // out
                                       )
{
    BOOLEAN result = FALSE;
    uint32 i = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;

    if(PNULL != tab_ptr && PNULL != valid_index_ptr)
    {
        item_ptr = GetItemPtr(tab_ptr, item_index);
        
        if (PNULL == item_ptr)
        {
            return result;
        }

        for (i = 0; i < tab_ptr->valid_item_list.item_num; i++)
        {
            if (item_ptr == GetValidItemPtr(tab_ptr, i))
            {
                result = TRUE;
                *valid_index_ptr = i;
                break;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 通过原始列表的索引号，查找有效item列表对应的索引号
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemIndexByValidIndex(
                                       CTRLTAB_OBJ_T *tab_ptr, 
                                       uint32 valid_index,
                                       uint32 *index_ptr // out
                                       )
{
    BOOLEAN result = FALSE;
    uint32 i = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;

    if(PNULL != tab_ptr && PNULL != index_ptr)
    {
        item_ptr = GetValidItemPtr(tab_ptr, valid_index);
        
        if (PNULL == item_ptr)
        {
            return result;
        }

        for (i = 0; i < tab_ptr->total_item_num; i++)
        {
            if (item_ptr == GetItemPtr(tab_ptr, i))
            {
                result = TRUE;
                *index_ptr = i;
                break;
            }
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : is item valid
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemValid(
                          GUITAB_ITEM_T *item_ptr
                          )
{
	BOOLEAN		result = FALSE;

    if (PNULL != item_ptr)
    {
		result = (BOOLEAN)(!GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_INVALID));
    }
	
	return (result);
}

/*****************************************************************************/
//  Description : 设置有效的item列表
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ResetValidItemList(
                              CTRLTAB_OBJ_T *tab_ptr
                              )
{
    uint32        item_index    = 0;
    GUITAB_ITEM_T *item_ptr     = PNULL;
    GUITAB_ITEM_T *pre_item_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    tab_ptr->valid_item_list.item_ptr = PNULL;
    tab_ptr->valid_item_list.item_num = 0;
	tab_ptr->valid_item_list.total_width = 0;

    for (item_index = 0; item_index < tab_ptr->total_item_num; item_index++)
    {
        item_ptr = GetItemPtr(tab_ptr, item_index);

        if (PNULL != item_ptr && IsItemValid(item_ptr))
        {
            tab_ptr->valid_item_list.item_num++;

			tab_ptr->valid_item_list.total_width += CalculateItemWidth(tab_ptr,item_ptr);
            
            if (PNULL == tab_ptr->valid_item_list.item_ptr)
            {
                tab_ptr->valid_item_list.item_ptr = item_ptr;
                pre_item_ptr = tab_ptr->valid_item_list.item_ptr;
                item_ptr->next_ptr = PNULL;
            }
            else
            {
                SCI_ASSERT(PNULL != pre_item_ptr); /*assert verified*/
                pre_item_ptr->next_ptr = item_ptr;
                item_ptr->next_ptr = PNULL;
            }
            pre_item_ptr = item_ptr;
        }
    }
}

/*****************************************************************************/
//  Description :  把一个Item插入有效的Item链表中
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertValidItem(
                              CTRLTAB_OBJ_T *tab_ptr, 
                              GUITAB_ITEM_T *item_ptr,
                              uint32 pos
                              )
{
    BOOLEAN result = TRUE;
    GUITAB_ITEM_T *tmp_ptr = PNULL;
    GUITAB_ITEM_T *tmp_next_ptr = PNULL;

    if (pos > tab_ptr->valid_item_list.item_num)
    {
        result = FALSE;
    }

    if (0 == pos) // 表头
    {
        item_ptr->next_ptr = tab_ptr->valid_item_list.item_ptr;
        tab_ptr->valid_item_list.item_ptr = item_ptr;
    }
    else if (pos == tab_ptr->valid_item_list.item_num) // 增加在表尾
    {
        tmp_ptr = GetValidItemPtr(tab_ptr, tab_ptr->valid_item_list.item_num - 1);
        if (PNULL != tmp_ptr)
        {
            tmp_ptr->next_ptr = item_ptr;
        }
        else
        {
            tab_ptr->valid_item_list.item_ptr = item_ptr;
        }
    }
    else
    {
        tmp_ptr = GetValidItemPtr(tab_ptr, pos - 1);
        tmp_next_ptr = GetValidItemPtr(tab_ptr, pos);
        tmp_ptr->next_ptr = item_ptr;
        item_ptr->next_ptr = tmp_next_ptr;
    }

	tab_ptr->valid_item_list.total_width += CalculateItemWidth(tab_ptr,item_ptr);

    return result;
}

/*****************************************************************************/
//  Description :  把一个Item插入有效的Item链表中
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteValidItem(
                              CTRLTAB_OBJ_T *tab_ptr, 
                              GUITAB_ITEM_T *item_ptr
                              )
{
    uint32  index = 0;
    BOOLEAN result = FALSE;
    GUITAB_ITEM_T *tmp_ptr = PNULL;
    GUITAB_ITEM_T *tmp_next_ptr = PNULL;

    if (PNULL != tab_ptr && PNULL != item_ptr && 0 != tab_ptr->valid_item_list.item_num)
    {
        for (index = 0; index < tab_ptr->valid_item_list.item_num; index++)
        {
            tmp_ptr = GetValidItemPtr(tab_ptr, index);

            if (tmp_ptr == item_ptr)
            {
                result = TRUE;
                break;
            }
        }

        if (result && PNULL != tmp_ptr)
        {
            if (0 == index) // 删除表头
            {
                tab_ptr->valid_item_list.item_ptr = tmp_ptr->next_ptr;
            }
            else if (index == tab_ptr->valid_item_list.item_num - 1) // 删除表尾
            {
                tmp_next_ptr = GetValidItemPtr(tab_ptr, index - 1);
                tmp_next_ptr->next_ptr = PNULL;
            }
            else
            {
                tmp_ptr = GetValidItemPtr(tab_ptr, index - 1);
                tmp_next_ptr = GetValidItemPtr(tab_ptr, index + 1);
                tmp_ptr->next_ptr = tmp_next_ptr;
            }

			tab_ptr->valid_item_list.total_width -= CalculateItemWidth(tab_ptr,item_ptr);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL GUITAB_ITEM_T* GetValidItemPtr(
                                     const CTRLTAB_OBJ_T* tab_ptr,
                                     uint32              index  //from 0 to total_item_num-1
                                     )
{
    uint32 item_index = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return PNULL;
    }

    item_ptr = tab_ptr->valid_item_list.item_ptr;

    for (item_index = 0; (item_ptr != PNULL) && (item_index < tab_ptr->valid_item_list.item_num); item_index++)
    {
        if (item_index == index)
        {
            break;
        }

        item_ptr = item_ptr->next_ptr;
    }
    
    return item_ptr;
}

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL GUITAB_ITEM_T* GetItemPtr(
                                 const CTRLTAB_OBJ_T* tab_ptr,
                                 uint32              index  //from 0 to total_item_num-1
                                 )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return PNULL;
    }

    if (index >= tab_ptr->total_item_num || 0 == tab_ptr->total_item_num)
    {
        return PNULL;
    }
    else
    {
        return tab_ptr->item_ptr + index;
    }
}

/*****************************************************************************/
//  Description : draw the whole control.
//  Global resource dependence :
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawCtrl(
                    CTRLTAB_OBJ_T *tab_ptr
                    )
{
	CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }
    
	//set scroll card rect
	UpdateScrollCardRect(tab_ptr);

    //update height & total item rect
    UpdateTabItemHeight(tab_ptr);

    //update item total rect
    UpdateTabItemTotalRect(tab_ptr);

	//update all items width
    UpdateAllItemWidth(tab_ptr);

    //draw back ground image
	DrawBackGround(tab_ptr, base_ctrl_ptr->display_rect);

    //display additional string
    DrawAddedString(tab_ptr);

	//draw all tab items
	DrawAllTabItems(tab_ptr);

    //reset scroll state
    ResetScrollState(tab_ptr);

    //draw scroll
	DrawScroll(tab_ptr);

	//draw progress
	DrawPrg(tab_ptr);

    //send msg MSG_NOTIFY_TAB_PAINTED to parent win
    MMK_SendMsg(base_ctrl_ptr->win_handle, MSG_NOTIFY_TAB_PAINTED, PNULL);

    return;
}

/*****************************************************************************/
//  Description : draw back ground image or color.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawBackGround(
                          CTRLTAB_OBJ_T		*tab_ptr,
                          GUI_RECT_T disp_rect
                          )
{
	GUI_BG_T			bg = {0};
	GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;
    
	if (PNULL == tab_ptr)
	{
		return;
	}
	//set image rect
    img_rect.left   = (int16)(base_ctrl_ptr->display_rect.left - base_ctrl_ptr->rect.left);
    img_rect.top    = (int16)(base_ctrl_ptr->display_rect.top - base_ctrl_ptr->rect.top);
    img_rect.right  = (int16)(img_rect.left + base_ctrl_ptr->display_rect.right - base_ctrl_ptr->display_rect.left);
    img_rect.bottom = (int16)(img_rect.top + base_ctrl_ptr->display_rect.bottom - base_ctrl_ptr->display_rect.top);

    //set bg display info
    bg_display.is_transparent = TRUE;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = disp_rect;//tab_ptr->display_rect;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = base_ctrl_ptr->win_handle;

	if (MMITHEME_CheckImageID(tab_ptr->tab_style.bg_image_id))
	{
		bg.bg_type = GUI_BG_IMG;
		bg.img_id  = tab_ptr->tab_style.bg_image_id;
	}
	else
	{
		bg.bg_type = GUI_BG_COLOR;
		bg.color   = tab_ptr->tab_style.bg_color;
	}

    //display bg
    GUI_DisplayBg(&bg,
        &bg_display,
        &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
//  Description : draw left scroll image and right scroll image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawScroll(CTRLTAB_OBJ_T * tab_ptr)
{
	MMI_IMAGE_ID_T		image_id = 0;
	GUI_COLOR_T			color = 0;
    GUI_RECT_T          display_rect = {0};
    uint16              width = 0;
    uint16              height = 0;
    CTRLBASE_OBJ_T *    base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;
	
    if (!GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL))
    {
        if (tab_ptr->left_scroll_card.card_state == GUITAB_SCROLL_CARD_PRESSED)
        {
            image_id = tab_ptr->left_scroll_card.pressed_image_id;
            color    = tab_ptr->tab_style.sel_item_color;
        }
        else if (tab_ptr->left_scroll_card.card_state == GUITAB_SCROLL_CARD_RELEASED)
        {
            image_id = tab_ptr->left_scroll_card.released_image_id;
            color    = tab_ptr->tab_style.unsel_item_color;
        }
        else
        {
            image_id = tab_ptr->left_scroll_card.disabled_image_id;
            color    = tab_ptr->tab_style.sel_item_color;
        }
	
        IMG_EnableTransparentColor(TRUE); 

        // draw left scroll	
        if (MMITHEME_CheckImageID(image_id))
        {
            GUIRES_GetImgWidthHeight(&width,&height,tab_ptr->left_scroll_card.pressed_image_id, base_ctrl_ptr->win_handle);

            display_rect = GUI_GetCenterRect(tab_ptr->left_scroll_card.card_rect, width, height);

            GUIRES_DisplayImg(PNULL,
                &display_rect,
                PNULL,
                base_ctrl_ptr->win_handle,
                image_id,
                &base_ctrl_ptr->lcd_dev_info);
        }
        else
        {
            GUI_FillRect(&base_ctrl_ptr->lcd_dev_info, tab_ptr->left_scroll_card.card_rect, color);
        }

        // right card begin
        if (tab_ptr->right_scroll_card.card_state == GUITAB_SCROLL_CARD_PRESSED)
        {
            image_id = tab_ptr->right_scroll_card.pressed_image_id;
            color    = tab_ptr->tab_style.sel_item_color;
        }
        else if (tab_ptr->right_scroll_card.card_state == GUITAB_SCROLL_CARD_RELEASED)
        {
            image_id = tab_ptr->right_scroll_card.released_image_id;
            color    = tab_ptr->tab_style.unsel_item_color;
        }
        else
        {
            image_id = tab_ptr->right_scroll_card.disabled_image_id;
            color    = tab_ptr->tab_style.sel_item_color;
        }

        // draw right scroll
        if (MMITHEME_CheckImageID(image_id))
        {
            GUIRES_GetImgWidthHeight(
                    &width,&height,
                    tab_ptr->right_scroll_card.pressed_image_id,
                    base_ctrl_ptr->win_handle);

            display_rect = GUI_GetCenterRect(tab_ptr->right_scroll_card.card_rect, width, height);

            GUIRES_DisplayImg(PNULL,
                &display_rect,
                PNULL,
                base_ctrl_ptr->win_handle,
                image_id,
                &base_ctrl_ptr->lcd_dev_info);
        }
        else
        {
            GUI_FillRect(&base_ctrl_ptr->lcd_dev_info, tab_ptr->right_scroll_card.card_rect, color);
        }

        IMG_EnableTransparentColor(FALSE);
    }
}

/*****************************************************************************/
// 	Description : display progress
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DrawPrg(
				   CTRLTAB_OBJ_T	*tab_ptr
				   )
{
	int16			item_width = 0;
	uint16			bar_len = 0;
	GUI_RECT_T		lcd_rect = {0};
	GUI_RECT_T		img_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

	if (PNULL == tab_ptr)
	{
		return;
	}
    item_width = tab_ptr->item_rect.right - tab_ptr->item_rect.left + 1;

	if (tab_ptr->tab_style.is_display_prg && tab_ptr->valid_item_list.total_width > (uint16)item_width)
	{
		//get bar len
		bar_len = (uint16)(base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1);
		bar_len = (uint16)(bar_len / tab_ptr->valid_item_list.item_num);
		if (bar_len < GUITAB_MIN_BAR_LENGTH)
        {
            bar_len = GUITAB_MIN_BAR_LENGTH;
        }

		//set lcd rect
		lcd_rect	   = base_ctrl_ptr->rect;
		lcd_rect.top   = (int16)(lcd_rect.bottom - tab_ptr->tab_style.prg_height + 1);
		lcd_rect.left  = (int16)(lcd_rect.left + tab_ptr->cur_sel_item_index * bar_len);/*lint !e737*/
		lcd_rect.right = (int16)(lcd_rect.left + bar_len - 1);

        //图片上下各留1px
        lcd_rect.top += 1;
        lcd_rect.bottom -= 1;

		//set image rect
		img_rect.left   = (int16)(bar_len - 1);
		img_rect.bottom = (int16)(lcd_rect.bottom - lcd_rect.top);

		//set image rect
		GUIRES_DisplayImg(PNULL,
			&lcd_rect,
			&img_rect,
			base_ctrl_ptr->win_handle,
			tab_ptr->tab_style.prg_scroll_id,
			&base_ctrl_ptr->lcd_dev_info);
	}
}

/*****************************************************************************/
//  Description : display additional string.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawAddedString(
                           CTRLTAB_OBJ_T * tab_ptr
                           )
{
    GUITAB_CAPTION_T added_string = tab_ptr->added_string;

    if (PNULL != tab_ptr->title_ptr)
    {
        // 设置文本信息
        GUITITLE_SetSubTextByPtr(    
            (CTRLTITLE_OBJ_T  *)tab_ptr->title_ptr,
            TRUE,
            added_string.text.text_buffer.wstr,
            added_string.text.text_buffer.wstr_len,
            FALSE
            );
        // 设置风格信息
        GUITITLE_SetSubTextParamByPtr(    
            (CTRLTITLE_OBJ_T*)tab_ptr->title_ptr,
            &added_string.text_style.rect,
            &added_string.text_style.font_type,
            &added_string.text_style.align
            );
    }
}

/*****************************************************************************/
//  Description : draw all tab items.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawAllTabItems(CTRLTAB_OBJ_T * tab_ptr)
{
	uint32              i             = 0;
    GUI_RECT_T          rect          = {0};
    uint32              show_item_num = 0;

    //if (tab_ptr->show_item_num == 0)
    //{
        // 这里要重新设置valid list
        ResetValidItemList(tab_ptr); // 更新tab项
        show_item_num = InitCurPageItemRect(tab_ptr);
    //}
    //else
    //{
    //    show_item_num = tab_ptr->show_item_num;
    //}

    // 删除无效的item数据
    DeleteInvalidItemData(tab_ptr);

    // 更新后，如果还是0，直接返回
    if (tab_ptr->valid_item_list.item_num == 0)
    {
        return;
    }
	
    if (tab_ptr->tab_style.is_show_cur_item_only)
    {
        DrawTabCurText(tab_ptr);		
    }
    else
    {
		//display title
		if (!GetStatePtr(tab_ptr, GUITAB_STATE_SINGLE_LINE) || MMITHEME_IsIstyle())
		{
			DrawTabTitleText(tab_ptr,tab_ptr->cur_sel_item_index);
		}

        for (i = 0; i<show_item_num; i++)
        {
            uint32        index     = (uint32)((tab_ptr->start_show_item_index + i) % tab_ptr->valid_item_list.item_num);
            GUITAB_ITEM_T *item_ptr = GetValidItemPtr(tab_ptr, index);
            
            if (item_ptr == PNULL)
            {
                break;
            }
            
            // 获取item区域
            rect = item_ptr->rect;
            
            if (item_ptr->item_data_ptr != PNULL && !GUI_IsRectEmpty( rect ))
            {
                //display item
                DrawTabItem(tab_ptr, index);
            }
        }
    }
}

//#ifdef MAINLCD_SIZE_128X64//syy modify 2010.11.29 -begin 黑白分类显示方式
/*****************************************************************************/
//  Description : draw current tab text.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawTabCurText(
                          CTRLTAB_OBJ_T * tab_ptr
                          )
{
    GUI_RECT_T      rect = {0};
    GUITAB_ITEM_T   *item_ptr = PNULL;
    GUISTR_STYLE_T  text_style = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_WORDBREAK|GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T  *)tab_ptr;

    if (tab_ptr->valid_item_list.item_num > 0)
    {
        wchar         *str_ptr   = PNULL;
        uint16        str_len    = 0;
        MMI_STRING_T  str_info   = {0};
        
        item_ptr = GetValidItemPtr(tab_ptr, tab_ptr->cur_sel_item_index);
        
        SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
        
        if( TABITEM_DATA_TEXT_ID == item_ptr->item_data_ptr->text_data.text_type)
        {
            MMI_STRING_T temp_str = {0};
            
            MMITHEME_GetResText(
                    item_ptr->item_data_ptr->text_data.text.text_id,
                    base_ctrl_ptr->win_handle, &temp_str );
            
            str_ptr = temp_str.wstr_ptr;
            str_len = temp_str.wstr_len;
        }
        else
        {
            str_ptr = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr;
            str_len = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr_len;
        }
        
        str_info.wstr_len = str_len;
        str_info.wstr_ptr = str_ptr;
        
        rect = base_ctrl_ptr->rect;
        rect.left  = (int16)(tab_ptr->left_scroll_card.card_rect.right + 1);
        rect.right = (int16)(tab_ptr->right_scroll_card.card_rect.left - 1); 
        
        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = tab_ptr->tab_style.font;
        text_style.angle = ANGLE_0;
        text_style.font_color = tab_ptr->tab_style.font_color;
        text_style.char_space = tab_ptr->tab_style.font_space;
        
        
        GUISTR_DrawTextToLCDInRect(&base_ctrl_ptr->lcd_dev_info, &rect,  
            &base_ctrl_ptr->rect, &str_info, &text_style, 
            text_state , GUISTR_TEXT_DIR_AUTO);
    }
}
//#endif//syy modify 2010.11.29 -end

/*****************************************************************************/
//  Description : display item description information.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawItemText(
                        CTRLTAB_OBJ_T * tab_ptr, 
                        uint32 index
                        )
{
	BOOLEAN			is_need_to_draw = FALSE;
	BOOLEAN			is_scroll = 0;
	uint16			str_index = 0;
    GUI_RECT_T		rect = {0};
    GUITAB_ITEM_T	*item_ptr = PNULL;
	GUISTR_STATE_T  str_state = GUISTR_STATE_WORDBREAK|GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T  *)tab_ptr;

    item_ptr = GetValidItemPtr(tab_ptr, index);
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return;
    }

	//get item text rect
	is_need_to_draw = GetItemTextRect(tab_ptr,item_ptr,&rect);

	if (is_need_to_draw && (tab_ptr->valid_item_list.item_num > 0))
	{
        MMI_STRING_T    str_info   = {0};
		GUI_COLOR_T		font_color = 0;
        GUISTR_STYLE_T  style_info = {0}; /*lint !e64*/
		MMI_IMAGE_ID_T	bg_img_id = 0;

		if (index == tab_ptr->cur_sel_item_index
			|| (tab_ptr->is_tp_down && index == tab_ptr->tp_down_item_index))
		{
			font_color = tab_ptr->tab_style.sel_font_color;
			bg_img_id  = item_ptr->item_data_ptr->text_data.pressed_image_id;
		}
		else
		{
			font_color = tab_ptr->tab_style.font_color;
			bg_img_id = item_ptr->item_data_ptr->text_data.released_image_id;
		}

		//draw item bg
		if ((item_ptr->item_data_ptr->text_data.is_disp_font_bg) && 
			(MMITHEME_CheckImageID(bg_img_id)))
		{
			DrawItemIcon(tab_ptr, rect, bg_img_id);
		}

		//get item text string
		GetItemTextStr(tab_ptr,item_ptr,&str_info);

        style_info.align	  = ALIGN_HVMIDDLE;
        style_info.font	      = tab_ptr->tab_style.font;
        style_info.angle	  = ANGLE_0;
        style_info.font_color = font_color;
        style_info.char_space = tab_ptr->tab_style.font_space;

		//get string is need scroll
		if (index == tab_ptr->cur_sel_item_index)
		{
			is_scroll = IsItemTextScroll(tab_ptr,str_info.wstr_ptr,str_info.wstr_len,rect);

			//set string index
			str_index = tab_ptr->str_index;
		}

		//set state
        if (is_scroll)
        {
            //start string scroll timer
			StartItemTextScrollTimer(tab_ptr);
        }
        else
        {
            str_state = str_state|GUISTR_STATE_ELLIPSIS;
        }

		//display string
        if ((is_scroll) &&
            (MMITHEME_GetIsScrollAsPixel()))
        {
            GUISTR_DrawTextToLCDByOffset((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T*)&rect,
                PNULL,
                str_index,
                0,
                &str_info,
                &style_info,
                str_state);
        }
        else
		{
			//set label string
            str_info.wstr_ptr = str_info.wstr_ptr + str_index;
            str_info.wstr_len = (uint16)(str_info.wstr_len - str_index);

            GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T*)&rect,
                (const GUI_RECT_T*)&rect,
                &str_info,
                &style_info,
                str_state,
                GUISTR_TEXT_DIR_AUTO);
		}
	}

    return;
}

/*****************************************************************************/
// 	Description : item string is need scroll
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsItemTextScroll(
							   CTRLTAB_OBJ_T	*tab_ptr,	//in:
							   wchar			*str_ptr,	//in:
							   uint16			str_len,	//in:
							   GUI_RECT_T		str_rect	//in:
							   )
{
    BOOLEAN     result = FALSE;
    uint16      str_width = 0;
    uint16      display_width = 0;

    //get string width
    str_width = GUI_CalculateStringPiexlNum(str_ptr,
                    str_len,
                    tab_ptr->tab_style.font,
                    tab_ptr->tab_style.font_space);

    //get display width
    display_width = (uint16)(str_rect.right - str_rect.left + 1);

    if (str_width > display_width)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : display item icon
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DrawItemIcon(
                        CTRLTAB_OBJ_T *tab_ptr, 
                        GUI_RECT_T disp_rect, 
                        MMI_IMAGE_ID_T image_id
                        )
{
    uint16          image_width = 0;
    uint16          image_height = 0;
    GUI_RECT_T      display_rect = {0};
    GUI_RECT_T      image_rect       = {0};
    GUI_RECT_T      clip_rect       = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T  *)tab_ptr;

    GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, base_ctrl_ptr->win_handle);

    display_rect = GUI_GetCenterRect(disp_rect, image_width, image_height);

    if (0 != image_width && 0 != image_height && GUI_IntersectRect(&clip_rect, tab_ptr->item_rect, display_rect))
    {
        image_rect = GUI_CreateRect(0, 0, (uint16)(image_width - 1), (uint16)(image_height - 1));

        if (clip_rect.left > display_rect.left)
        {
            image_rect.left += clip_rect.left- display_rect.left;
        }
        
        if (clip_rect.right < display_rect.right)
        {
            image_rect.right += clip_rect.right - display_rect.right;
        }
        
        IMG_EnableTransparentColor(TRUE); 

        GUIRES_DisplayImg(PNULL,
            &clip_rect,
            &image_rect,
            base_ctrl_ptr->win_handle,
            image_id,
            &base_ctrl_ptr->lcd_dev_info);
        
        IMG_EnableTransparentColor(FALSE); 
    }
}

/*****************************************************************************/
//  Description : draw select bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DrawSelectBg(
						CTRLTAB_OBJ_T	*tab_ptr,	//in:
						uint32			index		//in:
						)
{
    uint16          image_width = 0;
    uint16          image_height = 0;
	GUI_RECT_T		bg_rect = {0};
    GUI_RECT_T		clip_rect = {0};
	GUITAB_ITEM_T	*item_ptr = GetValidItemPtr(tab_ptr,index);
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T  *)tab_ptr;
	
	if ((PNULL != item_ptr)&&
        (tab_ptr->cur_sel_item_index == index) &&
		(!MMITHEME_CheckImageID(item_ptr->item_data_ptr->bg_image_id)) &&
		(!MMITHEME_CheckImageID(item_ptr->item_data_ptr->text_data.pressed_image_id)))
	{
		//set bg rect
		bg_rect = item_ptr->rect;

		if (CTRLTAB_GetImageState(&item_ptr->item_data_ptr->image_data, GUITAB_IMAGE_NEED_SEL_BG_IMG))
		{
            GUI_BG_DISPLAY_T    bg_display      = {0};
            GUI_BG_T            bg_info         = {0};
            
			if (!GetStatePtr(tab_ptr, GUITAB_STATE_SINGLE_LINE) || MMITHEME_IsIstyle())
            {
                bg_rect.left   += tab_ptr->tab_style.height_bg_margin.left;
                bg_rect.top	   += tab_ptr->tab_style.height_bg_margin.top;
                bg_rect.right  -= tab_ptr->tab_style.height_bg_margin.right;
                bg_rect.bottom -= tab_ptr->tab_style.height_bg_margin.bottom;
            }
            
            if (!GUI_IntersectRect(&clip_rect, tab_ptr->item_rect, bg_rect))
            {
                return;
            }

            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = bg_rect;
            bg_display.display_rect   = clip_rect;
            bg_display.ctrl_handle    = base_ctrl_ptr->handle;
            bg_display.win_handle     = base_ctrl_ptr->win_handle;

            // 调整图片显示区域
            if ( GUIRES_IsSabm( tab_ptr->tab_style.height_bg_info.img_id, base_ctrl_ptr->win_handle ) )
            {
                bg_display.img_rect = clip_rect;
                bg_display.img_rect.left -= bg_display.rect.left;
                bg_display.img_rect.right -= bg_display.rect.left;
                bg_display.img_rect.top -= bg_display.rect.top;
                bg_display.img_rect.bottom -= bg_display.rect.top;
            }
            else
            {
                if (GUIRES_GetImgWidthHeight(
                            &image_width, &image_height,
                            tab_ptr->tab_style.height_bg_info.img_id,
                            base_ctrl_ptr->win_handle))
                {
                    bg_display.img_rect.right = image_width - 1;
                    bg_display.img_rect.bottom = image_height - 1;
                }
            }

            // 清背景，否则对于argb888的图片会有重叠
            DrawBackGround(tab_ptr, clip_rect);

            bg_info.bg_type = GUI_BG_IMG;
            bg_info.img_id = tab_ptr->tab_style.height_bg_info.img_id;
            // 显示图片
            GUI_DisplayBg(&bg_info,
                &bg_display,
                &base_ctrl_ptr->lcd_dev_info);
            
// 			IMG_EnableTransparentColor(TRUE);
// 
// 			GUIRES_DisplayImg(PNULL,
// 				&bg_rect,
// 				PNULL,
// 				base_ctrl_ptr->win_handle,
// 				tab_ptr->tab_style.height_bg_info.img_id,
// 				&tab_ptr->lcd_dev_info);
//     
// 			IMG_EnableTransparentColor(FALSE);
		}
		else
		{
			if (!GetStatePtr(tab_ptr, GUITAB_STATE_SINGLE_LINE) || MMITHEME_IsIstyle())
            {
                bg_rect.left   += tab_ptr->tab_style.text_margin.left;
                bg_rect.top	   += tab_ptr->tab_style.text_margin.top;
                bg_rect.right  -= tab_ptr->tab_style.text_margin.right;
                bg_rect.bottom -= tab_ptr->tab_style.text_margin.bottom;
            }

			GUI_FillRect(&base_ctrl_ptr->lcd_dev_info,bg_rect,tab_ptr->tab_style.focus_rect_color);
		}
	}
}

/*****************************************************************************/
//  Description : draw tp down bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DrawTpDownBg(
						CTRLTAB_OBJ_T	*tab_ptr,	//in:
						uint32			index		//in:
						)
{
	GUI_RECT_T		bg_rect = {0};
	GUITAB_ITEM_T	*item_ptr = GetValidItemPtr(tab_ptr,index);
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

	if ((tab_ptr->is_tp_down) &&
		(tab_ptr->tp_down_item_index == index) &&
		(MMITHEME_CheckImageID(tab_ptr->tab_style.tp_sel_img_id))&&
		(PNULL != item_ptr))
	{
		//set bg rect
		bg_rect = item_ptr->rect;

		IMG_EnableTransparentColor(TRUE);

		GUIRES_DisplayImg(PNULL,
			&bg_rect,
			PNULL,
			base_ctrl_ptr->win_handle,
			tab_ptr->tab_style.tp_sel_img_id,
			&base_ctrl_ptr->lcd_dev_info);

		IMG_EnableTransparentColor(FALSE);
	}
}

/*****************************************************************************/
//  Description : display item description information.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawTabItem(
                       CTRLTAB_OBJ_T *tab_ptr, 
                       uint32 index
                       )
{
	//draw select bg
	DrawSelectBg(tab_ptr,index);

	//draw tp down bg
	DrawTpDownBg(tab_ptr,index);

	DrawDividingLine(tab_ptr,index);

	//draw item anim
	DrawItemAnim(tab_ptr,index);

	//draw item text
	DrawItemText(tab_ptr,index);
}

/*****************************************************************************/
//  Description : display item description information. while state not
//                   GUITAB_STATE_SINGLE_LINE
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawTabTitleText(
                            CTRLTAB_OBJ_T * tab_ptr, 
                            uint32 index
                            )
{
    GUI_RECT_T  rect = {0};
	uint16		font_height = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;
	CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

    // 显示在中间，need modified
    rect.left = base_ctrl_ptr->rect.left;
    rect.right = base_ctrl_ptr->rect.right;
    rect.top = (int16)(base_ctrl_ptr->rect.top + GUITAB_SHOW_TEXT_RELATIVE_TOP);

    //rect.bottom = tab_ptr->rect.bottom;
	//标题只显示单行
	font_height = GUI_GetFontHeight(tab_ptr->tab_style.font, UNICODE_HANZI);
	rect.bottom = (int16)MIN ( (base_ctrl_ptr->rect.bottom), (rect.top + font_height) );

	if (tab_ptr->valid_item_list.item_num > 0)
	{
        wchar           * str_ptr   = PNULL;
        uint16          str_len     = 0;
        MMI_STRING_T    str_info    = {0};
        
        if (!GetStatePtr(tab_ptr, GUITAB_STATE_COMMON_TITLE))
        {
            item_ptr = GetValidItemPtr(tab_ptr, index);
            
            SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
            
            if( TABITEM_DATA_TEXT_ID == item_ptr->item_data_ptr->text_data.text_type)
            {
                MMI_STRING_T temp_str = {0};
                
                MMITHEME_GetResText(
                        item_ptr->item_data_ptr->text_data.text.text_id,
                        base_ctrl_ptr->win_handle, &temp_str );
                
                str_ptr = temp_str.wstr_ptr;
                str_len = temp_str.wstr_len;
            }
            else
            {
                str_ptr = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr;
                str_len = item_ptr->item_data_ptr->text_data.text.text_buffer.wstr_len;
            }
        }
        else
        {
            str_len = tab_ptr->title_str_info.wstr_len;
            str_ptr = tab_ptr->title_str_info.wstr_ptr;
        }

        str_info.wstr_len = str_len;
        str_info.wstr_ptr = str_ptr;

        if (tab_ptr->is_reset_title)
        {
            if ((GetStatePtr(tab_ptr, GUITAB_STATE_COMMON_TITLE) && !tab_ptr->is_reset_common_title)
                || !GetStatePtr(tab_ptr, GUITAB_STATE_COMMON_TITLE))
            {
                if (PNULL != tab_ptr->title_ptr)
                {
                    tab_ptr->is_reset_common_title = TRUE;
                    GUITITLE_SetTextByPtr((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr, str_ptr, str_len, TRUE);
                }
            }
        }
        else
        {
            tab_ptr->is_reset_title = TRUE;
        }
	}
}

/*****************************************************************************/
//  Description : draw item dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DrawDividingLine(
							CTRLTAB_OBJ_T	*tab_ptr,
							uint32			index
							)
{
    GUI_POINT_T     dis_point = {0};
    GUITAB_ITEM_T   *item_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T  *)tab_ptr;

    item_ptr = GetValidItemPtr(tab_ptr,index);
    if (PNULL != item_ptr)
    {
        if (((GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_UIMG_DTEXT)) || (GetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_SHOW_TEXT)))&&
            (item_ptr->rect.right < base_ctrl_ptr->rect.right))
        {
            dis_point.x = item_ptr->rect.right;
            dis_point.y = item_ptr->rect.top;

            GUIRES_DisplayImg(
                &dis_point,
                PNULL,
                PNULL,
                base_ctrl_ptr->win_handle,
                tab_ptr->tab_style.dividing_line,
                &base_ctrl_ptr->lcd_dev_info);
        }
    }
     
}

/*****************************************************************************/
//  Description : draw item anim
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DrawItemAnim(
                           CTRLTAB_OBJ_T	*tab_ptr,
                           uint32			index
                           )
{
    BOOLEAN			result = FALSE;
	GUI_RECT_T		rect = {0};
    GUI_RECT_T		cut_rect = {0};
    GUITAB_ITEM_T	*item_ptr = GetValidItemPtr(tab_ptr, index);
	MMI_IMAGE_ID_T	image_id = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T  *)tab_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return FALSE;
    }

    if ((PNULL != item_ptr->item_data_ptr) &&
		(GetItemImageRect(tab_ptr,index,&rect)))
    {
		//draw item bg
		if (index == tab_ptr->cur_sel_item_index && MMITHEME_CheckImageID(item_ptr->item_data_ptr->bg_image_id))
		{
			DrawItemIcon(tab_ptr, rect, item_ptr->item_data_ptr->bg_image_id);
		}

        // 计算压缩区域
		cut_rect = ReadjustAnimRect(rect, item_ptr->item_data_ptr->image_data.comp_image_w, item_ptr->item_data_ptr->image_data.comp_image_h);

        switch(item_ptr->item_data_ptr->image_data.image_data_type)
        {
        case TABITEM_DATA_IMAGE_ID:
            // 这里首先显示焦点item的背景图
            if (((index == tab_ptr->cur_sel_item_index) || (index == tab_ptr->tp_down_item_index && tab_ptr->is_tp_down))
				&& (MMITHEME_CheckImageID(item_ptr->item_data_ptr->sel_image_id)))
            {
				image_id = item_ptr->item_data_ptr->sel_image_id;
            }
			else
			{
				image_id = item_ptr->item_data_ptr->image_data.image_data_u.anim_data.img_id;
			}
            DrawItemIcon(tab_ptr, cut_rect, image_id);
            break;

        case TABITEM_DATA_ANIM_ID:
        case TABITEM_DATA_ANIM_DATA:
        case TABITEM_DATA_ANIM_PATH:
            if(0 == item_ptr->add_data_ptr) // 如果item对应的动画控件没有创建,则需要新建
            {
                SetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_NEED_CONTENT, TRUE);
                
                if( !GetStatePtr(tab_ptr, GUITAB_STATE_SETTING_ITEM_DATA))
                {
                    if ( MMK_IsFocusWin(base_ctrl_ptr->win_handle) )
                    {
                        uint32 i = 0;
                        for (i = 0; i < tab_ptr->total_item_num; i++)
                        {
                            if (item_ptr == GetItemPtr(tab_ptr, i))
                            {
                                break;
                            }
                        }
                        // 记录正在解析的item的索引号
                        tab_ptr->cur_object_index = i;
                        // 发送需要图片数据的消息
                        SendTabNotifyMessage(
                                tab_ptr, i, tab_ptr->cur_object_index,
                                MSG_NOTIFY_TAB_NEED_ITEM_CONTENT, FALSE,
                                base_ctrl_ptr->handle);
                        // 正在加载数据
                        SetStatePtr( tab_ptr, GUITAB_STATE_SETTING_ITEM_DATA, TRUE );
                    }
                }
            }
            else // 如果item对应的动画控件已经创建,则根据是否显示了焦点状态图片而重新设置.只有当sel_image_id有效的时候才会重新设置
            {
                CTRLBASE_OBJ_T* anim_ctrl_ptr = item_ptr->add_data_ptr;
                
                SCI_ASSERT( PNULL != anim_ctrl_ptr ); /*assert verified*/

				if ((index == tab_ptr->cur_sel_item_index) &&
					(MMITHEME_CheckImageID(item_ptr->item_data_ptr->sel_image_id)))
				{
					image_id = item_ptr->item_data_ptr->sel_image_id;
				}
				else
				{
					image_id = tab_ptr->tab_style.item_deficon_id;
				}

                GUIANIM_SetDefaultIconByPtr(
                        item_ptr->add_data_ptr, image_id, tab_ptr->tab_style.item_error_id);
                
                GUIANIM_SetCtrlRectByPtr(anim_ctrl_ptr, &cut_rect );
               
                IGUICTRL_HandleEvent((IGUICTRL_T *)anim_ctrl_ptr, MSG_CTL_PAINT, PNULL );
                
                result = TRUE;
            }
            
            break;
            
        default:
            //SCI_TRACE_LOW:"DrawItemAnim type error! type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_3029_112_2_18_3_21_54_351,(uint8*)"d", item_ptr->item_data_ptr->image_data.image_data_type);
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : draw child window.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void DrawChildWindow(
                           CTRLTAB_OBJ_T * tab_ptr
                           )
{
    uint32		        index = tab_ptr->cur_sel_item_index;
    MMI_HANDLE_T		win_handle = 0;
    GUITAB_ITEM_T       *item_ptr = PNULL;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)tab_ptr;

    item_ptr = GetValidItemPtr(tab_ptr, index);
    
    if(PNULL != item_ptr)
    {
        win_handle = item_ptr->child_win_handle;
    }
    
    if (tab_ptr->valid_item_list.item_num > 0)
    {
        if (win_handle > 0 && MMK_IsOpenWin(win_handle))
        {
            // 切换窗口焦点
            MMK_WinGrabFocus( win_handle );

            // 通知父亲切换item
            SendTabNotifyMessage(
                    tab_ptr, tab_ptr->cur_sel_item_index, tab_ptr->pre_sel_item_index,
                    MSG_NOTIFY_TAB_SWITCH, FALSE,
                    base_ctrl_ptr->handle);

            // 通知儿子切换item
            SendTabNotifyMessage(
                    tab_ptr, tab_ptr->cur_sel_item_index, tab_ptr->pre_sel_item_index,
                    MSG_NOTIFY_TAB_SWITCH, TRUE,
                    win_handle);
        }
        else if (MMK_IsOpenWin(base_ctrl_ptr->win_handle))
        {
            //@xin.li for mbbms2.0
            //无字窗口时，需要给父窗口发MSG_NOTIFY_TAB_SWITCH消息 
            GUICTRL_SendNotify( base_ctrl_ptr->handle, MSG_NOTIFY_TAB_SWITCH );
            MMK_SendMsg(base_ctrl_ptr->handle, MSG_CTL_PAINT, 0);
        }
        else
        {
            //SCI_TRACE_LOW:"GUITAB: DrawChildWindow no focus window!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_3075_112_2_18_3_21_55_352,(uint8*)"");
        }
    }
}

/*****************************************************************************/
//  Description : stop the tab control timer.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopTabTimer(
                           CTRLTAB_OBJ_T *tab_ptr
                           )
{
	if (tab_ptr->timer_id > 0)
	{
		MMK_StopTimer(tab_ptr->timer_id);
		tab_ptr->timer_id = 0;
		tab_ptr->timer_status = GUITAB_TIMER_NULL;
	}
    
    return TRUE;
}

/*****************************************************************************/
//  Description : stop the tab control timer.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint8 StartTabTimer(
                          CTRLTAB_OBJ_T *tab_ptr,
                          uint32 timer_out
                          )
{
	CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

    StopTabTimer(tab_ptr);
    
    return (MMK_CreateWinTimer(base_ctrl_ptr->handle, timer_out, TRUE));
}

/*****************************************************************************/
//  Description : handle MSG_TIMER message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimer(
                               CTRLTAB_OBJ_T * tab_ptr
                               )
{
    uint32          i               = 0;
    uint32          move_num        = 0;
    BOOLEAN         is_precessed    = FALSE;
    MMI_RESULT_E	result          = MMI_RESULT_TRUE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    if (tab_ptr->valid_item_list.item_num == 0)
    {
        return MMI_RESULT_FALSE;
    }
    
    if (tab_ptr->timer_status == GUITAB_TIMER_LEFT_SCROLL) // left scroll tab items
    {
        if (tab_ptr->cur_sel_item_index > 0)
        {
            MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_PRE);
            UpdateCurPageItemRect(tab_ptr);
            SetScrollCardState(tab_ptr, GUITAB_SWITCH_PRE);
            DrawChildWindow(tab_ptr);
            is_precessed = TRUE;
        }
    }
    else if (tab_ptr->timer_status == GUITAB_TIMER_RIGHT_SCROLL) // right scroll tab items
    {
        if (tab_ptr->cur_sel_item_index < (tab_ptr->valid_item_list.item_num - 1))
        {
            uint32 cur_item_index = tab_ptr->cur_sel_item_index;
            
            move_num = GetMoveItemNum(tab_ptr, GUITAB_SWITCH_NEXT);
            
            for (i = 0; i < move_num; i++)
            {
                MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_NEXT);
            }
            // 这里之所以重新计算当前的item,是因为上一步如果移动超过1的时候,当前item的索引号就将出错
            tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
            tab_ptr->cur_sel_item_index = cur_item_index + 1;
            tab_ptr->cur_sel_item_index %= tab_ptr->valid_item_list.item_num;
            
            SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, FALSE);

            UpdateCurPageItemRect(tab_ptr);
            SetScrollCardState(tab_ptr, GUITAB_SWITCH_NEXT);
            DrawChildWindow(tab_ptr);
            is_precessed = TRUE;
        }
    }

    if (!is_precessed)
	{
        StopTabTimer(tab_ptr);
	}
    
	return(result);
}

/*****************************************************************************/
// 	Description : start item text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartItemTextScrollTimer(
									CTRLTAB_OBJ_T	*tab_ptr
									)
{
	CTRLBASE_OBJ_T * base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;
	
	if (PNULL == tab_ptr)
	{
        return;
	}

    if ((MMK_IsFocusWin(base_ctrl_ptr->win_handle)) && 
        (0 == tab_ptr->text_timer_id))
    {
        tab_ptr->text_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
									tab_ptr->timeout,
									FALSE);
    }
}

/*****************************************************************************/
// 	Description : stop item text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StopItemTextScrollTimer(
								   CTRLTAB_OBJ_T	*tab_ptr
								   )
{
    if (0 < tab_ptr->text_timer_id)
    {
        MMK_StopTimer(tab_ptr->text_timer_id);
        tab_ptr->text_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle item text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleItemTextScrollTimer(
									 CTRLTAB_OBJ_T	*tab_ptr
									 )
{
    uint16			str_width = 0;
    uint16			display_width = 0;
    GUI_RECT_T		str_rect = {0};
	MMI_STRING_T	text_str = {0};
	GUITAB_ITEM_T	*item_ptr = PNULL;

    //stop timer
    StopItemTextScrollTimer(tab_ptr);

	//get current item
	item_ptr = GetValidItemPtr(tab_ptr,tab_ptr->cur_sel_item_index);

    //get item string rect
	GetItemTextRect(tab_ptr,item_ptr,&str_rect);

    //get display width
    display_width = (uint16)(str_rect.right - str_rect.left + 1);

	//get item text string
	GetItemTextStr(tab_ptr,item_ptr,&text_str);

    //set string index
    if (MMITHEME_GetIsScrollAsPixel())
    {
        //get item text len
        str_width = GUI_CalculateStringPiexlNum(text_str.wstr_ptr,
                            text_str.wstr_len,
                            tab_ptr->tab_style.font,
                            tab_ptr->tab_style.font_space);

        if ((str_width - 1 - tab_ptr->str_index) > display_width)
        {
            tab_ptr->str_index = (uint16)(tab_ptr->str_index + MMITHEME_ScrollPixelNum());
        }
        else
        {
            tab_ptr->str_index = 0;
        }
    }
    else
    {
        //get label string len
        str_width = GUI_CalculateStringPiexlNum((text_str.wstr_ptr+tab_ptr->str_index),
                            (uint16)(text_str.wstr_len - tab_ptr->str_index),
                            tab_ptr->tab_style.font,
                            tab_ptr->tab_style.font_space);

        //string has display
        if (str_width > display_width)
        {
            tab_ptr->str_index++;
        }
        else
        {
            tab_ptr->str_index = 0;
        }
    }

    //display item
	DrawTabItem(tab_ptr,tab_ptr->cur_sel_item_index);
}

/*****************************************************************************/
//  Description : handle tab tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabTpDown(
								   CTRLTAB_OBJ_T	*tab_ptr,
								   DPARAM			param
								   )
{
	uint32			i = 0;
	uint32			move_num = 0;
	GUI_POINT_T     tp_point = {0};
	MMI_RESULT_E    result = MMI_RESULT_FALSE;

	//stop tab timer
	StopTabTimer(tab_ptr);

	if (0 < tab_ptr->valid_item_list.item_num)
	{
		//set tp press point
		tp_point.x = MMK_GET_TP_X(param);
		tp_point.y = MMK_GET_TP_Y(param);

		//set tp point
		tab_ptr->tp_prev_point = tp_point;

		//is tp item
		if (GUI_PointIsInRect(tp_point,tab_ptr->item_rect))
		{
			HandleItemTpDown(tab_ptr,&tp_point);
		}
		else if ((GUI_PointIsInRect(tp_point,tab_ptr->left_scroll_card.card_rect)) &&
				 (GUITAB_SCROLL_CARD_DISABLED != tab_ptr->left_scroll_card.card_state))
		{
			if (tab_ptr->is_scroll && !GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL)
			 &&(tab_ptr->is_rotative || (tab_ptr->cur_sel_item_index > 0)))
			{
				//stop item text timer
				StopItemTextScrollTimer(tab_ptr);
				tab_ptr->str_index = 0;

				MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_PRE);
				UpdateCurPageItemRect(tab_ptr);
				SetScrollCardState(tab_ptr, GUITAB_SWITCH_PRE);
				DrawChildWindow(tab_ptr);
				tab_ptr->timer_status = GUITAB_TIMER_LEFT_SCROLL;
				tab_ptr->timer_id = StartTabTimer(tab_ptr, tab_ptr->timeout);
			}
		}
		else if ((GUI_PointIsInRect(tp_point,tab_ptr->right_scroll_card.card_rect)) &&
				 (GUITAB_SCROLL_CARD_DISABLED != tab_ptr->right_scroll_card.card_state))
		{
			if (tab_ptr->is_scroll && !GetStatePtr(tab_ptr, GUITAB_STATE_NONEED_SCROLL)
			 && (tab_ptr->is_rotative || (tab_ptr->cur_sel_item_index < (tab_ptr->valid_item_list.item_num - 1))))
			{
				uint32 cur_item_index = tab_ptr->cur_sel_item_index;

				//stop item text timer
				StopItemTextScrollTimer(tab_ptr);
				tab_ptr->str_index = 0;

				move_num = GetMoveItemNum(tab_ptr, GUITAB_SWITCH_NEXT);
				for (i = 0; i < move_num; i++)
				{
					MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_NEXT);
				}

				tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
				tab_ptr->cur_sel_item_index = cur_item_index + 1;
				tab_ptr->cur_sel_item_index %= tab_ptr->valid_item_list.item_num;
            
				UpdateCurPageItemRect(tab_ptr);
				SetScrollCardState(tab_ptr, GUITAB_SWITCH_NEXT);
				DrawChildWindow(tab_ptr);
				tab_ptr->timer_status = GUITAB_TIMER_RIGHT_SCROLL;
				tab_ptr->timer_id = StartTabTimer(tab_ptr, tab_ptr->timeout);
			}
		}
		else
		{
			//SCI_TRACE_LOW:"HandleTabTpDown:tp point not in tab valid rect!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_3809_112_2_18_3_21_56_356,(uint8*)"");
		}
	}

	return (result);
}

/*****************************************************************************/
//  Description : handle tab item tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleItemTpDown(
									CTRLTAB_OBJ_T	*tab_ptr,
									GUI_POINT_T		*point_ptr
									)
{
	uint32			i = 0;
	uint32			show_item_num = 0;
	uint32			item_index = 0;
	MMI_RESULT_E    result = MMI_RESULT_FALSE;
	GUITAB_ITEM_T   *item_ptr = PNULL;

	//get show item number
	show_item_num = GetShowItemNum(tab_ptr);
	for (i=0; i<show_item_num; i++)
	{
		//get item pointer
		item_index = (tab_ptr->start_show_item_index + i)%tab_ptr->valid_item_list.item_num;
		item_ptr = GetValidItemPtr(tab_ptr,item_index);

		if (PNULL != item_ptr && GUI_PointIsInRect(*point_ptr,item_ptr->rect))
		{
			//stop item text timer
			StopItemTextScrollTimer(tab_ptr);
			
			//set tp down index
			tab_ptr->tp_down_item_index = item_index;
			result = MMI_RESULT_TRUE;
			break;
		}
	}

	if (MMI_RESULT_TRUE == result)
	{
		//draw item
		tab_ptr->is_tp_down = TRUE;
		DrawTabItem(tab_ptr,item_index);
		tab_ptr->is_tp_down = FALSE;

		//stop item text timer
		StopItemTextScrollTimer(tab_ptr);
		tab_ptr->str_index = 0;
	}

	return (result);
}

/*****************************************************************************/
//  Description : handle tab tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabTpMove(
								   CTRLTAB_OBJ_T	*tab_ptr,
								   DPARAM			param
								   )
{
	int16			move_x = 0;
	GUI_POINT_T     tp_point = {0};
	MMI_RESULT_E    result = MMI_RESULT_FALSE;

	StopTabTimer(tab_ptr);
    ResetScrollState(tab_ptr);

	if (0 < tab_ptr->valid_item_list.item_num)
	{
		//set tp press point
		tp_point.x = MMK_GET_TP_X(param);
		tp_point.y = MMK_GET_TP_Y(param);

		//set tp down item invalid index
		tab_ptr->tp_down_item_index = tab_ptr->valid_item_list.item_num;

		//set move x
        move_x = (int16)(tp_point.x - tab_ptr->tp_prev_point.x);

		if (TpMoveItem(tab_ptr,move_x))
		{
			//draw control
			DrawCtrl(tab_ptr);

			result = MMI_RESULT_TRUE;
		}
	}

	return (result);
}

/*****************************************************************************/
//  Description : handle tab tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTabTpUp(
								 CTRLTAB_OBJ_T	*tab_ptr,
								 DPARAM			param
								 )
{
	uint32			i = 0;
	uint32			show_item_num = 0;
	uint32			item_index = 0;
	GUI_POINT_T     tp_point = {0};
	MMI_RESULT_E    result = MMI_RESULT_FALSE;
	GUITAB_ITEM_T   *item_ptr = PNULL;

	StopTabTimer(tab_ptr);
    ResetScrollState(tab_ptr);

	if (0 < tab_ptr->valid_item_list.item_num)
	{
		//set tp press point
		tp_point.x = MMK_GET_TP_X(param);
		tp_point.y = MMK_GET_TP_Y(param);

		//is tp item
		if (GUI_PointIsInRect(tp_point,tab_ptr->item_rect))
		{
			//get show item number
			show_item_num = GetShowItemNum(tab_ptr);
			for (i=0; i<show_item_num; i++)
			{
				//get item pointer
				item_index = (tab_ptr->start_show_item_index + i)%tab_ptr->valid_item_list.item_num;
				item_ptr = GetValidItemPtr(tab_ptr,item_index);

				if (PNULL != item_ptr && GUI_PointIsInRect(tp_point,item_ptr->rect))
				{
					result = MMI_RESULT_TRUE;
					break;
				}
			}
		}
	}

	if ((MMI_RESULT_TRUE == result) &&
		(item_index == tab_ptr->tp_down_item_index) &&
		(item_index != tab_ptr->cur_sel_item_index))
	{
		//stop item text timer
		tab_ptr->str_index = 0;

		tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
        tab_ptr->cur_sel_item_index = item_index;

        SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, FALSE);

        UpdateCurPageItemRect(tab_ptr);
        ResetScrollState(tab_ptr);
        DrawChildWindow(tab_ptr);
	}
	else
	{
		//draw control
		DrawCtrl(tab_ptr);
	}

	return (result);
}

/*****************************************************************************/
//  Description : tp move item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TpMoveItem(
						 CTRLTAB_OBJ_T	*tab_ptr,
						 int16			move_x
						 )
{
	BOOLEAN		result = FALSE;
	BOOLEAN		is_find = FALSE;
	int32		offset_x = 0;
	int32		min_offset = 0;
	uint32		i = 0;
	uint32		total_width = 0;
	uint32		start_item_index = 0;
	uint32		cur_page_item_num = 0;
	GUI_RECT_T	rect = {0};
	GUI_RECT_T	cross_rect = {0};

	total_width = tab_ptr->item_rect.right - tab_ptr->item_rect.left + 1;

	//get min offset
	min_offset = total_width - tab_ptr->valid_item_list.total_width;
	if (0 < min_offset)
	{
		min_offset = 0;
	}

	//get offset
	offset_x = tab_ptr->offset_x;
	if (0 != move_x)
	{
		offset_x = offset_x + move_x;
		if (0 < offset_x)
		{
			offset_x = 0;
		}
		else if (offset_x < min_offset)
		{
			offset_x = min_offset;
		}
	}

	if (offset_x != tab_ptr->offset_x)
	{
		//set offset
		tab_ptr->offset_x = offset_x;

		//calculate all item rect
		rect = tab_ptr->item_rect;
		rect.left  = (int16)(tab_ptr->item_rect.left + tab_ptr->offset_x);
		rect.right = rect.left;

		for (i = 0; i<tab_ptr->valid_item_list.item_num; i++)
		{
			CalculateItemRect(tab_ptr,rect,&rect,i);

			if ((!is_find) && 
				(rect.right > tab_ptr->item_rect.left))
			{
				start_item_index = i;
				is_find = TRUE;
			}

			if (GUI_IntersectRect(&cross_rect,rect,tab_ptr->item_rect))
			{
				cur_page_item_num++;
			}
		}

		tab_ptr->start_show_item_index = start_item_index;
		tab_ptr->show_item_num = cur_page_item_num;

		result = TRUE;
	}

	return (result);
}

/*****************************************************************************/
//  Description : handle MSG_KEYDOWN_LEFT message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLeftKeyDown(
                                     CTRLTAB_OBJ_T * tab_ptr
                                     )
{
	MMI_RESULT_E	result = MMI_RESULT_TRUE;

    StopTabTimer(tab_ptr);

	if (!GetStatePtr(tab_ptr, GUITAB_STATE_SWITCH_ITEM))
	{
		return MMI_RESULT_FALSE; // 
	}

	if (tab_ptr->is_rotative || (tab_ptr->cur_sel_item_index > 0))
	{
		//stop item text timer
		StopItemTextScrollTimer(tab_ptr);
		tab_ptr->str_index = 0;

        MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_PRE);
        UpdateCurPageItemRect(tab_ptr);
        SetScrollCardState(tab_ptr, GUITAB_SWITCH_PRE);
		DrawChildWindow(tab_ptr);
	}
    else
    {
        result = MMI_RESULT_FALSE;
    }
	
	return(result);
}

/*****************************************************************************/
//  Description : handle MSG_KEYDOWN_RIGHT message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRightKeyDown(
                                      CTRLTAB_OBJ_T * tab_ptr
                                      )
{
    MMI_RESULT_E	result = MMI_RESULT_TRUE;
    uint32          i = 0;
    uint32          move_num = 0;
    uint32          cur_item_index = 0;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
	if (!GetStatePtr(tab_ptr, GUITAB_STATE_SWITCH_ITEM))
	{
		return MMI_RESULT_FALSE; // 
	}

    if (tab_ptr->valid_item_list.item_num == 0)
    {
        return MMI_RESULT_FALSE; // 该返回值没有实际意义
    }

    cur_item_index = tab_ptr->cur_sel_item_index;
    
    StopTabTimer(tab_ptr);
    
    if (tab_ptr->is_rotative || (tab_ptr->cur_sel_item_index < (tab_ptr->valid_item_list.item_num - 1)))
    {
		//stop item text timer
		StopItemTextScrollTimer(tab_ptr);
		tab_ptr->str_index = 0;

        move_num = GetMoveItemNum(tab_ptr, GUITAB_SWITCH_NEXT);
        
        tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
        
        for (i = 0; i < move_num; i++)
        {
            MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_NEXT);
        }
        tab_ptr->cur_sel_item_index = cur_item_index + 1;
        tab_ptr->cur_sel_item_index %= tab_ptr->valid_item_list.item_num;
        
        SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, FALSE);
        UpdateCurPageItemRect(tab_ptr);
        SetScrollCardState(tab_ptr, GUITAB_SWITCH_NEXT);
        DrawChildWindow(tab_ptr);
    }		
    else
    {
        result = MMI_RESULT_FALSE;
    }
    
    return(result);
}

/*****************************************************************************/
//  Description : handle MSG_KEYUP_LEFT and MSG_KEYUP_RIGHT message.
//  Global resource dependence : 
//  Author:  hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleKeyUp(
                               CTRLTAB_OBJ_T *tab_ptr  // [in] tab handle
                               )
{
    StopTabTimer(tab_ptr);

	if (!GetStatePtr(tab_ptr, GUITAB_STATE_SWITCH_ITEM))
	{
		return MMI_RESULT_FALSE; // 
	}

    ResetScrollState(tab_ptr);

    if (0 == tab_ptr->valid_item_list.item_num)
    {
        return MMI_RESULT_FALSE;
    }

	return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : move item
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void MoveToNextValidItem(
                               CTRLTAB_OBJ_T *tab_ptr,
                               GUITAB_SWITCH_TYPE_E move_type
                               )
{
    uint32		        index = 0;
	MMI_HANDLE_T		win_handle = 0;
    GUITAB_ITEM_T       *item_ptr = PNULL;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }
	index = tab_ptr->cur_sel_item_index;
	base_ctrl_ptr = (CTRLBASE_OBJ_T*)tab_ptr;

    item_ptr = GetValidItemPtr(tab_ptr, index);

    //SCI_TRACE_LOW:"MoveToNextValidItem index = %d, total num = %d, valid num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_3692_112_2_18_3_21_56_354,(uint8*)"ddd", index, tab_ptr->total_item_num, tab_ptr->valid_item_list.item_num);

    if (PNULL == item_ptr) // 未搜索到,可能用户传的数据不对,不能直接Assert,但需要返回
    {
        return;
    }

	win_handle = item_ptr->child_win_handle;
    
    tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;

    switch(move_type) 
    {
    case GUITAB_SWITCH_PRE:
        ToSelPreItem(tab_ptr);

        if (tab_ptr->cur_sel_item_index == 0)
        {
            GUICTRL_SendNotify( base_ctrl_ptr->handle, MSG_NOTIFY_TAB_START );
            MMK_SendMsg(win_handle, MSG_NOTIFY_TAB_START, NULL);
        }
        break;

    case GUITAB_SWITCH_NEXT:
        ToSelNextItem(tab_ptr);

        if (tab_ptr->cur_sel_item_index == tab_ptr->valid_item_list.item_num - 1)
        {
            GUICTRL_SendNotify( base_ctrl_ptr->handle, MSG_NOTIFY_TAB_END );
            MMK_SendMsg(win_handle, MSG_NOTIFY_TAB_END, NULL);
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"MoveToNextValidItem error type! %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_3724_112_2_18_3_21_56_355,(uint8*)"d", move_type);
        break;
    }
}

/*****************************************************************************/
//  Description : move to pre item
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ToSelPreItem(
                        CTRLTAB_OBJ_T * tab_ptr
                        )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    if (tab_ptr->valid_item_list.item_num > 0)
    {
        uint32 cur_item_index = tab_ptr->cur_sel_item_index;
        uint32 page_item_num = GetShowItemNum(tab_ptr);// tab_ptr->show_item_num;
        
        tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
        if (tab_ptr->cur_sel_item_index == 0)
        {
            tab_ptr->cur_sel_item_index = tab_ptr->valid_item_list.item_num - 1;
        }
        else
        {
            tab_ptr->cur_sel_item_index--;
        }

        if (cur_item_index == tab_ptr->start_show_item_index
            && tab_ptr->valid_item_list.item_num > page_item_num)
        {
            if (cur_item_index == 0)
            {
                tab_ptr->start_show_item_index = tab_ptr->valid_item_list.item_num - page_item_num;

                // 需要重新加载
                DestroyAllITabtemData(tab_ptr, TRUE);
            }
            else
            {
                if (DestroyTabItemData(
                        tab_ptr,
                        GetValidItemPtr(tab_ptr, tab_ptr->start_show_item_index + page_item_num - 1),
                        TRUE))
                {
                    DestroyTabItemDecode(tab_ptr);// 终止解码
                }

                tab_ptr->start_show_item_index--;
            }
        }
    }
}

/*****************************************************************************/
//  Description : move to next item
//  Global resource dependence : 
//  Author:  Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void ToSelNextItem(
                         CTRLTAB_OBJ_T * tab_ptr
                         )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    if (tab_ptr->valid_item_list.item_num > 0)
    {
        uint32 cur_item_index = tab_ptr->cur_sel_item_index;
        uint32 page_item_num = GetShowItemNum(tab_ptr);
        
        tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
        tab_ptr->cur_sel_item_index++;
        tab_ptr->cur_sel_item_index %= tab_ptr->valid_item_list.item_num;

        if (cur_item_index == tab_ptr->start_show_item_index + page_item_num - 1
            && tab_ptr->valid_item_list.item_num > page_item_num)
        {
            if (cur_item_index == tab_ptr->valid_item_list.item_num - 1)
            {
                DestroyAllITabtemData(tab_ptr, TRUE);
            }
            else
            {
                if (DestroyTabItemData(
                        tab_ptr,
                        GetItemPtr(tab_ptr, tab_ptr->start_show_item_index),
                        TRUE))
                {
                    DestroyTabItemDecode(tab_ptr);
                }
            }

            tab_ptr->start_show_item_index++;
            tab_ptr->start_show_item_index %= tab_ptr->valid_item_list.item_num;
        }
    }
}

/*****************************************************************************/
//  Description : handle MSG_KEYDOWN_RIGHT message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetMoveItemNum(
                            CTRLTAB_OBJ_T *tab_ptr, 
                            GUITAB_SWITCH_TYPE_E type
                            )
{
    uint32 item_width = 0;
    uint32 total_width = 0;
    uint32 item_index = 0;
    uint32 move_num = 0;
    GUITAB_ITEM_T *item_ptr = PNULL;
    
    total_width = tab_ptr->item_rect.right - tab_ptr->item_rect.left + 1;
    
    switch(type)
    {
    case GUITAB_SWITCH_PRE:
        move_num++;
        break;
        
    case GUITAB_SWITCH_NEXT:
        if (tab_ptr->cur_sel_item_index + 1 == tab_ptr->valid_item_list.item_num)
        {
            move_num++;
        }
        else if (tab_ptr->cur_sel_item_index == (tab_ptr->start_show_item_index + tab_ptr->show_item_num - 1))
        {
            item_index = tab_ptr->cur_sel_item_index + 1;
            while (item_width <= total_width)
            {
                item_ptr = GetValidItemPtr(tab_ptr, item_index);

                if ( PNULL == item_ptr )
                {
                    break;
                }

                item_width += item_ptr->item_width;
                if (item_width > total_width)
                {
                    item_index++;
                    break;
                }
                item_index--;
            }
            move_num = item_index - tab_ptr->start_show_item_index;
        }
        else
        {
            move_num++;
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"GetMoveItemNum error! type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_4111_112_2_18_3_21_57_357,(uint8*)"d", type);
        SCI_ASSERT(0); /*assert verified*/
        break;
    }

    return move_num;
}

/*****************************************************************************/
//  Description : set the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void SetState( 
                    uint32* dst_state_ptr,
                    uint32  src_state,
                    BOOLEAN is_true
                    )
{
    uint32 state = 0;
    
    if ( PNULL != dst_state_ptr )
    {
        state = *dst_state_ptr;
        
        if( is_true )
        {
            state |= src_state;
        }
        else
        {
            state &= ~src_state;
        }
        
        *dst_state_ptr = state;
    }
}

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState(
                       uint32 dst_state,
                       uint32 src_state
                       )
{
    return (BOOLEAN)( ( dst_state & src_state ) == src_state );
}

/*****************************************************************************/
//  Description : set the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL void SetStatePtr( 
                       CTRLTAB_OBJ_T* tab_ptr,
                       GUITAB_STATE_T src_state,
                       BOOLEAN is_true
                       )
{
    if (PNULL == tab_ptr || !TabTypeOf( (CTRLBASE_OBJ_T*)tab_ptr ))
    {
        return;
    }

    SetState( &tab_ptr->tab_state, src_state, is_true );

    // 如果设置为单行，那么应该释放title内置控件
    if (GetState(src_state, GUITAB_STATE_SINGLE_LINE)
        && is_true)
    {
        if (PNULL != tab_ptr->title_ptr)
        {
            MMK_DestroyControl( GUICTRL_GetCtrlHandle( tab_ptr->title_ptr) );
            tab_ptr->title_ptr = PNULL;
        }
    }
    else if (GetState(src_state, GUITAB_STATE_SINGLE_LINE) // 如果设置为双行，那么应该创建title控件
        && !is_true)
    {
        CreateTabTitleCtrl(tab_ptr);
    }
}

/*****************************************************************************/
//  Description : get the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStatePtr(
                          const CTRLTAB_OBJ_T* tab_ptr,
                          GUITAB_STATE_T    src_state
                          )
{
    if (PNULL == tab_ptr || !TabTypeOf( (CTRLBASE_OBJ_T*)tab_ptr ))
    {
        return FALSE;
    }

    return GetState(tab_ptr->tab_state, src_state);
}

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetItemStatePtr( 
                           GUITAB_ITEM_T* item_ptr,
                           GUITAB_ITEM_STATE_T  src_state,
                           BOOLEAN is_true
                           )
{
    if (PNULL != item_ptr)
    {
        SetState( &item_ptr->item_state, src_state, is_true );
    }
}

/*****************************************************************************/
//  Description : get the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemStatePtr(
                              const GUITAB_ITEM_T* item_ptr,
                              GUITAB_ITEM_STATE_T src_state
                              )
{
    if (PNULL != item_ptr)
    {
        return GetState( item_ptr->item_state, src_state );
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Set the max item of list
//  Global resource dependence : 
//  Author:Lin.Lin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetMaxItemByPtr(
                               CTRLTAB_OBJ_T *tab_ptr,
                               uint32        max_item_num
                               )
{
	BOOLEAN	result = FALSE;

    if (PNULL != tab_ptr && TabTypeOf( (CTRLBASE_OBJ_T*)tab_ptr ))
    {
        DestroyAllITabtemData(tab_ptr, FALSE);
        
        tab_ptr->max_item_num = max_item_num;
        
        if ( AllocAllItemsSpace( tab_ptr ) )
        {
            ResetTabItem(tab_ptr); 
            result = TRUE;
        }
    }

	return(result);  
}

/*****************************************************************************/
//  Description : Add the item to tab list
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertItemByPtr(
                               CTRLTAB_OBJ_T*       tab_ptr,
                               const GUITAB_ITEM_T* item_ptr,
                               uint32               pos
                               )
{
    BOOLEAN result = FALSE;
    
    if (PNULL == tab_ptr || !TabTypeOf( (CTRLBASE_OBJ_T*)tab_ptr ))
    {
        //SCI_TRACE_LOW:"InsertItemByPtr error ctrl type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_5070_112_2_18_3_21_58_361,(uint8*)"");

        return result;
    }

    if (InsertTabItem( tab_ptr, item_ptr, pos ))
    {
        SetStatePtr( tab_ptr, GUITAB_STATE_INVALID, TRUE );
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : insert a new tab item to the given position.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InsertTabItem(
                            CTRLTAB_OBJ_T      *tab_ptr, 
                            const GUITAB_ITEM_T* add_item_ptr, // 指定的Item
                            uint32             pos           // 指定的位置, from 0 to total_item_num
                            )
{
    BOOLEAN             result        = FALSE;
    GUITAB_ITEM_T* item_ptr      = PNULL;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != add_item_ptr); /*assert verified*/
    if ((PNULL == tab_ptr)||(PNULL == add_item_ptr))
    {
        return FALSE;
    }


    if ( tab_ptr->total_item_num < tab_ptr->max_item_num )
    {
        pos = (uint32)MIN( pos, tab_ptr->total_item_num );
        
        /* memmove */
        if( tab_ptr->total_item_num - pos > 0 )
        {
            memmove( tab_ptr->item_ptr + pos + 1,\
                tab_ptr->item_ptr + pos,\
                ( tab_ptr->total_item_num - pos ) * sizeof(GUITAB_ITEM_T) );
        }
        item_ptr = tab_ptr->item_ptr + pos;
        SCI_MEMCPY( item_ptr, add_item_ptr, sizeof(GUITAB_ITEM_T));
        item_ptr->item_data_ptr = PNULL;
        
        if (PNULL != add_item_ptr->item_data_ptr)
        {
            CreateTabItemData( tab_ptr, item_ptr, add_item_ptr->item_data_ptr );
        }
        
        if (IsItemValid(item_ptr))
        {
            if(InsertValidItem(tab_ptr, item_ptr, tab_ptr->valid_item_list.item_num))
            {
                tab_ptr->valid_item_list.item_num++;

                if (pos == tab_ptr->cur_sel_item_index)
                {
                    ResetTabTitleText(tab_ptr, pos, FALSE);
                }
            }
        }
        tab_ptr->total_item_num ++;
        
        result = TRUE;
    }

	return(TRUE);
}

/*****************************************************************************/
//  Description : append a new tab item to tail.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendSimpleItem(
                             CTRLTAB_OBJ_T *	tab_ptr, 
                             MMI_STRING_T *		text_ptr,
                             MMI_IMAGE_ID_T		sel_image_id,
                             MMI_IMAGE_ID_T		unsel_image_id
                             )
{
	uint32		pos = tab_ptr->total_item_num;

    if( PNULL == tab_ptr->item_ptr[pos].item_data_ptr )
    {
        tab_ptr->item_ptr[pos].item_data_ptr = (GUITAB_ITEM_CONTENT_T*)SCI_ALLOC_APP(sizeof(GUITAB_ITEM_CONTENT_T));
        SCI_MEMSET(tab_ptr->item_ptr[pos].item_data_ptr, 0, sizeof(GUITAB_ITEM_CONTENT_T));
    }

	// if oversize, truncate it
	if (text_ptr->wstr_len > GUITAB_MAX_ITEM_NAME_LENGTH)
	{
		tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr_len = GUITAB_MAX_ITEM_NAME_LENGTH;
	}
	else
	{
		tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr_len = text_ptr->wstr_len;
	}

    // 刷新的时候需要重新设置Title
    tab_ptr->is_reset_title = TRUE;
    MMIAPICOM_Wstrncpy(tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr,
        text_ptr->wstr_ptr, tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr_len);

    tab_ptr->item_ptr[pos].item_data_ptr->image_data.image_data_type = TABITEM_DATA_IMAGE_ID;
    tab_ptr->item_ptr[pos].item_data_ptr->image_data.image_data_u.anim_data.img_id = unsel_image_id;

    tab_ptr->item_ptr[pos].item_data_ptr->sel_image_id = sel_image_id;

    tab_ptr->item_ptr[pos].item_state |= tab_ptr->tab_style.item_state;
    
	if(MMITHEME_IsIstyle())
	{
		tab_ptr->tab_style.is_display_prg = FALSE;
	}
	else if (GUITAB_ITEMSTATE_UIMG_DTEXT == tab_ptr->tab_style.item_state)
	{
		tab_ptr->tab_style.is_display_prg = TRUE;
	}

    CTRLTAB_SetImageState(
        &tab_ptr->item_ptr[pos].item_data_ptr->image_data,
        GUITAB_IMAGE_NEED_SEL_BG_IMG|GUITAB_IMAGE_FILLED, TRUE);

    tab_ptr->item_ptr[pos].child_win_handle = 0;

    if (IsItemValid(&tab_ptr->item_ptr[pos]))
    {
        if(InsertValidItem(tab_ptr, &tab_ptr->item_ptr[pos], tab_ptr->valid_item_list.item_num))
        {
            tab_ptr->valid_item_list.item_num++;

            if (pos == tab_ptr->cur_sel_item_index)
            {
                ResetTabTitleText(tab_ptr, pos, FALSE);
            }
        }
    }

    CalculateItemWidth(tab_ptr, &tab_ptr->item_ptr[pos]);

    return(TRUE);
}

/*****************************************************************************/
//  Description : set item image
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:设置非选中状态下的图标
/*****************************************************************************/
LOCAL BOOLEAN SetItemImagePtr(
                              CTRLTAB_OBJ_T *tab_ptr,   // 控件ID
                              GUITAB_IMAGE_DATA_T *img_ptr,  // 设置数据
                              GUI_POINT_T start_point,
                              uint32 index             // 索引号
                              )
{
    BOOLEAN result = FALSE;
    GUITAB_ITEM_T *item_ptr = GetItemPtr(tab_ptr, index);
    GUITAB_ITEM_DATA_TYPE_E type = TABITEM_DATA_NONE;
    GUI_RECT_T rect = {0};
    GUI_RECT_T cut_rect = {0};
    uint32 valid_item_index = 0;
    CTRLBASE_OBJ_T * base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

    if (PNULL == tab_ptr || !TabTypeOf( (CTRLBASE_OBJ_T*)tab_ptr ))
    {
        return result;
    }

    if (PNULL != item_ptr && img_ptr != PNULL)
    {
        if (item_ptr->item_data_ptr == PNULL)
        {
            item_ptr->item_data_ptr = (GUITAB_ITEM_CONTENT_T*)SCI_ALLOC_APP(sizeof(GUITAB_ITEM_CONTENT_T));
            SCI_MEMSET(item_ptr->item_data_ptr, 0, sizeof(GUITAB_ITEM_CONTENT_T));
        }
        
        SCI_MEMCPY(&item_ptr->item_data_ptr->image_data, img_ptr, sizeof(GUITAB_IMAGE_DATA_T));

        if (!GetValidIndexByItemIndex(tab_ptr, index, &valid_item_index))
        {
            return result;
        }
        if (!GetItemImageRect(tab_ptr, valid_item_index, &rect)) 
        {
            return result; // 不需要画图片,直接返回
        }
        // 计算压缩区域
		cut_rect = ReadjustAnimRect(rect, item_ptr->item_data_ptr->image_data.comp_image_w, item_ptr->item_data_ptr->image_data.comp_image_h);

        type = item_ptr->item_data_ptr->image_data.image_data_type;
        
        switch( type )
        {
        case TABITEM_DATA_ANIM_DATA:
        case TABITEM_DATA_ANIM_PATH:
        case TABITEM_DATA_IMAGE_ID:
        case TABITEM_DATA_ANIM_ID:    
            {
                MMI_CONTROL_CREATE_T    create = {0};
                GUIANIM_INIT_DATA_T     init_data = {0};
                GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_FAIL;
                GUIANIM_CTRL_INFO_T     ctrl_info = {0};
                GUIANIM_DISPLAY_INFO_T  display_info = {0};
                
                SCI_ASSERT( 0 == item_ptr->add_data_ptr ); /*assert verified*/
                init_data.both_rect.h_rect = init_data.both_rect.v_rect = cut_rect;
                create.ctrl_id = 0;
                create.guid    = SPRD_GUI_ANIM_ID;
                create.parent_win_handle = base_ctrl_ptr->win_handle;
                create.parent_ctrl_handle = base_ctrl_ptr->handle;
                create.init_data_ptr = &init_data;
                item_ptr->add_data_ptr = MMK_CreateControl( &create );

                SCI_ASSERT( PNULL != item_ptr->add_data_ptr ); /*assert verified*/
                GUIANIM_SetDefaultIconByPtr(
                        item_ptr->add_data_ptr,
                        tab_ptr->tab_style.item_deficon_id,
                        tab_ptr->tab_style.item_error_id);

                //set anim param
                ctrl_info.is_ctrl_id = FALSE;
                ctrl_info.ctrl_id    = 0;
                ctrl_info.ctrl_ptr   = item_ptr->add_data_ptr;
                
                display_info.align_style       = GUIANIM_ALIGN_HMIDDLE_BOTTOM;
                display_info.is_play_once      = FALSE;
                display_info.is_syn_decode     = FALSE;
                display_info.is_update         = TRUE;
                display_info.is_disp_one_frame = TRUE;
                display_info.bg.bg_type        = GUI_BG_COLOR;
                display_info.bg.color          = tab_ptr->tab_style.bg_color;

                // 设置是否同步解码
                if (GetStatePtr(tab_ptr, GUITAB_STATE_SYNC_DECODE_ANIM))
                {
                    display_info.is_syn_decode = TRUE;
                }
               
                // 这里首先画焦点item的背景，然后再显示动画
                if (valid_item_index == tab_ptr->cur_sel_item_index)
				{
					MMI_IMAGE_ID_T image_id = 0;
					image_id = item_ptr->item_data_ptr->bg_image_id;

					if(MMITHEME_CheckImageID(image_id))
					{
						//display select icon
						DrawItemIcon(tab_ptr, rect, image_id);
					}
				}
                // 显示动画
                if(TABITEM_DATA_ANIM_DATA == type || TABITEM_DATA_IMAGE_ID == type || TABITEM_DATA_ANIM_ID == type)
                {
                    if ((index == tab_ptr->cur_sel_item_index) &&
						(MMITHEME_CheckImageID(item_ptr->item_data_ptr->sel_image_id)))
                    {
                        GUIANIM_DATA_INFO_T     anim_data    = {0};
                        // sel_image_id 图片只有一种类型为bmp图片,为了兼容以前的接口
                        anim_data.img_id = item_ptr->item_data_ptr->sel_image_id;
                        anim_result = GUIANIM_SetParam(&ctrl_info, (GUIANIM_DATA_INFO_T*)&anim_data, PNULL, &display_info );
                    }
                    else
                    {
                        anim_result = GUIANIM_SetParam(&ctrl_info, 
                            (GUIANIM_DATA_INFO_T*)&item_ptr->item_data_ptr->image_data.image_data_u.anim_data, PNULL, &display_info );
                    }
                }
                else
                {
                    anim_result = GUIANIM_SetParam(&ctrl_info, PNULL,
                        (GUIANIM_FILE_INFO_T*)&item_ptr->item_data_ptr->image_data.image_data_u.anim_path, &display_info );
                }
                SetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_NEED_CONTENT, FALSE);
            }
            break;
            
        default:
            //SCI_TRACE_LOW:"SetItemImagePtr: type %d is not anim!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_5660_112_2_18_3_22_0_365,(uint8*)"d",type);
            break;
        }
        
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetItemContentByPtr(
                                  CTRLTAB_OBJ_T               *tab_ptr,
                                  const GUITAB_ITEM_CONTENT_T *item_data_ptr,
                                  uint32                      index
                                  )
{
    BOOLEAN         result = FALSE;
    GUITAB_ITEM_T   *item_ptr = GetItemPtr( tab_ptr, index );
    
    if (PNULL != tab_ptr && PNULL != item_ptr && TabTypeOf( (CTRLBASE_OBJ_T*)tab_ptr ))
    {
        if (PNULL != item_data_ptr && index < tab_ptr->total_item_num)
        {
            CreateTabItemData( tab_ptr, item_ptr, item_data_ptr );
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 根据Item指针，判断是否需要插入列表，如果需要，则输出插入的位置
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetInsertPosition(
                                CTRLTAB_OBJ_T *tab_ptr, 
                                GUITAB_ITEM_T *item_ptr, 
                                uint32 *pos
                                )
{
    BOOLEAN       result    = FALSE;
    uint32        index     = 0;
    GUITAB_ITEM_T *valid_pre_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return FALSE;
    }

    if (PNULL == item_ptr) // 判断输入参数是否为空
    {
        result = FALSE;
    }
    else if (0 == tab_ptr->valid_item_list.item_num) // 当列表为空的时候
    {
        *pos = 0;
        result = TRUE;
    }
    else
    {
        for (index = 0; index < tab_ptr->valid_item_list.item_num; index++)
        {
            valid_pre_ptr = GetValidItemPtr(tab_ptr, index);
            
            if (item_ptr < tab_ptr->valid_item_list.item_ptr) // 增加在开头
            {
                *pos = 0;
                result = TRUE;
                break;
            }
            else if (PNULL != valid_pre_ptr && 
                     PNULL == valid_pre_ptr->next_ptr) // 增加在末尾
            {
                *pos = tab_ptr->valid_item_list.item_num;
                result = TRUE;
                break;
            }
            else if (PNULL != valid_pre_ptr && 
                     item_ptr > valid_pre_ptr && 
                     item_ptr < valid_pre_ptr->next_ptr)
            {
				*pos = index + 1;
                result = TRUE;
                break;
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : create item data
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateTabItemData(
                                CTRLTAB_OBJ_T*      tab_ptr,  // the list control pointer
                                GUITAB_ITEM_T*      item_ptr,     // 指定的Item
                                const GUITAB_ITEM_CONTENT_T* item_data_ptr
                                )
{
    BOOLEAN result = FALSE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == tab_ptr)||(PNULL == item_ptr))
    {
        return FALSE;
    }
    
    if( PNULL != item_data_ptr )
    {
        if( PNULL == item_ptr->item_data_ptr )
        {
            item_ptr->item_data_ptr = (GUITAB_ITEM_CONTENT_T*)SCI_ALLOC_APP(sizeof(GUITAB_ITEM_CONTENT_T));
            SCI_MEMSET(item_ptr->item_data_ptr, 0, sizeof(GUITAB_ITEM_CONTENT_T));
        }
        
        tab_ptr->is_reset_title = TRUE;
        SCI_MEMCPY( item_ptr->item_data_ptr, item_data_ptr, sizeof(GUITAB_ITEM_CONTENT_T) );
        
        result = TRUE;
    } 
    
    return result;
}

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyTabItemData(//is destroy anim ctrl
                                 CTRLTAB_OBJ_T*      tab_ptr,  // the tab control pointer
                                 GUITAB_ITEM_T*      item_ptr,  // 指定的Item
                                 BOOLEAN              is_page
                                 )
{
    BOOLEAN     result = FALSE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return FALSE;
    }
    
    if(PNULL != item_ptr && PNULL != item_ptr->item_data_ptr )
    {
        /* destroy inner object */
        if (PNULL != item_ptr->item_data_ptr)
        {
            if( 0 != item_ptr->add_data_ptr)
            {
                switch(item_ptr->item_data_ptr->image_data.image_data_type)
                {
                case TABITEM_DATA_ANIM_DATA:
                case TABITEM_DATA_ANIM_PATH:
                case TABITEM_DATA_IMAGE_ID:
                case TABITEM_DATA_ANIM_ID:
                    MMK_DestroyControl( GUICTRL_GetCtrlHandle( item_ptr->add_data_ptr) );
                    SetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_NEED_CONTENT, FALSE);
                    item_ptr->add_data_ptr = 0;
                    result = TRUE;
                    break;
                    
                default:
                    SCI_PASSERT(FALSE,("DestroyTabItemData: item_data_type %d is error!",item_ptr->item_data_ptr->image_data.image_data_type));/*assert verified*/
                    break;
                }
            }
        }
        
        if(!is_page)
        {
            SCI_FREE( item_ptr->item_data_ptr );
            item_ptr->item_data_ptr = PNULL;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyAllITabtemData(
                                    CTRLTAB_OBJ_T*       tab_ptr,  // the list control pointer
                                    BOOLEAN              is_page
                                    )
{
    uint32 i = 0;
    BOOLEAN result = FALSE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return FALSE;
    }
    
    if(!is_page)
    {
        for ( i = 0; i < tab_ptr->total_item_num; i++ )
        {
            DestroyTabItemData( tab_ptr, GetItemPtr( tab_ptr, i ), is_page );
        }
        
        result = TRUE;
    }

    //终止当前正在解码的item
    DestroyTabItemDecode(tab_ptr);
    
    return result;
}

/*****************************************************************************/
//  Description : alloc all items memory of the tab control
//  Global resource dependence : 
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocAllItemsSpace(
                                 CTRLTAB_OBJ_T* tab_ptr  // the list control pointer
                                 )
{
    BOOLEAN result = FALSE;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return FALSE;
    }
    
    if ( PNULL != tab_ptr->item_ptr )
    {
        SCI_FREE(tab_ptr->item_ptr);
    }
    
    if( tab_ptr->max_item_num > 0 )
    {
        // alloc the space
        tab_ptr->item_ptr = (GUITAB_ITEM_T*)SCI_ALLOCA( tab_ptr->max_item_num * sizeof(GUITAB_ITEM_T) );
        
        if ( PNULL != tab_ptr->item_ptr )
        {
            // init the space to 0
            SCI_MEMSET((void*)tab_ptr->item_ptr, 0, (tab_ptr->max_item_num*sizeof(GUITAB_ITEM_T)));
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"guitab.c: AllocAllItemsSpace failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_4925_112_2_18_3_21_58_359,(uint8*)"");
        }
    }
    
    return result;
}

/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: if is_set = TRUE, set title; else if is_set = FALSE, release title
/*****************************************************************************/
LOCAL BOOLEAN SetTabCommonTitleInfo(
                                    CTRLTAB_OBJ_T* tab_ptr,
                                    MMI_STRING_T *str_ptr,
                                    BOOLEAN is_set
                                    )
{ 
    BOOLEAN result = FALSE;

    if (PNULL != tab_ptr)
    {
        if (!is_set)
        {
            if (PNULL != tab_ptr->title_str_info.wstr_ptr)
            {
                ReleaseTabCommonTitleInfo(tab_ptr);

                result = TRUE;
            }
        }
        else
        {
            if (PNULL != str_ptr && PNULL != str_ptr->wstr_ptr && 0 != str_ptr->wstr_len)
            {
                ReleaseTabCommonTitleInfo(tab_ptr);

                tab_ptr->title_str_info.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar) * str_ptr->wstr_len);

                SCI_MEMSET(tab_ptr->title_str_info.wstr_ptr, 0, sizeof(wchar) * str_ptr->wstr_len);

                SCI_MEMCPY(tab_ptr->title_str_info.wstr_ptr, str_ptr->wstr_ptr, sizeof(wchar) * str_ptr->wstr_len);
                
                tab_ptr->title_str_info.wstr_len = str_ptr->wstr_len;

                result = TRUE;
            }
        }

        SetStatePtr(tab_ptr, GUITAB_STATE_COMMON_TITLE, is_set);
    }

    return result;
}

/*****************************************************************************/
// Description : release title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL void ReleaseTabCommonTitleInfo(
                                     CTRLTAB_OBJ_T* tab_ptr
                                     )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    if (PNULL != tab_ptr->title_str_info.wstr_ptr)
    {
        SCI_FREE(tab_ptr->title_str_info.wstr_ptr);
        tab_ptr->title_str_info.wstr_ptr = PNULL;
        tab_ptr->title_str_info.wstr_len = 0;
    }

    return;
}

/*****************************************************************************/
//  Description : handle MSG_TIMER message.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
LOCAL void CreateTabTitleCtrl(
                              CTRLTAB_OBJ_T * tab_ptr
                              )
{
	CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }
    
    if (PNULL == tab_ptr->title_ptr 
        && (!GetStatePtr(tab_ptr, GUITAB_STATE_SINGLE_LINE) || MMITHEME_IsIstyle()))
    {
        MMI_CONTROL_CREATE_T	create = {0};
        GUITITLE_INIT_DATA_T	init_data = {0};
        MMI_CTRL_ID_T			ctrl_id = 0;
        GUI_BOTH_RECT_T         both_rect = {0};
        uint16                  v_height = 0;
        uint16                  h_height = 0;
        
        init_data.text_id	= 0;
        
        ctrl_id = 0;// MMITHEME_GetTitleCtrlId();
        create.ctrl_id = ctrl_id;
        create.guid    = SPRD_GUI_TITLE_ID;
        create.parent_win_handle = base_ctrl_ptr->win_handle;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr = &init_data;
        
        tab_ptr->title_ptr = (void*)MMK_CreateControl( &create );
        
        if(PNULL != tab_ptr->title_ptr)
        {
//            ((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr)->bg_image = tab_ptr->bg_image_id;

            GUITITLE_SetFontColorByPtr((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr, tab_ptr->tab_style.font_color);

//             rect = ((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr)->rect;
//             rect.top = tab_ptr->rect.top;
//             rect.bottom = rect.top + (((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr)->rect.bottom - ((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr)->rect.top + 1);
// 
//             GUITITLE_SetRectByPtr((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr, rect);

            MMK_GetBothRect( ((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr)->base_ctrl.handle, &both_rect );

            //高度不变, 宽度适应tab
            v_height = (uint16)(both_rect.v_rect.bottom - both_rect.v_rect.top + 1);
            h_height = (uint16)(both_rect.h_rect.bottom - both_rect.h_rect.top + 1);
            
            both_rect = base_ctrl_ptr->both_rect;
            
            both_rect.v_rect.bottom = (int16)(both_rect.v_rect.top + v_height - 1);
            both_rect.h_rect.bottom = (int16)(both_rect.h_rect.top + h_height - 1);

            MMK_SetBothRect( ((CTRLTITLE_OBJ_T*)tab_ptr->title_ptr)->base_ctrl.handle, &both_rect );
        }

        tab_ptr->is_reset_title = TRUE;
    }
}

/*****************************************************************************/
//  Description : send need item data message
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SendTabNotifyMessage(
                                CTRLTAB_OBJ_T    *tab_ptr,
                                uint32           index,
                                uint32           pre_index,
                                MMI_MESSAGE_ID_E msg_id,
                                BOOLEAN          is_child,
                                MMI_HANDLE_T     handle
                                )
{
    GUITAB_ITEM_T           *item_ptr = PNULL;
    GUITAB_NEED_ITEM_DATA_T need_item_data = {0};
    CTRLBASE_OBJ_T *        base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }
    
    item_ptr = GetValidItemPtr(tab_ptr, index);

    SCI_ASSERT(PNULL != item_ptr); /*assert verified*/

    need_item_data.handle     = base_ctrl_ptr->handle; // MMK_GetCtrlId( tab_ptr->handle );
    need_item_data.item_index = index;

    if (index > pre_index)
    {
        need_item_data.is_to_left = TRUE;
    }
    else
    {
        need_item_data.is_to_left = FALSE;
    }

    need_item_data.point.x  = item_ptr->rect.left;
    need_item_data.point.y  = item_ptr->rect.top;
    need_item_data.is_child = is_child;
    
    if (is_child)
    {
        MMK_SendMsg( handle, msg_id, &need_item_data );
    }
    else
    {
        GUICTRL_SendNotifyEx( handle, msg_id, &need_item_data );
    }

    return;
}

/*****************************************************************************/
//  Description : 发送图片解析结果给应用窗口
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SendAnimDispResultToWin(
                                   CTRLTAB_OBJ_T    *tab_ptr,
                                   MMI_MESSAGE_ID_E msg_id,
                                   DPARAM            param
                                   )
{
    GUITAB_ANIM_DISP_RESULT_T anim_display_ind = {0};
    GUIANIM_DISPLAY_IND_T anim_ind = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = (CTRLBASE_OBJ_T *)tab_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }

    if (param != PNULL)
    {
        anim_ind = *(GUIANIM_DISPLAY_IND_T *)param;
    }
    anim_display_ind.result = anim_ind.result;
    GetItemIndexByValidIndex(tab_ptr, tab_ptr->cur_object_index, &anim_display_ind.item_index);
    
    GUICTRL_SendNotifyEx( base_ctrl_ptr->handle, msg_id, &anim_display_ind );
}

/*****************************************************************************/
// 	Description : send msg to win, notify it append the next item content
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void NotifyAppendNextItemContent(
                                       CTRLTAB_OBJ_T   *tab_ptr
                                       )
{
    BOOLEAN         is_finished = TRUE;
    uint32          i = 0;
    uint32          index = 0;
    uint32          page_item_num = 0;
    GUI_POINT_T     start_point = {0};
    GUITAB_ITEM_T   *item_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T  *)tab_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != tab_ptr); /*assert verified*/
    if (PNULL == tab_ptr)
    {
        return;
    }
    
    if (tab_ptr->valid_item_list.item_num == 0)
    {
        return;
    }

    page_item_num = GetShowItemNum(tab_ptr);
    for (i = 0; i < page_item_num; i++)
    {
        index     = (uint32)((tab_ptr->start_show_item_index + i) % tab_ptr->valid_item_list.item_num);
        item_ptr = GetValidItemPtr(tab_ptr, index);
        SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
        
        if (GetItemStatePtr( item_ptr, GUITAB_ITEMSTATE_NEED_CONTENT ) &&
            (PNULL != item_ptr->item_data_ptr))
        {
            if ((TABITEM_DATA_ANIM_DATA == item_ptr->item_data_ptr->image_data.image_data_type || 
                TABITEM_DATA_ANIM_PATH == item_ptr->item_data_ptr->image_data.image_data_type ||
                TABITEM_DATA_IMAGE_ID == item_ptr->item_data_ptr->image_data.image_data_type ||
                TABITEM_DATA_ANIM_ID == item_ptr->item_data_ptr->image_data.image_data_type) && 
                (0 == item_ptr->add_data_ptr))
            {
                is_finished = FALSE;
                break;
            }
        }
        
        if(!is_finished)
        {
            break;
        }
    }
    
    if(is_finished)
    {
        // 数据加载完毕
        SetStatePtr( tab_ptr, GUITAB_STATE_SETTING_ITEM_DATA, FALSE );
        ResetScrollState(tab_ptr);   
    }
    else
    {
        item_ptr = GetValidItemPtr(tab_ptr, index);
        SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
        
        for (i = 0; i < tab_ptr->total_item_num; i++)
        {
            if (item_ptr == GetItemPtr(tab_ptr, i))
            {
                break;
            }
        }

        tab_ptr->cur_object_index = i;
        start_point.x = item_ptr->rect.left;
        start_point.y = item_ptr->rect.top;

        if ( PNULL != item_ptr->item_data_ptr
             && CTRLTAB_GetImageState(&item_ptr->item_data_ptr->image_data, GUITAB_IMAGE_FILLED))
        {
            SetItemImagePtr(tab_ptr, &item_ptr->item_data_ptr->image_data, start_point, i);
        }
        else
        {
            SendTabNotifyMessage(tab_ptr, i, i, MSG_NOTIFY_TAB_NEED_ITEM_CONTENT, FALSE, base_ctrl_ptr->handle);
        }

        // 正在加载数据
        SetStatePtr( tab_ptr, GUITAB_STATE_SETTING_ITEM_DATA, TRUE );
    }
}

/*****************************************************************************/
//  Description : set the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTAB_SetState( 
                            MMI_CTRL_ID_T   ctrl_id,
                            GUITAB_STATE_T  src_state,
                            BOOLEAN         is_true
                            )
{
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);

    if(tab_ptr != PNULL)
    {
        SetStatePtr(tab_ptr, src_state, is_true);
    }
}

/*****************************************************************************/
//  Description : get the tab state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetState(
                               MMI_CTRL_ID_T    ctrl_id,
                               GUITAB_STATE_T   src_state
                               )
{
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);

    if(tab_ptr != PNULL)
    {
        return (GetStatePtr((const CTRLTAB_OBJ_T*)tab_ptr, src_state));
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTAB_SetItemState( 
                                MMI_CTRL_ID_T           ctrl_id,
                                uint32                  item_index,
                                GUITAB_ITEM_STATE_T     src_state,
                                BOOLEAN                 is_true
                                )
{
    CTRLTAB_OBJ_T *tab_ptr = PNULL;//
    GUITAB_ITEM_T *item_ptr = PNULL;

    tab_ptr = GetTabPtr(ctrl_id);
    
    if(tab_ptr != PNULL)
    {
        item_ptr = GetItemPtr((const CTRLTAB_OBJ_T*)tab_ptr, item_index);
        
        SetItemStatePtr(item_ptr, src_state, is_true);
    }
}

/*****************************************************************************/
//  Description : get the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetItemState(
                                   MMI_CTRL_ID_T        ctrl_id,
                                   uint32               item_index,
                                   GUITAB_ITEM_STATE_T  src_state
                                   )
{
    CTRLTAB_OBJ_T *tab_ptr = PNULL;//
    GUITAB_ITEM_T *item_ptr = PNULL;

    tab_ptr = GetTabPtr(ctrl_id);

    if(tab_ptr != PNULL)
    {
        item_ptr = GetItemPtr((const CTRLTAB_OBJ_T*)tab_ptr, item_index);
        
        return GetItemStatePtr(item_ptr, src_state);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTAB_SetImageState( 
                                GUITAB_IMAGE_DATA_T*    image_data_ptr,
                                GUITAB_IMAGE_STATE_T    src_state,
                                BOOLEAN                 is_true
                                )
{
    if(PNULL != image_data_ptr)
    {
        SetState( &image_data_ptr->state, src_state, is_true );
    }
}

/*****************************************************************************/
//  Description : get the item state
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetImageState(
                                    const GUITAB_IMAGE_DATA_T*  image_data_ptr,
                                    GUITAB_IMAGE_STATE_T        src_state
                                    )
{
    if (PNULL != image_data_ptr)
    {
        return GetState(image_data_ptr->state, src_state );
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :  set max item num, must be called before append item or insert item.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetMaxItemNum(
                                    MMI_CTRL_ID_T ctrl_id, // control id
                                    uint32        max_item_num
                                    )
{
    if(GUITAB_MAX_ITEM_NUM >= max_item_num)
    {
        return SetMaxItemByPtr(GetTabPtr(ctrl_id), max_item_num);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : add child window in the specifed position.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AddChildWindow(
                                     MMI_CTRL_ID_T     ctrl_id, 
                                     MMI_HANDLE_T	   child_win_handle,
                                     uint32            pos
                                     )
{
	BOOLEAN	result = FALSE;
	CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL != tab_ptr)
    {
        if ((tab_ptr->max_item_num > 0))
        {
            if (tab_ptr->item_ptr[pos].child_win_handle == 0)
            {
                tab_ptr->item_ptr[pos].child_win_handle = child_win_handle;	
                result = TRUE;
            }
        }
    }

    return(result);
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AppendItem(
                                 MMI_CTRL_ID_T         ctrl_id,
                                 const GUITAB_ITEM_T   *item_ptr
                                 )
{
    BOOLEAN         result = FALSE;
    CTRLTAB_OBJ_T   *tab_ptr = GetTabPtr(ctrl_id);

    if (PNULL == tab_ptr)
    {
        //SCI_TRACE_LOW:"CTRLTAB_AppendItem error ctrl type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_5040_112_2_18_3_21_58_360,(uint8*)"");

        return result;
    }

    result = InsertItemByPtr( tab_ptr, item_ptr, tab_ptr->total_item_num );

    tab_ptr->cur_sel_item_index = 0;
    tab_ptr->start_show_item_index = 0;
    tab_ptr->is_scroll = IsItemScroll(tab_ptr);

    return result;
}

/*****************************************************************************/
//  Description : Add the item to tab list
//  Global resource dependence : 
//  Author:Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_InsertItem(
                                  MMI_CTRL_ID_T	           ctrl_id,     // control id
                                  const GUITAB_ITEM_T      *item_ptr,	// 指定的Item
                                  uint32                   pos		    // 指定的位置, from 0 to total_item_num
                                  )
{
    return InsertItemByPtr( GetTabPtr( ctrl_id ), item_ptr, pos );
}

/*****************************************************************************/
//  Description : append a new tab item to tail.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AppendSimpleItem(
                                       MMI_CTRL_ID_T     ctrl_id, 
                                       MMI_STRING_T *    text_ptr,
                                       MMI_IMAGE_ID_T    sel_image_id,
                                       MMI_IMAGE_ID_T    unsel_image_id
                                       )
{
	BOOLEAN				result = FALSE;
	CTRLTAB_OBJ_T       *tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL == tab_ptr || PNULL == text_ptr)
    {
        //SCI_TRACE_LOW:"GUITAB_AppendSimpleItem input param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_5113_112_2_18_3_21_59_362,(uint8*)"");

        return result;
    }

	if ((tab_ptr->max_item_num > 0) && (tab_ptr->total_item_num < tab_ptr->max_item_num))
	{
		// append item
		result = AppendSimpleItem(tab_ptr, text_ptr, sel_image_id, unsel_image_id);

        tab_ptr->total_item_num++;
		tab_ptr->cur_sel_item_index = 0;
		tab_ptr->start_show_item_index = 0;
		tab_ptr->is_scroll = IsItemScroll(tab_ptr);
	}

    return(result);
}

/*****************************************************************************/
//  Description : delete a tab item.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_DeleteItem(
                                 MMI_CTRL_ID_T  ctrl_id, 
                                 uint32         pos
                                 )
{
	BOOLEAN				result      = FALSE;
	CTRLTAB_OBJ_T       *tab_ptr    = GetTabPtr(ctrl_id);
    
	if(PNULL == tab_ptr)
    {
        return result;
    }
	
	if (pos < tab_ptr->total_item_num)
	{
		if (tab_ptr->item_ptr[pos].child_win_handle > 0)
		{
			// close child window
			MMK_CloseWin(tab_ptr->item_ptr[pos].child_win_handle);
		}

        DestroyTabItemData(tab_ptr, GetItemPtr( tab_ptr, pos ), TRUE);

        /* memmove */
        if( tab_ptr->total_item_num - pos > 0 )
        {
            memmove(
                tab_ptr->item_ptr + pos,
                tab_ptr->item_ptr + pos + 1,
                ( tab_ptr->total_item_num - pos - 1) * sizeof(GUITAB_ITEM_T));
        }

        tab_ptr->total_item_num--;
		tab_ptr->cur_sel_item_index = 0;
		tab_ptr->start_show_item_index = 0;

        SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, FALSE);

		if (tab_ptr->total_item_num == 0)
		{
			tab_ptr->is_scroll = FALSE;
			tab_ptr->show_item_num = 0;
			SCI_MEMSET(&tab_ptr->item_rect, 0, sizeof(GUI_RECT_T));
		}
		else
		{
		    // update scroll status
			tab_ptr->is_scroll = IsItemScroll(tab_ptr);
		}
		
		result = TRUE;
	}

    return(result);
}

/*****************************************************************************/
//  Description : delete all tab items.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_DeleteAllItems(
                                     MMI_CTRL_ID_T ctrl_id
                                     )
{
	uint32				i = 0;
	CTRLTAB_OBJ_T *	    tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL == tab_ptr)
    {
        return FALSE;
    }

	if (tab_ptr->max_item_num > 0)
	{
		for (i = 0; i < tab_ptr->total_item_num; i++)
		{
			if (tab_ptr->item_ptr[i].child_win_handle > 0)
			{
			    // close child window
				MMK_CloseWin(tab_ptr->item_ptr[i].child_win_handle);
			}
		}
		
        tab_ptr->is_reset_title = TRUE;

        DestroyAllITabtemData(tab_ptr, FALSE);

		tab_ptr->total_item_num = 0;
		tab_ptr->cur_sel_item_index = 0;
		tab_ptr->start_show_item_index = 0;
		tab_ptr->is_scroll = FALSE;
		tab_ptr->show_item_num = 0;

        SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, FALSE);

 		SCI_MEMSET(&tab_ptr->item_rect, 0, sizeof(GUI_RECT_T));

        // 这里要重新设置valid list
        ResetValidItemList(tab_ptr);
	}
    
	//SCI_TRACE_LOW:"GUITAB_DeleteAllItems cur num = %d, total num = %d, valid num = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_5232_112_2_18_3_21_59_363,(uint8*)"ddd", tab_ptr->cur_sel_item_index, tab_ptr->total_item_num, tab_ptr->valid_item_list.item_num);

    return(TRUE);
}

/*****************************************************************************/
//  Description : get tab item num.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLTAB_GetItemNum(
                                 MMI_CTRL_ID_T ctrl_id
                                 )
{
	CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL != tab_ptr)
    {
        return(tab_ptr->total_item_num);     
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : modify item of the given position.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetSimpleItem( 
                                    MMI_CTRL_ID_T           ctrl_id,
                                    const MMI_STRING_T *    text_ptr,
                                    MMI_IMAGE_ID_T          sel_image_id,
                                    MMI_IMAGE_ID_T          unsel_image_id,
                                    uint32                  pos
                                    )
{
	CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);
	BOOLEAN				result = FALSE;

	if(PNULL == tab_ptr || PNULL == text_ptr)
    {
        return result;
    }
	
	if (pos < tab_ptr->total_item_num)
	{
		if (text_ptr->wstr_len > GUITAB_MAX_ITEM_NAME_LENGTH)
		{
			tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr_len = GUITAB_MAX_ITEM_NAME_LENGTH;
		}
		else
		{
			tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr_len = text_ptr->wstr_len;
		}

		SCI_MEMSET(
            tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr,
            0, sizeof(tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr));

		MMIAPICOM_Wstrncpy(
            tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr, 
            text_ptr->wstr_ptr,
            tab_ptr->item_ptr[pos].item_data_ptr->text_data.text.text_buffer.wstr_len);
		
		tab_ptr->item_ptr[pos].item_data_ptr->image_data.image_data_type = TABITEM_DATA_IMAGE_ID;
        tab_ptr->item_ptr[pos].item_data_ptr->sel_image_id = sel_image_id;
		tab_ptr->item_ptr[pos].item_data_ptr->image_data.image_data_u.anim_data.img_id = unsel_image_id;
        tab_ptr->item_ptr[pos].item_state |= tab_ptr->tab_style.item_state;//GUITAB_ITEMSTATE_SHOW_IMG;

        CTRLTAB_SetImageState(&tab_ptr->item_ptr[pos].item_data_ptr->image_data, GUITAB_IMAGE_NEED_SEL_BG_IMG|GUITAB_IMAGE_FILLED, TRUE);

        // 刷新的时候需要重新设置Title
        tab_ptr->is_reset_title = TRUE;

		result = TRUE;
	}

    return(result);
}

/*****************************************************************************/
//  Description : Add the item to list
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemContent(
                                     MMI_CTRL_ID_T	                ctrl_id,
                                     const GUITAB_ITEM_CONTENT_T    *item_data_ptr,
                                     uint32                         index
                                     )
{
    return SetItemContentByPtr(GetTabPtr( ctrl_id ), item_data_ptr, index );
}

/*****************************************************************************/
//  Description : set item text
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemText(
                                  MMI_CTRL_ID_T     ctrl_id, 
                                  GUITAB_TEXT_T *   text_ptr, 
                                  uint32            index
                                  )
{
    BOOLEAN result = FALSE;
    uint16  str_len = 0;
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T *item_ptr = GetItemPtr(tab_ptr, index);

    if(tab_ptr != PNULL && item_ptr != PNULL && text_ptr != PNULL)
    {
        if (item_ptr->item_data_ptr == PNULL)
        {
            item_ptr->item_data_ptr = (GUITAB_ITEM_CONTENT_T*)SCI_ALLOC_APP(sizeof(GUITAB_ITEM_CONTENT_T));
            SCI_MEMSET(item_ptr->item_data_ptr, 0, sizeof(GUITAB_ITEM_CONTENT_T));
        }

        str_len = (uint16)MIN(GUITAB_MAX_ITEM_NAME_LENGTH, text_ptr->wstr_len);
        item_ptr->item_data_ptr->text_data.text.text_buffer.wstr_len = str_len;

        MMIAPICOM_Wstrncpy(item_ptr->item_data_ptr->text_data.text.text_buffer.wstr, text_ptr->wstr, str_len);

        item_ptr->item_data_ptr->text_data.text_type = TABITEM_DATA_TEXT_BUFFER;

        // 刷新的时候需要重新设置title
        tab_ptr->is_reset_title = TRUE;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set item text
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:设置非选中状态下的图标
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemImage(
                                   MMI_CTRL_ID_T        ctrl_id,        // 控件ID
                                   GUITAB_IMAGE_DATA_T* img_ptr,        // 设置数据
                                   GUI_POINT_T          start_point,
                                   uint32               index           // 索引号
                                   )
{
    return SetItemImagePtr(GetTabPtr(ctrl_id), img_ptr, start_point, index);
}

/*****************************************************************************/
//  Description : set the tab item valid or not
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemValid( 
                                   MMI_CTRL_ID_T        ctrl_id,
                                   uint32               item_index,
                                   BOOLEAN              is_valid
                                   )
{
    BOOLEAN       result    = FALSE;
    uint32        index     = 0;
    CTRLTAB_OBJ_T *tab_ptr  = PNULL;//
    GUITAB_ITEM_T *item_ptr = PNULL;

    tab_ptr = GetTabPtr(ctrl_id);

    if(tab_ptr == PNULL)
    {
        return result;
    }

    item_ptr = GetItemPtr((const CTRLTAB_OBJ_T*)tab_ptr, item_index);

    result = IsItemValid(item_ptr);

    if (PNULL != item_ptr && is_valid != result)
    {
	    tab_ptr->show_item_num = 0;

        SetItemStatePtr(item_ptr, GUITAB_ITEMSTATE_INVALID, (BOOLEAN)!is_valid);

        if (is_valid) // 这一步增加一个item到valid list
        {
            if (GetInsertPosition(tab_ptr, item_ptr, &index) 
                && InsertValidItem(tab_ptr, item_ptr, index))
            {
                tab_ptr->valid_item_list.item_num++;

                if (index <= tab_ptr->cur_sel_item_index)
				{
                    tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
					tab_ptr->cur_sel_item_index++;
				}
            }
        }
        else // 这一步，删除一个item从valid list中
        {
            if (GetValidIndexByItemIndex(tab_ptr, item_index, &index))
            {
                if (DeleteValidItem(tab_ptr, item_ptr))
                {
                    tab_ptr->valid_item_list.item_num--;
                    
                    if (index <= tab_ptr->cur_sel_item_index)
                    {
                        tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
                        if (0 == tab_ptr->valid_item_list.item_num || 0 == tab_ptr->cur_sel_item_index)
                        {
                            tab_ptr->cur_sel_item_index = 0;
                        }
                        else
                        {
                            tab_ptr->cur_sel_item_index--;
                            tab_ptr->cur_sel_item_index %= tab_ptr->valid_item_list.item_num;
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

/*****************************************************************************/
// Description : set title icon
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleIcon(
                                   MMI_CTRL_ID_T    ctrl_id,
                                   MMI_IMAGE_ID_T   icon_id
                                   )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

    if (PNULL != tab_ptr && PNULL != tab_ptr->title_ptr)
    {
        result = TRUE;
        ((CTRLTITLE_OBJ_T*)(tab_ptr->title_ptr))->icon_id = icon_id;
    }

    return result;
}

/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: if is_set = TRUE, set title; else if is_set = FALSE, release title
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleInfo(
                                   MMI_CTRL_ID_T    ctrl_id,
                                   MMI_STRING_T *   str_ptr,
                                   BOOLEAN          is_set
                                   )
{
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

    return SetTabCommonTitleInfo(tab_ptr, str_ptr, is_set);
}

/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleTextId(
                                     MMI_CTRL_ID_T ctrl_id,
                                     MMI_TEXT_ID_T text_id
                                     )
{
    MMI_STRING_T  text = {0};
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);

    MMITHEME_GetResText(text_id, ctrl_id, &text);
    
    return SetTabCommonTitleInfo(tab_ptr, &text, TRUE);
}

/*****************************************************************************/
//  Description : set current selected item.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetCurSel(
                                MMI_CTRL_ID_T   ctrl_id, 
                                uint32          item_index
                                )
{
	BOOLEAN			result = FALSE;
    uint32          i = 0;
    CTRLTAB_OBJ_T  *tab_ptr = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T  *item_ptr = PNULL;

	if(PNULL == tab_ptr)
    {
        return result;
    }

    item_ptr = GetItemPtr(tab_ptr, item_index);

    if (PNULL == item_ptr)
    {
        return result;
    }

    for (i = 0; i < tab_ptr->valid_item_list.item_num; i++)
    {
        if (item_ptr == GetValidItemPtr(tab_ptr, i))
        {
            if (i != tab_ptr->cur_sel_item_index)
            {
                // 需要重新计算
                tab_ptr->show_item_num = 0;
            }

            tab_ptr->cur_sel_item_index = i;

            SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, FALSE);

            tab_ptr->is_reset_title = TRUE;

            result = TRUE;
            break;
        }
    }

	return(result);
}

/*****************************************************************************/
//  Description : get current selected item.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC GUITAB_ITEM_T* CTRLTAB_GetCurItem(
                                         MMI_CTRL_ID_T ctrl_id
                                         )
{
    CTRLTAB_OBJ_T  *tab_ptr = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T  *item_ptr = PNULL;

	if(PNULL != tab_ptr)
    {
        item_ptr = GetValidItemPtr(tab_ptr, tab_ptr->cur_sel_item_index);
    }
	
    return(item_ptr);
}


/*****************************************************************************/
//  Description : get current selected item.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLTAB_GetCurSel(
                                MMI_CTRL_ID_T ctrl_id
                                )
{
    uint32			cur_index = 0;
    uint32          i = 0;
    CTRLTAB_OBJ_T  *tab_ptr = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T  *item_ptr = PNULL;

	if(PNULL != tab_ptr)
    {
        item_ptr = GetValidItemPtr(tab_ptr, tab_ptr->cur_sel_item_index);
        
        for (i = 0; i < tab_ptr->total_item_num; i++)
        {
            if (item_ptr == GetItemPtr(tab_ptr, i))
            {
                cur_index = i;
                break;
            }
        }
    }
	
    return(cur_index);
}

/*****************************************************************************/
//  Description : set 开始位置的item index
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetStartIndex(
                                    MMI_CTRL_ID_T   ctrl_id, 
                                    uint32          item_index
                                    )
{
	BOOLEAN			result = FALSE;
    uint32          i = 0;
    CTRLTAB_OBJ_T  *tab_ptr = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T  *item_ptr = PNULL;

	if(PNULL == tab_ptr)
    {
        return result;
    }

    item_ptr = GetItemPtr(tab_ptr, item_index);

    if (PNULL == item_ptr)
    {
        return result;
    }

    for (i = 0; i < tab_ptr->valid_item_list.item_num; i++)
    {
        if (item_ptr == GetValidItemPtr(tab_ptr, i))
        {
            if (i != tab_ptr->start_show_item_index)
            {
                // 需要重新计算
                tab_ptr->show_item_num = 0;
            }

            tab_ptr->start_show_item_index = i;

            result = TRUE;

            break;
        }
    }

	return(result);
}

/*****************************************************************************/
//  Description : 获得开始位置的item index
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLTAB_GetStartIndex(
                                   MMI_CTRL_ID_T ctrl_id
                                   )
{
    uint32			start_index = 0;
    uint32          i = 0;
    CTRLTAB_OBJ_T  *tab_ptr = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T  *item_ptr = PNULL;

	if(PNULL != tab_ptr)
    {
        item_ptr = GetValidItemPtr(tab_ptr, tab_ptr->start_show_item_index);

        for (i = 0; i < tab_ptr->total_item_num; i++)
        {
            if (item_ptr == GetItemPtr(tab_ptr, i))
            {
                start_index = i;

                break;
            }
        }
    }
	
    return(start_index);
}

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetDividingLine(
                                       MMI_CTRL_ID_T    ctrl_id, 
                                       MMI_IMAGE_ID_T   image_id
                                       )
{
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.dividing_line = image_id;

        return(TRUE);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTpDownBg(
                                   MMI_CTRL_ID_T    ctrl_id, 
                                   MMI_IMAGE_ID_T   image_id
                                   )
{
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.tp_sel_img_id = image_id;

        return(TRUE);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetIsDisplayPrg(
                                       MMI_CTRL_ID_T    ctrl_id, 
                                       BOOLEAN          is_display_prg
                                       )
{
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.is_display_prg = is_display_prg;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : set back ground image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetBackGroundImage(
                                         MMI_CTRL_ID_T  ctrl_id, 
                                         MMI_IMAGE_ID_T image_id
                                         )
{
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

	if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.bg_image_id = image_id;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetBackGroundColor(
                                         MMI_CTRL_ID_T  ctrl_id, 
                                         GUI_COLOR_T    bg_color
                                         )
{
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);
	
    if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.bg_color = bg_color;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : set font parameter.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFontParam( 
                                   MMI_CTRL_ID_T    ctrl_id,
                                   GUI_FONT_T       font,
                                   GUI_COLOR_T      font_color,
                                   uint8            font_space
                                   )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);
    
    if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.font = font;
        tab_ptr->tab_style.font_color = font_color;
        tab_ptr->tab_style.font_space = font_space;

        result = TRUE;
    }

	return result;
}

/*****************************************************************************/
//  Description : set additional information
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetAddedString( 
                                     MMI_CTRL_ID_T      ctrl_id,
                                     GUITAB_CAPTION_T   added_string
                                     )
{
    BOOLEAN result = FALSE;
	CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

    if(PNULL != tab_ptr)
    {
        tab_ptr->added_string = added_string;
        result = TRUE;
    }

//     if (PNULL != tab_ptr->title_ptr)
//     {
//         // 设置文本信息
//         GUITITLE_SetSubTextByPtr(    
//             (CTRLTITLE_OBJ_T  *)tab_ptr->title_ptr,
//             TRUE,
//             added_string.text.text_buffer.wstr,
//             added_string.text.text_buffer.wstr_len,
//             FALSE
//             );
//         // 设置风格信息
//         GUITITLE_SetSubTextParamByPtr(    
//             (CTRLTITLE_OBJ_T*)tab_ptr->title_ptr,
//             &added_string.text_style.rect,
//             &added_string.text_style.font_type,
//             &added_string.text_style.align
//             );
//     }

    return result;
}

/*****************************************************************************/
//  Description : get additional information
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetAddedString( 
                                     MMI_CTRL_ID_T    ctrl_id,
                                     GUITAB_CAPTION_T *added_string_ptr
                                     )
{
    BOOLEAN result = FALSE;
	CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);

    if(PNULL != tab_ptr && PNULL != added_string_ptr)
    {
        *added_string_ptr = tab_ptr->added_string;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set auto switch
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetAutoSwitch(
                                    MMI_CTRL_ID_T   ctrl_id, 
                                    uint32          timer, 
                                    BOOLEAN         is_true
                                    )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);

    if(tab_ptr != PNULL)   
    {
        tab_ptr->auto_switch_time = timer;
        
        SetStatePtr(tab_ptr, GUITAB_STATE_AUTO_SWITCH, is_true);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get auto switch
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetAutoSwitch(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    uint32 *        timer
                                    )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);

    if(tab_ptr != PNULL && timer != PNULL)
    {
        *timer = tab_ptr->auto_switch_time;

        result = GetStatePtr(tab_ptr, GUITAB_STATE_AUTO_SWITCH);
    }

    return result;
}

/*****************************************************************************/
//  Description : get auto switch
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SwitchToNextItem(
                                       MMI_CTRL_ID_T            ctrl_id,
                                       GUITAB_SWITCH_TYPE_E     type
                                       )
{
    uint32          i           = 0;
    uint32          move_num    = 0;
    BOOLEAN	        result      = TRUE;
    CTRLTAB_OBJ_T   *tab_ptr    = GetTabPtr(ctrl_id);
    uint32          cur_item_index = 0;

    if (tab_ptr == PNULL || tab_ptr->valid_item_list.item_num == 0)
    {
        //SCI_TRACE_LOW:"GUITAB_SwitchToNextItem FALSE!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_6233_112_2_18_3_22_1_367,(uint8*)"");
        return FALSE;
    }
    
    switch(type) 
    {
    case GUITAB_SWITCH_NEXT:
        cur_item_index = tab_ptr->cur_sel_item_index;
        move_num = GetMoveItemNum(tab_ptr, GUITAB_SWITCH_NEXT);

        for (i = 0; i < move_num; i++)
        {
            MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_NEXT);
        }

        tab_ptr->pre_sel_item_index = tab_ptr->cur_sel_item_index;
        tab_ptr->cur_sel_item_index = cur_item_index + 1;
        tab_ptr->cur_sel_item_index %= tab_ptr->valid_item_list.item_num;

        SetStatePtr(tab_ptr, GUITAB_STATE_IS_PAINT, FALSE);

        UpdateCurPageItemRect(tab_ptr);
        SetScrollCardState(tab_ptr, type);
		DrawChildWindow(tab_ptr);
    	break;

    case GUITAB_SWITCH_PRE:
        MoveToNextValidItem(tab_ptr, GUITAB_SWITCH_PRE);
        UpdateCurPageItemRect(tab_ptr);
        SetScrollCardState(tab_ptr, type);
		DrawChildWindow(tab_ptr);
        break;
        
    default:
        //SCI_TRACE_LOW:"GUITAB_SwitchToNextItem type error! %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUITAB_6265_112_2_18_3_22_1_368,(uint8*)"d", type);
        result = FALSE;
        break;
    }

    return(result);
}

/*****************************************************************************/
//  Description : set space between two items
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
//       ctrl_id: 控件id
//       item_space：item space value
//       is_use_default: 是否使用缺省值，TRUE:使用is_use_default；FALSE:设置值为
//                            MIN(item_space, GUITAB_DEFAULT_ITEM_SPACE)
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemSpace(
                                   MMI_CTRL_ID_T    ctrl_id,
                                   uint32           item_space,
                                   BOOLEAN          is_use_default
                                   )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);

    if(PNULL != tab_ptr)
    {
        if (is_use_default)
        {
            tab_ptr->item_space = GUITAB_DEFAULT_ITEM_SPACE;
        }
        else
        {
            tab_ptr->item_space = MIN(item_space, GUITAB_DEFAULT_ITEM_SPACE);
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set the item's width
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemWidth(
                                   MMI_CTRL_ID_T    ctrl_id,
                                   uint16           width
                                   )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);
    
    if(PNULL != tab_ptr)
    {
        if ( width > 0 )
        {
            tab_ptr->item_width = width;
            
            SetStatePtr(tab_ptr, INNERTAB_STATE_CUSTOMER_WIDTH, TRUE); // 客户自设置区域
			SetStatePtr(tab_ptr, GUITAB_STATE_IS_ADAPT_WIDTH, FALSE);// 不做自适应
            
            // set scroll card rect
            UpdateScrollCardRect(tab_ptr);
            
            // update heght & total item rect
            UpdateTabItemHeight(tab_ptr);
            
            // update item total rect
            UpdateTabItemTotalRect(tab_ptr);
            
            // update all items width
            UpdateAllItemWidth(tab_ptr);
            
            // 这里要重新设置valid list
            ResetValidItemList(tab_ptr); // 更新tab项

            InitCurPageItemRect(tab_ptr);

            // 删除无效的item数据
            DeleteInvalidItemData(tab_ptr);
            
            // reset scroll state
            ResetScrollState(tab_ptr);
            
            result = TRUE;
        }
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set the item's default width
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemDefaultHeight(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          uint32            item_height
                                          )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);
    
    if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.item_default_height = item_height;
        
        // set scroll card rect
        UpdateScrollCardRect(tab_ptr);
        
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 设置tab控件是否循环显示。返回设置之前的状态
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTabRotative(
                                     MMI_CTRL_ID_T      ctrl_id, 
                                     BOOLEAN            is_rotative
                                     )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);
    
    if(PNULL != tab_ptr)
    {   
        result = TRUE;
        
        tab_ptr->is_rotative = is_rotative;
    }

	return (result);
}

/*****************************************************************************/
//  Description : 设置Tab控件的默认图片ID
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetDefaultIcon(
                                     MMI_CTRL_ID_T     ctrl_id, //in
                                     MMI_IMAGE_ID_T    def_img_id,  // default image id
                                     MMI_IMAGE_ID_T    err_img_id   // error image id
                                     )
{
    BOOLEAN         result   = FALSE;
    CTRLTAB_OBJ_T   *tab_ptr = GetTabPtr(ctrl_id);
    
    if (PNULL != tab_ptr)
    {
        if (MMITHEME_CheckImageID(def_img_id))
        {
            tab_ptr->tab_style.item_deficon_id = def_img_id;
        }

        if (MMITHEME_CheckImageID(err_img_id))
        {
            tab_ptr->tab_style.item_error_id = err_img_id;
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFocusItemBgColor(
                                          MMI_CTRL_ID_T     ctrl_id, 
                                          GUI_COLOR_T       bg_color
                                          )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *	tab_ptr = GetTabPtr(ctrl_id);
    
    if(PNULL != tab_ptr)
    {
        tab_ptr->tab_style.focus_rect_color = bg_color;
        result = TRUE;
    }
    
    return(result);
}

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFocusItemTextBgImage(
                                              MMI_CTRL_ID_T     ctrl_id, 
                                              MMI_IMAGE_ID_T    image_id,
                                              uint32            item_index
                                              )
{
    BOOLEAN       result    = FALSE;
    CTRLTAB_OBJ_T *tab_ptr  = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T *item_ptr = PNULL;

    if(PNULL != tab_ptr)
    {
        item_ptr = GetItemPtr(tab_ptr, item_index);
        if (PNULL != item_ptr && PNULL != item_ptr->item_data_ptr)
        {
            result = TRUE;
            item_ptr->item_data_ptr->text_data.pressed_image_id = image_id;
        }
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : Set the back ground color.
//	Global resource dependence : 
//  Author: 当item处于焦点状态时，会显示焦点状态的背景，调用该接口可改变焦点背景图片
//	Note:
/*****************************************************************************/
PUBLIC void CTRLTAB_SetItemSelBgImage(
									 MMI_CTRL_ID_T      ctrl_id, 
									 MMI_IMAGE_ID_T     image_id,
									 BOOLEAN            is_update
									 )
{
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr(ctrl_id);
    GUITAB_ITEM_T *item_ptr = PNULL;
	uint32		  item_index = 0;
    
    if(PNULL != tab_ptr)
    {
        for (item_index = 0; item_index < tab_ptr->total_item_num; item_index++)
        {
            item_ptr = GetItemPtr(tab_ptr, item_index);
            if (PNULL != item_ptr && PNULL != item_ptr->item_data_ptr)
            {
                item_ptr->item_data_ptr->bg_image_id = image_id;
            }
        }
        
        if(is_update)
        {
            DrawCtrl(tab_ptr);
        }
    }
    
    return;
}

/*****************************************************************************/
//  Description : 设置显示动画的时候是否同步解码，默认是异步解码
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 返回设置之前的状态
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetSyncDecodeAnim(
                                        MMI_CTRL_ID_T   ctrl_id,
                                        BOOLEAN         is_sync_decode
                                        )
{
    BOOLEAN result = FALSE;
    CTRLTAB_OBJ_T *tab_ptr = GetTabPtr( ctrl_id );

    if(PNULL != tab_ptr)
    {
        result = GetStatePtr(tab_ptr, GUITAB_STATE_SYNC_DECODE_ANIM);
        
        SetStatePtr( tab_ptr, GUITAB_STATE_SYNC_DECODE_ANIM, is_sync_decode );
    }

    return result;
}

