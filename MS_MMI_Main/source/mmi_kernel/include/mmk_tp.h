/****************************************************************************
** File Name:      mmk_tp.h                                                *
** Author:                                                                 *
** Date:           03/14/2007                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the message id of key     *
**                  and define the function of key                         *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2007       Liqing.peng         Create								   *
**																		   *
****************************************************************************/

#ifndef  _MMK_TP_H_    
#define _MMK_TP_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_signal.h"
#include "mmi_osbridge.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
//#if (defined TOUCHPANEL_TYPE_MULTITP \
//     || (defined TOUCHPANEL_TYPE_HW && defined TPC_TYPE_DOUBLE))
//#define TOUCH_GESTURE_ENABLE
//#endif

#define MMI_TP_POINTS_MAX 5

//多点触摸数据
typedef struct
{
    GUI_POINT_T points[MMI_TP_POINTS_MAX];
    uint16      num;
} MMI_TP_POINTS_T;

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/
/******************************************************************************/
/*! \brief check the TP single because of the system queue full,
 *			call by TPCallBackFunc
 *  \param[in]
 *  \param[in]
 *  \author  liqing.peng
 *  \return 
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_CheckTPSignale(
                                  uint16 state,  //[IN] TP_DOWN or TP_UP
                                  uint32 x,
                                  uint32 y
                                  );

/******************************************************************************/
/*! \brief get multi tp message
 *  \param[in]		tp_status_ptr	pointer to touch panel status
 *  \param[in]		point_ptr		pointer to touch panel point
 *  \author  bruce.chi
 *  \return 
 *		- <i><c>message number</c></i>
 */
/******************************************************************************/
PUBLIC int MMK_GetMultiTPMsg(MMI_TP_STATUS_E *tp_status_ptr, GUI_POINT_T *point_ptr);


/******************************************************************************/
/*! \brief get multi tp message
 *  \param[in]		tp_status_ptr	pointer to touch panel status
 *  \param[in]		point_ptr		pointer to touch panel point
 *  \author  bruce.chi
 *  \return 
 *		- <i><c>message number</c></i>
 */
/******************************************************************************/
PUBLIC void MMK_GetLogicTPMsg(MMI_TP_STATUS_E *tp_status_ptr, GUI_POINT_T *point_ptr);

/*****************************************************************************/
// 	Description : check tp status, whether is tp long now
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
/*PUBLIC BOOLEAN MMK_IsTPLongStat(void);*/

/*****************************************************************************/
// 	Description : is exist tp move
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsExistTpMove( void );

/******************************************************************************/
/*! \brief			quick tp, tp is nearly full
 *  \author			James.Zhang
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsQuickTp(void);

/*****************************************************************************/
//     Description : 获得多点信息
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetTpPoints( MMI_TP_POINTS_T* points_ptr );

/*****************************************************************************/
// Description : 获取硬图标快捷菜单数目
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC uint16 MMK_GetShortcutIconNum(void);

/*****************************************************************************/
// Description : point是否在有效的硬图标区
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsPointInShortcutIconRect(GUI_POINT_T point, uint32 icon_index);

/*****************************************************************************/
// Description : 获取当前硬图标要虚拟的物理键值
// Global resource dependence : 
// Author:xin.li
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetShortcutIconVirtualKeycode(uint32 icon_index,     //in
                                                 uint32 *keycode_ptr    //out
                                                 );

/*****************************************************************************/
// Description : 是否正在校准触摸屏
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsCoordinatingTp(void);

/*****************************************************************************/
// Description : 设置正在校准触摸屏标志
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC void MMK_SetCoordinateTpFlag(BOOLEAN is_coordinating);
/*****************************************************************************/
//     Description : delay tp move
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMK_DelayTpMove( 
                            BOOLEAN is_delay
                            );

/*****************************************************************************/
//     Description : tp init
//    Global resource dependence : 
//  Author:Cheng.luo
//    Note:
/*****************************************************************************/
PUBLIC void MMK_InitTp(void);

/*****************************************************************************/
// 	Description : set TP down win
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetTPDownWin(
                             MMI_HANDLE_T  win_handle
                             );

/*****************************************************************************/
// 	Description : get TP down win
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetTPDownWin( void );

/*****************************************************************************/
//  Description : reset tp
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMK_ResetTp(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_TP_H_  */
