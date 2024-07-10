/******************************************************************************
 ** File Name:      auto_download.h                                           *
 ** Author:         Jackey.Ling                                              *
 ** DATE:           2012.05.04                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2012.05.04   Jackey.Ling          Create.                               *
 ******************************************************************************/

#ifndef _AUTOTEST_AUDIO_H
#define _AUTOTEST_AUDIO_H

PUBLIC uint32 Autotest_Speaker_Start(    uint8 *srcdata, uint32 datalen, uint8 dev_mode, uint32 is_stereo);
PUBLIC uint32 Autotest_Speaker_Start_Local(uint8 dev_mode);
PUBLIC uint32 Autotest_Speaker_Stop(void);

PUBLIC uint32 Autotest_Mic_Start(uint8 dev_mode);
PUBLIC uint32 Autotest_Mic_Stop(void);
PUBLIC uint8 Autotest_GetMicState(void);
PUBLIC uint16* Autotest_GetDataAddr(uint32 index);
#endif


