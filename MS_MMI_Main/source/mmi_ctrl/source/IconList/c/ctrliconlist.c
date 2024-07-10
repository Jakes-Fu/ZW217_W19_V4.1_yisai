/*****************************************************************************
** File Name:      ctrliconlist.c                                               *
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
#ifdef GUIF_ICONLIST
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "mmk_timer.h"
#include "chng_freq.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_tp.h"
#include "ui_layer.h"
//#include "guiiconfolder.h"
#include "mmi_theme.h"
#include "mmitheme_iconlist.h"
#include "guistring.h"
#include "guires.h"
#include "guiwin.h"
#include "cafctrliconlist.h"
#ifdef DYNAMIC_MODULE_SUPPORT  
#include "caffilesys.h"
#endif
#ifdef GUIF_ICONFOLDER
#include "ctrliconfolder.h"
#endif
#include "ctrlanim.h"

#include "ctrliconlist.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define ASCII_CODE_ZHE              		    0x2F  // ascii code
#define SELECT_FLAG_WIDTH           	    1   	//select rect width

#define MIN_ITEM_MEM_ALLOC_NUM      20      //动态分配item空间时，最小的一次分配单位，item个数

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
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
//  Description : init iconlist class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlInitVtbl(
    CTRLICONLIST_VTBL_T        *iconlist_vtbl_ptr
);

/*****************************************************************************/
//  Description : iconlist construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLICONLIST_INIT_PARAM_T   *iconlist_param_ptr
);

/*****************************************************************************/
//  Description : iconlist destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void IconlistCtrlPackInitParam (
    GUIICONLIST_INIT_DATA_T     *iconlist_init_ptr,   //in
    CTRLICONLIST_INIT_PARAM_T   *iconlist_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle iconlist msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IconlistCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : set icon list rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlSetRect(
                              CTRLBASE_OBJ_T	*base_ctrl_ptr,
                              const CAF_RECT_T    *rect_ptr
                              );

/*****************************************************************************/
//  Description : init icon list display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitDisplayRect(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : get row and column number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetRowColNum(
                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                        );

/*****************************************************************************/
//  Description : alloc item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AllocItemMemory(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : free item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeItemMemory(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : free one item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeOneItemMemory(
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr, //in:
                             GUIICONLIST_ITEM_T     *item_ptr           //in:
                             );

/*****************************************************************************/
//  Description : free current page item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeCurPageItemMemory(
                                 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : free not in current page item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeNotCurPageItemMemory(
                                    uint16              pre_top_index,
                                    uint16              pre_bottom_index,
                                    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : display icon list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconList(
                           BOOLEAN              is_update_prgbox,
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : display icon list bg image or color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconListBg(
                             GUI_RECT_T         bg_rect,
                             CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : display all item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayAllItem(
                          BOOLEAN               is_update_prgbox,
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : display item icon or folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayItemIcon(
                           BOOLEAN              is_update,
                           uint16               index,
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : set icon control rect and display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlRect(
                       BOOLEAN      is_update,          //in:
                       GUI_RECT_T   icon_rect,          //in:
                       GUI_RECT_T   iconlist_dis_rect,  //in:
                       void         *ctrl_ptr           //in:
                       );

/*****************************************************************************/
//  Description : display scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar(
                            BOOLEAN             is_update,
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : is need progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedScroll(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePrgCtrl(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : destroy progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DestroyPrgCtrl(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : display selected flag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySelectedFlag(
                               uint16               index,
                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : is display selected flag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplaySelectedFlag(
                                    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : send msg to win, notify it append the icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SendMsgAppendIcon(
                             uint16                 index,              //in
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr  //in
                             );

/*****************************************************************************/
//  Description : notify load the next icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void NotifyLoadNextIcon(
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                              );

/*****************************************************************************/
//  Description : display icon list default icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDefaultIcon(
                              uint16                index,              //in
                              GUI_RECT_T            icon_rect,          //in
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                              );

/*****************************************************************************/
//  Description : display item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayItemText(
                           uint16               index,
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
// 	Description : text string is need scroll
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTextScroll(
                           GUI_RECT_T           text_rect,          //in:
                           MMI_STRING_T         *text_ptr,          //in:
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in:
                           );

/*****************************************************************************/
// 	Description : start text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartTextScrollTimer(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                );

/*****************************************************************************/
// 	Description : stop text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StopTextScrollTimer(
                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : set icon list sub title
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetIconListTitleIndex(
                                 CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : display icon list title
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayTitle(
                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                        );

/*****************************************************************************/
//  Description : display icon list softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySoftkey(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : handle text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextScrollTimer(
                                 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : handle icon list web key
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleIconListWebKey(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : select item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SelectItemOrNot(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : handle icon list direction key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIconListDirectKey(
                                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
                                           MMI_MESSAGE_ID_E     msg_id
                                           );

/*****************************************************************************/
//  Description : get next index
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetNextIndex(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,
                          MMI_MESSAGE_ID_E      msg_id
                          );

/*****************************************************************************/
//  Description : get the max top index,the icon must display all
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetMaxTopIndex(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : get the max top index,the icon may display part
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetPartMaxTopIndex(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : handle icon list star/hash key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIconListPageKey(
                                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
                                         MMI_MESSAGE_ID_E   msg_id
                                         );

/*****************************************************************************/
//  Description : get the top index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTopIndex(
                         BOOLEAN            is_prev_page,
                         uint16             index,
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                         );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle icon list tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListTpDown(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr, //in:
                                DPARAM              param               //in:
                                );

/*****************************************************************************/
//  Description : get tp icon index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpIndex(
                         uint16             *index_ptr,         //in/out:
                         GUI_POINT_T        *point_ptr,         //in:
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in:
                         );

/*****************************************************************************/
//  Description : is tp folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpFolder(
                         uint16             index,              //i:
                         GUI_POINT_T        *point_ptr,         //in:
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in:
                         );

/*****************************************************************************/
//  Description : start tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpSlideTimer(
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : stop tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopTpSlideTimer(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle icon list tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListTpMove(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr, //in:
                                DPARAM              param               //in:
                                );

/*****************************************************************************/
//  Description : handle icon list tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListTpUp(
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in:
                              DPARAM                param               //in:
                              );

/*****************************************************************************/
//  Description : handle icon list tp long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListTpLong(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr, //in:
                                DPARAM              param               //in:
                                );

/*****************************************************************************/
//  Description : handle icon list tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListSlideTimer(
                                    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : handle icon list slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListSlide(
                               GUI_POINT_T          tp_point,
                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : reset icon list slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetIconListSlide(
                              BOOLEAN               is_update,
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : handle icon list tp progress bar msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleIconListTpPrg(
                                  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in:
                                  MMI_MESSAGE_ID_E      msg_id,             //in:
                                  DPARAM                param               //in:
                                  );
#endif

/*****************************************************************************/
//  Description : set icon list relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRelativeRect(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : get icon list top and bottom index by relative display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL void GetTopBottomIndex(
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
                             );

/*******************************************************************************************/
//  Description : get cross rect by two rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL BOOLEAN GetCrossRect(
                           GUI_BIG_RECT_T   *rect_dest_ptr,
                           GUI_BIG_RECT_T   rect1,
                           GUI_BIG_RECT_T   rect2
                           );

/*****************************************************************************/
//  Description : insert icon and display icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertIcon(
                      uint16                index,              //in:
                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in
                      GUIANIM_DATA_INFO_T   *data_info_ptr,     //in,may PNULL
                      GUIANIM_FILE_INFO_T   *file_info_ptr      //in,may PNULL
                      );

/*****************************************************************************/
//  Description : insert icon and display icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertIconEx(
                      uint16                index,              //in:
                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in
                      GUIANIM_DATA_INFO_T   *data_info_ptr,     //in,may PNULL
                      GUIANIM_FILE_INFO_T   *file_info_ptr ,     //in,may PNULL
                      GUIANIM_DISPLAY_INFO_T * display_info
                      
                      );


/*****************************************************************************/
//  Description : icon anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void IconAnimCallback(
                            GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                            );

/*****************************************************************************/
//  Description : adjust relative display rect by current index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustDisplayRect(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get operate anim index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetOperateAnim(
                             uint16             *index_ptr,         //in/out
                             MMI_HANDLE_T       handle,             //in:
                             CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in:
                             );

/*****************************************************************************/
//  Description : display mark icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMarkIcon(
                           uint16               index,              //in:
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in:
                           );

#ifdef GUIF_ICONFOLDER
/*****************************************************************************/
//  Description : insert folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertFolder(
                        uint16              index,              //in:
                        uint16              icon_num,           //in:
                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                        );
#endif

/*****************************************************************************/
//  Description : rect1 is covered rect2
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL BOOLEAN IsRectCovered(
							CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
                            GUI_BIG_RECT_T      rect1,
                            GUI_BIG_RECT_T      rect2
                            );

/*****************************************************************************/
//  Description : remove one item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void RemoveOneItem(
                         uint16             index,
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                         );

/*****************************************************************************/
//  Description : 获取总列数，即一行元素的总个数
//  Global resource dependence :
//  Author: xiyuan 2011.12
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTotalColNum(
							CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
							);

/*****************************************************************************/
//  Description : iconlist横向滑动情况下，按#，*键翻页
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVSlidePageKey(
									   CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
									   MMI_MESSAGE_ID_E   msg_id
									   );

/*****************************************************************************/
//  Description : Calculate Item Text Rect
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.03
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T CalculateItemTextRect (
										CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
										uint16               index
										);

/*****************************************************************************/
//  Description: post mark item
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void PostMarkItemMsg(
                           CTRLICONLIST_OBJ_T* iconlist_ctrl_ptr
                           );
#ifdef GUIF_ICONLIST_STUNT
/*****************************************************************************/
//  Description : 保存folder到file的信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetFolder2FileImgInfo(
                                 CTRLICONLIST_OBJ_T  *folder_iconlist_ctrl_ptr,
                                 CTRLICONLIST_OBJ_T  *icon_iconlist_ctrl_ptr
                                 );

/*****************************************************************************/
// Description : store info for animation from file to folder
// Global resource dependence : 
// Author: hua.fang
// Note:
//  保存从文件到文件夹的动画信息，现在动画由文件夹iconlist来做，所以是将file里的
// 信息保存到folder里，这里只保存位置信息，因为folder里已经有图片和角度信息
/*****************************************************************************/
LOCAL void GetFile2FolderImgInfo(
                                 CTRLICONLIST_OBJ_T  *icon_iconlist_ctrl_ptr,
                                 CTRLICONLIST_OBJ_T  *folder_iconlist_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : 清除存储的旋转信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ClearRotateInfo(
                           CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                           );
#endif

#ifdef DYNAMIC_MODULE_SUPPORT    
/*****************************************************************************/
//  Description : 类型转化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_TYPE_E ConvertAnimTypeFromCAFToGUI(
                                                 CAF_ANIM_TYPE_E caf_type
                                                 );

//added by andrew.zhang at 2009/03/16
LOCAL BOOLEAN IconListCtrlSetTotalItem( IGUIICONLIST_T * me_ptr, uint16 total_num );
LOCAL BOOLEAN IconListCtrlSetCurIndex( IGUIICONLIST_T * me_ptr, uint16 selected_index );
LOCAL uint16 IconListCtrlGetCurIndex( IGUIICONLIST_T * me_ptr );
LOCAL BOOLEAN IconListCtrlAppendItem( IGUIICONLIST_T * me_ptr, uint16 icon_index, CAF_ICONLIST_ICON_DISP_T *icon_ptr );
LOCAL BOOLEAN IconListCtrlAppendIcon( IGUIICONLIST_T * me_ptr, uint16 icon_index, void * data_ptr,  CAF_RESOURCE_SOURCE_E resource_source );
#endif

#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
//  Description : 获取某位置在哪个group中
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUIICONLIST_GROUP_INFO_T *SearchPosOfGroup(
												 CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr,
												 int32                  position
												 );
/*****************************************************************************/
//  Description : 获取分隔符的坐标信息
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDelimiterRect(
								   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
								   MMI_LINK_NODE_T      *group_ptr
								   );

/*****************************************************************************/
//  Description : 计算Delimiter相对于group的坐标
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetDelimiterToGroupRect(
											 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
											 MMI_LINK_NODE_T     *group_ptr
											 );
/*****************************************************************************/
//  Description : 显示分隔符
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL void DisplayDelimiter(
                            BOOLEAN             is_update,
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : 根据item index找到它在iconlist的第几行中
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:   
/*****************************************************************************/
LOCAL uint16 SearchRowOfItemIndex(
								  CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
								  uint16 item_index
								  );

/*****************************************************************************/
//  Description : remove one group node without redisplay
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.03
//  Note: 
/*****************************************************************************/
LOCAL void RemoveOneDelimiter(
                              CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr, 
                              uint16             index
                              );

/*****************************************************************************/
//  Description : iconlist有分隔符的情况下，按#，*键翻页
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelimiterPageKey(
                                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
                                         MMI_MESSAGE_ID_E   msg_id
                                         );

/*****************************************************************************/
//  Description : 显示分隔符背景
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconListDelimiterBg(
									  CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
									  GUI_RECT_T         delimiter_bg_rect,
									  GUI_RECT_T         delimiter_rect
									  );

/*****************************************************************************/
//  Description : 找到指定位置内的第一个或者最后一个item index
//  Global resource dependence : 
//  Author: xiyuan edit 2011.12
//  Note:   
/*****************************************************************************/
LOCAL uint16 GetItemIndexInPosRect(
								   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
								   BOOLEAN             is_under_pos_first_index,
                                   int32               pos
								   );

/*****************************************************************************/
// 	Description : 销毁group链表
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
LOCAL void DestroyIconListGroupLink(
									CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
									);

/*****************************************************************************/
//  Description : UpdateGroupDisRect
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.09
//  Note:
/*****************************************************************************/
LOCAL void UpdateGroupDisRect(
                              CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                              );
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get iconlist type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T ICONLIST_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "iconlist",                                         // control name
                       sizeof (CTRLICONLIST_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) IconlistCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) IconlistCtrlDestruct,      // control destructor function
                       sizeof (CTRLICONLIST_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) IconlistCtrlPackInitParam,     // convert init param
                       sizeof (CTRLICONLIST_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) IconlistCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init iconlist class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlInitVtbl (
    CTRLICONLIST_VTBL_T        *iconlist_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) iconlist_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = IconlistCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect	   = IconlistCtrlSetRect; 
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
LOCAL void IconlistCtrlPackInitParam (
    GUIICONLIST_INIT_DATA_T     *iconlist_init_ptr,   //in
    CTRLICONLIST_INIT_PARAM_T   *iconlist_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) iconlist_param_ptr;

    if (PNULL == iconlist_init_ptr || PNULL == iconlist_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = iconlist_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : iconlist construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLICONLIST_INIT_PARAM_T   *iconlist_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == iconlist_param_ptr)
    {
        return FALSE;
    }

    //get icon list theme
    MMITHEME_GetIconlistTheme(&iconlist_ctrl_ptr->theme);
    base_ctrl_ptr->lcd_dev_info = iconlist_ctrl_ptr->theme.lcd_dev;

    //set icon list other info
    iconlist_ctrl_ptr->win_id       = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);

    //set style
    iconlist_ctrl_ptr->style 	     = GUIICONLIST_STYLE_ICON;
    iconlist_ctrl_ptr->is_dynamic_append_mode 	= FALSE;
    iconlist_ctrl_ptr->icon_cache_num   		= 0;
    iconlist_ctrl_ptr->is_display_title 			= TRUE;

    //init display rect
    InitDisplayRect(iconlist_ctrl_ptr);

    //get row and column number
    GetRowColNum(iconlist_ctrl_ptr);

    iconlist_ctrl_ptr->layout_style = GUIICONLIST_LAYOUT_V;

#ifdef GUIF_ICONLIST_DELIMITER
    iconlist_ctrl_ptr->is_group_rect_dirty = TRUE;
#endif

    return (result);
}

/*****************************************************************************/
//  Description : iconlist destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T*) base_ctrl_ptr;

    if (PNULL == iconlist_ctrl_ptr)
    {
        return FALSE;
    }

#ifdef TOUCH_PANEL_SUPPORT
    //stop slide timer
    StopTpSlideTimer(iconlist_ctrl_ptr);
#endif    
    //stop text timer
    StopTextScrollTimer(iconlist_ctrl_ptr);
    
    //free item memory
    FreeItemMemory(iconlist_ctrl_ptr);
    
    //free prgbox control memory
    if (PNULL != iconlist_ctrl_ptr->prgbox_ctrl_ptr)
    {
        iconlist_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
    }
    
#ifdef GUIF_ICONLIST_STUNT
    ClearRotateInfo(iconlist_ctrl_ptr);
#endif
    
    //有分隔符的情况下，销毁node
#ifdef GUIF_ICONLIST_DELIMITER
    DestroyIconListGroupLink(iconlist_ctrl_ptr);
#endif
    return (result); 
}

/*****************************************************************************/
//  Description : handle iconlist msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IconlistCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == iconlist_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
#ifdef GUIF_ICONLIST_STUNT
        if (GUIICONLIST_SPECIAL_IMAGE_TO_FILE == iconlist_ctrl_ptr->specail_type)
        {
            //display icon list bg
            DisplayIconListBg(base_ctrl_ptr->display_rect,iconlist_ctrl_ptr);

            ICONLIST_RunImg2File(iconlist_ctrl_ptr);
        }
        else if (GUIICONLIST_SPECIAL_FOLDER_TO_FILE == iconlist_ctrl_ptr->specail_type)
        {
            //display icon list bg
            DisplayIconListBg(base_ctrl_ptr->display_rect,iconlist_ctrl_ptr);

            ICONLIST_RunFolder2File(iconlist_ctrl_ptr);
        }
        else if (GUIICONLIST_SPECIAL_FILE_TO_FOLDER == iconlist_ctrl_ptr->specail_type)
        {
            //display icon list bg
            DisplayIconListBg(base_ctrl_ptr->display_rect,iconlist_ctrl_ptr);

            ICONLIST_RunFile2Folder(iconlist_ctrl_ptr);
        }

        iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_NONE;
#endif

        DisplayIconList(FALSE,iconlist_ctrl_ptr);
        break;

    case MSG_CTL_ANIM_DISPLAY_IND:
        //notify load next icon
        NotifyLoadNextIcon(iconlist_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        iconlist_ctrl_ptr->is_active = TRUE;
        break;

    case MSG_CTL_LOSE_ACTIVE:
        iconlist_ctrl_ptr->is_active = FALSE;
        iconlist_ctrl_ptr->is_tp_pressed = FALSE;
#ifdef TOUCH_PANEL_SUPPORT       
        //stop slide timer
        StopTpSlideTimer(iconlist_ctrl_ptr);
#endif
        //stop text timer
        StopTextScrollTimer(iconlist_ctrl_ptr);

        //reset slide state
        iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        break;

    case MSG_CTL_LOSE_FOCUS:
        iconlist_ctrl_ptr->is_tp_pressed = FALSE;
#ifdef TOUCH_PANEL_SUPPORT
        //stop slide timer
        StopTpSlideTimer(iconlist_ctrl_ptr);
#endif
        //stop text timer
        StopTextScrollTimer(iconlist_ctrl_ptr);

#ifdef GUIF_ICONLIST_STUNT
        if (GUIICONLIST_SPECIAL_FILE_TO_IMAGE == iconlist_ctrl_ptr->specail_type)
        {
            //display icon list bg
            DisplayIconListBg(base_ctrl_ptr->display_rect,iconlist_ctrl_ptr);

            ICONLIST_RunFile2Img(iconlist_ctrl_ptr);

            iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_NONE;
        }
#endif
#ifdef TOUCH_PANEL_SUPPORT
        //reset icon list slide
        ResetIconListSlide(FALSE,iconlist_ctrl_ptr);
#endif
        break;

    case MSG_NOTIFY_UPDATE:
        //display icon list bg
        DisplayIconListBg(*(GUI_RECT_T *)param,iconlist_ctrl_ptr);
        break;

    case MSG_TIMER:
        if (iconlist_ctrl_ptr->scroll_timer_id == *(uint8*)param)
        {
            HandleTextScrollTimer(iconlist_ctrl_ptr);
        }
#ifdef TOUCH_PANEL_SUPPORT
        else if (iconlist_ctrl_ptr->slide_timer_id == *(uint8*)param)
        {
            HandleIconListSlideTimer(iconlist_ctrl_ptr);
        }
#endif
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_APP_OK:
        if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
        {
            result = GUICTRL_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_OK);
        }
        break;

    case MSG_APP_WEB:
        HandleIconListWebKey(iconlist_ctrl_ptr);
        break;

    case MSG_APP_CANCEL:
        if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
        {
            result = GUICTRL_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_CANCEL);
        }
        break;

    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        result = HandleIconListDirectKey(iconlist_ctrl_ptr,MSG_APP_UP);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        result = HandleIconListDirectKey(iconlist_ctrl_ptr,MSG_APP_DOWN);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        result = HandleIconListDirectKey(iconlist_ctrl_ptr,MSG_APP_LEFT);
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleIconListDirectKey(iconlist_ctrl_ptr,MSG_APP_RIGHT);
        break;
        
    case MSG_APP_STAR:
    case MSG_KEYREPEAT_STAR:
#ifdef GUIF_ICONLIST_DELIMITER
		if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
		{
			result = HandleDelimiterPageKey(iconlist_ctrl_ptr, MSG_APP_STAR);
		}
		else
#endif
		{
			//纵向滑动
			if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
			{
				result = HandleIconListPageKey(iconlist_ctrl_ptr,MSG_APP_STAR);
			}
			//横向滑动
			else
			{
				result = HandleVSlidePageKey(iconlist_ctrl_ptr,MSG_APP_STAR);
			}
		}
        break;

    case MSG_APP_HASH:
    case MSG_KEYREPEAT_HASH:
#ifdef GUIF_ICONLIST_DELIMITER
		if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
		{
			result = HandleDelimiterPageKey(iconlist_ctrl_ptr, MSG_APP_HASH);
		}
		else
#endif
		{
			//纵向滑动
			if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
			{
				result = HandleIconListPageKey(iconlist_ctrl_ptr,MSG_APP_HASH);
			}
			//横向滑动
			else
			{
				result = HandleVSlidePageKey(iconlist_ctrl_ptr,MSG_APP_HASH);
			}
		}
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        HandleIconListTpDown(iconlist_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        HandleIconListTpMove(iconlist_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        HandleIconListTpUp(iconlist_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        HandleIconListTpLong(iconlist_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PRGBOX_MOVEUP_ONE:
    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
    case MSG_CTL_PRGBOX_MOVE:
        if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
        {
            HandleIconListTpPrg(iconlist_ctrl_ptr,msg_id,param);
        }
        break;
#endif 

    case MSG_LCD_SWITCH:
        iconlist_ctrl_ptr->is_group_rect_dirty = TRUE;
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_ICONLIST_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set icon list rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IconlistCtrlSetRect(
                              CTRLBASE_OBJ_T	*base_ctrl_ptr,
                              const CAF_RECT_T    *rect_ptr
                              )
{
    BOOLEAN             result = TRUE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    if ((PNULL == base_ctrl_ptr)||
		(PNULL == rect_ptr))
    {
    	return FALSE;
    }

    iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T *)base_ctrl_ptr;

    if (!GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr))
    {
        //set rect
        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;
        
        GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));

        //free current page item memory
        FreeCurPageItemMemory(iconlist_ctrl_ptr);

        //destroy progress
        DestroyPrgCtrl(iconlist_ctrl_ptr);
#ifdef TOUCH_PANEL_SUPPORT
        //stop slide timer
        StopTpSlideTimer(iconlist_ctrl_ptr);
#endif
        //stop text timer
        StopTextScrollTimer(iconlist_ctrl_ptr);
        iconlist_ctrl_ptr->str_index = 0;

        //reset slide state
        iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

        //init display rect
        InitDisplayRect(iconlist_ctrl_ptr);

        //get row and column number
        GetRowColNum(iconlist_ctrl_ptr);

        //set relative rect
        SetRelativeRect(iconlist_ctrl_ptr);

        //adjust relative display rect by current index
        AdjustDisplayRect(iconlist_ctrl_ptr);

        //get top and bottom index
        GetTopBottomIndex(iconlist_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : init icon list display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitDisplayRect(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
    GUI_RECT_T      iconlist_rect = {0};

	if (PNULL != iconlist_ctrl_ptr)
	{
		//get icon list rect
		iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);
		
		//set relative display rect
		iconlist_ctrl_ptr->relative_disp_rect.left   = 0;
		iconlist_ctrl_ptr->relative_disp_rect.top    = 0;
		iconlist_ctrl_ptr->relative_disp_rect.right  = iconlist_rect.right - iconlist_rect.left;
		iconlist_ctrl_ptr->relative_disp_rect.bottom = iconlist_rect.bottom - iconlist_rect.top;
	}
}

/*****************************************************************************/
//  Description : get row and column number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetRowColNum(
                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                        )
{
    uint16      display_width = 0;
    uint16      display_height = 0;
    uint16      item_width = 0;
    uint16      item_height = 0;
    GUI_RECT_T  content_rect = {0};

	if (PNULL != iconlist_ctrl_ptr)
	{		
		//get icon list content rect
		content_rect = ICONLIST_GetContRect(iconlist_ctrl_ptr);
		
		//get display width and height
		display_width  = (uint16)(content_rect.right - content_rect.left + 1);
		display_height = (uint16)(content_rect.bottom - content_rect.top + 1);
		
		//get column number
		item_width = ICONLIST_GetItemWidth(TRUE,iconlist_ctrl_ptr);
		iconlist_ctrl_ptr->col_num_page = (uint16)(display_width / item_width);
		if ((display_width % item_width) >= ICONLIST_GetItemWidth(FALSE,iconlist_ctrl_ptr))
		{
			iconlist_ctrl_ptr->col_num_page++;
		}
		
		//get row number
		item_height = ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);
		iconlist_ctrl_ptr->row_num_page = (uint16)(display_height / item_height);
		iconlist_ctrl_ptr->row_max_num_page = iconlist_ctrl_ptr->row_num_page;
		
		if (0 < (display_height % item_height))
		{
			iconlist_ctrl_ptr->row_max_num_page++;
			
			if ((display_height % item_height) >= ICONLIST_GetItemHeight(FALSE,iconlist_ctrl_ptr))
			{
				iconlist_ctrl_ptr->row_num_page++;
			}
		}
		
		iconlist_ctrl_ptr->icon_num_page = (uint16)(iconlist_ctrl_ptr->row_num_page*iconlist_ctrl_ptr->col_num_page);		
	}
}

/*****************************************************************************/
// 	Description : 销毁group链表
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
LOCAL void DestroyIconListGroupLink(
									CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
									)
{
    if (PNULL == iconlist_ctrl_ptr || PNULL == iconlist_ctrl_ptr->group_list_info.group_list_ptr)
    {
        return;
    }
    
	MMILINK_DestroyLink(iconlist_ctrl_ptr->group_list_info.group_list_ptr, ICONLIST_DestroyIconListGroupNode);
	
	iconlist_ctrl_ptr->group_list_info.group_list_ptr = PNULL;
	iconlist_ctrl_ptr->group_list_info.group_num      = 0;
    iconlist_ctrl_ptr->is_group_rect_dirty = TRUE;

	return;
}

