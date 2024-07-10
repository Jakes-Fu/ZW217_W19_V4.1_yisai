/*****************************************************************************
** File Name:      mmiqbtheme_virtualwin.h                                   *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe dockbar virtualwin          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2011     ying.xu              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifndef _MMIQBTHEME_VIRTUALWIN_H_
#define _MMIQBTHEME_VIRTUALWIN_H_ 

//VIRTUAL WIN type 
typedef enum
{
    MMIQBTHEME_VIRTUALWIN_WIDGET,
    MMIQBTHEME_VIRTUALWIN_DOCKBAR,
}MMIQBTHEME_VIRTUALWIN_TYPE_E;


//ǧ����������ⴰ�ڵ�title
typedef struct
{
    GUI_RECT_T     rect ;                 
    int32          current_page;    
    int32          page_num;
}MMIQBTHEME_VIRTUALWIN_TITLE_T;


//ǧ�������widget ��dockbar ���ڶ���������
typedef struct
{    
    MMIQBTHEME_VIRTUALWIN_TYPE_E    type ;     //widget/ dockbar
    //��̬�����ڴ棬����ÿһҳ������ҳ��˳��ֻ��Ҫ����page[MAX]�е�ָ�롣
    MMIQBTHEME_PAGE_T*              p_page[MMIQBTHEME_MAX_WIDGET_PAGE_NUM]; 
    int32                           current_page;    
    int32                           page_num;
    MMIQBTHEME_FSM_STATE_E          fsm_state;    //normal editing  item_moving  page_sliding
    MMI_MESSAGE_ID_E                tp_state ;    //tp down up��
    MMIQBTHEME_DISPLAY_T            display;    //normal ����µ���ʾ����
    GUI_POINT_T                     prevous_tp_point ;      //��һ��TP��Ϣ������    
}MMIQBTHEME_VIRTUALWIN_T;


//create an virtual win objcet 
//�����ڴ�,����ʼֵ
PUBLIC int32 MMIQBTHEME_VirtualWinCreate(MMIQBTHEME_VIRTUALWIN_T** p_virtualwin,
                                            MMIQBTHEME_VIRTUALWIN_TYPE_E type      //widget/ dockbar
                                            );


//destroy an virtual win objcet
PUBLIC int32 MMIQBTHEME_VirtualWinDestroy(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin);



//���ڴ�����
PUBLIC MMI_RESULT_E MMIQBTHEME_VirtualWinHandleMsg(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                            MMI_WIN_ID_T      win_id,
                                            MMI_MESSAGE_ID_E  msg_id,
                                            DPARAM            param
                                              );

//set״̬��״̬
PUBLIC int32  MMIQBTHEME_VirtualWinSetFSMState(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                             MMIQBTHEME_FSM_STATE_E      fsm_state
                                              );

//get״̬��״̬
PUBLIC MMIQBTHEME_FSM_STATE_E  MMIQBTHEME_VirtualWinGetFSMState(
                                MMIQBTHEME_VIRTUALWIN_T* p_virtualwin
                                );
                                              
//get  page num
PUBLIC int32  MMIQBTHEME_VirtualWinGetPageNum(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin
                                              );

//add one page in virtual win                       
PUBLIC int32  MMIQBTHEME_VirtualWinAddPage(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                          MMIQBTHEME_PAGE_T* p_page
                                              );                                              


//set display object
PUBLIC int32  MMIQBTHEME_VirtualWinSetDisplayObject(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,                                            
                                              MMIQBTHEME_DISPLAY_T     *display
                                              );

//set current page
PUBLIC int32  MMIQBTHEME_VirtualWinSetCurrentPage(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                            int32 page_index);

//get display rect,                                            
PUBLIC void  MMIQBTHEME_VirtualWinGetRect(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin,
                                            GUI_RECT_T *rect
                                              );

//refresh win
PUBLIC int32 MMIQBTHEME_VirtualWinDraw(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin);

//refresh win background
PUBLIC int32 MMIQBTHEME_VirtualWinDrawBackground(MMIQBTHEME_VIRTUALWIN_T* p_virtualwin);


#endif  
