/*****************************************************************************
** File Name:      spml_api.h                                                *
** Author:                                                                   *
** Date:           21/07/2009                                                *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file provide functions about font handle text line   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                    NAME             DESCRIPTION                      *
** 21/07/2004              James.Zhang         Create                        *
******************************************************************************/
#ifndef SPML_API_H
#define SPML_API_H
/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "caf.h"
#include "os_api.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
//用于SPML内部使用缓冲区进行优化
#define SPML_CACHE_SUPPORT
#define SPML_CACHE_MASK 0xFF
#define SPML_CACHE_SIZE (SPML_CACHE_MASK + 1)

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef CAF_POINT_T SPML_POINT_T;
typedef CAF_RECT_T  SPML_RECT_T;

typedef uint16  SPML_COLOR_T;                  //颜色
typedef uint16  SPML_TEXT_SIZE_T;              //字体大小, 为opentype中定义的字体大小

typedef int8    SPML_TEXT_DIR_T;               //文字的方向
#define TEXT_DIR_LTR            (0)            //从左向右
#define TEXT_DIR_RTL            (1)            //从右向左
#define TEXT_DIR_AUTO           (-1)           //由文本自身决定

typedef uint32  SPML_TEXT_FORMAT_T;
#define TEXT_FORMAT_NORMAL       0x00000000
#define TEXT_FORMAT_LEFT         TEXT_FORMAT_NORMAL //左对齐
#define TEXT_FORMAT_RIGHT        0x00000001         //右对齐
#define TEXT_FORMAT_CENTER       0x00000002         //水平中对齐
#define TEXT_FORMAT_TOP          TEXT_FORMAT_NORMAL //上对齐
#define TEXT_FORMAT_BOTTOM       0x00000004         //下对齐
#define TEXT_FORMAT_MIDDLE       0x00000008         //垂直中对齐

#define TEXT_FORMAT_INVERSE      0x00000010         //反显效果
#define TEXT_FORMAT_UNDERLINE    0x00000020         //下划线效果
#define TEXT_FORMAT_BACKGROUND   0x00000040         //背景色
#define TEXT_FORMAT_BG_RECT      0x00000080         //背景色基于显示区域, 默认基于字形区域
#define TEXT_FORMAT_EDGE         0x00000100         //边缘效果
#define TEXT_FORMAT_BOLD         0x00000200         //粗体
#define TEXT_FORMAT_ITALIC       0x00000400         //斜体

#define TEXT_FORMAT_WORDSPACE    0x00001000         //支持word space


#define TEXT_FORMAT_NO_AUTOBY    0x00002000       //不需要自动调整bearing y的功能
#define TEXT_FORMAT_BRIGHTNESS   0x00004000       //调整字体亮度，即灰度值
#define TEXT_FORMAT_ELLIPSIS     0x00008000       //显示渐变

#define TEXT_FORMAT_FROM_HEAD    0x00010000        //从头显示
#define TEXT_FORMAT_FROM_TAIL    0x00020000        //从尾部显示, 用于向右滚动, 仅当字符串宽度超过显示宽度时才有效
#define TEXT_FORMAT_ELLIPSIS_EX  0x00040000       //显示...

#define SPML_DEFUALT_BRIGHTNESS  0x80             //默认亮度值，调亮或调暗矢量字体的灰度值

typedef uint16 SPML_BUF_TYPE_T;
#define SPML_BUF_DEFAULT  0                        //default
#define SPML_BUF_RGB565   SPML_BUF_DEFAULT         //565
#define SPML_BUF_ARGB888  (SPML_BUF_DEFAULT + 1)   //argb8888
#define SPML_BUF_TYPE_MAX (SPML_BUF_DEFAULT + 2)   //max

typedef struct
{
    uint16  width;  //宽度
    uint16  height; //高度
    //以下字段暂时保留
    uint16  ascent;
    uint16  leading;
    int16   lsb;
    int16   rsb;
    int16   bearing_y;
}SPML_TEXT_METRICS_T;

