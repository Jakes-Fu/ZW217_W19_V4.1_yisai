// IImagWraper.h: interface for the IImagWraper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IIMAGWRAPPER_H__F6657370_F267_406F_AEEC_A317D49EBAC5__INCLUDED_)
#define AFX_IIMAGWRAPPER_H__F6657370_F267_406F_AEEC_A317D49EBAC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sci_types.h"
#include "SciDllExport.h"
#include <memory.h>

const unsigned long IMAG_DPL = 0; // the based img lib
const unsigned long IMAG_PRV = 1; // modified the animimation key color
const unsigned long IMAG_OPT = 2; // optimized img lib

//error define
#define X_IMG_ERROR_NONE			0
#define X_IMG_ERROR_NO_ALPHA		1
#define X_IMG_ERROR_COLOR_FULL      2

typedef struct _X_IMG_PARAM_T
{
	uint8*	pSrcData;
	uint32	dwSrcSize;
	uint8*	pDstData;
	uint32	dwDstSize;
	uint32  dwWidth;
	uint32  dwHeight;
	uint8	nImgType;
	uint8	nImgCmpType;
	uint8   nQualityType;       // 0:Q40,1:Q50, 2:Q70, 3:Q85, 4:Q95
	uint8   nYUVType;      // 0:420, 1:411, 2:444, 3:422, 4:400
	char    szImgFilePath[260];
	uint32   dwSabmFlag;

	_X_IMG_PARAM_T()
	{
		memset(this,0,sizeof(_X_IMG_PARAM_T));
	}
}X_IMG_PARAM_T,*X_IMG_PARAM_PTR;

class IImagWrapper  
{
public:
	virtual void   X_IMG_xFreeMem(void *ptr)=0;
	virtual bool   X_IMG_CompressBmp(
					   const uint8 *bmpfile_stream,
					   uint8 **code_stream,
					   uint32 *codelen,
					   uint32 reserved =0
					   )=0;
	virtual bool   X_IMG_CompressMovie(
						 const char *dirname,
						 uint32 framenum,
						 uint8 **code_stream,
						 uint32 *codelen,
						 uint32 same_color888,
						 uint32 reserved =0	
						 )=0;
	virtual bool   X_IMG_ExtractBmp(
					  const uint8* code_stream,
					  uint8 **bmpfile_stream,
					  uint32 *filelen,
					  uint32 reserved =0
					  )=0;
	virtual bool   X_IMG_ExtractMovie(
						const uint8* code_stream,
						char *dirname,
						uint32 *framenum,
						uint32 reserved =0
						)=0;
	virtual bool   X_IMG_ExtractMovieByIdx(
							 const uint8* code_stream,
							 uint8 ** pBmp,
							 uint16 index,
							 uint32 reserved =0
							 )=0;
	virtual bool   X_IMG_Dithering( uint8  *psrc,	// input RGB888 data
									uint8  *pdst,	    // output RGB565 data
									uint16 sub_block_start_x,  // subBlock start row position
									uint16 sub_block_start_y,  // subBlock start col position
									uint16 sub_block_width,   // subBlock width
									uint16 sub_block_height,  // subBlock height
									uint16 img_width,		// image data width
									uint16 img_height,		// image data height
									uint32 reserved =0
			                      ) =0;
	/**
	 *  reserved == 0, remove head
	 *  reserved == 1, not remove head
	 */
	virtual bool   X_IMG_GenJpeg(   uint8  *pRGBData,	// input RGB888 data
									uint32 nRGBSize,// bmp size
									uint32 nHeight,
									uint32 nWidth,
									uint32 nMCU,  // mcu info
									uint32 nQL,  // quant level					
									uint8  **pJpegData,
									uint32 *pJpegSize,
									uint32 reserved =0
			                      ) =0;
	virtual bool   X_IMG_Compress(X_IMG_PARAM_PTR pImgParam) = 0;
	virtual bool   X_IMG_Uncompress(X_IMG_PARAM_PTR pImgParam) = 0;
	virtual uint32 X_IMG_GetLastError()=0;
	virtual void Release() = 0;
	virtual ~IImagWrapper()=0;

};


SCIDLL_EXPORT_API bool CreateImagFactory ( IImagWrapper ** ppImagWrapper, unsigned long uVersionID = 0);


#endif // !defined(AFX_IIMAGWRAPPER_H__F6657370_F267_406F_AEEC_A317D49EBAC5__INCLUDED_)
