/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_util.h
 *	Author		: sunguochen    
 *	Description	: �ֻ�����ӿ�
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_UTIL_H_
#define _MAIL_UTIL_H_



#include "mail_type.h"
#include "mmicom_time.h"

#define MAIL_DATE_STR_LEN                   50


#define EMAIL_ADDR_LEN_MIN   3
#define EMAIL_ADDR_LEN_MAX   255

#define MAIL_UNREFERENCED_PARAMETER(P)      (void)(P)


/*==========================================================
 * Function 	: mail_cmnToLower
 * Author		: sunguochen      
 * Description : ��дתСд�ַ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_cmnToLower (int c);



/*==========================================================
 * Function 	: mail_realAddresses2String
 * Author		: sunguochen      
 * Description : ����ַ�޸ĳ���;�ָ�ĵ�ַ�ַ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_realAddresses2String( MailAddress *addressList, unsigned int maxLength);



/*==========================================================
 * Function 	: mail_charisspace
 * Author		: sunguochen      
 * Description : �ַ��Ƿ��ǿո�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_charisspace( char c );



/*==========================================================
 * Function 	: mail_gnb
 * Author		: sunguochen      
 * Description : �����ַ����еĵ�һ���ո�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_gnb( char *s );



/*==========================================================
 * Function 	: mail_gnnb
 * Author		: sunguochen      
 * Description : �����ַ����еĵ�һ���ǿո�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_gnnb( char *s );



/*==========================================================
 * Function 	: mail_trimline
 * Author		: sunguochen      
 * Description :  ���ص�һ���ǿո��ַ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_trimline( char *s );



/*==========================================================
 * Function 	: mail_bpnoTopLevel
 * Author		: sunguochen      
 * Description : ��ȡ��һ��.֮ǰ���ַ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_bpnoTopLevel(const char *bpno, char *buf );



/*==========================================================
 * Function 	: mail_bpnoLevels
 * Author		: sunguochen      
 * Description : ��ȡ��ַ�ε�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_bpnoLevels(const char *bpno );



/*==========================================================
 * Function 	: mail_bpnoComponents
 * Author		: sunguochen      
 * Description : ����ַ���ַ���תΪ32λ����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int *mail_bpnoComponents(const char *bpno );



/*==========================================================
 * Function 	: mail_bpnoAssemble
 * Author		: sunguochen      
 * Description : ��32λ��ʽ�ĵ�ַ��ӡΪ�ַ�����ʽ��ַ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_bpnoAssemble(const int *table, int levels );



/*==========================================================
 * Function 	: mail_bpnoAddToLevel
 * Author		: sunguochen      
 * Description : ��*.*.*.*��ʽ�ĵ�ַĳλ����addThis
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_bpnoAddToLevel(const char *bpno, int addThis, int level );



/*==========================================================
 * Function 	: mail_bpnoAddToTopLevel
 * Author		: sunguochen      
 * Description : ��ַ��һλ����addThis
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_bpnoAddToTopLevel(const char *bpno, int addThis );



/*==========================================================
 * Function 	: mail_convertToUTF8
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char* mail_convertToUTF8(const char *bufPointer, unsigned int bufLen, comm_int32 charset);



/*==========================================================
 * Function 	: count_utf8_str_step_null_check
 * Author		: sunguochen      
 * Description : ���� UTF8 �ַ�������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *count_utf8_str_step_null_check (const char *start, int steps);



/*==========================================================
 * Function 	: chopUTF8string
 * Author		: sunguochen      
 * Description :  UTF8 �ַ���ָ��λ�÷ֶ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int chopUTF8string( char *s, int nbytes );



/*==========================================================
 * Function 	: find_first_utf8_byte
 * Author		: sunguochen      
 * Description : ���� UTF8 �ַ�����ʼλ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *find_first_utf8_byte(char *start);



/*==========================================================
 * Function 	: mail_IsDigits
 * Author		: sunguochen      
 * Description : �ַ����ǲ�������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_IsDigits(const char *str);
/*==========================================================
 * Function 	: 
 * Author		:lixu
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	:  
 ==========================================================*/
 int mail_ishex(const char c);
/*==========================================================
 * Function 	: 
 * Author		:lixu
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	:  
 ==========================================================*/
int mail_hex2int (const char *s);


