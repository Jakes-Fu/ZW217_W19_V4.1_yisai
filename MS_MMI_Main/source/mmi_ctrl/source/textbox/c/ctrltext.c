/*****************************************************************************
** File Name:      ctrltext.c                                                *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2009      Jassmine              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "chng_freq.h"
#include "mmk_timer.h"
#include "mmk_tp.h"
#include "cafctrltextbox.h"
#include "ctrltext_export.h"
#include "ctrltext.h"
#include "guiform.h"
#include "guires.h"
#include "guistring.h"
#include "guiurl.h"
#include "spml_api.h"
#include "mmi_theme.h"

#include "ctrltext.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
//  Description : init text class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextInitVtbl(
                              CTRLTEXT_VTBL_T        *text_vtbl_ptr
                              );

/*****************************************************************************/
//  Description : text construct 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextConstruct(
                            CTRLBASE_OBJ_T          *base_ctrl_ptr,
                            CTRLTEXT_INIT_PARAM_T   *text_param_ptr
                            );

/*****************************************************************************/
//  Description : init text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitDisplayRect(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : text destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextDestruct(
                           CTRLBASE_OBJ_T   *iguictrl_ptr
                           );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextCtrlPackInitParam(
                                  GUITEXT_INIT_DATA_T     *text_init_ptr,   //in
                                  CTRLTEXT_INIT_PARAM_T   *text_param_ptr   //out
                                  );

/*****************************************************************************/
//  Description : free text tag info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeTagInfo(
                       CTRLTEXT_OBJ_T   *text_ctrl_ptr
                       );

/*****************************************************************************/
//  Description : handle text msg function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TextHandleMsg(
                                 CTRLBASE_OBJ_T     *ctrl_ptr,  //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,     //message
                                 DPARAM             param       //add data
                                 );

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetPageLineNum(
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                            );

/*****************************************************************************/
// 	Description : start text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartTextScrollTimer(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                );

/*****************************************************************************/
// 	Description : stop text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StopTextScrollTimer(
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr
                               );

/*****************************************************************************/
// 	Description : display scroll bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar(
                            BOOLEAN         is_update,
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                            );

/*****************************************************************************/
// 	Description : judge and create prg control
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void JudgeAndCreatePrgCtrl(
                                 BOOLEAN            is_calc_total,  //in:is calculate total line number
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                                 );

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePrgCtrl(
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : destroy progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DestroyPrgCtrl(
                          CTRLTEXT_OBJ_T    *text_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollBarRect(
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDividingLine(
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr
                               );

/*****************************************************************************/
// 	Description : display text string
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DisplayTextString(
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                             );

/*****************************************************************************/
// 	Description : get text align for one line
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetTextLineAlign(
                                   GUI_ALIGN_E  text_align
                                   );

/*****************************************************************************/
//  Description : handle text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextScrollTimer(
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : notify parent line change msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void NotifyLineChangeMsg(
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr      //in:
                               );

/*****************************************************************************/
//  Description : handle text up or down key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextUpDownKey(
                                  BOOLEAN			is_up,
                                  BOOLEAN           is_handle_tag,
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : text scroll up or down lines
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollTextLines(
                              BOOLEAN			is_up,          //in:
                              int32             *disp_top_ptr,  //in/out:
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                              );

/*****************************************************************************/
//  Description : get text scroll pixel
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetScrollPixel(
                            CTRLTEXT_OBJ_T      *text_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle text up key msg for tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextTagUp(
                              int32             *disp_top_ptr,  //in/out:       
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : handle text down key msg for tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextTagDown(
                                int32           *disp_top_ptr,  //in/out:       
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get tag relative top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetTagRelativeTop(
                              GUITEXT_TAG_T     *tag_ptr,
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : get tag relative bottom
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetTagRelativeBottom(
                                 GUITEXT_TAG_T      *tag_ptr,
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : is set tag highlight
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSetTagHighlight(
                                GUITEXT_TAG_T   *tag_ptr,       //in:
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                );

/*****************************************************************************/
//  Description : get line string length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTextLineStrLen(
                               uint16           line_index,     //in:from 0
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr  //in:
                               );

/*****************************************************************************/
//  Description : get tp char offset
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTpCharOffset(
                             BOOLEAN            *is_tp_str_ptr, //in/out:may PNULL
                             uint16             line_index,     //in:
                             GUI_POINT_T        tp_point,       //in:
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                             );

/*****************************************************************************/
//  Description : is tp on the highlight string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpHighLight(
                            BOOLEAN         is_tp_str,      //in:is tp on string
                            uint16          char_offset,    //in:tp char offset
                            uint16          line_index,     //in:tp line index
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                            );

/*****************************************************************************/
//  Description : adjust relative display rect by line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustDisplayRect(
                                uint16          line_index,
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : text first line is display all
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayFirstLine(
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : text last line is display all
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayLastLine(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle tp text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextTpScrollTimer(
                                   CTRLTEXT_OBJ_T   *text_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : get tp line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpLineIndex(//is tp in text rect
                             uint16             *line_index_ptr,    //in/out:
                             GUI_POINT_T        tp_point,
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : handle text tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpDown(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr,
                                    DPARAM          param
                                    );

/*****************************************************************************/
//  Description : start tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpSlideTimer(
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : stop tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopTpSlideTimer(
                            CTRLTEXT_OBJ_T      *text_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle text tp short msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpShort(
                                     CTRLTEXT_OBJ_T *text_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : handle tp move msg which in text rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTpMoveInText(
                              GUI_POINT_T       *tp_point_ptr,  //in:
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                              );

/*****************************************************************************/
//  Description : start tp scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpScrollTimer(
                              BOOLEAN           is_scroll_up,   //in:
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                              );

/*****************************************************************************/
//  Description : handle text tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpUp(
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr,
                                  DPARAM            param
                                  );

/*****************************************************************************/
//  Description : is tp on tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpOnTag(
                        uint16          tp_pos,         //in:
                        GUITEXT_LINE_T  *line_info_ptr, //in:
                        GUITEXT_TAG_T   **tp_tag_pptr   //in/out:
                        );

/*****************************************************************************/
//  Description : handle text tag tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTagTpUp(
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                     DPARAM             param
                                     );
/*****************************************************************************/
//  Description : handle text tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextSlideTimer(
                                CTRLTEXT_OBJ_T      *text_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : handle text slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextSlide(
                           GUI_POINT_T      tp_point,
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : reset text slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetTextSlide(
                          BOOLEAN           is_update,
                          CTRLTEXT_OBJ_T    *text_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : handle text tp long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpLong(
                                    CTRLTEXT_OBJ_T      *text_ctrl_ptr,
                                    DPARAM              param
                                    );

/*****************************************************************************/
//  Description : handle text tp progress bar msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextTpPrg(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr, //in:
                           MMI_MESSAGE_ID_E  msg_id,         //in:
                           DPARAM            param           //in:
                           );
#endif
/*****************************************************************************/
//  Description : get text pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLTEXT_OBJ_T* GetTextPtr(
                                 MMI_HANDLE_T   ctrl_handle
                                 );

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TextTypeOf(
                         CTRLBASE_OBJ_T     *text_ptr
                         );

/*****************************************************************************/
//  Description : set text string by pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTextStrByPtr(
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr, //in:
                              wchar             *str_ptr,   //in:
                              uint16            str_len,    //in:
                              BOOLEAN           is_update   //in:
                              );

/*****************************************************************************/
//  Description : add text string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddTextString(
                         CTRLTEXT_OBJ_T     *text_ctrl_ptr, //in:
                         wchar              *str_ptr,       //in:
                         uint16             str_len         //in:
                         );

/*****************************************************************************/
//  Description : alloc text string memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* AllocStrMemory(
                            uint16      max_len
                            );

/*****************************************************************************/
//  Description : alloc text line info memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUITEXT_LINE_T* AllocLineMemory(
                                      uint16    max_len
                                      );

/*****************************************************************************/
//  Description : parse text tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParsetTextTag(
                         CTRLTEXT_OBJ_T     *text_ctrl_ptr, //in:
                         GUITEXT_SET_TAG_T  *set_tag_ptr    //in:may PNULL
                         );

/*****************************************************************************/
//  Description : parse text set tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParsetTextSetTag(
                            CTRLTEXT_OBJ_T      *text_ctrl_ptr, //in:
                            GUITEXT_SET_TAG_T   *set_tag_ptr    //in:may PNULL
                            );

/*****************************************************************************/
//  Description : parse text common tag,include phone number,email or url
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParsetTextCommonTag(
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : add one tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddOneTag(
                     CTRLTEXT_OBJ_T         *text_ctrl_ptr, //in/out:
                     uint16                 start_pos,      //in:
                     uint16                 len,            //in:
                     GUITEXT_TAG_TYPE_E     tag_type        //in:
                     );

/*****************************************************************************/
//  Description : parse text,include start position,total line number and top line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParseTextString(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : set line tag info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLineTag(
                      uint16            line_start_pos, //in:
                      uint16            line_end_pos,   //in:
                      GUITEXT_TAG_T     **tag_pptr,     //in/out:
                      GUITEXT_LINE_T    *line_info_ptr  //in/out:
                      );

/*****************************************************************************/
//  Description : clear text string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearTextString(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : set text relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRelativeRect(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           );

/*****************************************************************************/
//  Description : get text top and bottom index by relative display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetTopBottomIndex(
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr
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
//  Description : set text bg color or bg image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFontByPtr(
                        CTRLTEXT_OBJ_T  *text_ctrl_ptr, //in:
                        GUI_FONT_T      *font_ptr,      //in:may PNULL
                        GUI_COLOR_T     *font_color_ptr //in:may PNULL
                        );

/*****************************************************************************/
//  Description : set text rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetRect(
                              CTRLBASE_OBJ_T    *ctrl_ptr,
                              const CAF_RECT_T  *rect_ptr
                              );

/*****************************************************************************/
//  Description : set text border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetBorder(
                                CTRLBASE_OBJ_T  *ctrl_ptr,
                                GUI_BORDER_T    *border_ptr
                                );

/*****************************************************************************/
//  Description : set text bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetBg(
                            CTRLBASE_OBJ_T  *ctrl_ptr,
                            GUI_BG_T        *bg_ptr
                            );

/*****************************************************************************/
//  Description : set text font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetFont(
                              CTRLBASE_OBJ_T    *ctrl_ptr,
                              GUI_FONT_ALL_T    *font_ptr
                              );

/*****************************************************************************/
//  Description : get text font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetFont(
                              CTRLBASE_OBJ_T    *ctrl_ptr,
                              GUI_FONT_ALL_T    *font_ptr
                              );

/*****************************************************************************/
//  Description : set text is display progress
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetProgress(
                                  CTRLBASE_OBJ_T    *ctrl_ptr,
                                  BOOLEAN           is_display
                                  );

/*****************************************************************************/
//  Description : set text is circular handle key direction msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetCircularHandle(
                                        CTRLBASE_OBJ_T  *ctrl_ptr,
                                        BOOLEAN         is_circular
                                        );

/*****************************************************************************/
//  Description : get text height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetHeightByWidth(
                                       CTRLBASE_OBJ_T   *ctrl_ptr,  //in:
                                       uint16           width,      //in:
                                       uint16           *height_ptr //in/out:
                                       );

/*****************************************************************************/
//  Description : set text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetDisplayRect(
                                     CTRLBASE_OBJ_T     *ctrl_ptr,  //in:
                                     const GUI_RECT_T   *rect_ptr,  //in:
                                     BOOLEAN            is_update   //in:
                                     );

/*****************************************************************************/
//  Description : get text highlight end pos line rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetHlEndLineRect(
                                       CTRLBASE_OBJ_T   *ctrl_ptr,  //in:
                                       GUI_RECT_T       *rect_ptr   //in/out:
                                       );

/*****************************************************************************/
//  Description : get text highlight end pos line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetHlEndLineIndex(
                                        CTRLBASE_OBJ_T  *ctrl_ptr,      //in:
                                        BOOLEAN         *is_first_ptr,  //in/out:
                                        BOOLEAN         *is_last_ptr    //in/out:
                                        );

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get text type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TEXT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BASE_TYPE,
            "textbox",
            sizeof(CTRLTEXT_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)TextConstruct,
            (OBJECT_DESTRUCTOR_FUNC)TextDestruct,
            sizeof(CTRLTEXT_INIT_PARAM_T),
            (PARAM_PACKING_FUNC)TextCtrlPackInitParam,
            sizeof(CTRLTEXT_VTBL_T),
            (VTBL_INIT_FUNC)TextInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init text class
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextInitVtbl(
                              CTRLTEXT_VTBL_T        *text_vtbl_ptr
                              )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr =  (CTRLBASE_VTBL_T*)text_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = TextHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = TextCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBorder = TextCtrlSetBorder;
    base_ctrl_vtbl_ptr->SetBg = TextCtrlSetBg;
    base_ctrl_vtbl_ptr->SetFont = TextCtrlSetFont;
    base_ctrl_vtbl_ptr->GetFont = TextCtrlGetFont;
    base_ctrl_vtbl_ptr->SetScrollbarEnabled = TextCtrlSetProgress;
    base_ctrl_vtbl_ptr->SetUpDownLoop = TextCtrlSetCircularHandle;
    base_ctrl_vtbl_ptr->GetHeightByWidth = TextCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = TextCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->GetSelectRect = TextCtrlGetHlEndLineRect;
    base_ctrl_vtbl_ptr->GetSelectIndex = TextCtrlGetHlEndLineIndex;

    return TRUE;
}

/*****************************************************************************/
//  Description : text construct 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextConstruct(
                            CTRLBASE_OBJ_T          *base_ctrl_ptr,
                            CTRLTEXT_INIT_PARAM_T   *text_param_ptr
                            )
{
    BOOLEAN                 result = TRUE;
    CTRLTEXT_OBJ_T          *text_ctrl_ptr = (CTRLTEXT_OBJ_T*)base_ctrl_ptr;

    //set text display progress
    text_ctrl_ptr->is_display_prg = TRUE;

    //set win handle
    text_ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);

    //get text theme
    MMITHEME_GetTextTheme(&text_ctrl_ptr->theme);
    SCI_ASSERT(text_ctrl_ptr->theme.line_width <= text_ctrl_ptr->theme.line_space); /*assert verified*/
    base_ctrl_ptr->lcd_dev_info = text_ctrl_ptr->theme.lcd_dev;

    //init display rect
    InitDisplayRect(text_ctrl_ptr);

    //get line number in one page
    text_ctrl_ptr->page_line_num = GetPageLineNum(text_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
    text_ctrl_ptr->is_clipbrd_support = TRUE;
#endif

    return (result);
}

/*****************************************************************************/
//  Description : init text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitDisplayRect(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           )
{
    GUI_RECT_T      text_rect = {0};

    //get text rect
    text_rect = GUITEXT_GetRect(text_ctrl_ptr);
    
    //set relative display rect
    text_ctrl_ptr->relative_disp_rect.left   = 0;
    text_ctrl_ptr->relative_disp_rect.top    = 0;
    text_ctrl_ptr->relative_disp_rect.right  = text_rect.right - text_rect.left;
    text_ctrl_ptr->relative_disp_rect.bottom = text_rect.bottom - text_rect.top;
}

