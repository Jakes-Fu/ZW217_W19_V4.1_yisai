/*! @file gui_ucs2b_converter.h
@brief ���ļ��ṩ�˸����������͵��ַ���֮���ת��
@author songbin.zeng 
@version 1.0
@date 2011/02/18
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 08/07/2006    songbin.zeng           Create 
*******************************************************************************/

/*! @page page_Ucs2b_converter Ucs2b_converter

(����)

*******************************************************************************/

/*! @addtogroup ucs2b_converterGrp Ucs2b_converter
@brief ucs2b_converterģ��
@details
��ģ���ṩ�����������͵��ַ���֮���ת��
@{
*******************************************************************************/

#ifndef _GUI_UCS_CONVERTER_H_
#define _GUI_UCS_CONVERTER_H_
#include "sci_types.h"
#include "ffs.h"
/*----------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*! @enum GUI_CHAR_CODE_TYPE_E
@brief �ַ���������
*/
typedef enum
{
	GUI_CHAR_CODE_TYPE_MIN = 0,
	GUI_CHAR_CODE_ANSI,                 /*!< ANSI���� */
	GUI_CHAR_CODE_ANSI_BIG5,            /*!< ANSI BIG5���� */
	GUI_CHAR_CODE_UCS2_LE,              /*!< UCS2С�˱��� */
	GUI_CHAR_CODE_UCS2_BE,              /*!< UCS2��˱��� */
	GUI_CHAR_CODE_UTF8,                 /*!< UTF8���� */
    GUI_CHAR_CODE_WINDOW1251,
    GUI_CHAR_CODE_CP855,
    GUI_CHAR_CODE_CP866,
    GUI_CHAR_CODE_KOI8R,
    GUI_CHAR_CODE_MACCYRILLIC,
	GUI_CHAR_CODE_TYPE_MAX
} GUI_CHAR_CODE_TYPE_E;

/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ���UCS2�ַ���ת����С��UCS2�ַ���
@author songbin.zeng
@param ucs2l_ptr     [in/out] ָ��ת�����С��UCS2�ַ���
@param ucs2l_buf_len [in] С���ַ�����buffer��С
@param ucs2b_ptr     [in] ���UCS2�ַ���Դ
@param ucs2b_len     [in] ���UCS2�ַ����ĳ���
@return ʵ��ת���ĸ���
@note
ucs2l_buf_len�������ucs2b_len
*******************************************************************************/
PUBLIC uint32 GUI_UCS2B2UCS2L(//ucs2l len
                              uint8 *ucs2l_ptr,//out
                              uint32 ucs2l_buf_len,//in
                              const uint8 *ucs2b_ptr,//in
                              uint32 ucs2b_len//in
                              );


/***************************************************************************//*!
@brief С��UCS2�ַ���ת���ɴ��UCS2�ַ���
@author songbin.zeng
@param ucs2b_ptr     [in/out] ָ��ת����Ĵ��UCS2�ַ���
@param ucs2b_buf_len [in] ����ַ�����buffer��С
@param ucs2l_ptr     [in] С��UCS2�ַ���Դ
@param ucs2l_len     [in] С��UCS2�ַ����ĳ���
@return ʵ��ת���ĸ���
@note 
ucs2b_buf_len�������ucs2l_len
*******************************************************************************/
PUBLIC uint32 GUI_UCS2L2UCS2B(//ucs2b len
                              uint8 *ucs2b_ptr,//out
                              uint32 ucs2b_buf_len,//in
                              const uint8 *ucs2l_ptr,//in
                              uint32 ucs2l_len//in
                              );


