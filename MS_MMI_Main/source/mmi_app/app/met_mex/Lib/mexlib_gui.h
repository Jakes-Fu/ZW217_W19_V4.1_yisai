/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexLib_Gui.h
* 文件标识：
* 内容摘要： //mex平台gui库
*
************************************************************************/

/************************************************************************
 *
 *  MEX GUI LIB
 *  MEX平台 字符串/图形/图像 函数库
 *  version 1.3
 *  2010.4.1    by met guo_tf
 *
 *  说明：
 *      ① 使用本库函数前，需调用【GUI_BEGIN】宏指定目标缓存，函数后，需
 *  调用【GUI_END】宏结束绘制。
 *      【GUI_BEGIN】【GUI_END】必须成对出现。否则将出现编译错误。
 *      使用本库函数时若未调用【GUI_BEGIN】及【GUI_END】则默认屏幕缓存为
 *  目标缓存。
 *      ② 本库严格遵守左上填充原则，及绘制函数绘制区域包含左上角点，不包
 *  含右下角点。若有未遵从左上填充原则的函数将会在函数注释的注意项中说明，
 *  用户使用时需注意。
 *      ③ 本库提供了两套图形绘制函数，结尾以【Internal】结尾的函数为使用
 *  底层的图形绘制函数，否则为本库提供的图形绘制函数。
 *      ④ 图像方面本库推荐使用MIF格式图片，其他格式图片因使用底层解码函数，
 *  故可能出现内存不足导致图像加载不正确的情况。
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
#define MEX_COLOR_WHITE                         0xFFFF    // 纯白色
#define MEX_COLOR_BLACK                         0x0000    // 纯黑色
#define MEX_COLOR_RED                           0xF800    // 纯红色
#define MEX_COLOR_GREEN                         0x07E0    // 纯绿色
#define MEX_COLOR_BLUE                          0x001F    // 纯蓝色
#define MEX_COLOR_YELLOW                        0xFFE0    // 纯黄色
#define MEX_COLOR_CYAN                          0x07FF    // 淡青色
#define MEX_COLOR_MAGENTA                       0xF81F    // 洋红色

#define MEX_COLOR_LIGHTPINK                     0xFDB8    // 浅粉色
#define MEX_COLOR_PINK                          0xFE19    // 粉色
#define MEX_COLOR_DEEPPINK                      0xF8B2    // 深粉色
#define MEX_COLOR_DARKMAGENTA                   0x8811    // 深洋红
#define MEX_COLOR_PURPLE                        0x8010    // 紫色
#define MEX_COLOR_INDIGO                        0x4810    // 靛青色
#define MEX_COLOR_MEDIUMPURPLE                  0x939B    // 适中的紫色
#define MEX_COLOR_MEDIUMBLUE                    0x0019    // 适中的蓝色
#define MEX_COLOR_MIDNIGHTBLUE                  0x18CE    // 午夜蓝
#define MEX_COLOR_DARKBLUE                      0x0011    // 深蓝色
#define MEX_COLOR_NAVYBLUE                      0x0010    // 海军蓝
#define MEX_COLOR_LIGHTSKYBLUE                  0x867F    // 淡天蓝色
#define MEX_COLOR_SKYBLUE                       0x867D    // 天蓝色
#define MEX_COLOR_DARKSKYBLUE                   0x05FF    // 深天蓝色
#define MEX_COLOR_LIGHTBLUE                     0xAEDC    // 浅蓝色
#define MEX_COLOR_LIGHTCYAN                     0xE7FF    // 淡青色
#define MEX_COLOR_DARKCYAN                      0x0451    // 深青色
#define MEX_COLOR_LIGHTGREEN                    0x9772    // 浅绿色
#define MEX_COLOR_FORESTGREEN                   0x2444    // 森林绿
#define MEX_COLOR_DARKGREEN                     0x0320    // 深绿色
#define MEX_COLOR_YELLOWGREEN                   0xAFE5    // 黄绿色
#define MEX_COLOR_LIGHTYELLOW                   0xFFFC    // 浅黄色
#define MEX_COLOR_OLIVE                         0x8400    // 橄榄色
#define MEX_COLOR_GOLD                          0xFEA0    // 金色
#define MEX_COLOR_ORANGE                        0xFD20    // 橙色
#define MEX_COLOR_DARKORANGE                    0xFC60    // 深橙色
#define MEX_COLOR_PEACHPUFF                     0xFED7    // 桃色
#define MEX_COLOR_CHOCOLATE                     0xD343    // 巧克力色
#define MEX_COLOR_ORANGERED                     0xFA20    // 橙红色
#define MEX_COLOR_INDIANRED                     0xCAEB    // 印度红
#define MEX_COLOR_BROWN                         0xA145    // 棕色
#define MEX_COLOR_DARKRED                       0x8800    // 深红色
#define MEX_COLOR_LIGHTGRAY                     0xD69A    // 浅灰色
#define MEX_COLOR_SILVER                        0xC618    // 银色
#define MEX_COLOR_DARKGRAY                      0xAD55    // 深灰色
#define MEX_COLOR_GRAY                          0x8410    // 灰色

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
 * 图形处理函数
 ************************************************************************/

