/*****************************************************************************
** File Name:      mmiqbtheme_dockbar.h                                          *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_DOCKBAR_H_
#define _MMIQBTHEME_DOCKBAR_H_ 

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


//״��ת�����飬[old_state][input_msg] = [new_state]

PUBLIC MMIQBTHEME_FSM_STATE_E MMIQBTHEME_FSMTranfer( MMIQBTHEME_FSM_STATE_E *array,
                                                     MMIQBTHEME_FSM_STATE_E old_state,
                                                     MMI_MESSAGE_ID_E  msg_id                                                     
                                                            );

//��ʾ������غ���
PUBLIC void MMIQBTHEME_CreateDisplay( MMIQBTHEME_DISPLAY_T  *display);
PUBLIC void MMIQBTHEME_CreateDisplayEx( MMIQBTHEME_DISPLAY_T  *display);
PUBLIC void MMIQBTHEME_DestroyDisplay( MMIQBTHEME_DISPLAY_T  *display);
PUBLIC void MMIQBTHEME_AppendDisplay( MMIQBTHEME_DISPLAY_T  *display);
PUBLIC void MMIQBTHEME_RemoveDisplay( MMIQBTHEME_DISPLAY_T  *display);


PUBLIC int32 MMIQBTHEME_SetBufferAlpha(GUI_LCD_DEV_INFO *layer_ptr, GUI_RECT_T *rect_ptr, uint32 alpha);

//input start_rect -> end_rect
// input/output step_x/step_y //xy�����ϵ�Ĭ�ϲ���
//output frame      ���������֡��
PUBLIC int32 MMIQBTHEME_CalculateMovingParameter( int32 *step_x , int32 *step_y, int32 *frame  ,
                                        GUI_RECT_T start_rect, GUI_RECT_T end_rect);
//reset factory settings
PUBLIC int32 MMIQBTHEME_ResetDockbarPage2Data(void);

//save dockbar page2 data
PUBLIC int32 MMIQBTHEME_SaveDockbarPage2Data(MMIQBTHEME_PAGE_T *p_page);

/*****************************************************************************/
//  Description : re-initial grid idle dockbar page2
//  Global resource dependence : 
//  Author: YING.XU
//  Note:
/*****************************************************************************/
PUBLIC void MMIQBTHEME_InitDockbarPage2(void);


//��ʼ��dockbar 
PUBLIC void MMIQBTHEME_InitDockbar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

//get a widget page preview icon in dockbar page3
PUBLIC int32 MMIQBTHEME_GetWidgetPagePreviewItemNum(void) ;                      

//��dockbar��widgetԤ��ҳ������һ��item��ͬʱwidgetҳ����һҳ
PUBLIC int32 MMIQBTHEME_AddWidgetPagePreviewItem(void);                       
PUBLIC int32 MMIQBTHEME_AddWidgetPagePreviewItemEx(void);                       

//���dockbar�����ⴰ�ڵ�ָ��
PUBLIC MMIQBTHEME_VIRTUALWIN_T ** MMIQBTHEME_GetDockbarPtrPtr(void);


PUBLIC MMIQBTHEME_VIRTUALWIN_T * MMIQBTHEME_GetDockbarPtr(void);

PUBLIC MMI_RESULT_E MMIQBTHEME_HandleDockbarMsg(                                
                                        MMI_IDLE_GRID_DATA_T        *idle_grid_ptr ,
                                        MMI_MESSAGE_ID_E  msg_id,
                                        DPARAM            param
                                        );
//add one menu icon in dockbar page2
PUBLIC int32 MMIQBTHEME_InsertMenuIconItem(
                    int32 item_position, 
                    MMIMAINMENU_ITEM_INFO_T menu_id
                                                );

//set  focued 
PUBLIC int32 MMIQBTHEME_SetWidgetPreviewPageFocusedItem(int32 item_index);  

//�������������ڵ�item��ʹ���ܹ���ʾ����
PUBLIC int32  MMIQBTHEME_SlideWidgetPreviewPage2FocusedItem(void);

//delete one item of dockbar 
PUBLIC int32 MMIQBTHEME_DeleteItem(
                                    MMIQBTHEME_ITEM_T *p_item 
                                                );
                                                
PUBLIC int32 MMIQBTHEME_OpenDeletingItemQueryWin(
                                    MMIQBTHEME_ITEM_T *p_item 
                                                );
                                                
PUBLIC GUI_RECT_T MMIQBIDLE_GetDockbarRect(
                                MMI_IDLE_GRID_DATA_T *idle_grid_ptr                       
                                );
                                
//move menu icon from dockbar page2 to widget win
PUBLIC int32 MMIQBTHEME_MoveMenu2WidgetWin(MMIQBTHEME_PAGE_T *p_page, 
                                            int32 index, 
                                            GUI_POINT_T point);


                                            



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
