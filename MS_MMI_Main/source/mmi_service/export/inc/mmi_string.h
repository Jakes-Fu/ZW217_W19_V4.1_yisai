/****************************************************************************
** File Name:      mmi_string.h                                         *
** Author:                                                                 *
** Date:           1/4/2008                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2008        jianma       Create
** 
****************************************************************************/
#ifndef __MMI_STRING_H__
#define __MMI_STRING_H__

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/
#include "mmi_base_common.h"

#include "sci_types.h"
#include "mmi_font.h"
#include "mmk_type.h"
#include "caf.h"

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
#define SWAP_UINT16(x1, x2) {uint16 temp; temp = (x1); (x1) = (x2); (x2) = temp;}

#define ISLOWER(c) ('a' <= (c) && (c) <= 'z')
#define ISUPPER(c) ('A' <= (c) && (c) <= 'Z')
#define TOLOWER(c) (ISUPPER(c) ? 'a' + ((c) - 'A') : (c))
#define TOUPPER(c) (ISLOWER(c) ? 'A' + ((c) - 'a') : (c))

#define EURO_UNICODE    0x20AC
/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*------------------------------char string API start -------------------------------*/
/*****************************************************************************/
//  Description : 复制字符串
//  Global resource dependence : none
//  Parameter: out_len_ptr  : return the copied string length 
//             out_alpha_ptr: return the copied string
//             in_len       : the length of string to be copied
//             max_len      : the max length of out_len_ptr
//             in_alpha_ptr : string of to be copied to out_len_ptr
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_CopyString(
                                 uint32      *out_len_ptr,       //OUT:
                                 uint8       *out_alpha_ptr,     //OUT:
                                 uint32      in_len,             //IN:
                                 uint32      max_len,            //IN:
                                 uint8       *in_alpha_ptr       //IN:
                                 );

/*******************************************************************************************/
// 	Description : compare string, 
//                if str1 > str2, return >0; 
//                   str1 = str2, return =0; 
//                   str1 < str2, return <0
//	Global resource dependence :
//  Author: lin.lin
//	Note:
/*********************************************************************************************/
PUBLIC int32 MMIAPICOM_StrCmp(
							  uint8* str1,         //compared string
							  uint16 str1_len,     //string length
							  uint8* str2,         //compared string
							  uint16 str2_len      //string length
							  );

/*****************************************************************************/
//  Description : compare string, 
//                if str1 > str2, return >0; 
//                   str1 = str2, return =0; 
//                   str1 < str2, return <0
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_CompareTwoUint8String(
	                        uint8    *str1_ptr,  //compared string
	                        uint16    str1_len,   //string length,字节数
	                        uint8    *str2_ptr,  //compared string
	                        uint16    str2_len    //string length,字节数
	                        );

/*****************************************************************************/
//  Description : cmp two string
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_Stricmp( 
							   const uint8 *str1, 
							   const uint8 *str2
							   );

/*****************************************************************************/
//  Description : 
// This function searches for a given character in a string. If it is found
// a pointer to that character is returned. If it is not found a NULL
// pointer is returned. If the the given string is in unicode it is searched
// for the given char in unicode format. Only searching for default GSM
// alphabet characters is possible. The parameter i2_text is given in
// default GSM alphabet.
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC uint8 *MMIAPICOM_SearchChar(
								   uint8  *i1_text_ptr, 
								   int8  i2_char);


/*****************************************************************************/
//  Description : stristr(str1, str2) - look for str2 (case insensitive) inside str1
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC char * MMIAPICOM_Stristr(char * str1, char * str2);

/*------------------------------char string API end -------------------------------*/

/*--------------------------char/wchar string API start ---------------------------*/

/*****************************************************************************/
//  Description : str lower
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPICOM_Strlower( char * string );

/*****************************************************************************/
//  Description : str upper
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPICOM_Strupper( char * string );

/*****************************************************************************/
//  Description : str to wstr
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_StrToWstr(
								  const uint8* src,
								  wchar* dst
								  );

/*****************************************************************************/
//  Description : wstr to str
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPICOM_WstrToStr( 
								  const wchar* src,
								  uint8* dst
								  );

/*****************************************************************************/
//  Description : str to wstr, for 5 param
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMI_STRNTOWSTR(
							 wchar* dst,
							 size_t dst_max_count,
							 const uint8* src,
							 size_t src_max_count,
							 size_t count
							 );

/*****************************************************************************/
//  Description : 把string对应的字符串连接到一个字符串中
//  Global resource dependence : 
//  Author:yongwei.he
//  Note: used to textbox
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrcatFromStrToUCS2(
	uint16  *ucs2_str_ptr,    //in, out
	uint16  *ucs2_str_len_ptr,    //in, out, uint16 numbers, not bytes
	const uint8 *str_ptr,   //in
	uint16   str_len
	);