// 【功  能】  绘制像素
// 【说  明】  该函数在指定缓存中绘制指定点，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 像素X坐标
// 【参数 2】      [IN]  y                 像素Y坐标
// 【参数 3】      [IN]  color             像素颜色
// 【返回值】      [OUT] 无
void DrawPoint(int16 x, int16 y, mex_color color);

// 【功  能】  绘制像素（不检测边界）
// 【说  明】  该函数在指定缓存中绘制指定点，默认为屏幕缓存。
// 【注  意】  该函数不进行边界检测，调用者必须明确清楚调用该函数的目的，否则会出现不可预料错误
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 像素X坐标
// 【参数 2】      [IN]  y                 像素Y坐标
// 【参数 3】      [IN]  color             像素颜色
// 【返回值】      [OUT] 无
void DrawPointNoClip(int16 x, int16 y, mex_color color);

// 【功  能】  绘制直线
// 【说  明】  该函数在指定缓存中绘制指定颜色直线，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                直线端点1的X坐标
// 【参数 2】      [IN]  y1                直线端点1的Y坐标
// 【参数 3】      [IN]  x2                直线端点2的X坐标
// 【参数 4】      [IN]  y2                直线端点2的Y坐标
// 【参数 5】      [IN]  color             直线颜色
// 【返回值】      [IN]  无
void DrawLine(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color);

// 【功  能】  绘制空心三角型
// 【说  明】  该函数在指定缓存中绘制指定颜色空心三角形，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                三角形顶点1的X坐标
// 【参数 2】      [IN]  y1                三角形顶点1的Y坐标
// 【参数 3】      [IN]  x2                三角形顶点2的X坐标
// 【参数 4】      [IN]  y2                三角形顶点2的Y坐标
// 【参数 5】      [IN]  x3                三角形顶点3的X坐标
// 【参数 6】      [IN]  y3                三角形顶点3的Y坐标
// 【参数 7】      [IN]  color             直线颜色
// 【返回值】      [IN]  无
void DrawFrameTriangle(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color);

// 【功  能】  绘制实心三角型
// 【说  明】  该函数在指定缓存中绘制指定颜色实心三角型，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                三角形顶点1的X坐标
// 【参数 2】      [IN]  y1                三角形顶点1的Y坐标
// 【参数 3】      [IN]  x2                三角形顶点2的X坐标
// 【参数 4】      [IN]  y2                三角形顶点2的Y坐标
// 【参数 5】      [IN]  x3                三角形顶点3的X坐标
// 【参数 6】      [IN]  y3                三角形顶点3的Y坐标
// 【参数 7】      [IN]  color             直线颜色
// 【返回值】      [IN]  无
void DrawFillTriangle(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color);

// 【功  能】  绘制空心矩形区域
// 【说  明】  该函数在指定缓存中绘制指定颜色空心矩形区域，默认为屏幕缓存。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                区域左上角的X坐标
// 【参数 2】      [IN]  y1                区域左上角的Y坐标
// 【参数 3】      [IN]  x2                区域右下角的X坐标
// 【参数 4】      [IN]  y2                区域右下角的Y坐标
// 【参数 5】      [IN]  color             边框颜色
// 【返回值】      [OUT] 无
void DrawFrameRect(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color);

// 【功  能】  绘制实心矩形区域
// 【说  明】  该函数在指定缓存中绘制指定颜色实心矩形区域，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                区域左上角的X坐标
// 【参数 2】      [IN]  y1                区域左上角的Y坐标
// 【参数 3】      [IN]  x2                区域右下角的X坐标
// 【参数 4】      [IN]  y2                区域右下角的Y坐标
// 【参数 5】      [IN]  color             矩形颜色
// 【返回值】      [OUT] 无
void DrawFillRect(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color);

// 【功  能】  绘制空心圆形区域
// 【说  明】  该函数在指定缓存中绘制指定颜色空心圆形区域，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 圆心X坐标
// 【参数 2】      [IN]  y                 圆心Y坐标
// 【参数 3】      [IN]  r                 圆的半径
// 【参数 4】      [IN]  color             圆边框颜色
// 【返回值】      [OUT] 无
void DrawFrameCircle(int16 x, int16 y, uint16 r, mex_color color);

// 【功  能】  绘制实心圆形区域
// 【说  明】  该函数在指定缓存中绘制指定颜色实心圆形区域，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 圆心X坐标
// 【参数 2】      [IN]  y                 圆心Y坐标
// 【参数 3】      [IN]  r                 圆的半径
// 【参数 4】      [IN]  color             圆颜色
// 【返回值】      [OUT] 无
void DrawFillCircle(int16 x, int16 y, uint16 r, mex_color color);

