/****************************************************************************
** File Name:      mmieng_main.h							                                                 *
** Author:          allen								                                                 *
** Date:             2004.08.17														*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.      				*
** Description:    									               					*
*****************************************************************************
**                         Important Edit History                        							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             						*
** 08/2004      allen               Create
** 
****************************************************************************/
#ifdef ENGTD_SUPPORT
#ifndef  _MMIENG_MAIN_H_    
#define  _MMIENG_MAIN_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif

typedef enum
{
	MMI_VOICEPARAM_NULL,
	
	MMI_HANDHOLD_SIDETONE_GAIN,
	MMI_EARPHONE_SIDETONE_GAIN,
	
	MMI_HANDHOLD_DIGITAL_GAIN,
	MMI_HANDFREE_DIGITAL_GAIN,
	MMI_EARPHONE_DIGITAL_GAIN,
	
	MMI_HANDHOLD_PGA_GAIN,
	MMI_HANDFREE_PGA_GAIN,
	MMI_EARPHONE_PGA_GAIN,
	
	MMI_HANDHOLD_DIGITAL_SCALE,
	MMI_HANDFREE_DIGITAL_SCALE,
	MMI_EARPHONE_DIGITAL_SCALE
} MMI_VOICEPARAM_E;

#ifdef __cplusplus
    }
#endif

#endif
#endif// ENGTD_SUPPORT