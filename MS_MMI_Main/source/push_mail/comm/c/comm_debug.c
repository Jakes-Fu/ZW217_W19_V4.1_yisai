/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_debug.c
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#include "mmi_push_mail.h"
#include "std_header.h"

#include "stdarg.h"

#include "comm_debug.h"
#include "comm_platform.h"
#include "mail_file.h"

#define MAIL_DEBUG_FILE "D:\\mail\\debug.txt"

#define COMM_LOG_MAX_LEN (2*1024)

#define COMM_LOG_BUFF_LEN  (5*1024)
char mail_trace_buffer[COMM_LOG_BUFF_LEN] = {0};

uint32 mail_debug_res = 0;
static int local_char_len = 0;



/*==========================================================
 * Function 	: mail_debug_init
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/26/2010 
 ==========================================================*/
void mail_debug_init(void)
{
    local_char_len = 0;
    mail_debug_res = mail_FileOperationStartBuf(MAIL_DEBUG_FILE);
}



/*==========================================================
 * Function 	: mail_debug_finial
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/26/2010 
 ==========================================================*/
void mail_debug_finial(void)
{
    uint32 char_len = 0;

    char_len = strlen(mail_trace_buffer);

    if(mail_debug_res && (local_char_len || char_len))
    {
        uint32 buf_write = 0;
    
        uint32 write_pos = 0;
        
        MMIAPIFMM_WriteFile(mail_debug_res, mail_trace_buffer, char_len, &buf_write, NULL); 

        while(buf_write < char_len)
        {
            //写完为止
            write_pos += buf_write;
            char_len -= buf_write;
            buf_write = 0;

            MMIAPIFMM_WriteFile(mail_debug_res, mail_trace_buffer + write_pos, char_len, &buf_write, NULL);
        }
        memset(mail_trace_buffer, 0, COMM_LOG_BUFF_LEN);
    }

    if(mail_debug_res)
    {
        mail_FileOperationStopBuf(&mail_debug_res);
    }

    local_char_len = 0;
    
}




/*==========================================================
 * Function 	: comm_TraceLogType
 * Author		: sunguochen      
 * Description :  log写文件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_TraceLogType(char *in_format, ...)
{
    va_list args;
    int nBuf;
    uint32 char_len = 0;

    char_len = strlen(mail_trace_buffer);

    if(char_len > (COMM_LOG_BUFF_LEN - COMM_LOG_MAX_LEN))
    {
        uint32 buf_write = 0;
            
        MMIAPIFMM_WriteFile(mail_debug_res, mail_trace_buffer, char_len, &buf_write, NULL); 

        memset(mail_trace_buffer, 0, COMM_LOG_BUFF_LEN);
		char_len = 0;
    }
    
    va_start(args, in_format);/*lint !e718 !e64*/
    nBuf = vsprintf(mail_trace_buffer + char_len, in_format, args);
    va_end(args);
/*
    char_add = strlen(mail_trace_buffer);

    if((char_add - char_len) > 512)
    {
        memset(mail_trace_buffer + char_len + 512, 0, (char_add - char_len - 512));
    }

    strcat(mail_trace_buffer, "\r\n");
*/

}




/*==========================================================
 * Function 	: comm_TraceFile
 * Author		: sunguochen      
 * Description :  log写文件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_TraceFile(char *buf, int len)
{

#ifdef MAIL_DEBUG_SAVE_FILE
    int space_num =0;
    char space_str[17] = {0};
    
    local_char_len += len;

    if(mail_debug_res == 0)
    {
        mail_debug_res = mail_FileOperationStartBuf(MAIL_DEBUG_FILE);
    }

    if(len)
    {
        int buf_write = 0;

        space_num = (16 - (len%16))%16;

        //COMM_TRACE:"comm_TraceFile: len :%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COMM_DEBUG_165_112_2_18_3_26_5_0,(uint8*)"d", space_num);
    
        MMIAPIFMM_WriteFile(mail_debug_res, buf, len, &buf_write, NULL); 
        if(space_num > 0)
        {
            MMIAPIFMM_WriteFile(mail_debug_res, space_str, space_num, &buf_write, NULL);
        }

    }

#endif

}