/***************************************************************************//*!
@brief UCS2�ַ���ת����UTF8�ַ���
@author Lianxiang
@param utf8_ptr     [in/out] ָ��ת�����UTF8�ַ���
@param utf8_buf_len [in] utf8_ptr�Ĵ�С
@param wstr_ptr     [in] UCS2�ַ���
@param wstr_len     [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToUTF8(//utf8 len
                             uint8 *utf8_ptr,//out
                             uint32 utf8_buf_len,//in
                             const wchar *wstr_ptr,//in
                             uint32 wstr_len//in
                             );


/***************************************************************************//*!
@brief UCS2�ַ���ת����UTF8�ַ���
@author Lianxiang
@param utf8_ptr     [in/out] ָ��ת�����UTF8�ַ���
@param utf8_buf_len [in] utf8_ptr�Ĵ�С
@param wstr_ptr     [in] UCS2�ַ���Դ
@param wstr_len     [in] �ַ�������
@return ʵ��ת���ĸ���
@note
��";"ת����"\;"���÷���ר��ΪVCARD�ṩ
*******************************************************************************/
PUBLIC uint32 GUI_WstrToUTF8_vCard(//utf8 len
                             uint8 *utf8_ptr,//out
                             uint32 utf8_buf_len,//in
                             const wchar *wstr_ptr,//in
                             uint32 wstr_len//in
                             );


/***************************************************************************//*!
@brief UTF8�ַ���ת����UCS2�ַ���
@author Lianxiang
@param wstr_ptr [in/out] ָ��ת�����UCS2�ַ���
@param wstr_len [in] wstr_ptr�Ĵ�С
@param utf8_ptr [in] UTF8�ַ���
@param utf8_len [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_UTF8ToWstr(//ucs2b len
                             wchar *wstr_ptr,//out
                             uint32 wstr_len,//in
                             const uint8 *utf8_ptr,//in
                             uint32 utf8_len//in
                             );


/***************************************************************************//*!
@brief UCS2�ַ���ת����ָ���������͵��ַ���
@author Lianxiang
@param code_type    [in] ָ��Ŀ���������
@param code_ptr     [in/out] ָ��ת������ַ���
@param code_buf_len [in] code_ptr�Ĵ�С
@param wstr_ptr     [in] UCS2�ַ���
@param wstr_len     [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToOtherCode(//othercode len
                                  GUI_CHAR_CODE_TYPE_E code_type,//in
                                  uint8 *code_ptr,//out
                                  uint32 code_buf_len,//in
                                  const wchar *wstr_ptr,//in
                                  uint32 wstr_len//in
                                  );


/***************************************************************************//*!
@brief  ָ���������͵��ַ���ת����UCS2�ַ���
@author Lianxiang
@param wstr_ptr  [in/out] ָ��ת�����UCS2�ַ���
@param wstr_len  [in] wstr_ptr�Ĵ�С
@param code_type [in] Դ�ַ���������
@param code_ptr  [in] Դ�ַ���
@param code_len  [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_OtherCodeToWstr(//ucs2b len
                                  wchar *wstr_ptr,//out
                                  uint32 wstr_len,//in
                                  GUI_CHAR_CODE_TYPE_E code_type,//in
                                  const uint8 *code_ptr,//in
                                  uint32 code_len//in
                                  );



/***************************************************************************//*!
@brief GB������ַ���ת����UCS2�ַ�����������Դ�ַ�������ת���ĵ�ַ
@author Yintang.ren
@param wstr_ptr    [in/out] ָ��ת�����UCS2�ַ���
@param gb_ptr      [in] GB������ַ���
@param str_len     [in] ��Ҫת�����ַ�����
@param src_end_pos [in/out] Դ�ַ�������ת���ĵ�ַ
@return ʵ��ת���ĸ���
@note
*******************************************************************************/
PUBLIC uint32 GUI_GBToWstrWithEndPos(					  //return the number of UCS2 code that have been converted.
									 wchar *wstr_ptr, 	  //destination: string pointer to UCS2 string.
									 const uint8 *gb_ptr, //source: string pointer to GB string
									 uint32 str_len,	  //the length to convert from GB to UCS2.
									 uint32 *src_end_pos  //the end pos of src string
									 );


