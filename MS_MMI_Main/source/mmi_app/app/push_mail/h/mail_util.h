/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_util.h
 *	Author		: sunguochen    
 *	Description	: 手机邮箱接口
 *	Vesion		: 1.0        
 *	Date			: 2010-4-29  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-29      1.0           初步编写完成
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
 * Description : 大写转小写字符
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_cmnToLower (int c);



/*==========================================================
 * Function 	: mail_realAddresses2String
 * Author		: sunguochen      
 * Description : 将地址修改成以;分割的地址字符串
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_realAddresses2String( MailAddress *addressList, unsigned int maxLength);



/*==========================================================
 * Function 	: mail_charisspace
 * Author		: sunguochen      
 * Description : 字符是否是空格
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_charisspace( char c );



/*==========================================================
 * Function 	: mail_gnb
 * Author		: sunguochen      
 * Description : 查找字符串中的第一个空格
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_gnb( char *s );



/*==========================================================
 * Function 	: mail_gnnb
 * Author		: sunguochen      
 * Description : 查找字符串中的第一个非空格
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_gnnb( char *s );



/*==========================================================
 * Function 	: mail_trimline
 * Author		: sunguochen      
 * Description :  返回第一个非空格地址
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_trimline( char *s );



/*==========================================================
 * Function 	: mail_bpnoTopLevel
 * Author		: sunguochen      
 * Description : 获取第一个.之前的字符串
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_bpnoTopLevel(const char *bpno, char *buf );



/*==========================================================
 * Function 	: mail_bpnoLevels
 * Author		: sunguochen      
 * Description : 获取地址段的数量
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_bpnoLevels(const char *bpno );



/*==========================================================
 * Function 	: mail_bpnoComponents
 * Author		: sunguochen      
 * Description : 将地址从字符串转为32位数字
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int *mail_bpnoComponents(const char *bpno );



/*==========================================================
 * Function 	: mail_bpnoAssemble
 * Author		: sunguochen      
 * Description : 将32位格式的地址打印为字符串格式地址
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_bpnoAssemble(const int *table, int levels );



/*==========================================================
 * Function 	: mail_bpnoAddToLevel
 * Author		: sunguochen      
 * Description : 将*.*.*.*格式的地址某位增加addThis
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *mail_bpnoAddToLevel(const char *bpno, int addThis, int level );



/*==========================================================
 * Function 	: mail_bpnoAddToTopLevel
 * Author		: sunguochen      
 * Description : 地址第一位增加addThis
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
 * Description : 查找 UTF8 字符结束符
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *count_utf8_str_step_null_check (const char *start, int steps);



/*==========================================================
 * Function 	: chopUTF8string
 * Author		: sunguochen      
 * Description :  UTF8 字符串指定位置分段
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int chopUTF8string( char *s, int nbytes );



/*==========================================================
 * Function 	: find_first_utf8_byte
 * Author		: sunguochen      
 * Description : 查找 UTF8 字符串开始位置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char *find_first_utf8_byte(char *start);



/*==========================================================
 * Function 	: mail_IsDigits
 * Author		: sunguochen      
 * Description : 字符串是不是数字
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
 * Description : 小写字母转为大写字母
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char mail_toUpper(const char toUpper);



/*==========================================================
 * Function 	: mail_islower
 * Author		: sunguochen      
 * Description : 是不是小写字母
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_islower(char c);



/*==========================================================
 * Function 	: mail_uCase
 * Author		: sunguochen      
 * Description : 将字符串的小写字母变为大写
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
char* mail_uCase(char *s, int nOfChars);



/*==========================================================
 * Function 	: findPrevNOTTESTED
 * Author		: sunguochen      
 * Description : 查找字符串中指定字符的位置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int findPrevNOTTESTED(const char* data, int count, char chr);



/*==========================================================
 * Function 	: mail_SearchDigitStr
 * Author		: sunguochen      
 * Description : 从字符串中找出第一个数字串
                      char * str_ptr  --字符串
                      int str_len      -- 字符串长度
                      int* digit_num--找出的数字串结束位置，用于下一次查找
 * Parameter	: type          parameter       Description
 * Return		: char* 数字串，需要释放内存
 * Effect		: NO
 * History	: created  5/8/2010 
 ==========================================================*/
char* mail_SearchDigitStr(char * str_ptr, int str_len, char** digit_end);



/*==========================================================
 * Function 	: mail_printBuf
 * Author		: sunguochen      
 * Description : 分行打印log
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
 * Description : base64解码后长度
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int msf_base64_decode_len (const char *str, int len);



/*==========================================================
 * Function 	: mail_base64_decode
 * Author		: sunguochen      
 * Description : base64解码
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_base64_decode (const char *inbuf, int inlen, char *outbuf, int *outlen);



/*==========================================================
 * Function 	: mail_base64_decode_ext
 * Author		: sunguochen      
 * Description : base64解码
 * Parameter	: type          parameter       Description
 * Return		: Void
                       -1 失败
                       0  成功
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
int mail_base64_decode_ext (const unsigned char *inbuf, comm_uint32 *inlen, char *outbuf, comm_uint32 *outlen);



/*==========================================================
 * Function 	: mail_base64_encode_len
 * Author		: sunguochen      
 * Description : 计算字符base64编码后的长度
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/23/2010 
 ==========================================================*/
int mail_base64_encode_len (int len);



/*==========================================================
 * Function 	: mail_base64_encode
 * Author		: sunguochen      
 * Description : base64编码
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
 * Description : utf8 转化成unicode
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/30/2010 
 ==========================================================*/
 unsigned int mail_Utf82Unicode( CONST unsigned char * utf8_ptr,unsigned char * unicode_ptr,unsigned int unicode_max_size);
/*==========================================================
 * Function 	: mail_GetUnicodeLen
 * Author		: lixu      
 * Description : 返回unicode字符长度
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
 * Description : 将地址修改成以;分割的地址字符串
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
 * Description : 将邮件时间从字符串转为以秒标记的数
                      至少有两种格式
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
comm_boolean mail_DateStrToSeconds (const char *s, comm_uint32 *t);



/*==========================================================
 * Function 	: mail_DateStrToTime
 * Author		: sunguochen      
 * Description : 将邮件时间从字符串转为以秒标记的数
                      至少有两种格式
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
comm_boolean mail_DateStrToTime (const char *s, MMI_TM_T *t);



/*==========================================================
 * Function 	: mail_DateStrToTime
 * Author		: sunguochen      
 * Description : 从时间转为字符串
                      返回指针需要释放
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
char* mail_DateTimeToStr (MMI_TM_T *time);



/*==========================================================
 * Function 	: mail_Wstrrchr
 * Author		: sunguochen      
 * Description : 获取wstr中c 的位置
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
wchar* mail_Wstrrchr(wchar* src, int src_len, char c);



/*==========================================================
 * Function 	: mail_isValidEmailAddress
 * Author		: sunguochen      
 * Description :  判断是否为合法email地址
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
 * Description : 从文件后缀获得mime类型
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/10/2010 
 ==========================================================*/
char * mail_mimeExt_to_mimeTpye(const char *mime_ext);



/*==========================================================
 * Function 	: mail_mimeTypeFromName
 * Author		: sunguochen      
 * Description : 从文件名获得类型
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/10/2010 
 ==========================================================*/
char * mail_mimeTypeFromName(wchar * file_name);



/*==========================================================
 * Function 	: mail_StrnToWstr
 * Author		: sunguochen      
 * Description : 转化指定长度数据
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
 * Description :  扩充此函数，因为平台最多支持uint16
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