/*****************************************************************************/
//  Description : alloc item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AllocItemMemory(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
	if (PNULL != iconlist_ctrl_ptr)
	{
		if (0 < iconlist_ctrl_ptr->total_num)
		{
			iconlist_ctrl_ptr->item_ptr = SCI_ALLOC_APP(iconlist_ctrl_ptr->total_num*sizeof(GUIICONLIST_ITEM_T));
			SCI_MEMSET(iconlist_ctrl_ptr->item_ptr,0,(iconlist_ctrl_ptr->total_num*sizeof(GUIICONLIST_ITEM_T)));
		}
	}
}

/*****************************************************************************/
//  Description : free item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeItemMemory(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          )
{
    uint16      i = 0;

    if ((PNULL != iconlist_ctrl_ptr)&&
		(PNULL != iconlist_ctrl_ptr->item_ptr))
    {
        for (i=0; i<iconlist_ctrl_ptr->total_num; i++)
        {
            //free one item
            FreeOneItemMemory(iconlist_ctrl_ptr,&iconlist_ctrl_ptr->item_ptr[i]);
        }
        
        SCI_FREE(iconlist_ctrl_ptr->item_ptr);
    }
}

/*****************************************************************************/
//  Description : free one item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeOneItemMemory(
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr, //in:
                             GUIICONLIST_ITEM_T     *item_ptr           //in:
                             )
{
	if ((PNULL != iconlist_ctrl_ptr)&&
		(PNULL != item_ptr))
	{		
		//free text
		if (PNULL != item_ptr->text.display.name_str.wstr_ptr)
		{
			SCI_FREE(item_ptr->text.display.name_str.wstr_ptr);
			item_ptr->text.display.name_str.wstr_len = 0;
			item_ptr->text.is_load = FALSE;
		}
		
		if (PNULL != item_ptr->text_addtion.display.name_str.wstr_ptr)
		{
			SCI_FREE(item_ptr->text_addtion.display.name_str.wstr_ptr);
			item_ptr->text_addtion.display.name_str.wstr_len = 0;
			item_ptr->text_addtion.is_load = FALSE;
		}

		//free icon
		if (0 != item_ptr->icon.ctrl_add)
		{
			//set not loading
			iconlist_ctrl_ptr->is_loading_icon = FALSE;
			
			MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T*)item_ptr->icon.ctrl_add));
			item_ptr->icon.ctrl_add = PNULL;
			item_ptr->icon.is_load = FALSE;
		}
	}
}

/*****************************************************************************/
//  Description : free current page item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeCurPageItemMemory(
                                 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                                 )
{
    uint16      i = 0;

    if ((PNULL != iconlist_ctrl_ptr)&&
		(PNULL != iconlist_ctrl_ptr->item_ptr))
    {
        for (i=iconlist_ctrl_ptr->top_index; i <= iconlist_ctrl_ptr->bottom_index; i++)
        {
            //free one item
            FreeOneItemMemory(iconlist_ctrl_ptr,&iconlist_ctrl_ptr->item_ptr[i]);
        }
    }
}

/*****************************************************************************/
//  Description : free not in current page item memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeNotCurPageItemMemory(
                                    uint16              pre_top_index,
                                    uint16              pre_bottom_index,
                                    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                    )
{
    uint16      i = 0;
    uint16      top_index = 0;
    uint16      bottom_index = 0;
    uint16      icon_max_num_page = 0;
    uint16      pre_cache_ahead   = 0;
    uint16      pre_cache_behind  = 0;
    uint16      cache_ahead       = 0;
    uint16      cache_behind      = 0;
    CTRLBASE_OBJ_T     *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

	if (pre_top_index >= iconlist_ctrl_ptr->total_num)
	{
		if (0 == iconlist_ctrl_ptr->total_num)
		{
			pre_top_index = 0;
		} 
		else
		{
			pre_top_index = iconlist_ctrl_ptr->total_num -1;
		}		
	}
	
	if (pre_bottom_index >= iconlist_ctrl_ptr->total_num)
	{
		if (0 == iconlist_ctrl_ptr->total_num)
		{
			pre_bottom_index = 0;
		} 
		else
		{
			pre_bottom_index = iconlist_ctrl_ptr->total_num -1;
		}
	}

    if ((PNULL != iconlist_ctrl_ptr)&&
		(PNULL != iconlist_ctrl_ptr->item_ptr))
    {
        //set current top/bottom index
        top_index    = iconlist_ctrl_ptr->top_index;
        bottom_index = iconlist_ctrl_ptr->bottom_index;

        //first page,往下移动不销毁当页的数据
        if (0 == iconlist_ctrl_ptr->top_index)
        {
#ifdef GUIF_ICONLIST_DELIMITER
            //有分隔符的情况
            if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
            {
                bottom_index = GetItemIndexInPosRect(iconlist_ctrl_ptr, FALSE, iconlist_ctrl_ptr->relative_disp_rect.bottom);
                //防个数越界判断
                if (iconlist_ctrl_ptr->bottom_index >= iconlist_ctrl_ptr->total_num)
                {
                    iconlist_ctrl_ptr->bottom_index = iconlist_ctrl_ptr->total_num - 1;
                }
            }
            else
#endif    
            {
                icon_max_num_page = (uint16)(iconlist_ctrl_ptr->row_max_num_page*iconlist_ctrl_ptr->col_num_page);
                if (0 < icon_max_num_page)
                {
                    bottom_index = (uint16)(icon_max_num_page - 1);
                }
            }
        }

        //last page,往上移动不销毁当页的数据
        if (iconlist_ctrl_ptr->bottom_index >= iconlist_ctrl_ptr->total_num)
        {
            //get max top index
            top_index = GetPartMaxTopIndex(iconlist_ctrl_ptr);
        }


        if (0 == iconlist_ctrl_ptr->icon_cache_num)
        {
            for (i=pre_top_index; i<=pre_bottom_index; i++)
            {                
                if ((i < iconlist_ctrl_ptr->total_num) &&
                        ((i < top_index) || (i > bottom_index)))
                {
                    //free one item
                    FreeOneItemMemory(iconlist_ctrl_ptr, &iconlist_ctrl_ptr->item_ptr[i]);
                }
            }
        }
        else            
        {
            pre_cache_ahead     = (uint16)(pre_top_index - (uint16)MIN(pre_top_index, iconlist_ctrl_ptr->icon_cache_num));            
            pre_cache_behind    = (uint16)(MIN((pre_bottom_index + iconlist_ctrl_ptr->icon_cache_num), (iconlist_ctrl_ptr->total_num-1)));
            
            cache_ahead         = (uint16)(top_index - (uint16)MIN(top_index, iconlist_ctrl_ptr->icon_cache_num));            
            cache_behind        = (uint16)(MIN((bottom_index + iconlist_ctrl_ptr->icon_cache_num), (iconlist_ctrl_ptr->total_num-1)));
            
            for (i=pre_cache_ahead; i<=pre_cache_behind; i++)
            {                
                if ((i < cache_ahead) || (i > cache_behind))
                {
                    //free one item
                    FreeOneItemMemory(iconlist_ctrl_ptr, &iconlist_ctrl_ptr->item_ptr[i]);
                }
                else
                {
                    if (((i>= cache_ahead) && (i < top_index))
                        || ((i <= cache_behind) && (i > bottom_index))
                        )
                    {
                        if (PNULL != iconlist_ctrl_ptr->item_ptr[i].icon.ctrl_add)
                        {
                            GUI_RECT_T      rect        = ICONLIST_GetIconRect(i,iconlist_ctrl_ptr);
							SetCtrlRect(FALSE,
								        rect,
								        base_ctrl_ptr->display_rect,
								        (void *)iconlist_ctrl_ptr->item_ptr[i].icon.ctrl_add);
                        }
                    }                   
                }
            }            
        }
    }
}

/*****************************************************************************/
//  Description : display icon list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconList(
                           BOOLEAN              is_update_prgbox,
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
	CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;
	
	if (PNULL != iconlist_ctrl_ptr)
	{
		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
#ifndef WIN32
		CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif
		
		//display icon list bg
		DisplayIconListBg(base_ctrl_ptr->display_rect,iconlist_ctrl_ptr);
		
		//display item
		DisplayAllItem(is_update_prgbox,iconlist_ctrl_ptr);
		
#ifndef GUIF_ICONLIST_PDASTYLE
		//set sub title
		if (iconlist_ctrl_ptr->is_display_title)
		{
		  SetIconListTitleIndex(iconlist_ctrl_ptr);
		}
#endif
		
		if ((0 < iconlist_ctrl_ptr->total_num) &&
			(PNULL != iconlist_ctrl_ptr->item_ptr) &&
			(iconlist_ctrl_ptr->cur_index < iconlist_ctrl_ptr->total_num))
		{
			//if (iconlist_ctrl_ptr->is_display_title)
			{	
				//display title
				DisplayTitle(iconlist_ctrl_ptr);
			}			
			
			//display softkey
			DisplaySoftkey(iconlist_ctrl_ptr);
		}
		
#ifndef WIN32
		CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif
	}
}

/*****************************************************************************/
//  Description : display icon list bg image or color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconListBg(
                             GUI_RECT_T         bg_rect,
                             CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                             )
{
    	GUI_RECT_T          img_rect = {0};
    	GUI_BG_DISPLAY_T    bg_display = {0};
    	CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;
	
	if (PNULL != iconlist_ctrl_ptr)
	{
		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
		
		//set image rect
		img_rect.left   = (int16)(bg_rect.left - base_ctrl_ptr->rect.left);
		img_rect.top    = (int16)(bg_rect.top - base_ctrl_ptr->rect.top);
		img_rect.right  = (int16)(img_rect.left + bg_rect.right - bg_rect.left);
		img_rect.bottom = (int16)(img_rect.top + bg_rect.bottom - bg_rect.top);
		
		//set bg display info
		bg_display.is_transparent = FALSE;
		bg_display.rect           = base_ctrl_ptr->rect;
		bg_display.display_rect   = bg_rect;
		bg_display.img_rect       = img_rect;
		bg_display.ctrl_handle    = base_ctrl_ptr->handle;
		bg_display.win_handle     = iconlist_ctrl_ptr->win_id;
		
		//display bg
		GUI_DisplayBg(&iconlist_ctrl_ptr->theme.bg,
			&bg_display,
			&base_ctrl_ptr->lcd_dev_info);
	}
}

/*****************************************************************************/
//  Description : display all item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL void DisplayAllItem(
                          BOOLEAN               is_update_prgbox,
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          )
{
    uint16              i = 0;
    uint16              top_index = 0;
    uint16              bottom_index = 0;
    MMI_STRING_T        string = {0};
    GUISTR_STYLE_T      style = {0};/*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR;
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;	

    if (PNULL != iconlist_ctrl_ptr)
    {
    	 base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

        if (0 == iconlist_ctrl_ptr->total_num)
        {
            //get string
            MMITHEME_GetResText(iconlist_ctrl_ptr->theme.empty_text_id,iconlist_ctrl_ptr->win_id,&string);

            //set style
            style.align      = ALIGN_HVMIDDLE;
            style.font       = iconlist_ctrl_ptr->theme.font.font;
            style.font_color = iconlist_ctrl_ptr->theme.font.color;

            GUISTR_DrawTextToLCDInRect(&base_ctrl_ptr->lcd_dev_info,
                &base_ctrl_ptr->rect,
                &base_ctrl_ptr->rect,
                &string, 
                &style, 
                state,
                GUISTR_TEXT_DIR_AUTO);
        }
        else
        {
			//纵向滑动
            if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr) && iconlist_ctrl_ptr->theme.groove_width > 0)
			{
				//display scroll bar
				DisplayScrollBar(is_update_prgbox,iconlist_ctrl_ptr);
			}

        //calculate iconlist rect
#ifdef GUIF_ICONLIST_DELIMITER
    		if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
    		{
                //只有在group&item的数量发生变化时，才重新计算一次
                SetRelativeRect(iconlist_ctrl_ptr);
                //但是每次发生移动后，都要更新一下group的dis_rect
                UpdateGroupDisRect(iconlist_ctrl_ptr);
    		}
#endif

			//set top and bottom index
			if (GUIICONLIST_LOAD_ALL == iconlist_ctrl_ptr->load_type)
			{
				top_index = 0;
				bottom_index = (uint16)(iconlist_ctrl_ptr->total_num - 1);
			}
			else
			{   
                //adjust relative display rect by current index
                if (FLING_RUN_MIN_VELOCITY > abs((int32)iconlist_ctrl_ptr->fling_velocity))
                {
                  AdjustDisplayRect(iconlist_ctrl_ptr);
                }
                //update top & bottom index
                GetTopBottomIndex(iconlist_ctrl_ptr);

				top_index    = iconlist_ctrl_ptr->top_index;
				bottom_index = iconlist_ctrl_ptr->bottom_index;

				if (bottom_index >= iconlist_ctrl_ptr->total_num)
				{
					bottom_index = (uint16)(iconlist_ctrl_ptr->total_num - 1);
				}
			}
			
			//display all icon
			for (i=top_index; i<=bottom_index; i++)
			{
				//display icon
				DisplayItemIcon(is_update_prgbox,i,iconlist_ctrl_ptr);
				
				//display item text
				DisplayItemText(i,iconlist_ctrl_ptr);
			}

#ifdef GUIF_ICONLIST_DELIMITER
			if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
			{
				DisplayDelimiter(is_update_prgbox,iconlist_ctrl_ptr);
			}
#endif
        }
    }
}

/*****************************************************************************/
//  Description : display item icon or folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayItemIcon(
                           BOOLEAN              is_update,
                           uint16               index,
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
    BOOLEAN         is_append = FALSE;
    GUI_RECT_T      icon_rect = {0};
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;	

	if (PNULL != iconlist_ctrl_ptr)
	{
		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
		
		if (index >= iconlist_ctrl_ptr->total_num)
		{
			return;
		}

		//get icon display rect
		icon_rect = ICONLIST_GetIconRect(index,iconlist_ctrl_ptr);
		
		if (iconlist_ctrl_ptr->item_ptr[index].icon.is_load)
		{
			//set ctrl rect
			SetCtrlRect(is_update,
				icon_rect,
				base_ctrl_ptr->display_rect,
				(void *)iconlist_ctrl_ptr->item_ptr[index].icon.ctrl_add);
		}
		else
		{
			//is append
			if ((MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)&&
				(MMK_IsFocusWin(iconlist_ctrl_ptr->win_id)))
			{
				is_append = TRUE;
				
				if ((GUIICONLIST_LOAD_ONE_BY_ONE == iconlist_ctrl_ptr->load_type) &&
					(iconlist_ctrl_ptr->is_loading_icon))
				{
					is_append = FALSE;
				}
			}
			
			if (is_append)
			{
				//append icon
				SendMsgAppendIcon(index,iconlist_ctrl_ptr);
			}
			else
			{
				//display default icon
				DisplayDefaultIcon(index,icon_rect,iconlist_ctrl_ptr);
			}
		}
		
		//display selected flag
		DisplaySelectedFlag(index,iconlist_ctrl_ptr);
	}
}

/*****************************************************************************/
//  Description : set icon control rect and display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlRect(
                       BOOLEAN      is_update,          //in:
                       GUI_RECT_T   icon_rect,          //in:
                       GUI_RECT_T   iconlist_dis_rect,  //in:
                       void         *ctrl_ptr           //in:
                       )
{
    GUI_RECT_T  display_rect = {0};

    if (PNULL != ctrl_ptr)
    {
        //set control rect
        IGUICTRL_SetRect(ctrl_ptr,&icon_rect);

        //get display rect
        if (GUI_IntersectRect(&display_rect,icon_rect,iconlist_dis_rect))
        {
            //set display rect
            IGUICTRL_SetDisplayRect(ctrl_ptr,
                &display_rect,
                is_update);
        }
        else
        {
            //set display rect
            IGUICTRL_SetDisplayRect(ctrl_ptr,
                &display_rect,
                FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : display scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar(
                            BOOLEAN             is_update,
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            )
{
    int32           display_max_top = 0;
    uint32          item_index = 0;
    GUI_RECT_T      prg_rect = {0};

    if ((PNULL != iconlist_ctrl_ptr)&&
		(IsNeedScroll(iconlist_ctrl_ptr)))
    {
        //create scroll
        if (CreatePrgCtrl(iconlist_ctrl_ptr))
        {
            is_update = TRUE;
        }

        prg_rect = ICONLIST_GetScrollBarRect(iconlist_ctrl_ptr);

        GUIPRGBOX_SetParamByPtr(iconlist_ctrl_ptr->prgbox_ctrl_ptr,
            ICONLIST_GetRelativeHeight(iconlist_ctrl_ptr),
            (prg_rect.bottom - prg_rect.top + 1));

        //get display max top
        display_max_top = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);

        //get current item index
        if (0 > iconlist_ctrl_ptr->relative_disp_rect.top)
        {
            item_index = 0;
        }
        else if (iconlist_ctrl_ptr->relative_disp_rect.top > display_max_top)
        {
            item_index = display_max_top;
        }
        else
        {
            item_index = iconlist_ctrl_ptr->relative_disp_rect.top;
        }

        //set current position
        GUIPRGBOX_SetPosByPtr(is_update,
            item_index,
            item_index,
            iconlist_ctrl_ptr->prgbox_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : is need progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedScroll(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
    BOOLEAN     result = FALSE;
    uint16      display_height = 0;
    uint32      relative_height = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

    //get icon list relative hight
    relative_height = ICONLIST_GetRelativeHeight(iconlist_ctrl_ptr);

    //get display height
    display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);

    //is need progress
    if (relative_height > display_height)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePrgCtrl(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            )
{
    BOOLEAN                 result = FALSE;
    GUI_RECT_T              scroll_bar_rect = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUIPRGBOX_INIT_DATA_T   init_data = {0};
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;	

    if ((PNULL != iconlist_ctrl_ptr)&&
		(PNULL == iconlist_ctrl_ptr->prgbox_ctrl_ptr))
    {
    	 base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
        //set scroll bar rect
        scroll_bar_rect = ICONLIST_GetScrollBarRect(iconlist_ctrl_ptr);
        
        init_data.lcd_id = base_ctrl_ptr->lcd_dev_info.lcd_id;
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = scroll_bar_rect;
        init_data.style = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
        
        create.ctrl_id = 0;
        create.guid    = SPRD_GUI_PRGBOX_ID;
        create.parent_win_handle = iconlist_ctrl_ptr->win_id;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr = &init_data;
        
        iconlist_ctrl_ptr->prgbox_ctrl_ptr = MMK_CreateControl(&create);

        if (iconlist_ctrl_ptr->theme.is_support_slide)
        {
            GUIPRGBOX_SetNeedBg(iconlist_ctrl_ptr->prgbox_ctrl_ptr,FALSE);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : destroy progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DestroyPrgCtrl(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          )
{
    if ((PNULL != iconlist_ctrl_ptr)&&
		(PNULL != iconlist_ctrl_ptr->prgbox_ctrl_ptr))
    {
        //destroy progress bar control
        MMK_DestroyControl(GUICTRL_GetCtrlHandle(iconlist_ctrl_ptr->prgbox_ctrl_ptr));
        iconlist_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : display selected flag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplaySelectedFlag(
                               uint16               index,
                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                               )
{
    GUI_RECT_T          icon_rect = {0};
    GUI_RECT_T          select_rect = {0};
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

    if ((PNULL != iconlist_ctrl_ptr)&&
		(index == iconlist_ctrl_ptr->cur_index))
    {
        if (index >= iconlist_ctrl_ptr->total_num)
        {
			return;
        }
	 	base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
	 
        //get icon display rect
        icon_rect = ICONLIST_GetIconRect(index,iconlist_ctrl_ptr);

        if (IsDisplaySelectedFlag(iconlist_ctrl_ptr))
        {
            //set the rect larger
            select_rect.left   = (int16)(icon_rect.left - SELECT_FLAG_WIDTH);
            select_rect.top    = (int16)(icon_rect.top - SELECT_FLAG_WIDTH);
            select_rect.right  = (int16)(icon_rect.right + SELECT_FLAG_WIDTH);
            select_rect.bottom = (int16)(icon_rect.bottom + SELECT_FLAG_WIDTH);

            GUI_DisplayBorder(select_rect,//icon_rect,
                base_ctrl_ptr->display_rect,
                &iconlist_ctrl_ptr->theme.select_border,
                &base_ctrl_ptr->lcd_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : is display selected flag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplaySelectedFlag(
                                    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                    )
{
    BOOLEAN     result = FALSE;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

#ifdef GUIF_ICONLIST_PDASTYLE
	if ((iconlist_ctrl_ptr->is_tp_pressed) || 
		(MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()))
	{
		result = TRUE;
	}
#else
	result = TRUE;
#endif

    return (result);
}

/*****************************************************************************/
//  Description : send msg to win, notify it append the icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SendMsgAppendIcon(
                             uint16                 index,              //in
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr  //in
                             )
{
	CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL != iconlist_ctrl_ptr)
	{
		if (index >= iconlist_ctrl_ptr->total_num)
        	{
			return;
        	}
		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
		//append the current icon
		if (!iconlist_ctrl_ptr->item_ptr[index].icon.is_load)
		{
			switch (iconlist_ctrl_ptr->style)
			{
			case GUIICONLIST_STYLE_ICON:
			case GUIICONLIST_STYLE_ICON_UIDT:
				//set is loading
				iconlist_ctrl_ptr->is_loading_icon = TRUE;
				
				//notify to append the item icon
				GUICTRL_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_ICONLIST_APPEND_ICON,&index);
				break;
				
			case GUIICONLIST_STYLE_FOLDER_UIDT:
				//notify to folder include icon number
				GUICTRL_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_ICONLIST_ICON_NUM,&index);
				break;
				
			default:
				break;
			}
		}
	}
}

/*****************************************************************************/
//  Description : notify load the next icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void NotifyLoadNextIcon(
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                              )
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;

	if (PNULL != iconlist_ctrl_ptr)
	{
		if ((GUIICONLIST_LOAD_ONE_BY_ONE == iconlist_ctrl_ptr->load_type) &&
			(iconlist_ctrl_ptr->is_loading_icon))
		{
			//set not loading
			iconlist_ctrl_ptr->is_loading_icon = FALSE;
			
			//is load the next icon
			for (i=iconlist_ctrl_ptr->top_index; i<=iconlist_ctrl_ptr->bottom_index; i++)
			{
				if (!iconlist_ctrl_ptr->item_ptr[i].icon.is_load)
				{
					result = TRUE;
					break;
				}
			}
			
			//need load
			if ((result) &&
				(MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state) &&
				(MMK_IsFocusWin(iconlist_ctrl_ptr->win_id)))
			{
				SendMsgAppendIcon(i,iconlist_ctrl_ptr);
			}
		}
	}
}
/*****************************************************************************//*!
@brief  设置iconlist前后解码缓冲页数
@author kevin.lou
@param  ctrl_id                 [in] 控件  id
@param  num                     [in] 页数，前后个缓冲num页
@note   
*****************************************************************************/
PUBLIC void GUIICONLIST_SetIconCacheNum(
										MMI_HANDLE_T        ctrl_id,
										uint16  num
										)
{
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        iconlist_ctrl_ptr->icon_cache_num = num * iconlist_ctrl_ptr->col_num_page * iconlist_ctrl_ptr->row_num_page;
    }
}
/*****************************************************************************/
//  Description : display icon list default icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDefaultIcon(
                              uint16                index,              //in
                              GUI_RECT_T            icon_rect,          //in
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                              )
{
    int16           display_x = 0;
    int16           display_y = 0;
    uint16          default_width = 0;
    uint16          default_height = 0;
    GUI_RECT_T      lcd_rect = {0};
    GUI_RECT_T      img_rect = {0};
    GUI_RECT_T      image_display_rect = {0};
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL != iconlist_ctrl_ptr)
	{
		if (index >= iconlist_ctrl_ptr->total_num)
        	{
			return;
        	}
		
		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

		if (GUIRES_GetImgWidthHeight(&default_width,&default_height,iconlist_ctrl_ptr->theme.default_icon_id,iconlist_ctrl_ptr->win_id))
		{
			//set init display x and y and adjust display position
			display_x = icon_rect.left;
			display_y = icon_rect.top;
			GUIANIM_AdjustDisplayPosition(&display_x,
				&display_y,
				default_width,
				default_height,
				icon_rect,
				GUIANIM_ALIGN_HVMIDDLE);
			
			//set image display rect
			image_display_rect.left   = (int16)MAX(display_x,icon_rect.left);
			image_display_rect.top    = (int16)MAX(display_y,icon_rect.top);
			image_display_rect.right  = (int16)MIN((image_display_rect.left + (int16)default_width - 1),icon_rect.right);
			image_display_rect.bottom = (int16)MIN((image_display_rect.top + (int16)default_height - 1),icon_rect.bottom);
			
			//get crossed rect is lcd rect
			if (GUI_IntersectRect(&lcd_rect,base_ctrl_ptr->display_rect,image_display_rect))
			{
				//image rect
				img_rect.left   = (int16)(lcd_rect.left - display_x);
				img_rect.top    = (int16)(lcd_rect.top - display_y);
				img_rect.right  = (int16)(lcd_rect.right - display_x);
				img_rect.bottom = (int16)(lcd_rect.bottom - display_y);
				
				IMG_EnableTransparentColor(TRUE);
				//display default image icon
				GUIRES_DisplayImg(PNULL,
					&lcd_rect,
					&img_rect,
					iconlist_ctrl_ptr->win_id,
					iconlist_ctrl_ptr->theme.default_icon_id,
					&base_ctrl_ptr->lcd_dev_info);
				IMG_EnableTransparentColor(FALSE);
			}
			
			//display mark icon
			DisplayMarkIcon(index,iconlist_ctrl_ptr);
		}
	}
}

/*****************************************************************************/
//  Description : display item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayItemText(
                           uint16               index,
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
    BOOLEAN             result     		   = FALSE;
    BOOLEAN             is_scroll  		   = FALSE;
    uint16              str_index  		   = 0;
    GUI_RECT_T          text_rect  		   = {0};
    GUI_RECT_T          cross_rect 		   = {0};
    MMI_STRING_T        text_str   		   = {0};
    GUISTR_STYLE_T      str_style  		   = {0};
    GUISTR_STATE_T      str_state  		   = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;
    GUIICONLIST_TEXT_T  *text_ptr  		   = PNULL;
	
	GUI_RECT_T          fir_text_rect      = {0};
	GUI_RECT_T          sec_text_rect      = {0};
	GUIICONLIST_TEXT_T  *text_addtion_ptr  = PNULL;
	BOOLEAN             is_two_str         = FALSE;
	MMI_STRING_T        str_info           = {0};
	uint16              text_width         = 0;
	uint16              text_addtion_width = 0;
    MMI_STRING_T        text_addtion_str   = {0};
	MMI_STRING_T        cpy_str            = {0};
	CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL != iconlist_ctrl_ptr)
	{
        if (index >= iconlist_ctrl_ptr->total_num)
        {
			return;
        }

		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
		
		//is need display text
		switch (iconlist_ctrl_ptr->style)
		{
		case GUIICONLIST_STYLE_ICON_UIDT:
		case GUIICONLIST_STYLE_FOLDER_UIDT:
			result = TRUE;
			break;
			
		default:
			result = FALSE;
			break;
		}
		
		if (result)
		{
			//get text
			text_ptr            = &iconlist_ctrl_ptr->item_ptr[index].text;
			text_addtion_ptr    = &iconlist_ctrl_ptr->item_ptr[index].text_addtion;
			
			//get current icon title and softkey info
			if (!text_ptr->is_load)
			{
				GUICTRL_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_ICONLIST_APPEND_TEXT,&index);
			}
			
			if (0 < text_ptr->display.name_str.wstr_len)
			{
				//get text rect
				text_rect = ICONLIST_GetTextRect(index,iconlist_ctrl_ptr);
				
				//get crossed rect
				if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,text_rect))
				{
					//set string style
					str_style.font       = iconlist_ctrl_ptr->theme.font.font;
					str_style.font_color = iconlist_ctrl_ptr->theme.font.color;
					str_style.align      = ALIGN_HVMIDDLE;

                    					
					if (PNULL != iconlist_ctrl_ptr->item_ptr[index].text_addtion.display.name_str.wstr_ptr)
					{
						text_rect               = ICONLIST_GetTextRect(index,iconlist_ctrl_ptr);
						
						str_info                = iconlist_ctrl_ptr->item_ptr[index].text.display.name_str;
						text_width              = GUISTR_GetStringWidth(&str_style, &str_info, str_state);
						str_info                = iconlist_ctrl_ptr->item_ptr[index].text_addtion.display.name_str;
						text_addtion_width      = GUISTR_GetStringWidth(&str_style, &str_info, str_state);
						
						if ((text_width + text_addtion_width) >= (text_rect.right - text_rect.left))
						{
							sec_text_rect       = CalculateItemTextRect (iconlist_ctrl_ptr, index);
							fir_text_rect       = text_rect;
							fir_text_rect.right = sec_text_rect.left - 1;
							
							is_two_str = TRUE;
						}
					}
					//get string index and is need scroll
					if (index == iconlist_ctrl_ptr->cur_index)
					{   
                      if (PNULL != iconlist_ctrl_ptr->item_ptr[index].text_addtion.display.name_str.wstr_ptr)
					  {
					    is_scroll = IsTextScroll(fir_text_rect,&text_ptr->display.name_str,iconlist_ctrl_ptr);
					  }
                      else
                      {
                        is_scroll = IsTextScroll(text_rect,&text_ptr->display.name_str,iconlist_ctrl_ptr);
                      }
                      str_index = iconlist_ctrl_ptr->str_index;
					}
					
					//set state
					if (is_scroll)
					{
						if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
						{
							//start string scroll timer
							StartTextScrollTimer(iconlist_ctrl_ptr);
						}
					}
					else
					{
						str_state = str_state|GUISTR_STATE_ELLIPSIS;
					}
					
					//display bg
					DisplayIconListBg(cross_rect,iconlist_ctrl_ptr);
										
					//display string
					if ((is_scroll) &&
						(MMITHEME_GetIsScrollAsPixel()))
					{
						//set string
						text_str.wstr_ptr = text_ptr->display.name_str.wstr_ptr;
						text_str.wstr_len = text_ptr->display.name_str.wstr_len;
						
						GUISTR_DrawTextToLCDByOffset((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
							(const GUI_RECT_T*)&cross_rect,
							PNULL,
							str_index,
							0,
							&text_str,
							&str_style,
							str_state);
					}
					else
					{
						if ((PNULL != iconlist_ctrl_ptr->item_ptr[index].text_addtion.display.name_str.wstr_ptr)&&
							(TRUE == is_two_str))
						{
							//draw first str
							text_str.wstr_ptr = text_ptr->display.name_str.wstr_ptr + str_index;
							text_str.wstr_len = (uint16)(text_ptr->display.name_str.wstr_len - str_index);
							
							GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
								(const GUI_RECT_T*)&fir_text_rect,
								(const GUI_RECT_T*)&cross_rect,
								&text_str,
								&str_style,
								str_state,
								GUISTR_TEXT_DIR_AUTO);
							
							//draw Sec str
							text_str.wstr_ptr = text_addtion_ptr->display.name_str.wstr_ptr;
							text_str.wstr_len = (uint16)(text_addtion_ptr->display.name_str.wstr_len - str_index);
							
							GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
								(const GUI_RECT_T*)&sec_text_rect,
								(const GUI_RECT_T*)&cross_rect,
								&text_str,
								&str_style,
								str_state,
								GUISTR_TEXT_DIR_AUTO);
						}
						else if ((PNULL != iconlist_ctrl_ptr->item_ptr[index].text_addtion.display.name_str.wstr_ptr)&&
							(FALSE == is_two_str))
						{
							//first str
							text_str.wstr_ptr = text_ptr->display.name_str.wstr_ptr + str_index;
							text_str.wstr_len = (uint16)(text_ptr->display.name_str.wstr_len - str_index);
							
							//Sec str
							text_addtion_str.wstr_ptr = text_addtion_ptr->display.name_str.wstr_ptr;
							text_addtion_str.wstr_len = (uint16)(text_addtion_ptr->display.name_str.wstr_len - str_index);
							
							//alloc name memory
							cpy_str.wstr_ptr = SCI_ALLOC_APP((text_str.wstr_len + text_addtion_str.wstr_len + 1)*sizeof(wchar));
							SCI_MEMSET(cpy_str.wstr_ptr, 0, ((text_str.wstr_len + text_addtion_str.wstr_len + 1)*sizeof(wchar)));
							
							//copy name to be one str
							MMI_WSTRNCPY(cpy_str.wstr_ptr,
								(text_str.wstr_len + text_addtion_str.wstr_len + 1),
								text_str.wstr_ptr,
								text_str.wstr_len,
								text_str.wstr_len);
							
							MMI_WSTRNCPY(cpy_str.wstr_ptr + text_str.wstr_len,
								(text_str.wstr_len + text_addtion_str.wstr_len + 1),
								text_addtion_str.wstr_ptr,
								text_addtion_str.wstr_len,
								text_addtion_str.wstr_len);
							
							//copy name to be one str
							cpy_str.wstr_len = text_str.wstr_len + text_addtion_str.wstr_len;
							
							GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
								(const GUI_RECT_T*)&text_rect,
								(const GUI_RECT_T*)&cross_rect,
								&cpy_str,
								&str_style,
								str_state,
								GUISTR_TEXT_DIR_AUTO);
							
							SCI_FREE(cpy_str.wstr_ptr);
						}
						else
						{
							//set string
							text_str.wstr_ptr = text_ptr->display.name_str.wstr_ptr + str_index;
							text_str.wstr_len = (uint16)(text_ptr->display.name_str.wstr_len - str_index);
							
							GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
								(const GUI_RECT_T*)&text_rect,
								(const GUI_RECT_T*)&cross_rect,
								&text_str,
								&str_style,
								str_state,
								GUISTR_TEXT_DIR_AUTO);
						}
					}
            }
        }
    }
	}
}