/*==========================================================
 * Function 	: mail_toUpper
 * Author		: sunguochen      
 * Description : Сд��ĸתΪ��д��ĸ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char mail_toUpper(const char toUpper);



/*==========================================================
 * Function 	: mail_islower
 * Author		: sunguochen      
 * Description : �ǲ���Сд��ĸ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_islower(char c);



/*==========================================================
 * Function 	: mail_uCase
 * Author		: sunguochen      
 * Description : ���ַ�����Сд��ĸ��Ϊ��д
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char* mail_uCase(char *s, int nOfChars);



/*==========================================================
 * Function 	: findPrevNOTTESTED
 * Author		: sunguochen      
 * Description : �����ַ�����ָ���ַ���λ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int findPrevNOTTESTED(const char* data, int count, char chr);



/*==========================================================
 * Function 	: mail_SearchDigitStr
 * Author		: sunguochen      
 * Description : ���ַ������ҳ���һ�����ִ�
                      char * str_ptr  --�ַ���
                      int str_len      -- �ַ�������
                      int* digit_num--�ҳ������ִ�����λ�ã�������һ�β���
 * Parameter	: type          parameter       Description
 * Return		: char* ���ִ�����Ҫ�ͷ��ڴ�
 * Effect		: NO
 * History	: created  5/8/2010 
 ==========================================================*/
char* mail_SearchDigitStr(char * str_ptr, int str_len, char** digit_end);



/*==========================================================
 * Function 	: mail_printBuf
 * Author		: sunguochen      
 * Description : ���д�ӡlog
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
void mail_printBuf(const unsigned char *buf, comm_uint32 bufLen);




/*==========================================================
 * Function 	: mail_crptCreateMd5Hash
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char* mail_crptCreateMd5Hash(const unsigned char* inData, comm_uint32 inDataLen);



/*==========================================================
 * Function 	: mail_strtok_r
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_strtok_r(char *string, const char *control, char **nextoken);



/*==========================================================
 * Function 	: msf_base64_decode_len
 * Author		: sunguochen      
 * Description : base64����󳤶�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int msf_base64_decode_len (const char *str, int len);



/*==========================================================
 * Function 	: mail_base64_decode
 * Author		: sunguochen      
 * Description : base64����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_base64_decode (const char *inbuf, int inlen, char *outbuf, int *outlen);



/*==========================================================
 * Function 	: mail_base64_decode_ext
 * Author		: sunguochen      
 * Description : base64����
 * Parameter	: type          parameter       Description
 * Return		: Void
                       -1 ʧ��
                       0  �ɹ�
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_base64_decode_ext (const unsigned char *inbuf, comm_uint32 *inlen, char *outbuf, comm_uint32 *outlen);



/*==========================================================
 * Function 	: mail_base64_encode_len
 * Author		: sunguochen      
 * Description : �����ַ�base64�����ĳ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/23/2010 
 ==========================================================*/
int mail_base64_encode_len (int len);



/*==========================================================
 * Function 	: mail_base64_encode
 * Author		: sunguochen      
 * Description : base64����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_base64_encode (const char *inbuf, int inlen, char *outbuf, int *outlen);
/*==========================================================
 * Function 	: mail_countEmailAddresses
 * Author		: sunguochen      
 * Description :
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/23/2010 
 ==========================================================*/
comm_uint32 mail_countEmailAddresses(const MailAddress *address);
/*==========================================================
 * Function 	: mail_freeEmailAddress
 * Author		: lixu      
 * Description :
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/23/2010 
 ==========================================================*/
void mail_freeEmailAddress(MailAddress *address);
/*==========================================================
 * Function 	: mail_freeEmailEncText
 * Author		: lixu      
 * Description :
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/23/2010 
 ==========================================================*/
void mail_freeEmailEncText(MailEncodedText *eText);
/*==========================================================
 * Function 	: mail_freeEmailAddress
 * Author		: lixu      
 * Description :
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/23/2010 
 ==========================================================*/
void mail_freeEmailHeader(MailHeader *header);

/*==========================================================
 * Function 	: mail_Utf82Unicode
 * Author		: lixu      
 * Description : utf8 ת����unicode
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
 unsigned int mail_Utf82Unicode( CONST unsigned char * utf8_ptr,unsigned char * unicode_ptr,unsigned int unicode_max_size);
/*==========================================================
 * Function 	: mail_GetUnicodeLen
 * Author		: lixu      
 * Description : ����unicode�ַ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
unsigned int  mail_GetUnicodeLen( const unsigned char * uc_ptr );
/*****************************************************************************/
// 	Description : convert utf8 to ucs big endian
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
uint16 mail_UTF8ToUnicode(//ucs2b len
					uint8 *ucs2b_ptr,//out
					uint16 ucs2b_buf_len,//in
					const uint8 *utf8_ptr,//in
					uint16 utf8_len//in
					);
