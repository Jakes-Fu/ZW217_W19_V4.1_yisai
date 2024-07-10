/****************************************************************************
** File Name:      caffont.h                                            *
** Author:         Xiaoqing.Lu                                             *
** Date:           05/19/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        Xiaoqing.Lu       Create
** 
****************************************************************************/

#ifndef  _CAFFONT_H_    
#define  _CAFFONT_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "caf.h"
#include "cafguid_def.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

typedef struct IFONT_T        IFONT_T;


#define IFONT_QueryInterface( _this_ , _guid_, _pptr_ )\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->QueryInterface( _this_ , _guid_, _pptr_ )

#define IFONT_AddRef( _this_ )\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->AddRef( _this_ )

#define IFONT_Release( _this_ )\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->Release( _this_ )

#define IFONT_GetFontWidth( _this_, _font_ )\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->GetFontWidth( _this_, _font_ )

#define IFONT_GetFontHeight( _this_, _font_ )\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->GetFontHeight( _this_, _font_ )

#define IFONT_GetStringWidth( _this_, _font_, _str_ptr )\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->GetStringWidth( _this_, _font_, _str_ptr )

#define IFONT_GetStringHeight( _this_, _font_, _str_ptr )\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->GetStringHeight( _this_, _font_, _str_ptr )

#define IFONT_ConvertUCS2GB( _this_, _ucs_ptr_, _length_, _gb_ptr_)\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->ConvertUCS2GB( _this_, _ucs_ptr_, _length_, _gb_ptr_)

#define IFONT_ConvertGB2UCS( _this_, _gb_ptr_, _length_, _ucs_ptr_)\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->ConvertGB2UCS( _this_, _gb_ptr_, _length_, _ucs_ptr_)

#define IFONT_ConvertUCS2B2UTF8( _this_, _ucs2b_ptr_, _ucs2b_len_, _utf8_ptr_, _utf8_buf_len_)\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->ConvertUCS2B2UTF8( _this_, _ucs2b_ptr_, _ucs2b_len_, _utf8_ptr_, _utf8_buf_len_)
    
#define IFONT_ConvertUTF82UCS2B( _this_, _utf8_ptr_, _utf8_len_, _ucs2b_ptr_, _ucs2b_buf_len_)\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->ConvertUTF82UCS2B( _this_, _utf8_ptr_, _utf8_len_, _ucs2b_ptr_, _ucs2b_buf_len_)

#define IFONT_ConvertUCS2BE2UCS2LE( _this_, ucs2b_ptr, _ucs2b_len_, _ucs2l_ptr_, _ucs2l_buf_len_)\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->ConvertUCS2BE2UCS2LE( _this_, _ucs2b_len_, _ucs2l_ptr_, _ucs2l_buf_len_)

#define IFONT_ConvertUCS2LE2UCS2BE( _this_, _ucs2l_ptr_, _ucs2l_len_, _ucs2b_ptr_, _ucs2b_buf_len_)\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->ConvertUCS2LE2UCS2BE( _this_, _ucs2l_ptr_, _ucs2l_len_, _ucs2b_ptr_, _ucs2b_buf_len_)

#define IFONT_GetCharacterPixelArry( _this_, _text_ptr_, _font_)\
    CAF_GET_VTBLPTR( _this_ ,IFONT_T)->GetCharacterPixelArry( _this_, _text_ptr_, _font_)

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/


// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(IFONT_T)
{
    INHERIT_IUNKNOWN(IFONT_T);

    // get font width or height by font type, example SONG_FONT_8 etc.
    uint16 (*GetFontWidth)(IFONT_T *ifont_ptr, GUI_FONT_T font_type);
    uint16 (*GetFontHeight)(IFONT_T *ifont_ptr, GUI_FONT_T font_type);

    // get the max width or height of a string
    uint16 (*GetStringWidth)(IFONT_T *ifont_ptr, GUI_FONT_T font_type, MMI_STRING_T *str_ptr);
    uint16 (*GetStringHeight)(IFONT_T *ifont_ptr, GUI_FONT_T font_type, MMI_STRING_T *str_ptr);

    // convert font coding format
    uint16 (*ConvertUCS2GB)(IFONT_T *ifont_ptr, const uint8 *ucs_ptr, uint16 length, uint8 *gb_ptr);
    uint16 (*ConvertGB2UCS)(IFONT_T *ifont_ptr, const uint8 *gb_ptr, uint16 length, uint8 *ucs_ptr);

    uint16 (*ConvertUCS2B2UTF8)(IFONT_T *ifont_ptr, const uint8 *ucs2b_ptr, uint16 ucs2b_len, uint8 *utf8_ptr, uint16 utf8_buf_len);
    uint16 (*ConvertUTF82UCS2B)(IFONT_T *ifont_ptr, const uint8 *utf8_ptr, uint16 utf8_len, uint8 *ucs2b_ptr, uint16 ucs2b_buf_len);

    uint16 (*ConvertUCS2BE2UCS2LE)(IFONT_T *ifont_ptr, const uint8 *ucs2b_ptr, uint16 ucs2b_len, uint8 *ucs2l_ptr, uint16 ucs2l_buf_len);
    uint16 (*ConvertUCS2LE2UCS2BE)(IFONT_T *ifont_ptr, const uint8 *ucs2l_ptr, uint16 ucs2l_len, uint8 *ucs2b_ptr, uint16 ucs2b_buf_len);

    uint16 (*GetCharacterPixelArry)(IFONT_T *ifont_ptr, const uint8 *text_ptr, GUI_FONT_T  font);
};


// Define pointer to poit the virtual table of the control base class
struct IFONT_T
{
    CAF_VTBL_T(IFONT_T)  *vtbl_ptr;
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFFONT_H_