// 【功  能】  绘制水平空心椭圆区域
// 【说  明】  该函数在指定缓存中绘制指定颜色空心椭圆区域，默认为屏幕缓存。
// 【测  试】  已测试 T1 
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 椭圆中心X坐标
// 【参数 2】      [IN]  y                 椭圆中心Y坐标
// 【参数 3】      [IN]  a                 水平半轴
// 【参数 4】      [IN]  b                 垂直半轴
// 【参数 5】      [IN]  color             椭圆边框颜色
// 【返回值】      [OUT] 无
void DrawFrameEllipse(int16 x, int16 y, uint16 a, uint16 b, mex_color color);

// 【功  能】  绘制水平实心椭圆区域
// 【说  明】  该函数在指定缓存中绘制指定颜色空心椭圆区域，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 椭圆中心X坐标
// 【参数 2】      [IN]  y                 椭圆中心Y坐标
// 【参数 3】      [IN]  a                 水平半轴
// 【参数 4】      [IN]  b                 垂直半轴
// 【参数 5】      [IN]  color             椭圆颜色
// 【返回值】      [OUT] 无
void DrawFillEllipse(int16 x, int16 y, uint16 a, uint16 b, mex_color color);

// 【功  能】  绘制像素（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定点，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 像素X坐标
// 【参数 2】      [IN]  y                 像素Y坐标
// 【参数 3】      [IN]  color             像素颜色
// 【参数 4】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawPointAlpha(int16 x, int16 y, mex_color color, byte alpha);

// 【功  能】  绘制像素（不检测边界）（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定点，默认为屏幕缓存。
// 【注  意】  该函数不进行边界检测，调用者必须明确清楚调用该函数的目的，否则会出现不可预料错误
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 像素X坐标
// 【参数 2】      [IN]  y                 像素Y坐标
// 【参数 3】      [IN]  color             像素颜色
// 【参数 4】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawPointNoClipAlpha(int16 x, int16 y, mex_color color, byte alpha);

// 【功  能】  绘制直线（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色直线，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                直线端点1的X坐标
// 【参数 2】      [IN]  y1                直线端点1的Y坐标
// 【参数 3】      [IN]  x2                直线端点2的X坐标
// 【参数 4】      [IN]  y2                直线端点2的Y坐标
// 【参数 5】      [IN]  color             直线颜色
// 【参数 6】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [IN]  无
void DrawLineAlpha(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha);

// 【功  能】  绘制空心三角型（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色空心三角形，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                三角形顶点1的X坐标
// 【参数 2】      [IN]  y1                三角形顶点1的Y坐标
// 【参数 3】      [IN]  x2                三角形顶点2的X坐标
// 【参数 4】      [IN]  y2                三角形顶点2的Y坐标
// 【参数 5】      [IN]  x3                三角形顶点3的X坐标
// 【参数 6】      [IN]  y3                三角形顶点3的Y坐标
// 【参数 7】      [IN]  color             直线颜色
// 【参数 8】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [IN]  无
void DrawFrameTriangleAlpha(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha);

// 【功  能】  绘制实心三角型（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色实心三角型，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                三角形顶点1的X坐标
// 【参数 2】      [IN]  y1                三角形顶点1的Y坐标
// 【参数 3】      [IN]  x2                三角形顶点2的X坐标
// 【参数 4】      [IN]  y2                三角形顶点2的Y坐标
// 【参数 5】      [IN]  x3                三角形顶点3的X坐标
// 【参数 6】      [IN]  y3                三角形顶点3的Y坐标
// 【参数 7】      [IN]  color             直线颜色
// 【参数 8】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [IN]  无
void DrawFillTriangleAlpha(int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, mex_color color, byte alpha);

// 【功  能】  绘制空心矩形区域（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色空心矩形区域，默认为屏幕缓存。
// 【注  意】  绘制区域包括左上角点，但不包括右下角点
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                区域左上角的X坐标
// 【参数 2】      [IN]  y1                区域左上角的Y坐标
// 【参数 3】      [IN]  x2                区域右下角的X坐标
// 【参数 4】      [IN]  y2                区域右下角的Y坐标
// 【参数 5】      [IN]  color             边框颜色
// 【参数 8】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawFrameRectAlpha(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha);

// 【功  能】  绘制实心矩形区域（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色实心矩形区域，默认为屏幕缓存。
// 【注  意】  填充区域包括左上角点，但不包括右下角点
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1                区域左上角的X坐标
// 【参数 2】      [IN]  y1                区域左上角的Y坐标
// 【参数 3】      [IN]  x2                区域右下角的X坐标
// 【参数 4】      [IN]  y2                区域右下角的Y坐标
// 【参数 5】      [IN]  color             矩形颜色
// 【参数 6】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawFillRectAlpha(int16 x1, int16 y1, int16 x2, int16 y2, mex_color color, byte alpha);