/***************************************************************************//*!
@brief UTF8������ַ���ת����UCS2�ַ�����������Դ�ַ�������ת���ĵ�ַ
@author Lianxiang
@param wstr_ptr    [in/out] ָ��ת�����UCS2�ַ���
@param wstr_len    [in] wstr_ptr�Ĵ�С
@param utf8_ptr    [in] UTF8������ַ���
@param utf8_len    [in] �ַ�������
@param src_end_pos [in/out] Դ�ַ�������ת���ĵ�ַ
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_UTF8ToWstrWithEndPos(//ucs2b len
									   wchar *wstr_ptr,//out
									   uint32 wstr_len,//in
									   const uint8 *utf8_ptr,//in
									   uint32 utf8_len,//in
									   uint32 *src_end_pos
									   );


/***************************************************************************//*!
@brief ���UCS2������ַ���ת����С��UCS2�ַ�����������Դ�ַ�������ת���ĵ�ַ
@author Lianxiang
@param ucs2l_ptr     [in/out] ָ��ת�����С��UCS2�ַ���
@param ucs2l_buf_len [in] ucs2l_ptr�Ĵ�С
@param ucs2b_ptr     [in] ��˵�UCS2������ַ���
@param ucs2b_len     [in] �ַ�������
@param src_end_pos   [in/out] Դ�ַ�������ת���ĵ�ַ
@return ʵ��ת���ĸ���
@return 
@note 
*******************************************************************************/
PUBLIC uint32 GUI_UCS2B2UCS2LWithEndPos(//ucs2l len
										uint8 *ucs2l_ptr,//out
										uint32 ucs2l_buf_len,//in
										const uint8 *ucs2b_ptr,//in
										uint32 ucs2b_len,//in
										uint32 *src_end_pos
										);


/***************************************************************************//*!
@brief С��UCS2������ַ���ת���ɴ��UCS2�ַ�����������Դ�ַ�������ת���ĵ�ַ
@author Lianxiang
@param ucs2b_ptr     [in/out] ָ��ת����Ĵ��UCS2�ַ���
@param ucs2b_buf_len [in] ucs2b_ptr�Ĵ�С
@param ucs2l_ptr     [in] С�˵�UCS2������ַ���
@param ucs2l_len     [in] �ַ�������
@param src_end_pos   [in/out] Դ�ַ�������ת���ĵ�ַ
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_UCS2L2UCS2BWithEndPos(//ucs2b len
										uint8 *ucs2b_ptr,//out
										uint32 ucs2b_buf_len,//in
										const uint8 *ucs2l_ptr,//in
										uint32 ucs2l_len,//in
										uint32 *src_end_pos
										);


/***************************************************************************//*!
@brief ָ���������͵��ַ���ת����UCS2�ַ�����������Դ�ַ�������ת���ĵ�ַ
@author Lianxiang
@param wstr_ptr    [in/out] ָ��ת����Ĵ��UCS2�ַ���
@param wstr_len    [in] ucs2b_ptr�Ĵ�С
@param code_type   [in] С�˵�UCS2������ַ���
@param code_ptr    [in] �ַ�������
@param code_len    [in] Դ�ַ�������ת���ĵ�ַ
@param src_end_pos [in/out] Դ�ַ�������ת���ĵ�ַ
@return ʵ��ת���ĸ���
@note
*******************************************************************************/
PUBLIC uint32 GUI_OtherCodeToWstrWithEndPos(//ucs2b len
											wchar *wstr_ptr,//out
											uint32 wstr_len,//in
											GUI_CHAR_CODE_TYPE_E code_type,//in
											const uint8 *code_ptr,//in
											uint32 code_len, //in
											uint32 *src_end_pos
											);
											

