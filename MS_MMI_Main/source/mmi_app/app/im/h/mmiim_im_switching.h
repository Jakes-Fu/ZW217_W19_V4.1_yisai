/*==============================================================================
File Name: mmiim_im_switching.h
Author: Haiwu.Chen
Date: 
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: �����������ļ�mmiim_sp_setting.c��ʵ����ʽ
�ڶ�����£�����ʵ�ָ������뷨���л�����
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/06/26   Haiwu.Chen      Create.
==============================================================================*/

#ifndef _MMIIM_IM_SWITHING_H_
#define _MMIIM_IM_SWITHING_H_

#include "mmiim_base.h"
//#include "mmiset.h"
#include "mmiset_export.h"

//�������뷨��Ӧ����ʾ����
/*
method Ψһ��ֵ��������־���뷨�ģ������ñ��ж����ֵһ��
kernel ���ֵҲ�϶���Ҫ�еģ������޷���������
is_display ��ʾ�Ƿ���ʾ�����Թ��û�ѡ�����ض�Ӧ�û�������Щ���뷨����������ʹ�ã����䷢���˵绰����ʱ
*/
typedef struct
{
    GUIIM_METHOD_T          method;
    //wchar const *         method_name_str;
    MMI_TEXT_ID_T           txt_id;
    BOOLEAN                 is_display;
}MMIIM_IM_NAME_T;

//��һ���������ò˵���ÿ����GUIIM_METHOD_T��ֵ��Ӧ������
//����ĸ�ֵ�ڲ˵��ĳ�ʼ��ʱһ������
typedef struct 
{
    uint16                  idx;
    GUIIM_METHOD_T          method;     
}MMIIM_IM_MENU_T;


//const define
//����ֵ���������꣬���������ص�һЩ���ã�ֻ��һ���ο���������ȷ��
enum
{
    
    MMIIM_INPUT_METHOD_COMMON_COUNT = 4,            //ͨ�����뷨�л������ֵ

    //���²������������飬����
    MMIIM_SP_SETTING_COLOR_BG = 0x2104,
    MMIIM_SP_SETTING_COLOR_BORDER = 0xc618,
    MMIIM_SP_SETTING_COLOR_FONT = 0xe71c,
    MMIIM_SP_SETTING_COLOR_HOT = 0xfcc0,
    MMIIM_SP_SETTING_FONT = SONG_FONT_14,
    //item min width
    MMIIM_SP_SETTING_ITEM_WIDTH = 48,
    MMIIM_SP_SETTING_ITEM_MARGIN = 20,
    //item default height
    MMIIM_SP_SETTING_ITEM_HEIGHT = 24,
};

//popmenu ��������㷽��
typedef enum
{
    MMIIM_EP_TOP_LEFT     = 0,
    MMIIM_EP_TOP_RIGHT    = 1,
    MMIIM_EP_BOTTOM_LEFT  = 2,
    MMIIM_EP_BOTTOM_RIGHT = 3
} MMIIM_POPMENU_EP_E;


//win data, �л����뷨������ص�����
typedef struct
{
    GUIIM_INIT_KERNEL_T*    im_kernel_ptr;          //���뷨��������
    GUI_BOTH_RECT_T         disp_rect;              //��ʾ����Ӧ�õ���ʱ��������
    MMIIM_POPMENU_EP_E      effective_point;        //popmenu���������
    
    void*                   user_data_ptr;          //���ڵ��������ݣ�Ԥ��

    GUIIM_LANGUAGE_T        last_lang;
    uint16                  item_hot;
    uint16                  item_counts;
    MMIIM_IM_NAME_T*        item_ptr;

    MMIIM_IM_MENU_T*        menu_table_ptr;         //�˵���Ӧ�����뷨������

    //�ڴ���չ
    GUIIM_METHOD_T          default_method;         //�û��л��������ԣ�Ĭ�ϵ�һ��Ϊ��ǰ���뷨

} MMIIM_SWITCHING_IM_WIN_DATA_T;


//win data, ���õ�ǰ����������
typedef struct
{
    GUIIM_INIT_KERNEL_T*    im_kernel_ptr;          //���뷨��������
    GUI_BOTH_RECT_T         disp_rect;              //��ʾ����Ӧ�õ���ʱ��������
    MMIIM_POPMENU_EP_E      effective_point;        //popmenu���������
    
    uint16                  hot_lang;               //�����Ĭ�Ͻ�������
    
    void*                   user_data_ptr;          //���ڵ��������ݣ�Ԥ��

    //�ڴ���չ

} MMIIM_SET_INPUT_LANG_WIN_DATA_T;




/*****************************************************************************/
//  Description : ������뷨ѡ���radio list��item
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_AppendIMListItem(
                                   MMI_CTRL_ID_T ctrl_id, 
                                   MMIIM_SWITCHING_IM_WIN_DATA_T* win_data_ptr
                                   );

/*****************************************************************************/
//  Description : ��ɴ��ڴ���֮ǰ��׼��
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: Ҫ��ʾ������
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIIM_SwitchingCreateWin(
                                             GUIIM_INIT_KERNEL_T* im_kernel_ptr, 
                                             GUI_BOTH_RECT_T disp_rect,
                                             MMIIM_POPMENU_EP_E effective_point
                                             );

/*****************************************************************************/
//  Description : ������õ�ǰ�������Դ��ڵĴ���
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIIM_SetLangCreateWin(
                                           GUIIM_INIT_KERNEL_T* im_kernel_ptr,
                                           GUI_BOTH_RECT_T disp_rect,
                                           MMIIM_POPMENU_EP_E effective_point
                                           );

/*****************************************************************************/
//  Description : ��set�е������򣬵õ��ؼ��е�����ֵ
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetGUILangBySetLangIdx(
                                              MMISET_LANGUAGE_TYPE_E set_lang_index
                                              );

/*****************************************************************************/
//  Description : �ɿؼ��е�����ֵ���õ�set�е�������
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIIM_GetSetLangByGUILangIdx(
                                              GUIIM_LANGUAGE_T gui_lang_index
                                              );

#endif /*_MMIIM_IM_SWITHING_H_*/


