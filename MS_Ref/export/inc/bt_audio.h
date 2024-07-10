/******************************************************************************
** File Name:      bt_audio.h
** Author:         xiangxin
** DATE:           2012/8/13
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.
** Description:
******************************************************************************/

#ifndef BT_AUDIO_H_
#define BT_AUDIO_H_

#include "sci_types.h"

#if defined(BT_SCO_OVER_UART)

#if defined(BT_MUSIC_OVER_HFG)

extern void BLUE_PlayMusicByHfg(void);
extern void BLUE_StopMusicByHfg(void);

#endif

extern void BLUE_RecScoData(uint8 *data_ptr, uint16 len, uint8 errFlags);


#endif

#endif /* BT_AUDIO_H_ */