/*****************************************************************************/
// 	Description : text string is need scroll
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTextScroll(
                           GUI_RECT_T           text_rect,          //in:
                           MMI_STRING_T         *text_ptr,          //in:
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in:
                           )
{
    BOOLEAN     result = FALSE;
#ifdef GUIF_ICONLIST_PDASTYLE
    GUI_INVALID_PARAM(text_rect);/*lint !e522*/
    GUI_INVALID_PARAM(text_ptr);/*lint !e522*/
    GUI_INVALID_PARAM(iconlist_ctrl_ptr);/*lint !e522*/
#else
    uint16      str_width = 0;
    uint16      display_width = 0;

	if ((PNULL == iconlist_ctrl_ptr)||(PNULL == text_ptr))
	{
		return FALSE;
	}

    //is active
    if (iconlist_ctrl_ptr->is_active)
    {
        //get string width
        str_width = GUI_CalculateStringPiexlNum(text_ptr->wstr_ptr,
                        text_ptr->wstr_len,
                        iconlist_ctrl_ptr->theme.font.font,
                        0);

        //get display width
        display_width = (uint16)(text_rect.right - text_rect.left + 1);

        if (str_width > display_width)
        {
            result = TRUE;
        }
    }
#endif

    return (result);
}

/*****************************************************************************/
// 	Description : start text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartTextScrollTimer(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                )
{
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

    if ((PNULL != iconlist_ctrl_ptr)&&
		(MMK_IsFocusWin(iconlist_ctrl_ptr->win_id)) && 
        	(0 == iconlist_ctrl_ptr->scroll_timer_id))
    {
    	 base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
        iconlist_ctrl_ptr->scroll_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
                                                iconlist_ctrl_ptr->theme.scroll_period,
                                                FALSE);
    }
}

/*****************************************************************************/
// 	Description : stop text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StopTextScrollTimer(
                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                               )
{
    if ((PNULL != iconlist_ctrl_ptr)&&
		(0 < iconlist_ctrl_ptr->scroll_timer_id))
    {
        MMK_StopTimer(iconlist_ctrl_ptr->scroll_timer_id);
        iconlist_ctrl_ptr->scroll_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : set icon list sub title
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetIconListTitleIndex(
                                 CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
                                 )
{
    uint16      cur_idx = 0;
    uint16      display_str_len = 0;
    uint8       display_str[10] = {0};
    wchar       display_wstr[10] = {0};
    int16       i = 0;

    if (PNULL != iconlist_ctrl_ptr)
    {

        if (0 < iconlist_ctrl_ptr->total_num)
        {
            cur_idx = (uint16)(iconlist_ctrl_ptr->cur_index + 1);

            // 格式化cur_num
            sprintf((char*)display_str,"%d", cur_idx);
            display_str_len = (uint16)strlen((char *)display_str); /*lint !e64*/

            for (i=0; i<display_str_len; i++)
            {
                *(display_wstr+i) = *(display_str+i);
            }

            GUIWIN_SetTitleSubText(iconlist_ctrl_ptr->win_id,
                TRUE,
                display_wstr,
                display_str_len,
                FALSE);
        }
        else
        {
            GUIWIN_SetTitleSubText(iconlist_ctrl_ptr->win_id,
                FALSE,
                PNULL,
                0,
                FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : display icon list title
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayTitle(
                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                        )
{
    BOOLEAN             result 		  = FALSE;
    GUIICONLIST_TEXT_T  *text_ptr 	  = PNULL;
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL != iconlist_ctrl_ptr)
	{
		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

		//is display title
		switch (iconlist_ctrl_ptr->style)
		{
		case GUIICONLIST_STYLE_ICON_UIDT:
		case GUIICONLIST_STYLE_FOLDER_UIDT:
			result = FALSE;
			break;
			
		default:
#ifdef GUIF_ICONLIST_PDASTYLE
			result = FALSE;
#else
			result = TRUE;
#endif
			break;
		}
		
		if (result) /*lint !e774*/
		{
			//get text
			text_ptr = &iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index].text;
			
			//get current icon title and softkey info
			if (!text_ptr->is_load)
			{
				GUICTRL_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_ICONLIST_APPEND_TEXT,&iconlist_ctrl_ptr->cur_index);
			}
			if (iconlist_ctrl_ptr->is_display_title)
			{
			  GUIWIN_SetTitleText(iconlist_ctrl_ptr->win_id,
				text_ptr->display.name_str.wstr_ptr,
				text_ptr->display.name_str.wstr_len,
				TRUE);
			}
		}
	}
}

/*****************************************************************************/
//  Description : display icon list softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplaySoftkey(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                          )
{
    MMI_TEXT_ID_T       mid_softkey = 0;
    GUIICONLIST_ITEM_T  *item_ptr 	= PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}
	item_ptr 	= &iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index];

    if (iconlist_ctrl_ptr->theme.is_display_softkey)
    {
        return;
    }

    //kevin.lou modified:delete assert
    //SCI_ASSERT(iconlist_ctrl_ptr->cur_index < iconlist_ctrl_ptr->total_num); /*assert verified*/
    if (iconlist_ctrl_ptr->cur_index >= iconlist_ctrl_ptr->total_num)
    {
        return;
    }

    if ((iconlist_ctrl_ptr->is_markable) &&
        (0 < iconlist_ctrl_ptr->selected_num))
    {
        if (item_ptr->is_selected)
        {
            //cancel
            mid_softkey = iconlist_ctrl_ptr->theme.cancel_text_id;
        }
        else
        {
            //mark
            mid_softkey = iconlist_ctrl_ptr->theme.mark_text_id;
        }
    }
    else
    {
        mid_softkey = item_ptr->text.display.softkey_id[1];
    }
    
    GUIWIN_SetSoftkeyTextId(iconlist_ctrl_ptr->win_id, 
        item_ptr->text.display.softkey_id[0],
        mid_softkey,
        item_ptr->text.display.softkey_id[2],
        TRUE);
}

/*****************************************************************************/
//  Description : handle text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextScrollTimer(
                                 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                                 )
{
    uint16              str_width = 0;
    uint16              display_width = 0;
    GUI_RECT_T          text_rect = {0};
    GUIICONLIST_TEXT_T  *text_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    //stop text timer
    StopTextScrollTimer(iconlist_ctrl_ptr);

    //get text rect
    text_rect = ICONLIST_GetTextRect(iconlist_ctrl_ptr->cur_index,iconlist_ctrl_ptr);

    //get display width
    display_width = (uint16)(text_rect.right - text_rect.left + 1);

    //get text
    text_ptr = &iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index].text;

    //set string index
    if (MMITHEME_GetIsScrollAsPixel())
    {
        //get item text len
        str_width = GUI_CalculateStringPiexlNum(text_ptr->display.name_str.wstr_ptr,
                            text_ptr->display.name_str.wstr_len,
                            iconlist_ctrl_ptr->theme.font.font,
                            0);

        if ((str_width - 1 - iconlist_ctrl_ptr->str_index) > display_width)
        {
            iconlist_ctrl_ptr->str_index = (uint16)(iconlist_ctrl_ptr->str_index + iconlist_ctrl_ptr->theme.scroll_pixel);
        }
        else
        {
            iconlist_ctrl_ptr->str_index = 0;
        }
    }
    else
    {
        //get label string len
        str_width = GUI_CalculateStringPiexlNum((text_ptr->display.name_str.wstr_ptr+iconlist_ctrl_ptr->str_index),
                            (uint16)(text_ptr->display.name_str.wstr_len - iconlist_ctrl_ptr->str_index),
                            iconlist_ctrl_ptr->theme.font.font,
                            0);

        //string has display
        if (str_width > display_width)
        {
            iconlist_ctrl_ptr->str_index++;
        }
        else
        {
            iconlist_ctrl_ptr->str_index = 0;
        }
    }
    
    //display item text
    DisplayItemText(iconlist_ctrl_ptr->cur_index,iconlist_ctrl_ptr);
}

/*****************************************************************************/
//  Description : handle icon list web key
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleIconListWebKey(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                )
{
	CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}
	base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
    {
        //is markable
        if ((0 < iconlist_ctrl_ptr->total_num) && 
            (iconlist_ctrl_ptr->is_markable) && 
            (0 < iconlist_ctrl_ptr->selected_num))
        {
            //select or unselect
            SelectItemOrNot(iconlist_ctrl_ptr);
        }
        else
        {
            GUICTRL_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_MIDSK);
        }
    }
}

/*****************************************************************************/
//  Description : select item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SelectItemOrNot(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
    BOOLEAN         is_selected = FALSE;
    MMI_HANDLE_T    ctrl_handle = 0;
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

    if (PNULL == iconlist_ctrl_ptr)
    {
		return;
    }

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
	
    //set selected item
    SCI_ASSERT(iconlist_ctrl_ptr->cur_index < iconlist_ctrl_ptr->total_num); /*assert verified*/
    is_selected = (BOOLEAN)(!iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index].is_selected);
    CTRLICONLIST_SetSelectedItem(base_ctrl_ptr->handle,
        iconlist_ctrl_ptr->cur_index,
        is_selected);

    if (0 != (CTRLBASE_OBJ_T*)iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index].icon.ctrl_add)
    { 
        //get control handle
        ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index].icon.ctrl_add);
        
        //update icon
        MMK_SendMsg(ctrl_handle,MSG_CTL_PAINT,PNULL);
    } 
    else
    {
        DisplayMarkIcon(iconlist_ctrl_ptr->cur_index, iconlist_ctrl_ptr);
    }

    //post mark item
    PostMarkItemMsg(iconlist_ctrl_ptr);
    //display softkey
    DisplaySoftkey(iconlist_ctrl_ptr);
}


/*****************************************************************************/
//  Description: post mark item
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void PostMarkItemMsg(
                           CTRLICONLIST_OBJ_T* iconlist_ctrl_ptr
                           )
{
    CTRLBASE_OBJ_T* base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    if (PNULL == iconlist_ctrl_ptr)
    {
        return;
    }
    
    GUICTRL_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_ICONLIST_MARK_ITEM);
}

/*****************************************************************************/
//  Description : handle icon list direction key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIconListDirectKey(
                                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
                                           MMI_MESSAGE_ID_E     msg_id
                                           )
{
    uint16          next_index = 0;
    MMI_RESULT_E    result = MMI_RESULT_FALSE;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return MMI_RESULT_FALSE;
	}

    if ((MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state) && 
        (0 < iconlist_ctrl_ptr->total_num))
    {
        //get next index
        next_index = GetNextIndex(iconlist_ctrl_ptr,msg_id);

        if (next_index != iconlist_ctrl_ptr->cur_index)
        {
            //stop text timer
            StopTextScrollTimer(iconlist_ctrl_ptr);
            iconlist_ctrl_ptr->str_index = 0;

            //set current index
            iconlist_ctrl_ptr->cur_index = next_index;

            //adjust relative display rect by current index
            if (AdjustDisplayRect(iconlist_ctrl_ptr))
            {
                //get top and bottom index
                GetTopBottomIndex(iconlist_ctrl_ptr);
            }

            //display icon list
            DisplayIconList(TRUE,iconlist_ctrl_ptr);

            result = MMI_RESULT_TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get next index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan.ma edit 2011.12
/*****************************************************************************/
LOCAL uint16 GetNextIndex(
                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,
                          MMI_MESSAGE_ID_E      msg_id
                          )
{
    uint16                    next_index         = 0;
#ifdef GUIF_ICONLIST_DELIMITER
	uint16                    cur_index_in_group = 0;
	MMI_LINK_NODE_T           *group_ptr         = PNULL;
	GUIICONLIST_GROUP_INFO_T  *tmp_ptr           = PNULL;
	GUIICONLIST_GROUP_INFO_T  *tmp_pre_ptr       = PNULL;
#endif

	if (PNULL == iconlist_ctrl_ptr)
	{
		return next_index;
	}

    //init next index
    next_index = iconlist_ctrl_ptr->cur_index;
	//获取group node的data
#ifdef GUIF_ICONLIST_DELIMITER
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
		group_ptr = ICONLIST_SearchGroupInListByIndex(iconlist_ctrl_ptr, iconlist_ctrl_ptr->cur_index);
		if (PNULL != group_ptr)
		{
			//review修正，避免多次强转
			tmp_ptr = (GUIICONLIST_GROUP_INFO_T*)group_ptr->data;
			if (PNULL == tmp_ptr)
			{
				return next_index;
			}
		}
	}
#endif

    switch (msg_id)
    {
    case MSG_APP_UP:
#ifdef GUIF_ICONLIST_DELIMITER
		//有分隔符的情况
		if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
		{
			//防空判断
			if (PNULL != group_ptr && PNULL != tmp_ptr)
			{	
				//计算当前index是group中是第几个
				cur_index_in_group = iconlist_ctrl_ptr->cur_index - tmp_ptr->start_item_index;
				
				//如果这是第一个group的第一行，就不移动了
				if ((group_ptr->prev_ptr == iconlist_ctrl_ptr->group_list_info.group_list_ptr->prev_ptr)&&
					(cur_index_in_group < iconlist_ctrl_ptr->col_num_page))
				{
					next_index = (uint16)iconlist_ctrl_ptr->cur_index;
				}
				//如果这时除第一个group以外的某个group的第一行，就移动到上一个group的最后一个元素上
				else if ((group_ptr->prev_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr->prev_ptr)&&
					(cur_index_in_group < iconlist_ctrl_ptr->col_num_page))
				{
					tmp_pre_ptr = (GUIICONLIST_GROUP_INFO_T*)group_ptr->prev_ptr->data;
					next_index = (uint16)(tmp_pre_ptr->start_item_index + tmp_pre_ptr->group_item_total_num - 1);
				}
				else
				{
					next_index = (int16)(iconlist_ctrl_ptr->cur_index - iconlist_ctrl_ptr->col_num_page);
				}
			}
			break;
		}
		//无分隔符的情况
		else
#endif
		{
			//纵向滑动
			if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
			{
				if (iconlist_ctrl_ptr->cur_index >= iconlist_ctrl_ptr->col_num_page)
				{
					next_index = (uint16)(iconlist_ctrl_ptr->cur_index - iconlist_ctrl_ptr->col_num_page);
				}
				else
				{
					uint16  cur_icon_col = 0;
					uint16  last_icon_col = 0;
					
					//get current and last icon col num
					cur_icon_col  = (uint16)(iconlist_ctrl_ptr->cur_index % iconlist_ctrl_ptr->col_num_page);
					last_icon_col = (uint16)((iconlist_ctrl_ptr->total_num - 1) % iconlist_ctrl_ptr->col_num_page);
					
					//set next icon index
					if (cur_icon_col < last_icon_col)
					{
						next_index = (uint16)(iconlist_ctrl_ptr->total_num - 1 - (last_icon_col - cur_icon_col));
					}
					else
					{
						next_index = (uint16)(iconlist_ctrl_ptr->total_num - 1);
					}
				}
				break;
			}
			//横向滑动
			else
			{
				if (0 < iconlist_ctrl_ptr->cur_index)
				{				
					next_index = (uint16)(iconlist_ctrl_ptr->cur_index - 1);	
				}
				else
				{
					next_index = 0;
				}
				break;
			}			
		}
		
    case MSG_APP_DOWN:
#ifdef GUIF_ICONLIST_DELIMITER
		//有分隔符的情况
		if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
		{
			//防空判断
			if (PNULL != group_ptr && PNULL != tmp_ptr)
			{
				next_index = (uint16)(iconlist_ctrl_ptr->cur_index + iconlist_ctrl_ptr->col_num_page);
				
				//这个group中当前index所在的列的下一行没有元素了
				if (next_index > (tmp_ptr->start_item_index + (tmp_ptr->group_item_total_num - 1)))
				{				
					//这是最后一个group，不移动了
					if (group_ptr->next_ptr == iconlist_ctrl_ptr->group_list_info.group_list_ptr)
					{
						next_index = (uint16)iconlist_ctrl_ptr->cur_index;
					}
					else
					{
						next_index = ((GUIICONLIST_GROUP_INFO_T*)group_ptr->next_ptr->data)->start_item_index;
					}
				}
			}
			break;			
		}
		//无分隔符的情况
		else
#endif
		{
			//纵向滑动
			if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
			{
				next_index = (uint16)(iconlist_ctrl_ptr->cur_index + iconlist_ctrl_ptr->col_num_page);
				if (next_index > (iconlist_ctrl_ptr->total_num-1))
				{
					uint16  cur_icon_row = 0;
					uint16  last_icon_row = 0;
					
					//get the next and last icon row
					cur_icon_row = (uint16)(iconlist_ctrl_ptr->cur_index / iconlist_ctrl_ptr->col_num_page);
					last_icon_row = (uint16)((iconlist_ctrl_ptr->total_num - 1) / iconlist_ctrl_ptr->col_num_page);
					
					if (cur_icon_row == last_icon_row)
					{
						next_index = (uint16)(iconlist_ctrl_ptr->cur_index % iconlist_ctrl_ptr->col_num_page);
					}
					else
					{
						next_index = (uint16)(iconlist_ctrl_ptr->total_num-1);
					}
				}    
				break;
			}
			//横向滑动
			else
			{
				next_index = (uint16)(iconlist_ctrl_ptr->cur_index + 1);
				
				if (next_index > (iconlist_ctrl_ptr->total_num - 1))
				{
					next_index = iconlist_ctrl_ptr->total_num - 1;
				}
				break;
			}
		}

    case MSG_APP_LEFT:
		//纵向滑动
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))	
		{
			if (0 < iconlist_ctrl_ptr->cur_index)
			{				
				next_index = (uint16)(iconlist_ctrl_ptr->cur_index - 1);	
			}
			else
			{
				next_index = (uint16)(iconlist_ctrl_ptr->total_num - 1);
			}
			break;
		}
		//横向滑动
		else
		{
			if (iconlist_ctrl_ptr->cur_index >= iconlist_ctrl_ptr->row_num_page)
			{
				next_index = (uint16)(iconlist_ctrl_ptr->cur_index - iconlist_ctrl_ptr->row_num_page);
			}
			else
			{
				uint16  cur_icon_row = 0;
				uint16  last_icon_row = 0;
				
				//get current and last icon col num
				cur_icon_row  = (uint16)(iconlist_ctrl_ptr->cur_index % iconlist_ctrl_ptr->row_num_page);
				last_icon_row = (uint16)((iconlist_ctrl_ptr->total_num - 1) % iconlist_ctrl_ptr->row_num_page);
				
				//set next icon index
				if (cur_icon_row < last_icon_row)
				{
					next_index = (uint16)(iconlist_ctrl_ptr->total_num - 1 - (last_icon_row - cur_icon_row));
				}
				else
				{
					next_index = (uint16)(iconlist_ctrl_ptr->total_num - 1);
				}
			}
			break;
		}

    case MSG_APP_RIGHT:
		//纵向滑动
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			next_index = (uint16)(iconlist_ctrl_ptr->cur_index + 1);
			
			if (next_index > (iconlist_ctrl_ptr->total_num-1))
			{
				next_index = 0;
			}
			break;
		}
		//横向滑动
		else
		{
			next_index = (uint16)(iconlist_ctrl_ptr->cur_index + iconlist_ctrl_ptr->row_num_page);
			if (next_index > (iconlist_ctrl_ptr->total_num - 1))
			{
				uint16  cur_icon_col = 0;
				uint16  last_icon_col = 0;
				
				//get the next and last icon row
				cur_icon_col = (uint16)(iconlist_ctrl_ptr->cur_index / iconlist_ctrl_ptr->row_num_page);
				last_icon_col = (uint16)((iconlist_ctrl_ptr->total_num - 1) / iconlist_ctrl_ptr->row_num_page);
				
				if (cur_icon_col == last_icon_col)
				{
					next_index = (uint16)(iconlist_ctrl_ptr->cur_index % iconlist_ctrl_ptr->row_num_page);
				}
				else
				{
					next_index = (uint16)(iconlist_ctrl_ptr->total_num - 1);
				}
			}    
			break;	
		}

    case MSG_APP_STAR:
        //is first one
        if (0 == iconlist_ctrl_ptr->cur_index)
        {
            next_index = GetMaxTopIndex(iconlist_ctrl_ptr);
        }
        else
        {
            if (iconlist_ctrl_ptr->cur_index >= iconlist_ctrl_ptr->icon_num_page)
            {
                next_index = (uint16)(iconlist_ctrl_ptr->cur_index - iconlist_ctrl_ptr->icon_num_page);
            }
            else
            {
                next_index = 0;
            }
        }
        break;

    case MSG_APP_HASH:
        //is last one
        if (iconlist_ctrl_ptr->cur_index == (uint16)(iconlist_ctrl_ptr->total_num - 1))
        {
            next_index = 0;
        }
        else
        {
            next_index = (uint16)(iconlist_ctrl_ptr->cur_index + iconlist_ctrl_ptr->icon_num_page);
            if (next_index > (iconlist_ctrl_ptr->total_num-1))
            {
                next_index = (uint16)(iconlist_ctrl_ptr->total_num - 1);
            }
        }
        break;

    default:
        //SCI_TRACE_LOW:"GetNextIndex:msg_id %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_2770_112_2_18_3_19_8_175,(uint8*)"d",msg_id);
        break;
    }

    return (next_index);
}