/*****************************************************************************/
//  Description : wstr to str, for 5 param
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMI_WSTRNTOSTR( 
							 uint8* dst,
							 size_t dst_max_count,
							 const wchar* src,
							 size_t src_max_count,
							 size_t count
							 );

/*****************************************************************************/
//  Description : convert ascii string to unicode string(UCS2)
//  Global resource dependence : none
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_ConvertAlphabetToUnicode(
	uint8 *i1_default_text_ptr,         // alphabet
	uint8 *o1_unicode_text_ptr,     // unicode
	int16  i2_output_buf_size
	);       // length of unicode

/*****************************************************************************/
//  Description : wstr str cmp
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_WstrStrncmp(
                                 const wchar* wstr_ptr,
                                 const uint8*  str_ptr,
                                 size_t count
                                 );
/*--------------------------- char/wchar string API end -----------------------------*/

/*---------------------------- wchar string API start -------------------------------*/
/*****************************************************************************/
//  Description : wstrcpy
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrcpy( 
								wchar* dst,
								const wchar* src
								);

/*****************************************************************************/
//  Description : wstrncpy
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrncpy( 
								 wchar* dst,
								 const wchar* src,
								 size_t count
								 );

/*****************************************************************************/
//  Description : wstrncpy, for 5 param
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMI_WSTRNCPY(
						   wchar* dst,
						   size_t dst_max_count,
						   const wchar* src,
						   size_t src_max_count,
						   size_t count
						   );

/*****************************************************************************/
//  Description : wstrcat
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrcat( 
								wchar* dst,
								const wchar* src
								);

/*****************************************************************************/
//  Description : wstrncat
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrncat( 
								 wchar* dst,
								 const wchar* src,
								 size_t count
								 );

/*****************************************************************************/
//  Description : wstrcmp
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_Wstrcmp(
							 const wchar * string1,
							 const wchar * string2
							 );

/*****************************************************************************/
//  Description : wstrncmp
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_Wstrncmp(
							  const wchar* string1,
							  const wchar* string2,
							  size_t count
							  );

/*****************************************************************************/
//  Description : compare string, 
//                if str1 > str2, return >0; 
//                   str1 = str2, return =0; 
//                   str1 < str2, return <0
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_CompareTwoWstr(
									  const wchar    *wstr1_ptr,  //compared string
									  uint16         wstr1_len,   //string length,字节数
									  const wchar    *wstr2_ptr,  //compared string
									  uint16         wstr2_len    //string length,字节数
									  );

/*****************************************************************************/
//  Description : compare string, 
//                if str1 > str2, return >0; 
//                   str1 = str2, return =0; 
//                   str1 < str2, return <0
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_CompareTwoWstrExt(
	const wchar    *wstr1_ptr,  //wchar string1
	uint16         wstr1_len,   //string length
	const wchar    *wstr2_ptr,  //wchar string2
	uint16         wstr2_len,   //string length
	BOOLEAN        str_flag     //是否字符串区分大小写
	);

int MMIAPICOM_Wstrzcmp(const wchar *s1, size_t n1, const wchar *s2, size_t n2);

int MMIAPICOM_Wstrzcmp_h_n(const wchar *s1H, size_t n1, const wchar *s2N, size_t n2);

/*****************************************************************************/
//  Description : compair string by its pinyin string
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_WstrncmpByPinYin(
									  const wchar* string1,
									  const wchar* string2,
									  size_t count
									  );

/*****************************************************************************/
//  Description : wstrlen
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC size_t MMIAPICOM_Wstrlen( const wchar* str );

/*****************************************************************************/
//  Description : wstrnlen
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC size_t MMIAPICOM_Wstrnlen( const wchar* str, size_t max_len );

/*****************************************************************************/
//  Description : check whether string1 contain string2
//  Global resource dependence : 
//  Author:   liqing.peng 2006-7-16
//  RETRUN: 
//  Note:   
/*****************************************************************************/
wchar* MMIAPICOM_Wstrstr(wchar *str1, wchar *str2);

/*****************************************************************************/
//  Description : check whether string1 contain string2
//  Global resource dependence : 
//  Author:   liqing.peng 2006-7-16
//  RETRUN: 
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_Wstrnstr(
								  const wchar  *str1_ptr,
								  uint16       str1_len,
								  const wchar  *str2_ptr,
								  uint16       str2_len
								  );

/*****************************************************************************/
//  Description : check whether string1 contain string2 ext
//  Global resource dependence : 
//  Author:   tao.xue
//  RETRUN: 
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WstrnstrExt(
									 const wchar  *str1_ptr,
									 uint16       str1_len,
									 const wchar  *str2_ptr,
									 uint16       str2_len,
									 BOOLEAN        str_flag     //是否字符串区分大小写
									 );