typedef struct
{
    SPML_TEXT_SIZE_T    size;   //字体大小
    SPML_TEXT_FORMAT_T  format; //文字格式
    SPML_COLOR_T        color;  //文字颜色
}SPML_TEXT_STYLE_T;

typedef struct
{
    int32               start;  //起始位置
    int32               end;    //结束位置
    SPML_TEXT_STYLE_T   style;  //文字风格
}SPML_TEXT_REGION_T;

typedef struct
{
    SPML_TEXT_FORMAT_T  format;         //文字的格式
    SPML_TEXT_DIR_T     dir;            //文字方向
    SPML_TEXT_SIZE_T    size;           //文字大小
    uint16              glyph_space;    //字形间距
    uint16              region_num;     //特效个数
    const SPML_TEXT_REGION_T* region_ptr; //特效区域

    //以下参数, 在绘制文字时需要填充
    SPML_COLOR_T        font_color;     //文字颜色
    SPML_COLOR_T        bg_color;       //背景颜色
    SPML_COLOR_T        edge_color;     //文字边缘颜色
    
    SPML_COLOR_T        line_color;     //下划线的颜色
    SPML_BUF_TYPE_T     buffer_type;    //buffer的类型
    uint16              buffer_width;   //buffer的宽度
    uint16              buffer_height;  //buffer的高度
    void*               buffer_ptr;     //显示的buffer, 不能为空
    SPML_RECT_T         display_rect;   //显示区域, 相对于显示buffer的原点
    SPML_RECT_T         clip_rect;      //剪切区域, 相对于显示buffer的原点
    
    uint16              word_space;     //词间距, 用于控制空格所占的字宽
    uint8               brightness;     //矢量字体小于16号，建议调亮亮度
    uint8               edge_alpha;     //文字边缘alpha

}SPML_TEXT_PARAM_T;


