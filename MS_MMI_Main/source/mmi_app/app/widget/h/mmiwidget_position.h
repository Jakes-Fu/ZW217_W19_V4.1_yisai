/*****************************************************************************
** File Name:      mmiwidget_position.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#ifndef _MMIWIDGET_POSITION_H_
#define _MMIWIDGET_POSITION_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif




#if defined(MAINLCD_SIZE_240X320)
      #include"mmiwidget_position_240X320.h"
#elif defined(MAINLCD_SIZE_320X480)
      #include"mmiwidget_position_320X480.h"
#elif defined(MAINLCD_SIZE_240X400)
      #include"mmiwidget_position_240X400.h"
#elif defined(MAINLCD_SIZE_176X220)
      #include"mmiwidget_position_176X220.h"	  
#else
      #include"mmiwidget_position_128X160.h"
#endif
















#ifdef   __cplusplus
    }
#endif

#endif


