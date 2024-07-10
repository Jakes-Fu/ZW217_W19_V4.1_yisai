/*****************************************************************************
** File Name:      watch_extra_export.h                                      *
** Author:         xiaotong.su                                               *
** Date:           01/06/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2021      xiaotong.su              Creat                              *
******************************************************************************/
#ifndef ADULT_WATCH_SUPPORT

#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)

#ifndef _WATCH_EXTRA_EXPORT_H_
#define _WATCH_EXTRA_EXPORT_H_

#ifdef APP_STORE_SUPPORT
#include"appstore_common.h"
#endif

/*****************************************************************************/
//  Discription: enter extra window                                          *
//  Global resource dependence:                                              *
//  Author: xiaotong.su                                                     *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void WatchEXTRA_MainWin_Enter( void );

#endif
#endif
#endif