/*****************************************************************************/
//  Description : get the max top index,the icon must display all
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL uint16 GetMaxTopIndex(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            )
{
    uint16                   max_row_index         = 0;
    uint16                   max_top_index         = 0;
    uint16                   top_line_last_page    = 0;
#ifdef GUIF_ICONLIST_DELIMITER
	int32                    max_top               = 0; //最后一页的top
	GUI_RECT_T        		 iconlist_rect         = {0};
	int16                    disp_height           = 0;
#endif

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

#ifdef GUIF_ICONLIST_DELIMITER
	//有分隔符的情况
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
		//最后一页的top
		iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);
		disp_height = iconlist_rect.bottom - iconlist_rect.top;
		//最后一页的top = 最后一个group的bottom - 页高
		max_top = ((GUIICONLIST_GROUP_INFO_T *)iconlist_ctrl_ptr->group_list_info.group_list_ptr->prev_ptr->data)->group_relative_rect.bottom - iconlist_rect.top - disp_height;
		max_top_index = GetItemIndexInPosRect(iconlist_ctrl_ptr, TRUE, max_top);
	}
	//无分隔符的情况
	else
#endif
	{
		//获得最后Icon行索引
		max_row_index = (uint16)((iconlist_ctrl_ptr->total_num-1)/iconlist_ctrl_ptr->col_num_page);
		
		//获得最后一页首行数
		if (max_row_index >= iconlist_ctrl_ptr->row_num_page)
		{
			top_line_last_page = (uint16)(max_row_index + 1 - iconlist_ctrl_ptr->row_num_page);
		}
		else
		{
			top_line_last_page = 0;
		}
		
		//获得最后一页第一个完全显示的icon索引
		max_top_index = (uint16)(top_line_last_page * iconlist_ctrl_ptr->col_num_page);
    }

    return (max_top_index);
}

/*****************************************************************************/
//  Description : get the max top index,the icon may display part
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit
/*****************************************************************************/
LOCAL uint16 GetPartMaxTopIndex(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                )
{
    uint16                   max_row_index      = 0;
    uint16                   max_top_index      = 0;
    uint16                   top_line_last_page = 0;
#ifdef GUIF_ICONLIST_DELIMITER
	int32                    max_top            = 0; //最后一页的top
	GUI_RECT_T               iconlist_rect      = {0};
	int16                    disp_height        = 0;
#endif

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

#ifdef GUIF_ICONLIST_DELIMITER
	//有分隔符的情况
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
		//最后一页的top
		iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);
		disp_height = iconlist_rect.bottom - iconlist_rect.top;
		max_top = ((GUIICONLIST_GROUP_INFO_T *)iconlist_ctrl_ptr->group_list_info.group_list_ptr->prev_ptr->data)->group_relative_rect.bottom - iconlist_rect.top - disp_height;
		max_top_index = GetItemIndexInPosRect(iconlist_ctrl_ptr, TRUE, max_top);
	}
	//无分隔符的情况
	else
#endif
	{		
		max_row_index = (uint16)((iconlist_ctrl_ptr->total_num - 1)/iconlist_ctrl_ptr->col_num_page);
		
		//获得最后一页首行数
		if (max_row_index >= iconlist_ctrl_ptr->row_max_num_page)
		{
			top_line_last_page = (uint16)(max_row_index + 1 - iconlist_ctrl_ptr->row_max_num_page);
		}
		else
		{
			top_line_last_page = 0;
		}
		
		//获得最后一页第一个完全显示的icon索引
		max_top_index = (uint16)(top_line_last_page * iconlist_ctrl_ptr->col_num_page);
	}

    return (max_top_index);
}

/*****************************************************************************/
//  Description : handle icon list star/hash key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIconListPageKey(
                                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
                                         MMI_MESSAGE_ID_E   msg_id
                                         )
{
    BOOLEAN             is_prev_page 	= FALSE;
    uint16              next_index 		= 0;
    uint16              top_index 		= 0;
    uint16              row_index 		= 0;
    uint16              last_row_index 	= 0;
    uint16              display_height 	= 0;
    int32               display_top 	= 0;
    int32               display_max_top = 0;
    MMI_RESULT_E        result 			= MMI_RESULT_FALSE;
    GUI_BIG_RECT_T      relative_rect 	= {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return MMI_RESULT_FALSE;
	}

    if ((MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state) && 
        (iconlist_ctrl_ptr->total_num > iconlist_ctrl_ptr->icon_num_page))
    {
        //get next index
        next_index = GetNextIndex(iconlist_ctrl_ptr,msg_id);

        if (next_index != iconlist_ctrl_ptr->cur_index)
        {
            //is previous page
            if (next_index < iconlist_ctrl_ptr->cur_index)
            {
                is_prev_page = TRUE;
            }
            
            //get top index
            top_index = GetTopIndex(is_prev_page,next_index,iconlist_ctrl_ptr);

			//get top item relative rect
			relative_rect = ICONLIST_GetItemRelativeRect(top_index,
											iconlist_ctrl_ptr,TRUE);

            //selected,adjust relative rect
            if (SELECT_FLAG_WIDTH < relative_rect.top)
            {
                relative_rect.top = relative_rect.top - SELECT_FLAG_WIDTH;
            }
            relative_rect.bottom = relative_rect.bottom + SELECT_FLAG_WIDTH;

            //get the current icon row index
            row_index = (uint16)(top_index / iconlist_ctrl_ptr->col_num_page);

            //get the last icon row index
            last_row_index = (uint16)((iconlist_ctrl_ptr->total_num - 1) / iconlist_ctrl_ptr->col_num_page);

            //set display top
            if (0 == row_index)
            {
                display_top = 0;
            }
            else if (row_index == last_row_index)
            {
                //get display height
                display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);

                //the icon is in the last line
                display_top = iconlist_ctrl_ptr->relative_rect.bottom - display_height + 1;
            }
            else
            {
                display_top = relative_rect.top;
            }

            //get display max top
            display_max_top = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);

            //display top
            display_top = (int32)MIN(display_top,display_max_top);
        
            //set current index
            iconlist_ctrl_ptr->cur_index = next_index;

            //set display rect
            if (ICONLIST_SetDispRect(display_top,iconlist_ctrl_ptr))
            {
                //get top and bottom index
                GetTopBottomIndex(iconlist_ctrl_ptr);
            }

            //display icon list
            DisplayIconList(TRUE,iconlist_ctrl_ptr);

            result = MMI_RESULT_TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get the top index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTopIndex(
                         BOOLEAN            is_prev_page,
                         uint16             index,
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                         )
{
    uint16      top_index 	  = 0;
    uint16      line_index 	  = 0;
    uint16      max_top_index = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return 0;
	}

	//get the icon line index
	line_index = (uint16)(index / iconlist_ctrl_ptr->col_num_page);

    if (is_prev_page)
    {
        //the icon in the last line
        if ((line_index + 1) < iconlist_ctrl_ptr->row_num_page)
        {
            line_index = 0;
        }
        else
        {
            line_index = (uint16)(line_index + 1 - iconlist_ctrl_ptr->row_num_page);
        }
    }

    //the icon in the first line
    top_index = (uint16)(line_index * iconlist_ctrl_ptr->col_num_page);

    //get max top index
    max_top_index = GetMaxTopIndex(iconlist_ctrl_ptr);

    if (top_index > max_top_index)
    {
        top_index = max_top_index;
    }

    return (top_index);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle icon list tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListTpDown(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr, //in:
                                DPARAM              param               //in:
                                )
{
#ifdef TOUCH_PANEL_SUPPORT
    uint16          index 		= 0;
    GUI_POINT_T     tp_point 	= {0};
    MMI_HANDLE_T    ctrl_handle = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}
    //防止烂屏抖动
    //MMK_DelayTpMove();

    //stop text timer
    StopTextScrollTimer(iconlist_ctrl_ptr);

    iconlist_ctrl_ptr->is_tp_down 	 = TRUE;
    iconlist_ctrl_ptr->is_tp_pressed = TRUE;

    //get tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    if (MMK_TP_SLIDE_FLING != iconlist_ctrl_ptr->slide_state)
    {
        //get tp icon index
        if (GetTpIndex(&index,&tp_point,iconlist_ctrl_ptr))
        {
            if (index != iconlist_ctrl_ptr->cur_index)
            {
                //reset string index
                iconlist_ctrl_ptr->str_index = 0;
            
                //switch current selected icon
                iconlist_ctrl_ptr->cur_index = index;
    
                //adjust relative display rect by current index
                if (AdjustDisplayRect(iconlist_ctrl_ptr))
                {
                    //get top and bottom index
                    GetTopBottomIndex(iconlist_ctrl_ptr);
                }

                //display icon list
                DisplayIconList(TRUE,iconlist_ctrl_ptr);
            }
            else
            {
                //display selected flag
                DisplaySelectedFlag(index,iconlist_ctrl_ptr);
            }

            //is tp folder
            if (IsTpFolder(index,&tp_point,iconlist_ctrl_ptr))
            {
                //get control handle
                ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconlist_ctrl_ptr->item_ptr[index].icon.ctrl_add);
                
                //dispatch tp down msg to icon folder control
                MMK_SendMsg(ctrl_handle,MSG_TP_PRESS_DOWN,param);

                //display selected flag
                DisplaySelectedFlag(index,iconlist_ctrl_ptr);
            }
        }
    }
    else
    {
        iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
    }

    //is glide
    if ((iconlist_ctrl_ptr->theme.is_support_slide) &&
        (0 < iconlist_ctrl_ptr->total_num))
    {
        //set start and previous point
        iconlist_ctrl_ptr->start_point = tp_point;
        iconlist_ctrl_ptr->prev_point  = tp_point;

        //set tp down slide state
        iconlist_ctrl_ptr->tpdown_slide_state = iconlist_ctrl_ptr->slide_state;

        //add velocity item
        MMK_AddVelocityItem(tp_point.x,tp_point.y);

        if (MMK_TP_SLIDE_FLING != iconlist_ctrl_ptr->slide_state)
        {
            //start tp slide timer
            StartTpSlideTimer(iconlist_ctrl_ptr);
        }
    }
#endif    
}

/*****************************************************************************/
//  Description : get tp icon index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpIndex(
                         uint16             *index_ptr,         //in/out:
                         GUI_POINT_T        *point_ptr,         //in:
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in:
                         )
{
    BOOLEAN         result        = FALSE;
    uint16          i	          = 0;
    GUI_RECT_T     item_rect  	  = {0};
    GUI_RECT_T     cross_rect 	  = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;

    if ((PNULL == iconlist_ctrl_ptr)||(PNULL == point_ptr))
    {
    	return FALSE;
    }

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    //if press in icon list control
    if ((0 < iconlist_ctrl_ptr->total_num) &&
        (GUI_PointIsInRect(*point_ptr,base_ctrl_ptr->rect)))
    {
        for (i=iconlist_ctrl_ptr->top_index; i<=iconlist_ctrl_ptr->bottom_index; i++)
        {
            //get item rect
            item_rect = ICONLIST_GetItemRect(i,iconlist_ctrl_ptr);

            if ((GUI_IntersectRect(&cross_rect,item_rect,base_ctrl_ptr->display_rect)) && 
                (GUI_PointIsInRect(*point_ptr,cross_rect)))
            {
                if (PNULL != index_ptr)
                {
                    *index_ptr = i;
                }
                
                result = TRUE;
                break;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : is tp folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpFolder(
                         uint16             index,              //i:
                         GUI_POINT_T        *point_ptr,         //in:
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in:
                         )
{
    BOOLEAN     	result 		   = FALSE;
    GUI_RECT_T  	icon_rect 	   = {0};
    GUI_RECT_T  	cross_rect 	   = {0};
    CTRLBASE_OBJ_T 	*base_ctrl_ptr = PNULL;
    
	if ((PNULL == iconlist_ctrl_ptr)||(PNULL == point_ptr))
	{
		return FALSE;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return FALSE;
	}
	
	base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
	
    if (GUIICONLIST_STYLE_FOLDER_UIDT == iconlist_ctrl_ptr->style)
    {
        //get icon rect
        icon_rect = ICONLIST_GetIconRect(index,iconlist_ctrl_ptr);

        if ((GUI_IntersectRect(&cross_rect,icon_rect,base_ctrl_ptr->display_rect)) && 
            (GUI_PointIsInRect(*point_ptr,cross_rect)))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : start tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpSlideTimer(
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
                             )
{
	CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    //include progress bar
    if (0 == iconlist_ctrl_ptr->slide_timer_id)
    {
        iconlist_ctrl_ptr->slide_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
                                                iconlist_ctrl_ptr->theme.slide_period,
                                                FALSE);
    }
}

/*****************************************************************************/
//  Description : stop tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopTpSlideTimer(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            )
{
	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    if (0 < iconlist_ctrl_ptr->slide_timer_id)
    {
        MMK_StopTimer(iconlist_ctrl_ptr->slide_timer_id);
        iconlist_ctrl_ptr->slide_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle icon list tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL void HandleIconListTpMove(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr, //in:
                                DPARAM              param               //in:
                                )
{
    GUI_POINT_T     tp_point = {0};

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    iconlist_ctrl_ptr->is_tp_pressed = FALSE;

    //is glide
    if ((iconlist_ctrl_ptr->theme.is_support_slide) &&
        (0 < iconlist_ctrl_ptr->total_num))
    {
        //get tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

		//纵向滑动
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			if (IS_TP_MOVE_Y(iconlist_ctrl_ptr->start_point.y,tp_point.y))
			{
#ifdef TOUCH_PANEL_SUPPORT			
				//set slide state
				if ((MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->tpdown_slide_state) ||
					(MMK_IsWayChanged(iconlist_ctrl_ptr->fling_velocity,&iconlist_ctrl_ptr->start_point,&tp_point,FALSE)))
				{
					iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
				}
#else
				//set slide state
				if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->tpdown_slide_state)
				{
					iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
				}
#endif				
			}
		}
		//横向滑动
		else
		{
			if (IS_TP_MOVE_X(iconlist_ctrl_ptr->start_point.x,tp_point.x))
			{
#ifdef TOUCH_PANEL_SUPPORT				
				//set slide state
				if ((MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->tpdown_slide_state) ||
					(MMK_IsWayChanged(iconlist_ctrl_ptr->fling_velocity,&iconlist_ctrl_ptr->start_point,&tp_point,TRUE)))
				{
					iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
				}
#else
				//set slide state
				if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->tpdown_slide_state)
				{
					iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
				}
#endif
			}
		}	

        //add velocity item
        MMK_AddVelocityItem(tp_point.x,tp_point.y);
    }
}

/*****************************************************************************/
//  Description : handle icon list tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListTpUp(
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in:
                              DPARAM                param               //in:
                              )
{
#ifdef TOUCH_PANEL_SUPPORT
    uint16          index 		   = 0;
    GUI_POINT_T     tp_point 	   = {0};
    MMI_HANDLE_T    ctrl_handle    = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;    

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    iconlist_ctrl_ptr->is_tp_down = FALSE;
    iconlist_ctrl_ptr->is_tp_pressed = FALSE;

    //get tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    //stop slide timer
    StopTpSlideTimer(iconlist_ctrl_ptr);

    if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
    {
        //get tp icon index
        if ((GetTpIndex(&index,&tp_point,iconlist_ctrl_ptr)) && 
            (index == iconlist_ctrl_ptr->cur_index))
        {
            //is markable
            //xiyuan edit for NEWMS00174415
			if (iconlist_ctrl_ptr->is_markable)
            {
                //select or unselect
                SelectItemOrNot(iconlist_ctrl_ptr);
            }
            else
            {
                GUICTRL_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENOK);
            }

            //is tp folder
            if (IsTpFolder(index,&tp_point,iconlist_ctrl_ptr))
            {
                //get control handle
                ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconlist_ctrl_ptr->item_ptr[index].icon.ctrl_add);
                
                //dispatch tp down msg to icon folder control
                MMK_SendMsg(ctrl_handle,MSG_TP_PRESS_UP,param);
            }
        }
    }
    else
    {
        //add velocity item
        MMK_AddVelocityItem(tp_point.x,tp_point.y);
		
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
            //calculate current velocity
            MMK_ComputeCurrentVelocity(PNULL,&iconlist_ctrl_ptr->fling_velocity,FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);
        } 
		else
		{
            //calculate current velocity
            MMK_ComputeCurrentVelocity(&iconlist_ctrl_ptr->fling_velocity,PNULL,FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);
        }

        //fling velocity
        if (FLING_MIN_VELOCITY > abs((int32)iconlist_ctrl_ptr->fling_velocity))
        {
            //reset velocity item
#ifdef TOUCH_PANEL_SUPPORT            
            MMK_ResetVelocityItem();
#endif
            iconlist_ctrl_ptr->fling_velocity = 0;

            //reset icon list slide
            ResetIconListSlide(TRUE,iconlist_ctrl_ptr);
        }
        else
        {
            //enter fling state
            iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;

            //start tp slide timer
            StartTpSlideTimer(iconlist_ctrl_ptr);
        }
    }
#endif    
}

/*****************************************************************************/
//  Description : handle icon list tp long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIconListTpLong(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr, //in:
                                DPARAM              param               //in:
                                )
{
    uint16          index 		   = 0;
    GUI_POINT_T     tp_point 	   = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    switch (iconlist_ctrl_ptr->slide_state)
    {
    case MMK_TP_SLIDE_NONE:
        //get tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        //get tp icon index
        if (GetTpIndex(&index,&tp_point,iconlist_ctrl_ptr))
        {
            GUICTRL_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_ICONLIST_TPLONG,&index);
        }
        break;

    case MMK_TP_SLIDE_FLING:
        //stop slide timer
        StopTpSlideTimer(iconlist_ctrl_ptr);

        //reset velocity item
#ifdef TOUCH_PANEL_SUPPORT        
        MMK_ResetVelocityItem();
#endif
        iconlist_ctrl_ptr->fling_velocity = 0;
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : handle icon list tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL void HandleIconListSlideTimer(
                                    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                    )
{
#ifdef TOUCH_PANEL_SUPPORT
    GUI_POINT_T         tp_point  = {0};
    MMI_TP_STATUS_E     tp_status = MMI_TP_NONE;
#endif	

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    //stop slide timer
    StopTpSlideTimer(iconlist_ctrl_ptr);

#ifdef TOUCH_PANEL_SUPPORT
    if (MMK_IsFocusWin(iconlist_ctrl_ptr->win_id))
    {
        //start tp slide timer
        StartTpSlideTimer(iconlist_ctrl_ptr);
        
        if (MMK_TP_SLIDE_NONE != iconlist_ctrl_ptr->slide_state)
        {
            MMK_GetLogicTPMsg(&tp_status,&tp_point);
			//纵向滑动
			if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
			{
				//handle slide
				if ((!iconlist_ctrl_ptr->is_tp_down) || 
					(abs(iconlist_ctrl_ptr->prev_point.y - tp_point.y) > 3))
				{
					HandleIconListSlide(tp_point,iconlist_ctrl_ptr);
					
					//set previous point
					iconlist_ctrl_ptr->prev_point = tp_point;
				}
			}
			//横向滑动
			else
			{
				//handle slide
				if ((!iconlist_ctrl_ptr->is_tp_down) || 
					(abs(iconlist_ctrl_ptr->prev_point.x - tp_point.x) > 3))
				{
					HandleIconListSlide(tp_point,iconlist_ctrl_ptr);
					
					//set previous point
					iconlist_ctrl_ptr->prev_point = tp_point;
				}
			}
        }
    }
#endif
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle icon list slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL void HandleIconListSlide(
                               GUI_POINT_T          tp_point,
                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                               )
{
    BOOLEAN     is_reset          = FALSE;
    int32       scroll_step       = 0;   
    uint16      display_tmp       = 0; //disp height  or width
	int32       display_rect      = 0; //disp top     or left
	int32       display_max_tmp   = 0; //disp max top or left

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	//纵向滑动
	if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		//init display top
		display_rect = iconlist_ctrl_ptr->relative_disp_rect.top;
	}
	//横向滑动
	else
	{
		//init display left
		display_rect = iconlist_ctrl_ptr->relative_disp_rect.left;
	}

    switch (iconlist_ctrl_ptr->slide_state)
    {
    case MMK_TP_SLIDE_TP_SCROLL:
		//纵向滑动
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			scroll_step = tp_point.y - iconlist_ctrl_ptr->prev_point.y;
		}
		//横向滑动
		else
		{
			scroll_step = tp_point.x - iconlist_ctrl_ptr->prev_point.x;
		}			
		display_rect = display_rect - scroll_step;	
        break;
		
    case MMK_TP_SLIDE_FLING:
        //get fling offset
        scroll_step = MMK_GetFlingOffset(0,iconlist_ctrl_ptr->fling_velocity,(float)FLING_TIME,&iconlist_ctrl_ptr->fling_velocity);
		
		//纵向滑动
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			//get display height
			display_tmp   = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);
			
			//get display max top
			display_max_tmp  = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);
		}
		//横向滑动
		else
		{
			//get display height
			display_tmp    = ICONLIST_GetDispWidth(iconlist_ctrl_ptr);
			
			//get display max top
			display_max_tmp = ICONLIST_GetDispMaxLeft(iconlist_ctrl_ptr);
		}	
				
        if (FLING_RUN_MIN_VELOCITY > abs((int32)iconlist_ctrl_ptr->fling_velocity))
        {
            //stop slide timer
            StopTpSlideTimer(iconlist_ctrl_ptr);
			
            is_reset = TRUE;
        }
        else if (0 > iconlist_ctrl_ptr->fling_velocity)//up or left
        {
			//set display top or left
			display_rect = display_rect - scroll_step;
			
			//last item display 1/2 in display rect
			if (display_rect > display_max_tmp + (display_tmp>>1))
			{
				//stop slide timer
				StopTpSlideTimer(iconlist_ctrl_ptr);
				
				display_rect = display_max_tmp + (display_tmp>>1);
				
				is_reset = TRUE;
			}
			
		}	
		else//down or right
		{
			//set display top or left
			display_rect = display_rect - scroll_step;
			
			//last item display 1/2 in display rect
			if ((0 > display_rect) &&
				((-display_rect) > (display_tmp>>1)))
			{
				//stop slide timer
				StopTpSlideTimer(iconlist_ctrl_ptr);
				
				display_rect = -(display_tmp>>1);
				
				is_reset = TRUE;
			}
		}
		break;
			
    default:
        break;
	}
	
	if (ICONLIST_SetDispRect(display_rect,iconlist_ctrl_ptr))
	{
		//get top and bottom index
		GetTopBottomIndex(iconlist_ctrl_ptr);
	
	    //display icon list
	    DisplayIconList(TRUE,iconlist_ctrl_ptr);
     }

    //resest
    if (is_reset)
    {
        ResetIconListSlide(TRUE,iconlist_ctrl_ptr);
    }
    else
    {
        UILAYER_SetDirectDraw(TRUE);
    }
}
#endif

/*****************************************************************************/
//  Description : reset icon list slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL void ResetIconListSlide(
                              BOOLEAN               is_update,
                              CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                              )
{
    BOOLEAN     is_append         = FALSE;
    uint16      i                 = 0;
    uint16      reset_slide_frame = 0;
    int32       move_offset       = 0; //纵滑为y偏移，横滑为x偏移
    int32       scroll_step       = 0;
    int32       display_tmp       = 0;
    int32       display_max_tmp   = 0;	
	BOOLEAN		is_drawn          = FALSE;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	//纵向滑动
	if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		//init display top
		display_tmp = iconlist_ctrl_ptr->relative_disp_rect.top;		
		//get display max top
		display_max_tmp = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);
	}
	//横向滑动
	else
	{
		//init display top
		display_tmp = iconlist_ctrl_ptr->relative_disp_rect.left;		
		//get display max top
		display_max_tmp = ICONLIST_GetDispMaxLeft(iconlist_ctrl_ptr);
	}

    //is need reset
    if ((0 > display_tmp) ||
        (display_max_tmp < display_tmp))
    {
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif
        if (0 > display_tmp)
        {
            move_offset = -display_tmp;
        }
        else
        {
            move_offset = display_max_tmp - display_tmp;
        }

        if (is_update)
        {
            reset_slide_frame = iconlist_ctrl_ptr->theme.reset_slide_frame;
        }
        else
        {
            reset_slide_frame = iconlist_ctrl_ptr->theme.reset_slide_start;
        }

        for (i = iconlist_ctrl_ptr->theme.reset_slide_start; i <= reset_slide_frame; i++)
        {
            //calculate scroll step
            scroll_step = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, (int16)move_offset, reset_slide_frame, i);

            //set display top
            if (ICONLIST_SetDispRect(display_tmp + scroll_step, iconlist_ctrl_ptr))
            {
                //get top and bottom index
                GetTopBottomIndex(iconlist_ctrl_ptr);

                //display icon list
                DisplayIconList(TRUE,iconlist_ctrl_ptr);

                if (i != reset_slide_frame)
                {
					is_drawn = TRUE;
                    //update
                    MMITHEME_UpdateRect();
                }
            }
        }

#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif
    }

    //reset slide state
    iconlist_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

    if ((MMK_IsFocusWin(iconlist_ctrl_ptr->win_id)) &&
        (0 < iconlist_ctrl_ptr->total_num))
    {
        //update current item text for scroll text
        DisplayItemText(iconlist_ctrl_ptr->cur_index,iconlist_ctrl_ptr);

        //notify append icon
        for (i=iconlist_ctrl_ptr->top_index; i<=iconlist_ctrl_ptr->bottom_index; i++)
        {
            is_append = TRUE;

            if ((GUIICONLIST_LOAD_ONE_BY_ONE == iconlist_ctrl_ptr->load_type) &&
                (iconlist_ctrl_ptr->is_loading_icon))
            {
                is_append = FALSE;
            }

            if (is_append)
            {
                //append icon
                SendMsgAppendIcon(i,iconlist_ctrl_ptr);
            }
            else
            {
                break;
            }
        }
    }

	//xiyuan edit 修正UILAYER_SetDirectDraw时，引起的iconlist刷新问题
	if (is_update && !is_drawn)
	{
        DisplayIconList(TRUE,iconlist_ctrl_ptr);
	}
}

