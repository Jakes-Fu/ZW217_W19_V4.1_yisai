/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_smtp_lmnd.h
 *	Author		: sunguochen    
 *	Description	: smtp support
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/




#ifndef _MAIL_SMTP_LMND_H_
#define _MAIL_SMTP_LMND_H_


#include "mail_smtp.h"


#define SMTP_PIPELINING             "PIPELINING"
#define SMTP_8BITMIME               "8BITMIME"
#define SMTP_CHUNKING               "CHUNKING"
#define SMTP_BURL                   "BURL"

#define SMTP_BOUNDARY_START         CRLF "--"
#define SMTP_BDAT                   "BDAT"
#define SMTP_CHUNKING_TERMINATION   "BDAT 0 LAST \r\n"



typedef struct SMTPSupportDataStuct
{
    comm_boolean   burl;               
    comm_boolean   pipelining;         
    comm_boolean   _8bitmime;          
    comm_boolean   chunking;           
} tSMTPSupportData;



typedef struct SMTPchunkingDataStuct
{
    //  MailFldrMgrMessageContainer *MsgInfo;               
    comm_int32                   nextBodyPart;               
    comm_int32                   chunkStart;               
    comm_int32                   chunkSize;               
    comm_int32                   bytesRead;               
    comm_boolean                    burl_boundary;

} SMTPchunkingData;




/*==========================================================
 * Function 	: initSMTPSupportedData
 * Author		: sunguochen      
 * Description : 获得服务器支持信息
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void initSMTPSupportedData(const char *serverReply, int serverReplyLength, tSMTPSupportData* SMTPSupportData);



/*==========================================================
 * Function 	: resetSMTPSupportedData
 * Author		: sunguochen      
 * Description : 支持信息复位
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/1/2010 
 ==========================================================*/
void resetSMTPSupportedData(tSMTPSupportData* SMTPSupportData);


#endif 
