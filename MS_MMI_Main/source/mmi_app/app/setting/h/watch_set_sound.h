/*****************************************************************************
** File Name:      watch_set_sound.h                                         *
** Author:         bin.wang1                                                 *
** Date:           02/26/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      bin.wang1              Creat                                 *
******************************************************************************/

#ifndef _WATCH_SET_SOUND_H_
#define _WATCH_SET_SOUND_H_

#include "mmiset_export.h"

/*****************************************************************************/
//  Discription: execute settings ring preview                               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_PreviewRing( uint16 ring_id, MMISET_ALL_RING_TYPE_E ring_type, uint8 ring_volume, BOOLEAN is_vibrate );

/*****************************************************************************/
//  Discription: stop settings ring preview                               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_StopPreviewRing( void );

/*****************************************************************************/
//  Discription: enter setttings->sound window                               *
//  Global resource dependence:                                              *
//  Author: bin.wang1                                                        *
//  Note:                                                                    *
/*****************************************************************************/
PUBLIC void Settings_SoundWin_Enter( void );

#endif