/*****************************************************************************/
//  Description : handle icon list tp progress bar msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL BOOLEAN HandleIconListTpPrg(
                                  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in:
                                  MMI_MESSAGE_ID_E      msg_id,             //in:
                                  DPARAM                param               //in:
                                  )
{
    BOOLEAN     result 		    = FALSE;
    BOOLEAN     is_up 		    = FALSE;
    uint16      display_height  = 0;
    uint16      scroll_step     = 0;
    int32       display_top     = 0;
    int32       display_max_top = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

	//横向滑动,无滚动条，不处理滚动条事件
	if (GUIICONLIST_LAYOUT_H == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
        return result;
	}

	//get display height
	display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);
	//get display max top
	display_max_top = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);

    switch (msg_id)
    {
    case MSG_CTL_PRGBOX_MOVEUP_ONE:
        is_up = TRUE;

        //get scroll step
        scroll_step = ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);
        break;

    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
        //get scroll step
        scroll_step = ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);
        break;

    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
        is_up = TRUE;

        //get scroll step
        scroll_step = display_height;
        break;

    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
        //get scroll step
        scroll_step = display_height;
        break;

    case MSG_CTL_PRGBOX_MOVE:
        //set display top
        display_top = *(uint32*)param;
        display_top = (int32)MIN(display_top,display_max_top);
        break;

    default:
        SCI_PASSERT(FALSE,("HandleIconListTpPrg: msg_id %d is error!",msg_id)); /*assert verified*/
        break;
    }

    //set display top
    if (MSG_CTL_PRGBOX_MOVE != msg_id)
    {
        if (is_up)
        {
            if (iconlist_ctrl_ptr->relative_disp_rect.top > scroll_step)
            {
                display_top = iconlist_ctrl_ptr->relative_disp_rect.top - scroll_step;
            }
            else
            {
                display_top = 0;
            }
        }
        else
        {
            display_top = (int32)MIN((iconlist_ctrl_ptr->relative_disp_rect.top + scroll_step),display_max_top);
        }
    }

    //set display rect
    if (ICONLIST_SetDispRect(display_top,iconlist_ctrl_ptr))
    {
        //stop text timer
        StopTextScrollTimer(iconlist_ctrl_ptr);

        //get top and bottom index
        GetTopBottomIndex(iconlist_ctrl_ptr);

        //display icon list
        DisplayIconList(TRUE,iconlist_ctrl_ptr);
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : set icon list style,default icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetStyle(
                                MMI_HANDLE_T        ctrl_id,
                                GUIICONLIST_STYLE_E style
                                )
{
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if ((PNULL != iconlist_ctrl_ptr) &&
        (style != iconlist_ctrl_ptr->style))
    {
        //set style
        iconlist_ctrl_ptr->style = style;

        //get row and column number
        GetRowColNum(iconlist_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : set icon list is display title,default display
//  Global resource dependence : 
//  Author: hongjun.jia
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetIsDispTitle(
								 MMI_HANDLE_T        ctrl_id,
								 BOOLEAN             is_display
								 )
{
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;
	
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(MMK_ConvertIdToHandle(ctrl_id));
	
    if(PNULL != iconlist_ctrl_ptr)
    {
		iconlist_ctrl_ptr->is_display_title = is_display;
    }
}

/*****************************************************************************/
//  Description : set icon list load anim type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetLoadType(
                                    MMI_HANDLE_T        ctrl_id,
                                    GUIICONLIST_LOAD_E  load_type
                                    )
{
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        iconlist_ctrl_ptr->load_type = load_type;
    }
}

/*****************************************************************************/
// 	Description : set icon list background
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetBg(
                                 MMI_CTRL_ID_T  ctrl_id,//in
                                 GUI_BG_T       *bg_ptr //in
                                 )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        //set bg
        iconlist_ctrl_ptr->theme.bg = *bg_ptr;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set icon list rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetRect(
                                MMI_HANDLE_T    ctrl_id,
                                GUI_RECT_T      *rect_ptr
                                )
{
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        IconlistCtrlSetRect((CTRLBASE_OBJ_T *)iconlist_ctrl_ptr,rect_ptr);
    }
}

/*****************************************************************************/
//  Description : get icon list total icons number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetTotalIcon(
									   MMI_CTRL_ID_T	ctrl_id	//in
									   )
{
	uint16				total_num 			= 0;
	CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

    if (PNULL != iconlist_ctrl_ptr)
    {
		total_num = iconlist_ctrl_ptr->total_num;
	}

	return (total_num);
}

/*****************************************************************************/
//  Description : set icon list total icons number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetTotalIcon(
                                     MMI_CTRL_ID_T    ctrl_id,  //in
                                     uint16           total_num //in
                                     )
{
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

    if (PNULL != iconlist_ctrl_ptr)
    {
        //destroy progress
        DestroyPrgCtrl(iconlist_ctrl_ptr);

        //free item memory
        FreeItemMemory(iconlist_ctrl_ptr);
        
        //set max icon
        iconlist_ctrl_ptr->total_num = total_num;
        
        iconlist_ctrl_ptr->is_dynamic_append_mode = FALSE;

        //alloc
        AllocItemMemory(iconlist_ctrl_ptr);

        //set relative rect
        SetRelativeRect(iconlist_ctrl_ptr);

        //set current index
        iconlist_ctrl_ptr->cur_index = 0;

        //get top and bottom index
        GetTopBottomIndex(iconlist_ctrl_ptr);

        if (0 == total_num)
        {
            GUIWIN_SetTitleSubText(iconlist_ctrl_ptr->win_id,
                FALSE,
                PNULL,
                0,
                FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : set icon list relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan.ma edit 2011.12
/*****************************************************************************/
LOCAL void SetRelativeRect(
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                           )
{
    uint16                    line_num         = 0;
    uint32                    relative_height  = 0;
    uint16                    item_distance    = 0;
	uint16                    item_space       = 0;
	uint16                    num_one_line     = 0;	
#ifdef GUIF_ICONLIST_DELIMITER
	GUIICONLIST_GROUP_INFO_T  *last_group_ptr  = PNULL;
	GUIICONLIST_GROUP_INFO_T  *first_group_ptr = PNULL;
#endif

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

#ifdef GUIF_ICONLIST_DELIMITER
	//有分隔符的情况
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
        if (iconlist_ctrl_ptr->is_group_rect_dirty)
        {            
    		//获取所有group的rect
    		ICONLIST_GetGroupRect(iconlist_ctrl_ptr);
            //rect更新完成了，避免重复计算rect
            iconlist_ctrl_ptr->is_group_rect_dirty = FALSE;
        }

		//最后一个group的bottom - 第一个group的top
		last_group_ptr = ((GUIICONLIST_GROUP_INFO_T *)iconlist_ctrl_ptr->group_list_info.group_list_ptr->prev_ptr->data);
		first_group_ptr = ((GUIICONLIST_GROUP_INFO_T *)iconlist_ctrl_ptr->group_list_info.group_list_ptr->data);	
		relative_height = last_group_ptr->group_relative_rect.bottom - first_group_ptr->group_relative_rect.top;				
	}
	//无分隔符的情况	
	else
#endif
	{
		//纵向滑动
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			num_one_line  = iconlist_ctrl_ptr->col_num_page;
			item_distance = ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);
			item_space    = ICONLIST_GetItemVspace(iconlist_ctrl_ptr);
			
			//get number
			line_num = (uint16)((iconlist_ctrl_ptr->total_num + iconlist_ctrl_ptr->col_num_page - 1) / num_one_line);
			
		}
		//横向滑动
		else
		{
			num_one_line  = iconlist_ctrl_ptr->row_num_page;
			item_distance = ICONLIST_GetItemWidth(TRUE,iconlist_ctrl_ptr);
			item_space    = ICONLIST_GetItemHspace(iconlist_ctrl_ptr);
			
			//get row number
			line_num      = GetTotalColNum(iconlist_ctrl_ptr);
		}
		
		//get all icon height
		relative_height = line_num * item_distance;
		
		//最后一个item下侧或右侧无item space
		if (0 < line_num)
		{
			relative_height = relative_height - item_space;
		}		
	}
	relative_height = relative_height + (iconlist_ctrl_ptr->theme.vmargin_space << 1);
	
	//set relative rect
	iconlist_ctrl_ptr->relative_rect = iconlist_ctrl_ptr->relative_disp_rect;
	
	//纵向滑动
	if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		iconlist_ctrl_ptr->relative_rect.top     = 0;
		iconlist_ctrl_ptr->relative_rect.bottom  = relative_height - 1;
	}
	//横向滑动	
	else
	{
		iconlist_ctrl_ptr->relative_rect.left    = 0;
		iconlist_ctrl_ptr->relative_rect.right   = relative_height - 1;
	}
}

/*****************************************************************************/
//  Description : UpdateGroupDisRect
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.09
//  Note:
/*****************************************************************************/
LOCAL void UpdateGroupDisRect(
                               CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                               )
{
    MMI_LINK_NODE_T *group_ptr = PNULL;
    GUI_BIG_RECT_T group_rect = {0};
    GUI_BIG_RECT_T group_relative_rect = {0};

    if (PNULL == iconlist_ctrl_ptr)
    {
        return;
    }
		
	//get head node ptr of group list
	group_ptr = iconlist_ctrl_ptr->group_list_info.group_list_ptr;

    if(PNULL == group_ptr || PNULL == group_ptr->data)
    {
        return;
    }
    
    group_relative_rect = ((GUIICONLIST_GROUP_INFO_T *)group_ptr->data)->group_relative_rect;    
    
    do 
    {
        //update group display rect
        group_rect.top = group_relative_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top;
        group_rect.bottom = group_relative_rect.bottom - iconlist_ctrl_ptr->relative_disp_rect.top;
        
        //set now group display rect
        ((GUIICONLIST_GROUP_INFO_T *)group_ptr->data)->group_rect = group_rect;

        //do next one
        group_ptr = group_ptr->next_ptr;
        group_relative_rect = ((GUIICONLIST_GROUP_INFO_T *)group_ptr->data)->group_relative_rect;    
    } 
    while(group_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr);

    return;
}

/*****************************************************************************/
//  Description : get icon list first and last index by relative display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL void GetTopBottomIndex(
                             CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
                             )
{
    BOOLEAN                   is_find          = FALSE;
    uint16                    pre_top_index    = 0;
    uint16                    pre_bottom_index = 0;
    uint16                    line_index       = 0;
    uint16                    top_index        = 0;
    uint16                    i                = 0;
    GUI_BIG_RECT_T            relative_rect    = {0};
    GUI_BIG_RECT_T            dest_rect        = {0};
    MMITHEME_ICONLIST_T       *theme_ptr       = &iconlist_ctrl_ptr->theme;
	uint16                    margin_space     = 0;
    int32                     min_position     = 0;
	uint16                    item_distance    = 0;
	uint16                    line_num         = 0;	
#ifdef GUIF_ICONLIST_DELIMITER
    uint16                    bottom_index     = 0;
	GUI_RECT_T                iconlist_rect    = {0};
#endif

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    //set previous first and last index
    pre_top_index    = iconlist_ctrl_ptr->top_index;
    pre_bottom_index = iconlist_ctrl_ptr->bottom_index;

	//获取top index
#ifdef GUIF_ICONLIST_DELIMITER
	//有分隔符的情况
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{
		//get icon list rect
		iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);
		top_index = GetItemIndexInPosRect(iconlist_ctrl_ptr, TRUE, iconlist_ctrl_ptr->relative_disp_rect.top);
		iconlist_ctrl_ptr->top_index = top_index;
	}
	//无分隔符的情况
	else
#endif
	{
		//纵向滑动
		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			margin_space     = theme_ptr->vmargin_space;
			min_position     = iconlist_ctrl_ptr->relative_disp_rect.top;
			item_distance    = ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);
			line_num         = iconlist_ctrl_ptr->col_num_page;		
		}
		//横向滑动
		else
		{
			margin_space     = theme_ptr->hmargin_space;
			min_position     = iconlist_ctrl_ptr->relative_disp_rect.left;
			item_distance    = ICONLIST_GetItemWidth(TRUE,iconlist_ctrl_ptr);
			line_num         = iconlist_ctrl_ptr->row_num_page;	
		}	
		
		//get top line index
		if (min_position > margin_space)
		{
			line_index = (uint16)((min_position - margin_space)/item_distance);
		}
		else
		{
			line_index = 0;
		}
		
		//get the first index
		top_index = (uint16)(line_index * line_num);
		
	}

	//获取bottom index
#ifdef GUIF_ICONLIST_DELIMITER
	//有分隔符的情况
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
	{

		bottom_index = GetItemIndexInPosRect(iconlist_ctrl_ptr, FALSE, iconlist_ctrl_ptr->relative_disp_rect.bottom);
		iconlist_ctrl_ptr->bottom_index = bottom_index;
		//防个数越界判断
        if (iconlist_ctrl_ptr->bottom_index > iconlist_ctrl_ptr->total_num)
        {
			if (0 == iconlist_ctrl_ptr->total_num)
			{
				iconlist_ctrl_ptr->bottom_index = 0;
			} 
			else
			{
				iconlist_ctrl_ptr->bottom_index = iconlist_ctrl_ptr->total_num -1;
			}
        }

		if (iconlist_ctrl_ptr->bottom_index < iconlist_ctrl_ptr->top_index)
		{
			iconlist_ctrl_ptr->bottom_index = iconlist_ctrl_ptr->top_index;
		}
	}
	//无分隔符的情况
	else
#endif
	{
		for (i = top_index; i < iconlist_ctrl_ptr->total_num; i++)
		{
			//get icon relative rect
			relative_rect = ICONLIST_GetItemRelativeRect(i,iconlist_ctrl_ptr,TRUE);
			
			//is cross
			if (GetCrossRect(&dest_rect,relative_rect,iconlist_ctrl_ptr->relative_disp_rect))
			{
				//set top and bottom index
				if (!is_find)
				{
					is_find = TRUE;
					iconlist_ctrl_ptr->top_index    = i;
					iconlist_ctrl_ptr->bottom_index = i;
				}
				else
				{
					iconlist_ctrl_ptr->bottom_index = i;
				}
			}
			else
			{
				if (is_find)
				{
					break;
				}
			}
		}

		if (!is_find)
		{
			iconlist_ctrl_ptr->bottom_index = iconlist_ctrl_ptr->top_index;
		}
	}

    //set start and end index
    switch (iconlist_ctrl_ptr->load_type)
    {
    case GUIICONLIST_LOAD_CUR_PAGE:
    case GUIICONLIST_LOAD_ONE_BY_ONE:
        if ((pre_top_index != iconlist_ctrl_ptr->top_index) ||
        (pre_bottom_index != iconlist_ctrl_ptr->bottom_index))
        {
            //free not current page item
            FreeNotCurPageItemMemory(pre_top_index,pre_bottom_index,iconlist_ctrl_ptr);
        }
        break;

    default:
        //do nothing
        break;
    }
}

/*******************************************************************************************/
//  Description : get cross rect by two rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*********************************************************************************************/
LOCAL BOOLEAN GetCrossRect(
                           GUI_BIG_RECT_T   *rect_dest_ptr,
                           GUI_BIG_RECT_T   rect1,
                           GUI_BIG_RECT_T   rect2
                           )
{
    BOOLEAN     result = FALSE;

    if (PNULL != rect_dest_ptr)
    {
        //get cross rect
        rect_dest_ptr->left   = (int32)(MAX(rect1.left,rect2.left));
        rect_dest_ptr->top    = (int32)(MAX(rect1.top,rect2.top));
        rect_dest_ptr->right  = (int32)(MIN(rect1.right,rect2.right));
        rect_dest_ptr->bottom = (int32)(MIN(rect1.bottom,rect2.bottom));

        if ((rect_dest_ptr->left <= rect_dest_ptr->right) &&
            (rect_dest_ptr->top <= rect_dest_ptr->bottom))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : append one icon text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendText(
                                      uint16                index,      //in
                                      MMI_CTRL_ID_T         ctrl_id,    //in
                                      GUIICONLIST_DISP_T    *display_ptr//in
                                      )
{
    BOOLEAN                 result 				= FALSE;
    CTRLICONLIST_OBJ_T      *iconlist_ctrl_ptr  = PNULL;
    GUIICONLIST_TEXT_T      *text_ptr 			= PNULL;

    if (PNULL != display_ptr)
    {
        //get icon list control pointer
        iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

        if ((PNULL != iconlist_ctrl_ptr) &&
            (index < iconlist_ctrl_ptr->total_num))
        {
            //get text
            text_ptr = &iconlist_ctrl_ptr->item_ptr[index].text;

            //free name
            if (PNULL != text_ptr->display.name_str.wstr_ptr)
            {
                SCI_FREE(text_ptr->display.name_str.wstr_ptr);
                text_ptr->display.name_str.wstr_len = 0;
            }

            //set name and softkey
            text_ptr->display = *display_ptr;

            //alloc name memory
            text_ptr->display.name_str.wstr_ptr = SCI_ALLOC_APP((display_ptr->name_str.wstr_len+1)*sizeof(wchar));
            SCI_MEMSET(text_ptr->display.name_str.wstr_ptr,0,((display_ptr->name_str.wstr_len+1)*sizeof(wchar)));

            //copy name
            MMI_WSTRNCPY(text_ptr->display.name_str.wstr_ptr,
                (display_ptr->name_str.wstr_len+1),
                display_ptr->name_str.wstr_ptr,
                display_ptr->name_str.wstr_len,
                display_ptr->name_str.wstr_len);

            text_ptr->is_load = TRUE;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : append one icon text addtion
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:   2012.03
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendTextAddtion(
											 uint16                index,      //in
											 MMI_CTRL_ID_T         ctrl_id,    //in
											 GUIICONLIST_DISP_T    *display_ptr//in
											 )
{
    BOOLEAN                 result 				= FALSE;
    CTRLICONLIST_OBJ_T      *iconlist_ctrl_ptr  = PNULL;
    GUIICONLIST_TEXT_T      *text_ptr 			= PNULL;

    if (PNULL != display_ptr)
    {
        //get icon list control pointer
        iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

        if ((PNULL != iconlist_ctrl_ptr) &&
            (index < iconlist_ctrl_ptr->total_num))
        {
            //get text
            text_ptr = &iconlist_ctrl_ptr->item_ptr[index].text_addtion;

            //free name
            if (PNULL != text_ptr->display.name_str.wstr_ptr)
            {
                SCI_FREE(text_ptr->display.name_str.wstr_ptr);
                text_ptr->display.name_str.wstr_len = 0;
            }

            //set name and softkey
            text_ptr->display = *display_ptr;

            //alloc name memory
            text_ptr->display.name_str.wstr_ptr = SCI_ALLOC_APP((display_ptr->name_str.wstr_len+1)*sizeof(wchar));
            SCI_MEMSET(text_ptr->display.name_str.wstr_ptr,0,((display_ptr->name_str.wstr_len+1)*sizeof(wchar)));

            //copy name
            MMI_WSTRNCPY(text_ptr->display.name_str.wstr_ptr,
                (display_ptr->name_str.wstr_len+1),
                display_ptr->name_str.wstr_ptr,
                display_ptr->name_str.wstr_len,
                display_ptr->name_str.wstr_len);

            text_ptr->is_load = TRUE;

            result = TRUE;
        }
    }

    return (result);
}
/*****************************************************************************/
//  Description : append one icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:一次只能加载一个icon，显示后控件通知App加载下一个
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendIcon(
                                      uint16                index,          //in
                                      MMI_CTRL_ID_T         ctrl_id,        //in
                                      GUIANIM_DATA_INFO_T   *data_info_ptr, //in
                                      GUIANIM_FILE_INFO_T   *file_info_ptr  //in
                                      )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = PNULL;

    if (PNULL != file_info_ptr || PNULL != data_info_ptr)
    {
        //get icon list control pointer
        iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

        if ((PNULL != iconlist_ctrl_ptr) &&
            (index < iconlist_ctrl_ptr->total_num))
        {
            //inset icon
            InsertIcon(index,iconlist_ctrl_ptr,data_info_ptr,file_info_ptr);

            result = TRUE;
        }
    }

    return (result);
}

/***************************************************************************//*!
@brief 添加图片信息
@author Jassmine
@param index         [in] item索引
@param ctrl_id       [in] 控件id
@param data_info_ptr [in] 图片数据信息
@param file_info_ptr [in] 图片文件信息
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendIconEx(
                                      uint16                index,          //in
                                      MMI_CTRL_ID_T         ctrl_id,        //in
									  GUIANIM_DATA_INFO_T   *data_info_ptr,	//in
                                      GUIANIM_FILE_INFO_T   *file_info_ptr , //in
                                      GUIANIM_DISPLAY_INFO_T * display_info
                                      )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = PNULL;

    if (PNULL != file_info_ptr || PNULL != data_info_ptr)
    {
        //get icon list control pointer
        iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

        if ((PNULL != iconlist_ctrl_ptr) &&
            (index < iconlist_ctrl_ptr->total_num))
        {
            //inset icon
            InsertIconEx(index,iconlist_ctrl_ptr,data_info_ptr,file_info_ptr , display_info);

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : insert icon and display icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertIconEx(
                      uint16                index,              //in:
                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in
                      GUIANIM_DATA_INFO_T   *data_info_ptr,     //in,may PNULL
                      GUIANIM_FILE_INFO_T   *file_info_ptr,      //in,may PNULL
                      GUIANIM_DISPLAY_INFO_T * display_info
                      )
{
    void                    *anim_ctrl_ptr  = PNULL;
    GUI_RECT_T              icon_rect 		= {0};
    GUI_RECT_T              display_rect 	= {0};
    GUIICONLIST_ICON_T      *icon_ptr 		= PNULL;
    GUIANIM_CTRL_INFO_T     ctrl_info 		= {0};
    MMI_CONTROL_CREATE_T    create 			= {0};
    GUIANIM_INIT_DATA_T     init_data 		= {0};
    GUIANIM_CALL_BACK_T     call_back 		= {0};
    CTRLBASE_OBJ_T     		*base_ctrl_ptr 	= PNULL;
    GUIANIM_DISPLAY_INFO_T  tmp_display_info = {0}; 
	
	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return;
	}

    SCI_MEMCPY((void*) &tmp_display_info , display_info , sizeof(GUIANIM_DISPLAY_INFO_T));

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    //get icon rect
    icon_rect = ICONLIST_GetIconRect(index,iconlist_ctrl_ptr);

    //get icon pointer
    icon_ptr = &iconlist_ctrl_ptr->item_ptr[index].icon;

    if (PNULL == icon_ptr->ctrl_add)
    {
        //init anim control
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = icon_rect;

        create.guid               = SPRD_GUI_ANIM_ID;
        create.parent_win_handle  = iconlist_ctrl_ptr->win_id;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr      = &init_data;

        anim_ctrl_ptr = MMK_CreateControl( &create );
        SCI_ASSERT(PNULL != anim_ctrl_ptr); /*assert verified*/

        //set anim control address
        icon_ptr->ctrl_add = (uint32)anim_ctrl_ptr;
    }
    else
    {
        anim_ctrl_ptr = (void *)icon_ptr->ctrl_add;
    }

    //set layer
    GUICTRL_SetLcdDevInfo((IGUICTRL_T *)anim_ctrl_ptr,
        &base_ctrl_ptr->lcd_dev_info);

    //set border
    GUIANIM_SetBorder(GUICTRL_GetCtrlHandle((IGUICTRL_T *)anim_ctrl_ptr),&iconlist_ctrl_ptr->theme.unselect_border);

    //set the icon has load
    icon_ptr->is_load = TRUE;

    GUIANIM_SetDefaultIconByPtr(anim_ctrl_ptr,iconlist_ctrl_ptr->theme.default_icon_id,iconlist_ctrl_ptr->theme.error_icon_id);

    //set call back
    call_back.param         = (uint32)iconlist_ctrl_ptr;
    call_back.OwnerDrawFunc = IconAnimCallback;
    GUIANIM_SetOwnerDrawEx(GUICTRL_GetCtrlHandle((IGUICTRL_T*)anim_ctrl_ptr),&call_back);

    //set anim param
    ctrl_info.is_ctrl_id = FALSE;
    ctrl_info.ctrl_ptr   = anim_ctrl_ptr;
   
    if (PNULL != data_info_ptr)
    {
        GUIANIM_SetParam(&ctrl_info,data_info_ptr,PNULL, &tmp_display_info);
    }
    else
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(PNULL != file_info_ptr); /*assert verified*/
        if (PNULL != file_info_ptr)
        {
            tmp_display_info.is_zoom       = iconlist_ctrl_ptr->is_zoom; //file support zoom, data not support
            GUIANIM_SetParam(&ctrl_info,PNULL,file_info_ptr, &tmp_display_info);
        }        
    }

    //get display rect
    if (GUI_IntersectRect(&display_rect,icon_rect,base_ctrl_ptr->display_rect))
    {
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            TRUE);
    }
    else
    {
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            FALSE);
    }
}
/*****************************************************************************/
//  Description : insert icon and display icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertIcon(
                      uint16                index,              //in:
                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr, //in
                      GUIANIM_DATA_INFO_T   *data_info_ptr,     //in,may PNULL
                      GUIANIM_FILE_INFO_T   *file_info_ptr      //in,may PNULL
                      )
{
    void                    *anim_ctrl_ptr  = PNULL;
    GUI_RECT_T              icon_rect 		= {0};
    GUI_RECT_T              display_rect 	= {0};
    GUIICONLIST_ICON_T      *icon_ptr 		= PNULL;
    GUIANIM_CTRL_INFO_T     ctrl_info 		= {0};
    GUIANIM_DISPLAY_INFO_T  display_info 	= {0};
    MMI_CONTROL_CREATE_T    create 			= {0};
    GUIANIM_INIT_DATA_T     init_data 		= {0};
    GUIANIM_CALL_BACK_T     call_back 		= {0};
    CTRLBASE_OBJ_T     		*base_ctrl_ptr 	= PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return;
	}

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    //get icon rect
    icon_rect = ICONLIST_GetIconRect(index,iconlist_ctrl_ptr);

    //get icon pointer
    icon_ptr = &iconlist_ctrl_ptr->item_ptr[index].icon;

    if (PNULL == icon_ptr->ctrl_add)
    {
        //init anim control
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = icon_rect;

        create.guid               = SPRD_GUI_ANIM_ID;
        create.parent_win_handle  = iconlist_ctrl_ptr->win_id;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr      = &init_data;

        anim_ctrl_ptr = MMK_CreateControl( &create );
        SCI_ASSERT(PNULL != anim_ctrl_ptr); /*assert verified*/

        //set anim control address
        icon_ptr->ctrl_add = (uint32)anim_ctrl_ptr;
    }
    else
    {
        anim_ctrl_ptr = (void *)icon_ptr->ctrl_add;
    }

    //set layer
    GUICTRL_SetLcdDevInfo((IGUICTRL_T *)anim_ctrl_ptr,
        &base_ctrl_ptr->lcd_dev_info);

    //set border
    GUIANIM_SetBorder(GUICTRL_GetCtrlHandle((IGUICTRL_T *)anim_ctrl_ptr),&iconlist_ctrl_ptr->theme.unselect_border);

    //set the icon has load
    icon_ptr->is_load = TRUE;

    GUIANIM_SetDefaultIconByPtr(anim_ctrl_ptr,iconlist_ctrl_ptr->theme.default_icon_id,iconlist_ctrl_ptr->theme.error_icon_id);

    //set call back
    call_back.param         = (uint32)iconlist_ctrl_ptr;
    call_back.OwnerDrawFunc = IconAnimCallback;
    GUIANIM_SetOwnerDrawEx(GUICTRL_GetCtrlHandle((IGUICTRL_T*)anim_ctrl_ptr),&call_back);

    //set anim param
    ctrl_info.is_ctrl_id = FALSE;
    ctrl_info.ctrl_ptr   = anim_ctrl_ptr;

    //set display info
    display_info.bg                = iconlist_ctrl_ptr->theme.icon_bg;
    display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_crop_rect      = TRUE;
    display_info.is_disp_one_frame = TRUE;
    
    if (PNULL != data_info_ptr)
    {
        GUIANIM_SetParam(&ctrl_info,data_info_ptr,PNULL,&display_info);
    }
    else
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(PNULL != file_info_ptr); /*assert verified*/
        if (PNULL != file_info_ptr)
        {
            display_info.is_zoom       = iconlist_ctrl_ptr->is_zoom; //file support zoom, data not support
            GUIANIM_SetParam(&ctrl_info,PNULL,file_info_ptr,&display_info);
        }        
    }

    //get display rect
    if (GUI_IntersectRect(&display_rect,icon_rect,base_ctrl_ptr->display_rect))
    {
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            TRUE);
    }
    else
    {
        GUIANIM_SetDisplayRectByPtr(anim_ctrl_ptr,
            &display_rect,
            FALSE);
    }
}

/*****************************************************************************/
//  Description : icon anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void IconAnimCallback(
                            GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                            )
{
    uint16              index 				= 0;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr 	= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T *)owner_draw_ptr->param;

    //get relative index
    if (GetOperateAnim(&index,owner_draw_ptr->ctrl_handle,iconlist_ctrl_ptr))
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(index < iconlist_ctrl_ptr->total_num); /*assert verified*/
        if (index < iconlist_ctrl_ptr->total_num)
        {
            //display selected flag
            DisplaySelectedFlag(index,iconlist_ctrl_ptr);

            //display mark icon
            DisplayMarkIcon(index,iconlist_ctrl_ptr);
        }        
    }
}

