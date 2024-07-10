/*****************************************************************************
** File Name:      mmiqbtheme_item.h                                         *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe dockbar item                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_ITEM_H_
#define _MMIQBTHEME_ITEM_H_ 


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//  Description : item type defination
typedef enum
{
    MMIQBTHEME_ITEM_NONE,
    MMIQBTHEME_ITEM_NULL,       //ʲô����Ҳ���ǣ�ռλ����
    MMIQBTHEME_ITEM_WIDGET,     //widget ��ʱ��û��
    MMIQBTHEME_ITEM_FUNC,       //dockbar��һҳ�еĹ��� �������˵� ����widgetҳ��
    MMIQBTHEME_ITEM_MENU,       //���˵��е�menu
    MMIQBTHEME_ITEM_WIDGET_PAGE_PREVIEW_ICON    //widgetҳ������ͼ    
}MMIQBTHEME_ITEM_TPYE_E;

//  Description : item id union defination
typedef union
{
    int32 widget_id;            //widget id
    int32 qbtheme_func_id ;     //dockbar����Ĺ��ܣ���������widget���������˵�������һҳwidgetҳ��
    MMIMAINMENU_ITEM_INFO_T  menu_info    ; //mainmenu
    int32 widget_page_index;                //widget page preview icon
}MMIQBTHEME_ITEM_ID_U;

//  Description : item struct defination
typedef struct
{
    MMIQBTHEME_ITEM_TPYE_E  type;             
    MMIQBTHEME_ITEM_ID_U    id;    
    int32                   item_index;       //item��page�еı��
    int32                   grid_index;       //��Ӧ��grid�и��ӵı��
    MMIQBTHEME_FSM_STATE_E fsm_state;         //normal editing  item_moving  page_sliding
    MMIQBTHEME_DISPLAY_T    display;
    int32                focus_flag;            //0: not focused 1 focused
    MMI_MESSAGE_ID_E    tp_state ;             //tp down/up��
    GUI_POINT_T         prevous_tp_point ;      //��һ��TP��Ϣ������    
}MMIQBTHEME_ITEM_T;

//item data list
typedef struct MMIQBTHEME_ITEM_LIST
{
    MMIQBTHEME_ITEM_T *p_item;
    struct MMIQBTHEME_ITEM_LIST *p_next_list_item;
}MMIQBTHEME_ITEM_LIST_T;

//item ����ʾͼ�������Լ�����ִ�в����Ľṹ�嶨��
typedef struct 
{    
    MMI_TEXT_ID_T    text;     //[in] the label 
    MMI_IMAGE_ID_T icon;
    void (*func)()      ;       //���ܺ������
}MMIQBTHEME_ITEM_FUNCTION_DATA;    



/*****************************************************************************/
//  Description : ���ó�Ա������ֵ�������ڴ�
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIQBTHEME_ItemCreate(MMIQBTHEME_ITEM_T   ** p_item, MMIQBTHEME_ITEM_TPYE_E type);

/*****************************************************************************/
//  Description : destroy an  item objcet
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIQBTHEME_ItemDestroy(MMIQBTHEME_ITEM_T   *p_item);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: ying.xu
//  Note: 
/*****************************************************************************/
PUBLIC int32  MMIQBTHEME_ItemSetFSMState(MMIQBTHEME_ITEM_T* p_item,
                                        MMIQBTHEME_FSM_STATE_E      fsm_state
                                        );

//get״̬��״̬
PUBLIC MMIQBTHEME_FSM_STATE_E  MMIQBTHEME_ItemGetFSMState(MMIQBTHEME_ITEM_T* p_item );
 

//set display object
PUBLIC int32 MMIQBTHEME_ItemSetDisplayObject(MMIQBTHEME_ITEM_T   *p_item , 
                                        MMIQBTHEME_DISPLAY_T      *display
                                        ); 

//���ڴ�����
PUBLIC MMI_RESULT_E MMIQBTHEME_ItemHandleMsg(MMIQBTHEME_ITEM_T   *p_item,
                                            MMI_WIN_ID_T      win_id,
                                            MMI_MESSAGE_ID_E  msg_id,
                                            DPARAM            param
                                              );

//delete itsself
PUBLIC int32 MMIQBTHEME_ItemDelete(MMIQBTHEME_ITEM_T   *p_item);

//draw item
PUBLIC int32 MMIQBTHEME_ItemDraw(MMIQBTHEME_ITEM_T *p_item);

//item moving ,�ṩ�����ԭʼλ�õ�ƫ��ֵ,only change display rect
PUBLIC int32 MMIQBTHEME_ItemMove(MMIQBTHEME_ITEM_T   *p_item , 
                                 int32 offset_x,
                                 int32 offset_y

                                              );

PUBLIC int32 MMIQBTHEME_ItemSetDisplayRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T rect);


//return moving item current position
PUBLIC int32 MMIQBTHEME_ItemGetDisplayRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T *rect);

//return  item  position
PUBLIC int32 MMIQBTHEME_ItemGetLayoutRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T *rect);

//set layout and display rect
PUBLIC int32 MMIQBTHEME_ItemSetRect(MMIQBTHEME_ITEM_T *p_item, GUI_RECT_T rect);

//set item focused state
PUBLIC int32 MMIQBTHEME_ItemSetFocus(MMIQBTHEME_ITEM_T *p_item, int32 fucous_flag);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
