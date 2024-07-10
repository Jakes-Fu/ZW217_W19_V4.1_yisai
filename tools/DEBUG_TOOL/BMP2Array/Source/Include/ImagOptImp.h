// ImagOptImp.h: interface for the CImagOptImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_IMAGOPTIMP_H__)
#define _IMAGOPTIMP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4514)
#include "IImagWrapper.h"

class SCIDLL_EXPORT_CLASS CImagOptImp : public IImagWrapper  
{
public:
	CImagOptImp();
	virtual ~CImagOptImp();

	virtual void  X_IMG_xFreeMem(void *ptr);
	virtual bool  X_IMG_CompressBmp(
					   const uint8 *bmpfile_stream,
					   uint8 **code_stream,
					   uint32 *codelen,
					   uint32 reserved =0
					   );
	virtual bool X_IMG_CompressMovie(
						 const char *dirname,
						 uint32 framenum,
						 uint8 **code_stream,
						 uint32 *codelen,
						 uint32 same_color888,
						 uint32 reserved =0
						 );
	virtual bool X_IMG_ExtractBmp(
					  const uint8* code_stream,
					  uint8 **bmpfile_stream,
					  uint32 *filelen,
					  uint32 reserved =0
					  );
	virtual bool X_IMG_ExtractMovie(
						const uint8* code_stream,
						char *dirname,
						uint32 *framenum,
						uint32 reserved =0
						);
	virtual bool X_IMG_ExtractMovieByIdx(
							 const uint8* code_stream,
							 uint8 ** pBmp,
							 uint16 index,
							 uint32 reserved =0
							 );
	virtual bool   X_IMG_Dithering( uint8  *psrc,	// input RGB888 data
									uint8  *pdst,	    // output RGB565 data
									uint16 sub_block_start_x,  // subBlock start row position
									uint16 sub_block_start_y,  // subBlock start col position
									uint16 sub_block_width,   // subBlock width
									uint16 sub_block_height,  // subBlock height
									uint16 img_width,		// image data width
									uint16 img_height,		// image data height
									uint32 reserved =0
			                      );
	virtual bool   X_IMG_GenJpeg(   uint8  *pRGBData,	// input RGB888 data
									uint32 nRGBSize,// bmp size
									uint32 nHeight,
									uint32 nWidth,
									uint32 nMCU,  // mcu info
									uint32 nQL,  // quant level						
									uint8  **pJpegData,
									uint32 *pJpegSize,
									uint32 reserved =0  );
	virtual bool X_IMG_Compress(X_IMG_PARAM_PTR pImgParam);
	virtual bool X_IMG_Uncompress(X_IMG_PARAM_PTR pImgParam);
	virtual uint32 X_IMG_GetLastError();
	virtual void Release();
protected:
	bool ConvRGBToYUV( uint8 *pRGBData, 
		               uint32 nRGBSize,// bmp size
					   uint32 nHeight,
					   uint32 nWidth,
					   uint8 **pYUVData, 
					   uint32 *pYUVSize,
					   uint32 nMCU);
	bool CompressGif(X_IMG_PARAM_PTR pImgParam);
	bool CompressArgb(X_IMG_PARAM_PTR pImgParam);
	bool CompressJpg(X_IMG_PARAM_PTR pImgParam);
	bool PngToArgb(X_IMG_PARAM_PTR pImgParam);
	bool PngToBmp32(char * szFileName, uint8** ppDstData, uint32 * pDstSize);
	bool PngToSabm(X_IMG_PARAM_PTR pImgParam);
	bool CompressSabm(X_IMG_PARAM_PTR pImgParam);
	bool ArgbToBmp32(X_IMG_PARAM_PTR pImgParam);
	bool SabmToBmp32(X_IMG_PARAM_PTR pImgParam);
	bool SgifToGif(X_IMG_PARAM_PTR pImgParam);
	bool SjpgToJpg(X_IMG_PARAM_PTR pImgParam);
	bool Bmp32ToPng(uint8 *pbmpbuf,uint32 bmpsize,uint8** ppDstData, uint32 * pDstSize);
	
private:
	uint32 m_uErrorCode;

};

#endif // !defined(_IMAGOPTIMP_H__)
