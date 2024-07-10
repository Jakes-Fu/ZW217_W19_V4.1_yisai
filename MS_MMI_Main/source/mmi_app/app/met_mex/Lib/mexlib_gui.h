/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
* 
* �ļ����ƣ� // MexLib_Gui.h
* �ļ���ʶ��
* ����ժҪ�� //mexƽ̨gui��
*
************************************************************************/

/************************************************************************
 *
 *  MEX GUI LIB
 *  MEXƽ̨ �ַ���/ͼ��/ͼ�� ������
 *  version 1.3
 *  2010.4.1    by met guo_tf
 *
 *  ˵����
 *      �� ʹ�ñ��⺯��ǰ������á�GUI_BEGIN����ָ��Ŀ�껺�棬��������
 *  ���á�GUI_END����������ơ�
 *      ��GUI_BEGIN����GUI_END������ɶԳ��֡����򽫳��ֱ������
 *      ʹ�ñ��⺯��ʱ��δ���á�GUI_BEGIN������GUI_END����Ĭ����Ļ����Ϊ
 *  Ŀ�껺�档
 *      �� �����ϸ������������ԭ�򣬼����ƺ�����������������Ͻǵ㣬����
 *  �����½ǵ㡣����δ����������ԭ��ĺ��������ں���ע�͵�ע������˵����
 *  �û�ʹ��ʱ��ע�⡣
 *      �� �����ṩ������ͼ�λ��ƺ�������β�ԡ�Internal����β�ĺ���Ϊʹ��
 *  �ײ��ͼ�λ��ƺ���������Ϊ�����ṩ��ͼ�λ��ƺ�����
 *      �� ͼ���汾���Ƽ�ʹ��MIF��ʽͼƬ��������ʽͼƬ��ʹ�õײ���뺯����
 *  �ʿ��ܳ����ڴ治�㵼��ͼ����ز���ȷ�������
 *
 **************************************************************************/

#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_GUI_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_GUI_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
	#define __MEXLIB_GUI_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
	#define __MEXLIB_GUI_CLIENT_H__
#endif

#include "MexLib_Os.h"

//common macro definitions for GUI
#define MEX_H_LAUNCH_FUNC_GUI					MEX_H_LAUNCH_FUNC( Gui )
#define MEX_C_LIB_FUNC_NAME_GUI					MEX_C_LIB_FUNC_NAME( Gui )

#define MEX_H_GET_FUNC_DEFINE_GUI(x)			MEX_H_GET_FUNC_DEFINE( Gui, x )

#define MEX_H_ENUM_FUNC_START_GUI				MEX_H_ENUM_FUNC_START( Gui )
#define MEX_H_ENUM_FUNC_GUI(x)					MEX_H_ENUM_FUNC( Gui, x )
#define MEX_H_ENUM_FUNC_END_GUI					MEX_H_ENUM_FUNC_END( Gui )

#define MEX_H_NB_FUNC_GUI						MEX_H_NB_FUNC( Gui )
//end common macro definitions

// config define ///////////////////////////////////////////////////////////////////////////////
#define MEX_ALPHA_LEVEL                         17

// image define //////////////////////////////////////////////////////////////////////////////////

// mex image type
#define MEX_IMAGE_TYPE_NULL                     0x0000
#define MEX_IMAGE_TYPE_NORMAL                   0x0001
#define MEX_IMAGE_TYPE_SHADOWCLONE              0x0002
#define MEX_IMAGE_TYPE_DEEPCLONE                0x0003
#define MEX_IMAGE_TYPE_SHELL                    0x0004
#define MEX_IMAGE_TYPE_ARRAYIMAGE               0x0005
#define MEX_IMAGE_TYPE_SHELL_BUFFER             MEX_IMAGE_TYPE_SHELL
#define MEX_IMAGE_TYPE_SHELL_BUFFER_ABUFFER     0x0006

// mex image attr
#define MEX_IMAGE_ATTR_NULL                     0x0000
#define MEX_IMAGE_ATTR_TRANSPARENT              0x0001
#define MEX_IMAGE_ATTR_TRANSACCEL               0x0002
#define MEX_IMAGE_ATTR_ALPHA                    0x0004
#define MEX_IMAGE_ATTR_PIXELALPHA               0x0008
#define MEX_IMAGE_ATTR_PALETTE                  0x0010
#define MEX_IMAGE_ATTR_INITSUCCESS              0x4000
#define MEX_IMAGE_ATTR_LOADSUCCESS              0x8000

// image load type
#define MEX_IMAGE_LOAD_FROM_ABSPATH             0x0001
#define MEX_IMAGE_LOAD_FROM_RELPATH             0x0002
#define MEX_IMAGE_LOAD_FROM_MEMORY              0x0003
#define MEX_IMAGE_LOAD_FROM_VIRMEMORY           0x0004

// overturn type
#define MEX_IMAGE_OVERTURN_NULL                 0x0000
#define MEX_IMAGE_OVERTURN_RIGHT                0x0001    // rotate 90 by C.W.
#define MEX_IMAGE_OVERTURN_BOTTOM               0x0002    // rotate 180 by C.W.
#define MEX_IMAGE_OVERTURN_LEFT                 0x0004    // rotate 270 by C.W.

// flip type
#define MEX_IMAGE_FLIP_NULL                     0x0000
#define MEX_IMAGE_FLIP_HORIZON                  0x0010
#define MEX_IMAGE_FLIP_VERTICAL                 0x0020
#define MEX_IMAGE_FLIP_CENTER                   0x0040

#define MEX_IMAGE_AFFINE_HORIZON                0x0001
#define MEX_IMAGE_AFFINE_VERTICAL               0x0002

// color define /////////////////////////////////////////////////////////////////////////////////
// color
#define MEX_COLOR_WHITE                         0xFFFF    // ����ɫ
#define MEX_COLOR_BLACK                         0x0000    // ����ɫ
#define MEX_COLOR_RED                           0xF800    // ����ɫ
#define MEX_COLOR_GREEN                         0x07E0    // ����ɫ
#define MEX_COLOR_BLUE                          0x001F    // ����ɫ
#define MEX_COLOR_YELLOW                        0xFFE0    // ����ɫ
#define MEX_COLOR_CYAN                          0x07FF    // ����ɫ
#define MEX_COLOR_MAGENTA                       0xF81F    // ���ɫ

#define MEX_COLOR_LIGHTPINK                     0xFDB8    // ǳ��ɫ
#define MEX_COLOR_PINK                          0xFE19    // ��ɫ
#define MEX_COLOR_DEEPPINK                      0xF8B2    // ���ɫ
#define MEX_COLOR_DARKMAGENTA                   0x8811    // �����
#define MEX_COLOR_PURPLE                        0x8010    // ��ɫ
#define MEX_COLOR_INDIGO                        0x4810    // ����ɫ
#define MEX_COLOR_MEDIUMPURPLE                  0x939B    // ���е���ɫ
#define MEX_COLOR_MEDIUMBLUE                    0x0019    // ���е���ɫ
#define MEX_COLOR_MIDNIGHTBLUE                  0x18CE    // ��ҹ��
#define MEX_COLOR_DARKBLUE                      0x0011    // ����ɫ
#define MEX_COLOR_NAVYBLUE                      0x0010    // ������
#define MEX_COLOR_LIGHTSKYBLUE                  0x867F    // ������ɫ
#define MEX_COLOR_SKYBLUE                       0x867D    // ����ɫ
#define MEX_COLOR_DARKSKYBLUE                   0x05FF    // ������ɫ
#define MEX_COLOR_LIGHTBLUE                     0xAEDC    // ǳ��ɫ
#define MEX_COLOR_LIGHTCYAN                     0xE7FF    // ����ɫ
#define MEX_COLOR_DARKCYAN                      0x0451    // ����ɫ
#define MEX_COLOR_LIGHTGREEN                    0x9772    // ǳ��ɫ
#define MEX_COLOR_FORESTGREEN                   0x2444    // ɭ����
#define MEX_COLOR_DARKGREEN                     0x0320    // ����ɫ
#define MEX_COLOR_YELLOWGREEN                   0xAFE5    // ����ɫ
#define MEX_COLOR_LIGHTYELLOW                   0xFFFC    // ǳ��ɫ
#define MEX_COLOR_OLIVE                         0x8400    // ���ɫ
#define MEX_COLOR_GOLD                          0xFEA0    // ��ɫ
#define MEX_COLOR_ORANGE                        0xFD20    // ��ɫ
#define MEX_COLOR_DARKORANGE                    0xFC60    // ���ɫ
#define MEX_COLOR_PEACHPUFF                     0xFED7    // ��ɫ
#define MEX_COLOR_CHOCOLATE                     0xD343    // �ɿ���ɫ
#define MEX_COLOR_ORANGERED                     0xFA20    // �Ⱥ�ɫ
#define MEX_COLOR_INDIANRED                     0xCAEB    // ӡ�Ⱥ�
#define MEX_COLOR_BROWN                         0xA145    // ��ɫ
#define MEX_COLOR_DARKRED                       0x8800    // ���ɫ
#define MEX_COLOR_LIGHTGRAY                     0xD69A    // ǳ��ɫ
#define MEX_COLOR_SILVER                        0xC618    // ��ɫ
#define MEX_COLOR_DARKGRAY                      0xAD55    // ���ɫ
#define MEX_COLOR_GRAY                          0x8410    // ��ɫ

