/*****************************************************************************
** File Name:      mmitv_out.h		                                         *
** Author:		   haiyanghu                                                 *
** Date:           21/06/2006                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe tv-out interface			 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2006		  haiyanghu        Create
******************************************************************************/

#ifndef _TV_OUT_H_
#define _TV_OUT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmi_id.h"
#include "mmi_position.h"

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
#define MMITVOUT_WIDTH				MMI_MAINSCREEN_WIDTH    //visible width in TV
#define MMITVOUT_HEIGHT				MMI_MAINSCREEN_HEIGHT   //visible height in TV
#define MMITVOUT_RESOLUTION			2

#define MMITVOUT_PAL_WIDTH			(720/2)
#define MMITVOUT_PAL_HEIGHT			(576/2)
#define MMITVOUT_NTSC_WIDTH			(720/2)
#define MMITVOUT_NTSC_HEIGHT		(480/2)

#define MMITVOUT_CFG_TYPE			1			//cfg paramater
#define MMITVOUT_CFG_PRIORITY		1			//
#define MMITVOUT_CFG_ALPHA			1			//

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 
//the type of tv out mode
typedef enum
{
	MMICONNECTION_TV_SET_PAL_LINE_BY_LINE = 0,		//PAL line by line mode
	MMICONNECTION_TV_SET_NTSC_LINE_BY_LINE,			//NTSC line by line mode
	MMICONNECTION_TV_SET_PAL_INTERLEAVING_LINE,       //PAL interleaving line mode
	MMICONNECTION_TV_SET_NTSC_INTERLEAVING_LINE,		//NTSC interleaving line mode
	MMICONNECTION_TV_SET_TV_OUT_MODE_MAX_NUM
}MMICONNECTION_TV_SET_TYPE_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get tv out starting status
//	Global resource dependence : 
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_GetStartingStatus(void);


/*****************************************************************************/
// 	Description : open tv out window and start tv out
//	Global resource dependence : none
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_OpenWin(void);

/*****************************************************************************/
// 	Description : open tv out setting window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUTSetting_OpenWin(void);

/*****************************************************************************/
// 	Description : close tv out
//	Global resource dependence :
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITVOUT_Close(void);
/*****************************************************************************/
// 	Description : get the tv out size
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_GetTVOutShowRect(GUI_RECT_T * show_rect_ptr);


/*****************************************************************************/
// 	Description : start tv out
//	Global resource dependence : 
//  Author: haiyanghu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_StartTvOut(void);
/*****************************************************************************/
// 	Description : set the tv out size
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITVOUT_SetTVOutShowRect(GUI_RECT_T * show_rect_ptr);

/*****************************************************************************/
// 	Description : init tv out
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPITVOUT_Init(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif
