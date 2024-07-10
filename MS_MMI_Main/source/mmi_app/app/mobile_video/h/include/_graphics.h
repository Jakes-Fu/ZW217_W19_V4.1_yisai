//
//	_Graphics.h
//
#ifndef __GRAPHICS_API_H
#define	__GRAPHICS_API_H

#ifdef	__cplusplus
extern "C"
{
#endif

typedef _DWORD		_COLORREF;
#define _RGB(r,g,b)	((_COLORREF)(((_BYTE)(r)|((_WORD)((_BYTE)(g))<<8))|(((_DWORD)(_BYTE)(b))<<16)))

//////////////////////////////////////////////////////////////////////////
//	Font
typedef enum
{
	EFontStyle_Normal	= 0x00,
	EFontStyle_Bold		= 0x02,
	EFontStyle_Italic	= 0x04,
	EFontStyle_Underline= 0x08,
} TFontStyleEnum;

extern _HANDLE Font_Create(_WCHAR*, _INT, _UINT);
extern _VOID Font_Release(_HANDLE);
//extern _INT  Font_GetSize(_HANDLE);
//extern _BOOLEAN Font_IsBold(_HANDLE);
//extern _BOOLEAN Font_IsItalic(_HANDLE);
//extern _BOOLEAN Font_IsUnderline(_HANDLE);
extern _INT  Font_GetCharWidth(_HANDLE, _WCHAR);
extern _INT  Font_GetFontHeight(_HANDLE);


//////////////////////////////////////////////////////////////////////////
//	Pixmap
extern _HANDLE Pixmap_Create(_INT, _INT, _INT);
//extern _HANDLE Pixmap_Load(_INT);
extern _HANDLE Pixmap_LoadFromResource(_CONST _WCHAR*, _INT);
extern _HANDLE Pixmap_LoadFromFile(_CONST _WCHAR*);
extern _VOID Pixmap_Release(_HANDLE);
extern _INT  Pixmap_GetWidth(_HANDLE);
extern _INT  Pixmap_GetHeight(_HANDLE);
extern _INT  Pixmap_GetColorBits(_HANDLE);
extern _INT  Pixmap_GetBytesWidth(_HANDLE);
extern _UINT16* Pixmap_GetPalettePtr(_HANDLE);
extern _UINT8*  Pixmap_GetBufferPtr(_HANDLE);
extern _BOOLEAN Pixmap_GetPalette(_HANDLE, _COLORREF*, _INT);
extern _BOOLEAN Pixmap_SetPalette(_HANDLE, _COLORREF*, _INT);
extern _BOOLEAN Pixmap_GetBuffer(_HANDLE, _BYTE*, _INT);
extern _BOOLEAN Pixmap_SetBuffer(_HANDLE, _BYTE*, _INT);
extern _BOOLEAN Pixmap_GetLine(_HANDLE, _COLORREF*, _INT, _INT);

//////////////////////////////////////////////////////////////////////////
//	Canvas
extern _HANDLE Canvas_Create(_INT, _INT, _UINT);
extern _VOID Canvas_Release(_HANDLE);
extern _VOID Canvas_SetViewPort(_HANDLE, _INT, _INT);
extern _VOID Canvas_ClearViewPort(_HANDLE);
extern _BOOLEAN Canvas_GetViewPort(_HANDLE, _INT*, _INT*);
extern _VOID Canvas_SetClippingRect(_HANDLE, _INT, _INT, _INT, _INT);
extern _VOID Canvas_ClearClippingRect(_HANDLE);
extern _BOOLEAN Canvas_GetClipRect(_HANDLE, _INT*, _INT*, _INT*, _INT*);
extern _VOID Canvas_Flip(_HANDLE, _INT, _INT, _INT, _INT);
extern _HANDLE Canvas_SetFont(_HANDLE, _HANDLE);
extern _HANDLE Canvas_GetFont(_HANDLE);
extern _VOID Canvas_SetPenColor(_HANDLE, _COLORREF);
extern _COLORREF Canvas_GetPenColor(_HANDLE);
extern _VOID Canvas_CharOut(_HANDLE, _INT, _INT, _WCHAR);
extern _VOID Canvas_TextOut(_HANDLE, _INT, _INT, _CONST _WCHAR*);
extern _VOID Canvas_ShowPixmap(_HANDLE, _INT, _INT, _HANDLE, _INT, _INT, _INT, _INT);
extern _VOID Canvas_ShowPixmapAlpha(_HANDLE, _INT, _INT, _HANDLE, _INT, _INT, _INT, _INT, _INT);
extern _VOID Canvas_SetPixel(_HANDLE, _INT, _INT, _COLORREF);
extern _COLORREF Canvas_GetPixel(_HANDLE, _INT, _INT);
extern _VOID Canvas_DrawLine(_HANDLE, _INT, _INT, _INT, _INT);
extern _VOID Canvas_DrawRect(_HANDLE, _INT, _INT, _INT, _INT);
extern _VOID Canvas_FillRect(_HANDLE, _INT, _INT, _INT, _INT);
extern _VOID Canvas_DrawEllipse(_HANDLE, _INT, _INT, _INT, _INT);
extern _VOID Canvas_FillEllipse(_HANDLE, _INT, _INT, _INT, _INT);

extern _VOID Canvas_Shadow(_HANDLE, _INT, _INT, _INT, _INT, _INT);
extern _VOID Canvas_Mix(_HANDLE, _UINT32, _INT, _INT, _INT, _INT, _INT);
extern _VOID Canvas_Gray(_HANDLE, _INT, _INT, _INT, _INT);

#ifdef	__cplusplus
}
#endif

#endif	//__GRAPHICS_API_H
