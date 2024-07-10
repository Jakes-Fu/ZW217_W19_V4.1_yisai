/****************************************************************************
** File Name:      mmieng_internal.h							            *
** Author:           xiyuan.ma								                    *
** Date:             2012.02.11												*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.      	*
** Description:    									               			*
*****************************************************************************
**                         Important Edit History                        	*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             	*
** 08/2004      allen               Create
** 02/2012      xiyuan.ma           Edit
****************************************************************************/
#ifndef  _MMIENG_INTERNAL_H_    
#define  _MMIENG_INTERNAL_H_   

#include "mmiparse_export.h"
//#include "mmirecord.h"

//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmieng_main.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_main.h"
#include "mmimbbms_main.h"
#include "mbbms_service.h"
#endif
#include "adpcmrecord_adp.h"
#include "mmieng_export.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMIENG_3RDPARTYVER_TEXT_LEN	600	
#define MMIENG_WRE_VER_MAX_LEN      100
#define MMIENG_VERSION_TEXT_LEN     1280
#define MMIENG_SENSOR_VERSION_TEXT_LEN  100

#define MMIENG_TEST_PASS_COLOR  MMI_GREEN_COLOR
#define MMIENG_TEST_FAIL_COLOR  MMI_RED_COLOR
#define MMIENG_NOT_TEST_COLOR   MMI_WHITE_COLOR


typedef const uint32 * FUNCTION;

typedef struct//POWERON TIME
{
	uint32 standbyEn;
	uint32 startDay;
	uint32 standbyStartTime;
	uint32 standbytime;
}STANDBY_RECORD_T;

//#ifdef ENG_SUPPORT
#ifdef CMMB_SUPPORT
/********************************************************************************
 NAME: CmmbTest_ParseIni
 DESCRIPTION:Ω‚Œˆcmmbtest.ini≈‰÷√Œƒº˛
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC BOOLEAN CmmbTest_ParseIni(MMIENG_CMMB_INI_FILE_INFO_T *ini_file_info);
#endif
//#endif//ENG_SUPPORT

/*****************************************************************************/
//  Description : MMIENG_SetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_SetUa(MMIENG_UA_T *ua);

/*****************************************************************************/
//  Description : MMIENG_GetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUa(MMIENG_UA_T *ua_ptr);

/********************************************************************************
 NAME:			MMIENG_GetUserAgentIsUseList
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2009.07.27
********************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUserAgentIsUseList(void);

/********************************************************************************
 NAME:			MMIENG_SetUserAgentIsUseList
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2006.08.08
********************************************************************************/
PUBLIC void MMIENG_SetUserAgentIsUseList(BOOLEAN is_use_list);

/********************************************************************************
 NAME:			MMIENG_GetUaProfileIsUseList
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2009.07.27
********************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUaProfileIsUseList(void);

/********************************************************************************
 NAME:			MMIENG_SetUaProfileIsUseList
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2006.08.08
********************************************************************************/
PUBLIC void MMIENG_SetUaProfileIsUseList(BOOLEAN is_use_list);

/********************************************************************************
 NAME:			MMIENG_GetUserAgentListNum
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2009.07.27
********************************************************************************/
PUBLIC uint32 MMIENG_GetUserAgentListNum(void);

/********************************************************************************
 NAME:			MMIENG_SetUserAgentListNum
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2006.08.08
********************************************************************************/
PUBLIC void MMIENG_SetUserAgentListNum(uint32 list_num);

/********************************************************************************
 NAME:			MMIENG_GetUaProfileListNum
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2009.07.27
********************************************************************************/
PUBLIC uint32 MMIENG_GetUaProfileListNum(void);

/********************************************************************************
 NAME:			MMIENG_SetUaProfileListNum
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		li.li
 DATE:			2006.08.08
********************************************************************************/
PUBLIC void MMIENG_SetUaProfileListNum(uint32 list_num);

/*****************************************************************************/
//  Description : MMIENG_GetUserAgentIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUserAgentIsUse(void);

/*****************************************************************************/
//  Description : MMIENG_SetUserAgentIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUserAgentIsUse(BOOLEAN is_use);

/*****************************************************************************/
//  Description : MMIENG_GetUaProfileIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUaProfileIsUse(void);

/*****************************************************************************/
//  Description : MMIENG_SetUaProfileIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUaProfileIsUse(BOOLEAN is_use);

/*****************************************************************************/
// 	Description : create public window, for list selcetion
//	Global resource dependence : 
//  Author: ying.xu
//	Note:   
/*****************************************************************************/
PUBLIC int32 MMIENG_OpenListSelectWin(uint32  handle_function);
#ifdef __cplusplus
    }
#endif
#endif //_MMIENG_INTERNAL_H_
