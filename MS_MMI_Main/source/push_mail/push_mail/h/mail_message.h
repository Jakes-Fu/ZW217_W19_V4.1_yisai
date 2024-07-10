/*
* hisense pushmail
*/
#ifndef  _MAIL_MESSAGE_H_    
#define  _MAIL_MESSAGE_H_  

#include "mmk_type.h"
#include "comm_platform.h"


#define MAIL_MAX_CONTENT_LEN 50*1024

#define MAIL_MAX_MSGID_LEN    128

#define ATTRS_MAXMIMEPARTS 200

#define MAXMIMEPARTS 100
#define MAXTO 100
#define MAXCC 100
#define MAXBCC 100
#define MAXHEADERS 100
#define MINMAILSIZE 128	
#define MAXFILEDLENGTH 1024

#define MAIL_BIN_ROW_SIZE_LEN 54

typedef enum {
    Notenc, Base64, QuotedPrintable, bit_7, bit_8, Binary
}TransferEncoding;

typedef enum {
    Nocharset, Utf8, Gbk, Gb2312, Ascii, Iso8859_1
}Charset;

typedef enum {
    Inbox, Outbox, Sent, Draft, Trash
}MessageLocation;

typedef enum {
    General, Other
}MessageType;

typedef enum {
	Fine,
	FileNotExist,
	FileReadFailed,
	MemoryNotEnough,
	FileTooLarge,
	FileTooSmall,
	SyntaxError
}ErrorType;

typedef enum {
    To, Cc, Bcc, From, Replyto
}AddressType;

typedef struct MAIL_MESSAGE_PART
{
    /*
    Content-Type: application/octet-stream
    Content-Transfer-Encoding: base64
    Content-Disposition: attachment; filename="=?UTF-8?B?aXAudHh0?="
    Content-ID: 1
    X-Attachment-Size: 272
    X-Attachment: False
    */

    uint8             * content;
    uint32              content_l;
    //uint8             * content_decode_tenc;
    TransferEncoding    transferEncoding;
    Charset				charset;
    uint8        	  * contenttype;
    uint8        	  * contentdisposiation;
    comm_string_t       name;
    comm_string_t       filename;
    uint8        	  * contentid;
    uint8        	  * contentlocation;
    uint32        		size;	//X-Attachment-Size
    BOOLEAN				attr_has_data; //pushmail is attachment has data 

	//encapsuled
    comm_string_t       fullname;	// attr path and name
    //uint32				filesize;	// file size

}MAIL_MESSAGE_PART_T;

typedef struct MAIL_CONVERT_ATTR
{
    /*
    ----attachment
    Content-Type: application/octet-stream; name="=?UTF-8?B?5omL5py66YKu566xVUnnlYzpnaLorr7orqEyMDA4LTAxMzAucHB0?="
    Content-Transfer-Encoding: base64
    X-Attachment-Size: 2258432
    Content-ID: 1.1
    Content-Disposition: attachment
    */
    TransferEncoding    transferEncoding;
    uint8        	  * contenttype;
    comm_string_t       name;
    uint8        	  * contentid;
    uint32        		size;	//X-Attachment-Size
}MAIL_CONVERT_ATTR_T;

typedef struct MAIL_ADDRESS
{
    comm_string_t       name;
    char        	  *	email;
}MAIL_ADDRESS_T;

typedef struct MAIL_CONVERT_ATTRS
{
    int				  attrcount;
    MAIL_CONVERT_ATTR_T * convertparts[ATTRS_MAXMIMEPARTS];  
}MAIL_CONVERT_ATTRS_T;

typedef struct 
{
    SIGNAL_VARS
    BOOLEAN is_success;  
}MAIL_RESULT_INFO_T;

