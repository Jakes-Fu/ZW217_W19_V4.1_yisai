/*! @file guifont.h
@brief 此文件定义了与字体相关的方法、类型
@author Jim.Zhang 
@version 1.0
@date 21/05/2004    
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 21/05/2004       Jim.Zhang           Create 
*******************************************************************************/

/*! @page page_Font Font

(暂无)

*******************************************************************************/

/*! @addtogroup fontGrp Font
@brief font模块
@details
定义与字体相关的方法、类型
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
@brief 换行符
*/
#define LF_CHAR        			0x0a    			
/*! @def CR_CHAR
@brief 回车符
*/
#define CR_CHAR        			0x0d    			
/*! @def BL_CHAR
@brief 空格符
*/
#define BL_CHAR					0x20				
/*! @def COM_CHAR
@brief 逗号符
*/
#define COM_CHAR				0x2C				
/*! @def SEM_CHAR
@brief 分号符
*/
#define SEM_CHAR				0x3B				

// 不用，要删
#define CUSTOME_ISLABEL 		0x01
// 不用，要删
#define CUSTOME_INRECT  		0x02
// 不用，要删
#define CUSTOME_REVERSE 		0x04
// 不用，要删
#define CUSTOME_ALIGN   		0x08

/*! @typedef GUIFONT_EFFECT_T
@brief 字体效果
*/
typedef uint16 GUIFONT_EFFECT_T;
// begin
/*! @def FONT_EFFECT_NONE
@brief 无
*/
#define FONT_EFFECT_NONE        	0
/*! @def FONT_EFFECT_REVERSE_EDGE
@brief 与字体颜色相反的字边框
*/
#define FONT_EFFECT_REVERSE_EDGE    1
/*! @def FONT_EFFECT_WIHTE_EDGE
@brief 白色字边框
*/
#define FONT_EFFECT_WIHTE_EDGE	    2
/*! @def FONT_EFFECT_CUSTOM
@brief 用户自定义字边框
*/
#define FONT_EFFECT_CUSTOM			3
// end