/*****************************************************************************/
//  Description : get operate anim index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetOperateAnim(
                             uint16             *index_ptr,         //in/out
                             MMI_HANDLE_T       handle,             //in:
                             CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr  //in:
                             )
{
    BOOLEAN         result 		= FALSE;
    uint16          i 			= 0;
    MMI_HANDLE_T    ctrl_handle = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

    for (i=iconlist_ctrl_ptr->top_index; i<=iconlist_ctrl_ptr->bottom_index; i++)
    {
        //get anim control pointer
        if (0 != iconlist_ctrl_ptr->item_ptr[i].icon.ctrl_add)
        {
            ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconlist_ctrl_ptr->item_ptr[i].icon.ctrl_add);
            if (handle == ctrl_handle)
            {
                if (PNULL != index_ptr)
                {
                    //get item index
                    *index_ptr = i;
                }
                result = TRUE;
                break;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display mark icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayMarkIcon(
                           uint16               index,              //in:
                           CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in:
                           )
{
    uint16          img_width 		= 0;
    uint16          img_height 		= 0;
    GUI_RECT_T      icon_rect 		= {0};
    GUI_RECT_T      display_rect 	= {0};
    GUI_RECT_T      cross_rect 		= {0};
    GUI_RECT_T      img_rect 		= {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr 	= PNULL;
    MMI_IMAGE_ID_T      mark_img_id = iconlist_ctrl_ptr->theme.unmark_img_id;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return;
	}

    if (iconlist_ctrl_ptr->is_markable)
	{
		base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;
        if (iconlist_ctrl_ptr->item_ptr[index].is_selected)
        {
            mark_img_id = iconlist_ctrl_ptr->theme.mark_img_id;
        }
        {
            //get icon display rect
            icon_rect = ICONLIST_GetIconRect(index,iconlist_ctrl_ptr);

            //get image width and height
            GUIRES_GetImgWidthHeight(&img_width,
                &img_height,
                mark_img_id,
                iconlist_ctrl_ptr->win_id);

            //set display rect
            display_rect.left   = (int16)(icon_rect.right - img_width);
            display_rect.top    = (int16)(icon_rect.bottom - img_height);
            display_rect.right  = (int16)(display_rect.left + img_width - 1);
            display_rect.bottom = (int16)(display_rect.top + img_height - 1);

            if (GUI_IntersectRect(&cross_rect,display_rect,base_ctrl_ptr->display_rect))
            {
                //set image rect
                img_rect.left   = (int16)(cross_rect.left - display_rect.left);
                img_rect.top    = (int16)(cross_rect.top - display_rect.top);
                img_rect.right  = (int16)(img_rect.left + cross_rect.right - cross_rect.left);
                img_rect.bottom = (int16)(img_rect.top + cross_rect.bottom - cross_rect.top);

                //display selected image
                GUIRES_DisplayImg(PNULL,
                    &cross_rect,
                    &img_rect,
                    iconlist_ctrl_ptr->win_id,
                    mark_img_id,
                    &base_ctrl_ptr->lcd_dev_info);
            }
        }
	}

    
}

#ifdef GUIF_ICONFOLDER
/*****************************************************************************/
//  Description : set icon number in one folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconNum(
                                      uint16            index,      //in
                                      uint16            icon_num,   //in
                                      MMI_CTRL_ID_T     ctrl_id     //in
                                      )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

    if ((PNULL != iconlist_ctrl_ptr) &&
        (index < iconlist_ctrl_ptr->total_num))
    {
        //inset folder
        InsertFolder(index,icon_num,iconlist_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : insert folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InsertFolder(
                        uint16              index,              //in:
                        uint16              icon_num,           //in:
                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                        )
{
    void                        *iconfolder_ctrl_ptr = PNULL;
    GUI_BG_T                    folder_bg 			 = {0};
    GUI_RECT_T                  icon_rect 			 = {0};
    GUI_RECT_T                  display_rect		 = {0};
    MMI_HANDLE_T                ctrl_handle			 = 0;               
    GUIICONLIST_ICON_T          *icon_ptr			 = PNULL;
    MMI_CONTROL_CREATE_T        create				 = {0};
    CTRLICONFOLDER_INIT_DATA_T  init_data			 = {0};
    CTRLBASE_OBJ_T     			*base_ctrl_ptr		 = PNULL;


	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return;
	}

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    //get icon rect
    icon_rect = ICONLIST_GetIconRect(index,iconlist_ctrl_ptr);

    //get icon pointer
    icon_ptr = &iconlist_ctrl_ptr->item_ptr[index].icon;

    if (PNULL == icon_ptr->ctrl_add)
    {
        //init icon folder control
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = icon_rect;

        create.guid               = SPRD_GUI_ICONFOLDER_ID;
        create.parent_win_handle  = iconlist_ctrl_ptr->win_id;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr      = &init_data;

        iconfolder_ctrl_ptr = MMK_CreateControl(&create);
        SCI_ASSERT(PNULL != iconfolder_ctrl_ptr); /*assert verified*/

        //set anim control address
        icon_ptr->ctrl_add = (uint32)iconfolder_ctrl_ptr;
    }
    else
    {
        iconfolder_ctrl_ptr = (void *)icon_ptr->ctrl_add;
    }

    //set layer
    GUICTRL_SetLcdDevInfo((IGUICTRL_T *)iconfolder_ctrl_ptr,
        &base_ctrl_ptr->lcd_dev_info);

    //set the icon has load
    icon_ptr->is_load = TRUE;

    //disable tp
    IGUICTRL_SetState(iconfolder_ctrl_ptr,GUICTRL_STATE_DISABLE_TP,TRUE);

    //get control handle
    ctrl_handle = GUICTRL_GetCtrlHandle(iconfolder_ctrl_ptr);

    //set bg
    CTRLICONFOLDER_SetBg(ctrl_handle,&folder_bg);

    //set icon number
    CTRLICONFOLDER_SetTotalIcon(ctrl_handle,icon_num,index);

    //get display rect
    if (GUI_IntersectRect(&display_rect,icon_rect,base_ctrl_ptr->display_rect))
    {
        IGUICTRL_SetDisplayRect(iconfolder_ctrl_ptr,
            &display_rect,
            TRUE);
    }
    else
    {
        IGUICTRL_SetDisplayRect(iconfolder_ctrl_ptr,
            &display_rect,
            FALSE);
    }
}
#endif

/*****************************************************************************/
//  Description : get current selected icon index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetCurIconIndex(
                                          MMI_CTRL_ID_T     ctrl_id //in
                                          )
{
    uint16              cur_index		   = 0;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;
    
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        cur_index = iconlist_ctrl_ptr->cur_index;
    }

    return (cur_index);
}

/*****************************************************************************/
//  Description : set current selected icon index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetCurIconIndex(
                                        uint16          cur_index,  //in:
                                        MMI_CTRL_ID_T   ctrl_id     //in:
                                        )
{
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
	
    if ((PNULL != iconlist_ctrl_ptr) &&
        (cur_index < iconlist_ctrl_ptr->total_num) && 
        (cur_index != iconlist_ctrl_ptr->cur_index))
    {
        //set current index
        iconlist_ctrl_ptr->cur_index = cur_index;

        //adjust relative display rect by current index
        if (AdjustDisplayRect(iconlist_ctrl_ptr))
        {
            //get top and bottom index
            GetTopBottomIndex(iconlist_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : adjust relative display rect by current index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL BOOLEAN AdjustDisplayRect(
                                CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                )
{
    BOOLEAN                   result               = FALSE;
    uint16                    display_height       = 0;
    uint16                    cur_row              = 0;
    uint16                    last_row             = 0;
    int32                     display_top          = 0;
    int32                     display_max_top      = 0;
    GUI_BIG_RECT_T            relative_rect        = {0};
	GUI_RECT_T                iconlist_rect        = {0};
    int32                     relative_tmp         = 0;
	int32                     relative_disp_tmp    = 0;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

	//get icon list rect
	iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

#ifdef GUIF_ICONLIST_DELIMITER
    if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
    {
		//get icon relative rect
		relative_rect  = ICONLIST_GetItemToGroupRect(iconlist_ctrl_ptr, iconlist_ctrl_ptr->cur_index);
	}
	else
#endif
	{
		//get icon relative rect
		relative_rect = ICONLIST_GetItemRelativeRect(iconlist_ctrl_ptr->cur_index,
			iconlist_ctrl_ptr,TRUE);
	}

    //selected,adjust relative rect
    if (SELECT_FLAG_WIDTH < relative_rect.top)
    {
        relative_rect.top = relative_rect.top - SELECT_FLAG_WIDTH;
    }
    relative_rect.bottom = relative_rect.bottom + SELECT_FLAG_WIDTH;

#ifdef GUIF_ICONLIST_DELIMITER
	if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
    {
        //焦点在第一行时，且当前index在显示区域内能显示完全的情况下，调整显示位置要从最顶端开始
        if (0 == (iconlist_ctrl_ptr->cur_index / iconlist_ctrl_ptr->col_num_page))
        {
            display_top = 0;

            //set display rect
            result = ICONLIST_SetDispRect(display_top,iconlist_ctrl_ptr);
        }
    }
#endif

    //icon is not display all
    if (!IsRectCovered(iconlist_ctrl_ptr, iconlist_ctrl_ptr->relative_disp_rect, relative_rect))
    {
#ifdef GUIF_ICONLIST_DELIMITER
		if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
		{
			cur_row           = SearchRowOfItemIndex(iconlist_ctrl_ptr, iconlist_ctrl_ptr->cur_index);
			relative_tmp      = relative_rect.top;
			relative_disp_tmp = iconlist_ctrl_ptr->relative_disp_rect.top;		
		}
		else
#endif
		{
			//纵向
			if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
			{
				//get the current icon row index
				cur_row = (uint16)(iconlist_ctrl_ptr->cur_index / iconlist_ctrl_ptr->col_num_page);
				
				relative_tmp      = relative_rect.top;
				relative_disp_tmp = iconlist_ctrl_ptr->relative_disp_rect.top;
			}
			//横向
			else
			{
				cur_row = (uint16)(iconlist_ctrl_ptr->cur_index / iconlist_ctrl_ptr->row_num_page);
				
				relative_tmp      = relative_rect.left;
				relative_disp_tmp = iconlist_ctrl_ptr->relative_disp_rect.left;
			}
		}

        if (relative_tmp < relative_disp_tmp)
        {
            if (0 == cur_row)
            {
                display_top = 0;
            }
            else
            {
                display_top = relative_tmp;
            }
        }
        else
        {
            //get display height
            display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);

#ifdef GUIF_ICONLIST_DELIMITER
			if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
			{
				last_row = SearchRowOfItemIndex(iconlist_ctrl_ptr, (iconlist_ctrl_ptr->total_num - 1));
				//get display height
				display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);
			}
			else
#endif
			{
				if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
				{
					//get display height
					display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);
					//get the last icon row index
					last_row = (uint16)((iconlist_ctrl_ptr->total_num - 1) / iconlist_ctrl_ptr->col_num_page);
				} 
				else
				{
					//get display height
					display_height = ICONLIST_GetDispWidth(iconlist_ctrl_ptr);
					//get the last icon row index
					last_row       = GetTotalColNum(iconlist_ctrl_ptr) - 1;
				}
			}

            //the icon is in the last line
            if (cur_row == last_row)
            {
				if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
				{
					display_top = iconlist_ctrl_ptr->relative_rect.bottom - display_height + 1;
				} 
				else
				{
					display_top = iconlist_ctrl_ptr->relative_rect.right  - display_height + 1;
				}
            }
            else
            {
				if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
				{
					display_top = relative_rect.bottom - display_height + 1;
				} 
				else
				{
					display_top = relative_rect.right  - display_height + 1;
				}
            }
        }

		if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
		{
			//get display max top
			display_max_top = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);
		} 
		else
		{
			//get display max top
			display_max_top = ICONLIST_GetDispMaxLeft(iconlist_ctrl_ptr);
		}
	
        //adjust display top
        display_top = (int32)MIN(display_top,display_max_top);

        //set display rect
        result = ICONLIST_SetDispRect(display_top,iconlist_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : rect1 is covered rect2
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
LOCAL BOOLEAN IsRectCovered(
							CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
                            GUI_BIG_RECT_T      rect1,
                            GUI_BIG_RECT_T      rect2
                            )
{
    BOOLEAN     result = TRUE;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return FALSE;
	}

	//纵向滑动，只需比较top、bottom
	if (GUIICONLIST_LAYOUT_V == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		//only need compare top and bottom
		if ((rect2.top < rect1.top) ||
			(rect2.bottom > rect1.bottom))
		{
			result = FALSE;
		}
	}
	//横向滑动，只需比较left、right
	else
	{
		//only need compare top and bottom
		if ((rect2.left < rect1.left) ||
			(rect2.right > rect1.right))
		{
			result = FALSE;
		}
	}
    return (result);
}

/*****************************************************************************/
//  Description : Set the allowed max select item num
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetMaxSelectedItem(
                                              MMI_CTRL_ID_T ctrl_id,
                                              uint16        selected_max_num
                                              )
{
    BOOLEAN             result 			   = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        iconlist_ctrl_ptr->selected_max_num = selected_max_num;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get icon list can markable or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_GetMarkable(
                                       MMI_CTRL_ID_T    ctrl_id
                                       )
{
    BOOLEAN             result 			   = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        result = iconlist_ctrl_ptr->is_markable;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set icon list can markable or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetMarkable(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       BOOLEAN          is_markable
                                       )
{
    BOOLEAN             result			   = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        if (!is_markable)
        {
            iconlist_ctrl_ptr->selected_max_num = 0;
            CTRLICONLIST_SetAllSelected(ctrl_id,FALSE);
        }
        
        iconlist_ctrl_ptr->is_markable = is_markable;
        result = TRUE;
        //******************加了未选中图标后没刷新不显示未选框***************************
        if(is_markable)
        {
            uint16 i = 0;
            uint16 icon_num_page = iconlist_ctrl_ptr->icon_num_page;
            for(; i<icon_num_page; i++)
            {
                if(i<iconlist_ctrl_ptr->total_num)
                {
                    DisplayMarkIcon(i, iconlist_ctrl_ptr);
                } 
            }
        }
        //**************************************************************************************************
    }

    return (result);
}

/*****************************************************************************/
//  Description : set selected all or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note: if can't select all,it will select the max allow select num
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetAllSelected(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          BOOLEAN           is_selected
                                          )
{
    BOOLEAN             result 				= FALSE;
    uint16              i					= 0;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr	= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        if (((is_selected) && (iconlist_ctrl_ptr->selected_num < iconlist_ctrl_ptr->total_num)) ||
            ((!is_selected) && (0 < iconlist_ctrl_ptr->selected_num)))
        {
            for (i=0; i<iconlist_ctrl_ptr->total_num; i++)
            {
                CTRLICONLIST_SetSelectedItem(ctrl_id,i,is_selected);
            }
        }
        //******************加了全选中图标后没刷新不显示选中框***************************
        if(iconlist_ctrl_ptr->is_markable)
        {
            uint16 i = 0;
            uint16 icon_num_page = iconlist_ctrl_ptr->icon_num_page;
            for(; i<icon_num_page; i++)
            {
                if(i<iconlist_ctrl_ptr->total_num)
                {
                    DisplayMarkIcon(i, iconlist_ctrl_ptr);
                }  
            }
        }
        //**************************************************************************************************

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get selected item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_IsSelectedItem(
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint16        index
                                          )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr 	= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if ((PNULL != iconlist_ctrl_ptr) &&
        (index < iconlist_ctrl_ptr->total_num))
    {
        result = iconlist_ctrl_ptr->item_ptr[index].is_selected;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get selected item indexs
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetSelectedItemIndex(//selected item number
                                               MMI_CTRL_ID_T    ctrl_id,    //in:
                                               uint16*          index_ptr,  //in/out:may PNULL
                                               uint16           array_len   //in:
                                               )
{
    uint16              i 					= 0;
    uint16              selected_num 		= 0;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr 	= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        for (i=0; i<iconlist_ctrl_ptr->total_num; i++)
        {
            if (iconlist_ctrl_ptr->item_ptr[i].is_selected)
            {
                if ((selected_num < array_len) &&
                    (PNULL != index_ptr))
                {
                    index_ptr[selected_num] = i;
                }
                
                selected_num++;
            }
        }
    }

    return (selected_num);
}

/*****************************************************************************/
//  Description : set selected item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetSelectedItem(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           uint16           index,
                                           BOOLEAN          is_selected
                                           )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr	= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if ((PNULL != iconlist_ctrl_ptr) &&
        (index < iconlist_ctrl_ptr->total_num))
    {
        if (is_selected != iconlist_ctrl_ptr->item_ptr[index].is_selected)
        {
            if (is_selected)
            {
                if (iconlist_ctrl_ptr->selected_num < iconlist_ctrl_ptr->selected_max_num)
                {
                    iconlist_ctrl_ptr->selected_num++;

                    iconlist_ctrl_ptr->item_ptr[index].is_selected = is_selected;
                }
            }
            else
            {
                if (0 < iconlist_ctrl_ptr->selected_num)
                {
                    iconlist_ctrl_ptr->selected_num--;

                    iconlist_ctrl_ptr->item_ptr[index].is_selected = is_selected;
                }
            }
#ifdef GUIF_ICONFOLDER
            //set is selected
            if (GUIICONLIST_STYLE_FOLDER_UIDT == iconlist_ctrl_ptr->style)
            {
                CTRLICONFOLDER_SetSelected(GUICTRL_GetCtrlHandle((IGUICTRL_T*)iconlist_ctrl_ptr->item_ptr[index].icon.ctrl_add),is_selected);
            }
#endif
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : remove the appointed item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveItem(
                                      MMI_CTRL_ID_T ctrl_id,
                                      uint16        index
                                      )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr 	= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if ((PNULL != iconlist_ctrl_ptr) &&
        (index < iconlist_ctrl_ptr->total_num))
    {
        //is selected
        if ((iconlist_ctrl_ptr->item_ptr[index].is_selected) &&
            (0 < iconlist_ctrl_ptr->selected_num))
        {
            iconlist_ctrl_ptr->selected_num--;
            iconlist_ctrl_ptr->item_ptr[index].is_selected = FALSE;
        }

        //remove one item
        RemoveOneItem(index,iconlist_ctrl_ptr);

		result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : remove one item
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void RemoveOneItem(
                         uint16             index,
                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                         )
{
	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

	if (index >= iconlist_ctrl_ptr->total_num)
	{
		return;
	}
    //free one item
    FreeOneItemMemory(iconlist_ctrl_ptr,&iconlist_ctrl_ptr->item_ptr[index]);

#ifdef GUIF_ICONLIST_DELIMITER
    iconlist_ctrl_ptr->is_group_rect_dirty = TRUE;
#endif
    
    //modify total number
    if (0 < iconlist_ctrl_ptr->total_num)
    {
        iconlist_ctrl_ptr->total_num--;
        
        //set relative rect
        SetRelativeRect(iconlist_ctrl_ptr);
    }

    /* memmove */
    if (index < iconlist_ctrl_ptr->total_num)
    {
        memmove(iconlist_ctrl_ptr->item_ptr + index,
            iconlist_ctrl_ptr->item_ptr + index + 1,
            (iconlist_ctrl_ptr->total_num - index)* sizeof(GUIICONLIST_ITEM_T));

        SCI_MEMSET(&iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->total_num],0,sizeof(GUIICONLIST_ITEM_T));
    }

    //modify current index
    if (0 < iconlist_ctrl_ptr->cur_index)
    {
        if (iconlist_ctrl_ptr->cur_index > index)
        {
            iconlist_ctrl_ptr->cur_index--;
        }
        if (iconlist_ctrl_ptr->cur_index == iconlist_ctrl_ptr->total_num)
        {
            iconlist_ctrl_ptr->cur_index--;
        }
    }

    //adjust relative display rect by current index
    AdjustDisplayRect(iconlist_ctrl_ptr);

    //get top and bottom index
    GetTopBottomIndex(iconlist_ctrl_ptr);

    if (!IsNeedScroll(iconlist_ctrl_ptr))
    {
        //destroy progress
        DestroyPrgCtrl(iconlist_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : set icon width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconWidthHeight(
                                              MMI_CTRL_ID_T     ctrl_id,  //in
                                              uint16            width,    //in
                                              uint16            height    //in
                                              )
{
    BOOLEAN             result				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr 	= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if ((PNULL != iconlist_ctrl_ptr))
    {
        iconlist_ctrl_ptr->theme.icon_item.width  = width;
        iconlist_ctrl_ptr->theme.icon_item.height = height;

        //get row and column number
        GetRowColNum(iconlist_ctrl_ptr);

        //set relative rect
        SetRelativeRect(iconlist_ctrl_ptr);

        //adjust relative display rect by current index
        AdjustDisplayRect(iconlist_ctrl_ptr);

        //get top and bottom index
        GetTopBottomIndex(iconlist_ctrl_ptr);
        
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 准备动画
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_PrepareSpecial(
                                       MMI_CTRL_ID_T    from_ctrl_id,
                                       MMI_CTRL_ID_T    to_ctrl_id
                                       )
{
#ifdef GUIF_ICONLIST_STUNT
    CTRLICONLIST_OBJ_T  *from_iconlist_ctrl_ptr = PNULL;
    CTRLICONLIST_OBJ_T  *to_iconlist_ctrl_ptr 	= PNULL;
    IGUICTRL_T          *ctrl_ptr 				= PNULL;

    ctrl_ptr = MMK_GetCtrlPtr(from_ctrl_id);
    if (PNULL != ctrl_ptr)
    {
        if (SPRD_GUI_ICONLIST_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
        {
            from_iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T*)ctrl_ptr;
            from_iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_NONE;
        }
    }

    ctrl_ptr = MMK_GetCtrlPtr(to_ctrl_id);
    if (PNULL != ctrl_ptr)
    {
        if (SPRD_GUI_ICONLIST_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
        {
            to_iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T*)ctrl_ptr;
            to_iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_NONE;
        }
    }

    // 如果都是iconlist，则说明是文件夹与文件之间的动画
    if (PNULL != from_iconlist_ctrl_ptr && PNULL != to_iconlist_ctrl_ptr)
    {
        ClearRotateInfo(to_iconlist_ctrl_ptr);

        // 从文件夹到文件的动画数据
        if (GUIICONLIST_STYLE_FOLDER_UIDT == from_iconlist_ctrl_ptr->style
            && (GUIICONLIST_STYLE_ICON == to_iconlist_ctrl_ptr->style 
                || GUIICONLIST_STYLE_ICON_UIDT == to_iconlist_ctrl_ptr->style))
        {
            GetFolder2FileImgInfo(
                    from_iconlist_ctrl_ptr,
                    to_iconlist_ctrl_ptr);
        }
        // 从文件到文件夹的动画数据
        else if ((GUIICONLIST_STYLE_ICON == from_iconlist_ctrl_ptr->style 
                || GUIICONLIST_STYLE_ICON_UIDT == from_iconlist_ctrl_ptr->style)
            && GUIICONLIST_STYLE_FOLDER_UIDT == to_iconlist_ctrl_ptr->style)
        {
            GetFile2FolderImgInfo(
                    from_iconlist_ctrl_ptr,
                    to_iconlist_ctrl_ptr);
        }
    }
    else if (PNULL != from_iconlist_ctrl_ptr)
    {
        if (GUIICONLIST_STYLE_ICON == from_iconlist_ctrl_ptr->style
            || GUIICONLIST_STYLE_ICON_UIDT == from_iconlist_ctrl_ptr->style)
        {
            from_iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_FILE_TO_IMAGE;
        }
    }
    else if (PNULL != to_iconlist_ctrl_ptr)
    {
        if (GUIICONLIST_STYLE_ICON == to_iconlist_ctrl_ptr->style
            || GUIICONLIST_STYLE_ICON_UIDT == to_iconlist_ctrl_ptr->style)
        {
            to_iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_IMAGE_TO_FILE;
        }
    }
#endif
}

#ifdef GUIF_ICONLIST_STUNT
/*****************************************************************************/
// Description : store info for animation from file to folder
// Global resource dependence : 
// Author: hua.fang
// Note:
//  保存从文件到文件夹的动画信息，现在动画由文件夹iconlist来做，所以是将file里的
// 信息保存到folder里，这里只保存位置信息，因为folder里已经有图片和角度信息
/*****************************************************************************/
LOCAL void GetFile2FolderImgInfo(
                                 CTRLICONLIST_OBJ_T  *icon_iconlist_ctrl_ptr,
                                 CTRLICONLIST_OBJ_T  *folder_iconlist_ctrl_ptr
                                 )
{
    uint16                  i 						= 0;
    uint16                  max_icon_num 			= 0;
    uint16                  icon_num 				= 0;
    CTRLICONFOLDER_OBJ_T 	*iconfolder_ctrl_ptr  	= PNULL;
	uint16                  top_index    			= 0;
	uint16                  bottom_index 			= 0;

    if (PNULL != folder_iconlist_ctrl_ptr->item_ptr
        && PNULL != icon_iconlist_ctrl_ptr->item_ptr
        && icon_iconlist_ctrl_ptr->total_num > 0)
    {
        iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*)folder_iconlist_ctrl_ptr->item_ptr[folder_iconlist_ctrl_ptr->cur_index].icon.ctrl_add;

        // 计算最多的icon个数，文件夹iconlist里最多只能容纳这么多个
        max_icon_num = ICONFOLDER_GetMaxIconNum(iconfolder_ctrl_ptr);

		// 防溢出处理
		top_index    = icon_iconlist_ctrl_ptr->top_index;
		bottom_index = icon_iconlist_ctrl_ptr->bottom_index;

		if (top_index > bottom_index)
		{
			bottom_index = top_index;
		}
		
        if ((bottom_index - top_index + 1) >= max_icon_num)
        {
            icon_num = max_icon_num;
        }
        else
        {
            icon_num = (uint16)(icon_iconlist_ctrl_ptr->bottom_index - icon_iconlist_ctrl_ptr->top_index + 1);
        }

        if (icon_num > 0)
        {
            folder_iconlist_ctrl_ptr->rotate_info_ptr = SCI_ALLOC_APP(icon_num * sizeof(GUIICONLIST_ROTATE_INFO_T));
            SCI_MEMSET(folder_iconlist_ctrl_ptr->rotate_info_ptr, 0, icon_num * sizeof(GUIICONLIST_ROTATE_INFO_T));

            icon_iconlist_ctrl_ptr->rotate_info_count = icon_num;

            // 保留位置信息
            for (i = 0; i < icon_num; i++)
            {
                folder_iconlist_ctrl_ptr->rotate_info_ptr[i].dis_rect = 
                        ICONLIST_GetIconRect((uint16)(icon_iconlist_ctrl_ptr->top_index + i),icon_iconlist_ctrl_ptr);
            }

            folder_iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_FILE_TO_FOLDER;
        }
    }
}

/*****************************************************************************/
//  Description : 保存folder到file的信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetFolder2FileImgInfo(
                                 CTRLICONLIST_OBJ_T  *folder_iconlist_ctrl_ptr,
                                 CTRLICONLIST_OBJ_T  *icon_iconlist_ctrl_ptr
                                 )
{
    uint16                  i 					  = 0;
    uint16                  total_num			  = 0;
    CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr  = PNULL;
    CTRLBASE_OBJ_T*         anim_ctrl_ptr		  = PNULL;

    if (PNULL != folder_iconlist_ctrl_ptr->item_ptr
        && 0 < folder_iconlist_ctrl_ptr->total_num)
    {
        iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*)folder_iconlist_ctrl_ptr->item_ptr[folder_iconlist_ctrl_ptr->cur_index].icon.ctrl_add;
        total_num = ICONFOLDER_GetTotalNum(iconfolder_ctrl_ptr);

        if (total_num > 0)
        {
            icon_iconlist_ctrl_ptr->rotate_info_ptr = SCI_ALLOC_APP(total_num * sizeof(GUIICONLIST_ROTATE_INFO_T));
            SCI_MEMSET(icon_iconlist_ctrl_ptr->rotate_info_ptr, 0, total_num * sizeof(GUIICONLIST_ROTATE_INFO_T));

            icon_iconlist_ctrl_ptr->rotate_info_count = total_num;

            // 保留各个icon的信息
            for (i = 0; i < total_num; i++)
            {
                anim_ctrl_ptr = ICONFOLDER_GetAnimCtrl(iconfolder_ctrl_ptr, i);

                icon_iconlist_ctrl_ptr->rotate_info_ptr[i].index = i;
                icon_iconlist_ctrl_ptr->rotate_info_ptr[i].rotate_angle = ICONFOLDER_GetIconAngle(i, iconfolder_ctrl_ptr);
                icon_iconlist_ctrl_ptr->rotate_info_ptr[i].dis_rect = ICONFOLDER_GetIconRect(i, iconfolder_ctrl_ptr);

                if (PNULL != anim_ctrl_ptr)
                {
                    GUIANIM_GetImgBuf(
                            ((CTRLBASE_OBJ_T*)anim_ctrl_ptr)->handle,
                            &icon_iconlist_ctrl_ptr->rotate_info_ptr[i].img_info.img_bitmap,
                            &icon_iconlist_ctrl_ptr->rotate_info_ptr[i].img_info.img_id);
                }
                else
                {
                    icon_iconlist_ctrl_ptr->rotate_info_ptr[i].img_info.img_id = icon_iconlist_ctrl_ptr->theme.default_icon_id;
                }
            }

            icon_iconlist_ctrl_ptr->specail_type = GUIICONLIST_SPECIAL_FOLDER_TO_FILE;
        }
    }
}

/*****************************************************************************/
//  Description : 清除存储的旋转信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ClearRotateInfo(
                           CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                           )
{
    if (PNULL != iconlist_ctrl_ptr->rotate_info_ptr)
    {
        SCI_FREE(iconlist_ctrl_ptr->rotate_info_ptr);
    }

    iconlist_ctrl_ptr->rotate_info_count = 0;
}

#endif

#ifdef DYNAMIC_MODULE_SUPPORT    
//added by andrew.zhang at 2009/03/16 for caf
/*****************************************************************************/
//  Description:   set total item count
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//      total_num:total item count
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IconListCtrlSetTotalItem( IGUIICONLIST_T * me_ptr, uint16 total_num )
{
    CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T *)me_ptr;

    if (PNULL != iconlist_ctrl_ptr)
    {
        GUIICONLIST_SetTotalIcon(iconlist_ctrl_ptr->handle, total_num);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description:   set current index
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//      selected_index:index
//  Return:
//       TRUE:success;FALSE:fail
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IconListCtrlSetCurIndex( IGUIICONLIST_T * me_ptr, uint16 selected_index )
{
    CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T *)me_ptr;

    if (PNULL != iconlist_ctrl_ptr)
    {
        GUIICONLIST_SetCurIconIndex(selected_index, iconlist_ctrl_ptr->handle );
    }

    return TRUE;
}

/*****************************************************************************/
//  Description:   get current index
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//  Return:
//       current index
//  Note: 
/*****************************************************************************/
LOCAL uint16 IconListCtrlGetCurIndex( IGUIICONLIST_T * me_ptr )
{
    uint16              cur_index = 0;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = (CTRLICONLIST_OBJ_T *)me_ptr;

    if (PNULL != iconlist_ctrl_ptr)
    {
        cur_index = GUIICONLIST_GetCurIconIndex( iconlist_ctrl_ptr->handle );
    }

    return (cur_index);
}

/*****************************************************************************/
//  Description:   add icon item display infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//      icon_index:icon item index
//      icon_ptr:icon display infomation
//  Return:
//       current index
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IconListCtrlAppendItem( IGUIICONLIST_T * me_ptr, uint16 icon_index, CAF_ICONLIST_ICON_DISP_T *icon_ptr )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr 	= (CTRLICONLIST_OBJ_T *)me_ptr;

    if (PNULL != iconlist_ctrl_ptr)
    {
        GUIICONLIST_AppendText( icon_index, iconlist_ctrl_ptr->handle, (GUIICONLIST_DISP_T *)icon_ptr );

        result = TRUE;
    }

    return (result);
}

#ifdef DYNAMIC_MODULE_SUPPORT  
/*****************************************************************************/
//  Description : 类型转化
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_TYPE_E ConvertAnimTypeFromCAFToGUI(
                                                 CAF_ANIM_TYPE_E caf_type
                                                 )
{
    GUIANIM_TYPE_E gui_type = GUIANIM_TYPE_NONE;

    switch(caf_type)
    {
    case CAF_ANIM_TYPE_NONE:
        gui_type = GUIANIM_TYPE_NONE;
        break;
    case CAF_ANIM_TYPE_UNKNOW:
        gui_type = GUIANIM_TYPE_UNKNOWN;
        break;
    case CAF_ANIM_TYPE_IMAGE_BMP:
        gui_type = GUIANIM_TYPE_IMAGE_BMP;
        break;
    case CAF_ANIM_TYPE_IMAGE_WBMP:
        gui_type = GUIANIM_TYPE_IMAGE_WBMP;
        break;    
    case CAF_ANIM_TYPE_IMAGE_GIF:
        gui_type = GUIANIM_TYPE_IMAGE_GIF;
        break;
    case CAF_ANIM_TYPE_IMAGE_JPG:
        gui_type = GUIANIM_TYPE_IMAGE_JPG;
        break;
    case CAF_ANIM_TYPE_IMAGE_PNG:
        gui_type = GUIANIM_TYPE_IMAGE_PNG;
        break;
    case CAF_ANIM_TYPE_IMAGE_WEBP:
        gui_type = GUIANIM_TYPE_IMAGE_WEBP;
        break;
    case CAF_ANIM_TYPE_ANIM:
        gui_type = GUIANIM_TYPE_ANIM;
        break;
    default:
        gui_type = GUIANIM_TYPE_MAX;
        break;
    }

    return gui_type;
}
#endif

/*****************************************************************************/
//  Description:   add icon item data infomation
//  Author: Andrew.Zhang
//  Param
//       me_ptr :control object
//      icon_index:icon item index
//      data_ptr:icon item data
//      resource_source:icon data source type
//  Return:
//       current index
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IconListCtrlAppendIcon( IGUIICONLIST_T * me_ptr, uint16 icon_index, void * data_ptr,  CAF_RESOURCE_SOURCE_E resource_source )
{
    CTRLICONLIST_OBJ_T      *iconlist_ctrl_ptr  = (CTRLICONLIST_OBJ_T *)me_ptr;
    GUIANIM_DATA_INFO_T     data_info; 
    GUIANIM_FILE_INFO_T     file_info;
    GUIANIM_DATA_INFO_T     *data_info_ptr 		= NULL; 
    GUIANIM_FILE_INFO_T     *file_info_ptr 		= NULL;
    BOOLEAN 				result 				= TRUE;
    uint16 					*sysfilename_ptr 	= NULL;

    if (PNULL != iconlist_ctrl_ptr);
    {
        memset(&data_info, 0, sizeof(data_info));
        memset(&file_info, 0, sizeof(file_info));
    
        if(resource_source == CAF_RESOURCE_SOURCE_FILE) //图像来自单独的文件
        { 
            file_info_ptr = &file_info; 
            file_info.full_path_wstr_ptr = ((CAF_ANIM_FILE_INFO_T *)data_ptr)->full_path_wstr_ptr;
            file_info.full_path_wstr_len = ((CAF_ANIM_FILE_INFO_T *)data_ptr)->full_path_wstr_len;
        
            //转换文件路径到平台路径
            SCI_ASSERT(file_info.full_path_wstr_len <= IFILE_MAX_PATH); /*assert to do*/
            sysfilename_ptr = SCI_ALLOC_APP((file_info.full_path_wstr_len * 2) + 4);
            SCI_ASSERT(TransferFileNameFromCAFToSys(sysfilename_ptr, file_info.full_path_wstr_ptr) == TRUE); /*assert to do*/
            file_info.full_path_wstr_ptr = (wchar *)sysfilename_ptr;
            file_info.full_path_wstr_len = 0;
            while(*sysfilename_ptr++ != 0)
            {
                file_info.full_path_wstr_len++;
            }
        }
        else if(resource_source == CAF_RESOURCE_SOURCE_ID) //图像来自资源包
        {
            data_info_ptr = &data_info;
            if(((CAF_ANIM_ID_INFO_T *)data_ptr)->type == CAF_ANIM_TYPE_ANIM)
                data_info.anim_id = ((CAF_ANIM_ID_INFO_T *)data_ptr)->id;
            //else if(((CAF_ANIM_ID_INFO_T *)data_ptr)->type == CAF_ID_TYPE_IMAGE)
            else
                data_info.img_id = ((CAF_ANIM_ID_INFO_T *)data_ptr)->id;

            //data_info.type = ConvertAnimTypeFromCAFToGUI(((CAF_ANIM_ID_INFO_T *)data_ptr)->type);
        }
        else if(resource_source == CAF_RESOURCE_SOURCE_BUFFER) //图像来自buffer
        {
            data_info_ptr = &data_info;
            data_info.data_ptr = ((CAF_ANIM_BUFFER_INFO_T *)data_ptr)->data_ptr;
            data_info.data_size = ((CAF_ANIM_BUFFER_INFO_T *)data_ptr)->data_size;
        }
        else
        {
            //SCI_TRACE_LOW:"IconListCtrlAppendIcon d%"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIICONLIST_5711_112_2_18_3_19_14_176,(uint8*)"", resource_source);
        }

        result = GUIICONLIST_AppendIcon( icon_index, iconlist_ctrl_ptr->handle, data_info_ptr, file_info_ptr );

        if(resource_source == CAF_RESOURCE_SOURCE_FILE)  //图像来自单独的文件
        {
            SCI_FREE(sysfilename_ptr);
        }
    }

    return result;
}
//end 
#endif

/*******************************************************************************************/
//  Description : 获取总列数，即一行元素的总个数,仅限横向滑动使用
//  Global resource dependence :
//  Author: xiyuan 2011.12
//  Note:
/*********************************************************************************************/
LOCAL uint16 GetTotalColNum(
							CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr
							)
{
	uint16 col_num = 0;
	
	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

	if (GUIICONLIST_LAYOUT_H == ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr))
	{
		if (0 != iconlist_ctrl_ptr->row_num_page)
		{			
			//总列数
			col_num = iconlist_ctrl_ptr->total_num / iconlist_ctrl_ptr->row_num_page;
			
			//有非整页的情况
			if (0 != (iconlist_ctrl_ptr->total_num % iconlist_ctrl_ptr->row_num_page))
			{
				col_num ++;
			}
		}	
	}
	return col_num;
}

/*****************************************************************************/
// Description : 设置支持横向滑动
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetLayoutStyle(
										  MMI_CTRL_ID_T   ctrl_id,
										  GUIICONLIST_LAYOUT_STYLE_E layout_style
										  )
{
	BOOLEAN             result             = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

	//get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

	if (PNULL != iconlist_ctrl_ptr)
	{
		result = ICONLIST_SetLayoutStyleInter(iconlist_ctrl_ptr, layout_style);
	}

	return result;
}

/*****************************************************************************/
// Description : 是否支持横向滑动
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
PUBLIC GUIICONLIST_LAYOUT_STYLE_E CTRLICONLIST_GetLayoutStyle(
															 MMI_CTRL_ID_T   ctrl_id
															 )
{
    CTRLICONLIST_OBJ_T          *iconlist_ctrl_ptr = PNULL;
	GUIICONLIST_LAYOUT_STYLE_E  layout_style       = GUIICONLIST_LAYOUT_V;

	//get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

	if (PNULL != iconlist_ctrl_ptr)
	{
		layout_style = ICONLIST_GetLayoutStyleInter(iconlist_ctrl_ptr);
	}

    return layout_style;
}

/*****************************************************************************/
//  Description : iconlist横向滑动情况下，按#，*键翻页
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVSlidePageKey(
									   CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
									   MMI_MESSAGE_ID_E   msg_id
									   )
{
	uint16       display_width     = 0;
	int32        display_max_left  = 0;
	int32        display_left      = 0;
	MMI_RESULT_E result            = MMI_RESULT_FALSE;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return MMI_RESULT_FALSE;
	}
											 
	if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
	{
		//get display height
		display_width  = ICONLIST_GetDispWidth(iconlist_ctrl_ptr);
		//get display max top
		display_max_left = ICONLIST_GetDispMaxLeft(iconlist_ctrl_ptr);
		
		switch (msg_id)
		{
		case MSG_APP_STAR:			
			//前翻一页
			display_left = iconlist_ctrl_ptr->relative_disp_rect.left - display_width;
			//第一页
			if (0 >= display_left)
			{
				display_left = 0;
			}
			break;
			
		case MSG_APP_HASH:
			//后翻一页
			display_left = iconlist_ctrl_ptr->relative_disp_rect.left + display_width;
			//最后一页
			if (display_max_left <= display_left)
			{
				display_left = display_max_left;
			}
			break;
		
		default:
			break;
		}
		
		//set display rect
		if (ICONLIST_SetDispRect(display_left, iconlist_ctrl_ptr))
		{
			//get top and bottom index
			GetTopBottomIndex(iconlist_ctrl_ptr);
		}
		
		//display icon list
		DisplayIconList(TRUE,iconlist_ctrl_ptr);
		
		result = MMI_RESULT_TRUE;
	}
											 
	return result;
}
/*****************************************************************************/
// 	Description : set iconlist text font & color
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconListTextInfo(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   GUI_FONT_ALL_T iconlist_text_style //in
											   )
{
	BOOLEAN             result			   = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
	
    if (PNULL != iconlist_ctrl_ptr)
    {			
		//set text font & color
		iconlist_ctrl_ptr->theme.font = iconlist_text_style;
	    //更新一下iconlist的rect,字体大小影响iconlist的RelativeRect
	    SetRelativeRect(iconlist_ctrl_ptr);	
    	
		result = TRUE;
	}

    return result;
}

/*****************************************************************************/
//  Description : Set item space & margin
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconItemSpace(
											MMI_CTRL_ID_T                     ctrl_id,       //in
											GUIICONLIST_MARGINSPACE_INFO_T    margin_space   //in
											)
{
	BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr 	= PNULL;
	
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
	
    if (PNULL != iconlist_ctrl_ptr)
    {			
		//列间距
		iconlist_ctrl_ptr->theme.icon_item.hspace = margin_space.icon_item_hspace;
		//行间距
		iconlist_ctrl_ptr->theme.icon_item.vspace = margin_space.icon_item_vspace;
		//左右边距
		iconlist_ctrl_ptr->theme.hmargin_space = margin_space.iconlist_hmargin;
		//上下边距
		iconlist_ctrl_ptr->theme.vmargin_space = margin_space.iconlist_vmargin;

		result = TRUE;
	}
	
    return result;
}

/*****************************************************************************/
//  Description : 设置item的icon是否支持缩放
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetItemIconZoom(
										   MMI_CTRL_ID_T  ctrl_id,
										   BOOLEAN        is_itemicon_zoom
										   )
{
    BOOLEAN             result             = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;
	
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

	if (PNULL != iconlist_ctrl_ptr)
	{
		iconlist_ctrl_ptr->is_zoom = is_itemicon_zoom;
		result = TRUE;
	}

	return result;
}

/*****************************************************************************/
//  Description : 用户可自定义Item的Border的颜色、风格、粗细
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.1
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetItemBorderStyle(
                                              MMI_CTRL_ID_T  ctrl_id,
                                              BOOLEAN        is_select_border,
                                              GUI_BORDER_T   *select_border
                                              )
{
    BOOLEAN             result             = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;
	
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

	if ((PNULL != iconlist_ctrl_ptr)&&
        (PNULL != select_border))
	{
        if (is_select_border)
        {
            iconlist_ctrl_ptr->theme.select_border  = *select_border;
        } 
        else
        {
            iconlist_ctrl_ptr->theme.unselect_border = *select_border;
        }
		result = TRUE;
	}

	return result;
}  

/*****************************************************************************/
//  Description : 用户可自定义默认图、Error图
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.2
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDefaultIconByPtr(
											   MMI_CTRL_ID_T    ctrl_id,
                                               MMI_IMAGE_ID_T   def_img_id,
                                               MMI_IMAGE_ID_T   err_img_id
											   )
{
    BOOLEAN             result             = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;
	
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

	if (PNULL != iconlist_ctrl_ptr)
	{
	    iconlist_ctrl_ptr->theme.default_icon_id = def_img_id;
	    iconlist_ctrl_ptr->theme.error_icon_id   = err_img_id;
		result = TRUE;
	}

	return result;
}

/*****************************************************************************/
//  Description : 封装对外接口---get icon list relative height,except border
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLICONLIST_GetRelativeHeight(
                                            MMI_CTRL_ID_T    ctrl_id
                                            )
{
	uint32      		relative_height    = 0;
	CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);//get icon list control pointer	

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

	//to get relative height
	relative_height = ICONLIST_GetRelativeHeight(iconlist_ctrl_ptr);

    	return (relative_height);    
}
/*****************************************************************************/
//  Description : 封装对外接口---get icon list display rect height
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetDispHeight(
                                        MMI_CTRL_ID_T    ctrl_id
                                        )
{
	uint16      		display_height 		= 0;
	CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = ICONLIST_GetPtr(ctrl_id);//get icon list control pointer			

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

    	//to get display height
    	display_height = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);

    	return (display_height);
}
#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
//  Description : 获取某位置在哪个group中
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUIICONLIST_GROUP_INFO_T *SearchPosOfGroup(
												 CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr,
												 int32                  position
												 )
{    
	GUI_RECT_T               iconlist_rect = {0};
	MMI_LINK_NODE_T          *tmp_node_ptr = PNULL;
	GUIICONLIST_GROUP_INFO_T *tmp_ptr      = PNULL;
	BOOLEAN                  is_find       = FALSE;

	if(PNULL == iconlist_ctrl_ptr)
	{
		return PNULL;
	}

	//get icon list rect
    iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);
	
	if(0 > position)
	{
		return tmp_ptr;
	}
	else
	{	
		//get head node ptr of group list
		tmp_node_ptr = iconlist_ctrl_ptr->group_list_info.group_list_ptr;
		
		if (PNULL != tmp_node_ptr)
		{
			do 
			{
				tmp_ptr = (GUIICONLIST_GROUP_INFO_T*)tmp_node_ptr->data;
				if (PNULL == tmp_ptr)
				{
					return tmp_ptr;
				}
				
				//if find the position's group
				if ((tmp_ptr->group_relative_rect.top <= position)&&
					(tmp_ptr->group_relative_rect.bottom >= position))
				{
					is_find = TRUE;
					break;
				}
				else
				{
					tmp_node_ptr = tmp_node_ptr->next_ptr;
				}
			} 
			while(tmp_node_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr);
		}
		
		// 如果根本没有找到，则初始化PNULL
		if (!is_find)
		{
			tmp_ptr = PNULL;
		}
	}

	return tmp_ptr;	
}