// 【功  能】  绘制空心圆形区域（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色空心圆形区域，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 圆心X坐标
// 【参数 2】      [IN]  y                 圆心Y坐标
// 【参数 3】      [IN]  r                 圆的半径
// 【参数 4】      [IN]  color             圆边框颜色
// 【参数 6】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawFrameCircleAlpha(int16 x, int16 y, uint16 r, mex_color color, byte alpha);

// 【功  能】  绘制实心圆形区域（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色实心圆形区域，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 圆心X坐标
// 【参数 2】      [IN]  y                 圆心Y坐标
// 【参数 3】      [IN]  r                 圆的半径
// 【参数 4】      [IN]  color             圆颜色
// 【参数 5】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawFillCircleAlpha(int16 x, int16 y, uint16 r, mex_color color, byte alpha);

// 【功  能】  绘制水平空心椭圆区域（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色空心椭圆区域，默认为屏幕缓存。
// 【测  试】  已测试 T1 
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 椭圆中心X坐标
// 【参数 2】      [IN]  y                 椭圆中心Y坐标
// 【参数 3】      [IN]  a                 水平半轴
// 【参数 4】      [IN]  b                 垂直半轴
// 【参数 5】      [IN]  color             椭圆边框颜色
// 【参数 6】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawFrameEllipseAlpha(int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha);

// 【功  能】  绘制水平实心椭圆区域（alpha混合）
// 【说  明】  该函数在指定缓存中绘制指定颜色空心椭圆区域，默认为屏幕缓存。
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x                 椭圆中心X坐标
// 【参数 2】      [IN]  y                 椭圆中心Y坐标
// 【参数 3】      [IN]  a                 水平半轴
// 【参数 4】      [IN]  b                 垂直半轴
// 【参数 5】      [IN]  color             椭圆颜色
// 【参数 6】      [IN]  alpha             alpha等级（0-16）
// 【返回值】      [OUT] 无
void DrawFillEllipseAlpha(int16 x, int16 y, uint16 a, uint16 b, mex_color color, byte alpha);

/************************************************************************
 * 图像处理函数
 *
 * 图像各属性说明
 *-----------------------------------------------------------------------
 * 图像属性                      说明
 * MEX_IMAGE_ATTR_NORMAL         标准图像
 * MEX_IMAGE_ATTR_SHADOWCLONE    浅表克隆图像
 * MEX_IMAGE_ATTR_DEEPCLONE      深度克隆图像
 * MEX_IMAGE_ATTR_SHELL          图像外壳（非自包含图像缓存）
 *-----------------------------------------------------------------------
 * 图像加载类型                  说明
 * MEX_IMAGE_LOAD_FROM_ABSPATH   绝对路径
 * MEX_IMAGE_LOAD_FROM_RELPATH   相对路径（MEX文件内部路径或相对路径）
 * MEX_IMAGE_LOAD_FROM_MEMORY    内存
 * MEX_IMAGE_LOAD_FROM_VIRMEMORY 虚拟内存（未使用）
 *-----------------------------------------------------------------------
 * 图像反转类型                  说明（函数【FlipImage】使用）
 * MEX_IMAGE_FLIP_HORIZON        水平反转
 * MEX_IMAGE_FLIP_VERTICAL       竖直反转
 * MEX_IMAGE_FLIP_CENTER         中心反转
 *-----------------------------------------------------------------------
 * 图像翻转类型                  说明（函数【Overturn】系列使用）
 * MEX_IMAGE_OVERTURN_RIGHT      右翻转（即顺时针90度）
 * MEX_IMAGE_OVERTURN_BOTTOM     底翻转（即顺时针180度）
 * MEX_IMAGE_OVERTURN_LEFT       左翻转（即顺时针270度）
 *-----------------------------------------------------------------------
 * 图像仿射类型                  说明（函数【Affine】系列使用）
 * MEX_IMAGE_AFFINE_HORIZON      水平仿射
 * MEX_IMAGE_AFFINE_VERTICAL     竖直仿射
 ************************************************************************/

// 【功  能】  从文件句柄获取Mif图像尺寸
// 【说  明】  该函数从文件句柄当前位置获取Mif图像尺寸，获取尺寸后文件句柄指针位置不变
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT]  width          图像宽度
// 【参数 2】      [OUT]  height	     图像高度
// 【参数 3】      [IN]   pFileHandle	 文件句柄
// 【返回值】      [OUT]  bool型         是否成功
bool GetMifImageDimensionFromFileHandle(int32 *width, int32 *height, int32 *pFileHandle);

