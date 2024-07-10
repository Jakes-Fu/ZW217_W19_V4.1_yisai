/****************************************************************************
** File Name:      mmimms_editwin.c                                         *
** Author:         minghu.mao                                                *
** Date:           05/26/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2009        minghu.mao       create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmipb_export.h"
#include "guirichtext.h"
#include "guimenu.h"
#include "mmifmm_export.h"
#include "mmifmm_id.h"
#include "guibutton.h"
#include "mmisms_id.h"
#include "mmi_default.h"
#include "guiedit.h"

//testing start
//#include "mmipic_image.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "guires.h"
//test end

#include "mmimms_id.h"
#include "sysMemFile.h"
#include "mmimms_internal.h"
#include "mmimms_text.h"
#include "mms_image.h"
#include "mmimms_edit.h"
#include "mmimms_file.h"
#include "mmimms_menutable.h"
#include "guiim_base.h"
#include "guistring.h"
#include "mmisms_export.h"
#include "gui_ucs2b_converter.h "
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef MMI_PDA_SUPPORT 
#include "guiform.h"
#endif
#if defined(MMI_PDA_SUPPORT) || defined(MMIMMS_SMS_IN_1_SUPPORT)
#include "guictrl_api.h"
#endif
#include "mmicl_export.h"
#include "mmisms_image.h"
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmisms_image.h"
#include "mmisms_text.h"
#include "mmitheme_text.h"
#include "img_dec_interface.h"
#include "guitext.h"
#include "mmisms_send.h"
#include "mmisms_edit.h"
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#include "mmicc_export.h"
#endif
#include "guiurl.h"
#endif
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif

#ifdef MMI_MMS_MINI_SUPPORT
#include "mmisms_text.h"
#endif

//#include "mmismsapp_wintab.h"
#include "mmivcard_export.h"
#include "mmisms_internal.h"
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
#include "guidropdownlist.h"
#include "mmicl_export.h"
#endif
#include "mmiset_export.h"
#include "mmidc_export.h"
#include "mmimms_id.h"
#include "mmisms_appcontrol.h"

#include "mmipb_search.h"
#include "mmisms_export.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_RICHTEXT_ITEM_NUM_MAX  20

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
#define MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_NUM 250
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT

#if (defined MAINLCD_SIZE_240X400)||(defined MAINLCD_SIZE_240X320)
#define MMIMMS_EDIT_CONTACT_ICON_WIDTH  19
#else
#define MMIMMS_EDIT_CONTACT_ICON_WIDTH  15
#endif
#endif

#define MMIMMS_PHIZ_NUM    21    
#define MMIMMS_PHIZ_MAX_LEN  7

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
#define MMIMMS_SEMICOLON 0x3B //分号
#define MMIMMS_CR 0x0A        //回车
#endif

#define MMIMMS_EDIT_NUMBER_COUNT_LEN  7

#define MMIMMS_SIZE_MIN_DV  (11*1024) //录制的最小size单位要10K+head
 /**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/
//彩信编辑左软键选项节点类型
typedef enum
{
    MMS_CONTEDIT_OPT_NODE_NULL,
    MMS_CONTEDIT_OPT_NODE_SEND,
    MMS_CONTEDIT_OPT_NODE_ADD_RECV,
    MMS_CONTEDIT_OPT_NODE_PREV,
    MMS_CONTEDIT_OPT_NODE_INSERT,
    MMS_CONTEDIT_OPT_NODE_INSTEXT,    
    MMS_CONTEDIT_OPT_NODE_INSIMAGE,
    MMS_CONTEDIT_OPT_NODE_INSMUSIC,
    MMS_CONTEDIT_OPT_NODE_INSVIDEO,
    MMS_CONTEDIT_OPT_NODE_INDC,
    MMS_CONTEDIT_OPT_NODE_INAU,
    MMS_CONTEDIT_OPT_NODE_INDV,
    MMS_CONTEDIT_OPT_NODE_INSADDFILE,
    MMS_CONTEDIT_OPT_NODE_INSSLIDE,
    MMS_CONTEDIT_OPT_NODE_INSSLIDER,
    MMS_CONTEDIT_OPT_NODE_DELETE,
    MMS_CONTEDIT_OPT_NODE_DELTEXT,
    MMS_CONTEDIT_OPT_NODE_DELIMAGE,
    MMS_CONTEDIT_OPT_NODE_DELMUSIC,
    MMS_CONTEDIT_OPT_NODE_DELVIDEO,
    MMS_CONTEDIT_OPT_NODE_DELADDFILE,
    MMS_CONTEDIT_OPT_NODE_DELSLIDE,    
    MMS_CONTEDIT_OPT_NODE_SETTIME,
    MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT,
    MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST,
    MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST,
    MMS_CONTEDIT_OPT_NODE_TEXT_POS,
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    MMS_CONTEDIT_OPT_NODE_FONT_BIG,
    MMS_CONTEDIT_OPT_NODE_FONT_NOMAL,
    MMS_CONTEDIT_OPT_NODE_FONT_SMALL,
    MMS_CONTEDIT_OPT_NODE_FONT,
#endif
    MMS_CONTEDIT_OPT_NODE_CANCEL,
    MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE,
    MMS_CONTEDIT_OPT_NODE_OPEN_POP,
    MMS_CONTEDIT_OPT_NODE_HALF_POP,
    MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE,
    MMS_CONTEDIT_OPT_NODE_INSERT_PHIZ,
    MMS_CONTEDIT_OPT_NODE_INSERT_TEMPLATE,
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMS_CONTEDIT_OPT_NODE_INSERT_CONTACT_INFO,
    MMS_CONTEDIT_OPT_NODE_INSERT_SUBJECT,
    MMS_CONTEDIT_OPT_NODE_DEL_SUBJECT,
#ifdef MMIMMS_SMS_CC_SUPPORT    
    MMS_CONTEDIT_OPT_NODE_CARBON_COPY,
    MMS_CONTEDIT_OPT_NODE_INSCC,
    MMS_CONTEDIT_OPT_NODE_DELCC,    
    MMS_CONTEDIT_OPT_NODE_INSBCC,
    MMS_CONTEDIT_OPT_NODE_DELBCC,   
#endif 
    MMS_CONTEDIT_OPT_NODE_OPEN_TIMERMSG,    
    MMS_CONTEDIT_OPT_NODE_CLOSE_TIMERMSG,   
    MMS_CONTEDIT_OPT_NODE_ITEM_DELETE,
    MMS_CONTEDIT_OPT_NODE_ITEM_REPLACE,
#endif
    MMS_CONTEDIT_OPT_NODE_MAX
}MMIMMS_CONTEDIT_OPT_MENU_NODE_E;

//编辑彩信时候进行的选项菜单操作的类型（插入音乐，插入图片，插入视频...)
typedef enum
{
    MMIMMS_INSERT_MUSIC,  //插入音乐文件
    MMIMMS_INSERT_PIC,    //插入图片文件
//    MMIMMS_INSERT_VIDEO,  //插入视频文件
    MMIMMS_INSERT_ADD_FILE,
    MMIMMS_OP_MAX
}MMIMMS_EDIT_INS_OP_TYPE;    

typedef enum
{
    MMIMMS_RICHTEXT_ADD_ITEM,  //richedit add new item
    MMIMMS_RICHTEXT_MOD_ITEM  //richedit mod item
//    MMIMMS_RICHTEXT_OP_MAX
}MMIMMS_RICHTEXT_OP_TYPE; 

#ifdef MMIMMS_SMS_IN_1_SUPPORT
typedef enum
{
    MMIMMS_TYPE_PIC,   
    MMIMMS_TYPE_MUSIC, 
    MMIMMS_TYPE_VIDEO,
    MMIMMS_TYPE_ATTACH,
    MMIMMS_TYPE_OP_MAX
}MMIMMS_EDIT_TYPE_OP_TYPE;    

#endif

//彩信编辑全局变量结构
typedef struct 
{
    BOOLEAN is_add_menu_get_focus;                           //richedit add menu get focus
    BOOLEAN is_multi_op_pop;
    BOOLEAN is_replace_multifile;
    BOOLEAN is_save_draft;
    BOOLEAN is_insert_normal_attachment;
    BOOLEAN is_answer_from_same_simcard;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    BOOLEAN is_have_email_addr;
    BOOLEAN is_have_subject;
#ifdef MMIMMS_SMS_CC_SUPPORT
    BOOLEAN is_have_carbon_cc;
    BOOLEAN is_have_carbon_bcc;
#endif
    BOOLEAN is_mms_mode;//是否是彩信    
    BOOLEAN is_send_gray;
    MMIMMS_EDIT_ADD_CONTACT_TYPE mms_add_contact_type;
#endif
    MN_DUAL_SYS_E answer_sim;
    uint16 losefocus_index;                                  //lose focus richeidt item index
    uint16 pre_focus_index;                                  //pre index used for add menu
    uint32 replace_multifile_size;
    MMIMMS_SEND_MMS_STATUS mms_send_status;                  //new mms or outbox mms
    MMIMMS_CONTEDIT_OPT_MENU_NODE_E edit_lose_focus_reason;  //lose focus reason
    MMIMMS_EDIT_INS_OP_TYPE mms_edit_insert_op_type;         //insert multifile type
    MMIMMS_EDIT_REASON_TYPE_E mms_edit_enter_type;           //彩信编辑入口原因
    MMIFMM_FILE_TYPE_E   file_type;
    MMIFILE_FILE_INFO_T   *file_info_ptr;                    //for pic insert    
    MMIMMS_EDIT_DOCUMENT_T * editdoc;        
}MMIMMS_EDIT_GLOBLE_VARIABLE_T;

extern const char * const s_sms_phiz[MMIMMS_PHIZ_NUM];

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef MMIMMS_SMS_IN_1_SUPPORT
LOCAL MMIMMS_RICHTEXT_ITEM_INDEX_E g_mms_edit_richtext_item_type[MMIMMS_RICHTEXT_ITEM_NUM_MAX] = {
    MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,
    MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,
    MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,
    MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX,MMIMMS_RICHTEXT_MAX};
#endif

MMIMMS_EDIT_GLOBLE_VARIABLE_T g_mms_edit_globle = {0};
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
LOCAL MMIPB_HANDLE_T s_pb_num_search_handle = 0;
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifndef MMI_PDA_SUPPORT
LOCAL GUI_BG_TYPE_E s_edit_to_bg_type = GUI_BG_NONE;
#endif
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :mms edit menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditMainMenuWin(
                                              MMIMMS_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :mms edit main richedit win add multimedia file handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditAddMenuWinMsg(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              );
#endif

/*****************************************************************************/
//  Description :mms edit main richedit win edit multimedia file pop menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditMultiPopWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
// 	Description : HandleMMSEditMainMenuOptDelWinMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSEditMainMenuOptDelWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            );

/*****************************************************************************/
//  Description :mms edit main richedit win opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditMainMenuOptWinMsg(
                                                  MMIMMS_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                              );

/*****************************************************************************/
//  Description :mms edit main richedit add to menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditToListWinMsg(
                                                   MMIMMS_WINDOW_ID_E win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                              );

/*****************************************************************************/
//  Description :mms edit main richedit edit subject menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditSubjectWinMsg(
                                              MMIMMS_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
//  Description :mms edit each slide dur time menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditSetSlideTimeWin(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                              );

/*****************************************************************************/
//  Description :mms edit text edit menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditTextWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Description :mms send select sim card
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMSelectSendWinMsg(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);


/*****************************************************************************/
//  Description :get to num for string 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetRecieverNumberFromStr(MMI_STRING_T * to_str);
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add richtext  to list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddToItem(MMIMMS_RICHTEXT_OP_TYPE is_add);

/*****************************************************************************/
//  Description :add richtext  additional file list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddAddFileItem(MMIMMS_RICHTEXT_OP_TYPE is_add, char * file_name_ptr);

/*****************************************************************************/
//  Description :add richtext subject item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddSubjectItem(MMIMMS_RICHTEXT_OP_TYPE is_add);

/*****************************************************************************/
//  Description :add richtext text item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddTextItem(MMIMMS_RICHTEXT_OP_TYPE is_add);
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add form pic item
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddPicItem(void);
#else
/*****************************************************************************/
//  Description :add richtext pic item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddPicItem(MMIMMS_RICHTEXT_OP_TYPE is_add);
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add form music item
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddMusicItem(void);
#else
/*****************************************************************************/
//  Description :add richtext music item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddMusicItem(MMIMMS_RICHTEXT_OP_TYPE is_add);
#endif
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add richtext add multi file menu item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddMenuItem(MMIMMS_RICHTEXT_OP_TYPE is_add);

/*****************************************************************************/
//  Description :add richtext items to edit win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMSEditEntry(MMIMMS_RICHTEXT_OP_TYPE is_add);
#endif

/*****************************************************************************/
//  Description :open mms edit option menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSEditOptPopMenu(void);
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :open mms edit add menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSEditAddPopMenu(void);
#endif
/*****************************************************************************/
//  Description :edit and send new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void EditAndSendNewMMS(void);

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertPic(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace);

/*****************************************************************************/
//  Description :mms edit insert Music
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertMusic(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace);

/*****************************************************************************/
//  Description :mms edit insert normal attachment
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertNormalAttachment(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace);

/*****************************************************************************/
//  Description :mms edit insert Video
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertVideo(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace);
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :mms edit enter corresponding richedit  items
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  MMSEditEnterCorrespondItem(MMI_CTRL_ID_T ctrl_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//	Description : handle 
//	Global resource dependence : 
//	Author:jixin.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MmsHandleCallogSelectReturn(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);
#endif
#endif
/*****************************************************************************/
//  Description : set  edit mms music or video name
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetEditMMSMusicOrVideoName(wchar *name_ptr, uint16 name_len);

/*****************************************************************************/
//  Description :Get flag if add menu get focus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditFocusOnAddMenu( void);

/*****************************************************************************/
//  Description :Set flag if add menu get focus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditFocusOnAddMenu( BOOLEAN add_menu_get_focus);

/*****************************************************************************/
//  Description :Get flag if op multifile  popmenu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsOpPopMenu( void);

/*****************************************************************************/
//  Description :Set flag if if op multifile  popmenu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsOpPopMenu( BOOLEAN op_pop);

/*****************************************************************************/
//  Description :Get mms edit losefucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMMSEditLoseFocusIndex( void);

/*****************************************************************************/
//  Description :Set mms edit losefucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditLoseFocusIndex( uint16 lose_focus_index);

/*****************************************************************************/
//  Description :Get mms edit pre fucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMMSEditPreFocusIndex( void);

/*****************************************************************************/
//  Description :Set mms edit pre fucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditPreFocusIndex( uint16 pre_focus_index);

/*****************************************************************************/
//  Description :Get mms edit lose fucus reason
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_CONTEDIT_OPT_MENU_NODE_E GetMMSEditLoseFocusReason( void);

/*****************************************************************************/
//  Description :Set mms edit lose fucus reason
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditLoseFocusReason( MMIMMS_CONTEDIT_OPT_MENU_NODE_E lose_fucus_reason);

/*****************************************************************************/
//  Description :Get mms edit insert op type
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_EDIT_INS_OP_TYPE GetMMSEditInsertOpType( void);

/*****************************************************************************/
//  Description :Set mms edit lose fucus reason
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditInsertOpType( MMIMMS_EDIT_INS_OP_TYPE insert_op_type);

/*****************************************************************************/
//  Description :list box add string
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void ToListBoxSetString(MMI_CTRL_ID_T ctrl_id, const wchar *str_ptr, const uint16 str_len);

/*****************************************************************************/
//  Description :list box add array
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void ToListBoxSetArray(MMI_CTRL_ID_T ctrl_id, MMIMMS_EDIT_NUMBER_LIST_T *numberlist_header_ptr);

/*****************************************************************************/
//  Description :Get mms edit if replace multifile
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsReplaceMultifile( void);

/*****************************************************************************/
//  Description :Set mms edit if replace multifile
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsReplaceMultifile( BOOLEAN is_replace );

/*****************************************************************************/
//  Description :Get mms edit if replace multifile size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetMMSEditReplaceMultifileSize( void);

/*****************************************************************************/
//  Description :Set mms edit if replace multifile size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditReplaceMultifileSize( uint32 size );

/*****************************************************************************/
//  Description :insert dynamenu node by label
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,       //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,           //位置
                             BOOLEAN            is_graged
                             );


#ifndef MMI_GUI_STYLE_TYPICAL
LOCAL void InsertNodeByLabelEx(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             MMI_IMAGE_ID_T          menu_icon_id,
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,          //位置
                             BOOLEAN            is_graged
                             );
#endif                             
/*****************************************************************************/
//  Description :Get mms edit if save draft
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsSaveDraft( void);

/*****************************************************************************/
//  Description :Set mms edit if save draft
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsSaveDraft( BOOLEAN is_save );
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :Get item index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMMSEditItemIndex( MMIMMS_RICHTEXT_ITEM_INDEX_E item_type);

/*****************************************************************************/
//  Description :Set item index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditItemIndex( MMIMMS_RICHTEXT_ITEM_INDEX_E item_type, uint16 index);
#endif
/*****************************************************************************/
//  Description :mms edit attachment type win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAttachmenTypeWinMsg(
                                              MMIMMS_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
//  Description :Get mms edit if insert normal attachment
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsInsertNormalAttachment( void);

/*****************************************************************************/
//  Description :Set mms edit if insert normal attachment
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsInsertNormalAttachment( BOOLEAN is_insert );

/*****************************************************************************/
//  Description :Get answer sim card
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMIMMS_GetAnswerSim( void);
/*****************************************************************************/
//  Description :get edit name num
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/                                              
LOCAL uint16    MMIMMS_GetEditNameNum(void);      

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :edit win insert opt
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSInsertOptWinMsg(
                                                  MMIMMS_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                              );

 
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :edit win button form init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditWinButtonFormInit(MMIMMS_WINDOW_ID_E win_id);    
#endif

/*****************************************************************************/
//  Description :left right key init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditWinLFRHButtonInit(void);

#endif

/*****************************************************************************/
//  Description :摄像 后 的 处理  
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL int32 MMSDVWorkCallBack(MMIDC_WORK_ERROR_E error_ret,MMIDC_WORK_DATA_INFO_T *data_info_ptr);

/*****************************************************************************/
//  Description :摄像  
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenDV(void);

/*****************************************************************************/
//  Description :现拍图片 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenDC(void);

/*****************************************************************************/
//  Description :拍照 后 的 处理  
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL int32 MMSDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret,MMIDC_WORK_DATA_INFO_T *data_info_ptr);

/*****************************************************************************/
//     Description : to handle the message of window which choice the phiz     
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSPhizListWinMsg(
                                        MMI_WIN_ID_T        win_id,        //IN:
                                        MMI_MESSAGE_ID_E    msg_id,        //IN:
                                        DPARAM                param        //IN:
                                        );

/*****************************************************************************/
//     Description : load total phiz to list
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSLoadTotalPhiz2List(
                              MMI_CTRL_ID_T        ctrl_id        //IN:
                              );

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
// 	Description : 将pb中选中的收件人添加到彩信收件人编辑框
//	Global resource dependence : 
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
LOCAL void MMSAddPbSelectedContacts(uint16 pb_select_num, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, uint32 param, MMI_HANDLE_T  win_id);

/*****************************************************************************/
//  Description : to handle convert sms to mms waiting window    
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConvertSmsToMmsWaitWinMsg(
                                                   MMI_WIN_ID_T        win_id,    //IN:
                                                   MMI_MESSAGE_ID_E    msg_id,    //IN:
                                                   DPARAM                param    //IN:
                                                   );
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :mms edit  main menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSendButtonDisplay(BOOLEAN is_mms_mode);
/*****************************************************************************/
//	Description : MmsHandleCallogSelectReturn 
//	Global resource dependence : 
//	Author:jixin.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MmsHandleCallogSelectReturn(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, 
                                          MMI_HANDLE_T  win_handle,
                                          MMI_MESSAGE_ID_E  msg_id);
#else

/*****************************************************************************/
//  Description :mms set edit disp bg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetEditDispBG(MMIMMS_WINDOW_ID_E win_id,GUI_BG_TYPE_E bg_type);

/*****************************************************************************/
//  Description :mms set form disp bg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetFormDispBG(void);
#endif

/*****************************************************************************/
//  Description :设置彩信添加联系人类型
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditAddContactType( MMIMMS_EDIT_ADD_CONTACT_TYPE add_contactinfo_type);

/*****************************************************************************/
//  Description :判断彩信添加联系人类型
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_EDIT_ADD_CONTACT_TYPE GetMMSEditAddContactType( void);

/*****************************************************************************/
//  Description :设置是否有主题
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsSubjectExist( BOOLEAN is_have_subject);

/*****************************************************************************/
//  Description :判断是否有主题
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsSubjectExist( void);

#ifdef MMIMMS_SMS_CC_SUPPORT    
/*****************************************************************************/
//  Description :设置是否有抄送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsCCExist( BOOLEAN is_have_cc);

/*****************************************************************************/
//  Description :判断是否有抄送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsCCExist( void);

/*****************************************************************************/
//  Description :设置是否有密送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsBCCExist( BOOLEAN is_have_bcc);

/*****************************************************************************/
//  Description :判断是否有密送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsBCCExist( void);
#endif
/*****************************************************************************/
//  Description :将选中的联系人信息插入到彩信正文中
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSAddContact2Content(uint32 handle);

/*****************************************************************************/
//  Description :show to contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowToContactInfo(void);
#ifdef MMIMMS_SMS_CC_SUPPORT    
/*****************************************************************************/
//  Description :show cc contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowCCContactInfo(void);

/*****************************************************************************/
//  Description :show bcc contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowBCCContactInfo(void);
#endif
/*****************************************************************************/
//  Description :show contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowContactInfo(void);

/*****************************************************************************/
//  Description :set mms contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetMMSContactInfo(MMIMMS_EDIT_ADD_CONTACT_TYPE add_contact_type);

/*****************************************************************************/
//  Description : to handle the message of copy waiting window    
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSSmsConvertWaitWinMsg(
                                               MMI_WIN_ID_T        win_id,    //IN:
                                               MMI_MESSAGE_ID_E    msg_id,    //IN:
                                               DPARAM                param    //IN:
                                               );
                                               
/*****************************************************************************/
//  Description : to handle the message of copy waiting window    
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_CheckIsMMS(void);              
          
#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
// 	Description : set Timer SMS display
//	Global resource dependence : 
//  Author: rong.gu
//	Note:   
/*****************************************************************************/
LOCAL void MMSSetTimerMsgDisplay(
								 BOOLEAN	is_update
								 );

/*****************************************************************************/
//     Description : load timer info to editbox
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSAddTimer2Label(void);
/*****************************************************************************/
// 	Description : set Timer SMS label param
//	Global resource dependence : 
//  Author:  rong.gu
//	Note:   
/*****************************************************************************/
LOCAL void MMSSetTimerMsgStartParam(void);
#endif


/*****************************1************************************************/
//     Description : get phone number is valid
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************1************************************************/
LOCAL BOOLEAN MMSCheckDestNum(wchar *wstr_ptr, uint16 wstr_len);

/*****************************1************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************1************************************************/
LOCAL BOOLEAN MMSGetSendNumber(                            //RETURN: 
                                   MMI_CTRL_ID_T ctrl_id,     //IN:
                                   BOOLEAN  is_delete_invalid //IN:
                                   );
/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandleMMSEditSendSms(
                       MMIMMS_WINDOW_ID_E win_id //IN
                       );

/*****************************************************************************/
//     Description : to display send err win
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSDisplaySmsSendErr(
                          MN_DUAL_SYS_E         dual_sys, //IN:
                          MMISMS_OPER_ERR_E    oper_err    //IN:
                          );
/*****************************************************************************/
//  Description :MMSEditAssembleInfo
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAssembleInfo(void);

/*****************************************************************************/
//  Description :save sms draf
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSaveSmsDraft(MMI_HANDLE_T win_id);  

/*****************************************************************************/
//  Description :get contact count
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL uint16 MMIMMS_GetAllContactCount(void);  //判断联系人数量

/*****************************************************************************/
//     Description : to handle the attach list
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSAttachListWinMsg(
                                        MMI_WIN_ID_T        win_id,        //IN:
                                        MMI_MESSAGE_ID_E    msg_id,        //IN:
                                        DPARAM                param        //IN:
                                        );
/*****************************************************************************/
//     Description : load total attach to list
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSLoadTotalAttach2List(
                       MMI_CTRL_ID_T        ctrl_id //IN
                       );           
/*****************************************************************************/
//  Description :create attach list win 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateAttachListWin(void);

/*****************************************************************************/
//  Description :create attach list win 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void   MMSEditDelAttach(void);

/*****************************************************************************/
//     Description :add sms number to edit
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSAddSmsNumber(
                       MMI_CTRL_ID_T    ctrl_id    //IN
                       );

/*****************************************************************************/
//     Description : load number edit box
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSSubjectInit(void);

/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSGetNumberFromPb(                            //RETURN: 
                              uint32 handle,
                              MMI_MESSAGE_ID_E    msg_id    //IN:
                              );
                              
/*****************************************************************************/
//     Description : 彩信草稿变为短信保存或发送，需要删除原来的彩信草稿
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSDeleteDraftMMS(void);

/*****************************************************************************/
//     Description : 彩信草稿变为短信保存或发送，需要删除原来的彩信草稿
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSDeleteDraftSMS(void);

/*****************************************************************************/
//  Description :open mms multi edit option menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenEditMultiOptPopMenu(
                       MMIMMS_EDIT_TYPE_OP_TYPE file_type//IN
                       );

/*****************************************************************************/
//  Description :mms show mms info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowMMSInfo(void);

/*****************************************************************************/
//  Description :设置附件中图片显示的高度
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetMMSPicHeight(
                       uint8 *image_buf_ptr, //IN
                       uint32 image_buf_len //IN
                       );

/*****************************************************************************/
//  Description :设置界面中edit默认的文字
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditSetEditDefaultString(MMIMMS_WINDOW_ID_E win_id);

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
/*****************************************************************************/
//  Description :append matching num list for DDL
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void MMSAppendDDLMatchingNumList(uint16 index, uint16 cl_match_item_num,
                        MMIPB_HANDLE_T pb_num_search_handle);

/*****************************************************************************/
//  Description :update matching num DDL
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void MMSUpdateDDLMatchingNumList(MMIMMS_WINDOW_ID_E win_id);
#endif

/*****************************************************************************/
//  Description :home键保存草稿
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSRedkeySaveSmsDraft(void);  

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 显示编辑联系人的个数
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSShowEditToCount(uint16 item_num);

/*****************************************************************************/
// 	Description : 显示编辑短信字数的个数
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSShowEditWordCount(void);

/*****************************************************************************/
// 	Description : 设置发送按钮区域的属性
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSSetButtonAreaProperty(void);

/*****************************************************************************/
// 	Description : 设置添加联系人区域的属性
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSSetEditToAreaProperty(void);

/*****************************************************************************/
// 	Description : 设置信息内容区域的属性
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSSetContentAreaProperty(void);
#endif

#endif

#if defined(MMS_SMS_IN_1_SUPPORT ) || defined(MMI_RECORD_SUPPORT)
/*****************************************************************************/
//  Description :add new slide
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSAddNewSlide(void);

/*****************************************************************************/
//  Description :show file info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowMultiFileInfo(void);
#endif

/*****************************************************************************/
#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************///  Description :录音 后 的 处 理  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr);

/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenRecord(void);
#endif

/*****************************************************************************/
//  Description :mms edit insert DC Camera
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertDCPic(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace);
/*****************************************************************************/
//  Description :mms edit insert DV
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertDV(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace);

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :save sms
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMSSaveSms(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :send sms
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMSSendSms(MN_DUAL_SYS_E dual_sys);
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


// the phiz list window
WINDOW_TABLE( MMIMMS_PHIZLIST_WIN_TAB ) = 
{
    WIN_TITLE( TXT_SMS_PHIZ ),
    WIN_FUNC((uint32) HandleMMSPhizListWinMsg),    
    WIN_ID( MMIMMS_ADD_PHIZ_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),      
#else    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),      
#endif
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMMS_PHIZ_LISTBOX_CTRL_ID ),    
    END_WIN
};
#ifdef MMIMMS_SMS_IN_1_SUPPORT
// the phiz list window
WINDOW_TABLE( MMIMMS_ATTACH_LIST_TAB ) = 
{
    WIN_TITLE( TXT_MMS_ATTACHMENT ),
    WIN_FUNC((uint32) HandleMMSAttachListWinMsg),    
    WIN_ID( MMIMMS_ATTACH_WIN_ID ),
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIMMS_ATTACH_LISTBOX_CTRL_ID ),    
    END_WIN
};

#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMIMMS_EDIT_MENU_TAB_EX) =    //has title ,for iphone style
{
    WIN_FUNC((uint32)HandleMMSEditMainMenuWin),    
    WIN_ID(MMIMMS_EDIT_MENU_WIN_ID),
    WIN_TITLE(TXT_NEW),
   
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_SMS_EDIT_FORM_ID),
    
        //Dock top 收件人
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),
#else
            CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_TO_LIST_ID,MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),
#endif
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_TO_BUTTON_ID,MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),

        //Dock right Button
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID,MMIMMS_SMS_EDIT_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_BUTTON_INSERT_CTRL_ID,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_BUTTON_SEND_CTRL_ID,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID),

        //content
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,MMIMMS_SMS_EDIT_FORM_ID) ,
            //timer msg
#ifdef MMI_TIMERMSG_SUPPORT
            CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIMMS_SMS_TIMER_SMS_FORM_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIMMS_SMS_EDIT_TIMER_CTRL_ID,MMIMMS_SMS_TIMER_SMS_FORM_ID),
                CHILD_TEXT_CTRL(FALSE,MMIMMS_SMS_EDIT_TIMER_FREQDATE_CTRL_ID,MMIMMS_SMS_TIMER_SMS_FORM_ID),
#endif

            //subject
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIMMS_MAX_SUBJECT_LEN,MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),

            //input content
            CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMIMMS_SMS_EDIT_TEXT_CTRL_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
    
            //element
            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
                //anim
                CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_ANIM_CTRL_ID,MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),

                //music
                CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_MUSIC_FORM_ID, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),    
                    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_MUSIC_ANIM_ID,MMIMMS_SMS_EDIT_MUSIC_FORM_ID),
                    CHILD_TEXT_CTRL(FALSE,MMIMMS_SMS_EDIT_MUSIC_TEXT_ID,MMIMMS_SMS_EDIT_MUSIC_FORM_ID),
    
                //attachment
                CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),    
                    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_ATTACHMENT_ANIM_ID,MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID),
                    CHILD_TEXT_CTRL(FALSE,MMIMMS_SMS_EDIT_ATTACHMENT_TEXT_ID,MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID),
    
            //title page
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_TITLE_FORM_ID, MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
                CHILD_BUTTON_CTRL(FALSE,IMAGE_MMS_COUNT_LBTN_UN ,MMIMMS_EDIT_BUTTON_LF_CTRL_ID, MMIMMS_SMS_EDIT_TITLE_FORM_ID),
                CHILD_TEXT_CTRL(TRUE,MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID,MMIMMS_SMS_EDIT_TITLE_FORM_ID),
                CHILD_BUTTON_CTRL(FALSE,IMAGE_MMS_COUNT_RBTN_UN ,MMIMMS_EDIT_BUTTON_RH_CTRL_ID, MMIMMS_SMS_EDIT_TITLE_FORM_ID),    

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
            //add for auot signature
            CHILD_EDIT_TEXT_CTRL(FALSE, MMISMS_MAX_SIGNATURE_NUMBER_LEN, MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
#endif

#ifdef MMIMMS_SMS_CC_SUPPORT
        //cc
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
            CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_CC_TO_LIST_ID,MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID,MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID),

        //bcc
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
            CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_BCC_TO_LIST_ID,MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID,MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID),
#endif
    
    END_WIN
};

WINDOW_TABLE(MMIMMS_EDIT_MENU_TAB) =    //not have title
{
    WIN_FUNC((uint32)HandleMMSEditMainMenuWin),    
    WIN_ID(MMIMMS_EDIT_MENU_WIN_ID),
  
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_SMS_EDIT_FORM_ID),
    
        //Dock top 收件人
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),
#else
            CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_TO_LIST_ID,MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),
#endif
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIMMS_SMS_EDIT_TO_LABEL_ID,MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID),
                CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_TO_BUTTON_ID,MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID),

        //Dock right Button
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID,MMIMMS_SMS_EDIT_FORM_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIMMS_SMS_LABEL_COUNT_CTRL_ID,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_BUTTON_INSERT_CTRL_ID,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_BUTTON_SEND_CTRL_ID,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID),

        //content
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,MMIMMS_SMS_EDIT_FORM_ID),
            //timer msg
#ifdef MMI_TIMERMSG_SUPPORT
            CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIMMS_SMS_TIMER_SMS_FORM_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
                  CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIMMS_SMS_EDIT_TIMER_CTRL_ID,MMIMMS_SMS_TIMER_SMS_FORM_ID),
                  CHILD_TEXT_CTRL(FALSE,MMIMMS_SMS_EDIT_TIMER_FREQDATE_CTRL_ID,MMIMMS_SMS_TIMER_SMS_FORM_ID),
#endif

            //subject
            CHILD_EDIT_TEXT_CTRL(TRUE,MMIMMS_MAX_SUBJECT_LEN,MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),

            //input content
            CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMIMMS_SMS_EDIT_TEXT_CTRL_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),

            //element
            CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
                //anim
                CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_ANIM_CTRL_ID,MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),

                //music
                CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_MUSIC_FORM_ID, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),    
                    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_MUSIC_ANIM_ID,MMIMMS_SMS_EDIT_MUSIC_FORM_ID),
                    CHILD_TEXT_CTRL(TRUE,MMIMMS_SMS_EDIT_MUSIC_TEXT_ID,MMIMMS_SMS_EDIT_MUSIC_FORM_ID),
    
                //attachment
                CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),    
                    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_ATTACHMENT_ANIM_ID,MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID),
                    CHILD_TEXT_CTRL(FALSE,MMIMMS_SMS_EDIT_ATTACHMENT_TEXT_ID,MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID),
    
            //title page
            CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_TITLE_FORM_ID, MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
                CHILD_BUTTON_CTRL(FALSE,IMAGE_MMS_COUNT_LBTN_UN ,MMIMMS_EDIT_BUTTON_LF_CTRL_ID, MMIMMS_SMS_EDIT_TITLE_FORM_ID),
                CHILD_TEXT_CTRL(TRUE,MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID,MMIMMS_SMS_EDIT_TITLE_FORM_ID),
                CHILD_BUTTON_CTRL(FALSE,IMAGE_MMS_COUNT_RBTN_UN ,MMIMMS_EDIT_BUTTON_RH_CTRL_ID, MMIMMS_SMS_EDIT_TITLE_FORM_ID),    

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
            //signature
            CHILD_EDIT_TEXT_CTRL(FALSE,MMISMS_MAX_SIGNATURE_NUMBER_LEN,MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID),
#endif

#ifdef MMIMMS_SMS_CC_SUPPORT
        //cc
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
            CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_CC_TO_LIST_ID,MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID,MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID),

        //bcc
        CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
            CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_BCC_TO_LIST_ID,MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID,MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID),
#endif

    END_WIN
};
#else
WINDOW_TABLE(MMIMMS_EDIT_MENU_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSEditMainMenuWin),    
    WIN_ID(MMIMMS_EDIT_MENU_WIN_ID),
    WIN_TITLE(TXT_NEW),
    
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_SMS_EDIT_FORM_ID),    

#ifdef MMI_TIMERMSG_SUPPORT
    CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIMMS_SMS_TIMER_SMS_FORM_ID,MMIMMS_SMS_EDIT_FORM_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIMMS_SMS_EDIT_TIMER_CTRL_ID,MMIMMS_SMS_TIMER_SMS_FORM_ID),
    CHILD_TEXT_CTRL(FALSE,MMIMMS_SMS_EDIT_TIMER_FREQDATE_CTRL_ID,MMIMMS_SMS_TIMER_SMS_FORM_ID),
#endif

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            CHILD_DROPDOWNLIST_CTRL(TRUE, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),
#else
            CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_TO_LIST_ID,MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),//MMIMMS_SMS_EDIT_TO_LIST_FORM_ID),
#endif    


#ifdef MMIMMS_SMS_CC_SUPPORT
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
    CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_CC_TO_LIST_ID,MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID,MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
    CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_SMS_EDIT_BCC_TO_LIST_ID,MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_NULL,MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID,MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID),
#endif

    CHILD_EDIT_TEXT_CTRL(TRUE,MMIMMS_MAX_SUBJECT_LEN,MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID,MMIMMS_SMS_EDIT_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMIMMS_SMS_EDIT_TEXT_CTRL_ID,MMIMMS_SMS_EDIT_FORM_ID),
    
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
   
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_ANIM_CTRL_ID,MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_MUSIC_FORM_ID, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),    
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_MUSIC_ANIM_ID,MMIMMS_SMS_EDIT_MUSIC_FORM_ID),
    CHILD_TEXT_CTRL(FALSE,MMIMMS_SMS_EDIT_MUSIC_TEXT_ID,MMIMMS_SMS_EDIT_MUSIC_FORM_ID),
    
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID),    
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIMMS_SMS_EDIT_ATTACHMENT_ANIM_ID,MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID),
    CHILD_TEXT_CTRL(FALSE,/*256* 10,*/MMIMMS_SMS_EDIT_ATTACHMENT_TEXT_ID,MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID),//(FALSE,/*256* 10,*/MMIMMS_SMS_EDIT_ATTACHMENT_TEXT_ID,MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID),
    
#ifndef MMI_GUI_STYLE_TYPICAL    
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIMMS_SMS_EDIT_TITLE_FORM_ID, MMIMMS_SMS_EDIT_FORM_ID),
    //CHILD_BUTTON_CTRL(FALSE,IMAGE_SMS_BUTTON_NORMAL , MMIMMS_SMS_EDIT_BACK_BUTTON_ID, MMIMMS_SMS_EDIT_TITLE_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_MMS_COUNT_LBTN_UN ,MMIMMS_EDIT_BUTTON_LF_CTRL_ID, MMIMMS_SMS_EDIT_TITLE_FORM_ID),
    CHILD_TEXT_CTRL(TRUE,MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID,MMIMMS_SMS_EDIT_TITLE_FORM_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_MMS_COUNT_RBTN_UN ,MMIMMS_EDIT_BUTTON_RH_CTRL_ID, MMIMMS_SMS_EDIT_TITLE_FORM_ID),    
#endif

    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    //add for auot signature
    CHILD_EDIT_TEXT_CTRL(FALSE, 
                        MMISMS_MAX_SIGNATURE_NUMBER_LEN, 
                        MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, 
                        MMIMMS_SMS_EDIT_FORM_ID),
#endif
    END_WIN
};
#endif

#else
WINDOW_TABLE(MMIMMS_EDIT_MENU_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSEditMainMenuWin),    
    WIN_ID(MMIMMS_EDIT_MENU_WIN_ID),
#ifdef MMI_MMS_MINI_SUPPORT
    WIN_TITLE(TXT_SMS_NEW_MMS),
#else
    WIN_TITLE(TXT_MMS),
#endif
    #ifdef MMI_PDA_SUPPORT 
    WIN_STATUSBAR,
    #endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#ifdef MMI_PDA_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_NULL,MMIMMS_EDIT_BUTTON_LF_CTRL_ID),   
    CREATE_BUTTON_CTRL(IMAGE_NULL,MMIMMS_EDIT_BUTTON_RH_CTRL_ID),
#else
#ifdef MMI_MMS_MINI_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_LBTN_UN,MMIMMS_EDIT_BUTTON_LF_CTRL_ID),    
    CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_RBTN_UN,MMIMMS_EDIT_BUTTON_RH_CTRL_ID),
#else
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_BUTTON_CTRL(IMAGE_MMS_COUNT_LBTN_UN,MMIMMS_EDIT_BUTTON_LF_CTRL_ID),    
    CREATE_BUTTON_CTRL(IMAGE_MMS_COUNT_RBTN_UN,MMIMMS_EDIT_BUTTON_RH_CTRL_ID),
#endif
#endif
#endif

    CREATE_RICHTEXT_CTRL(MMIMMS_EDIT_MENU_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMIMMS_EDIT_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL, MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID, MMIMMS_EDIT_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL,MMIMMS_EDIT_BUTTON_FORM_INSERT_ID, MMIMMS_EDIT_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL,MMIMMS_EDIT_BUTTON_FORM_SEND_ID, MMIMMS_EDIT_BUTTON_FORM_CTRL_ID),
#endif
    END_WIN
};
#endif


WINDOW_TABLE(MMIMMS_EDIT_MAINMENU_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSEditMainMenuOptWinMsg),
    WIN_ID(MMIMMS_EDIT_MAINMENU_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE(MMIMMS_EDIT_MAINMENU_INSERT_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSInsertOptWinMsg),
    WIN_ID(MMIMMS_EDIT_MAINMENU_INSERT_OPT_WIN_ID),
    //WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MMS_MAINMENU_INSERT_POP_MENU,MMIMMS_EDIT_INSERT_OPT_POP_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

#ifndef MMIMMS_SMS_IN_1_SUPPORT 
WINDOW_TABLE(MMIMMS_EDIT_ADDMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSEditAddMenuWinMsg),
    WIN_ID(MMIMMS_EDIT_ADDMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN 
};
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT 
WINDOW_TABLE(MMIMMS_EDIT_MULTI_POP_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSEditMultiPopWinMsg),    
    WIN_ID(MMIMMS_EDIT_MULTI_POP_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#else
WINDOW_TABLE(MMIMMS_EDIT_MULTI_POP_WIN_TAB) = 
{
#ifndef MMI_PDA_SUPPORT
    WIN_TITLE(STXT_OPTION),
#endif
    WIN_FUNC((uint32)HandleMMSEditMultiPopWinMsg),    
    WIN_ID(MMIMMS_EDIT_MULTI_POP_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_MULTI_POP_MENU, MMIMMS_EDIT_MULTI_POP_CTRL_ID),
    END_WIN
};
#endif

//to edit box
WINDOW_TABLE(MMIMMS_EDIT_TO_LIST_TAB) = 
{
    WIN_TITLE(TXT_COMMON_RECIPIENT),
    WIN_FUNC((uint32)HandleMMSEditToListWinMsg),    
    WIN_ID(MMIMMS_EDIT_TO_LIST_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_EDIT_TO_LIST_FORM_ID),    
#endif
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN),
#endif

#ifdef  MMI_PDA_SUPPORT
    CHILD_EDIT_LIST_CTRL(TRUE,MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_EDIT_TO_LIST_CTRL_ID,MMIMMS_EDIT_TO_LIST_FORM_ID),
#else
    CREATE_EDIT_LIST_CTRL(MMIMMS_MAX_RECEIVER_NUM,MMIMMS_MAX_ADDRESS_LEN,MMIMMS_EDIT_TO_LIST_CTRL_ID),
#endif
    
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIMMS_EDIT_TO_LIST_FORM_ID),
#endif

    END_WIN
};

//subject edit box
WINDOW_TABLE(MMIMMS_EDIT_SUBJECT_TAB) = 
{
    WIN_TITLE(TXT_MMS_EDIT_SUBJECT),
    WIN_FUNC((uint32)HandleMMSEditSubjectWinMsg),  
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_EDIT_SUBJECT_FORM_ID),    
#endif
    WIN_ID(MMIMMS_EDIT_SUBJECT_WIN_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif

#ifdef MMI_PDA_SUPPORT
    CHILD_EDIT_TEXT_CTRL(TRUE,0,MMIMMS_EDIT_SUBJECT_CTRL_ID,MMIMMS_EDIT_SUBJECT_FORM_ID),   
#else
    CREATE_EDIT_TEXT_CTRL(0 ,MMIMMS_EDIT_SUBJECT_CTRL_ID),
#endif
    
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIMMS_EDIT_SUBJECT_FORM_ID),
#endif
    
    END_WIN
};

//text edit box
WINDOW_TABLE(MMIMMS_EDIT_TEXT_TAB) = 
{
    WIN_TITLE(TXT_TEXT),
    WIN_FUNC((uint32)HandleMMSEditTextWinMsg),    
    WIN_ID(MMIMMS_EDIT_TEXT_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIMMS_EDIT_TEXT_FORM_ID),  
#endif
    
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif

#ifdef MMI_PDA_SUPPORT
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIMMS_MAX_TEXT_LEN/sizeof(wchar) ,MMIMMS_EDIT_TEXT_CTRL_ID,MMIMMS_EDIT_TEXT_FORM_ID),   
#else
    CREATE_EDIT_TEXT_CTRL(MMIMMS_MAX_TEXT_LEN/sizeof(wchar) ,MMIMMS_EDIT_TEXT_CTRL_ID),
#endif
    
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIMMS_EDIT_TEXT_FORM_ID),
#endif
    END_WIN
};


//slide duration time 
WINDOW_TABLE(MMIMMS_EDIT_SET_SLIDE_TIME_TAB) = 
{
    WIN_TITLE(TXT_MMS_SET_SLIDE_TIME),
    WIN_FUNC((uint32)HandleMMSEditSetSlideTimeWin),    
    WIN_ID(MMIMMS_EDIT_SET_SLIDE_TIME_WIN_ID),    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(3, MMIMMS_SET_SLIDE_TIME_CTRL_ID),
    END_WIN
};

/*
WINDOW_TABLE( MMIMMS_SELECT_SIM_SEND_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleSIMSelectSendWinMsg),    
    WIN_ID(MMIMMS_SELECT_SIM_SEND_WIN_ID),
    WIN_TITLE(TXT_SIM_SEL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMMS_SELECT_SIM_SEND_CTRL_ID),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
*/
WINDOW_TABLE( MMIMMS_ATTACHMENT_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleAttachmenTypeWinMsg),
    WIN_TITLE(TXT_MMS_ATTACHMENT_TYPE),
    WIN_ID(MMIMMS_ATTACHMENT_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIMMS_EDIT_ATTACHMENT_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : speed dial edit select contact callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void EditSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
}

/*****************************************************************************/
//	Description : speed dial edit select contact callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void EditSelectVcardCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, MMI_PB_VCARD_SELECTED, (DPARAM)handle);   
}

/*****************************************************************************/
//	Description : speed dial edit select contact callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void ListSelectContactCallback(MMIPB_HANDLE_T handle)
{
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);
#else
    MMK_SendMsg(MMIMMS_EDIT_TO_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
#endif
}

/*****************************************************************************/
//	Description : speed dial edit select contact callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void VcardSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMIMMS_ATTACHMENT_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);   
}

/*****************************************************************************/
//  Description : alert window when edit new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_E mms_error_e)
{
    switch(mms_error_e)
    {
    case MMIMMS_ERROR_NOT_INIT://彩信初始化完成
        {
            uint32 i = 0;
            BOOLEAN is_not_inited = FALSE;

            for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                if(SIM_NOT_INITED == MMIAPIPHONE_GetSimStatus((MN_DUAL_SYS_E)i))
                {
                    is_not_inited = TRUE;
                    break;
                }
            }

            if (is_not_inited)
            {
                MMSMMI_OpenMMSAlertWin(TXT_SIM_NOT_INIT, TXT_NULL, IMAGE_PUBWIN_WARNING);
            }
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_SYS_WAITING_AND_TRY_LATER, TXT_NULL, IMAGE_PUBWIN_WARNING);
            }
        }
    	break;
    case MMIMMS_ERROR_UDISK_USING:////U盘使用中
        MMSMMI_OpenMMSAlertWin(TXT_COMMON_UDISK_USING, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_CC_IN_CALL://电话通话中
        MMSMMI_OpenMMSAlertWin(TXT_MMS_IN_CALL, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_CMMB_RUNNING://CMMB播放中
        MMSMMI_OpenMMSAlertWin(TXT_EXIT_MOBILETV, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_MAX_SLIDE://彩信帧满
        MMSMMI_OpenMMSAlertWin(TXT_MMS_MAX_SLIDE, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_INVALID_SIZE://彩信大小满
        MMSMMI_OpenMMSAlertWin(TXT_MMS_INVALID_SIZE, TXT_NULL, IMAGE_PUBWIN_WARNING);
        break;
    case MMIMMS_ERROR_NO_MEMORY://彩信内存不足
        MMSMMI_OpenMMSAlertWin(TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_NO_CONTENT://彩信无内容
        MMSMMI_OpenMMSAlertWin(TXT_NO_CONTENT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_SFS_ERROR: // 文件系统错误，提示失败
    case MMIMMS_ERROR_ENCODE_ERROR: // 编码错误，提示失败
    case MMIMMS_ERROR_FAIL://失败
    case MMIMMS_ERROR_FATAL:
    case MMIMMS_ERROR_SENDRECVING://失败
        MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_NOT_SUPPORT://添加视频格式不支持
        MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    	break;
    case MMIMMS_ERROR_NO_SPACE://彩信满
        MMSMMI_OpenMMSAlertWin(TXT_MMS_FULLSPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);        
        break;
    case MMIMMS_ERROR_INVALID_URL://无效url
        MMSMMI_OpenMMSAlertWin(TXT_MMS_RECV_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);        
        break;
    case MMIMMS_ERROR_INVALIDPERIOD:
        MMSMMI_OpenMMSAlertWin(TXT_MMS_EXPIRED, TXT_NULL, IMAGE_PUBWIN_WARNING);        
        break;
    case MMIMMS_ERROR_FDN_NUMBER:
        MMSMMI_OpenMMSAlertWin(TXT_CC_FDN_ONLY, TXT_NULL, IMAGE_PUBWIN_WARNING);
        break;
    case MMIMMS_ERROR_INVALID_ADDRESS:
        MMSMMI_OpenMMSAlertWin(TXT_MMS_TEXT_ADDRESS_UNRESOLVED, TXT_NULL, IMAGE_PUBWIN_WARNING);
        break;
#ifdef VT_SUPPORT
    case MMIMMS_ERROR_IN_VT_CALL:
        MMSMMI_OpenMMSAlertWin(TXT_VT_VIDEOCALLING, TXT_NULL, IMAGE_PUBWIN_WARNING);        
        break;
#endif
    default:
        break;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description :mms add data adjust
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsCanAddMediumData(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    BOOLEAN ret = FALSE;
    uint16 slide_num                      = 0;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
    {
        return FALSE;
    }
    //插入文本
    if (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 0)
    {
        ret |= TRUE;
    }
    //插入新页
    slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
    if (slide_num < MMIMMS_MAX_SLIDE)
    {
        ret |= TRUE;
    }
    //插入图片
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        ret |= TRUE;
    }
    //插入视频文件
    if (   0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
    {
        ret |= TRUE;
    }
    //插入音频文件
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
    {
        ret |= TRUE;
    }
    return ret;
}

/*****************************************************************************/
//  Description :get edit name num
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/    
LOCAL uint16    MMIMMS_GetEditNameNum(void)
{
    MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr = g_mms_edit_globle.editdoc;
    uint16 num = 0;
    MMIMMS_EDIT_NUMBER_LIST_T *cur_numberlist_ptr = PNULL;
    if(editdoc_ptr == PNULL)
    {
        return 0;
    }
    cur_numberlist_ptr = editdoc_ptr->numberlist_header_ptr;

    while(PNULL != cur_numberlist_ptr)
    {
        cur_numberlist_ptr = cur_numberlist_ptr->next;
        num++;
    }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMIMMS_SMS_CC_SUPPORT    
    cur_numberlist_ptr = editdoc_ptr->numberlist_cc_header_ptr;
    while(PNULL != cur_numberlist_ptr)
    {
        cur_numberlist_ptr = cur_numberlist_ptr->next;
        num++;
    }

    cur_numberlist_ptr = editdoc_ptr->numberlist_bcc_header_ptr;
    while(PNULL != cur_numberlist_ptr)
    {
        cur_numberlist_ptr = cur_numberlist_ptr->next;
        num++;
    }       
#endif
#endif
    
    return num;
}
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :left right key init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMSEditWinLFRHButtonInit(void)
{
     GUI_BG_T         bg = {0}; 
     GUIFORM_CHILD_WIDTH_T  width = {0};
     bg.bg_type = GUI_BG_IMG;            
     
     GUIBUTTON_SetRunSheen(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,FALSE);
     GUIBUTTON_SetRunSheen(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,FALSE);
     bg.img_id = IMAGE_MMS_COUNT_LBTN_UN;
     GUIBUTTON_SetBg(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&bg);
     GUIBUTTON_SetFg(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&bg);
     bg.img_id = IMAGE_MMS_COUNT_LBTN_PR;
     GUIBUTTON_SetPressedFg(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&bg);  
     bg.img_id = IMAGE_MMS_COUNT_RBTN_UN;
     GUIBUTTON_SetBg(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&bg);
     GUIBUTTON_SetFg(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&bg);
     bg.img_id = IMAGE_MMS_COUNT_RBTN_PR;
     GUIBUTTON_SetPressedFg(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&bg);  

     width.type = GUIFORM_CHILD_WIDTH_PERCENT;
     width.add_data = 70;
     GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_TITLE_FORM_ID, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID, &width);           
     width.add_data = 15;
     GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_TITLE_FORM_ID, MMIMMS_EDIT_BUTTON_LF_CTRL_ID, &width);     
     GUIFORM_PermitChildBorder(MMIMMS_SMS_EDIT_TITLE_FORM_ID, FALSE);
     GUITEXT_SetAlign(MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID, ALIGN_HMIDDLE);

}
#else
#ifndef MMI_RES_LOW_COST
/*****************************************************************************/
//  Description :left right key init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMSEditWinLFRHButtonInit(void)
{
     GUI_BG_T         bg = {0}; 
     bg.bg_type = GUI_BG_IMG;            
     
     GUIBUTTON_SetRunSheen(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,FALSE);
     GUIBUTTON_SetRunSheen(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,FALSE);
     bg.img_id = IMAGE_MMS_COUNT_LBTN_UN;
     GUIBUTTON_SetBg(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&bg);
     GUIBUTTON_SetFg(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&bg);
     bg.img_id = IMAGE_MMS_COUNT_LBTN_PR;
     GUIBUTTON_SetPressedFg(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&bg);  
     bg.img_id = IMAGE_MMS_COUNT_RBTN_UN;
     GUIBUTTON_SetBg(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&bg);
     GUIBUTTON_SetFg(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&bg);
     bg.img_id = IMAGE_MMS_COUNT_RBTN_PR;
     GUIBUTTON_SetPressedFg(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&bg);  
}
#endif
#endif
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :edit win button form init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMSPreviewButtonCallBack(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_PREV);
    GUIRICHTEXT_DeleteAllItems(MMIMMS_EDIT_MENU_CTRL_ID);
    MMIMMS_CreateMMSPreviewWin(edit_doc_ptr, MMS_PREVIEW);

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description :edit win button form init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMSInsertButtonCallBack(void)
{
    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
#ifndef MMI_GUI_STYLE_TYPICAL
    MMK_CreateWin((uint32*)MMIMMS_EDIT_MAINMENU_INSERT_OPT_WIN_TAB, PNULL);
#endif

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description :edit win button form init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMSendButtonCallBack(void)
{
    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SEND);
    EditAndSendNewMMS();
    
    return MMI_RESULT_TRUE;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :edit win button form init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditWinButtonFormInit(MMIMMS_WINDOW_ID_E win_id)
{
    GUI_BG_T         bg = {0};
    GUI_BOTH_RECT_T both_rect = {0};
    GUI_BOTH_RECT_T client_rect = {0};
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);       
    GUIBUTTON_SetCallBackFunc(MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID, MMSPreviewButtonCallBack);
    GUIBUTTON_SetCallBackFunc(MMIMMS_EDIT_BUTTON_FORM_INSERT_ID, MMSInsertButtonCallBack);
    GUIBUTTON_SetCallBackFunc(MMIMMS_EDIT_BUTTON_FORM_SEND_ID, MMSendButtonCallBack);
    
    both_rect.v_rect.left = client_rect.v_rect.left ;
    both_rect.v_rect.right = client_rect.v_rect.right;
    both_rect.v_rect.top = client_rect.v_rect.bottom - MMIMMS_BUTTON_FORM_HEIGHT;
    both_rect.v_rect.bottom = client_rect.v_rect.bottom ;
    
    both_rect.h_rect.left = client_rect.h_rect.left;
    both_rect.h_rect.right = client_rect.h_rect.right;
    both_rect.h_rect.top = client_rect.h_rect.bottom - MMIMMS_BUTTON_FORM_HEIGHT ;
    both_rect.h_rect.bottom = client_rect.h_rect.bottom;
    GUIAPICTRL_SetBothRect(MMIMMS_EDIT_BUTTON_FORM_CTRL_ID, &both_rect);
    GUIFORM_IsSlide(MMIMMS_EDIT_BUTTON_FORM_CTRL_ID, FALSE);
    GUIFORM_SetMargin(MMIMMS_EDIT_BUTTON_FORM_CTRL_ID, 0);
    
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
    GUIFORM_SetBg(MMIMMS_EDIT_BUTTON_FORM_CTRL_ID, &bg);
    GUIFORM_SetMargin(MMIMMS_EDIT_BUTTON_FORM_CTRL_ID, 6);
    
    GUIBUTTON_SetRunSheen(MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMIMMS_EDIT_BUTTON_FORM_INSERT_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMIMMS_EDIT_BUTTON_FORM_SEND_ID, FALSE);   
    
    bg.img_id = IMAGE_SMS_BUTTON_NORMAL;
    GUIBUTTON_SetBg(MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID, &bg);
    GUIBUTTON_SetBg(MMIMMS_EDIT_BUTTON_FORM_INSERT_ID, &bg);
    GUIBUTTON_SetBg(MMIMMS_EDIT_BUTTON_FORM_SEND_ID, &bg);            
    
    bg.img_id = IMAGE_SMS_BUTTON_PRESSED;           
    
    GUIBUTTON_SetPressedBg(MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID, &bg);
    GUIBUTTON_SetPressedBg(MMIMMS_EDIT_BUTTON_FORM_INSERT_ID, &bg);
    GUIBUTTON_SetPressedBg(MMIMMS_EDIT_BUTTON_FORM_SEND_ID, &bg);
    
    bg.img_id =  IMAGE_MMS_PREVIEW_PDA;          
    GUIBUTTON_SetFg(MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMIMMS_EDIT_BUTTON_FORM_PREVIEW_CTRL_ID, &bg);
    
    bg.img_id =  IMAGE_MMS_INSERT_PDA;          
    GUIBUTTON_SetFg(MMIMMS_EDIT_BUTTON_FORM_INSERT_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMIMMS_EDIT_BUTTON_FORM_INSERT_ID, &bg);
    
    bg.img_id =  IMAGE_MMS_SEND_PDA;          
    GUIBUTTON_SetFg(MMIMMS_EDIT_BUTTON_FORM_SEND_ID, &bg);       
    GUIBUTTON_SetPressedFg(MMIMMS_EDIT_BUTTON_FORM_SEND_ID, &bg);        
}
#endif
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
// 	Description : 将pb中选中的收件人添加到彩信收件人编辑框
//	Global resource dependence : 
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
LOCAL void MMSAddPbSelectedContacts(uint16 pb_select_num, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, uint32 param, MMI_HANDLE_T  win_id)
{
    MMIMMS_EDIT_NUMBER_LIST_T *add_numberlist_ptr = PNULL;
    uint16  temp_len                             = 0;
    wchar  *all_number_wstr                      = PNULL;
    uint8  tele_len                              = 0;
    wchar  tele_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    uint8  tele_num[MMIMMS_MAX_ADDRESS_LEN + 2]  = {0};
    MMIPB_BCD_NUMBER_T pb_bcd_number             = {0};
    MMI_STRING_T email_addr                      = {0};
    uint32 max_number_len = 0;         
    uint16 all_number_len = 0;
    uint16 pb_select_index = 0;
    MMI_STRING_T number_name_addr                = {0};
    MMI_STRING_T  to_edit_string                  = {0};
    MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    wchar semicolon[]                            = {';', 0};
    MMIMMS_EDIT_ADD_CONTACT_TYPE add_contact_type = MMIMMS_ADD_CONTACT_OP_MAX;
    MMI_STRING_T      edit_str = {0};
    BOOLEAN is_same_num = FALSE;
    
    add_contact_type = GetMMSEditAddContactType();
    if(g_mms_edit_globle.editdoc != PNULL)
    {
        if(MMIMMS_ADD_TO_CONTACT == add_contact_type)
        {
            add_numberlist_ptr = g_mms_edit_globle.editdoc->numberlist_header_ptr;
        }
#ifdef MMIMMS_SMS_CC_SUPPORT    
        else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
        {
            add_numberlist_ptr = g_mms_edit_globle.editdoc->numberlist_cc_header_ptr; 
        }
        else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
        {
            add_numberlist_ptr = g_mms_edit_globle.editdoc->numberlist_bcc_header_ptr;
        }
#endif
    }
    if (pb_select_num > 0)
    {
        temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
        all_number_wstr = (wchar *)MMIMMS_ALLOC(temp_len);
        if (PNULL == all_number_wstr)
        {
            //SCI_TRACE_LOW:"malloc all_number_wstr failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_4677_112_2_18_2_37_48_42,(uint8*)"");
            MMSMMI_OpenMMSAlertWin(TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            return;
        }
        SCI_MEMSET(all_number_wstr, 0, temp_len);
        
        if(MMIMMS_ADD_TO_CONTACT == add_contact_type)
        {
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            {
                MMI_HANDLE_T edit_handle = 0;
                edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                    win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                    );
                GUIEDIT_GetString(edit_handle,&edit_str);
            }               
            
#else
            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TO_LIST_ID,&edit_str);
#endif
        }
#ifdef MMIMMS_SMS_CC_SUPPORT
        else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
        {
            GUIEDIT_GetString(MMIMMS_SMS_EDIT_CC_TO_LIST_ID,&edit_str);
        }
        else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
        {
            GUIEDIT_GetString(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID,&edit_str);
        }
#endif                            
        if( edit_str.wstr_len > 0)
        {
            MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIMMS_MAX_ADDRESS_LEN, edit_str.wstr_ptr, MMIMMS_MAX_ADDRESS_LEN,  edit_str.wstr_len);                
            all_number_len +=edit_str.wstr_len;
            if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
            {
                MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                all_number_len++;
            }
        }
        
        SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
        SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
        SCI_MEMSET(&email_addr, 0, sizeof(email_addr));
        email_addr.wstr_ptr = (wchar *)MMIMMS_ALLOC((MMIPB_MAX_MAIL_LEN + 1)*sizeof(wchar));
        if (PNULL == email_addr.wstr_ptr)
        {
            //SCI_TRACE_LOW:"malloc email_addr failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_4726_112_2_18_2_37_48_43,(uint8*)"");
            MMIMMS_FREE(all_number_wstr);
            all_number_wstr = PNULL;
            MMSMMI_OpenMMSAlertWin(TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            return;
        }
        SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
        number_name_addr.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
        SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
        
        for (pb_select_index = 0; pb_select_index < MIN(MMIMMS_MAX_RECEIVER_NUM, pb_select_num); pb_select_index++)
        {
            SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
            SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
            email_addr.wstr_len = 0;
            SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
            number_name_addr.wstr_len = 0;

            MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &pb_bcd_number,MMIPB_BCD_NUMBER_MAX_LEN, pb_select_index);
            MMIAPIPB_GetSelectEmail((MMIPB_HANDLE_T)param,&email_addr, MMIPB_MAX_MAIL_LEN, pb_select_index);
            MMIAPIPB_GetSelectName((MMIPB_HANDLE_T)param,&number_name_addr, MMIMMS_FILE_NAME_LEN, pb_select_index);
            max_number_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, pb_bcd_number.number_len);
            tele_len = MMIAPIPB_BCDNumberToString(&pb_bcd_number, MMIMMS_MAX_ADDRESS_LEN + 2, tele_num);
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_4752_112_2_18_2_37_48_44,(uint8*)"d",tele_len);
            if(0 != tele_len)
            {  
                MMI_STRNTOWSTR(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, (uint8 *)tele_num, MMIMMS_MAX_ADDRESS_LEN, tele_len );
                //判断该号码是否已经存在                        
                temp_numberlist_ptr = add_numberlist_ptr;
                is_same_num = FALSE;
                while(PNULL != temp_numberlist_ptr)
                {
                    if (0 != temp_numberlist_ptr->number_len
                        && temp_numberlist_ptr->number_len == MMIAPICOM_Wstrlen(tele_wstr)
                        && !MMIAPICOM_Wstrncmp(temp_numberlist_ptr->number_ptr, tele_wstr, MMIMMS_MAX_ADDRESS_LEN))
                    {
                        is_same_num = TRUE;
                        break;
                    }
                    else
                    {
                        temp_numberlist_ptr = temp_numberlist_ptr->next;
                    }
                    
                }
                if (!is_same_num)
                {
                    //加入到to的结构中去
                    //todo to_edit_string里面是原有的的号码，在尾部添加；同时加上现在的号码
                    MMIAPICOM_Wstrncat(all_number_wstr + all_number_len, tele_wstr, tele_len);
                    all_number_len = all_number_len + tele_len;
                    MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                    all_number_len = all_number_len +1;
                    
                    cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                    SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                    if(0 != number_name_addr.wstr_len)
                    {//有pb中名称
                        cursor_numberlist_ptr->is_object = TRUE;
                        cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                        cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                            number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);
                        
                        cursor_numberlist_ptr->number_len = tele_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                    }
                    else
                    {//没有名称按普通的号码处理
                        cursor_numberlist_ptr->is_object = FALSE;
                        cursor_numberlist_ptr->number_len = tele_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                    }
                    if(PNULL == add_numberlist_ptr)
                    {
                        if(MMIMMS_ADD_TO_CONTACT == add_contact_type)
                        {
                            g_mms_edit_globle.editdoc->numberlist_header_ptr = cursor_numberlist_ptr;
                        }
#ifdef MMIMMS_SMS_CC_SUPPORT    
                        else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
                        {
                            g_mms_edit_globle.editdoc->numberlist_cc_header_ptr = cursor_numberlist_ptr;
                        }
                        else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
                        {
                            g_mms_edit_globle.editdoc->numberlist_bcc_header_ptr = cursor_numberlist_ptr;
                        }
#endif
                        add_numberlist_ptr = cursor_numberlist_ptr;
                    }
                    else
                    {
                        temp_numberlist_ptr = add_numberlist_ptr;
                        while(PNULL != temp_numberlist_ptr->next)
                        {
                            temp_numberlist_ptr = temp_numberlist_ptr->next;
                        }
                        temp_numberlist_ptr->next = cursor_numberlist_ptr;
                    }
                }
            }  
            else
            {
                //SCI_TRACE_MID:"warning_ tele_len 0"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_4764_112_2_18_2_37_48_45,(uint8*)"");
            }
            if (0 != email_addr.wstr_len)
            {
                //判断该号码是否已经存在                        
                temp_numberlist_ptr = add_numberlist_ptr;
                is_same_num = FALSE;
                while(PNULL != temp_numberlist_ptr)
                {
                    if (0 != temp_numberlist_ptr->number_len
                        && temp_numberlist_ptr->number_len == email_addr.wstr_len
                        && !MMIAPICOM_Wstrncmp(temp_numberlist_ptr->number_ptr, email_addr.wstr_ptr, MMIMMS_MAX_ADDRESS_LEN))
                    {
                        is_same_num = TRUE;
                        break;
                    }
                    else
                    {
                        temp_numberlist_ptr = temp_numberlist_ptr->next;
                    }
                }
                if (!is_same_num)
                {
                    if (MMIAPICOM_IsASCIIString(email_addr.wstr_ptr, email_addr.wstr_len))
                    {     
                        MMIAPICOM_Wstrncat(all_number_wstr, email_addr.wstr_ptr, email_addr.wstr_len);
                        all_number_len = all_number_len + email_addr.wstr_len;
                        MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                        all_number_len = all_number_len +1;
                    }
                    cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOCAZ(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));

                    if(0 != number_name_addr.wstr_len)
                    {//有pb中名称
                        cursor_numberlist_ptr->is_object = TRUE;
                        cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                        cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                            number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);
                        
                        // cursor_numberlist_ptr->is_object = FALSE;
                        cursor_numberlist_ptr->number_len = email_addr.wstr_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            email_addr.wstr_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                    }
                    else//按普通的号码处理
                    {
                        cursor_numberlist_ptr->is_object = FALSE;
                        cursor_numberlist_ptr->number_len = email_addr.wstr_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            email_addr.wstr_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                    }
                    if(PNULL == add_numberlist_ptr)
                    {
                        if(MMIMMS_ADD_TO_CONTACT == add_contact_type)
                        {
                            g_mms_edit_globle.editdoc->numberlist_header_ptr = cursor_numberlist_ptr;
                        }
#ifdef MMIMMS_SMS_CC_SUPPORT    
                        else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
                        {
                            g_mms_edit_globle.editdoc->numberlist_cc_header_ptr = cursor_numberlist_ptr;
                        }
                        else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
                        {
                            g_mms_edit_globle.editdoc->numberlist_bcc_header_ptr = cursor_numberlist_ptr;
                        }
#endif
                        add_numberlist_ptr = cursor_numberlist_ptr;
                    }
                    else
                    {
                        temp_numberlist_ptr = add_numberlist_ptr;
                        while(PNULL != temp_numberlist_ptr->next)
                        {
                            temp_numberlist_ptr = temp_numberlist_ptr->next;
                        }
                        temp_numberlist_ptr->next = cursor_numberlist_ptr;
                    }               
                }
                
            }  
            //显示添加号码
            
        }       
        //ToListBoxSetArray(ctrl_id, add_numberlist_ptr);
        //free add_numberlist_ptr
        
        //把得到的号码加入edit document中去
        //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_4877_112_2_18_2_37_48_46,(uint8*)"");
        to_edit_string.wstr_ptr = all_number_wstr;
        to_edit_string.wstr_len = MMIAPICOM_Wstrlen(all_number_wstr);
        SetMMSEditIsSaveDraft(FALSE); 
        if(MMIMMS_ADD_TO_CONTACT == add_contact_type)
        {
            MMIMMS_SetMMSTo(&to_edit_string,edit_doc_ptr);
        }
#ifdef MMIMMS_SMS_CC_SUPPORT
        else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
        {
            MMIMMS_SetMMSCC(&to_edit_string,edit_doc_ptr);            
        }
        else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
        {
            MMIMMS_SetMMSBCC(&to_edit_string,edit_doc_ptr);
        }
#endif
        MMSShowToContactInfo();
        if (PNULL != email_addr.wstr_ptr)
        { 
            MMIMMS_FREE(email_addr.wstr_ptr);
            email_addr.wstr_ptr = NULL;
        }
        if (PNULL != number_name_addr.wstr_ptr)
        {
            SCI_FREE(number_name_addr.wstr_ptr);
            number_name_addr.wstr_ptr = NULL;
        }
        MMIMMS_FREE(all_number_wstr);
        all_number_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description : to handle convert sms to mms waiting window    
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConvertSmsToMmsWaitWinMsg(
                                                   MMI_WIN_ID_T        win_id,    //IN:
                                                   MMI_MESSAGE_ID_E    msg_id,    //IN:
                                                   DPARAM                param    //IN:
                                                   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_CLOSE_WINDOW:
        {                   
            MMI_STRING_T edit_string = {0};
            wchar *edit_ptr = PNULL;

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            MMI_HANDLE_T edit_handle = 0;
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
#endif
            MMIMMS_SetIsMMSMode(TRUE);
            MMIMMS_SlideInit();
            
#ifdef MMI_PDA_SUPPORT                 
            GUISOFTKEY_SetTextId(
                MMIMMS_EDIT_MENU_WIN_ID,                      
                MMICOMMON_SOFTKEY_CTRL_ID,
                STXT_PREVIEW,   // the left softkey id
                TXT_COMM_INSERT, // the middle softkey id
                TXT_SEND,  // the right softkey id
                FALSE       // whether update
                );  
#endif                    
#ifdef MMI_TIMERMSG_SUPPORT   
            if(MMIAPISMS_GetIsTimerMode())
            {
                MMIAPISMS_SetIsTimerMode(FALSE);       //关闭timer msg                                          
            }
#endif     
            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &edit_string);
            edit_ptr = SCI_ALLOCAZ((edit_string.wstr_len + 1)*sizeof(wchar));
            if(PNULL == edit_ptr)
            {
			    //SCI_TRACE_LOW:"MMS alloc error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_15924_112_2_18_2_38_11_114,(uint8*)"");
                return FALSE;
            }
            SCI_MEMSET(edit_ptr,0x00,(edit_string.wstr_len + 1)*sizeof(wchar));
            MMIAPICOM_Wstrncpy(edit_ptr, edit_string.wstr_ptr, edit_string.wstr_len);

            GUIEDIT_SetSmsEncode(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,FALSE);
            GUIEDIT_SetMaxLen(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,MMIMMS_MAX_TEXT_LEN/sizeof(wchar));


            
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            //GUIEDIT_SetListStyle(edit_handle, GUIEDIT_LIST_STYLE_ALL);
            
#else
            GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
#endif
#ifdef MMIMMS_SMS_CC_SUPPORT    
            GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_CC_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
            GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
#endif
            GUIEDIT_SetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, edit_ptr,edit_string.wstr_len);
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, edit_handle);
            
            
#else
            MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif
            SCI_FREE(edit_ptr);
            edit_ptr = PNULL;
#ifdef MMI_PDA_SUPPORT             
            HandleSendButtonDisplay(MMIMMS_GetIsMMSMode());
#endif

            if (!MMITHEME_IsIstyle())
            {
                GUIFORM_SetChildDisplay(MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID, MMIMMS_SMS_LABEL_COUNT_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            }

            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//	Description : MmsHandleCallogSelectReturn 
//	Global resource dependence : 
//	Author:jixin.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MmsHandleCallogSelectReturn(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, 
                                          MMI_HANDLE_T  win_handle,
                                          MMI_MESSAGE_ID_E  msg_id)
{
    uint16 cl_select_num = 0;
    MMIMMS_EDIT_NUMBER_LIST_T *add_numberlist_ptr = PNULL;
    uint16  temp_len                             = 0;
    wchar  *all_number_wstr                      = PNULL;
    uint8  tele_len                              = 0;
    wchar  tele_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    uint8  tele_num[MMIMMS_MAX_ADDRESS_LEN + 2]  = {0};
    MMIPB_BCD_NUMBER_T cl_bcd_number             = {0};
    uint32 max_number_len = 0;         
    uint16 all_number_len = 0;
    MMI_STRING_T number_name_addr                = {0};
    MMI_STRING_T  to_edit_string                  = {0};
    MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    wchar semicolon[]                            = {';', 0};
    BOOLEAN is_same_num = FALSE;
    MMI_STRING_T      edit_str = {0};
    uint16 item_num = 0;
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT 
        MMI_HANDLE_T edit_handle = 0;
#endif  
    if (PNULL == edit_doc_ptr)
    {
        return FALSE;
    }

    cl_select_num = MMIAPICL_GetSelectedItemCount();

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT 
    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
        win_handle,
        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
        );
    item_num = GUIEDIT_GetListItemNum(edit_handle);
#else
    item_num = GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif                
    if(item_num + cl_select_num > MMIMMS_MAX_RECEIVER_NUM)  //暂时短彩都是30条，待控件支持
    {
        MMSMMI_OpenMMSAlertWin(TXT_SMS_EXCEED_MAX_NUM, TXT_NULL, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }

    if(MMIMMS_GetIsMMSMode())
    {
        if(g_mms_edit_globle.editdoc != PNULL)
        {
            add_numberlist_ptr = g_mms_edit_globle.editdoc->numberlist_header_ptr;
        }
        if (cl_select_num > 0)
        {
            temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
            all_number_wstr = (wchar *)MMIMMS_ALLOC(temp_len);
            if (PNULL == all_number_wstr)
            {
                //SCI_TRACE_LOW:"malloc all_number_wstr failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_1970_112_2_18_2_37_42_23,(uint8*)"");
                MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                return FALSE;
            }
            SCI_MEMSET(all_number_wstr, 0, temp_len);
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            edit_handle = 0;
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_handle,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
            GUIEDIT_GetString(edit_handle,&edit_str);
#else
            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TO_LIST_ID,&edit_str);
#endif
            
            if( edit_str.wstr_len > 0)
            {
                MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIMMS_MAX_ADDRESS_LEN, edit_str.wstr_ptr, MMIMMS_MAX_ADDRESS_LEN,  edit_str.wstr_len);                
                all_number_len +=edit_str.wstr_len;
                if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
                {
                    MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                    all_number_len++;
                }
            }
            
            SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
            SCI_MEMSET(&cl_bcd_number, 0, sizeof(cl_bcd_number));
            number_name_addr.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
            SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
            
            /*read the selected phone number from calllog*/
            {
                uint16 i = 0;
                uint16 j = 0;
                
      //          if (cl_select_num > 0)
       //         {
                    // read the selected phone number from calllog one by one
                    for (i=0, j=0; i<MMICL_RECORD_MAX_NUM; i++)
                    {
                        SCI_MEMSET(&cl_bcd_number, 0, sizeof(MMIPB_BCD_NUMBER_T));
                        SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
                        if (MMIAPICL_SelectedListIsValid(i))
                        {
                            MMIAPICL_GetSelectPhonenum(&cl_bcd_number, i);
                            MMIAPICL_GetSelectPhoneName( &number_name_addr, i,
                                MMIMMS_FILE_NAME_LEN, &cl_bcd_number
                                );
                            max_number_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, cl_bcd_number.number_len);
                            tele_len = MMIAPICOM_GenDispNumber( 
                                MMIPB_GetNumberTypeFromUint8(cl_bcd_number.npi_ton), 
                                max_number_len, 
                                cl_bcd_number.number,  
                                tele_num,
                                MMIMMS_MAX_ADDRESS_LEN + 2
                                );
                            //SCI_TRACE_LOW:"HandleMMSEditToListWinMsg,tele_len = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2028_112_2_18_2_37_42_24,(uint8*)"d",tele_len);
                            
                            
                            if(0 != tele_len)
                            {
                                MMI_STRNTOWSTR(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, (uint8 *)tele_num, MMIMMS_MAX_ADDRESS_LEN, tele_len );
                                //判断该号码是否已经存在                        
                                temp_numberlist_ptr = add_numberlist_ptr;
                                is_same_num = FALSE;
                                while(PNULL != temp_numberlist_ptr)
                                {
                                    if (0 != temp_numberlist_ptr->number_len
                                        && temp_numberlist_ptr->number_len == MMIAPICOM_Wstrlen(tele_wstr)
                                        && !MMIAPICOM_Wstrncmp(temp_numberlist_ptr->number_ptr, tele_wstr, MMISMS_NUMBER_MAX_LEN))
                                    {
                                        is_same_num = TRUE;
                                        break;
                                    }
                                    else
                                    {
                                        temp_numberlist_ptr = temp_numberlist_ptr->next;
                                    }
                                    
                                }
                                if (!is_same_num)
                                {
                                    //加入到to的结构中去
                                    //todo to_edit_string里面是原有的的号码，在尾部添加；同时加上现在的号码
                                    MMIAPICOM_Wstrncat(all_number_wstr + all_number_len, tele_wstr, tele_len);
                                    all_number_len = all_number_len + tele_len;
                                    MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                                    all_number_len = all_number_len +1;
                                    
                                    //加入到namelist的结构中去
                                    cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                                    SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                                    if(0 != number_name_addr.wstr_len)
                                    {//有pb中名称
                                        cursor_numberlist_ptr->is_object = TRUE;
                                        cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                                        cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                            number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);
                                        
                                        cursor_numberlist_ptr->number_len = tele_len;
                                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                        MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                            tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                                    }
                                    else
                                    {//没有名称按普通的号码处理
                                        cursor_numberlist_ptr->is_object = FALSE;
                                        cursor_numberlist_ptr->number_len = tele_len;
                                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                        MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                            tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                                    }
                                    if(PNULL == add_numberlist_ptr)
                                    {
                                        g_mms_edit_globle.editdoc->numberlist_header_ptr = cursor_numberlist_ptr;
                                        add_numberlist_ptr = cursor_numberlist_ptr;
                                    }
                                    else
                                    {
                                        temp_numberlist_ptr = add_numberlist_ptr;
                                        while(PNULL != temp_numberlist_ptr->next)
                                        {
                                            temp_numberlist_ptr = temp_numberlist_ptr->next;
                                        }
                                        temp_numberlist_ptr->next = cursor_numberlist_ptr;
                                    } 
                                } 
                                {
                                    //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!"
                                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2102_112_2_18_2_37_42_25,(uint8*)"");
                                }
                            }//if (tele_len != 0)
                            else
                            {
                                //SCI_TRACE_MID:"warning_ tele_len 0"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2107_112_2_18_2_37_42_26,(uint8*)"");
                            }                        
                            
                            j++;
                        }
                        if (j >= cl_select_num)//reach the last selected item
                        {
                            break;
                        }
                    }
                //}
                }
            }
        
        //显示添加号码
        //把得到的号码加入edit document中去
        //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2123_112_2_18_2_37_42_27,(uint8*)"");
        to_edit_string.wstr_ptr = all_number_wstr;
        to_edit_string.wstr_len = MMIAPICOM_Wstrlen(all_number_wstr);
        SetMMSEditIsSaveDraft(FALSE); 
        MMIMMS_SetMMSTo(&to_edit_string,edit_doc_ptr);
        MMSShowToContactInfo();
        
        if (PNULL != number_name_addr.wstr_ptr)
        {
            SCI_FREE(number_name_addr.wstr_ptr);
            number_name_addr.wstr_ptr = NULL;
        }
        MMIMMS_FREE(all_number_wstr);
        all_number_wstr = PNULL;
    }
    else  //短信模式
    {
        // from PHONEBOOK, call the API to get the phone number.
        //MMSGetNumberFromPb((uint32)param, msg_id);
        MMISMS_GetNumberFromCalllog(msg_id);
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
            win_handle,
            MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
            );
        MMSAddSmsNumber(edit_handle);
        MMK_SetAtvCtrl(win_handle, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);//控件已替换成dropdownlist
#else
        MMSAddSmsNumber(MMIMMS_SMS_EDIT_TO_LIST_ID);
        MMK_SetAtvCtrl(win_handle, MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif
    }

    return TRUE;
}
#endif

#else

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//	Description : MmsHandleCallogSelectReturn 
//	Global resource dependence : 
//	Author:jixin.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MmsHandleCallogSelectReturn(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    uint16 cl_select_num = 0;
    MMIMMS_EDIT_NUMBER_LIST_T *add_numberlist_ptr = PNULL;
    uint16  temp_len                             = 0;
    wchar  *all_number_wstr                      = PNULL;
    uint8  tele_len                              = 0;
    wchar  tele_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    uint8  tele_num[MMIMMS_MAX_ADDRESS_LEN + 2]  = {0};
    MMIPB_BCD_NUMBER_T pb_bcd_number             = {0};
    MMIPB_BCD_NUMBER_T cl_bcd_number             = {0};
    uint32 max_number_len = 0;         
    uint16 all_number_len = 0;
    MMI_STRING_T number_name_addr                = {0};
    MMI_STRING_T  to_edit_string                  = {0};
    MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    wchar semicolon[]                            = {';', 0};
    BOOLEAN is_same_num = FALSE;

    if (PNULL == edit_doc_ptr)
    {
        return FALSE;
    }
    cl_select_num = MMIAPICL_GetSelectedItemCount();
    if(g_mms_edit_globle.editdoc != PNULL)
    {
        add_numberlist_ptr = g_mms_edit_globle.editdoc->numberlist_header_ptr;
    }
    if (cl_select_num > 0)
    {
        temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
        all_number_wstr = (wchar *)MMIMMS_ALLOC(temp_len);
        if (PNULL == all_number_wstr)
        {
            //SCI_TRACE_LOW:"malloc all_number_wstr failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2201_112_2_18_2_37_42_28,(uint8*)"");
            MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            return FALSE;
        }
        SCI_MEMSET(all_number_wstr, 0, temp_len);
        if (PNULL != edit_doc_ptr->edit_to.wstr_ptr && 0 != edit_doc_ptr->edit_to.wstr_len)
        {
            MMI_WSTRNCPY(all_number_wstr, temp_len/2, edit_doc_ptr->edit_to.wstr_ptr, edit_doc_ptr->edit_to.wstr_len, edit_doc_ptr->edit_to.wstr_len);
            all_number_len = all_number_len + edit_doc_ptr->edit_to.wstr_len;
        }
        
        SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
        SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
        number_name_addr.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
        SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));

        /*read the selected phone number from calllog*/
        {
            uint16 i = 0;
            uint16 j = 0;

            if (cl_select_num > 0)
            {
                // read the selected phone number from calllog one by one
                for (i=0, j=0; i<MMICL_RECORD_MAX_NUM; i++)
                {
                    SCI_MEMSET(&cl_bcd_number, 0, sizeof(MMIPB_BCD_NUMBER_T));
                    SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
                    if (MMIAPICL_SelectedListIsValid(i))
                    {
                        MMIAPICL_GetSelectPhonenum(&cl_bcd_number, i);
                        MMIAPICL_GetSelectPhoneName( &number_name_addr, i,
                                                     MMIMMS_FILE_NAME_LEN, &cl_bcd_number
                                                   );
                        max_number_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, cl_bcd_number.number_len);
                        tele_len = MMIAPICOM_GenDispNumber( 
                            MMIPB_GetNumberTypeFromUint8(cl_bcd_number.npi_ton), 
                            max_number_len, 
                            cl_bcd_number.number,  
                            tele_num,
                            MMISMS_NUMBER_MAX_LEN + 2
                            );
                        //SCI_TRACE_LOW:"HandleMMSEditToListWinMsg,tele_len = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2243_112_2_18_2_37_42_29,(uint8*)"d",tele_len);
                        
                        
                        if(0 != tele_len)
                        {
                            MMI_STRNTOWSTR(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, (uint8 *)tele_num, MMIMMS_MAX_ADDRESS_LEN, tele_len );
                            //判断该号码是否已经存在                        
                            temp_numberlist_ptr = add_numberlist_ptr;
                            is_same_num = FALSE;
                            while(PNULL != temp_numberlist_ptr)
                            {
                                if (0 != temp_numberlist_ptr->number_len
                                    && temp_numberlist_ptr->number_len == MMIAPICOM_Wstrlen(tele_wstr)
                                    && !MMIAPICOM_Wstrncmp(temp_numberlist_ptr->number_ptr, tele_wstr, MMISMS_NUMBER_MAX_LEN))
                                {
                                    is_same_num = TRUE;
                                    break;
                                }
                                else
                                {
                                    temp_numberlist_ptr = temp_numberlist_ptr->next;
                                }
                                
                            }
                            if (!is_same_num)
                            {
                                //加入到to的结构中去
                                //todo to_edit_string里面是原有的的号码，在尾部添加；同时加上现在的号码
                                MMIAPICOM_Wstrncat(all_number_wstr + all_number_len, tele_wstr, tele_len);
                                all_number_len = all_number_len + tele_len;
                                MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                                all_number_len = all_number_len +1;
                                
                                //加入到namelist的结构中去
                                cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                                SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                                if(0 != number_name_addr.wstr_len)
                                {//有pb中名称
                                    cursor_numberlist_ptr->is_object = TRUE;
                                    cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                                    cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                    MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                        number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);

                                    cursor_numberlist_ptr->number_len = tele_len;
                                    cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                        tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                                }
                                else
                                {//没有名称按普通的号码处理
                                    cursor_numberlist_ptr->is_object = FALSE;
                                    cursor_numberlist_ptr->number_len = tele_len;
                                    cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                        tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                                }
                                if(PNULL == add_numberlist_ptr)
                                {
                                    g_mms_edit_globle.editdoc->numberlist_header_ptr = cursor_numberlist_ptr;
                                    add_numberlist_ptr = cursor_numberlist_ptr;
                                }
                                else
                                {
                                    temp_numberlist_ptr = add_numberlist_ptr;
                                    while(PNULL != temp_numberlist_ptr->next)
                                    {
                                        temp_numberlist_ptr = temp_numberlist_ptr->next;
                                    }
                                    temp_numberlist_ptr->next = cursor_numberlist_ptr;
                                } 
                            } 
                            {
                                //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2317_112_2_18_2_37_43_30,(uint8*)"");
                            }
                        }//if (tele_len != 0)
                        else
                        {
                            //SCI_TRACE_MID:"warning_ tele_len 0"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2322_112_2_18_2_37_43_31,(uint8*)"");
                        }                        
                        
                        j++;
                    }
                    if (j >= cl_select_num)//reach the last selected item
                    {
                        break;
                    }
                }
                
            }
        }
        //显示添加号码
        //把得到的号码加入edit document中去
        //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2337_112_2_18_2_37_43_32,(uint8*)"");
        to_edit_string.wstr_ptr = all_number_wstr;
        to_edit_string.wstr_len = MMIAPICOM_Wstrlen(all_number_wstr);
        SetMMSEditIsSaveDraft(FALSE); 
        MMIMMS_SetMMSTo(&to_edit_string,edit_doc_ptr);

        if (PNULL != number_name_addr.wstr_ptr)
        {
            SCI_FREE(number_name_addr.wstr_ptr);
            number_name_addr.wstr_ptr = NULL;
        }
        MMIMMS_FREE(all_number_wstr);
        all_number_wstr = PNULL;
    }

    return TRUE;
}
#endif /*MMI_PDA_SUPPORT*/   

#endif



#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :mms edit menu handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSContactButtonInit(void)
{
#ifdef MMI_PDA_SUPPORT 
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    GUI_BG_T bg = {0};   
    child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
    child_width.add_data = MMIMMS_EDIT_CONTACT_ICON_WIDTH;        
    GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_BUTTON_ID,&child_width);       
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
    GUIBUTTON_SetBg(MMIMMS_SMS_EDIT_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_PRESSED_BG;
    GUIBUTTON_SetPressedBg(MMIMMS_SMS_EDIT_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
    GUIBUTTON_SetFg(MMIMMS_SMS_EDIT_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
    GUIBUTTON_SetPressedFg(MMIMMS_SMS_EDIT_TO_BUTTON_ID, &bg);
    GUIBUTTON_SetRunSheen(MMIMMS_SMS_EDIT_TO_BUTTON_ID,FALSE);
    GUIBUTTON_PermitBorder(MMIMMS_SMS_EDIT_TO_BUTTON_ID,  FALSE);
  
#ifdef MMIMMS_SMS_CC_SUPPORT
    child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
    child_width.add_data = MMIMMS_EDIT_CONTACT_ICON_WIDTH;        
    GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID,&child_width);       
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
    GUIBUTTON_SetBg(MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_PRESSED_BG;
    GUIBUTTON_SetPressedBg(MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
    GUIBUTTON_SetFg(MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
    GUIBUTTON_SetPressedFg(MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID, &bg);
    GUIBUTTON_SetRunSheen(MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID,FALSE);
    GUIBUTTON_PermitBorder(MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID,  FALSE);
    
    child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
    child_width.add_data = MMIMMS_EDIT_CONTACT_ICON_WIDTH;        
    GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID,&child_width);       
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
    GUIBUTTON_SetBg(MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_PRESSED_BG;
    GUIBUTTON_SetPressedBg(MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
    GUIBUTTON_SetFg(MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT;
    GUIBUTTON_SetPressedFg(MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID, &bg);
    GUIBUTTON_SetRunSheen(MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID,FALSE);
    GUIBUTTON_PermitBorder(MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID,  FALSE);
#endif
#endif  
}

/*****************************************************************************/
//  Description :mms show mms info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowEditContent(void)
{
    MMI_STRING_T          mms_text_string = {0};
    MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;
    MMI_CTRL_ID_T ctrl_id = MMIMMS_SMS_EDIT_TEXT_CTRL_ID;
    edit_doc_ptr = g_mms_edit_globle.editdoc;    

    GUIEDIT_ClearAllStr(ctrl_id);
    MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string);
    GUIEDIT_SetString(ctrl_id,mms_text_string.wstr_ptr,mms_text_string.wstr_len);   
    //MMK_PostMsg(ctrl_id, MSG_CTL_PAINT, PNULL, 0);
}

/*****************************************************************************/
//  Description :mms show mms info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowMMSInfo(void)
{
    uint16 cur_slide = 0;
    uint16 total_slide = 0;                       
    uint8    display_str_slide[MMIMMS_SLIDE_TIME_STR_LEN]  = {0};
    uint8    display_str_size[MMIMMS_SLIDE_TIME_STR_LEN]   = {0};
    uint8   display_title_str[MMIMMS_SLIDE_TIME_STR_LEN]   = {0};
    wchar    display_title_wstr[MMIMMS_SLIDE_TIME_STR_LEN] = {0};
    uint16 wstr_len= 0;
    int32 total_size = 0;
    MMIMMS_EDIT_DOCUMENT_T  *edit_doc_ptr = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    cur_slide = MMIMMS_GetSlideIndex(edit_doc_ptr);
    total_slide = MMIMMS_GetSlideCount(edit_doc_ptr);    
    total_size = edit_doc_ptr->editbody_ptr->totalsize;
    SCI_MEMSET((void*)display_str_slide, 0, sizeof(display_str_slide));
    //N/M, N==current slide, M==total slide number
    _snprintf((char *)display_str_slide,MMIMMS_SLIDE_TIME_STR_LEN, "%ld/%ld", cur_slide,  total_slide);   
    
    if (0 != total_size)
    {
        //SCI_TRACE_LOW:"FNH total_size = %ld"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2459_112_2_18_2_37_43_33,(uint8*)"d",total_size);
        _snprintf((void *)display_str_size,MMIMMS_SLIDE_TIME_STR_LEN ,"%.1fK",(float)total_size/ONE_K_LEN); 
        if (((float)total_size/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)total_size/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
        {
            _snprintf((void*)display_str_size, MMIMMS_SLIDE_TIME_STR_LEN,":%.1fk\r",0.1);
        }
    }   
    
    SCI_MEMSET((void *)display_title_str,0,sizeof(display_title_str));
    _snprintf((void *)display_title_str,MMIMMS_SLIDE_TIME_STR_LEN, "%s %s",display_str_slide, display_str_size);
    wstr_len = strlen((const char *)display_title_str);
    MMI_STRNTOWSTR( display_title_wstr, MMIMMS_SLIDE_TIME_STR_LEN, (uint8 *)display_title_str, MMIMMS_SLIDE_TIME_STR_LEN, wstr_len );
    GUITEXT_SetString(MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID,display_title_wstr, wstr_len, FALSE);
    
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :mms show edit to and theme
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetElementVisibility(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMIMMS_ERROR_E add_slide_err  = MMIMMS_NO_ERROR;
    uint16 slide_count = 0;
    uint16 cur_slide_index = 0;
    uint32 additional_file_num = 0;
    
    SCI_TRACE_LOW("MMISMS: is_mms_mode = %d",g_mms_edit_globle.is_mms_mode);
    if(!MMIMMS_GetIsMMSMode()) //短信模式
    {
#ifdef MMI_TIMERMSG_SUPPORT    
        GUIFORM_SetStyle(MMIMMS_SMS_TIMER_SMS_FORM_ID,GUIFORM_STYLE_UNIT);

        if (!MMIAPISMS_GetIsTimerMode())
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                MMIMMS_SMS_TIMER_SMS_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);
        }
        else
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                MMIMMS_SMS_TIMER_SMS_FORM_ID, 
                GUIFORM_CHILD_DISP_NORMAL);
        }
#endif        

        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
        if (!MMISMS_GetAutoSignatureFlag())
        {                                    
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
            MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, 
            GUIFORM_CHILD_DISP_HIDE);
        }
#endif        
                    
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_ANIM_CTRL_ID, 
            GUIFORM_CHILD_DISP_HIDE);    
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_MUSIC_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);  
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);  
//         GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID,
//             MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,
//             GUIFORM_CHILD_DISP_HIDE);
#ifdef MMIMMS_SMS_CC_SUPPORT
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);
#endif  
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
            GUIFORM_CHILD_DISP_HIDE);

        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_TITLE_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);        
    }
    else
    {
        edit_doc_ptr = g_mms_edit_globle.editdoc;

        if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
        {
            return ;
        }
        
        slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);
        if (0 == slide_count)
        {
            SetMMSEditIsSaveDraft(FALSE);
            add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
            if (MMIMMS_NO_ERROR != add_slide_err)
            {
                MMIMMS_EditMMSAlertWin(add_slide_err);
                return;
            }        
        }
        if (PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
        {
            return ;
        }
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,  //OK
            MMIMMS_SMS_EDIT_TITLE_FORM_ID, 
            GUIFORM_CHILD_DISP_NORMAL);        //是彩信必显示该内容

#ifdef MMI_TIMERMSG_SUPPORT    
       GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
            MMIMMS_SMS_TIMER_SMS_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);
#endif

        cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
        MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);     
        if (1 == cur_slide_index)//该页是首页，需要显示to和subject
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_NORMAL); 

            if(!MMITHEME_IsIstyle())
            {
                MMI_HANDLE_T edit_handle = 0;
                uint16 item_num = 0;
                #ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT 
                   edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                       MMIMMS_EDIT_MENU_WIN_ID,
                       MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                       );
                   item_num = GUIEDIT_GetListItemNum(edit_handle);
                #else
                   item_num = GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID);
                #endif 
            
                if(item_num > 1)    
                {     
                   GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LABEL_ID, GUIFORM_CHILD_DISP_NORMAL);           
                }
                else
                {
                   GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LABEL_ID, GUIFORM_CHILD_DISP_HIDE);     
                }
            }
            
            
            if(GetMMSEditIsSubjectExist())  //主题
            {
                BOOLEAN ret = FALSE;
                ret = GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                    MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);  
                if(ret)
                {
                    MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID);
                }
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                    MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
                    GUIFORM_CHILD_DISP_HIDE);     
            }
#ifdef MMIMMS_SMS_CC_SUPPORT            
            if(GetMMSEditIsCCExist())  //抄送
            {
                BOOLEAN ret = FALSE;

                ret = GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);

                if (ret)
                {
                    MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_CC_TO_LIST_ID);
                }
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_HIDE);
            }
            
            if(GetMMSEditIsBCCExist())  //密送
            {
                BOOLEAN ret = FALSE;

                ret = GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);

                if (ret)
                {
                    MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_BCC_TO_LIST_ID);
                }
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_HIDE);     
            }                 
#endif            
        }
        else
        { 
   //         if(!MMITHEME_IsIstyle())
   //         {
			//GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, 
			//    MMIMMS_SMS_EDIT_TO_LABEL_ID, 
			//	GUIFORM_CHILD_DISP_HIDE);  
   //         }
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);
#ifdef MMIMMS_SMS_CC_SUPPORT       
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);
            
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);   
#endif    
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, //主题 
                MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
                GUIFORM_CHILD_DISP_HIDE);    

            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);   
        }

        if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize
            ||
            (
            1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag
            ||1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag
            )
            ||
            (
            (0 != additional_file_num) && (cur_slide_index == 1)
            )
            )  //false表示没有audio
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,
                MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,
                GUIFORM_CHILD_DISP_NORMAL); 
        }
        
        if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)  //false表示没有audio
        {                        
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_MUSIC_FORM_ID, 
                GUIFORM_CHILD_DISP_NORMAL);            
        }
        else
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_MUSIC_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);    
        }
        
        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag
            ||1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag
            )
        {       
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ANIM_CTRL_ID, 
                GUIFORM_CHILD_DISP_NORMAL);            
        }
        else
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ANIM_CTRL_ID, 
                GUIFORM_CHILD_DISP_HIDE);    
        }
        
        if(cur_slide_index == 1)
        {
            if(0 == additional_file_num )
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                    MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                    GUIFORM_CHILD_DISP_HIDE); 
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                    MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);
            }
        }
		else
		{
		    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE); 
		}
        
        if (1 < cur_slide_index && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize 
            && 1 != edit_doc_ptr->editbody_ptr->slide_cur->image_flag
            && 1 != edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,
            MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,
            GUIFORM_CHILD_DISP_HIDE);
        }
    }    
}

#else
/*****************************************************************************/
//  Description :mms show edit to and theme
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetElementVisibility(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMIMMS_ERROR_E add_slide_err  = MMIMMS_NO_ERROR;
    uint16 slide_count = 0;
    uint16 cur_slide_index = 0;
    uint32 additional_file_num = 0;
    
    if(!MMIMMS_GetIsMMSMode()) //短信模式
    {
#ifdef MMI_TIMERMSG_SUPPORT    
        GUIFORM_SetStyle(MMIMMS_SMS_TIMER_SMS_FORM_ID,GUIFORM_STYLE_UNIT);

        if (!MMIAPISMS_GetIsTimerMode())
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_TIMER_SMS_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);
        }
        else
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_TIMER_SMS_FORM_ID, 
                GUIFORM_CHILD_DISP_NORMAL);
        }
#endif        

        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);

#ifdef SMS_AUTO_SIGNATURE_SUPPORT
        if (!MMISMS_GetAutoSignatureFlag())
        {                                    
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, 
            GUIFORM_CHILD_DISP_HIDE);
        }
#endif                    
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_ANIM_CTRL_ID, 
            GUIFORM_CHILD_DISP_HIDE);    
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_MUSIC_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);  
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
            MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);  
//         GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID,
//             MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,
//             GUIFORM_CHILD_DISP_HIDE);
#ifdef MMIMMS_SMS_CC_SUPPORT
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);
#endif  

        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
            GUIFORM_CHILD_DISP_HIDE);

        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_EDIT_TITLE_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);        
    }
    else
    {
        edit_doc_ptr = g_mms_edit_globle.editdoc;
        if (PNULL == edit_doc_ptr)
        {
            return ;
        }
        if (PNULL == edit_doc_ptr->editbody_ptr)
        {
            return ;
        }
        
        slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);
        if (0 == slide_count)
        {
            SetMMSEditIsSaveDraft(FALSE);
            add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
            if (MMIMMS_NO_ERROR != add_slide_err)
            {
                MMIMMS_EditMMSAlertWin(add_slide_err);
                return;
            }        
        }
        if (PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
        {
            return ;
        }

        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_EDIT_TITLE_FORM_ID, 
            GUIFORM_CHILD_DISP_NORMAL);        //是彩信必显示该内容

#ifdef MMI_TIMERMSG_SUPPORT    
       GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
            MMIMMS_SMS_TIMER_SMS_FORM_ID, 
            GUIFORM_CHILD_DISP_HIDE);
#endif

        cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
        MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);  
        if (1 == cur_slide_index)//该页是首页，需要显示to和subject
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_NORMAL); 
            
            if(GetMMSEditIsSubjectExist())  //主题
            {
                BOOLEAN ret = FALSE;

                ret = GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);			

                if (ret)
                {
                    MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID);
                }
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
                    GUIFORM_CHILD_DISP_HIDE);    				
            }
#ifdef MMIMMS_SMS_CC_SUPPORT            
            if(GetMMSEditIsCCExist())  //抄送
            {
                BOOLEAN ret = FALSE;

                ret = GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);

                if (ret)
                {
                    MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_CC_TO_LIST_ID);
                }
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_HIDE);
            }
            
            if(GetMMSEditIsBCCExist())  //密送
            {
                BOOLEAN ret = FALSE;

                ret = GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);

                if (ret)
                {
                    MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_BCC_TO_LIST_ID);
                }
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                    MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
                    GUIFORM_CHILD_DISP_HIDE);     
            }                
#endif            
        }
        else
        { 
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);
#ifdef MMIMMS_SMS_CC_SUPPORT       
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_CC_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);
            
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_BCC_TO_LIST_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);   
#endif    
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, //主题 
                MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 
                GUIFORM_CHILD_DISP_HIDE);    

            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);   
        }

        if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize
            ||
            (
            1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag
            ||1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag
            )
            ||
            (
            (0 != additional_file_num) && (cur_slide_index == 1)
            )
            )  //false表示没有audio
        {
		    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
                MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                GUIFORM_CHILD_DISP_NORMAL);
        }
			
        if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)  //false表示没有audio
        {		
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_MUSIC_FORM_ID, 
                GUIFORM_CHILD_DISP_NORMAL);            
        }
        else
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_MUSIC_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE);    
        }
        
        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag
            ||1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag
            )
        {		
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ANIM_CTRL_ID, 
                GUIFORM_CHILD_DISP_NORMAL);            
        }
        else
        {
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ANIM_CTRL_ID, 
                GUIFORM_CHILD_DISP_HIDE);    
        }

        if(cur_slide_index == 1)
        {
            if(0 == additional_file_num )
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                    MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                    GUIFORM_CHILD_DISP_HIDE);   
            }
            else
            {
                GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                    MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                    GUIFORM_CHILD_DISP_NORMAL);
            }
        }
		else
		{
		    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
                MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
                GUIFORM_CHILD_DISP_HIDE); 
		}
                
        if (1 < cur_slide_index && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize 
            && 1 != edit_doc_ptr->editbody_ptr->slide_cur->image_flag
            && 1 != edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
        {		
            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID,
            MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,
            GUIFORM_CHILD_DISP_HIDE);
        }
    }
}
#endif
/*****************************************************************************/
//  Description :mms show attach file
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowAttachFile(void)
{
      GUIANIM_CTRL_INFO_T anim_ctrl_info = {0};        
      GUIANIM_DISPLAY_INFO_T anim_disp_info = {0};
      GUIANIM_DATA_INFO_T    anim_data_info = {0};
      MMIMMS_EDIT_DOCUMENT_T  *edit_doc_ptr = PNULL;
      uint32 additional_file_num = 0;
      MMS_ACCESSORY_PARAM_T *add_file_ptr = PNULL;
      uint16 len = 0;
      wchar *disp_str = SCI_ALLOC_APP(MMIMMS_MAX_ADDFILE_NUM *(MMS_FILE_NAME_LEN + 1)*sizeof(wchar));
      wchar file_name[MMS_FILE_NAME_LEN] = {0};
      uint16 disp_len = 0;
      GUIFORM_CHILD_WIDTH_T width = {0};
      uint16 cur_slide_index = 0;
      edit_doc_ptr = g_mms_edit_globle.editdoc;

      if( PNULL == disp_str)
      {
          //SCI_TRACE_LOW:"alloc space not enough"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2978_112_2_18_2_37_44_34,(uint8*)"");
          return;
      }

      cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
      if(1 != cur_slide_index )
      {
          //SCI_TRACE_LOW:"mms curr slide not first slide, not show attachment"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2985_112_2_18_2_37_44_35,(uint8*)"");
          SCI_FREE(disp_str);
          return;
      }
      
      SCI_MEMSET(disp_str,0x00,MMIMMS_MAX_ADDFILE_NUM *(MMS_FILE_NAME_LEN + 1)*sizeof(wchar));
      MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);
      if(0 == additional_file_num)
      {
          //SCI_TRACE_LOW:"mms additional_file_num == 0, not show attachment"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_2993_112_2_18_2_37_44_36,(uint8*)"");
          SCI_FREE(disp_str);
          return;
      }
      add_file_ptr = edit_doc_ptr->editbody_ptr->accessory_ptr;

      while(add_file_ptr )
      {           
          len = strlen(add_file_ptr->filename);
          SCI_MEMSET(file_name,0,sizeof(file_name));
          GUI_UTF8ToWstr(file_name,len,(uint8*)add_file_ptr->filename,len);
          MMIAPICOM_Wstrncpy(disp_str + disp_len , file_name, len);          
          disp_len +=  MMIAPICOM_Wstrlen(file_name);
          MMIAPICOM_Wstrncat(disp_str + disp_len,L";", 1);
          disp_len += 1;
          
          add_file_ptr  = add_file_ptr->next;          
      }
                      
      width.type = GUIFORM_CHILD_WIDTH_PERCENT;
      width.add_data = 80;
      GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, MMIMMS_SMS_EDIT_ATTACHMENT_TEXT_ID, &width);
       
      anim_ctrl_info.is_ctrl_id = TRUE;
      anim_ctrl_info.ctrl_id = MMIMMS_SMS_EDIT_ATTACHMENT_ANIM_ID;
            
      anim_disp_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
      anim_data_info.img_id = IMAGE_MMS_ACCESSORY_FILE;
      
      GUIANIM_SetParam(&anim_ctrl_info, &anim_data_info, PNULL, &anim_disp_info);            
                       
      GUITEXT_SetString(MMIMMS_SMS_EDIT_ATTACHMENT_TEXT_ID, disp_str,  disp_len,TRUE);

      GUIFORM_SetChildDisplay(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID, 
        MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID, 
        GUIFORM_CHILD_DISP_NORMAL);
        
      SCI_FREE(disp_str);
}

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
/*****************************************************************************/
//  Description :append matching num list for DDL
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void MMSAppendDDLMatchingNumList(uint16 index, uint16 cl_match_item_num, MMIPB_HANDLE_T pb_num_search_handle)
{
    MMI_STRING_T num_str = {0};
    MMI_STRING_T name_str = {0};
    wchar num_wchar[MMISMS_NUMBER_MAX_LEN + 1] = {0};
    wchar name_wchar[MMIPB_MAX_STRING_LEN + 1] = {0};
    BOOLEAN is_num_valid = FALSE;
    BOOLEAN is_name_valid = FALSE;
    char bcd_to_str[MMISMS_NUMBER_MAX_LEN + 1] = {0};
    MMIPB_BCD_NUMBER_T  pb_num = {0};
    uint16 total_item_num = 0;
    uint32 group = 0;
    const wchar angle_bracket_left[2] = {'<', 0};
    const wchar angle_bracket_right[2] = {'>', 0};
    
    num_str.wstr_ptr = num_wchar;
    name_str.wstr_ptr = name_wchar;
    if(index < cl_match_item_num)
    {
        //call log匹配记录
        is_num_valid = MMIAPICL_GetSearchEntryNumber(&num_str, index);
        is_name_valid = MMIAPICL_GetSearchEntryName(&name_str, index, FALSE);
        if(is_name_valid && is_num_valid)
        {
            GUIDROPDOWNLIST_AppendItemExt(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, &name_str, &num_str);
        }
        else if(!is_name_valid && is_num_valid)
        {
            //MMI_GetLabelTextByLang(TXT_NO_NAME, &name_str);
            MMIAPICOM_Wstrncat(name_str.wstr_ptr, angle_bracket_left, 1);
            MMIAPICOM_Wstrncat(name_str.wstr_ptr, num_str.wstr_ptr, MIN(MMIPB_MAX_STRING_LEN-2, num_str.wstr_len));
            MMIAPICOM_Wstrncat(name_str.wstr_ptr, angle_bracket_right, 1);
            name_str.wstr_len = MIN(MMIPB_MAX_STRING_LEN, num_str.wstr_len + 2);

            GUIDROPDOWNLIST_AppendItemExt(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, &name_str, &num_str);
            total_item_num = GUIDROPDOWNLIST_GetTotalItemNum(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
            
            if (0 < total_item_num)
            {
                GUIDROPDOWNLIST_SetValidItemValue(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, total_item_num - 1, GUIDNLIST_VALID_VALUE_1);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMMS: MMSAppendDDLMatchingNumList num&name invalid"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_3074_112_2_18_2_37_44_37,(uint8*)"");
            return;
        }
    }
    else
    {      
        //pb匹配记录
        uint32 pb_index = index - cl_match_item_num;
        if(MMIAPIPB_GetSearchEntry(pb_num_search_handle, &name_str, MMIPB_MAX_STRING_LEN, &pb_num, &group,pb_index))
        {

            MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, 
                pb_num.number, 
                pb_num.number_len<<1, 
                bcd_to_str);
            MMI_STRNTOWSTR(num_wchar, MMISMS_NUMBER_MAX_LEN, 
                (const uint8*)bcd_to_str, MMISMS_NUMBER_MAX_LEN, 
                strlen(bcd_to_str));
            num_str.wstr_len = strlen(bcd_to_str);
            if(0 == name_str.wstr_len)
            {
                //MMI_GetLabelTextByLang(TXT_NO_NAME, &name_str);
                MMIAPICOM_Wstrncat(name_str.wstr_ptr, angle_bracket_left, 1);
                MMIAPICOM_Wstrncat(name_str.wstr_ptr, num_str.wstr_ptr, MIN(MMIPB_MAX_STRING_LEN-2, num_str.wstr_len));
                MMIAPICOM_Wstrncat(name_str.wstr_ptr, angle_bracket_right, 1);
                name_str.wstr_len = MIN(MMIPB_MAX_STRING_LEN, num_str.wstr_len + 2);

                GUIDROPDOWNLIST_AppendItemExt(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, &name_str, &num_str);

                total_item_num = GUIDROPDOWNLIST_GetTotalItemNum(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
                
                if (0 < total_item_num)
                {
                    GUIDROPDOWNLIST_SetValidItemValue(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, total_item_num - 1, GUIDNLIST_VALID_VALUE_1);
                }
            }
            else if(0 == num_str.wstr_len)
            {
                //SCI_TRACE_LOW:"MMIMMS: MMSAppendDDLMatchingNumList 0 == num_str.wstr_len"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_3107_112_2_18_2_37_44_38,(uint8*)"");
                return;
            }
            else
            {
                GUIDROPDOWNLIST_AppendItemExt(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, &name_str, &num_str);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMMS: MMSAppendDDLMatchingNumList MMIAPIPB_GetSearchEntry reutrn false"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_3117_112_2_18_2_37_44_39,(uint8*)"");
            return;
        }
    }
    
    return;
}


/*****************************************************************************/
//  Description :update matching num DDL
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL void MMSUpdateDDLMatchingNumList(MMIMMS_WINDOW_ID_E win_id)
{
    int index = 0;
    uint16 cl_match_item_num = 0;
    uint16 pb_match_item_num = 0;
    MMI_STRING_T edit_wstr = {0};
    uint16 cursor_pos = 0;
    int pos = 0;
    wchar* edit_list_item_str_startptr = PNULL;
    wchar* edit_list_item_str_endptr = PNULL;
    int16 item_str_len = 0;
    MMI_STRING_T edit_list_item_wstr = {0};
    MMIPB_HANDLE_T pb_num_search_handle = 0;
    int32 email_len = 0;
	int32 dropdownlist_number = 0;

    MMI_HANDLE_T edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                                               win_id,
                                                               MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                                               );

    do
    {
        //根据当前光标位置获得当前操作的字符串
        GUIEDIT_GetString(edit_handle, &edit_wstr);
        cursor_pos = GUIEDIT_GetCursorCurPos(edit_handle);//获得当前操作时光标的位置

        for (pos = cursor_pos; pos >= 0; pos--)
        {
           if(*(edit_wstr.wstr_ptr+pos) == MMIMMS_CR && *(edit_wstr.wstr_ptr+pos-1) == MMIMMS_SEMICOLON)//找到回车符同时前一位置符号为分号的位置
           {
               edit_list_item_str_startptr = edit_wstr.wstr_ptr + pos + 1;
               break;
           }
           else if (0 == pos)//找到edit框最开始的位置，也就是当前字符串是第一个号码的情况
           {
               edit_list_item_str_startptr = edit_wstr.wstr_ptr;
               break;
           }
           else if (*(edit_wstr.wstr_ptr+pos-1) == MMIMMS_SEMICOLON)//找到分号同时分号的下一个位置不是回车符的位置，也就是当前分号所处的号码是最后一个号码的情况
           {
               edit_list_item_str_startptr = edit_wstr.wstr_ptr + pos;
               break;
           }
        }
        for (pos = cursor_pos; pos <= edit_wstr.wstr_len; pos++)
        {
           if(*(edit_wstr.wstr_ptr+pos) == MMIMMS_SEMICOLON || pos == edit_wstr.wstr_len)//找到分号结束符的位置，或者最后一个不带分号的当前编辑的字符串的结束位置
           {
               edit_list_item_str_endptr = edit_wstr.wstr_ptr + pos;
               break;
           }
        }
        if (PNULL != edit_list_item_str_endptr && PNULL != edit_list_item_str_startptr)
        {
            item_str_len = edit_list_item_str_endptr - edit_list_item_str_startptr;
        }
        else
        {
            break;
        }
        if (item_str_len > 0)
        {
           edit_list_item_wstr.wstr_ptr = (wchar*)SCI_ALLOCAZ(((uint16)item_str_len + 1)*sizeof(wchar));
           if (PNULL == edit_list_item_wstr.wstr_ptr)
           {
               break;
           }
           edit_list_item_wstr.wstr_len = item_str_len;
           MMI_WSTRNCPY(edit_list_item_wstr.wstr_ptr, item_str_len, 
                        edit_list_item_str_startptr, item_str_len,
                        item_str_len);
           //根据字符串查找pb、cl
           cl_match_item_num = MMIAPICL_SearchList(&edit_list_item_wstr);
           pb_num_search_handle = MMIAPIPB_SearchList(&edit_list_item_wstr, MMIPB_LIST_FIELD_NUMBER|MMIPB_LIST_FIELD_MAIL , &pb_match_item_num);//lint !e655

           //根据匹配结果打开下拉框
           if (cl_match_item_num+pb_match_item_num > 0)
           {
			   dropdownlist_number = MIN((cl_match_item_num+pb_match_item_num), MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_NUM);
               GUIDROPDOWNLIST_SetMaxItem(
                   MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                   dropdownlist_number           //cl_match_item_num+pb_match_item_num bug145920
                   );
               GUIDROPDOWNLIST_SetDispItemDetail(
                   MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                   TRUE       
                   );
               while(index < dropdownlist_number)
               {
                   MMSAppendDDLMatchingNumList(index, cl_match_item_num, pb_num_search_handle);
                   index++;
               }
               GUIDROPDOWNLIST_OpenList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
#ifndef MMI_PDA_SUPPORT               
               GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, MIDDLE_BUTTON, TRUE);
#endif
           }
           else
           {
               GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
           }
           if(!MMIMMS_GetIsMMSMode() && ((URL_IsMailto((URL_CHAR_T const *)edit_list_item_wstr.wstr_ptr, &email_len))
               ||(URL_IsMailtoDefault((URL_CHAR_T const *)edit_list_item_wstr.wstr_ptr, &email_len))))
           {
		       MMIMMS_SetMMSEditIsEmailAddrExist(TRUE);
               MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                   PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleConvertSmsToMmsWaitWinMsg,(uint32)0);
           }
           else if (MMIMMS_GetIsMMSMode())
           { 
               uint16 to_number = 0;
               GUIEDIT_LIST_ITEM_STR_T *num_str_ptr = PNULL;
               to_number = GUIEDIT_GetListItemNum(edit_handle);
               num_str_ptr = SCI_ALLOCAZ(to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
               if (PNULL == num_str_ptr)
               {
                   break;
               }
               GUIEDIT_GetListItemString(edit_handle,to_number,num_str_ptr);
               //GUIEDIT_GetListItemArray(edit_handle,to_number,item_array_ptr);               
               
               for (index = 0; index < to_number; index++)
               {
                   if ((URL_IsMailto((URL_CHAR_T const *)num_str_ptr[index].str_ptr, &email_len))
                       ||(URL_IsMailtoDefault((URL_CHAR_T const *)num_str_ptr[index].str_ptr, &email_len)))
                   {
                       break;
                   }
               }
               SCI_FREE(num_str_ptr);
               if (index == to_number)
               {
                   MMIMMS_SetMMSEditIsEmailAddrExist(FALSE);
                   if (!MMIMMS_CheckIsMMS())
                   {
                       MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                           PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);
                   }
               }
           }
        }
        else//空字符串
        {
           GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
           if (MMIMMS_GetIsMMSMode() && 0 == edit_wstr.wstr_len)
           {
               MMIMMS_SetMMSEditIsEmailAddrExist(FALSE);
               if (!MMIMMS_CheckIsMMS())
               {
                   MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                       PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);
               }
           }
           break;
        }

    }while(0);
  
    //释放内存
    if (PNULL != edit_list_item_wstr.wstr_ptr)
    {
        SCI_FREE(edit_list_item_wstr.wstr_ptr);
    }
    //结束搜索
    MMIAPICL_SearchExit();
    MMIAPIPB_SearchExit(pb_num_search_handle);
    
    return;
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :mms edit  main menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void HandleSendButtonDisplay(BOOLEAN is_mms_mode)
{
    MMI_HANDLE_T edit_handle = 0;
    uint16              item_num = 0;
    //uint16 content_num = 0;
    MMI_STRING_T  str = {0};
    GUI_BG_T button_bg = {0};
    MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                    MMIMMS_EDIT_MENU_WIN_ID,
                                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                    );
#else
    edit_handle = MMIMMS_SMS_EDIT_TO_LIST_ID;
#endif
    item_num = GUIEDIT_GetListItemNum(edit_handle);
    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &str);
    
    if(is_mms_mode)
    {
        //content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);  
        if(0 == item_num || (0 == MMIMMS_GetAllContentCount(edit_doc_ptr) && 0 == str.wstr_len))
        {
           if(!g_mms_edit_globle.is_send_gray)
           {           
               button_bg.bg_type = GUI_BG_IMG;
               button_bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_DISABLE_BG;
               GUIBUTTON_SetGrayed(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, TRUE, &button_bg, TRUE);
               g_mms_edit_globle.is_send_gray = TRUE;
           }
        }
        else
        {
           if(g_mms_edit_globle.is_send_gray)
           {
               button_bg.bg_type = GUI_BG_IMG;
               button_bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
               GUIBUTTON_SetGrayed(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, FALSE, &button_bg, TRUE);
               g_mms_edit_globle.is_send_gray = FALSE;
           }
        } 
    }
    else
    {

        if(0 == item_num /*|| 0 == str.wstr_len */)
        {
           if(!g_mms_edit_globle.is_send_gray)
           {           
               button_bg.bg_type = GUI_BG_IMG;
               button_bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_DISABLE_BG;
               GUIBUTTON_SetGrayed(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, TRUE, &button_bg, TRUE);
               g_mms_edit_globle.is_send_gray = TRUE;
           }
        }
        else
        {
           if(g_mms_edit_globle.is_send_gray)
           {
               button_bg.bg_type = GUI_BG_IMG;
               button_bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
               GUIBUTTON_SetGrayed(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, FALSE, &button_bg, TRUE);
               g_mms_edit_globle.is_send_gray = FALSE;
           }
        }    
    }
}

/*****************************************************************************/
// 	Description : 显示编辑联系人的个数
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSShowEditToCount(uint16 item_num)
{
    MMI_STRING_T text_str = {0};    
    uint8 index_str[MMIMMS_EDIT_NUMBER_COUNT_LEN + 1] = {0};
    wchar index_wstr[MMIMMS_EDIT_NUMBER_COUNT_LEN + 1] = {0};
    uint16 index_str_len = 0;
    
    index_str_len = (uint16)sprintf((char *)index_str,"%d/%d",item_num,MMIMMS_MAX_RECEIVER_NUM);
    MMI_STRNTOWSTR(index_wstr,MMIMMS_EDIT_NUMBER_COUNT_LEN,
    (const uint8 *)index_str,MMIMMS_EDIT_NUMBER_COUNT_LEN,
    index_str_len);
    text_str.wstr_ptr = index_wstr;
    text_str.wstr_len = index_str_len;
    GUILABEL_SetFont(MMIMMS_SMS_EDIT_TO_LABEL_ID, SONG_FONT_12, MMI_BLACK_COLOR);
    GUILABEL_SetText(MMIMMS_SMS_EDIT_TO_LABEL_ID, &text_str, TRUE);                          
}

/*****************************************************************************/
// 	Description : 显示编辑短信字数的个数
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSShowEditWordCount(void)
{
    uint16 page_num = 0;
    uint16 left_number = 0;
    MMI_STRING_T text_str = {0};
    uint8 index_str[MMIMMS_EDIT_NUMBER_COUNT_LEN + 1] = {0};
    wchar index_wstr[MMIMMS_EDIT_NUMBER_COUNT_LEN + 1] = {0};
    uint16 index_str_len = 0;

    left_number = GUIEDIT_GetLeftNumInTextEditBox(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &page_num);
    index_str_len = (uint16)sprintf((char *)index_str,"%d/%d",left_number,page_num);
    MMI_STRNTOWSTR(index_wstr,MMIMMS_EDIT_NUMBER_COUNT_LEN,
    (const uint8 *)index_str,MMIMMS_EDIT_NUMBER_COUNT_LEN,
    index_str_len);
    text_str.wstr_ptr = index_wstr;
    text_str.wstr_len = index_str_len;

    GUILABEL_SetFont(MMIMMS_SMS_LABEL_COUNT_CTRL_ID, SONG_FONT_12, MMI_BLACK_COLOR);
    GUILABEL_SetText(MMIMMS_SMS_LABEL_COUNT_CTRL_ID, &text_str, TRUE);                          
}

/*****************************************************************************/
// 	Description : 设置发送按钮区域的属性
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSSetButtonAreaProperty(void)
{
    GUI_BG_T bg = {0}; 
    GUI_FONT_ALL_T font_all = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};    
    uint16 v_space = 5;
    
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    child_width.add_data = 43;

    GUIFORM_SetChildDock(MMIMMS_SMS_EDIT_FORM_ID,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID,GUIFORM_CHILD_DOCK_RIGHT);

    GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_FORM_ID,MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID,&child_width);
    GUIFORM_SetSpace(MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID,PNULL,&v_space);
    
    GUIBUTTON_SetTextId(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, TXT_SEND);
    font_all.font = MMI_DEFAULT_SMALL_FONT;
    GUIBUTTON_SetFont(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, &font_all);
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
    GUIBUTTON_SetBg(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, &bg);   
    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_PRESSED_BG;
    GUIBUTTON_SetPressedBg(MMIMMS_SMS_BUTTON_SEND_CTRL_ID, &bg);    
    GUIBUTTON_SetRunSheen(MMIMMS_SMS_BUTTON_SEND_CTRL_ID,FALSE);
    
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_BG;
    GUIBUTTON_SetBg(MMIMMS_SMS_BUTTON_INSERT_CTRL_ID, &bg);    
    bg.img_id = IMAGE_SMS_WRITE_SMS_ADD_CONTACT_PRESSED_BG;
    GUIBUTTON_SetPressedBg(MMIMMS_SMS_BUTTON_INSERT_CTRL_ID, &bg);    
    bg.img_id = IMAGE_MMS_ACCESSORY_FILE;
    GUIBUTTON_SetFg(MMIMMS_SMS_BUTTON_INSERT_CTRL_ID, &bg);    
    bg.img_id = IMAGE_MMS_ACCESSORY_FILE;
    GUIBUTTON_SetPressedFg(MMIMMS_SMS_BUTTON_INSERT_CTRL_ID, &bg);
    GUIBUTTON_SetRunSheen(MMIMMS_SMS_BUTTON_INSERT_CTRL_ID,FALSE);
    GUIBUTTON_PermitBorder(MMIMMS_SMS_BUTTON_INSERT_CTRL_ID,FALSE);    
}

/*****************************************************************************/
// 	Description : 设置添加联系人区域的属性
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSSetEditToAreaProperty(void)
{
    GUI_BG_T bg = {0};
	MMI_HANDLE_T edit_handle = 0;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};

    //set edit list form padding
    GUIFORM_SetMargin(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,2);

    //form not set child bg
	GUIFORM_PermitChildBg(MMIMMS_SMS_EDIT_FORM_ID,FALSE);
    GUIFORM_PermitChildBg(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,FALSE);

    //set edit list form bg
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;
    
    GUIFORM_SetBg(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, &bg);

    //set edit no bg
    bg.bg_type = GUI_BG_NONE;
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
	GUIAPICTRL_SetBg(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,&bg);

    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
        MMIMMS_EDIT_MENU_WIN_ID,
        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
        );

	GUIAPICTRL_SetBg(edit_handle,&bg);
#else
	GUIAPICTRL_SetBg(MMIMMS_SMS_EDIT_TO_LIST_ID,&bg);
#endif
     
    if(!MMITHEME_IsIstyle())
    {
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
        GUIFORM_SetChildHeight(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID,MMIMMS_SMS_EDIT_TO_BUTTON_ID,&child_height); 
    }

}

/*****************************************************************************/
// 	Description : 设置信息内容区域的属性
//	Global resource dependence : 
//  Author: rong.gu
//	Note:
/*****************************************************************************/
LOCAL void MMSSetContentAreaProperty(void)
{
    GUIFORM_CHILD_HEIGHT_T child_height = {0};

    child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(MMIMMS_SMS_EDIT_FORM_ID,MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,&child_height);

    child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
    GUIFORM_SetChildHeight(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID,MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&child_height);
  
    GUIFORM_SetMargin(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 2);
}

/*****************************************************************************/
//  Description :mms edit menu handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSEditMainMenuWin(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              )
{
    MMI_RESULT_E err_code                          = MMI_RESULT_TRUE;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    MMIFILE_FILE_INFO_T  file_info                 = {0};
    uint16 content_num                             = 0;
    MULTIM_SELECT_RETURN_T *multim_return_info_ptr = PNULL;
    uint16 cur_slide_index                         = 0;
    uint16 slide_num                               = 0;
    MMIMMS_ERROR_E   slide_err                     = MMIMMS_NO_ERROR;
    uint32 insert_total_size                       = 0;
    uint32 real_file_size                          = 0;
    uint32 replaceed_size                          = 0;
    uint32  vcard_data_len                         = 0;
    uint8 *vcard_buf_ptr                           = 0;
    GUIANIM_TYPE_E    pic_type                     = GUIANIM_TYPE_NONE;
    MMI_STRING_T  vacard_name_str                  = {0};
    wchar  vacard_suf[]                            = {'.', 'v', 'c', 'f',0};
    uint16 num = 0;
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMI_STRING_T edit_string = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    MMIMMS_EDIT_INS_OP_TYPE edit_type = MMIMMS_OP_MAX;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_3454_112_2_18_2_37_45_40,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 pic_width = 0;
            uint16 pic_height = 0;
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            GUIDROPDOWNLIST_TYPE_T edit_to_dropdownlist = {GUIDNLIST_EDITABLE_NUMLIST,
                MMIMMS_MAX_ADDRESS_LEN,
                MMIMMS_MAX_RECEIVER_NUM};
#endif          
            MMIAPISMS_SetNormalClose(FALSE);
            if(!MMITHEME_IsIstyle())
            {
                child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
                child_width.add_data = MMIMMS_EDIT_CONTACT_ICON_WIDTH;            
                GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID,&child_width);
            }
            
            GUITEXT_SetHandleTpMsg(
                FALSE,  //in:
                MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID    //in:
                );  
			
            GUIFORM_PermitChildBorder(MMIMMS_SMS_EDIT_MUSIC_FORM_ID, FALSE);
            GUITEXT_SetHandleTpMsg(
                                   FALSE,  //in:
                                   MMIMMS_SMS_EDIT_MUSIC_TEXT_ID    //in:
                                   );
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            GUIFORM_SetChildDock(MMIMMS_SMS_EDIT_FORM_ID, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, GUIFORM_CHILD_DOCK_TOP);
            if (MMITHEME_IsMainScreenLandscape())
            {
                GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX_H);
                GUIDROPDOWNLIST_SetPageNum(win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                    MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_H);
            }
            else
            {
                GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX);
                GUIDROPDOWNLIST_SetPageNum(win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                    MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM);
            }
            GUIDROPDOWNLIST_SetCtrlType(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                (const GUIDROPDOWNLIST_TYPE_T *)(&edit_to_dropdownlist)
                );
#endif	
            
            MMSSetEditToAreaProperty();
            MMSSetButtonAreaProperty();
            MMSSetContentAreaProperty();            
            
            MMIMMS_GetPicDefaultWidthAndHeight(&pic_width, &pic_height);
            child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            child_height.add_data = pic_height;
            GUIFORM_SetChildHeight(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,MMIMMS_SMS_EDIT_ANIM_CTRL_ID,&child_height);    
            
#ifdef MMI_TIMERMSG_SUPPORT
            //设置收件人图标和号码框的比例
            child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            child_width.add_data = 25;        
            GUIFORM_SetChildWidth(MMIMMS_SMS_TIMER_SMS_FORM_ID,MMIMMS_SMS_EDIT_TIMER_CTRL_ID,&child_width);
            GUILABEL_SetIcon(MMIMMS_SMS_EDIT_TIMER_CTRL_ID, IMAGE_SMS_TIMER_ICON);
            MMSSetTimerMsgStartParam();
            MMSSetTimerMsgDisplay(FALSE);
#endif
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
            GUIEDIT_SetFont(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, MMISMS_GetWinFont());
#endif								          
            MMSContactButtonInit();         //to ,cc ,bcc的button显示        
            MMSEditWinLFRHButtonInit();    
            MMSEditSetEditDefaultString(win_id);
            if(MMIMMS_GetIsMMSMode())
            {
			    MMIMMS_WinInit(EDIT_OPTION_WIN_ENTRY);   
                MMIMMS_SlideInit();
                MMSSubjectInit();  
#ifdef MMI_PDA_SUPPORT            
                GUISOFTKEY_SetTextId(win_id,MMICOMMON_SOFTKEY_CTRL_ID,STXT_PREVIEW, TXT_COMM_INSERT, TXT_SEND,FALSE);
#endif
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                {
                    MMI_HANDLE_T edit_handle = 0;
                    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                        win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                        );
                    //GUIEDIT_SetListStyle(edit_handle, GUIEDIT_LIST_STYLE_ALL);
                }
#else
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
                
#endif//这个地方要不要改?控件属性可能不一样了

                GUIEDIT_SetSmsEncode(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,FALSE);
                GUIEDIT_SetMaxLen(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,MMIMMS_MAX_TEXT_LEN/sizeof(wchar));
                
          
                MMSShowContactInfo();             
                MMSShowEditContent(); 
                MMSShowMMSInfo();
                MMSShowMultiFileInfo();
                MMSShowAttachFile();  
            }
            else
            {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
                if (MMISMS_GetAutoSignatureFlag() &&        
                    MMISMS_BOX_SENDSUCC != MMISMS_GetCurBoxType() && 
                    MMISMS_BOX_SENDFAIL != MMISMS_GetCurBoxType())
                {
                    wchar  signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
                    uint16 signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
                    
                    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                        MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, 
                        GUIFORM_CHILD_DISP_NORMAL); 
                    
                    GUIEDIT_PermitBorder(MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, TRUE);
                    //set edit height
                    //child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
                    //GUIFORM_SetChildHeight(MMIMMS_SMS_SINATURE_FORM_ID, MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, &child_height);
                    
                    signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr);
                    
                    if (MMISMS_BOX_SENDSUCC != MMISMS_GetCurBoxType() && MMISMS_BOX_SENDFAIL != MMISMS_GetCurBoxType())
                    {
                        GUIEDIT_SetString(MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, signature_wstr, signature_str_len);
                    }                              
                }
                else
                {
                    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                        MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, 
                        GUIFORM_CHILD_DISP_HIDE);
                }
#endif
#endif
#ifdef MMI_PDA_SUPPORT               
                GUISOFTKEY_SetTextId(win_id,MMICOMMON_SOFTKEY_CTRL_ID,TXT_MMS_SMS_TEMPLATE,TXT_COMM_INSERT,TXT_SEND,FALSE);     
#endif
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                {
                    MMI_HANDLE_T edit_handle = 0;
                    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                        win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                        );
                    //GUIEDIT_SetListStyle(edit_handle, GUIEDIT_LIST_STYLE_PHONENUM);
                    //更改输入法配置，要跟需求确认默认输入法
                    //GUIEDIT_SetIm(edit_handle, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ENGLISH);//lint !e655
                    MMSAddSmsNumber(edit_handle);
                }
#else
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_TO_LIST_ID, GUIEDIT_LIST_STYLE_PHONENUM);                       
                MMSAddSmsNumber(MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif          
            }
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            {
                MMI_HANDLE_T edit_handle = 0;
                uint16              item_num = 0;
                edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                    win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                    );
                item_num = GUIEDIT_GetListItemNum(edit_handle); 
                if(item_num > 0)
                {        
                    MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);  
                    if(!MMITHEME_IsIstyle())
                    {
                        if(item_num > 1)
                        {
                            MMSShowEditToCount(item_num);
                            GUIFORM_SetChildDisplay(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LABEL_ID, GUIFORM_CHILD_DISP_NORMAL);           
                        }
                        else
                        {
                            GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LABEL_ID, GUIFORM_CHILD_DISP_HIDE);     
                        }
                    }
                }
                else
                {
                    if(!MMITHEME_IsIstyle())
                    {
                        GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LABEL_ID, GUIFORM_CHILD_DISP_HIDE); 
                    }
                    MMK_SetAtvCtrl(win_id, edit_handle);   
                }
            }   
#else
            if(GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID) > 0)
            {        
                MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);   
            }
            else
            {
                MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_LIST_ID);   
            }
#endif
            g_mms_edit_globle.is_send_gray = FALSE;
            
            HandleSendButtonDisplay(MMIMMS_GetIsMMSMode());
            
            if (!MMITHEME_IsIstyle())
            {
                if (!MMIMMS_CheckIsMMS())
                {
                    MMSShowEditWordCount();

                    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID, MMIMMS_SMS_LABEL_COUNT_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
                }
                else
                {
                    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID, MMIMMS_SMS_LABEL_COUNT_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                }
            }
            
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            if(GetMMSEditIsSubjectExist())  //主题
            {
                MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID);
            }
        }            
        break;
        
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT        
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_HANDLE_T edit_handle = 0;
            uint16 item_num = 0;
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
            if(((MMI_NOTIFY_T*)param)->src_handle == edit_handle)
            {
                if(!MMITHEME_IsIstyle())
                {
                    item_num = GUIEDIT_GetListItemNum(edit_handle);
                    if(item_num > 1)    
                    {
                        MMSShowEditToCount(item_num);
                        GUIFORM_SetChildDisplay(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LABEL_ID, GUIFORM_CHILD_DISP_NORMAL);           
                    }
                    else
                    {
                        BOOLEAN ret = FALSE;
                        ret = GUIFORM_SetChildDisplay(MMIMMS_SMS_LABEL_AND_BUTTON_FORM_ID, MMIMMS_SMS_EDIT_TO_LABEL_ID, GUIFORM_CHILD_DISP_HIDE);     
                        if(ret && MMK_IsFocusWin(win_id))
                        {
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                        }
                    }
                }
                if (!MMITHEME_IsMainScreenLandscape() || !GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
                    MMSUpdateDDLMatchingNumList(win_id);
                }
            }
            else if(((MMI_NOTIFY_T*)param)->src_id == MMIMMS_SMS_EDIT_TEXT_CTRL_ID)
            {
                if (!MMITHEME_IsIstyle())
                {
                    MMSShowEditWordCount();
                }
            }     
            
            HandleSendButtonDisplay(MMIMMS_GetIsMMSMode());
        }
        break;
//for 162615
#if 0
    case MSG_NOTIFY_RESIZE:
        {
            //uint16 im_height = 0;
            MMI_HANDLE_T active_handle = 0;
            MMI_HANDLE_T edit_handle = 0;
            
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
            
            active_handle = MMK_GetActiveCtrl(win_id);
            
            while (0 != active_handle && active_handle != edit_handle)
            {
                active_handle = MMK_GetParentCtrlHandle(active_handle);
            }
            
            //             im_height = ((GUI_RECT_T*)param)->top - ((GUI_RECT_T*)param)->bottom;
            // 
            //             if (edit_handle == active_handle && 0 == im_height)
            if (MMITHEME_IsMainScreenLandscape())
            {
                if (edit_handle == active_handle && !GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    MMSUpdateDDLMatchingNumList(win_id);
                }
                else
                {
                    GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
                }
            }
            else
            {
                if (edit_handle == active_handle && !GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    GUIDROPDOWNLIST_SetPageNum(win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                        MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_EX);
                    MMSUpdateDDLMatchingNumList(win_id);
                }
                else if(edit_handle == active_handle && GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    GUIDROPDOWNLIST_SetPageNum(win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                        MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM);
                    MMSUpdateDDLMatchingNumList(win_id);
                }
                else
                {
                    GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
                }
            }       
        }
        break;
#endif

#endif

    case MSG_LCD_SWITCH:
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT        
        if (MMITHEME_IsMainScreenLandscape())
        {
            GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX_H);
        }
        else
        {
            GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX);
        }
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        if(MMIMMS_GetIsMMSMode())
        {
            GUI_RECT_T  form_rect = {0};
            uint32             additional_file_num  = 0;
            MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
            MMI_STRING_T    edit_str = {0};

            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
            MMIMMS_AddText(edit_doc_ptr, &edit_str);
            
            SCI_MEMCPY(&para,(MMI_MULTI_KEY_TP_MSG_PARAM_T*)param,sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag  || 1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag )
            { 
                GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_EDIT_ANIM_CTRL_ID), &form_rect);
                if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSIMAGE);
                    if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                    {                        
                        MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_PIC);
                    }
                    else
                    {
                        MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_VIDEO);
                    }
                    break;
                }
            }
            
            if(0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
            { 
                GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_EDIT_MUSIC_FORM_ID), &form_rect);
                if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSMUSIC);
                    MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_MUSIC);
                break;
                }                
            }
            
            MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);     
            
            if(0 != additional_file_num)
            { 
                GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID), &form_rect);
                if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
                {          
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSADDFILE);
                    MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_ATTACH);
                }
            }
            
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if 0
    case MSG_NOTIFY_EDITBOX_MAXTEXT:
        if(!MMIMMS_GetIsMMSMode())
        {
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,0);     
        }    
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T     ctrl_id = 0;
            MMI_STRING_T    edit_str = {0};
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch(ctrl_id)
            {
            case MMIMMS_EDIT_BUTTON_LF_CTRL_ID:
                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (cur_slide_index > 1)
                {
                    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
                    MMIMMS_AddText(edit_doc_ptr, &edit_str);
                    slide_err = MMIMMS_PrevSlide(edit_doc_ptr);//切换slide前需要将text文本的内容记录
                    if (MMIMMS_NO_ERROR == slide_err)
                    {
                        SetMMSEditFocusOnAddMenu(FALSE);
                        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                        MMSShowEditContent();   
                        MMSShowMultiFileInfo();  
                        MMSShowAttachFile();
                        MMSShowMMSInfo();     
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                    }
                }
                break;
            case MMIMMS_EDIT_BUTTON_RH_CTRL_ID:
                {
                    cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                    
                    slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                    if (cur_slide_index < slide_num)
                    {
                        GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
                        MMIMMS_AddText(edit_doc_ptr, &edit_str);
                        slide_err = MMIMMS_NextSlide(edit_doc_ptr);
                        if (MMIMMS_NO_ERROR == slide_err)
                        {
                            SetMMSEditFocusOnAddMenu(FALSE);
                            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                            MMSShowEditContent();   
                            MMSShowMultiFileInfo();
                            MMSShowMMSInfo();                         
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);                     
                        }
                    }
                }   
                break;
                
            case MMIMMS_SMS_BUTTON_INSERT_CTRL_ID:
#ifndef MMI_GUI_STYLE_TYPICAL            
                MMK_CreateWin((uint32*)MMIMMS_EDIT_MAINMENU_INSERT_OPT_WIN_TAB, PNULL);   
#endif
                break;
                
            case MMIMMS_SMS_BUTTON_SEND_CTRL_ID:
                if(MMIMMS_GetIsMMSMode()) //彩信发送方式
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SEND);
                    //MMIAPISMS_ClearDestNum();
                    EditAndSendNewMMS();
                }
                else 
                {
                    HandleMMSEditSendSms(win_id);//发送短信的流程
                }
                break;
#ifdef MMI_PDA_SUPPORT
            case MMIMMS_SMS_EDIT_TO_BUTTON_ID:
                SetMMSEditAddContactType(MMIMMS_ADD_TO_CONTACT);     
                if(MMIMMS_GetIsMMSMode())
                {
                    MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                    num = MMIMMS_GetEditNameNum();
                    
                    MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,//lint !e655
                        (uint16)MMIMMS_MAX_RECEIVER_NUM - num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSelectContactCallback
                        );
                    //                 MMIAPIPB_OpenNumberList(
                    //                     MMIPB_LIST_FOR_MMS,
                    //                     MMIPB_LIST_TYPE_MUTIPLE_FORALL,
                    //                     (uint16)MMIMMS_MAX_RECEIVER_NUM - num,
                    //                     PNULL
                    //                     );
                }
                else
                {
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                    {
                        MMI_HANDLE_T edit_handle = 0;
                        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                            win_id,
                            MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                            );
                        MMSGetSendNumber(edit_handle,FALSE);
                        MMSAddSmsNumber(edit_handle);
                    }                
#else
                    MMSGetSendNumber(MMIMMS_SMS_EDIT_TO_LIST_ID,FALSE);
                    MMSAddSmsNumber(MMIMMS_SMS_EDIT_TO_LIST_ID); 
#endif                    
                    /*MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_SMS,
                    MMIPB_LIST_TYPE_MUTIPLE_FORALL,
                    MMISMS_DEST_ADDR_MAX_NUM - MMIAPISMS_GetDestCount(),
                    PNULL
                    );
                    */
                    MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,//lint !e655
                        (uint16)MMISMS_DEST_ADDR_MAX_NUM - num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSelectContactCallback
                        );
                }
                
                break;
#endif
                
#ifdef MMIMMS_SMS_CC_SUPPORT
            case MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID:
                SetMMSEditAddContactType(MMIMMS_ADD_CC_CONTACT);
                MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                num = MMIMMS_GetEditNameNum();
                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER | MMIPB_LIST_FIELD_MAIL,//lint !e655
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSelectContactCallback
                    );
                    /*
                    MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_TYPE_MUTIPLE_FORALL,
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,
                    PNULL
                    );
                */
                break;
                
            case MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID:
                SetMMSEditAddContactType(MMIMMS_ADD_BCC_CONTACT);
                MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                num = MMIMMS_GetEditNameNum();
                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER | MMIPB_LIST_FIELD_MAIL,//lint !e655
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSelectContactCallback
                    );
                    /*
                    MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_TYPE_MUTIPLE_FORALL,
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,
                    PNULL
                    );
                */
                break;
#endif            
            default:        
                break;
        }
    }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_GET_FOCUS:
        {  
            if(MMIMMS_GetIsMMSMode())
            {
                if (MMS_CONTEDIT_OPT_NODE_PREV == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_HeadSlide(edit_doc_ptr); //从第一页开始显示
                    MMSShowEditContent(); 
                    MMSShowMultiFileInfo();
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_NULL);
                }
                
                MMSShowAttachFile();    
                MMSShowMMSInfo();     
            }
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            MMSSetElementVisibility();   
        }        
        break;
        
#ifdef MMI_TIMERMSG_SUPPORT
    case MMISMS_MSG_UPDATE_TIMER:
        if (MMIAPISMS_GetIsTimerMode())
        {
            MMSAddTimer2Label();
        }
        MMSSetTimerMsgDisplay(TRUE);
        break;
#endif
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        //进入选项
        OpenMMSEditOptPopMenu();
        break;
        
    case MSG_PROMPTWIN_OK:
        //关闭窗口，退出彩信编辑，以及删除多媒体pop menu处理
        if(MMIMMS_GetIsMMSMode() )
        {            
            if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE)
            {
                //需要判断是否加帧显示
                BOOLEAN is_need_add_new_slide = FALSE;
                if((1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag || 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                   && !GetMMSEditIsReplaceMultifile()
                    ) 
                {
                    is_need_add_new_slide = TRUE;
                }
                
                if(is_need_add_new_slide)
                {
                    MMSAddNewSlide();  //增加新页显示
                }
                
                if (PNULL != g_mms_edit_globle.file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == g_mms_edit_globle.file_type)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    
                    if(MMIMMS_AddFile(edit_doc_ptr,g_mms_edit_globle.file_info_ptr,g_mms_edit_globle.file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))
                    {                          
                        MMSEditAddPicItem();
                    }
                    else
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                    }
                    
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                }
                MMK_CloseWin(quer_win_id); 
                MMIPUB_CloseQuerytWin(PNULL);
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
                SetMMSEditReplaceMultifileSize(0);
            }
            else if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_CANCEL)
            {
                MMIMMS_ERROR_E   mms_op_result        = MMIMMS_NO_ERROR; 
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMSEditAssembleInfo();
#endif
                mms_op_result = MMIMMS_SaveMMS(MN_DUAL_SYS_1, MMISMS_MO_DRAFT, edit_doc_ptr);
                if (MMIMMS_NO_ERROR != mms_op_result)
                {
                    MMIMMS_EditMMSAlertWin(mms_op_result);
                    MMK_CloseWin(quer_win_id);
                    MMK_CloseWin(win_id);
                    break;
                }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                else
                {
                    if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS == MMIMMS_GetMMSEditEnterReason())
                    {
                        MMSDeleteDraftSMS();            
                    }
                }
#endif
                SetMMSEditIsSaveDraft(TRUE);
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS); 
#endif
                MMK_CloseWin(quer_win_id); 
                MMIMMS_CloseEditWin();
            }           
            else if(TRUE == GetMMSEditIsOpPopMenu())
            {
                BOOLEAN is_mms_mode = FALSE;

                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag && MMS_CONTEDIT_OPT_NODE_DELIMAGE == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelVideo(edit_doc_ptr);
                }
                else if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag && MMS_CONTEDIT_OPT_NODE_DELIMAGE == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelImage(edit_doc_ptr);
                }
                else if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize && MMS_CONTEDIT_OPT_NODE_DELMUSIC == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelMusic(edit_doc_ptr);
                } 
                MMK_CloseWin(quer_win_id); 
                MMIPUB_CloseQuerytWin(PNULL);
                
                is_mms_mode = MMIMMS_CheckIsMMS();  
                if(!is_mms_mode)
                {
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,0); 
                } 
                else
                {
                    HandleSendButtonDisplay(TRUE);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
                }
                
            }      
        }        
        else
        {            
            if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE)
            {                
                if (PNULL != g_mms_edit_globle.file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == g_mms_edit_globle.file_type)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    
                    if(MMIMMS_AddFile(edit_doc_ptr,g_mms_edit_globle.file_info_ptr,g_mms_edit_globle.file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))
                    {                          
                        MMSEditAddPicItem();
                    }
                    else
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                    }
                    
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                }
                MMK_CloseWin(quer_win_id); 
                MMIPUB_CloseQuerytWin(PNULL);
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
                SetMMSEditReplaceMultifileSize(0);
                
                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                    PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);                     
            }
            else
            {
                            // to get the content which the user input
                MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
                
                edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
                
                SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
                GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_string);
                
                // to save the information
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
                if (MMISMS_GetAutoSignatureFlag() && !MMIMMS_GetIsMMSMode())
                {
                    MMI_STRING_T sig_string = {0};
                    MMI_STRING_T display_string = {0};  
                    wchar        signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
                    uint16       signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                    signature_wstr[0] = 0x0a00;
#else
                    signature_wstr[0] = 0x000a;
#endif
                    signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr + 1) + 1;
                    
                    sig_string.wstr_ptr = signature_wstr;
                    sig_string.wstr_len = signature_str_len;
                    
                    display_string.wstr_ptr = (wchar *)SCI_ALLOCAZ( ( MMISMS_MAX_MESSAGE_LEN + 1 ) * sizeof(wchar) );
                    
                    if (PNULL == display_string.wstr_ptr)
                    {
                        break;
                    }
                    
                    MMIAPICOM_MergeTwoString(
                        &edit_string,
                        &sig_string,
                        &display_string,
                        MMISMS_MAX_MESSAGE_LEN
                        );
                    
                    // to save the information
                    MMIAPISMS_SetMessageContent( 
                        display_string.wstr_len,
                        display_string.wstr_ptr, 
                        TRUE, 
                        edit_content_ptr
                        );
                    
                    SCI_FREE(display_string.wstr_ptr);
                }
                else
#endif
                {
                    MMIAPISMS_SetMessageContent(edit_string.wstr_len, edit_string.wstr_ptr, TRUE, edit_content_ptr);                   
                }

                
                MMSSaveSmsDraft(win_id);
                MMIPUB_CloseQuerytWin(PNULL) ;
                MMIMMS_CloseEditWin();   
            }            
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:        

        //回到彩信编辑
        if (PNULL != g_mms_edit_globle.file_info_ptr)
        { 
            SCI_FREE(g_mms_edit_globle.file_info_ptr);
            g_mms_edit_globle.file_info_ptr = PNULL;
        }
        g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
        MMK_CloseWin(quer_win_id);
        MMIPUB_CloseQuerytWin(PNULL);
        if (GetMMSEditLoseFocusReason() != MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE && TRUE != GetMMSEditIsOpPopMenu()) //resize，删除图片
        {
            MMIMMS_CloseEditWin();  
        }
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);

        break;
    case MSG_MULTIM_SELECTED_RETURN:
        multim_return_info_ptr = (MULTIM_SELECT_RETURN_T *)param;     
        //选中多媒体文件返回
        {            
            MMIAPIFMM_GetFileInfoFormFullPath(multim_return_info_ptr->file_name_ptr,multim_return_info_ptr->file_name_len,&file_info);
            real_file_size = file_info.file_size;
            replaceed_size = GetMMSEditReplaceMultifileSize();
            edit_type = GetMMSEditInsertOpType();
            if(MMIMMS_GetIsMMSMode())
            {
                if (GetMMSEditIsReplaceMultifile() && 0 != replaceed_size)
                {
                    insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize - replaceed_size + real_file_size - MMIMMS_SIZE_LEFT;
                }
                else
                {
                    insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize + real_file_size;
                }
            }
            else
            {
                insert_total_size = real_file_size ;
            }
            
            if (0 == real_file_size)
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else if(insert_total_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;
                
                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);
                
                reduce_pic_size = reduce_width*reduce_height*2;
                
                pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                if (   MMIFMM_FILE_TYPE_PICTURE == multim_return_info_ptr->file_type 
                    && GUIANIM_TYPE_IMAGE_JPG == pic_type
                    && real_file_size > reduce_pic_size
                    && insert_total_size - real_file_size + reduce_pic_size <= MMIMMS_MAX_CONTENT_SIZE
                    && MMIMMS_INSERT_ADD_FILE != edit_type
                    )
                {
#ifdef MMIMMS_AUTO_COMPRESS_IMAGE
                    SetMMSEditIsSaveDraft(FALSE);
                    if(!MMIMMS_AddFile(edit_doc_ptr,&file_info,multim_return_info_ptr->file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                    }
                    SetMMSEditReplaceMultifileSize(0);
#else
                    g_mms_edit_globle.file_type = multim_return_info_ptr->file_type;
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOC_APP(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == g_mms_edit_globle.file_info_ptr)
                    {
                        g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        break;
                    }
                    SCI_MEMSET((char *)g_mms_edit_globle.file_info_ptr, 0, (sizeof(MMIFILE_FILE_INFO_T)));
                    SCI_MEMCPY((char *)g_mms_edit_globle.file_info_ptr, (char *)&file_info, sizeof(MMIFILE_FILE_INFO_T));
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC,IMAGE_PUBWIN_QUERY, &quer_win_id,PNULL);
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE);
#endif
                }
                else
                {
                    SetMMSEditReplaceMultifileSize(0);
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
            }
            else
            {    
                SetMMSEditIsSaveDraft(FALSE);
                
                if (!(GetMMSEditIsInsertNormalAttachment() && MMIMMS_INSERT_ADD_FILE == edit_type))
                {  
                    MMIMMS_CONTEDIT_OPT_MENU_NODE_E lose_focus_reason = MMS_CONTEDIT_OPT_NODE_NULL;
                    BOOLEAN is_need_add_new_slide = FALSE;
                    lose_focus_reason= GetMMSEditLoseFocusReason();
                    if(!GetMMSEditIsReplaceMultifile()) //替换的情况不需要添加新帧显示
                    {
                        if(1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)  //是否有视频
                        {
                            is_need_add_new_slide = TRUE;
                        }
                        else
                        {
                            if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                            {
                                if(MMS_CONTEDIT_OPT_NODE_INSMUSIC == lose_focus_reason || MMS_CONTEDIT_OPT_NODE_INSVIDEO == lose_focus_reason)
                                {
                                    is_need_add_new_slide = TRUE;;//本来有音乐，添加音乐或视频需增加新页
                                }
                            }
                            
                            if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                            {
                                if(MMS_CONTEDIT_OPT_NODE_INSIMAGE== lose_focus_reason || MMS_CONTEDIT_OPT_NODE_INSVIDEO == lose_focus_reason)
                                {
                                    is_need_add_new_slide = TRUE;;//本来有图片  ，添加图片或视频需增加新页
                                }
                            }                        
                        }
                        if(is_need_add_new_slide)
                        {
                            MMSAddNewSlide();  //增加新页显示
                        }
                    }
                    
                    
                    if(MMIMMS_AddFile(edit_doc_ptr,&file_info,multim_return_info_ptr->file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))//添加成功，同时更新显示
                    {         
                        if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                        {
                            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);     
                            
                        } 
                        else
                        {
                            HandleSendButtonDisplay(TRUE);
                        }
                        
                        MMSShowMultiFileInfo();                          
                        MMK_SetAtvCtrl( win_id, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID);
                    }                     
                }
                else
                {
                    if (GetMMSEditIsReplaceMultifile())
                    {   
                        MMI_CTRL_ID_T ctrl_id = MMIMMS_ATTACH_LISTBOX_CTRL_ID;
                        uint16 index = 0;
                        const GUILIST_ITEM_T *item_ptr = PNULL;
                        uint8 *add_file_ptr               = PNULL;
                        wchar *file_name = PNULL;
                        uint16 file_name_len = 0;
                        index = GUILIST_GetCurItemIndex(ctrl_id);
                        item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
                        if(PNULL == item_ptr || PNULL == item_ptr->item_data_ptr)
                        {
                            break;
                        }
                        file_name_len =  item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
                        file_name = item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
                        
                        add_file_ptr = SCI_ALLOC_APP((file_name_len + 1)*3);
                        if (PNULL == add_file_ptr)
                        {
                            return MMI_RESULT_FALSE;
                        }
                        SCI_MEMSET(add_file_ptr, 0, ((file_name_len + 1)*3));            
                        GUI_WstrToUTF8((uint8 *)add_file_ptr, (file_name_len + 1)*3, file_name, file_name_len);        
                        
                        if (!MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, &file_info, add_file_ptr, PNULL, 0, MMIFMM_FILE_TYPE_NORMAL, PNULL))
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                            break;
                        }                    
                        
                        MMK_CloseWin(MMIMMS_ATTACH_WIN_ID);
                        SCI_Free(add_file_ptr);
                        add_file_ptr = PNULL;
                    }
                    else
                    {
                        if(MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, &file_info, PNULL, PNULL, 0, MMIFMM_FILE_TYPE_NORMAL, PNULL))
                        {
                            if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                            {
                                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                    PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);   
                            } 
                            else
                            {
                                HandleSendButtonDisplay(TRUE);
                            }
                        }
                        
                    }           
                    MMK_SetAtvCtrl( win_id, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID);                    
                    SetMMSEditIsInsertNormalAttachment(FALSE);
                    {   //Jump to first slide
                        MMI_STRING_T    edit_str = {0};

                        cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                        if (cur_slide_index > 1)
                        {
                            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
                            MMIMMS_AddText(edit_doc_ptr, &edit_str);
                            slide_err = MMIMMS_HeadSlide(edit_doc_ptr);//切换slide前需要将text文本的内容记录
                            if (MMIMMS_NO_ERROR == slide_err)
                            {
                                SetMMSEditFocusOnAddMenu(FALSE);
                                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                                MMSShowEditContent();   
                                MMSShowMultiFileInfo();  
                                MMSShowAttachFile();
                                MMSShowMMSInfo();     
                                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                            }
                        }

                    }
                } 
                SetMMSEditIsReplaceMultifile(FALSE);
                SetMMSEditReplaceMultifileSize(0);
            }
        }
        break;
#if defined MMI_VCARD_SUPPORT
    case MMI_PB_VCARD_SELECTED:
        {            
            vcard_buf_ptr = (uint8 *)SCI_ALLOCA(VCARD_MAX_FILE_LEN*sizeof(uint8));
            if(vcard_buf_ptr == PNULL)
            {
                break;
            }
            SCI_MEMSET(vcard_buf_ptr, 0x00, VCARD_MAX_FILE_LEN*sizeof(uint8));
            MMIAPIPB_GetSelectVcard((MMIPB_HANDLE_T)param, vcard_buf_ptr, VCARD_MAX_FILE_LEN, 0);
            vcard_data_len = strlen((char *)vcard_buf_ptr);
            vacard_name_str.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar));
            if (PNULL == vacard_name_str.wstr_ptr)
            {
                SCI_FREE(vcard_buf_ptr);
                break;
            }
            SCI_MEMSET((char *)vacard_name_str.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar)));
            MMIAPIPB_GetSelectName((uint32)param, &vacard_name_str, MMIMMS_FILE_NAME_LEN - MMIAPICOM_Wstrlen(vacard_suf), 0);
            MMIAPICOM_Wstrncpy(vacard_name_str.wstr_ptr + vacard_name_str.wstr_len, vacard_suf, MMIAPICOM_Wstrlen(vacard_suf));
            vacard_name_str.wstr_len = vacard_name_str.wstr_len + MMIAPICOM_Wstrlen(vacard_suf);
            if(MMIMMS_GetIsMMSMode())
            {
                insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize + vcard_data_len;
            }
            else
            {  
                insert_total_size = real_file_size;
            }
            
            if (0 == vcard_data_len)
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else if(insert_total_size > MMIMMS_MMS_LIMIT_SIZE)
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else
            {                
                SetMMSEditIsSaveDraft(FALSE);
                if (GetMMSEditIsReplaceMultifile())
                {
                    MMI_CTRL_ID_T ctrl_id = MMIMMS_ATTACH_LISTBOX_CTRL_ID;
                    uint16 index = 0;
                    const GUILIST_ITEM_T *item_ptr = PNULL;
                    uint8 *add_file_ptr               = PNULL;
                    wchar *file_name = PNULL;
                    uint16 file_name_len = 0;
                    index = GUILIST_GetCurItemIndex(ctrl_id);
                    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
                    if(PNULL == item_ptr || PNULL == item_ptr->item_data_ptr)
                    {
                        SCI_FREE(vcard_buf_ptr);
                        break;
                    }
                                            
                    file_name_len =  item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
                    file_name = item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
                    add_file_ptr = SCI_ALLOC_APP((file_name_len + 1)*3);
                    if (PNULL != add_file_ptr)
                    {
                        SCI_MEMSET(add_file_ptr, 0, ((file_name_len + 1)*3));            
                        GUI_WstrToUTF8((uint8 *)add_file_ptr, (file_name_len + 1)*3, file_name, file_name_len);        
                        
                        if (!MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, PNULL, add_file_ptr, vcard_buf_ptr, vcard_data_len, MMIFMM_FILE_TYPE_VCARD, &vacard_name_str))
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                        }
                        else
                        {
                            MMK_CloseWin(MMIMMS_ATTACH_WIN_ID);
                            SetMMSEditIsReplaceMultifile(FALSE);
                        }
                        
                        SCI_Free(add_file_ptr);
                        add_file_ptr = PNULL;                                                                    
                    }
                }
                else
                {
                    if (!MMIMMS_AddAndReplaceAttachment(edit_doc_ptr, PNULL, PNULL, vcard_buf_ptr, vcard_data_len, MMIFMM_FILE_TYPE_VCARD, &vacard_name_str))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                    } 
                    else
                    {
                        if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                        {
                            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);  
                        } 
                        else
                        {
                            HandleSendButtonDisplay(TRUE);
                        }
                    }
                    
                    MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID);
                }
                SetMMSEditReplaceMultifileSize(0);
            }
            SCI_FREE(vcard_buf_ptr);
            SCI_FREE(vacard_name_str.wstr_ptr);
        }
        break;
#endif
        
    case MSG_APP_RED:
        //MMK_CloseWin(win_id);
        err_code = MMI_RESULT_FALSE;
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(MMIMMS_GetIsMMSMode())  //彩信退出流程
        {
            //退出编辑主界面
            if (PNULL != edit_doc_ptr)
            {
                content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
                if (0 == content_num
                    &&(PNULL == edit_doc_ptr->edit_subject.wstr_ptr || 0 == edit_doc_ptr->edit_subject.wstr_len) 
                    &&(PNULL == edit_doc_ptr->edit_to.wstr_ptr || 0 == edit_doc_ptr->edit_to.wstr_len)
                    )
                {
                    MMIMMS_CloseEditWin();
                }
                else
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
                    MMIPUB_OpenQueryWinByTextId(TXT_SMS_ASKSAVE_DRAFT,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);               
                }
            }
        }
        else   //短信退出流程
        {
            SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &edit_string);
            
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            {
                MMI_HANDLE_T edit_handle = 0;
                edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                    win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                    );
                if(!(MMSGetSendNumber(edit_handle,FALSE)) &&
                    (0 == edit_string.wstr_len))
                {
                    // close the edit window
                    MMIMMS_CloseEditWin();			
                }
                else
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_SMS_ASKSAVE_DRAFT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                }
            }               
#else
            if(!(MMSGetSendNumber(MMIMMS_SMS_EDIT_TO_LIST_ID,FALSE)) &&
                (0 == edit_string.wstr_len))
            {
                // close the edit window
                MMK_CloseWin(win_id);			
            }
            else
            {
                MMIPUB_OpenQueryWinByTextId(TXT_SMS_ASKSAVE_DRAFT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
#endif
        }
        
        break;
    case MSG_CLOSE_WINDOW:        
        MMIMMS_MMSEditExit();
        if(MMIMMS_GetIsMMSMode()) 
        {
            //退出编辑，释放全局变量
            if (PNULL != g_mms_edit_globle.file_info_ptr)
            {
                SCI_FREE(g_mms_edit_globle.file_info_ptr);
                g_mms_edit_globle.file_info_ptr = PNULL;
            }
            
            g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
            SetMMSEditIsInsertNormalAttachment(FALSE);
            MMIMS_SetAnswerSim(MN_DUAL_SYS_MAX);
            SetMMSEditIsSaveDraft(FALSE);
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            SetMMSEditLoseFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
            
#ifdef MMI_TIMERMSG_SUPPORT
            MMIAPISMS_SetIsTimerMode(FALSE);
#endif
            MMIMMS_WinExit(EDIT_OPTION_WIN_ENTRY);
            MMIMMS_SetIsMMSMode(FALSE);
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
            MMIAPISMS_SetIsAnwserSMS(FALSE);
#endif		
        }        
        else
        {
            if(!MMIAPISMS_GetNormalClose() )
            {			            
                // to get the content which the user input
                MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
                uint16    edit_to_count = 0;

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                {
                MMI_HANDLE_T edit_handle = 0;
                edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                        win_id,
                                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                        );
                edit_to_count = GUIEDIT_GetListItemNum(edit_handle);
                }                
#else
                edit_to_count = GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif

                edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
            
                SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
                GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_string);
                if(edit_to_count > 0 && 0 != edit_string.wstr_len)
                {                    
                    // to save the information
        #if 0
                    MMIAPISMS_SetGloblalMsgContent( 
                        edit_string.wstr_len,
                        edit_string.wstr_ptr
                        );
        #endif
                    MMIAPISMS_SetMessageContent(edit_string.wstr_len, edit_string.wstr_ptr, TRUE, edit_content_ptr);
                    MMIAPISMS_ClearDestNum();
                    MMSRedkeySaveSmsDraft();
        
                }    
            }
            MMIAPISMS_SetNormalClose(FALSE);
#ifdef MMI_TIMERMSG_SUPPORT
            //MMIAPISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_NOSEND);
            //MMIAPISMS_SetIsTimerMode(FALSE);
#endif
            MMIMMS_SetIsMMSMode(FALSE);
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
            MMIAPISMS_SetIsAnwserSMS(FALSE);
#endif
        }
        if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
        {
            MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
        }
        
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
#ifdef MMI_TIMERMSG_SUPPORT    
        if(MMISMS_GetIsTimerMode())
        {
            GUI_RECT_T  form_rect = {0};
            MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
            SCI_MEMCPY(&para,(MMI_MULTI_KEY_TP_MSG_PARAM_T*)param,sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_TIMER_SMS_FORM_ID), &form_rect);
            if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
            {
                MMISMS_EnterTimerMsgMainWin();
            }
        }        
#endif        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MMI_SELECT_CONTACT_CNF:
        {
            uint16 pb_select_num = 0;
            MMI_STRING_T email_addr = {0};
            uint16 pb_select_index = 0;
            MMIMMS_EDIT_ADD_CONTACT_TYPE add_contact_type = MMIMMS_ADD_CONTACT_OP_MAX;
            add_contact_type = GetMMSEditAddContactType();
            
            if(MMIMMS_ADD_CONTACT_OP_MAX == add_contact_type) 
            {
                //SCI_TRACE_LOW:"mms add contact type max: return"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_4619_112_2_18_2_37_48_41,(uint8*)"");
                break;
            }
            
            if(MMIMMS_ADD_CONTENT_CONTACT == add_contact_type) //是彩信正文添加联系人
            {
                //AddContactInfo2Editbox();
                MMSAddContact2Content((uint32)param);
                SetMMSEditAddContactType(MMIMMS_ADD_CONTACT_OP_MAX);
            }
            else
            {
                uint16 item_num = 0;
                pb_select_num = MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param);
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT 
                {
                    MMI_HANDLE_T edit_handle = 0;
                    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                        win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                        );
                    item_num = GUIEDIT_GetListItemNum(edit_handle);
                }              
                
#else
                item_num = GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif                
                if(item_num + pb_select_num > MMIMMS_MAX_RECEIVER_NUM)  //暂时短彩都是30条，待控件支持
                {
                    MMSMMI_OpenMMSAlertWin(TXT_SMS_EXCEED_MAX_NUM, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    break;
                }
                if (!MMIMMS_GetIsMMSMode())//当前模式为短信，查看是否要转换为彩信
                {
                    SCI_MEMSET(&email_addr, 0, sizeof(email_addr));
                    email_addr.wstr_ptr = (wchar *)SCI_ALLOCAZ((MMIPB_MAX_MAIL_LEN + 1)*sizeof(wchar));
                    if (PNULL == email_addr.wstr_ptr)
                    {
                        break;
                    }
                    for (pb_select_index = 0; pb_select_index < MIN(MMIMMS_MAX_RECEIVER_NUM, pb_select_num); pb_select_index++)
                    {
                        SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
                        MMIAPIPB_GetSelectEmail((uint32)param,&email_addr, MMIPB_MAX_MAIL_LEN, pb_select_index);
                        if(email_addr.wstr_len!=0)//所添加的联系人有邮箱地址时转化为彩信
                        {
                            MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleConvertSmsToMmsWaitWinMsg,(uint32)0);
                            MMIMMS_SetIsMMSMode(TRUE);
                            MMIMMS_SetMMSEditIsEmailAddrExist(TRUE);
                            break;
                        }
                    }
                    SCI_FREE(email_addr.wstr_ptr);
                }
                
                if(MMIMMS_GetIsMMSMode())//彩信模式
                {
                    MMSAddPbSelectedContacts(pb_select_num, edit_doc_ptr, (uint32)param, win_id);
                }
                else
                {
                    // from PHONEBOOK, call the API to get the phone number.
                    MMSGetNumberFromPb((uint32)param, msg_id);
                    // update the edit box
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                    {
                        MMI_HANDLE_T edit_handle = 0;
                        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                            win_id,
                            MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                            );
                        MMSAddSmsNumber(edit_handle);
                        MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);//控件已替换成dropdownlist
                    }                
#else
                    MMSAddSmsNumber(MMIMMS_SMS_EDIT_TO_LIST_ID);
                    MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif
                } 
            }
        }
        break;

#ifdef MMI_PDA_SUPPORT
    case MMI_CL_MULTI_SELECT_LIST_RETURN:
        MmsHandleCallogSelectReturn(edit_doc_ptr, win_id, MMI_CL_MULTI_SELECT_LIST_RETURN);
        break;
#endif
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}
#else
/*****************************************************************************/
//  Description :mms set form disp bg
//  Global resource dependence : dropdown内嵌edit比较特殊，需单独设背景
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetEditDispBG(MMIMMS_WINDOW_ID_E win_id,GUI_BG_TYPE_E bg_type)
{
    //GUI_BG_T focus_bg = {0};
    GUI_BG_T bg = {0};
    MMI_HANDLE_T edit_handle = 0;                    
    
    bg.bg_type = bg_type;
    s_edit_to_bg_type = bg_type;
    if(bg_type == GUI_BG_COLOR)
    {
        bg.color = GUI_RGB2RGB565(243, 255, 211);  
    }
    #ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    GUIAPICTRL_SetBg(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,&bg);
    
    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
    win_id,
    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
    );
    
    GUIAPICTRL_SetBg(edit_handle,&bg);
    #else
    GUIAPICTRL_SetBg(MMIMMS_SMS_EDIT_TO_LIST_ID,&bg);
    #endif
   
}

/*****************************************************************************/
//  Description :mms set form disp bg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetFormDispBG(void)
{
    GUI_BG_T focus_bg = {0};
    
    focus_bg.bg_type = GUI_BG_COLOR;
    focus_bg.color = GUI_RGB2RGB565(243, 255, 211);    
    GUIFORM_SetActiveChildTheme(MMIMMS_SMS_EDIT_FORM_ID,&focus_bg,PNULL,PNULL);
        
    focus_bg.color = MMI_WHITE_COLOR;
    GUIFORM_SetChildTheme(MMIMMS_SMS_EDIT_FORM_ID,&focus_bg,PNULL,PNULL);
    GUIFORM_PermitChildBg(MMIMMS_SMS_EDIT_TITLE_FORM_ID,FALSE);    
}   

/*****************************************************************************/
//  Description :mms edit menu handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSEditMainMenuWin(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              )
{
    MMI_RESULT_E err_code                          = MMI_RESULT_TRUE;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    MMIFILE_FILE_INFO_T  file_info                 = {0};
    uint16 content_num                             = 0;
    MULTIM_SELECT_RETURN_T *multim_return_info_ptr = PNULL;
    uint16 cur_slide_index                         = 0;
    uint16 slide_num                               = 0;
    MMIMMS_ERROR_E   slide_err                     = MMIMMS_NO_ERROR;
    uint32 insert_total_size                       = 0;
    uint32 real_file_size                          = 0;
    uint32 replaceed_size                          = 0;
    GUIANIM_TYPE_E    pic_type                     = GUIANIM_TYPE_NONE;
    uint16 num = 0;
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMI_STRING_T edit_string = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    MMIMMS_EDIT_INS_OP_TYPE edit_type = MMIMMS_OP_MAX;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_4983_112_2_18_2_37_48_47,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint16 pic_width = 0;
            uint16 pic_height = 0;
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            GUIDROPDOWNLIST_TYPE_T edit_to_dropdownlist = {GUIDNLIST_EDITABLE_NUMLIST,
                MMIMMS_MAX_ADDRESS_LEN,
                MMIMMS_MAX_RECEIVER_NUM};
#endif          
#ifdef MMI_PDA_SUPPORT
            child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            child_width.add_data = MMIMMS_EDIT_CONTACT_ICON_WIDTH;
            
            GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_BUTTON_ID,&child_width);
#endif
          
            MMIAPISMS_SetNormalClose(FALSE);
            
            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
            GUIFORM_SetChildHeight(MMIMMS_SMS_EDIT_FORM_ID,MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&child_height);
            
            GUITEXT_SetHandleTpMsg(
                FALSE,  //in:
                MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID    //in:
                ); 
            MMIMMS_GetPicDefaultWidthAndHeight(&pic_width, &pic_height);
            child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            child_height.add_data = pic_height;
            GUIFORM_SetChildHeight(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,MMIMMS_SMS_EDIT_ANIM_CTRL_ID,&child_height);    
            
#ifdef MMI_TIMERMSG_SUPPORT
            //设置收件人图标和号码框的比例
            child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
            child_width.add_data = 15;        
            GUIFORM_SetChildWidth(MMIMMS_SMS_TIMER_SMS_FORM_ID,MMIMMS_SMS_EDIT_TIMER_CTRL_ID,&child_width);
            GUILABEL_SetIcon(MMIMMS_SMS_EDIT_TIMER_CTRL_ID, IMAGE_SMS_TIMER_ICON);
            MMSSetTimerMsgStartParam();
            MMSSetTimerMsgDisplay(FALSE);
#endif
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
            GUIEDIT_SetFont(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, MMISMS_GetWinFont());
#endif
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            GUIDROPDOWNLIST_SetOpenListMethod(MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, FALSE, FALSE);
            GUIFORM_SetChildDock(MMIMMS_SMS_EDIT_FORM_ID, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID, GUIFORM_CHILD_DOCK_TOP);
            if (MMITHEME_IsMainScreenLandscape())
            {
                GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX_H);
                GUIDROPDOWNLIST_SetPageNum(win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                    MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_H);
            }
            else
            {
                GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX);
                GUIDROPDOWNLIST_SetPageNum(win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                    MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM);
            }
            GUIDROPDOWNLIST_SetCtrlType(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                (const GUIDROPDOWNLIST_TYPE_T *)(&edit_to_dropdownlist)
                );
#endif								          
            MMSSetFormDispBG();
            MMSContactButtonInit();         //to ,cc ,bcc的button显示      
#ifndef MMI_GUI_STYLE_TYPICAL              
            MMSEditWinLFRHButtonInit();    
#endif
#ifdef TOUCH_PANEL_SUPPORT
            MMSEditSetEditDefaultString(win_id);
#endif
            if(MMIMMS_GetIsMMSMode())
            {
			    MMIMMS_WinInit(EDIT_OPTION_WIN_ENTRY);   
                MMIMMS_SlideInit();
                MMSSubjectInit();        
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                {
                    MMI_HANDLE_T edit_handle = 0;
                    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                        win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                        );
                    //GUIEDIT_SetListStyle(edit_handle, GUIEDIT_LIST_STYLE_ALL);
					GUIEDIT_SetSoftkey(edit_handle, 1, 0, STXT_MAIN_PB, STXT_MAIN_PB, PNULL);
                }
#else
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
                GUIEDIT_SetSoftkey(MMIMMS_SMS_EDIT_TO_LIST_ID, 1, 0, STXT_MAIN_PB, STXT_MAIN_PB, PNULL);
#endif//这个地方要不要改?控件属性可能不一样了
                GUIEDIT_SetSmsEncode(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,FALSE);
                GUIEDIT_SetMaxLen(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,MMIMMS_MAX_TEXT_LEN/sizeof(wchar));
      
                MMSShowContactInfo();             
                MMSShowEditContent(); 
                MMSShowMMSInfo();
                MMSShowMultiFileInfo();
                MMSShowAttachFile();  
            }
            else
            {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
                if (MMISMS_GetAutoSignatureFlag() &&        
                    MMISMS_BOX_SENDSUCC != MMISMS_GetCurBoxType() && 
                    MMISMS_BOX_SENDFAIL != MMISMS_GetCurBoxType())
                {
                    wchar  signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
                    uint16 signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
                    
                    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                        MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, 
                        GUIFORM_CHILD_DISP_NORMAL); 
                    
                    GUIEDIT_PermitBorder(MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, TRUE);
                    //set edit height
                    //child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
                    //GUIFORM_SetChildHeight(MMIMMS_SMS_SINATURE_FORM_ID, MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, &child_height);
                    
                    signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr);
                    
                    if (MMISMS_BOX_SENDSUCC != MMISMS_GetCurBoxType() && MMISMS_BOX_SENDFAIL != MMISMS_GetCurBoxType())
                    {
                        GUIEDIT_SetString(MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, signature_wstr, signature_str_len);
                    }                              
                }
                else
                {
                    GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_SUBFORM_CONTENT_AREA_FORM_ID, 
                        MMIMMS_SMS_DISPLAY_SINATURE_CTRL_ID, 
                        GUIFORM_CHILD_DISP_HIDE);
                }
#endif
#endif
                
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                {
                    MMI_HANDLE_T edit_handle = 0;
                    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                        win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                        );
                    //GUIEDIT_SetListStyle(edit_handle, GUIEDIT_LIST_STYLE_PHONENUM);
                    //更改输入法配置，要跟需求确认默认输入法
                    //GUIEDIT_SetIm(edit_handle, GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ENGLISH);//lint !e655
                    MMSAddSmsNumber(edit_handle);
                    
                    GUIEDIT_SetSoftkey(edit_handle, 1, 0, STXT_MAIN_PB, STXT_MAIN_PB, PNULL);
                }
#else
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_TO_LIST_ID, GUIEDIT_LIST_STYLE_PHONENUM);                       
                MMSAddSmsNumber(MMIMMS_SMS_EDIT_TO_LIST_ID);
                
                GUIEDIT_SetSoftkey(MMIMMS_SMS_EDIT_TO_LIST_ID, 1, 0, STXT_MAIN_PB, STXT_MAIN_PB, PNULL);
#endif                    
            }
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            {
                MMI_HANDLE_T edit_handle = 0;
                edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                    win_id,
                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                    );
                if(GUIEDIT_GetListItemNum(edit_handle) > 0)
                {        
                    MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);   
                }
                else
                {
                    MMSSetEditDispBG(win_id,GUI_BG_COLOR);
                    MMK_SetAtvCtrl(win_id, edit_handle);   
                }
            }   
#else
            if(GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID) > 0)
            {        
                MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);   
            }
            else
            {
                MMSSetEditDispBG(win_id,GUI_BG_COLOR);
                MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_LIST_ID);   
            }
#endif
            GUIEDIT_SetSoftkey(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, 1, 0, TXT_SEND, TXT_NULL, PNULL);
            IGUICTRL_SetCircularHandleLeftRight(MMK_GetCtrlPtr(MMIMMS_SMS_EDIT_TEXT_CTRL_ID), FALSE);
            GUIEDIT_SetSoftkey(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, 1, 0, TXT_SEND, TXT_NULL, PNULL);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }            
        break;
        
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            MMI_HANDLE_T active_handle = 0;
            MMI_HANDLE_T edit_handle = 0;
            
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
            
            active_handle = MMK_GetActiveCtrl(win_id);
            
            if (MMK_GetCtrlHandleByWin(win_id, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID) == active_handle)
            {

                if(s_edit_to_bg_type != GUI_BG_COLOR)
                {
					GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_MAIN_PB, MIDDLE_BUTTON, FALSE);  
                    MMSSetEditDispBG(win_id,GUI_BG_COLOR);
                    if (MMK_IsFocusWin(win_id))
                    {
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                    }
                }
				else
				{
                    GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_MAIN_PB, MIDDLE_BUTTON, TRUE);   
                }
            }
            else
            {
                while (0 != active_handle && active_handle != edit_handle)
                {
                    active_handle = MMK_GetParentCtrlHandle(active_handle);
                }
                if (edit_handle == active_handle)
                {                    
					if(s_edit_to_bg_type != GUI_BG_COLOR)
                    {
                            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_MAIN_PB, MIDDLE_BUTTON, FALSE);
                            MMSSetEditDispBG(win_id,GUI_BG_COLOR);
                            if (MMK_IsFocusWin(win_id))
                            {
    					        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                            }
                    }
                    else
                    {
                            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_MAIN_PB, MIDDLE_BUTTON, TRUE);  
                    }
                }
                else
                {
                    if(s_edit_to_bg_type != GUI_BG_NONE)
					{                    
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_SEND, MIDDLE_BUTTON, FALSE);     
                        MMSSetEditDispBG(win_id,GUI_BG_NONE);
                        if (MMK_IsFocusWin(win_id))
                        {
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);                    
                        }
                    }
					else
					{
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_SEND, MIDDLE_BUTTON, TRUE);  
				    }
                }
            }
        }
        break;
#endif		
        
        
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT        
    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_HANDLE_T edit_handle = 0;
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
            if(((MMI_NOTIFY_T*)param)->src_handle == edit_handle)
            {
                if (!MMITHEME_IsMainScreenLandscape() || !GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
                    MMSUpdateDDLMatchingNumList(win_id);
                }
            }
        }
        break;
        
    case MSG_NOTIFY_RESIZE:
        {
            //uint16 im_height = 0;
            MMI_HANDLE_T active_handle = 0;
            MMI_HANDLE_T edit_handle = 0;
            
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
            
            active_handle = MMK_GetActiveCtrl(win_id);
            
            while (0 != active_handle && active_handle != edit_handle)
            {
                active_handle = MMK_GetParentCtrlHandle(active_handle);
            }
            
            //             im_height = ((GUI_RECT_T*)param)->top - ((GUI_RECT_T*)param)->bottom;
            // 
            //             if (edit_handle == active_handle && 0 == im_height)
            if (MMITHEME_IsMainScreenLandscape())
            {
                if (edit_handle == active_handle && !GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    MMSUpdateDDLMatchingNumList(win_id);
                }
                else
                {
                    GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
                }
            }
            else
            {
                if (edit_handle == active_handle && !GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    GUIDROPDOWNLIST_SetPageNum(win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                        MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_EX);
                    MMSUpdateDDLMatchingNumList(win_id);
                }
                else if(edit_handle == active_handle && GUIEDIT_IsTPPanelVisible(edit_handle))
                {
                    GUIDROPDOWNLIST_SetPageNum(win_id,
                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID,
                        MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM);
                    MMSUpdateDDLMatchingNumList(win_id);
                }
                else
                {
                    GUIDROPDOWNLIST_CloseList(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);
                }
            }       
        }
        break;
        
#endif     
    case MSG_LCD_SWITCH:
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT        
        if (MMITHEME_IsMainScreenLandscape())
        {
            GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX_H);
        }
        else
        {
            GUIFORM_SetSizeRange(MMIMMS_SMS_EDIT_TO_LIST_FORM_ID,MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID, MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN, MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX);
        }
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        if(MMIMMS_GetIsMMSMode())
        {
            GUI_RECT_T  form_rect = {0};
            uint32             additional_file_num  = 0;
            MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
            MMI_STRING_T    edit_str = {0};

            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
            MMIMMS_AddText(edit_doc_ptr, &edit_str);
            
            SCI_MEMCPY(&para,(MMI_MULTI_KEY_TP_MSG_PARAM_T*)param,sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag  || 1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag )
            { 
                GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_EDIT_ANIM_CTRL_ID), &form_rect);
                if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSIMAGE);
                    if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                    {                        
                        MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_PIC);
                    }
                    else
                    {
                        MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_VIDEO);
                    }
                    break;
                }
            }
            
            if(0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
            { 
                GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_EDIT_MUSIC_FORM_ID), &form_rect);
                if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSMUSIC);
                    MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_MUSIC);
                break;
            }
            
            }
            
            MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);     
            
            if(0 != additional_file_num)
            { 
                GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID), &form_rect);
                if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
                {          
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSADDFILE);
                    MMSOpenEditMultiOptPopMenu(MMIMMS_TYPE_ATTACH);
                }
            }
            
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if 0
    case MSG_NOTIFY_EDITBOX_MAXTEXT:
        if(!MMIMMS_GetIsMMSMode())
        {
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,0);     
        }    
        break;
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T     ctrl_id = 0;
            MMI_STRING_T    edit_str = {0};
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch(ctrl_id)
            {
            case MMIMMS_EDIT_BUTTON_LF_CTRL_ID:
                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (cur_slide_index > 1)
                {
                    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
                    MMIMMS_AddText(edit_doc_ptr, &edit_str);
                    slide_err = MMIMMS_PrevSlide(edit_doc_ptr);//切换slide前需要将text文本的内容记录
                    if (MMIMMS_NO_ERROR == slide_err)
                    {
                        SetMMSEditFocusOnAddMenu(FALSE);
                        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                        MMSShowEditContent();   
                        MMSShowMultiFileInfo();  
                        MMSShowAttachFile();
                        MMSShowMMSInfo();     
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                    }
                }
                break;
            case MMIMMS_EDIT_BUTTON_RH_CTRL_ID:
                {
                    cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                    
                    slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                    if (cur_slide_index < slide_num)
                    {
                        GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
                        MMIMMS_AddText(edit_doc_ptr, &edit_str);
                        slide_err = MMIMMS_NextSlide(edit_doc_ptr);
                        if (MMIMMS_NO_ERROR == slide_err)
                        {
                            SetMMSEditFocusOnAddMenu(FALSE);
                            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                            MMSShowEditContent();   
                            MMSShowMultiFileInfo();
                            MMSShowAttachFile();
                            MMSShowMMSInfo();                         
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0);                     
                        }
                    }
                }   
                break;
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID: 
                if(MMIMMS_GetIsMMSMode()) 
                {
                    MMI_STRING_T text_str_info = {0};
                    edit_doc_ptr = g_mms_edit_globle.editdoc;
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_PREV);
                    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&text_str_info); //加入当前的文本信息
                    MMIMMS_AddText(edit_doc_ptr, &text_str_info);
                    MMIMMS_CreateMMSPreviewWin(edit_doc_ptr, MMS_PREVIEW);
                }
                else//插入范本
                {
                    MMIAPISMS_CreateSelectWordWin(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
                    MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
                }
                break;
                
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
#ifndef MMI_GUI_STYLE_TYPICAL
                MMK_CreateWin((uint32*)MMIMMS_EDIT_MAINMENU_INSERT_OPT_WIN_TAB, PNULL);
#endif
                break;
                
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                if(MMIMMS_GetIsMMSMode()) //彩信发送方式
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SEND);
                    //MMIAPISMS_ClearDestNum();
                    EditAndSendNewMMS();
                }
                else 
                {
                    HandleMMSEditSendSms(win_id);//发送短信的流程
                }
                break;
                
#ifdef MMIMMS_SMS_CC_SUPPORT
            case MMIMMS_SMS_EDIT_CC_TO_BUTTON_ID:
                SetMMSEditAddContactType(MMIMMS_ADD_CC_CONTACT);
                MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                num = MMIMMS_GetEditNameNum();
                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER | MMIPB_LIST_FIELD_MAIL,//lint !e655
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSelectContactCallback
                    );
                    /*
                    MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_TYPE_MUTIPLE_FORALL,
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,
                    PNULL
                    );
                */
                break;
                
            case MMIMMS_SMS_EDIT_BCC_TO_BUTTON_ID:
                SetMMSEditAddContactType(MMIMMS_ADD_BCC_CONTACT);
                MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                num = MMIMMS_GetEditNameNum();
                MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_CHECK, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_NUMBER | MMIPB_LIST_FIELD_MAIL,//lint !e655
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSelectContactCallback
                    );
                    /*
                    MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_TYPE_MUTIPLE_FORALL,
                    (uint16)MMIMMS_MAX_RECEIVER_NUM - num,
                    PNULL
                    );
                */
                break;
#endif            
            default:        
                break;
        }
    }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
    case MSG_GET_FOCUS:
        {  
            if(MMIMMS_GetIsMMSMode())
            {
                if (MMS_CONTEDIT_OPT_NODE_PREV == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_HeadSlide(edit_doc_ptr); //从第一页开始显示
                    MMSShowEditContent(); 
                    MMSShowMultiFileInfo();
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_NULL);
                }
                
                MMSShowAttachFile();    
                MMSShowMMSInfo();     
            }
        }
        break;
        
    case MSG_FULL_PAINT:
        {
            MMSSetElementVisibility();   
#ifdef TOUCH_PANEL_SUPPORT            
            GUIFORM_ForceLayout(MMIMMS_SMS_EDIT_FORM_ID);
#endif
        }        
        break;
        
#ifdef MMI_TIMERMSG_SUPPORT
    case MMISMS_MSG_UPDATE_TIMER:
        if (MMIAPISMS_GetIsTimerMode())
        {
            MMSAddTimer2Label();
        }
        MMSSetTimerMsgDisplay(TRUE);
        break;
#endif
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMI_HANDLE_T active_handle = 0;
            MMI_HANDLE_T edit_handle = 0;
            BOOLEAN open_pb = FALSE;            
            uint16              item_num = 0;
            MMI_STRING_T  str = {0};
        
        #ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                            win_id,
                                            MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                            );
        #else
            edit_handle = MMIMMS_SMS_EDIT_TO_LIST_ID;
        #endif
            item_num = GUIEDIT_GetListItemNum(edit_handle);
            GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &str);
            
            active_handle = MMK_GetActiveCtrl(win_id);
            
            if (MMK_GetCtrlHandleByWin(win_id, MMIMMS_SMS_EDIT_TO_LIST_FORM_ID) == active_handle)
            {
                open_pb = TRUE;   
            }
            else
            {
                while (0 != active_handle && active_handle != edit_handle)
                {
                    active_handle = MMK_GetParentCtrlHandle(active_handle);
                }
                if (edit_handle == active_handle)
                {
                    open_pb = TRUE;   
                }
            }            
            if (open_pb)
            {
                SetMMSEditAddContactType(MMIMMS_ADD_TO_CONTACT);     
                if(MMIMMS_GetIsMMSMode())
                {
                    MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                    num = MMIMMS_GetEditNameNum();
                    
                    MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,/*lint !e655*/   
                        (uint16)MMIMMS_MAX_RECEIVER_NUM - num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSelectContactCallback
                        );
                }
                else
                {
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                    MMSGetSendNumber(edit_handle,FALSE);
                    MMSAddSmsNumber(edit_handle);
#else
                    MMSGetSendNumber(MMIMMS_SMS_EDIT_TO_LIST_ID,FALSE);
                    MMSAddSmsNumber(MMIMMS_SMS_EDIT_TO_LIST_ID); 
#endif                    
                    MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,/*lint !e655*/ 
                        (uint16)MMISMS_DEST_ADDR_MAX_NUM - num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSelectContactCallback
                        );
                }
            }
            else //发送
            {            
                if(MMIMMS_GetIsMMSMode()) //彩信发送方式
                {
                    if(0 == item_num)
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_RECEIVER_NONE, TXT_NULL, IMAGE_PUBWIN_WARNING); 
                    }
                    else if(0 == MMIMMS_GetAllContentCount(edit_doc_ptr) && 0 == str.wstr_len)
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_CONTENT_EMPTY, TXT_NULL, IMAGE_PUBWIN_WARNING); 
                    }
                    else
                    {
                        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SEND);
                        //MMIAPISMS_ClearDestNum();
                        EditAndSendNewMMS();
                    }
                }
                else 
                {
                    if(0 == item_num)
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_RECEIVER_NONE, TXT_NULL, IMAGE_PUBWIN_WARNING);  
                    }
                    else if(0 == str.wstr_len)
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_CONTENT_EMPTY, TXT_NULL, IMAGE_PUBWIN_WARNING);  
                    }
                    else 
                    {
                        HandleMMSEditSendSms(win_id);//发送短信的流程
                    }
                }
            }
        }
        
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        //进入选项
        OpenMMSEditOptPopMenu();
        break;
        
    case MSG_PROMPTWIN_OK:
        //关闭窗口，退出彩信编辑，以及删除多媒体pop menu处理
        if(MMIMMS_GetIsMMSMode() )
        { 
            MMK_CloseWin(quer_win_id); 
            MMIPUB_CloseQuerytWin(PNULL) ;
            if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE)
            {
                //需要判断是否加帧显示
                BOOLEAN is_need_add_new_slide = FALSE;
                if((1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag || 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                    && !GetMMSEditIsReplaceMultifile()
                    )
                {
                    is_need_add_new_slide = TRUE;
                }
                
                if(is_need_add_new_slide)
                {
                    MMSAddNewSlide();  //增加新页显示
                }
                
                if (PNULL != g_mms_edit_globle.file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == g_mms_edit_globle.file_type)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    
                    if(MMIMMS_AddFile(edit_doc_ptr,g_mms_edit_globle.file_info_ptr,g_mms_edit_globle.file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))
                    {                          
                        MMSEditAddPicItem();
                    }
                    else
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                    }
                    
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                }
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
                SetMMSEditReplaceMultifileSize(0);

                MMSShowMMSInfo();
                MMSShowMultiFileInfo();
                
            }
            else if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_CANCEL)
            {
                MMIMMS_ERROR_E   mms_op_result        = MMIMMS_NO_ERROR; 
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMSEditAssembleInfo();
#endif
                mms_op_result = MMIMMS_SaveMMS(MN_DUAL_SYS_1, MMISMS_MO_DRAFT, edit_doc_ptr);
                if (MMIMMS_NO_ERROR != mms_op_result)
                {
                    MMIMMS_EditMMSAlertWin(mms_op_result);
                    MMIMMS_CloseEditWin();
                    break;
                }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                else
                {
                    if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS == MMIMMS_GetMMSEditEnterReason())
                    {
                        MMSDeleteDraftSMS();            
                    }
                }
#endif
                SetMMSEditIsSaveDraft(TRUE);
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS); 
#endif
                MMIMMS_CloseEditWin();	
                MMK_CloseWin(quer_win_id); 
            }           
            else if(TRUE == GetMMSEditIsOpPopMenu())
            {
                BOOLEAN is_mms_mode = FALSE;
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag && MMS_CONTEDIT_OPT_NODE_DELIMAGE == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelVideo(edit_doc_ptr);
                }
                else if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag && MMS_CONTEDIT_OPT_NODE_DELIMAGE == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelImage(edit_doc_ptr);
                }
                else if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize && MMS_CONTEDIT_OPT_NODE_DELMUSIC == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelMusic(edit_doc_ptr);
                }               
                
                is_mms_mode = MMIMMS_CheckIsMMS();  
                if(!is_mms_mode)
                {
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,0); 
                }
#ifdef MMI_GUI_STYLE_TYPICAL
                else
                {
                    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);   
                }
#endif                
            }      
        }        
        else
        {            
            if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE)
            {
                MMK_CloseWin(quer_win_id); 
                
                if (PNULL != g_mms_edit_globle.file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == g_mms_edit_globle.file_type)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    
                    if(MMIMMS_AddFile(edit_doc_ptr,g_mms_edit_globle.file_info_ptr,g_mms_edit_globle.file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))
                    {                          
                        MMSEditAddPicItem();
                    }
                    else
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                    }
                    
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                }
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
                SetMMSEditReplaceMultifileSize(0);
                
                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                    PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);                     
            }
            else
            {   			
			    // to get the content which the user input
                MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
                
                edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
                
                SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
                GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_string);
                
                // to save the information

                MMIAPISMS_SetMessageContent(edit_string.wstr_len, edit_string.wstr_ptr, TRUE, edit_content_ptr);     
                MMSSaveSmsDraft(win_id);
                MMIPUB_CloseQuerytWin(PNULL) ;
                MMIMMS_CloseEditWin();	   
            }            

        }
        break;
		
		
		case MSG_PROMPTWIN_CANCEL:        
        if(MMIMMS_GetIsMMSMode() )
        {
            //回到彩信编辑
            if (PNULL != g_mms_edit_globle.file_info_ptr)
            { 
                SCI_FREE(g_mms_edit_globle.file_info_ptr);
                g_mms_edit_globle.file_info_ptr = PNULL;
            }
            g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
            MMK_CloseWin(quer_win_id);
			if (GetMMSEditLoseFocusReason() != MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE && TRUE != GetMMSEditIsOpPopMenu()) //resize，删除图片
            {
                MMIMMS_CloseEditWin();
			}
        }
        else
        {            
            MMIPUB_CloseQuerytWin(PNULL);
            MMIMMS_CloseEditWin();
        }
        break;
    case MSG_MULTIM_SELECTED_RETURN:
        multim_return_info_ptr = (MULTIM_SELECT_RETURN_T *)param;     
        //选中多媒体文件返回
        {            
            MMIAPIFMM_GetFileInfoFormFullPath(multim_return_info_ptr->file_name_ptr,multim_return_info_ptr->file_name_len,&file_info);
            real_file_size = file_info.file_size;
            replaceed_size = GetMMSEditReplaceMultifileSize();
            edit_type = GetMMSEditInsertOpType();
            if(MMIMMS_GetIsMMSMode())
            {
                if (GetMMSEditIsReplaceMultifile() && 0 != replaceed_size)
                {
                    insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize - replaceed_size + real_file_size - MMIMMS_SIZE_LEFT;
                }
                else
                {
                    insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize + real_file_size;
                }
            }
            else
            {
                insert_total_size = real_file_size ;
            }
            
            if (0 == real_file_size)
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else if(insert_total_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;
                
                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);
                
                reduce_pic_size = reduce_width*reduce_height*2;
                
                pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                if (   MMIFMM_FILE_TYPE_PICTURE == multim_return_info_ptr->file_type 
                    && GUIANIM_TYPE_IMAGE_JPG == pic_type
                    && real_file_size > reduce_pic_size
                    && insert_total_size - real_file_size + reduce_pic_size <= MMIMMS_MAX_CONTENT_SIZE
                    && MMIMMS_INSERT_ADD_FILE != edit_type
                    )
                {
                    g_mms_edit_globle.file_type = multim_return_info_ptr->file_type;
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOC_APP(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == g_mms_edit_globle.file_info_ptr)
                    {
                        g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        break;
                    }
                    SCI_MEMSET((char *)g_mms_edit_globle.file_info_ptr, 0, (sizeof(MMIFILE_FILE_INFO_T)));
                    SCI_MEMCPY((char *)g_mms_edit_globle.file_info_ptr, (char *)&file_info, sizeof(MMIFILE_FILE_INFO_T));
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC,IMAGE_PUBWIN_QUERY, &quer_win_id,PNULL);
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE);
                }
                else
                {
                    SetMMSEditReplaceMultifileSize(0);
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
            }
            else
            {    
                SetMMSEditIsSaveDraft(FALSE);
                
                if (!(GetMMSEditIsInsertNormalAttachment() && MMIMMS_INSERT_ADD_FILE == edit_type))
                {  
                    MMIMMS_CONTEDIT_OPT_MENU_NODE_E lose_focus_reason = MMS_CONTEDIT_OPT_NODE_NULL;
                    BOOLEAN is_need_add_new_slide = FALSE;
                    lose_focus_reason= GetMMSEditLoseFocusReason();
                    if(!GetMMSEditIsReplaceMultifile()) //替换的情况不需要添加新帧显示
                    {
                        if(1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)  //是否有视频
                        {
                            is_need_add_new_slide = TRUE;
                        }
                        else
                        {
                            if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                            {
                                if(MMS_CONTEDIT_OPT_NODE_INSMUSIC == lose_focus_reason || MMS_CONTEDIT_OPT_NODE_INSVIDEO == lose_focus_reason)
                                {
                                    is_need_add_new_slide = TRUE;;//本来有音乐，添加音乐或视频需增加新页
                                }
                            }
                            
                            if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                            {
                                if(MMS_CONTEDIT_OPT_NODE_INSIMAGE== lose_focus_reason || MMS_CONTEDIT_OPT_NODE_INSVIDEO == lose_focus_reason)
                                {
                                    is_need_add_new_slide = TRUE;;//本来有图片  ，添加图片或视频需增加新页
                                }
                            }                        
                        }
                        if(is_need_add_new_slide)
                        {
                            MMSAddNewSlide();  //增加新页显示
                        }
                    }
                    
                    
                    if(MMIMMS_AddFile(edit_doc_ptr,&file_info,multim_return_info_ptr->file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))//添加成功，同时更新显示
                    {         
                        if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                        {
                            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);     
                            
                        } 
                        
                        MMSShowMultiFileInfo();                          
                        MMK_SetAtvCtrl( win_id, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID);
                    }                     
                }
                else
                {
                    if (GetMMSEditIsReplaceMultifile())
                    {   
                        MMI_CTRL_ID_T ctrl_id = MMIMMS_ATTACH_LISTBOX_CTRL_ID;
                        uint16 index = 0;
                        const GUILIST_ITEM_T *item_ptr = PNULL;
                        uint8 *add_file_ptr               = PNULL;
                        wchar *file_name = PNULL;
                        uint16 file_name_len = 0;
                        index = GUILIST_GetCurItemIndex(ctrl_id);
                        item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
                        if(PNULL == item_ptr || PNULL == item_ptr->item_data_ptr)
                        {
                            break;
                        }
                                                
                        file_name_len =  item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
                        file_name = item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
                        add_file_ptr = SCI_ALLOC_APP((file_name_len + 1)*3);
                        if (PNULL == add_file_ptr)
                        {
                            return MMI_RESULT_FALSE;
                        }
                        SCI_MEMSET(add_file_ptr, 0, ((file_name_len + 1)*3));            
                        GUI_WstrToUTF8((uint8 *)add_file_ptr, (file_name_len + 1)*3, file_name, file_name_len);        
                        
                        if (!MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, &file_info, add_file_ptr, PNULL, 0, MMIFMM_FILE_TYPE_NORMAL, PNULL))
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                            break;
                        }                    
                        
                        MMK_CloseWin(MMIMMS_ATTACH_WIN_ID);
                        SCI_Free(add_file_ptr);
                        add_file_ptr = PNULL;
                    }
                    else
                    {
                        if(MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, &file_info, PNULL, PNULL, 0, MMIFMM_FILE_TYPE_NORMAL, PNULL))
                        {
                            if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                            {
                                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                    PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);   
                            } 
                        }
                        
                    }           
                    MMK_SetAtvCtrl( win_id, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID);                    
                    SetMMSEditIsInsertNormalAttachment(FALSE);
                } 
                SetMMSEditIsReplaceMultifile(FALSE);
                SetMMSEditReplaceMultifileSize(0);
            }
        }
        
        break;
#if defined MMI_VCARD_SUPPORT
    case MMI_PB_VCARD_SELECTED:
        {            
            uint32  vcard_data_len = 0;
            uint8 *vcard_buf_ptr = 0;
            MMI_STRING_T  vacard_name_str = {0};
            wchar vacard_suf[] = {'.', 'v', 'c', 'f',0};
            
            vcard_buf_ptr = (uint8 *)SCI_ALLOCA(VCARD_MAX_FILE_LEN*sizeof(uint8));
            if(vcard_buf_ptr == PNULL)
            {
                break;
            }
            SCI_MEMSET(vcard_buf_ptr, 0x00, VCARD_MAX_FILE_LEN*sizeof(uint8));
            MMIAPIPB_GetSelectVcard((MMIPB_HANDLE_T)param, vcard_buf_ptr, VCARD_MAX_FILE_LEN, 0);
            vcard_data_len = strlen((char *)vcard_buf_ptr);
            vacard_name_str.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar));
            if (PNULL == vacard_name_str.wstr_ptr)
            {
                SCI_FREE(vcard_buf_ptr);
                break;
            }
            SCI_MEMSET((char *)vacard_name_str.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar)));
            MMIAPIPB_GetSelectName((uint32)param, &vacard_name_str, MMIMMS_FILE_NAME_LEN - MMIAPICOM_Wstrlen(vacard_suf), 0);
            MMIAPICOM_Wstrncpy(vacard_name_str.wstr_ptr + vacard_name_str.wstr_len, vacard_suf, MMIAPICOM_Wstrlen(vacard_suf));
            vacard_name_str.wstr_len = vacard_name_str.wstr_len + MMIAPICOM_Wstrlen(vacard_suf);
            if(MMIMMS_GetIsMMSMode())
            {
                insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize + vcard_data_len;
            }
            else
            {  
                insert_total_size = real_file_size;
            }
            
            if (0 == vcard_data_len)
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else if(insert_total_size > MMIMMS_MMS_LIMIT_SIZE)
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else
            {                
                SetMMSEditIsSaveDraft(FALSE);
                if (GetMMSEditIsReplaceMultifile())
                {
                    MMI_CTRL_ID_T ctrl_id = MMIMMS_ATTACH_LISTBOX_CTRL_ID;
                    uint16 index = 0;
                    const GUILIST_ITEM_T *item_ptr = PNULL;
                    uint8 *add_file_ptr               = PNULL;
                    wchar *file_name = PNULL;
                    uint16 file_name_len = 0;
                    index = GUILIST_GetCurItemIndex(ctrl_id);
                    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
                    if(PNULL == item_ptr || PNULL == item_ptr->item_data_ptr)
                    {
                        SCI_FREE(vcard_buf_ptr);
                        break;
                    }
                                            
                    file_name_len =  item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
                    file_name = item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
                    add_file_ptr = SCI_ALLOC_APP((file_name_len + 1)*3);
                    if (PNULL != add_file_ptr)
                    {
                        SCI_MEMSET(add_file_ptr, 0, ((file_name_len + 1)*3));            
                        GUI_WstrToUTF8((uint8 *)add_file_ptr, (file_name_len + 1)*3, file_name, file_name_len);        
                        
                        if (!MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, PNULL, add_file_ptr, vcard_buf_ptr, vcard_data_len, MMIFMM_FILE_TYPE_VCARD, &vacard_name_str))
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                        }
                        else
                        {
                            MMK_CloseWin(MMIMMS_ATTACH_WIN_ID);
                            SetMMSEditIsReplaceMultifile(FALSE);
                        }
                        
                        SCI_Free(add_file_ptr);
                        add_file_ptr = PNULL;                                                                    
                    }
                }
                else
                {
                    if (!MMIMMS_AddAndReplaceAttachment(edit_doc_ptr, PNULL, PNULL, vcard_buf_ptr, vcard_data_len, MMIFMM_FILE_TYPE_VCARD, &vacard_name_str))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                    } 
                    else
                    {
                        if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                        {
                            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);  
                        } 
                    }
                    
                    MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_ATTACHMENT_FORM_ID);
                }
                SetMMSEditReplaceMultifileSize(0);
            }
            SCI_FREE(vcard_buf_ptr);
            SCI_FREE(vacard_name_str.wstr_ptr);
        }
        break;
#endif
        
     case MSG_APP_LEFT:
         {
             cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
#ifdef MMI_PDA_SUPPORT
             GUIBUTTON_Update(MMIMMS_EDIT_BUTTON_LF_CTRL_ID);            
#endif
             if (1 == cur_slide_index)
             {                
                 MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_LIST_ID);
             }
             else
             {
                 MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
             }
             if (cur_slide_index > 1)
             {
                 MMI_STRING_T edit_str ={0};
                 GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &edit_str);
                 MMIMMS_AddText(edit_doc_ptr, &edit_str);
                 slide_err = MMIMMS_PrevSlide(edit_doc_ptr);
                 if (MMIMMS_NO_ERROR == slide_err)
                 {
                     SetMMSEditFocusOnAddMenu(FALSE);
                     SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                     MMSShowEditContent();   
                     MMSShowMultiFileInfo();       
                     MMSShowMMSInfo();     
                     MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                 }
             }
         }
         break;
     case MSG_APP_RIGHT:
         { 
             cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
#ifdef MMI_PDA_SUPPORT
             GUIBUTTON_Update(MMIMMS_EDIT_BUTTON_RH_CTRL_ID);          
#endif            
             if (1 == cur_slide_index)
             {                
                 MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_LIST_ID);
             }
             else
             {
                 MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
             }
             slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
             if (cur_slide_index < slide_num)
             {
                 MMI_STRING_T edit_str ={0};
                 GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &edit_str);
                 MMIMMS_AddText(edit_doc_ptr, &edit_str);
                 slide_err = MMIMMS_NextSlide(edit_doc_ptr);
                 if (MMIMMS_NO_ERROR == slide_err)
                 {
                     SetMMSEditFocusOnAddMenu(FALSE);
                     SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                     MMSShowEditContent();   
                     MMSShowMultiFileInfo();
                     MMSShowMMSInfo();                         
                     MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,0); 
                 }
             }
         }
         break;
         
     case MSG_APP_RED:
         
         //MMK_CloseWin(win_id);
         err_code = MMI_RESULT_FALSE;
         break;
     case MSG_APP_CANCEL:
     case MSG_CTL_CANCEL:
         if(MMIMMS_GetIsMMSMode())  //彩信退出流程
         {
             //退出编辑主界面
             if (PNULL != edit_doc_ptr)
             {
                 content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
                 if (0 == content_num
                     &&(PNULL == edit_doc_ptr->edit_subject.wstr_ptr || 0 == edit_doc_ptr->edit_subject.wstr_len) 
                     &&(PNULL == edit_doc_ptr->edit_to.wstr_ptr || 0 == edit_doc_ptr->edit_to.wstr_len)
                     )
                 {
                     MMIMMS_CloseEditWin();	
                 }
                 else
                 {
                     SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
                     MMIPUB_OpenQueryWinByTextId(TXT_SMS_ASKSAVE_DRAFT,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);               
                 }
             }
         }
         else   //短信退出流程
         {
             SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
             GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &edit_string);
             
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
             {
                 MMI_HANDLE_T edit_handle = 0;
                 edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                     win_id,
                     MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                     );
                 if(!(MMSGetSendNumber(edit_handle,FALSE)) &&
                     (0 == edit_string.wstr_len))
                 {
                     // close the edit window
                     MMIMMS_CloseEditWin();		
                 }
                 else
                 {
                     MMIPUB_OpenQueryWinByTextId(TXT_SMS_ASKSAVE_DRAFT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                 }
             }               
#else
             if(!(MMSGetSendNumber(MMIMMS_SMS_EDIT_TO_LIST_ID,FALSE)) &&
                 (0 == edit_string.wstr_len))
             {
                 // close the edit window
                 MMIMMS_CloseEditWin();			
             }
             else
             {
                 MMIPUB_OpenQueryWinByTextId(TXT_SMS_ASKSAVE_DRAFT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
             }
#endif
         }
         
         break;
     case MSG_CLOSE_WINDOW:        
         MMIMMS_MMSEditExit();
         if(MMIMMS_GetIsMMSMode()) 
         {
             //退出编辑，释放全局变量
             if (PNULL != g_mms_edit_globle.file_info_ptr)
             {
                 SCI_FREE(g_mms_edit_globle.file_info_ptr);
                 g_mms_edit_globle.file_info_ptr = PNULL;
             }
             
             g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
             SetMMSEditIsInsertNormalAttachment(FALSE);
             MMIMS_SetAnswerSim(MN_DUAL_SYS_MAX);
             SetMMSEditIsSaveDraft(FALSE);
             MMIDEFAULT_AllowTurnOffBackLight(TRUE);
             SetMMSEditLoseFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
             
#ifdef MMI_TIMERMSG_SUPPORT
             MMIAPISMS_SetIsTimerMode(FALSE);
#endif
             MMIMMS_WinExit(EDIT_OPTION_WIN_ENTRY);
             MMIMMS_SetIsMMSMode(FALSE);
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
             MMIAPISMS_SetIsAnwserSMS(FALSE);
#endif		
         }        
         else
         {

#ifdef MMI_TIMERMSG_SUPPORT
             MMIAPISMS_TIMERMSG_SetCurrBoxType(MMISMS_BOX_NOSEND);
             MMIAPISMS_SetIsTimerMode(FALSE);
#endif
             MMIMMS_SetIsMMSMode(FALSE);
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
             MMIAPISMS_SetIsAnwserSMS(FALSE);
#endif
             if(!MMIAPISMS_GetNormalClose())
             {			            
                 // to get the content which the user input
                 MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
             
                 edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
             
                 SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
                 GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_string);
                 MMIAPISMS_SetMessageContent(edit_string.wstr_len, edit_string.wstr_ptr, TRUE, edit_content_ptr);
             // to save the information
 
                 MMSRedkeySaveSmsDraft();
             }
             
             MMIAPISMS_SetNormalClose(FALSE);
         }
         break;
         
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_TP_PRESS_UP:
#ifdef MMI_TIMERMSG_SUPPORT    
         if(MMISMS_GetIsTimerMode())
         {
             GUI_RECT_T  form_rect = {0};
             MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
             SCI_MEMCPY(&para,(MMI_MULTI_KEY_TP_MSG_PARAM_T*)param,sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
             GUICTRL_GetRect( MMK_GetCtrlPtr(MMIMMS_SMS_TIMER_SMS_FORM_ID), &form_rect);
             if(GUI_PointIsInRect(para.cur_tp_point,form_rect))
             {
                 MMISMS_EnterTimerMsgMainWin();
             }
         }        
#endif        
         break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
         
     case MMI_SELECT_CONTACT_CNF:
         {
             uint16 pb_select_num = 0;
             MMI_STRING_T email_addr = {0};
             uint16 pb_select_index = 0;
             MMIMMS_EDIT_ADD_CONTACT_TYPE add_contact_type = MMIMMS_ADD_CONTACT_OP_MAX;
             add_contact_type = GetMMSEditAddContactType();
             
             if(MMIMMS_ADD_CONTACT_OP_MAX == add_contact_type) 
             {
                 //SCI_TRACE_LOW:"mms add contact type max: return"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_6210_112_2_18_2_37_51_48,(uint8*)"");
                 break;
             }
             
             if(MMIMMS_ADD_CONTENT_CONTACT == add_contact_type) //是彩信正文添加联系人
             {
                 //AddContactInfo2Editbox();
                 MMSAddContact2Content((uint32)param);
                 SetMMSEditAddContactType(MMIMMS_ADD_CONTACT_OP_MAX);
             }
             else
             {
                 uint16 item_num = 0;
                 pb_select_num = MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param);
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT 
                 {
                     MMI_HANDLE_T edit_handle = 0;
                     edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                         win_id,
                         MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                         );
                     item_num = GUIEDIT_GetListItemNum(edit_handle);
                 }              
                 
#else
                 item_num = GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif                
                 if(item_num + pb_select_num > MMIMMS_MAX_RECEIVER_NUM)  //暂时短彩都是30条，待控件支持
                 {
                     MMSMMI_OpenMMSAlertWin(TXT_SMS_EXCEED_MAX_NUM, TXT_NULL, IMAGE_PUBWIN_WARNING);
                     break;
                 }
                 if (!MMIMMS_GetIsMMSMode())//当前模式为短信，查看是否要转换为彩信
                 {
                     SCI_MEMSET(&email_addr, 0, sizeof(email_addr));
                     email_addr.wstr_ptr = (wchar *)SCI_ALLOCAZ((MMIPB_MAX_MAIL_LEN + 1)*sizeof(wchar));
                     if (PNULL == email_addr.wstr_ptr)
                     {
                         break;
                     }
                     for (pb_select_index = 0; pb_select_index < MIN(MMIMMS_MAX_RECEIVER_NUM, pb_select_num); pb_select_index++)
                     {
                         SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
                         MMIAPIPB_GetSelectEmail((uint32)param,&email_addr, MMIPB_MAX_MAIL_LEN, pb_select_index);
                         if(email_addr.wstr_len!=0)//所添加的联系人有邮箱地址时转化为彩信
                         {
                             MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
                             MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                 PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleConvertSmsToMmsWaitWinMsg,(uint32)0);
                             MMIMMS_SetIsMMSMode(TRUE);
                             MMIMMS_SetMMSEditIsEmailAddrExist(TRUE);
                             //email_is_selected = TRUE;
                             break;
                         }
                     }
                     SCI_FREE(email_addr.wstr_ptr);
                 }
                 
                 if(MMIMMS_GetIsMMSMode())
                 {
                     MMSAddPbSelectedContacts(pb_select_num, edit_doc_ptr, (uint32)param, win_id);
                 }
                 else  //短信模式
                 {
                     // from PHONEBOOK, call the API to get the phone number.
                     MMSGetNumberFromPb((uint32)param, msg_id);
                     // update the edit box
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                     {
                         MMI_HANDLE_T edit_handle = 0;
                         edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                             win_id,
                             MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                             );
                         MMSAddSmsNumber(edit_handle);
                         MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID);//控件已替换成dropdownlist
                     }                
#else
                     MMSAddSmsNumber(MMIMMS_SMS_EDIT_TO_LIST_ID);
                     MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif
                 }                
             }
         }
         break;
#ifdef MMI_PDA_SUPPORT
     case MMI_CL_MULTI_SELECT_LIST_RETURN:
         
         MmsHandleCallogSelectReturn(edit_doc_ptr, win_id, MMI_CL_MULTI_SELECT_LIST_RETURN);
         break;
         
#endif
         
     default :
         err_code = MMI_RESULT_FALSE;
         break;
    }
    
    return err_code;
}
#endif
#else
/*****************************************************************************/
//  Description :mms edit menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSEditMainMenuWin(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                              )
{
    MMI_RESULT_E err_code                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                          = MMIMMS_EDIT_MENU_CTRL_ID;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    uint16 richedit_index                          = 0;
    uint16 item_index                              = MMIMMS_RICHTEXT_TO;  
    MMIFILE_FILE_INFO_T  file_info                 = {0};
    uint16 content_num                             = 0;
    uint16 get_focus_index                         = 0;    
    MULTIM_SELECT_RETURN_T *multim_return_info_ptr = PNULL;
    uint16 cur_slide_index                         = 0;
    uint16 slide_num                               = 0;
    MMIMMS_ERROR_E   slide_err                     = MMIMMS_NO_ERROR;
    uint32 insert_total_size                       = 0;
    uint32 real_file_size                          = 0;
    BOOLEAN is_have_pic                            = FALSE;
    uint32 replaceed_size                          = 0;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element      = {0};/*lint !e64*/    
    uint8 *add_file_ptr                            = PNULL;
    uint16 add_file_len                            = 0;
    GUIANIM_TYPE_E    pic_type                     = GUIANIM_TYPE_NONE;
    static BOOLEAN is_init_focus_item              = FALSE;
    GUI_RECT_T      richtext_rect                  = {0};
    GUI_RECT_T      button_rect1                   = {0};
    GUI_RECT_T      button_rect2                   = {0};
    static BOOLEAN is_switch_richtext              = FALSE;
    GUI_COLOR_T rich_border_color                  = MMITHEME_GetCurThemeFontColor(MMI_THEME_MMS_RICHTEXT_BORDER);
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

    edit_doc_ptr = g_mms_edit_globle.editdoc;
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMainMenuWin msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_6586_112_2_18_2_37_51_54,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_LCD_SWITCH:
        is_switch_richtext = TRUE;
        MMIMMS_GetRichtextRect(&richtext_rect);
        MMIMMS_GetPagebarLbinRect(&button_rect1);
        MMIMMS_GetPagebarRbinRect(&button_rect2);
        GUIBUTTON_SetRect(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&button_rect2);
        GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);
        if (GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
        {
            SetMMSEditLoseFocusIndex(richedit_index);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(MMIMMS_EDIT_BUTTON_LF_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
        {
            msg_id = MSG_APP_LEFT;
            MMK_SendMsg(win_id,msg_id,param);
        }
        else if(MMIMMS_EDIT_BUTTON_RH_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
        {
            msg_id = MSG_APP_RIGHT;
            MMK_SendMsg(win_id,msg_id,param);
        }   
        else 
        {
            MMSEditEnterCorrespondItem(ctrl_id, edit_doc_ptr);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_OPEN_WINDOW:
        MMIMMS_GetRichtextRect(&richtext_rect);
        MMIMMS_GetPagebarLbinRect(&button_rect1);
	        MMIMMS_GetPagebarRbinRect(&button_rect2);
        GUIBUTTON_SetRect(MMIMMS_EDIT_BUTTON_LF_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMIMMS_EDIT_BUTTON_RH_CTRL_ID,&button_rect2);
        GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);
        //显示richedit内容，第一帧彩信的显示包括主题和收件人，
        //其他帧的彩信包括文本图片/视频和音乐，如果有视频则不显示音乐图标，
        //显示的时候没有文件图片和音乐用默认图标表示
        MMIMMS_WinInit(EDIT_OPTION_WIN_ENTRY);
        is_init_focus_item = TRUE;
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);       
        GUIRICHTEXT_SetBorderColor(ctrl_id, rich_border_color);
#ifdef MMI_PDA_SUPPORT//pda不需要边框
        GUIRICHTEXT_SetBorderFocusColor(ctrl_id, rich_border_color);
        MMSEditWinLFRHButtonInit();                        
#endif
        GUIRICHTEXT_SetFocusBgColor(ctrl_id, MMI_DARK_GRAY_COLOR);
        GUIRICHTEXT_DisableClipBoard(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id); 
        //MMIDEFAULT_SetBackLight(TRUE);
        is_switch_richtext = FALSE;
        //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        #ifndef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        #endif

#ifdef MMI_PDA_SUPPORT
        MMSEditWinButtonFormInit(win_id);
        //GUIWIN_SetTitleIstyleIconState(win_id, TITLE_ISTYLE_MENU_ICON, TITLE_ISTYLE_ICON_NORMAL, FALSE);
#endif
        break;
    case MSG_LOSE_FOCUS:
        {
            //MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            if (GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
            {
                SetMMSEditLoseFocusIndex(richedit_index);
            }
        }        
        break;
    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MSG_GET_FOCUS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_6662_112_2_18_2_37_52_55,(uint8*)"");
        //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
        {
            MMIMMS_CLEAR_CLIENT(win_id);
            if (    PNULL != edit_doc_ptr 
                &&  PNULL != edit_doc_ptr->editbody_ptr 
                &&  PNULL != edit_doc_ptr->editbody_ptr->slide_cur
                &&  1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
            {
                is_have_pic = TRUE;
            }
            //判断是否需要画richedit的items，不需要则更新即可
            if (   !is_switch_richtext &&
                (
                   MMS_CONTEDIT_OPT_NODE_SEND == GetMMSEditLoseFocusReason()
                || MMS_CONTEDIT_OPT_NODE_SETTIME == GetMMSEditLoseFocusReason()
                || MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT == GetMMSEditLoseFocusReason()
                || (MMS_CONTEDIT_OPT_NODE_CANCEL == GetMMSEditLoseFocusReason() && (!is_have_pic))
                || (MMS_CONTEDIT_OPT_NODE_DELIMAGE == GetMMSEditLoseFocusReason() && is_have_pic && GetMMSEditIsOpPopMenu())//删除图片pop的时候刷新不要把图片刷新掉cr179461
                || !MMK_IsFocusWin(MMIMMS_EDIT_MENU_WIN_ID)
                )                
				)
            {
                GUIRICHTEXT_Update(ctrl_id);  

               //设置焦点
                if (GetMMSEditLoseFocusIndex() < MMIMMS_RICHTEXT_ITEM_NUM_MAX)
                {
                    GUIRICHTEXT_SetFocusItem(ctrl_id, GetMMSEditLoseFocusIndex());
                    SetMMSEditPreFocusIndex(GetMMSEditLoseFocusIndex());
                    if (MMS_CONTEDIT_OPT_NODE_HALF_POP == GetMMSEditLoseFocusReason())
                    {
                        SetMMSEditLoseFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
                    }                
                }
            }
            else
            {
                if (is_switch_richtext)
                {
                    is_switch_richtext = FALSE; 
                }
                for (item_index = 0; item_index < MMIMMS_RICHTEXT_ITEM_NUM_MAX; item_index++)
                {
                    g_mms_edit_richtext_item_type[item_index] = MMIMMS_RICHTEXT_MAX;
                }  
                if (MMS_CONTEDIT_OPT_NODE_PREV == GetMMSEditLoseFocusReason() && PNULL != edit_doc_ptr)
                {
                    MMIMMS_HeadSlide(edit_doc_ptr);
                }
                GUIRICHTEXT_DeleteAllItems(ctrl_id);
                GUIRICHTEXT_ResetState(ctrl_id);
                //SetMMSEditFocusOnAddMenu(FALSE);
                if (!MMSEditEntry(MMIMMS_RICHTEXT_ADD_ITEM))
                {
                    //提示彩信编辑错误，退出编辑发送MSG_CLOSE_WINDOW消息                
                }
                GUIRICHTEXT_Update(ctrl_id);
                if (GetMMSEditLoseFocusIndex() < MMIMMS_RICHTEXT_ITEM_NUM_MAX && is_init_focus_item)
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex((MMIMMS_RICHTEXT_ITEM_INDEX_E)GetMMSEditLoseFocusIndex()));
                    is_init_focus_item = FALSE;
                }
                //设置焦点
                if (GetMMSEditLoseFocusIndex() < MMIMMS_RICHTEXT_ITEM_NUM_MAX)
                {
                    GUIRICHTEXT_SetFocusItem(ctrl_id, GetMMSEditLoseFocusIndex());
                    SetMMSEditPreFocusIndex(GetMMSEditLoseFocusIndex());
                    if (MMS_CONTEDIT_OPT_NODE_HALF_POP == GetMMSEditLoseFocusReason())
                    {
                        SetMMSEditLoseFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
                    }                
                }
                
                GUIRICHTEXT_ScrollToFocus(ctrl_id);
            }       
            
            get_focus_index = GetMMSEditLoseFocusIndex();

            if ( PNULL == edit_doc_ptr
                || PNULL == edit_doc_ptr->editbody_ptr
                || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
            {
                break;
            }
            //对有焦点的item添加中间键
            if (   MMIMMS_RICHTEXT_TO   == g_mms_edit_richtext_item_type[get_focus_index] 
                || MMIMMS_RICHTEXT_SUBJECT  == g_mms_edit_richtext_item_type[get_focus_index])
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, FALSE);
            }
            else if (MMIMMS_RICHTEXT_TEXT == g_mms_edit_richtext_item_type[get_focus_index])
            {
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_TEXT, STXT_RETURN, FALSE);
                }   
                else
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, FALSE);
                }
            }
            else if (MMIMMS_RICHTEXT_PIC_OR_MOVIE == g_mms_edit_richtext_item_type[get_focus_index])
            {
#ifdef VIDEO_PLAYER_SUPPORT
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_VIDEO, STXT_RETURN, FALSE);
                }
                else 
#endif
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PIC, STXT_RETURN, FALSE);
                }
                else
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, FALSE);
                }
            }
            else if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[get_focus_index])
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MMS_ATTACHMENT, STXT_RETURN, FALSE);
            }
            else if (MMIMMS_RICHTEXT_MUSIC == g_mms_edit_richtext_item_type[get_focus_index])
            {
                if (0  != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_AUDIO, STXT_RETURN, FALSE);
                }
                else 
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, FALSE);
                }
            }  
            else if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[get_focus_index])
            {
                if(IsCanAddMediumData())
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, FALSE);
                }
                else
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, NULL, STXT_RETURN, FALSE);
                }
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            }
            
            //画pagebar
            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr, FALSE);

            GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED/*GUIRICHTEXT_FOCUS_SHIFT_BASIC*/);
        }        
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        //如果第一帧进入收件人编辑，如果第一帧进入主题编辑，
        //进入文字编辑，进入图片选择窗口，进入音乐选择窗口，进入视频选择窗口  
        MMSEditEnterCorrespondItem(ctrl_id, edit_doc_ptr);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        //进入选项
        OpenMMSEditOptPopMenu();
        break;
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        //(1)当焦点到to 和 subject及text时,中间键为编辑，
        //(2)当焦点到图片/视频时，如果有视频，中间键视频，其他为图片
        //(3)当焦点为音乐时，中间键为音乐   
#ifdef MMI_PDA_SUPPORT
        MMSEditEnterCorrespondItem(ctrl_id, edit_doc_ptr);
#else
        if (!GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
        {
            break;
        }
        get_focus_index = GetMMSEditPreFocusIndex();
        if (  MMIMMS_RICHTEXT_TO == g_mms_edit_richtext_item_type[richedit_index] 
            || MMIMMS_RICHTEXT_SUBJECT == g_mms_edit_richtext_item_type[richedit_index]
            )
        {        
            SetMMSEditFocusOnAddMenu(FALSE);            
            if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[get_focus_index])
            {
                //GUIRICHTEXT_DeleteItem(ctrl_id, g_mms_edit_richtext_item_type[MMIMMS_RICHTEXT_ADDMENU]);
                SetMMSEditFocusOnAddMenu(FALSE);
                MMSEditAddMenuItem(MMIMMS_RICHTEXT_MOD_ITEM);
                GUIRICHTEXT_Update(ctrl_id);
            }            
            SetMMSEditPreFocusIndex(richedit_index);
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);
        }
        else if (MMIMMS_RICHTEXT_TEXT == g_mms_edit_richtext_item_type[richedit_index])
        {
            SetMMSEditFocusOnAddMenu(FALSE);
            if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[get_focus_index])
            {
                //GUIRICHTEXT_DeleteItem(ctrl_id, g_mms_edit_richtext_item_type[MMIMMS_RICHTEXT_ADDMENU]);
                SetMMSEditFocusOnAddMenu(FALSE);
                MMSEditAddMenuItem(MMIMMS_RICHTEXT_MOD_ITEM);
                GUIRICHTEXT_Update(ctrl_id);
            }            
            SetMMSEditPreFocusIndex(richedit_index);
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_TEXT, STXT_RETURN, TRUE);
            }   
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, TRUE);
            }
        }
        else if (MMIMMS_RICHTEXT_PIC_OR_MOVIE == g_mms_edit_richtext_item_type[richedit_index])
        {
            SetMMSEditFocusOnAddMenu(FALSE);
            if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[get_focus_index])
            {
                MMSEditAddMenuItem(MMIMMS_RICHTEXT_MOD_ITEM);
                GUIRICHTEXT_Update(ctrl_id);
            }  
            SetMMSEditPreFocusIndex(richedit_index);
#ifdef VIDEO_PLAYER_SUPPORT
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_VIDEO, STXT_RETURN, TRUE);
            }
            else
#endif
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PIC, STXT_RETURN, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, TRUE);
            }
        }
        else if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[richedit_index])
        {
            SetMMSEditFocusOnAddMenu(FALSE);
            if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[get_focus_index])
            {
                MMSEditAddMenuItem(MMIMMS_RICHTEXT_MOD_ITEM);
                GUIRICHTEXT_Update(ctrl_id);
            }  
            SetMMSEditPreFocusIndex(richedit_index);
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_MMS_ATTACHMENT, STXT_RETURN, TRUE);
        }
        else if (MMIMMS_RICHTEXT_MUSIC == g_mms_edit_richtext_item_type[richedit_index])
        {
            SetMMSEditFocusOnAddMenu(FALSE);
            if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[get_focus_index])
            {
                MMSEditAddMenuItem(MMIMMS_RICHTEXT_MOD_ITEM);
                GUIRICHTEXT_Update(ctrl_id);
            }  
            SetMMSEditPreFocusIndex(richedit_index);
            if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_AUDIO, STXT_RETURN, TRUE);
            }
            else 
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, TRUE);
            }
        }  
        else if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[richedit_index])
        {
            SetMMSEditFocusOnAddMenu(TRUE);
            if (MMIMMS_RICHTEXT_ADDMENU != g_mms_edit_richtext_item_type[get_focus_index])
            {
                MMSEditAddMenuItem(MMIMMS_RICHTEXT_MOD_ITEM);
                GUIRICHTEXT_Update(ctrl_id);
            } 
            SetMMSEditPreFocusIndex(richedit_index);
            if(IsCanAddMediumData())
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_ADD, STXT_RETURN, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, NULL, STXT_RETURN, TRUE);
            }
        }
#endif
        break;
    case MSG_PROMPTWIN_OK:
        //关闭窗口，退出彩信编辑，以及删除多媒体pop menu处理
        { 
            GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index);

            MMK_CloseWin(quer_win_id); 
            if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE)
            {
                if (PNULL != g_mms_edit_globle.file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == g_mms_edit_globle.file_type)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    if (!(GetMMSEditIsInsertNormalAttachment() && MMIMMS_INSERT_ADD_FILE == GetMMSEditInsertOpType()))
                    {
                        if(!MMIMMS_AddFile(edit_doc_ptr,g_mms_edit_globle.file_info_ptr,g_mms_edit_globle.file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size))
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                        }
                    }
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                }
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
                SetMMSEditReplaceMultifileSize(0);
            }
            else if (GetMMSEditLoseFocusReason() == MMS_CONTEDIT_OPT_NODE_CANCEL)
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
                MMIMMS_CloseEditWin();
            }
            else if(TRUE == GetMMSEditIsOpPopMenu())
            {
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag && MMS_CONTEDIT_OPT_NODE_DELIMAGE == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelVideo(edit_doc_ptr);
                }
                else if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag && MMS_CONTEDIT_OPT_NODE_DELIMAGE == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelImage(edit_doc_ptr);
                }
                else if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize && MMS_CONTEDIT_OPT_NODE_DELMUSIC == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_DelMusic(edit_doc_ptr);
                }
                else if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[richedit_index])//根据附件名称删除附件，根据focus的item获得附件名称
                {
                    GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
                    
                    add_file_ptr = SCI_ALLOC_APP((richtext_element.data.len + 1)*3);
                    if (PNULL == add_file_ptr)
                    {
                        break;
                    }
                    SCI_MEMSET(add_file_ptr, 0, ((richtext_element.data.len + 1)*3));
                    add_file_len = GUI_WstrToUTF8((uint8 *)add_file_ptr, ((richtext_element.data.len + 1)*3), richtext_element.data.str_ptr, richtext_element.data.len);

                    if (!MMIMMS_DelAttachment(add_file_ptr, edit_doc_ptr))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                        break;
                    }                    

                    SCI_Free(add_file_ptr);
                    add_file_ptr = PNULL;
                }
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                
#endif
            }            
        }        
        break;
    case MSG_PROMPTWIN_CANCEL:
        //回到彩信编辑
        if (PNULL != g_mms_edit_globle.file_info_ptr)
        {
            SCI_FREE(g_mms_edit_globle.file_info_ptr);
            g_mms_edit_globle.file_info_ptr = PNULL;
        }
        g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
        MMK_CloseWin(quer_win_id);
        break;
    case MSG_MULTIM_SELECTED_RETURN:
        //选中多媒体文件返回
        {    
            multim_return_info_ptr = (MULTIM_SELECT_RETURN_T *)param;
            MMIAPIFMM_GetFileInfoFormFullPath(multim_return_info_ptr->file_name_ptr,multim_return_info_ptr->file_name_len,&file_info);
            real_file_size = file_info.file_size;
            replaceed_size = GetMMSEditReplaceMultifileSize();
            
            if (GetMMSEditIsReplaceMultifile() && 0 != replaceed_size)
            {
                insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize - replaceed_size + real_file_size - MMIMMS_SIZE_LEFT;
            }
            else
            {
                insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize + real_file_size;
            }
            if (0 == real_file_size)
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else if(insert_total_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;

                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);
                
                reduce_pic_size = reduce_width*reduce_height*2;

                pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                if (   MMIFMM_FILE_TYPE_PICTURE == multim_return_info_ptr->file_type 
                    && (GUIANIM_TYPE_IMAGE_JPG == pic_type || GUIANIM_TYPE_IMAGE_BMP == pic_type || GUIANIM_TYPE_IMAGE_PNG == pic_type || GUIANIM_TYPE_IMAGE_GIF == pic_type)
                    && real_file_size > reduce_pic_size
                    && insert_total_size - real_file_size + reduce_pic_size <= MMIMMS_MAX_CONTENT_SIZE
                    && MMIMMS_INSERT_ADD_FILE != GetMMSEditInsertOpType()
                    )
                {
                    g_mms_edit_globle.file_type = multim_return_info_ptr->file_type;
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOC_APP(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == g_mms_edit_globle.file_info_ptr)
                    {
                        g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        break;
                    }
                    SCI_MEMSET((char *)g_mms_edit_globle.file_info_ptr, 0, (sizeof(MMIFILE_FILE_INFO_T)));
                    SCI_MEMCPY((char *)g_mms_edit_globle.file_info_ptr, (char *)&file_info, sizeof(MMIFILE_FILE_INFO_T));
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC,IMAGE_PUBWIN_QUERY, &quer_win_id,PNULL);
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE);
                }
                else
                {
                    SetMMSEditReplaceMultifileSize(0);
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
            }
            else if(MMIFMM_FILE_TYPE_EBOOK == MMIMMS_GetFileType(file_info.file_name, file_info.file_name_len))
            {
                SetMMSEditReplaceMultifileSize(0);
                MMSMMI_OpenMMSAlertWin(TXT_COMMON_UNSUPPORT_FILE_TYPE, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
            }
            else
            {                
                SetMMSEditIsSaveDraft(FALSE);
                if (!(GetMMSEditIsInsertNormalAttachment() && MMIMMS_INSERT_ADD_FILE == GetMMSEditInsertOpType()))
                {
                    get_focus_index = GetMMSEditLoseFocusIndex();
                    if (MMIFMM_FILE_TYPE_MOVIE == multim_return_info_ptr->file_type && MMIMMS_RICHTEXT_PIC_OR_MOVIE < g_mms_edit_richtext_item_type[get_focus_index])
                    {
                        SetMMSEditLoseFocusIndex(get_focus_index - 1);//如果是视频的话index会少1,同时焦点在视频位置下面，因为没有music而减1
                    }
                    MMIMMS_AddFile(edit_doc_ptr,&file_info,multim_return_info_ptr->file_type,g_mms_edit_globle.is_replace_multifile,g_mms_edit_globle.replace_multifile_size);
                }
                else
                {
                    GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index);
                    if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[richedit_index] && GetMMSEditIsReplaceMultifile())//
                    {
                        SetMMSEditIsReplaceMultifile(FALSE);
                        
                        GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);                    
                        add_file_len =(richtext_element.data.len + 1)*3;
                        add_file_ptr = SCI_ALLOC_APP(add_file_len);
                        if (PNULL == add_file_ptr)
                        {
                            break;
                        }
                        SCI_MEMSET(add_file_ptr, 0, add_file_len);
                        add_file_len = GUI_WstrToUTF8((uint8 *)add_file_ptr, add_file_len, richtext_element.data.str_ptr, richtext_element.data.len);
                        
                        if (!MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, &file_info, add_file_ptr, PNULL, 0, MMIFMM_FILE_TYPE_NORMAL, PNULL))
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                            break;
                        }                    
                        
                        SCI_Free(add_file_ptr);
                        add_file_ptr = PNULL;
                    }
                    else
                    {
                        MMIMMS_AddAndReplaceAttachment( edit_doc_ptr, &file_info, PNULL, PNULL, 0, MMIFMM_FILE_TYPE_NORMAL, PNULL);
                    }                    
                    SetMMSEditIsInsertNormalAttachment(FALSE);
                } 
                SetMMSEditIsReplaceMultifile(FALSE);
                SetMMSEditReplaceMultifileSize(0);
            }
        }
        break;
#if defined MMI_VCARD_SUPPORT
    case MMI_PB_VCARD_SELECTED:
        {           
            uint32  vcard_data_len = 0;
            uint8 *vcard_buf_ptr  = 0;
            MMI_STRING_T  vacard_name_str = {0};
            wchar  vacard_suf[] = {'.', 'v', 'c', 'f',0};

            vcard_buf_ptr = (uint8 *)SCI_ALLOC_APP(VCARD_MAX_FILE_LEN*sizeof(uint8));
            if(vcard_buf_ptr != PNULL)
            {
                SCI_MEMSET(vcard_buf_ptr, 0x00, VCARD_MAX_FILE_LEN*sizeof(uint8));
                if(MMIAPIPB_GetSelectVcard((MMIPB_HANDLE_T)param, vcard_buf_ptr, VCARD_MAX_FILE_LEN*sizeof(uint8), 0))
                {
                    vcard_data_len = strlen((char*)vcard_buf_ptr);
                    vacard_name_str.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar));
                    if( vacard_name_str.wstr_ptr != PNULL)
                    {
                        
                        SCI_MEMSET((char *)vacard_name_str.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar)));
                        MMIAPIPB_GetSelectName((MMIPB_HANDLE_T)param, &vacard_name_str, MMIMMS_FILE_NAME_LEN - MMIAPICOM_Wstrlen(vacard_suf), 0);
                        MMIAPICOM_Wstrncpy(vacard_name_str.wstr_ptr + vacard_name_str.wstr_len, vacard_suf, MMIAPICOM_Wstrlen(vacard_suf));
                        vacard_name_str.wstr_len = vacard_name_str.wstr_len + MMIAPICOM_Wstrlen(vacard_suf);
                        insert_total_size = (uint32)edit_doc_ptr->editbody_ptr->totalsize + vcard_data_len;
                        if (0 == vcard_data_len)
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                        }
                        else if(insert_total_size > MMIMMS_MMS_LIMIT_SIZE)
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                        }
                        else
                        {                
                            SetMMSEditIsSaveDraft(FALSE);
                            GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index);
                            if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[richedit_index] && GetMMSEditIsReplaceMultifile())//
                            {
                                SetMMSEditIsReplaceMultifile(FALSE);
                                
                                GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
                                add_file_len =(richtext_element.data.len + 1)*3;
                                add_file_ptr = SCI_ALLOC_APP(add_file_len);
                                if (PNULL != add_file_ptr)
                                {
                                    SCI_MEMSET(add_file_ptr, 0, add_file_len);
                                    add_file_len = GUI_WstrToUTF8((uint8 *)add_file_ptr, add_file_len, richtext_element.data.str_ptr, richtext_element.data.len);
                                    
                                    if (!MMIMMS_AddAndReplaceAttachment(edit_doc_ptr, PNULL, add_file_ptr, vcard_buf_ptr, vcard_data_len, MMIFMM_FILE_TYPE_VCARD, &vacard_name_str))
                                    {
                                        MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                                        //break;
                                    }                    
                                    
                                    SCI_Free(add_file_ptr);
                                    add_file_ptr = PNULL;
                                }
                            }
                            else
                            {
                                if (!MMIMMS_AddAndReplaceAttachment(edit_doc_ptr, PNULL, PNULL, vcard_buf_ptr, vcard_data_len, MMIFMM_FILE_TYPE_VCARD, &vacard_name_str))
                                {
                                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                                } 
                            }
                            SetMMSEditReplaceMultifileSize(0);
                        }
                        SCI_FREE( vacard_name_str.wstr_ptr);
                    }
                }
                SCI_FREE(vcard_buf_ptr)
            }
            
        }
        break;
#endif
        
    case MSG_APP_LEFT:
        {
            cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
#ifdef MMI_PDA_SUPPORT
            GUIBUTTON_Update(MMIMMS_EDIT_BUTTON_LF_CTRL_ID);            
#endif
            if (1 == cur_slide_index)
            {
                SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
            }
            else
            {
                SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
            }
            if (cur_slide_index > 1)
            {
                slide_err = MMIMMS_PrevSlide(edit_doc_ptr);
                if (MMIMMS_NO_ERROR == slide_err)
                {
                    SetMMSEditFocusOnAddMenu(FALSE);
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL); 
                }
            }
        }
        break;
    case MSG_APP_RIGHT:
        { 
            cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
#ifdef MMI_PDA_SUPPORT
            GUIBUTTON_Update(MMIMMS_EDIT_BUTTON_RH_CTRL_ID);            
#endif
            if (1 == cur_slide_index)
            {
                SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
            }
            else
            {
                SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
            }
            slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
            if (cur_slide_index < slide_num)
            {
                slide_err = MMIMMS_NextSlide(edit_doc_ptr);
                if (MMIMMS_NO_ERROR == slide_err)
                {
                    SetMMSEditFocusOnAddMenu(FALSE);
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CHANGE_SLIDE);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL); 
                }
            }
        }
        break;
    case MSG_APP_RED:
        if (PNULL != edit_doc_ptr)
        {
            MMIMMS_ERROR_E   mms_save_result        = MMIMMS_NO_ERROR;

            content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
            if (0 < content_num 
                || (PNULL != edit_doc_ptr->edit_subject.wstr_ptr && 0 != edit_doc_ptr->edit_subject.wstr_len)
                || (PNULL != edit_doc_ptr->edit_to.wstr_ptr && 0 != edit_doc_ptr->edit_to.wstr_len))
            {//如果有内容提示保存，然后返回上级菜单
                mms_save_result = MMIMMS_SaveMMS(MN_DUAL_SYS_1, MMISMS_MO_DRAFT, edit_doc_ptr);
                if (MMIMMS_NO_ERROR == mms_save_result)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_SAVE_TO_DRAFTBOX, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
                }
            }
        }
        MMK_CloseWin(win_id);
        err_code = MMI_RESULT_FALSE;
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //退出编辑主界面
        if (PNULL != edit_doc_ptr)
        {
            content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
            if (0 == content_num
                &&(PNULL == edit_doc_ptr->edit_subject.wstr_ptr || 0 == edit_doc_ptr->edit_subject.wstr_len) 
                &&(PNULL == edit_doc_ptr->edit_to.wstr_ptr || 0 == edit_doc_ptr->edit_to.wstr_len)
                )
            {
                MMK_CloseWin(win_id);
            }
            else
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
                if (GetMMSEditIsSaveDraft())
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL); //cr121316
                }
                else
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
                }                
            }
        }
        
        break;
    case MSG_CLOSE_WINDOW:
        //退出编辑，释放全局变量
        if (PNULL != g_mms_edit_globle.file_info_ptr)
        {
            SCI_FREE(g_mms_edit_globle.file_info_ptr);
            g_mms_edit_globle.file_info_ptr = PNULL;
        }
        g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
        SetMMSEditIsInsertNormalAttachment(FALSE);
        MMIMS_SetAnswerSim(MN_DUAL_SYS_MAX);
        SetMMSEditIsSaveDraft(FALSE);
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        SetMMSEditLoseFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
        MMIMMS_MMSEditExit();
        MMIMMS_WinExit(EDIT_OPTION_WIN_ENTRY);
        break;

#ifdef MMI_PDA_SUPPORT
        case MMI_SELECT_CONTACT_CNF:
        {
            uint16 pb_select_num = 0;
            MMIMMS_EDIT_NUMBER_LIST_T *add_numberlist_ptr = PNULL;
            uint16  temp_len                             = 0;
            wchar  *all_number_wstr                      = PNULL;
            uint8  tele_len                              = 0;
            wchar  tele_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
            uint8  tele_num[MMIMMS_MAX_ADDRESS_LEN + 2]  = {0};
            MMIPB_BCD_NUMBER_T pb_bcd_number             = {0};
            MMI_STRING_T email_addr                      = {0};
            uint32 max_number_len = 0;         
            uint16 all_number_len = 0;
            uint16 pb_select_index = 0;
            MMI_STRING_T number_name_addr                = {0};
            MMI_STRING_T  to_edit_string                  = {0};
            MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;
            MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
            wchar semicolon[]                            = {';', 0};
            BOOLEAN is_same_num = FALSE;

            pb_select_num = MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param);
            if(g_mms_edit_globle.editdoc != PNULL)
            {
                add_numberlist_ptr = g_mms_edit_globle.editdoc->numberlist_header_ptr;
            }
            if (pb_select_num > 0)
            {
                temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
                all_number_wstr = (wchar *)MMIMMS_ALLOC(temp_len);
                if (PNULL == all_number_wstr)
                {
                    //SCI_TRACE_LOW:"malloc all_number_wstr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7374_112_2_18_2_37_54_56,(uint8*)"");
                    MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                    break;
                }
                SCI_MEMSET(all_number_wstr, 0, temp_len);
                if (PNULL != edit_doc_ptr->edit_to.wstr_ptr && 0 != edit_doc_ptr->edit_to.wstr_len)
                {
                    MMI_WSTRNCPY(all_number_wstr, temp_len/2, edit_doc_ptr->edit_to.wstr_ptr, edit_doc_ptr->edit_to.wstr_len, edit_doc_ptr->edit_to.wstr_len);
                    all_number_len = all_number_len + edit_doc_ptr->edit_to.wstr_len;
                }
                
                SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
                SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
                SCI_MEMSET(&email_addr, 0, sizeof(email_addr));
                email_addr.wstr_ptr = (wchar *)MMIMMS_ALLOC((MMIPB_MAX_MAIL_LEN + 1)*sizeof(wchar));
                if (PNULL == email_addr.wstr_ptr)
                {
                    //SCI_TRACE_LOW:"malloc email_addr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7391_112_2_18_2_37_54_57,(uint8*)"");
                    MMIMMS_FREE(all_number_wstr);
                    all_number_wstr = PNULL;
                    MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                    break;
                }
                SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
                number_name_addr.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
                SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
          
                for (pb_select_index = 0; pb_select_index < MIN(MMIMMS_MAX_RECEIVER_NUM, pb_select_num); pb_select_index++)
                {
                    SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
                    SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
                    email_addr.wstr_len = 0;
                    SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
                    number_name_addr.wstr_len = 0;

                    MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &pb_bcd_number,MMIPB_BCD_NUMBER_MAX_LEN, pb_select_index);
                    MMIAPIPB_GetSelectEmail((MMIPB_HANDLE_T)param, &email_addr, MMIPB_MAX_MAIL_LEN, pb_select_index);
                    MMIAPIPB_GetSelectName((MMIPB_HANDLE_T)param, &number_name_addr, MMIMMS_FILE_NAME_LEN, pb_select_index);
                    max_number_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, pb_bcd_number.number_len);
                    tele_len = MMIAPIPB_BCDNumberToString(&pb_bcd_number, MMIMMS_MAX_ADDRESS_LEN + 2, tele_num);
//                     tele_len = MMIAPICOM_GenDispNumber( 
//                         MMIPB_GetNumberTypeFromUint8(pb_bcd_number.npi_ton), 
//                         (uint8)max_number_len, 
//                         pb_bcd_number.number,  
//                         tele_num, 
//                         MMIMMS_MAX_ADDRESS_LEN + 2 
//                         );
                    //SCI_TRACE_LOW:"HandleMMSEditToListWinMsg,tele_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7415_112_2_18_2_37_54_58,(uint8*)"d",tele_len);
                    if(0 != tele_len)
                    {
                        MMI_STRNTOWSTR(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, (uint8 *)tele_num, MMIMMS_MAX_ADDRESS_LEN, tele_len );
                        //判断该号码是否已经存在                        
                        temp_numberlist_ptr = add_numberlist_ptr;
                        is_same_num = FALSE;
                        while(PNULL != temp_numberlist_ptr)
                        {
                            if (0 != temp_numberlist_ptr->number_len
                                && temp_numberlist_ptr->number_len == MMIAPICOM_Wstrlen(tele_wstr)
                                && !MMIAPICOM_Wstrncmp(temp_numberlist_ptr->number_ptr, tele_wstr, MMIMMS_MAX_ADDRESS_LEN))
                            {
                                is_same_num = TRUE;
                                break;
                            }
                            else
                            {
                                temp_numberlist_ptr = temp_numberlist_ptr->next;
                            }
                            
                        }
                        if (!is_same_num)
                        {
                            //加入到to的结构中去
                            //todo to_edit_string里面是原有的的号码，在尾部添加；同时加上现在的号码
                            MMIAPICOM_Wstrncat(all_number_wstr + all_number_len, tele_wstr, tele_len);
                            all_number_len = all_number_len + tele_len;
                            MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                            all_number_len = all_number_len +1;
                            
                            //加入到namelist的结构中去
                            cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                            SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                            if(0 != number_name_addr.wstr_len)
                            {//有pb中名称
                                cursor_numberlist_ptr->is_object = TRUE;
                                cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                                cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                    number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);

                                cursor_numberlist_ptr->number_len = tele_len;
                                cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                    tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                            }
                            else
                            {//没有名称按普通的号码处理
                                cursor_numberlist_ptr->is_object = FALSE;
                                cursor_numberlist_ptr->number_len = tele_len;
                                cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                    tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                            }
                            if(PNULL == add_numberlist_ptr)
                            {
                                g_mms_edit_globle.editdoc->numberlist_header_ptr = cursor_numberlist_ptr;
                                add_numberlist_ptr = cursor_numberlist_ptr;
                            }
                            else
                            {
                                temp_numberlist_ptr = add_numberlist_ptr;
                                while(PNULL != temp_numberlist_ptr->next)
                                {
                                    temp_numberlist_ptr = temp_numberlist_ptr->next;
                                }
                                temp_numberlist_ptr->next = cursor_numberlist_ptr;
                            } 
                        } 
                        {
                            //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo number has exist!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7487_112_2_18_2_37_54_59,(uint8*)"");
                        }
                    }//if (tele_len != 0)
                    else
                    {
                        //SCI_TRACE_MID:"warning_ tele_len 0"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7492_112_2_18_2_37_54_60,(uint8*)"");
                    }
                    if (0 != email_addr.wstr_len)
                    {//加入email的结构中去
                        if (MMIAPICOM_IsASCIIString(email_addr.wstr_ptr, email_addr.wstr_len))
                        {     
                            MMIAPICOM_Wstrncat(all_number_wstr, email_addr.wstr_ptr, email_addr.wstr_len);
                            all_number_len = all_number_len + email_addr.wstr_len;
                            MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                            all_number_len = all_number_len +1;
                        }
                        cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                        SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                        //按普通的号码处理
                        cursor_numberlist_ptr->is_object = FALSE;
                        cursor_numberlist_ptr->number_len = email_addr.wstr_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            email_addr.wstr_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                        if(PNULL == add_numberlist_ptr)
                        {
                            g_mms_edit_globle.editdoc->numberlist_header_ptr = cursor_numberlist_ptr;
                        }
                        else
                        {
                            temp_numberlist_ptr = add_numberlist_ptr;
                            while(PNULL != temp_numberlist_ptr->next)
                            {
                                temp_numberlist_ptr = temp_numberlist_ptr->next;
                            }
                            temp_numberlist_ptr->next = cursor_numberlist_ptr;
                        }
                    }                
                }  
                //显示添加号码


                //ToListBoxSetArray(ctrl_id, add_numberlist_ptr);
                //free add_numberlist_ptr

                //把得到的号码加入edit document中去
                //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7534_112_2_18_2_37_54_61,(uint8*)"");
                to_edit_string.wstr_ptr = all_number_wstr;
                to_edit_string.wstr_len = MMIAPICOM_Wstrlen(all_number_wstr);
                SetMMSEditIsSaveDraft(FALSE); 
                MMIMMS_SetMMSTo(&to_edit_string,edit_doc_ptr);

                if (PNULL != email_addr.wstr_ptr)
                {
                    MMIMMS_FREE(email_addr.wstr_ptr);
                    email_addr.wstr_ptr = NULL;
                }
                if (PNULL != number_name_addr.wstr_ptr)
                {
                    SCI_FREE(number_name_addr.wstr_ptr);
                    number_name_addr.wstr_ptr = NULL;
                }
                MMIMMS_FREE(all_number_wstr);
                all_number_wstr = PNULL;
            }
        }
        break;

        case MMI_CL_MULTI_SELECT_LIST_RETURN:
            MmsHandleCallogSelectReturn(edit_doc_ptr);
        break;
#endif        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }

    
    return err_code;
}
#endif
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :mms edit enter corresponding richedit  items
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditEnterCorrespondItem(MMI_CTRL_ID_T ctrl_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    uint16 richedit_index = 0;
    
    if (!GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
    {
        return;
    }
    if (MMIMMS_RICHTEXT_TO == g_mms_edit_richtext_item_type[richedit_index])
    {
#ifdef MMI_PDA_SUPPORT 
        GUIRICHTEXT_FOCUS_ELEM_T element = {0};
        GUIRICHTEXT_GetFocusElement(ctrl_id,&element);
        if(element.type == GUIRICHTEXT_FOCUS_TXT_NORMAL)
        {
          SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_ADD_RECV);
          MMIMMS_CreateToListEditWin();
        }
        else
        {
                uint16 num = 0;
                num = MMIMMS_GetEditNameNum();
                /*MMIAPIPB_OpenNumberList(
                MMIPB_LIST_FOR_MMS,
                MMIPB_LIST_CHECK,
                (uint16)MMIMMS_MAX_RECEIVER_NUM - num,
                PNULL
                );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,//lint !e655
                        (uint16)MMIMMS_MAX_RECEIVER_NUM - num,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        EditSelectContactCallback
                        );
        }
#else
		SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_ADD_RECV);
        MMIMMS_CreateToListEditWin();
#endif
    }
    else if (MMIMMS_RICHTEXT_SUBJECT == g_mms_edit_richtext_item_type[richedit_index])
    {
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
        MMIMMS_CreateSubjectEditWin();
    }
    else if (MMIMMS_RICHTEXT_TEXT == g_mms_edit_richtext_item_type[richedit_index])
    {
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
        MMK_CreateWin((uint32*)MMIMMS_EDIT_TEXT_TAB, PNULL);
    }
    else if (MMIMMS_RICHTEXT_PIC_OR_MOVIE == g_mms_edit_richtext_item_type[richedit_index])
    {
        if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
        {
            MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return;
        }        
        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag || 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
        {
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSIMAGE);            
            MMK_CreateWin((uint32 *)MMIMMS_EDIT_MULTI_POP_WIN_TAB,PNULL);
        }
        else 
        {
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
            MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
        }
    }
    else if (MMIMMS_RICHTEXT_MUSIC == g_mms_edit_richtext_item_type[richedit_index])
    {
        if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
        {
            MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return;
        }         
        if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
        {
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSMUSIC);
            MMK_CreateWin((uint32 *)MMIMMS_EDIT_MULTI_POP_WIN_TAB,PNULL);
        }
        else
        {
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
            MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
        }
    }
    else if (MMIMMS_RICHTEXT_ADDMENU == g_mms_edit_richtext_item_type[richedit_index])
    {
        if(IsCanAddMediumData())
        {
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_OPEN_POP);
            OpenMMSEditAddPopMenu();
        }
        else
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);
        }
    } 
    else if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[richedit_index])
    {
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSADDFILE);
        MMK_CreateWin((uint32 *)MMIMMS_EDIT_MULTI_POP_WIN_TAB,PNULL);
    }
}
#endif

#ifndef MMIMMS_SMS_IN_1_SUPPORT 
/*****************************************************************************/
//  Description :mms edit richedit add multimedia file handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditAddMenuWinMsg(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_RESULT_E                 err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = MMIMMS_EDIT_ADDMENU_CTRL_ID;
    uint32 node_id                        = 0;
    uint16 slide_num                      = 0;
    uint16 cur_slide_index                = 0;

    edit_doc_ptr = g_mms_edit_globle.editdoc;

    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditAddMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7698_112_2_18_2_37_54_62,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //弹出popmenu
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入文件管理器界面选择文件
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch ( node_id)
        {
        case MMS_CONTEDIT_OPT_NODE_INDC:
            MMSEditInsertDCPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
            break;
        case MMS_CONTEDIT_OPT_NODE_INSTEXT:
            MMK_CreateWin((uint32*)MMIMMS_EDIT_TEXT_TAB, PNULL);
            break;
        case MMS_CONTEDIT_OPT_NODE_INSIMAGE:
            {
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else 
                {
                    MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                }                
            }            
            break;
        case MMS_CONTEDIT_OPT_NODE_INSMUSIC:
            {
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                }
            }            
            break;
        case MMS_CONTEDIT_OPT_NODE_INSVIDEO:
            {
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditLoseFocusIndex());//cr182924
                    SetMMSEditPreFocusIndex(GetMMSEditPreFocusIndex());
                    MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                }
            }  
            break;
        case MMS_CONTEDIT_OPT_NODE_INSSLIDE:
            {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);

                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (1 == cur_slide_index)
                {
                    // add next/pre 时MMIMMS_RICHTEXT_TO早已对用户不可见，代码永远失效，故if 0掉,时间验证后待删除该代码
#if 0
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
#endif
                    //该页是首页，next后不显示to和subject
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT)-2);
                }
                else
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
                }
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    }                    
                }
                else
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    MMK_CloseWin(win_id);
                }
            }
            break;
        case MMS_CONTEDIT_OPT_NODE_INSSLIDER:
            {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                
                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (1 == cur_slide_index)
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
                }
                else
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
                }
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,0);
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    }                    
                }
                else
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    MMK_CloseWin(win_id);
                }
            }
            break; 
#if defined (MMI_MMS_MINI_SUPPORT) || defined(CMCC_UI_STYLE)
         case MMS_CONTEDIT_OPT_NODE_INSADDFILE:
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                //插入附件
                //if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                {
                    //MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                    MMK_CreateWin((uint32 *)MMIMMS_ATTACHMENT_WIN_TAB,PNULL);
                    MMK_CloseWin(win_id);
                }
            }            
            break;
#endif            
        default :
            break;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :mms edit main sms 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditMultiPopWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_RESULT_E                 err_code = MMI_RESULT_TRUE;
    uint32   node_id                      = 0;
    MMI_CTRL_ID_T menu_ctrl_id            = MMIMMS_EDIT_MULTI_POP_CTRL_ID;
    uint16 lose_focus_index               = MMIMMS_RICHTEXT_ITEM_NUM_MAX;
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    lose_focus_index = GetMMSEditLoseFocusIndex();
 
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7868_112_2_18_2_37_55_63,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //弹出popmenu
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入文件管理器界面选择文件
        node_id = GUIMENU_GetCurNodeId(menu_ctrl_id);
        switch (node_id)
        {
        case MMS_CONTEDIT_OPT_NODE_ITEM_REPLACE:
            {
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (MMS_CONTEDIT_OPT_NODE_INSIMAGE == GetMMSEditLoseFocusReason())
                {
                    if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                    {
                        MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, TRUE);
                    } 
                    else if(1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                    {
                        MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, TRUE);
                    }
                }
                else if (MMS_CONTEDIT_OPT_NODE_INSMUSIC == GetMMSEditLoseFocusReason())
                {
                    if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                    {
                        MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, TRUE);
                    }
                }
                else if (MMS_CONTEDIT_OPT_NODE_INSADDFILE == GetMMSEditLoseFocusReason())
                {
                    MMIMMS_CreateAttachListWin();
                    //MMK_CreateWin((uint32 *)MMIMMS_ATTACHMENT_WIN_TAB,PNULL);
                }
            }            
            break;
        case MMS_CONTEDIT_OPT_NODE_ITEM_DELETE:
            {
                if (MMS_CONTEDIT_OPT_NODE_INSMUSIC == GetMMSEditLoseFocusReason())
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELMUSIC);
                }
                else if (MMS_CONTEDIT_OPT_NODE_INSIMAGE == GetMMSEditLoseFocusReason())
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELIMAGE);
                }
                else if(MMS_CONTEDIT_OPT_NODE_INSVIDEO== GetMMSEditLoseFocusReason())
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELVIDEO);
                }
                else if (MMS_CONTEDIT_OPT_NODE_INSADDFILE == GetMMSEditLoseFocusReason())
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELADDFILE);
                    MMIMMS_CreateAttachListWin();                     
                    break;
                }
                else
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELETE);
                }
                SetMMSEditIsOpPopMenu(TRUE);
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            }            
            break;               
        default :
            break;
        }
        MMK_CloseWin(win_id);
        break;     
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}
#else
/*****************************************************************************/
//  Description :mms edit main richedit win edit multimedia file pop menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditMultiPopWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_RESULT_E                 err_code = MMI_RESULT_TRUE;
    uint32     group_id                   = 0;
    uint32   menu_id                      = 0;
    MMI_CTRL_ID_T menu_ctrl_id            = MMIMMS_EDIT_MULTI_POP_CTRL_ID;
    uint16 lose_focus_index               = MMIMMS_RICHTEXT_ITEM_NUM_MAX;
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

    edit_doc_ptr = g_mms_edit_globle.editdoc;
    lose_focus_index = GetMMSEditLoseFocusIndex();
 
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_7985_112_2_18_2_37_55_64,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //弹出popmenu
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入文件管理器界面选择文件
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch (menu_id)
        {
        case ID_MMS_MULTI_REPLACE:
            {
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (MMIMMS_RICHTEXT_PIC_OR_MOVIE == g_mms_edit_richtext_item_type[lose_focus_index])
                {
                    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                    {
                        MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, TRUE);
                    }
                    else if(1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                    {
                        MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, TRUE);
                    } 
                }
                else if (MMIMMS_RICHTEXT_MUSIC == g_mms_edit_richtext_item_type[lose_focus_index])
                {
                    if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                    {
                        MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, TRUE);
                    }
                }
                else if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[lose_focus_index])
                {
                    SetMMSEditIsReplaceMultifile(TRUE);
                    MMK_CreateWin((uint32 *)MMIMMS_ATTACHMENT_WIN_TAB,PNULL);
                }
            }            
            break;
        case ID_MMS_MULTI_DELETE:
            {
                if (MMS_CONTEDIT_OPT_NODE_INSMUSIC == GetMMSEditLoseFocusReason())
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELMUSIC);
                }
                else if (MMS_CONTEDIT_OPT_NODE_INSIMAGE == GetMMSEditLoseFocusReason())
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELIMAGE);
                }
                else if (MMS_CONTEDIT_OPT_NODE_INSADDFILE == GetMMSEditLoseFocusReason())
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELADDFILE);
                }
                else
                {
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELETE);
                }
                SetMMSEditIsOpPopMenu(TRUE);
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            }            
            break;               
        default :
            break;
        }
        MMK_CloseWin(win_id);
        break;     
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}
#endif

/*****************************************************************************/
// 	Description : HandleMMSEditMainMenuOptDelWinMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSEditMainMenuOptDelWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
        {
            MMIPUB_INFO_T *win_info_ptr = PNULL;
            MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            BOOLEAN is_mms_mode = FALSE;
#endif

            edit_doc_ptr = g_mms_edit_globle.editdoc;
            win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

            if (PNULL != win_info_ptr && PNULL != edit_doc_ptr)
            {
                uint32 node_id = (uint32)win_info_ptr->user_data;

                switch (node_id)
                {
                case MMS_CONTEDIT_OPT_NODE_INSIMAGE:
                    MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                    break;

                case MMS_CONTEDIT_OPT_NODE_INSMUSIC:
                    MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                    break;

                case MMS_CONTEDIT_OPT_NODE_INSVIDEO:
                    MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                    break;

                case MMS_CONTEDIT_OPT_NODE_DELSLIDE:
                    SetMMSEditLoseFocusIndex(GetMMSEditLoseFocusIndex());
                    SetMMSEditIsSaveDraft(FALSE);
                    MMIMMS_DelSlide(edit_doc_ptr);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    is_mms_mode = MMIMMS_CheckIsMMS();  

                    if(!is_mms_mode)
                    {                        
                        while (PNULL != edit_doc_ptr->editbody_ptr->accessory_ptr)
                        {
                            MMIMMS_DelAttachment(edit_doc_ptr->editbody_ptr->accessory_ptr->filename, edit_doc_ptr);
                            if (PNULL != edit_doc_ptr->editbody_ptr->accessory_ptr)
                            {
                                edit_doc_ptr->editbody_ptr->accessory_ptr = edit_doc_ptr->editbody_ptr->accessory_ptr->next;
                            }
                        }
                        MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                            PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
                    }
                    else
                    {   
#ifdef MMI_PDA_SUPPORT
                        HandleSendButtonDisplay(TRUE);
#endif
                        MMSShowEditContent();   
                        MMSShowMultiFileInfo();
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
                    }
#else
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
#endif
                    break;

                case MMS_CONTEDIT_OPT_NODE_DELTEXT:
                    SetMMSEditIsSaveDraft(FALSE);
                    MMIMMS_DelText(edit_doc_ptr);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
                    break;

                case MMS_CONTEDIT_OPT_NODE_DELIMAGE:
                    SetMMSEditIsSaveDraft(FALSE);
                    MMIMMS_DelImage(edit_doc_ptr);

#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    is_mms_mode = MMIMMS_CheckIsMMS();  
                    if(!is_mms_mode)
                    {
                        MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
                    } 
                    else
#endif
                    {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT 
                        HandleSendButtonDisplay(TRUE);
#endif
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
                    }
                    break;

                case MMS_CONTEDIT_OPT_NODE_DELMUSIC:
                    SetMMSEditIsSaveDraft(FALSE);
                    MMIMMS_DelMusic(edit_doc_ptr);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    is_mms_mode = MMIMMS_CheckIsMMS();  
                    if(!is_mms_mode)
                    {
                        MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                            PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
                    }  
                    else
#endif
                    {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT             
                        HandleSendButtonDisplay(TRUE);
#endif
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
                    }
                    break;

                case MMS_CONTEDIT_OPT_NODE_DELVIDEO:
                    SetMMSEditIsSaveDraft(FALSE);
                    MMIMMS_DelVideo(edit_doc_ptr);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    is_mms_mode = MMIMMS_CheckIsMMS();  
                    if(!is_mms_mode)
                    {
                        MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                            PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
                    }       
                    else
#endif
                    {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT             
                        HandleSendButtonDisplay(TRUE);
#endif
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
                        MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
                    }
                    break;

                default :
                    break;
                }
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);    
}

/*****************************************************************************/
//  Description :mms edit main richedit win opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditMainMenuOptWinMsg(
                                                   MMIMMS_WINDOW_ID_E win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                              )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_RESULT_E  err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = MMIMMS_EDIT_MAINMENU_OPT_CTRL_ID;
    uint32 node_id                        = 0;
    uint16 slide_num                      = 0;
    MMIMMS_ERROR_E   mms_op_result        = MMIMMS_NO_ERROR; 
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    uint16 cur_slide_index                = 0;
#endif
    MMIMMS_WINDOW_ID_E quer_win_id        = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    BOOLEAN is_mms_mode = FALSE;
    MMI_STRING_T      mms_text_string = {0};
    MMIMMS_ERROR_E edit_err = MMIMMS_NO_ERROR;
#endif
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMainMenuOptWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_8100_112_2_18_2_37_55_65,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //打开popmenu
        SetMMSEditIsReplaceMultifile(FALSE); 
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入选项               
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        SCI_MEMSET(&mms_text_string, 0, sizeof(MMI_STRING_T));
        GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &mms_text_string);
        MMIMMS_AddText(edit_doc_ptr, &mms_text_string);
#endif
        
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (node_id)
        {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        case MMS_CONTEDIT_OPT_NODE_INSERT_CONTACT_INFO:
            //SCI_TRACE_LOW:"MMISMS: HandleEditMenuWinMsg call MMIAPIPB_OpenNumberList"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_8124_112_2_18_2_37_55_66,(uint8*)"");
//             MMIAPIPB_OpenNumberList(
//                 MMIPB_LIST_FOR_MMS,
//                 MMIPB_LIST_TYPE_SINGLE,
//                 1,
//                 PNULL
//                 );
            MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_RADIO, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_WHOLE,
                1,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                ListSelectContactCallback
                );
            SetMMSEditAddContactType(MMIMMS_ADD_CONTENT_CONTACT);
            MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
            MMK_CloseWin(win_id);    
            break;
            
        case MMS_CONTEDIT_OPT_NODE_INSERT_PHIZ:
            MMK_CreatePubListWin((uint32 *)MMIMMS_PHIZLIST_WIN_TAB, PNULL);
            MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
            MMK_CloseWin(win_id);    
            break;
            
        case MMS_CONTEDIT_OPT_NODE_INSERT_SUBJECT:
            edit_err = MMIMMS_EnterEditCheck();
            if (MMIMMS_NO_ERROR != edit_err)
            {
                //提示彩信初始化失败
                MMK_CloseWin(win_id); 
                MMIMMS_EditMMSAlertWin(edit_err);
                break;
            }
    
            SetMMSEditIsSubjectExist(TRUE);
            if(!MMIMMS_GetIsMMSMode())
            {
                //MMIMMS_InitData();
                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)node_id);            
            } 
            GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID);
            MMK_CloseWin(win_id);    
            break;
            
        case MMS_CONTEDIT_OPT_NODE_DEL_SUBJECT:
            SetMMSEditIsSubjectExist(FALSE);
            if(MMIMMS_GetIsMMSMode())
            {
                BOOLEAN ret = FALSE;
                ret = MMIMMS_CheckIsMMS();  
                if(!ret)
                {
                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                    PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
                }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT                
                else
                {
                    HandleSendButtonDisplay(TRUE);
                }     
#endif
#endif
            }
            GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID);
            MMK_CloseWin(win_id);    
            break;
        case MMS_CONTEDIT_OPT_NODE_OPEN_TIMERMSG:
#ifdef MMI_TIMERMSG_SUPPORT                
                MMIAPISMS_CreateTimerMsgWin();
#endif
                MMK_CloseWin(win_id);
                break;
        
        case MMS_CONTEDIT_OPT_NODE_CLOSE_TIMERMSG:
#ifdef MMI_TIMERMSG_SUPPORT       
                MMIAPISMS_SetIsTimerMode(FALSE);
#endif
                MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MMISMS_MSG_UPDATE_TIMER, PNULL, 0);
                MMK_CloseWin(win_id);
                break;
#ifdef MMIMMS_SMS_CC_SUPPORT    
        case MMS_CONTEDIT_OPT_NODE_INSCC:
            SetMMSEditIsCCExist(TRUE);
            if(!MMIMMS_GetIsMMSMode())
            {
            //MMIMMS_InitData();
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)node_id);          
            } 
            GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_CC_TO_LIST_ID);
            MMK_CloseWin(win_id);                
            break;
            
        case MMS_CONTEDIT_OPT_NODE_DELCC:
            SetMMSEditIsCCExist(FALSE);
            if(MMIMMS_GetIsMMSMode())
            {
                BOOLEAN ret = FALSE;
                ret = MMIMMS_CheckIsMMS();  
                if(!ret)
                {
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
                }  
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT                  
                else
                {
                    HandleSendButtonDisplay(TRUE);
                }
#endif
#endif                
            }
            GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_CC_TO_LIST_ID);
            MMK_CloseWin(win_id);      
            break;
            
        case MMS_CONTEDIT_OPT_NODE_INSBCC:
            SetMMSEditIsBCCExist(TRUE);
            if(!MMIMMS_GetIsMMSMode())
            {
                //MMIMMS_InitData();
                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                    PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);         
            } 
            GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID);
            MMK_CloseWin(win_id);      
            break;
        
        case MMS_CONTEDIT_OPT_NODE_DELBCC:
            SetMMSEditIsBCCExist(FALSE);
            if(MMIMMS_GetIsMMSMode())
            {
                BOOLEAN ret = FALSE;
                ret = MMIMMS_CheckIsMMS();  
                if(!ret)
                {
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)node_id); 
                }
#ifdef MMIMMS_SMS_IN_1_SUPPORT                
#ifdef MMI_PDA_SUPPORT
                else
                {
                    HandleSendButtonDisplay(TRUE);
                }
#endif
#endif                
            }
            GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID);
            MMK_CloseWin(win_id);      
            break;
#endif            
        case MMS_CONTEDIT_OPT_NODE_DELADDFILE:
        {
            MMK_CloseWin(win_id);  
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELADDFILE);
            MMIMMS_CreateAttachListWin();                   
        }            
        break;        
#else
case MMS_CONTEDIT_OPT_NODE_INSERT_PHIZ:
            MMK_CreatePubListWin((uint32 *)MMIMMS_PHIZLIST_WIN_TAB, PNULL);
            MMK_SetAtvCtrl(win_id, MMIMMS_EDIT_MENU_CTRL_ID);
            MMK_CloseWin(win_id);    
            break;
#endif
            
        case MMS_CONTEDIT_OPT_NODE_SEND:
#ifndef MMIMMS_SMS_IN_1_SUPPORT            
            if (PNULL != edit_doc_ptr->edit_to.wstr_ptr && 0 != edit_doc_ptr->edit_to.wstr_len)
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SEND);
                EditAndSendNewMMS();
            }  
            else
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_ADD_RECV);
                MMIMMS_CreateToListEditWin();
            }
#else
            if(MMIMMS_GetIsMMSMode()) //彩信发送方式
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SEND);
                //MMIAPISMS_ClearDestNum();
                EditAndSendNewMMS();
            }
            else 
            {
                HandleMMSEditSendSms(win_id);//发送短信的流程
            }
#endif
            MMK_CloseWin(win_id);           
            break;

#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifndef MMI_PDA_SUPPORT
        case MMS_CONTEDIT_OPT_NODE_INSERT_TEMPLATE:
            MMK_CloseWin(win_id); 
            MMIAPISMS_CreateSelectWordWin(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
            break;
            
        case MMS_CONTEDIT_OPT_NODE_INSERT:
            MMK_CloseWin(win_id); 
#ifndef MMI_GUI_STYLE_TYPICAL
            MMK_CreateWin((uint32*)MMIMMS_EDIT_MAINMENU_INSERT_OPT_WIN_TAB, PNULL);
#endif
            break;
#endif
#else
#ifndef MMI_PDA_SUPPORT
        case MMS_CONTEDIT_OPT_NODE_INSERT_TEMPLATE:
            MMK_CloseWin(win_id); 
            MMIAPISMS_CreateSelectWordWin(MMIMMS_EDIT_MENU_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMIMMS_EDIT_MENU_CTRL_ID);
            break;
#endif
#endif


        case MMS_CONTEDIT_OPT_NODE_ADD_RECV:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_ADD_RECV);
            MMIMMS_CreateToListEditWin();
            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_PREV:
            {
                #ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMI_STRING_T text_str_info = {0};
                edit_doc_ptr = g_mms_edit_globle.editdoc;
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_PREV);
                GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&text_str_info); //加入当前的文本信息
                MMIMMS_AddText(edit_doc_ptr, &text_str_info);
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                MMIMMS_CreateMMSPreviewWin(edit_doc_ptr, MMS_PREVIEW);
                MMK_CloseWin(win_id);
                #else
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_PREV);
                GUIRICHTEXT_DeleteAllItems(MMIMMS_EDIT_MENU_CTRL_ID);
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);                
                MMIMMS_CreateMMSPreviewWin(edit_doc_ptr, MMS_PREVIEW);
                MMK_CloseWin(win_id);
                #endif
            }

            break;
        case MMS_CONTEDIT_OPT_NODE_INSTEXT:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
            MMK_CreateWin((uint32*)MMIMMS_EDIT_TEXT_TAB, PNULL);
            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_DELTEXT:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELETE);
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
            {
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,HandleMMSEditMainMenuOptDelWinMsg,(uint32)node_id);
            }
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_NO_CONTENT, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }

            MMK_CloseWin(win_id);
            break;

        case MMS_CONTEDIT_OPT_NODE_INSIMAGE:
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else 
                {
                    MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                    MMK_CloseWin(win_id);
                }                
            }            
            break;
        case MMS_CONTEDIT_OPT_NODE_INSMUSIC:
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr,FALSE);
                    MMK_CloseWin(win_id);
                }
            }            
            break;
        case MMS_CONTEDIT_OPT_NODE_INSVIDEO:
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                    MMK_CloseWin(win_id);
                }
            }            
            break;
        case MMS_CONTEDIT_OPT_NODE_INSADDFILE:
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                //插入附件
                //if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                {
                    //MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                    MMK_CreateWin((uint32 *)MMIMMS_ATTACHMENT_WIN_TAB,PNULL);
                    MMK_CloseWin(win_id);
                }
            }            
            break;
        case MMS_CONTEDIT_OPT_NODE_INSSLIDE:
            {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
#ifndef MMIMMS_SMS_IN_1_SUPPORT
                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (1 == cur_slide_index)
                {
                    // add next/pre 时MMIMMS_RICHTEXT_TO早已对用户不可见，代码永远失效，故if 0掉,时间验证后待删除该代码
#if 0
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
#endif
                    //该页是首页，next后不显示to和subject
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT)-2);
                }
                else
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
                }
#endif                
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    } 
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    MMK_CloseWin(win_id);
                }
            }
            break;
        case MMS_CONTEDIT_OPT_NODE_INSSLIDER:
            {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
#ifndef MMIMMS_SMS_IN_1_SUPPORT                
                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (1 == cur_slide_index)
                {
                    // add next/pre 时MMIMMS_RICHTEXT_TO早已对用户不可见，代码永远失效，故if 0掉,时间验证后待删除该代码
#if 0
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
#endif
                    //该页是首页，next后不显示to和subject
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT)-2);
                }
                else
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
                }
#endif                
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,0);
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    } 
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    MMK_CloseWin(win_id);
                }
            }
            break;
        case MMS_CONTEDIT_OPT_NODE_INDC:
            MMSEditInsertDCPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_RECORD_SUPPORT            
        case MMS_CONTEDIT_OPT_NODE_INAU:
            MMK_CloseWin(win_id);
            MMSOpenRecord();
            break;
#endif
            
        case MMS_CONTEDIT_OPT_NODE_INDV:
            MMSEditInsertDV(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
            MMK_CloseWin(win_id);
            break;

        case MMS_CONTEDIT_OPT_NODE_DELIMAGE:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELETE);
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
            {
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,HandleMMSEditMainMenuOptDelWinMsg,(uint32)node_id);
            }
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_NO_CONTENT, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }

            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_DELMUSIC:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELETE);
            if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
            {
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,HandleMMSEditMainMenuOptDelWinMsg,(uint32)node_id);
            }
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_NO_CONTENT, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }

            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_DELVIDEO:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELETE);
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
            {
                MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,HandleMMSEditMainMenuOptDelWinMsg,(uint32)node_id);
            }
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_NO_CONTENT, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }

            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_DELSLIDE:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_DELETE);
            MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,HandleMMSEditMainMenuOptDelWinMsg,(uint32)node_id);

            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_SETTIME:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SETTIME);
            MMIMMS_CreateSlideDurationEditWin();
            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT:
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            if(MMIMMS_GetIsMMSMode())
#endif    			
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMSEditAssembleInfo();
#endif
                mms_op_result = MMIMMS_SaveMMS(MN_DUAL_SYS_1, MMISMS_MO_DRAFT, edit_doc_ptr);
                if (MMIMMS_NO_ERROR != mms_op_result)
                {
                    MMIMMS_EditMMSAlertWin(mms_op_result);
                    MMK_CloseWin(win_id);
                    break;
                }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                else
                {
                    if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS == MMIMMS_GetMMSEditEnterReason())
                    {
                        MMSDeleteDraftSMS();            
                    }
                }
#endif
                SetMMSEditIsSaveDraft(TRUE);
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS); 
#endif
                MMK_CloseWin(win_id);
                MMIMMS_CloseEditWin();
            }       
#ifdef MMIMMS_SMS_IN_1_SUPPORT     
            else
            {
                MMSSaveSmsDraft(win_id);
				MMIMMS_CloseEditWin();
            }
#endif
            break;
        case MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST:
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST);
            MMIMMS_SetTextImageOrder(edit_doc_ptr, MMS_ORDER_TEXT_FIRST);            
            MMK_CloseWin(win_id);
            break;
        case MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST: 
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST);
            MMIMMS_SetTextImageOrder(edit_doc_ptr, MMS_ORDER_IMAGE_FIRST);
            MMK_CloseWin(win_id);
            break;
        default :
            break;
        }        
        break;
    case MSG_PROMPTWIN_OK:
        //删除图片，音乐，当前页，视频
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        MMK_CloseWin(quer_win_id);
        switch (node_id)
        {
        case MMS_CONTEDIT_OPT_NODE_INSIMAGE:
            if (PNULL != edit_doc_ptr)
            {
                MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
            }
            break;
        case MMS_CONTEDIT_OPT_NODE_INSMUSIC:
            if (PNULL != edit_doc_ptr)
            {
                MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
            }
            break;
        case MMS_CONTEDIT_OPT_NODE_INSVIDEO:
            if (PNULL != edit_doc_ptr)
            {
                MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
            }
            break;
        case MMS_CONTEDIT_OPT_NODE_DELSLIDE:
            SetMMSEditLoseFocusIndex(GetMMSEditLoseFocusIndex());
            //SetMMSEditFocusOnAddMenu(FALSE);
            SetMMSEditIsSaveDraft(FALSE);
            MMIMMS_DelSlide(edit_doc_ptr);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            is_mms_mode = MMIMMS_CheckIsMMS();  

            if(!is_mms_mode)
            {
                MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                    PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
            }
            else
            {   
#ifdef MMI_PDA_SUPPORT            
                HandleSendButtonDisplay(TRUE);
#endif
                MMSShowEditContent();   
                MMSShowMultiFileInfo();
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
            }
#else
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
#endif
            break;
        case MMS_CONTEDIT_OPT_NODE_DELTEXT:
            SetMMSEditIsSaveDraft(FALSE);
            MMIMMS_DelText(edit_doc_ptr);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
            break;
        case MMS_CONTEDIT_OPT_NODE_DELIMAGE:
            SetMMSEditIsSaveDraft(FALSE);
            MMIMMS_DelImage(edit_doc_ptr);

#ifdef MMIMMS_SMS_IN_1_SUPPORT
            is_mms_mode = MMIMMS_CheckIsMMS();  
            if(!is_mms_mode)
            {
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
            } 
            else
#endif
            {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT 
               HandleSendButtonDisplay(TRUE);
#endif
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
               MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
            }

                     
            break;
        case MMS_CONTEDIT_OPT_NODE_DELMUSIC:
            SetMMSEditIsSaveDraft(FALSE);
            MMIMMS_DelMusic(edit_doc_ptr);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            is_mms_mode = MMIMMS_CheckIsMMS();  
            if(!is_mms_mode)
            {
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
            }  
            else
#endif
            {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT             
                HandleSendButtonDisplay(TRUE);
#endif
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
            }
            break;
        case MMS_CONTEDIT_OPT_NODE_DELVIDEO:
            SetMMSEditIsSaveDraft(FALSE);
            MMIMMS_DelVideo(edit_doc_ptr);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            is_mms_mode = MMIMMS_CheckIsMMS();  
            if(!is_mms_mode)
            {
            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0); 
            }       
            else
#endif
            {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMI_PDA_SUPPORT             
                HandleSendButtonDisplay(TRUE);
#endif
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
            }
            break;
        default :
            break;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_PROMPTWIN_CANCEL:
        //close query window
        MMK_CloseWin(quer_win_id);        
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_CANCEL);
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        //返回编辑主界面        
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    
    return err_code;
}

/*****************************************************************************/
//  Description :list box add string
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void ToListBoxSetString(MMI_CTRL_ID_T ctrl_id, const wchar *str_ptr, const uint16 str_len)
{
    wchar * cur_wstr                              = PNULL;
    uint16 cur_to_len                             = 0;
    wchar cur_to_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    wchar *list_wstr                              = PNULL;
    uint16 temp_len                               = 0;
    uint16 num_len                                = 0;

    if (PNULL == str_ptr || 0 == str_len)
    {
        return;
    }
    temp_len = (str_len + 1) * sizeof(wchar);
    list_wstr = SCI_ALLOC_APP(temp_len);
    SCI_MEMSET(list_wstr, 0, temp_len);
    MMI_WSTRNCPY(list_wstr, str_len, str_ptr, str_len, str_len);

    cur_wstr = list_wstr;
    GUIEDIT_ClearAllStr(ctrl_id);
    while ('\0' != *cur_wstr)
    {
        cur_to_len++;
        if (';' == *cur_wstr || '\0' == *(cur_wstr + 1))
        {
            if (';' != *cur_wstr && '\0' == *(cur_wstr + 1))
            {
                num_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_to_len);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIMMS_MAX_ADDRESS_LEN, num_len);
                GUIEDIT_AddListItem(ctrl_id, cur_to_wstr, num_len);
                cur_to_len = 0;
            }
            else
            {
                num_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_to_len - 1);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIMMS_MAX_ADDRESS_LEN, num_len);
                GUIEDIT_AddListItem(ctrl_id, cur_to_wstr, num_len);
                cur_to_len = 0;
            }
        }
        cur_wstr = cur_wstr + 1;        
    }
    
    SCI_FREE(list_wstr);
    list_wstr = PNULL;
}

/*****************************************************************************/
//  Description :list box add array
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void ToListBoxSetArray(MMI_CTRL_ID_T ctrl_id, MMIMMS_EDIT_NUMBER_LIST_T *numberlist_header_ptr)
{
    GUIEDIT_LIST_ITEM_T list_item_array[MMIMMS_MAX_RECEIVER_NUM] = {0};
    uint16 number_num = 0;
    uint16 index = 0;
    MMIMMS_EDIT_NUMBER_LIST_T *cur_numberlist_ptr = numberlist_header_ptr;
    if(PNULL == numberlist_header_ptr)
    {
        return;
    }
    while(PNULL != cur_numberlist_ptr)
    {
        list_item_array[number_num].is_valid = TRUE;
        list_item_array[number_num].is_object = cur_numberlist_ptr->is_object;
        if(list_item_array[number_num].is_object)
        {
            list_item_array[number_num].display_str_len = cur_numberlist_ptr->number_name_len;
            list_item_array[number_num].display_str_ptr = SCI_ALLOC_APP((list_item_array[number_num].display_str_len+ 1)*sizeof(wchar));
            if(PNULL == list_item_array[number_num].display_str_ptr)
            {
                return;
            }
            SCI_MEMSET((char*)list_item_array[number_num].display_str_ptr, 0 , (list_item_array[number_num].display_str_len + 1)*sizeof(wchar));
            MMI_WSTRNCPY(list_item_array[number_num].display_str_ptr, list_item_array[number_num].display_str_len, 
                cur_numberlist_ptr->number_name_ptr, list_item_array[number_num].display_str_len, list_item_array[number_num].display_str_len);
            list_item_array[number_num].user_str_len= cur_numberlist_ptr->number_len;
            list_item_array[number_num].user_str_ptr= SCI_ALLOC_APP((list_item_array[number_num].user_str_len+ 1)*sizeof(wchar));
            if(PNULL == list_item_array[number_num].user_str_ptr)
            {
                SCI_FREE(list_item_array[number_num].display_str_ptr);
                list_item_array[number_num].display_str_ptr = PNULL;
                return;
            }
            SCI_MEMSET((char*)list_item_array[number_num].user_str_ptr, 0 , (list_item_array[number_num].user_str_len + 1)*sizeof(wchar));
            MMI_WSTRNCPY(list_item_array[number_num].user_str_ptr, list_item_array[number_num].user_str_len, 
                cur_numberlist_ptr->number_ptr, list_item_array[number_num].user_str_len, list_item_array[number_num].user_str_len);
        }
        else
        {
            list_item_array[number_num].display_str_len = cur_numberlist_ptr->number_len;
            list_item_array[number_num].display_str_ptr = SCI_ALLOC_APP((list_item_array[number_num].display_str_len+ 1)*sizeof(wchar));
            if(PNULL == list_item_array[number_num].display_str_ptr)
            {
             return;
            }
            SCI_MEMSET((char*)list_item_array[number_num].display_str_ptr, 0 , (list_item_array[number_num].display_str_len + 1)*sizeof(wchar));
            MMI_WSTRNCPY(list_item_array[number_num].display_str_ptr, list_item_array[number_num].display_str_len, 
                cur_numberlist_ptr->number_ptr, list_item_array[number_num].display_str_len, list_item_array[number_num].display_str_len);
        }        
        number_num++;
        cur_numberlist_ptr = cur_numberlist_ptr->next;
    }
    GUIEDIT_AddListItemArray(ctrl_id, list_item_array, number_num);
    for(index = 0; index < number_num; index++)
    {
        if(PNULL != list_item_array[index].display_str_ptr)
        {
            SCI_FREE(list_item_array[index].display_str_ptr);
            list_item_array[index].display_str_ptr = PNULL;
        }
        if(PNULL != list_item_array[index].user_str_ptr)
        {
            SCI_FREE(list_item_array[index].user_str_ptr);
            list_item_array[index].user_str_ptr = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description :set mms name list by to string /for draft box and sendfail box
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSNameListByTo(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr)
{
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr     = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *add_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    MMI_STRING_T  to_edit_string                  = {0};
    MMI_STRING_T  to_name_string                  = {0};
    wchar number_name_wstr[MMIPB_NAME_MAX_LEN + 1]= {0};
    MMIPB_BCD_NUMBER_T black_bcd_num              = {0};
    MMI_PARTY_NUMBER_T  party_num                 = {MN_NUM_TYPE_UNKNOW, 0, 0};
    BOOLEAN find_result                           = FALSE;
    wchar *list_wstr                              = PNULL;
    wchar * cur_wstr                              = PNULL;
    uint16 cur_to_len                             = 0;
    wchar cur_to_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    uint8 tele_num[MMIMMS_MAX_ADDRESS_LEN + 1]    = {0};
    uint16 num_len                                = 0;
    
    to_edit_string = editdoc_ptr->edit_to;
    if(PNULL == to_edit_string.wstr_ptr || 0 == to_edit_string.wstr_len)
    {
        return;
    }

    list_wstr = SCI_ALLOC_APP((to_edit_string.wstr_len + 1) * sizeof(wchar));
    SCI_MEMSET(list_wstr, 0, (to_edit_string.wstr_len + 1) * sizeof(wchar));
    MMI_WSTRNCPY(list_wstr, to_edit_string.wstr_len, to_edit_string.wstr_ptr, to_edit_string.wstr_len, to_edit_string.wstr_len);
    cur_wstr = list_wstr;
    while ('\0' != *cur_wstr)
    {
        cur_to_len++;
        if (';' == *cur_wstr || '\0' == *(cur_wstr + 1))
        {
            add_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
            SCI_MEMSET((char*)add_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
            if (';' != *cur_wstr && '\0' == *(cur_wstr + 1))
            {
                num_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_to_len);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIMMS_MAX_ADDRESS_LEN, num_len);
                cur_to_len = 0;
            }
            else
            {
                num_len = MIN(MMIMMS_MAX_ADDRESS_LEN, cur_to_len - 1);
                MMI_WSTRNCPY(cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIMMS_MAX_ADDRESS_LEN, num_len);
                cur_to_len = 0;
            }
            MMI_WSTRNTOSTR(tele_num, MMIMMS_MAX_ADDRESS_LEN, cur_to_wstr, MMIMMS_MAX_ADDRESS_LEN, num_len);
            MMIAPICOM_GenPartyNumber(tele_num, num_len, &party_num);
            black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
            SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
            to_name_string.wstr_ptr = number_name_wstr;
            find_result = MMIAPIPB_GetNameByNumber(&black_bcd_num, &to_name_string, MMIPB_NAME_MAX_LEN, FALSE);
            if(find_result && 0 < to_name_string.wstr_len)
            {//如果匹配名称，则显示名称
                add_numberlist_ptr->is_object = TRUE;
                add_numberlist_ptr->number_name_len = to_name_string.wstr_len;
                add_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((add_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                SCI_MEMSET((char *)add_numberlist_ptr->number_name_ptr, 0, (add_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                MMI_WSTRNCPY(add_numberlist_ptr->number_name_ptr, add_numberlist_ptr->number_name_len,
                to_name_string.wstr_ptr, add_numberlist_ptr->number_name_len, add_numberlist_ptr->number_name_len);

                add_numberlist_ptr->number_len = num_len;
                add_numberlist_ptr->number_ptr = SCI_ALLOC_APP((add_numberlist_ptr->number_len + 1) * sizeof(wchar));
                SCI_MEMSET((char *)add_numberlist_ptr->number_ptr, 0, (add_numberlist_ptr->number_len + 1) * sizeof(wchar));
                MMI_WSTRNCPY(add_numberlist_ptr->number_ptr, add_numberlist_ptr->number_len,
                cur_to_wstr, add_numberlist_ptr->number_len, add_numberlist_ptr->number_len);
            }
            else
            {
                add_numberlist_ptr->is_object = FALSE;
                add_numberlist_ptr->number_len = num_len;
                add_numberlist_ptr->number_ptr = SCI_ALLOC_APP((add_numberlist_ptr->number_len + 1) * sizeof(wchar));
                SCI_MEMSET((char *)add_numberlist_ptr->number_ptr, 0, (add_numberlist_ptr->number_len + 1) * sizeof(wchar));
                MMI_WSTRNCPY(add_numberlist_ptr->number_ptr, add_numberlist_ptr->number_len,
                cur_to_wstr, add_numberlist_ptr->number_len, add_numberlist_ptr->number_len);
            }
            if(PNULL == numberlist_ptr)
            {
                numberlist_ptr = add_numberlist_ptr;
            }
            else
            {
                temp_numberlist_ptr = numberlist_ptr;
                while(PNULL != temp_numberlist_ptr->next)
                {
                    temp_numberlist_ptr = temp_numberlist_ptr->next;
                }
                temp_numberlist_ptr->next = add_numberlist_ptr;
            }
        }
        cur_wstr = cur_wstr + 1; 
    }
    MMIMMS_SetMMSToNameList(numberlist_ptr, editdoc_ptr);
    SCI_FREE(list_wstr);
    list_wstr = PNULL;
}

/*****************************************************************************/
//  Description :mms edit main richedit add to menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditToListWinMsg(
                                              MMIMMS_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_CTRL_ID_T ctrl_id                        = MMIMMS_EDIT_TO_LIST_CTRL_ID;
    MMI_RESULT_E err_code                        = MMI_RESULT_TRUE;
    uint16  to_number                            = 0;
    MMI_STRING_T  to_edit_string                 = {0};
    MMIPB_BCD_NUMBER_T pb_bcd_number             = {0};
    MMI_STRING_T email_addr                      = {0};
    uint16 pb_select_num                         = 0;
    uint16  pb_select_index                      = 0;
    uint32  max_number_len                       = 0;
    uint8  tele_len                              = 0;
    uint8  tele_num[MMIMMS_MAX_ADDRESS_LEN + 2]  = {0};
    wchar  tele_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    wchar  *all_number_wstr                      = PNULL;
    uint16   all_number_len                      = 0;
    wchar semicolon[]                            = {';', 0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr        = PNULL;
    uint16  temp_len                             = 0;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr         = PNULL;
    uint16 index                                 = 0;
    uint16 num_index                             = 0;
    uint8 is_have_empty                          = 0;
    wchar num_temp[MMIMMS_MAX_ADDRESS_LEN + 1]   = {0};
    wchar empty_wstr[]                           = {' '};

    MMI_STRING_T number_name_addr                = {0};
    wchar number_name_wstr[MMIPB_NAME_MAX_LEN + 1] = {0};
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *add_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *cursor_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *free_numberlist_ptr = PNULL;
    GUIEDIT_LIST_ITEM_T       *item_array_ptr = PNULL;
    BOOLEAN is_same_num = FALSE;    
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL != edit_doc_ptr)
    {
        if (PNULL != edit_doc_ptr->edit_to.wstr_ptr && PNULL != edit_doc_ptr->numberlist_header_ptr)
        {
            to_edit_string = edit_doc_ptr->edit_to;
            numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
        }

        //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditToListWinMsg msg_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9003_112_2_18_2_37_57_67,(uint8*)"d", msg_id);
        switch(msg_id)
        {
        case MSG_FULL_PAINT:
            MMIMMS_CLEAR_CLIENT(win_id);
            break;
        case MSG_OPEN_WINDOW: 
            //todo 打开彩信收件人的输入界面，没有收件人的时候左软键是电话簿（中软键也是进入电话簿），右软键是返回
            //当有收件人的时候左软键是确认，中软键是添加，右软键是返回
            GUIEDIT_SetListStyle(ctrl_id, GUIEDIT_LIST_STYLE_ALL);
            //set font
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
            GUIEDIT_SetFont(ctrl_id, MMISMS_GetWinFont());
#else
#if defined (MAINLCD_SIZE_176X220)
            GUIEDIT_SetFont(ctrl_id,MMI_DEFAULT_TEXT_FONT);//SONG_FONT_18
#else
            GUIEDIT_SetFont(ctrl_id,SONG_FONT_18);
#endif
#endif

            if (PNULL != numberlist_ptr)
            {
                ToListBoxSetArray(ctrl_id, numberlist_ptr);
            } 
            MMK_SetAtvCtrl(win_id, ctrl_id);

			//to support Orange test case: default input method of sms/mms recipients should be GUIIM_TYPE_DIGITAL
			//to adjust setIm to execute after ctrl actived so as to enable setting effective          
            GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_SMART | GUIIM_TYPE_DIGITAL|GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC, GUIIM_TYPE_DIGITAL);/*lint !e655*/ 
            GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_EMAIL);
            break;
    
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
           if (PNULL != param)
           {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                
                switch (src_id)
                {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    SCI_MEMSET(((void *)&to_edit_string), 0, sizeof(MMI_STRING_T));
                    add_numberlist_ptr = PNULL;
                    to_number = GUIEDIT_GetListItemNum(ctrl_id);
                    if (to_number > MMIMMS_MAX_RECEIVER_NUM)
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_ADD_TO_FULL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                        break;
                    }
                    if (0 < to_number)
                    { 
                        num_str_ptr = SCI_ALLOC_APP(to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
                        if (PNULL == num_str_ptr)
                        {
                            break;
                        }
                        item_array_ptr = SCI_ALLOC_APP(to_number*sizeof(GUIEDIT_LIST_ITEM_T));
                        if(PNULL == item_array_ptr)
                        {
                            SCI_FREE(num_str_ptr);
                            num_str_ptr = PNULL;
                            break;
                        }
                        GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
                        GUIEDIT_GetListItemArray(ctrl_id,to_number,item_array_ptr);               
                        
                        all_number_len = 0;
                        for (index = 0; index < to_number; index++)
                        {
                            all_number_len = all_number_len + num_str_ptr[index].str_len;
                        }
                        if (0 == all_number_len)
                        {
                            if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
                            {
                                MMIMMS_FREE(edit_doc_ptr->edit_to.wstr_ptr);
                                edit_doc_ptr->edit_to.wstr_ptr = NULL;
                                edit_doc_ptr->edit_to.wstr_len = 0;
                            }
                            SCI_FREE(num_str_ptr);
                            num_str_ptr = PNULL;
                            SCI_FREE(item_array_ptr);
                            item_array_ptr = PNULL;
                            break;
                        }
                        temp_len = (all_number_len + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
                        //temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
                        all_number_wstr = (wchar *)MMIMMS_ALLOC(temp_len);            
                        if (PNULL == all_number_wstr)
                        {
                            //SCI_TRACE_LOW:"malloc email_addr failed"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9089_112_2_18_2_37_57_68,(uint8*)"");
                            SCI_FREE(num_str_ptr);
                            num_str_ptr = PNULL;
                            SCI_FREE(item_array_ptr);
                            item_array_ptr = PNULL;
                            MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                            break;
                        }
                        SCI_MEMSET(all_number_wstr, 0, temp_len);
                        all_number_len = 0;
                        
                        for (index = 0; index < to_number; index++)
                        {
                            tele_len = MIN(MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                            MMI_WSTRNCPY(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
                            
                            for (num_index = 0; num_index <= tele_len; num_index++)
                            {
                                MMI_WSTRNCPY(num_temp, MMIMMS_MAX_ADDRESS_LEN, tele_wstr, MMIMMS_MAX_ADDRESS_LEN, tele_len);
                                num_index = num_index - is_have_empty;
                                is_have_empty = 0;
                                if (0 == MMIAPICOM_Wstrncmp(&(tele_wstr[num_index]), empty_wstr, 1))
                                {
                                    tele_wstr[tele_len - 1] = 0;
                                    tele_len--;
                                    MMI_WSTRNCPY(&(tele_wstr[num_index]), MMIMMS_MAX_ADDRESS_LEN, 
                                        &(num_temp[num_index + 1]), MMIMMS_MAX_ADDRESS_LEN, tele_len - num_index);
                                    is_have_empty = 1;
                                }
                                SCI_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                            }
                            is_same_num = FALSE;
                            if (tele_len > 0)
                            {
                                temp_numberlist_ptr = add_numberlist_ptr;
                                while(PNULL != temp_numberlist_ptr)
                                {
                                    if (0 != temp_numberlist_ptr->number_len
                                        && temp_numberlist_ptr->number_len == tele_len
                                        && !MMIAPICOM_Wstrncmp(temp_numberlist_ptr->number_ptr, tele_wstr, tele_len))
                                    {
                                        is_same_num = TRUE;
                                        break;
                                    }
                                    else
                                    {
                                        temp_numberlist_ptr = temp_numberlist_ptr->next;
                                    }
                                    
                                }
                            }
                            if (tele_len > 0 && !is_same_num)
                            {
                                MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIMMS_ALL_ADDRESS_LEN, 
                                    tele_wstr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
                                all_number_len = tele_len + all_number_len;
                                if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
                                {
                                    MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                                    all_number_len++;
                                }
                                
                                cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                                SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                                cursor_numberlist_ptr->is_object = item_array_ptr[index].is_object;
                                if(cursor_numberlist_ptr->is_object)
                                {
                                    cursor_numberlist_ptr->number_name_len = item_array_ptr[index].display_str_len;
                                    cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                    MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                        item_array_ptr[index].display_str_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);
                                    
                                    cursor_numberlist_ptr->number_len = item_array_ptr[index].user_str_len;
                                    cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                        item_array_ptr[index].user_str_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                                }
                                else
                                {//手动输入需要匹配
                                    MMIPB_BCD_NUMBER_T black_bcd_num = {0};
                                    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
                                    BOOLEAN find_result = FALSE; 
                                    BOOLEAN ret = TRUE; 
                                    uint16 i   = 0;
                                    
                                    temp_len = item_array_ptr[index].display_str_len;
                                    MMI_WSTRNTOSTR(tele_num, MMIMMS_MAX_ADDRESS_LEN, item_array_ptr[index].display_str_ptr, temp_len, temp_len);
                                    for(i = 0; i < temp_len; i++)
                                    {
                                        if(!MMIAPICOM_IsTelephoneNum(tele_num[i]))
                                        {
                                            ret = FALSE;
                                            break;
                                        }                               
                                    }
                                    if(ret)
                                    {
                                        MMIAPICOM_GenPartyNumber(tele_num, temp_len, &party_num);
                                        black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                                        black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                                        SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
                                        number_name_addr.wstr_ptr = number_name_wstr;
                                        find_result = MMIAPIPB_GetNameByNumber(&black_bcd_num, &number_name_addr, MMIPB_NAME_MAX_LEN, FALSE);
                                        if(find_result && 0 < number_name_addr.wstr_len)
                                        {//如果匹配名称，则显示名称，不能够修改了
                                            cursor_numberlist_ptr->is_object = TRUE;
                                            
                                            cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                                            cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                            SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                            MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                            number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);                                                                                        
                                        }                                        
                                    }                                   
                                    cursor_numberlist_ptr->number_len = item_array_ptr[index].display_str_len;
                                    cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                    MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                    item_array_ptr[index].display_str_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                                                                        
                                }                        
                                if(PNULL == add_numberlist_ptr)
                                {
                                    add_numberlist_ptr = cursor_numberlist_ptr;
                                }
                                else
                                {
                                    temp_numberlist_ptr = add_numberlist_ptr;
                                    while(PNULL != temp_numberlist_ptr->next)
                                    {
                                        temp_numberlist_ptr = temp_numberlist_ptr->next;
                                    }
                                    temp_numberlist_ptr->next = cursor_numberlist_ptr;
                                }
                            }                   
                            
                            SCI_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                            SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
                          }
                            
                            //把得到的号码加入edit document中去
                            //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9225_112_2_18_2_37_58_69,(uint8*)"");
                            to_edit_string.wstr_ptr = all_number_wstr;
                            to_edit_string.wstr_len = MMIAPICOM_Wstrlen(all_number_wstr);
                            SetMMSEditIsSaveDraft(FALSE); 
                
                            if(MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == src_id)
                            {
                                GUIEDIT_ClearAllStr(ctrl_id);
                                ToListBoxSetArray(ctrl_id, add_numberlist_ptr);
                                /*MMIAPIPB_OpenNumberList(
                                    MMIPB_LIST_FOR_MMS,
                                    MMIPB_LIST_CHECK,
                                    (uint16)(MMIMMS_MAX_RECEIVER_NUM - to_number),
                                    PNULL
                                    );*/
                                MMIAPIPB_OpenMainWin(
                                        MMIPB_ALL_CONTACT,//
                                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                                        MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,//lint !e655
                                        (uint16)MMIMMS_MAX_RECEIVER_NUM - to_number,//如果是多选时，此项限制了多选的最大个数
                                        PNULL,
                                        ListSelectContactCallback
                                        );
                                while(PNULL != add_numberlist_ptr)
                                {//进入pb要释放空间
                                    if(PNULL != add_numberlist_ptr->number_name_ptr)
                                    {
                                        SCI_FREE(add_numberlist_ptr->number_name_ptr);
                                        add_numberlist_ptr->number_name_ptr = PNULL;
                                    }
                                    if(PNULL != add_numberlist_ptr->number_ptr)
                                    {
                                        SCI_FREE(add_numberlist_ptr->number_ptr);
                                        add_numberlist_ptr->number_ptr = PNULL;
                                    }
                                    free_numberlist_ptr = add_numberlist_ptr;
                                    add_numberlist_ptr = add_numberlist_ptr->next;
                                    SCI_FREE(free_numberlist_ptr);
                                    free_numberlist_ptr = PNULL;                    
                                }
                            }
                            else
                            {//保存的话不用释放空间，以后还要使用
                                MMIMMS_SetMMSTo(&to_edit_string,edit_doc_ptr);
                                MMIMMS_SetMMSToNameList(add_numberlist_ptr, edit_doc_ptr);
                                MMK_CloseWin(win_id);
                            }                    
                            
                            MMIMMS_FREE(num_str_ptr);
                            num_str_ptr = PNULL;
                            
                            SCI_FREE(item_array_ptr);
                            item_array_ptr = PNULL;
                            
                            MMIMMS_FREE(all_number_wstr);
                            all_number_wstr = PNULL;                    
                        }
                        else
                        {
                            if(MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == src_id)
                            {
                                 /*MMIAPIPB_OpenNumberList(
                                    MMIPB_LIST_FOR_MMS,
                                    MMIPB_LIST_CHECK,
                                    (uint16)MMIMMS_MAX_RECEIVER_NUM,
                                    PNULL
                                    );*/
                                MMIAPIPB_OpenMainWin(
                                        MMIPB_ALL_CONTACT,//
                                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                                        MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,//lint !e655
                                        (uint16)MMIMMS_MAX_RECEIVER_NUM ,//如果是多选时，此项限制了多选的最大个数
                                        PNULL,
                                        ListSelectContactCallback
                                        );
                            }
                            else
                            {
                                MMK_CloseWin(win_id);
                            }
                            if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
                            {
                                MMIMMS_FREE(edit_doc_ptr->edit_to.wstr_ptr);
                                edit_doc_ptr->edit_to.wstr_ptr = NULL;
                                edit_doc_ptr->edit_to.wstr_len = 0;
                            }
                            add_numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
                            while(PNULL != add_numberlist_ptr)
                            {
                                if(PNULL != add_numberlist_ptr->number_name_ptr)
                                {
                                    SCI_FREE(add_numberlist_ptr->number_name_ptr);
                                    add_numberlist_ptr->number_name_ptr = PNULL;
                                }
                                if(PNULL != add_numberlist_ptr->number_ptr)
                                {
                                    SCI_FREE(add_numberlist_ptr->number_ptr);
                                    add_numberlist_ptr->number_ptr = PNULL;
                                }
                                free_numberlist_ptr = add_numberlist_ptr;
                                add_numberlist_ptr = add_numberlist_ptr->next;
                                SCI_FREE(free_numberlist_ptr);
                                free_numberlist_ptr = PNULL;                    
                            }
                            edit_doc_ptr->numberlist_header_ptr = PNULL;
                        }

                    break;
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                                    //todo 返回编辑主界面
                    MMK_CloseWin(win_id);
                    break;            
                default:
                    break;
                }
            }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif              

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
#ifdef MMI_PDA_SUPPORT
        case MSG_KEYDOWN_MENU:
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        case MSG_APP_OK:
            SCI_MEMSET(((void *)&to_edit_string), 0, sizeof(MMI_STRING_T));
            add_numberlist_ptr = PNULL;
            to_number = GUIEDIT_GetListItemNum(ctrl_id);
            if (to_number > MMIMMS_MAX_RECEIVER_NUM)
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_ADD_TO_FULL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                break;
            }
            if (0 < to_number)
            { 
                num_str_ptr = SCI_ALLOC_APP(to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
                if (PNULL == num_str_ptr)
                {
                    break;
                }
                item_array_ptr = SCI_ALLOC_APP(to_number*sizeof(GUIEDIT_LIST_ITEM_T));
                if(PNULL == item_array_ptr)
                {
                    SCI_FREE(num_str_ptr);
                    num_str_ptr = PNULL;
                    break;
                }
                GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
                GUIEDIT_GetListItemArray(ctrl_id,to_number,item_array_ptr);               
                
                all_number_len = 0;
                for (index = 0; index < to_number; index++)
                {
                    all_number_len = all_number_len + num_str_ptr[index].str_len;
                }
                if (0 == all_number_len)
                {
                    if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
                    {
                        MMIMMS_FREE(edit_doc_ptr->edit_to.wstr_ptr);
                        edit_doc_ptr->edit_to.wstr_ptr = NULL;
                        edit_doc_ptr->edit_to.wstr_len = 0;
                    }
                    SCI_FREE(num_str_ptr);
                    num_str_ptr = PNULL;
                    SCI_FREE(item_array_ptr);
                    item_array_ptr = PNULL;
                    break;
                }
                temp_len = (all_number_len + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
                //temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
                all_number_wstr = (wchar *)MMIMMS_ALLOC(temp_len);            
                if (PNULL == all_number_wstr)
                {
                    //SCI_TRACE_LOW:"malloc email_addr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9402_112_2_18_2_37_58_70,(uint8*)"");
                    SCI_FREE(num_str_ptr);
                    num_str_ptr = PNULL;
                    SCI_FREE(item_array_ptr);
                    item_array_ptr = PNULL;
                    MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                    break;
                }
                SCI_MEMSET(all_number_wstr, 0, temp_len);
                all_number_len = 0;
                
                for (index = 0; index < to_number; index++)
                {
                    tele_len = MIN(MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                    MMI_WSTRNCPY(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
                    
                    for (num_index = 0; num_index <= tele_len; num_index++)
                    {
                        MMI_WSTRNCPY(num_temp, MMIMMS_MAX_ADDRESS_LEN, tele_wstr, MMIMMS_MAX_ADDRESS_LEN, tele_len);
                        num_index = num_index - is_have_empty;
                        is_have_empty = 0;
                        if (0 == MMIAPICOM_Wstrncmp(&(tele_wstr[num_index]), empty_wstr, 1))
                        {
                            tele_wstr[tele_len - 1] = 0;
                            tele_len--;
                            MMI_WSTRNCPY(&(tele_wstr[num_index]), MMIMMS_MAX_ADDRESS_LEN, 
                                &(num_temp[num_index + 1]), MMIMMS_MAX_ADDRESS_LEN, tele_len - num_index);
                            is_have_empty = 1;
                        }
                        SCI_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                    }
                    is_same_num = FALSE;
                    if (tele_len > 0)
                    {
                        temp_numberlist_ptr = add_numberlist_ptr;
                        while(PNULL != temp_numberlist_ptr)
                        {
                            if (0 != temp_numberlist_ptr->number_len
                                && temp_numberlist_ptr->number_len == tele_len
                                && !MMIAPICOM_Wstrncmp(temp_numberlist_ptr->number_ptr, tele_wstr, tele_len))
                            {
                                is_same_num = TRUE;
                                break;
                            }
                            else
                            {
                                temp_numberlist_ptr = temp_numberlist_ptr->next;
                            }
                            
                        }
                    }
                    if (tele_len > 0 && !is_same_num)
                    {
                        MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIMMS_ALL_ADDRESS_LEN, 
                            tele_wstr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
                        all_number_len = tele_len + all_number_len;
                        if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
                        {
                            MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                            all_number_len++;
                        }
                        
                        cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                        SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                        cursor_numberlist_ptr->is_object = item_array_ptr[index].is_object;
                        if(cursor_numberlist_ptr->is_object)
                        {
                            cursor_numberlist_ptr->number_name_len = item_array_ptr[index].display_str_len;
                            cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                            SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                            MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                item_array_ptr[index].display_str_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);
                            
                            cursor_numberlist_ptr->number_len = item_array_ptr[index].user_str_len;
                            cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                            SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                            MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                item_array_ptr[index].user_str_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                        }
                        else
                        {//手动输入需要匹配
                            MMIPB_BCD_NUMBER_T black_bcd_num = {0};
                            MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
                            BOOLEAN find_result = FALSE;  
                            BOOLEAN ret = TRUE; 
                            uint16 i   = 0;
                            
                            temp_len = item_array_ptr[index].display_str_len;
                            MMI_WSTRNTOSTR(tele_num, MMIMMS_MAX_ADDRESS_LEN, item_array_ptr[index].display_str_ptr, temp_len, temp_len);
                            for(i = 0; i < temp_len; i++)
                            {
                                if(!MMIAPICOM_IsTelephoneNum(tele_num[i]))
                                {
                                    ret = FALSE;
                                    break;
                                }                               
                            }
                            if(ret)
                            {
                                MMIAPICOM_GenPartyNumber(tele_num, temp_len, &party_num);
                                black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                                black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                                SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
                                number_name_addr.wstr_ptr = number_name_wstr;
                                find_result = MMIAPIPB_GetNameByNumber(&black_bcd_num, &number_name_addr, MMIPB_NAME_MAX_LEN, FALSE);
                                if(find_result && 0 < number_name_addr.wstr_len)
                                {//如果匹配名称，则显示名称，不能够修改了
                                    cursor_numberlist_ptr->is_object = TRUE;
                                    
                                    cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                                    cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                    MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                        number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);                                
                                }
                                 
                            }

                            cursor_numberlist_ptr->number_len = item_array_ptr[index].display_str_len;
                            cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                            SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                            MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            item_array_ptr[index].display_str_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);  
                            
                        }                        
                        if(PNULL == add_numberlist_ptr)
                        {
                            add_numberlist_ptr = cursor_numberlist_ptr;
                        }
                        else
                        {
                            temp_numberlist_ptr = add_numberlist_ptr;
                            while(PNULL != temp_numberlist_ptr->next)
                            {
                                temp_numberlist_ptr = temp_numberlist_ptr->next;
                            }
                            temp_numberlist_ptr->next = cursor_numberlist_ptr;
                        }
                    }                   
                    
                    SCI_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                    SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
                    }
                    
                    //把得到的号码加入edit document中去
                    //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9538_112_2_18_2_37_58_71,(uint8*)"");
                    to_edit_string.wstr_ptr = all_number_wstr;
                    to_edit_string.wstr_len = MMIAPICOM_Wstrlen(all_number_wstr);
                    SetMMSEditIsSaveDraft(FALSE); 
                    if(MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
                    {
                        GUIEDIT_ClearAllStr(ctrl_id);
                        ToListBoxSetArray(ctrl_id, add_numberlist_ptr);
                        /*MMIAPIPB_OpenNumberList(
                            MMIPB_LIST_FOR_MMS,
                            MMIPB_LIST_CHECK,
                            (uint16)(MMIMMS_MAX_RECEIVER_NUM - to_number),
                            PNULL
                            );*/
                        MMIAPIPB_OpenMainWin(
                                MMIPB_ALL_CONTACT,//
                                MMIPB_LIST_CHECK, //IN: 号码列表的类型
                                MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,//lint !e655
                                (uint16)MMIMMS_MAX_RECEIVER_NUM - to_number,//如果是多选时，此项限制了多选的最大个数
                                PNULL,
                                ListSelectContactCallback
                                );
                        while(PNULL != add_numberlist_ptr)
                        {//进入pb要释放空间
                            if(PNULL != add_numberlist_ptr->number_name_ptr)
                            {
                                SCI_FREE(add_numberlist_ptr->number_name_ptr);
                                add_numberlist_ptr->number_name_ptr = PNULL;
                            }
                            if(PNULL != add_numberlist_ptr->number_ptr)
                            {
                                SCI_FREE(add_numberlist_ptr->number_ptr);
                                add_numberlist_ptr->number_ptr = PNULL;
                            }
                            free_numberlist_ptr = add_numberlist_ptr;
                            add_numberlist_ptr = add_numberlist_ptr->next;
                            SCI_FREE(free_numberlist_ptr);
                            free_numberlist_ptr = PNULL;                    
                        }
                    }
                    else
                    {//保存的话不用释放空间，以后还要使用
                        MMIMMS_SetMMSTo(&to_edit_string,edit_doc_ptr);
                        MMIMMS_SetMMSToNameList(add_numberlist_ptr, edit_doc_ptr);
                        MMK_CloseWin(win_id);
                    }                    
                    
                    MMIMMS_FREE(num_str_ptr);
                    num_str_ptr = PNULL;
                    
                    SCI_FREE(item_array_ptr);
                    item_array_ptr = PNULL;
                    
                    MMIMMS_FREE(all_number_wstr);
                    all_number_wstr = PNULL;                    
                }
                else
                {
                    if(MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
                    {
                        /* MMIAPIPB_OpenNumberList(
                            MMIPB_LIST_FOR_MMS,
                            MMIPB_LIST_CHECK,
                            (uint16)MMIMMS_MAX_RECEIVER_NUM,
                            PNULL
                            );*/
                    MMIAPIPB_OpenMainWin(
                            MMIPB_ALL_CONTACT,//
                            MMIPB_LIST_CHECK, //IN: 号码列表的类型
                            MMIPB_LIST_FIELD_MAIL | MMIPB_LIST_FIELD_NUMBER,//lint !e655
                            (uint16)MMIMMS_MAX_RECEIVER_NUM ,//如果是多选时，此项限制了多选的最大个数
                            PNULL,
                            ListSelectContactCallback
                            );
                    }
                    else
                    {
                        MMK_CloseWin(win_id);
                    }
                    if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
                    {
                        MMIMMS_FREE(edit_doc_ptr->edit_to.wstr_ptr);
                        edit_doc_ptr->edit_to.wstr_ptr = NULL;
                        edit_doc_ptr->edit_to.wstr_len = 0;
                    }
                    add_numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
                    while(PNULL != add_numberlist_ptr)
                    {
                        if(PNULL != add_numberlist_ptr->number_name_ptr)
                        {
                            SCI_FREE(add_numberlist_ptr->number_name_ptr);
                            add_numberlist_ptr->number_name_ptr = PNULL;
                        }
                        if(PNULL != add_numberlist_ptr->number_ptr)
                        {
                            SCI_FREE(add_numberlist_ptr->number_ptr);
                            add_numberlist_ptr->number_ptr = PNULL;
                        }
                        free_numberlist_ptr = add_numberlist_ptr;
                        add_numberlist_ptr = add_numberlist_ptr->next;
                        SCI_FREE(free_numberlist_ptr);
                        free_numberlist_ptr = PNULL;                    
                    }
                    edit_doc_ptr->numberlist_header_ptr = PNULL;
                }
            break;
          
        case MMI_SELECT_CONTACT_CNF:
            pb_select_num = MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)param);
            add_numberlist_ptr = PNULL;
            if (pb_select_num > 0)
            { 
                temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
                all_number_wstr = (wchar *)MMIMMS_ALLOC(temp_len);
                if (PNULL == all_number_wstr)
                {
                    //SCI_TRACE_LOW:"malloc all_number_wstr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9654_112_2_18_2_37_59_72,(uint8*)"");
                    MMSMMI_OpenMMSAlertWin(TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                    break;
                }
                SCI_MEMSET(all_number_wstr, 0, temp_len);

                {//解决上次的号码尾部没有分号问题
                    to_number = GUIEDIT_GetListItemNum(ctrl_id);
                    if (MMIMMS_MAX_RECEIVER_NUM <= to_number)
                    {
                        MMIMMS_FREE(all_number_wstr);
                        all_number_wstr = PNULL;
                        //SCI_TRACE_LOW:"to num is full MMIMMS_MAX_RECEIVER_NUM"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9666_112_2_18_2_37_59_73,(uint8*)"");
                        break;
                    }
                    if (0 < to_number)
                    {
                        num_str_ptr = SCI_ALLOC_APP(to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
                        if (PNULL == num_str_ptr)
                        {
                            MMIMMS_FREE(all_number_wstr);
                            all_number_wstr = PNULL;
                            //SCI_TRACE_LOW:"malloc num_str_ptr failed"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9676_112_2_18_2_37_59_74,(uint8*)"");
                            break;
                        }
                        GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
                        all_number_len = 0;
                        for (index = 0; index < to_number; index++)
                        {
                            tele_len = MIN(MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                            MMI_WSTRNCPY(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
                            if (tele_len > 0)
                            {
                                MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIMMS_ALL_ADDRESS_LEN, 
                                    tele_wstr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
                                all_number_len = tele_len + all_number_len;
                                if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
                                {
                                    MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                                    all_number_len++;
                                }
                            }
                            SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
                        }
                        MMIMMS_FREE(num_str_ptr);
                        num_str_ptr = PNULL;
                    }
                }
                
                SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
                SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
                SCI_MEMSET(&email_addr, 0, sizeof(email_addr));
                email_addr.wstr_ptr = (wchar *)MMIMMS_ALLOC((MMIPB_MAX_MAIL_LEN + 1)*sizeof(wchar));
                if (PNULL == email_addr.wstr_ptr)
                {
                    //SCI_TRACE_LOW:"malloc email_addr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9709_112_2_18_2_37_59_75,(uint8*)"");
                    MMIMMS_FREE(all_number_wstr);
                    all_number_wstr = PNULL;
                    MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                    break;
                }
                number_name_addr.wstr_ptr = SCI_ALLOC_APP((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
          
                for (pb_select_index = 0; pb_select_index < MIN(MMIMMS_MAX_RECEIVER_NUM, pb_select_num); pb_select_index++)
                {
                	uint16                  entry_id = 0;
    			uint16                  storage_id = 0;
			uint32    group_id = 0;
                    SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
                    SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
                    email_addr.wstr_len = 0;
                    SCI_MEMSET(number_name_addr.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar)));
                    number_name_addr.wstr_len = 0;

                    MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)param, &pb_bcd_number,MMIPB_BCD_NUMBER_MAX_LEN, pb_select_index);
                    MMIAPIPB_GetSelectEmail((MMIPB_HANDLE_T)param, &email_addr, MMIPB_MAX_MAIL_LEN, pb_select_index);
                // because most used contact do not have name information
					//MMIAPIPB_GetSelectName((MMIPB_HANDLE_T)param, &number_name_addr, MMIMMS_FILE_NAME_LEN, pb_select_index);
                    MMIPB_GetNameByNumber(&pb_bcd_number,&number_name_addr,MMIPB_NAME_MAX_LEN,
						&entry_id,&storage_id,&group_id,FALSE,FALSE);
			
					//if (0 != pb_bcd_number.number_len)
                    {
                        max_number_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, pb_bcd_number.number_len);
                        tele_len = MMIAPIPB_BCDNumberToString(&pb_bcd_number, MMIMMS_MAX_ADDRESS_LEN + 2, tele_num);
//                         tele_len = MMIAPICOM_GenDispNumber( 
//                             MMIPB_GetNumberTypeFromUint8(pb_bcd_number.npi_ton), 
//                             (uint8)max_number_len, 
//                             pb_bcd_number.number,  
//                             tele_num, 
//                             MMIMMS_MAX_ADDRESS_LEN + 2 
//                             );
                        //SCI_TRACE_LOW:"HandleMMSEditToListWinMsg,tele_len = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9735_112_2_18_2_37_59_76,(uint8*)"d",tele_len);
                        if(0 != tele_len)
                        {                    
                            MMI_STRNTOWSTR(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, (uint8 *)tele_num, MMIMMS_MAX_ADDRESS_LEN, tele_len );
                            //todo to_edit_string里面是原有的的号码，在尾部添加；同时加上现在的号码
                            MMIAPICOM_Wstrncat(all_number_wstr + all_number_len, tele_wstr, tele_len);
                            all_number_len = all_number_len + tele_len;
                            MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                            all_number_len = all_number_len +1;
                        }
                        else
                        {
                            //SCI_TRACE_MID:"warning_ tele_len 0"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9747_112_2_18_2_37_59_77,(uint8*)"");
                        }
                        is_same_num = FALSE;
                        if (tele_len > 0)
                        {                            
                            num_str_ptr = SCI_ALLOC_APP(to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
                            if (PNULL == num_str_ptr)
                            {
                                MMIMMS_FREE(all_number_wstr);
                                all_number_wstr = PNULL;
                                //SCI_TRACE_LOW:"malloc num_str_ptr failed"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9676_112_2_18_2_37_59_74,(uint8*)"");
                                break;
                            }
                            GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
                            for (index = 0; index < to_number; index++)
                            {
                                //tele_len = MIN(MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                                if (0 != num_str_ptr[index].str_len && num_str_ptr[index].str_len == tele_len 
									&& !MMIAPICOM_Wstrncmp(num_str_ptr[index].str_ptr, tele_wstr, tele_len))
                                {
                                    is_same_num = TRUE;   
                                    MMIMMS_FREE(num_str_ptr);
                                    break;
                                }
                            }
                            if (num_str_ptr != PNULL)
                            {
                                MMIMMS_FREE(num_str_ptr);
                            }
                            
                        }
                        if(0 != tele_len && !is_same_num)
                        {
                            cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                            SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                            if(0 != number_name_addr.wstr_len)
                            {//有pb中名称
                                cursor_numberlist_ptr->is_object = TRUE;
                                cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                                cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                                MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                                    number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);

                                cursor_numberlist_ptr->number_len = tele_len;
                                cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                    tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                            }
                            else
                            {//没有名称按普通的号码处理
                                cursor_numberlist_ptr->is_object = FALSE;
                                cursor_numberlist_ptr->number_len = tele_len;
                                cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                                MMI_STRNTOWSTR(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                                    tele_num, MMIMMS_MAX_ADDRESS_LEN, cursor_numberlist_ptr->number_len);
                            }
                            if(PNULL == add_numberlist_ptr)
                            {
                                add_numberlist_ptr = cursor_numberlist_ptr;
                            }
                            else
                            {
                                temp_numberlist_ptr = add_numberlist_ptr;
                                while(PNULL != temp_numberlist_ptr->next)
                                {
                                    temp_numberlist_ptr = temp_numberlist_ptr->next;
                                }
                                temp_numberlist_ptr->next = cursor_numberlist_ptr;
                            }
                        }                        
                    }//if (pb_number.number_len != 0)
                    if (0 != email_addr.wstr_len)
                    {
                        if (MMIAPICOM_IsASCIIString(email_addr.wstr_ptr, email_addr.wstr_len))
                        {     
                            MMIAPICOM_Wstrncat(all_number_wstr, email_addr.wstr_ptr, email_addr.wstr_len);
                            all_number_len = all_number_len + email_addr.wstr_len;
                            MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                            all_number_len = all_number_len +1;
                        }
                        cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                        SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                        //按普通的号码处理
                        cursor_numberlist_ptr->is_object = FALSE;
                        cursor_numberlist_ptr->number_len = email_addr.wstr_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            email_addr.wstr_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                        if(PNULL == add_numberlist_ptr)
                        {
                            add_numberlist_ptr = cursor_numberlist_ptr;
                        }
                        else
                        {
                            temp_numberlist_ptr = add_numberlist_ptr;
                            while(PNULL != temp_numberlist_ptr->next)
                            {
                                temp_numberlist_ptr = temp_numberlist_ptr->next;
                            }
                            temp_numberlist_ptr->next = cursor_numberlist_ptr;
                        }
                    }                
                }  
                //显示添加号码
                ToListBoxSetArray(ctrl_id, add_numberlist_ptr);
                //free add_numberlist_ptr
                while(PNULL != add_numberlist_ptr)
                {
                    if(PNULL != add_numberlist_ptr->number_name_ptr)
                    {
                        SCI_FREE(add_numberlist_ptr->number_name_ptr);
                        add_numberlist_ptr->number_name_ptr = PNULL;
                    }
                    if(PNULL != add_numberlist_ptr->number_ptr)
                    {
                        SCI_FREE(add_numberlist_ptr->number_ptr);
                        add_numberlist_ptr->number_ptr = PNULL;
                    }
                    free_numberlist_ptr = add_numberlist_ptr;
                    add_numberlist_ptr = add_numberlist_ptr->next;
                    SCI_FREE(free_numberlist_ptr);
                    free_numberlist_ptr = PNULL;                    
                }
                //free email_addr
                if (PNULL != email_addr.wstr_ptr)
                {
                    MMIMMS_FREE(email_addr.wstr_ptr);
                    email_addr.wstr_ptr = NULL;
                }
                if (PNULL != number_name_addr.wstr_ptr)
                {
                    SCI_FREE(number_name_addr.wstr_ptr);
                    number_name_addr.wstr_ptr = NULL;
                }
                MMIMMS_FREE(all_number_wstr);
                all_number_wstr = PNULL;
            }
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            //todo 返回编辑主界面
            MMK_CloseWin(win_id);
            break;
        case MSG_ALERTWIN_CLOSED:
            MMIPUB_CloseAlertWin();
            break;
        default :
            err_code = MMI_RESULT_FALSE;
            break;
        }
    }
    else
    {
        MMK_CloseWin(win_id);
        err_code = MMI_RESULT_FALSE;
    }
    return err_code;
}

/*****************************************************************************/
//  Description :mms edit main richedit edit subject menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditSubjectWinMsg(
                                               MMIMMS_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                              )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_STRING_T  subject_edit_string = {0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;

    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL != edit_doc_ptr)
    {
        if (PNULL != edit_doc_ptr->edit_subject.wstr_ptr)
        {
            subject_edit_string = edit_doc_ptr->edit_subject;
        }
    }
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditSubjectWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_9938_112_2_18_2_37_59_78,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIMMS_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
        //todo 如果存在就显示主题
#ifdef CMCC_UI_STYLE        
        GUIEDIT_SetTextMaxLen(MMIMMS_EDIT_SUBJECT_CTRL_ID, MMIMMS_MAX_SUBJECT_LEN/3, MMIMMS_MAX_SUBJECT_LEN);
#else
        GUIEDIT_SetTextMaxLen(MMIMMS_EDIT_SUBJECT_CTRL_ID, MMIMMS_MAX_SUBJECT_LEN/2, MMIMMS_MAX_SUBJECT_LEN);
#endif
        if (PNULL != subject_edit_string.wstr_ptr)
        {
            GUIEDIT_SetString(
                MMIMMS_EDIT_SUBJECT_CTRL_ID, 
                subject_edit_string.wstr_ptr,
                subject_edit_string.wstr_len
                );
        }
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
        GUIEDIT_SetFont(MMIMMS_EDIT_SUBJECT_CTRL_ID, MMISMS_GetWinFont());
#endif
        MMK_SetAtvCtrl(win_id, MMIMMS_EDIT_SUBJECT_CTRL_ID);
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
 case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                //todo 保存主题，回到编辑主界面
                SCI_MEMSET(&subject_edit_string, 0, sizeof(MMI_STRING_T));
                GUIEDIT_GetString( MMIMMS_EDIT_SUBJECT_CTRL_ID, &subject_edit_string);
        
                if (PNULL != edit_doc_ptr)
                {
                    //使用接口进行设置主题
                    SetMMSEditIsSaveDraft(FALSE);
                    MMIMMS_SetSubject(&subject_edit_string,edit_doc_ptr);
                }
                MMK_CloseWin(MMIMMS_EDIT_SUBJECT_WIN_ID);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:

                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                        //todo 返回编辑主界面 
                MMK_CloseWin(MMIMMS_EDIT_SUBJECT_WIN_ID);
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //todo 保存主题，回到编辑主界面
        SCI_MEMSET(&subject_edit_string, 0, sizeof(MMI_STRING_T));
        GUIEDIT_GetString( MMIMMS_EDIT_SUBJECT_CTRL_ID, &subject_edit_string);

        if (PNULL != edit_doc_ptr)
        {
            //使用接口进行设置主题
            SetMMSEditIsSaveDraft(FALSE);
            MMIMMS_SetSubject(&subject_edit_string,edit_doc_ptr);
        }
        MMK_CloseWin(MMIMMS_EDIT_SUBJECT_WIN_ID);
        break;
#endif        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //todo 返回编辑主界面 
        MMK_CloseWin(MMIMMS_EDIT_SUBJECT_WIN_ID);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :mms edit text edit menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditTextWinMsg(
                                            MMIMMS_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E                 err_code = MMI_RESULT_TRUE;
    MMI_STRING_T          mms_text_string = {0};
    MMI_STRING_T   mms_pretext_string     = {0};
    MMI_STRING_T   mms_add_string         = {0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMIMMS_ERROR_E add_text_res           = MMIMMS_NO_ERROR;
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditTextWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10040_112_2_18_2_37_59_79,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIMMS_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
        //显示文本
        if (PNULL != edit_doc_ptr && edit_doc_ptr->editbody_ptr)
        {
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
            {
                if (MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string))
                {
                    GUIEDIT_SetString(
                        MMIMMS_EDIT_TEXT_CTRL_ID, 
                        mms_text_string.wstr_ptr,
                        mms_text_string.wstr_len
                        );
                }
            }
             MMK_SetAtvCtrl(win_id, MMIMMS_EDIT_TEXT_CTRL_ID);
        }
        else 
        {
            MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            MMK_CloseWin(win_id);
        }

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
        GUIEDIT_SetFont(MMIMMS_EDIT_TEXT_CTRL_ID, MMISMS_GetWinFont());
#endif
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
 case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
        //todo 保存主题，回到编辑主界面
            SCI_MEMSET(&mms_text_string, 0, sizeof(MMI_STRING_T));
            GUIEDIT_GetString(MMIMMS_EDIT_TEXT_CTRL_ID, &mms_text_string);
            if (PNULL != edit_doc_ptr && edit_doc_ptr->editbody_ptr)
            {
                MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_pretext_string);
                if (PNULL != mms_pretext_string.wstr_ptr && 0 != mms_pretext_string.wstr_len)
                {
                    mms_add_string.wstr_ptr = (wchar *)SCI_ALLOC_APP((mms_pretext_string.wstr_len + 1) * sizeof(wchar));
                    SCI_MEMSET(mms_add_string.wstr_ptr, 0, ((mms_pretext_string.wstr_len + 1) * sizeof(wchar)));
                    mms_add_string.wstr_len = mms_pretext_string.wstr_len;
                    MMIAPICOM_Wstrncpy(mms_add_string.wstr_ptr, mms_pretext_string.wstr_ptr, mms_add_string.wstr_len);
                }            
    
                SetMMSEditIsSaveDraft(FALSE);
                add_text_res = MMIMMS_AddText(edit_doc_ptr, &mms_text_string);
                if (MMIMMS_NO_ERROR != add_text_res)
                {
                    MMIMMS_EditMMSAlertWin(add_text_res);
                    if (PNULL != mms_add_string.wstr_ptr)
                    {
                        SCI_Free(mms_add_string.wstr_ptr);
                        mms_add_string.wstr_ptr = PNULL;
                        mms_add_string.wstr_len = 0;
                    }
                    break;
                }
                if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    add_text_res = MMIMMS_AddText(edit_doc_ptr, &mms_add_string);
                    if (MMIMMS_NO_ERROR != add_text_res)
                    {
                        MMIMMS_EditMMSAlertWin(add_text_res);
                    }
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_INVALID_SIZE, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                }
                else
                {
                    MMK_CloseWin(win_id);
                } 
                if (PNULL != mms_add_string.wstr_ptr)
                {
                    SCI_FREE(mms_add_string.wstr_ptr);
                    mms_add_string.wstr_ptr = PNULL;
                    mms_add_string.wstr_len = 0;
                }
            }
            else 
            {
                MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            }        
            break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
             //todo 返回编辑主界面 
                MMK_CloseWin(win_id);
                break;      
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //todo 保存主题，回到编辑主界面
        SCI_MEMSET(&mms_text_string, 0, sizeof(MMI_STRING_T));
        GUIEDIT_GetString(MMIMMS_EDIT_TEXT_CTRL_ID, &mms_text_string);
        if (PNULL != edit_doc_ptr && edit_doc_ptr->editbody_ptr)
        {
            MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_pretext_string);
            if (PNULL != mms_pretext_string.wstr_ptr && 0 != mms_pretext_string.wstr_len)
            {
                mms_add_string.wstr_ptr = (wchar *)SCI_ALLOC_APP((mms_pretext_string.wstr_len + 1) * sizeof(wchar));
                SCI_MEMSET(mms_add_string.wstr_ptr, 0, ((mms_pretext_string.wstr_len + 1) * sizeof(wchar)));
                mms_add_string.wstr_len = mms_pretext_string.wstr_len;
                MMIAPICOM_Wstrncpy(mms_add_string.wstr_ptr, mms_pretext_string.wstr_ptr, mms_add_string.wstr_len);
            }            

            SetMMSEditIsSaveDraft(FALSE);
            add_text_res = MMIMMS_AddText(edit_doc_ptr, &mms_text_string);
            if (MMIMMS_NO_ERROR != add_text_res)
            {
                MMIMMS_EditMMSAlertWin(add_text_res);
                if (PNULL != mms_add_string.wstr_ptr)
                {
                    SCI_Free(mms_add_string.wstr_ptr);
                    mms_add_string.wstr_ptr = PNULL;
                    mms_add_string.wstr_len = 0;
                }
                break;
            }
            if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
            {
                SetMMSEditIsSaveDraft(FALSE);
                add_text_res = MMIMMS_AddText(edit_doc_ptr, &mms_add_string);
                if (MMIMMS_NO_ERROR != add_text_res)
                {
                    MMIMMS_EditMMSAlertWin(add_text_res);
                }
                MMSMMI_OpenMMSAlertWin(TXT_MMS_INVALID_SIZE, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
            else
            {
                MMK_CloseWin(win_id);
            } 
            if (PNULL != mms_add_string.wstr_ptr)
            {
                SCI_FREE(mms_add_string.wstr_ptr);
                mms_add_string.wstr_ptr = PNULL;
                mms_add_string.wstr_len = 0;
            }
        }
        else 
        {
            MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);            
        }        
        break;
#endif        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //todo 返回编辑主界面 
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :mms edit each slide dur time menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSEditSetSlideTimeWin(
                                                 MMIMMS_WINDOW_ID_E win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E err_code                      = MMI_RESULT_TRUE;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr      = PNULL;
    MMI_STRING_T  edit_string                  = {0};
    uint32 duration                            = 0;
    uint8 dura_buf[MMIMMS_SLIDE_TIME_STR_LEN]  = {0};
    wchar dura_wstr[MMIMMS_SLIDE_TIME_STR_LEN] = {0};
    uint16 dura_len                            = 0;
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditSetSlideTimeWin msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10233_112_2_18_2_38_0_80,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //todo 默认的每一帧的播放时间是3000ms
        if ((PNULL != edit_doc_ptr) && (PNULL != edit_doc_ptr->editbody_ptr) && (PNULL != edit_doc_ptr->editbody_ptr->slide_cur))
        {
            duration = edit_doc_ptr->editbody_ptr->slide_cur->times;
        }
        duration = duration/MMIMMS_SLIDE_TIME_DURING;
        if (duration > MMIMMS_SLIDE_DURATION_MAX)
        {
            duration = MMIMMS_SLIDE_DURATION_MAX;
        }
        if (duration < MMIMMS_SLIDE_DURATION_MIN)
        {
            duration = MMIMMS_SLIDE_DURATION_MIN;
        }

        _snprintf((char *)dura_buf, MMIMMS_SLIDE_TIME_STR_LEN,"%d", duration);        
        dura_len = strlen((char *)dura_buf);        
        MMI_STRNTOWSTR( dura_wstr, MMIMMS_SLIDE_TIME_STR_LEN, (uint8 *)dura_buf, 16, dura_len );        
        GUIEDIT_SetString(
            MMIMMS_SET_SLIDE_TIME_CTRL_ID,
            dura_wstr,
            dura_len
            );
        
        GUIEDIT_SetSoftkey(MMIMMS_SET_SLIDE_TIME_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        GUIEDIT_SetImTag(MMIMMS_SET_SLIDE_TIME_CTRL_ID, GUIIM_TAG_PURE_NUM);
        MMK_SetAtvCtrl(win_id, MMIMMS_SET_SLIDE_TIME_CTRL_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //todo 对设置的播放时间存储进入全局变量，提示设置成功，关闭该窗口返回opt界面
        SCI_MEMSET( ((void *)&edit_string), 0, (sizeof(edit_string)) );
        GUIEDIT_GetString(MMIMMS_SET_SLIDE_TIME_CTRL_ID, &edit_string);
        MMI_WSTRNTOSTR( (uint8 *)dura_buf, MMIMMS_SLIDE_TIME_STR_LEN, edit_string.wstr_ptr, edit_string.wstr_len, edit_string.wstr_len );
        duration = atoi((char *)(dura_buf));
        
        if (0 == edit_string.wstr_len || duration > MMIMMS_SLIDE_DURATION_MAX || duration < MMIMMS_SLIDE_DURATION_MIN)
        {
            //修复 CR MS00150697
//             if ((PNULL != edit_doc_ptr) && (PNULL != edit_doc_ptr->editbody_ptr) && (PNULL != edit_doc_ptr->editbody_ptr->slide_cur))
//             {
//                 duration = edit_doc_ptr->editbody_ptr->slide_cur->times;
//             }
//            duration = duration/MMIMMS_SLIDE_TIME_DURING;
            if (duration > MMIMMS_SLIDE_DURATION_MAX)
            {
                duration = MMIMMS_SLIDE_DURATION_MAX;
            }
            if (duration < MMIMMS_SLIDE_DURATION_MIN)
            {
                duration = MMIMMS_SLIDE_DURATION_MIN;
            }

            _snprintf((char *)dura_buf, MMIMMS_SLIDE_TIME_STR_LEN,"%d", duration);        
            dura_len = strlen((char *)dura_buf);        
            MMI_STRNTOWSTR( dura_wstr, MMIMMS_SLIDE_TIME_STR_LEN, (uint8 *)dura_buf, 16, dura_len );   
            //GUIEDIT_ClearAllStr(MMIMMS_SET_SLIDE_TIME_CTRL_ID);
            GUIEDIT_SetString(
	            MMIMMS_SET_SLIDE_TIME_CTRL_ID,
	            dura_wstr,
	            dura_len
	            );	
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			//修复 CR MS00150697 end
            //Alert : invalidate input
            //MMSMMI_OpenMMSAlertWin(TXT_MMS_INVALID_INPUT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
        }
        else
        {
            //save
            duration = duration * MMIMMS_SLIDE_TIME_DURING;
            MMIMMS_SetCurSlideDuration(duration,edit_doc_ptr);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
            MMK_CloseWin(MMIMMS_EDIT_SET_SLIDE_TIME_WIN_ID);
        }
        break;


    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 关闭本窗口
        MMK_CloseWin(MMIMMS_EDIT_SET_SLIDE_TIME_WIN_ID);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :mms send select sim card
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMSelectSendWinMsg(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{    
    MMI_RESULT_E        recode           = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        win_id           = MMK_GetFocusWinId();
    
    if(isReturnOK)
    {
        MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
        MMIMMS_ERROR_E   send_result          = MMIMMS_NO_ERROR;
        int32 record_id                       = 0;
        
        //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10354_112_2_18_2_38_0_81,(uint8*)"d", dual_sys);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif

        if(MMIMMS_GetIsMMSMode())
        {
            if (SEND_NEW_MMS == MMIMMS_GetMMSSendStatus())
            {   
                edit_doc_ptr = g_mms_edit_globle.editdoc;
                record_id = edit_doc_ptr->record_id;
                //MMSMMI_CreateSendingWaitingWin((MN_DUAL_SYS_E)dual_sys, record_id);
                if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS == MMIMMS_GetMMSEditEnterReason()) //短信编辑草稿，最后以彩信发送，需要将草稿中的短信删除
                {
                    MMSDeleteDraftSMS();
                }
                send_result = MMIMMS_SendNewMMS((MN_DUAL_SYS_E)dual_sys, edit_doc_ptr); 
                MMSMMI_SetSendNewMMSId(edit_doc_ptr->record_id);
                //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg record id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10369_112_2_18_2_38_0_82,(uint8*)"d", edit_doc_ptr->record_id);
            }
            else if (SEND_OUTBOX_MMS == MMIMMS_GetMMSSendStatus())
            {
                record_id = (uint32)MMK_GetWinAddDataPtr(win_id);
                if (0 == record_id)
                {
                    //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg : error record id"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10376_112_2_18_2_38_0_83,(uint8*)"");
                    return recode;
                }
                //MMSMMI_CreateSendingWaitingWin((MN_DUAL_SYS_E)dual_sys, record_id); 
                send_result =  MMIMMS_SendOldMMS((MN_DUAL_SYS_E)dual_sys, record_id);
#ifdef MMI_SMS_CHAT_SUPPORT                
                MMIAPISMS_UpdateResendMMSLabel(record_id);
#endif
            }
            else
            {
                //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg MMIMMS_GetMMSSendStatus error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10384_112_2_18_2_38_0_84,(uint8*)"");
                return recode;
            }


            if (MMIMMS_NO_ERROR != send_result && MMIMMS_ERROR_NO_SPACE != send_result)
            {
                MMIMMS_EditMMSAlertWin(send_result);
                if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_SENDING_WIN_ID);
                }
                return recode;
            }            
            //退出编辑界面，清理空间
            if (SEND_NEW_MMS == MMIMMS_GetMMSSendStatus())
            {
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMIMMS_CloseEditWin();
#ifdef MMI_SMS_CHAT_SUPPORT            
                    MMIAPISMS_ClearChatEditString();
#endif                    
                    if (MMK_IsOpenWin(MMISMS_NEW_MSG_MENU_WIN_ID))
                    {
                        MMK_CloseWin(MMISMS_NEW_MSG_MENU_WIN_ID);
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg : exit error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10411_112_2_18_2_38_0_85,(uint8*)"");
                }
            }
            else if (SEND_OUTBOX_MMS == MMIMMS_GetMMSSendStatus())
            {
                if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                }
                else
                {
                    //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg : list"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10422_112_2_18_2_38_0_86,(uint8*)"");
                }
            }
        }
        else //短信的情况
        {
            MMIVC_SMS_SEND_TYPE_E send_type = MMIVC_SMS_SEND_NORMAL;
            // 开始发送消息
            if(NULL != (uint32)MMK_GetWinAddDataPtr(win_id))
            {
                send_type = (uint32)MMK_GetWinAddDataPtr(win_id);
            }

            if (MMIVC_SMS_SEND_NORMAL==send_type)
            {

                MMSSendSms(dual_sys);
                MMK_CloseWin(win_id); 
                MMK_UpdateScreen();
                MMIMMS_CloseEditWin();
            }
#ifdef MMI_VCARD_SUPPORT            
            else if(MMIVC_SMS_SEND_VCARD==send_type)
            {
                MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;

                send_err_code = (MMISMS_OPER_ERR_E)MMIAPISMS_WintabSendVcard((MN_DUAL_SYS_E)dual_sys);
                MMSDisplaySmsSendErr((MN_DUAL_SYS_E)dual_sys, send_err_code );
            }
#endif            
#ifdef MMI_VCALENDAR_SUPPORT
            else if(MMIVC_SMS_SEND_VCALENDAR==send_type)
            {
                MMISMS_OPER_ERR_E send_err_code = MMISMS_NO_ERR;

                send_err_code = (MMISMS_OPER_ERR_E)MMIAPISMS_WintabSendVcalendar((MN_DUAL_SYS_E)dual_sys);
                MMSDisplaySmsSendErr((MN_DUAL_SYS_E)dual_sys, send_err_code );
            }
#endif
        }
    }        
    else    //if user cancel sim selection
    {
            if(!MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
            {
                //MMSWin_Exit(SENDING_WIN_ENTRY);
            }
    }
    

    return recode;
}
#else
/*****************************************************************************/
//  Description :mms send select sim card
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSIMSelectSendWinMsg(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{    
    MMI_RESULT_E    recode                  = MMI_RESULT_TRUE;
    MMI_WIN_ID_T    win_id                  = MMK_GetFocusWinId();
 
    
    if (isReturnOK)
    {
        MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
        MMIMMS_ERROR_E   send_result          = MMIMMS_NO_ERROR;
        int32 record_id                       = 0;
        
        //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10491_112_2_18_2_38_0_87,(uint8*)"d", dual_sys);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif

        if (SEND_NEW_MMS == MMIMMS_GetMMSSendStatus())
        {   
            edit_doc_ptr = g_mms_edit_globle.editdoc;
            record_id = edit_doc_ptr->record_id;
#ifdef MMI_MMS_MINI_SUPPORT        
            MMSMMI_CreateSendingWaitingWin((MN_DUAL_SYS_E)dual_sys, record_id);
#endif
            send_result = MMIMMS_SendNewMMS((MN_DUAL_SYS_E)dual_sys, edit_doc_ptr); 
            MMSMMI_SetSendNewMMSId(edit_doc_ptr->record_id);
            //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg record id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10501_112_2_18_2_38_0_88,(uint8*)"d", edit_doc_ptr->record_id);
        }
        else if (SEND_OUTBOX_MMS == MMIMMS_GetMMSSendStatus())
        {
            record_id = (uint32)MMK_GetWinAddDataPtr(win_id);
            if (0 == record_id)
            {
                //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg : error record id"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10508_112_2_18_2_38_0_89,(uint8*)"");
                return recode;
            }
#ifdef MMI_MMS_MINI_SUPPORT
            MMSMMI_CreateSendingWaitingWin((MN_DUAL_SYS_E)dual_sys, record_id); 
#endif
            send_result =  MMIMMS_SendOldMMS((MN_DUAL_SYS_E)dual_sys, record_id);          
#ifdef MMI_SMS_CHAT_SUPPORT
            MMIAPISMS_UpdateResendMMSLabel(record_id);
#endif
        }
        else
        {
            //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg MMIMMS_GetMMSSendStatus error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10516_112_2_18_2_38_0_90,(uint8*)"");
            return recode;
        }

        if (MMIMMS_NO_ERROR != send_result && MMIMMS_ERROR_NO_SPACE != send_result)
        {
            MMIMMS_EditMMSAlertWin(send_result);
            if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_SENDING_WIN_ID);
            }
            return recode;
        }            
        //退出编辑界面，清理空间
        if (SEND_NEW_MMS == MMIMMS_GetMMSSendStatus())
        {
            if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
            {
                MMIMMS_CloseEditWin();
                if (MMK_IsOpenWin(MMISMS_NEW_MSG_MENU_WIN_ID))
                {
                    MMK_CloseWin(MMISMS_NEW_MSG_MENU_WIN_ID);
                }
            }
            else
            {
                //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg : exit error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10542_112_2_18_2_38_0_91,(uint8*)"");
            }
        }
        else if (SEND_OUTBOX_MMS == MMIMMS_GetMMSSendStatus())
        {
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
            else
            {
                //SCI_TRACE_LOW:"HandleSIMSelectSendWinMsg : list"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10553_112_2_18_2_38_0_92,(uint8*)"");
            }
        }
    }        
    return recode;
}
#endif
/*****************************************************************************/
//  Description :mms edit attachment type win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAttachmenTypeWinMsg(
                                              MMIMMS_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E err_code          = MMI_RESULT_TRUE;
    uint16 index_type              = MMIMMS_ATTACHMENT_TYPE_OHTER;
    MMI_STRING_T string_other      = {0};
#ifdef MMI_VCARD_SUPPORT
    MMI_STRING_T string_vcard      = {0};
#endif
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    //SCI_TRACE_LOW:"MMIMMS: HandleAttachmenTypeWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10577_112_2_18_2_38_0_93,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIMMS_EDIT_ATTACHMENT_CTRL_ID, MMIMMS_MAX_ATTACHMENT_NUM, FALSE);
#ifdef MMI_MMS_MINI_SUPPORT        
        MMI_GetLabelTextByLang(TXT_MMS_ATTACHMENT_OTHER, &string_other);
        MMIMMS_AppendOneLineTextListItem(MMIMMS_EDIT_ATTACHMENT_CTRL_ID,
                    GUIITEM_STYLE_ONE_LINE_RADIO,
                    TXT_COMMON_OK,  0,  string_other.wstr_ptr, 
                    string_other.wstr_len, (uint16)MMIMMS_ATTACHMENT_TYPE_OHTER, FALSE, 0);
#endif        
#ifdef MMI_VCARD_SUPPORT
        MMI_GetLabelTextByLang(TXT_COMMON_VCARD, &string_vcard);
        MMIMMS_AppendOneLineTextListItem(MMIMMS_EDIT_ATTACHMENT_CTRL_ID,
            GUIITEM_STYLE_ONE_LINE_RADIO,
            TXT_COMMON_OK,  0,  string_vcard.wstr_ptr, 
            string_vcard.wstr_len, (uint16)MMIMMS_ATTACHMENT_TYPE_VCARD, FALSE, 0);
#endif                 
#ifndef MMI_MMS_MINI_SUPPORT        
        MMI_GetLabelTextByLang(TXT_MMS_ATTACHMENT_OTHER, &string_other);
        MMIMMS_AppendOneLineTextListItem(MMIMMS_EDIT_ATTACHMENT_CTRL_ID,
                    GUIITEM_STYLE_ONE_LINE_RADIO,
                    TXT_COMMON_OK,  0,  string_other.wstr_ptr, 
                    string_other.wstr_len, (uint16)MMIMMS_ATTACHMENT_TYPE_OHTER, FALSE, 0);
#endif        

        GUILIST_SetCurItemIndex(MMIMMS_EDIT_ATTACHMENT_CTRL_ID, MMIMMS_ATTACHMENT_TYPE_VCARD);        	
        MMK_SetAtvCtrl(win_id, MMIMMS_EDIT_ATTACHMENT_CTRL_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        index_type = GUILIST_GetCurItemIndex(MMIMMS_EDIT_ATTACHMENT_CTRL_ID);
        GUILIST_SetSelectedItem(MMIMMS_EDIT_ATTACHMENT_CTRL_ID, index_type, TRUE);

        switch(index_type) 
        {
#ifdef MMI_VCARD_SUPPORT
        case MMIMMS_ATTACHMENT_TYPE_OHTER:
            if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
            {
                break;
            }
            MMSEditInsertNormalAttachment(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, GetMMSEditIsReplaceMultifile());
        	break;
        case MMIMMS_ATTACHMENT_TYPE_VCARD:
     
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSelectVcardCallback
                    );
            SetMMSEditInsertOpType(MMIMMS_INSERT_ADD_FILE);
            break;
#else
        case MMIMMS_ATTACHMENT_TYPE_OHTER:
            if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
            {
                break;
            }
            MMSEditInsertNormalAttachment(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, GetMMSEditIsReplaceMultifile());
            break;
        case MMIMMS_ATTACHMENT_TYPE_VCARD:
            if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
            {
                break;
            }
            MMSEditInsertNormalAttachment(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, GetMMSEditIsReplaceMultifile());
            break;
#endif            
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :create subject edit win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateSubjectEditWin(void)
{
    MMK_CreateWin((uint32*)MMIMMS_EDIT_SUBJECT_TAB, PNULL);
}

/*****************************************************************************/
//  Description :create slide time duration edit win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateSlideDurationEditWin(void)
{
    MMK_CreatePubEditWin((uint32*)MMIMMS_EDIT_SET_SLIDE_TIME_TAB, PNULL);
}

/*****************************************************************************/
//  Description :create sim card select win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateSimCardSelectWin(uint32 record_id)
{
//    MMK_CloseWin(MMIMMS_SELECT_SIM_SEND_WIN_ID);
//    MMK_CreatePubListWin((uint32 *)MMIMMS_SELECT_SIM_SEND_WIN_TAB,(ADD_DATA)record_id);
    MMIAPISMS_SelectSIMFunc(HandleSIMSelectSendWinMsg,(ADD_DATA)record_id);
}

/*****************************************************************************/
//  Description :create to list edit win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateToListEditWin(void)
{
    MMK_CreateWin((uint32*)MMIMMS_EDIT_TO_LIST_TAB, PNULL);
}

/*****************************************************************************/
//  Description :get to num for string 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetRecieverNumberFromStr(MMI_STRING_T * to_str)
{
    uint16  to_num    = 0;
    wchar  *to_wstr   = PNULL;
    uint8  *to_ptr    = PNULL;
    uint8  *temp_ptr  = PNULL;
    uint16 to_ptr_len = 0;

    if (PNULL == to_str)
    {
        return 0;
    }
    if (PNULL == to_str->wstr_ptr || 0 == to_str->wstr_len)
    {
        return 0;
    }
    to_wstr = to_str->wstr_ptr;
    to_ptr_len = to_str->wstr_len +1;
    to_ptr = (uint8 *)MMIMMS_ALLOC(to_ptr_len);
    if (PNULL == to_ptr)
    {
        return 0;
    }
    SCI_MEMSET( to_ptr, 0, to_ptr_len);
    MMI_WSTRNTOSTR((uint8 *)to_ptr, to_str->wstr_len, to_wstr, to_str->wstr_len, to_str->wstr_len);
    temp_ptr = to_ptr;
    while (*temp_ptr != '\0')
    {
        if (*temp_ptr == ';')
        {
            to_num++;
        }
        temp_ptr = temp_ptr + 1;
    }

    MMIMMS_FREE(to_ptr);
    to_ptr = PNULL;

    return to_num;
}

/*****************************************************************************/
//  Description :create edit win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CreateEditWinExt(BOOLEAN is_permitted, DPARAM param_ptr)
{    
    MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr = g_mms_edit_globle.editdoc;
    if(is_permitted)
    {
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
    #if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if(MMITHEME_IsIstyle())
        {        
            MMK_CreateWin((uint32 *)MMIMMS_EDIT_MENU_TAB_EX, PNULL);
        }
        else
    #endif
    #endif
        {
            MMK_CreateWin((uint32 *)MMIMMS_EDIT_MENU_TAB, PNULL);
        }
    }
    else
    {
        MMIMMS_DestroyEditDocument(editdoc_ptr);
    }
}
/*****************************************************************************/
//  Description :create edit win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CreateEditWin(void)
{
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if(MMITHEME_IsIstyle())
    {        
        MMK_CreateWin((uint32 *)MMIMMS_EDIT_MENU_TAB_EX, PNULL);
    }
    else
#endif
#endif
    {
        MMK_CreateWin((uint32 *)MMIMMS_EDIT_MENU_TAB, PNULL);
    }

    if (   MMIMMS_EDIT_FROM_OUTBOX_READ_TO_EDIT == MMIMMS_GetMMSEditEnterReason()
        || MMIMMS_EDIT_FROM_SENDBOX_READ_TO_FARWARD == MMIMMS_GetMMSEditEnterReason()
        || MMIMMS_EDIT_FROM_INBOX_READ_TO_FARWARD == MMIMMS_GetMMSEditEnterReason()
        || MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER == MMIMMS_GetMMSEditEnterReason()
        || MMIMMS_EDIT_FROM_INBOX_READ_NUMBER_SEND == MMIMMS_GetMMSEditEnterReason())
    {
        if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
        {
            MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
        }
    }
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add new slide
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_SlideInit(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMIMMS_ERROR_E add_slide_err          = MMIMMS_NO_ERROR;
    uint16 slide_count                    = 0;
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
    {
        return FALSE;
    }

    slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);
    if (0 == slide_count)
    {
        SetMMSEditIsSaveDraft(FALSE);
        add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
        if (MMIMMS_NO_ERROR != add_slide_err)
        {
            MMIMMS_EditMMSAlertWin(add_slide_err);
            return FALSE;
        }        
    }
    if (PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
    {
        return FALSE;
    }
    

    return TRUE;
}
#endif

#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add richtext items to edit win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMSEditEntry(MMIMMS_RICHTEXT_OP_TYPE is_add)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMIMMS_ERROR_E add_slide_err          = MMIMMS_NO_ERROR;
    P_MMS_SLIDE_T temp_slide_ptr             = PNULL;
    uint16 slide_count                    = 0;
    uint16 cur_slide_index                = 0;
//  BOOLEAN is_have_pic                   = FALSE;
//  BOOLEAN is_have_video                 = FALSE;
//  BOOLEAN is_have_music                 = FALSE;
    MMS_TEXT_IMAGE_ORDER_E text_ordor     = MMS_ORDER_TEXT_FIRST;
    MMS_ACCESSORY_PARAM_T *add_file_ptr   = PNULL;
    
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == edit_doc_ptr)
    {
        return FALSE;
    }
    if (PNULL == edit_doc_ptr->editbody_ptr)
    {
        return FALSE;
    }
    slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);
    if (0 == slide_count)
    {
        SetMMSEditIsSaveDraft(FALSE);
        add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
        if (MMIMMS_NO_ERROR != add_slide_err)
        {
            MMIMMS_EditMMSAlertWin(add_slide_err);
            return FALSE;
        }        
    }
    if (PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
    {
        return FALSE;
    }
    temp_slide_ptr = edit_doc_ptr->editbody_ptr->slide_cur;
    cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
    text_ordor = temp_slide_ptr->textimage_order;
    add_file_ptr = edit_doc_ptr->editbody_ptr->accessory_ptr;
    if (1 == cur_slide_index)//该页是首页，需要显示to和subject
    {
        MMSEditAddToItem(is_add);
        MMSEditAddSubjectItem(is_add);
        while (PNULL != add_file_ptr)
        {
            MMSEditAddAddFileItem(is_add, add_file_ptr->filename);
            add_file_ptr = add_file_ptr->next;
        }        
    }
    if (MMS_ORDER_IMAGE_FIRST == text_ordor)
    {
        MMSEditAddPicItem(is_add);
        MMSEditAddTextItem(is_add);        
    }
    else
    {
        MMSEditAddTextItem(is_add); 
        MMSEditAddPicItem(is_add);
    }
    
    if (!temp_slide_ptr->video_flag)
    {
        MMSEditAddMusicItem(is_add);
    }
    MMSEditAddMenuItem(is_add);    
    return TRUE;
    
}


/*****************************************************************************/
//  Description :add richtext  to list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddToItem(MMIMMS_RICHTEXT_OP_TYPE is_add)
{
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    wchar *to_wstr                        = PNULL;
    uint16 to_len                         = 0;
    wchar temp_wch[]                      = L" ";
    uint16 to_alloc_len                   = 0;
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    BOOLEAN is_alloc                      = FALSE;
    wchar semicolon[]                     = {';', 0};

    edit_doc_ptr = g_mms_edit_globle.editdoc;
    numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
    temp_numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
    if (PNULL != edit_doc_ptr && PNULL != numberlist_ptr)   
    {
        while(PNULL != temp_numberlist_ptr)
        {
            to_alloc_len = to_alloc_len + MAX(temp_numberlist_ptr->number_len, temp_numberlist_ptr->number_name_len);
            temp_numberlist_ptr = temp_numberlist_ptr->next;
        }
        to_alloc_len = to_alloc_len + MMIMMS_MAX_RECEIVER_NUM + 1;//分号
        if(0 < to_alloc_len)
        {
            to_wstr = (wchar *)MMIMMS_ALLOC(to_alloc_len * sizeof(wchar));
            is_alloc = TRUE;
            SCI_MEMSET(to_wstr, 0, to_alloc_len * sizeof(wchar));
            //SCI_TRACE_LOW:"MMSEditAddToItem %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10920_112_2_18_2_38_1_94,(uint8*)"d", to_alloc_len);
            while(PNULL != numberlist_ptr)
            {
                if(numberlist_ptr->is_object && PNULL != numberlist_ptr->number_name_ptr)
                {
                    MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_name_ptr, numberlist_ptr->number_name_len);
                    to_len = to_len + numberlist_ptr->number_name_len;
                }
                else
                {
                    MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_ptr, numberlist_ptr->number_len);
                    to_len = to_len + numberlist_ptr->number_len;
                }
                MMIAPICOM_Wstrncpy(to_wstr + to_len, semicolon, MMIAPICOM_Wstrlen(semicolon));
                to_len = to_len + MMIAPICOM_Wstrlen(semicolon);
                numberlist_ptr = numberlist_ptr->next;
            }            
        }
    }
    else
    {
        to_wstr = temp_wch;
        to_len = MMIAPICOM_Wstrlen(to_wstr);
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_MMS_EDIT_TO;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = to_wstr;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(to_wstr);
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    item.text_set_font = TRUE;
    item.setting_font = MMIAPISMS_GetWinFont();
#endif

#ifdef MMI_PDA_SUPPORT
    item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL | GUIRICHTEXT_TAG_IMG_NORMAL;/*lint !e655*/
#else
	item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
#endif
    item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
    
    GUIRICHTEXT_AddItem(MMIMMS_EDIT_MENU_CTRL_ID, &item, &index);
    g_mms_edit_richtext_item_type[index] = MMIMMS_RICHTEXT_TO;
    if (is_alloc)
    {
        MMIMMS_FREE(to_wstr);
        to_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :add richtext  additional file list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddAddFileItem(MMIMMS_RICHTEXT_OP_TYPE is_add, char * file_name_ptr)
{
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    wchar *file_name_wstr                 = PNULL;
    uint32 file_str_len                   = 0;
    wchar temp_wch[]                      = L" ";
//  uint16 to_alloc_len                   = 0;
    uint16 file_name_len                  = 0;

    file_name_len = strlen(file_name_ptr);
    
    if (0 != file_name_len)   
    {
        file_str_len = (file_name_len+ 1) * sizeof(wchar);
        file_name_wstr = (wchar *)MMIMMS_ALLOC(file_str_len);
        SCI_MEMSET(file_name_wstr, 0, file_str_len);
        //MMI_STRNTOWSTR(file_name_wstr, file_name_len, file_name_ptr, file_name_len,file_name_len);
        file_str_len = GUI_UTF8ToWstr(file_name_wstr, (file_name_len+ 1)*2, (uint8 *)file_name_ptr, file_name_len);
        //file_str_len = file_name_len;
        //SCI_TRACE_LOW:"MMSEditAddToItem %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_10998_112_2_18_2_38_1_95,(uint8*)"d", file_str_len);
    }
    else
    {
        file_name_wstr = temp_wch;
        file_str_len = MMIAPICOM_Wstrlen(file_name_wstr);
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_MMS_ACCESSORY_FILE;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = file_name_wstr;
    item.text_data.buf.len = file_str_len;
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    item.text_set_font = TRUE;
    item.setting_font = MMIAPISMS_GetWinFont();
#endif

    item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
    
    GUIRICHTEXT_AddItem(MMIMMS_EDIT_MENU_CTRL_ID, &item, &index);
    g_mms_edit_richtext_item_type[index] = MMIMMS_RICHTEXT_ADDFILE;
    if (0 != MMIAPICOM_Wstrcmp(file_name_wstr, temp_wch))
    {
        MMIMMS_FREE(file_name_wstr);
        file_name_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :add richtext subject item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddSubjectItem(MMIMMS_RICHTEXT_OP_TYPE is_add)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    wchar *subject_wstr                   = PNULL;
    uint16 subject_len                    = 0;
    wchar temp_wch[]                      = L" ";
    uint16 temp_len                       = 0;

    edit_doc_ptr = g_mms_edit_globle.editdoc;

    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->edit_subject.wstr_ptr && 0 != edit_doc_ptr->edit_subject.wstr_len)   
    {
        temp_len = (edit_doc_ptr->edit_subject.wstr_len + 1) * sizeof(wchar);
        subject_wstr = (wchar *)MMIMMS_ALLOC(temp_len);
        SCI_MEMSET((char *)subject_wstr, 0, temp_len);
        subject_len = MIN(MMIMMS_MAX_SUBJECT_LEN , edit_doc_ptr->edit_subject.wstr_len);
        MMIAPICOM_Wstrncpy(subject_wstr, edit_doc_ptr->edit_subject.wstr_ptr, subject_len);
        
    }
    else
    {
        subject_wstr = temp_wch;
        subject_len = MMIAPICOM_Wstrlen(subject_wstr);
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_MMS_EDIT_SUBJECT;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = subject_wstr;
    item.text_data.buf.len = subject_len;
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    item.text_set_font = TRUE;
    item.setting_font = MMIAPISMS_GetWinFont();
#endif

    item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    //item.tag_type = GUIRICHTEXT_TAG_NORMAL;
    item.align_type = GUIRICHTEXT_ALIGN_RIGHT;;
    
    GUIRICHTEXT_AddItem(MMIMMS_EDIT_MENU_CTRL_ID, &item, &index);
    g_mms_edit_richtext_item_type[index] = MMIMMS_RICHTEXT_SUBJECT;
    if (0 != MMIAPICOM_Wstrcmp(subject_wstr, temp_wch))
    {
        MMIMMS_FREE(subject_wstr);
        subject_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :add richtext text item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddTextItem(MMIMMS_RICHTEXT_OP_TYPE is_add)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    MMI_STRING_T text_str                 = {0};

    edit_doc_ptr = g_mms_edit_globle.editdoc;

    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    {
        if (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 1)
        {
            MMIMMS_GetCurrentSlideText(edit_doc_ptr, &text_str);
        }
    }
    if (text_str.wstr_ptr == PNULL)
    {
        MMI_GetLabelTextByLang(TXT_TEXT, &text_str);
        item.text_set_align = TRUE;
        item.setting_align = ALIGN_HVMIDDLE;
    }   
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = text_str.wstr_ptr;
    item.text_data.buf.len = text_str.wstr_len;
    //item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    item.text_set_font = TRUE;
    item.setting_font = MMIAPISMS_GetWinFont();
#endif
    
    item.tag_type = GUIRICHTEXT_TAG_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;

    GUIRICHTEXT_AddItem(MMIMMS_EDIT_MENU_CTRL_ID, &item, &index);
    g_mms_edit_richtext_item_type[index] = MMIMMS_RICHTEXT_TEXT;
}
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add form pic item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddPicItem(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint8 *image_buf_ptr                  = PNULL;
    uint32 image_buf_len                  = 0;
    uint32 image_type                     = 0;
    GUIANIM_CTRL_INFO_T anim_ctrl_info = {0};        
    GUIANIM_DISPLAY_INFO_T anim_disp_info = {0};
    GUIANIM_DATA_INFO_T anim_data_info = {0};
    edit_doc_ptr = g_mms_edit_globle.editdoc;

    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    {
        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)//图片存在
        {
#ifdef DRM_SUPPORT
            uint8 file_name_str[MMIMMS_FILE_NAME_LEN + 1] = {0};            
            
            MMIMMS_GetCurrentSlideImageInfo(file_name_str, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str))
            {

                //SCI_TRACE_LOW:"MMSEditAddPicItem drm file "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_11158_112_2_18_2_38_2_96,(uint8*)"");
                
                anim_ctrl_info.is_ctrl_id = TRUE;
                anim_ctrl_info.ctrl_id = MMIMMS_SMS_EDIT_ANIM_CTRL_ID;

                anim_data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
                
                anim_disp_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
                GUIANIM_SetParam(&anim_ctrl_info, &anim_data_info, PNULL, &anim_disp_info);
            }
            else
#endif
            {
                //SCI_TRACE_LOW:"MMSEditAddPicItem"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_11171_112_2_18_2_38_2_97,(uint8*)"");
                if (MMK_IsOpenWin(MMIFMM_PICTURE_PREVIEW_WIN_ID))
                {
                    //关闭图像浏览窗口
                    MMK_CloseWin(MMIFMM_PICTURE_PREVIEW_WIN_ID);
                }
                MMIMMS_GetCurrentSlideImage(&image_type, &image_buf_len, &image_buf_ptr, edit_doc_ptr);
                if (PNULL != image_buf_ptr)
                {
                    anim_ctrl_info.is_ctrl_id = TRUE;
                    anim_ctrl_info.ctrl_id = MMIMMS_SMS_EDIT_ANIM_CTRL_ID;
    
                    anim_data_info.data_ptr= image_buf_ptr;
                    anim_data_info.data_size = image_buf_len;
                    
                    anim_disp_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
                    GUIANIM_SetParam(&anim_ctrl_info, &anim_data_info, PNULL, &anim_disp_info);
                }
                else
                {
                    //get data error
                }
                MMSSetMMSPicHeight(image_buf_ptr,image_buf_len);
            }            

        }
        else if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
        {
#ifdef DRM_SUPPORT
            uint8 file_name_str[MMIMMS_FILE_NAME_LEN + 1] = {0};
            
            MMIMMS_GetCurrentSlideVideoInfo(file_name_str, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);            
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str))
            {
                anim_data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }
            else
#endif
            {
                anim_data_info.img_id = IMAGE_MMS_EDIT_VIDEO;
            }
            anim_ctrl_info.is_ctrl_id = TRUE;
            anim_ctrl_info.ctrl_id = MMIMMS_SMS_EDIT_ANIM_CTRL_ID;
            anim_disp_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            GUIANIM_SetParam(&anim_ctrl_info, &anim_data_info, PNULL, &anim_disp_info);
        }//短彩合一不显示如没有图片不显示默认图片
    } 

}
#else
/*****************************************************************************/
//  Description :add richtext pic item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddPicItem(MMIMMS_RICHTEXT_OP_TYPE is_add)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
//  GUIRES_IMG_SRC_T img_src              = {0};
//  GUIRES_IMG_INFO_T info_ptr            = {0};
    uint8 *image_buf_ptr                  = PNULL;
    uint32 image_buf_len                  = 0;
    uint32 image_type                     = 0;
    uint16 pic_width                      = 0;
    uint16 pic_height                     = 0;

    edit_doc_ptr = g_mms_edit_globle.editdoc;
    MMIMMS_GetPicDefaultWidthAndHeight(&pic_width, &pic_height);

    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    {
        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)//图片存在
        {
#ifdef DRM_SUPPORT
            uint8 file_name_str[MMIMMS_FILE_NAME_LEN + 1] = {0};            
            
            MMIMMS_GetCurrentSlideImageInfo(file_name_str, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str))
            {
                //SCI_TRACE_LOW:"MMSEditAddPicItem drm file "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_11251_112_2_18_2_38_2_98,(uint8*)"");
                item.img_type = GUIRICHTEXT_IMAGE_RES;
                item.img_data.res.id = IMAGE_DRM_PIC_LOCK_ICON;
                
                item.img_set_size = TRUE;
                item.img_setting_width  = pic_width;
                item.img_setting_height = pic_height;
                item.img_frame_type = GUIRICHTEXT_FRAME_NONE;
            }
            else
#endif
            {
                //SCI_TRACE_LOW:"MMSEditAddPicItem"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_11263_112_2_18_2_38_2_99,(uint8*)"");
                if (MMK_IsOpenWin(MMIFMM_PICTURE_PREVIEW_WIN_ID))
                {
                    //关闭图像浏览窗口
                    MMK_CloseWin(MMIFMM_PICTURE_PREVIEW_WIN_ID);
                }
                MMIMMS_GetCurrentSlideImage(&image_type, &image_buf_len, &image_buf_ptr, edit_doc_ptr);
                if (PNULL != image_buf_ptr)
                {
                    item.img_type = GUIRICHTEXT_IMAGE_BUF;
                    item.img_data.buf.buf_ptr = image_buf_ptr;
                    item.img_data.buf.size = image_buf_len;
                    
                    item.img_set_size = TRUE;
                    item.img_setting_width  = pic_width;
                    item.img_setting_height = pic_height;
                    item.img_frame_type = GUIRICHTEXT_FRAME_NONE;
                }
                else
                {
                    //get data error
                }
            }            
        }
        else if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
        {
#ifdef DRM_SUPPORT
            uint8 file_name_str[MMIMMS_FILE_NAME_LEN + 1] = {0};
            
            MMIMMS_GetCurrentSlideVideoInfo(file_name_str, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);            
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str))
            {
                item.img_type = GUIRICHTEXT_IMAGE_RES;
                item.img_data.res.id = IMAGE_DRM_PIC_LOCK_ICON;
            }
            else
#endif
            {
                item.img_type = GUIRICHTEXT_IMAGE_RES;
                item.img_data.res.id = IMAGE_MMS_EDIT_VIDEO;
            }

            item.img_set_size = TRUE;
            item.img_setting_width  = pic_width;
            item.img_setting_height = pic_height;
            item.img_frame_type = GUIRICHTEXT_FRAME_NONE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMSEditAddPicItem jackal12"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_11312_112_2_18_2_38_2_100,(uint8*)"");
            item.img_type = GUIRICHTEXT_IMAGE_RES;
            item.img_data.res.id = IMAGE_MMS_EDIT_ADD_PIC_OR_VIDEO;

            item.img_set_size = TRUE;
            item.img_setting_width  = pic_width;
            item.img_setting_height = pic_height;
            item.img_frame_type = GUIRICHTEXT_FRAME_NONE;
        }
    } 
    //SCI_TRACE_LOW:"MMSEditAddPicItem jackal13"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_11322_112_2_18_2_38_2_101,(uint8*)"");
    item.tag_type = GUIRICHTEXT_TAG_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;

    GUIRICHTEXT_AddItem(MMIMMS_EDIT_MENU_CTRL_ID, &item, &index);
    g_mms_edit_richtext_item_type[index] = MMIMMS_RICHTEXT_PIC_OR_MOVIE;
}
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :add richtext music item
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddMusicItem(void)
{
      GUIANIM_CTRL_INFO_T anim_ctrl_info = {0};        
      GUIANIM_DISPLAY_INFO_T anim_disp_info = {0};
      GUIANIM_DATA_INFO_T    anim_data_info = {0};
      MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;      
      wchar music_name_wstr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
      uint16 music_name_wlen                 = 0;
      uint8 *music_name_ptr                  = PNULL;
      uint16 music_name_len                  = 0;
      GUIFORM_CHILD_WIDTH_T  width = {0};
      edit_doc_ptr = g_mms_edit_globle.editdoc;
      if(PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
      {
          return; 
      }
      if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
      {            
      music_name_ptr = (uint8 *)edit_doc_ptr->editbody_ptr->slide_cur->audiofile; 
      music_name_len = (uint8)strlen((char *)music_name_ptr);
      music_name_wlen = GUI_UTF8ToWstr(music_name_wstr, music_name_len, music_name_ptr, music_name_len);
      }
      anim_ctrl_info.is_ctrl_id = TRUE;
      anim_ctrl_info.ctrl_id = MMIMMS_SMS_EDIT_MUSIC_ANIM_ID;
            
      anim_disp_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
      anim_data_info.img_id = IMAGE_MMS_EDIT_MUSIC;

      width.type = GUIFORM_CHILD_WIDTH_PERCENT;
      width.add_data = 80;
      GUIFORM_SetChildWidth(MMIMMS_SMS_EDIT_MUSIC_FORM_ID, MMIMMS_SMS_EDIT_MUSIC_TEXT_ID, &width);
      
      GUIANIM_SetParam(&anim_ctrl_info, &anim_data_info, PNULL, &anim_disp_info);            
      GUITEXT_SetString(MMIMMS_SMS_EDIT_MUSIC_TEXT_ID, music_name_wstr, music_name_wlen, FALSE);
}
#else
/*****************************************************************************/
//  Description :add richtext music item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddMusicItem(MMIMMS_RICHTEXT_OP_TYPE is_add)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;
    GUIRICHTEXT_ITEM_T item                = {0};/*lint !e64*/
    uint16 index                           = 0;
    wchar music_name_wstr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 music_name_wlen                 = 0;
    uint8 *music_name_ptr                  = PNULL;
    uint16 music_name_len                  = 0;
    uint16 music_width                     = 0;
    uint16 music_height                    = 0;

    edit_doc_ptr = g_mms_edit_globle.editdoc;
    MMIMMS_GetMusicDefaultWidthAndHeight(&music_width, &music_height);
    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    { 
        if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
        {            
            music_name_ptr = (uint8 *)edit_doc_ptr->editbody_ptr->slide_cur->audiofile; 
            music_name_len = (uint8)strlen((char *)music_name_ptr);
            music_name_wlen = GUI_UTF8ToWstr(music_name_wstr, music_name_len, music_name_ptr, music_name_len);
            //music_name_wlen = MMIMMS_GetFileNameFromVfname(edit_doc_ptr,music_name_wstr,MMIFILE_FILE_NAME_MAX_LEN,music_name_ptr,music_name_len);
        }
        else
        {
            //SCI_TRACE_LOW:"MMSEditAddMusicItem: no audio"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_11402_112_2_18_2_38_2_102,(uint8*)"");
        }
    }
      
    if (PNULL == music_name_ptr)
    { 
        item.img_type = GUIRICHTEXT_IMAGE_RES;
#ifdef MMI_GUI_STYLE_MINISCREEN
        item.img_data.res.id = IMAGE_COMMON_MUSIC_UDISK_ICON;//显示资源图片：音频.bmp
#else
        item.img_data.res.id = IMAGE_SECMENU_ICON_MULTIM_MP3;//显示资源图片：音频.bmp
#endif
        item.img_set_size = TRUE;
        item.img_setting_width  = music_width;
        item.img_setting_height = music_height;
        item.img_frame_type = GUIRICHTEXT_FRAME_NONE;
        
    }
    else 
    {
#ifdef DRM_SUPPORT  
        if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(music_name_ptr) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(music_name_ptr))
        {
            item.img_type = GUIRICHTEXT_IMAGE_RES;
            item.img_data.res.id = IMAGE_DRM_AUDIO_LOCK_ICON;
        }
        else              
#endif
        {
            item.img_type = GUIRICHTEXT_IMAGE_RES;
            item.img_data.res.id = IMAGE_SECMENU_ICON_MULTIM_MP3;//显示资源图片：音频.bmp
        }
        item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
        item.text_type = GUIRICHTEXT_TEXT_BUF;
        item.text_data.buf.str_ptr = music_name_wstr;
        item.text_data.buf.len = music_name_wlen;
        
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
        item.text_set_font = TRUE;
        item.setting_font = MMIAPISMS_GetWinFont();       
#endif
    }
    item.tag_type = GUIRICHTEXT_TAG_NORMAL;
    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    GUIRICHTEXT_AddItem(MMIMMS_EDIT_MENU_CTRL_ID, &item, &index);
    g_mms_edit_richtext_item_type[index] = MMIMMS_RICHTEXT_MUSIC;
}


/*****************************************************************************/
//  Description :add richtext add multi file menu item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAddMenuItem(MMIMMS_RICHTEXT_OP_TYPE is_add)
{
    GUIRICHTEXT_ITEM_T item = {0};/*lint !e64*/
    uint16 index            = 0;
//  uint16 slide_cur_index  = 1;
    uint16 modify_index     = MMIMMS_RICHTEXT_ITEM_NUM_MAX;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;

    edit_doc_ptr = g_mms_edit_globle.editdoc;

    if (PNULL == edit_doc_ptr)
    {
        return;
    }
    modify_index = GetMMSEditItemIndex(MMIMMS_RICHTEXT_ADDMENU);
    if (GetMMSEditFocusOnAddMenu())
    {
        item.img_type = GUIRICHTEXT_IMAGE_RES;
        item.img_data.res.id = IMAGE_MMS_EDIT_ADD_MENU_FOCUS;//显示资源图片：.bmp        
    }
    else
    {
        item.img_type = GUIRICHTEXT_IMAGE_RES;
        item.img_data.res.id = IMAGE_MMS_EDIT_ADD_MENU;//显示资源图片：.bmp
    }    
    item.tag_type = GUIRICHTEXT_TAG_IMG_NORMAL;
    item.align_type = GUIRICHTEXT_ALIGN_DEFAULT;
    item.frame_type = GUIRICHTEXT_FRAME_NONE;
    
    if (MMIMMS_RICHTEXT_ADD_ITEM == is_add)
    {
        GUIRICHTEXT_AddItem(MMIMMS_EDIT_MENU_CTRL_ID, &item, &index);
        g_mms_edit_richtext_item_type[index] = MMIMMS_RICHTEXT_ADDMENU;
    }
    else
    {
        GUIRICHTEXT_ModifyItem(MMIMMS_EDIT_MENU_CTRL_ID, modify_index, &item);
    }    
    return;
}
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :open mms edit option menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSEditOptPopMenu(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint16 menu_index                     = 0;
    uint16 child_index                    = 0;
    MMI_CTRL_ID_T	ctrl_id               = MMIMMS_EDIT_MAINMENU_OPT_CTRL_ID;
    uint16 slide_num                      = 0;
    uint16 content_num                    = 0;
    uint32 additional_file_num            = 0;//还差接口调用
    MMI_HANDLE_T edit_handle = 0;
    uint16              item_num = 0;
    
    MMI_STRING_T edit_str= {0};
    MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
            
    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str); 
    if(MMIMMS_GetIsMMSMode())
    {
        edit_doc_ptr = g_mms_edit_globle.editdoc;
        if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
        {
            return;
        }
        content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
        MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);           
        slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
    }
    else
    {
        edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
        
                // to save the information
        //MMIAPISMS_SetGloblalMsgContent(  //供保存草稿使用
                                    //edit_str.wstr_len,
                                    //edit_str.wstr_ptr
                                  //  );
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
        if (MMISMS_GetAutoSignatureFlag() && !MMIMMS_GetIsMMSMode())
        {
            MMI_STRING_T sig_string = {0};
            MMI_STRING_T display_string = {0};  
            wchar        signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
            uint16       signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
            signature_wstr[0] = 0x0a00;
#else
            signature_wstr[0] = 0x000a;
#endif
            signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr + 1) + 1;
            
            sig_string.wstr_ptr = signature_wstr;
            sig_string.wstr_len = signature_str_len;
            
            display_string.wstr_ptr = (wchar *)SCI_ALLOCAZ( ( MMISMS_MAX_MESSAGE_LEN + 1 ) * sizeof(wchar) );
            
            if (PNULL == display_string.wstr_ptr)
            {
                return;
            }
            
            MMIAPICOM_MergeTwoString(
                &edit_str,
                &sig_string,
                &display_string,
                MMISMS_MAX_MESSAGE_LEN
                );
            
            // to save the information
            MMIAPISMS_SetMessageContent( 
                display_string.wstr_len,
                display_string.wstr_ptr, 
                TRUE, 
                edit_content_ptr
                );
            
            SCI_FREE(display_string.wstr_ptr);
        }
        else
#endif
        {
                    
            MMIAPISMS_SetMessageContent(edit_str.wstr_len, edit_str.wstr_ptr, TRUE, edit_content_ptr);       
        }
         
    }

   
    MMK_CreateWin((uint32 *)MMIMMS_EDIT_MAINMENU_OPT_WIN_TAB,PNULL); 
    GUIMENU_CreatDynamic(PNULL, MMIMMS_EDIT_MAINMENU_OPT_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                    MMIMMS_EDIT_MENU_WIN_ID,
                                    MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                    );
#else
    edit_handle = MMIMMS_SMS_EDIT_TO_LIST_ID;
#endif
    item_num = GUIEDIT_GetListItemNum(edit_handle);

#ifndef MMI_PDA_SUPPORT
    if(((0 == edit_str.wstr_len || 0 == item_num) && !MMIMMS_GetIsMMSMode())
        || (MMIMMS_GetIsMMSMode() && 0 == edit_str.wstr_len && 0 == content_num && 0 == additional_file_num))
    {
        //for CR126474 need gray "send"
        InsertNodeByLabelEx(
            ctrl_id, 
            TXT_SEND,
            IMAGE_NULL,
            MMS_CONTEDIT_OPT_NODE_SEND, 
            0,
            menu_index++,
            TRUE);
    }
    else
    {
        InsertNodeByLabelEx( 
            ctrl_id, 
            TXT_SEND,
            IMAGE_NULL,
            MMS_CONTEDIT_OPT_NODE_SEND, 
            0,
            menu_index++,
            FALSE);
    }
#endif

    //预览彩信
    if (MMIMMS_GetIsMMSMode()
        && (0 != edit_str.wstr_len || 0 != content_num || 0 != additional_file_num))
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            STXT_PREVIEW,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_PREV,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
    
    
    InsertNodeByLabelEx(  //插入联系人信息，短彩合一用
        ctrl_id, 
        TXT_SMS_ADD_CONTACTINFO,
#ifdef MMI_PDA_SUPPORT        
        IMAGE_MESSAGE_OPTMENU_INSERT_CONTACT,
#else
        IMAGE_NULL,
#endif
        MMS_CONTEDIT_OPT_NODE_INSERT_CONTACT_INFO, 
        0,
        menu_index++,
        FALSE);
        
    InsertNodeByLabelEx( //插入表情符，短彩合一用
        ctrl_id, 
        TXT_SMS_ADD_PHIZ,
#ifdef MMI_PDA_SUPPORT         
        IMAGE_MESSAGE_OPTMENU_INSERT_PHIZ,
#else
        IMAGE_NULL,
#endif        
        MMS_CONTEDIT_OPT_NODE_INSERT_PHIZ, 
        0,
        menu_index++,
        FALSE);        

    if(GetMMSEditIsSubjectExist())//插入/ 移除主题，短彩合一用
    {
        InsertNodeByLabelEx(
        ctrl_id, 
        TXT_MMS_SMS_REMOVE_THEME,
#ifdef MMI_PDA_SUPPORT                
        IMAGE_MESSAGE_OPTMENU_INSERT_THEME,
#else
        IMAGE_NULL,
#endif
        MMS_CONTEDIT_OPT_NODE_DEL_SUBJECT, 
        0,
        menu_index++,
        FALSE); 
    }
    else
    {
        InsertNodeByLabelEx(
        ctrl_id, 
        TXT_MMS_SMS_ADD_THEME,
#ifdef MMI_PDA_SUPPORT        
        IMAGE_MESSAGE_OPTMENU_INSERT_THEME,
#else
        IMAGE_NULL,
#endif
        MMS_CONTEDIT_OPT_NODE_INSERT_SUBJECT, 
        0,
        menu_index++,
        FALSE); 
    }
#ifdef MMIMMS_SMS_CC_SUPPORT    
    InsertNodeByLabelEx(//设置抄送密送
    ctrl_id, 
    TXT_COMM_CC_BCC,
//#ifdef MMI_PDA_SUPPORT     
//    IMAGE_MESSAGE_OPTMENU_INSERT_CARBON_COPY,
//#else
    IMAGE_NULL,
//#endif    
    MMS_CONTEDIT_OPT_NODE_CARBON_COPY,
    0,
    menu_index++,
    FALSE); 

    if(GetMMSEditIsCCExist())  //设置抄送
    {
        InsertNodeByLabel(
        ctrl_id, 
        TXT_MMS_SMS_REMOVE_CC_COPY,
        MMS_CONTEDIT_OPT_NODE_DELCC, 
        MMS_CONTEDIT_OPT_NODE_CARBON_COPY,
        child_index++,
        FALSE); 
    }
    else
    {
        InsertNodeByLabel(
        ctrl_id, 
        TXT_MMS_SMS_ADD_CC_COPY,
        MMS_CONTEDIT_OPT_NODE_INSCC, 
        MMS_CONTEDIT_OPT_NODE_CARBON_COPY,
        child_index++,
        FALSE); 
    }

    if(GetMMSEditIsBCCExist())  //设置密送
    {
        InsertNodeByLabel(
        ctrl_id, 
        TXT_MMS_SMS_REMOVE_BCC_COPY,
        MMS_CONTEDIT_OPT_NODE_DELBCC, 
        MMS_CONTEDIT_OPT_NODE_CARBON_COPY,
        child_index++,
        FALSE); 
    }
    else
    {
        InsertNodeByLabel(
        ctrl_id, 
        TXT_MMS_SMS_ADD_BCC_COPY,
        MMS_CONTEDIT_OPT_NODE_INSBCC, 
        MMS_CONTEDIT_OPT_NODE_CARBON_COPY,
        child_index++,
        FALSE); 
    }
    
    child_index  = 0;
#endif   

#ifndef MMI_PDA_SUPPORT
    InsertNodeByLabelEx(  
        ctrl_id, 
        TXT_SMS_INSERT_WORD,
        IMAGE_NULL,
        MMS_CONTEDIT_OPT_NODE_INSERT_TEMPLATE, 
        0,
        menu_index++,
        FALSE);

    InsertNodeByLabelEx(  
        ctrl_id, 
        STXT_ADD,
        IMAGE_NULL,
        MMS_CONTEDIT_OPT_NODE_INSERT, 
        0,
        menu_index++,
        FALSE);
#endif

    if(MMIMMS_GetIsMMSMode())
    {
            if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr ||PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
            {
               return;
            }
           
            if((1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
               ||(0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
               ||(0 != edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
               ||(0 != additional_file_num )
               ||( slide_num > 0 )) //这些元素有一个则显示插入
            {
                InsertNodeByLabelEx(//设置抄送密送
                ctrl_id, 
                TXT_MMS_SMS_DELETE_INSERT_FILE,
#ifdef MMI_PDA_SUPPORT                
                IMAGE_COMMON_OPTMENU_DELETE,
#else
                IMAGE_NULL,
#endif                   
                MMS_CONTEDIT_OPT_NODE_DELETE,
                0,
                menu_index++,
                FALSE);
            }
            
            //删除当前帧图片
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
            {
                InsertNodeByLabel(
                    ctrl_id,//ctrl_id
                    TXT_IMAGE,//text_ptr 
                    MMS_CONTEDIT_OPT_NODE_DELIMAGE,//node_id
                    MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
                    child_index++,//index
                    FALSE
                    );
            }
        
            //删除当前帧音频
            if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
            {
                InsertNodeByLabel(
                    ctrl_id,//ctrl_id
                    TXT_AUDIO,//text_ptr 
                    MMS_CONTEDIT_OPT_NODE_DELMUSIC,//node_id
                    MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
                    child_index++,//index
                    FALSE
                    );
            }
#ifdef VIDEO_PLAYER_SUPPORT   
            //删除当前帧视频
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
            {
                InsertNodeByLabel(
                    ctrl_id,//ctrl_id
                    TXT_VIDEO,//text_ptr 
                    MMS_CONTEDIT_OPT_NODE_DELVIDEO,//node_id
                    MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
                    child_index++,//index
                    FALSE
                    );
            }
#endif
            //删除当前帧
            if (slide_num > 0)
            {
                InsertNodeByLabel(
                    ctrl_id,//ctrl_id
                    TXT_MMS_DEL_CUR_SLIDE,//text_ptr 
                    MMS_CONTEDIT_OPT_NODE_DELSLIDE,//node_id
                    MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
                    child_index++,//index
                    FALSE
                    );
            }
                         //删除附件
            if (additional_file_num != 0)
            {
                InsertNodeByLabel(
                    ctrl_id,//ctrl_id
                    TXT_MMS_ATTACHMENT,//text_ptr 
                    MMS_CONTEDIT_OPT_NODE_DELADDFILE,//node_id
                    MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
                    child_index++,//index
                    FALSE
                    );
            } 
    }

#ifdef MMI_TIMERMSG_SUPPORT  
    if(!MMIMMS_GetIsMMSMode()) //打开/关闭定时发送，短信的情况
    {
        if(!MMIAPISMS_GetIsTimerMode())  
        {
            InsertNodeByLabelEx(
            ctrl_id, 
            TXT_SMS_OPEN_TIMER_MESSAGE,
#ifdef MMI_PDA_SUPPORT            
            IMAGE_MESSAGE_OPTMENU_SMS_TIMER,
#else
            IMAGE_NULL,
#endif
            MMS_CONTEDIT_OPT_NODE_OPEN_TIMERMSG,
            0,
            menu_index++,
            FALSE); 
        }
        else
        {
            InsertNodeByLabelEx(
            ctrl_id, 
            TXT_SMS_CLOSE_TIMER_MESSAGE,
#ifdef MMI_PDA_SUPPORT             
            IMAGE_MESSAGE_OPTMENU_SMS_TIMER,
#else
            IMAGE_NULL,
#endif            
            MMS_CONTEDIT_OPT_NODE_CLOSE_TIMERMSG,
            0,
            menu_index++,
            FALSE); 
        }        
    }
#endif

    //帧页面定时
    if(MMIMMS_GetIsMMSMode())
    {
    InsertNodeByLabelEx(
        ctrl_id,//ctrl_id
        TXT_MMS_SET_SLIDE_TIME_OPT,//text_ptr 
#ifdef MMI_PDA_SUPPORT            
        IMAGE_MESSAGE_OPTMENU_SMS_TIMER,
#else
        IMAGE_NULL,
#endif
        MMS_CONTEDIT_OPT_NODE_SETTIME,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );
#ifndef MMI_PDA_SUPPORT 
    child_index = 0;
    //文字排版
    InsertNodeByLabelEx(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS,//text_ptr 
        IMAGE_NULL,
        MMS_CONTEDIT_OPT_NODE_TEXT_POS,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );


    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS_UP,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST,//node_id
        MMS_CONTEDIT_OPT_NODE_TEXT_POS, //parent_node_id
        child_index++,//index
        FALSE
        );

    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS_DOWN,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST,//node_id
        MMS_CONTEDIT_OPT_NODE_TEXT_POS, //parent_node_id
        child_index++,//index
        FALSE
        );
#endif    
    }
    InsertNodeByLabelEx(
    ctrl_id,//ctrl_id
    TXT_COMM_SAVE_AS_DRAFT,//text_ptr 
#ifdef MMI_PDA_SUPPORT    
    IMAGE_MESSAGE_OPTMENU_SAVE_DRAFT,
#else
    IMAGE_NULL,
#endif
    MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT,//node_id
    0, //parent_node_id
    menu_index++,//index
    FALSE
    );

    return;    
}

#else
/*****************************************************************************/
//  Description :open mms edit option menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSEditOptPopMenu(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint16 menu_index                     = 0;
    uint16 child_index                    = 0;
    MMI_CTRL_ID_T	ctrl_id               = MMIMMS_EDIT_MAINMENU_OPT_CTRL_ID;
//  BOOLEAN is_have_video                 = FALSE;
    uint16 slide_num                      = 0;
    uint16 content_num                    = 0;
    uint32 additional_file_num            = 0;//还差接口调用
    MMS_TEXT_IMAGE_ORDER_E text_ordor     = MMS_ORDER_TEXT_FIRST;
    GUIMENU_POP_SEL_INFO_T pop_info       = {0};
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
    {
        return;
    }
    content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
    MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);

    MMK_CreateWin((uint32 *)MMIMMS_EDIT_MAINMENU_OPT_WIN_TAB,PNULL);
    GUIMENU_CreatDynamic(PNULL, MMIMMS_EDIT_MAINMENU_OPT_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);

     slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);

    #ifndef MMI_PDA_SUPPORT
        //发送彩信  
    if (edit_doc_ptr->edit_to.wstr_ptr != PNULL && edit_doc_ptr->edit_to.wstr_len != 0
        && (0 != content_num || 0 != additional_file_num))
    {
        InsertNodeByLabel(
        ctrl_id,                    //ctrl_id
        TXT_SEND,               //text_ptr 
        MMS_CONTEDIT_OPT_NODE_SEND, //node_id
        0,                          //parent_node_id
        menu_index++,               //index
        FALSE
        );
    } 
    else
    {
	    //for CR126474 need gray "send"
        InsertNodeByLabel(
        ctrl_id,                    //ctrl_id
        TXT_SEND,               //text_ptr 
        MMS_CONTEDIT_OPT_NODE_SEND, //node_id
        0,                          //parent_node_id
        menu_index++,               //index
        TRUE
        );
    }
    //预览彩信
    if (0 != content_num || 0 != additional_file_num)
    {    
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            STXT_PREVIEW,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_PREV,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }

    //添加收件人
    InsertNodeByLabel(
        ctrl_id,                         //ctrl_id
        TXT_CMS_MMS_ADD_TO,              //text_ptr 
        MMS_CONTEDIT_OPT_NODE_ADD_RECV,  //node_id
        0,                               //parent_node_id
        menu_index++,                    //index
        FALSE
        );
   #endif 
    
    InsertNodeByLabel(
            ctrl_id,//ctrl_id
            STXT_ADD,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSERT,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
        );
    
    //插入文本
    if (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 0)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
#ifdef MMI_GUI_STYLE_MINISCREEN
            TXT_MMS_EDIT_TEXT,
#else
            TXT_TEXT,//text_ptr 
#endif
            MMS_CONTEDIT_OPT_NODE_INSTEXT,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
        );
    }
    
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_SMS_ADD_PHIZ,
        MMS_CONTEDIT_OPT_NODE_INSERT_PHIZ,//node_id
        MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
        child_index++,//index
        FALSE
        );

    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_SMS_INSERT_WORD,
        MMS_CONTEDIT_OPT_NODE_INSERT_TEMPLATE,//node_id
        MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
        child_index++,//index
        FALSE
        );

    //插入新页
    if (slide_num < MMIMMS_MAX_SLIDE)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_INSERT_SLIDE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSSLIDE,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
          InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_INSERT_SLIDER,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSSLIDER,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //插入图片
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_IMAGE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSIMAGE,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
    
    //Take DV
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_SMS_TAKE_PIC,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INDC,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //插入音频文件
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_AUDIO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSMUSIC,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
#ifdef MMI_RECORD_SUPPORT
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_SMS_TAKE_AUDIO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INAU,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );       
#endif
    }
#ifdef VIDEO_PLAYER_SUPPORT
    //插入视频文件
    if (   0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag )
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_VIDEO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSVIDEO,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    if (   0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag )
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_SMS_TAKE_VIDEO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INDV,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#endif
#ifdef CMCC_UI_STYLE
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            STXT_MAIN_CAMERA,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INDC,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#endif

    //插入附件
    if (additional_file_num < MMIMMS_MAX_ADDFILE_NUM)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_ATTACHMENT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSADDFILE,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
 
    child_index = 0;
    //删除
    if (slide_num == 1 && (content_num - additional_file_num) == 0)
    {
#if 0    
#if defined(MMI_MMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)
        InsertNodeByLabel(
#else
        InsertNodeByLabelEx(
#endif
            ctrl_id,//ctrl_id
            TXT_DELETE,//text_ptr 
#if !defined(MMI_MMS_MINI_SUPPORT) && !defined(MMI_GUI_STYLE_MINISCREEN)
#ifdef MMI_PDA_SUPPORT            
            IMAGE_COMMON_OPTMENU_DELETE,
#else
            IMAGE_NULL,
#endif
#endif
            MMS_CONTEDIT_OPT_NODE_DELETE,//node_id
            0, //parent_node_id
            menu_index++,//index
            TRUE
            );
#endif
    }
    else
    {
#if defined(MMI_MMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN)
        InsertNodeByLabel(
#else
        InsertNodeByLabelEx(
#endif
            ctrl_id,//ctrl_id
            TXT_DELETE,//text_ptr 
#if !defined(MMI_MMS_MINI_SUPPORT) && !defined(MMI_GUI_STYLE_MINISCREEN)
#ifdef MMI_PDA_SUPPORT            
            IMAGE_COMMON_OPTMENU_DELETE,
#else
            IMAGE_NULL,
#endif
#endif
            MMS_CONTEDIT_OPT_NODE_DELETE,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
    

    //删除文本
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_TEXT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELTEXT,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
        );
    }    

    //删除当前帧
    if (slide_num > 1)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_DEL_CUR_SLIDE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELSLIDE,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //删除当前帧图片
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_IMAGE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELIMAGE,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //删除当前帧音频
    if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_AUDIO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELMUSIC,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#ifdef VIDEO_PLAYER_SUPPORT   
    //删除当前帧视频
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_VIDEO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELVIDEO,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#endif
    //帧页面定时
#if defined(MMI_MMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN) || defined(ORANGE_SUPPORT)
        InsertNodeByLabel(
#else
        InsertNodeByLabelEx(
#endif
        ctrl_id,//ctrl_id
        TXT_COMM_SAVE_AS_DRAFT,//text_ptr 
#if !defined(MMI_MMS_MINI_SUPPORT) && !defined(MMI_GUI_STYLE_MINISCREEN) && !defined(ORANGE_SUPPORT)
#ifdef MMI_PDA_SUPPORT    
        IMAGE_MESSAGE_OPTMENU_SAVE_DRAFT,
#else
        IMAGE_NULL,
#endif
#endif
        MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );

    //保存彩信
#if defined(MMI_MMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN) || defined(ORANGE_SUPPORT)
        InsertNodeByLabel(
#else
        InsertNodeByLabelEx(
#endif
        ctrl_id,//ctrl_id
        TXT_MMS_SET_SLIDE_TIME_OPT,//text_ptr 
#if !defined(MMI_MMS_MINI_SUPPORT) && !defined(MMI_GUI_STYLE_MINISCREEN) && !defined(ORANGE_SUPPORT)
#ifdef MMI_PDA_SUPPORT            
        IMAGE_MESSAGE_OPTMENU_SMS_TIMER,
#else
        IMAGE_NULL,
#endif
#endif
        MMS_CONTEDIT_OPT_NODE_SETTIME,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );

    child_index = 0;
    //文字排版
#if defined(MMI_MMS_MINI_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN) || defined(ORANGE_SUPPORT)
        InsertNodeByLabel(
#else
        InsertNodeByLabelEx(
#endif
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS,//text_ptr 
#if !defined(MMI_MMS_MINI_SUPPORT) && !defined(MMI_GUI_STYLE_MINISCREEN) && !defined(ORANGE_SUPPORT)
#ifdef MMI_PDA_SUPPORT        
        IMAGE_MESSAGE_OPTMENU_SMS_TYPESET,
#else
        IMAGE_NULL,
#endif        
#endif
        MMS_CONTEDIT_OPT_NODE_TEXT_POS,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );


    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS_UP,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST,//node_id
        MMS_CONTEDIT_OPT_NODE_TEXT_POS, //parent_node_id
        child_index++,//index
        FALSE
        );

    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS_DOWN,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST,//node_id
        MMS_CONTEDIT_OPT_NODE_TEXT_POS, //parent_node_id
        child_index++,//index
        FALSE
        );
    pop_info.is_static = FALSE;    
    pop_info.ctrl_id = ctrl_id;
    text_ordor = MMIMMS_GetTextImageOrder(edit_doc_ptr);
    if (MMS_ORDER_IMAGE_FIRST == text_ordor)
    {
        pop_info.node_id = MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST;
    }
    else
    {
        pop_info.node_id = MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST;
    }
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    GUIMENU_SetSubMenuStyle(ctrl_id, MMS_CONTEDIT_OPT_NODE_TEXT_POS,  GUIMENU_STYLE_POPUP_RADIO);
    
    return;    
}
#endif
#else
/*****************************************************************************/
//  Description :open mms edit option menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSEditOptPopMenu(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint16 menu_index                     = 0;
    uint16 child_index                    = 0;
    MMI_CTRL_ID_T	ctrl_id               = MMIMMS_EDIT_MAINMENU_OPT_CTRL_ID;
//  BOOLEAN is_have_video                 = FALSE;
    uint16 slide_num                      = 0;
    uint16 content_num                    = 0;
    uint32 additional_file_num            = 0;//还差接口调用
    MMS_TEXT_IMAGE_ORDER_E text_ordor     = MMS_ORDER_TEXT_FIRST;
    GUIMENU_POP_SEL_INFO_T pop_info       = {0};
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
    {
        return;
    }
    content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
    MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);
    MMK_CreateWin((uint32 *)MMIMMS_EDIT_MAINMENU_OPT_WIN_TAB,PNULL);
    GUIMENU_CreatDynamic(PNULL, MMIMMS_EDIT_MAINMENU_OPT_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);

    //发送彩信
    #ifndef MMI_PDA_SUPPORT
    if (edit_doc_ptr->edit_to.wstr_ptr != PNULL && edit_doc_ptr->edit_to.wstr_len != 0
        && (0 != content_num || 0 != additional_file_num))
    {
        InsertNodeByLabel(
        ctrl_id,                    //ctrl_id
        TXT_SEND,               //text_ptr 
        MMS_CONTEDIT_OPT_NODE_SEND, //node_id
        0,                          //parent_node_id
        menu_index++,               //index
        FALSE
        );
    } 
    else
    {
	    //for CR126474 need gray "send"
        InsertNodeByLabel(
        ctrl_id,                    //ctrl_id
        TXT_SEND,               //text_ptr 
        MMS_CONTEDIT_OPT_NODE_SEND, //node_id
        0,                          //parent_node_id
        menu_index++,               //index
        TRUE
        );
    }
#endif


    #ifndef MMI_PDA_SUPPORT
    //添加收件人
    InsertNodeByLabel(
        ctrl_id,                         //ctrl_id
        TXT_CMS_MMS_ADD_TO,              //text_ptr 
        MMS_CONTEDIT_OPT_NODE_ADD_RECV,  //node_id
        0,                               //parent_node_id
        menu_index++,                    //index
        FALSE
        );
   #endif 
    
#ifndef MMI_PDA_SUPPORT
    //预览彩信
    if (0 != content_num || 0 != additional_file_num)
    {    
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            STXT_PREVIEW,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_PREV,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
#endif

#ifndef MMI_PDA_SUPPORT
    //插入
    slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
    if ((slide_num >= MMIMMS_MAX_SLIDE )
        && additional_file_num >= MMIMMS_MAX_ADDFILE_NUM
        && (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 1)
        &&((0 != edit_doc_ptr->editbody_ptr->slide_cur->video_flag) || ((0 != edit_doc_ptr->editbody_ptr->slide_cur->image_flag)&&(0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize))))
    {
        //SCI_TRACE_LOW:"can not insert again"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_12434_112_2_18_2_38_4_103,(uint8*)"");
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            STXT_ADD,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSERT,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
        );
    }
#endif

    //插入文本
    if (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 0)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_TEXT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSTEXT,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
        );
    }
    
    //插入新页
    if (slide_num < MMIMMS_MAX_SLIDE)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_INSERT_SLIDE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSSLIDE,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
          InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_INSERT_SLIDER,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSSLIDER,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //插入图片
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_IMAGE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSIMAGE,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //插入音频文件
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_AUDIO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSMUSIC,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#ifdef VIDEO_PLAYER_SUPPORT
    //插入视频文件
    if (   0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag )
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_VIDEO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSVIDEO,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#endif
    //插入附件
    if (additional_file_num < MMIMMS_MAX_ADDFILE_NUM)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_ATTACHMENT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSADDFILE,//node_id
            MMS_CONTEDIT_OPT_NODE_INSERT, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
 
    child_index = 0;
    //删除
    if (slide_num == 1 && (content_num - additional_file_num) == 0)
    {
#if 0    
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_DELETE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELETE,//node_id
            0, //parent_node_id
            menu_index++,//index
            TRUE
            );
#endif
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_DELETE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELETE,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
    

    //删除文本
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_TEXT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELTEXT,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
        );
    }    

    //删除当前帧
    if (slide_num > 1)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_DEL_CUR_SLIDE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELSLIDE,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //删除当前帧图片
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_IMAGE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELIMAGE,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }

    //删除当前帧音频
    if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_AUDIO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELMUSIC,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#ifdef VIDEO_PLAYER_SUPPORT   
    //删除当前帧视频
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_VIDEO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_DELVIDEO,//node_id
            MMS_CONTEDIT_OPT_NODE_DELETE, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
#endif
    //帧页面定时
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_SET_SLIDE_TIME_OPT,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_SETTIME,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );

    //保存彩信
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_COMM_SAVE_AS_DRAFT,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );

    child_index = 0;
    //文字排版
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_TEXT_POS,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
        );

    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS_UP,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST,//node_id
        MMS_CONTEDIT_OPT_NODE_TEXT_POS, //parent_node_id
        child_index++,//index
        FALSE
        );

    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MMS_TEXT_POS_DOWN,//text_ptr 
        MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST,//node_id
        MMS_CONTEDIT_OPT_NODE_TEXT_POS, //parent_node_id
        child_index++,//index
        FALSE
        );
    pop_info.is_static = FALSE;    
    pop_info.ctrl_id = ctrl_id;
    text_ordor = MMIMMS_GetTextImageOrder(edit_doc_ptr);
    if (MMS_ORDER_IMAGE_FIRST == text_ordor)
    {
        pop_info.node_id = MMS_CONTEDIT_OPT_NODE_TEXT_POS_IMAGE_FIRST;
    }
    else
    {
        pop_info.node_id = MMS_CONTEDIT_OPT_NODE_TEXT_POS_TEXT_FIRST;
    }
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    GUIMENU_SetSubMenuStyle(ctrl_id, MMS_CONTEDIT_OPT_NODE_TEXT_POS,  GUIMENU_STYLE_POPUP_RADIO);
    
    return;    
}

#endif

#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :open mms edit add menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSEditAddPopMenu(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint16 menu_index                     = 0;
    MMI_CTRL_ID_T	ctrl_id               = MMIMMS_EDIT_ADDMENU_CTRL_ID;
    uint16 slide_num                      = 0;
#ifdef MMI_MMS_MINI_SUPPORT
    uint32 additional_file_num = 0;
#endif

    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
    {
        return;
    }

#ifdef MMI_MMS_MINI_SUPPORT
    MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);
#endif
    
    slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
    MMK_CreateWin((uint32 *)MMIMMS_EDIT_ADDMENU_WIN_TAB,PNULL);
    GUIMENU_CreatDynamic(PNULL, MMIMMS_EDIT_ADDMENU_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);

    //插入文本
    if (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 0)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_TEXT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSTEXT,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
    
    //插入新页
    if (slide_num < MMIMMS_MAX_SLIDE)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_INSERT_SLIDE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSSLIDE,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_INSERT_SLIDER,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSSLIDER,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
    
    //插入图片
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_IMAGE,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSIMAGE,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
    
    //插入音频文件
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_AUDIO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSMUSIC,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
#ifdef VIDEO_PLAYER_SUPPORT  
    //插入视频文件
    if (   0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_VIDEO,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSVIDEO,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
#endif

#ifdef MMI_MMS_MINI_SUPPORT
    //插入附件
    if (additional_file_num < MMIMMS_MAX_ADDFILE_NUM)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_ATTACHMENT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSADDFILE,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
#endif

#ifdef CMCC_UI_STYLE
    //插入Camera
    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            STXT_MAIN_CAMERA,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INDC,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }

    if (0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
        && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS_ATTACHMENT,//text_ptr 
            MMS_CONTEDIT_OPT_NODE_INSADDFILE,//node_id
            0, //parent_node_id
            menu_index++,//index
            FALSE
            );
    }
#endif
}
#endif

/*****************************************************************************/
//  Description :insert dynamenu node by label
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,          //位置
                             BOOLEAN            is_graged
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_graged;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}
#ifndef MMI_GUI_STYLE_TYPICAL
LOCAL void InsertNodeByLabelEx(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             MMI_IMAGE_ID_T          menu_icon_id,
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,          //位置
                             BOOLEAN            is_graged
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_graged;
    node_item.select_icon_id = menu_icon_id;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :set mms contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetMMSContactInfo(MMIMMS_EDIT_ADD_CONTACT_TYPE add_contact_type)
{
    MMI_STRING_T  to_edit_string                 = {0};
    uint16 to_number= 0;
    MMIMMS_EDIT_NUMBER_LIST_T *add_numberlist_ptr = PNULL;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr         = PNULL;
    GUIEDIT_LIST_ITEM_T *item_array_ptr= PNULL;
    MMI_CTRL_ID_T  ctrl_id = 0;
    uint16   index = 0;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint16 temp_len = 0;
    wchar  *all_number_wstr                      = PNULL;
    uint16   all_number_len                      = 0;
    uint16 tele_len = 0;
    uint16    num_index = 0;
    uint8 is_have_empty                          = 0;
    wchar num_temp[MMIMMS_MAX_ADDRESS_LEN + 1]   = {0};
    wchar empty_wstr[]                           = {' '};
    MMIMMS_EDIT_NUMBER_LIST_T  *cursor_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *free_numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    uint8  tele_num[MMIMMS_MAX_ADDRESS_LEN + 2]  = {0};
    wchar  tele_wstr[MMIMMS_MAX_ADDRESS_LEN + 1] = {0};
    wchar semicolon[]                     = {';', 0};
    MMI_STRING_T number_name_addr                = {0};
    wchar number_name_wstr[MMIPB_NAME_MAX_LEN + 1] = {0};
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    
    if( MMIMMS_ADD_TO_CONTACT == add_contact_type)
    {
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT               
        {
            MMI_HANDLE_T edit_handle = 0;
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                            MMIMMS_EDIT_MENU_WIN_ID,//win_id
                                            MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                            );
            ctrl_id = edit_handle;
        }
#else
		ctrl_id = MMIMMS_SMS_EDIT_TO_LIST_ID;
#endif        
        if (PNULL != edit_doc_ptr->edit_to.wstr_ptr && PNULL != edit_doc_ptr->numberlist_header_ptr)
        {
            to_edit_string = edit_doc_ptr->edit_to;
        }
    }
#ifdef MMIMMS_SMS_CC_SUPPORT    
    else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
    {
        ctrl_id = MMIMMS_SMS_EDIT_CC_TO_LIST_ID;    
        if (PNULL != edit_doc_ptr->edit_cc.wstr_ptr && PNULL != edit_doc_ptr->numberlist_cc_header_ptr)
        {
            to_edit_string = edit_doc_ptr->edit_cc;
        }
    }
    else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
    {
        ctrl_id = MMIMMS_SMS_EDIT_BCC_TO_LIST_ID;
        if (PNULL != edit_doc_ptr->edit_bcc.wstr_ptr && PNULL != edit_doc_ptr->numberlist_bcc_header_ptr)
        {
            to_edit_string = edit_doc_ptr->edit_bcc;
        }
    }
#endif
    SCI_MEMSET(((void *)&to_edit_string), 0, sizeof(MMI_STRING_T));
    add_numberlist_ptr = PNULL;
    
    to_number = GUIEDIT_GetListItemNum(ctrl_id);
    
    if (to_number > MMIMMS_MAX_RECEIVER_NUM)
    {
        MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_ADD_TO_FULL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
        return;
    }
    if (0 < to_number)
    { 
        num_str_ptr = SCI_ALLOCAZ(to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
        if (PNULL == num_str_ptr)
        {
            return;
        }
        item_array_ptr = SCI_ALLOCAZ(to_number*sizeof(GUIEDIT_LIST_ITEM_T));
        if(PNULL == item_array_ptr)
        {
            SCI_FREE(num_str_ptr);
            num_str_ptr = PNULL;
            return;
        }
        GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
        GUIEDIT_GetListItemArray(ctrl_id,to_number,item_array_ptr);               
        
        all_number_len = 0;
        for (index = 0; index < to_number; index++)
        {
            all_number_len = all_number_len + num_str_ptr[index].str_len;
        }
        if (0 == all_number_len)
        {
            if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
            {
                MMIMMS_FREE(edit_doc_ptr->edit_to.wstr_ptr);
                edit_doc_ptr->edit_to.wstr_ptr = NULL;
                edit_doc_ptr->edit_to.wstr_len = 0;
            }
            SCI_FREE(num_str_ptr);
            num_str_ptr = PNULL;
            SCI_FREE(item_array_ptr);
            item_array_ptr = PNULL;
            return;
        }
        
        temp_len = (all_number_len + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
        //temp_len = (MMIMMS_ALL_ADDRESS_LEN + MMIMMS_MAX_RECEIVER_NUM + 1)*sizeof(wchar);
        all_number_wstr = (wchar *)SCI_ALLOCAZ(temp_len);            
        if (PNULL == all_number_wstr)
        {
            //SCI_TRACE_LOW:"malloc email_addr failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_13038_112_2_18_2_38_5_104,(uint8*)"");
            SCI_FREE(num_str_ptr);
            num_str_ptr = PNULL;
            SCI_FREE(item_array_ptr);
            item_array_ptr = PNULL;
            MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_MEMORY, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            return;
        }
        SCI_MEMSET(all_number_wstr, 0, temp_len);
        all_number_len = 0;
        
        for (index = 0; index < to_number; index++)
        {
            tele_len = MIN(MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
            MMI_WSTRNCPY(tele_wstr, MMIMMS_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
            
            for (num_index = 0; num_index <= tele_len; num_index++)
            {
                MMI_WSTRNCPY(num_temp, MMIMMS_MAX_ADDRESS_LEN, tele_wstr, MMIMMS_MAX_ADDRESS_LEN, tele_len);
                num_index = num_index - is_have_empty;
                is_have_empty = 0;
                if (0 == MMIAPICOM_Wstrncmp(&(tele_wstr[num_index]), empty_wstr, 1))
                {
                    tele_wstr[tele_len - 1] = 0;
                    tele_len--;
                    MMI_WSTRNCPY(&(tele_wstr[num_index]), MMIMMS_MAX_ADDRESS_LEN, 
                        &(num_temp[num_index + 1]), MMIMMS_MAX_ADDRESS_LEN, tele_len - num_index);
                    is_have_empty = 1;
                }
                SCI_MEMSET((char *)num_temp, 0, sizeof(num_temp));
            }
            if (tele_len > 0)
            {
                MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIMMS_ALL_ADDRESS_LEN, 
                    tele_wstr, MMIMMS_MAX_ADDRESS_LEN, tele_len);                
                all_number_len = tele_len + all_number_len;
                if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
                {
                    MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                    all_number_len++;
                }
                cursor_numberlist_ptr = (MMIMMS_EDIT_NUMBER_LIST_T *)SCI_ALLOC_APP(sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                SCI_MEMSET((char*)cursor_numberlist_ptr, 0, sizeof(MMIMMS_EDIT_NUMBER_LIST_T));
                cursor_numberlist_ptr->is_object = item_array_ptr[index].is_object;
                if(cursor_numberlist_ptr->is_object)
                {
                    cursor_numberlist_ptr->number_name_len = item_array_ptr[index].display_str_len;
                    cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                    MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                        item_array_ptr[index].display_str_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);
                    
                    cursor_numberlist_ptr->number_len = item_array_ptr[index].user_str_len;
                    cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                    SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                    MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                        item_array_ptr[index].user_str_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                }
                else
                {//手动输入需要匹配
                    MMIPB_BCD_NUMBER_T black_bcd_num = {0};
                    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
                    BOOLEAN find_result = FALSE;                                
                    
                    temp_len = item_array_ptr[index].display_str_len;
                    MMI_WSTRNTOSTR(tele_num, MMIMMS_MAX_ADDRESS_LEN, item_array_ptr[index].display_str_ptr, temp_len, temp_len);
                    MMIAPICOM_GenPartyNumber(tele_num, temp_len, &party_num);
                    black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                    black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                    SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
                    number_name_addr.wstr_ptr = number_name_wstr;
                    find_result = MMIAPIPB_GetNameByNumber(&black_bcd_num, &number_name_addr, MMIPB_NAME_MAX_LEN, FALSE);
                    if(find_result && 0 < number_name_addr.wstr_len)
                    {//如果匹配名称，则显示名称，不能够修改了
                        cursor_numberlist_ptr->is_object = TRUE;
                        
                        cursor_numberlist_ptr->number_name_len = number_name_addr.wstr_len;
                        cursor_numberlist_ptr->number_name_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_name_ptr, 0, (cursor_numberlist_ptr->number_name_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_name_ptr, cursor_numberlist_ptr->number_name_len,
                            number_name_addr.wstr_ptr, cursor_numberlist_ptr->number_name_len, cursor_numberlist_ptr->number_name_len);
                        
                        cursor_numberlist_ptr->number_len = item_array_ptr[index].display_str_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            item_array_ptr[index].display_str_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                    }
                    else
                    {
                        cursor_numberlist_ptr->number_len = item_array_ptr[index].display_str_len;
                        cursor_numberlist_ptr->number_ptr = SCI_ALLOC_APP((cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        SCI_MEMSET((char *)cursor_numberlist_ptr->number_ptr, 0, (cursor_numberlist_ptr->number_len + 1) * sizeof(wchar));
                        MMI_WSTRNCPY(cursor_numberlist_ptr->number_ptr, cursor_numberlist_ptr->number_len,
                            item_array_ptr[index].display_str_ptr, cursor_numberlist_ptr->number_len, cursor_numberlist_ptr->number_len);
                    } 
                }                        
                if(PNULL == add_numberlist_ptr)
                {
                    add_numberlist_ptr = cursor_numberlist_ptr;
                }
                else
                {
                    temp_numberlist_ptr = add_numberlist_ptr;
                    while(PNULL != temp_numberlist_ptr->next)
                    {
                        temp_numberlist_ptr = temp_numberlist_ptr->next;
                    }
                    temp_numberlist_ptr->next = cursor_numberlist_ptr;
                }
            }                   
            
            SCI_MEMSET((char *)num_temp, 0, sizeof(num_temp));
            SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
        }
        
        //把得到的号码加入edit document中去
        //SCI_TRACE_LOW:"HandleMMSEditMainMenuWin MMIMMS_SetMMSTo"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_13155_112_2_18_2_38_6_105,(uint8*)"");
        to_edit_string.wstr_ptr = all_number_wstr;
        to_edit_string.wstr_len = MMIAPICOM_Wstrlen(all_number_wstr);
        //SetMMSEditIsSaveDraft(FALSE); 

        {//保存的话不用释放空间，以后还要使用
            if(MMIMMS_ADD_TO_CONTACT == add_contact_type)
            {
                MMIMMS_SetMMSTo(&to_edit_string,edit_doc_ptr);
            }
#ifdef MMIMMS_SMS_CC_SUPPORT
            else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
            {
                MMIMMS_SetMMSCC(&to_edit_string,edit_doc_ptr);            
            }
            else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
            {
                MMIMMS_SetMMSBCC(&to_edit_string,edit_doc_ptr);
            }
#endif
            //MMIMMS_SetMMSToNameList(add_numberlist_ptr, edit_doc_ptr);
            MMIMMS_SetMMSToNameListEx(add_numberlist_ptr, edit_doc_ptr,add_contact_type);
        }                    
        
        SCI_FREE(num_str_ptr);
        num_str_ptr = PNULL;
        
        SCI_FREE(item_array_ptr);
        item_array_ptr = PNULL;
        
        SCI_FREE(all_number_wstr);
        all_number_wstr = PNULL;                    
    }
    else
    {
        if( MMIMMS_ADD_TO_CONTACT == add_contact_type)
        {
            if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
            {
                MMIMMS_FREE(edit_doc_ptr->edit_to.wstr_ptr);
                edit_doc_ptr->edit_to.wstr_ptr = NULL;
                edit_doc_ptr->edit_to.wstr_len = 0;
            }
            add_numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
        }
#ifdef MMIMMS_SMS_CC_SUPPORT    
        else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
        {
            if (PNULL != edit_doc_ptr->edit_cc.wstr_ptr)
            {
                MMIMMS_FREE(edit_doc_ptr->edit_cc.wstr_ptr);
                edit_doc_ptr->edit_cc.wstr_ptr = NULL;
                edit_doc_ptr->edit_cc.wstr_len = 0;
            }
            add_numberlist_ptr = edit_doc_ptr->numberlist_cc_header_ptr;
        }
        else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
        {
            if (PNULL != edit_doc_ptr->edit_bcc.wstr_ptr)
            {
                MMIMMS_FREE(edit_doc_ptr->edit_bcc.wstr_ptr);
                edit_doc_ptr->edit_bcc.wstr_ptr = NULL;
                edit_doc_ptr->edit_bcc.wstr_len = 0;
            }
            add_numberlist_ptr = edit_doc_ptr->numberlist_bcc_header_ptr;
        }
#endif
        
        while(PNULL != add_numberlist_ptr)
        {
            if(PNULL != add_numberlist_ptr->number_name_ptr)
            {
                SCI_FREE(add_numberlist_ptr->number_name_ptr);
                add_numberlist_ptr->number_name_ptr = PNULL;
            }
            if(PNULL != add_numberlist_ptr->number_ptr)
            {
                SCI_FREE(add_numberlist_ptr->number_ptr);
                add_numberlist_ptr->number_ptr = PNULL;
            }
            free_numberlist_ptr = add_numberlist_ptr;
            add_numberlist_ptr = add_numberlist_ptr->next;
            SCI_FREE(free_numberlist_ptr);
            free_numberlist_ptr = PNULL;                    
        }
        if( MMIMMS_ADD_TO_CONTACT == add_contact_type)
        {
            edit_doc_ptr->numberlist_header_ptr = PNULL;
        }
#ifdef MMIMMS_SMS_CC_SUPPORT    
        else if(MMIMMS_ADD_CC_CONTACT == add_contact_type)
        {
            edit_doc_ptr->numberlist_cc_header_ptr = PNULL;
        }
        else if(MMIMMS_ADD_BCC_CONTACT == add_contact_type)
        {
            edit_doc_ptr->numberlist_bcc_header_ptr = PNULL;
        }
#endif 
    }
}
#endif
/*****************************************************************************/
//  Description :edit and send new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void EditAndSendNewMMS(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MN_DUAL_SYS_E send_dual_sim           = MN_DUAL_SYS_1;
    MMIMMS_ERROR_E   send_result          = MMIMMS_NO_ERROR;
    uint16 content_num                    = 0;
    uint32 record_id                      = 0;    
    uint16 sim_sys                        = MN_DUAL_SYS_1;
    uint32 sim_num                        = 0;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    uint16 contact_num                        = 0;
#endif
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMSEditAssembleInfo();
#endif
    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    {
        record_id = edit_doc_ptr->record_id;
        #ifndef MMIMMS_SMS_IN_1_SUPPORT
        if (0 == edit_doc_ptr->edit_to.wstr_len || PNULL == edit_doc_ptr->edit_to.wstr_ptr)
        {
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
			MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_RECEIVER_NONE, TXT_NULL, IMAGE_PUBWIN_WARNING); 			
            return;
        }
        #else
        if ((0 == edit_doc_ptr->edit_to.wstr_len || PNULL == edit_doc_ptr->edit_to.wstr_ptr)
            && (0 == edit_doc_ptr->edit_cc.wstr_len || PNULL == edit_doc_ptr->edit_cc.wstr_ptr)
            && (0 == edit_doc_ptr->edit_bcc.wstr_len || PNULL == edit_doc_ptr->edit_bcc.wstr_ptr))
        {
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_MAX);
			MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_RECEIVER_NONE, TXT_NULL, IMAGE_PUBWIN_WARNING); 		
            return;
        }

        contact_num = MMIMMS_GetAllContactCount();
        if(contact_num >  MMIMMS_MAX_RECEIVER_NUM) //最多三十个联系人,to do
        {
            MMSMMI_OpenMMSAlertWin(TXT_SMS_EXCEED_MAX_NUM, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return;
        }
        #endif
        content_num = MMIMMS_GetAllContentCount(edit_doc_ptr);
        if (0 == content_num)
        {
            MMSMMI_OpenMMSAlertWin(TXT_NO_CONTENT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return;
        }

        if (!MMIMMS_IsMMSNumberValid(edit_doc_ptr->edit_to))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_TEXT_ADDRESS_UNRESOLVED, TXT_NULL, IMAGE_PUBWIN_WARNING);
            return;
        }

        if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
        {
            MMSMMI_OpenMMSAlertWin(TXT_SENDING, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return;
        }
        MMIMMS_SetMMSSendStatus(SEND_NEW_MMS);
        sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
            return;
       }
        else if (1 == sim_num)
        {
            send_dual_sim = (MN_DUAL_SYS_E)sim_sys;
            //MMSMMI_CreateSendingWaitingWin(send_dual_sim, record_id);
        }
        else 
        {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            if(MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS)
                && MMIAPISMS_GetDestDualSys() < MMI_DUAL_SYS_MAX
                && MMIAPIPHONE_IsSimAvailable(MMIAPISMS_GetDestDualSys()))
            {
                send_dual_sim = MMIAPISMS_GetDestDualSys();
            }
#else        
            if (
                    MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MMS)
                    && MMIMMS_GetAnswerSim() < MMI_DUAL_SYS_MAX
                    && MMIAPIPHONE_IsSimAvailable(MMIMMS_GetAnswerSim()))
            {                
                send_dual_sim = MMIMMS_GetAnswerSim();
                //MMSMMI_CreateSendingWaitingWin(send_dual_sim, record_id);
            }
#endif            
            else
            {                
                MMIMMS_CreateSimCardSelectWin(record_id);
                return;
            }
        }       

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(send_dual_sim))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return;
        }
#endif

        send_result= MMIMMS_SendNewMMS(send_dual_sim, edit_doc_ptr);
        MMSMMI_SetSendNewMMSId(edit_doc_ptr->record_id);
        //SCI_TRACE_LOW:"MMIMMS: MMIMMS_SendNewMMS send_result = %d, record_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_13355_112_2_18_2_38_6_106,(uint8*)"dd", send_result, edit_doc_ptr->record_id);
        if (MMIMMS_NO_ERROR != send_result && MMIMMS_ERROR_NO_SPACE != send_result)
        {
            MMIMMS_EditMMSAlertWin(send_result);
            if (MMK_IsOpenWin(MMIMMS_SENDING_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_SENDING_WIN_ID);
            }
            return;
        }

        if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
        {
            MMIMMS_CloseEditWin();
#ifdef MMI_SMS_CHAT_SUPPORT            
            MMIAPISMS_ClearChatEditString();
#endif
            #ifndef MMIMMS_SMS_IN_1_SUPPORT
            if (MMK_IsOpenWin(MMISMS_NEW_MSG_MENU_WIN_ID))
            {
                MMK_CloseWin(MMISMS_NEW_MSG_MENU_WIN_ID);
            }
            #endif
        }
        else
        {
            //SCI_TRACE_LOW:"EditAndSendNewMMS : exit error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_13377_112_2_18_2_38_6_107,(uint8*)"");
        }
    }
    return;
}

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertPic(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace)
{
    uint32 pic_max_size                      = 0;
    uint32 file_size                         = 0;
    uint8 multi_temp[MMS_FILE_NAME_LEN + 1]  = {0};
    MMIFMM_SELECT_INFO_T    select_info = {
        MMIFMM_PIC_ALL,0,
        0,0,PNULL,0,-1};
#ifdef MMIMMS_SMS_IN_1_SUPPORT    
    if(MMIMMS_GetIsMMSMode()) 
#endif
    {
        if (PNULL == edit_doc_ptr)
        {
            return;
        }
        if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
        {
            SetMMSEditInsertOpType(MMIMMS_OP_MAX);
            MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_HALF_POP);
            return;
        }
        if (is_replace && 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
        {
            file_size = MMIMMS_GetCurrentSlideImageInfo(multi_temp, MMS_FILE_NAME_LEN, edit_doc_ptr);
            SetMMSEditIsReplaceMultifile(TRUE);
            SetMMSEditReplaceMultifileSize(file_size);
            pic_max_size = MMIMMS_MMS_LIMIT_SIZE - (uint32)edit_doc_ptr->editbody_ptr->totalsize + file_size;
        }
        else 
        {
            pic_max_size = MMIMMS_MMS_LIMIT_SIZE - (uint32)edit_doc_ptr->editbody_ptr->totalsize;
        }
    }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    else
    {
        pic_max_size = MMIMMS_MMS_LIMIT_SIZE;
    }
#endif
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
    select_info.max_size = pic_max_size;
    select_info.win_id = pre_win_id;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if(MMIAPIFMM_OpenSelectPictureWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        SetMMSEditInsertOpType(MMIMMS_INSERT_PIC);
    }
    else
    {
        SetMMSEditInsertOpType(MMIMMS_OP_MAX);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMSMMI_OpenMMSAlertWin(TXT_NO_SD_CARD_ALERT, TXT_NULL, IMAGE_PUBWIN_WARNING);
#endif       
    }
    return;
}

/*****************************************************************************/
//  Description :mms edit insert music
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertMusic(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace)
{
    int32 pic_max_size     = 0;
    uint32 aodio_file_size = 0;
    MMIFMM_SELECT_INFO_T        select_info = {MMIFMM_MUSIC_ALL,0,0,0,PNULL,0,-1};
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if(MMIMMS_GetIsMMSMode()) 
#endif
    {
        if (PNULL == edit_doc_ptr)
        {
            return;
        }
        if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
        {
            SetMMSEditInsertOpType(MMIMMS_OP_MAX);
            MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_HALF_POP);
            return;
        }
        if (is_replace && 0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)//如果替换音乐
        {
            aodio_file_size = edit_doc_ptr->editbody_ptr->slide_cur->audiosize;
            SetMMSEditIsReplaceMultifile(TRUE);
            SetMMSEditReplaceMultifileSize(aodio_file_size);
            pic_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize + (int32)aodio_file_size;
        }
        else
        {
            pic_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize;
        }
    }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    else
    {
        pic_max_size = MMIMMS_MMS_LIMIT_SIZE;
    }
#endif
    select_info.max_size = pic_max_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if(MMIAPIFMM_OpenSelectMusicWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        SetMMSEditInsertOpType(MMIMMS_INSERT_MUSIC);
    }
    else
    {
        SetMMSEditInsertOpType(MMIMMS_OP_MAX);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMSMMI_OpenMMSAlertWin(TXT_NO_SD_CARD_ALERT, TXT_NULL, IMAGE_PUBWIN_WARNING);
#endif       
    } 
    return;
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :mms edit insert normal attachment
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertNormalAttachment(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace)
{
    int32 file_max_size               = 0;
    uint32 attachment_file_size       = 0;
    MMIFMM_SELECT_INFO_T  select_info = {MMIFMM_FILE_ALL,0,0,0,PNULL,0,-1};
    uint8 *add_file_ptr               = PNULL;
    uint16 add_file_len               = 0;
   // GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/
    if(MMIMMS_GetIsMMSMode()) 
    {
        if (PNULL == edit_doc_ptr)
        {
            return;
        }
        if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
        {
            SetMMSEditInsertOpType(MMIMMS_OP_MAX);
            MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_HALF_POP);
            return;
        }
    }

    if (is_replace)//如果替换附件文件
    {
        if (GetMMSEditIsReplaceMultifile())//
        {  
            MMI_CTRL_ID_T ctrl_id = MMIMMS_ATTACH_LISTBOX_CTRL_ID;
            uint16 index = 0;
            const GUILIST_ITEM_T *item_ptr = PNULL;
            wchar *file_name = PNULL;
            uint16 file_name_len = 0;
            index = GUILIST_GetCurItemIndex(ctrl_id);
            item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
            if(PNULL == item_ptr || PNULL == item_ptr->item_data_ptr)
            {
                return;
            }
            file_name_len =  item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
            file_name = item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
            add_file_ptr = SCI_ALLOC_APP((file_name_len + 1)*3);
            if (PNULL == add_file_ptr)
            {
                return;
            }
            SCI_MEMSET(add_file_ptr, 0, ((file_name_len + 1)*3));            
            add_file_len = GUI_WstrToUTF8((uint8 *)add_file_ptr, MMIMMS_FILE_NAME_LEN, file_name, file_name_len);
            
            if (!MMIMMS_GetAttachmentSize((char *)add_file_ptr, edit_doc_ptr->editbody_ptr, &attachment_file_size))
            {
                SCI_Free(add_file_ptr);
                add_file_ptr = PNULL;
                return;
            }
            
            SCI_Free(add_file_ptr);
            add_file_ptr = PNULL;
        }
        
        SetMMSEditIsReplaceMultifile(TRUE);
        SetMMSEditReplaceMultifileSize(attachment_file_size);
        file_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize + (int32)attachment_file_size;
    }
    else
    {
        if(MMIMMS_GetIsMMSMode()) 
        {
            file_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize;
        }
        else
        {
            file_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE;
        }
    }
    select_info.max_size = file_max_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
    if(MMIAPIFMM_OpenSelectAllFileWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        SetMMSEditIsInsertNormalAttachment(TRUE);
        SetMMSEditInsertOpType(MMIMMS_INSERT_ADD_FILE);
    }
    else
    {
        SetMMSEditInsertOpType(MMIMMS_OP_MAX);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMSMMI_OpenMMSAlertWin(TXT_NO_SD_CARD_ALERT, TXT_NULL, IMAGE_PUBWIN_WARNING);
#endif        
    } 
    return;
}
#else
/*****************************************************************************/
//  Description :mms edit insert normal attachment
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertNormalAttachment(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace)
{
    int32 file_max_size               = 0;
    uint32 attachment_file_size       = 0;
    MMIFMM_SELECT_INFO_T  select_info = {MMIFMM_FILE_ALL,0,0,0,PNULL,0,-1};
    uint16 richedit_index             = 0;
    uint8 *add_file_ptr               = PNULL;
    uint16 add_file_len               = 0;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/

    if (PNULL == edit_doc_ptr)
    {
        return;
    }
    if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
    {
        SetMMSEditInsertOpType(MMIMMS_OP_MAX);
        MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_HALF_POP);
        return;
    }

    if (is_replace)//如果替换附件文件
    {
        GUIRICHTEXT_GetFocusItemIndex(MMIMMS_EDIT_MENU_CTRL_ID, &richedit_index);
        if (MMIMMS_RICHTEXT_ADDFILE == g_mms_edit_richtext_item_type[richedit_index] && GetMMSEditIsReplaceMultifile())//
        {        
            GUIRICHTEXT_GetFocusElement(MMIMMS_EDIT_MENU_CTRL_ID, &richtext_element);                    
            add_file_ptr = SCI_ALLOC_APP((richtext_element.data.len + 1)*3);
            if (PNULL == add_file_ptr)
            {
                return;
            }
            SCI_MEMSET(add_file_ptr, 0, ((richtext_element.data.len + 1)*3));
            add_file_len = GUI_WstrToUTF8((uint8 *)add_file_ptr, MMIMMS_FILE_NAME_LEN, richtext_element.data.str_ptr, richtext_element.data.len);
            
            if (!MMIMMS_GetAttachmentSize((char *)add_file_ptr, edit_doc_ptr->editbody_ptr, &attachment_file_size))
            {
                SCI_Free(add_file_ptr);
                add_file_ptr = PNULL;
                return;
            }
            
            SCI_Free(add_file_ptr);
            add_file_ptr = PNULL;
        }
        
        SetMMSEditIsReplaceMultifile(TRUE);
        SetMMSEditReplaceMultifileSize(attachment_file_size);
        file_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize + (int32)attachment_file_size;
    }
    else
    {
        file_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize;
    }
    select_info.max_size = file_max_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
    if(MMIAPIFMM_OpenSelectAllFileWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        SetMMSEditIsInsertNormalAttachment(TRUE);
        SetMMSEditInsertOpType(MMIMMS_INSERT_ADD_FILE);
    }
    else
    {
        SetMMSEditInsertOpType(MMIMMS_OP_MAX);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMSMMI_OpenMMSAlertWin(TXT_NO_SD_CARD_ALERT, TXT_NULL, IMAGE_PUBWIN_WARNING);
#endif        
    } 
    return;
}
#endif


/*****************************************************************************/
//  Description :mms edit insert video
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertVideo(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace)
{
    int32 pic_max_size                       = 0;
    uint32 file_size                         = 0;
    uint8 multi_temp[MMS_FILE_NAME_LEN + 1]  = {0};
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MOVIE_ALL,0,0,0,PNULL,0,-1};
	#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if(MMIMMS_GetIsMMSMode()) 
	#endif
    {
        if (PNULL == edit_doc_ptr)
        {
            return;
        }
        if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
        {
            SetMMSEditInsertOpType(MMIMMS_OP_MAX);
            MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_HALF_POP);
            return;
        }
        if (is_replace && 1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
        {
            file_size = MMIMMS_GetCurrentSlideVideoInfo(multi_temp, MMS_FILE_NAME_LEN, edit_doc_ptr);
            SetMMSEditIsReplaceMultifile(TRUE);
            SetMMSEditReplaceMultifileSize(file_size);
            pic_max_size =(int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize - MMIMMS_SIZE_LEFT + (int32)file_size; 
        }
        else
        {
            pic_max_size = (int32)MMIMMS_MMS_LIMIT_SIZE - edit_doc_ptr->editbody_ptr->totalsize; 
        }
    }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
	else
	{
	    pic_max_size = MMIMMS_MMS_LIMIT_SIZE;
	}
#endif
    select_info.max_size = pic_max_size;
    select_info.win_id = pre_win_id;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if (MMIAPIFMM_OpenSelectMovieWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        SetMMSEditInsertOpType(MMIMMS_INSERT_MUSIC);
    }
    else
    {
        SetMMSEditInsertOpType(MMIMMS_OP_MAX);
#ifdef NANDBOOT_SUPPORT
        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);
#else
        MMSMMI_OpenMMSAlertWin(TXT_NO_SD_CARD_ALERT, TXT_NULL, IMAGE_PUBWIN_WARNING);
#endif        
    }
    return;
}


/*****************************************************************************/
//  Description :draw the mms edit page bar
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_DrawPageBarText(MMIMMS_WINDOW_ID_E win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_playing_mms)
{
    uint32  total_slide                                    = 0;
    uint32  cur_slide                                      = 0;
    uint32   total_size                                    = 0;
    uint8    display_str_slide[MMIMMS_SLIDE_TIME_STR_LEN]  = {0};
    uint8    display_str_size[MMIMMS_SLIDE_TIME_STR_LEN]   = {0};
    uint8   display_title_str[MMIMMS_SLIDE_TIME_STR_LEN]   = {0};
    wchar    display_title_wstr[MMIMMS_SLIDE_TIME_STR_LEN] = {0};
    uint16   wstr_len                                      = 0;
    GUI_RECT_T  dis_rect                                   = {0};
    GUI_POINT_T   dis_point                                = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info                       = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STYLE_T      text_style                         = {0};/*lint !e64*/
    GUISTR_STATE_T      state                              = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T        string                             = {0};
    BOOLEAN is_have_accessory                              = TRUE;
    
    uint16 page_bar_bin_len                                = 0;
    uint16 page_bar_height                                 = 0;
    uint16 page_bar_slide_len                              = 0;
    uint16 page_bar_size_len                               = 0;

    uint16 page_bar_top                                    = 0;
    uint16 page_bar_bottom                                 = 0;

    uint16 page_bar_slide_left                             = 0;
    uint16 page_bar_slide_top                              = 0;
    uint16 page_bar_slide_right                            = 0;
    uint16 page_bar_slide_bottom                           = 0;

    uint16 page_bar_accessory_left                         = 0;
    uint16 page_bar_accessory_top                          = 0;

    uint16 mainscreen_height                               = 0;
    uint16 mainscreen_width                                = 0;
    
    uint16 page_bar_height_max                             = MMIMMS_PAGE_BAR_HEIGHT_MAX;
    uint16 page_bar_width_max                              = MMIMMS_PAGE_BAR_LEN_MAX;

    if (PNULL == edit_doc_ptr)
    {
        return;
    }
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return ;
    }
    is_have_accessory = MMIMMS_IsWithAttachment(edit_doc_ptr);
    if (MMITHEME_IsMainScreenLandscape())
    {
        page_bar_bin_len = MMIMMS_PAGE_BAR_BIN_LEN*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.15w
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }
    else
    {
        page_bar_bin_len = MMIMMS_PAGE_BAR_BIN_LEN*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.15w
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }

    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#endif
    }
    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
    {  
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#endif
    }
    page_bar_height_max = page_bar_height_max + 1;
    page_bar_height = MIN(page_bar_height_max, page_bar_height);
    page_bar_slide_len = MMIMMS_PAGE_BAR_SLIDE_LEN*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.34w
    page_bar_size_len = MMIMMS_PAGE_BAR_SIZE_LEN*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.25w


#ifdef MMI_PDA_SUPPORT
    page_bar_top = mainscreen_height  - MMIMMS_BUTTON_FORM_HEIGHT - page_bar_height - 1;
#else
    page_bar_top = mainscreen_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - page_bar_height - 1;
#endif
    page_bar_bottom = page_bar_top + page_bar_height;

    page_bar_slide_left = (mainscreen_width - page_bar_slide_len - page_bar_size_len)/2;
    if (is_have_accessory)
    {
        page_bar_slide_left = page_bar_slide_left - 4;
    }
    page_bar_slide_top = page_bar_top;
    page_bar_slide_right = page_bar_slide_left + page_bar_slide_len + page_bar_size_len;
    page_bar_slide_bottom = page_bar_bottom;

#if defined (MAINLCD_SIZE_176X220)
    page_bar_accessory_left = page_bar_slide_right - MMIMMS_PAGE_BAR_ATTACH_MARGIN_LEFT*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;
#else
    page_bar_accessory_left = page_bar_slide_right - MMIMMS_PAGE_BAR_ATTACH_MARGIN_LEFT*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED + 4;
#endif
    //page_bar_accessory_top = page_bar_top + MMIMMS_PAGE_BAR_ATTACH_MARGIN_HEIGHT*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;
    page_bar_accessory_top = page_bar_top + 2;
    //calculate string
    cur_slide = MMIMMS_GetSlideIndex(edit_doc_ptr);
    total_slide = MMIMMS_GetSlideCount(edit_doc_ptr);    

    SCI_MEMSET((void*)display_str_slide, 0, sizeof(display_str_slide));
    //N/M, N==current slide, M==total slide number
    _snprintf((char *)display_str_slide,MMIMMS_SLIDE_TIME_STR_LEN, "%ld/%ld", cur_slide,  total_slide);   
    
    total_size = edit_doc_ptr->editbody_ptr->totalsize;
    if (0 != total_size)
    {
        //SCI_TRACE_LOW:"FNH total_size = %ld"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_13900_112_2_18_2_38_7_108,(uint8*)"d",total_size);
        _snprintf((void *)display_str_size,MMIMMS_SLIDE_TIME_STR_LEN ,"%.1fK",(float)total_size/ONE_K_LEN); 
        if (((float)total_size/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)total_size/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
        {
            _snprintf((void*)display_str_size, MMIMMS_SLIDE_TIME_STR_LEN,":%.1fk\r",0.1);
        }
    }   
    
    //重画背景
    dis_rect.left   = page_bar_slide_left;
    dis_rect.top    = page_bar_slide_top;
    dis_rect.right  = page_bar_slide_right;
    dis_rect.bottom = page_bar_slide_bottom;
    GUIRES_DisplayImg(PNULL,
        &dis_rect,
        &dis_rect,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info);

    if (is_have_accessory)
    {
        dis_point.x = page_bar_accessory_left;
        dis_point.y = page_bar_accessory_top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_MMS_ACCESSORY_FILE,
            &lcd_dev_info);
    }    
    
#ifdef MMI_PDA_SUPPORT
	LCD_DrawLine(&lcd_dev_info, 0, dis_rect.top, mainscreen_width, dis_rect.top, MMI_BLACK_COLOR);
	LCD_DrawLine(&lcd_dev_info, 0, dis_rect.bottom-1, mainscreen_width, dis_rect.bottom-1, MMI_BLACK_COLOR);
#endif
    SCI_MEMSET((void *)display_title_str,0,sizeof(display_title_str));
    _snprintf((void *)display_title_str,MMIMMS_SLIDE_TIME_STR_LEN, "%s %s",display_str_slide, display_str_size);
    wstr_len = strlen((const char *)display_title_str);
    MMI_STRNTOWSTR( display_title_wstr, MMIMMS_SLIDE_TIME_STR_LEN, (uint8 *)display_title_str, MMIMMS_SLIDE_TIME_STR_LEN, wstr_len );
    
    // display
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = MMI_DEFAULT_TEXT_FONT;
    text_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_TEXT_FONT);
    
    string.wstr_len = wstr_len;
    string.wstr_ptr = display_title_wstr;
    
    dis_rect = GUI_CreateRect(page_bar_slide_left, page_bar_slide_top, page_bar_slide_right, page_bar_slide_bottom);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&dis_rect,       //the fixed display area
        (const GUI_RECT_T      *)&dis_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );  

    return;
}

/*****************************************************************************/
//  Description : set mms send status  
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSSendStatus(MMIMMS_SEND_MMS_STATUS send_status)
{
    g_mms_edit_globle.mms_send_status = send_status;
}

/*****************************************************************************/
//  Description : get mms send status  
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SEND_MMS_STATUS MMIMMS_GetMMSSendStatus(void)
{
    return g_mms_edit_globle.mms_send_status;
}

/*****************************************************************************/
//  Description : set mms enter reason  [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_REASON_TYPE_E edit_type)
{
    g_mms_edit_globle.mms_edit_enter_type = edit_type;
}

/*****************************************************************************/
//  Description : get mms enter reason   [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_REASON_TYPE_E MMIMMS_GetMMSEditEnterReason(void)
{
    return g_mms_edit_globle.mms_edit_enter_type;
}

/*****************************************************************************/
//  Description : change edit document attachment text code type
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:编辑草稿箱彩信，收件箱和已发信箱转发，发件箱编辑发送需要调用本
//       函数来改变text类型的附件的编码类型，把uniconde编码变成utf8编码
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AttachmentTextEncodeType(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr)
{
    MMS_ACCESSORY_PARAM_T * accessory_ptr = PNULL;
    uint8 * file_name_ptr  = PNULL;
    uint8 * text_buf_ptr = PNULL;
    uint8 * text_utf8_ptr = PNULL;
    uint32 text_len = 0;
    uint32 text_unicode_len = 0;
    uint32 text_utf8_len = 0;

    accessory_ptr = editdoc_ptr->editbody_ptr->accessory_ptr;
    while (PNULL != accessory_ptr)
    {
        if (!strcmp(accessory_ptr->content_type_string, "text/plain"))
        {
            //change code
            file_name_ptr = (uint8 *)accessory_ptr->filename;
            
            sysVFGetConstFileData((int8*)file_name_ptr,(int8**)&text_buf_ptr,(int32*)&text_len);
            text_unicode_len = *((uint32 *)text_buf_ptr)/2;

            text_utf8_len = (text_unicode_len + 1) * 2;
            text_utf8_ptr = SCI_ALLOC_APP(text_utf8_len);
            if (PNULL == text_utf8_ptr)
            {
                return FALSE;
            }
            SCI_MEMSET(text_utf8_ptr, 0, text_utf8_len);
            if (text_unicode_len < text_len)
            {
                text_len = GUI_WstrToGB((uint8*)text_utf8_ptr, (uint16*)(text_buf_ptr+4), text_unicode_len);
                sysVFremove((int8 *)file_name_ptr);
                sysVFCreateByData((int8*)file_name_ptr,(int8*)text_utf8_ptr,(int32)text_len);
            }
        }
        accessory_ptr = accessory_ptr->next;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : mms edit init 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_MMSEditInit(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr, MMIMMS_RICHTEXT_ITEM_INDEX_E focus_index)
{
//  MMIMMS_EDIT_DOCUMENT_T * preview_doc_ptr = PNULL;

    g_mms_edit_globle.editdoc = editdoc_ptr;
    SetMMSEditLoseFocusIndex(focus_index);
    SetMMSEditFocusOnAddMenu(FALSE);
    SetMMSEditIsOpPopMenu(FALSE);
    SetMMSEditPreFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_NULL);
    SetMMSEditInsertOpType(MMIMMS_OP_MAX);
    MMIMMS_SetMMSSendStatus(SEND_MMS_STATUS_MAX);
    SetMMSEditReplaceMultifileSize(0);
    SetMMSEditIsSaveDraft(TRUE);
    if (   MMIMMS_EDIT_FROM_OUTBOX_READ_TO_EDIT == MMIMMS_GetMMSEditEnterReason()
        || MMIMMS_EDIT_FROM_SENDBOX_READ_TO_FARWARD == MMIMMS_GetMMSEditEnterReason()
        || MMIMMS_EDIT_FROM_INBOX_READ_TO_FARWARD == MMIMMS_GetMMSEditEnterReason())
    {   
        MMIMMS_MMSPreviewInit(PNULL, 0);        
    }
    if (   MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER == MMIMMS_GetMMSEditEnterReason()
        || MMIMMS_EDIT_FROM_INBOX_READ_NUMBER_SEND == MMIMMS_GetMMSEditEnterReason())
    { 
        MMIMMS_MMSPreviewInit(PNULL, 0);
    }
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT 
/*****************************************************************************/
//  Description : mms edit exit
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_MMSEditExit(void)
{
    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_NULL);
    SetMMSEditInsertOpType(MMIMMS_OP_MAX);
    MMIMMS_SetMMSSendStatus(SEND_MMS_STATUS_MAX);
    MMIMMS_SetMMSEditIsEmailAddrExist(FALSE);
    SetMMSEditIsSubjectExist(FALSE);    
#ifdef MMIMMS_SMS_CC_SUPPORT   
    SetMMSEditIsCCExist(FALSE);
    SetMMSEditIsBCCExist(FALSE);
#endif    
  
    MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_MAX);
    if (PNULL != g_mms_edit_globle.editdoc)
    {
        MMIMMS_DestroyEditDocument(g_mms_edit_globle.editdoc);
        g_mms_edit_globle.editdoc = PNULL;
    }
}
#else
/*****************************************************************************/
//  Description : mms edit exit
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_MMSEditExit(void)
{
    SetMMSEditLoseFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
    SetMMSEditFocusOnAddMenu(FALSE);
    SetMMSEditPreFocusIndex(MMIMMS_RICHTEXT_ITEM_NUM_MAX);
    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_NULL);
    SetMMSEditInsertOpType(MMIMMS_OP_MAX);
    MMIMMS_SetMMSSendStatus(SEND_MMS_STATUS_MAX);

    if (PNULL != g_mms_edit_globle.editdoc)
    {
        MMIMMS_DestroyEditDocument(g_mms_edit_globle.editdoc);
        g_mms_edit_globle.editdoc = PNULL;
    }
}
#endif

/*****************************************************************************/
//  Description : set  edit mms music or video name
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetEditMMSMusicOrVideoName(wchar *name_ptr, uint16 name_len)
{
    uint16 index         = 0;
    uint16 name_begin    = 0;
    uint16 tmep_len      = 0;
    wchar  patition[]    = {'\\', 0};

    if (PNULL == name_ptr || 0 == name_len)
    {
        return;
    }
    while (index < (name_len + 1))
    {
        if (name_ptr[index] == patition[0])
        {
            name_begin = index;
        } 
        index++;
    }
    if (name_begin > 0)
    {
        name_begin++;
    }
    name_len = name_len - name_begin;
    tmep_len = (name_len + 1) * sizeof(wchar);
}

/*****************************************************************************/
//  Description :Get flag if add menu get focus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditFocusOnAddMenu( void)
{
    return g_mms_edit_globle.is_add_menu_get_focus;
}

/*****************************************************************************/
//  Description :Set flag if add menu get focus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditFocusOnAddMenu( BOOLEAN add_menu_get_focus)
{
    g_mms_edit_globle.is_add_menu_get_focus = add_menu_get_focus;
}

/*****************************************************************************/
//  Description :Get flag if op multifile  popmenu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsOpPopMenu( void)
{
    return g_mms_edit_globle.is_multi_op_pop;
}

/*****************************************************************************/
//  Description :Set flag if if op multifile  popmenu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsOpPopMenu( BOOLEAN op_pop)
{
    g_mms_edit_globle.is_multi_op_pop = op_pop;
}

/*****************************************************************************/
//  Description :Get mms edit losefucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMMSEditLoseFocusIndex( void)
{
    return g_mms_edit_globle.losefocus_index;
}

/*****************************************************************************/
//  Description :Set mms edit losefucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditLoseFocusIndex( uint16 lose_focus_index)
{
    g_mms_edit_globle.losefocus_index = lose_focus_index;
}

/*****************************************************************************/
//  Description :Get mms edit pre fucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMMSEditPreFocusIndex( void)
{
    return g_mms_edit_globle.pre_focus_index;
}

/*****************************************************************************/
//  Description :Set mms edit pre fucus index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditPreFocusIndex( uint16 pre_focus_index)
{
    g_mms_edit_globle.pre_focus_index = pre_focus_index;
}

/*****************************************************************************/
//  Description :Get mms edit lose fucus reason
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_CONTEDIT_OPT_MENU_NODE_E GetMMSEditLoseFocusReason( void)
{
    return g_mms_edit_globle.edit_lose_focus_reason;
}

/*****************************************************************************/
//  Description :Set mms edit lose fucus reason
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditLoseFocusReason( MMIMMS_CONTEDIT_OPT_MENU_NODE_E lose_fucus_reason)
{
    g_mms_edit_globle.edit_lose_focus_reason = lose_fucus_reason;
}

/*****************************************************************************/
//  Description :Get mms edit insert op type
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_EDIT_INS_OP_TYPE GetMMSEditInsertOpType( void)
{
    return g_mms_edit_globle.mms_edit_insert_op_type;
}

/*****************************************************************************/
//  Description :Set mms edit lose fucus reason
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditInsertOpType( MMIMMS_EDIT_INS_OP_TYPE insert_op_type)
{
    g_mms_edit_globle.mms_edit_insert_op_type = insert_op_type;
}

/*****************************************************************************/
//  Description :Get mms edit if replace multifile
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsReplaceMultifile( void)
{
    return g_mms_edit_globle.is_replace_multifile;
}

/*****************************************************************************/
//  Description :Set mms edit if replace multifile
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsReplaceMultifile( BOOLEAN is_replace )
{
    g_mms_edit_globle.is_replace_multifile = is_replace;
}

/*****************************************************************************/
//  Description :Get mms edit if save draft
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsSaveDraft( void)
{
    return g_mms_edit_globle.is_save_draft;
}

/*****************************************************************************/
//  Description :Set mms edit if save draft
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsSaveDraft( BOOLEAN is_save )
{
    g_mms_edit_globle.is_save_draft = is_save;
}

/*****************************************************************************/
//  Description :Get mms edit if replace multifile size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetMMSEditReplaceMultifileSize( void)
{
    return g_mms_edit_globle.replace_multifile_size;
}

/*****************************************************************************/
//  Description :Set mms edit if replace multifile size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditReplaceMultifileSize( uint32 size )
{
    g_mms_edit_globle.replace_multifile_size = size;
}
#ifndef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :Get item index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetMMSEditItemIndex( MMIMMS_RICHTEXT_ITEM_INDEX_E item_type)
{
    uint16 item_index = MMIMMS_RICHTEXT_ITEM_NUM_MAX;
    
    for (item_index = 0; item_index < MMIMMS_RICHTEXT_ITEM_NUM_MAX; item_index ++)
    {
        if (g_mms_edit_richtext_item_type[item_index] == item_type)
        {
            break;
        }
    }
    return item_index;
}

/*****************************************************************************/
//  Description :Set item index
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditItemIndex( MMIMMS_RICHTEXT_ITEM_INDEX_E item_type, uint16 index)
{
    g_mms_edit_richtext_item_type[index] = item_type;
}
#endif
/*****************************************************************************/
//  Description :Get mms edit if insert normal attachment
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsInsertNormalAttachment( void)
{
    return g_mms_edit_globle.is_insert_normal_attachment;
}

/*****************************************************************************/
//  Description :Set mms edit if insert normal attachment
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsInsertNormalAttachment( BOOLEAN is_insert )
{
    g_mms_edit_globle.is_insert_normal_attachment = is_insert;
}

/*****************************************************************************/
//     Description : insert string to sms edit
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPIMMS_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             )
{
	BOOLEAN result = FALSE;

	if(MMK_IsOpenWin(MMIMMS_EDIT_TEXT_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
	{
		GUIEDIT_InsertString(
			MMIMMS_EDIT_TEXT_CTRL_ID, 
			wstr_ptr,
			wstr_len
			);
		MMK_SendMsg(MMIMMS_EDIT_TEXT_WIN_ID, MSG_FULL_PAINT, PNULL);
		result = TRUE;
	}
	else if(MMK_IsOpenWin(MMIMMS_EDIT_SUBJECT_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
	{
		GUIEDIT_InsertString(
			MMIMMS_EDIT_SUBJECT_CTRL_ID, 
			wstr_ptr,
			wstr_len
			);
		MMK_SendMsg(MMIMMS_EDIT_SUBJECT_WIN_ID, MSG_FULL_PAINT, PNULL);
		result = TRUE; 
	}
	return result;
}

/*****************************************************************************/
//  Description :get default pic width and height
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPicDefaultWidthAndHeight(uint16 *width_ptr, uint16 *height_ptr)
{
    uint16 mainscreen_height          = 0;
    uint16 mainscreen_width           = 0;
    
    if (PNULL == width_ptr || PNULL == height_ptr)
    {
        return FALSE;
    }
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return FALSE;
    }
    *width_ptr = MMIMMS_PIC_DEFAULT_WIDTH*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.98w
    *height_ptr = MMIMMS_PIC_DEFAULT_HEIGHT*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.42h
    
    return TRUE;
}

/*****************************************************************************/
//  Description :get default music width and height
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetMusicDefaultWidthAndHeight(uint16 *width_ptr, uint16 *height_ptr)
{
    uint16 mainscreen_height          = 0;
    uint16 mainscreen_width           = 0;
    
    if (PNULL == width_ptr || PNULL == height_ptr)
    {
        return FALSE;
    }
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return FALSE;
    }
    *width_ptr = MMIMMS_MUSIC_DEFAULT_WIDTH*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.9w
    *height_ptr = MMIMMS_MUSIC_DEFAULT_HEIGHT*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.13h
    
    return TRUE;
}

/*****************************************************************************/
//  Description :get richtext rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetRichtextRect(GUI_RECT_T *rect_ptr)
{
    uint16 mainscreen_height          = 0;
    uint16 mainscreen_width           = 0;
    uint16 page_bar_height            = 0;
    uint16 page_bar_height_max        = MMIMMS_PAGE_BAR_HEIGHT_MAX;
    uint16 page_bar_width_max         = MMIMMS_PAGE_BAR_LEN_MAX;

    if (PNULL == rect_ptr)
    {
        return FALSE;
    }
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return FALSE;
    }
    if (MMITHEME_IsMainScreenLandscape())
    {
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }
    else
    {
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }

    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#endif
    }
    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
    {  
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#endif
    }
  
    page_bar_height_max = page_bar_height_max + 3;
    page_bar_height = MIN(page_bar_height_max, page_bar_height);
#ifndef MMI_PDA_SUPPORT
    rect_ptr->left = 1;
#endif
#ifdef MMI_PDA_SUPPORT
    rect_ptr->top = MMI_TITLE_HEIGHT + MMI_STATUSBAR_HEIGHT;
#else
#ifdef MMI_ONLY_IDLE_DISP_STATUSBAR
    rect_ptr->top = MMI_TITLE_HEIGHT + 1;
#else
    rect_ptr->top = MMI_TITLE_HEIGHT + MMI_STATUSBAR_HEIGHT + 1;
#endif
#endif
    rect_ptr->right = mainscreen_width - 1;
#ifdef MMI_PDA_SUPPORT
    rect_ptr->bottom = mainscreen_height - MMIMMS_BUTTON_FORM_HEIGHT - page_bar_height -1;  // MMIMMS_BUTTON_FORM_HEIGHT BUTTON FORM HEIGHT
#else
    rect_ptr->bottom = mainscreen_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - page_bar_height -1;
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description :get pagebar left bin rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPagebarLbinRect(GUI_RECT_T *rect_ptr)
{
    uint16 page_bar_bin_len                                = 0;
    uint16 page_bar_height                                 = 0;

    uint16 page_bar_left                                   = 0;
    uint16 page_bar_top                                    = 0;
    uint16 page_bar_bottom                                 = 0;
    
    uint16 mainscreen_height                               = 0;
    uint16 mainscreen_width                                = 0;

    uint16 page_bar_height_max                             = MMIMMS_PAGE_BAR_HEIGHT_MAX;
    uint16 page_bar_width_max                              = MMIMMS_PAGE_BAR_LEN_MAX;
    
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return  FALSE;
    }
    if (MMITHEME_IsMainScreenLandscape())
    {
        page_bar_bin_len = MMIMMS_PAGE_BAR_BIN_LEN*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.15w
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }
    else
    {
        page_bar_bin_len = MMIMMS_PAGE_BAR_BIN_LEN*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.15w
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }

    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#endif
    }
    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
    {  
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#endif
    }
    page_bar_height_max = page_bar_height_max + 1;
    page_bar_height = MIN(page_bar_height_max, page_bar_height);
    page_bar_left = 1;
#ifdef MMI_PDA_SUPPORT
    page_bar_top = mainscreen_height - MMIMMS_BUTTON_FORM_HEIGHT - page_bar_height - 1;
#else
    page_bar_top = mainscreen_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - page_bar_height - 1;
#endif
    page_bar_bottom = page_bar_top + page_bar_height;

    rect_ptr->left = page_bar_left;
    rect_ptr->top = page_bar_top;
    rect_ptr->right = page_bar_left + page_bar_bin_len;
    rect_ptr->bottom = page_bar_bottom;

    return TRUE;
}

/*****************************************************************************/
//  Description :get pagebar right bin rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPagebarRbinRect(GUI_RECT_T *rect_ptr)
{
    uint16 page_bar_bin_len                                = 0;
    uint16 page_bar_height                                 = 0;

    uint16 page_bar_top                                    = 0;
    uint16 page_bar_right                                  = 0;
    uint16 page_bar_bottom                                 = 0;
    
    uint16 mainscreen_height                               = 0;
    uint16 mainscreen_width                                = 0;

    uint16 page_bar_height_max                             = MMIMMS_PAGE_BAR_HEIGHT_MAX;
    uint16 page_bar_width_max                              = MMIMMS_PAGE_BAR_LEN_MAX;
    
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return  FALSE;
    }
    if (MMITHEME_IsMainScreenLandscape())
    {
        page_bar_bin_len = MMIMMS_PAGE_BAR_BIN_LEN*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.15w
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }
    else
    {
        page_bar_bin_len = MMIMMS_PAGE_BAR_BIN_LEN*mainscreen_width/MMIMMS_PERCENT_ONE_HUNDRED;//0.15w
        page_bar_height = MMIMMS_PAGE_BAR_HEIGHT*mainscreen_height/MMIMMS_PERCENT_ONE_HUNDRED;//0.10h
    }

    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_EDIT_MENU_WIN_ID);
#endif
    }
    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
    {  
#ifdef MMI_PDA_SUPPORT
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_MMS_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#else
        GUIRES_GetImgWidthHeight(&page_bar_width_max, &page_bar_height_max, IMAGE_COMMON_COUNT_LBTN_UN, MMIMMS_PREVIEW_WIN_ID);
#endif
    }
    page_bar_height_max = page_bar_height_max + 1;
    page_bar_height = MIN(page_bar_height_max, page_bar_height);

#ifdef MMI_PDA_SUPPORT
    page_bar_top = mainscreen_height - MMIMMS_BUTTON_FORM_HEIGHT - page_bar_height - 1;
#else
    page_bar_top = mainscreen_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - page_bar_height - 1;
#endif
    
    page_bar_right = mainscreen_width - 1; 
    page_bar_bottom = page_bar_top + page_bar_height;

    rect_ptr->left = page_bar_right - page_bar_bin_len;
    rect_ptr->top = page_bar_top;
    rect_ptr->right = page_bar_right;
    rect_ptr->bottom = page_bar_bottom;

    return TRUE;
}

/*****************************************************************************/
//  Description :Set answer sim card
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMS_SetAnswerSim(MN_DUAL_SYS_E answer_sim)
{
    if (answer_sim == MN_DUAL_SYS_MAX)
    {
        g_mms_edit_globle.is_answer_from_same_simcard = FALSE;
    }
    else
    {
        g_mms_edit_globle.answer_sim = answer_sim;
        g_mms_edit_globle.is_answer_from_same_simcard = TRUE;
    }    
}

/*****************************************************************************/
//  Description :Get answer sim card
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMIMMS_GetAnswerSim( void)
{
    if (g_mms_edit_globle.is_answer_from_same_simcard && g_mms_edit_globle.answer_sim != MN_DUAL_SYS_MAX)
    {
        return g_mms_edit_globle.answer_sim;
    }
    else
    {
        return MN_DUAL_SYS_MAX;
    }
}

/*****************************************************************************/
//  Description :关闭窗口
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CloseEditWin(void)
{
    MMIAPISMS_SetNormalClose(TRUE);
    MMK_CloseWin(MMIMMS_EDIT_MENU_WIN_ID);
    MMK_CloseWin(MMIMMS_EDIT_TEXT_WIN_ID);
}

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description :if the file is drm file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetDrmFileTypeFromName(uint8 * file_name_ptr)
{
    uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1] = {0};
    uint16 suffix_len                         = MMIMMS_SUFFIX_LEN;
    wchar file_name_wstr[MMIMMS_FILE_NAME_LEN + 1] = {0};
    uint32 drm_file_type = 0;

    MMI_STRNTOWSTR(file_name_wstr, MMIMMS_FILE_NAME_LEN, file_name_ptr, MMIMMS_FILE_NAME_LEN, SCI_STRLEN((char *)file_name_ptr));
    MMIAPIFMM_SplitFileName(file_name_wstr, MMIAPICOM_Wstrlen(file_name_wstr), PNULL, PNULL, suffix_wstr, &suffix_len);
    drm_file_type = MMIAPIFMM_GetFileType(suffix_wstr, suffix_len);
    return drm_file_type;
}

/*****************************************************************************/
//  Description :if the mms can farward
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsMMSForbidFarward(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    P_MMS_SLIDE_T p_mms_slide_temp                   = PNULL;
    MMS_ACCESSORY_PARAM_T *accessory_ptr             = PNULL;
    P_MMS_CONTENT_T p_mms_content_temp               = PNULL;
    uint8 *multifile_name_buf_ptr                    = PNULL;
    BOOLEAN is_forbid_farward                        = FALSE;

    if (PNULL == edit_doc_ptr)
    {
        return FALSE;
    }
    p_mms_slide_temp = edit_doc_ptr->editbody_ptr->slide_root;
    while (p_mms_slide_temp)
    {
        p_mms_content_temp = p_mms_slide_temp->content_root;
        while (p_mms_content_temp)
        {
            multifile_name_buf_ptr = (uint8 *)p_mms_content_temp->filename;
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(multifile_name_buf_ptr))
            {
                is_forbid_farward = TRUE;
                break;
            }                    
            p_mms_content_temp = p_mms_content_temp->next;
        }
        p_mms_slide_temp = p_mms_slide_temp->next;
    }
    accessory_ptr = edit_doc_ptr->editbody_ptr->accessory_ptr;                
    while (PNULL != accessory_ptr)
    {
        multifile_name_buf_ptr = (uint8 *)accessory_ptr->filename;
        if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(multifile_name_buf_ptr))
        {
            is_forbid_farward = TRUE;
            break;
        }
        accessory_ptr = accessory_ptr->next;
    }
    return is_forbid_farward;
}
#endif


#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :edit win insert opt
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSInsertOptWinMsg(
                                                  MMIMMS_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                              )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_CTRL_ID_T ctrl_id                 = MMIMMS_EDIT_INSERT_OPT_POP_CTRL_ID;
    //uint32 node_id                        = 0;
    uint16 slide_num                      = 0;
    uint32          group_id = 0, menu_id = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 additional_file_num            = 0;

    if(MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID)) 
    {
        edit_doc_ptr = g_mms_edit_globle.editdoc;
        if (PNULL == edit_doc_ptr)
        {
            //SCI_TRACE_LOW:"MMS edit doc ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_14765_112_2_18_2_38_9_109,(uint8*)"");
            return result;
        }
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:       
        {
#ifdef MMI_PDA_SUPPORT            
            GUIMENU_BUTTON_INFO_T button_info = {0};
            button_info.is_static = TRUE;
            button_info.group_id = MENU_MMS_MAINMENU_INSERT_POP_MENU;
            button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
            GUIMENU_SetButtonStyle(ctrl_id, &button_info);
            button_info.group_id = MENU_MMS_ATTACH_SUB_MENU;
            GUIMENU_SetButtonStyle(ctrl_id, &button_info);
#endif      
            SetMMSEditIsReplaceMultifile(FALSE); 
            slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
            if (PNULL != edit_doc_ptr)
            {
                MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);
            }
 
            if (slide_num >= MMIMMS_MAX_SLIDE)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_NEXTPAGE,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_PREPAGE,TRUE);
            }
   
            if (additional_file_num >= MMIMMS_MAX_ADDFILE_NUM)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_ATTACH,TRUE);
#ifdef MMI_VCARD_SUPPORT
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_ATTACH_VCARD,TRUE);
#endif
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_ATTACH_OTHER,TRUE);
            }

            {
                edit_doc_ptr = g_mms_edit_globle.editdoc;
                if(edit_doc_ptr)
                {
                    if((MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT - (uint32)edit_doc_ptr->editbody_ptr->totalsize) <= MMIMMS_SIZE_MIN_DV)
                    {
                        MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_TAKE_VIDEO,TRUE);
                    }
                    else
                    {
                        MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_TAKE_VIDEO,FALSE);
                    }
                }
            }
            
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:    
    case MSG_CTL_OK:
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:       
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(MMIMMS_EDIT_INSERT_OPT_POP_CTRL_ID,&group_id,&menu_id);
        if(menu_id != ID_MMS_INSERT_TXT) //短信可以加范本
        {
            MMIMMS_ERROR_E edit_err = MMIMMS_NO_ERROR;
            edit_err = MMIMMS_EnterEditCheck();
            if (MMIMMS_NO_ERROR != edit_err)
            {
                //提示彩信初始化失败
                MMK_CloseWin(win_id);
                MMIMMS_EditMMSAlertWin(edit_err);
                break;
            }   
        }
        switch (menu_id)
        {
        case ID_MMS_INSERT_TAKE_PIC:
            MMK_CloseWin(win_id);
            MMSOpenDC();
            break;

#ifdef MMI_RECORD_SUPPORT            
        case ID_MMS_INSERT_TAKE_AUDIO:
            MMK_CloseWin(win_id);
            MMSOpenRecord();
            break;
#endif

        case ID_MMS_INSERT_TAKE_VIDEO:         
            MMK_CloseWin(win_id);
            MMSOpenDV();
            break;
        
        case ID_MMS_INSERT_PIC:
             {             
                if(MMIMMS_GetIsMMSMode()) //彩信编辑界面
                { 
                    if (PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                        break;
                    }              
                }
                MMK_CloseWin(win_id);
                MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSIMAGE);                   
            }   
        
            break;
        case ID_MMS_INSERT_TXT:  //插入范本
            MMIAPISMS_CreateSelectWordWin(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
            MMK_SetAtvCtrl(win_id, MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
            MMK_CloseWin(win_id);
            break;
        case ID_MMS_INSERT_AUDIO:
            {
                if(MMIMMS_GetIsMMSMode()) 
                {
                    if ((PNULL == edit_doc_ptr ||PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                      
                        break;
                    }
                }
                MMK_CloseWin(win_id);
                MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSMUSIC);     
            }  
            break;
        case ID_MMS_INSERT_VIDEO:
            {
                if(MMIMMS_GetIsMMSMode())
                {                    
                    if (PNULL == edit_doc_ptr ||PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                        break;
                    }
                 }
                 MMK_CloseWin(win_id);
                 MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                 SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSVIDEO);
            } 
            break;
        case ID_MMS_INSERT_NEXTPAGE:
           {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                MMI_STRING_T    edit_str = {0};
                if(!MMIMMS_GetIsMMSMode())
                {
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,0);         
                }             
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {

                    SetMMSEditIsSaveDraft(FALSE);
                    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
                    MMIMMS_AddText(edit_doc_ptr, &edit_str);
                    GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
                    
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);  
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    }
                }
                else
                {
                    MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_MAX_SLIDE);
                }
                MMK_CloseWin(win_id);
            }
            break;
        case ID_MMS_INSERT_PREPAGE:
            {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                MMI_STRING_T    edit_str = {0};
                if(!MMIMMS_GetIsMMSMode())
                {
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,0);         
                } 
                
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {                    
                    SetMMSEditIsSaveDraft(FALSE);
                    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
                    MMIMMS_AddText(edit_doc_ptr, &edit_str);
                    GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,0);
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    }     
                }
                else
                {
                    MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_MAX_SLIDE);
                }
                MMK_CloseWin(win_id);
            }
            break;
            
          case ID_MMS_INSERT_ATTACH_VCARD:
            MMK_CloseWin(win_id);
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSelectVcardCallback
                    );
            //MMIAPIPB_OpenNumberList(MMIPB_LIST_VCARD,MMIPB_LIST_TYPE_SINGLE,1,PNULL);
            SetMMSEditInsertOpType(MMIMMS_INSERT_ADD_FILE);
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSADDFILE);
            break;

          case ID_MMS_INSERT_ATTACH_OTHER:              
              if(MMIMMS_GetIsMMSMode())
              {                    
                  if (PNULL == edit_doc_ptr ||PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                  {
                      MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                      break;
                  }
               }
              MMSEditInsertNormalAttachment(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, GetMMSEditIsReplaceMultifile());
              MMK_CloseWin(win_id);
              break;
  
         default:
             result = MMI_RESULT_FALSE;
                 break;
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#else
/*****************************************************************************/
//  Description :edit win insert opt
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSInsertOptWinMsg(
                                                  MMIMMS_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                              )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_CTRL_ID_T ctrl_id                 = MMIMMS_EDIT_INSERT_OPT_POP_CTRL_ID;
    //uint32 node_id                        = 0;
    uint16 slide_num                      = 0;
#ifdef TOUCH_PANEL_SUPPORT 
    uint16 cur_slide_index                = 0;
    uint32          group_id = 0, menu_id = 0;
#endif    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 additional_file_num            = 0;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == edit_doc_ptr)
    {
        return result;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        {
            GUIMENU_BUTTON_INFO_T button_info = {0};
            button_info.is_static = TRUE;
            button_info.group_id = MENU_MMS_MAINMENU_INSERT_POP_MENU;
            button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
            GUIMENU_SetButtonStyle(ctrl_id, &button_info);
            button_info.group_id = MENU_MMS_ATTACH_SUB_MENU;
            GUIMENU_SetButtonStyle(ctrl_id, &button_info);
            SetMMSEditIsReplaceMultifile(FALSE);  
            slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
            MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);
            if (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 1)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_TXT,TRUE);
            } 
            if (slide_num >= MMIMMS_MAX_SLIDE)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_NEXTPAGE,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_PREPAGE,TRUE);
            }
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag 
                || 1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_PIC,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_TAKE_PIC,TRUE);
            }
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
                || 0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_AUDIO,TRUE);
            }
#ifdef VIDEO_PLAYER_SUPPORT  
            //插入视频文件
            if (   1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
                || 0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize
                || 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_VIDEO,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_TAKE_VIDEO,TRUE);
            }
#endif    
            if (additional_file_num >= MMIMMS_MAX_ADDFILE_NUM)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_ATTACH,TRUE);
#ifdef MMI_VCARD_SUPPORT
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_ATTACH_VCARD,TRUE);
#endif
                MMIAPICOM_EnableGrayed(win_id,MENU_MMS_MAINMENU_INSERT_POP_MENU,ID_MMS_INSERT_ATTACH_OTHER,TRUE);
            }
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CTL_OK:
    case MSG_APP_OK:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
       
        GUIMENU_GetId(MMIMMS_EDIT_INSERT_OPT_POP_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_MMS_INSERT_TAKE_PIC:
            MMK_CloseWin(win_id);
            MMSOpenDC();
            break;

#ifdef MMI_RECORD_SUPPORT            
        case ID_MMS_INSERT_TAKE_AUDIO:
            MMK_CloseWin(win_id);
            MMSOpenRecord();
            break;
#endif

        case ID_MMS_INSERT_TAKE_VIDEO:         
            MMK_CloseWin(win_id);
            MMSOpenDV();
            break;
            
        case ID_MMS_INSERT_PIC:
             {
                if (PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else 
                {
                    MMSEditInsertPic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                }     
                 MMK_CloseWin(win_id);
            }   
        
            break;
        case ID_MMS_INSERT_TXT:
            MMK_CreateWin((uint32*)MMIMMS_EDIT_TEXT_TAB, PNULL);
            MMK_CloseWin(win_id);
            break;
        case ID_MMS_INSERT_AUDIO:
            {
                if (PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    MMSEditInsertMusic(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                }
                 MMK_CloseWin(win_id);
            }  
            break;
        case ID_MMS_INSERT_VIDEO:
            {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                if (PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else if (0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                }
                 MMK_CloseWin(win_id);
            } 
            break;
        case ID_MMS_INSERT_NEXTPAGE:
           {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                
                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (1 == cur_slide_index)
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
                }
                else
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
                }
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    }                    
                }
                 MMK_CloseWin(win_id);
            }
            break;
        case ID_MMS_INSERT_PREPAGE:
            {
                MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
                
                cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                if (1 == cur_slide_index)
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
                }
                else
                {
                    SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
                }
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                SetMMSEditFocusOnAddMenu(FALSE);
                slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
                if (slide_num < MMIMMS_MAX_SLIDE)
                {
                    SetMMSEditIsSaveDraft(FALSE);
                    add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,0);
                    if (MMIMMS_NO_ERROR != add_slide_err)
                    {
                        MMIMMS_EditMMSAlertWin(add_slide_err);
                    }                    
                }
                MMK_CloseWin(win_id);
            }
            break;
         case ID_MMS_INSERT_ATTACH:
             {
                SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
                if (PNULL == edit_doc_ptr->editbody_ptr || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                //插入附件
                //if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
                {
                    //MMSEditInsertVideo(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, FALSE);
                  //MMK_CreateWin((uint32 *)MMIMMS_ATTACHMENT_WIN_TAB,PNULL);
                   // MMK_CloseWin(win_id);
                }
            }  
            break;
            
          case ID_MMS_INSERT_ATTACH_VCARD:
            MMK_CloseWin(win_id);
            //MMIAPIPB_OpenNumberList(MMIPB_LIST_VCARD,MMIPB_LIST_RADIO,1,PNULL);
            MMIAPIPB_OpenMainWin(
                    MMIPB_ALL_CONTACT,//
                    MMIPB_LIST_RADIO, //IN: 号码列表的类型
                    MMIPB_LIST_FIELD_WHOLE,
                    1,//如果是多选时，此项限制了多选的最大个数
                    PNULL,
                    EditSelectVcardCallback
                    );
            SetMMSEditInsertOpType(MMIMMS_INSERT_ADD_FILE);
            break;

          case ID_MMS_INSERT_ATTACH_OTHER:
            MMK_CloseWin(win_id);
            if (PNULL == edit_doc_ptr->editbody_ptr)
            {
                break;
            }
            MMSEditInsertNormalAttachment(MMIMMS_EDIT_MENU_WIN_ID, edit_doc_ptr, GetMMSEditIsReplaceMultifile());
            break;
  
         default:
             result = MMI_RESULT_FALSE;
                break;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif
#endif

/*****************************************************************************/
//     Description : to handle the message of window which choice the phiz     
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSPhizListWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM                param        //IN:
    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMMS_PHIZ_LISTBOX_CTRL_ID;
    uint16 cur_index = 0;
    BOOLEAN is_right = TRUE;
    wchar sms_phiz[20] = {0};
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMI_CTRL_ID_T ctrl_id_win = MMIMMS_SMS_EDIT_TEXT_CTRL_ID; 
#else
    MMI_CTRL_ID_T ctrl_id_win = MMIMMS_EDIT_MENU_CTRL_ID;// default
    MMIMMS_EDIT_DOCUMENT_T* edit_doc_ptr = g_mms_edit_globle.editdoc;
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMSLoadTotalPhiz2List(ctrl_id);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        
        MMI_STRNTOWSTR( sms_phiz,
            20,
            (const uint8 *)s_sms_phiz[cur_index],
            strlen((char *)s_sms_phiz[cur_index]),
            strlen((char *)s_sms_phiz[cur_index]));
        
        // insert the selected phiz to editbox
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        {
        is_right = GUIEDIT_InsertString(
            ctrl_id_win, 
            sms_phiz, 
            (uint16)MMIAPICOM_Wstrlen(sms_phiz));
        }
#else
        is_right = MMIMMS_SetEditTextContent(sms_phiz, (uint16)MMIAPICOM_Wstrlen(sms_phiz));
#endif
        if (!is_right)
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
        }
        MMK_CloseWin(MMISMS_EDITOPTION_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//     Description : load total phiz to list
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSLoadTotalPhiz2List(
    MMI_CTRL_ID_T        ctrl_id        //IN:
    )
{
    uint16 i = 0;
    uint8 j = 0;
    uint8 phiz_len = 0;
    MMI_STRING_T item_str = {0};
    MMI_STRING_T prompt_str = {0};
    
    item_str.wstr_ptr = SCI_ALLOC_APP((GUILIST_STRING_MAX_NUM+1)*sizeof(wchar));
    
    GUILIST_SetMaxItem(ctrl_id, MMIMMS_PHIZ_NUM, FALSE );//max item 10
    
    for (i=0; i<MMIMMS_PHIZ_NUM; i++)
    {
        GUILIST_ITEM_T item_t = {0};/*lint !e64*/
        GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
        SCI_MEMSET(item_str.wstr_ptr, 0, ((GUILIST_STRING_MAX_NUM+1)*sizeof(wchar)));
        
        phiz_len = strlen((char *)s_sms_phiz[i]);
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(TXT_SMS_SMILE+i), &prompt_str);
        
        MMI_STRNTOWSTR(item_str.wstr_ptr, GUILIST_STRING_MAX_NUM, (const uint8 *)s_sms_phiz[i], phiz_len, phiz_len);
        
        for (j=phiz_len; j<MMIMMS_PHIZ_MAX_LEN; j++)
        {
            item_str.wstr_ptr[j] = 0x0020;
        }
        
        MMI_WSTRNCPY(&(item_str.wstr_ptr[MMIMMS_PHIZ_MAX_LEN]),
            MIN(GUILIST_STRING_MAX_NUM -MMIMMS_PHIZ_MAX_LEN , prompt_str.wstr_len),
            prompt_str.wstr_ptr,
            MIN(GUILIST_STRING_MAX_NUM -MMIMMS_PHIZ_MAX_LEN , prompt_str.wstr_len),
            MIN(GUILIST_STRING_MAX_NUM -MMIMMS_PHIZ_MAX_LEN , prompt_str.wstr_len));
        
        item_str.wstr_len = MIN(MMIMMS_PHIZ_MAX_LEN + prompt_str.wstr_len, GUILIST_STRING_MAX_NUM);
        
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        
        item_data.item_content[0].item_data.text_buffer.wstr_len = item_str.wstr_len;
        
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = item_str.wstr_ptr;
        
        GUILIST_AppendItem( ctrl_id, &item_t );              
    }
    
    if (PNULL != item_str.wstr_ptr)
    {
        SCI_FREE(item_str.wstr_ptr);
        item_str.wstr_ptr = PNULL;
    }
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetIsMMSMode( BOOLEAN is_mms_mode)
{
    g_mms_edit_globle.is_mms_mode = is_mms_mode;
}

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetIsMMSMode( void)
{
    return g_mms_edit_globle.is_mms_mode;
}

/*****************************************************************************/
//  Description :设置是否有主题
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSEditIsSubjectExist( BOOLEAN is_have_subject)
{
    g_mms_edit_globle.is_have_subject = is_have_subject;
}

/*****************************************************************************/
//  Description :设置彩信添加联系人类型
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditAddContactType( MMIMMS_EDIT_ADD_CONTACT_TYPE add_contactinfo_type)
{
    g_mms_edit_globle.mms_add_contact_type = add_contactinfo_type;
}

/*****************************************************************************/
//  Description :判断彩信添加联系人类型
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMIMMS_EDIT_ADD_CONTACT_TYPE GetMMSEditAddContactType( void)
{
    return g_mms_edit_globle.mms_add_contact_type;
} 

/*****************************************************************************/
//  Description :设置是否有主题
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsSubjectExist( BOOLEAN is_have_subject)
{
    g_mms_edit_globle.is_have_subject = is_have_subject;
}

/*****************************************************************************/
//  Description :判断是否有主题
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsSubjectExist( void)
{
    return g_mms_edit_globle.is_have_subject;
} 

/*****************************************************************************/
//  Description :设置是否有email地址
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSEditIsEmailAddrExist(BOOLEAN is_have_email_addr)
{
    g_mms_edit_globle.is_have_email_addr = is_have_email_addr;
}
/*****************************************************************************/
//  Description :判断是否有email地址
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetMMSEditIseEmailAddrExist(void)
{
    return g_mms_edit_globle.is_have_email_addr;
}
#ifdef MMIMMS_SMS_CC_SUPPORT    
/*****************************************************************************/
//  Description :设置是否有抄送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsCCExist( BOOLEAN is_have_cc)
{
    g_mms_edit_globle.is_have_carbon_cc = is_have_cc;
}

/*****************************************************************************/
//  Description :判断是否有抄送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsCCExist( void)
{
    return g_mms_edit_globle.is_have_carbon_cc;
} 

/*****************************************************************************/
//  Description :设置是否有密送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSEditIsBCCExist( BOOLEAN is_have_bcc)
{
    g_mms_edit_globle.is_have_carbon_bcc = is_have_bcc;
}

/*****************************************************************************/
//  Description :判断是否有密送
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetMMSEditIsBCCExist( void)
{
    return g_mms_edit_globle.is_have_carbon_bcc;
} 
#endif
/*****************************************************************************/
//  Description :将选中的联系人信息插入到彩信正文中
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSAddContact2Content(uint32 handle)
{
    MMI_STRING_T contact_str = {0};
    BOOLEAN is_right = TRUE;
    
    contact_str.wstr_ptr = SCI_ALLOC_APP((MMIMMS_MAX_TEXT_LEN+1)*sizeof(wchar));
    SCI_MEMSET(contact_str.wstr_ptr, 0, ((MMIMMS_MAX_TEXT_LEN+1)*sizeof(wchar)));
    
    // get the detail information from the phone_book
    MMIAPISMS_GetContactInfo(
        handle,
        &contact_str,
        MMIMMS_MAX_TEXT_LEN
        );
    
    is_right = GUIEDIT_InsertString(
        MMIMMS_SMS_EDIT_TEXT_CTRL_ID, 
        contact_str.wstr_ptr, 
        contact_str.wstr_len
        );
    if (!is_right)
    {
        // prompt the error information
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
    }
    
    if (PNULL != contact_str.wstr_ptr)
    {
        SCI_FREE(contact_str.wstr_ptr);
        contact_str.wstr_ptr = PNULL;
    }
}
/*****************************************************************************/
//     Description : to handle the message of window which choice the phiz     
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSAttachListWinMsg(
                                             MMI_WIN_ID_T        win_id,        //IN:
                                             MMI_MESSAGE_ID_E    msg_id,        //IN:
                                             DPARAM                param        //IN:
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMMS_ATTACH_LISTBOX_CTRL_ID;
    MMIMMS_WINDOW_ID_E query_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMSLoadTotalAttach2List(ctrl_id);
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID: 
                if(MMS_CONTEDIT_OPT_NODE_DELADDFILE == GetMMSEditLoseFocusReason())
                {                    
                    MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);                    
                }
                else
                {
                    SetMMSEditIsReplaceMultifile(TRUE);
                    MMK_CreateWin((uint32 *)MMIMMS_ATTACHMENT_WIN_TAB,PNULL);
                    //单个替换
                }
                //MMK_CloseWin(win_id);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID: 
                MMK_CloseWin( win_id );
                break;
            default:
                recode = MMI_RESULT_FALSE;
                break;
            }
        }
        break;
        case MMI_PB_VCARD_SELECTED:        //给编辑主界面转发此消息
            MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, msg_id,  param);
            break;

        case MSG_PROMPTWIN_OK:
            MMSEditDelAttach();  //多个删除s
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMK_CloseWin(win_id);
            break;
        
        case MSG_PROMPTWIN_CANCEL:        
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;
        
        case MSG_APP_OK:
        case MSG_CTL_OK:
            MMSEditDelAttach();  //多个删除
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin( win_id );
            break;
            
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
                                             
/*****************************************************************************/
//     Description : load total attach to list
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSLoadTotalAttach2List(
    MMI_CTRL_ID_T        ctrl_id        //IN:
    )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint32             additional_file_num = 0;
    MMS_ACCESSORY_PARAM_T *add_file_ptr = PNULL;
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16 len = 0;
    wchar file_name[MMS_FILE_NAME_LEN] = {0};
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    
    MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);            
    GUILIST_SetMaxItem(ctrl_id, additional_file_num, FALSE );
    GUILIST_SetMaxSelectedItem(ctrl_id, additional_file_num );
    add_file_ptr = edit_doc_ptr->editbody_ptr->accessory_ptr;
    if(MMS_CONTEDIT_OPT_NODE_INSADDFILE == GetMMSEditLoseFocusReason()) //替换的情况
    {
        GUILIST_ChangeDisplayType(
            ctrl_id,
            GUILIST_RADIOLIST_E,
            FALSE
            );
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_CHECK;
    }
    
    while(add_file_ptr )
    {           
        len = strlen(add_file_ptr->filename);
        SCI_MEMSET(file_name,0,sizeof(file_name));
        GUI_UTF8ToWstr(file_name,len,(uint8*)add_file_ptr->filename,len);    
        
        item_t.item_data_ptr = &item_data;
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(file_name);
        
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = file_name;
        
        GUILIST_AppendItem( ctrl_id, &item_t );          
        
        add_file_ptr  = add_file_ptr->next;          
    }    
}

#ifdef MMIMMS_SMS_CC_SUPPORT    
/*****************************************************************************/
//  Description :show contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowCCContactInfo(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    wchar *to_wstr                        = PNULL;
    uint16 to_len                         = 0;
    wchar temp_wch[]                      = L" ";
    uint16 to_alloc_len                   = 0;
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    BOOLEAN is_alloc                      = FALSE;
    wchar semicolon[]                     = {';', 0};
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    numberlist_ptr = edit_doc_ptr->numberlist_cc_header_ptr;
    temp_numberlist_ptr = edit_doc_ptr->numberlist_cc_header_ptr;
    if (PNULL != edit_doc_ptr && PNULL != numberlist_ptr)   
    {
        GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_CC_TO_LIST_ID);
        while(PNULL != temp_numberlist_ptr)
        {
            to_alloc_len = to_alloc_len + MAX(temp_numberlist_ptr->number_len, temp_numberlist_ptr->number_name_len);
            temp_numberlist_ptr = temp_numberlist_ptr->next;
        }
        to_alloc_len = to_alloc_len + MMIMMS_MAX_RECEIVER_NUM + 1;//分号
        if(0 < to_alloc_len)
        {
            to_wstr = (wchar *)MMIMMS_ALLOC(to_alloc_len * sizeof(wchar));
            is_alloc = TRUE;
            if(PNULL == to_wstr)
            {
                //SCI_TRACE_LOW:"mms alloc failed "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_15660_112_2_18_2_38_11_110,(uint8*)"");
                return;
            }
            SCI_MEMSET(to_wstr, 0, to_alloc_len * sizeof(wchar));
            //SCI_TRACE_LOW:"MMSEditAddToItem %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_15664_112_2_18_2_38_11_111,(uint8*)"d", to_alloc_len);
            while(PNULL != numberlist_ptr)
            {
                if(numberlist_ptr->is_object && PNULL != numberlist_ptr->number_name_ptr)
                {
                    MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_name_ptr, numberlist_ptr->number_name_len);
                    to_len = to_len + numberlist_ptr->number_name_len;
                }
                else
                {
                    MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_ptr, numberlist_ptr->number_len);
                    to_len = to_len + numberlist_ptr->number_len;
                }
                MMIAPICOM_Wstrncpy(to_wstr + to_len, semicolon, MMIAPICOM_Wstrlen(semicolon));
                to_len = to_len + MMIAPICOM_Wstrlen(semicolon);
                if(numberlist_ptr->is_object)
                {
                    GUIEDIT_AddListItem(MMIMMS_SMS_EDIT_CC_TO_LIST_ID, numberlist_ptr->number_name_ptr, numberlist_ptr->number_name_len);
                }
                else
                {
                    GUIEDIT_AddListItem(MMIMMS_SMS_EDIT_CC_TO_LIST_ID, numberlist_ptr->number_ptr, numberlist_ptr->number_len);
                }
                numberlist_ptr = numberlist_ptr->next;
            }            
        }
    }
    else
    {
        to_wstr = temp_wch;
        to_len = MMIAPICOM_Wstrlen(to_wstr);
    }    
    
    if (is_alloc)
    {
        MMIMMS_FREE(to_wstr);
        to_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :show contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowBCCContactInfo(void)
{
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    wchar *to_wstr                        = PNULL;
    uint16 to_len                         = 0;
    wchar temp_wch[]                      = L" ";
    uint16 to_alloc_len                   = 0;
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *temp_numberlist_ptr = PNULL;
    BOOLEAN is_alloc                      = FALSE;
    wchar semicolon[]                     = {';', 0};
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    numberlist_ptr = edit_doc_ptr->numberlist_bcc_header_ptr;
    temp_numberlist_ptr = edit_doc_ptr->numberlist_bcc_header_ptr;
    if (PNULL != edit_doc_ptr && PNULL != numberlist_ptr)   
    {
        GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID);
        while(PNULL != temp_numberlist_ptr)
        {
            to_alloc_len = to_alloc_len + MAX(temp_numberlist_ptr->number_len, temp_numberlist_ptr->number_name_len);
            temp_numberlist_ptr = temp_numberlist_ptr->next;
        }
        to_alloc_len = to_alloc_len + MMIMMS_MAX_RECEIVER_NUM + 1;//分号
        if(0 < to_alloc_len)
        {
            to_wstr = (wchar *)MMIMMS_ALLOC(to_alloc_len * sizeof(wchar));
            is_alloc = TRUE;
            if(PNULL == to_wstr)
            {
                //SCI_TRACE_LOW:"mms alloc failed "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_15739_112_2_18_2_38_11_112,(uint8*)"");
                return;
            }
            SCI_MEMSET(to_wstr, 0, to_alloc_len * sizeof(wchar));
            //SCI_TRACE_LOW:"MMSEditAddToItem %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_15743_112_2_18_2_38_11_113,(uint8*)"d", to_alloc_len);
            while(PNULL != numberlist_ptr)
            {
                if(numberlist_ptr->is_object && PNULL != numberlist_ptr->number_name_ptr)
                {
                    MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_name_ptr, numberlist_ptr->number_name_len);
                    to_len = to_len + numberlist_ptr->number_name_len;
                }
                else
                {
                    MMIAPICOM_Wstrncpy(to_wstr + to_len, numberlist_ptr->number_ptr, numberlist_ptr->number_len);
                    to_len = to_len + numberlist_ptr->number_len;
                }
                MMIAPICOM_Wstrncpy(to_wstr + to_len, semicolon, MMIAPICOM_Wstrlen(semicolon));
                to_len = to_len + MMIAPICOM_Wstrlen(semicolon);
                if(numberlist_ptr->is_object)
                {
                    GUIEDIT_AddListItem(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID, numberlist_ptr->number_name_ptr, numberlist_ptr->number_name_len);
                }
                else
                {
                    GUIEDIT_AddListItem(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID, numberlist_ptr->number_ptr, numberlist_ptr->number_len);
                }
                numberlist_ptr = numberlist_ptr->next;
            }            
        }
    }
    else
    {
        to_wstr = temp_wch;
        to_len = MMIAPICOM_Wstrlen(to_wstr);
    }
    
    if (is_alloc)
    {
        MMIMMS_FREE(to_wstr);
        to_wstr = PNULL;
    }
}
#endif
/*****************************************************************************/
//  Description :show contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowToContactInfo(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr = PNULL;
    GUIEDIT_LIST_ITEM_T *list_item_ptr = PNULL;
    uint16 list_count = 0;
    MMI_CTRL_ID_T ctrl_id = 0;

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    {
        MMI_HANDLE_T edit_handle = 0;
        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                        MMIMMS_EDIT_MENU_WIN_ID,//win_id,
                                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                        );
        ctrl_id = edit_handle;
    }                
#else
    ctrl_id = MMIMMS_SMS_EDIT_TO_LIST_ID;
#endif
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if(PNULL != edit_doc_ptr )
    {
        numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
    }
    if (PNULL != edit_doc_ptr && PNULL != numberlist_ptr)   
    {
        GUIEDIT_ClearAllStr(ctrl_id);
        while(PNULL != numberlist_ptr)
        {
            list_count++;
            numberlist_ptr = numberlist_ptr->next;
        }
        list_item_ptr = SCI_ALLOCAZ(list_count * sizeof(GUIEDIT_LIST_ITEM_T)); 
        if(PNULL == list_item_ptr )
        {
            return;
        }
        numberlist_ptr = edit_doc_ptr->numberlist_header_ptr;
        list_count = 0;
        while(PNULL != numberlist_ptr)
        {
            list_item_ptr[list_count].is_valid = TRUE;
            if(numberlist_ptr->is_object)
            {
                list_item_ptr[list_count].is_object = TRUE;
                list_item_ptr[list_count].display_str_ptr = numberlist_ptr->number_name_ptr;
                list_item_ptr[list_count].display_str_len = numberlist_ptr->number_name_len;
                list_item_ptr[list_count].user_str_ptr = numberlist_ptr->number_ptr;            
                list_item_ptr[list_count].user_str_len= numberlist_ptr->number_len;            
            }
            else
            {
                list_item_ptr[list_count].is_object = FALSE;
                list_item_ptr[list_count].display_str_ptr = numberlist_ptr->number_ptr;
                list_item_ptr[list_count].display_str_len = numberlist_ptr->number_len;
                list_item_ptr[list_count].user_str_ptr = numberlist_ptr->number_ptr;            
                list_item_ptr[list_count].user_str_len= numberlist_ptr->number_len; 
            }
            list_count++;
            numberlist_ptr = numberlist_ptr->next;
        }
        
        GUIEDIT_AddListItemArray(ctrl_id, list_item_ptr, list_count);
        
        SCI_FREE(list_item_ptr);
        list_item_ptr = PNULL;
    }
}
/*****************************************************************************/
//  Description :show contact info
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowContactInfo(void)
{    
    MMSShowToContactInfo();    
#ifdef     MMIMMS_SMS_CC_SUPPORT
    if(GetMMSEditIsCCExist())
    {
        MMSShowCCContactInfo();     
    }
    
    if(GetMMSEditIsBCCExist())
    {
        MMSShowBCCContactInfo();     
    }
#endif    
}
/*****************************************************************************/
//  Description : to handle the message of copy waiting window    
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMMSSmsConvertWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_CLOSE_WINDOW:
        { 
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
            
            MMI_HANDLE_T edit_handle = 0;
            edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                win_id,
                MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                );
#endif
            if(!MMIMMS_GetIsMMSMode()) //不是彩信模式 ,转化为彩信
            {
                MMI_STRING_T edit_string = {0};
                wchar *edit_ptr = PNULL;

                MMIMMS_SetIsMMSMode(TRUE);
                MMIMMS_SlideInit();
                                         
#ifdef MMI_PDA_SUPPORT                 
                GUISOFTKEY_SetTextId(
                    MMIMMS_EDIT_MENU_WIN_ID,                      
                    MMICOMMON_SOFTKEY_CTRL_ID,
                    STXT_PREVIEW,   // the left softkey id
                    TXT_COMM_INSERT, // the middle softkey id
                    TXT_SEND,  // the right softkey id
                    FALSE       // whether update
                    );  
#endif                    
#ifdef MMI_TIMERMSG_SUPPORT   
                if(MMIAPISMS_GetIsTimerMode())
                {
                    MMIAPISMS_SetIsTimerMode(FALSE);       //关闭timer msg                                          
                }
#endif     
                GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &edit_string);
                edit_ptr = SCI_ALLOCAZ((edit_string.wstr_len + 1)*sizeof(wchar));
                if(PNULL == edit_ptr)
                {
                    //SCI_TRACE_LOW:"MMS alloc error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_15924_112_2_18_2_38_11_114,(uint8*)"");
                    return FALSE;
                }
                SCI_MEMSET(edit_ptr,0x00,(edit_string.wstr_len + 1)*sizeof(wchar));
                MMIAPICOM_Wstrncpy(edit_ptr, edit_string.wstr_ptr, edit_string.wstr_len);

                GUIEDIT_SetSmsEncode(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,FALSE);
                GUIEDIT_SetMaxLen(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,MMIMMS_MAX_TEXT_LEN/sizeof(wchar));

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                //GUIEDIT_SetListStyle(edit_handle, GUIEDIT_LIST_STYLE_ALL);
#else
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
#endif
#ifdef MMIMMS_SMS_CC_SUPPORT    
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_CC_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID, GUIEDIT_LIST_STYLE_ALL);
#endif
                GUIEDIT_SetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, edit_ptr,edit_string.wstr_len);
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, edit_handle);
                
                
#else
                MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif

                if (!MMITHEME_IsIstyle())
                {
                    GUIFORM_SetChildDisplay(MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID, MMIMMS_SMS_LABEL_COUNT_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
                }

                SCI_FREE(edit_ptr);
                edit_ptr = PNULL;
            }
            else//变为短信模式
            {
                MMI_STRING_T edit_string = {0};
                wchar *edit_ptr = PNULL;

                MMIMMS_SetIsMMSMode(FALSE);
#ifdef MMI_PDA_SUPPORT                
                GUISOFTKEY_SetTextId(
                    MMIMMS_EDIT_MENU_WIN_ID,
                    MMICOMMON_SOFTKEY_CTRL_ID,
                    TXT_MMS_SMS_TEMPLATE,   
                    TXT_COMM_INSERT, 
                    TXT_SEND, 
                    FALSE 
                    );  
#endif                
                GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, &edit_string);
                edit_ptr = SCI_ALLOC_APP((edit_string.wstr_len + 1)*sizeof(wchar));
                if(PNULL == edit_ptr)
                {
                    //SCI_TRACE_LOW:"MMS alloc error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_15975_112_2_18_2_38_11_115,(uint8*)"");
                    return FALSE;
                }
                SCI_MEMSET(edit_ptr,0x00,(edit_string.wstr_len + 1)*sizeof(wchar));
                MMIAPICOM_Wstrncpy(edit_ptr, edit_string.wstr_ptr, edit_string.wstr_len);
                GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
                MMIAPISMS_SetEditTextProperty(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
         
                GUIEDIT_SetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID, edit_ptr, MIN(edit_string.wstr_len,MMISMS_MAX_MESSAGE_LEN));
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                //GUIEDIT_SetListStyle(edit_handle, GUIEDIT_LIST_STYLE_PHONENUM);
#else
                GUIEDIT_SetListStyle(MMIMMS_SMS_EDIT_TO_LIST_ID, GUIEDIT_LIST_STYLE_PHONENUM);
#endif
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
                MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, edit_handle);
                
                
#else
                MMK_SetAtvCtrl(MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif

#ifdef MMI_PDA_SUPPORT
                if (!MMITHEME_IsIstyle())
                {
                    MMSShowEditWordCount();
                    GUIFORM_SetChildDisplay(MMIMMS_SMS_SUBFORM_BUTTON_AREA_FORM_ID, MMIMMS_SMS_LABEL_COUNT_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
                }
#endif

                SCI_FREE(edit_ptr);
                edit_ptr= PNULL;                
            }

#ifdef MMI_PDA_SUPPORT             
            HandleSendButtonDisplay(MMIMMS_GetIsMMSMode());
#endif
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        }
        break;
             
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description : to handle the message of copy waiting window    
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_CheckIsMMS(void)
{
    uint16 slide_count = 0;
    BOOLEAN is_mms_mode = FALSE;
    uint32 additional_file_num            = 0;
    MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;
    BOOLEAN is_text_exceed_sms_length = FALSE;

    edit_doc_ptr = g_mms_edit_globle.editdoc;

    if (PNULL == edit_doc_ptr
        || PNULL == edit_doc_ptr->editbody_ptr
        || PNULL == edit_doc_ptr->editbody_ptr->slide_cur)
    {
        return FALSE;
    }

    if (0 != edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
    {
        MMI_STRING_T text_str = {0};
        MMISMS_SMSEDIT_CONTENT_T *edit_content_ptr = PNULL;

        MMIMMS_GetCurrentSlideText(edit_doc_ptr, &text_str);

        edit_content_ptr = SCI_ALLOCAZ(sizeof(MMISMS_SMSEDIT_CONTENT_T));

        if (PNULL == edit_content_ptr)
        {
            return FALSE;
        }

        // to save the information
        MMIAPISMS_SetMessageContent( 
            text_str.wstr_len,
            text_str.wstr_ptr, 
            TRUE, 
            edit_content_ptr
            );

        if (MN_SMS_UCS2_ALPHABET == edit_content_ptr->send_message.alphabet)
        {
            if (edit_content_ptr->send_message.length != (text_str.wstr_len * sizeof(wchar)))
            {
                is_text_exceed_sms_length = TRUE;
            }
        }
        else if (MN_SMS_8_BIT_ALPHBET == edit_content_ptr->send_message.alphabet)
        {
            if (edit_content_ptr->send_message.length != text_str.wstr_len)
            {
                is_text_exceed_sms_length = TRUE;
            }
        }
        else
        {
            wchar *temp_str_ptr = PNULL;
            uint16 temp_str_len = 0;

            temp_str_ptr = SCI_ALLOCAZ((edit_content_ptr->send_message.length + 1)*sizeof(wchar));

            if (PNULL == temp_str_ptr)
            {
                SCI_FREE(edit_content_ptr);

                return FALSE;
            }
        
            MMIAPICOM_Default2Wchar(edit_content_ptr->send_message.data, temp_str_ptr, edit_content_ptr->send_message.length);

            temp_str_len = MMIAPICOM_Wstrlen(temp_str_ptr);

            if (temp_str_len != text_str.wstr_len)
            {
                is_text_exceed_sms_length = TRUE;
            }

            SCI_FREE(temp_str_ptr);
        }

        SCI_TRACE_LOW("MMIMMS_CheckIsMMS is_text_exceed_sms_length=%d, len=%d, alphabet=%d, length=%d",
                                    is_text_exceed_sms_length, text_str.wstr_len, edit_content_ptr->send_message.alphabet, edit_content_ptr->send_message.length);

        SCI_FREE(edit_content_ptr);
    }

    //GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,  &text_str);
    slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);//只有一页
    MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);     
    if(slide_count == 0)
    {
        SetMMSEditIsSubjectExist(FALSE);
    }
    //只有1帧的情况才进行彩短的转换 ，大于1帧必为彩信
    if((slide_count > 1 ) || ((1== slide_count ) && (
        (0 != additional_file_num )
        || MMIMMS_GetMMSEditIseEmailAddrExist()
        || GetMMSEditIsSubjectExist()
#ifdef MMIMMS_SMS_CC_SUPPORT
        || GetMMSEditIsCCExist()
        || GetMMSEditIsBCCExist()
#endif        
        || (edit_doc_ptr->editbody_ptr->slide_cur->audiosize  != 0 )
        || (edit_doc_ptr->editbody_ptr->slide_cur->image_flag != 0 ) 
        || (edit_doc_ptr->editbody_ptr->slide_cur->video_flag != 0 ) 
        || is_text_exceed_sms_length
        ))
        )
    {
        is_mms_mode  = TRUE;              
    }

    return is_mms_mode;    
}
/*****************************************************************************/
//  Description : edit new mms 
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_InitData(void)
{
    BOOLEAN result = FALSE;

    MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr = PNULL;
    
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif      

    editdoc_ptr = MMIMMS_CreateEditDocument();
    if (editdoc_ptr)
    {
        MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
        MMIMMS_SlideInit();
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIMMS_EditNewMMS : edit document alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_16096_112_2_18_2_38_11_116,(uint8*)"");
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
        result = FALSE;
    }    

    return result;
}


#ifdef MMI_TIMERMSG_SUPPORT
/*****************************************************************************/
// 	Description : set Timer SMS display
//	Global resource dependence : 
//  Author: rong.gu
//	Note:   
/*****************************************************************************/
LOCAL void MMSSetTimerMsgDisplay(
								 BOOLEAN	is_update
								 )
{
    GUIFORM_SetStyle(MMIMMS_SMS_TIMER_SMS_FORM_ID,GUIFORM_STYLE_UNIT);
    
    if (!MMIAPISMS_GetIsTimerMode())
    {
		if (is_update)
		{
			GUIFORM_SetChildDisplay(MMIMMS_SMS_EDIT_FORM_ID, 
				MMIMMS_SMS_TIMER_SMS_FORM_ID, 
				GUIFORM_CHILD_DISP_HIDE);
		}
		else
		{
			GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
				MMIMMS_SMS_TIMER_SMS_FORM_ID, 
				GUIFORM_CHILD_DISP_HIDE);
		}
    }
    else
    {
		if (is_update)
		{
			GUIFORM_SetChildDisplay(MMIMMS_SMS_EDIT_FORM_ID, 
				MMIMMS_SMS_TIMER_SMS_FORM_ID, 
				GUIFORM_CHILD_DISP_NORMAL);
		}
		else
		{
			GUIFORM_SetChildDisplayOnly(MMIMMS_SMS_EDIT_FORM_ID, 
				MMIMMS_SMS_TIMER_SMS_FORM_ID, 
				GUIFORM_CHILD_DISP_NORMAL);
		}
    }
}

/*****************************************************************************/
// 	Description : set Timer SMS label param
//	Global resource dependence : 
//  Author:  rong.gu
//	Note:   
/*****************************************************************************/
LOCAL void MMSSetTimerMsgStartParam(void)
{		
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMIAPISMS_SetCurOrderArrIndex(0);
    order_id = MMIAPISMS_GetCurOperationOrderId();
    
    if (PNULL != order_id)
    {         
        MMSAddTimer2Label();
    }
}

/*****************************************************************************/
//     Description : load timer info to editbox
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSAddTimer2Label(void)
{
    MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;
    MMI_CTRL_ID_T ctrl_id = MMIMMS_SMS_EDIT_TIMER_FREQDATE_CTRL_ID;
    
#ifdef MMISMS_TIMERMSG_ONCE_SUPPORT//只做了单次
    char str[32] = {0};
    uint8 len = 0;
    wchar *wstr_ptr = PNULL;
    MMI_STRING_T label_string = {0};
    SCI_DATE_T cur_date = {0};
    uint8 time_str[MMISET_DATE_STR_LEN + 1] = {0};
    uint8 date_str[MMISET_DATE_STR_LEN + 1] = {0};
    BOOLEAN is_display_year = FALSE;
    GUI_FONT_T font = SONG_FONT_18;
    GUI_COLOR_T font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_FONT);
    event_fast_info_ptr = MMIAPISMS_TIMERMSG_GetCurEvent();
    if (PNULL == event_fast_info_ptr)
    {
        return;
    }
    
    TM_GetSysDate(&cur_date);
    
    if (event_fast_info_ptr->year != cur_date.year)
    {
        is_display_year = TRUE;
    }
    
    MMIAPISET_FormatDateStrByDateEx(event_fast_info_ptr->year,
        event_fast_info_ptr->mon,
        event_fast_info_ptr->day,
        '.',
        date_str,
        MMISET_DATE_STR_LEN,
        is_display_year);
    
    MMIAPISET_FormatTimeStrByTime(event_fast_info_ptr->hour,event_fast_info_ptr->minute,time_str,MMISET_DATE_STR_LEN);
    
    len = sprintf(str, "%s %s", time_str, date_str);
    
    label_string.wstr_len = len;
    
    wstr_ptr = SCI_ALLOCA((label_string.wstr_len + 1) * sizeof(wchar));
    
    if (PNULL == wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMS malloc failed "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_16195_112_2_18_2_38_12_117,(uint8*)"");
        return;
    }
    
    label_string.wstr_ptr = wstr_ptr;
    
    SCI_MEMSET(label_string.wstr_ptr, 0x00, ((label_string.wstr_len + 1) * sizeof(wchar)));
    
    MMI_STRNTOWSTR(label_string.wstr_ptr, label_string.wstr_len, str, len, len);
    
    GUITEXT_SetString(ctrl_id, label_string.wstr_ptr, label_string.wstr_len, FALSE);
    
    GUITEXT_SetFont(ctrl_id, &font, &font_color);
    
    SCI_FREE(wstr_ptr);
#else
    char str[32] = {0};
    uint8 len = 0;
    wchar *wstr_ptr = PNULL;
    MMI_STRING_T fre_mode_string = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T label_string = {0};
    SCI_DATE_T cur_date = {0};
    uint8 time_str[MMISET_DATE_STR_LEN + 1] = {0};
    uint8 date_str[MMISET_DATE_STR_LEN + 1] = {0};
    BOOLEAN is_display_year = FALSE;
    GUI_FONT_T font = SONG_FONT_18;
    GUI_COLOR_T font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_EDIT_FONT);
    event_fast_info_ptr = MMIAPISMS_TIMERMSG_GetCurEvent();
    if (PNULL == event_fast_info_ptr)
    {
        return;
    }
    
    switch(event_fast_info_ptr->fre_mode)
    {
    case ALM_MODE_ONCE:
        text_id = TXT_ALARM_MODE_ONCE;
        break;
        
    case ALM_MODE_EVERYDAY:
        text_id = TXT_ALARM_MODE_EVERYDAY;
        break;
        
    case ALM_MODE_EVERYWEEK:
        text_id = TXT_ALARM_MODE_WEEK;
        break;
        
    case ALM_MODE_EVERYMONTH:
        text_id = TXT_ALARM_MODE_MONTH;
        break;
        
    case ALM_MODE_EVERYYEAR:
        text_id = TXT_ALARM_MODE_YEAR;
        break;
        
    default:
        break;
    }
    
    MMI_GetLabelTextByLang(text_id, &fre_mode_string);
    
    TM_GetSysDate(&cur_date);
    
    if (event_fast_info_ptr->year != cur_date.year)
    {
        is_display_year = TRUE;
    }
    
    MMIAPISET_FormatDateStrByDateEx(event_fast_info_ptr->year,
        event_fast_info_ptr->mon,
        event_fast_info_ptr->day,
        '.',
        date_str,
        MMISET_DATE_STR_LEN,
        is_display_year);
    
    MMIAPISET_FormatTimeStrByTime(event_fast_info_ptr->hour,event_fast_info_ptr->minute,time_str,MMISET_DATE_STR_LEN);
    
    len = sprintf(str, ";\n%s %s", time_str, date_str);
    
    label_string.wstr_len = fre_mode_string.wstr_len + len;
    
    wstr_ptr = SCI_ALLOCA((label_string.wstr_len + 1) * sizeof(wchar));
    
    if (PNULL == wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMS malloc failed "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_16282_112_2_18_2_38_12_118,(uint8*)"");
        return;
    }
    
    label_string.wstr_ptr = wstr_ptr;
    
    SCI_MEMSET(label_string.wstr_ptr, 0x00, ((label_string.wstr_len + 1) * sizeof(wchar)));
    
    MMI_WSTRNCPY(label_string.wstr_ptr, label_string.wstr_len, fre_mode_string.wstr_ptr, fre_mode_string.wstr_len, fre_mode_string.wstr_len);
    
    MMI_STRNTOWSTR(label_string.wstr_ptr + fre_mode_string.wstr_len, len, (uint8 *)str, len, len);
    
    GUITEXT_SetString(ctrl_id, label_string.wstr_ptr, label_string.wstr_len, FALSE);
    
    GUITEXT_SetFont(ctrl_id, &font, &font_color);
    
    SCI_FREE(wstr_ptr);
#endif
}
#endif

/*****************************1************************************************/
//     Description : get phone number is valid
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************1************************************************/
LOCAL BOOLEAN MMSCheckDestNum(wchar *wstr_ptr, uint16 wstr_len)
{
   BOOLEAN is_valid_num = FALSE;
   MMI_STRING_T num_string = {0};
   uint16 match_count = 0;
   MMIPB_HANDLE_T pb_num_search_handle = 0;
   MMI_STRING_T name_str = {0};
   wchar name_wchar[MMIPB_MAX_STRING_LEN + 1] = {0};
   wchar num_wchar[MMIPB_MAX_STRING_LEN + 1] = {0};
   MMIPB_BCD_NUMBER_T pb_num = {0};
   uint16 i = 0;
   uint8 dest_addr[MMISMS_PBNUM_MAX_LEN+2] = {0};
   uint32 group = 0;
          
   if(PNULL == wstr_ptr || 0 == wstr_len)
   {
       return FALSE;
   }
   num_string.wstr_ptr = num_wchar;
   MMI_WSTRNCPY(num_string.wstr_ptr, MMIPB_MAX_STRING_LEN + 1, wstr_ptr, wstr_len, wstr_len);

   num_string.wstr_len = wstr_len;
   pb_num_search_handle = MMIAPIPB_SearchList(&num_string, MMIPB_LIST_FIELD_NUMBER|MMIPB_LIST_FIELD_MAIL , &match_count);//lint !e655
   name_str.wstr_ptr = name_wchar;
   if(match_count != 0)
   {
       for(i = 0; i < match_count; i++)
       {
           SCI_MEMSET(name_wchar, 0, sizeof(name_wchar));
           if(MMIAPIPB_GetSearchEntry(pb_num_search_handle, &name_str, MMIPB_MAX_STRING_LEN, &pb_num, &group,i))
           {
               if(wstr_len == name_str.wstr_len && (0 == MMIAPICOM_Wstrncmp(name_str.wstr_ptr,wstr_ptr,wstr_len))) 
               {
                   is_valid_num = TRUE; 
                   MMIAPIPB_SearchExit(pb_num_search_handle);

                   
                   return TRUE;
               }
           }
       }
       
   }
   
   //查找是否是合法的号码
   SCI_MEMSET(dest_addr,0, sizeof(dest_addr));
   MMI_WSTRNTOSTR((uint8 *)dest_addr,MMISMS_PBNUM_MAX_LEN+2,wstr_ptr,wstr_len,wstr_len);       
   if(MMIAPICOM_IsPhoneNumber(dest_addr,wstr_len))
   {
       is_valid_num = TRUE;  
   }

   MMIAPIPB_SearchExit(pb_num_search_handle);
   return is_valid_num;
}

/*****************************1************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************1************************************************/
LOCAL BOOLEAN MMSGetSendNumber(                            //RETURN: 
                               MMI_CTRL_ID_T ctrl_id,     //IN:
                               BOOLEAN  is_delete_invalid
                               )
{
    BOOLEAN result = FALSE;
    uint8 dest_addr[MMISMS_PBNUM_MAX_LEN+2] = {0};
    uint16 num_count = 0;
    uint16 i = 0;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr = PNULL;
    
    MMIAPISMS_ClearDestNum();
    
    //get number count
    num_count = GUIEDIT_GetListItemNum(ctrl_id);
    if (0 < num_count)
    {
        //alloc
        num_str_ptr = SCI_ALLOCAZ(num_count*sizeof(GUIEDIT_LIST_ITEM_STR_T));
        if(PNULL == num_str_ptr)
        {
            //SCI_TRACE_LOW:"MMS malloc failed "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_16385_112_2_18_2_38_12_119,(uint8*)"");
            return FALSE;
        }
        //get all phone number
        GUIEDIT_GetListItemString(ctrl_id,num_count,num_str_ptr);
        
        //set dest
        for (i=0; i<num_count; i++)
        {
            if (MMISMS_PBNUM_MAX_LEN < num_str_ptr[i].str_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
            }
            else if(!is_delete_invalid)
            {
                if (num_str_ptr[i].is_valid)
                {
                    if (MMISMS_DEST_ADDR_MAX_NUM >= num_count)
                    {
                        MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
                        
                        edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
                        SCI_MEMSET(dest_addr,0, sizeof(dest_addr));
                        MMI_WSTRNTOSTR((uint8 *)dest_addr,MMISMS_PBNUM_MAX_LEN+2,
                            num_str_ptr[i].str_ptr,num_str_ptr[i].str_len,
                            num_str_ptr[i].str_len);
                        
                        //MMIAPISMS_SetDestNum((uint8 *)dest_addr,(uint8)strlen((char *)dest_addr));
                        if(!MMSCheckDestNum(num_str_ptr[i].str_ptr,num_str_ptr[i].str_len))
                        {
                            MMIAPISMS_ClearDestNum();
                            SCI_FREE(num_str_ptr);
                            return FALSE;
                        }
                        
                        MMIAPISMS_SetAddressToMessage((uint8 *)dest_addr,(uint8)strlen((char *)dest_addr), (uint32 *)&edit_content_ptr->dest_info.dest_list);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_INVALID_NUMBER);
                }
            }
            else
            {
                if (MMISMS_DEST_ADDR_MAX_NUM >= num_count)
                {
                    SCI_MEMSET(dest_addr,0, sizeof(dest_addr));
                    MMI_WSTRNTOSTR((uint8 *)dest_addr,MMISMS_PBNUM_MAX_LEN+2,
                        num_str_ptr[i].str_ptr,num_str_ptr[i].str_len,
                        num_str_ptr[i].str_len);
                    
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
                }
            }
        }
        SCI_FREE(num_str_ptr);
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void HandleMMSEditSendSms(MMIMMS_WINDOW_ID_E win_id)
{        
    MMI_STRING_T edit_string = {0};
    MMI_CTRL_ID_T ctrl_text_id = MMIMMS_SMS_EDIT_TEXT_CTRL_ID;
    MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
    BOOLEAN is_valid_number = FALSE;
    SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
    GUIEDIT_GetString(ctrl_text_id, &edit_string);
    // to save the information
    edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
    
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if (MMISMS_GetAutoSignatureFlag() && !MMIMMS_GetIsMMSMode())
    {
        MMI_STRING_T sig_string = {0};
        MMI_STRING_T display_string = {0};  
        wchar        signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
        uint16       signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    signature_wstr[0] = 0x0a00;
#else
    signature_wstr[0] = 0x000a;
#endif
        signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr + 1) + 1;
        
        sig_string.wstr_ptr = signature_wstr;
        sig_string.wstr_len = signature_str_len;
        
        display_string.wstr_ptr = (wchar *)SCI_ALLOCAZ( ( MMISMS_MAX_MESSAGE_LEN + 1 ) * sizeof(wchar) );

        if (PNULL == display_string.wstr_ptr)
        {
            return;
        }
        
        MMIAPICOM_MergeTwoString(
            &edit_string,
            &sig_string,
            &display_string,
            MMISMS_MAX_MESSAGE_LEN
            );
        
        // to save the information
        MMIAPISMS_SetMessageContent( 
            display_string.wstr_len,
            display_string.wstr_ptr, 
            TRUE, 
            edit_content_ptr
            );
        
        SCI_FREE(display_string.wstr_ptr);
    }
    else
#endif
#endif
    {
        MMIAPISMS_SetMessageContent(edit_string.wstr_len, edit_string.wstr_ptr, TRUE, edit_content_ptr);
    } 

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    {
        MMI_HANDLE_T edit_handle = 0;
        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                        win_id,
                                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                        );
        is_valid_number = MMSGetSendNumber(edit_handle,FALSE);
    }                
#else
    is_valid_number = MMSGetSendNumber(MMIMMS_SMS_EDIT_TO_LIST_ID,FALSE);
#endif
    if(!is_valid_number) 
    {
         MMIPUB_OpenAlertWarningWin(TXT_SMS_INVALID_NUMBER);
         return;
    }
    // open the input number window
    if (0 < MMIAPISMS_GetDestCount())
    {   
        // send all message
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        uint16 sim_sys = MN_DUAL_SYS_1;
        uint32 sim_num = 0;

        sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_num)
        {
            if (!MMIAPICC_IsSimServiceAvail((MN_DUAL_SYS_E)sim_sys))
            {
                MMIAPICC_OpenSIMCallingPrompt();
                return;
            }

            MMSSendSms((MN_DUAL_SYS_E)sim_sys);
            MMIMMS_CloseEditWin();
        }
        else
        {
            if (MMIAPISMS_GetDestDualSys() == MN_DUAL_SYS_MAX)
            {
                MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim;
                major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
                if(major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                {
                    if (!MMIAPICC_IsSimServiceAvail((MN_DUAL_SYS_E)(MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1))))
                    {
                        MMIAPICC_OpenSIMCallingPrompt();
                        return;
                    }

                    MMSSendSms(MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1));
                    MMIMMS_CloseEditWin();
                }
                else
                {
                    MMIMMS_CreateSimCardSelectWin(0);
                }                            
            }
            else
            {
                if (!MMIAPICC_IsSimServiceAvail(MMIAPISMS_GetDestDualSys()))
                {
                    MMIAPICC_OpenSIMCallingPrompt();
                    return;
                }

                MMSSendSms(MMIAPISMS_GetDestDualSys());
                MMIMMS_CloseEditWin();
            }
        }
#else
        if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else
        {
            MMSSendSms(MN_DUAL_SYS_1);
            MMIMMS_CloseEditWin();
        }
#endif
    } 
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }
}

/*****************************************************************************/
//     Description : to display send err win
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSDisplaySmsSendErr(
                                MN_DUAL_SYS_E         dual_sys,
                                MMISMS_OPER_ERR_E    oper_err    //IN:
                                )
{
    switch (oper_err)
    {
    case MMISMS_NO_ERR:
        //打开消息发送窗口提示正在发送消息
        {
            MMI_STRING_T    prompt_str = {0};
            
            MMI_GetLabelTextByLang(TXT_SENDING, &prompt_str);
            
            MMIAPISMS_OpenSendingWindow(dual_sys, &prompt_str); //后台发送不需要显示
        }
        break;
        
    case MMISMS_NO_SC_ADDRESS:
        //打开提示窗口：短消息中心未设置
        MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_SC);
        
#ifdef MMI_TIMERMSG_SUPPORT
        MMIAPISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;
        
    case MMISMS_FDN_SCA_FAILED:
        MMIPUB_OpenAlertWarningWin(TXT_SMS_SC_NOT_IN_FDN);
        
#ifdef MMI_TIMERMSG_SUPPORT
        MMIAPISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif        
        break;

    case MMISMS_FDN_DEST_NUM_FAILED:
        MMIPUB_OpenAlertWarningWin(TXT_CC_FDN_ONLY);
        
#ifdef MMI_TIMERMSG_SUPPORT
        MMIAPISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;
        
    case MMISMS_NO_ENOUGH_SPACE:
        //打开提示窗口：存储空间不足
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        
#ifdef MMI_TIMERMSG_SUPPORT
        MMIAPISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;
        
    case MMISMS_SYSTEM_BUSY:
        MMIPUB_OpenAlertWarningWin(TXT_SMS_BUSY);
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMISMS_CALLING_BUSY:
        MMIAPICC_OpenSIMCallingPrompt();
        break;
#endif

    case MMISMS_NO_DEST_ADDRESS:
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
        
#ifdef MMI_TIMERMSG_SUPPORT
        MMIAPISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
        break;
        
    default:
        SCI_TRACE_LOW("MMISMS: MMSDisplaySmsSendErr oper_err = %d", oper_err);
        break;
    }
}

/*****************************************************************************/
//  Description :MMSEditAssembleInfo
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditAssembleInfo(void)  //在发送前获得当前文本信息，subject ,to，cc，bcc联系人信息
{
    MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;
    MMI_STRING_T text_str_info = {0};
    MMI_STRING_T subject_edit_string = {0};
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if(PNULL == edit_doc_ptr)
    {
        return;
    }
    MMSSetMMSContactInfo(MMIMMS_ADD_TO_CONTACT);
#ifdef MMIMMS_SMS_CC_SUPPORT    
    if(GetMMSEditIsCCExist())
    {
        MMSSetMMSContactInfo(MMIMMS_ADD_CC_CONTACT);
    }
    
    if(GetMMSEditIsBCCExist())
    {
        MMSSetMMSContactInfo(MMIMMS_ADD_BCC_CONTACT);
    }
#endif
    GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&text_str_info);
    MMIMMS_AddText(edit_doc_ptr, &text_str_info);
    
    GUIEDIT_GetString(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID,&subject_edit_string);
    MMIMMS_SetSubject(&subject_edit_string,edit_doc_ptr);
    
}

/*****************************************************************************/
//  Description :add new slide
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMSSendSms(MN_DUAL_SYS_E dual_sys)
{    
    if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT == MMIMMS_GetMMSEditEnterReason())
    {
        MMSDeleteDraftMMS();
        MMIAPISMS_InitUpdateInfo();
    }
#ifdef MMI_TIMERMSG_SUPPORT
    //MMIAPISMS_HandleSendOperation(dual_sys, win_id, FALSE);
    if (MMIAPISMS_GetIsTimerMode())
    {
        MMSSaveSms(dual_sys);
        return;
    }
#endif    

    MMIAPISMS_SendSms(dual_sys,FALSE);
    return;
}

/*****************************************************************************/
//  Description :add new slide
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMSSaveSms(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_replace = FALSE;
    
    if (MMISMS_MsgIsInDraftBox())
    {
        if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT == MMIAPIMMS_GetMMSEditEnterReason()) //开始编辑时是彩信，最后保存为短信
        {                                       
            MMISMS_ORDER_ID_T current_order_id = PNULL;
            current_order_id = MMISMS_GetCurOperationOrderId();                 
                                   
            if(current_order_id != PNULL && MMISMS_TYPE_MMS == current_order_id->flag.msg_type)
            {
                MMIMMS_DeleteMMSAndSaveList(current_order_id->record_id);
            }
            MMISMS_SetSecurityBoxType(MMISMS_BOX_NOSEND);
            MMISMS_SetIsSaveDraft(TRUE);
            is_replace = FALSE;
        }
        else
        {
            is_replace = TRUE;
        }
    }
    else
    {
        is_replace = FALSE;
    }
#ifdef MMI_TIMERMSG_SUPPORT
    MMIAPISMS_SaveAsDraft(dual_sys,is_replace,MMIAPISMS_GetIsTimerMode());
#else
    MMIAPISMS_SaveAsDraft(dual_sys,is_replace,FALSE);
#endif

    return;
}


/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SaveSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    if (isReturnOK)
    {
        MMSSaveSms(dual_sys);
    }
    return recode;
}

/*****************************************************************************/
//  Description :save sms draf
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSaveSmsDraft(MMI_HANDLE_T win_id)  
{

    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    {
        MMI_HANDLE_T edit_handle = 0;
        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                        MMIMMS_EDIT_MENU_WIN_ID,//win_id,
                                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                        );
        MMSGetSendNumber(edit_handle,FALSE);
    }               
#else
    MMSGetSendNumber(MMIMMS_SMS_EDIT_TO_LIST_ID,FALSE);
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if(!MMIAPISMS_MsgIsInDraftBox())
    {  
        // uint32 i = 0;
        MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim;
        major_select_sim = MMIAPISET_GetMajorSimCard(MMISET_MAJOR_SIM_SEL_FUNC_SMS);
        if(major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
        {
            dual_sys = (MN_DUAL_SYS_1+((MN_DUAL_SYS_E)major_select_sim-MMISET_MAJOR_SIM_SEL_SIM1));
        }
        else
        {
            if( MMIAPISMS_MEHaveEnoughSpace()
                || ((MMIAPIPHONE_GetSimCardOkNum(PNULL, 0) == 0)
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
                || (MMISMS_BOX_SECURITY == MMIAPISMS_GetCurBoxType())
#endif
                )
                )
            {
                dual_sys = MN_DUAL_SYS_1;
            }
            else
            {
                MMIAPISMS_SelectSIMFunc(SaveSelectSimCallback,(ADD_DATA)win_id);
                return;
            }
        }
        MMSSaveSms(dual_sys);
    }          
    else
    {
        MMSSaveSms(MMIAPISMS_GetCurReadMsgDualSys());
    }
#else

    MMSSaveSms(MN_DUAL_SYS_1);
#endif

    MMK_CloseWin(MMIMMS_EDIT_MAINMENU_OPT_WIN_ID);
}

/*****************************************************************************/
//  Description :get contact count
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL uint16 MMIMMS_GetAllContactCount(void)  //判断联系人数量
{
    uint16 count = 0;
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    {
        MMI_HANDLE_T edit_handle = 0;
        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                        MMIMMS_EDIT_MENU_WIN_ID,//win_id,
                                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                        );
        count = GUIEDIT_GetListItemNum(edit_handle);
    }                
#else
    count = GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_TO_LIST_ID);
#endif
#ifdef MMIMMS_SMS_CC_SUPPORT    
    if(GetMMSEditIsCCExist())
    {
        count += GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_CC_TO_LIST_ID);
    }
    
    if(GetMMSEditIsBCCExist())
    {
        count += GUIEDIT_GetListItemNum(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID);
    }
#endif
    return count;
}

/*****************************************************************************/
//  Description :create attach list win 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateAttachListWin(void)
{
    MMK_CreateWin((uint32*)MMIMMS_ATTACH_LIST_TAB, PNULL);
}
/*****************************************************************************/
//  Description :create attach list win 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void   MMSEditDelAttach(void)
{
    uint32             additional_file_num  = 0;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint8 *add_file_ptr = PNULL;
    const GUILIST_ITEM_T *list_item_ptr = PNULL;
    uint16 i = 0;
    uint16 item_len = 0;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if( PNULL == edit_doc_ptr || PNULL == edit_doc_ptr->editbody_ptr)
    {
        return;
    }
    MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);     
    for(i  = 0; i < additional_file_num; i++)
    {
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMIMMS_ATTACH_LISTBOX_CTRL_ID,i);
        if((PNULL == list_item_ptr )||( PNULL == list_item_ptr->item_data_ptr ))
        {
            break;
        }
        
        if(GUILIST_GetItemState(list_item_ptr, GUIITEM_STATE_SELECTED)) //删除当前附件
        {  
            item_len = list_item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
            add_file_ptr = SCI_ALLOC_APP((item_len + 1)*3);
            if (PNULL == add_file_ptr)
            {
                break;
            }
            SCI_MEMSET(add_file_ptr, 0, (item_len + 1)*3);
            GUI_WstrToUTF8((uint8 *)add_file_ptr, (item_len + 1)*3,list_item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr  ,item_len);
            
            if (!MMIMMS_DelAttachment(add_file_ptr, edit_doc_ptr))
            {
                MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);   
            } 
            else
            {
                BOOLEAN is_mms_mode = TRUE;
                is_mms_mode = MMIMMS_CheckIsMMS();  
                if(!is_mms_mode)
                {
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_MMS_TO_SMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,0); 
                }   
                else
                {   
#ifdef MMI_PDA_SUPPORT                 
                    HandleSendButtonDisplay(TRUE);
#endif
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMSMMI_OpenMMSAlertWin(TXT_SUCCESS, TXT_NULL, IMAGE_PUBWIN_SUCCESS);   
#endif
                }
            }
            SCI_FREE(add_file_ptr);
            add_file_ptr = PNULL;
        }        
    }    
}

/*****************************************************************************/
//     Description : load number edit box
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSAddSmsNumber(
                           MMI_CTRL_ID_T    ctrl_id
                           )
{
    uint8 num_count = MMIAPISMS_GetDestCount();
    
    //显示目的号码信息
    if (0 < num_count)
    {
        uint16   i = 0;
        uint16   actual_count = 0;
        wchar	*number_wstr_ptr = PNULL;
        GUIEDIT_LIST_ITEM_T *list_item_ptr = PNULL;
        
        GUIEDIT_ClearAllStr(ctrl_id);
        
        list_item_ptr = SCI_ALLOC_APP(sizeof(GUIEDIT_LIST_ITEM_T) * num_count);
        SCI_MEMSET(list_item_ptr, 0, (sizeof(GUIEDIT_LIST_ITEM_T) * num_count));
        
        for (i = 0; i< num_count; i++)
        {
            uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
            uint8 num_len = 0;
            MMIPB_BCD_NUMBER_T black_bcd_num = {0};
            MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
            BOOLEAN find_result = FALSE;
            wchar *number_name_wstr = PNULL;
            MMI_STRING_T number_name_addr = {0};
            
            if (MMIAPISMS_GetPointedDestNum(i, &num_len, number))
            {                                                
                number_wstr_ptr = SCI_ALLOC_APP(((MMISMS_PBNUM_MAX_LEN + 2) + 1) * sizeof(wchar));
                SCI_MEMSET(number_wstr_ptr, 0x00, (((MMISMS_PBNUM_MAX_LEN + 2) + 1) * sizeof(wchar)));
                MMI_STRNTOWSTR(number_wstr_ptr, (MMISMS_PBNUM_MAX_LEN + 2), 
                    (const uint8 *)number,(MMISMS_PBNUM_MAX_LEN + 2),num_len);
                MMIAPICOM_GenPartyNumber(number, num_len, &party_num);
                black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
                SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
                number_name_wstr = SCI_ALLOC_APP((MMIPB_NAME_MAX_LEN  + 1) * sizeof(wchar));
                SCI_MEMSET(number_name_wstr, 0x00, ((MMIPB_NAME_MAX_LEN + 1) * sizeof(wchar)));
                number_name_addr.wstr_ptr = number_name_wstr;
                find_result = MMIAPIPB_GetNameByNumber(&black_bcd_num, &number_name_addr, MMIPB_NAME_MAX_LEN, FALSE);
                
                if(find_result && 0 < number_name_addr.wstr_len)
                {//如果匹配名称，则显示名称，不能够修改了
                    list_item_ptr[actual_count].is_object = TRUE;
                    
                    list_item_ptr[actual_count].display_str_ptr = number_name_addr.wstr_ptr;
                    list_item_ptr[actual_count].display_str_len = number_name_addr.wstr_len;
                    list_item_ptr[actual_count].user_str_ptr = number_wstr_ptr;
                    list_item_ptr[actual_count].user_str_len = num_len;
                }
                else
                {
                    list_item_ptr[actual_count].is_object = FALSE;
                    list_item_ptr[actual_count].display_str_ptr = number_wstr_ptr;
                    list_item_ptr[actual_count].display_str_len = num_len;
                    list_item_ptr[actual_count].user_str_ptr = PNULL;
                    list_item_ptr[actual_count].user_str_len = 0;
                    SCI_FREE(number_name_wstr);
                    number_name_wstr = PNULL;
                }
#ifdef MMIMMS_SMS_IN_1_SUPPORT                
                if(!MMIMMS_GetIsMMSMode() && ((URL_IsMailto((URL_CHAR_T const *)number_wstr_ptr, (int32*)&num_len))
                    ||(URL_IsMailtoDefault((URL_CHAR_T const *)number_wstr_ptr, (int32*)&num_len))))
                {
                    MMIMMS_SetMMSEditIsEmailAddrExist(TRUE);
                    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                        PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleConvertSmsToMmsWaitWinMsg,(uint32)0);
                }
#endif
                actual_count++;
            }
            else
            {
                //SCI_TRACE_LOW:"MMS sms get number error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17019_112_2_18_2_38_13_120,(uint8*)"");
            }
        }
        
        GUIEDIT_AddListItemArray(ctrl_id, list_item_ptr, actual_count);
        
        for (i = 0; i < num_count; i++)
        {
            if (PNULL != list_item_ptr[i].display_str_ptr)
            {
                SCI_FREE(list_item_ptr[i].display_str_ptr);
            }
            if (PNULL != list_item_ptr[i].user_str_ptr)
            {
                SCI_FREE(list_item_ptr[i].user_str_ptr);
            }
        }
        
        SCI_FREE(list_item_ptr);
    }
}

/*****************************************************************************/
//     Description :初始化subject
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSSubjectInit(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    uint32 additional_file_num = 0;
    edit_doc_ptr = g_mms_edit_globle.editdoc;    
    if(PNULL == edit_doc_ptr)
    {
        return;
    }
    if(edit_doc_ptr->edit_subject.wstr_ptr != PNULL && edit_doc_ptr->edit_subject.wstr_len != 0)
    {
        SetMMSEditIsSubjectExist(TRUE);
        GUIEDIT_SetString(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, edit_doc_ptr->edit_subject.wstr_ptr,edit_doc_ptr->edit_subject.wstr_len);
    }
    else
    { 
        MMIMMS_GetAttachmentNumber(edit_doc_ptr->editbody_ptr, &additional_file_num);  
        if(1 == MMIMMS_GetSlideCount(edit_doc_ptr) 
            && 0 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag
            && 0 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag
            && 0 == edit_doc_ptr->editbody_ptr->slide_cur->audiosize 
            && 0 == additional_file_num
            )
        {
            SetMMSEditIsSubjectExist(TRUE);
        }
        else
        {
            SetMMSEditIsSubjectExist(FALSE);
        }                     
    }                 
}

/*****************************************************************************/
//     Description : get phone number from phone_book
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSGetNumberFromPb(                            //RETURN: 
                              uint32 handle,  //IN:
                              MMI_MESSAGE_ID_E  msg_id //IN:
                              )
{
    int16 i = 0;
    uint16 count = MMIAPIPB_GetSelectCount(handle);
    uint8 max_len = 0;
    uint8 tele_num[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_SMSEDIT_CONTENT_T   *edit_content_ptr = PNULL;
    MMIPB_BCD_NUMBER_T    number = {0};
    
    edit_content_ptr  = MMIAPISMS_GetEditContentFromGlobal();
    //SCI_TRACE_LOW:"MMISMS: GetNumberFromPb msg_id = 0x%x, count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17095_112_2_18_2_38_13_121,(uint8*)"dd", msg_id, count);
    
    if (MMI_SELECT_CONTACT_CNF == msg_id)        
    {
        if (MMISMS_DEST_ADDR_MAX_NUM < count)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SMS_EXCEED_MAX_NUM);
        }
        for (i=0; i<MIN(MMISMS_DEST_ADDR_MAX_NUM, count); i++)
        {
            SCI_MEMSET(&number, 0, sizeof(MMIPB_BCD_NUMBER_T));
            
            // read the phone number from phonebook one by one
            MMIAPIPB_GetSelectNumber(handle, &number,MMIPB_BCD_NUMBER_MAX_LEN, i);
            
            max_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, number.number_len);
            MMIAPIPB_BCDNumberToString(&number, MMISMS_NUMBER_MAX_LEN + 2, tele_num);
            
            //MMIAPISMS_SetDestNum(tele_num, tele_len);
                        
            MMIAPISMS_SetAddressToMessage(tele_num,strlen((char *)tele_num),(uint32 *)&edit_content_ptr->dest_info.dest_list);
        }
    }        
}

/*****************************************************************************/
//     Description : 彩信草稿变为短信保存或发送，需要删除原来的彩信草稿
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSDeleteDraftMMS(void)
{
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    current_order_id = MMIAPISMS_GetCurOperationOrderId();                 
    
    if(current_order_id && MMISMS_TYPE_MMS == current_order_id->flag.msg_type)
    {
        MMIMMS_DeleteMMSAndSaveList(current_order_id->record_id);
    }
}

/*****************************************************************************/
//     Description : 彩信草稿变为短信保存或发送，需要删除原来的彩信草稿
//    Global resource dependence : 
//  Author: rong.gu
//    Note: 
/*****************************************************************************/
LOCAL void MMSDeleteDraftSMS(void)
{
    MMISMS_ORDER_ID_T current_order_id = PNULL;
    current_order_id = MMIAPISMS_GetCurOperationOrderId();                 
    
    if(current_order_id && MMISMS_TYPE_SMS == current_order_id->flag.msg_type)
    {
        MMIAPISMS_DeleteReqToMN( 
            MMISMS_BOX_NOSEND, 
            current_order_id, 
            MMISMS_DELETE_MSG 
            );
    }
}

/*****************************************************************************/
//  Description :open mms multi edit option menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenEditMultiOptPopMenu(MMIMMS_EDIT_TYPE_OP_TYPE file_type)
{
    MMI_CTRL_ID_T	ctrl_id = MMIMMS_EDIT_MULTI_POP_CTRL_ID;
    uint16 menu_index = 0;
    uint16 index = 0;
    MMI_STRING_T  title_text_str = {0};
    MMI_TEXT_ID_T str_array[MMIMMS_TYPE_OP_MAX][3] =   //菜单1，菜单2，标题
    {
        {TXT_MMS_REPLACE_PIC,TXT_MMS_DELETE_PIC,TXT_COMMON_PIC},
        {TXT_MMS_REPLACE_MUSIC,TXT_MMS_DELETE_MUSIC,TXT_COMMON_MUSIC},
        {TXT_MMS_REPLACE_VIDEO,TXT_MMS_DELETE_VIDEO,TXT_VIDEO},
        {TXT_MMS_REPLACE_ATTACH,TXT_MMS_DELETE_ATTACH,TXT_MMS_ATTACHMENT}
    };
    if(MMIMMS_TYPE_OP_MAX == file_type)   
    {
        //SCI_TRACE_LOW:"MMS file type error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17302_112_2_18_2_38_14_122,(uint8*)"");
        return;
    }
    index = (uint16)file_type;
    //SCI_TRACE_LOW:"MMS file type error %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17306_112_2_18_2_38_14_123,(uint8*)"d",index);
    MMI_GetLabelTextByLang(str_array[index][2], &title_text_str);        
    MMK_CreateWin((uint32 *)MMIMMS_EDIT_MULTI_POP_WIN_TAB,PNULL); 
    GUIMENU_CreatDynamic(PNULL, MMIMMS_EDIT_MULTI_POP_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP_AUTO);
    
    GUIMENU_SetMenuTitle(&title_text_str, ctrl_id);
    
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        str_array[index][0],//text_ptr 
        MMS_CONTEDIT_OPT_NODE_ITEM_REPLACE,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
    );
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        str_array[index][1],//text_ptr 
        MMS_CONTEDIT_OPT_NODE_ITEM_DELETE,//node_id
        0, //parent_node_id
        menu_index++,//index
        FALSE
    );
}

/*****************************************************************************/
//  Description :设置附件中图片显示的高度
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSSetMMSPicHeight(uint8 *image_buf_ptr,uint32 image_buf_len)
{
    IMG_DEC_SRC_T  dec_src = {0};
    IMG_DEC_INFO_T dec_info = {0};
    uint16 original_height                     = 0;
    uint16 pic_width = 0;
    uint16 pic_height = 0;
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    if(PNULL == image_buf_ptr || 0 == image_buf_len)
    {
        //SCI_TRACE_LOW:"MMS image is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17345_112_2_18_2_38_14_124,(uint8*)"");
        return;
    }
    MMIMMS_GetPicDefaultWidthAndHeight(&pic_width, &pic_height);
    dec_src.src_ptr = image_buf_ptr;
    dec_src.src_file_size = image_buf_len;
    if(IMG_DEC_RET_SUCCESS == IMG_DEC_GetInfo(&dec_src,&dec_info))
    {            
           switch (dec_info.img_type) 
           {
           case IMG_DEC_TYPE_JPEG:
               original_height= dec_info.img_detail_info.jpeg_info.img_heigth;
               break;
               
           case IMG_DEC_TYPE_PNG:
               original_height= dec_info.img_detail_info.png_info.img_height;
               break;
               
           case IMG_DEC_TYPE_WBMP:
               original_height= dec_info.img_detail_info.wbmp_info.img_height;
               break;

           case IMG_DEC_TYPE_GIF:
               original_height= dec_info.img_detail_info.gif_info.img_height;
               break;

           case IMG_DEC_TYPE_BMP:
               original_height= dec_info.img_detail_info.bmp_info.img_height;
               break;
               
           default:                    
               break;
           }    
           child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
           child_height.add_data = MIN(original_height,pic_height);
           GUIFORM_SetChildHeight(MMIMMS_SMS_EDIT_ELEMENT_AREA_FORM_ID,MMIMMS_SMS_EDIT_ANIM_CTRL_ID,&child_height);
           MMK_PostMsg(MMIMMS_EDIT_MENU_WIN_ID, MSG_FULL_PAINT, PNULL, 0);  //需要刷新窗口
    }   
}

/*****************************************************************************/
//  Description :设置界面中edit默认的文字
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditSetEditDefaultString(MMIMMS_WINDOW_ID_E win_id)
{
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    {
        MMI_HANDLE_T edit_handle = 0;
        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                        win_id,
                                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                        );
        GUIEDIT_SetDefaultStringById(edit_handle, TXT_COMMON_TO);
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
#ifdef MMS_SMS_IN_1_SUPPORT 
        GUIEDIT_SetFont(edit_handle, MMISMS_GetWinFont());
#endif
#endif
    }                
#else
    GUIEDIT_SetDefaultStringById(MMIMMS_SMS_EDIT_TO_LIST_ID, TXT_COMMON_TO);
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
#ifdef MMS_SMS_IN_1_SUPPORT 
    GUIEDIT_SetFont(edit_handle, MMISMS_GetWinFont());
#endif
#endif
#endif
    GUIEDIT_SetDefaultStringById(MMIMMS_SMS_EDIT_SUBJECT_EDIT_ID, TXT_INPUT_SUBJECT);
    GUIEDIT_SetDefaultStringById(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,TXT_MMS_TOUCH_TO_INPUT);
#ifdef MMIMMS_SMS_CC_SUPPORT        
    GUIEDIT_SetDefaultStringById(MMIMMS_SMS_EDIT_CC_TO_LIST_ID,TXT_MMS_CC);
    GUIEDIT_SetDefaultStringById(MMIMMS_SMS_EDIT_BCC_TO_LIST_ID,TXT_MMS_BCC);
#endif    
}

/*****************************************************************************/
//  Description :录 像 后 的 处 理  
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL int32 MMSDVWorkCallBack(MMIDC_WORK_ERROR_E error_ret,MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0; 
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == data_info_ptr) 
    { 
        return ret; 
    } 
    
    if (MMIDC_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name,data_info_ptr->file_name_len,&file_info);
            if(0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);   
            }
            else if(file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING); 
            }
            else
            {
                BOOLEAN is_need_add_new_slide = FALSE;
                if(MMIMMS_GetIsMMSMode())
                {
                    if(1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag
                      || 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag
                      || edit_doc_ptr->editbody_ptr->slide_cur->audiosize > 0)
                    {
                        is_need_add_new_slide = TRUE;
                    }

                    if(is_need_add_new_slide)
                    {
                       MMSAddNewSlide();  //增加新页显示
                    }
                }
                if(MMIMMS_AddFile(edit_doc_ptr,&file_info,MMIFMM_FILE_TYPE_MOVIE,FALSE,0))//添加成功，同时更新显示
                {         
                   if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                   {
                   MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                       PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);     
                       
                   } 
#ifdef MMI_PDA_SUPPORT                                   
                   else
                   {
                       HandleSendButtonDisplay(TRUE);
                   }
#endif                   
                   MMSShowMultiFileInfo();                          
                   MMK_SetAtvCtrl( MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID);
                }
            }            
        }
    }
    return ret; 
}
/*****************************************************************************/
//  Description :拍 照 后 的 处 理  
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL int32 MMSDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret,MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0; 
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == data_info_ptr) 
    { 
        return ret; 
    } 

    if (MMIDC_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name,data_info_ptr->file_name_len,&file_info);
            if(0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);   
            }
            else if(file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;
                GUIANIM_TYPE_E pic_type = GUIANIM_TYPE_NONE;
                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);
                
                reduce_pic_size = reduce_width*reduce_height*2;

                pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                if (GUIANIM_TYPE_IMAGE_JPG == pic_type)
                {
                    g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_PICTURE;
                    if (PNULL != g_mms_edit_globle.file_info_ptr)
                    {
                        SCI_FREE(g_mms_edit_globle.file_info_ptr);
                        g_mms_edit_globle.file_info_ptr = PNULL;
                    }
                    g_mms_edit_globle.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOCAZ(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == g_mms_edit_globle.file_info_ptr)
                    {
                        g_mms_edit_globle.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        return ret;
                    }
                    SCI_MEMCPY((char *)g_mms_edit_globle.file_info_ptr, (char *)&file_info, sizeof(MMIFILE_FILE_INFO_T));
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC,IMAGE_PUBWIN_QUERY, &quer_win_id,PNULL);
                    SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT_PIC_CHANGE_SIZE);
                }
                else
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);  
                }
            }
            else
            {
                BOOLEAN is_need_add_new_slide = FALSE;
                if(MMIMMS_GetIsMMSMode())
                {
                    if(1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag || 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)  //是否有视频
                    {
                        is_need_add_new_slide = TRUE;                        
                    }
                    if(is_need_add_new_slide)
                    {
                       MMSAddNewSlide();  //增加新页显示
                    }
                }
                if(MMIMMS_AddFile(edit_doc_ptr,&file_info,MMIFMM_FILE_TYPE_PICTURE,FALSE,0))//添加成功，同时更新显示
                {         
                   if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                   {
                   MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                       PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);     
                       
                   } 
#ifdef MMI_PDA_SUPPORT                       
                   else
                   {
                       HandleSendButtonDisplay(TRUE);
                   }
#endif                   
                   MMSShowMultiFileInfo();                          
                   MMK_SetAtvCtrl( MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID);
                }
            }
        }
    }

    return ret; 
}



/*****************************************************************************/
//  Description :save sms draf
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSRedkeySaveSmsDraft(void)  
{
    MN_DUAL_SYS_E dual_sys = 0;
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
    {
        MMI_HANDLE_T edit_handle = 0;
        edit_handle = GUIDROPDOWNLIST_GetInlineEditHandle(
                                        MMIMMS_EDIT_MENU_WIN_ID,//win_id,
                                        MMIMMS_SMS_EDIT_TO_DROPDOWN_LIST_ID
                                        );
        MMSGetSendNumber(edit_handle,FALSE);
    }               
#else
    MMSGetSendNumber(MMIMMS_SMS_EDIT_TO_LIST_ID,FALSE);
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if(!MMIAPISMS_MsgIsInDraftBox())
    {  
        if( ( MMISMS_SAVEPOS_PHONE == MMIAPISMS_GetSavePos() && MMIAPISMS_MEHaveEnoughSpace())
            || ((MMIAPIPHONE_GetSimCardOkNum(PNULL, 0) == 0)
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
            || (MMISMS_BOX_SECURITY == MMIAPISMS_GetCurBoxType())
#endif
            )
            )
        {
            dual_sys = MN_DUAL_SYS_1;
        }
        MMSSaveSms(dual_sys);
    }          
    else
    {

        MMSSaveSms(MMIAPISMS_GetCurReadMsgDualSys());
    }
#else

    MMSSaveSms(MN_DUAL_SYS_1);
#endif
}

#else

/*****************************************************************************/
//  Description :录 像 后 的 处 理  
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL int32 MMSDVWorkCallBack(MMIDC_WORK_ERROR_E error_ret,MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0; 
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == data_info_ptr) 
    { 
        return ret; 
    } 
    
    if (MMIDC_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name,data_info_ptr->file_name_len,&file_info);
            if(0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);   
            }
            else if(file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING); 
            }
            else
            {
                MMIFMM_SELECT_RETURN_T          selected_info = {0};
                
                //SCI_ASSERT(NULL != file_data_ptr);
                selected_info.file_size = file_info.file_size;
                selected_info.file_type = MMIFMM_FILE_TYPE_MOVIE;
                selected_info.pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, data_info_ptr->file_name,data_info_ptr->file_name_len);
                selected_info.file_name_len = data_info_ptr->file_name_len;
                
                selected_info.data_buf_ptr = NULL;
                MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);
            }            
        }
    }
    return ret; 
}

/*****************************************************************************/
//  Description :拍 照 后 的 处 理  
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL int32 MMSDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret,MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0; 
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == data_info_ptr) 
    { 
        return ret; 
    } 

    if (MMIDC_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name,data_info_ptr->file_name_len,&file_info);
            if(0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);   
            }
            else if(file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);  
            }
            else
            {
                MMIFMM_SELECT_RETURN_T          selected_info = {0};

                //SCI_ASSERT(NULL != file_data_ptr);
                selected_info.file_size = file_info.file_size;
                selected_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
                selected_info.pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, data_info_ptr->file_name,data_info_ptr->file_name_len);
                selected_info.file_name_len = data_info_ptr->file_name_len;

                selected_info.data_buf_ptr = NULL;
                MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);
            }
        }
    }
    return ret; 
}
#endif

#if defined(MMS_SMS_IN_1_SUPPORT ) || defined(MMI_RECORD_SUPPORT)
/*****************************************************************************/
//  Description :add new slide
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSAddNewSlide(void)  
{
    uint16 slide_num                      = 0;
    MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;
    MMIMMS_ERROR_E   add_slide_err        = MMIMMS_NO_ERROR;
    MMI_STRING_T    edit_str = {0};
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if(PNULL == edit_doc_ptr)
    {
        return;
    }
    //当前页有视频需要添加新页
    slide_num = MMIMMS_GetSlideCount(edit_doc_ptr);
    if (slide_num < MMIMMS_MAX_SLIDE)
    {
        SetMMSEditIsSaveDraft(FALSE);

#ifndef MMIMMS_SMS_IN_1_SUPPORT
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_INSERT);
        SetMMSEditFocusOnAddMenu(FALSE);

        if (1 == MMIMMS_GetSlideIndex(edit_doc_ptr))
        {
            SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TO));
        }
        else
        {
            SetMMSEditLoseFocusIndex(GetMMSEditItemIndex(MMIMMS_RICHTEXT_TEXT));
        }

        if (MMIMMS_GetSlideCount(edit_doc_ptr) < MMIMMS_MAX_SLIDE)
        {
            SetMMSEditIsSaveDraft(FALSE);
        }

#else
        GUIEDIT_GetString(MMIMMS_SMS_EDIT_TEXT_CTRL_ID,&edit_str);
        MMIMMS_AddText(edit_doc_ptr, &edit_str);
        GUIEDIT_ClearAllStr(MMIMMS_SMS_EDIT_TEXT_CTRL_ID);
#endif
        add_slide_err = MMIMMS_AddSlide(edit_doc_ptr,1);
        if (MMIMMS_NO_ERROR != add_slide_err)
        {
            MMIMMS_EditMMSAlertWin(add_slide_err);
        }
    }
    else
    {
        MMSMMI_OpenMMSAlertWin(TXT_MMS_MAX_SLIDE, TXT_NULL, IMAGE_PUBWIN_WARNING);     
    }
}

/*****************************************************************************/
//  Description :show file info not include attach
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowMultiFileInfo(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if(( PNULL == edit_doc_ptr  )||( PNULL == edit_doc_ptr->editbody_ptr )||(PNULL == edit_doc_ptr->editbody_ptr->slide_cur))
    {
        return;
    }
    if((1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag )||(1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag) )
    {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        MMSEditAddPicItem();
#else
        MMSEditAddPicItem(MMIMMS_RICHTEXT_ADD_ITEM);
#endif
    }
    if(0 != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)
    {
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        MMSEditAddMusicItem();
#else
        MMSEditAddMusicItem(MMIMMS_RICHTEXT_ADD_ITEM);
#endif
    }    
}
#endif

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenRecord(void)
{
    MMIRECORD_WORK_PARAM_T parameter={0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;

    edit_doc_ptr = g_mms_edit_globle.editdoc;

    parameter.max_file_size = MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT - (uint32)edit_doc_ptr->editbody_ptr->totalsize;
    parameter.max_file_time = NULL;
    parameter.record_support_type = MMIRECORD_WORK_FORMAT_WAV;
    parameter.work_callback = MMSRecordWorkCallBack;
    if(MMIAPIRECORD_OpenMainWin(&parameter) == MMIRECORD_WORK_SUCCESS)
    {
        //SCI_TRACE_LOW:"[MMIMMS]MMSOpenRecord mms record error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17473_112_2_18_2_38_14_127,(uint8*)"");
    }    
}


/*****************************************************************************/
//  Description :录音 后 的 处 理  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr)
{
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if (PNULL == data_info_ptr) 
    { 
        return; 
    } 

    if (MMIRECORD_WORK_SUCCESS == error_ret)  
    { 
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name,data_info_ptr->file_name_len,&file_info);
            if(0 == file_info.file_size )
            {
                MMSMMI_OpenMMSAlertWin(TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING);   
            }
            //including head audio ref:mmsGetAudioSmilLen cr151282
            else if((file_info.file_size + (SCI_STRLEN("<audio src=\"\" />")+SCI_STRLEN(file_info.file_name)+30+3*SCI_STRLEN(file_info.file_name))) > 
                (MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT))
            {                
                MMSMMI_OpenMMSAlertWin(TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING);  
            }
            else
            {
                BOOLEAN is_need_add_new_slide = FALSE;

#ifdef MMIMMS_SMS_IN_1_SUPPORT
                if(MMIMMS_GetIsMMSMode())
#endif
                {
                    if(1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag || 0 < edit_doc_ptr->editbody_ptr->slide_cur->audiosize)  //是否有视频
                    {
                        is_need_add_new_slide = TRUE;                        
                    }
                    if(is_need_add_new_slide)
                    {
                       MMSAddNewSlide();  //增加新页显示
                    }
                }
                if(MMIMMS_AddFile(edit_doc_ptr,&file_info,MMIFMM_FILE_TYPE_MUSIC,FALSE,0))//添加成功，同时更新显示
                {   
#ifdef MMIMMS_SMS_IN_1_SUPPORT                    
                   if(!MMIMMS_GetIsMMSMode()) //非彩信的情况
                   {
                   MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_MMS_SMS_TO_MMS,TXT_NULL,IMAGE_PUBWIN_WAIT,
                       PNULL,PNULL,MMIPUB_SOFTKEY_NONE,HandleMMSSmsConvertWaitWinMsg,(uint32)0);                            
                   }            
                   else
#endif
                   {   
#ifdef MMI_PDA_SUPPORT                       
                       HandleSendButtonDisplay(TRUE);  
#endif
                   }
                 
                   MMSShowMultiFileInfo();                          
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                   MMK_SetAtvCtrl( MMIMMS_EDIT_MENU_WIN_ID, MMIMMS_SMS_EDIT_MMS_INFO_TEXT_ID);
#endif
                }
            }
        }
    }

    return ;
}
#endif


/*****************************************************************************/
//  Description :现拍图片 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenDV(void) 
{
    MMIDC_WORK_PARAM_T work_param = {0}; 
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR; 
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    
    work_param.work_mode = MMIDC_WORK_DV; 
    work_param.param.dv.max_file_size = MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT - (uint32)edit_doc_ptr->editbody_ptr->totalsize;
    work_param.param.dv.max_video_size = WORK_VIDEO_SIZE_160_128;     
    work_param.work_callback = MMSDVWorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param); 
    if (MMIDC_WORK_SUCCESS != error_ret) 
    { 
        //SCI_TRACE_LOW:"mms dc error %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17451_112_2_18_2_38_14_126,(uint8*)"d",error_ret);
    } 

}

/*****************************************************************************/
//  Description :现拍图片 
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSOpenDC(void) 
{
    MMIDC_WORK_PARAM_T work_param = {0}; 
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR; 

    work_param.work_mode = MMIDC_WORK_DC; 
    //work_param.param.dc.max_photo_size = WORK_PHOTO_SIZE_640_480; //use default value
    work_param.work_callback = MMSDCWorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param); 
    if (MMIDC_WORK_SUCCESS != error_ret) 
    { 
        //SCI_TRACE_LOW:"mms dc error %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EDITWIN_17427_112_2_18_2_38_14_125,(uint8*)"d",error_ret);
    } 

}

/*****************************************************************************/
//  Description :mms edit insert DC Camera
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertDCPic(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace)
{
    uint32 pic_max_size                      = 0;
    uint32 file_size                         = 0;
    uint8 multi_temp[MMS_FILE_NAME_LEN + 1]  = {0};
    MMIFMM_SELECT_INFO_T    select_info = {
        MMIFMM_PIC_ALL,0,
        0,0,PNULL,0,-1};
    {
        if (PNULL == edit_doc_ptr)
        {
            return;
        }
        if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
        {
            SetMMSEditInsertOpType(MMIMMS_OP_MAX);
            MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_HALF_POP);
            return;
        }
        if (is_replace && 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
        {
            file_size = MMIMMS_GetCurrentSlideImageInfo(multi_temp, MMS_FILE_NAME_LEN, edit_doc_ptr);
            SetMMSEditIsReplaceMultifile(TRUE);
            SetMMSEditReplaceMultifileSize(file_size);
            pic_max_size = MMIMMS_MMS_LIMIT_SIZE - (uint32)edit_doc_ptr->editbody_ptr->totalsize + file_size;
        }
        else 
        {
            pic_max_size = MMIMMS_MMS_LIMIT_SIZE - (uint32)edit_doc_ptr->editbody_ptr->totalsize;
        }
    }
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
    select_info.max_size = pic_max_size;
    select_info.win_id = pre_win_id;
    MMSOpenDC();
    return;
}

/*****************************************************************************/
//  Description :mms edit insert DV
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMSEditInsertDV(MMIMMS_WINDOW_ID_E pre_win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_replace)
{
    uint32 pic_max_size                      = 0;
    uint32 file_size                         = 0;
    uint8 multi_temp[MMS_FILE_NAME_LEN + 1]  = {0};
    MMIFMM_SELECT_INFO_T    select_info = {
        MMIFMM_PIC_ALL,0,
        0,0,PNULL,0,-1};
    {
        if (PNULL == edit_doc_ptr)
        {
            return;
        }
        if (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE)
        {
            SetMMSEditInsertOpType(MMIMMS_OP_MAX);
            MMSMMI_OpenMMSAlertWin(TXT_MMS_REACH_MAXSIZE_LIMIT, TXT_NULL, IMAGE_PUBWIN_WARNING);        
            SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_HALF_POP);
            return;
        }
        if (is_replace && 1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)
        {
            file_size = MMIMMS_GetCurrentSlideImageInfo(multi_temp, MMS_FILE_NAME_LEN, edit_doc_ptr);
            SetMMSEditIsReplaceMultifile(TRUE);
            SetMMSEditReplaceMultifileSize(file_size);
            pic_max_size = MMIMMS_MMS_LIMIT_SIZE - (uint32)edit_doc_ptr->editbody_ptr->totalsize + file_size;
        }
        else 
        {
            pic_max_size = MMIMMS_MMS_LIMIT_SIZE - (uint32)edit_doc_ptr->editbody_ptr->totalsize;
        }
    }
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
    select_info.max_size = pic_max_size;
    select_info.win_id = pre_win_id;
    MMSOpenDV();
    return;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Interupt(void)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMIMMS_ERROR_E   mms_op_result        = MMIMMS_NO_ERROR; 
    
    edit_doc_ptr = g_mms_edit_globle.editdoc;
    if(PNULL == edit_doc_ptr)//bug337284 //影响到了VT的流程导致MMIVT_GetAppletInstance的值被提前置为NULL
    {
        SCI_TRACE_LOW("MMIMMS_Interupt edit_doc_ptr = NULL");
        return FALSE;//如果edit_doc_ptr为空说明MMS并未进入编辑界面 
    }

#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if(MMIMMS_GetIsMMSMode())
#endif    			
    {
        SetMMSEditLoseFocusReason(MMS_CONTEDIT_OPT_NODE_SAVE_DRAFT);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        MMSEditAssembleInfo();
#endif
        mms_op_result = MMIMMS_SaveMMS(MN_DUAL_SYS_1, MMISMS_MO_DRAFT, edit_doc_ptr);
        if (MMIMMS_NO_ERROR != mms_op_result)
        {
            MMIMMS_EditMMSAlertWin(mms_op_result);
            MMK_CloseWin(MMIMMS_EDIT_MAINMENU_OPT_WIN_ID);
            return FALSE;
        }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        else
        {
            if(MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS == MMIMMS_GetMMSEditEnterReason())
            {
                MMSDeleteDraftSMS();            
            }
        }
#endif
        SetMMSEditIsSaveDraft(TRUE);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS); 
#endif
        MMIMMS_CloseEditWin();
        MMK_CloseWin(MMIMMS_EDIT_MAINMENU_OPT_WIN_ID);
        return TRUE;
    }

}


/*****************************************************************************/
//  Description :to make sure mms not in 1 can insert to edittext
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_SetEditTextContent(wchar* str_phiz, uint16 str_len)
{
    MMI_STRING_T mms_text_string = {0};
    MMI_STRING_T mms_pretext_string = {0};
    MMI_STRING_T mms_add_string = {0};
    MMIMMS_ERROR_E add_text_res = MMIMMS_NO_ERROR;
    uint16 uiBackup_len = 0;
    BOOLEAN is_right = TRUE;
    MMIMMS_EDIT_DOCUMENT_T* edit_doc_ptr = g_mms_edit_globle.editdoc;
    
    //todo 保存主题，回到编辑主界面            
    mms_text_string.wstr_ptr = str_phiz;
    // mms_text_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(str_phiz); //公共API不准
    mms_text_string.wstr_len = str_len;
    
    if (PNULL != edit_doc_ptr && edit_doc_ptr->editbody_ptr)
    {
        MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_pretext_string);
        //////if (PNULL != mms_pretext_string.wstr_ptr && 0 != mms_pretext_string.wstr_len)
        {
            uiBackup_len = mms_pretext_string.wstr_len;
            
            mms_add_string.wstr_ptr = (wchar *)SCI_ALLOC_APP((mms_pretext_string.wstr_len + mms_text_string.wstr_len + 1) * sizeof(wchar));
            SCI_MEMSET(mms_add_string.wstr_ptr, 0, ((mms_pretext_string.wstr_len + mms_text_string.wstr_len + 1) * sizeof(wchar)));
            mms_add_string.wstr_len = mms_pretext_string.wstr_len + mms_text_string.wstr_len;
            MMIAPICOM_Wstrncpy(mms_add_string.wstr_ptr, mms_pretext_string.wstr_ptr, mms_pretext_string.wstr_len);
            MMIAPICOM_Wstrncpy(mms_add_string.wstr_ptr + mms_pretext_string.wstr_len, mms_text_string.wstr_ptr, mms_text_string.wstr_len);
        }            
        
        SetMMSEditIsSaveDraft(FALSE);
        add_text_res = MMIMMS_AddText(edit_doc_ptr, &mms_add_string);
        if (MMIMMS_NO_ERROR != add_text_res)
        {
            MMIMMS_EditMMSAlertWin(add_text_res);
            if (PNULL != mms_add_string.wstr_ptr)
            {
                SCI_Free(mms_add_string.wstr_ptr);
                mms_add_string.wstr_ptr = PNULL;
                mms_add_string.wstr_len = 0;
            }
            is_right = FALSE;
        }
        if (is_right && (edit_doc_ptr->editbody_ptr->totalsize > (int32)MMIMMS_MMS_LIMIT_SIZE))
        {
            SetMMSEditIsSaveDraft(FALSE);
            mms_add_string.wstr_len = uiBackup_len;
            add_text_res = MMIMMS_AddText(edit_doc_ptr, &mms_add_string);
            if (MMIMMS_NO_ERROR != add_text_res)
            {
                MMIMMS_EditMMSAlertWin(add_text_res);
            }
            MMSMMI_OpenMMSAlertWin(TXT_MMS_INVALID_SIZE, TXT_NULL, IMAGE_PUBWIN_WARNING);                
        }
        
        if (PNULL != mms_add_string.wstr_ptr)
        {
            SCI_FREE(mms_add_string.wstr_ptr);
            mms_add_string.wstr_ptr = PNULL;
            mms_add_string.wstr_len = 0;
        }
    }
    else 
    {
        is_right = FALSE;
    }
    return is_right;
}
/*Edit by script, ignore 4 case. Thu Apr 26 19:00:59 2012*/ //IGNORE9527


/*Edit by script, ignore 19 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