typedef enum
{                     
    TEXT_SCRIPT_INVALID     = -1,
    TEXT_SCRIPT_COMMON      = 0,            /**< ISO 15924 - Zyyy */
    TEXT_SCRIPT_INHERITED,                  /**< ISO 15924 - Qaai */
    TEXT_SCRIPT_ARABIC,                     /**< ISO 15924 - Arab */
    TEXT_SCRIPT_ARMENIAN,                   /**< ISO 15924 - Armn */
    TEXT_SCRIPT_BENGALI,                    /**< ISO 15924 - Beng */
    TEXT_SCRIPT_BOPOMOFO,                   /**< ISO 15924 - Bopo */
    TEXT_SCRIPT_CHEROKEE,                   /**< ISO 15924 - Cher */
    TEXT_SCRIPT_COPTIC,                     /**< ISO 15924 - Qaac */
    TEXT_SCRIPT_CYRILLIC,                   /**< ISO 15924 - Cyrl (Cyrs) */
    TEXT_SCRIPT_DESERET,                    /**< ISO 15924 - Dsrt */
    TEXT_SCRIPT_DEVANAGARI,                 /**< ISO 15924 - Deva */
    TEXT_SCRIPT_ETHIOPIC,                   /**< ISO 15924 - Ethi */
    TEXT_SCRIPT_GEORGIAN,                   /**< ISO 15924 - Geor (Geon, Geoa) */
    TEXT_SCRIPT_GOTHIC,                     /**< ISO 15924 - Goth */
    TEXT_SCRIPT_GREEK,                      /**< ISO 15924 - Grek */
    TEXT_SCRIPT_GUJARATI,                   /**< ISO 15924 - Gujr */
    TEXT_SCRIPT_GURMUKHI,                   /**< ISO 15924 - Guru */
    TEXT_SCRIPT_HAN,                        /**< ISO 15924 - Hani */
    TEXT_SCRIPT_HANGUL,                     /**< ISO 15924 - Hang */
    TEXT_SCRIPT_HEBREW,                     /**< ISO 15924 - Hebr */
    TEXT_SCRIPT_HIRAGANA,                   /**< ISO 15924 - Hira */
    TEXT_SCRIPT_KANNADA,                    /**< ISO 15924 - Knda */
    TEXT_SCRIPT_KATAKANA,                   /**< ISO 15924 - Kana */
    TEXT_SCRIPT_KHMER,                      /**< ISO 15924 - Khmr */
    TEXT_SCRIPT_LAO,                        /**< ISO 15924 - Laoo */
    TEXT_SCRIPT_LATIN,                      /**< ISO 15924 - Latn (Latf, Latg) */
    TEXT_SCRIPT_MALAYALAM,                  /**< ISO 15924 - Mlym */
    TEXT_SCRIPT_MONGOLIAN,                  /**< ISO 15924 - Mong */
    TEXT_SCRIPT_MYANMAR,                    /**< ISO 15924 - Mymr */
    TEXT_SCRIPT_OGHAM,                      /**< ISO 15924 - Ogam */
    TEXT_SCRIPT_OLD_ITALIC,                 /**< ISO 15924 - Ital */
    TEXT_SCRIPT_ORIYA,                      /**< ISO 15924 - Orya */
    TEXT_SCRIPT_RUNIC,                      /**< ISO 15924 - Runr */
    TEXT_SCRIPT_SINHALA,                    /**< ISO 15924 - Sinh */
    TEXT_SCRIPT_SYRIAC,                     /**< ISO 15924 - Syrc (Syrj, Syrn, Syre) */
    TEXT_SCRIPT_TAMIL,                      /**< ISO 15924 - Taml */
    TEXT_SCRIPT_TELUGU,                     /**< ISO 15924 - Telu */
    TEXT_SCRIPT_THAANA,                     /**< ISO 15924 - Thaa */
    TEXT_SCRIPT_THAI,                       /**< ISO 15924 - Thai */
    TEXT_SCRIPT_TIBETAN,                    /**< ISO 15924 - Tibt */
    TEXT_SCRIPT_CANADIAN_ABORIGINAL,        /**< ISO 15924 - Cans */
    TEXT_SCRIPT_YI,                         /**< ISO 15924 - Yiii */
    TEXT_SCRIPT_TAGALOG,                    /**< ISO 15924 - Tglg */
    TEXT_SCRIPT_HANUNOO,                    /**< ISO 15924 - Hano */
    TEXT_SCRIPT_BUHID,                      /**< ISO 15924 - Buhd */
    TEXT_SCRIPT_TAGBANWA,                   /**< ISO 15924 - Tagb */

    /* Unicode-4.0 additions */
    TEXT_SCRIPT_BRAILLE,                    /**< ISO 15924 - Brai */
    TEXT_SCRIPT_CYPRIOT,                    /**< ISO 15924 - Cprt */
    TEXT_SCRIPT_LIMBU,                      /**< ISO 15924 - Limb */
    TEXT_SCRIPT_OSMANYA,                    /**< ISO 15924 - Osma */
    TEXT_SCRIPT_SHAVIAN,                    /**< ISO 15924 - Shaw */
    TEXT_SCRIPT_LINEAR_B,                   /**< ISO 15924 - Linb */
    TEXT_SCRIPT_TAI_LE,                     /**< ISO 15924 - Tale */
    TEXT_SCRIPT_UGARITIC,                   /**< ISO 15924 - Ugar */
                                              
    /* Unicode-4.1 additions */                  
    TEXT_SCRIPT_NEW_TAI_LUE,                /**< ISO 15924 - Talu */
    TEXT_SCRIPT_BUGINESE,                   /**< ISO 15924 - Bugi */
    TEXT_SCRIPT_GLAGOLITIC,                 /**< ISO 15924 - Glag */
    TEXT_SCRIPT_TIFINAGH,                   /**< ISO 15924 - Tfng */
    TEXT_SCRIPT_SYLOTI_NAGRI,               /**< ISO 15924 - Sylo */
    TEXT_SCRIPT_OLD_PERSIAN,                /**< ISO 15924 - Xpeo */
    TEXT_SCRIPT_KHAROSHTHI,                 /**< ISO 15924 - Khar */

    TEXT_SCRIPT_MAX

}SPML_SCRIPT_E;