// 【功  能】  加载图像
// 【说  明】  该函数根据图像加载类型加载图像。图像加载类型参见【图像各属性说明】
//             若图像加载类型是绝对路径或相对路径，则内存数据设置为NULL，内存数据大小设置为0即可。
//             若图像加载类型是内存或虚拟内存，则文件地址设置为NULL即可。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] width           文件宽度
// 【参数 2】      [OUT] height          文件高度
// 【参数 3】      [OUT] numFrames       帧数
// 【参数 4】      [IN]  path            文件地址
// 【参数 5】      [IN]  mem_data        内存数据
// 【参数 6】      [IN]  mem_size        内存数据大小
// 【参数 7】      [IN]  imgLoadType     图像加载类型
// 【返回值】      [OUT] bool型          加载是否成功
bool GetImageDimension(int32 *width, int32 *height, int32 *numFrames, const wchar_t *path, uint8 *memData, size_t memSize, int16 imgLoadType);

// 【功  能】  初始化图像
// 【说  明】  该函数初始化图像，若初始化图像不成功返回false
//             该函数只为图像申请空间，并设置必要参数。若用户希望具体设置图像，请使用【SetImage】系列函数。
// 【注  意】  该函数初始化图像的缓存为自包含缓存，释放图像时会释放该缓存区。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image           图像结构（指针）
// 【参数 2】      [IN]  width           文件宽度
// 【参数 3】      [IN]  height          文件高度
// 【返回值】      [OUT] bool型          是否创建成功
bool InitImage(Mex_Image *image, int32 width, int32 height);

// 【功  能】  初始化图像（不创建缓存）
// 【说  明】  该函数初始化图像，若初始化图像不成功返回false
//             该函数只设置必要参数。若用户希望具体设置图像，请使用【SetImage】系列函数。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image           图像结构（指针）
// 【参数 2】      [IN]  width           文件宽度
// 【参数 3】      [IN]  height          文件高度
// 【返回值】      [OUT] bool型          是否创建成功
bool InitImageNoBuffer(Mex_Image *image, int32 width, int32 height);

// 【功  能】  用指定缓存初始化图像
// 【说  明】  该函数使用指定的缓存初始化图像
// 【注  意】  该缓存不属于Mex图像自包含缓存，释放图像时并不会释放该缓存区。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image           图像结构（指针）
// 【参数 2】      [IN]  buffer          源buffer
// 【参数 3】      [IN]  width           图像宽度
// 【参数 4】      [IN]  height          图像高度
// 【返回值】      [OUT] bool型          是否成功
bool InitImageShell(Mex_Image *image, mex_color *buffer, int32 width, int32 height);

// 【功  能】  浅克隆图像
// 【说  明】  该函数浅克隆源图像到目标图像
// 【注  意】  克隆图像缓存及加速器均使用源图像的缓存及加速器，且缓存为非自包含缓存。
//             释放图像时，一定要先释放克隆图像，待源图像的所有克隆体全部释放完成后再释放源图像。
//             若在源图像存在克隆图像时释放源图像，将导致图像缓存被释放，还未释放的克隆体将无法正常使用。
//             因为克隆图像与源图像公用一份加速器，所以无论克隆图像或源图像透明色改变，都将导致加速器改变。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] dest_image      目标图像结构（指针）
// 【参数 2】      [IN]  src_image       源图像结构（指针）
// 【返回值】      [OUT] bool型          是否成功
bool CloneImageShadow(Mex_Image *dest_image, Mex_Image *src_image);

// 【功  能】  深克隆图像
// 【说  明】  该函数深克隆源图像到目标图像
// 【注  意】  与浅克隆不同，深克隆图像将与源图像完全分离，即深克隆图像将使用自己的缓存区及加速器
//             深克隆失败可能有两种原因，1:创建缓存不成功，2：创建加速器不成功。
//             创建缓存不成功则该图像无法使用，创建加速器不成功仅图像无加速器，但可以正常使用
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] dest_image      目标图像结构（指针）
// 【参数 2】      [IN]  src_image       源图像结构（指针）
// 【返回值】      [OUT] bool型          是否成功
bool CloneImageDeep(Mex_Image *dest_image, Mex_Image *src_image);

// 【功  能】  创建反转图像
// 【说  明】  该函数根据反转类型创建指定图像的反转图像
//             反转类型参见【图像各属性说明】
// 【注  意】  反转失败可能有两种原因，1:创建缓存不成功，2：创建加速器不成功。
//             创建缓存不成功则该图像无法使用，创建加速器不成功仅图像无加速器，但可以正常使用
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  dest_image      目标图像结构（指针）
// 【参数 2】      [IN]  src_image       源图像结构（指针）
// 【参数 3】      [IN]  type            反转类型
// 【返回值】      [OUT] bool型          是否成功
bool FlipImage(Mex_Image *dest_image, Mex_Image *src_image, int16 type);

// 【功  能】  释放图像
// 【说  明】  该函数释放图像内部数据
// 【注  意】  该函数只释放图像结构内部数据，若用户使用的图像结构为指针，切记调用该函数之后释放该图像结构指针
//             根据图像属性同，释放图像时释放操作也将不同
//             标准图像       完全释放图像所有信息
//             图像外壳       不释放图像缓存区
//             深度克隆       完全释放图像所有信息
//             浅表克隆       不释放图像缓存区及加速器
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参  数】      [OUT] image           图像结构（指针）
// 【返回值】      [OUT] 无
void ReleaseImage(Mex_Image *image);

