/*! @file guifont.h
@brief ���ļ���������������صķ���������
@author Jim.Zhang 
@version 1.0
@date 21/05/2004    
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 21/05/2004       Jim.Zhang           Create 
*******************************************************************************/

/*! @page page_Font Font

(����)

*******************************************************************************/

/*! @addtogroup fontGrp Font
@brief fontģ��
@details
������������صķ���������
@{
*******************************************************************************/
  
#ifndef _GUI_FONT_H_
#define _GUI_FONT_H_

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "sci_types.h"
#include "guilcd.h"
#include "guicommon.h"
#include "mmi_font.h"

/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
/*! @def LF_CHAR
@brief ���з�
*/
#define LF_CHAR        			0x0a    			
/*! @def CR_CHAR
@brief �س���
*/
#define CR_CHAR        			0x0d    			
/*! @def BL_CHAR
@brief �ո��
*/
#define BL_CHAR					0x20				
/*! @def COM_CHAR
@brief ���ŷ�
*/
#define COM_CHAR				0x2C				
/*! @def SEM_CHAR
@brief �ֺŷ�
*/
#define SEM_CHAR				0x3B				

// ���ã�Ҫɾ
#define CUSTOME_ISLABEL 		0x01
// ���ã�Ҫɾ
#define CUSTOME_INRECT  		0x02
// ���ã�Ҫɾ
#define CUSTOME_REVERSE 		0x04
// ���ã�Ҫɾ
#define CUSTOME_ALIGN   		0x08

/*! @typedef GUIFONT_EFFECT_T
@brief ����Ч��
*/
typedef uint16 GUIFONT_EFFECT_T;
// begin
/*! @def FONT_EFFECT_NONE
@brief ��
*/
#define FONT_EFFECT_NONE        	0
/*! @def FONT_EFFECT_REVERSE_EDGE
@brief ��������ɫ�෴���ֱ߿�
*/
#define FONT_EFFECT_REVERSE_EDGE    1
/*! @def FONT_EFFECT_WIHTE_EDGE
@brief ��ɫ�ֱ߿�
*/
#define FONT_EFFECT_WIHTE_EDGE	    2
/*! @def FONT_EFFECT_CUSTOM
@brief �û��Զ����ֱ߿�
*/
#define FONT_EFFECT_CUSTOM			3
// end

#define	FONT_ACSII_NUN      	    128
#define	FONTLIB_FIX_HEAD_LEN	14
							 
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
/*! @enum GUI_ALIGN_E
@brief ���뷽ʽ
*/
typedef enum
{
	ALIGN_LEFT,                     /*!< ����� */
	ALIGN_RIGHT,                    /*!< �Ҷ��� */
	ALIGN_HMIDDLE,                  /*!< ˮƽ���� */
	ALIGN_TOP,                      /*!< �϶��� */
	ALIGN_BOTTOM,                   /*!< �¶��� */
	ALIGN_VMIDDLE,                  /*!< ��ֱ���� */
	ALIGN_LVMIDDLE,                 /*!< ˮƽ��������룬��ֱ������� */
    ALIGN_RVMIDDLE,                 /*!< ˮƽ�����Ҷ��룬��ֱ������� */
    ALIGN_HVMIDDLE                  /*!< ˮƽ���У���ֱ���� */
} GUI_ALIGN_E;


typedef enum
{
	FONT_CODELIB_GB,    			// GB font library
	FONT_CODELIB_BIG5,  			// BIG5 font library
    FONT_CODELIB_MAX
}GUIFONT_CODELIB_E;


#ifdef FONT_BIG5
typedef struct
{
	uint8 						asc_width;
	uint8 						asc_height;
	uint8 						big5_width;
	uint8 						big5_height;
}GUI_FONT_W_H_INFO_T;   
#endif

