/*****************************************************************************
** File Name:      mmiautodemo_api.c                                              *
** Author:                                                                   *
** Date:           3/16/2012                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe auto demo api               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                     DESCRIPTION                 *
** 3/2012        yongsheng.wang                Create                        *
******************************************************************************/
#define _MMIAUTODEMO_API_C_  
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
//#include "mmiautodemo_task.h"
#include "mmiautodemo_api.h"
#include "mmiautodemo_internal.h"
#include "mmiautodemo_text.h"
#include "mmipub.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :get msgid from s_autodemo_msg_mapping
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetKeycodeFrom_AutoDemo_Msg_Mapping(uint8* content_str_ptr);

/*****************************************************************************/
//  Description :get  line cmd content
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL uint8* GetContent(uint8 * cur_pos_str);

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   
LOCAL BOOLEAN s_autodemo_is_run = FALSE;
LOCAL BOOLEAN s_is_parseing = FALSE;
LOCAL MMI_AUTODEMO_INFO_T s_autodemo_info = {0};

/*will modify later to clean style*/
LOCAL MMI_AUTODEMO_MSG_MAPPING_T s_autodemo_msg_mapping[] =
{
	{"\"menu\"" ,   0x00000008},
	{"\"cancel\"",  0x00000009},
	{"\"red\""  ,   0x0000000d}
};
LOCAL uint32 s_offset_buffer = 0;
LOCAL uint32 s_line_str_offset = 0;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start auto demo
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAD_Start(void)
{
	MMIAD_CreateADTask();
}

/*****************************************************************************/
//  Description : stop auto demo
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAD_Stop(void)
{
    MMIFILE_HANDLE	file_handle = MMIAPIAUTODEMO_GetFileHandle();
    MMIAPIFMM_CloseFile(file_handle);
    MMIAPIAUTODEMO_SetFileHandle(PNULL);
    MMIAD_FreeCmdBuf();
    MMIAD_DeleteADTask();
}

/*****************************************************************************/
//  Description : is auto demo running? 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAD_IsRunning(void)
{
	return s_autodemo_is_run;
}

/*****************************************************************************/
//  Description : set is_run
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIAD_SetRunningFlag(BOOLEAN is_running)
{
	s_autodemo_is_run = is_running;
}

/*****************************************************************************/
//  Description : is auto demo parsing? 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAD_IsParsing(void)
{
	return s_is_parseing;	
}

