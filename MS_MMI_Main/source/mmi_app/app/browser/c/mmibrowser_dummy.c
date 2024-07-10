/****************************************************************************
** File Name:      mmibrowser_dummy.c                                      *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mn_type.h"
//#include "mimeApi.h"
//#include "wapApi.h"
#include "mmk_app.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
//li.li
//#ifndef FontType_t
//#define FontType_t unsigned char
//#endif
//typedef unsigned int		CMS_BROWSER_U32;
//li.li
//MMI_APPLICATION_T	    g_wap_app = {0};							//g_wap_app��ʵ�嶨��

//li.li
//int32	globalMachineFag	= 0;

//li.li
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

//juan.zhang
PUBLIC MMICMSWAP_GPRS_STATE_E MMIBRW_GetGprsState(void)
{
    return 0;
}

//juan.zhang ������ڹ���ģʽ���Զ���uaprofile�Ľӿڣ�Ϊ�˵����õ�
PUBLIC void MMIBRW_SetUAProfile(uint8 *ua_profile_ptr, uint16 ua_profile_length)
{
    return;
}

//juan.zhang ������ڹ���ģʽ���Զ���ua�Ľӿڣ�Ϊ�˵����õ�
PUBLIC void MMIBRW_SetUA(uint8 *ua_ptr, uint16 ua_length)
{
    return;
}

//juan.zhang ������ڹ���ģʽ�����þɵ�������Ƿ��log�ĺ���������ɾ��
PUBLIC void MMIBRW_SetDebugMode(BOOLEAN mode)
{
    return;
}

//juan.zhang ������ڹ���ģʽ���Զ���ua�Ľӿڣ�Ϊ�˵����õ�
PUBLIC void MMIBRW_SetUAToDefault(void)
{
    return;
}