/*****************************************************************************/
//  Description : text destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextDestruct(
                           CTRLBASE_OBJ_T    *base_ctrl_ptr
                           )
{
    BOOLEAN         result = TRUE;
    CTRLTEXT_OBJ_T  *text_ctrl_ptr = (CTRLTEXT_OBJ_T*)base_ctrl_ptr;

    if (PNULL != text_ctrl_ptr)
    {
#ifdef GUIF_CLIPBOARD
        //set not display clip board menu
        GUITEXT_CloseClipboardMenu(text_ctrl_ptr);

        // 清除滑块
        GUITEXT_DestroyTextSlider(text_ctrl_ptr);
#endif

        //stop timer
        StopTextScrollTimer(text_ctrl_ptr);
#ifdef TOUCH_PANEL_SUPPORT
        GUITEXT_StopTpScrollTimer(text_ctrl_ptr);
        StopTpSlideTimer(text_ctrl_ptr);
#endif
        //free text string memory
        if (PNULL != text_ctrl_ptr->str_ptr)
        {
            SCI_FREE(text_ctrl_ptr->str_ptr);
        }

        //free line info memory
        if (PNULL != text_ctrl_ptr->line_info_ptr)
        {
            SCI_FREE(text_ctrl_ptr->line_info_ptr);
        }

        //free tag
        FreeTagInfo(text_ctrl_ptr);

        //free prgbox control memory
        if (PNULL != text_ctrl_ptr->prgbox_ctrl_ptr)
        {
            text_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextCtrlPackInitParam(
                                  GUITEXT_INIT_DATA_T     *text_init_ptr,   //in
                                  CTRLTEXT_INIT_PARAM_T   *text_param_ptr   //out
                                  )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)text_param_ptr;

    base_ctrl_param_ptr->both_rect = text_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : handle text msg function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TextHandleMsg(
                                 CTRLBASE_OBJ_T     *ctrl_ptr,  //control pointer
                                 MMI_MESSAGE_ID_E   msg_id,     //message
                                 DPARAM             param       //add data
                                 )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = (CTRLTEXT_OBJ_T*)ctrl_ptr;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != text_ctrl_ptr); /*assert verified*/
    if (PNULL == text_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        text_ctrl_ptr->is_active = TRUE;
        
        //notify parent control
        result = CTRLMSG_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_GET_ACTIVE);
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_LOSE_FOCUS:
        //stop timer
        StopTextScrollTimer(text_ctrl_ptr);
#ifdef TOUCH_PANEL_SUPPORT
        GUITEXT_StopTpScrollTimer(text_ctrl_ptr);
        StopTpSlideTimer(text_ctrl_ptr);
#endif
        //reset slide state
        text_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        
        if (MSG_CTL_LOSE_ACTIVE == msg_id)
        {
#ifdef GUIF_CLIPBOARD
            GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_NORMAL);
#endif

            text_ctrl_ptr->is_active = FALSE;

            //reset highlight
            text_ctrl_ptr->highlight.end_pos = text_ctrl_ptr->highlight.start_pos;

            //notify parent control
            result = CTRLMSG_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_LOSE_ACTIVE);
        }
#ifdef TOUCH_PANEL_SUPPORT
        else
        {
            //reset tex slide
            ResetTextSlide(FALSE,text_ctrl_ptr);
        }
#endif
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
#ifdef GUIF_CLIPBOARD
        GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_NORMAL);
#endif

        if (!MMK_IsActiveCtrl(base_ctrl_ptr->handle))
        {
            text_ctrl_ptr->is_active = FALSE;

            //stop timer
            StopTextScrollTimer(text_ctrl_ptr);
#ifdef TOUCH_PANEL_SUPPORT
            GUITEXT_StopTpScrollTimer(text_ctrl_ptr);
#endif
        }
        break;

    case MSG_TIMER:
        if (text_ctrl_ptr->scroll_timer_id == *(uint8*)param)
        {
            HandleTextScrollTimer(text_ctrl_ptr);
        }
#ifdef TOUCH_PANEL_SUPPORT
        else if (text_ctrl_ptr->tp_scroll_timer_id == *(uint8 *)param)
        {
            HandleTextTpScrollTimer(text_ctrl_ptr);
        }
        else if (text_ctrl_ptr->slide_timer_id == *(uint8*)param)
        {
            HandleTextSlideTimer(text_ctrl_ptr);
        }
#endif
#ifdef GUIF_CLIPBOARD
        else if (GUITEXT_HandleSliderTimer(text_ctrl_ptr, *(uint8*)param))
        {
        }
#endif
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_APP_OK:
        if (MMK_TP_SLIDE_NONE == text_ctrl_ptr->slide_state)
        {
            //notify parent control
            result = CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_OK);
        }
        break;

    case MSG_APP_WEB:
        if (MMK_TP_SLIDE_NONE == text_ctrl_ptr->slide_state)
        {
            //notify parent control
            result = CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_MIDSK);
        }
        break;

    case MSG_APP_CANCEL:
        if (MMK_TP_SLIDE_NONE == text_ctrl_ptr->slide_state)
        {
            //notify parent control
            result = CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_CANCEL);
        }
        break;

    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        if ((MMK_TP_SLIDE_NONE == text_ctrl_ptr->slide_state) && 
            (!HandleTextUpDownKey(TRUE,TRUE,text_ctrl_ptr)))
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        if ((MMK_TP_SLIDE_NONE == text_ctrl_ptr->slide_state) && 
            (!HandleTextUpDownKey(FALSE,TRUE,text_ctrl_ptr)))
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        result = HandleTextTpDown(text_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_SHORT:
        result = HandleTextTpShort(text_ctrl_ptr);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        result = GUITEXT_HandleTextTpMove(text_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        result = HandleTextTpUp(text_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        result = HandleTextTpLong(text_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef GUIF_CLIPBOARD
    case MSG_LCD_SWITCH:
        GUITEXT_CloseClipboardMenu(text_ctrl_ptr);
        break;

    case MSG_NOTIFY_CLIPBRD_COPY:
        GUITEXT_HandleTextCopyMsg(text_ctrl_ptr);
        break;

    //case MSG_NOTIFY_CLIPBRD_SELECT:
    //    GUITEXT_HandleTextSelectMsg(text_ctrl_ptr);
    //    break;

    case MSG_NOTIFY_CLIPBRD_SELECTALL:
        GUITEXT_HandleTextSelectAllMsg(text_ctrl_ptr);
        break;

    case MSG_NOTIFY_CLIPBRD_MENU_CLOSED:
        text_ctrl_ptr->clipbrd_win_handle = 0;
        if (!MMK_IsFocusWin(text_ctrl_ptr->win_handle))
        {
            GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_NORMAL);
        }
        break;

    case MSG_NOTIFY_BEGIN_SLIDER:
        GUITEXT_HandleBeginSlider(text_ctrl_ptr, param);
        break;

    case MSG_NOTIFY_MOVE_SLIDER:
        GUITEXT_HandleMoveSlider(text_ctrl_ptr, param);
        break;

    case MSG_NOTIFY_END_SLIDER:
        GUITEXT_HandleEndSlider(text_ctrl_ptr, param);
        break;
#endif
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PRGBOX_MOVEUP_ONE:
    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
    case MSG_CTL_PRGBOX_MOVE:
        if (MMK_TP_SLIDE_NONE == text_ctrl_ptr->slide_state)
        {
            HandleTextTpPrg(text_ctrl_ptr,msg_id,param);
        }
        break;
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetPageLineNum(
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                            )
{
    uint16          font_height = 0;
    uint16          line_height = 0;
    uint16          page_height = 0;
    uint16          line_num = 0;
    GUI_RECT_T      text_rect = {0};

    //get font height
    font_height = GUI_GetFontHeight(text_ctrl_ptr->theme.font.font,UNICODE_HANZI);

    //get line height
    line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

    //get text rect
    text_rect = GUITEXT_GetRect(text_ctrl_ptr);
    if (!GUI_IsRectEmpty(text_rect))
    {
        //get page height
        page_height = (uint16)(text_rect.bottom - text_rect.top + 1 - (text_ctrl_ptr->theme.margin_top_space + text_ctrl_ptr->theme.margin_bottom_space));

        line_num = (uint16)(page_height / line_height);
        if ((page_height%line_height) >= font_height)
        {
            line_num++;
        }
    }

    return (line_num);
}

/*****************************************************************************/
//  Description : display text control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DisplayText(
                                BOOLEAN          is_update_prgbox,
                                CTRLTEXT_OBJ_T   *text_ctrl_ptr
                                )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if (PNULL != text_ctrl_ptr)
    {
        //display bg
        GUITEXT_DisplayBg(base_ctrl_ptr->display_rect,text_ctrl_ptr);

        //display border
        GUI_DisplayBorder(base_ctrl_ptr->rect,
            base_ctrl_ptr->display_rect,
            &text_ctrl_ptr->theme.border,
            &base_ctrl_ptr->lcd_dev_info);

        //display scroll bar
        DisplayScrollBar(is_update_prgbox,text_ctrl_ptr);

        //display text dividing line
        DisplayDividingLine(text_ctrl_ptr);

        //display string
        DisplayTextString(text_ctrl_ptr);

        //display hight light
        GUITEXT_DisplayHighlight(TRUE,text_ctrl_ptr);

        //start time scroll timer
        StartTextScrollTimer(text_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
        GUITEXT_ShowSlider(text_ctrl_ptr);
#endif
    }
}

/*****************************************************************************/
// 	Description : start text time scroll timer
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartTextScrollTimer(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if ((MMK_IsFocusWin(text_ctrl_ptr->win_handle)) && 
        (text_ctrl_ptr->is_time_scroll) && 
        (0 == text_ctrl_ptr->scroll_timer_id))
    {
        text_ctrl_ptr->scroll_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
                                            text_ctrl_ptr->theme.scroll_period,
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
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr
                               )
{
    if (0 < text_ctrl_ptr->scroll_timer_id)
    {
        MMK_StopTimer(text_ctrl_ptr->scroll_timer_id);
        text_ctrl_ptr->scroll_timer_id = 0;
    }
}

/*****************************************************************************/
// 	Description : display scroll bar
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DisplayScrollBar(
                            BOOLEAN         is_update,
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                            )
{
    int32           display_max_top = 0;
    uint32          item_index = 0;
    GUI_RECT_T      prg_rect = {0};

    if (PNULL != text_ctrl_ptr->prgbox_ctrl_ptr)
    {
        prg_rect = GetScrollBarRect(text_ctrl_ptr);

        GUIPRGBOX_SetParamByPtr(text_ctrl_ptr->prgbox_ctrl_ptr,
            GUITEXT_GetRelativeHeight(text_ctrl_ptr),
            (prg_rect.bottom - prg_rect.top + 1));

        //get display max top
        display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

        //get current item index
        if (0 > text_ctrl_ptr->relative_disp_rect.top)
        {
            item_index = 0;
        }
        else if (text_ctrl_ptr->relative_disp_rect.top > display_max_top)
        {
            item_index = display_max_top;
        }
        else
        {
            item_index = text_ctrl_ptr->relative_disp_rect.top;
        }

        //set current position
        GUIPRGBOX_SetPosByPtr(is_update,
            item_index,
            item_index,
            text_ctrl_ptr->prgbox_ctrl_ptr);
    }
}

/*****************************************************************************/
// 	Description : judge and create prg control
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void JudgeAndCreatePrgCtrl(
                                 BOOLEAN            is_calc_total,  //in:is calculate total line number
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                                 )
{
    uint16      text_width = 0;
    uint16      total_line_num = 0;
    GUI_RECT_T  text_rect = {0};

	if ((text_ctrl_ptr->is_display_prg) &&
        (0 < text_ctrl_ptr->page_line_num))
	{
        //get text box rect
        text_rect = GUITEXT_GetRect(text_ctrl_ptr);
        if (GUI_IsRectEmpty(text_rect))
        {
            is_calc_total = FALSE;
        }

        if (is_calc_total)
        {
		    //get text box width
		    text_width = (uint16)(text_rect.right - text_rect.left - (text_ctrl_ptr->theme.margin_left_space + text_ctrl_ptr->theme.margin_right_space) + 1);

		    //get text string line number
		    total_line_num = GUI_CalculateStringLinesByPixelNum(text_width,
							    text_ctrl_ptr->str_ptr,
							    text_ctrl_ptr->str_len,
							    text_ctrl_ptr->theme.font.font,
							    (uint8)text_ctrl_ptr->theme.char_space,
							    TRUE);

		    //is carriage return or line feed
		    if ((0 < text_ctrl_ptr->str_len) &&
			    (GUI_IsNewlineChar(text_ctrl_ptr->str_ptr[text_ctrl_ptr->str_len - 1])))
		    {
			    //cursor display the next line
			    total_line_num++;
		    }
        }
        else
        {
            total_line_num = text_ctrl_ptr->total_line_num;
        }

		//is need progress
		if ((total_line_num > text_ctrl_ptr->page_line_num) &&
            (PNULL == text_ctrl_ptr->prgbox_ctrl_ptr))
		{
			//create progress bar control
            CreatePrgCtrl(text_ctrl_ptr);
		}
	}
}

/*****************************************************************************/
//  Description : create progress control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePrgCtrl(
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                            )
{
    BOOLEAN                 result = FALSE;
    GUI_RECT_T              scroll_bar_rect = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUIPRGBOX_INIT_DATA_T   init_data = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if (PNULL == text_ctrl_ptr->prgbox_ctrl_ptr)
    {
        //set scroll bar rect
        scroll_bar_rect = GetScrollBarRect(text_ctrl_ptr);
        
        init_data.lcd_id = base_ctrl_ptr->lcd_dev_info.lcd_id;
        init_data.both_rect.h_rect = init_data.both_rect.v_rect = scroll_bar_rect;
        init_data.style = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
        init_data.is_forbid_paint = TRUE;
        
        create.guid               = SPRD_GUI_PRGBOX_ID;
        create.parent_win_handle  = text_ctrl_ptr->win_handle;
        create.parent_ctrl_handle = base_ctrl_ptr->handle;
        create.init_data_ptr      = &init_data;
        
        text_ctrl_ptr->prgbox_ctrl_ptr = MMK_CreateControl(&create);

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
                          CTRLTEXT_OBJ_T    *text_ctrl_ptr
                          )
{
    if (PNULL != text_ctrl_ptr->prgbox_ctrl_ptr)
    {
        //destroy progress bar control
        MMK_DestroyControl(GUICTRL_GetCtrlHandle(text_ctrl_ptr->prgbox_ctrl_ptr));
        text_ctrl_ptr->prgbox_ctrl_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetScrollBarRect(
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                  )
{
    uint16          prg_width = 0;
    GUI_RECT_T      prg_rect = {0};
    GUI_RECT_T      text_rect = {0};

    //get text rect
    text_rect = GUITEXT_GetRect(text_ctrl_ptr);

    //get progress bar width
    prg_width = GUITEXT_GetScrollBarWidth(text_ctrl_ptr);

    //set prg rect
    prg_rect = text_rect;
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        prg_rect.right = (int16)(prg_rect.left + prg_width - 1);
    }
    else
    {
        prg_rect.left = (int16)(prg_rect.right - prg_width + 1);
    }

    return (prg_rect);
}

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DisplayDividingLine(
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr
                               )
{
    uint16          i = 0;
    GUI_RECT_T      line_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if ((PNULL != text_ctrl_ptr) && 
        (0 < text_ctrl_ptr->theme.line_width))
    {
        //draw all line
        for (i=text_ctrl_ptr->top_line_index; i<text_ctrl_ptr->bottom_line_index; i++)
        {
            //get line rect
            line_rect        = GUITEXT_GetLineRect(i,text_ctrl_ptr);
            line_rect.top    = (int16)(line_rect.bottom + 1);
            line_rect.bottom = (int16)(line_rect.top + text_ctrl_ptr->theme.line_width - 1);

            //get crossed rect
            if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,line_rect))
            {
                //draw line
                GUI_FillRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                    cross_rect,
                    text_ctrl_ptr->theme.line_color);
            }
        }
    }
}

/*****************************************************************************/
// 	Description : display text string
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void DisplayTextString(
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                             )
{
    uint16                  i = 0;
    uint16                  region_num = 0;
    GUI_RECT_T              text_rect = {0};
    GUI_RECT_T              cross_rect = {0};
    GUI_ALIGN_E             align = ALIGN_LVMIDDLE;
    MMI_STRING_T            text_str = {0};
    GUISTR_STYLE_T          str_style = {0}; /*lint !e64*/
    GUISTR_STATE_T          str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;
    GUIFONT_REGION_STYLE_T  *region_style_ptr = PNULL;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if ((PNULL != text_ctrl_ptr) && 
        (0 < text_ctrl_ptr->str_len))
    {
        for (i=text_ctrl_ptr->top_line_index; i<=text_ctrl_ptr->bottom_line_index; i++)
        {
            //get text string rect
            text_rect = GUITEXT_GetLineStrRect(i,text_ctrl_ptr);

            //get crossed rect
            if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,text_rect))
            {
                //get line string len
                text_str.wstr_len = GUITEXT_GetLineStrLen(i,text_ctrl_ptr);
        
                if (0 < text_str.wstr_len)
                {
                    //get display string
                    text_str.wstr_ptr = &text_ctrl_ptr->str_ptr[text_ctrl_ptr->line_info_ptr[i].start_pos];
                    
                    //get text line align
                    align = GetTextLineAlign(text_ctrl_ptr->theme.align);
                    if (ALIGN_LVMIDDLE == align)
                    {
                        str_state = str_state | GUISTR_STATE_ALIGN_BY_DIR;
                    }

                    //alloc region memory
                    region_num = text_ctrl_ptr->line_info_ptr[i].tag_num;
                    if (0 < region_num)
                    {
                        region_style_ptr = SCI_ALLOC_APP(region_num*sizeof(GUIFONT_REGION_STYLE_T));
                        SCI_MEMSET(region_style_ptr,0,(region_num*sizeof(GUIFONT_REGION_STYLE_T)));
                    }

                    //get line string region style
                    region_num = GUITEXT_GetLineStrRegion(FALSE,i,region_num,text_ctrl_ptr,region_style_ptr);

                    //set string style
                    str_style.font       = text_ctrl_ptr->theme.font.font;
                    str_style.font_color = text_ctrl_ptr->theme.font.color;
                    str_style.align      = align;
                    str_style.line_color = (GUI_COLOR_T )(~text_ctrl_ptr->theme.font.color);
                    str_style.line_color = text_ctrl_ptr->theme.underline_color;
                    str_style.char_space = text_ctrl_ptr->theme.char_space;
                    str_style.region_ptr = region_style_ptr;
                    str_style.region_num = region_num;
            
                    //display string
                    GUISTR_DrawTextToLCDInRect((const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                        (const GUI_RECT_T*)&text_rect,
                        (const GUI_RECT_T*)&cross_rect,
                        &text_str,
                        &str_style,
                        str_state,
                        text_ctrl_ptr->line_info_ptr[i].direction);

                    //free region
                    if (PNULL != region_style_ptr)
                    {
                        SCI_FREE(region_style_ptr);
                    }
                }
            }
        }
    }
}