/*****************************************************************************/
//  Description : 显示分隔符
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL void DisplayDelimiter(
                            BOOLEAN             is_update,
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                            )
{
    uint16          delimiter_total_num = 0;
	GUI_RECT_T      str_disp_rect       = {0};
	GUI_RECT_T      delimiter_rect      = {0};
	GUI_RECT_T      iconlist_rect       = {0};
	GUI_RECT_T      clip_rect           = {0};              //相交区域
	GUISTR_STYLE_T  text_style          = {0};
	MMI_LINK_NODE_T *group_ptr          = PNULL;
	GUI_RECT_T      bg_rect             = {0};
    GUI_BIG_RECT_T  temp_rect           = {0};//iconlist_rect to be int32
    GUI_BIG_RECT_T  cross_rect          = {0};//cross rect of screen&group
	//优化查找次数
	BOOLEAN         now_find_state      = FALSE;
	BOOLEAN         before_find_state   = FALSE;
	CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

	//分隔符总数
	delimiter_total_num     = iconlist_ctrl_ptr->group_list_info.group_num;
	//分隔符字符设置
	text_style.align        = ALIGN_LVMIDDLE;
	text_style.font         = iconlist_ctrl_ptr->theme.delimiter_text_style.font;        //字体
	text_style.font_color   = iconlist_ctrl_ptr->theme.delimiter_text_style.color;       //字色
	iconlist_rect           = ICONLIST_GetRect(iconlist_ctrl_ptr);

	//get head node
	group_ptr   = iconlist_ctrl_ptr->group_list_info.group_list_ptr;
	if(PNULL == group_ptr)
	{
        return;
	}

    temp_rect.top       = (int32)iconlist_rect.top;
    temp_rect.bottom    = (int32)iconlist_rect.bottom;
    temp_rect.left      = (int32)iconlist_rect.left;
    temp_rect.right     = (int32)iconlist_rect.right;

    do 
    {        
        //group in screen
        if (GetCrossRect(&cross_rect,
            ((GUIICONLIST_GROUP_INFO_T*)group_ptr->data)->group_rect,
            temp_rect))
        {
            delimiter_rect  = GetDelimiterRect(iconlist_ctrl_ptr, group_ptr);
            
            //如果delimiter在屏幕区域内
            if (GUI_IntersectRect(&clip_rect, delimiter_rect, iconlist_rect))
            {
                //display delimiter string
                str_disp_rect       = delimiter_rect;
                str_disp_rect.left  = iconlist_ctrl_ptr->theme.delimiter_str_left_space;
                
                //get bg rect
                bg_rect.top     = delimiter_rect.top + iconlist_ctrl_ptr->theme.delimiter_bg_space.top;
                bg_rect.left    = delimiter_rect.left + iconlist_ctrl_ptr->theme.delimiter_bg_space.left;
                bg_rect.bottom  = delimiter_rect.top + iconlist_ctrl_ptr->theme.delimiter_bg_space.height;
                bg_rect.right   = delimiter_rect.left + iconlist_ctrl_ptr->theme.delimiter_bg_space.width;			
                
                //设置的bg区域有效
                if ((bg_rect.top < bg_rect.bottom)&&(bg_rect.left < bg_rect.right))
                {
                    if (GUI_IntersectRect(&bg_rect, clip_rect, bg_rect))
                    {
                        //display delimiter bg
                        DisplayIconListDelimiterBg(iconlist_ctrl_ptr, bg_rect, delimiter_rect);
                    }
                }
                else
                {
                    //display delimiter bg
                    DisplayIconListDelimiterBg(iconlist_ctrl_ptr, clip_rect, delimiter_rect);
                }		
                
                //display delimiter text
                GUISTR_DrawTextToLCDInRect(&(base_ctrl_ptr->lcd_dev_info),
                    (const GUI_RECT_T*)&str_disp_rect,               //显示区域
                    (const GUI_RECT_T*)&clip_rect,                   //裁减区域
                    (const MMI_STRING_T*)&((GUIICONLIST_GROUP_INFO_T*)group_ptr->data)->delimiter_str_info, 
                    &text_style, 
                    GUISTR_STATE_NONE, 
                    GUISTR_TEXT_DIR_AUTO
                    );
            }    
            //优化查找次数,当前group在屏幕区域内
            now_find_state = TRUE;
        }
        else
        {
            //优化查找次数,记录上一次的查找结果,当前group未在屏幕区域内
            before_find_state = now_find_state;
            now_find_state = FALSE;
            
            //优化查找次数,当找到最后一个在屏幕中的group后,退出查找,不再找了
            if(before_find_state)
            {
                break;
            }
        }
        //get next node
        group_ptr = group_ptr->next_ptr;		
    } 
    while(group_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr);
	return;
}

/*****************************************************************************/
//  Description : 获取分隔符的坐标信息
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetDelimiterRect(
								  CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
								  MMI_LINK_NODE_T     *group_ptr
								  )
{	
	GUI_RECT_T          delimiter_rect             = {0};
	GUI_BIG_RECT_T      delimiter_to_group_rect    = {0};
	GUI_RECT_T          iconlist_rect              = {0};

	if ((PNULL == iconlist_ctrl_ptr)||(PNULL == group_ptr))
	{
		return delimiter_rect;
	}
	
	//get icon list rect
	iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);

	//计算icon相对于group的坐标
	delimiter_to_group_rect  = GetDelimiterToGroupRect(iconlist_ctrl_ptr, group_ptr);
	
	delimiter_rect.left   = (int16)(delimiter_to_group_rect.left + iconlist_rect.left);
	delimiter_rect.top    = (int16)(delimiter_to_group_rect.top + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
	delimiter_rect.right  = (int16)(delimiter_to_group_rect.right + iconlist_rect.right);
	delimiter_rect.bottom = (int16)(delimiter_to_group_rect.bottom + iconlist_rect.top - iconlist_ctrl_ptr->relative_disp_rect.top);
	
	return delimiter_rect;
}

/*****************************************************************************/
//  Description : 计算Delimiter相对于group的坐标
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL GUI_BIG_RECT_T GetDelimiterToGroupRect(
											 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
											 MMI_LINK_NODE_T     *group_ptr
											 )
{
	GUI_BIG_RECT_T      delimiter_to_group_rect    = {0};
	GUI_BIG_RECT_T      tmp_rect                   = {0};
 
	if (PNULL == iconlist_ctrl_ptr)
	{
		return delimiter_to_group_rect;
	}
		
	if ((PNULL == group_ptr)||(PNULL == (GUIICONLIST_GROUP_INFO_T  *)group_ptr->data))
	{
       return delimiter_to_group_rect;
	}

      //get group relative rect
	tmp_rect = ((GUIICONLIST_GROUP_INFO_T  *)group_ptr->data)->group_relative_rect;

       //calculate icon rect in the group
	delimiter_to_group_rect.top    = tmp_rect.top;
	delimiter_to_group_rect.bottom = tmp_rect.top + iconlist_ctrl_ptr->theme.delimiter_height;
	delimiter_to_group_rect.left   = tmp_rect.left;
	delimiter_to_group_rect.right  = tmp_rect.right;
	
    return delimiter_to_group_rect;
}

