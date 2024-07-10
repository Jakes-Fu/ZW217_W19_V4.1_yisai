/*****************************************************************************
** File Name:      mmiqbtheme_page.h                                         *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe dockbar page                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_PAGE_H_
#define _MMIQBTHEME_PAGE_H_ 


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//qbtheme page type 
typedef enum
{
    MMIQBTHEME_PAGE_WIDGET,    
    MMIQBTHEME_PAGE_DOCKBAR,       
    MMIQBTHEME_PAGE_DOCKBAR_PAGE1,  //page 1 wallpaper adding shortcut widget
    MMIQBTHEME_PAGE_DOCKBAR_PAGE2,  //page 2 mianmenu 
    MMIQBTHEME_PAGE_DOCKBAR_PAGE3,  //page 3 preview icon

    //page type 主要决定了空间分配方式的不同
    //page2 每增加删除一个元素，grid都需要重新设定格子个数,分配计算
    //而page3 预先申请好所有元素的格子，每一个item的位置都预留好，增加删除只需要顺移即可
    
}MMIQBTHEME_PAGE_TYPE_E;

//page data 
typedef struct
{
    MMIQBTHEME_PAGE_TYPE_E type ;     //widget/ dockbar    
    MMIQBTHEME_DISPLAY_T    display;
    MMIQBTHEME_FSM_STATE_E  fsm_state;        //normal editing  item_moving  page_sliding
    MMI_MESSAGE_ID_E        tp_state ;             //tp down up…
    GUI_POINT_T             prevous_tp_point ;      //上一次TP消息的坐标    
    int32                  page_index;  //page index in virtual win
    MMIQBTHEME_GRID_T      *p_grid;      // rect区域的格子分配情况
    MMIQBTHEME_ITEM_LIST_T *p_special_item_head;   // page页内特殊元素的链表，比如dockbar第三页的增加一个的icon
    MMIQBTHEME_ITEM_T       *p_item[MMIQBTHEME_WIDGET_PAGE_MAX_ITEM];
    int32                   item_num;
    int32                   focused_item_index;  //item index focused
}MMIQBTHEME_PAGE_T;


//create an virtual win objcet 
//设置成员变量的值，申请内存
PUBLIC int32 MMIQBTHEME_PageCreate(MMIQBTHEME_PAGE_T   ** p_page, MMIQBTHEME_PAGE_TYPE_E type);

//destroy an virtual win objcet
PUBLIC int32 MMIQBTHEME_PageDestroy(MMIQBTHEME_PAGE_T* p_page);



//窗口处理函数
PUBLIC MMI_RESULT_E MMIQBTHEME_PageHandleMsg(MMIQBTHEME_PAGE_T* p_page,
                                            MMI_WIN_ID_T      win_id,
                                            MMI_MESSAGE_ID_E  msg_id,
                                            DPARAM            param
                                              );

//set状态机状态
PUBLIC int32  MMIQBTHEME_PageSetFSMState(MMIQBTHEME_PAGE_T* p_page,
                                             MMIQBTHEME_FSM_STATE_E      fsm_state
                                              );

//get状态机状态
PUBLIC MMIQBTHEME_FSM_STATE_E  MMIQBTHEME_PageGetFSMState(MMIQBTHEME_PAGE_T* p_page);

//set display 
PUBLIC int32  MMIQBTHEME_PageSetDisplayObject(MMIQBTHEME_PAGE_T* p_page,
                                            MMIQBTHEME_DISPLAY_T      *display
                                              );
//add one item in item array
PUBLIC int32  MMIQBTHEME_PageAddItem(MMIQBTHEME_PAGE_T* p_page,
                                    MMIQBTHEME_ITEM_T *p_item
                                    );
                                    
//add one item in special item list,for example, dockbar page3,item 6
PUBLIC int32  MMIQBTHEME_PageAddSpecialItem(MMIQBTHEME_PAGE_T* p_page,
                                    MMIQBTHEME_ITEM_T *p_item
                                    );                

//delete one item in item array
PUBLIC int32  MMIQBTHEME_PageDeleteItem(MMIQBTHEME_PAGE_T* p_page,
                                        int32 item_index 
                                        )      ;                              
                                    
//insert one item in item array, and items after item_index will be moved to the followed position
PUBLIC int32  MMIQBTHEME_PageInsertItem(MMIQBTHEME_PAGE_T* p_page,
                                    MMIQBTHEME_ITEM_T *p_item,                                    
                                    int32  item_position
                                    )  ;             

//delete all items in one page
PUBLIC int32  MMIQBTHEME_PageDeleteAllItems(MMIQBTHEME_PAGE_T* p_page);

//set focus item
PUBLIC int32  MMIQBTHEME_PageSetFocusItem(MMIQBTHEME_PAGE_T* p_page,
                                    int32  item_index
                                    )           ;

//slide page to  focus item position
PUBLIC int32  MMIQBTHEME_PageSlide2FocusedItem(MMIQBTHEME_PAGE_T* p_page);
                                    

PUBLIC int32 MMIQBTHEME_PageDraw(MMIQBTHEME_PAGE_T* p_page );

PUBLIC int32 MMIQBTHEME_PageSetRectByOffset(MMIQBTHEME_PAGE_T* p_page ,int32 offset_x, int32 offset_y);
PUBLIC int32 MMIQBTHEME_PageSetRect(MMIQBTHEME_PAGE_T* p_page ,GUI_RECT_T   rect);

//delete all items in one page
PUBLIC int32  MMIQBTHEME_PageDeleteAllItemsInPage(MMIQBTHEME_PAGE_T* p_page);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
