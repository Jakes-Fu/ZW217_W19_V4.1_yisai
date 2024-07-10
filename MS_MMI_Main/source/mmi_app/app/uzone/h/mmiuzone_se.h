/*****************************************************************************
** File Name:      mmiuzone_se.h                                       *
** Author:                                                                   *
** Date:           11/1/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to  uzone se                      *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011    Jessica                Create
******************************************************************************/

#ifndef _MMIUZONE_SE_H_
#define _MMIUZONE_SE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef void (*SE_ITEM_CALLBACK)(uint16 item_index);


typedef void (*SE_DRAWITEM_CALLBACK)(uint16 item_index);



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set dest layer handle
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetDestLayerHandle(GUI_LCD_DEV_INFO* des_layer);


/*****************************************************************************/
// Description : 告知算法需要的源BUF
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/

PUBLIC void SEAPI_SetSrcLayerHandle(GUI_LCD_DEV_INFO* page_src_layer,
                                            GUI_LCD_DEV_INFO* icon_src_layer,
                                            GUI_LCD_DEV_INFO* str_src_layer,
                                            GUI_LCD_DEV_INFO* str_shadow_layer
                                            );

/*****************************************************************************/
//  Description : set star layers handle
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetStarLayerHandle(GUI_LCD_DEV_INFO* big_star_layer, 
                                                GUI_LCD_DEV_INFO* small_star_layer);


/*****************************************************************************/
//  Description : set focus item layers handle
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetFocusItemLayerHandle(GUI_LCD_DEV_INFO* focus_item_layer);

/*****************************************************************************/
//  Description : set draw item callback
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetDrawItemCb(SE_DRAWITEM_CALLBACK cb); 



/*****************************************************************************/
//  Description : set total item number
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetItemNumber(uint16 item_number);


/*****************************************************************************/
//  Description : set current item index
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetCurItemIndex(uint16 item_index);


/*****************************************************************************/
//  Description : get total item number
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 SEAPI_GetCurItemIndex(void);

/*****************************************************************************/
//  Description : set last item index
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetLastItemIndex(uint16 item_index);


/*****************************************************************************/
//  Description : get last item number
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 SEAPI_GetLastItemIndex();

/*****************************************************************************/
//  Description : set callback function for one item is clicked
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SEAPI_SetItemCallback(SE_ITEM_CALLBACK cb);


/*****************************************************************************/
// Description : SEAPI_CreateShadow
// Global resource dependence : 
// Author: kyle.jin
// Note: 
/*****************************************************************************/
PUBLIC void SEAPI_CreateShadow(uint8* src_ptr, 
                                        uint8* des_ptr,
                                        uint32 *icon_buff_array,
                                        GUI_RECT_T *icon_rect_ptr,
                                        uint32 *str_buff_array,
                                        GUI_RECT_T *str_rect_ptr,
                                        uint32 *str_shadow_buff_array
                                        );


/*****************************************************************************/
//  Description : se handle message function
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E SEAPI_HandleMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              );



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif



#endif // _MMIUZONE_SE_H_ 
