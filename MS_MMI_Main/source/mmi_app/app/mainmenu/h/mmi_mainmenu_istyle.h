/*****************************************************************************
** File Name:      mmi_mainmenu_istyle.h                                     *
** Author:                                                                   *
** Date:           10/17/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_ISTYLE_H_
#define _MMI_MAINMENU_ISTYLE_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_link.h"
#include "mmitheme_mainmenu.h"
#include "mmi_mainmenu_data.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//page define
#define ISTYLE_MAX_FILEFOLD_NUM 25
#define ISTYLE_PAGE_CNT_MAX (MMITHEME_MENUMAIN_SLIDE_PAGE_NUM)
#define ISTYLE_PAGE_ITEM_CNT_MAX (MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM)

typedef struct
{
    uint32 cnt;
    MMIMAINMENU_ITEM_INFO_T item[ISTYLE_PAGE_ITEM_CNT_MAX];
} ISTYLE_PAGE_INFO_T;

typedef struct  
{
    uint32 cnt;
    ISTYLE_PAGE_INFO_T page[ISTYLE_PAGE_CNT_MAX];
} ISTYLE_PAGES_INFO_T;


//bar define
#define ISTYLE_BAR_ITEM_CNT_MAX (MMITHEME_ISTYLEMENU_BAR_ITEM_MAX)

typedef struct  
{
    uint32 cnt;
    MMIMAINMENU_ITEM_INFO_T item[ISTYLE_BAR_ITEM_CNT_MAX];
} ISTYLE_BAR_INFO_T;

//folder
#define ISTYLE_FOLDER_CNT_MAX (ISTYLE_MAX_FILEFOLD_NUM)
#define ISTYLE_FOLDER_ITEM_CNT_MAX (GUIMENU_FILEFOLD_MAX_ITEM_NUM)
#define ISTYLE_FOLDER_NAME_LEN_MAX (GUIMENU_DYMAINMENU_STR_MAX_NUM)

typedef struct
{
    uint32 menu_id;
    uint16 text[ISTYLE_FOLDER_NAME_LEN_MAX + 1];
    uint16 text_len;
    uint32 cnt;
    MMIMAINMENU_ITEM_INFO_T item[ISTYLE_FOLDER_ITEM_CNT_MAX];
} ISTYLE_FOLDER_INFO_T;

typedef struct  
{
    uint32 cnt;
    ISTYLE_FOLDER_INFO_T folder[ISTYLE_FOLDER_CNT_MAX];
} ISTYLE_FOLDERS_INFO_T;

typedef MMIMAINEMENU_PAGE_LIST_T MMIMAINEMENU_FOLDER_LIST_T;

typedef struct mmimainmenu_istyle_data_tag MMIMAINMENU_ISTYLE_DATA_T;

struct mmimainmenu_istyle_data_tag
{
    MMI_HANDLE_T                win_handle;                             //window handle
    MMI_HANDLE_T                ctrl_handle;

    BOOLEAN                     is_move_state;                          //item�Ƿ��ڶ���
    BOOLEAN                     is_delete_state;                        // item�Ƿ�ɾ��
    BOOLEAN                     is_moving;                              //item�Ƿ��϶�
    BOOLEAN                     is_empty_space;                         //tp upʱ���Ƿ���item��
    BOOLEAN                     is_item_pressed;                        //tp downʱ���Ƿ���item��
    BOOLEAN                     is_tp_pressed;                          //is tp down?
    BOOLEAN                     is_press_without_moving;                // for compiler
    BOOLEAN                     is_press_in_folder_rect;

    BOOLEAN                     is_bar_item;                            //is bar item
    uint16                      cur_page_index;                         //default 0,from 0 to max-1
    uint16                      cur_item_index;                         //current selected menu index. in option page style, it is also item index

    GUIMENU_ISTYLE_DATA_T       default_data;                         //���˵�����Ϣ
    MMITHEME_MAINMENU_COMMON_T  theme_data;                        //main menu theme

    ISTYLE_PAGES_INFO_T         page_info;
    ISTYLE_BAR_INFO_T           bar_info;
    ISTYLE_FOLDERS_INFO_T       folder_info;

    //open folder info
    BOOLEAN                     is_folder_open;
    BOOLEAN                     is_folder_open_from_bar;
    int16                       open_folder_page_index;
    int16                       open_folder_item_index;
    int16                       open_folder_index;
    MMI_HANDLE_T                ctrl_foldername_handle;

    BOOLEAN                     is_temp_filefold_create;                //�Ƿ�������ʱ�ļ���icon
    int16                       temp_filefold_index;                    // �ļ����γ�ʱ��index
    

    //for istyle search
    BOOLEAN                     is_search_display;    //search page data already exist
    BOOLEAN                     is_cur_pagesearch;    //����ҳ�ǲ��ǵ�ǰҳ
    BOOLEAN                     is_search_entry;      //enter search page
    BOOLEAN                     is_search_listslide;  //record list slide vertical or menu slide horizontal
    GUI_LCD_DEV_INFO            tmp_layer_handle;       //temp layer handle

    uint16                      tmp_width;                              //�ƶ���ʱ��ʹ��
    uint16                      tmp_height;                             //�ƶ���ʱ��ʹ��
    uint16                      tmp_cur_item_index;                     //�ƶ���ʱ��ʹ��
    uint16                      tmp_cur_page_index;                    //�ƶ���ʱ��ʹ��
    BOOLEAN                     tmp_is_bar_item;                        //�ƶ���ʱ��ʹ��

    uint8                       tp_timer_id;                            // Ϊ֧��TP�����ƶ����ӵ�timer
    uint8                       redrew_timer_id;                     // ����ʱ��slide��fling״̬�£���ͼ��timer
    uint8                       user_timer_id;                          // Ӧ���Զ����timer
    uint8                       shake_icon_timer_id;                    // ���붶��״̬ʱ��ÿ�ζ����ļ��
    CONTINUE_MOVE_TYPE_E        continue_move_type;                     // �����ƶ�������
    uint8                       start_move_timer_id;

    MMK_TP_SLIDE_E              slide_state;                            //current slide state
    MMI_MENU_ID_T               delete_menu_id;                         //��֯״̬��ʹ��,Ҫɾ����menu_id
    GUI_POINT_T                 slide_start_point;                      //Slideʱ��ʹ��
    GUI_POINT_T                 slide_pre_point;                        //Slideʱ��ʹ��
    GUI_POINT_T                 pre_tp_point;                           //��һ��tp move������

    GUI_LCD_DEV_INFO            icon_move_layer_handle;                 //ͼ���ƶ���
    GUI_LCD_DEV_INFO            page_layer_handle[GUIMENU_SLIDE_PAGE_NUM]; //ҳ���ƶ���1
    GUI_LCD_DEV_INFO            titleidx_layer_handle;                  // ҳ��������
    GUI_LCD_DEV_INFO            bar_item_layer;                             // �����˵���item�Ĳ���Ϣ

    float                       fling_velocity;                         //�����ٶ�

    BOOLEAN (*U_SetPageInfo)(MMIMAINMENU_ISTYLE_DATA_T *, uint16, void *);                // ����U������Ϣ
};



/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IStyleRegister(
                                          CTRLMAINMENU_PM_APIS_T *apis
                                          );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_MAINMENU_ISTYLE_H_

/*@}*/