// 【功  能】  加载图像
// 【说  明】  该函数根据图像加载类型加载图像。图像加载类型参见【图像各属性说明】
//             若图像加载类型是绝对路径或相对路径，则内存数据设置为NULL，内存数据大小设置为0即可。
//             若图像加载类型是内存或虚拟内存，则文件地址设置为NULL即可。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image           图像结构（指针）
// 【参数 2】      [IN]  path            文件地址
// 【参数 3】      [IN]  mem_data        内存数据
// 【参数 4】      [IN]  mem_size        内存数据大小
// 【参数 5】      [IN]  frame           第几帧（针对GIF图片）
// 【参数 6】      [IN]  imgLoadType     图像加载类型
// 【返回值】      [OUT] bool型          加载是否成功
bool LoadImage(Mex_Image *image, const wchar_t *path, uint8 *memData, size_t memSize, int32 frame, int16 imgLoadType);

// 【功  能】  从文件句柄加载Mif图像文件
// 【说  明】  该函数从文件句柄当前位置加载Mif图像数据
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           mex图像缓存（指针）
// 【参数 2】      [IN]  pFileHandle     文件句柄
// 【参数 3】      [IN]  bCreateAcce     是否使用加速器
// 【返回值】      [OUT] bool型          是否成功
bool LoadMifImageFromFileHandle(Mex_Image *image, int32 *pFileHandle, bool bCreateAcce);

// 【功  能】  获取图像占据内存总大小（byte字节）
// 【说  明】  该函数获取MexImage图像占据内存的总字节数
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参  数】      [IN]  image           mex图像缓存（指针）
// 【返回值】      [OUT] int32型         图像总大小
int32 GetImageMemorySize(Mex_Image *image);

// 【功  能】  转换MexImage图像到字节数组
// 【说  明】  该函数转换MexImage图像到连续字节数组缓存
// 【注  意】  该函数不检测数组是否越界，用户调用前须确保字节数组足够容纳图像内存长度，
//             获取图像内存长度可调用函数【GetImageMemorySize】
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] array           字节数组
// 【参数 2】      [IN]  image           mex图像缓存（指针）
// 【返回值】      无
void MexImageToByteArray(byte *array, Mex_Image *image);

// 【功  能】  转换字节数组到MexImage图像
// 【说  明】  该函数转换转换字节数组缓存到MexImage图像
// 【注  意】  由字节数组缓存转换出的MexImage图像是常量类型，对其内部外不能进行任何操作
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参  数】      [IN]  array           字节数组
// 【返回值】      [OUT] int32型         mex图像缓存（指针）
Mex_Image* ByteArrayToMexImage(byte *array);

// 【功  能】  设置图片透明色
// 【说  明】  该函数设置图片透明色，浅表克隆图像只能更改透明属性，无法更改透明色。
// 【注  意】  若使透明色生效，切记设置图像透明，否则透明色无效
//             该函数存在副作用，即设置透明色失败只代表创建加速器失败，但透明色会被更改
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image           图像结构
// 【参数 2】      [IN]  colorkey        透明色
// 【参数 3】      [IN]  isTrans         是否透明
// 【返回值】      [OUT] bool型          是否成功
bool SetImageColorkey(Mex_Image *image, mex_color colorkey, bool isTrans);

// 【功  能】  设置图片透明色（不使用加速器）
// 【说  明】  该函数设置图片透明色（不使用加速器），浅表克隆图像只能更改透明属性，无法更改透明色。
// 【注  意】  若使透明色生效，切记设置图像透明，否则透明色无效
//             该函数设置透明色但不创建加速器，若图像之前存在加速器将会被释放。
//             使用者使用该函数时要明确使用目的，否则可能出现不可预料错误
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image           图像结构
// 【参数 2】      [IN]  colorkey        透明色
// 【参数 3】      [IN]  isTrans         是否透明
// 【返回值】      [OUT] bool型          是否成功
bool SetImageColorkeyNoAccel(Mex_Image *image, mex_color colorkey, bool isTrans);

// 【功  能】  设置图像alpha值
// 【说  明】  该函数设置图像alpha值
// 【注  意】  该平台使用16位565格式RGB色，所以alpha混合度只支持0~31级
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image             Mex图像指针
// 【参数 2】      [IN]  alpha	           alpha值（0~31）
// 【返回值】      [OUT] 无
void SetImageAlpha(Mex_Image *image, byte alpha);

// 【功  能】  设置图像绘制区域
// 【说  明】  该函数设置图像绘制区域
// 【注  意】  设置绘制区域不适用左上填充原则，即包括左上角及右下角
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image             图像结构（指针）
// 【参数 2】      [IN]  x1	               绘制区域左上角X坐标
// 【参数 3】      [IN]  y1	               绘制区域左上角Y坐标
// 【参数 4】      [IN]  x2	               绘制区域右下角X坐标
// 【参数 5】      [IN]  y2	               绘制区域右下角X坐标
// 【返回值】      [OUT] 无
void SetImageDrawRegion(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2);