/*****************************************************************************/
//  Description : set s_is_parseing
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIAD_SetParsingFlag(BOOLEAN is_parsing)
{
	s_is_parseing = is_parsing;
}


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
								 )
{
    uint8* msg_type_str_ptr = PNULL;
    uint8* content_str_ptr = PNULL;
    MMIAD_MSG_TYPE_E msg_type = MMIAD_MSG_NONE;
    
    msg_type_str_ptr = GetContent(cmdstr_line_ptr);
    
	if(0 == MMIAPICOM_CompareTwoUint8String(msg_type_str_ptr , (uint16)strlen((char*)msg_type_str_ptr),
                                            (uint8 *)"BSObject.presskey", (uint16)strlen("BSObject.presskey")))
    {
        content_str_ptr = GetContent(cmdstr_line_ptr);
        *keycode_ptr = GetKeycodeFrom_AutoDemo_Msg_Mapping(content_str_ptr) ;

        content_str_ptr = GetContent(cmdstr_line_ptr);
		*pause_time_ptr = (uint16)atoi((char*)content_str_ptr);
        
        msg_type = MMIAD_SHORT_PRESS;
        return msg_type; 
    }
	else if(0 == MMIAPICOM_CompareTwoUint8String(msg_type_str_ptr , (uint16)strlen((char*)msg_type_str_ptr),
                                                 (uint8 *)"BSObject.Holdkey", (uint16)strlen("BSObject.Holdkey")))
    {
        
        content_str_ptr = GetContent(cmdstr_line_ptr);
        *keycode_ptr=GetKeycodeFrom_AutoDemo_Msg_Mapping(content_str_ptr);

        content_str_ptr = GetContent(cmdstr_line_ptr);
			*pause_time_ptr = (uint16)atoi((char*)content_str_ptr);

        msg_type = MMIAD_LONG_PRESS;
        return msg_type;	
    }
		else if(0 == MMIAPICOM_CompareTwoUint8String(msg_type_str_ptr , (uint16)strlen((char*)msg_type_str_ptr),
                                                   (uint8 *)"BSObject.TouchpadKeyClick", (uint16)strlen("BSObject.TouchpadKeyClick")))
    {
        content_str_ptr = GetContent(cmdstr_line_ptr);
				start_point_ptr -> x =  (uint16)atoi((char*)content_str_ptr);
        
        content_str_ptr = GetContent(cmdstr_line_ptr);
				start_point_ptr -> y =  (uint16)atoi((char*)content_str_ptr);
        
        content_str_ptr = GetContent(cmdstr_line_ptr);
				*pause_time_ptr = (uint16)atoi((char*)content_str_ptr);
        
        msg_type = MMIAD_TP_CLICK;
        return msg_type;	
    }
			else if(0 == MMIAPICOM_CompareTwoUint8String(msg_type_str_ptr , (uint16)strlen((char*)msg_type_str_ptr),
                                                        (uint8 *)"BSObject.TouchPadMoveLine", (uint16)strlen("BSObject.TouchPadMoveLine")))
    {
        content_str_ptr = GetContent(cmdstr_line_ptr);
					start_point_ptr -> x =  (uint16)atoi((char*)content_str_ptr);


        content_str_ptr = GetContent(cmdstr_line_ptr);
					start_point_ptr -> y =  (uint16)atoi((char*)content_str_ptr);

        content_str_ptr = GetContent(cmdstr_line_ptr);
					end_point_ptr -> x =  (uint16)atoi((char*)content_str_ptr);

        content_str_ptr = GetContent(cmdstr_line_ptr);
					end_point_ptr -> y =  (uint16)atoi((char*)content_str_ptr);

        content_str_ptr = GetContent(cmdstr_line_ptr);
					*pause_time_ptr = (uint16)atoi((char*)content_str_ptr);

        msg_type = MMIAD_TP_MOVE;
        return msg_type;			
    }
    else
    {
        s_line_str_offset = 0;
        msg_type = MMIAD_MSG_NONE;
        SCI_TRACE_LOW("AUTODEMO MSG_TYPE %d", msg_type);
        return msg_type;
    }

}

/*****************************************************************************/
//  Description :get cmd info
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC MMI_AUTODEMO_INFO_T* MMIAD_GetAUTODEMO_Info(void)
{
	return &s_autodemo_info;
}

/*****************************************************************************/
//  Description :get msgid from s_autodemo_msg_mapping
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 GetKeycodeFrom_AutoDemo_Msg_Mapping(uint8* content_str_ptr)
{
	uint32 i = 0;

    for (i = 0; i < sizeof(s_autodemo_msg_mapping) / sizeof(MMI_AUTODEMO_MSG_MAPPING_T); i++)
    {
        if (0 == MMIAPICOM_CompareTwoUint8String(content_str_ptr, (uint16)strlen((char*)content_str_ptr), 
                                                s_autodemo_msg_mapping[i].msg_str, (uint16)strlen((char*)s_autodemo_msg_mapping[i].msg_str)))
        {
            return s_autodemo_msg_mapping[i].key_code;
        }
    }
    SCI_TRACE_LOW("autodemo  No find keycode in AutoDemo");
    return 0;
}

