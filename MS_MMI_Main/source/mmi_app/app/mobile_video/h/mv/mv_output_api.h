#ifndef  _MV_OUTPUT_API_H
#define  _MV_OUTPUT_API_H

#include "_Types.h"

/*****************************************************************************/
//  Description:  create font
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_HANDLE CM_Font_Create(_WCHAR* nw , _INT nSIze , _UINT nStyle);

/*****************************************************************************/
//  Description: release font
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_VOID CM_Font_Release(_HANDLE hFont);

/*****************************************************************************/
//  Description:  get font height
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/

_INT CM_Font_GetFontHeight(_HANDLE hFont);

/*****************************************************************************/
//  Description:  get char width
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_INT  CM_Font_GetCharWidth(_HANDLE hFont,_WCHAR wChar);

/*****************************************************************************/
//  Description:  display text string
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_INT CM_Font_TextOut(_HANDLE hFont,
  						_WCHAR wchar,
  						_INT nLength,
  						_UINT16* pBuffer,
  						_UINT16 vBufWidth,
  						_UINT16 vBufHeight,
  						_UINT16 x,
  						_UINT y,
  						_UINT16 vMode, 		//输出位置和字体样式
  						_UINT16 clipX,
  						_UINT16 clipY,
  						_UINT16 clipRight,
  						_UINT16 clipBottom,//剪裁区域
  						_UINT32 vColor
    						); 								 //当前字体颜色

/*****************************************************************************/
//  Description:  refresh area screen
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_VOID CM_RefreshAreaScreen(void* pBuffer,    
											_INT nX,
											_INT nY,
											_INT nWidth,
											_INT nHeight
											);

/*****************************************************************************/
//  Description:  refresh area screen
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_UINT16* CM_GetScreenBuffer(void);

/*****************************************************************************/
//  Description:  get font size
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_INT  CM_Font_GetSize(_HANDLE hFont);

/*****************************************************************************/
//  Description:  to check whether the font is bold
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_BOOLEAN CM_Font_IsBold (_HANDLE hFont);

/*****************************************************************************/
//  Description:  to check whether the font is italic
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_BOOLEAN CM_Font_IsItalic(_HANDLE hFont);

/*****************************************************************************/
//  Description:  to check whether the font is underline
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_BOOLEAN CM_Font_IsUnderline(_HANDLE hFont);
#endif