// 【功  能】  设置图像旋转中心
// 【说  明】  该函数设置图像旋转中心，图像旋转中心默认为图像的几何中心，坐标为（0，0）
//             如图 图中阴影的方形为实际图像，X,Y坐标系为以图像几何中心为原点的局部坐标系，默认旋转中心为（0，0）
//                  若用户希望图像围绕点（1，1）位置旋转，则需使用该函数将旋转中心设置成（1，1）,则图像将围绕局部坐标系中的点（1，1）旋转
//                  若用户希望图像围绕图像外的点（5，5）旋转，则需使用该函数将旋转中心设置成（5，5），则图像将围绕点（5，5）旋转
//
//                           ∧Y
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
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] image             图像结构（指针）
// 【参数 2】      [IN]  center_x	       旋转中心X坐标
// 【参数 3】      [IN]  center_y	       旋转中心Y坐标
// 【返回值】      [OUT] 无
void SetImageRotateCenter(Mex_Image *image, int16 center_x, int16 center_y);

// 【功  能】  绘制图像
// 【说  明】  该函数在指定缓存中指定位置绘制指定图像，默认为屏幕缓存。该函数会自动判断图像的属性进行绘制
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像结构（指针）
// 【参数 2】      [IN]  x               x坐标
// 【参数 3】      [IN]  y               y坐标
// 【返回值】      [OUT] 无
void DrawImage(Mex_Image *image, int16 x, int16 y);

// 【功  能】  缩放图像
// 【说  明】  该函数在指定缓存中指定位置绘制缩放后图像，默认为屏幕缓存。该函数会自动判断图像的属性进行绘制
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像结构（指针）
// 【参数 2】      [IN]  x1              指定绘制区域左上角x坐标
// 【参数 3】      [IN]  y1              指定绘制区域左上角y坐标
// 【参数 4】      [IN]  x2              指定绘制区域右下角x坐标
// 【参数 5】      [IN]  y2              指定绘制区域右下角y坐标
// 【返回值】      [OUT] 无
void DrawImageScale(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2);

// 【功  能】  翻转图像
// 【说  明】  该函数在指定缓存中指定位置绘制翻转后图像，默认为屏幕缓存。该函数会自动判断图像的属性进行绘制
//             翻转类型参见【图像各属性说明】
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像结构（指针）
// 【参数 2】      [IN]  x               指定绘制区域左上角x坐标
// 【参数 3】      [IN]  y               指定绘制区域左上角y坐标
// 【参数 4】      [IN]  type            翻转类型
// 【返回值】      [OUT] 无
void DrawImageOverturn(Mex_Image *image, int16 x, int16 y, int16 type);

// 【功  能】  翻转图像
// 【说  明】  该函数在指定缓存中指定位置缩放翻转后图像，默认为屏幕缓存。该函数会自动判断图像的属性进行绘制
//             翻转类型参见【图像各属性说明】
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像结构（指针）
// 【参数 2】      [IN]  x1              指定绘制区域左上角x坐标
// 【参数 3】      [IN]  y1              指定绘制区域左上角y坐标
// 【参数 4】      [IN]  x2              指定绘制区域右下角x坐标
// 【参数 5】      [IN]  y2              指定绘制区域右下角y坐标
// 【参数 6】      [IN]  type            翻转类型
// 【返回值】      [OUT] 无
void DrawImageOverturnScale(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 type);

// 【功  能】  绘制旋转图像
// 【说  明】  该函数绘制旋转图像，正角度为逆时针，负角度为顺时针。
//             参数center_x, center_y为欲旋转的图像的旋转中心在屏幕中的位置
//             例 若参数（center_x，center_y）为（100，100），而图像的旋转中心在局部坐标系的（30，30）,则实际绘制时，
//                图像将把局部坐标系中的旋转中心（30，30）映射成屏幕上（100，100）的点，并且围绕该点旋转
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像指针
// 【参数 2】      [IN]  center_x        图像旋转中心在屏幕坐标系中X坐标
// 【参数 3】      [IN]  center_y        图像旋转中心在屏幕坐标系中Y坐标
// 【参数 4】      [IN]  angle           旋转角度（单位：角度）
// 【返回值】      [OUT] 无
void DrawImageRotate(Mex_Image *image, int16 center_x, int16 center_y, int16 angle);