#define	FONT_ACSII_NUN      	    128
#define	FONTLIB_FIX_HEAD_LEN	14
							 
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
/*! @enum GUI_ALIGN_E
@brief 对齐方式
*/
typedef enum
{
	ALIGN_LEFT,                     /*!< 左对齐 */
	ALIGN_RIGHT,                    /*!< 右对齐 */
	ALIGN_HMIDDLE,                  /*!< 水平居中 */
	ALIGN_TOP,                      /*!< 上对齐 */
	ALIGN_BOTTOM,                   /*!< 下对齐 */
	ALIGN_VMIDDLE,                  /*!< 垂直居中 */
	ALIGN_LVMIDDLE,                 /*!< 水平方向左对齐，垂直方向居中 */
    ALIGN_RVMIDDLE,                 /*!< 水平方向右对齐，垂直方向居中 */
    ALIGN_HVMIDDLE                  /*!< 水平居中，垂直居中 */
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
@brief 旋转角度
*/
typedef enum
{
	ANGLE_0,                        /*!< 无 */
	ANGLE_90,                       /*!< 顺时针旋转90度 */
	ANGLE_180,                      /*!< 顺时针旋转180度 */
	ANGLE_270                       /*!< 顺时针旋转270度 */
}GUI_ANGLE_E;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 获取单字的宽度
@author Jim.zhang
@param font_type [in] 字体
@param unicode   [in] 字的unicode码
@return 字宽
@note 
*******************************************************************************/
PUBLIC uint8 GUI_GetFontWidth(
                              GUI_FONT_T			font_type,  //font library type
                              uint16 unicode
                              );

/***************************************************************************//*!
@brief 获得字符串的宽度
@author yongwei.he
@param font_type  [in] 字体
@param str_ptr    [in] 字符串地址
@param str_len    [in] 字符串长度
@return 字符串宽度
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetStringWidth(
                                 GUI_FONT_T	font_type,     
                                 const wchar* str_ptr,
                                 uint16 str_len
                                 //BOOLEAN is_ucs2
                                 );


/***************************************************************************//*!
@brief 获得字符串的宽度
@author James.Zhang
@param font_type  [in] 字体
@param str_ptr    [in] 字符串地址
@param str_len    [in] 字符串长度
@param char_space [in] 字形间距
@return 
@note 字形间距可理解为之前的字间距, 但不允许用户自己计算, 必须通过该函数
*******************************************************************************/
PUBLIC uint16 GUI_GetStringWidthEx(
                                   GUI_FONT_T	font_type,     
                                   const wchar* str_ptr,
                                   uint16 str_len,
                                   uint16 char_space
                                   );


/***************************************************************************//*!
@brief  根据光标在字符中的偏移获取相对于屏幕的x坐标
@author James.Zhang
@param align      [in] 对齐方式
@param rect_ptr   [in] 字符串在屏幕上的显示区域
@param dir        [in] 文字方向
@param str_ptr    [in] 字符串地址
@param str_len    [in] 字符串长度
@param cursor_pos [in] 光标相对于字符串的偏移位置
@param font_type  [in] 字体
@param char_space [in] 字间距
@return 相对于屏幕的x坐标
@note 
*******************************************************************************/
PUBLIC int16 GUI_GetCusorCoordinate(//绝对坐标
                                    GUI_ALIGN_E       align,
                                    const GUI_RECT_T* rect_ptr,
                                    int8              dir,
                                    const wchar*      str_ptr,
                                    uint16            str_len,
                                    uint16            cursor_pos,	//相对位置
                                    GUI_FONT_T	      font_type,
                                    uint16            char_space
                                    );


/***************************************************************************//*!
@brief 根据光标在字符中的偏移获取相对于屏幕的x坐标，并微调光标位置
@author James.Zhang
@param align      [in] 对齐方式
@param rect_ptr   [in] 字符串在屏幕上的显示区域
@param dir        [in] 文字方向
@param str_ptr    [in] 字符串地址
@param str_len    [in] 字符串长度
@param is_left    [in] 是否向左移动
@param font_type  [in] 字体
@param char_space [in] 字间距
@param cursor_ptr [in/out] 输入光标相对于字符串的偏移位置，输出调用后的偏移位置
@return 相对于屏幕的x坐标
@note 
有些复杂语言，如印度语，会有多个unicode码组成一个字符，这时按左右键移动光标时，
就不能落在多个unicode之间，而应该移动整个字符。该方法是GUI_GetCusorCoordinate的
扩展版。
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
@brief 根据光标相对于屏幕的位置，获取其在字符串中的偏移量
@author James.Zhang
@param align       [in] 对齐方式
@param rect_ptr    [in] 字符串在屏幕上的显示区域
@param dir         [in] 文字方向
@param str_ptr     [in] 字符串地址
@param str_len     [in] 字符串长度
@param cursor_pos  [in] 光标相对于字符串的原始偏移位置，如果光标落在显示区域外，将返回该值
@param cursor_x    [in] 光标相对于屏幕的位置
@param font_type   [in] 字体
@param char_space  [in] 字间距
@param is_find_ptr [in] 是否落在显示区域内
@return 光标在字符串中的偏移量
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetCursorPos(//相对位置
                               GUI_ALIGN_E      align,
							   const GUI_RECT_T	*rect_ptr,      //in:
							   int8				dir,            //in:
							   const wchar		*str_ptr,       //in:
							   uint16           str_len,        //in:
							   uint16           cursor_pos,	    //in:相对位置
							   int16            cursor_x,	    //in:绝对坐标
							   GUI_FONT_T	    font_type,      //in:
							   uint16           char_space,     //in:
                               BOOLEAN          *is_find_ptr    //in/out:may PNULL
							   );


/***************************************************************************//*!
@brief 根据光标相对于屏幕的位置，获取其在字符串中的偏移量
@author  Lianxiang.Zhou
@param align               [in] 对齐方式
@param rect_ptr            [in] 字符串在屏幕上的显示区域
@param dir                 [in] 文字方向
@param str_ptr             [in] 字符串地址
@param str_len             [in] 字符串长度
@param cursor_x            [in] 光标相对于屏幕的位置
@param font_type           [in] 字体
@param char_space          [in] 字间距
@param nearest_chr_idx_ptr [in/out] 光标在字符串中的偏移量，如果找不到，返回最相近的偏移量。不能为空
@param under_chr_idx_ptr   [in/out] 不能为空
@param under_len_ptr       [in/out] 不能为空
@return 根据光标坐标是否找到相应的偏移
@note 增强型，同时获得光标与字符的相对位置关系
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
@brief 获取字体高度
@author Jassmine
@param font_type [in] 字体
@return 字体高度
@note 
*******************************************************************************/
PUBLIC uint16 GUIFONT_GetHeight(
                                GUI_FONT_T  font_type
                                );

                       
/***************************************************************************//*!
@brief 获取字符的高度
@author Jim.zhang 
@param font_type [in] 字体
@param unicode   [in] 字符的unicode码
@return 字符高度
@note 
*******************************************************************************/
PUBLIC uint8 GUI_GetFontHeight(                      
                               GUI_FONT_T			font_type,  //font library type
                               uint16 unicode                        		
                               );

/***************************************************************************//*!
@brief 获得字符串的高度
@author yongwei.he
@param font_type [in] 字体
@param str_ptr   [in] 字符串地址
@param str_len   [in] 字符串长度
@return 字符串高度
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetStringHeight(
                                  GUI_FONT_T	font_type,     
                                  const wchar* str_ptr,
                                  uint16 str_len
                                  //BOOLEAN is_ucs2
                                  );
/***************************************************************************//*!
@brief 获得字符串的高度
@author zhewei.tang
@param font_type [in] 字体
@param str_ptr   [in] 字符串地址
@param str_len   [in] 字符串长度
@return 字符串高度
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GetStringHeightEx(
                                  GUI_FONT_T   font_type,     
                                  const wchar* str_ptr,
                                  uint16       str_len
                                  );

/***************************************************************************//*!
@brief GB字符串转换成UCS2码
@author Jim.zhang
@param wstr_ptr [in/out] 指向转换后的UCS2字符串
@param gb_ptr   [in] GB字符串
@param str_len  [in] 字符串长度
@return 实际转换的长度
@note 
*******************************************************************************/
PUBLIC uint32 GUI_GBToWstr(					//return the number of UCS2 code that have been converted.
                           wchar *wstr_ptr, 	//destination: string pointer to UCS2 string.
                           const uint8 *gb_ptr, //source: string pointer to GB string
                           uint32 str_len			//the length to convert from GB to UCS2.
                           );


/***************************************************************************//*!
@brief GB字符串转换成UCS2码
@author  jiqun.li
@param  wstr_ptr    [in/out] 指向转换后的UCS2字符串
@param  gb2312_ptr  [in] GB字符串
@param  len         [in] 字符串长度
@return 实际转换的长度
@note 
此方法专为BROWSER提供。\n
在gb2312的网页中，"a&#160;c"需要转换成"a\x00\xA0c"，但调用GUI_GBToWstr()接口的话，
它会被转换成{0x0061, 0x0000, 0x00a0, 0x0063}，由于字符串是以"\0"结束，\link GUI_GBToWstr() \endlink
转换出来的字符串会被截断，不满足需求，所以另提供方法避免这种0x0000的字符。
*******************************************************************************/
PUBLIC uint16 GUI_GBToWstrEx(wchar *wstr_ptr, const uint8 *gb_ptr, uint16 len);


/***************************************************************************//*!
@brief GB字符串转换成UCS2码
@author Jim.zhang
@param ucs2_ptr [in/out] 指向转换后的UCS2字符串
@param gb_ptr   [in] GB字符串
@param len      [in] 字符串长度
@return 实际转换的长度
@note 
*******************************************************************************/
PUBLIC uint16 GUI_GB2UCS(					//return the number of UCS2 code that have been converted.
                         uint16 *ucs2_ptr, 	//destination: string pointer to UCS2 string.
                         const uint8 *gb_ptr,//source: string pointer to GB string
                         uint16 len			//the length to convert from GB to UCS2.
                         );


/***************************************************************************//*!
@brief UCS2字符串转换成GB码字符串
@author Jim.zhang
@param gb_ptr   [in/out] 指向转换后的GB字符串
@param wstr_ptr [in] UCS2字符串
@param wstr_len [in] 字符串长度
@return 实际转换的长度
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToGB(						// return the length of gb2312_ptr
                           uint8  *gb_ptr,			// store the gb2312 string code
                           const wchar  *wstr_ptr,  // the ucs2 stringcode that need to be converted.
                           uint32   wstr_len         //the unicode length
                           );

#ifdef BIG5_SUPPORT
/***************************************************************************//*!
@brief BIG5字符串转换成UCS2字符串
@author James.Zhang
@param wstr_ptr [in/out] 指向转换后的UCS2字符串
@param big5_ptr [in] BIG5字符串
@param str_len  [in] 字符串长度
@return 实际转换的长度
@note 
*******************************************************************************/
PUBLIC uint32 GUI_Big5ToWstr(					    //return the number of UCS2 code that have been converted.
                             wchar *wstr_ptr, 	    //destination: string pointer to UCS2 string.
                             const uint8 *big5_ptr, //source: string pointer to Big5 string
                             uint32 str_len			//the length to convert from Big5 to UCS2.
                             );

/***************************************************************************//*!
@brief BIG5字符串转换成UCS2字符串，并返回源字符串结束转换的地址
@author Yintang.ren
@param wstr_ptr    [in/out] 指向转换后的UCS2字符串
@param big5_ptr    [in] BIG5字符串
@param str_len     [in] 字符串长度
@param src_end_pos [in/out] 源字符串结束转换的地址
@return 实际转换的长度
@note 
*******************************************************************************/
PUBLIC uint32 GUI_Big5ToWstrWithEndPos(    //return the number of UCS2 code that have been converted.
									   wchar *wstr_ptr, 	    //destination: string pointer to UCS2 string.
									   const uint8 *big5_ptr,   //source: string pointer to Big5 string
									   uint32 str_len,			//the length to convert from Big5 to UCS2.
									   uint32* src_end_pos      //the end of src string
									   );

/***************************************************************************//*!
@brief UCS2字符串转换成BIG5字符串
@author James.Zhang
@param big5_ptr [in/out] 指向转换后的BIG5字符串
@param wstr_ptr [in] UCS2字符串
@param wstr_len [in] 字符串长度
@return 实际转换的长度
@note 
*******************************************************************************/
PUBLIC uint32 GUI_WstrToBig5(                         // return the length of big5_ptr
                             uint8        *big5_ptr,  // store the big5 string code
                             const wchar  *wstr_ptr,  // the unicode stringcode that need to be converted.
                             uint32       wstr_len    //the unicode length
                             );
#endif
/***************************************************************************//*!
@brief 是否是换行符
@author Great.Tian 
@param char_code [in] 字符，支持ASCII和UINCODE
@return 是否是换行符
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUI_IsNewlineChar(
                                 uint16 char_code //[IN] the char code, Use uint16 can support ASCII and UINCODE
                                 );    

/***************************************************************************//*!
@brief 根据字体获取其大小
@author Jim.zhang
@param font [in] 字体
@return 字体大小
@note 
*******************************************************************************/
uint16 GUI_ConvertGUIFontToSize(
                                GUI_FONT_T font 
                                );


/***************************************************************************//*!
@brief 根据字体大小获取对应的字体
@author Jim.zhang
@param size [in] 字体大小
@return 字体
@note 
*******************************************************************************/
GUI_FONT_T GUI_ConvertSizeToGUIFont( uint16 size );

/***************************************************************************//*!
@brief 替换字符串中指定字符
@author Jim.zhang
@param src_str_ptr [in/out] 字符串地址
@param src_char    [in] 被替换的字符
@param des_char    [in] 替换的字符
@return 
@note 
*******************************************************************************/
void GUI_ReplaceChar(
                     MMI_STRING_T *src_str_ptr, 
                     uint16 src_char,
                     uint16 des_char
                     );


/***************************************************************************//*!
@brief GBK字符串转换成UCS2字符串
@author James.Zhang
@param wstr_ptr [in/out] 指向转换后的UCS2字符串
@param gbk_ptr  [in] GBK字符串地址
@param str_len  [in] 字符串长度
@return 实际转换的长度
@note 
*******************************************************************************/
PUBLIC uint32 GUI_GBKToWstr(					    //return the number of UCS2 code that have been converted.
                            wchar *wstr_ptr, 	    //destination: string pointer to UCS2 string.
                            const uint8 *gbk_ptr,   //source: string pointer to GBK string
                            uint32 str_len			//the length to convert from GBK to UCS2.
                            );

/***************************************************************************//*!
@brief GBK字符串转换成UCS2字符串，并返回源字符串结束转换的地址
@author Yintang.ren
@param wstr_ptr    [in/out] 指向转换后的UCS2字符串
@param gbk_ptr     [in] GBK字符串地址
@param str_len     [in] 字符串长度
@param src_end_pos [in/out] 源字符串结束转换的地址
@return 实际转换的长度
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