/*! @enum GUI_ANGLE_E
@brief ��ת�Ƕ�
*/
typedef enum
{
	ANGLE_0,                        /*!< �� */
	ANGLE_90,                       /*!< ˳ʱ����ת90�� */
	ANGLE_180,                      /*!< ˳ʱ����ת180�� */
	ANGLE_270                       /*!< ˳ʱ����ת270�� */
}GUI_ANGLE_E;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ��ȡ���ֵĿ��
@author Jim.zhang
@param font_type [in] ����
@param unicode   [in] �ֵ�unicode��
@return �ֿ�
@note 
*******************************************************************************/
PUBLIC uint8 GUI_GetFontWidth(
                              GUI_FONT_T			font_type,  //font library type
                              uint16 unicode
                              );

/***************************************************************************//*!
@brief ����ַ����Ŀ��
@author yongwei.he
@param font_type  [in] ����
@param str_ptr    [in] �ַ�����ַ
@param str_len    [in] �ַ�������
@return �ַ������
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetStringWidth(
                                 GUI_FONT_T	font_type,     
                                 const wchar* str_ptr,
                                 uint16 str_len
                                 //BOOLEAN is_ucs2
                                 );


/***************************************************************************//*!
@brief ����ַ����Ŀ��
@author James.Zhang
@param font_type  [in] ����
@param str_ptr    [in] �ַ�����ַ
@param str_len    [in] �ַ�������
@param char_space [in] ���μ��
@return 
@note ���μ������Ϊ֮ǰ���ּ��, ���������û��Լ�����, ����ͨ���ú���
*******************************************************************************/
PUBLIC uint16 GUI_GetStringWidthEx(
                                   GUI_FONT_T	font_type,     
                                   const wchar* str_ptr,
                                   uint16 str_len,
                                   uint16 char_space
                                   );


/***************************************************************************//*!
@brief  ���ݹ�����ַ��е�ƫ�ƻ�ȡ�������Ļ��x����
@author James.Zhang
@param align      [in] ���뷽ʽ
@param rect_ptr   [in] �ַ�������Ļ�ϵ���ʾ����
@param dir        [in] ���ַ���
@param str_ptr    [in] �ַ�����ַ
@param str_len    [in] �ַ�������
@param cursor_pos [in] ���������ַ�����ƫ��λ��
@param font_type  [in] ����
@param char_space [in] �ּ��
@return �������Ļ��x����
@note 
*******************************************************************************/
PUBLIC int16 GUI_GetCusorCoordinate(//��������
                                    GUI_ALIGN_E       align,
                                    const GUI_RECT_T* rect_ptr,
                                    int8              dir,
                                    const wchar*      str_ptr,
                                    uint16            str_len,
                                    uint16            cursor_pos,	//���λ��
                                    GUI_FONT_T	      font_type,
                                    uint16            char_space
                                    );


/***************************************************************************//*!
@brief ���ݹ�����ַ��е�ƫ�ƻ�ȡ�������Ļ��x���꣬��΢�����λ��
@author James.Zhang
@param align      [in] ���뷽ʽ
@param rect_ptr   [in] �ַ�������Ļ�ϵ���ʾ����
@param dir        [in] ���ַ���
@param str_ptr    [in] �ַ�����ַ
@param str_len    [in] �ַ�������
@param is_left    [in] �Ƿ������ƶ�
@param font_type  [in] ����
@param char_space [in] �ּ��
@param cursor_ptr [in/out] ������������ַ�����ƫ��λ�ã�������ú��ƫ��λ��
@return �������Ļ��x����
@note 
��Щ�������ԣ���ӡ������ж��unicode�����һ���ַ�����ʱ�����Ҽ��ƶ����ʱ��
�Ͳ������ڶ��unicode֮�䣬��Ӧ���ƶ������ַ����÷�����GUI_GetCusorCoordinate��
��չ�档
*******************************************************************************/
PUBLIC int16 GUI_GetCursorInfo(
                               GUI_ALIGN_E      align,
                               const GUI_RECT_T	*rect_ptr,
                               int8				dir,
                               const wchar		*str_ptr,
                               uint16           str_len,
                               BOOLEAN          is_left,
                               GUI_FONT_T	    font_type,
                               uint16           char_space,
                               int32*           cursor_ptr
                               );


/***************************************************************************//*!
@brief ���ݹ���������Ļ��λ�ã���ȡ�����ַ����е�ƫ����
@author James.Zhang
@param align       [in] ���뷽ʽ
@param rect_ptr    [in] �ַ�������Ļ�ϵ���ʾ����
@param dir         [in] ���ַ���
@param str_ptr     [in] �ַ�����ַ
@param str_len     [in] �ַ�������
@param cursor_pos  [in] ���������ַ�����ԭʼƫ��λ�ã�������������ʾ�����⣬�����ظ�ֵ
@param cursor_x    [in] ����������Ļ��λ��
@param font_type   [in] ����
@param char_space  [in] �ּ��
@param is_find_ptr [in] �Ƿ�������ʾ������
@return ������ַ����е�ƫ����
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetCursorPos(//���λ��
                               GUI_ALIGN_E      align,
							   const GUI_RECT_T	*rect_ptr,      //in:
							   int8				dir,            //in:
							   const wchar		*str_ptr,       //in:
							   uint16           str_len,        //in:
							   uint16           cursor_pos,	    //in:���λ��
							   int16            cursor_x,	    //in:��������
							   GUI_FONT_T	    font_type,      //in:
							   uint16           char_space,     //in:
                               BOOLEAN          *is_find_ptr    //in/out:may PNULL
							   );


/***************************************************************************//*!
@brief ���ݹ���������Ļ��λ�ã���ȡ�����ַ����е�ƫ����
@author  Lianxiang.Zhou
@param align               [in] ���뷽ʽ
@param rect_ptr            [in] �ַ�������Ļ�ϵ���ʾ����
@param dir                 [in] ���ַ���
@param str_ptr             [in] �ַ�����ַ
@param str_len             [in] �ַ�������
@param cursor_x            [in] ����������Ļ��λ��
@param font_type           [in] ����
@param char_space          [in] �ּ��
@param nearest_chr_idx_ptr [in/out] ������ַ����е�ƫ����������Ҳ����������������ƫ����������Ϊ��
@param under_chr_idx_ptr   [in/out] ����Ϊ��
@param under_len_ptr       [in/out] ����Ϊ��
@return ���ݹ�������Ƿ��ҵ���Ӧ��ƫ��
@note ��ǿ�ͣ�ͬʱ��ù�����ַ������λ�ù�ϵ
*******************************************************************************/
PUBLIC BOOLEAN GUI_GetCursorPosEx(
                                  GUI_ALIGN_E      align,
                                  const GUI_RECT_T *rect_ptr,
                                  int8         dir,
                                  const wchar  *str_ptr,
                                  uint16       str_len,
                                  int16        cursor_x,
                                  GUI_FONT_T   font_type,
                                  uint16       char_space,
                                  int32        *nearest_chr_idx_ptr,
                                  int32        *under_chr_idx_ptr,
                                  int32        *under_len_ptr
                                  );


/***************************************************************************//*!
@brief ��ȡ����߶�
@author Jassmine
@param font_type [in] ����
@return ����߶�
@note 
*******************************************************************************/
PUBLIC uint16 GUIFONT_GetHeight(
                                GUI_FONT_T  font_type
                                );

                       
/***************************************************************************//*!
@brief ��ȡ�ַ��ĸ߶�
@author Jim.zhang 
@param font_type [in] ����
@param unicode   [in] �ַ���unicode��
@return �ַ��߶�
@note 
*******************************************************************************/
PUBLIC uint8 GUI_GetFontHeight(                      
                               GUI_FONT_T			font_type,  //font library type
                               uint16 unicode                        		
                               );

