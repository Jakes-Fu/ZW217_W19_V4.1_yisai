/*****************************************************************************
** File Name:      mmiwidget_aclock_se.c                                       *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/7/11      Juan.wu          Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifndef _MMIWIDGET_ACOLCK_SE_
#define _MMIWIDGET_ACOLCK_SE_
#include "mmiwidget_special_effect.h"
#include "graphics_3d_rotate.h"

typedef struct Watch_Fouction_Param {
	IMGREF_SIZE_T out_size;
	IMGREF_3D_FLOAT_P_T watch_centre;
	void * out_buffer;
	int32 cur_frame;
	int32 total_frame;
	BOOLEAN Is_swing;
	BMP_OBJ * resource_collection;
	uint8 Hour;
	uint8 Min;
	uint8 Second;
}WATCH_F_PARAM;


#define  WATCH_SUR_ONE_Z 0 
#define  WATCH_SUR_TWO_Z 15

PUBLIC void MMIWIDGET_Creat_Watch(WATCH_F_PARAM * indata_param);

PUBLIC int32 * MMIWIDGET_Aclock_GetWatchAnglePtr(void);

#endif
