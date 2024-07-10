/***************************************************************************
** File Name:      mmiautodemo_api.h                                             *
** Author:                                                                 *
** Date:           3/16/2012                                              *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                  NAME                  DESCRIPTION                 *
** 3/2012         yongsheng.wang                  Create                   *
**																		   *
****************************************************************************/
#ifndef _MMIAUTODEMO_API_H_
#define  _MMIAUTODEMO_API_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_osbridge.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define CMD_STR_LINE_MAX_NUM 100

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef enum
{
    MMIAD_MSG_NONE,
	MMIAD_SHORT_PRESS,
	MMIAD_LONG_PRESS,
	MMIAD_TP_CLICK,
	MMIAD_TP_MOVE,
	MMIAD_TP_LONG_CLICK,
    MMIAD_MSG_TYPE_MAX
}MMIAD_MSG_TYPE_E;

typedef struct MMI_AUTODEMO_INFO_TAG
{
	uint8  *cmd_buf_ptr;
	uint32 cmd_buf_len;
} MMI_AUTODEMO_INFO_T;

typedef struct MMI_AUTODEMO_MSG_MAPPING_TAG
{
	uint8 msg_str[20];
	uint32      key_code;  
}MMI_AUTODEMO_MSG_MAPPING_T;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start auto demo
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAD_Start(void);
/*****************************************************************************/
//  Description : stop auto demo
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAD_Stop(void) ;

/*****************************************************************************/
//  Description : is auto demo running? 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAD_IsRunning(void);

/*****************************************************************************/
//  Description : set is_run
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIAD_SetRunningFlag(BOOLEAN is_running);

/*****************************************************************************/
//  Description : is auto demo parsing? 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAD_IsParsing(void);

/*****************************************************************************/
//  Description : set is_parse
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIAD_SetParsingFlag(BOOLEAN is_parsing);

/*****************************************************************************/
//  Description :get cmd info
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC MMI_AUTODEMO_INFO_T* MMIAD_GetAUTODEMO_Info(void);

/*****************************************************************************/
//  Description :get next line cmd
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAD_GetNextCmdLine(uint8 * cur_pos_str, uint8* buf_ptr, uint32 *buf_len);

/*****************************************************************************/
//  Description : destroy timer for restoring auto demo
//  Global resource dependence : g_autodemo_restore_timer_id
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIAD_MSG_TYPE_E MMIAD_ParseCmdByLine(
								 uint8* cmdstr_line_ptr,
								 uint16* pause_time_ptr,//out
								 GUI_POINT_T *start_point_ptr,//out
								 GUI_POINT_T *end_point_ptr, //out
								 uint32 *keycode_ptr //out
								 );

/*****************************************************************************/
//  Description :open autodemo finished window by text_id 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_OpenAutoDemoFinishedWin(void);

/*****************************************************************************/
//  Description :open autodemo read script error
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_OpenAutoDemoErrorWin(MMI_TEXT_ID_T text_id);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_MMIAUTODEMO_API_H_