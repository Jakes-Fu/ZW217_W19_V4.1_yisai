/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmidrv_fm.h
**		This module include the Nmi Fm driver APIs.
**	Creator: Michael Jia
** 
*******************************************************************************/

void nmi_fm_set_fm_step(unsigned char step);

unsigned char nmi_fm_get_fm_step(void);
	
void  FMDrv_PowerOnReset(void);

void  FMDrv_PowerOffProc(void);

unsigned char FMDrv_ValidStop( int freq, char signalvl, char is_step_up);

void  FMDrv_SetFreq( signed int curf );

unsigned char  FMDrv_GetSigLvl( int curf );

void  FMDrv_Mute(unsigned char mute);


void  FMDrv_SetVolumeLevel(unsigned char level);