/*==========================================================
 * Function 	: mail_address2String
 * Author		: lixu  
 * Description : ����ַ�޸ĳ���;�ָ�ĵ�ַ�ַ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
 char *mail_address2String(MAIL_ADDRESS_T    	*address,uint16 count,
                                                  unsigned int             
                                                  max_length);
#endif 



/*==========================================================
 * Function 	: mail_DateStrToSeconds
 * Author		: sunguochen      
 * Description : ���ʼ�ʱ����ַ���תΪ�����ǵ���
                      ���������ָ�ʽ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
comm_boolean mail_DateStrToSeconds (const char *s, comm_uint32 *t);



/*==========================================================
 * Function 	: mail_DateStrToTime
 * Author		: sunguochen      
 * Description : ���ʼ�ʱ����ַ���תΪ�����ǵ���
                      ���������ָ�ʽ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
comm_boolean mail_DateStrToTime (const char *s, MMI_TM_T *t);



/*==========================================================
 * Function 	: mail_DateStrToTime
 * Author		: sunguochen      
 * Description : ��ʱ��תΪ�ַ���
                      ����ָ����Ҫ�ͷ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
char* mail_DateTimeToStr (MMI_TM_T *time);



/*==========================================================
 * Function 	: mail_Wstrrchr
 * Author		: sunguochen      
 * Description : ��ȡwstr��c ��λ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
wchar* mail_Wstrrchr(wchar* src, int src_len, char c);



/*==========================================================
 * Function 	: mail_isValidEmailAddress
 * Author		: sunguochen      
 * Description :  �ж��Ƿ�Ϊ�Ϸ�email��ַ
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/1/2010 
 ==========================================================*/
comm_boolean mail_isValidEmailAddress(char *addr);



/*****************************************************************************/
// 	Description : UTF8 to wstr 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int mail_UTF8ToWstr(//ucs2b len
                             wchar *wstr_ptr,//out
                             int     wstr_len,//in
                             const uint8 *utf8_ptr,//in
                             int     utf8_len//in
                             );



/*==========================================================
 * Function 	: mail_mimeExt_to_mimeTpye
 * Author		: sunguochen      
 * Description : ���ļ���׺���mime����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/10/2010 
 ==========================================================*/
char * mail_mimeExt_to_mimeTpye(const char *mime_ext);



/*==========================================================
 * Function 	: mail_mimeTypeFromName
 * Author		: sunguochen      
 * Description : ���ļ����������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/10/2010 
 ==========================================================*/
char * mail_mimeTypeFromName(wchar * file_name);



/*==========================================================
 * Function 	: mail_StrnToWstr
 * Author		: sunguochen      
 * Description : ת��ָ����������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/15/2010 
 ==========================================================*/
wchar *mail_StrnToWstr(const char* src, wchar* dst, int len);



/*==========================================================
 * Function 	: Mail_DecodeQuoted
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  7/5/2010 
 ==========================================================*/
int Mail_DecodeQuoted(const char* pSrc, unsigned char* pDst, int nSrcLen);



/*==========================================================
 * Function 	: mail_GBKToWstr
 * Author		: sunguochen      
 * Description :  ����˺�������Ϊƽ̨���֧��uint16
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  7/8/2010 
 ==========================================================*/
int mail_GBKToWstr(					    //return the number of UCS2 code that have been converted.
                            wchar *wstr_ptr, 	    //destination: string pointer to UCS2 string.
                            const char *gbk_ptr,   //source: string pointer to GBK string
                            int str_len			//the length to convert from GBK to UCS2.
                            );


/*==========================================================
 * Function 	: mail_GBToWstr
 * Author		: sunguochen      
 * Description : 
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  7/8/2010 
 ==========================================================*/
 int mail_GBToWstr(					//return the number of UCS2 code that have been converted.
                           wchar *wstr_ptr, 	//destination: string pointer to UCS2 string.
                           const char *gb_ptr, //source: string pointer to GB string
                           int str_len			//the length to convert from GB to UCS2.
                           );