/*****************************************************************************/
// 	Description : get text align for one line
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E GetTextLineAlign(
                                   GUI_ALIGN_E  text_align
                                   )
{
    GUI_ALIGN_E     align = ALIGN_LVMIDDLE;

    switch (text_align)
    {
    case ALIGN_LEFT:
    case ALIGN_TOP:
    case ALIGN_BOTTOM:
    case ALIGN_VMIDDLE:
    case ALIGN_LVMIDDLE:
        align = ALIGN_LVMIDDLE;
        break;

    case ALIGN_RIGHT:
    case ALIGN_RVMIDDLE:
        align = ALIGN_RVMIDDLE;
        break;

    case ALIGN_HMIDDLE:
    case ALIGN_HVMIDDLE:
        align = ALIGN_HVMIDDLE;
        break;

    default:
        break;
    }

    return (align);
}

/*****************************************************************************/
//  Description : handle text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextScrollTimer(
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                 )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //stop timer
    StopTextScrollTimer(text_ctrl_ptr);

    //the last page
    if (IsDisplayLastLine(text_ctrl_ptr))
    {
        CTRLMSG_PostNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_TEXT_END_LINE,PNULL,0);
    }

    //handle down key msg
    HandleTextUpDownKey(FALSE,FALSE,text_ctrl_ptr);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle tp text scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextTpScrollTimer(
                                   CTRLTEXT_OBJ_T   *text_ctrl_ptr
                                   )
{
    BOOLEAN     result = TRUE;

    //stop tp timer
    GUITEXT_StopTpScrollTimer(text_ctrl_ptr);

    if (text_ctrl_ptr->is_tp_scroll_up)
    {
        //the first page
        if (IsDisplayFirstLine(text_ctrl_ptr))
        {
            result = FALSE;
        }
    }
    else
    {
        //the last page
        if (IsDisplayLastLine(text_ctrl_ptr))
        {
            result = FALSE;
        }
    }

    if (result)
    {
        //notify line change
        NotifyLineChangeMsg(text_ctrl_ptr);
        
        //handle down key msg
        HandleTextUpDownKey(text_ctrl_ptr->is_tp_scroll_up,FALSE,text_ctrl_ptr);

        //handle move msg in text
        HandleTpMoveInText(&text_ctrl_ptr->tp_move_point,text_ctrl_ptr);

        //start tp timer
        StartTpScrollTimer(text_ctrl_ptr->is_tp_scroll_up,text_ctrl_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : notify parent line change msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void NotifyLineChangeMsg(
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr      //in:
                               )
{
    GUIFORM_NOTIFY_LINE_T   notify_line = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //post parent control
    if (0 != MMK_GetParentCtrlHandle(base_ctrl_ptr->handle))
    {
        //set notify param
        notify_line.ctrl_handle = base_ctrl_ptr->handle;
        CTRLMSG_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_LINE_CHANGE,&notify_line);
    }
}

/*****************************************************************************/
//  Description : handle text up or down key msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextUpDownKey(
                                  BOOLEAN			is_up,
                                  BOOLEAN           is_handle_tag,
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                  )
{
    BOOLEAN         result = FALSE;
    int32           display_top = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //set init display top
    display_top = text_ctrl_ptr->relative_disp_rect.top;

    if ((is_handle_tag) &&
        (0 < text_ctrl_ptr->tag_num))
    {
        if (is_up)
        {
            result = HandleTextTagUp(&display_top,text_ctrl_ptr);
        }
        else
        {
            result = HandleTextTagDown(&display_top,text_ctrl_ptr);
        }
    }
    else
    {
        //scroll up/down lines
        result = ScrollTextLines(is_up,&display_top,text_ctrl_ptr);
    }

    if (result)
    {
        if (GUITEXT_SetDisplayRect(display_top,text_ctrl_ptr))
        {
            //get top and bottom index
            GetTopBottomIndex(text_ctrl_ptr);
        }

        if ((GUI_BG_NONE == text_ctrl_ptr->theme.bg.bg_type) && (text_ctrl_ptr->is_time_scroll))
	    {
            CTRLMSG_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_TEXT_SCROLL, NULL);
	    }
	    else
	    {
		 	//update text
            GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
	    }
    }

    return (result);
}

/*****************************************************************************/
//  Description : text scroll up or down lines
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ScrollTextLines(
                              BOOLEAN			is_up,          //in:
                              int32             *disp_top_ptr,  //in/out:
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                              )
{
    BOOLEAN     result = FALSE;
    uint16      scroll_pixel = 0;
    int32       display_top = 0;
    int32       display_max_top = 0;

    //set init display top
    display_top = text_ctrl_ptr->relative_disp_rect.top;

    //get display max top
    display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

    //get scroll pixel
    scroll_pixel = GetScrollPixel(text_ctrl_ptr);
    
    if (is_up)
    {
        //the first page
        if (0 == display_top)
        {
            //is circular handle key msg
            if (text_ctrl_ptr->theme.is_circular_handle)
            {
                //set display top
                display_top = display_max_top;
            }
        }
        else
        {
            //set display top
            if (text_ctrl_ptr->relative_disp_rect.top > scroll_pixel)
            {
                display_top = text_ctrl_ptr->relative_disp_rect.top - scroll_pixel;
            }
            else
            {
                display_top = 0;
            }
        }
    }
    else
    {
        //the last page
        if (text_ctrl_ptr->relative_disp_rect.top == display_max_top)
        {
            //is circular handle key msg
            if (text_ctrl_ptr->theme.is_circular_handle)
            {
                //set display top
                display_top = 0;
            }
        }
        else
        {
            //set display top
            display_top = (int32)MIN((text_ctrl_ptr->relative_disp_rect.top + scroll_pixel),display_max_top);
        }
    }

    //set display top
    if (PNULL != disp_top_ptr)
    {
        *disp_top_ptr = display_top;
    }

    //set result
    if (display_top != text_ctrl_ptr->relative_disp_rect.top)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get text scroll pixel
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetScrollPixel(
                            CTRLTEXT_OBJ_T      *text_ctrl_ptr
                            )
{
    uint16  scroll_pixel = 0;
    uint16  line_height = 0;
    uint16  display_height = 0;

    //get line height
    line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

    //get display height
    display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

    switch (text_ctrl_ptr->theme.scroll_mode)
    {
    case GUITEXT_SCROLL_LINE:
        scroll_pixel = (uint16)(text_ctrl_ptr->theme.scroll_step*line_height);
        break;

    case GUITEXT_SCROLL_PAGE:
        scroll_pixel = (uint16)(text_ctrl_ptr->theme.scroll_step*display_height);
        break;

    case GUITEXT_SCROLL_PIXEL:
        scroll_pixel = text_ctrl_ptr->theme.scroll_step;
        break;

    case GUITEXT_SCROLL_RATIO:
        scroll_pixel = (uint16)(text_ctrl_ptr->theme.scroll_step * display_height/100);
        break;

    default:
        break;
    }

    return (scroll_pixel);
}

/*****************************************************************************/
//  Description : get line string length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTextLineStrLen(
                               uint16           line_index,     //in:from 0
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr  //in:
                               )
{
    uint16      str_len = 0;

    if (line_index < text_ctrl_ptr->total_line_num)
    {
        //is last line
        if ((line_index+1) == text_ctrl_ptr->total_line_num)
        {
            str_len = (uint16)(text_ctrl_ptr->str_len - text_ctrl_ptr->line_info_ptr[line_index].start_pos);
        }
        else
        {
            str_len = (uint16)(text_ctrl_ptr->line_info_ptr[line_index+1].start_pos - text_ctrl_ptr->line_info_ptr[line_index].start_pos);
        }
    }

    return (str_len);
}

/*****************************************************************************/
//  Description : get tp char offset
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetTpCharOffset(
                             BOOLEAN            *is_tp_str_ptr, //in/out:may PNULL
                             uint16             line_index,     //in:
                             GUI_POINT_T        tp_point,       //in:
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                             )
{
    wchar       *str_ptr = PNULL;
    uint16      str_len = 0;
    uint16      char_offset = 0;
    uint16      text_width = 0;
	int32		adjust_offset = 0;
    GUI_RECT_T  text_rect = {0};

    //get text string rect
    text_rect = GUITEXT_GetLineStrRect(line_index,text_ctrl_ptr);

    //get display string width
    if (tp_point.x <= text_rect.left)
    {
        char_offset = 0;

        //set is tp on the string
        if (PNULL != is_tp_str_ptr)
        {
            *is_tp_str_ptr = FALSE;
        }
    }
    else
    {
        text_width = (uint16)(tp_point.x - text_rect.left + 1);

        //get display string
        str_ptr = text_ctrl_ptr->str_ptr + text_ctrl_ptr->line_info_ptr[line_index].start_pos;
        str_len = GetTextLineStrLen(line_index,text_ctrl_ptr);

        //calculate char number in one line
        char_offset = GUI_GetCursorPos(ALIGN_LEFT,
            &text_rect,
            text_ctrl_ptr->line_info_ptr[line_index].direction,
            str_ptr,
            str_len,
            0,
            tp_point.x,
            text_ctrl_ptr->theme.font.font,
            text_ctrl_ptr->theme.char_space,
            is_tp_str_ptr);
		
		//adjust char offset
		adjust_offset = char_offset;
		GUI_GetCursorInfo(ALIGN_LEFT,
			&text_rect,
			text_ctrl_ptr->line_info_ptr[line_index].direction,
            str_ptr,
            str_len,
			FALSE,
			text_ctrl_ptr->theme.font.font,
            text_ctrl_ptr->theme.char_space,
			&adjust_offset);
		char_offset = (uint16)(adjust_offset);

        //last char is LF
        if ((PNULL != is_tp_str_ptr) &&
            (0 < str_len) &&
            (char_offset == str_len) &&
            (GUI_IsNewlineChar(str_ptr[str_len - 1])))
        {
            char_offset--;
            *is_tp_str_ptr = TRUE;
        }
    }

    return (char_offset);
}