/***************************************************************************//*!
@brief ����ַ����ĸ߶�
@author yongwei.he
@param font_type [in] ����
@param str_ptr   [in] �ַ�����ַ
@param str_len   [in] �ַ�������
@return �ַ����߶�
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetStringHeight(
                                  GUI_FONT_T	font_type,     
                                  const wchar* str_ptr,
                                  uint16 str_len
                                  //BOOLEAN is_ucs2
                                  );
/***************************************************************************//*!
@brief ����ַ����ĸ߶�
@author zhewei.tang
@param font_type [in] ����
@param str_ptr   [in] �ַ�����ַ
@param str_len   [in] �ַ�������
@return �ַ����߶�
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetStringHeightEx(
                                  GUI_FONT_T   font_type,     
                                  const wchar* str_ptr,
                                  uint16       str_len
                                  );

/***************************************************************************//*!
@brief GB�ַ���ת����UCS2��
@author Jim.zhang
@param wstr_ptr [in/out] ָ��ת�����UCS2�ַ���
@param gb_ptr   [in] GB�ַ���
@param str_len  [in] �ַ�������
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_GBToWstr(					//return the number of UCS2 code that have been converted.
                           wchar *wstr_ptr, 	//destination: string pointer to UCS2 string.
                           const uint8 *gb_ptr, //source: string pointer to GB string
                           uint32 str_len			//the length to convert from GB to UCS2.
                           );


/***************************************************************************//*!
@brief GB�ַ���ת����UCS2��
@author  jiqun.li
@param  wstr_ptr    [in/out] ָ��ת�����UCS2�ַ���
@param  gb2312_ptr  [in] GB�ַ���
@param  len         [in] �ַ�������
@return ʵ��ת���ĳ���
@note 
�˷���רΪBROWSER�ṩ��\n
��gb2312����ҳ�У�"a&#160;c"��Ҫת����"a\x00\xA0c"��������GUI_GBToWstr()�ӿڵĻ���
���ᱻת����{0x0061, 0x0000, 0x00a0, 0x0063}�������ַ�������"\0"������\link GUI_GBToWstr() \endlink
ת���������ַ����ᱻ�ضϣ������������������ṩ������������0x0000���ַ���
*******************************************************************************/
PUBLIC uint16 GUI_GBToWstrEx(wchar *wstr_ptr, const uint8 *gb_ptr, uint16 len);


