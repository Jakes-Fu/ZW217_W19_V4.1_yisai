/*************************************************************************
 ** File Name:      tp_nv.c                                             *
 ** Author:         wancan.you                                           *
 ** Date:           11/21/2005                                           *
 ** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 11/21/2005     wancan.you        Create.                              *
*************************************************************************/
//#include "std_header.h"
//#include "sci_types.h"
#include "tp_internal.h"
#ifdef TOUCH_PANEL_SUPPORT

//#include "tpd_drv.h"
//#include "mmi_module.h"
//#include "mmi_modu_main.h"


/*****************************************************************************/
// 	Description : �Ƿ�ʱ��һ�ο���
//	Global resource dependence : 
//  Author: Robert
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITP_IsPowerOnFirst(void)
{
    return TP_IsPowerOnFirst();
}

/*****************************************************************************/
// 	Description : coordinate window open
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPITP_COORDINATE_OpenWin(BOOLEAN is_first)
{
    COORDINATE_OpenWin(is_first);
}

/*****************************************************************************/
// 	Description : ����ʱ��ʼ������������
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPITP_COORDINATE_Init(void)
{
    COORDINATE_Init();
}

/*****************************************************************************/
// 	Description : intial tp module  
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITP_InitModule(void)
{
    //TP_RegMenuGroup();  //��ʼ��tp menu
    TP_RegTPNv();   //��ʼ��im nv
    TP_RegWinIdNameArr();
}

#else       /*--------------dummy---------------*/

/*****************************************************************************/
// 	Description : �Ƿ�ʱ��һ�ο���
//	Global resource dependence : 
//  Author: Robert
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITP_IsPowerOnFirst(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : coordinate window open
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPITP_COORDINATE_OpenWin(BOOLEAN is_first)
{
}

/*****************************************************************************/
// 	Description : ����ʱ��ʼ������������
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPITP_COORDINATE_Init(void)
{
}

/*****************************************************************************/
// 	Description : intial tp module  
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITP_InitModule(void)
{
}
/*****************************************************************************/
//  Description : is coordinate win open
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITP_Coordinate_IsWinOpen(void)
{
    return FALSE;
}
#endif