/*****************************************************************************/
//  Description : is tp on the highlight string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpHighLight(
                            BOOLEAN         is_tp_str,      //in:is tp on string
                            uint16          char_offset,    //in:tp char offset
                            uint16          line_index,     //in:tp line index
                            CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                            )
{
    BOOLEAN     result = FALSE;
    uint16      highlight_start = 0;
    uint16      highlight_end = 0;
    uint16      highlight_len = 0;

    //get highlight start pos and length
    highlight_len = GUITEXT_GetHighlightStartPosAndLen(&highlight_start,
                                line_index,
                                text_ctrl_ptr);

    if ((is_tp_str) && 
        (0 < highlight_len))
    {
        //get highlight end pos
        highlight_end = (uint16)(highlight_start + highlight_len);

        if ((char_offset >= highlight_start) &&
            (char_offset <= highlight_end))
        {
            //set tp region
            text_ctrl_ptr->tp_region = GUITEXT_TP_REGION_HIGHLIGHT;

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : adjust relative display rect by line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AdjustDisplayRect(
                                uint16          line_index,
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                )
{
    BOOLEAN         result = FALSE;
    uint16          display_height = 0;
    int32           display_top = 0;
    int32           display_max_top = 0;
    GUI_BIG_RECT_T  relative_rect = {0};

    //adjust line index
    if (line_index >= text_ctrl_ptr->total_line_num)
    {
        line_index = (uint16)(text_ctrl_ptr->total_line_num - 1);
    }

    //set init display top
    display_top = text_ctrl_ptr->relative_disp_rect.top;

    //get icon relative rect
    relative_rect = GUITEXT_GetLineRelativeRect(line_index,text_ctrl_ptr);

    //the line is display all
    if (relative_rect.top < text_ctrl_ptr->relative_disp_rect.top)
    {
        //first line
        if (0 == line_index)
        {
            display_top = 0;
        }
        else
        {
            display_top = relative_rect.top;
        }
    }
    else if (relative_rect.bottom > text_ctrl_ptr->relative_disp_rect.bottom)
    {
        //get display height
        display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

        //last line
        if (line_index == (text_ctrl_ptr->total_line_num - 1))
        {
            display_top = text_ctrl_ptr->relative_rect.bottom - display_height + 1;
        }
        else
        {
            display_top = relative_rect.bottom - display_height + 1;
        }
    }

    //get display max top
    display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

    //adjust display top
    display_top = (int32)MIN(display_top,display_max_top);

    //set display rect
    result = GUITEXT_SetDisplayRect(display_top,text_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : text first line is display all
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayFirstLine(
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                 )
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //the first page
    if (0 == text_ctrl_ptr->relative_disp_rect.top)
    {
        //first line is display all
        if (base_ctrl_ptr->display_rect.top <= base_ctrl_ptr->rect.top)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : text last line is display all
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisplayLastLine(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                )
{
    BOOLEAN         result = FALSE;
    int32           display_max_top = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //get display max top
    display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

    if (text_ctrl_ptr->relative_disp_rect.top == display_max_top)
    {
        //last line is display all
        if (base_ctrl_ptr->display_rect.bottom >= base_ctrl_ptr->rect.bottom)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }

    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle text tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpDown(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr,
                                    DPARAM          param
                                    )
{
    BOOLEAN         is_tp_line = FALSE;
    BOOLEAN         is_tp_char = FALSE;
    BOOLEAN         is_tp_str = FALSE;
    uint16          highlight_len = 0;
    uint16          tp_line_index = 0;
    uint16          char_offset = 0;
    uint16          tp_pos = 0;
    GUI_POINT_T     tp_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    
    //is tp down
    text_ctrl_ptr->is_tp_down = TRUE;

#ifdef GUIF_CLIPBOARD
    text_ctrl_ptr->is_tp_moved = FALSE;
#endif

    if ((text_ctrl_ptr->theme.is_handle_tp) &&
        (0 < text_ctrl_ptr->str_len))
    {
        //get highlight len
        highlight_len = GUITEXT_GetHighlightLen(text_ctrl_ptr);

        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        // store tp down y point. when move less than half of line height, we
        // consider tp is not moved vertically.
        text_ctrl_ptr->tp_down_y = tp_point.y;

        //get tp line index
        is_tp_line = GetTpLineIndex(&tp_line_index,tp_point,text_ctrl_ptr);

        //get tp x offset
        char_offset = GetTpCharOffset(&is_tp_char,tp_line_index,tp_point,text_ctrl_ptr);

        //set tp position
        tp_pos = (uint16)(text_ctrl_ptr->line_info_ptr[tp_line_index].start_pos + char_offset);

        //reset tp state
        text_ctrl_ptr->tp_state = GUITEXT_TP_STATE_NONE;

        //reset tp region
        text_ctrl_ptr->tp_region = GUITEXT_TP_REGION_NONE;

#ifdef GUIF_CLIPBOARD
        //set not display clip board menu
        GUITEXT_CloseClipboardMenu(text_ctrl_ptr);
#endif

        //no highlight
        if (0 == highlight_len)
        {
            //set highlight
            text_ctrl_ptr->highlight.start_pos = tp_pos;
            text_ctrl_ptr->highlight.end_pos   = text_ctrl_ptr->highlight.start_pos;

#ifdef GUIF_CLIPBOARD
            GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_NORMAL);
#endif
        }
        else
        {
            //is tp string
            if ((is_tp_line) && (is_tp_char))
            {
                is_tp_str = TRUE;
            }

            //is tp highlight
            if (!IsTpHighLight(is_tp_str,char_offset,tp_line_index,text_ctrl_ptr))
            {
                //reset highlight not display
                GUITEXT_DisplayHighlight(FALSE,text_ctrl_ptr);

                //set highlight
                text_ctrl_ptr->highlight.start_pos = tp_pos;
                text_ctrl_ptr->highlight.end_pos   = text_ctrl_ptr->highlight.start_pos;

#ifdef GUIF_CLIPBOARD
                GUITEXT_ChangeDragState(text_ctrl_ptr, GUITEXT_DRAGSTATE_NORMAL);
#endif
            }
        }

        //adjust relative display rect by line index
        if (AdjustDisplayRect(tp_line_index,text_ctrl_ptr))
        {
            //get top and bottom index
            GetTopBottomIndex(text_ctrl_ptr);

            //update display
            GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
        }

        //is glide
        if (text_ctrl_ptr->theme.is_support_slide)
        {
            //set start and previous point
            text_ctrl_ptr->start_point = tp_point;
            text_ctrl_ptr->prev_point  = tp_point;

            //set tp down slide state
            text_ctrl_ptr->tpdown_slide_state = text_ctrl_ptr->slide_state;

            //add velocity item
            MMK_AddVelocityItem(tp_point.x,tp_point.y);

            if (MMK_TP_SLIDE_FLING != text_ctrl_ptr->slide_state)
            {
                //start tp slide timer
                StartTpSlideTimer(text_ctrl_ptr);
            }
        }

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get tp line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTpLineIndex(//is tp in text rect
                             uint16             *line_index_ptr,    //in/out:
                             GUI_POINT_T        tp_point,
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr
                             )
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;
    GUI_RECT_T  line_rect = {0};

    //get top line rect
    line_rect = GUITEXT_GetLineRect(text_ctrl_ptr->top_line_index,text_ctrl_ptr);
    if (tp_point.y < line_rect.top)
    {
        //set line index is the top line
        *line_index_ptr = text_ctrl_ptr->top_line_index;
    }
    else
    {
        //get bottom line rect
        line_rect = GUITEXT_GetLineRect(text_ctrl_ptr->bottom_line_index,text_ctrl_ptr);
        if (tp_point.y > line_rect.bottom)
        {
            //set line index is the bottom line
            *line_index_ptr = text_ctrl_ptr->bottom_line_index;
        }
        else
        {
            //get tp line index
            for (i=text_ctrl_ptr->top_line_index; i<=text_ctrl_ptr->bottom_line_index; i++)
            {
                //get line rect,include dividing line
                line_rect = GUITEXT_GetLineRect(i,text_ctrl_ptr);
                line_rect.bottom = (int16)(line_rect.bottom + text_ctrl_ptr->theme.line_width);

                if ((tp_point.y >= line_rect.top) &&
                    (tp_point.y <= line_rect.bottom))
                {
                    *line_index_ptr = i;
                    result = TRUE;
                    break;
                }
            }
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
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr
                             )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //include progress bar
    if (0 == text_ctrl_ptr->slide_timer_id)
    {
        text_ctrl_ptr->slide_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
                                                text_ctrl_ptr->theme.slide_period,
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
                            CTRLTEXT_OBJ_T      *text_ctrl_ptr
                            )
{
    if (0 < text_ctrl_ptr->slide_timer_id)
    {
        MMK_StopTimer(text_ctrl_ptr->slide_timer_id);
        text_ctrl_ptr->slide_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle text tp short msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpShort(
                                     CTRLTEXT_OBJ_T *text_ctrl_ptr
                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;

    if ((text_ctrl_ptr->theme.is_handle_tp) &&
        (0 < text_ctrl_ptr->str_len))
    {
        if (MMITHEME_DRAGMODE_DEFAULT == text_ctrl_ptr->theme.drag_mode)
        {
            //set text tp state
            text_ctrl_ptr->tp_state = GUITEXT_TP_STATE_DRAG;
        }
        
        //stop slide timer
        StopTpSlideTimer(text_ctrl_ptr);

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle text tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E GUITEXT_HandleTextTpMove(
                                             CTRLTEXT_OBJ_T  *text_ctrl_ptr,
                                             DPARAM          param
                                             )
{
    BOOLEAN         is_handle = FALSE;
    GUI_RECT_T      cross_rect = {0};
    GUI_POINT_T     tp_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //set tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    if ((text_ctrl_ptr->theme.is_handle_tp) &&
        (0 < text_ctrl_ptr->str_len))
    {
        if (GUITEXT_TP_STATE_DRAG == text_ctrl_ptr->tp_state)
        {
#ifdef GUIF_CLIPBOARD
            text_ctrl_ptr->is_tp_moved = TRUE;
#endif

            //is in text rect
            if ((GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,base_ctrl_ptr->rect)) && 
                (GUI_PointIsInRect(tp_point,cross_rect)))
            {
                is_handle = TRUE;
            }
            else
            {
                //set tp move point
                text_ctrl_ptr->tp_move_point = tp_point;
            
                if (tp_point.y < cross_rect.top)
                {
                    //the first page
                    if (IsDisplayFirstLine(text_ctrl_ptr))
                    {
                        is_handle = TRUE;
                    }
                    else
                    {
                        //start tp scroll timer
                        StartTpScrollTimer(TRUE,text_ctrl_ptr);
                    }
                }
                else if (tp_point.y > cross_rect.bottom)
                {
                    //the last page
                    if (IsDisplayLastLine(text_ctrl_ptr))
                    {
                        is_handle = TRUE;
                    }
                    else
                    {
                        //start tp scroll timer
                        StartTpScrollTimer(FALSE,text_ctrl_ptr);
                    }
                }
            }

            if (is_handle)
            {
                //stop tp scroll timer
                GUITEXT_StopTpScrollTimer(text_ctrl_ptr);

                if (-1 != text_ctrl_ptr->tp_down_y)
                {
                    uint16 line_height = 0;

                    //get line height
                    line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

                    // 如果未超过规定的高度，则认为纵向不准备移动
                    if (abs(text_ctrl_ptr->tp_down_y - tp_point.y) < line_height * 3 / 4)
                    {
                        tp_point.y = text_ctrl_ptr->tp_down_y;
                    }
                    else
                    {
                        text_ctrl_ptr->tp_down_y = -1;      //　将-1定义为可以纵向移动
                    }
                }

                //handle move msg in text
                HandleTpMoveInText(&tp_point,text_ctrl_ptr);
            }

            result = MMI_RESULT_TRUE;
        }
        else if (text_ctrl_ptr->theme.is_support_slide)
        {
            if (IS_TP_MOVE_Y(text_ctrl_ptr->start_point.y, tp_point.y))
            {
                //set slide state
                if ((MMK_TP_SLIDE_NONE == text_ctrl_ptr->tpdown_slide_state) ||
                    (MMK_IsWayChanged(text_ctrl_ptr->fling_velocity,&text_ctrl_ptr->start_point,&tp_point,FALSE)))
                {
                    text_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
                }
            }

            //add velocity item
            MMK_AddVelocityItem(tp_point.x,tp_point.y);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle tp move msg which in text rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTpMoveInText(
                              GUI_POINT_T       *tp_point_ptr,  //in:
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                              )
{
    uint16          tp_line_index = 0;
    uint16          char_offset = 0;
    uint16          tp_pos = 0;
    
    //get tp line index
    GetTpLineIndex(&tp_line_index,*tp_point_ptr,text_ctrl_ptr);

    //get tp x offset
    char_offset = GetTpCharOffset(PNULL,tp_line_index,*tp_point_ptr,text_ctrl_ptr);

    //set tp position
    tp_pos = (uint16)(text_ctrl_ptr->line_info_ptr[tp_line_index].start_pos + char_offset);

    //reset highlight not display
    GUITEXT_DisplayHighlight(FALSE,text_ctrl_ptr);

    //move on highlight
    if (GUITEXT_TP_REGION_HIGHLIGHT == text_ctrl_ptr->tp_region)
    {
        //reset tp region
        text_ctrl_ptr->tp_region = GUITEXT_TP_REGION_NONE;

        text_ctrl_ptr->highlight.start_pos = tp_pos;
    }

    if (GUITEXT_SLIDER_LEFT_SELECTOR == text_ctrl_ptr->tpdown_slider)
    {
        if (tp_pos < text_ctrl_ptr->highlight.end_pos)
        {
            text_ctrl_ptr->highlight.start_pos = tp_pos;
        }
    }
    else if (GUITEXT_SLIDER_RIGHT_SELECTOR == text_ctrl_ptr->tpdown_slider
        || MMITHEME_DRAGMODE_DEFAULT == text_ctrl_ptr->theme.drag_mode)
    {
        if (tp_pos > text_ctrl_ptr->highlight.start_pos)
        {
            text_ctrl_ptr->highlight.end_pos = tp_pos;
        }
    }

    //display highlight
    GUITEXT_DisplayHighlight(TRUE,text_ctrl_ptr);
}

/*****************************************************************************/
//  Description : start tp scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpScrollTimer(
                              BOOLEAN           is_scroll_up,   //in:
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                              )
{
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //include progress bar
    if ((0 == text_ctrl_ptr->tp_scroll_timer_id) &&
        (MMK_IsFocusWin(text_ctrl_ptr->win_handle)))
    {
        text_ctrl_ptr->tp_scroll_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
                                                text_ctrl_ptr->theme.tp_scroll_period,
                                                FALSE);

        //set tp scroll direction
        text_ctrl_ptr->is_tp_scroll_up = is_scroll_up;
    }
}

/*****************************************************************************/
//  Description : stop tp scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_StopTpScrollTimer(
                                      CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                                      )
{
    if (0 < text_ctrl_ptr->tp_scroll_timer_id)
    {
        MMK_StopTimer(text_ctrl_ptr->tp_scroll_timer_id);
        text_ctrl_ptr->tp_scroll_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle text tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpUp(
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr,
                                  DPARAM            param
                                  )
{
    uint16          highlight_len = 0;
    uint16          tp_line_index = 0;
    uint16          char_offset = 0;
    uint16          tp_pos = 0;
    GUI_POINT_T     tp_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    GUITEXT_TAG_T   *tp_tag_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //is tp down
    text_ctrl_ptr->is_tp_down = FALSE;

    //not slide
    if ((text_ctrl_ptr->theme.is_handle_tp) &&
        (0 < text_ctrl_ptr->str_len))
    {
        //stop tp scroll timer
        GUITEXT_StopTpScrollTimer(text_ctrl_ptr);

        //stop slide timer
        StopTpSlideTimer(text_ctrl_ptr);

        //not slide
        if ((!MMK_GET_TP_SLIDE(param)) &&
            (0 < text_ctrl_ptr->str_len))
        {
            //set tp press point
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
    		
    		if (MMK_TP_SLIDE_NONE != text_ctrl_ptr->slide_state)
    		{
    			//text is slide
                ((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->is_slide = TRUE;
    		}

            if ((text_ctrl_ptr->theme.is_support_slide) &&
                ((MMK_TP_SLIDE_NONE != text_ctrl_ptr->slide_state) ||
                 ((IS_TP_MOVE_Y(text_ctrl_ptr->prev_point.y, tp_point.y)) &&
			     (GUITEXT_TP_STATE_DRAG != text_ctrl_ptr->tp_state))))
            {
                //add velocity item
                MMK_AddVelocityItem(tp_point.x,tp_point.y);
                
                //calculate current velocity
                MMK_ComputeCurrentVelocity(PNULL,&text_ctrl_ptr->fling_velocity,FLING_MAX_VELOCITY, FLING_MAX_VELOCITY );

                //fling velocity
                if (FLING_MIN_VELOCITY > abs((int32)text_ctrl_ptr->fling_velocity))
                {
                    //reset velocity item
                    MMK_ResetVelocityItem();

                    text_ctrl_ptr->fling_velocity = 0;

                    //reset slide state
                    text_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

                    //reset text slide
                    ResetTextSlide(TRUE,text_ctrl_ptr);
                }
                else
                {
                    //enter fling state
                    text_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;

                    //start tp slide timer
                    StartTpSlideTimer(text_ctrl_ptr);
                }
            }
            else
            {
                //get highlight len
                highlight_len = GUITEXT_GetHighlightLen(text_ctrl_ptr);
            
                if ((0 < highlight_len) ||
                    (0 < text_ctrl_ptr->tag_num))
                {
                    if (-1 != text_ctrl_ptr->tp_down_y)
                    {
                        uint16 line_height = 0;

                        //get line height
                        line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

                        // 如果未超过规定的高度，则认为纵向不准备移动
                        if (abs(text_ctrl_ptr->tp_down_y - tp_point.y) < line_height * 3 / 4)
                        {
                            tp_point.y = text_ctrl_ptr->tp_down_y;
                        }
                    }

                    //get tp line index
                    GetTpLineIndex(&tp_line_index,tp_point,text_ctrl_ptr);

                    //get tp x offset
                    char_offset = GetTpCharOffset(PNULL,tp_line_index,tp_point,text_ctrl_ptr);

                    //set tp position
                    tp_pos = (uint16)(text_ctrl_ptr->line_info_ptr[tp_line_index].start_pos + char_offset);

                    //reset highlight not display
                    GUITEXT_DisplayHighlight(FALSE,text_ctrl_ptr);

                    //has highlight
                    if (0 < highlight_len)
                    {
                        //tp on highlight
                        if (GUITEXT_TP_REGION_HIGHLIGHT == text_ctrl_ptr->tp_region)
                        {
                            //set highlight
                            text_ctrl_ptr->highlight.start_pos = tp_pos;
                            text_ctrl_ptr->highlight.end_pos   = text_ctrl_ptr->highlight.start_pos;
                        }
                        else if (GUITEXT_TP_STATE_DRAG == text_ctrl_ptr->tp_state)
                        {
                            text_ctrl_ptr->highlight.end_pos = tp_pos;

                            //display highlight
                            GUITEXT_DisplayHighlight(TRUE,text_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
                            //display clip board menu
                            GUITEXT_DisplayClipboardMenu(&tp_point,text_ctrl_ptr);
#endif
                        }
                        else
                        {
                            //notify parent control
                            CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENOK);
                        }
                    }
                    else if (0 < text_ctrl_ptr->tag_num)
                    {
                        //is tp on tag
                        if (IsTpOnTag(tp_pos,&text_ctrl_ptr->line_info_ptr[tp_line_index],&tp_tag_ptr))
                        {
                            //set current tag
                            text_ctrl_ptr->cur_tag_ptr = tp_tag_ptr;

                            //set highlight
                            text_ctrl_ptr->highlight.start_pos = tp_tag_ptr->start_pos;
                            text_ctrl_ptr->highlight.end_pos   = (uint16)(tp_tag_ptr->start_pos + tp_tag_ptr->len);

                            //display highlight
                            GUITEXT_DisplayHighlight(TRUE,text_ctrl_ptr);

                            //notify parent control
                            CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENOK);
                        }
                        else
                        {
                            //notify parent control
                            CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENOK);
                        }
                    }

#ifdef GUIF_CLIPBOARD
                    GUITEXT_ChangeDragState(text_ctrl_ptr,GUITEXT_DRAGSTATE_NORMAL);
#endif

                    result = MMI_RESULT_TRUE;
                }
                else
                {
                    //notify parent control
                    CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENOK);
                }
            }
        }
    }
    else
    {
        result = HandleTextTagTpUp(text_ctrl_ptr,param);
    }

    return (result);
}

/*****************************************************************************/
//  Description : is tp on tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpOnTag(
                        uint16          tp_pos,         //in:
                        GUITEXT_LINE_T  *line_info_ptr, //in:
                        GUITEXT_TAG_T   **tp_tag_pptr   //in/out:
                        )
{
    BOOLEAN         result = FALSE;
    uint16          i = 0;
    uint16          tag_end_pos = 0;
    GUITEXT_TAG_T   *tag_ptr = PNULL;

    if (0 < line_info_ptr->tag_num)
    {
        //set start tag
        tag_ptr = line_info_ptr->start_tag_ptr;

        for (i=0; i<line_info_ptr->tag_num; i++)
        {
            //set tag end pos
            tag_end_pos = (uint16)(tag_ptr->start_pos + tag_ptr->len);
            if ((tp_pos >= tag_ptr->start_pos) &&
                (tp_pos <= tag_end_pos))
            {
                if (PNULL != tp_tag_pptr)
                {
                    *tp_tag_pptr = tag_ptr;
                }
                result = TRUE;
                break;
            }

            //set next tag
            tag_ptr = tag_ptr->next_ptr;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle text tag tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTagTpUp(
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                     DPARAM             param
                                     )
{
    uint16          tp_line_index = 0;
    uint16          char_offset = 0;
    uint16          tp_pos = 0;
    GUI_POINT_T     tp_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    GUITEXT_TAG_T   *tp_tag_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //not slide
    if ((!MMK_GET_TP_SLIDE(param)) &&
        (0 < text_ctrl_ptr->str_len) && 
        (0 < text_ctrl_ptr->tag_num))
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        if (-1 != text_ctrl_ptr->tp_down_y)
        {
            uint16 line_height = 0;

            //get line height
            line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

            // 如果未超过规定的高度，则认为纵向不准备移动
            if (abs(text_ctrl_ptr->tp_down_y - tp_point.y) < line_height * 3 / 4)
            {
                tp_point.y = text_ctrl_ptr->tp_down_y;
            }
        }

        //get tp line index
        GetTpLineIndex(&tp_line_index,tp_point,text_ctrl_ptr);

        //get tp x offset
        char_offset = GetTpCharOffset(PNULL,tp_line_index,tp_point,text_ctrl_ptr);

        //set tp position
        tp_pos = (uint16)(text_ctrl_ptr->line_info_ptr[tp_line_index].start_pos + char_offset);

        //reset highlight not display
        GUITEXT_DisplayHighlight(FALSE,text_ctrl_ptr);

        //is tp on tag
        if (IsTpOnTag(tp_pos,&text_ctrl_ptr->line_info_ptr[tp_line_index],&tp_tag_ptr))
        {
            //set current tag
            text_ctrl_ptr->cur_tag_ptr = tp_tag_ptr;

            //set highlight
            text_ctrl_ptr->highlight.start_pos = tp_tag_ptr->start_pos;
            text_ctrl_ptr->highlight.end_pos   = (uint16)(tp_tag_ptr->start_pos + tp_tag_ptr->len);

            //display highlight
            GUITEXT_DisplayHighlight(TRUE,text_ctrl_ptr);

            //notify parent control
            CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENOK);
        }

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle text tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextSlideTimer(
                                CTRLTEXT_OBJ_T      *text_ctrl_ptr
                                )
{
    GUI_POINT_T         tp_point = {0};
    MMI_TP_STATUS_E     tp_status = MMI_TP_NONE;

    //stop slide timer
    StopTpSlideTimer(text_ctrl_ptr);

    if (MMK_IsFocusWin(text_ctrl_ptr->win_handle))
    {
        //start tp slide timer
        StartTpSlideTimer(text_ctrl_ptr);
        
        if (MMK_TP_SLIDE_NONE != text_ctrl_ptr->slide_state)
        {
            MMK_GetLogicTPMsg(&tp_status,&tp_point);

            //handle slide
            if ((!text_ctrl_ptr->is_tp_down) || IS_TP_MOVE_Y(text_ctrl_ptr->prev_point.y, tp_point.y))
            {
                HandleTextSlide(tp_point,text_ctrl_ptr);
                
                //set previous point
                text_ctrl_ptr->prev_point = tp_point;
            }

        }
    }
}

/*****************************************************************************/
//  Description : handle text slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextSlide(
                           GUI_POINT_T      tp_point,
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           )
{
    BOOLEAN     is_reset = FALSE;
    uint16      display_height = 0;
    int32       scroll_step = 0;
    int32       display_top = 0;
    int32       display_max_top = 0;

    //init display top
    display_top = text_ctrl_ptr->relative_disp_rect.top;

    switch (text_ctrl_ptr->slide_state)
    {
    case MMK_TP_SLIDE_TP_SCROLL:
        scroll_step = tp_point.y - text_ctrl_ptr->prev_point.y;

        display_top = display_top - scroll_step;
        break;

    case MMK_TP_SLIDE_FLING:
        //get fling offset
        scroll_step = MMK_GetFlingOffset(0,text_ctrl_ptr->fling_velocity,(float)FLING_TIME,&text_ctrl_ptr->fling_velocity);
        
        //get display height
        display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

        //get display max top
        display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

        if (0 == text_ctrl_ptr->fling_velocity)
        {
            //stop slide timer
            StopTpSlideTimer(text_ctrl_ptr);

            //reset slide state
            text_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

            is_reset = TRUE;
        }
        else if (0 > text_ctrl_ptr->fling_velocity)//up
        {
            //set display top
            display_top = display_top - scroll_step;

            //last item display 1/2 in display rect
            if (display_top > display_max_top + (display_height>>1))
            {
                //stop slide timer
                StopTpSlideTimer(text_ctrl_ptr);

                //reset slide state
                text_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

                display_top = display_max_top + (display_height>>1);

                is_reset = TRUE;
            }
        }
        else//down
        {
            //set display top
            display_top = display_top - scroll_step;

            //last item display 1/2 in display rect
            if ((0 > display_top) &&
                ((-display_top) > (display_height>>1)))
            {
                //stop slide timer
                StopTpSlideTimer(text_ctrl_ptr);

                //reset slide state
                text_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;

                display_top = -(display_height>>1);

                is_reset = TRUE;
            }
        }
        break;

    default:
        break;
    }

    if (GUITEXT_SetDisplayRect(display_top,text_ctrl_ptr))
    {
        //get top and bottom index
        GetTopBottomIndex(text_ctrl_ptr);

        //update display
        GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
    }

    //resest
    if (is_reset)
    {
        ResetTextSlide(TRUE,text_ctrl_ptr);
    }
    else
    {
        UILAYER_SetDirectDraw(TRUE);
    }
}

/*****************************************************************************/
//  Description : reset text slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetTextSlide(
                          BOOLEAN           is_update,
                          CTRLTEXT_OBJ_T    *text_ctrl_ptr
                          )
{
    uint16      i = 0;
    uint16      reset_slide_frame = 0;
    int32       display_top = 0;
    int32       display_max_top = 0;
    int32       move_y_offset = 0;
    int32       scroll_step = 0;
    BOOLEAN     is_draw = FALSE;

    //init display top
    display_top = text_ctrl_ptr->relative_disp_rect.top;

    //get display max top
    display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

    //is need reset
    if ((0 > display_top) ||
        (display_max_top < display_top))
    {
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif

        if (0 > display_top)
        {
            move_y_offset = -display_top;
        }
        else
        {
            move_y_offset = display_max_top - display_top;
        }

        if (is_update)
        {
            reset_slide_frame = text_ctrl_ptr->theme.reset_slide_frame;
        }
        else
        {
            reset_slide_frame = text_ctrl_ptr->theme.reset_slide_start;
        }

        for (i=text_ctrl_ptr->theme.reset_slide_start; i<=reset_slide_frame; i++)
        {
            //calculate scroll step
            scroll_step = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,(int16)move_y_offset,reset_slide_frame,i);

            //set display top
            if (GUITEXT_SetDisplayRect(display_top + scroll_step,text_ctrl_ptr))
            {
                //get top and bottom index
                GetTopBottomIndex(text_ctrl_ptr);

                //update display
                GUITEXT_DisplayText(TRUE,text_ctrl_ptr);

                is_draw = TRUE;

                if (i != reset_slide_frame)
                {
                    //update
                    MMITHEME_UpdateRect();
                }
            }
        }

#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif
    }
    
    //for direct draw
    if ( is_update
        && !is_draw )
    {
        //get top and bottom index
        GetTopBottomIndex(text_ctrl_ptr);
        
        //update display
        GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : handle text tp long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTextTpLong(
                                    CTRLTEXT_OBJ_T      *text_ctrl_ptr,
                                    DPARAM              param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;

#ifdef GUIF_CLIPBOARD
    GUI_POINT_T     tp_point = {0};

    if ((MMK_TP_SLIDE_NONE == text_ctrl_ptr->slide_state) &&
        (text_ctrl_ptr->theme.is_handle_tp) &&
        (0 < text_ctrl_ptr->str_len))
    {
        // show clipboard only when not moved.
        if (GUITEXT_IsSupportClipbrd(text_ctrl_ptr) && !text_ctrl_ptr->is_tp_moved)
        {
            GUITEXT_HandleTextSelectMsg(text_ctrl_ptr);

            //// 潜规则
            //// 后面会打开窗口，在LOSE FOCUS消息里拷LCD内容，这里主层还没画到LCD上，
            //// 不会显示到出来，这里主动更新一下
            ////MMITHEME_UpdateRect();
            //GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );

            //set tp press point
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);

            //display clip board menu
            GUITEXT_DisplayClipboardMenu(&tp_point,text_ctrl_ptr);
        }

        result = MMI_RESULT_TRUE;
    }
#endif

    return (result);
}


/*****************************************************************************/
//  Description : handle text tp progress bar msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTextTpPrg(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr, //in:
                           MMI_MESSAGE_ID_E  msg_id,         //in:
                           DPARAM            param           //in:
                           )
{
    BOOLEAN     is_up = FALSE;
    uint16      display_height = 0;
    int32       display_max_top = 0;
    int32       scroll_pixel = 0;
    int32       display_top = 0;

    //get display height and max top
    display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

    //get display max top
    display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

    switch (msg_id)
    {
    case MSG_CTL_PRGBOX_MOVEUP_ONE:
        is_up = TRUE;

        //get scroll pixel
        scroll_pixel = GetScrollPixel(text_ctrl_ptr);
        break;

    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
        is_up = FALSE;

        //get scroll pixel
        scroll_pixel = GetScrollPixel(text_ctrl_ptr);
        break;

    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
        is_up = TRUE;

        //get scroll step
        scroll_pixel = display_height;
        break;

    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
        is_up = FALSE;

        //get scroll step
        scroll_pixel = display_height;
        break;

    case MSG_CTL_PRGBOX_MOVE:
        //set display top
        display_top = *(uint32*)param;
        display_top = (int32)MIN(display_top,display_max_top);
        break;

    default:
        SCI_PASSERT(FALSE,("HandleTextTpPrg: msg_id %d is error!",msg_id)); /*assert verified*/
        break;
    }

    if (MSG_CTL_PRGBOX_MOVE != msg_id)
    {
        if (is_up)
        {
            //set display top
            if (text_ctrl_ptr->relative_disp_rect.top > scroll_pixel)
            {
                display_top = text_ctrl_ptr->relative_disp_rect.top - scroll_pixel;
            }
            else
            {
                display_top = 0;
            }
        }
        else
        {
            //set display top
            display_top = (int32)MIN((text_ctrl_ptr->relative_disp_rect.top + scroll_pixel),display_max_top);
        }
    }

    //set display rect
    if (GUITEXT_SetDisplayRect(display_top,text_ctrl_ptr))
    {
        //get top and bottom index
        GetTopBottomIndex(text_ctrl_ptr);

        //update display
        GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
    }
}
#endif
/*****************************************************************************/
//  Description : get edit string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetString(
								 MMI_CTRL_ID_T	ctrl_id,    //in:
								 MMI_STRING_T	*str_ptr    //in:/out
								 )
{
	BOOLEAN				result = FALSE;
    CTRLTEXT_OBJ_T		*text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if ((PNULL != text_ctrl_ptr) &&
        (PNULL != str_ptr))
    {
        //set string
        str_ptr->wstr_ptr = text_ctrl_ptr->str_ptr;
        str_ptr->wstr_len = text_ctrl_ptr->str_len;

		result = TRUE;
    }

	return (result);
}

/*****************************************************************************/
//  Description : set text string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetString(
                                 MMI_CTRL_ID_T      ctrl_id,    //in:
                                 wchar              *str_ptr,   //in:
                                 uint16             str_len,    //in:
                                 BOOLEAN            is_update   //in:
                                 )
{
    BOOLEAN				result = FALSE;
    CTRLTEXT_OBJ_T		*text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
#ifdef GUIF_CLIPBOARD
        GUITEXT_CloseClipboardMenu(text_ctrl_ptr);
#endif

        //set text string by pointer
        result = SetTextStrByPtr(text_ctrl_ptr,str_ptr,str_len,is_update);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get text pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLTEXT_OBJ_T* GetTextPtr(
                                 MMI_HANDLE_T   ctrl_handle
                                 )
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(ctrl_handle);

    if ((PNULL == ctrl_ptr) || (!TextTypeOf(ctrl_ptr)))
    {
        ctrl_ptr = PNULL;
    }

    return (CTRLTEXT_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TextTypeOf(
                         CTRLBASE_OBJ_T     *text_ptr
                         )
{
    return TYPEMNG_IsTypeOf(text_ptr, CTRL_TEXT_TYPE);
}

/*****************************************************************************/
//  Description : set text string by pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetTextStrByPtr(
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr, //in:
                              wchar             *str_ptr,   //in:
                              uint16            str_len,    //in:
                              BOOLEAN           is_update   //in:
                              )
{
    BOOLEAN     result = TRUE;

	if ((0 < str_len) &&
        (PNULL != str_ptr))
	{
        //clear all string
        ClearTextString(text_ctrl_ptr);

        //add string
        AddTextString(text_ctrl_ptr,str_ptr,str_len);

        //set relative rect
        SetRelativeRect(text_ctrl_ptr);

        //get top and bottom index
        GetTopBottomIndex(text_ctrl_ptr);

        if ((is_update) &&
            (MMK_IsFocusWin(text_ctrl_ptr->win_handle)))
        {
            //update display
            GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
        }
	}

    return (result);
}

/*****************************************************************************/
//  Description : add text string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddTextString(
                         CTRLTEXT_OBJ_T     *text_ctrl_ptr, //in:
                         wchar              *str_ptr,       //in:
                         uint16             str_len         //in:
                         )
{
    //alloc string memory
    SCI_ASSERT(PNULL == text_ctrl_ptr->str_ptr); /*assert verified*/
    text_ctrl_ptr->str_ptr = AllocStrMemory(str_len);

    //alloc line info memory
    SCI_ASSERT(PNULL == text_ctrl_ptr->line_info_ptr); /*assert verified*/
    text_ctrl_ptr->line_info_ptr = AllocLineMemory(str_len);

    //copy string
    MMI_MEMCPY(text_ctrl_ptr->str_ptr,(str_len*sizeof(wchar)),
        str_ptr,(str_len*sizeof(wchar)),
        (str_len*sizeof(wchar)));
    text_ctrl_ptr->str_len = str_len;

    //is need progress bar
    JudgeAndCreatePrgCtrl(TRUE,text_ctrl_ptr);

    //parse tag
    ParsetTextTag(text_ctrl_ptr,PNULL);

    //parse string
    ParseTextString(text_ctrl_ptr);
}

/*****************************************************************************/
//  Description : alloc text string memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* AllocStrMemory(
                            uint16      max_len
                            )
{
    wchar   *text_str_ptr = PNULL;

    if (0 < max_len)
    {
        text_str_ptr = SCI_ALLOC_APP((max_len+1)*sizeof(wchar));

        SCI_MEMSET(text_str_ptr,0,((max_len+1)*sizeof(wchar)));
    }

    return (text_str_ptr);
}

/*****************************************************************************/
//  Description : alloc text line info memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUITEXT_LINE_T* AllocLineMemory(
                                      uint16    max_len
                                      )
{
    GUITEXT_LINE_T  *line_info_ptr = PNULL;

    if (0 < max_len)
    {
        line_info_ptr = SCI_ALLOC_APP((max_len+1)*sizeof(GUITEXT_LINE_T));

        SCI_MEMSET(line_info_ptr,0,((max_len+1)*sizeof(GUITEXT_LINE_T)));
    }

    return (line_info_ptr);
}

/*****************************************************************************/
//  Description : free text tag info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FreeTagInfo(
                       CTRLTEXT_OBJ_T   *text_ctrl_ptr
                       )
{
    GUITEXT_TAG_T   *tag_ptr = PNULL;
    GUITEXT_TAG_T   *tag_next_ptr = PNULL;

    tag_ptr = text_ctrl_ptr->first_tag_ptr;
    while (PNULL != tag_ptr)
    {
        tag_next_ptr = tag_ptr->next_ptr;

        //free
        SCI_FREE(tag_ptr);

        tag_ptr = tag_next_ptr;
    }

    text_ctrl_ptr->tag_num       = 0;
    text_ctrl_ptr->first_tag_ptr = PNULL;
    text_ctrl_ptr->cur_tag_ptr   = PNULL;
}

/*****************************************************************************/
//  Description : handle text up key msg for tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextTagUp(
                              int32             *disp_top_ptr,  //in/out:       
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr
                              )
{
    BOOLEAN         result = FALSE;
    uint16          display_height = 0;
    int32           display_top = 0;
    int32           cur_tag_top = 0;
    int32           next_tag_top = 0;
    int32           next_tag_bottom = 0;
    int32           display_max_top = 0;
    GUITEXT_TAG_T   *cur_tag_ptr = PNULL;
    GUITEXT_TAG_T   *next_tag_ptr = PNULL;

    //set init display top
    display_top = text_ctrl_ptr->relative_disp_rect.top;

    if (0 < text_ctrl_ptr->tag_num)
    {
        //set current tag
        cur_tag_ptr = text_ctrl_ptr->cur_tag_ptr;

        //get tag top
        cur_tag_top = GetTagRelativeTop(cur_tag_ptr,text_ctrl_ptr);

        //set next tag
        if (cur_tag_ptr != text_ctrl_ptr->first_tag_ptr)
        {
            next_tag_ptr = cur_tag_ptr->prev_ptr;
        }

        //get next tag top and bottom
        if (PNULL == next_tag_ptr)
        {
            next_tag_top    = 0;
            next_tag_bottom = GUITEXT_GetLineHeight(text_ctrl_ptr);
        }
        else
        {
            next_tag_top    = GetTagRelativeTop(next_tag_ptr,text_ctrl_ptr);
            next_tag_bottom = GetTagRelativeBottom(next_tag_ptr,text_ctrl_ptr);
        }

        //current tag top is not display all
        //next tag bottom is not display
        if ((text_ctrl_ptr->relative_disp_rect.top > cur_tag_top) ||
            (next_tag_bottom < text_ctrl_ptr->relative_disp_rect.top))
        {
            //get display top by scroll
            ScrollTextLines(TRUE,&display_top,text_ctrl_ptr);

            //not change tag
            next_tag_ptr = cur_tag_ptr;
        }
        else
        {
            //change tag
            //next tag is not display all
            if ((next_tag_top < text_ctrl_ptr->relative_disp_rect.top) ||
                (next_tag_bottom > text_ctrl_ptr->relative_disp_rect.bottom))
            {
                //get display height
                display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

                //next tag height > display height
                if ((next_tag_bottom - next_tag_top + 1) > display_height)
                {
                    //set display top
                    display_top = next_tag_bottom - display_height + 1;
                }
                else
                {
                    //get display top by next tag top
                    display_top = next_tag_top;
                }

                //get display max top
                display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

                //adjust display top
                display_top = (int32)MIN(display_top,display_max_top);
            }
        }

        if ((PNULL != next_tag_ptr) && 
            (cur_tag_ptr != next_tag_ptr))
        {
            //set current tag
            text_ctrl_ptr->cur_tag_ptr = next_tag_ptr;

            //set highlight
            text_ctrl_ptr->highlight.start_pos = next_tag_ptr->start_pos;
            text_ctrl_ptr->highlight.end_pos   = (uint16)(next_tag_ptr->start_pos + next_tag_ptr->len);

            result = TRUE;
        }
        else
        {
            if (IsSetTagHighlight(cur_tag_ptr,text_ctrl_ptr))
            {
                //set highlight
                text_ctrl_ptr->highlight.start_pos = cur_tag_ptr->start_pos;
                text_ctrl_ptr->highlight.end_pos   = (uint16)(cur_tag_ptr->start_pos + cur_tag_ptr->len);

                result = TRUE;
            }
        }
    }

    //set display top
    if (PNULL != disp_top_ptr)
    {
        *disp_top_ptr = display_top;
    }

    //set result
    if (display_top != text_ctrl_ptr->relative_disp_rect.top)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle text down key msg for tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTextTagDown(
                                int32           *disp_top_ptr,  //in/out:       
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                )
{
    BOOLEAN         result = FALSE;
    uint16          display_height = 0;
    int32           display_top = 0;
    int32           cur_tag_bottom = 0;
    int32           next_tag_top = 0;
    int32           next_tag_bottom = 0;
    int32           display_max_top = 0;
    GUITEXT_TAG_T   *cur_tag_ptr = PNULL;
    GUITEXT_TAG_T   *next_tag_ptr = PNULL;

    //set init display top
    display_top = text_ctrl_ptr->relative_disp_rect.top;

    if (0 < text_ctrl_ptr->tag_num)
    {
        //set current tag
        cur_tag_ptr = text_ctrl_ptr->cur_tag_ptr;

        //set next tag
        next_tag_ptr = cur_tag_ptr->next_ptr;

        //get tag bottom
        cur_tag_bottom = GetTagRelativeBottom(cur_tag_ptr,text_ctrl_ptr);

        //get tag top and bottom
        if (PNULL == next_tag_ptr)
        {
            next_tag_top    = text_ctrl_ptr->relative_rect.bottom - GUITEXT_GetLineHeight(text_ctrl_ptr);
            next_tag_bottom = text_ctrl_ptr->relative_rect.bottom;
        }
        else
        {
            next_tag_top    = GetTagRelativeTop(next_tag_ptr,text_ctrl_ptr);
            next_tag_bottom = GetTagRelativeBottom(next_tag_ptr,text_ctrl_ptr);
        }

        //current tag bottom is not display all
        //next tag top is not display
        if ((text_ctrl_ptr->relative_disp_rect.bottom < cur_tag_bottom) ||
            (next_tag_top >= text_ctrl_ptr->relative_disp_rect.bottom))
        {
            //get display top by scroll
            ScrollTextLines(FALSE,&display_top,text_ctrl_ptr);

            //not change tag
            next_tag_ptr = cur_tag_ptr;
        }
        else
        {
            //change tag
            //next tag is not display all
            if ((next_tag_top < text_ctrl_ptr->relative_disp_rect.top) ||
                (next_tag_bottom > text_ctrl_ptr->relative_disp_rect.bottom))
            {
                //get display height
                display_height = GUITEXT_GetDispHeight(text_ctrl_ptr);

                //next tag height > display height
                if ((next_tag_bottom - next_tag_top + 1) > display_height)
                {
                    //set display top
                    display_top = next_tag_top;
                }
                else
                {
                    //get display top by child bottom
                    if (next_tag_bottom < display_height)
                    {
                        display_top = next_tag_top;
                    }
                    else
                    {
                        display_top = next_tag_bottom - display_height + 1;
                    }
                }

                //get display max top
                display_max_top = GUITEXT_GetDispMaxTop(text_ctrl_ptr);

                //adjust display top
                display_top = (int32)MIN(display_top,display_max_top);
            }
        }

        if ((PNULL != next_tag_ptr) && 
            (cur_tag_ptr != next_tag_ptr))
        {
            //set current tag
            text_ctrl_ptr->cur_tag_ptr = next_tag_ptr;

            //set highlight
            text_ctrl_ptr->highlight.start_pos = (uint16)(next_tag_ptr->start_pos + next_tag_ptr->len);
            text_ctrl_ptr->highlight.end_pos   = next_tag_ptr->start_pos;

            result = TRUE;
        }
        else
        {
            if (IsSetTagHighlight(cur_tag_ptr,text_ctrl_ptr))
            {
                //set highlight
                text_ctrl_ptr->highlight.start_pos = (uint16)(cur_tag_ptr->start_pos + cur_tag_ptr->len);
                text_ctrl_ptr->highlight.end_pos   = cur_tag_ptr->start_pos;

                result = TRUE;
            }
        }
    }

    //set display top
    if (PNULL != disp_top_ptr)
    {
        *disp_top_ptr = display_top;
    }

    //set result
    if (display_top != text_ctrl_ptr->relative_disp_rect.top)
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get tag relative top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetTagRelativeTop(
                              GUITEXT_TAG_T     *tag_ptr,
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr
                              )
{
    uint16          line_index = 0;
    GUI_BIG_RECT_T  relative_rect = {0};

    if (PNULL != tag_ptr)
    {
        //get line index
        line_index = GUITEXT_GetHighlightLineIndex(FALSE,tag_ptr->start_pos,text_ctrl_ptr);

        //get icon relative rect
        if (0 == line_index)
        {
            //first line,include margin
            relative_rect.top = 0;
        }
        else
        {
            relative_rect = GUITEXT_GetLineRelativeRect(line_index,text_ctrl_ptr);
        }
    }

    return (relative_rect.top);
}

/*****************************************************************************/
//  Description : get tag relative bottom
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL int32 GetTagRelativeBottom(
                                 GUITEXT_TAG_T      *tag_ptr,
                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                 )
{
    uint16          tag_end_pos = 0;
    uint16          line_index = 0;
    GUI_BIG_RECT_T  relative_rect = {0};

    if (PNULL != tag_ptr)
    {
        //get tag end pos
        tag_end_pos = (uint16)(tag_ptr->start_pos + tag_ptr->len);

        //get line index
        line_index = GUITEXT_GetHighlightLineIndex(TRUE,tag_end_pos,text_ctrl_ptr);

        //get icon relative rect
        if (line_index + 1 == text_ctrl_ptr->total_line_num)
        {
            //last line,include margin
            relative_rect.bottom = text_ctrl_ptr->relative_rect.bottom;
        }
        else
        {
            relative_rect = GUITEXT_GetLineRelativeRect(line_index,text_ctrl_ptr);
        }
    }

    return (relative_rect.bottom);
}

/*****************************************************************************/
//  Description : is set tag highlight
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSetTagHighlight(
                                GUITEXT_TAG_T   *tag_ptr,       //in:
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                )
{
    BOOLEAN     result = FALSE;
    uint16      highlight_len = 0;
    uint16      highlight_min_pos = 0;
    uint16      highlight_max_pos = 0;

    if (PNULL != tag_ptr)
    {
        //get highlight len
        highlight_len = GUITEXT_GetHighlightLen(text_ctrl_ptr);
        
        if (0 == highlight_len)
        {
            result = TRUE;
        }
        else
        {
            //get highlight min and max pos
            highlight_min_pos = (uint16)MIN(text_ctrl_ptr->highlight.start_pos,text_ctrl_ptr->highlight.end_pos);
            highlight_max_pos = (uint16)MAX(text_ctrl_ptr->highlight.start_pos,text_ctrl_ptr->highlight.end_pos);

            if ((highlight_min_pos != tag_ptr->start_pos) ||
                (highlight_max_pos != (uint16)(tag_ptr->start_pos + tag_ptr->len)))
            {
                result = TRUE;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : parse text tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParsetTextTag(
                         CTRLTEXT_OBJ_T     *text_ctrl_ptr, //in:
                         GUITEXT_SET_TAG_T  *set_tag_ptr    //in:may PNULL
                         )
{
     if ((0 < text_ctrl_ptr->str_len) && 
        (0 < text_ctrl_ptr->theme.tag_type))
    {
        /*lint -e655*/
        if (GUITEXT_TAG_SET & text_ctrl_ptr->theme.tag_type)
        {
            ParsetTextSetTag(text_ctrl_ptr,set_tag_ptr);
        }
        else
        {
            ParsetTextCommonTag(text_ctrl_ptr);
        }
        /*lint +e655*/
        
        //tag,not circular handle
        if (0 < text_ctrl_ptr->tag_num)
        {
            TextCtrlSetCircularHandle((CTRLBASE_OBJ_T *)text_ctrl_ptr,FALSE);
        }
     }
}

/*****************************************************************************/
//  Description : parse text set tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParsetTextSetTag(
                            CTRLTEXT_OBJ_T      *text_ctrl_ptr, //in:
                            GUITEXT_SET_TAG_T   *set_tag_ptr    //in:may PNULL
                            )
{
    uint16                  i = 0;
    GUITEXT_TAG_DATA_T      *tag_ptr = PNULL;
    GUITEXT_TAG_DATA_T      *prev_tag_ptr = PNULL;

    if ((PNULL != set_tag_ptr) &&
        (0 < set_tag_ptr->tag_num))
    {
        for (i=0; i<set_tag_ptr->tag_num; i++)
        {
            //set tag data
            tag_ptr = &set_tag_ptr->tag_ptr[i];

            if (0 == i)
            {
                prev_tag_ptr = PNULL;
            }
            else
            {
                prev_tag_ptr = &set_tag_ptr->tag_ptr[i-1];
            }

            if ((tag_ptr->start_pos < text_ctrl_ptr->str_len) &&
                ((tag_ptr->start_pos + tag_ptr->len ) <= text_ctrl_ptr->str_len) &&
                ((PNULL == prev_tag_ptr) || (tag_ptr->start_pos > (prev_tag_ptr->start_pos + prev_tag_ptr->len))))
            {
                //add one tag
                AddOneTag(text_ctrl_ptr,
                    tag_ptr->start_pos,
                    tag_ptr->len,
                    GUITEXT_TAG_SET);
            }
        }
    }
}

/*****************************************************************************/
//  Description : parse text common tag,include phone number,email or url
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParsetTextCommonTag(
                               CTRLTEXT_OBJ_T   *text_ctrl_ptr
                               )
{
    int32               tag_num = 0;
    URL_DATA_T          *tag_ptr = PNULL;
    URL_DATA_T          *cur_tag_ptr = PNULL;
    URL_PARSE_TYPE_T    parse_type = 0;
    GUITEXT_TAG_TYPE_E  tag_type = GUITEXT_TAG_NONE;

    /*lint -e655*/
    //parse type
    if (GUITEXT_TAG_PHONENUM & text_ctrl_ptr->theme.tag_type)
    {
        parse_type |= URL_PARSE_PHONENUM;
    }
    if (GUITEXT_TAG_EMAIL & text_ctrl_ptr->theme.tag_type)
    {
        parse_type |= URL_PARSE_EMAIL;
    }
    if (GUITEXT_TAG_URL & text_ctrl_ptr->theme.tag_type)
    {
        parse_type |= URL_PARSE_URL;
    }
    /*lint +e655*/
    
    //parse tag
    tag_num = URL_ParseDataGen(text_ctrl_ptr->str_ptr,parse_type,&tag_ptr);
    if (0 < tag_num)
    {
        //set current tag
        cur_tag_ptr = tag_ptr;

        while (PNULL != cur_tag_ptr)
        {
            //set tag type
            switch (cur_tag_ptr->type)
            {
            case URL_PARSE_PHONENUM:
                tag_type = GUITEXT_TAG_PHONENUM;
                break;

            case URL_PARSE_EMAIL:
                tag_type = GUITEXT_TAG_EMAIL;
                break;

            case URL_PARSE_URL:
                tag_type = GUITEXT_TAG_URL;
                break;

            default:
                break;
            }

            if (GUITEXT_TAG_NONE != tag_type)
            {
                //add one tag
                AddOneTag(text_ctrl_ptr,
                    (uint16)cur_tag_ptr->off_set,
                    (uint16)cur_tag_ptr->len,
                    tag_type);
            }

            //set next tag
            cur_tag_ptr = cur_tag_ptr->next;
        }
    }

    //release parse data
    URL_ParseDataRelease(tag_ptr);
}

/*****************************************************************************/
//  Description : add one tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AddOneTag(
                     CTRLTEXT_OBJ_T         *text_ctrl_ptr, //in/out:
                     uint16                 start_pos,      //in:
                     uint16                 len,            //in:
                     GUITEXT_TAG_TYPE_E     tag_type        //in:
                     )
{
    GUITEXT_TAG_T   *new_tag_ptr = PNULL;

    //alloc tag item memory
    new_tag_ptr = SCI_ALLOC_APP(sizeof(GUITEXT_TAG_T));
    SCI_MEMSET(new_tag_ptr,0,sizeof(GUITEXT_TAG_T));

    //set tag item param
    new_tag_ptr->type      = tag_type;
    new_tag_ptr->start_pos = start_pos;
    new_tag_ptr->len       = len;

    //add to tag list,将末尾指针存放在头指针的prev
    if (PNULL == text_ctrl_ptr->first_tag_ptr)
    {
        //set new child previous and next
        new_tag_ptr->prev_ptr = new_tag_ptr;
        new_tag_ptr->next_ptr = PNULL;

        //set first child previous
        text_ctrl_ptr->first_tag_ptr = new_tag_ptr;

        //set current tag
        text_ctrl_ptr->cur_tag_ptr = new_tag_ptr;

        //set highlight
        text_ctrl_ptr->highlight.start_pos = (uint16)(new_tag_ptr->start_pos + new_tag_ptr->len);
        text_ctrl_ptr->highlight.end_pos   = new_tag_ptr->start_pos;
    }
    else
    {
        //set new tag previous and next
        new_tag_ptr->prev_ptr = text_ctrl_ptr->first_tag_ptr->prev_ptr;
        new_tag_ptr->next_ptr = PNULL;

        //set previous child next
        text_ctrl_ptr->first_tag_ptr->prev_ptr->next_ptr = new_tag_ptr;

        //set first child previous
        text_ctrl_ptr->first_tag_ptr->prev_ptr = new_tag_ptr;
    }

    //set tag number
    text_ctrl_ptr->tag_num++;
}

/*****************************************************************************/
//  Description : parse text,include start position,total line number and top line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ParseTextString(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           )
{
    wchar           *remain_str_ptr = PNULL;
    uint16          remain_str_len = 0;
    uint16          line_index = 0;
    uint16          start_pos = 0;
    uint16          text_width = 0;
    uint16          char_num = 0;
    uint16          string_pixel = 0;
    GUI_RECT_T      text_rect = {0};
    GUITEXT_TAG_T   *tag_ptr = PNULL;

    SCI_ASSERT(PNULL != text_ctrl_ptr->line_info_ptr); /*assert verified*/

    //reset line info
    SCI_MEMSET(text_ctrl_ptr->line_info_ptr,
        0,
        ((text_ctrl_ptr->str_len+1)*sizeof(GUITEXT_LINE_T)));

    //get text string rect
    text_rect = GUITEXT_GetLineStrRect(text_ctrl_ptr->top_line_index,text_ctrl_ptr);
    if (!GUI_IsRectEmpty(text_rect))
    {
        //get text width
        text_width = (uint16)(text_rect.right - text_rect.left + 1);

        //set tag
        tag_ptr = text_ctrl_ptr->first_tag_ptr;

        //get line info
        start_pos = text_ctrl_ptr->line_info_ptr[line_index].start_pos;
        while (start_pos < text_ctrl_ptr->str_len)
        {
		    //set line start position and text direction
		    text_ctrl_ptr->line_info_ptr[line_index].start_pos = start_pos;

            //get remain string
            remain_str_ptr = text_ctrl_ptr->str_ptr + start_pos;
            remain_str_len = (uint16)(text_ctrl_ptr->str_len - start_pos);

		    //set line text direction
		    if ((0 == start_pos) ||
			    (GUI_IsNewlineChar(text_ctrl_ptr->str_ptr[start_pos - 1])))
		    {
			    text_ctrl_ptr->line_info_ptr[line_index].direction = SPMLAPI_GetTextDirection(remain_str_ptr,remain_str_len);
		    }
		    else
		    {
			    SCI_ASSERT(0 < line_index); /*assert verified*/
			    text_ctrl_ptr->line_info_ptr[line_index].direction = text_ctrl_ptr->line_info_ptr[line_index - 1].direction;
		    }

            //calculate char number in one line
		    if (GUI_GetStringInfoInLineWithDir(text_ctrl_ptr->line_info_ptr[line_index].direction,
			        remain_str_ptr,
                    remain_str_len,
                    text_ctrl_ptr->theme.font.font,
                    (uint8)text_ctrl_ptr->theme.char_space,
                    text_width,
                    &char_num,
                    &string_pixel,
                    TRUE))
            {
                //set line tag info
                SetLineTag(start_pos,
                    (uint16)(start_pos + char_num),
                    &tag_ptr,
                    &text_ctrl_ptr->line_info_ptr[line_index]);

                start_pos = (uint16)(start_pos + char_num);
                line_index++;
            }
            else
            {
                break;
            }
        }

        //judge the last char
        if ((0 < text_ctrl_ptr->str_len) &&
            (GUI_IsNewlineChar(text_ctrl_ptr->str_ptr[text_ctrl_ptr->str_len - 1])))
        {
            text_ctrl_ptr->line_info_ptr[line_index].start_pos = text_ctrl_ptr->str_len;
            line_index++;
        }

        //total line number
        text_ctrl_ptr->total_line_num = line_index;
        if (0 == text_ctrl_ptr->total_line_num)
        {
            text_ctrl_ptr->total_line_num = 1;
        }
    }
}

/*****************************************************************************/
//  Description : set line tag info
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLineTag(
                      uint16            line_start_pos, //in:
                      uint16            line_end_pos,   //in:
                      GUITEXT_TAG_T     **tag_pptr,     //in/out:
                      GUITEXT_LINE_T    *line_info_ptr  //in/out:
                      )
{
    uint16          tag_end_pos = 0;
    GUITEXT_TAG_T   *tag_ptr = *tag_pptr;

    while (PNULL != tag_ptr)
    {
        //set tag end pos
        tag_end_pos = (uint16)(tag_ptr->start_pos + tag_ptr->len);

        //set line tag info
        if (line_start_pos <= tag_ptr->start_pos)
        {
            if (line_end_pos > tag_ptr->start_pos)
            {
                //set tag
                if (PNULL == line_info_ptr->start_tag_ptr)
                {
                    line_info_ptr->start_tag_ptr = tag_ptr;
                }
                line_info_ptr->tag_num++;

                if (tag_end_pos <= line_end_pos)
                {
                    //next tag
                    tag_ptr = tag_ptr->next_ptr;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            if (line_start_pos < tag_end_pos)
            {
                //set tag
                if (PNULL == line_info_ptr->start_tag_ptr)
                {
                    line_info_ptr->start_tag_ptr = tag_ptr;
                }
                line_info_ptr->tag_num++;

                if (tag_end_pos <= line_end_pos)
                {
                    //next tag
                    tag_ptr = tag_ptr->next_ptr;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    //set output tag ptr
    *tag_pptr = tag_ptr;
}

/*****************************************************************************/
//  Description : clear text string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearTextString(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           )
{
    if (0 < text_ctrl_ptr->str_len)
    {
        //reset highlight
        text_ctrl_ptr->highlight.start_pos = 0;
        text_ctrl_ptr->highlight.end_pos = 0;

        //free text string memory
        if (PNULL != text_ctrl_ptr->str_ptr)
        {
            SCI_FREE(text_ctrl_ptr->str_ptr);
        }
        text_ctrl_ptr->str_len = 0;

        //free line info memory
        if (PNULL != text_ctrl_ptr->line_info_ptr)
        {
            SCI_FREE(text_ctrl_ptr->line_info_ptr);
        }

        //reset
        text_ctrl_ptr->top_line_index = 0;
        text_ctrl_ptr->total_line_num = 0;

        //destroy progress bar
        DestroyPrgCtrl(text_ctrl_ptr);
    }

    //free tag
    FreeTagInfo(text_ctrl_ptr);
}

/*****************************************************************************/
//  Description : set text relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRelativeRect(
                           CTRLTEXT_OBJ_T   *text_ctrl_ptr
                           )
{
    uint16      line_height = 0;
    uint32      relative_height = 0;

    //get line height
    line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

    //get all text height
    relative_height = text_ctrl_ptr->total_line_num * line_height;
    if (0 < text_ctrl_ptr->total_line_num)
    {
        relative_height = relative_height - text_ctrl_ptr->theme.line_space;
    }
    relative_height = relative_height + (text_ctrl_ptr->theme.margin_top_space + text_ctrl_ptr->theme.margin_bottom_space);

    //set relative rect
    text_ctrl_ptr->relative_rect = text_ctrl_ptr->relative_disp_rect;

    text_ctrl_ptr->relative_rect.top    = 0;
    text_ctrl_ptr->relative_rect.bottom = relative_height - 1;
}

/*****************************************************************************/
//  Description : get text top and bottom index by relative display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetTopBottomIndex(
                             CTRLTEXT_OBJ_T     *text_ctrl_ptr
                             )
{
    BOOLEAN             is_find = FALSE;
    uint16              i = 0;
    uint16              top_line_index = 0;
    uint16              line_height = 0;
    GUI_BIG_RECT_T      relative_rect = {0};
    GUI_BIG_RECT_T      dest_rect = {0};
    MMITHEME_TEXT_T     *theme_ptr = &text_ctrl_ptr->theme;

    //get top line index,text无滚动条时,由于align不同,统一从0开始计算
    if ((PNULL != text_ctrl_ptr->prgbox_ctrl_ptr) && 
        (text_ctrl_ptr->relative_disp_rect.top > theme_ptr->margin_top_space))
    {
        //get line height
        line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

        top_line_index = (uint16)((text_ctrl_ptr->relative_disp_rect.top - theme_ptr->margin_top_space)/line_height);
    }
    else
    {
        top_line_index = 0;
    }

    for (i=top_line_index; i<text_ctrl_ptr->total_line_num; i++)
    {
        //get line relative rect
        relative_rect = GUITEXT_GetLineRelativeRect(i,text_ctrl_ptr);
        
        //is cross
        if (GetCrossRect(&dest_rect,relative_rect,text_ctrl_ptr->relative_disp_rect))
        {
            //set top and bottom index
            if (!is_find)
            {
                is_find = TRUE;
                text_ctrl_ptr->top_line_index    = i;
                text_ctrl_ptr->bottom_line_index = i;
            }
            else
            {
                text_ctrl_ptr->bottom_line_index = i;
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
//  Description : set text rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_SetRect(
                            MMI_HANDLE_T    ctrl_handle,
                            GUI_RECT_T      *rect_ptr
                            )
{
    CTRLTEXT_OBJ_T  *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_handle);
    if (PNULL != text_ctrl_ptr)
    {
        TextCtrlSetRect((CTRLBASE_OBJ_T *)text_ctrl_ptr,rect_ptr);
    }
}

/*****************************************************************************/
//  Description : set text margin
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetMargin(
                                 MMI_CTRL_ID_T  ctrl_id,	//in:
                                 uint16			margin_space//in:
                                 )
{
    BOOLEAN             result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        text_ctrl_ptr->theme.margin_left_space = margin_space;
        text_ctrl_ptr->theme.margin_right_space = margin_space;
        text_ctrl_ptr->theme.margin_top_space = margin_space;
        text_ctrl_ptr->theme.margin_bottom_space = margin_space;
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set text margin
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetMarginEx(
                                   MMI_CTRL_ID_T   ctrl_id,    //in
                                   uint16          margin_left,  //in:left margin
                                   uint16          margin_right,   //in:right margin
                                   uint16          margin_top,  //in:top margin
                                   uint16          margin_bottom   //in:bottom margin
                                   )
{
    BOOLEAN             result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        text_ctrl_ptr->theme.margin_left_space = margin_left;
        text_ctrl_ptr->theme.margin_right_space = margin_right;
        text_ctrl_ptr->theme.margin_top_space = margin_top;
        text_ctrl_ptr->theme.margin_bottom_space = margin_bottom;
        result = TRUE;
    }

    return (result);
}
            
/*****************************************************************************/
//  Description : set text line space
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetLineSpace(
                                    MMI_CTRL_ID_T  ctrl_id,	//in:
                                    uint16		   line_space//in:
                                    )
{
    BOOLEAN             result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
		text_ctrl_ptr->theme.line_space = line_space;
		result = TRUE;
    }

    return (result);
}
                 
/*****************************************************************************/
//  Description : set text border,type, width and color
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetBorder(
                                 GUI_BORDER_T   *border_ptr,//in
                                 MMI_CTRL_ID_T  ctrl_id     //in:
                                 )
{
    BOOLEAN             result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        result = TextCtrlSetBorder((CTRLBASE_OBJ_T *)text_ctrl_ptr,border_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set text bg color or bg image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_SetBg(
                          MMI_CTRL_ID_T     ctrl_id,
                          GUI_BG_T          *bg_ptr
                          )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        //set bg
        text_ctrl_ptr->theme.bg = *bg_ptr;
    }
}

/*****************************************************************************/
//  Description : set text bg color or bg image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_SetFont(
                            MMI_CTRL_ID_T       ctrl_id,        //in:
                            GUI_FONT_T          *font_ptr,      //in:may PNULL
                            GUI_COLOR_T         *font_color_ptr //in:may PNULL
                            )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        //set font
        SetFontByPtr(text_ctrl_ptr,font_ptr,font_color_ptr);
    }
}

/*****************************************************************************/
//  Description : set text bg color or bg image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetFontByPtr(
                        CTRLTEXT_OBJ_T  *text_ctrl_ptr, //in:
                        GUI_FONT_T      *font_ptr,      //in:may PNULL
                        GUI_COLOR_T     *font_color_ptr //in:may PNULL
                        )
{
    BOOLEAN     is_reparse = FALSE;

    if ((PNULL != font_ptr) && 
        (*font_ptr != text_ctrl_ptr->theme.font.font))
    {
        text_ctrl_ptr->theme.font.font = *font_ptr;

        //get line number in one page
        text_ctrl_ptr->page_line_num = GetPageLineNum(text_ctrl_ptr);

        //destroy progress bar control
        DestroyPrgCtrl(text_ctrl_ptr);
        
        is_reparse = TRUE;
    }

    if (PNULL != font_color_ptr)
    {
        text_ctrl_ptr->theme.font.color = *font_color_ptr;
    }
    
    if ((is_reparse) &&
        (0 < text_ctrl_ptr->str_len))
    {
        //is need progress bar
        JudgeAndCreatePrgCtrl(TRUE,text_ctrl_ptr);

        //parse string
        ParseTextString(text_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : get text current tag
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetCurTag(
                                 MMI_CTRL_ID_T          ctrl_id,    //in:
                                 GUITEXT_TAG_INFO_T     *tag_ptr    //in/out:
                                 )
{
    BOOLEAN             result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);

    if ((PNULL != text_ctrl_ptr) &&
        (PNULL != tag_ptr) &&
        (PNULL != text_ctrl_ptr->cur_tag_ptr))
    {
        //set tag info
        tag_ptr->type    = text_ctrl_ptr->cur_tag_ptr->type;
        tag_ptr->str_ptr = text_ctrl_ptr->str_ptr + text_ctrl_ptr->cur_tag_ptr->start_pos;
        tag_ptr->str_len = text_ctrl_ptr->cur_tag_ptr->len;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set text tag type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_SetTagType(
                               MMI_CTRL_ID_T        ctrl_id,    //in:
                               GUITEXT_TAG_TYPE_E   tag_type,   //in:
                               GUITEXT_SET_TAG_T    *set_tag_ptr//in:may PNULL,GUITEXT_TAG_SET,must set the pointer 
                               )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);

    if ((PNULL != text_ctrl_ptr) &&
        (tag_type != text_ctrl_ptr->theme.tag_type))
    {
        //set tag type
        text_ctrl_ptr->theme.tag_type = tag_type;

        if (0 < text_ctrl_ptr->str_len)
        {
            //parse tag
            ParsetTextTag(text_ctrl_ptr,set_tag_ptr);

            //parse string
            ParseTextString(text_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set edit align,default left and top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_SetAlign(
                             MMI_CTRL_ID_T  ctrl_id,//in
                             GUI_ALIGN_E    align   //in
                             )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);

    if (PNULL != text_ctrl_ptr)
    {
        //set align
        text_ctrl_ptr->theme.align = align;
    }
}

/*****************************************************************************/
//  Description : set auto scroll text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_SetAutoScroll(
                                  BOOLEAN           is_auto_scroll,     //in:
                                  uint32            *scroll_period_ptr, //in:may PNULL default
                                  MMI_CTRL_ID_T     ctrl_id             //in:
                                  )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        //set auto scroll
        text_ctrl_ptr->is_time_scroll = is_auto_scroll;

        //set time
        if (PNULL != scroll_period_ptr)
        {
            text_ctrl_ptr->theme.scroll_period = *scroll_period_ptr;
        }
    }
}

/*****************************************************************************/
//  Description : set circular handle text key direction msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_SetCircularHandle(
                                      BOOLEAN       is_circular_handle, //in:
                                      MMI_CTRL_ID_T ctrl_id             //in:
                                      )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        //set circular_handle msg
        text_ctrl_ptr->theme.is_circular_handle = is_circular_handle;
    }
}

/*****************************************************************************/
//  Description : set text handle tp msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default handle tp msg
/*****************************************************************************/
PUBLIC void GUITEXT_SetHandleTpMsg(
                                   BOOLEAN          is_handle,  //in:
                                   MMI_CTRL_ID_T    ctrl_id     //in:
                                   )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        //set circular_display
        text_ctrl_ptr->theme.is_handle_tp = is_handle;
    }
}

/*****************************************************************************/
//  Description : set text is display progress bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default display
/*****************************************************************************/
PUBLIC void GUITEXT_IsDisplayPrg(
                                 BOOLEAN            is_display, //in:
                                 MMI_CTRL_ID_T      ctrl_id     //in:
                                 )
{
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        //set is display progress
        text_ctrl_ptr->is_display_prg = is_display;
    }
}

/*****************************************************************************/
// 	Description : set text is support slide,default TRUE
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_IsSlide(
                               MMI_HANDLE_T   text_handle,
                               BOOLEAN        is_support
                               )
{
    BOOLEAN             result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(text_handle);
    if (PNULL != text_ctrl_ptr)
    {
        text_ctrl_ptr->theme.is_support_slide = is_support;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set whether supports clipbroad, default is TRUE
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetClipboardEnabled(
                                           MMI_HANDLE_T     text_handle,
                                           BOOLEAN          is_support
                                           )
{
    BOOLEAN             result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(text_handle);
    if (PNULL != text_ctrl_ptr)
    {
#ifdef GUIF_CLIPBOARD
        text_ctrl_ptr->is_clipbrd_support = is_support;
#endif

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set text rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetRect(
                              CTRLBASE_OBJ_T    *ctrl_ptr,
                              const CAF_RECT_T  *rect_ptr
                              )
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_change_width = FALSE;
    BOOLEAN             is_change_height = FALSE;
    BOOLEAN             is_prg_prev = FALSE;
    BOOLEAN             is_prg_now = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    if ((!GUI_IsRectEmpty(*rect_ptr)) && 
        (!GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr)))
    {
        //rect width is change
        if ((rect_ptr->right - rect_ptr->left) != (base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left))
        {
            is_change_width = TRUE;
        }

        //rect height is change
        if ((rect_ptr->bottom - rect_ptr->top) != (base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top))
        {
            is_change_height = TRUE;
        }

        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;
        
        GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));

        //stop timer
        StopTextScrollTimer(text_ctrl_ptr);

        //is progress bar
        if (PNULL != text_ctrl_ptr->prgbox_ctrl_ptr)
        {
            is_prg_prev = TRUE;
        }

        //destroy progress bar control
        if ((is_change_width) ||
            (is_change_height))
        {
            DestroyPrgCtrl(text_ctrl_ptr);
        }
#ifdef TOUCH_PANEL_SUPPORT
        //stop slide timer
        StopTpSlideTimer(text_ctrl_ptr);
#endif
        //reset slide state
        text_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
        
        //init display rect
        InitDisplayRect(text_ctrl_ptr);
        
        //get line number in one page
        if (is_change_height)
        {
            text_ctrl_ptr->page_line_num = GetPageLineNum(text_ctrl_ptr);
        }

        if (0 < text_ctrl_ptr->str_len)
        {
            //is need progress bar
            if ((is_change_width) || 
                (is_change_height))
            {
                JudgeAndCreatePrgCtrl(is_change_width,text_ctrl_ptr);
            }

            if (PNULL != text_ctrl_ptr->prgbox_ctrl_ptr)
            {
                is_prg_now = TRUE;
            }

            //parse string
            if ((is_change_width) ||
                (is_prg_now != is_prg_prev))
            {
                ParseTextString(text_ctrl_ptr);
            }

            //set relative rect
            SetRelativeRect(text_ctrl_ptr);

            //adjust relative display rect by line index
            AdjustDisplayRect(text_ctrl_ptr->top_line_index,text_ctrl_ptr);

            //get top and bottom index
            GetTopBottomIndex(text_ctrl_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set text border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetBorder(
                                CTRLBASE_OBJ_T  *ctrl_ptr,
                                GUI_BORDER_T    *border_ptr
                                )
{
    BOOLEAN             result = TRUE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    //set border
    text_ctrl_ptr->theme.border = *border_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : set text bg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetBg(
                            CTRLBASE_OBJ_T  *ctrl_ptr,
                            GUI_BG_T        *bg_ptr
                            )
{
    BOOLEAN             result = TRUE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    //set bg
    text_ctrl_ptr->theme.bg = *bg_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : set text font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetFont(
                              CTRLBASE_OBJ_T    *ctrl_ptr,
                              GUI_FONT_ALL_T    *font_ptr
                              )
{
    BOOLEAN             result = TRUE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    //set font
    SetFontByPtr(text_ctrl_ptr,&font_ptr->font,&font_ptr->color);

    return (result);
}

/*****************************************************************************/
//  Description : get text font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetFont(
                              CTRLBASE_OBJ_T    *ctrl_ptr,
                              GUI_FONT_ALL_T    *font_ptr
                              )
{
    BOOLEAN             result = TRUE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    //get font
    *font_ptr = text_ctrl_ptr->theme.font;

    return (result);
}

/*****************************************************************************/
//  Description : set text is display progress
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetProgress(
                                  CTRLBASE_OBJ_T    *ctrl_ptr,
                                  BOOLEAN           is_display
                                  )
{
    BOOLEAN             result = TRUE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    //set is display progress
    text_ctrl_ptr->is_display_prg = is_display;

    return (result);
}

/*****************************************************************************/
//  Description : set text is circular handle key direction msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetCircularHandle(
                                        CTRLBASE_OBJ_T  *ctrl_ptr,
                                        BOOLEAN         is_circular
                                        )
{
    BOOLEAN             result = TRUE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //set circular display
    GUITEXT_SetCircularHandle(is_circular,base_ctrl_ptr->handle);

    return (result);
}

/*****************************************************************************/
//  Description : get text height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetHeightByWidth(
                                       CTRLBASE_OBJ_T   *ctrl_ptr,  //in:
                                       uint16           width,      //in:
                                       uint16           *height_ptr //in/out:
                                       )
{
    BOOLEAN         result = TRUE;
    BOOLEAN         is_border = TRUE;
    uint16          line_height = 0;
    uint16          text_width = 0;
    uint16          total_line_num = 0;
    CTRLTEXT_OBJ_T  *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    //text is exist border
    if (GUI_BORDER_NONE == text_ctrl_ptr->theme.border.type)
    {
        is_border = FALSE;
    }

    //get line height
    line_height = GUITEXT_GetLineHeight(text_ctrl_ptr);

	//get text box width
	text_width = (uint16)(width - (text_ctrl_ptr->theme.margin_left_space + text_ctrl_ptr->theme.margin_right_space));
    if (is_border)
    {
        text_width = (uint16)(text_width - (text_ctrl_ptr->theme.border.width << 1));
    }

	//get text string line number
	total_line_num = GUI_CalculateStringLinesByPixelNum(text_width,
						text_ctrl_ptr->str_ptr,
						text_ctrl_ptr->str_len,
						text_ctrl_ptr->theme.font.font,
						(uint8)text_ctrl_ptr->theme.char_space,
						TRUE);

    //is carriage return or line feed
	if ((0 < text_ctrl_ptr->str_len) &&
		(GUI_IsNewlineChar(text_ctrl_ptr->str_ptr[text_ctrl_ptr->str_len - 1])))
	{
		//cursor display the next line
		total_line_num++;
	}

    //must one line
    if (0 == total_line_num)
    {
        total_line_num = 1;
    }

    //get text height
    *height_ptr = (uint16)(line_height * total_line_num);
    if (0 < total_line_num)
    {
        *height_ptr = (uint16)(*height_ptr - text_ctrl_ptr->theme.line_space);
    }

    //add margin
    *height_ptr = (uint16)(*height_ptr + (text_ctrl_ptr->theme.margin_top_space + text_ctrl_ptr->theme.margin_bottom_space));

    //add border
    if (is_border)
    {
        *height_ptr = (uint16)(*height_ptr + (text_ctrl_ptr->theme.border.width << 1));
    }

    return (result);
}

/*****************************************************************************/
//  Description : set text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlSetDisplayRect(
                                     CTRLBASE_OBJ_T     *ctrl_ptr,  //in:
                                     const GUI_RECT_T   *rect_ptr,  //in:
                                     BOOLEAN            is_update   //in:
                                     )
{
    BOOLEAN         result = TRUE;
    CTRLTEXT_OBJ_T  *text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;

    //set display rect
    base_ctrl_ptr->display_rect = *rect_ptr;

    if (is_update)
    {
        //update text
        GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get text highlight end pos line rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetHlEndLineRect(
                                       CTRLBASE_OBJ_T   *ctrl_ptr,  //in:
                                       GUI_RECT_T       *rect_ptr   //in/out:
                                       )
{
    BOOLEAN         result = FALSE;
    uint16          line_index = 0;
    CTRLTEXT_OBJ_T  *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    if ((text_ctrl_ptr->is_active) &&
        (text_ctrl_ptr->highlight.start_pos != text_ctrl_ptr->highlight.end_pos) &&
        (PNULL != rect_ptr))
    {
        line_index = GUITEXT_GetHighlightLineIndex(TRUE,text_ctrl_ptr->highlight.end_pos,text_ctrl_ptr);

        *rect_ptr = GUITEXT_GetLineRect(line_index,text_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get text highlight end pos line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextCtrlGetHlEndLineIndex(
                                        CTRLBASE_OBJ_T  *ctrl_ptr,      //in:
                                        BOOLEAN         *is_first_ptr,  //in/out:
                                        BOOLEAN         *is_last_ptr    //in/out:
                                        )
{
    BOOLEAN         result = FALSE;
    uint16          line_index = 0;
    CTRLTEXT_OBJ_T  *text_ctrl_ptr = PNULL;

    text_ctrl_ptr = (CTRLTEXT_OBJ_T *)ctrl_ptr;

    if ((text_ctrl_ptr->is_active) &&
        (text_ctrl_ptr->highlight.start_pos != text_ctrl_ptr->highlight.end_pos))
    {
        line_index = GUITEXT_GetHighlightLineIndex(TRUE,text_ctrl_ptr->highlight.end_pos,text_ctrl_ptr);

        if (0 == line_index)
        {
            if (PNULL != is_first_ptr)
            {
                *is_first_ptr = TRUE;
            }
        }

        if (line_index == (text_ctrl_ptr->total_line_num - 1))
        {
            if (PNULL != is_last_ptr)
            {
                *is_last_ptr = TRUE;
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetPosInfo(
                                  MMI_CTRL_ID_T      ctrl_id,
                                  uint16             pos,                //in
                                  GUI_RECT_T         *cursor_rect_ptr    //in/out:
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        result = GUITEXT_GetPosRect(text_ctrl_ptr, pos, cursor_rect_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetHighlightInfo(
                                     MMI_CTRL_ID_T      ctrl_id,
                                     uint16             *highlight_min_ptr,
                                     uint16             *highlight_max_ptr
                                     )
{
    BOOLEAN         result = FALSE;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;

    //get text pointer by control id
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if (PNULL != text_ctrl_ptr)
    {
        if (PNULL != highlight_min_ptr)
        {
            *highlight_min_ptr = GUITEXT_GetHighlightMinPos(text_ctrl_ptr);
        }

        if (PNULL != highlight_max_ptr)
        {
            *highlight_max_ptr = GUITEXT_GetHighlightMaxPos(text_ctrl_ptr);
        }
    }

    return result;
}

//zmt add start
/*****************************************************************************/
//  Description : set text string display top index
//  Global resource dependence : 
//  Author: fys
//  Note:2024.06.29
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetResetTopDisplay(MMI_CTRL_ID_T ctrl_id, int32 top_index)
{
    BOOLEAN         result = FALSE;
    int32           display_top = 0;
    CTRLTEXT_OBJ_T      *text_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;
    
    text_ctrl_ptr = GetTextPtr(ctrl_id);
    if(text_ctrl_ptr == PNULL){
        return result;
    }
    
    base_ctrl_ptr = (CTRLBASE_OBJ_T*)text_ctrl_ptr;
    if(base_ctrl_ptr == PNULL){
        return result;
    }

    display_top = top_index;
    if (GUITEXT_SetDisplayRect(display_top,text_ctrl_ptr))
    {
        //get top and bottom index
        GetTopBottomIndex(text_ctrl_ptr);
    }

    if ((GUI_BG_NONE == text_ctrl_ptr->theme.bg.bg_type) && (text_ctrl_ptr->is_time_scroll))
    {
        CTRLMSG_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_TEXT_SCROLL, NULL);
    }
    else
    {
        //update text
        GUITEXT_DisplayText(TRUE,text_ctrl_ptr);
    }

    return (result);
}
//zmt add end