/***************************************************************************//*!
@brief GB�ַ���ת����UCS2��
@author Jim.zhang
@param ucs2_ptr [in/out] ָ��ת�����UCS2�ַ���
@param gb_ptr   [in] GB�ַ���
@param len      [in] �ַ�������
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GB2UCS(					//return the number of UCS2 code that have been converted.
                         uint16 *ucs2_ptr, 	//destination: string pointer to UCS2 string.
                         const uint8 *gb_ptr,//source: string pointer to GB string
                         uint16 len			//the length to convert from GB to UCS2.
                         );


/***************************************************************************//*!
@brief UCS2�ַ���ת����GB���ַ���
@author Jim.zhang
@param gb_ptr   [in/out] ָ��ת�����GB�ַ���
@param wstr_ptr [in] UCS2�ַ���
@param wstr_len [in] �ַ�������
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToGB(						// return the length of gb2312_ptr
                           uint8  *gb_ptr,			// store the gb2312 string code
                           const wchar  *wstr_ptr,  // the ucs2 stringcode that need to be converted.
                           uint32   wstr_len         //the unicode length
                           );

#ifdef BIG5_SUPPORT
/***************************************************************************//*!
@brief BIG5�ַ���ת����UCS2�ַ���
@author James.Zhang
@param wstr_ptr [in/out] ָ��ת�����UCS2�ַ���
@param big5_ptr [in] BIG5�ַ���
@param str_len  [in] �ַ�������
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_Big5ToWstr(					    //return the number of UCS2 code that have been converted.
                             wchar *wstr_ptr, 	    //destination: string pointer to UCS2 string.
                             const uint8 *big5_ptr, //source: string pointer to Big5 string
                             uint32 str_len			//the length to convert from Big5 to UCS2.
                             );

/***************************************************************************//*!
@brief BIG5�ַ���ת����UCS2�ַ�����������Դ�ַ�������ת���ĵ�ַ
@author Yintang.ren
@param wstr_ptr    [in/out] ָ��ת�����UCS2�ַ���
@param big5_ptr    [in] BIG5�ַ���
@param str_len     [in] �ַ�������
@param src_end_pos [in/out] Դ�ַ�������ת���ĵ�ַ
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_Big5ToWstrWithEndPos(    //return the number of UCS2 code that have been converted.
									   wchar *wstr_ptr, 	    //destination: string pointer to UCS2 string.
									   const uint8 *big5_ptr,   //source: string pointer to Big5 string
									   uint32 str_len,			//the length to convert from Big5 to UCS2.
									   uint32* src_end_pos      //the end of src string
									   );

/***************************************************************************//*!
@brief UCS2�ַ���ת����BIG5�ַ���
@author James.Zhang
@param big5_ptr [in/out] ָ��ת�����BIG5�ַ���
@param wstr_ptr [in] UCS2�ַ���
@param wstr_len [in] �ַ�������
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToBig5(                         // return the length of big5_ptr
                             uint8        *big5_ptr,  // store the big5 string code
                             const wchar  *wstr_ptr,  // the unicode stringcode that need to be converted.
                             uint32       wstr_len    //the unicode length
                             );
#endif
/***************************************************************************//*!
@brief �Ƿ��ǻ��з�
@author Great.Tian 
@param char_code [in] �ַ���֧��ASCII��UINCODE
@return �Ƿ��ǻ��з�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsNewlineChar(
                                 uint16 char_code //[IN] the char code, Use uint16 can support ASCII and UINCODE
                                 );    

/***************************************************************************//*!
@brief ���������ȡ���С
@author Jim.zhang
@param font [in] ����
@return �����С
@note 
*******************************************************************************/
uint16 GUI_ConvertGUIFontToSize(
                                GUI_FONT_T font 
                                );


/***************************************************************************//*!
@brief ���������С��ȡ��Ӧ������
@author Jim.zhang
@param size [in] �����С
@return ����
@note 
*******************************************************************************/
GUI_FONT_T GUI_ConvertSizeToGUIFont( uint16 size );

/***************************************************************************//*!
@brief �滻�ַ�����ָ���ַ�
@author Jim.zhang
@param src_str_ptr [in/out] �ַ�����ַ
@param src_char    [in] ���滻���ַ�
@param des_char    [in] �滻���ַ�
@return 
@note 
*******************************************************************************/
void GUI_ReplaceChar(
                     MMI_STRING_T *src_str_ptr, 
                     uint16 src_char,
                     uint16 des_char
                     );


/***************************************************************************//*!
@brief GBK�ַ���ת����UCS2�ַ���
@author James.Zhang
@param wstr_ptr [in/out] ָ��ת�����UCS2�ַ���
@param gbk_ptr  [in] GBK�ַ�����ַ
@param str_len  [in] �ַ�������
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_GBKToWstr(					    //return the number of UCS2 code that have been converted.
                            wchar *wstr_ptr, 	    //destination: string pointer to UCS2 string.
                            const uint8 *gbk_ptr,   //source: string pointer to GBK string
                            uint32 str_len			//the length to convert from GBK to UCS2.
                            );

/***************************************************************************//*!
@brief GBK�ַ���ת����UCS2�ַ�����������Դ�ַ�������ת���ĵ�ַ
@author Yintang.ren
@param wstr_ptr    [in/out] ָ��ת�����UCS2�ַ���
@param gbk_ptr     [in] GBK�ַ�����ַ
@param str_len     [in] �ַ�������
@param src_end_pos [in/out] Դ�ַ�������ת���ĵ�ַ
@return ʵ��ת���ĳ���
@note 
*******************************************************************************/
PUBLIC uint32 GUI_GBKToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *gbk_ptr,  //source: string pointer to GBK string
									  uint32 str_len,        //the length to convert from GBK to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  );

#endif //_GUI_FONT_H_


/*@}*/