/***************************************************************************//*!
@brief ��cp1252�ַ���ת��ΪUCS2�ַ���
@author James.Zhang
@param wstr_ptr   [in/out] ָ��ת�����UCS2�ַ���
@param cp1252_ptr [in] cp1252�ַ���
@param str_len    [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_CP1252ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp1252_ptr,    //source: string pointer to cp1252 string
                               uint32      str_len         //the length to convert from cp1252 to UCS2.
                               );


/***************************************************************************//*!
@brief ��UCS2�ַ���ת��Ϊcp1252�ַ���
@author James.Zhang
@param cp1252_ptr [in/out] ָ��ת�����cp1252�ַ���
@param wstr_ptr   [in] UCS2�ַ���
@param wstr_len   [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToCP1252(//return the number of UCS2 code that have been converted.
                               uint8 *cp1252_ptr,        //destination: string pointer to cp1252 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp1252.
                               );
                               
/***************************************************************************//*!
@brief ��cp1256�ַ���ת��ΪUCS2�ַ���
@author James.Zhang
@param wstr_ptr   [in/out] ָ��ת�����UCS2�ַ���
@param cp1256_ptr [in] cp1256�ַ���
@param str_len    [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_CP1256ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp1256_ptr,    //source: string pointer to cp1256 string
                               uint32      str_len         //the length to convert from cp1256 to UCS2.
                               );


/***************************************************************************//*!
@brief ��UCS2�ַ���ת��Ϊcp1256�ַ���
@author James.Zhang
@param cp1256_ptr [in/out] ָ��ת�����cp1256�ַ���
@param wstr_ptr   [in] UCS2�ַ���
@param wstr_len   [in] �ַ�������
@return ʵ��ת���ĸ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToCP1256(//return the number of UCS2 code that have been converted.
                               uint8 *cp1256_ptr,        //destination: string pointer to cp1256 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp1256.
                               );                        

/*****************************************************************************/
// 	Description : Ѱ��һ��UTF8�����е�1��UTF8�ַ�����ʼƫ��
//	Global resource dependence : ��������ĳһ���Ѿ�ȷ��ΪUTF8�����������
//   Author: Ying.Xu
//	Note: ���ص�1��������UTF8�������ʼƫ��
/*****************************************************************************/
PUBLIC int32 GUI_FindFirstUTF8CodeStartPosition(
                                            uint8 *p_src,      //in
                                            int32 src_length   //in
                                            );
#ifndef GBK_SUPPORT	
/*****************************************************************************/
// 	Description : Ѱ��һ��ANSI�����е�1��ANSI�ַ�����ʼƫ��
//	Global resource dependence : ��������ĳһ���Ѿ�ȷ��ΪANSI�����������
//   Author: Ying.Xu
//	Note: ���ص�1��������ANSI�������ʼƫ��
/*****************************************************************************/
PUBLIC int32 GUI_FindFirstANSICodeStartPosition(
												uint8 *p_src,//in
												int32 src_length //in
												);   
#endif

#ifdef IM_RUSSIAN_SUPPORT
PUBLIC uint32 GUI_CP866ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp866_ptr,    //source: string pointer to cp855 string
                               uint32      str_len         //the length to convert from cp855 to UCS2.
                               );

PUBLIC uint32 GUI_CP855ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp855_ptr,    //source: string pointer to cp855 string
                               uint32      str_len         //the length to convert from cp855 to UCS2.
                               );

PUBLIC uint32 GUI_WstrToCP855(//return the number of UCS2 code that have been converted.
                               uint8 *cp855_ptr,        //destination: string pointer to cp855 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp855.
                               );

PUBLIC uint32 GUI_WstrToCP866(//return the number of UCS2 code that have been converted.
                               uint8 *cp866_ptr,        //destination: string pointer to cp855 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp855.
                               );

PUBLIC uint32 GUI_KOI8RToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *koi8r_ptr,    //source: string pointer to cp1251 string
                               uint32      str_len         //the length to convert from cp1251 to UCS2.
                               );

