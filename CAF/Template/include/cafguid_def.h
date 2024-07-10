/****************************************************************************
** File Name:      cafguid_def.h                                           *
** Author:         James.Zhang                                             *
** Date:           04/15/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_GUID_DEF_H_    
#define  _CAF_GUID_DEF_H_  
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define IUNKNOWN_ID  1
#define IMODULE_ID  2

#define SPRD_GUID_DC              6
#define SPRD_GUID_SFS             7


//Defined by Riancy.Zhang
#define SPRD_GUID_SOUND          9
#define SPRD_GUID_MEDIA          10

/*  by zw  */
#define SPRD_GUID_NETMGR          20
#define SPRD_GUID_SOCKET          21
#define SPRD_GUID_TAPI            22
#define SPRD_GUID_HEAP            23

//define by songliang.liu
#define SPRD_GUID_THREAD          24



//defined by andrew.zhang at 2009/03/05
#define SPRD_GUID_IMAGE          30

//guid control guid
#define SPRD_GUI_CTRL_GUID_STATR  1000
#define SPRD_GUI_EDITBOX_ID       SPRD_GUI_CTRL_GUID_STATR
#define SPRD_GUI_LIST_ID          (SPRD_GUI_CTRL_GUID_STATR + 1)
#define SPRD_GUI_ANIM_ID          (SPRD_GUI_CTRL_GUID_STATR + 2)
#ifdef MMI_VER_MAIN
#define SPRD_GUI_MAINMENU_ID      (SPRD_GUI_CTRL_GUID_STATR + 3)
#define SPRD_GUI_MENU_ID          (SPRD_GUI_CTRL_GUID_STATR + 4)
#define SPRD_GUI_MSGBOX_ID        (SPRD_GUI_CTRL_GUID_STATR + 5)
#define SPRD_GUI_PRGBOX_ID        (SPRD_GUI_CTRL_GUID_STATR + 6)
#define SPRD_GUI_TEXTBOX_ID       (SPRD_GUI_CTRL_GUID_STATR + 7)
#endif
#ifdef MMI_VER_STABLE
#define SPRD_GUI_MENU_ID          (SPRD_GUI_CTRL_GUID_STATR + 3)
#define SPRD_GUI_MSGBOX_ID        (SPRD_GUI_CTRL_GUID_STATR + 4)
#define SPRD_GUI_PRGBOX_ID        (SPRD_GUI_CTRL_GUID_STATR + 5)
#define SPRD_GUI_TEXTBOX_ID       (SPRD_GUI_CTRL_GUID_STATR + 6)
#endif
#define SPRD_GUI_LABEL_ID         (SPRD_GUI_CTRL_GUID_STATR + 8)
#define SPRD_GUI_PUNCTUATION_ID   (SPRD_GUI_CTRL_GUID_STATR + 9)
#define SPRD_GUI_BUTTON_ID        (SPRD_GUI_CTRL_GUID_STATR + 10)
#define SPRD_GUI_DROPDOWNLIST_ID  (SPRD_GUI_CTRL_GUID_STATR + 11)
#define SPRD_GUI_ICONLIST_ID      (SPRD_GUI_CTRL_GUID_STATR + 12)
#define SPRD_GUI_RICHTEXT_ID      (SPRD_GUI_CTRL_GUID_STATR + 13)
#define SPRD_GUI_TAB_ID           (SPRD_GUI_CTRL_GUID_STATR + 14)
#define SPRD_GUI_STATUSBAR_ID     (SPRD_GUI_CTRL_GUID_STATR + 15)
#define SPRD_GUI_SOFTKEY_ID       (SPRD_GUI_CTRL_GUID_STATR + 16)
#define SPRD_GUI_TITLE_ID         (SPRD_GUI_CTRL_GUID_STATR + 17)
#define SPRD_GUI_TIPS_ID          (SPRD_GUI_CTRL_GUID_STATR + 18)
#define SPRD_GUI_IM_ID            (SPRD_GUI_CTRL_GUID_STATR + 19)
#define SPRD_GUI_SETLIST_ID       (SPRD_GUI_CTRL_GUID_STATR + 20)
#define SPRD_GUI_FORM_ID          (SPRD_GUI_CTRL_GUID_STATR + 21)
#define SPRD_GUI_OWNDRAW_ID       (SPRD_GUI_CTRL_GUID_STATR + 22)
#define SPRD_GUI_TOOLBAR_ID       (SPRD_GUI_CTRL_GUID_STATR + 23)
#define SPRD_GUI_ICONFOLDER_ID    (SPRD_GUI_CTRL_GUID_STATR + 24)
#ifdef MMI_VER_MAIN
#ifdef GUIF_SCROLLKEY
#define SPRD_GUI_SCROLLKEY_ID     (SPRD_GUI_CTRL_GUID_STATR + 25)
#endif
#define SPRD_GUI_CHECKBOX_ID      (SPRD_GUI_CTRL_GUID_STATR + 26)
#define SPRD_GUI_SLIDER_ID        (SPRD_GUI_CTRL_GUID_STATR + 27)
#define SPRD_GUI_PROGRESS_ID      (SPRD_GUI_CTRL_GUID_STATR + 28)
#define SPRD_GUI_LETTERS_ID       (SPRD_GUI_CTRL_GUID_STATR + 29)
#define SPRD_GUI_SCROLL_ID        (SPRD_GUI_CTRL_GUID_STATR + 30)
#define SPRD_GUI_PICKER_ID        (SPRD_GUI_CTRL_GUID_STATR + 31)
#define SPRD_GUI_TIMEPICKER_ID    (SPRD_GUI_CTRL_GUID_STATR + 32)
#define SPRD_GUI_DATEPICKER_ID    (SPRD_GUI_CTRL_GUID_STATR + 33)
#define SPRD_GUI_COMBOX_ID        (SPRD_GUI_CTRL_GUID_STATR + 34)
#define SPRD_GUI_TEXTEDIT_ID      (SPRD_GUI_CTRL_GUID_STATR + 35)
#define SPRD_GUI_PHONENUMEDIT_ID  (SPRD_GUI_CTRL_GUID_STATR + 36)
#define SPRD_GUI_DIGITALEDIT_ID   (SPRD_GUI_CTRL_GUID_STATR + 37)
#define SPRD_GUI_PSWEDIT_ID       (SPRD_GUI_CTRL_GUID_STATR + 38)
#define SPRD_GUI_LISTEDIT_ID      (SPRD_GUI_CTRL_GUID_STATR + 39)
#define SPRD_GUI_DATEEDIT_ID      (SPRD_GUI_CTRL_GUID_STATR + 40)
#define SPRD_GUI_TIMEEDIT_ID      (SPRD_GUI_CTRL_GUID_STATR + 41)
#define SPRD_GUI_IPEDIT_ID        (SPRD_GUI_CTRL_GUID_STATR + 42)
#define SPRD_GUI_HEADERANDDIVIDER_ID 	(SPRD_GUI_CTRL_GUID_STATR + 43)
#endif
#ifdef MMI_VER_STABLE
#ifdef GUI_CTRL_SCROLL_KEY
#define SPRD_GUI_SCROLLKEY_ID     (SPRD_GUI_CTRL_GUID_STATR + 25)
#endif
#endif
#define SPRD_GUI_CTRL_GUID_END    (SPRD_GUI_CTRL_GUID_STATR + 100)

//mmi applet guid, 0x8000~0xFFFF
#define SPRD_MMI_GUID_START       0x8000
#define SPRD_MMI_GUID_END         0xFFFF

#define SPRD_GUID_USER_START      0x10000

#endif