typedef enum
{                     
    SPML_VECTOR_CACHE,
    SPML_FILE_CACHE,
    SPML_GLYPH_CACHE,
	SPML_ADVANCE_CACHE,
}SPML_CACHE_TYPE_E;

typedef enum
{                     
    SPML_FONT_STORE_DEFAULT,  //保存在内存或nor中, 可直接读取
    SPML_FONT_STORE_NAND,
    SPML_FONT_STORE_FILE,
    SPML_FONT_STORE_MAX
}SPML_FONT_STORE_E;

//Set Arabic or Urdu number characters right to left
typedef enum
{
    SPML_BIDI_ARABIC_NUMBER_RIGHT_TO_LEFT = 0,
    SPML_BIDI_URDU_NUMBER_RIGHT_TO_LEFT,
    SPML_BIDI_ARABIC_URDU_NUMBER_RIGHT_TO_LEFT,
    SPML_BIDI_NUMBER_RIGHT_TO_LEFT_MAX
}SPML_BIDI_NUMBER_RIGHT_TO_LEFT_E;

typedef struct
{
    void*        (*MemPoolMalloc) ( uint32 handle, uint32 size );
    void         (*MemPoolFree)   ( void* mem_ptr );
    uint32       (*MemPoolConfig) ( SPML_CACHE_TYPE_E type, uint32 handle, BOOLEAN is_create );
    BOOLEAN      (*GetFontData)   ( SPML_FONT_STORE_E store, uint32 handle, void*  data_ptr, uint32 offset, uint32 size );
    BOOLEAN      (*IsNeedBidi)    ( void );
    uint16       (*GetFontLibNum) ( void );
    BOOLEAN      (*GetFontLibPtr) ( uint16 index, uint8** lib_ptr, uint32* lib_size );
    uint32       (*GetFontLangTag)( void );

    uint8*  vector_buffer_ptr;       //矢量字体处理缓冲区
    uint32  size_of_vector_buffer;   //显示缓冲区大小
    SPML_FONT_STORE_E store_type;    //默认字库的保存类型

}SPML_INIT_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init spml
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:初始化显示引擎
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_Init( 
                            const SPML_INIT_DATA_T* init_ptr
                            );

//Set Arabic or Urdu number characters right to left
/*****************************************************************************/
// 	Description : Set Arabic or Urdu number characters right to left
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_SetBidiNumberRightToLeft(
                            const SPML_BIDI_NUMBER_RIGHT_TO_LEFT_E set_bidi_right_to_left
                            );

/*****************************************************************************/
// 	Description : load font
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:加载T卡上的字体
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_LoadFont(
                                uint32 lib_size 
                                );

/*****************************************************************************/
// 	Description : unload font
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:卸载T卡上的字体
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_UnloadFont(
                                  void
                                  );

/*****************************************************************************/
// 	Description : get text direction
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:获得文字的方向, TEXT_DIR_LTR or TEXT_DIR_RTL, 如果系统中没有阿拉伯语
//       或希伯来语等RTL语言, 恒返回TEXT_DIR_LTR
/*****************************************************************************/
PUBLIC SPML_TEXT_DIR_T SPMLAPI_GetTextDirection(
                                                const wchar* text_ptr, //unicode字符串
                                                int32 len              //字符个数
                                                );

/*****************************************************************************/
// 	Description : is word break char
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:是否为按词断行字符,在按词断行时起作用,一般的标点符号都是
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_IsWordBreakChar(
                                       wchar ch //unicode字符
                                       );

/*****************************************************************************/
// 	Description : get char script
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:获得字符的script
/*****************************************************************************/
PUBLIC SPML_SCRIPT_E SPMLAPI_GetCharScript(
                                           wchar ch
                                           );

/*****************************************************************************/
// 	Description : line break
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:断行函数, 分为按像素断行和按词断行, 返回显示宽度中能显示的字符个数
/*****************************************************************************/
PUBLIC uint16 SPMLAPI_LineBreak(
                                const SPML_TEXT_PARAM_T* param_ptr,     //显示参数
                                const wchar*             text_ptr,      //unicode字符串
                                uint16                   len,           //字符个数
                                uint16                   disp_width,    //显示宽度
                                uint16*                  out_width_ptr, //out, 能显示的字符的宽度
                                BOOLEAN                  by_word        //是否按词断行
                                );

/*****************************************************************************/
// 	Description : get cursor coordinate
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:根据光标位置获得光标x坐标
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorCoordinate(
                                         const SPML_TEXT_PARAM_T* param_ptr, //显示参数
                                         const wchar*             text_ptr,  //unicode字符串
                                         uint16                   len,       //字符个数
                                         int32                    pos        //光标位置
                                         );

/*****************************************************************************/
// 	Description : get cursor pos
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:根据光标x坐标获得光标位置并微调光标位置
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorInfo(
                                   const SPML_TEXT_PARAM_T* param_ptr, //显示参数
                                   const wchar*             text_ptr,  //unicode字符串
                                   uint16                   len,       //字符个数
                                   BOOLEAN                  is_left,   //left or right
                                   int32*                   pos_ptr    //光标原位置, in\out
                                   );

/*****************************************************************************/
// 	Description : get cursor pos
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:根据光标x坐标获得光标位置
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorPos(
                                  const SPML_TEXT_PARAM_T* param_ptr,   //显示参数
                                  const wchar*             text_ptr,    //unicode字符串
                                  uint16                   len,         //字符个数
                                  int32                    pos,         //光标原位置
                                  int32                    x,           //光标x坐标
                                  BOOLEAN                  *is_find_ptr //in/out:may PNULL
                                  );

/*==============================================================================
Description: 根据光标x坐标获得光标位置
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 同时获得光标与字符的相对位置信息
==============================================================================*/
PUBLIC BOOLEAN SPMLAPI_GetCursorPosEx(
                                      const SPML_TEXT_PARAM_T* param_ptr,   //显示参数
                                      const wchar*             text_ptr,    //unicode字符串
                                      uint16                   len,         //字符个数
                                      int32                    x,           //光标x坐标
                                      int32        *nearest_chr_idx_ptr,
                                      int32        *under_chr_idx_ptr,
                                      int32        *under_len_ptr
                                      );

/*****************************************************************************/
// 	Description : get text metrics
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:获得字符串宽高
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetTextMetrics(
                                    const SPML_TEXT_PARAM_T* param_ptr,  //显示参数
                                    const wchar*             text_ptr,   //unicode字符串
                                    uint16                   len,        //字符个数
                                    SPML_TEXT_METRICS_T*     metrics_ptr //out, 字符串的宽高
                                    );

/*****************************************************************************/
// 	Description : draw text
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:绘制字符串
/*****************************************************************************/
PUBLIC int32 SPMLAPI_DrawText(
                              const SPML_TEXT_PARAM_T* param_ptr,  //显示参数
                              const wchar*             text_ptr,   //unicode字符串
                              uint16                   len,        //字符个数
                              SPML_TEXT_METRICS_T*     metrics_ptr //out, 字符串的宽高, 可以为PNULL
                              );

/*****************************************************************************/
// 	Description : test char
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:是否支持渲染该unicode
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_TestChar(
                                wchar ch
                                );

#ifdef SCRIPTEASY_SUPPORT
PUBLIC SPML_TEXT_DIR_T Scripteasy_GetTextDirection(const wchar* text_ptr, //unicode字符串
												int32 len              //字符个数
												);
#define SPMLAPI_GetTextDirection Scripteasy_GetTextDirection


PUBLIC BOOLEAN Scripteasy_TestChar(wchar ch);
#define SPMLAPI_TestChar Scripteasy_TestChar

#endif

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif /* SPML_API_H */