PUBLIC uint32 GUI_WstrToKOI8R(//return the number of UCS2 code that have been converted.
                               uint8 *koi8r_ptr,        //destination: string pointer to cp1251 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp1251.
                               );

PUBLIC uint32 GUI_MAC_CYRILLICToWstr(//return the number of uc2 code that have been converted.
                                     wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                                     const uint8 *cyrillic_ptr,    //source: string pointer to cyrillic string
                                     uint32      str_len         //the length to convert from cyrillic to UCS2.
                                    );

PUBLIC uint32 GUI_WstrToMAC_CYRILLIC(//return the number of UCS2 code that have been converted.
                               uint8 *cyrillic_ptr,        //destination: string pointer to cp1251 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp1251.
                               );


/*****************************************************************************/
// 	Description : cp1251 to wstr
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_CP1251ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp1251_ptr,    //source: string pointer to cp1251 string
                               uint32      str_len         //the length to convert from cp1251 to UCS2.
                               );

/*****************************************************************************/
// 	Description : wstr to cp1251 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToCP1251(//return the number of UCS2 code that have been converted.
                               uint8 *cp1251_ptr,        //destination: string pointer to cp1251 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp1251.
                               );


PUBLIC uint32 GUI_CP1251ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp1251_ptr,  //source: string pointer to CP1251 string
									  uint32 str_len,        //the length to convert from CP1251to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  );

PUBLIC uint32 GUI_CP855ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp855_ptr,  //source: string pointer to CP855 string
									  uint32 str_len,        //the length to convert from CP855 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  );

PUBLIC uint32 GUI_CP866ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp866_ptr,  //source: string pointer to CP866 string
									  uint32 str_len,        //the length to convert from CP866 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  );


PUBLIC uint32 GUI_KOI8RToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *koi8r_ptr,  //source: string pointer to koi8r string
									  uint32 str_len,        //the length to convert from koi8r to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  );

PUBLIC uint32 GUI_MAC_CYRILLICToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cyrillic_ptr,  //source: string pointer to cyrillic string
									  uint32 str_len,        //the length to convert from cyrillic to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  );
#endif

#ifdef IM_THAI_SUPPORT

/*****************************************************************************/
// 	Description : cp874 to wstr
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_CP874ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp874_ptr,    //source: string pointer to cp874 string
                               uint32      str_len         //the length to convert from cp874 to UCS2.
                               );

/*****************************************************************************/
// 	Description : wstr to cp874 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToCP874(//return the number of UCS2 code that have been converted.
                               uint8 *cp874_ptr,        //destination: string pointer to cp874 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp874.
                               );

PUBLIC uint32 GUI_CP874ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp874_ptr,  //source: string pointer to CP874 string
									  uint32 str_len,        //the length to convert from CP874 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  );                               

#endif

/*****************************************************************************/
// 	Description : Get  a string's coding type, Big5, GB2312, or UTF8
//	Global resource dependence : 
//  Author:  Yintang.Ren
//	Note: We support getting the coding type by TXT file format, that means if the code_ptr including 
//          TXT file's heading, we can return the coding type by the heading code. For example, Unicode
//          formating TXT file is begin with the code 0xFF,0xFE.
//          Big5: 
//              First bytes: 0x81 - 0xFE        Second bytes: 0x40 - 0x7E, 0xA1 - 0xFE
//          GB2312:
//              First bytes: 0x81 - 0xFE        Second bytes: 0xA1 - 0xFE 
//          GBK:
//              First bytes: 0x81 - 0xFE        Second bytes: 0x40 - 0x7E, 0x80 - 0xFE
//
//          Big5 and GB2312 have the same first byte range.
/*****************************************************************************/
PUBLIC GUI_CHAR_CODE_TYPE_E GUI_GetCodeType(
                                            uint8 *code_ptr,//in
                                            uint32 code_len //in
                                            );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif

/*@}*/
