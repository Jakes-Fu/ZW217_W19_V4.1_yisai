#if !defined(_IRESCONVERTER_H_)
#define _IRESCONVERTER_H_

#pragma once

#include "SciDllExport.h"
#include "GlobalDef.h"

const UINT RC_MEM_TYPE_NORMAL = 0;  // memory is malloced by this DLL
const UINT RC_MEM_TYPE_IMGDLL = 1;  // memory is malloced by IMG DLL
const UINT RC_MEM_TYPE_OSDDLL = 2;  // memory is malloced by OSD DLL
class IResConverter
{
public:
    //IMG & ANIM
    virtual HRESULT LoadBmpFile( LPCTSTR pszFileName, LPBYTE * pBmpStream, int * pSize = NULL, BOOL b565 = TRUE )=0;

    virtual HRESULT GetBmpSize( LPBYTE pBmp, CSize &size )=0;
    virtual HRESULT GetAnimSize( LPBYTE pAnim,CSize &size, UINT uCmpType = 0)=0;

    virtual HRESULT CreateBitmap( LPBYTE pBmpStream,HBITMAP &hBitmap )=0;
    virtual HRESULT CreateBitmap( LPCTSTR pszFileName,HBITMAP &hBitmap )=0;
    virtual void    DeleteBitmap( HBITMAP hBmp )=0;

    // 将原始位图数据流转换为16位565格式的数据流(这里的数据流指完整的位图文件内容)
    // 如果pBmp就是16位565格式的，直接返回此指针，否则会产生新的内存块
    virtual HRESULT Convert24To565Stream( LPBYTE p24Bmp, LPBYTE &p16Bmp, int &nSize,int nDither = 0 )=0;
    virtual HRESULT Convert555To565Stream( LPBYTE p555Bmp, LPBYTE &p565Bmp, int &nSize )=0;
    virtual HRESULT Convert16To24Stream( LPBYTE p16Bmp, LPBYTE & p24Bmp, int &nSize )=0;

    virtual HRESULT CompressBmp( LPBYTE p565Bmp, LPBYTE &pCompressed, int &nCompressedSize,
								UINT uCmpType=0,UINT uQuality = DEF_JEPG_QUALITY,
								DWORD dwImgType = IMG_TYPE_NORMAL)=0;
    virtual HRESULT CompressAnim( LPCTSTR pszDirName, LPBYTE &pCompressed, int &nCompressedSize, 
								int &nNum, UINT nTransColor = 0,BOOL b565 = TRUE,
								UINT uCmpType=0,UINT uQuality = DEF_JEPG_QUALITY,UINT uRate = DEF_MJPEG_RATE)=0;

    virtual HRESULT UnCompressBmp( LPBYTE pCompressed, LPBYTE &pBmp, int &nSize,UINT uCmpType=0 )=0;
    virtual HRESULT UnCompressAnim( LPBYTE pAnim, LPCTSTR pszDirName,UINT uCmpType=0 )=0;
    virtual HRESULT UnCompressAnim( LPBYTE pAnim, int nIdx, LPBYTE &pBmp,UINT uCmpType=0)=0;

	
	virtual HRESULT BmpConvertToBigEndian(LPBYTE p565Bmp, int nSize, BOOL bCompressed,UINT uImagType = IMG_TYPE_NORMAL)=0;
	virtual HRESULT BmpConvertToLitEndian(LPBYTE p565Bmp, int nSize, BOOL bCompressed,UINT uImagType = IMG_TYPE_NORMAL)=0;

	virtual HRESULT AnimConvertToBigEndian(LPBYTE pMovie, int nSize, BOOL bCompressed = TRUE)=0;
	virtual HRESULT AnimConvertToLitEndian(LPBYTE pMovie, int nSize, BOOL bCompressed = TRUE)=0;
	virtual HRESULT IsAnimValidate(LPCTSTR pszDirName, int * pFrameNum, UINT uImgType = IMG_BMP)=0;
    
	virtual HRESULT GetAnimTransColor(LPBYTE pAnim,DWORD &dwKeyColor)=0;
	virtual HRESULT ForceConvertBmpFile( LPCTSTR lpszSrcFileName,LPCTSTR lpszDstFileName = NULL, DWORD dwReserved =0)=0;
	
	virtual HRESULT DeleteDirectory(LPCTSTR lpszDirName)=0;
	//Text
	virtual HRESULT ConvertTextString(LPCTSTR lpszScr, LPBYTE &lpszDst,int &nDstSize,  int &nCodeType,
										BOOL bBigEndian,int nPrecomposedFlag =0,DWORD dwReserved = 0)=0;
	//Font
	virtual HRESULT LoadRingFile(LPCTSTR lpszRingFileName,LPVOID pXRingT,DWORD dwReserved = 0)=0;
	virtual HRESULT ReleaseXRingT(LPVOID pXRingT)=0;
	//Font Lib
/**
  * dwReserved 0, ARM FONT; 1, WIN FONT
  *
  */
	virtual HRESULT LoadFontLib(LPCTSTR lpszFontDBFileName,LPVOID pXFontDbT,DWORD dwReserved = 0)=0;

	virtual HRESULT ReleaseXFontDbT(LPVOID pXFontDbT)=0;

	//Font
	virtual HRESULT LoadOthrFile(LPCTSTR lpszOthrFileName,LPVOID pXOthrT,DWORD dwReserved = 0)=0;
	virtual HRESULT ReleaseXOthrT(LPVOID pXOthrT)=0;

    //bFromImgLib 表示是来自IMG库分配的内存空间
	virtual HRESULT ReleaseMemory(LPBYTE pMemory, UINT uMemType = RC_MEM_TYPE_NORMAL) = 0;

	virtual HRESULT LoadImagFile(LPCTSTR lpszFileName,IMG_PARAM_PTR pImgParam)=0;
	virtual HRESULT LoadAnimFolder(LPCTSTR lpszAnimFolder,ANIM_PARAM_PTR pAnimParam)=0;

	virtual HRESULT ExpImagFile(LPCTSTR lpszFileName,IMG_PARAM_PTR pImgParam)=0;
    virtual HRESULT ExpAnimFolder(LPCTSTR lpszAnimFolder,ANIM_PARAM_PTR pAnimParam)=0;

    virtual void Release()=0;
	virtual ~IResConverter()=0;
};

SCIDLL_EXPORT_API BOOL CreateResConverter ( IResConverter ** ppResConverter,UINT uPlatform = 0 );

#endif //_IRESCONVERTER_H_