/*****************************************************************************
** File Name:      mmiapwin_menu.h                                           *
** Author:                                                                   *
** Date:           13/08/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to ap win                               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       robert.wang       Create                                    *
******************************************************************************/
#ifndef _MMIAPWIN_MENU_H_
#define _MMIAPWIN_MENU_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"

#include "mmi_slist.h"

#include "mmimp3_menutable.h"

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
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    MMIMP3_ID_MENU_E menu_id; //menu id
    MMI_TEXT_ID_T text_id; //插入的节点的文本
    MMI_IMAGE_ID_T image_id; //图片id
} MMIAP_MENU_NODE_T;


typedef BOOLEAN (*GETNODECONTEXT_CALLBACK)(MMIMP3_ID_MENU_E menu_id, /*in*/\
MMIAP_MENU_NODE_T *node_out_ptr /*out*/ \
); 

typedef struct
{
    MMIMP3_ID_MENU_E menu_id; //menu id
    MMIMP3_ID_MENU_E parent_id; //menu id
} MMIAP_MENU_ITEM_T;


typedef struct
{
    BOOLEAN is_gray;
    MMIAP_MENU_ITEM_T menu_item; //menu id
} MMIAP_DYN_MENU_GRAY_T;

typedef BOOLEAN (*MENU_GRAY_CALLBACK)(MMIMP3_ID_MENU_E menu_id, /*in*/\
BOOLEAN *is_gray_out_ptr /*out*/ \
); 

typedef struct
{
    MMIAP_MENU_ITEM_T *menu_group_ptr;
    uint32 total_num;
    BOOLEAN is_gray;    
    MMI_CTRL_ID_T ctrl_id;
    MENU_GRAY_CALLBACK gray_func;
} MMIAP_STATIC_MENU_GRAY_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获得节点ID对应的节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetNodeContext(
    MMIMP3_ID_MENU_E menu_id, //in
    MMIAP_MENU_NODE_T *node_out_ptr, //out
    uint32 which_value//in 相同ID的 哪个内容
);

/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAP_OpenPlayerOptMenu(
    MMI_WIN_ID_T dst_win_id
);

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InsertMp3NodeByLabel(
    MMI_CTRL_ID_T      ctrl_id,        //控件id
    MMI_IMAGE_ID_T     image_id,       //图片id, for PDA style pop menu
    MMI_TEXT_ID_T      text_id,        //插入的节点的文本
    uint32             node_id,        //节点编号
    uint32             parent_node_id, //父节点编号
    uint16             index           //位置
);

/*****************************************************************************/
//  Description : build menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildMenuToCtrl(
    MMIAP_MENU_ITEM_T *menu_group_ptr, //in
    uint32 total_num,//in
    GETNODECONTEXT_CALLBACK func_ptr, //in
    BOOLEAN is_display_image, //in
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description :  build dynamic menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_SLISTHANDLE_T MMIAP_CreateDynamicMenu(
    uint32 element_size //in
);

/*****************************************************************************/
//  Description :  build dynamic menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyDynamicMenu(
    MMI_SLISTHANDLE_T handle //in
);

/*****************************************************************************/
//  Description :  add item to dynamic menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_AddDynamicMenuItem(
    MMI_SLISTHANDLE_T handle, //in
    MMIAP_MENU_ITEM_T *item_ptr //in
);

/*****************************************************************************/
//  Description :  build dynamic menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildDynamicMenuToCtrl(
    MMI_SLISTHANDLE_T menu_group_handle, //in
    GETNODECONTEXT_CALLBACK func_ptr, //in
    BOOLEAN is_display_image, //in
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description :  build dyn menu gray by ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildDynMenuGrayByCtrl(
    MMI_SLISTHANDLE_T menu_group_handle, //in
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description :  build dyn menu gray by ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildMenuGrayByCtrl(
    MMIAP_STATIC_MENU_GRAY_PARAM_T *menu_gray_param_ptr //in
);

/*****************************************************************************/
//  Description :  add item to dynamic menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_AddDynMenuGrayItem(
    MMI_SLISTHANDLE_T handle, //in
    MMIAP_DYN_MENU_GRAY_T *item_ptr //in
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif



