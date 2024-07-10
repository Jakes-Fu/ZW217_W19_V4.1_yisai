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
//����SPML�ڲ�ʹ�û����������Ż�
#define SPML_CACHE_SUPPORT
#define SPML_CACHE_MASK 0xFF
#define SPML_CACHE_SIZE (SPML_CACHE_MASK + 1)

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef CAF_POINT_T SPML_POINT_T;
typedef CAF_RECT_T  SPML_RECT_T;

typedef uint16  SPML_COLOR_T;                  //��ɫ
typedef uint16  SPML_TEXT_SIZE_T;              //�����С, Ϊopentype�ж���������С

typedef int8    SPML_TEXT_DIR_T;               //���ֵķ���
#define TEXT_DIR_LTR            (0)            //��������
#define TEXT_DIR_RTL            (1)            //��������
#define TEXT_DIR_AUTO           (-1)           //���ı��������

typedef uint32  SPML_TEXT_FORMAT_T;
#define TEXT_FORMAT_NORMAL       0x00000000
#define TEXT_FORMAT_LEFT         TEXT_FORMAT_NORMAL //�����
#define TEXT_FORMAT_RIGHT        0x00000001         //�Ҷ���
#define TEXT_FORMAT_CENTER       0x00000002         //ˮƽ�ж���
#define TEXT_FORMAT_TOP          TEXT_FORMAT_NORMAL //�϶���
#define TEXT_FORMAT_BOTTOM       0x00000004         //�¶���
#define TEXT_FORMAT_MIDDLE       0x00000008         //��ֱ�ж���

#define TEXT_FORMAT_INVERSE      0x00000010         //����Ч��
#define TEXT_FORMAT_UNDERLINE    0x00000020         //�»���Ч��
#define TEXT_FORMAT_BACKGROUND   0x00000040         //����ɫ
#define TEXT_FORMAT_BG_RECT      0x00000080         //����ɫ������ʾ����, Ĭ�ϻ�����������
#define TEXT_FORMAT_EDGE         0x00000100         //��ԵЧ��
#define TEXT_FORMAT_BOLD         0x00000200         //����
#define TEXT_FORMAT_ITALIC       0x00000400         //б��

#define TEXT_FORMAT_WORDSPACE    0x00001000         //֧��word space


#define TEXT_FORMAT_NO_AUTOBY    0x00002000       //����Ҫ�Զ�����bearing y�Ĺ���
#define TEXT_FORMAT_BRIGHTNESS   0x00004000       //�����������ȣ����Ҷ�ֵ
#define TEXT_FORMAT_ELLIPSIS     0x00008000       //��ʾ����

#define TEXT_FORMAT_FROM_HEAD    0x00010000        //��ͷ��ʾ
#define TEXT_FORMAT_FROM_TAIL    0x00020000        //��β����ʾ, �������ҹ���, �����ַ�����ȳ�����ʾ���ʱ����Ч
#define TEXT_FORMAT_ELLIPSIS_EX  0x00040000       //��ʾ...

#define SPML_DEFUALT_BRIGHTNESS  0x80             //Ĭ������ֵ�����������ʸ������ĻҶ�ֵ

typedef uint16 SPML_BUF_TYPE_T;
#define SPML_BUF_DEFAULT  0                        //default
#define SPML_BUF_RGB565   SPML_BUF_DEFAULT         //565
#define SPML_BUF_ARGB888  (SPML_BUF_DEFAULT + 1)   //argb8888
#define SPML_BUF_TYPE_MAX (SPML_BUF_DEFAULT + 2)   //max

typedef struct
{
    uint16  width;  //���
    uint16  height; //�߶�
    //�����ֶ���ʱ����
    uint16  ascent;
    uint16  leading;
    int16   lsb;
    int16   rsb;
    int16   bearing_y;
}SPML_TEXT_METRICS_T;

typedef struct
{
    SPML_TEXT_SIZE_T    size;   //�����С
    SPML_TEXT_FORMAT_T  format; //���ָ�ʽ
    SPML_COLOR_T        color;  //������ɫ
}SPML_TEXT_STYLE_T;

typedef struct
{
    int32               start;  //��ʼλ��
    int32               end;    //����λ��
    SPML_TEXT_STYLE_T   style;  //���ַ��
}SPML_TEXT_REGION_T;

typedef struct
{
    SPML_TEXT_FORMAT_T  format;         //���ֵĸ�ʽ
    SPML_TEXT_DIR_T     dir;            //���ַ���
    SPML_TEXT_SIZE_T    size;           //���ִ�С
    uint16              glyph_space;    //���μ��
    uint16              region_num;     //��Ч����
    const SPML_TEXT_REGION_T* region_ptr; //��Ч����

    //���²���, �ڻ�������ʱ��Ҫ���
    SPML_COLOR_T        font_color;     //������ɫ
    SPML_COLOR_T        bg_color;       //������ɫ
    SPML_COLOR_T        edge_color;     //���ֱ�Ե��ɫ
    
    SPML_COLOR_T        line_color;     //�»��ߵ���ɫ
    SPML_BUF_TYPE_T     buffer_type;    //buffer������
    uint16              buffer_width;   //buffer�Ŀ��
    uint16              buffer_height;  //buffer�ĸ߶�
    void*               buffer_ptr;     //��ʾ��buffer, ����Ϊ��
    SPML_RECT_T         display_rect;   //��ʾ����, �������ʾbuffer��ԭ��
    SPML_RECT_T         clip_rect;      //��������, �������ʾbuffer��ԭ��
    
    uint16              word_space;     //�ʼ��, ���ڿ��ƿո���ռ���ֿ�
    uint8               brightness;     //ʸ������С��16�ţ������������
    uint8               edge_alpha;     //���ֱ�Եalpha

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
    SPML_FONT_STORE_DEFAULT,  //�������ڴ��nor��, ��ֱ�Ӷ�ȡ
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

    uint8*  vector_buffer_ptr;       //ʸ�����崦������
    uint32  size_of_vector_buffer;   //��ʾ��������С
    SPML_FONT_STORE_E store_type;    //Ĭ���ֿ�ı�������

}SPML_INIT_DATA_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init spml
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:��ʼ����ʾ����
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
//	Note:����T���ϵ�����
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_LoadFont(
                                uint32 lib_size 
                                );

/*****************************************************************************/
// 	Description : unload font
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:ж��T���ϵ�����
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_UnloadFont(
                                  void
                                  );

/*****************************************************************************/
// 	Description : get text direction
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:������ֵķ���, TEXT_DIR_LTR or TEXT_DIR_RTL, ���ϵͳ��û�а�������
//       ��ϣ�������RTL����, �㷵��TEXT_DIR_LTR
/*****************************************************************************/
PUBLIC SPML_TEXT_DIR_T SPMLAPI_GetTextDirection(
                                                const wchar* text_ptr, //unicode�ַ���
                                                int32 len              //�ַ�����
                                                );

/*****************************************************************************/
// 	Description : is word break char
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:�Ƿ�Ϊ���ʶ����ַ�,�ڰ��ʶ���ʱ������,һ��ı����Ŷ���
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_IsWordBreakChar(
                                       wchar ch //unicode�ַ�
                                       );

/*****************************************************************************/
// 	Description : get char script
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����ַ���script
/*****************************************************************************/
PUBLIC SPML_SCRIPT_E SPMLAPI_GetCharScript(
                                           wchar ch
                                           );

/*****************************************************************************/
// 	Description : line break
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���к���, ��Ϊ�����ض��кͰ��ʶ���, ������ʾ���������ʾ���ַ�����
/*****************************************************************************/
PUBLIC uint16 SPMLAPI_LineBreak(
                                const SPML_TEXT_PARAM_T* param_ptr,     //��ʾ����
                                const wchar*             text_ptr,      //unicode�ַ���
                                uint16                   len,           //�ַ�����
                                uint16                   disp_width,    //��ʾ���
                                uint16*                  out_width_ptr, //out, ����ʾ���ַ��Ŀ��
                                BOOLEAN                  by_word        //�Ƿ񰴴ʶ���
                                );

/*****************************************************************************/
// 	Description : get cursor coordinate
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ݹ��λ�û�ù��x����
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorCoordinate(
                                         const SPML_TEXT_PARAM_T* param_ptr, //��ʾ����
                                         const wchar*             text_ptr,  //unicode�ַ���
                                         uint16                   len,       //�ַ�����
                                         int32                    pos        //���λ��
                                         );

/*****************************************************************************/
// 	Description : get cursor pos
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ݹ��x�����ù��λ�ò�΢�����λ��
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorInfo(
                                   const SPML_TEXT_PARAM_T* param_ptr, //��ʾ����
                                   const wchar*             text_ptr,  //unicode�ַ���
                                   uint16                   len,       //�ַ�����
                                   BOOLEAN                  is_left,   //left or right
                                   int32*                   pos_ptr    //���ԭλ��, in\out
                                   );

/*****************************************************************************/
// 	Description : get cursor pos
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ݹ��x�����ù��λ��
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorPos(
                                  const SPML_TEXT_PARAM_T* param_ptr,   //��ʾ����
                                  const wchar*             text_ptr,    //unicode�ַ���
                                  uint16                   len,         //�ַ�����
                                  int32                    pos,         //���ԭλ��
                                  int32                    x,           //���x����
                                  BOOLEAN                  *is_find_ptr //in/out:may PNULL
                                  );

/*==============================================================================
Description: ���ݹ��x�����ù��λ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ͬʱ��ù�����ַ������λ����Ϣ
==============================================================================*/
PUBLIC BOOLEAN SPMLAPI_GetCursorPosEx(
                                      const SPML_TEXT_PARAM_T* param_ptr,   //��ʾ����
                                      const wchar*             text_ptr,    //unicode�ַ���
                                      uint16                   len,         //�ַ�����
                                      int32                    x,           //���x����
                                      int32        *nearest_chr_idx_ptr,
                                      int32        *under_chr_idx_ptr,
                                      int32        *under_len_ptr
                                      );

/*****************************************************************************/
// 	Description : get text metrics
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����ַ������
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetTextMetrics(
                                    const SPML_TEXT_PARAM_T* param_ptr,  //��ʾ����
                                    const wchar*             text_ptr,   //unicode�ַ���
                                    uint16                   len,        //�ַ�����
                                    SPML_TEXT_METRICS_T*     metrics_ptr //out, �ַ����Ŀ��
                                    );

/*****************************************************************************/
// 	Description : draw text
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:�����ַ���
/*****************************************************************************/
PUBLIC int32 SPMLAPI_DrawText(
                              const SPML_TEXT_PARAM_T* param_ptr,  //��ʾ����
                              const wchar*             text_ptr,   //unicode�ַ���
                              uint16                   len,        //�ַ�����
                              SPML_TEXT_METRICS_T*     metrics_ptr //out, �ַ����Ŀ��, ����ΪPNULL
                              );

/*****************************************************************************/
// 	Description : test char
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:�Ƿ�֧����Ⱦ��unicode
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_TestChar(
                                wchar ch
                                );

#ifdef SCRIPTEASY_SUPPORT
PUBLIC SPML_TEXT_DIR_T Scripteasy_GetTextDirection(const wchar* text_ptr, //unicode�ַ���
												int32 len              //�ַ�����
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