/*****************************************************************************/
//  Description : wchar tolower
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar MMIAPICOM_Wchartolower(
									wchar wc
									);

/*****************************************************************************/
//  Description : wchar toupper
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar MMIAPICOM_Wchartoupper(
									wchar wc
									);

/*****************************************************************************/
//  Description : wstr lower
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrlower( wchar * string );

/*****************************************************************************/
//  Description : wstr upper
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPICOM_Wstrupper( wchar * string );

/*----------------------------- wchar string API end --------------------------------*/

/*----------------------------- mmi_string API start --------------------------------*/

/*****************************************************************************/
//    Description : destroy num string
//    Global resource dependence : 
//    Author: James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_DestroyString(
									MMI_STRING_T num_string
									);

/*****************************************************************************/
//  Description : copy unicode string
//  Global resource dependence : none
//  Author: taul.zhu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_UnicodeStrcpy(
									MMI_STRING_T  *o1_dest_text,        // DESTINATION 
									MMI_STRING_T i1_src_text                // SOURCE
									);

/*****************************************************************************/
//  Description : connect some string
//  Global resource dependence :
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_ConnectSomeString(
                                           MMI_STRING_T        *string_in,     //in
                                           uint32              string_num,     //in
                                           MMI_STRING_T        *string_out     //out
                                           );

/*****************************************************************************/
//  Description : merge two string
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_MergeTwoString(
                                        MMI_STRING_T        *string_1,      //in
                                        MMI_STRING_T        *string_2,      //in
                                        MMI_STRING_T        *string_3,      //out
                                        uint16              str3_len        //in
                                        );

/*****************************************************************************/
//     Description : cat two string
//    Global resource dependence : 
//  Author: 
//    Note:    should free the dst_ptr->str_ptr
///*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CatTwoString(    
									  MMI_STRING_T *dst_ptr,        //out.
									  MMI_STRING_T *src1_ptr,        //in.
									  MMI_STRING_T *src2_ptr        //in.
									  );

/*****************************************************************************/
//  Description : 字符串比较函数
//  Global resource dependence : none
//  Author: maqianlin
//  Note:用于比较文字资源和编辑的文字
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StrCompare(
									const MMI_STRING_T          *str_ptr,//in 编辑的文字
									const MMI_STRING_T          *str_info_ptr//in文字资源ID
									);

/*****************************************************************************/
//  Description : 把一个MMI_STRING_T对应的字符串连接到一个字符串中
//  Global resource dependence : 
//  Author:yongwei.he
//  Note: used to textbox
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrcatFromSTRINGToUCS2(
                                             uint16  *ucs2_str_ptr,    //in, out
                                             uint16  *ucs2_str_len_ptr,    //in, out, uint16 numbers, not bytes
                                             const MMI_STRING_T *string_ptr   //in
                                             );

/*****************************************************************************/
//  Description : 把一个text id对应的字符串连接到一个字符串中
//  Global resource dependence : 
//  Author:yongwei.he
//  Note: used to textbox
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrcatFromTextIDToUCS2(
	uint16  *ucs2_str_ptr,    //in, out
	uint16  *ucs2_str_len_ptr,    //in, out, uint16 numbers, not bytes
	MMI_TEXT_ID_T text_id   //in
	);

/*****************************************************************************/
//     Description : cat two string by a textid and a string
//    Global resource dependence : 
//  Author: 
//    Note:    should free the dst_ptr->str_ptr
///*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CatIDAndString(    
										MMI_STRING_T    *dst_ptr,        //out.
										uint32          text_id,        //in.
										MMI_STRING_T    *src2_ptr        //in.
										);


/*****************************************************************************/
//     Description : cat two string by two textids
//    Global resource dependence : 
//  Author: 
//    Note:    should free the dst_ptr->str_ptr
///*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CatTwoID(    
								  MMI_STRING_T      *dst_ptr,        //out.
								  uint32            text_id1,        //in.
								  uint32            text_id2        //in.
								  );
/*------------------------------- mmi_string API end ---------------------------------*/

/*****************************************************************************/
//  Description : Get transmitter display string
//  Global resource dependence : 
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
PUBLIC wchar MMIAPICOM_GetCRCh( void );

/*****************************************************************************/
//  Description : search the same name in album list
//  Global resource dependence : 
//  Author: bruce.chi
//  Return: TRUE: contain invalid char. FALSE: do not contain invalid char
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsIncludeInvalidChar(
	const wchar* name_ptr, 
	uint16 len//, 
	//BOOLEAN is_ucs2
	);

/*****************************************************************************/
//  Description : 判断是否为 UCS2 字符
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsUcs2Char(uint16 ch);

