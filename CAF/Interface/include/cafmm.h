/****************************************************************************
** File Name:      caf.h                                                   *
** Author:         Riancy.Zhang                                             *
** Date:           20/07/2009                                              *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2009        Riancy.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAFMM_H_    
#define  _CAFMM_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "sci_types.h" 


//////////
#define CAF_OBJ_CREATE_MAX	100	//OBJ is created for max sum
#define CAF_DEFAULT_HRECORD	0x00FFFFFF
#define CAF_NOUSED				-1
#define CAF_MAX_SPEAKER_VOLUME	9
#define CAF_MM_OUTPUT_SAMPLERATE	16000
///////////////////////////////////////////////////////////
#define CAF_MM_INVALIDVALUE			0xFFFFFFFF
////
//
typedef struct
{
	uint32			hand;//hand id
	uint32			dw_obj_addr;  //ISOUND,IMEDIA
	
}CAFMMOBJ_T;


#define CAF_HIWORD(dwvalue)		((uint16)(((uint32)( dwvalue ) >> 16 ) & 0xFFFF ))
#define CAF_LOWORD(dwvalue)		((uint16)(((uint32)(dwvalue) ) & 0xFFFF ))
#define CAF_HIBYTE(wvalue)		((uint8)(((uint16)(wvalue ) >> 8 ) & 0xFF ))
#define CAF_LOBYTE(wvalue)		((uint8)(((uint16)(wvalue) ) & 0xFF ))
#define CAF_MAKELONG(wa,wb) 	((uint32)(((uint32)(wa)&0x0000ffff) | ((uint32)(wb)) << 16 ))
#define CAF_MAKEWORD(ba,bb) 	((uint16)(((uint16)(ba)&0x00ff) | ((uint16)(bb)) << 8 ))

/*********************************************************************************/

/*****************************************************************************/
//  Description : init  object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void CAFMM_InitObject();

//  Description : add object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC CAF_MM_RESULT CAFMM_AddObject(uint32 dw_obj_addr);
/*****************************************************************************/
//  Description : modify object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC CAF_MM_RESULT CAFMM_ModifyObject(uint32 dw_obj_addr,uint32 dw_audio_addr);
/*****************************************************************************/
//  Description : remove object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC CAF_MM_RESULT CAFMM_RemoveObject(uint32 dw_obj_addr);
/*****************************************************************************/
//  Description : get object by haudio.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 CAFMM_GetObject(uint32 dw_audio_addr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