/*****************************************************************************/
//  Description : 根据item index找到它在iconlist的第几行中
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:   
/*****************************************************************************/
LOCAL uint16 SearchRowOfItemIndex(
								  CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
								  uint16 item_index
								  )
{
	uint16                    index_in_group  = 0;
	uint16                    row_in_group    = 0;
	uint16                    row_num_group   = 0;
	uint16                    row_in_iconlist = 0;
	MMI_LINK_NODE_T           *tmp_node       = PNULL;
	GUIICONLIST_GROUP_INFO_T  *tmp_ptr        = PNULL;

	tmp_node = ICONLIST_SearchGroupInListByIndex(iconlist_ctrl_ptr, item_index);
	//防空判断
	if (PNULL != tmp_node)
	{
	    tmp_ptr        = (GUIICONLIST_GROUP_INFO_T*)tmp_node->data;
		if (PNULL == tmp_ptr)
		{
			return row_in_iconlist;
		}
		index_in_group = item_index - tmp_ptr->start_item_index +1;

		//是group内的第几行
		row_in_group   = index_in_group/iconlist_ctrl_ptr->col_num_page;
		if (0 != index_in_group%iconlist_ctrl_ptr->col_num_page)
		{
			row_in_group++;
		}
		
		if (tmp_node == iconlist_ctrl_ptr->group_list_info.group_list_ptr)
		{
			row_in_iconlist = 0;
		}
		else
		{
			//从当前的group开始计算，算到第一个group，计算这个item index是第几行
			do
			{
				tmp_node        = tmp_node->prev_ptr;
				tmp_ptr         = (GUIICONLIST_GROUP_INFO_T*)tmp_node->data;
				if (PNULL == tmp_ptr)
				{
					return row_in_iconlist;
				}
				row_num_group   = tmp_ptr->group_item_total_num/iconlist_ctrl_ptr->col_num_page;
				if (0 != (tmp_ptr->group_item_total_num % iconlist_ctrl_ptr->col_num_page))
				{
					row_num_group++;
				}
				row_in_iconlist += row_num_group;		
			}
			while(tmp_node != iconlist_ctrl_ptr->group_list_info.group_list_ptr);
		}

		row_in_iconlist += row_in_group - 1;//从第0行开始算起，所以-1
	}

	return row_in_iconlist;	
}

/*****************************************************************************/
//  Description : remove one group node without redisplay
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.03
//  Note: 
/*****************************************************************************/
LOCAL void RemoveOneDelimiter(
                              CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr, 
                              uint16             index
                              )
{
    MMI_LINK_NODE_T          *node_ptr             = PNULL;

	if ((PNULL == iconlist_ctrl_ptr)||
		(index >= iconlist_ctrl_ptr->group_list_info.group_num))
	{
		return;
	}
    
    //find one group node
    node_ptr = ICONLIST_SearchGroupInListById(iconlist_ctrl_ptr, index);
	
	if (PNULL == node_ptr)
	{
		return;
	}
    
	iconlist_ctrl_ptr->group_list_info.group_list_ptr = 
		MMILINK_DestroyNode(node_ptr, iconlist_ctrl_ptr->group_list_info.group_list_ptr, ICONLIST_DestroyIconListGroupNode);
	iconlist_ctrl_ptr->group_list_info.group_num--;
    iconlist_ctrl_ptr->is_group_rect_dirty = TRUE;
}

/*****************************************************************************/
//  Description : iconlist有分隔符的情况下，按#，*键翻页
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDelimiterPageKey(
										  CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
										  MMI_MESSAGE_ID_E   msg_id
										 )
{
	uint16       display_height  = 0;
	int32        display_max_top = 0;
	int32        display_top     = 0;
	MMI_RESULT_E result          = MMI_RESULT_FALSE;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return MMI_RESULT_FALSE;
	}

	if (MMK_TP_SLIDE_NONE == iconlist_ctrl_ptr->slide_state)
	{
		//get display height
		display_height  = ICONLIST_GetDispHeight(iconlist_ctrl_ptr);
		//get display max top
		display_max_top = ICONLIST_GetDispMaxTop(iconlist_ctrl_ptr);
		
		switch (msg_id)
		{
		case MSG_APP_STAR:			
			//上翻一页
			display_top = iconlist_ctrl_ptr->relative_disp_rect.top - display_height;
			//第一页
			if (0 >= display_top)
			{
				display_top	= 0;
			}
			//最后一页
// 			else if (display_max_top <= display_top)
// 			{
// 				display_top = display_max_top;
// 			}
			break;
			
		case MSG_APP_HASH:
			//下翻一页
			display_top = iconlist_ctrl_ptr->relative_disp_rect.top + display_height;
			//第一页
// 			if (0 >= display_top)
// 			{
// 				display_top	= 0;
// 			}
			//最后一页
			if (display_max_top <= display_top)
			{
				display_top = display_max_top;
			}
			break;
		
		default:
			break;
		}
		
		//set display rect
		if (ICONLIST_SetDispRect(display_top,iconlist_ctrl_ptr))
		{
			//get top and bottom index
			GetTopBottomIndex(iconlist_ctrl_ptr);
		}
		
		//display icon list
		DisplayIconList(TRUE,iconlist_ctrl_ptr);
		
		result = MMI_RESULT_TRUE;
	}
											 
	return result;
}

/*****************************************************************************/
// 	Description : 背景图片区域比背景小时，设置delimiter bg space
//	Global resource dependence : 
//  Author: xiyuan.ma 2012.05
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterBgSpace(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   DELIMITER_BG_SPACE_T *delimiter_bg_space //in
											   )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
		if (PNULL != delimiter_bg_space)
		{
			//set bg
			iconlist_ctrl_ptr->theme.delimiter_bg_space.top    = delimiter_bg_space->top;
			iconlist_ctrl_ptr->theme.delimiter_bg_space.left   = delimiter_bg_space->left;
			iconlist_ctrl_ptr->theme.delimiter_bg_space.width= delimiter_bg_space->width;			
			iconlist_ctrl_ptr->theme.delimiter_bg_space.height  = delimiter_bg_space->height;
			result = TRUE;
		}
	}

    return result;
}

/*****************************************************************************/
// 	Description : set delimiter background
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterBg(
										  MMI_CTRL_ID_T  ctrl_id,//in
										  GUI_BG_T       *bg_ptr //in
										  )
{
    BOOLEAN             result 				= FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
		if (PNULL != bg_ptr)
		{
			//set bg
			iconlist_ctrl_ptr->theme.delimiter_bg = *bg_ptr;
			result = TRUE;
		}
	}

    return result;
}

/*****************************************************************************/
// 	Description : set delimiter string info
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterStrInfo(
												MMI_CTRL_ID_T  ctrl_id,//in
												GUI_FONT_ALL_T *delimiter_text_style_ptr, //in
												uint16         delimiter_str_left_space
												)
{
	BOOLEAN             result = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;
	
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
		if (PNULL != delimiter_text_style_ptr)
		{			
			//set bg
			iconlist_ctrl_ptr->theme.delimiter_text_style = *delimiter_text_style_ptr;
			//set str left space
			iconlist_ctrl_ptr->theme.delimiter_str_left_space = delimiter_str_left_space;
			
			result = TRUE;
		}
	}
	
    return result;
}

/*****************************************************************************/
// 	Description : set delimiter string info
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterHeight(
											  MMI_CTRL_ID_T  ctrl_id,//in
											  uint16         delimiter_height //in
											  )
{
	BOOLEAN             result = FALSE;
    CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr = PNULL;
	
	//get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
        //set delimiter_height
        iconlist_ctrl_ptr->theme.delimiter_height = delimiter_height;
		
        result = TRUE;
    }
	return result;
}
/*****************************************************************************/
//  Description : 显示分隔符背景
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
LOCAL void DisplayIconListDelimiterBg(
									  CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
									  GUI_RECT_T         delimiter_bg_rect,//in,delimiter_bg显示区域(裁剪后的)
									  GUI_RECT_T         delimiter_rect//in,delimiter显示区域
									  )
{
    GUI_RECT_T          img_rect = {0};//image rect,may PNULL代表从图片(0,0)点显示
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T     *base_ctrl_ptr = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return;
	}

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    /* 图片显示区域应为delimiter_bg_rect相对于delimiter_rect的位置；cr id :NEWMS00207715*/
    //set image rect
    img_rect.left   = (int16)(delimiter_bg_rect.left - delimiter_rect.left);
    img_rect.top    = (int16)(delimiter_bg_rect.top  -  delimiter_rect.top);
    img_rect.right  = (int16)(img_rect.left + delimiter_bg_rect.right - delimiter_bg_rect.left);
    img_rect.bottom = (int16)(img_rect.top + delimiter_bg_rect.bottom - delimiter_bg_rect.top);
	
    //set bg display info
    bg_display.is_transparent = FALSE;
    bg_display.rect           = delimiter_rect;//代表指定rect区域显示
    bg_display.display_rect   = delimiter_bg_rect;//代表指定起始点显示
    bg_display.img_rect       = img_rect;//image rect,may PNULL代表从图片(0,0)点显示
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = iconlist_ctrl_ptr->win_id;
    
    //display bg
    GUI_DisplayBg(&iconlist_ctrl_ptr->theme.delimiter_bg,
        &bg_display,
        &base_ctrl_ptr->lcd_dev_info);

	return;
}

/*****************************************************************************/
// Description : 设置iconlist的每组的信息
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_CreateGroup(
									   MMI_CTRL_ID_T                ctrl_id,         //in
									   GUIICONLIST_DELIMITER_INFO_T *group_info_ptr	 //in
									   )
{
	BOOLEAN                  result                = TRUE;
    MMI_LINK_NODE_T          *new_node_ptr         = PNULL;
	CTRLICONLIST_OBJ_T       *iconlist_ctrl_ptr    = PNULL;

    if(PNULL == group_info_ptr)
    {
        return FALSE;
    }
	
	//get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

    if (PNULL == iconlist_ctrl_ptr)
    {
        return FALSE;
    }

    if (group_info_ptr->start_item_index >= iconlist_ctrl_ptr->total_num)
    {
        return FALSE;    
    }

    //get the new group ptr
    new_node_ptr = ICONLIST_IconListAddGroup(iconlist_ctrl_ptr, group_info_ptr);
    if(PNULL ==  new_node_ptr)
    {
        return FALSE;
    }

    iconlist_ctrl_ptr->is_group_rect_dirty = TRUE;
    
	return result;
}

/*****************************************************************************/
//  Description : 删除某个分隔符
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveGroup(
									   MMI_CTRL_ID_T      ctrl_id, 
									   uint16             index
									   )
{
	uint16                   i                     = 0;
	//uint16                   item_total_num        = 0;
	BOOLEAN                  result                = FALSE;
	MMI_LINK_NODE_T          *node_ptr             = PNULL;
	//MMI_LINK_NODE_T          *is_search_group      = PNULL;
	CTRLICONLIST_OBJ_T       *iconlist_ctrl_ptr    = PNULL;
	MMI_LINK_NODE_T          *temp_ptr             = PNULL;	
	
	//get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

    if (PNULL != iconlist_ctrl_ptr)
    {
		if (index >= iconlist_ctrl_ptr->group_list_info.group_num)
		{
			return FALSE;
		}
		
        if (index < iconlist_ctrl_ptr->group_list_info.group_num)
        {
			//get delete node's next
			node_ptr = ICONLIST_SearchGroupInListById(iconlist_ctrl_ptr, index);		       
			if(PNULL == node_ptr)
			{
				return FALSE;
			}
			
			temp_ptr = node_ptr;
			
			//update group_id from delete node's next to end
			do
			{
				((GUIICONLIST_GROUP_INFO_T *)node_ptr->next_ptr->data)->group_id--;
				//next one
				temp_ptr = temp_ptr->next_ptr;			
			}
			while(temp_ptr != node_ptr);
				
			//update item number in group
			if(node_ptr != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
			{
				((GUIICONLIST_GROUP_INFO_T *)node_ptr->prev_ptr->data)->group_item_total_num += ((GUIICONLIST_GROUP_INFO_T *)node_ptr->data)->group_item_total_num;
			}
				
			//delete the node		 
			RemoveOneDelimiter(iconlist_ctrl_ptr, index);
			
			//在删除一个分隔符后，更新一下iconlist的rect
			SetRelativeRect(iconlist_ctrl_ptr);
			
			//adjust relative display rect by current index
			AdjustDisplayRect(iconlist_ctrl_ptr);
			
			//get top and bottom index
			GetTopBottomIndex(iconlist_ctrl_ptr);
			
			//display all icon
			for (i = iconlist_ctrl_ptr->top_index; i <= iconlist_ctrl_ptr->bottom_index; i++)
			{
				//display icon
				DisplayItemIcon(TRUE, i, iconlist_ctrl_ptr);
				
				//display item text
				DisplayItemText(i, iconlist_ctrl_ptr);
			}
			
			if (PNULL != iconlist_ctrl_ptr->group_list_info.group_list_ptr)
			{
				DisplayDelimiter(TRUE,iconlist_ctrl_ptr);
			}
			
			if (!IsNeedScroll(iconlist_ctrl_ptr))
			{
				//destroy progress
				DestroyPrgCtrl(iconlist_ctrl_ptr);	
			}
			
			if (0 == iconlist_ctrl_ptr->group_list_info.group_num)
			{
				iconlist_ctrl_ptr->group_list_info.group_list_ptr = PNULL;
				iconlist_ctrl_ptr->group_list_info.group_num      = 0;
			}
			
			result = TRUE;
        }
    }
	return result;
}

/*****************************************************************************/
//  Description : remove all delimiter
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.03
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveAllDelimiter(
                                              MMI_CTRL_ID_T      ctrl_id
                                              )
{    
    DestroyIconListGroupLink(ICONLIST_GetPtr(ctrl_id));
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 找到指定位置内的第一个或者最后一个item index
//  Global resource dependence : 
//  Author: xiyuan edit 2011.12
//  Note:   
/*****************************************************************************/
LOCAL uint16 GetItemIndexInPosRect(
								   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
								   BOOLEAN             is_under_pos_first_index,
                                   int32               pos
								   )
{
	uint16                   line_index            = 0;
	uint16                   top_index             = 0;
	uint16                   bottom_index          = 0;
	uint16                   find_index            = 0;
	int32                    group_height          = 0;
	GUI_RECT_T               iconlist_rect         = {0};
	GUIICONLIST_GROUP_INFO_T *tmp_ptr              = PNULL;
	uint16                   group_end_index       = 0;
	MMI_LINK_NODE_T          *first_node_ptr       = PNULL;
	MMI_LINK_NODE_T          *last_node_ptr        = PNULL;

	if (PNULL == iconlist_ctrl_ptr)
	{
		return 0;
	}

	//防越界
	//位置超过最后一个组的最下边
	//get last node ptr of group list
	last_node_ptr = iconlist_ctrl_ptr->group_list_info.group_list_ptr->prev_ptr;
	if (PNULL == last_node_ptr)
	{
		return 0;
	}
	tmp_ptr = (GUIICONLIST_GROUP_INFO_T*)last_node_ptr->data;
	if (PNULL == tmp_ptr)
	{
		return 0;
	}

	if ((pos > tmp_ptr->group_relative_rect.bottom))
	{
		if (0 == iconlist_ctrl_ptr->total_num)
		{
			find_index = 0;
		} 
		else
		{
			find_index = iconlist_ctrl_ptr->total_num -1;
		}

		return find_index;
	}

	//位置超过第一个组的最上边
	first_node_ptr  = iconlist_ctrl_ptr->group_list_info.group_list_ptr;
	if (PNULL == first_node_ptr)
	{
		return 0;
	}
	tmp_ptr = (GUIICONLIST_GROUP_INFO_T*)first_node_ptr->data;
	if (PNULL == tmp_ptr)
	{
		return 0;
	}

	if ((pos < tmp_ptr->group_relative_rect.top))
	{
		find_index = 0;
		
		return find_index;
	}

	//正常情况
	//查找disp top在哪个group中
	tmp_ptr = SearchPosOfGroup(iconlist_ctrl_ptr, pos);

	if (PNULL == tmp_ptr)
	{
		return find_index;
	}
	
	//get icon list rect
    iconlist_rect = ICONLIST_GetRect(iconlist_ctrl_ptr);
    //get group end index
    group_end_index = tmp_ptr->start_item_index + tmp_ptr->group_item_total_num -1;
	
	//pos - group top < 分隔符高度，则pos在group的dilimiter区域里，第一个index为group的第一个index
	if ((pos - tmp_ptr->group_relative_rect.top) <= iconlist_ctrl_ptr->theme.delimiter_height)		
	{
		//寻找在pos下的第一个index
		if (is_under_pos_first_index)
		{
			find_index = tmp_ptr->start_item_index;
		}
		//寻找在pos上的最后一个index
		else
		{
			if (0 == tmp_ptr->start_item_index)
			{
                find_index = 0;
			}
			else
			{
				find_index = tmp_ptr->start_item_index - 1;//bottom
			}	
		}
	}
	//pos - group top >= 分隔符高度，则disp_top在group的item区域里,第一个index为disp top以下的item区域内的index
	else 
	{
		//计算disp bottom和group top之间去除delimter后的高度
		group_height = pos - tmp_ptr->group_relative_rect.top - iconlist_ctrl_ptr->theme.delimiter_height;

		line_index = group_height/ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr);

		//寻找在pos下的第一个index
		if (is_under_pos_first_index)
		{            
            //dis_top在item上，为了将半行显示出来，半行也算在disp之内
            if (0 != group_height%ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr))
            {                
				if (0 == (line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index))
				{
					top_index = 0;
				} 
				else
				{
					//计算整行
					top_index = line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index - 1;
					//计算半行
					top_index = top_index + 1;

					if (top_index >= iconlist_ctrl_ptr->total_num)
					{
						if (0 == iconlist_ctrl_ptr->total_num)
						{
							top_index = 0;
						} 
						else
						{
							top_index = iconlist_ctrl_ptr->total_num -1;
						}
					}
				}
            } 
            else
            {
				if (0 == (line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index))
				{
					top_index = 0;
				}
				else
				{
					//计算整行
					top_index = line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index - 1;
					//防越界判断，当前行不满的情况         
					if (top_index > group_end_index)
					{
						top_index = group_end_index;
					}
					top_index = top_index + 1;
					
					if (top_index >= iconlist_ctrl_ptr->total_num)
					{
						if (0 == iconlist_ctrl_ptr->total_num)
						{
							top_index = 0;
						} 
						else
						{
							top_index = iconlist_ctrl_ptr->total_num -1;
						}
					}
				}	
            }

			find_index = top_index;
		}
		//寻找在pos上的最后一个index
		else
		{		
			//get the top index，为了将半行显示出来，半行也算disp内的			
			if (0 != group_height%ICONLIST_GetItemHeight(TRUE,iconlist_ctrl_ptr))
			{
				if (0 == (line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index))
				{
                    bottom_index = 0;
				} 
				else
				{
					//计算整行
					bottom_index = line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index - 1;
				}
				//计算半行
				bottom_index = bottom_index + iconlist_ctrl_ptr->col_num_page;
				//防越界判断
				if (bottom_index > group_end_index)
				{
					bottom_index = group_end_index;
				}
				
				if (bottom_index >= iconlist_ctrl_ptr->total_num)
				{
					if (0 == iconlist_ctrl_ptr->total_num)
					{
						bottom_index = 0;
					} 
					else
					{
						bottom_index = iconlist_ctrl_ptr->total_num -1;
					}
				}
			}
            else
            {
				if (0 == (line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index))
				{
					bottom_index = 0;
				} 
				else
                {
					//计算整行
					bottom_index = line_index * iconlist_ctrl_ptr->col_num_page + tmp_ptr->start_item_index - 1;
					//防越界判断
					if (bottom_index > group_end_index)
					{
						bottom_index = group_end_index;
					}
					
					if (bottom_index >= iconlist_ctrl_ptr->total_num)
					{
						if (0 == iconlist_ctrl_ptr->total_num)
						{
							bottom_index = 0;
						} 
						else
						{
							bottom_index = iconlist_ctrl_ptr->total_num -1;
						}
					}
				}

            }
            
			find_index = bottom_index;
		}
	}	

	return find_index;
}

/*****************************************************************************/
//  Description : 获取delimiter group num
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.02
//  Note: 
/*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetDelimiterGroupNum(
                                               MMI_CTRL_ID_T      ctrl_id
                                               )
{
    CTRLICONLIST_OBJ_T       *iconlist_ctrl_ptr    = PNULL;
    uint16                   delimiter_group_num   = 0;
    
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    
    if (PNULL != iconlist_ctrl_ptr)
    {
        delimiter_group_num = iconlist_ctrl_ptr->group_list_info.group_num;       
    }

    return delimiter_group_num;
}
#endif

/*****************************************************************************/
//  Description : 设置是否显示softkey
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.02
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDispCustomerSoftkey(
                                                  MMI_CTRL_ID_T ctrl_id, 
                                                  BOOLEAN       is_true
                                                  )
{
    BOOLEAN            result                = FALSE;
    CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr    = PNULL;
    
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    
    if (PNULL != iconlist_ctrl_ptr)
    {
        iconlist_ctrl_ptr->theme.is_display_softkey = is_true;
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : iconlist是否支持滑动，否则不支持
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.02
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetSlideState(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         BOOLEAN       is_support_slide
                                         )
{
    BOOLEAN            result                = FALSE;
    CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr    = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    
    if (PNULL != iconlist_ctrl_ptr)
    {
        iconlist_ctrl_ptr->theme.is_support_slide = is_support_slide;
        result = TRUE;
    }
    
    return result;
}
/*****************************************************************************//*!
@brief  设置iconlist为动态追加模式
@author kevin.lou
@param  ctrl_id                 [in] 控件  id
@param  flag                    [in] true,动态追加模式，false,非动态追加模式
@return 操作是否成功
@note   
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetToDynAppendMode(
											  MMI_CTRL_ID_T    ctrl_id,    //in 
                                              BOOLEAN          flag
                                             ) 
{
    BOOLEAN            result                = FALSE;
    CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr    = PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);

    if (PNULL != iconlist_ctrl_ptr)
    {
        if (flag != iconlist_ctrl_ptr->is_dynamic_append_mode)
        {
            //destroy progress
            DestroyPrgCtrl(iconlist_ctrl_ptr);

            //free item memory
            FreeItemMemory(iconlist_ctrl_ptr);            

            IconlistCtrlDestruct((CTRLBASE_OBJ_T   *)iconlist_ctrl_ptr);
            
            //set max icon
            iconlist_ctrl_ptr->total_num = 0;

            iconlist_ctrl_ptr->is_dynamic_append_mode = flag;

            //alloc
            AllocItemMemory(iconlist_ctrl_ptr);

            //set relative rect
            SetRelativeRect(iconlist_ctrl_ptr);

            //set current index
            iconlist_ctrl_ptr->cur_index = 0;

            //get top and bottom index
            GetTopBottomIndex(iconlist_ctrl_ptr);

            GUIWIN_SetTitleSubText(iconlist_ctrl_ptr->win_id,
                FALSE,
                PNULL,
                0,
                FALSE);

        }
        
        result = TRUE;
    }    
    return result;
}

/*****************************************************************************//*!
@brief  在iconlist尾部追加一项
@author kevin.lou
@param  ctrl_id	                [in] 控件  id
@return 操作是否成功
@note   
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_DynAppendOneItem(                                        
											MMI_CTRL_ID_T       ctrl_id       //in                                        
											)
{
    BOOLEAN            	result              = FALSE;
    CTRLICONLIST_OBJ_T 	*iconlist_ctrl_ptr  = PNULL;
    GUIICONLIST_ITEM_T  *item_ptr           = PNULL;//item
    uint32              win_handle          = 0; 
    CTRLBASE_OBJ_T     	*base_ctrl_ptr 		= PNULL;

    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    if (PNULL != iconlist_ctrl_ptr)
    {
    	 base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

        if (iconlist_ctrl_ptr->is_dynamic_append_mode)
        {
            if (0 == iconlist_ctrl_ptr->total_num%MIN_ITEM_MEM_ALLOC_NUM)
            {           
                item_ptr = SCI_ALLOC_APP((iconlist_ctrl_ptr->total_num + MIN_ITEM_MEM_ALLOC_NUM)*sizeof(GUIICONLIST_ITEM_T));
                if (PNULL != item_ptr)
                {
                    SCI_MEMSET(item_ptr,0,((iconlist_ctrl_ptr->total_num + MIN_ITEM_MEM_ALLOC_NUM)*sizeof(GUIICONLIST_ITEM_T)));
                    if (0 != iconlist_ctrl_ptr->total_num)
                    {
                        SCI_MEMCPY((void *)item_ptr,
                                   (const void *)iconlist_ctrl_ptr->item_ptr,
                                   (iconlist_ctrl_ptr->total_num*sizeof(GUIICONLIST_ITEM_T))
                                   );
                        SCI_FREE(iconlist_ctrl_ptr->item_ptr);
                    }
                    iconlist_ctrl_ptr->item_ptr = item_ptr;
                    item_ptr = PNULL;
                }
                else
                {
                    return FALSE;
                }    
            }
       
            iconlist_ctrl_ptr->total_num += 1; 
            
            SetRelativeRect(iconlist_ctrl_ptr);
            
            GetTopBottomIndex(iconlist_ctrl_ptr);
            
            win_handle = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);
            
            if (MMK_IsWinNeedPaint(win_handle))
            {
                DisplayIconList(TRUE,iconlist_ctrl_ptr);
            }                        
            result = TRUE;
        }
    }
    return result;    
}


/*****************************************************************************/
//  Description : Calculate Item Text Rect
//  Global resource dependence : 
//  Author: xiyuan.ma 2012.03
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T CalculateItemTextRect (
										CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
										uint16               index
										)
{
	GUIICONLIST_TEXT_T  *text_addtion_ptr = PNULL;
	GUI_RECT_T          text_addtion_rect = {0};
	GUI_RECT_T          text_rect         = {0};
	
	GUISTR_STYLE_T      str_style 		  = {0};
	MMI_STRING_T        str_info          = {0};
    GUISTR_STATE_T      str_state 		  = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;
	
	//uint16              text_width= 0;
	uint16              text_addtion_width= 0;

	if ((PNULL == iconlist_ctrl_ptr)||
		(index >= iconlist_ctrl_ptr->total_num))
	{
		return text_addtion_rect;
	}
	
	//get addtion text rect
	if (PNULL != iconlist_ctrl_ptr->item_ptr[index].text_addtion.display.name_str.wstr_ptr)
	{
		text_addtion_ptr     = &iconlist_ctrl_ptr->item_ptr[index].text_addtion;		
		//get text rect
		text_rect            = ICONLIST_GetTextRect(index,iconlist_ctrl_ptr);		
		//get addtion text str style
		str_style.font       = iconlist_ctrl_ptr->theme.font.font;
		str_style.font_color = iconlist_ctrl_ptr->theme.font.color;
		str_style.align      = ALIGN_HVMIDDLE;
		//get addtion text str info
		str_info             = iconlist_ctrl_ptr->item_ptr[index].text_addtion.display.name_str;
		//get addtion text width
		text_addtion_width   = GUISTR_GetStringWidth(&str_style, &str_info, str_state);
		
		if (text_addtion_width <= (text_rect.right - text_rect.left))
		{			
			//get addtion text rect
			text_addtion_rect      = text_rect;
			text_addtion_rect.left = text_addtion_rect.right - text_addtion_width;
		}
	}

	return text_addtion_rect;
}

/*****************************************************************************/
//  Description : 封装对外接口---get icon list empty string id
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetEmptyStrId(
                                        MMI_CTRL_ID_T    ctrl_id , 
                                        MMI_TEXT_ID_T empty_text_id
                                        )
{
    BOOLEAN            result                = FALSE;
    CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr    = PNULL;
    
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    
    if (PNULL != iconlist_ctrl_ptr)
    {
        iconlist_ctrl_ptr->theme.empty_text_id = empty_text_id;
        result = TRUE;
    }

    return result;

}

PUBLIC BOOLEAN CTRLICONLIST_SetHideSlideBar(MMI_CTRL_ID_T  ctrl_id )
{
    BOOLEAN            result                = FALSE;
    CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr    = PNULL;
    
    //get icon list control pointer
    iconlist_ctrl_ptr = ICONLIST_GetPtr(ctrl_id);
    
    if (PNULL != iconlist_ctrl_ptr)
    {
        iconlist_ctrl_ptr->theme.groove_width = 0;
        result = TRUE;
    }

    return result;

}

#endif  //end of GUIF_ICONLIST