/*****************************************************************************/
//  Description : 判断wstring是否为 ASCII 字符串
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsASCIIString(
                                        const wchar* wstr_ptr, 
                                        uint16 wstr_len
                                        );


/*****************************************************************************/
//  Description : 获取wstr_ptr中以ASCII 字符串开头的长度
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  uint16 MMIAPICOM_GetASCIIStringLen(
                                        const wchar* wstr_ptr, 
                                        uint16 wstr_len
                                        );

/*****************************************************************************/
//  Description : 是否是数字串
//  Global resource dependence : 
//  Author:yelianna.zhao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsNumberString(
                                  uint16* cmd_char,
                                  uint16 len
                                  );

/*****************************************************************************/
//  Description : 计算字符串中ucs2字符的个数
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_CountUcs2Char(uint16 *txt_ptr, uint16 len);

/*****************************************************************************/
//  Description : translate the string to number
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WstrToNum(
								   const wchar* str,
								   uint8   len,
								   uint32*  num                              
								   );

/********************************************************************************
NAME:          Int2Str
DESCRIPTION:   
PARAM IN:      value - 
buf - 
buf_len - 
PARAM OUT:     None
AUTHOR:        allen
DATE:          2004.09.14
********************************************************************************/
PUBLIC void MMIAPICOM_Int2Str(uint32 value, uint8* buf, uint8 buf_len);

/*****************************************************************************/
//  Description : 75进制转换成10进制
//  Global resource dependence : 
//  Author:yelianna.zhao
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_ConvertCmdChar2Num (
                                  uint16* cmd_char,
                                  uint16 len
                                  );

/*****************************************************************************/
//  Description : Get phone number form string
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetPhoneNumberFormString(uint8* string,uint32 len,uint8* num);

/***************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC void MMIAPICOM_GBToSTRING( 
                                 const uint8   *src_ptr,
                                 MMI_STRING_T  *des_str
                                 );

/*****************************************************************************/
// 	Description : copy string to buffer
//	Global resource dependence : 
//  Author: Tracy Zhang
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_CopyStr2Buf(wchar *dst_buf_ptr, MMI_STRING_T const *src_ptr, uint16 max_copy_len);

/*****************************************************************************/
//  Description : to get the int32 value of the buffer
//  Global resource dependence : none
//  Author: arthur.peng
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_GetInt32ValueFromBuf(
	uint8 * dataPtr,
	uint16 len
	);

/*****************************************************************************/
//  Description : to get the uint8 value of the buffer
//  Global resource dependence : none
//  Author: arthur.peng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_GetInt8ValueFromBuf(
	uint8 * dataPtr,
	uint16 len
	);

/*****************************************************************************/
//  Description : to get the length according the param
//  Global resource dependence : none
//  Author: arthur.peng
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIAPICOM_GetInt16ValueFromBuf(
	uint8 * dataPtr,
	uint16 len
	);

/*******************************************************************************************/
//  Description : 获得在指定区域内符串的宽度和长度
//  Global resource dependence :
//  Author: Jassmine
//  Note:   
/*********************************************************************************************/
PUBLIC uint16 MMIAPICOM_GetStrWidth(
									const wchar     *str_ptr,       //in:
									uint16          *str_len_ptr,   //in/out:
									uint16          str_space,      //in:
									uint16          display_width,  //in:
									GUI_FONT_T      font_type       //in:
									);
									
/*****************************************************************************/
//  Description : check if need low
//  Global resource dependence : 
//  Author:CBK
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_WstrnLowCmp(
                              const wchar* string1,
                              const wchar* string2,
                              size_t count
                              );
/*****************************************************************************/
//  Description : skip blank before and after string
//  Global resource dependence : none
//  Author: CBK
//  Note:skip blank before compare for smart compare
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_StrSmartCompare(
                                    const MMI_STRING_T          *str_ptr,//in 编辑的文字
                                    const MMI_STRING_T          *str_info_ptr,//in文字资源ID
                                    BOOLEAN ifLow,  // if need low
                                    BOOLEAN ifNeedCheckTail
                                    );
PUBLIC BOOLEAN MMIAPICON_ConvertStringtoNumber(uint8 *str_ptr, uint8 length, uint32 *num_ptr);

PUBLIC BOOLEAN  MMIAPICOM_CustomGenerateNumReplaceStrByTextId(
MMI_TEXT_ID_T text_id,
const wchar* find_str,
uint16 num,
uint16 result_max_len,
MMI_STRING_T *result_str//out
);

PUBLIC BOOLEAN  MMIAPICOM_CustomGenerateFloatNumReplaceStrByTextId
(
MMI_TEXT_ID_T text_id,
const wchar* find_str,
float num,
uint16 result_max_len,
MMI_STRING_T *result_str
);

#endif  /* __MMI_STRING_H__ */
