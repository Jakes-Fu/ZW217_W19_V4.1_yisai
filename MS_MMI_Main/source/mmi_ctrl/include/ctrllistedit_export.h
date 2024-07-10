/*****************************************************************************
** File Name:      ctrllistedit_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLLISTEDIT_EXPORT_H_
#define _CTRLLISTEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex_export.h"

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

/*! @enum GUIEDIT_LIST_STYLE_E
@brief �б�༭������
*/
typedef enum
{
    GUIEDIT_LIST_STYLE_NULL,        /*!<����λ*/
    GUIEDIT_LIST_STYLE_PHONENUM,    /*!<�绰�����б�*/
    GUIEDIT_LIST_STYLE_EMAIL,       /*!<E-mail�б�*/
    GUIEDIT_LIST_STYLE_ALL,         /*!<�绰�����E-mail����б�*/
    GUIEDIT_LIST_STYLE_MAX          /*!<����λ*/
} GUIEDIT_LIST_STYLE_E;

/*! @struct GUIEDIT_LIST_ITEM_STR_T
@brief �б�༭��item�ַ���
*/
typedef struct
{
    BOOLEAN     is_valid;   /*!<item�Ƿ���Ч*/
    wchar       *str_ptr;   /*!<�ַ�������*/
    uint16      str_len;    /*!<�ַ�������*/
} GUIEDIT_LIST_ITEM_STR_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit list item number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLLISTEDIT_GetListItemNum (
    MMI_CTRL_ID_T  ctrl_id //in:
);

/*****************************************************************************/
//  Description : get edit list item string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
//    when item object, return the user data.
/*****************************************************************************/
PUBLIC void CTRLLISTEDIT_GetListItemString (
    MMI_CTRL_ID_T             ctrl_id,        //in:
    uint16                    array_len,      //in
    GUIEDIT_LIST_ITEM_STR_T   *item_array_ptr //in/out:item string array
);

/*****************************************************************************/
//  Description : add edit list item string
//  Global resource dependence :
//  Author: Jassmine
//  Note:�������Χ����н�ȡ,Listֻ��һ��Itemһ��Item����
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_AddListItem (
    MMI_CTRL_ID_T    ctrl_id,    //in:
    wchar            *str_ptr,   //in:
    uint16           str_len     //in:
);

/*****************************************************************************/
//  Description : ����ı�����
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_AddListItemArray (
    MMI_CTRL_ID_T       ctrl_id,
    GUIEDIT_LIST_ITEM_T *list_item_ptr,
    uint16              list_item_count
);

/*****************************************************************************/
//  Description : get edit list item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLISTEDIT_GetListItemArray (
    MMI_CTRL_ID_T             ctrl_id,        //in:
    uint16                    array_len,      //in
    GUIEDIT_LIST_ITEM_T       *item_array_ptr //out
);

/*****************************************************************************/
//  Description : replace current item
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// this method is just for list style
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_ReplaceCurListItem (
    MMI_CTRL_ID_T         ctrl_id,        //in:
    GUIEDIT_LIST_ITEM_T   *list_item_ptr, //in:
    uint16                list_item_count
);

/*****************************************************************************/
//  Description : set edit max length for numlist style
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLISTEDIT_SetMaxListItemNum (
    MMI_CTRL_ID_T      ctrl_id,
    uint16             max_num,
    uint16             item_max_len
);

/*****************************************************************************/
//  Description : set list style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLISTEDIT_SetListStyle (
    MMI_CTRL_ID_T          ctrl_id,    //in
    GUIEDIT_LIST_STYLE_E   list_style  //in
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