// 【功  能】  绘制缩放旋转图像
// 【说  明】  该函数缩放绘制旋转图像，即绘制旋转时，将实际图像按找指定宽度及高度进行缩放。正角度为逆时针，负角度为顺时针。
//             参数center_x, center_y为欲旋转的图像的旋转中心在屏幕中的位置
//             例 若参数（center_x，center_y）为（100，100），而图像的旋转中心在局部坐标系的（30，30）,则实际绘制时，
//                图像将把局部坐标系中的旋转中心（30，30）映射成屏幕上（100，100）的点，并且围绕该点旋转
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像指针
// 【参数 2】      [IN]  scale_width     图像缩放后宽度
// 【参数 3】      [IN]  scale_height    图像缩放后高度
// 【参数 4】      [IN]  center_x        图像旋转中心在屏幕坐标系中X坐标
// 【参数 5】      [IN]  center_y        图像旋转中心在屏幕坐标系中Y坐标
// 【参数 6】      [IN]  angle           旋转角度（单位：角度）
// 【返回值】      [OUT] 无
void DrawImageRotateScale(Mex_Image *image, uint16 scale_width, uint16 scale_height, int16 center_x, int16 center_y, int16 angle);

// 【功  能】  反转图像
// 【说  明】  该函数在指定缓存中指定位置绘制反转后图像，默认为屏幕缓存。该函数会自动判断图像的属性进行绘制
//             反转类型参见【图像各属性说明】
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像结构（指针）
// 【参数 2】      [IN]  x1              指定绘制区域左上角x坐标
// 【参数 3】      [IN]  y1              指定绘制区域左上角y坐标
// 【参数 4】      [IN]  type            反转类型
// 【返回值】      [OUT] 无
void DrawImageFlip(Mex_Image *image, int16 x, int16 y, int16 type);

// 【功  能】  反转并翻转图像
// 【说  明】  该函数在指定缓存中指定位置绘制反转并翻转后图像，默认为屏幕缓存。该函数会自动判断图像的属性进行绘制
//             反转类型及翻转类型可进行【|】运算来进行组合
//             反转类型及翻转类型参见【图像各属性说明】
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像结构（指针）
// 【参数 2】      [IN]  x1              指定绘制区域左上角x坐标
// 【参数 3】      [IN]  y1              指定绘制区域左上角y坐标
// 【参数 4】      [IN]  type            反转类型及翻转类型
// 【返回值】      [OUT] 无
void DrawImageFlipOverturn(Mex_Image *image, int16 x, int16 y, int16 type);

// 【功  能】  旋转反转图像
// 【说  明】  该函数旋转翻转后图像，正角度为逆时针，负角度为顺时针。
//             参数center_x, center_y为欲旋转的图像的旋转中心在屏幕中的位置
//             例 若参数（center_x，center_y）为（100，100），而图像的旋转中心在局部坐标系的（30，30）,则实际绘制时，
//                图像将把局部坐标系中的旋转中心（30，30）映射成屏幕上（100，100）的点，并且围绕该点旋转
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像指针
// 【参数 2】      [IN]  center_x        图像旋转中心在屏幕坐标系中X坐标
// 【参数 3】      [IN]  center_y        图像旋转中心在屏幕坐标系中Y坐标
// 【参数 4】      [IN]  angle           旋转角度（单位：角度）
// 【参数 5】      [IN]  type            反转类型
// 【返回值】      [OUT] 无
void DrawImageFlipRotate(Mex_Image *image, int16 center_x, int16 center_y, int16 angle, int16 type);

// 【功  能】  仿射图像
// 【说  明】  该函数在指定缓存中指定位置仿射图像，默认为屏幕缓存。该函数会自动判断图像的属性进行绘制
//			   仿射类型参见【图像各属性说明】
//             若使用水平仿射，则只有x1, y1, x2, x3, y3, x4坐标起作用
//             若使用竖直仿射，则只有x1, y1, y2, x3, y3, y4坐标起作用
// 【注  意】  该函数传入的四个点为仿射后图形边界坐标，不遵从左上填充原则，及包含所有四个点
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  image           图像结构（指针）
// 【参数 2】      [IN]  x1              指定绘制区域左上角x坐标
// 【参数 3】      [IN]  y1              指定绘制区域左上角y坐标
// 【参数 4】      [IN]  x2              指定绘制区域右上角x坐标
// 【参数 5】      [IN]  y2              指定绘制区域右上角y坐标
// 【参数 6】      [IN]  x3              指定绘制区域右下角x坐标
// 【参数 7】      [IN]  y3              指定绘制区域右下角y坐标
// 【参数 8】      [IN]  x4              指定绘制区域左下角x坐标
// 【参数 9】      [IN]  y4              指定绘制区域左下角y坐标          
// 【参数10】      [IN]  type            仿射类型
// 【返回值】      [OUT] 无
void DrawImageAffine(Mex_Image *image, int16 x1, int16 y1, int16 x2, int16 y2, int16 x3, int16 y3, int16 x4, int16 y4, int16 type);

// 【功  能】  转换MexImage图像到文件
// 【说  明】  该函数转换MexImage图像到文件中连续字节数组
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN/OUT] file         文件句柄
// 【参数 2】      [IN]  image           mex图像缓存（指针）
// 【返回值】      无
void MexImageToFileHandle(int32 *file, Mex_Image *image);

#endif

#endif