// gui color macro
#define MEX_RGB565(r, g, b)                     (((b) & 31) + (((g) & 63) << 5) + (((r) & 31) << 11))
#define MEX_RGB888(a, r, g, b)                  ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
#define MEX_RGB888TO565(r, g, b)                MEX_RGB565(((r) >> 3), ((g) >> 2), ((b) >> 3))

#define MEX_EXTRACT_RGB565(clr, r, g, b)	    { r = ((clr) >> 11) & 31; g = ((clr) >> 5) & 63; b = (clr) & 31; }
#define MEX_EXTRACT_RGB888(clr, a, r, g, b)     { a = ((clr) >> 24) & 255; r = ((clr) >> 16) & 255; g = ((clr) >> 8) & 255; b = (clr) & 255; }

typedef struct _Mex_GuiActBuffer
{
	mex_color *buf_ptr;
	uint16 width, height;
	int16 clipx1, clipy1, clipx2, clipy2;
} Mex_GuiActBuffer;

// gui function
MEX_H_DECLARE_FUNC( void,			GetGuiActBuffer,				(Mex_GuiActBuffer *gui_act_buffer) )

MEX_H_DECLARE_FUNC( void,			DrawPoint,						(int16 x, int16 y, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawPointNoClip,				(int16 x, int16 y, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawLine,						(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFrameTriangle,				(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFillTriangle,				(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFrameRect,					(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFillRect,					(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFrameCircle,				(int16 x, int16 y, uint16 r, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFillCircle,					(int16 x, int16 y, uint16 r, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFrameEllipse,				(int16 x, int16 y, uint16 a, uint16 b, mex_color color) )
MEX_H_DECLARE_FUNC( void,			DrawFillEllipse,				(int16 x, int16 y, uint16 a, uint16 b, mex_color color) )

MEX_H_DECLARE_FUNC( void,			DrawPointAlpha,					(int16 x, int16 y, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawPointNoClipAlpha,			(int16 x, int16 y, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawLineAlpha,					(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFrameTriangleAlpha,			(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFillTriangleAlpha,			(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFrameRectAlpha,				(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFillRectAlpha,				(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFrameCircleAlpha,			(int16 x, int16 y, uint16 r, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFillCircleAlpha,			(int16 x, int16 y, uint16 r, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFrameEllipseAlpha,			(int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha) )
MEX_H_DECLARE_FUNC( void,			DrawFillEllipseAlpha,			(int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha) )

MEX_H_DECLARE_FUNC( bool,			GetImageDimension,				(int32 *width, int32 *height, int32 *numFrames, const wchar_t *path, uint8 *memData, size_t memSize, int16 imgLoadType) )
MEX_H_DECLARE_FUNC( bool,			GetMifImageDimensionFromFileHandle,	(int32 *width, int32 *height, int32 *pFileHandle) )
MEX_H_DECLARE_FUNC( bool,			InitImage,						(Mex_Image *image, int32 width, int32 height) )
MEX_H_DECLARE_FUNC( bool,			InitImageNoBuffer,				(Mex_Image *image, int32 width, int32 height) )
MEX_H_DECLARE_FUNC( bool,			InitImageShell,					(Mex_Image *image, mex_color *buffer, int32 width, int32 height) )
MEX_H_DECLARE_FUNC( bool,			CloneImageShadow,				(Mex_Image *dest_image, Mex_Image *src_image) )
MEX_H_DECLARE_FUNC( bool,			CloneImageDeep,					(Mex_Image *dest_image, Mex_Image *src_image) )
MEX_H_DECLARE_FUNC( bool,			FlipImage,						(Mex_Image *dest_image, Mex_Image *src_image, int16 type) )
MEX_H_DECLARE_FUNC( void,			ReleaseImage,					(Mex_Image *image) )
MEX_H_DECLARE_FUNC( bool,			LoadImage,						(Mex_Image *image, const wchar_t *path, uint8 *memData, size_t memSize, int32 frame, int16 imgLoadType) )
MEX_H_DECLARE_FUNC( bool,			LoadMifImageFromFileHandle,		(Mex_Image *image, int32 *pFileHandle, bool bCreateAcce) )

MEX_H_DECLARE_FUNC( int32,			GetImageMemorySize,				(Mex_Image *image) )
MEX_H_DECLARE_FUNC( void,			MexImageToByteArray,			(byte *array, Mex_Image *image) )
MEX_H_DECLARE_FUNC( Mex_Image*,		ByteArrayToMexImage,			(byte *array) )

MEX_H_DECLARE_FUNC( bool,			SetImageColorkey,				(Mex_Image *image, mex_color colorkey, bool isTrans) )
MEX_H_DECLARE_FUNC( bool,			SetImageColorkeyNoAccel,		(Mex_Image *image, mex_color colorkey, bool isTrans) )
MEX_H_DECLARE_FUNC( void,			SetImageAlpha,					(Mex_Image *image, byte alpha) )
MEX_H_DECLARE_FUNC( void,			SetImageDrawRegion,				(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2) )
MEX_H_DECLARE_FUNC( void,			SetImageRotateCenter,			(Mex_Image *image, int16 center_x, int16 center_y) )

MEX_H_DECLARE_FUNC( void,			DrawImage,						(Mex_Image *image, int16 x, int16 y) )
MEX_H_DECLARE_FUNC( void,			DrawImageScale,					(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2) )
MEX_H_DECLARE_FUNC( void,			DrawImageOverturn,				(Mex_Image *image, int16 x, int16 y, int16 type) )
MEX_H_DECLARE_FUNC( void,			DrawImageOverturnScale,			(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 type) )
MEX_H_DECLARE_FUNC( void,			DrawImageRotate,				(Mex_Image *image, int16 center_x, int16 center_y, int16 angle) )
MEX_H_DECLARE_FUNC( void,			DrawImageRotateScale,			(Mex_Image *image, uint16 scale_width, uint16 scale_height, int16 center_x, int16 center_y, int16 angle) )
MEX_H_DECLARE_FUNC( void,			DrawImageFlip,					(Mex_Image *image, int16 x, int16 y, int16 type) )
MEX_H_DECLARE_FUNC( void,			DrawImageFlipOverturn,			(Mex_Image *image, int16 x, int16 y, int16 type) )
MEX_H_DECLARE_FUNC( void,			DrawImageFlipRotate,			(Mex_Image *image, int16 center_x, int16 center_y, int16 angle, int16 type) )
MEX_H_DECLARE_FUNC( void,			DrawImageAffine,				(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, int16 x4, int16 y4, int16 type) )

MEX_H_DECLARE_FUNC( void,			MexImageToFileHandle,			(int32 *file, Mex_Image *image) )

#if MEX_SDK_VER >= 1011
MEX_H_DECLARE_FUNC( bool,			InitImageWithAbuffer,			(Mex_Image *image, int32 width, int32 height) )
MEX_H_DECLARE_FUNC( bool,			InitImageShellWithAbuffer,		(Mex_Image *image, mex_color *buffer, byte *Abuffer, int32 width, int32 height) )
#endif

MEX_H_ENUM_FUNC_START_GUI
	MEX_H_ENUM_FUNC_GUI( GetGuiActBuffer )

	MEX_H_ENUM_FUNC_GUI( DrawPoint )
	MEX_H_ENUM_FUNC_GUI( DrawPointNoClip )
	MEX_H_ENUM_FUNC_GUI( DrawLine )
	MEX_H_ENUM_FUNC_GUI( DrawFrameTriangle )
	MEX_H_ENUM_FUNC_GUI( DrawFillTriangle )
	MEX_H_ENUM_FUNC_GUI( DrawFrameRect )
	MEX_H_ENUM_FUNC_GUI( DrawFillRect )
	MEX_H_ENUM_FUNC_GUI( DrawFrameCircle )
	MEX_H_ENUM_FUNC_GUI( DrawFillCircle )
	MEX_H_ENUM_FUNC_GUI( DrawFrameEllipse )
	MEX_H_ENUM_FUNC_GUI( DrawFillEllipse )

	MEX_H_ENUM_FUNC_GUI( DrawPointAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawPointNoClipAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawLineAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFrameTriangleAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFillTriangleAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFrameRectAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFillRectAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFrameCircleAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFillCircleAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFrameEllipseAlpha )
	MEX_H_ENUM_FUNC_GUI( DrawFillEllipseAlpha )

	MEX_H_ENUM_FUNC_GUI( GetImageDimension )
	MEX_H_ENUM_FUNC_GUI( GetMifImageDimensionFromFileHandle )
	MEX_H_ENUM_FUNC_GUI( InitImage )
	MEX_H_ENUM_FUNC_GUI( InitImageNoBuffer )
	MEX_H_ENUM_FUNC_GUI( InitImageShell )
	MEX_H_ENUM_FUNC_GUI( CloneImageShadow )
	MEX_H_ENUM_FUNC_GUI( CloneImageDeep )
	MEX_H_ENUM_FUNC_GUI( FlipImage )
	MEX_H_ENUM_FUNC_GUI( ReleaseImage )
	MEX_H_ENUM_FUNC_GUI( LoadImage )
	MEX_H_ENUM_FUNC_GUI( LoadMifImageFromFileHandle )

	MEX_H_ENUM_FUNC_GUI( GetImageMemorySize )
	MEX_H_ENUM_FUNC_GUI( MexImageToByteArray )
	MEX_H_ENUM_FUNC_GUI( ByteArrayToMexImage )

	MEX_H_ENUM_FUNC_GUI( SetImageColorkey )
	MEX_H_ENUM_FUNC_GUI( SetImageColorkeyNoAccel )
	MEX_H_ENUM_FUNC_GUI( SetImageAlpha )
	MEX_H_ENUM_FUNC_GUI( SetImageDrawRegion )
	MEX_H_ENUM_FUNC_GUI( SetImageRotateCenter )

	MEX_H_ENUM_FUNC_GUI( DrawImage )
	MEX_H_ENUM_FUNC_GUI( DrawImageScale )
	MEX_H_ENUM_FUNC_GUI( DrawImageOverturn )
	MEX_H_ENUM_FUNC_GUI( DrawImageOverturnScale )
	MEX_H_ENUM_FUNC_GUI( DrawImageRotate )
	MEX_H_ENUM_FUNC_GUI( DrawImageRotateScale )
	MEX_H_ENUM_FUNC_GUI( DrawImageFlip )
	MEX_H_ENUM_FUNC_GUI( DrawImageFlipOverturn )
	MEX_H_ENUM_FUNC_GUI( DrawImageFlipRotate )
	MEX_H_ENUM_FUNC_GUI( DrawImageAffine )

	MEX_H_ENUM_FUNC_GUI( MexImageToFileHandle )

#if MEX_SDK_VER >= 1011
	MEX_H_ENUM_FUNC_GUI( InitImageWithAbuffer )
	MEX_H_ENUM_FUNC_GUI( InitImageShellWithAbuffer )
#endif

MEX_H_ENUM_FUNC_END_GUI

#if defined (MEX_TARGET_CLIENT)
	#define GetGuiActBuffer					MEX_H_GET_FUNC_DEFINE_GUI( GetGuiActBuffer )				// (Mex_GuiBuffer *gui_buffer)

	#define DrawPoint						MEX_H_GET_FUNC_DEFINE_GUI( DrawPoint )						// (int16 x, int16 y, mex_color color)
	#define DrawPointNoClip					MEX_H_GET_FUNC_DEFINE_GUI( DrawPointNoClip )				// (int16 x, int16 y, mex_color color)
	#define DrawLine						MEX_H_GET_FUNC_DEFINE_GUI( DrawLine )						// (int16 x1, int16 y1, int16 x2, int16 y2, mex_color color)
	#define DrawFrameTriangle				MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameTriangle )				// (int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color)
	#define DrawFillTriangle				MEX_H_GET_FUNC_DEFINE_GUI( DrawFillTriangle )				// (int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color)
	#define DrawFrameRect					MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameRect )					// (int16 x1, int16 y1, int16 x2, int16 y2, mex_color color)
	#define DrawFillRect					MEX_H_GET_FUNC_DEFINE_GUI( DrawFillRect )					// (int16 x1, int16 y1, int16 x2, int16 y2, mex_color color)
	#define DrawFrameCircle					MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameCircle )				// (int16 x, int16 y, uint16 r, mex_color color)
	#define DrawFillCircle					MEX_H_GET_FUNC_DEFINE_GUI( DrawFillCircle )					// (int16 x, int16 y, uint16 r, mex_color color)
	#define DrawFrameEllipse				MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameEllipse )				// (int16 x, int16 y, uint16 a, uint16 b, mex_color color)
	#define DrawFillEllipse					MEX_H_GET_FUNC_DEFINE_GUI( DrawFillEllipse )				// (int16 x, int16 y, uint16 a, uint16 b, mex_color color)

	#define DrawPointAlpha					MEX_H_GET_FUNC_DEFINE_GUI( DrawPointAlpha )					// (int16 x, int16 y, mex_color color, byte alpha)
	#define DrawPointNoClipAlpha			MEX_H_GET_FUNC_DEFINE_GUI( DrawPointNoClipAlpha )			// (int16 x, int16 y, mex_color color, byte alpha)
	#define DrawLineAlpha					MEX_H_GET_FUNC_DEFINE_GUI( DrawLineAlpha )					// (int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha)
	#define DrawFrameTriangleAlpha			MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameTriangleAlpha )			// (int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha)
	#define DrawFillTriangleAlpha			MEX_H_GET_FUNC_DEFINE_GUI( DrawFillTriangleAlpha )			// (int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha)
	#define DrawFrameRectAlpha				MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameRectAlpha )				// (int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha)
	#define DrawFillRectAlpha				MEX_H_GET_FUNC_DEFINE_GUI( DrawFillRectAlpha )				// (int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha)
	#define DrawFrameCircleAlpha			MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameCircleAlpha )			// (int16 x, int16 y, uint16 r, mex_color color, byte alpha)
	#define DrawFillCircleAlpha				MEX_H_GET_FUNC_DEFINE_GUI( DrawFillCircleAlpha )			// (int16 x, int16 y, uint16 r, mex_color color, byte alpha)
	#define DrawFrameEllipseAlpha			MEX_H_GET_FUNC_DEFINE_GUI( DrawFrameEllipseAlpha )			// (int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha)
	#define DrawFillEllipseAlpha			MEX_H_GET_FUNC_DEFINE_GUI( DrawFillEllipseAlpha )			// (int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha)

	#define GetImageDimension				MEX_H_GET_FUNC_DEFINE_GUI( GetImageDimension )				// (int32 *width, int32 *height, int32 *numFrames, const wchar_t *path, uint8 *memData, size_t memSize, int16 imgLoadType)
	#define GetMifImageDimensionFromFileHandle	MEX_H_GET_FUNC_DEFINE_GUI( GetMifImageDimensionFromFileHandle )	// (int32 *width, int32 *height, int32 *pFileHandle)
	#define InitImage						MEX_H_GET_FUNC_DEFINE_GUI( InitImage )						// (int32 width, int32 height, int32 numFrames)
	#define InitImageNoBuffer				MEX_H_GET_FUNC_DEFINE_GUI( InitImageNoBuffer )				// (int32 width, int32 height, int32 numFrames)
	#define InitImageShell					MEX_H_GET_FUNC_DEFINE_GUI( InitImageShell )					// (mex_color *buffer, int32 width, int32 height)
	#define CloneImageShadow				MEX_H_GET_FUNC_DEFINE_GUI( CloneImageShadow )				// (Mex_Image *dest_image, Mex_Image *src_image)
	#define CloneImageDeep					MEX_H_GET_FUNC_DEFINE_GUI( CloneImageDeep )					// (Mex_Image *dest_image, Mex_Image *src_image)
	#define FlipImage						MEX_H_GET_FUNC_DEFINE_GUI( FlipImage )						// (Mex_Image *dest_image, Mex_Image *src_image, int16 type)
	#define ReleaseImage					MEX_H_GET_FUNC_DEFINE_GUI( ReleaseImage )					// (Mex_Bitmap *image)
	#define LoadImage						MEX_H_GET_FUNC_DEFINE_GUI( LoadImage )						// (Mex_Image *image, const wchar_t *path, uint8 *memData, size_t memSize, int32 frame, int16 imgLoadType)
	#define LoadMifImageFromFileHandle		MEX_H_GET_FUNC_DEFINE_GUI( LoadMifImageFromFileHandle )		// (Mex_Image *image, int32 *pFileHandle, bool bCreateAcce)

	#define GetImageMemorySize				MEX_H_GET_FUNC_DEFINE_GUI( GetImageMemorySize )				// (Mex_Image *image)
	#define MexImageToByteArray				MEX_H_GET_FUNC_DEFINE_GUI( MexImageToByteArray )			// (byte *array, Mex_Image *image)
	#define ByteArrayToMexImage				MEX_H_GET_FUNC_DEFINE_GUI( ByteArrayToMexImage )			// (byte *array)

	#define SetImageColorkey				MEX_H_GET_FUNC_DEFINE_GUI( SetImageColorkey )				// (Mex_Image *image, mex_color colorkey, bool isTrans)	
	#define SetImageColorkeyNoAccel			MEX_H_GET_FUNC_DEFINE_GUI( SetImageColorkeyNoAccel )		// (Mex_Image *image, mex_color colorkey, bool isTrans)	
	#define SetImageAlpha					MEX_H_GET_FUNC_DEFINE_GUI( SetImageAlpha )					// (Mex_Image *image, byte alpha)
	#define SetImageDrawRegion				MEX_H_GET_FUNC_DEFINE_GUI( SetImageDrawRegion )				// (Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2)
	#define SetImageRotateCenter			MEX_H_GET_FUNC_DEFINE_GUI( SetImageRotateCenter )			// (Mex_Image *image, int16 center_x, int16 center_y)
	
	#define DrawImage						MEX_H_GET_FUNC_DEFINE_GUI( DrawImage )						// (Mex_Image *image, int16 x, int16 y)
	#define DrawImageScale					MEX_H_GET_FUNC_DEFINE_GUI( DrawImageScale )					// (Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2)
	#define DrawImageOverturn				MEX_H_GET_FUNC_DEFINE_GUI( DrawImageOverturn )				// (Mex_Image *image, int16 x, int16 y, int16 type)
	#define DrawImageOverturnScale			MEX_H_GET_FUNC_DEFINE_GUI( DrawImageOverturnScale )			// (Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 type)
	#define DrawImageRotate					MEX_H_GET_FUNC_DEFINE_GUI( DrawImageRotate )				// (Mex_Image *image, int16 center_x, int16 center_y, int16 angle)
	#define DrawImageRotateScale			MEX_H_GET_FUNC_DEFINE_GUI( DrawImageRotateScale )			// (Mex_Image *image, uint16 scale_width, uint16 scale_height, int16 center_x, int16 center_y, int16 angle)
	#define DrawImageFlip					MEX_H_GET_FUNC_DEFINE_GUI( DrawImageFlip )					// (Mex_Image *image, int16 x, int16 y, int16 type)
	#define DrawImageFlipOverturn			MEX_H_GET_FUNC_DEFINE_GUI( DrawImageFlipOverturn )			// (Mex_Image *image, int16 x, int16 y, int16 type)
	#define DrawImageFlipRotate				MEX_H_GET_FUNC_DEFINE_GUI( DrawImageFlipRotate )			// (Mex_Image *image, int16 center_x, int16 center_y, int16 angle, int16 type)
	#define DrawImageAffine					MEX_H_GET_FUNC_DEFINE_GUI( DrawImageAffine )				// (Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, int16 x4, int16 y4, int16 type)

	#define MexImageToFileHandle			MEX_H_GET_FUNC_DEFINE_GUI( MexImageToFileHandle )			// (int32 *file, Mex_Image *image)

#if MEX_SDK_VER >= 1011
	#define InitImageWithAbuffer			MEX_H_GET_FUNC_DEFINE_GUI( InitImageWithAbuffer )			// (Mex_Image *image, int32 width, int32 height)
	#define InitImageShellWithAbuffer		MEX_H_GET_FUNC_DEFINE_GUI( InitImageShellWithAbuffer )		// (Mex_Image *image, mex_color *buffer, byte *Abuffer, int32 width, int32 height)
#endif

#endif

#if defined(_MEX_COMMENTS_NEVER_DEFINE_THIS_)

/************************************************************************
 * ͼ�δ�����
 ************************************************************************/

// ����  �ܡ�  ��������
// ��˵  ����  �ú�����ָ�������л���ָ���㣬Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ����X����
// ������ 2��      [IN]  y                 ����Y����
// ������ 3��      [IN]  color             ������ɫ
// ������ֵ��      [OUT] ��
void DrawPoint(int16 x, int16 y, mex_color color);

// ����  �ܡ�  �������أ������߽磩
// ��˵  ����  �ú�����ָ�������л���ָ���㣬Ĭ��Ϊ��Ļ���档
// ��ע  �⡿  �ú��������б߽��⣬�����߱�����ȷ������øú�����Ŀ�ģ��������ֲ���Ԥ�ϴ���
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ����X����
// ������ 2��      [IN]  y                 ����Y����
// ������ 3��      [IN]  color             ������ɫ
// ������ֵ��      [OUT] ��
void DrawPointNoClip(int16 x, int16 y, mex_color color);

// ����  �ܡ�  ����ֱ��
// ��˵  ����  �ú�����ָ�������л���ָ����ɫֱ�ߣ�Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                ֱ�߶˵�1��X����
// ������ 2��      [IN]  y1                ֱ�߶˵�1��Y����
// ������ 3��      [IN]  x2                ֱ�߶˵�2��X����
// ������ 4��      [IN]  y2                ֱ�߶˵�2��Y����
// ������ 5��      [IN]  color             ֱ����ɫ
// ������ֵ��      [IN]  ��
void DrawLine(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color);

// ����  �ܡ�  ���ƿ���������
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ���������Σ�Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �����ζ���1��X����
// ������ 2��      [IN]  y1                �����ζ���1��Y����
// ������ 3��      [IN]  x2                �����ζ���2��X����
// ������ 4��      [IN]  y2                �����ζ���2��Y����
// ������ 5��      [IN]  x3                �����ζ���3��X����
// ������ 6��      [IN]  y3                �����ζ���3��Y����
// ������ 7��      [IN]  color             ֱ����ɫ
// ������ֵ��      [IN]  ��
void DrawFrameTriangle(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color);

// ����  �ܡ�  ����ʵ��������
// ��˵  ����  �ú�����ָ�������л���ָ����ɫʵ�������ͣ�Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �����ζ���1��X����
// ������ 2��      [IN]  y1                �����ζ���1��Y����
// ������ 3��      [IN]  x2                �����ζ���2��X����
// ������ 4��      [IN]  y2                �����ζ���2��Y����
// ������ 5��      [IN]  x3                �����ζ���3��X����
// ������ 6��      [IN]  y3                �����ζ���3��Y����
// ������ 7��      [IN]  color             ֱ����ɫ
// ������ֵ��      [IN]  ��
void DrawFillTriangle(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color);

// ����  �ܡ�  ���ƿ��ľ�������
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ���ľ�������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �������Ͻǵ�X����
// ������ 2��      [IN]  y1                �������Ͻǵ�Y����
// ������ 3��      [IN]  x2                �������½ǵ�X����
// ������ 4��      [IN]  y2                �������½ǵ�Y����
// ������ 5��      [IN]  color             �߿���ɫ
// ������ֵ��      [OUT] ��
void DrawFrameRect(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color);

// ����  �ܡ�  ����ʵ�ľ�������
// ��˵  ����  �ú�����ָ�������л���ָ����ɫʵ�ľ�������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �������Ͻǵ�X����
// ������ 2��      [IN]  y1                �������Ͻǵ�Y����
// ������ 3��      [IN]  x2                �������½ǵ�X����
// ������ 4��      [IN]  y2                �������½ǵ�Y����
// ������ 5��      [IN]  color             ������ɫ
// ������ֵ��      [OUT] ��
void DrawFillRect(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color);

// ����  �ܡ�  ���ƿ���Բ������
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ����Բ������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 Բ��X����
// ������ 2��      [IN]  y                 Բ��Y����
// ������ 3��      [IN]  r                 Բ�İ뾶
// ������ 4��      [IN]  color             Բ�߿���ɫ
// ������ֵ��      [OUT] ��
void DrawFrameCircle(int16 x, int16 y, uint16 r, mex_color color);

// ����  �ܡ�  ����ʵ��Բ������
// ��˵  ����  �ú�����ָ�������л���ָ����ɫʵ��Բ������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 Բ��X����
// ������ 2��      [IN]  y                 Բ��Y����
// ������ 3��      [IN]  r                 Բ�İ뾶
// ������ 4��      [IN]  color             Բ��ɫ
// ������ֵ��      [OUT] ��
void DrawFillCircle(int16 x, int16 y, uint16 r, mex_color color);

// ����  �ܡ�  ����ˮƽ������Բ����
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ������Բ����Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1 
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ��Բ����X����
// ������ 2��      [IN]  y                 ��Բ����Y����
// ������ 3��      [IN]  a                 ˮƽ����
// ������ 4��      [IN]  b                 ��ֱ����
// ������ 5��      [IN]  color             ��Բ�߿���ɫ
// ������ֵ��      [OUT] ��
void DrawFrameEllipse(int16 x, int16 y, uint16 a, uint16 b, mex_color color);

// ����  �ܡ�  ����ˮƽʵ����Բ����
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ������Բ����Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ��Բ����X����
// ������ 2��      [IN]  y                 ��Բ����Y����
// ������ 3��      [IN]  a                 ˮƽ����
// ������ 4��      [IN]  b                 ��ֱ����
// ������ 5��      [IN]  color             ��Բ��ɫ
// ������ֵ��      [OUT] ��
void DrawFillEllipse(int16 x, int16 y, uint16 a, uint16 b, mex_color color);

// ����  �ܡ�  �������أ�alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ���㣬Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ����X����
// ������ 2��      [IN]  y                 ����Y����
// ������ 3��      [IN]  color             ������ɫ
// ������ 4��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawPointAlpha(int16 x, int16 y, mex_color color, byte alpha);

// ����  �ܡ�  �������أ������߽磩��alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ���㣬Ĭ��Ϊ��Ļ���档
// ��ע  �⡿  �ú��������б߽��⣬�����߱�����ȷ������øú�����Ŀ�ģ��������ֲ���Ԥ�ϴ���
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ����X����
// ������ 2��      [IN]  y                 ����Y����
// ������ 3��      [IN]  color             ������ɫ
// ������ 4��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawPointNoClipAlpha(int16 x, int16 y, mex_color color, byte alpha);

// ����  �ܡ�  ����ֱ�ߣ�alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫֱ�ߣ�Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                ֱ�߶˵�1��X����
// ������ 2��      [IN]  y1                ֱ�߶˵�1��Y����
// ������ 3��      [IN]  x2                ֱ�߶˵�2��X����
// ������ 4��      [IN]  y2                ֱ�߶˵�2��Y����
// ������ 5��      [IN]  color             ֱ����ɫ
// ������ 6��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [IN]  ��
void DrawLineAlpha(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha);

// ����  �ܡ�  ���ƿ��������ͣ�alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ���������Σ�Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �����ζ���1��X����
// ������ 2��      [IN]  y1                �����ζ���1��Y����
// ������ 3��      [IN]  x2                �����ζ���2��X����
// ������ 4��      [IN]  y2                �����ζ���2��Y����
// ������ 5��      [IN]  x3                �����ζ���3��X����
// ������ 6��      [IN]  y3                �����ζ���3��Y����
// ������ 7��      [IN]  color             ֱ����ɫ
// ������ 8��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [IN]  ��
void DrawFrameTriangleAlpha(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha);

// ����  �ܡ�  ����ʵ�������ͣ�alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫʵ�������ͣ�Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �����ζ���1��X����
// ������ 2��      [IN]  y1                �����ζ���1��Y����
// ������ 3��      [IN]  x2                �����ζ���2��X����
// ������ 4��      [IN]  y2                �����ζ���2��Y����
// ������ 5��      [IN]  x3                �����ζ���3��X����
// ������ 6��      [IN]  y3                �����ζ���3��Y����
// ������ 7��      [IN]  color             ֱ����ɫ
// ������ 8��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [IN]  ��
void DrawFillTriangleAlpha(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha);

// ����  �ܡ�  ���ƿ��ľ�������alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ���ľ�������Ĭ��Ϊ��Ļ���档
// ��ע  �⡿  ��������������Ͻǵ㣬�����������½ǵ�
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �������Ͻǵ�X����
// ������ 2��      [IN]  y1                �������Ͻǵ�Y����
// ������ 3��      [IN]  x2                �������½ǵ�X����
// ������ 4��      [IN]  y2                �������½ǵ�Y����
// ������ 5��      [IN]  color             �߿���ɫ
// ������ 8��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawFrameRectAlpha(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha);

// ����  �ܡ�  ����ʵ�ľ�������alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫʵ�ľ�������Ĭ��Ϊ��Ļ���档
// ��ע  �⡿  �������������Ͻǵ㣬�����������½ǵ�
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x1                �������Ͻǵ�X����
// ������ 2��      [IN]  y1                �������Ͻǵ�Y����
// ������ 3��      [IN]  x2                �������½ǵ�X����
// ������ 4��      [IN]  y2                �������½ǵ�Y����
// ������ 5��      [IN]  color             ������ɫ
// ������ 6��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawFillRectAlpha(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha);

// ����  �ܡ�  ���ƿ���Բ������alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ����Բ������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 Բ��X����
// ������ 2��      [IN]  y                 Բ��Y����
// ������ 3��      [IN]  r                 Բ�İ뾶
// ������ 4��      [IN]  color             Բ�߿���ɫ
// ������ 6��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawFrameCircleAlpha(int16 x, int16 y, uint16 r, mex_color color, byte alpha);

// ����  �ܡ�  ����ʵ��Բ������alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫʵ��Բ������Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 Բ��X����
// ������ 2��      [IN]  y                 Բ��Y����
// ������ 3��      [IN]  r                 Բ�İ뾶
// ������ 4��      [IN]  color             Բ��ɫ
// ������ 5��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawFillCircleAlpha(int16 x, int16 y, uint16 r, mex_color color, byte alpha);

// ����  �ܡ�  ����ˮƽ������Բ����alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ������Բ����Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1 
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ��Բ����X����
// ������ 2��      [IN]  y                 ��Բ����Y����
// ������ 3��      [IN]  a                 ˮƽ����
// ������ 4��      [IN]  b                 ��ֱ����
// ������ 5��      [IN]  color             ��Բ�߿���ɫ
// ������ 6��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawFrameEllipseAlpha(int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha);

// ����  �ܡ�  ����ˮƽʵ����Բ����alpha��ϣ�
// ��˵  ����  �ú�����ָ�������л���ָ����ɫ������Բ����Ĭ��Ϊ��Ļ���档
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  x                 ��Բ����X����
// ������ 2��      [IN]  y                 ��Բ����Y����
// ������ 3��      [IN]  a                 ˮƽ����
// ������ 4��      [IN]  b                 ��ֱ����
// ������ 5��      [IN]  color             ��Բ��ɫ
// ������ 6��      [IN]  alpha             alpha�ȼ���0-16��
// ������ֵ��      [OUT] ��
void DrawFillEllipseAlpha(int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha);

/************************************************************************
 * ͼ������
 *
 * ͼ�������˵��
 *-----------------------------------------------------------------------
 * ͼ������                      ˵��
 * MEX_IMAGE_ATTR_NORMAL         ��׼ͼ��
 * MEX_IMAGE_ATTR_SHADOWCLONE    ǳ���¡ͼ��
 * MEX_IMAGE_ATTR_DEEPCLONE      ��ȿ�¡ͼ��
 * MEX_IMAGE_ATTR_SHELL          ͼ����ǣ����԰���ͼ�񻺴棩
 *-----------------------------------------------------------------------
 * ͼ���������                  ˵��
 * MEX_IMAGE_LOAD_FROM_ABSPATH   ����·��
 * MEX_IMAGE_LOAD_FROM_RELPATH   ���·����MEX�ļ��ڲ�·�������·����
 * MEX_IMAGE_LOAD_FROM_MEMORY    �ڴ�
 * MEX_IMAGE_LOAD_FROM_VIRMEMORY �����ڴ棨δʹ�ã�
 *-----------------------------------------------------------------------
 * ͼ��ת����                  ˵����������FlipImage��ʹ�ã�
 * MEX_IMAGE_FLIP_HORIZON        ˮƽ��ת
 * MEX_IMAGE_FLIP_VERTICAL       ��ֱ��ת
 * MEX_IMAGE_FLIP_CENTER         ���ķ�ת
 *-----------------------------------------------------------------------
 * ͼ��ת����                  ˵����������Overturn��ϵ��ʹ�ã�
 * MEX_IMAGE_OVERTURN_RIGHT      �ҷ�ת����˳ʱ��90�ȣ�
 * MEX_IMAGE_OVERTURN_BOTTOM     �׷�ת����˳ʱ��180�ȣ�
 * MEX_IMAGE_OVERTURN_LEFT       ��ת����˳ʱ��270�ȣ�
 *-----------------------------------------------------------------------
 * ͼ���������                  ˵����������Affine��ϵ��ʹ�ã�
 * MEX_IMAGE_AFFINE_HORIZON      ˮƽ����
 * MEX_IMAGE_AFFINE_VERTICAL     ��ֱ����
 ************************************************************************/

// ����  �ܡ�  ���ļ������ȡMifͼ��ߴ�
// ��˵  ����  �ú������ļ������ǰλ�û�ȡMifͼ��ߴ磬��ȡ�ߴ���ļ����ָ��λ�ò���
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT]  width          ͼ����
// ������ 2��      [OUT]  height	     ͼ��߶�
// ������ 3��      [IN]   pFileHandle	 �ļ����
// ������ֵ��      [OUT]  bool��         �Ƿ�ɹ�
bool GetMifImageDimensionFromFileHandle(int32 *width, int32 *height, int32 *pFileHandle);

// ����  �ܡ�  ����ͼ��
// ��˵  ����  �ú�������ͼ��������ͼ���ͼ��ͼ��������Ͳμ���ͼ�������˵����
//             ��ͼ����������Ǿ���·�������·�������ڴ���������ΪNULL���ڴ����ݴ�С����Ϊ0���ɡ�
//             ��ͼ������������ڴ�������ڴ棬���ļ���ַ����ΪNULL���ɡ�
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] width           �ļ����
// ������ 2��      [OUT] height          �ļ��߶�
// ������ 3��      [OUT] numFrames       ֡��
// ������ 4��      [IN]  path            �ļ���ַ
// ������ 5��      [IN]  mem_data        �ڴ�����
// ������ 6��      [IN]  mem_size        �ڴ����ݴ�С
// ������ 7��      [IN]  imgLoadType     ͼ���������
// ������ֵ��      [OUT] bool��          �����Ƿ�ɹ�
bool GetImageDimension(int32 *width, int32 *height, int32 *numFrames, const wchar_t *path, uint8 *memData, size_t memSize, int16 imgLoadType);

// ����  �ܡ�  ��ʼ��ͼ��
// ��˵  ����  �ú�����ʼ��ͼ������ʼ��ͼ�񲻳ɹ�����false
//             �ú���ֻΪͼ������ռ䣬�����ñ�Ҫ���������û�ϣ����������ͼ����ʹ�á�SetImage��ϵ�к�����
// ��ע  �⡿  �ú�����ʼ��ͼ��Ļ���Ϊ�԰������棬�ͷ�ͼ��ʱ���ͷŸû�������
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  width           �ļ����
// ������ 3��      [IN]  height          �ļ��߶�
// ������ֵ��      [OUT] bool��          �Ƿ񴴽��ɹ�
bool InitImage(Mex_Image *image, int32 width, int32 height);

// ����  �ܡ�  ��ʼ��ͼ�񣨲��������棩
// ��˵  ����  �ú�����ʼ��ͼ������ʼ��ͼ�񲻳ɹ�����false
//             �ú���ֻ���ñ�Ҫ���������û�ϣ����������ͼ����ʹ�á�SetImage��ϵ�к�����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  width           �ļ����
// ������ 3��      [IN]  height          �ļ��߶�
// ������ֵ��      [OUT] bool��          �Ƿ񴴽��ɹ�
bool InitImageNoBuffer(Mex_Image *image, int32 width, int32 height);

// ����  �ܡ�  ��ָ�������ʼ��ͼ��
// ��˵  ����  �ú���ʹ��ָ���Ļ����ʼ��ͼ��
// ��ע  �⡿  �û��治����Mexͼ���԰������棬�ͷ�ͼ��ʱ�������ͷŸû�������
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  buffer          Դbuffer
// ������ 3��      [IN]  width           ͼ����
// ������ 4��      [IN]  height          ͼ��߶�
// ������ֵ��      [OUT] bool��          �Ƿ�ɹ�
bool InitImageShell(Mex_Image *image, mex_color *buffer, int32 width, int32 height);

// ����  �ܡ�  ǳ��¡ͼ��
// ��˵  ����  �ú���ǳ��¡Դͼ��Ŀ��ͼ��
// ��ע  �⡿  ��¡ͼ�񻺴漰��������ʹ��Դͼ��Ļ��漰���������һ���Ϊ���԰������档
//             �ͷ�ͼ��ʱ��һ��Ҫ���ͷſ�¡ͼ�񣬴�Դͼ������п�¡��ȫ���ͷ���ɺ����ͷ�Դͼ��
//             ����Դͼ����ڿ�¡ͼ��ʱ�ͷ�Դͼ�񣬽�����ͼ�񻺴汻�ͷţ���δ�ͷŵĿ�¡�彫�޷�����ʹ�á�
//             ��Ϊ��¡ͼ����Դͼ����һ�ݼ��������������ۿ�¡ͼ���Դͼ��͸��ɫ�ı䣬�������¼������ı䡣
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] dest_image      Ŀ��ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  src_image       Դͼ��ṹ��ָ�룩
// ������ֵ��      [OUT] bool��          �Ƿ�ɹ�
bool CloneImageShadow(Mex_Image *dest_image, Mex_Image *src_image);

// ����  �ܡ�  ���¡ͼ��
// ��˵  ����  �ú������¡Դͼ��Ŀ��ͼ��
// ��ע  �⡿  ��ǳ��¡��ͬ�����¡ͼ����Դͼ����ȫ���룬�����¡ͼ��ʹ���Լ��Ļ�������������
//             ���¡ʧ�ܿ���������ԭ��1:�������治�ɹ���2���������������ɹ���
//             �������治�ɹ����ͼ���޷�ʹ�ã��������������ɹ���ͼ���޼�����������������ʹ��
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] dest_image      Ŀ��ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  src_image       Դͼ��ṹ��ָ�룩
// ������ֵ��      [OUT] bool��          �Ƿ�ɹ�
bool CloneImageDeep(Mex_Image *dest_image, Mex_Image *src_image);

// ����  �ܡ�  ������תͼ��
// ��˵  ����  �ú������ݷ�ת���ʹ���ָ��ͼ��ķ�תͼ��
//             ��ת���Ͳμ���ͼ�������˵����
// ��ע  �⡿  ��תʧ�ܿ���������ԭ��1:�������治�ɹ���2���������������ɹ���
//             �������治�ɹ����ͼ���޷�ʹ�ã��������������ɹ���ͼ���޼�����������������ʹ��
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  dest_image      Ŀ��ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  src_image       Դͼ��ṹ��ָ�룩
// ������ 3��      [IN]  type            ��ת����
// ������ֵ��      [OUT] bool��          �Ƿ�ɹ�
bool FlipImage(Mex_Image *dest_image, Mex_Image *src_image, int16 type);

// ����  �ܡ�  �ͷ�ͼ��
// ��˵  ����  �ú����ͷ�ͼ���ڲ�����
// ��ע  �⡿  �ú���ֻ�ͷ�ͼ��ṹ�ڲ����ݣ����û�ʹ�õ�ͼ��ṹΪָ�룬�мǵ��øú���֮���ͷŸ�ͼ��ṹָ��
//             ����ͼ������ͬ���ͷ�ͼ��ʱ�ͷŲ���Ҳ����ͬ
//             ��׼ͼ��       ��ȫ�ͷ�ͼ��������Ϣ
//             ͼ�����       ���ͷ�ͼ�񻺴���
//             ��ȿ�¡       ��ȫ�ͷ�ͼ��������Ϣ
//             ǳ���¡       ���ͷ�ͼ�񻺴�����������
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ����  ����      [OUT] image           ͼ��ṹ��ָ�룩
// ������ֵ��      [OUT] ��
void ReleaseImage(Mex_Image *image);

// ����  �ܡ�  ����ͼ��
// ��˵  ����  �ú�������ͼ��������ͼ���ͼ��ͼ��������Ͳμ���ͼ�������˵����
//             ��ͼ����������Ǿ���·�������·�������ڴ���������ΪNULL���ڴ����ݴ�С����Ϊ0���ɡ�
//             ��ͼ������������ڴ�������ڴ棬���ļ���ַ����ΪNULL���ɡ�
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  path            �ļ���ַ
// ������ 3��      [IN]  mem_data        �ڴ�����
// ������ 4��      [IN]  mem_size        �ڴ����ݴ�С
// ������ 5��      [IN]  frame           �ڼ�֡�����GIFͼƬ��
// ������ 6��      [IN]  imgLoadType     ͼ���������
// ������ֵ��      [OUT] bool��          �����Ƿ�ɹ�
bool LoadImage(Mex_Image *image, const wchar_t *path, uint8 *memData, size_t memSize, int32 frame, int16 imgLoadType);

// ����  �ܡ�  ���ļ��������Mifͼ���ļ�
// ��˵  ����  �ú������ļ������ǰλ�ü���Mifͼ������
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           mexͼ�񻺴棨ָ�룩
// ������ 2��      [IN]  pFileHandle     �ļ����
// ������ 3��      [IN]  bCreateAcce     �Ƿ�ʹ�ü�����
// ������ֵ��      [OUT] bool��          �Ƿ�ɹ�
bool LoadMifImageFromFileHandle(Mex_Image *image, int32 *pFileHandle, bool bCreateAcce);

// ����  �ܡ�  ��ȡͼ��ռ���ڴ��ܴ�С��byte�ֽڣ�
// ��˵  ����  �ú�����ȡMexImageͼ��ռ���ڴ�����ֽ���
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ����  ����      [IN]  image           mexͼ�񻺴棨ָ�룩
// ������ֵ��      [OUT] int32��         ͼ���ܴ�С
int32 GetImageMemorySize(Mex_Image *image);

// ����  �ܡ�  ת��MexImageͼ���ֽ�����
// ��˵  ����  �ú���ת��MexImageͼ�������ֽ����黺��
// ��ע  �⡿  �ú�������������Ƿ�Խ�磬�û�����ǰ��ȷ���ֽ������㹻����ͼ���ڴ泤�ȣ�
//             ��ȡͼ���ڴ泤�ȿɵ��ú�����GetImageMemorySize��
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] array           �ֽ�����
// ������ 2��      [IN]  image           mexͼ�񻺴棨ָ�룩
// ������ֵ��      ��
void MexImageToByteArray(byte *array, Mex_Image *image);

// ����  �ܡ�  ת���ֽ����鵽MexImageͼ��
// ��˵  ����  �ú���ת��ת���ֽ����黺�浽MexImageͼ��
// ��ע  �⡿  ���ֽ����黺��ת������MexImageͼ���ǳ������ͣ������ڲ��ⲻ�ܽ����κβ���
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ����  ����      [IN]  array           �ֽ�����
// ������ֵ��      [OUT] int32��         mexͼ�񻺴棨ָ�룩
Mex_Image* ByteArrayToMexImage(byte *array);

// ����  �ܡ�  ����ͼƬ͸��ɫ
// ��˵  ����  �ú�������ͼƬ͸��ɫ��ǳ���¡ͼ��ֻ�ܸ���͸�����ԣ��޷�����͸��ɫ��
// ��ע  �⡿  ��ʹ͸��ɫ��Ч���м�����ͼ��͸��������͸��ɫ��Ч
//             �ú������ڸ����ã�������͸��ɫʧ��ֻ������������ʧ�ܣ���͸��ɫ�ᱻ����
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image           ͼ��ṹ
// ������ 2��      [IN]  colorkey        ͸��ɫ
// ������ 3��      [IN]  isTrans         �Ƿ�͸��
// ������ֵ��      [OUT] bool��          �Ƿ�ɹ�
bool SetImageColorkey(Mex_Image *image, mex_color colorkey, bool isTrans);

// ����  �ܡ�  ����ͼƬ͸��ɫ����ʹ�ü�������
// ��˵  ����  �ú�������ͼƬ͸��ɫ����ʹ�ü���������ǳ���¡ͼ��ֻ�ܸ���͸�����ԣ��޷�����͸��ɫ��
// ��ע  �⡿  ��ʹ͸��ɫ��Ч���м�����ͼ��͸��������͸��ɫ��Ч
//             �ú�������͸��ɫ������������������ͼ��֮ǰ���ڼ��������ᱻ�ͷš�
//             ʹ����ʹ�øú���ʱҪ��ȷʹ��Ŀ�ģ�������ܳ��ֲ���Ԥ�ϴ���
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image           ͼ��ṹ
// ������ 2��      [IN]  colorkey        ͸��ɫ
// ������ 3��      [IN]  isTrans         �Ƿ�͸��
// ������ֵ��      [OUT] bool��          �Ƿ�ɹ�
bool SetImageColorkeyNoAccel(Mex_Image *image, mex_color colorkey, bool isTrans);

// ����  �ܡ�  ����ͼ��alphaֵ
// ��˵  ����  �ú�������ͼ��alphaֵ
// ��ע  �⡿  ��ƽ̨ʹ��16λ565��ʽRGBɫ������alpha��϶�ֻ֧��0~31��
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image             Mexͼ��ָ��
// ������ 2��      [IN]  alpha	           alphaֵ��0~31��
// ������ֵ��      [OUT] ��
void SetImageAlpha(Mex_Image *image, byte alpha);

// ����  �ܡ�  ����ͼ���������
// ��˵  ����  �ú�������ͼ���������
// ��ע  �⡿  ���û������������������ԭ�򣬼��������ϽǼ����½�
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image             ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x1	               �����������Ͻ�X����
// ������ 3��      [IN]  y1	               �����������Ͻ�Y����
// ������ 4��      [IN]  x2	               �����������½�X����
// ������ 5��      [IN]  y2	               �����������½�X����
// ������ֵ��      [OUT] ��
void SetImageDrawRegion(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2);

// ����  �ܡ�  ����ͼ����ת����
// ��˵  ����  �ú�������ͼ����ת���ģ�ͼ����ת����Ĭ��Ϊͼ��ļ������ģ�����Ϊ��0��0��
//             ��ͼ ͼ����Ӱ�ķ���Ϊʵ��ͼ��X,Y����ϵΪ��ͼ�񼸺�����Ϊԭ��ľֲ�����ϵ��Ĭ����ת����Ϊ��0��0��
//                  ���û�ϣ��ͼ��Χ�Ƶ㣨1��1��λ����ת������ʹ�øú�������ת�������óɣ�1��1��,��ͼ��Χ�ƾֲ�����ϵ�еĵ㣨1��1����ת
//                  ���û�ϣ��ͼ��Χ��ͼ����ĵ㣨5��5����ת������ʹ�øú�������ת�������óɣ�5��5������ͼ��Χ�Ƶ㣨5��5����ת
//
//                           ��Y
//                           |____________(5,5)
//                           |            |
//                           |            |
//                      -----+-----       |
//                     |/////|__(1,1)     |
//                     |/////|  |  |      |
//                 ----+-----+-----+----------> X
//                     |/////|(0,0)|
//                     |/////|/////|
//                      -----+-----
//                           | 
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [OUT] image             ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  center_x	       ��ת����X����
// ������ 3��      [IN]  center_y	       ��ת����Y����
// ������ֵ��      [OUT] ��
void SetImageRotateCenter(Mex_Image *image, int16 center_x, int16 center_y);

// ����  �ܡ�  ����ͼ��
// ��˵  ����  �ú�����ָ��������ָ��λ�û���ָ��ͼ��Ĭ��Ϊ��Ļ���档�ú������Զ��ж�ͼ������Խ��л���
// ����  �ԡ�  �Ѳ���  T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x               x����
// ������ 3��      [IN]  y               y����
// ������ֵ��      [OUT] ��
void DrawImage(Mex_Image *image, int16 x, int16 y);

// ����  �ܡ�  ����ͼ��
// ��˵  ����  �ú�����ָ��������ָ��λ�û������ź�ͼ��Ĭ��Ϊ��Ļ���档�ú������Զ��ж�ͼ������Խ��л���
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x1              ָ�������������Ͻ�x����
// ������ 3��      [IN]  y1              ָ�������������Ͻ�y����
// ������ 4��      [IN]  x2              ָ�������������½�x����
// ������ 5��      [IN]  y2              ָ�������������½�y����
// ������ֵ��      [OUT] ��
void DrawImageScale(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2);

// ����  �ܡ�  ��תͼ��
// ��˵  ����  �ú�����ָ��������ָ��λ�û��Ʒ�ת��ͼ��Ĭ��Ϊ��Ļ���档�ú������Զ��ж�ͼ������Խ��л���
//             ��ת���Ͳμ���ͼ�������˵����
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x               ָ�������������Ͻ�x����
// ������ 3��      [IN]  y               ָ�������������Ͻ�y����
// ������ 4��      [IN]  type            ��ת����
// ������ֵ��      [OUT] ��
void DrawImageOverturn(Mex_Image *image, int16 x, int16 y, int16 type);

// ����  �ܡ�  ��תͼ��
// ��˵  ����  �ú�����ָ��������ָ��λ�����ŷ�ת��ͼ��Ĭ��Ϊ��Ļ���档�ú������Զ��ж�ͼ������Խ��л���
//             ��ת���Ͳμ���ͼ�������˵����
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x1              ָ�������������Ͻ�x����
// ������ 3��      [IN]  y1              ָ�������������Ͻ�y����
// ������ 4��      [IN]  x2              ָ�������������½�x����
// ������ 5��      [IN]  y2              ָ�������������½�y����
// ������ 6��      [IN]  type            ��ת����
// ������ֵ��      [OUT] ��
void DrawImageOverturnScale(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 type);

// ����  �ܡ�  ������תͼ��
// ��˵  ����  �ú���������תͼ�����Ƕ�Ϊ��ʱ�룬���Ƕ�Ϊ˳ʱ�롣
//             ����center_x, center_yΪ����ת��ͼ�����ת��������Ļ�е�λ��
//             �� ��������center_x��center_y��Ϊ��100��100������ͼ�����ת�����ھֲ�����ϵ�ģ�30��30��,��ʵ�ʻ���ʱ��
//                ͼ�񽫰Ѿֲ�����ϵ�е���ת���ģ�30��30��ӳ�����Ļ�ϣ�100��100���ĵ㣬����Χ�Ƹõ���ת
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ָ��
// ������ 2��      [IN]  center_x        ͼ����ת��������Ļ����ϵ��X����
// ������ 3��      [IN]  center_y        ͼ����ת��������Ļ����ϵ��Y����
// ������ 4��      [IN]  angle           ��ת�Ƕȣ���λ���Ƕȣ�
// ������ֵ��      [OUT] ��
void DrawImageRotate(Mex_Image *image, int16 center_x, int16 center_y, int16 angle);

// ����  �ܡ�  ����������תͼ��
// ��˵  ����  �ú������Ż�����תͼ�񣬼�������תʱ����ʵ��ͼ����ָ����ȼ��߶Ƚ������š����Ƕ�Ϊ��ʱ�룬���Ƕ�Ϊ˳ʱ�롣
//             ����center_x, center_yΪ����ת��ͼ�����ת��������Ļ�е�λ��
//             �� ��������center_x��center_y��Ϊ��100��100������ͼ�����ת�����ھֲ�����ϵ�ģ�30��30��,��ʵ�ʻ���ʱ��
//                ͼ�񽫰Ѿֲ�����ϵ�е���ת���ģ�30��30��ӳ�����Ļ�ϣ�100��100���ĵ㣬����Χ�Ƹõ���ת
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ָ��
// ������ 2��      [IN]  scale_width     ͼ�����ź���
// ������ 3��      [IN]  scale_height    ͼ�����ź�߶�
// ������ 4��      [IN]  center_x        ͼ����ת��������Ļ����ϵ��X����
// ������ 5��      [IN]  center_y        ͼ����ת��������Ļ����ϵ��Y����
// ������ 6��      [IN]  angle           ��ת�Ƕȣ���λ���Ƕȣ�
// ������ֵ��      [OUT] ��
void DrawImageRotateScale(Mex_Image *image, uint16 scale_width, uint16 scale_height, int16 center_x, int16 center_y, int16 angle);

// ����  �ܡ�  ��תͼ��
// ��˵  ����  �ú�����ָ��������ָ��λ�û��Ʒ�ת��ͼ��Ĭ��Ϊ��Ļ���档�ú������Զ��ж�ͼ������Խ��л���
//             ��ת���Ͳμ���ͼ�������˵����
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x1              ָ�������������Ͻ�x����
// ������ 3��      [IN]  y1              ָ�������������Ͻ�y����
// ������ 4��      [IN]  type            ��ת����
// ������ֵ��      [OUT] ��
void DrawImageFlip(Mex_Image *image, int16 x, int16 y, int16 type);

// ����  �ܡ�  ��ת����תͼ��
// ��˵  ����  �ú�����ָ��������ָ��λ�û��Ʒ�ת����ת��ͼ��Ĭ��Ϊ��Ļ���档�ú������Զ��ж�ͼ������Խ��л���
//             ��ת���ͼ���ת���Ϳɽ��С�|���������������
//             ��ת���ͼ���ת���Ͳμ���ͼ�������˵����
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x1              ָ�������������Ͻ�x����
// ������ 3��      [IN]  y1              ָ�������������Ͻ�y����
// ������ 4��      [IN]  type            ��ת���ͼ���ת����
// ������ֵ��      [OUT] ��
void DrawImageFlipOverturn(Mex_Image *image, int16 x, int16 y, int16 type);

// ����  �ܡ�  ��ת��תͼ��
// ��˵  ����  �ú�����ת��ת��ͼ�����Ƕ�Ϊ��ʱ�룬���Ƕ�Ϊ˳ʱ�롣
//             ����center_x, center_yΪ����ת��ͼ�����ת��������Ļ�е�λ��
//             �� ��������center_x��center_y��Ϊ��100��100������ͼ�����ת�����ھֲ�����ϵ�ģ�30��30��,��ʵ�ʻ���ʱ��
//                ͼ�񽫰Ѿֲ�����ϵ�е���ת���ģ�30��30��ӳ�����Ļ�ϣ�100��100���ĵ㣬����Χ�Ƹõ���ת
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ָ��
// ������ 2��      [IN]  center_x        ͼ����ת��������Ļ����ϵ��X����
// ������ 3��      [IN]  center_y        ͼ����ת��������Ļ����ϵ��Y����
// ������ 4��      [IN]  angle           ��ת�Ƕȣ���λ���Ƕȣ�
// ������ 5��      [IN]  type            ��ת����
// ������ֵ��      [OUT] ��
void DrawImageFlipRotate(Mex_Image *image, int16 center_x, int16 center_y, int16 angle, int16 type);

// ����  �ܡ�  ����ͼ��
// ��˵  ����  �ú�����ָ��������ָ��λ�÷���ͼ��Ĭ��Ϊ��Ļ���档�ú������Զ��ж�ͼ������Խ��л���
//			   �������Ͳμ���ͼ�������˵����
//             ��ʹ��ˮƽ���䣬��ֻ��x1, y1, x2, x3, y3, x4����������
//             ��ʹ����ֱ���䣬��ֻ��x1, y1, y2, x3, y3, y4����������
// ��ע  �⡿  �ú���������ĸ���Ϊ�����ͼ�α߽����꣬������������ԭ�򣬼����������ĸ���
// ����  �ԡ�  �Ѳ��� T1
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN]  image           ͼ��ṹ��ָ�룩
// ������ 2��      [IN]  x1              ָ�������������Ͻ�x����
// ������ 3��      [IN]  y1              ָ�������������Ͻ�y����
// ������ 4��      [IN]  x2              ָ�������������Ͻ�x����
// ������ 5��      [IN]  y2              ָ�������������Ͻ�y����
// ������ 6��      [IN]  x3              ָ�������������½�x����
// ������ 7��      [IN]  y3              ָ�������������½�y����
// ������ 8��      [IN]  x4              ָ�������������½�x����
// ������ 9��      [IN]  y4              ָ�������������½�y����          
// ������10��      [IN]  type            ��������
// ������ֵ��      [OUT] ��
void DrawImageAffine(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, int16 x4, int16 y4, int16 type);

// ����  �ܡ�  ת��MexImageͼ���ļ�
// ��˵  ����  �ú���ת��MexImageͼ���ļ��������ֽ�����
// ---------------------------------------------------------------------------------------------------------------------
// ������ 1��      [IN/OUT] file         �ļ����
// ������ 2��      [IN]  image           mexͼ�񻺴棨ָ�룩
// ������ֵ��      ��
void MexImageToFileHandle(int32 *file, Mex_Image *image);

#endif

#endif
