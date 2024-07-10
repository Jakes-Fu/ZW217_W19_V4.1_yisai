/*****************************************************************************
** File Name:      mmivt_menutable.h                                            *
** Author:         samboo.shen                                               *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to fm menu                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/05/17     liqing.peng      Create                                    *
******************************************************************************/

#ifndef _VT_MENUTABLE_H_
#define _VT_MENUTABLE_H_

#ifdef VT_SUPPORT

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/


//Menu ID
typedef enum
{
    VT_ID_MENU_START = (MMI_MODULE_VT << 16),

//@xiongxiaoyan 可视电话设置begin
    ID_SET_VIDEO_FORMAT,
    ID_SET_VIDEO_MPG4,
    ID_SET_VIDEO_H263,
    ID_SET_H263_SIZE,
    ID_SET_H263_QCIF,
    ID_SET_H263_SQCIF,

    ID_SET_MY_RATE,
    ID_SET_REMOTE_RATE,
    ID_SET_SWITCH_TO_NORMAL,
//@xiongxiaoyan 可视电话设置end
    ID_SET_VIDEO_DAYNIGHTMODE,
	ID_SET_VIDEO_FALLBACK,
//@zhaohui add	
    ID_SET_REPLACE_PIC,//无条件替代画面
    ID_SET_DEFAULT_PIC,//缺省画面
    ID_SET_SELECT_PIC,//选择图片

   
    VT_ID_MENU_MAX
}VT_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    VT_MENU_LABEL_START = (MMI_MODULE_VT << 16),
    #include "mmivt_menutable.def"
    VT_MENUTABLE_MAX
} VT_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif	//#ifdef VT_SUPPORT

#endif /* MENUTABLE_H */