typedef struct MAIL_MESSAGE
{
    SIGNAL_VARS
	/*	header
	Received: from w2 ([61.51.111.6])
		(authenticated bits=0)
		by mail.com (8.12.8/8.12.8) with ESMTP id i9ONXPtU004713;
		Mon, 25 Oct 2004 07:33:27 +0800
	Message-ID: <001c01c4ba21$e33699f0$8e00a8c0@w2>
	From: "mailer" <mailer@Mail.com>
	To: <receiver@mail.com>
	Cc: <cc1@mail.com>, <cc2@mail.com>, "mailer" <mailer@mail.com>
	Subject: This is subject
	Date: Mon, 25 Oct 2004 07:33:29 +0800
	MIME-Version: 1.0
	Content-Type: multipart/mixed;
		boundary="----=_NextPart_000_0016_01C4BA64.EC564830"
	X-Mail-Type: 0
	X-Priority: 3
	X-MSMail-Priority: Normal
	X-Mailer: Microsoft Outlook Express 6.00.2900.2180
	X-MimeOLE: Produced By Microsoft MimeOLE V6.00.2900.2180
	X-Virus-Scanned: ClamAV 0.80/540/Tue Oct 19 20:59:23 2004
		clamav-milter version 0.80j
		on mail.com
	X-Virus-Status: Clean
	*/

    uint8			  	* uidl;	
    uint8			  	* msgid;
    MessageLocation		  maillocation;
    MAIL_ADDRESS_T    	* from[1];
    uint16				  from_count;
    MAIL_ADDRESS_T    	* to[MAXTO];
    uint16				  to_count;
    MAIL_ADDRESS_T    	* cc[MAXCC];
    uint16				  cc_count;
    MAIL_ADDRESS_T    	* bcc[MAXBCC];
    uint16				  bcc_count;
    MMI_STRING_T          subject;
    MAIL_ADDRESS_T    	* replyto[1];
    uint16				  replyto_count;
    uint8			  	* date;
    comm_string_t         transactionid;
    uint8        	  	* mimevertion;
    uint8        	  	* contenttype;
    MessageType			  messagetype;	// 1 or 0 
    uint8        	  	* priority;	
    uint8        	  	* mailer;	
    uint8        	  	* boundary;
    uint8        	  	* content;
    uint8			  	* mailheader;
    uint8			  	* mailbody;
    uint8				* textcontent;
    MMI_STRING_T          textbody;
    uint8               * htmlbody;
    uint32        		  size;	//get from server
    uint32				  filesize;	//read the mailmessage file
    Charset				  charset;
    BOOLEAN               hasattr;  //解析头信息使用--是否有附件
    TransferEncoding	  transferEncoding;
    //comm_string_t          headers[MAXHEADERS];	
    uint16				  partcount;
    MAIL_MESSAGE_PART_T * mailmessageparts[MAXMIMEPARTS];
    
    BOOLEAN				  fine;	// no error or not 
    
}MAIL_MESSAGE_T;


//parse
//PUBLIC BOOLEAN MailMessage(wchar * fullname, MAIL_MESSAGE_T * mailmessage);

PUBLIC MAIL_MESSAGE_T *MMIEMAIL_CreateMessage(wchar * fullname, uint32 size, MessageLocation location);
PUBLIC void MMIEMAIL_DestroyMessage(MAIL_MESSAGE_T *message_ptr);


//BOOLEAN parseMessage(uint8 * content, MAIL_MESSAGE_T * mailmessage);
void ParseMessage(MAIL_MESSAGE_T * mailmessage, BOOLEAN entire);
void ParseHeaders(MAIL_MESSAGE_T * mailmessage);
void ParseBody(MAIL_MESSAGE_T * mailmessage);
void parseHeaderField(uint8 * str, MAIL_MESSAGE_T * mailmessage);

void ParseAddresses(uint8 * str, uint16 * addresscount, 
					MAIL_ADDRESS_T ** address, MAIL_MESSAGE_T * mailmessage);
BOOLEAN Parseaddress(char * src, MAIL_ADDRESS_T * a, MAIL_MESSAGE_T * mailmessage);

//void ParseMimePart(uint8 * str, uint8 * boundary, uint16 * bodyId);
//volatile void ParseMimePart(uint8 * boundary, uint16 * count, MAIL_MESSAGE_T * mailmessage);
volatile void ParseMimePart(uint8 * boundary, uint8 * body, uint16 * count, MAIL_MESSAGE_T * mailmessage);

uint8 * Content(uint8 * s, uint8 * target, MAIL_MESSAGE_T * mailmessage, BOOLEAN quotation);
comm_uint8 * Content_mail_header(comm_uint8 * s, comm_uint8 * target, MAIL_MESSAGE_T * mailmessage, comm_boolean quotation);

BOOLEAN ParseTextbody(TransferEncoding tenc, 
									Charset charset,
									MAIL_MESSAGE_T * mailmessage);

//encapsulation
uint16 MessagePartCount(MAIL_MESSAGE_T * mailmessage);
//first should the text part, then attachment part
//void AddMessagePart(MAIL_MESSAGE_PART_T * messagepart, MAIL_MESSAGE_T * mailmessage);

//void EncodeMail(MAIL_MESSAGE_T * mailmessage);
//void SaveMail(MAIL_MESSAGE_T * mailmessage, wchar * fullname);

BOOLEAN Parseatrrhasdata(char * v);
comm_uint8 * Getcleanvalue(char * v, BOOLEAN quote, MAIL_MESSAGE_T * mailmessage);
int Getmessagedatasize(MAIL_MESSAGE_T * msg);
char * EncapsulateMsg(MAIL_MESSAGE_T * msg);
char * Encodeaddresses(MAIL_ADDRESS_T ** addresses, int count);
char * Encodetextbody(wchar * text);
char * Encodefile(wchar * fullname, int *data_len);//解码后长度
BOOLEAN Base64_encode_fold(const unsigned char *in,  unsigned int inlen, 
                        unsigned char *out, unsigned int *outlen);
MAIL_CONVERT_ATTRS_T * MMIEMAIL_Parseattrs(char * content);
char * GetContent(char * s, char * target, BOOLEAN quotation);
void MMIEMAIL_DestroyAttrs(MAIL_CONVERT_ATTRS_T * attrs_ptr);
#endif

