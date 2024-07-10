#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      cafsound.c                                              *
** Author:         Riancy.Zhang                                            *
** Date:           02/23/2009                                              *
** Copyright:      2008-2010 Spreatrum, Incoporated. All Rights Reserved.  *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 02/2009        Riancy.Zhang       Create
** 
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafmodule_gen.h"
#include "cafmm.h"
#include "sci_types.h"
#include "cafsound.h"
#include "cafmedia.h"
#include "audio_api.h"
/////////////////////////////////////////////////////////////////////////////
//obj
LOCAL CAFMMOBJ_T 	s_cafobj[CAF_OBJ_CREATE_MAX];
LOCAL uint32		s_cafobjsum=0x0;//sum
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          functions definition                             */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init  object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void CAFMM_InitObject()
{
	uint32 i=0;
	if(0 == s_cafobjsum)
	{
		for(i=0; i< CAF_OBJ_CREATE_MAX; i++)
		{
			s_cafobj[i].dw_obj_addr=0;
			s_cafobj[i].hand=INVALID_HANDLE;
		}
	}
}
/*****************************************************************************/
//  Description : add object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC CAF_MM_RESULT CAFMM_AddObject(uint32 dw_obj_addr)
{
	uint32 i=0;

	
	//out of range
	if(s_cafobjsum >= CAF_OBJ_CREATE_MAX)
	{
		return CAF_MM_OUTOFRANGE;
	}
	//invalid address
	if(0 == dw_obj_addr)
	{
		return CAF_MM_INVALPARAM;
	}
	
	for(i=0;i<s_cafobjsum;i++)
	{
		if(s_cafobj[i].dw_obj_addr == dw_obj_addr)
		{
			return CAF_MM_NOERROR;
		}
	}


	s_cafobj[s_cafobjsum].dw_obj_addr=dw_obj_addr;
	s_cafobjsum++;

	return CAF_MM_NOERROR;
	
}
/*****************************************************************************/
//  Description : modify object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC CAF_MM_RESULT CAFMM_ModifyObject(uint32 dw_obj_addr,uint32 dw_audio_addr)
{
	uint32 i=0;
	
	
	//invalid address
	if(0 == dw_obj_addr || 0 == dw_audio_addr)
	{
		return CAF_MM_INVALPARAM;
	}
	
	for(i=0;i<s_cafobjsum;i++)
	{
		if(s_cafobj[i].dw_obj_addr == dw_obj_addr)
		{
			s_cafobj[i].hand = dw_audio_addr;
			break;
		}
	}
	
		
	return CAF_MM_NOERROR;
	
}
/*****************************************************************************/
//  Description : remove OBJECT object.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC CAF_MM_RESULT CAFMM_RemoveObject(uint32 dw_obj_addr)
{
	uint32 i=0;
	
	//invalid address
	if(0 == dw_obj_addr)
	{
		return CAF_MM_INVALPARAM;
	}
	for(i=0;i<s_cafobjsum;i++)
	{
		if(s_cafobj[i].dw_obj_addr == dw_obj_addr)
		{
			memmove((void*)(s_cafobj+i),(void*)(s_cafobj+i+1),(CAF_OBJ_CREATE_MAX-i-1)*sizeof(uint32));
			
			if(s_cafobjsum > 0)
			{
				s_cafobjsum--;
			}
			s_cafobj[s_cafobjsum].dw_obj_addr=0;
			s_cafobj[s_cafobjsum].hand=0;
			return CAF_MM_NOERROR;
		}
	}
	
	
	return CAF_MM_OUTOFRANGE;
	

}
/*****************************************************************************/
//  Description : get OBJECT by haudio.
//  Global resource dependence : none
//  Author: Riancy.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 CAFMM_GetObject(uint32 dw_audio_addr)
{
	uint32 i=0;

	//invalid address
	if(INVALID_HANDLE == dw_audio_addr)
	{
		return 0;
	}
	
	for(i=0;i<s_cafobjsum;i++)
	{
		if(s_cafobj[i].hand == dw_audio_addr)
		{
			return s_cafobj[i].dw_obj_addr;
		}
	}

	return 0;
}
#endif