/*****************************************************************************/
//  Description :get next line cmd
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAD_GetNextCmdLine(uint8 * cur_pos_str, uint8* buf_ptr, uint32 *buf_len)
{
	uint8* tmp = s_autodemo_info.cmd_buf_ptr + s_offset_buffer;
	uint8* head = tmp;
    //uint8  out_len = 0;
    //uint8* out_len_ptr = &out_len;
    //SCI_PASSERT(s_offset_buffer < s_autodemo_info.cmd_buf_len, ("buf_len is %d", s_autodemo_info.cmd_buf_len));
    //如果读入的是空文件结束autodemo_task并弹出错误窗口，将全局变量置0
    if(0 == *buf_len || PNULL == buf_ptr || PNULL == buf_len)
    {
        //MMIAD_OpenAutoDemoErrorWin(TXT_AUTODEMO_BUFFER_EMPTY);
        s_offset_buffer = 0;
        s_line_str_offset =0;
        MMIAD_OpenAutoDemoFinishedWin();
        MMIAPIAD_Stop();
        SCI_TRACE_LOW("MMIAD_GetNextCmdLine: EMPTY BUFFER");   
        return FALSE;
    }
    //以\r\n判断一行的结束，遍历后取得一行脚本存入cur_pos_str以便后续解析
	while(*tmp != '\r' && *tmp != '\0')
	{
		tmp++;
		s_offset_buffer++;
	}
    if ('\r' == *tmp)
    {
	*tmp++ = '\0';
    if(*tmp == '\n')
    {
        s_offset_buffer += 2;
    }
    else
    {
        s_offset_buffer++;
    }
    }
	
    //简单筛选一行脚本，如果不是以B开头的行一定为无效行直接跳过
    if((strlen((char*)head) <= CMD_STR_LINE_MAX_NUM)   && *head == 'B')
    {
        strcpy((char*)cur_pos_str , (char*)head );
        //没有查明为什么不能够用下面的语句
        //SCI_MEMCPY(cur_pos_str,head,(uint32)strlen(head));

        SCI_TRACE_LOW("autodemo this line %s", cur_pos_str);
    }
    else
    {
        if (strlen((char*)head) > CMD_STR_LINE_MAX_NUM)
        {
            MMIAD_OpenAutoDemoErrorWin(TXT_AUTODEMO_FORMAT_ERROR);
        }
        *cur_pos_str = '\0';
        SCI_TRACE_LOW("autodemo  this line has none keyword");
    }
    //当buf遍历完后需要将s_offset_buffer置0
	if(s_autodemo_info.cmd_buf_len == s_offset_buffer)
	{
		s_offset_buffer = 0;
        //判断文件是否结束，如果读取完了不再重新装载buf否则继续装载buf
		if(!MMIAPIAUTODEMO_IsFileOver())
        {
            MMIAPIAUTODEMO_LoadCmd(buf_ptr, buf_len);
        }
        else
        {
            SCI_TRACE_LOW("autodemo this file is over");    
            return FALSE;//不存在下一行脚本
        }
	}
    //判断脚本是否不符合格式没有换行或者一行脚本过长
    if(s_offset_buffer > s_autodemo_info.cmd_buf_len)
    {
        MMIAD_OpenAutoDemoErrorWin(TXT_AUTODEMO_FORMAT_ERROR);
        s_offset_buffer = 0;
        s_line_str_offset =0;
        MMIAPIAD_Stop();
    }

    return TRUE;//存在下一行脚本
}

/*****************************************************************************/
//  Description :get  line cmd content
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL uint8* GetContent(uint8 * cur_pos_str)
{
    uint8 * tmp = cur_pos_str + s_line_str_offset;
	uint8 * head = tmp;

    //SCI_PASSERT(s_line_str_offset < CMD_STR_LINE_MAX_NUM, ("s_line_str_offset is %d", s_line_str_offset));
    if(s_line_str_offset >= CMD_STR_LINE_MAX_NUM)
    {
        s_offset_buffer = 0;
        s_line_str_offset =0;
        MMIAD_OpenAutoDemoFinishedWin();
        MMIAPIAD_Stop();  
        return PNULL;
    }
    SCI_TRACE_LOW("GetContent in mmiautodemo_api.c, s_line_str_offset is %d", s_line_str_offset);
//以空格 逗号和‘\0’截取关键字
	while((*tmp != ' ' )&& (*tmp != ',' )&& (*tmp != '\0'))
	{
		tmp++;
        s_line_str_offset++;
	}

    if(*tmp == '\0')
    {
        s_line_str_offset = 0;
        
    }
    else
    {
        *tmp++ = '\0';
        s_line_str_offset++;
    }

    SCI_TRACE_LOW("autodemo  get keyword is %s,offset is %d",head,s_line_str_offset);
    
    return head;
}

/*****************************************************************************/
//  Description :open autodemo finished window by text_id 
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_OpenAutoDemoFinishedWin(void)
{
   MMIPUB_OpenAlertWinByTextId(PNULL,TXT_AUTODEMO_FINISHED,
                               TXT_NULL,IMAGE_PUBWIN_SUCCESS,
                               PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
//  Description :open autodemo read script error
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_OpenAutoDemoErrorWin(MMI_TEXT_ID_T text_id)
{
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,
                                TXT_NULL,IMAGE_PUBWIN_WARNING,
                                PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}
