// Generater.cpp: implementation of the CGenerater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BmpToArray.h"
#include "Generater.h"
#include "Converter.h"
#include "ImagOptImp.h"
#include "ijl.h"

#pragma warning(push,3)
#include <memory>
#include <algorithm>
#pragma warning(pop)

#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int CGenerater::NUMPERLINE = 20;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGenerater::CGenerater()
{
	m_bBigEndian = TRUE;
	m_pImgWrapper = new CImagOptImp;
	m_pResConv = NULL;
	m_szErrMsg[0] = '\0';
	CreateResConverter(&m_pResConv,2);
	
}

CGenerater::CGenerater(BOOL bBigEndian)
{
	m_bBigEndian = bBigEndian;
	m_pImgWrapper = new CImagOptImp;
	m_pResConv = NULL;
	m_szErrMsg[0] = '\0';
	CreateResConverter(&m_pResConv,2);
}

CGenerater::~CGenerater()
{
	if(m_pImgWrapper != NULL)
	{
		delete m_pImgWrapper;
		m_pImgWrapper = NULL;
	}

	if(m_pResConv != NULL)
	{
		m_pResConv->Release();
		m_pResConv = NULL;
	}
}


BOOL CGenerater::BeginGenerate(LPCTSTR pFileName)
{
    _ASSERTE(pFileName != NULL);

    try{
        if( !m_file.Open(pFileName, CFile::modeCreate | CFile::modeWrite) )
        {
            AfxMessageBox(ID_ERR_CREATE_FILE);
            return FALSE;
        }
        // write some flag(myself define)
        m_file.WriteString( _T("\n// Array file.\n\n") );
    }
    catch(CFileException * pfex)
    {
        pfex->ReportError();
        pfex->Delete();
        return FALSE;
    }

    return TRUE;
}

BOOL CGenerater::Generate(LPCTSTR pArrayName, const uint8 *pContent, uint32 nContentSize, BOOL bReverse /* = FALSE */)
{
    _ASSERTE(pArrayName != NULL);
    _ASSERTE(pContent != NULL);
    _ASSERTE(nContentSize > 0);

    try{
        CString strLine;
        strLine.Format(_T("__align(4) const uint8 %s[] =\n{"), pArrayName);
        m_file.WriteString(strLine);        
          
        if( bReverse )
        {
            pContent += ( nContentSize - 1);
        }

        for(uint32 i = 0; i < nContentSize; ++i)
        {
            if( i % NUMPERLINE == 0 )
            {
                m_file.WriteString(_T("\n    "));
            }            

			strLine.Format(_T("0x%02X,"), *pContent);
			m_file.WriteString(strLine);  

            if( bReverse )
			{
				pContent--;
			}
            else
			{
                pContent++;
			}
        }
        
        m_file.WriteString(_T("\n};\n"));
    }
    catch(CFileException *pfex)
    {
        pfex->ReportError();
        pfex->Delete();
        return FALSE;
    }
    
    return TRUE;
}
BOOL CGenerater::GenerateBinFile(LPCTSTR lpszDstFile, const uint8 *pContent, uint32 nContentSize)
{
	FILE *pFile = NULL;
	pFile = _tfopen(lpszDstFile,_T("wb"));
	if (pFile == NULL)
	{
		_tprintf(_T("\n[Error] Created destination File failed:\"%s\"\n"),lpszDstFile);
		return FALSE;
	}
	fwrite(pContent,1,nContentSize,pFile);
	fclose(pFile);
	pFile = NULL;
	return true;
}
void CGenerater::EndGenerate()
{
    m_file.Close();
}

void CGenerater::FreeCompressed(uint8 *pCompressed)
{
    m_pImgWrapper->X_IMG_xFreeMem(pCompressed);
}

BOOL CGenerater::LoadFromFile(LPCTSTR pFileName, LPBYTE *pContent, int &nContentSize, int nConvertType)
{
    _ASSERTE( pFileName != NULL && pContent != NULL);

    try{
        CFile file;
        if( !file.Open(pFileName, CFile::modeRead) )
        {
            AfxMessageBox(ID_ERR_READFILE);
            return FALSE;
        }

        DWORD dwFileSize = file.GetLength();

        BITMAPFILEHEADER bmfh = { 0 };
        file.Read(&bmfh, sizeof(BITMAPFILEHEADER));
        if( bmfh.bfType != 0x4d42 ) // 0x4d42 "BM"
        {
            file.Close();
            AfxMessageBox(IDS_ERR_TYPE);
            return FALSE;
        }

        BITMAPINFOHEADER bmih = { sizeof(BITMAPINFOHEADER) };
        file.Read(&bmih, sizeof(BITMAPINFOHEADER));

        if( bmih.biBitCount != 24 )
        {
            file.Close();
            AfxMessageBox(IDS_ERR_COLORDEPTH);
            return FALSE;
        }

		int nBmpWidth     = bmih.biWidth;
		int nBmpHeight    = bmih.biHeight;
        int nBmp24RowSize = ( (nBmpWidth * 24 + 31) & ~31 ) / 8;
		int nBmp16RowSize = nBmpWidth * 2;

        file.Seek(bmfh.bfOffBits, CFile::begin);
        
		int nBmp24Size = nBmp24RowSize * nBmpHeight;
		if( (DWORD)nBmp24Size > dwFileSize - bmfh.bfOffBits)
		{
			file.Close();
            AfxMessageBox(_T("file content size error!"));
            return FALSE;
		}

		std::auto_ptr<BYTE>Buffer24( new BYTE[nBmp24Size] );
		LPBYTE pBmp24 = Buffer24.get();
		if( pBmp24 == NULL )
        {
			AfxMessageBox(_T("memory no enough!"));
            file.Close();
            return FALSE;
        }
	
        nContentSize = nBmp16RowSize * nBmpHeight;
        *pContent = new BYTE[ nContentSize ];
        if( *pContent == NULL )
        {
			AfxMessageBox(_T("memory no enough!"));
            file.Close();
            return FALSE;
        }

		std::auto_ptr<BYTE>Buf16Row( new BYTE[nBmp16RowSize] );
		LPBYTE pBmp16Row = Buf16Row.get();
		if( pBmp16Row == NULL )
		{
			delete []pContent;
			AfxMessageBox(_T("memory no enough!"));
            file.Close();
            return FALSE;
		}

		VERIFY( file.Read(pBmp24, nBmp24Size) == (DWORD)nBmp24Size );
        file.Close();

		typedef void (CGenerater::* PCONVERTFUN)( LPBYTE, int, WORD *, int);
		PCONVERTFUN pConvertFun = NULL;
		switch( nConvertType )
		{
		case 0:        // 8:8:8->5:6:5
			pConvertFun = Bmp_888To565;
			break;
		case 1:        // 8:8:8->4:4:4
			pConvertFun = Bmp_888To444;
			break;
		default:
			_ASSERTE(0);
			break;
		}
		_ASSERTE( pConvertFun != NULL );

		WORD * pWord = NULL;
		for( int i = 0; i < nBmpHeight; ++i)
		{
			memset( pBmp16Row, 0, nBmp16RowSize );
			// 转换时，只转换有效位数的数据，对齐用的数据0忽略。
		    (this->*pConvertFun)(pBmp24 + i * nBmp24RowSize, nBmpWidth * 3, (WORD *)pBmp16Row, nBmp16RowSize);
			
			pWord = (WORD *)pBmp16Row;
			std::reverse(pWord, pWord + nBmpWidth);
			memcpy((*pContent + i * nBmp16RowSize), pBmp16Row, nBmp16RowSize);
		}
    }
    catch(CFileException *pfex)
    {
        pfex->ReportError();
        pfex->Delete();
        return FALSE;
    }

    return TRUE;
}

void CGenerater::Bmp_888To444(LPBYTE p888, int n888Size, WORD * p444, int n444Size)
{
	UNUSED_ALWAYS(n444Size);
	_ASSERTE( p888 != NULL && p444 != NULL );
	_ASSERTE( n888Size * 2 / 3 == n444Size );

	WORD wValue;
	for(int i = 0; i < (int)n888Size; i += 3)
	{
		wValue = (WORD)(*p888 >> 4);
        *p444  = wValue;
		p888++;
	
		wValue = (WORD)(*p888 >> 4);
		wValue <<= 4;
		*p444 |= wValue;
		p888++;
	
		wValue = (WORD)(*p888 >> 4);
		wValue <<= 8;
		*p444 |= wValue;
		p888++;
		
		p444++;
	}
}

void CGenerater::DeleteLoadContent(LPBYTE pContent)
{
    delete []pContent;
}

void CGenerater::Bmp_888To565(LPBYTE p888, int n888Size, WORD * p565, int n565Size)
{
	UNUSED_ALWAYS(n565Size);
	_ASSERTE( p888 != NULL && p565 != NULL );
	_ASSERTE( n888Size * 2 / 3 == n565Size );

	WORD wValue;
	for(int i = 0; i < (int)n888Size; i += 3)
	{
		wValue = (WORD)(*p888 >> 3);
        *p565  = wValue;
		p888++;
	
		wValue = (WORD)(*p888 >> 2);
		wValue <<= 5;
		*p565 |= wValue;
		p888++;
	
		wValue = (WORD)(*p888 >> 3);
		wValue <<= 11;
		*p565 |= wValue;
		p888++;

		p565++;
	}
}
// 将原始位图数据流转换为16位565格式的数据流(这里的数据流指完整的位图文件内容)
// 如果pBmp就是16位565格式的，直接返回此指针，否则会产生新的内存块
BOOL CGenerater::Convert24To565Stream( LPBYTE p24Bmp, LPBYTE &p16Bmp, int &nSize )
{
    _ASSERTE( p24Bmp != NULL );

    PBITMAPFILEHEADER p24bfh = (PBITMAPFILEHEADER)p24Bmp;
    _ASSERTE(p24bfh->bfType == 0x4d42);

    PBITMAPINFOHEADER p24bih = (PBITMAPINFOHEADER)(p24bfh + 1);
    _ASSERTE( p24bih->biBitCount == 24 );

    LPBYTE p24Data = p24Bmp + p24bfh->bfOffBits;

    int nWidth     = p24bih->biWidth;
    int nHeight    = p24bih->biHeight;
    int n24RowSize = ( (nWidth * 24 + 31) & ~31 ) / 8;
    int n16RowSize = ( (nWidth * 16 + 31) & ~31 ) / 8;

    nSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
            3 * sizeof(DWORD) + n16RowSize * nHeight;

    p16Bmp = (LPBYTE)malloc(nSize);
    if( NULL == p16Bmp )
    {
        nSize = 0;
        return FALSE;
    }
    memset( p16Bmp, 0, nSize );

    PBITMAPFILEHEADER p16bfh = (PBITMAPFILEHEADER)p16Bmp;
    memcpy(p16bfh, p24bfh, sizeof(BITMAPFILEHEADER));
    p16bfh->bfOffBits = nSize - n16RowSize * nHeight;
    p16bfh->bfSize    = nSize;

    PBITMAPINFOHEADER p16bih = (PBITMAPINFOHEADER)(p16bfh + 1);
    memcpy(p16bih, p24bih, sizeof(BITMAPINFOHEADER));
    p16bih->biSize        = sizeof(BITMAPINFOHEADER);
    p16bih->biBitCount    = 16;
    p16bih->biCompression = BI_BITFIELDS;
    p16bih->biSizeImage   = n16RowSize * nHeight;
    p16bih->biClrUsed     = 0;

    LPDWORD pRedMask = (LPDWORD)(p16bih + 1);
    *pRedMask = 0xF800;

    LPDWORD pGreenMask = pRedMask + 1;
    *pGreenMask = 0x07E0;

    LPDWORD pBlueMask = pGreenMask + 1;
    *pBlueMask = 0x001F;

    LPBYTE p16Data = (LPBYTE)(pBlueMask + 1);
    
    for( int i = 0; i < nHeight; ++i )
    {
        Bmp_888To565(p24Data + i * n24RowSize, nWidth * 3, (LPWORD)p16Data, n16RowSize);

        p16Data += n16RowSize;
    }
    
    // 调试用
    //FILE * pfile = fopen("c:\\24to565.bmp", "wb");
    //fwrite(p16Bmp, 1, nSize, pfile);
    //fclose(pfile);

    return TRUE;
}


BOOL CGenerater::Convert555To565Stream( LPBYTE p555Bmp, LPBYTE &p565Bmp, int &nSize )
{
    _ASSERTE( p555Bmp != NULL );

    PBITMAPFILEHEADER p555bfh = (PBITMAPFILEHEADER)p555Bmp;
    _ASSERTE(p555bfh->bfType == 0x4d42);

    PBITMAPINFOHEADER p555bih = (PBITMAPINFOHEADER)(p555bfh + 1);
    _ASSERTE( p555bih->biBitCount == 16 );

    LPWORD p555Data = (LPWORD)(p555Bmp + p555bfh->bfOffBits);

    int nWidth     = p555bih->biWidth;
    int nHeight    = p555bih->biHeight;
    int n16RowSize = ( (nWidth * 16 + 31) & ~31 ) / 8;

    nSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
            3 * sizeof(DWORD) + n16RowSize * nHeight;

    p565Bmp = (LPBYTE)malloc(nSize);
    if( NULL == p565Bmp )
    {
        nSize = 0;
        return FALSE;
    }
    memset( p565Bmp, 0, nSize );
    //hongliang.xin 2006-11-13
    PBITMAPFILEHEADER p565bfh = (PBITMAPFILEHEADER)p565Bmp;
    memcpy(p565bfh, p555bfh, sizeof(BITMAPFILEHEADER));
    p565bfh->bfOffBits = nSize - n16RowSize * nHeight;
    p565bfh->bfSize    = nSize;

    PBITMAPINFOHEADER p565bih = (PBITMAPINFOHEADER)(p565bfh + 1);
    memcpy(p565bih, p555bih, sizeof(BITMAPINFOHEADER));
    p565bih->biSize        = sizeof(BITMAPINFOHEADER);
    p565bih->biBitCount    = 16;
    p565bih->biCompression = BI_BITFIELDS;
    p565bih->biSizeImage   = 0;
    p565bih->biClrUsed     = 0;

    LPDWORD pRedMask = (LPDWORD)(p565bih + 1);
    *pRedMask = 0xF800;

    LPDWORD pGreenMask = pRedMask + 1;
    *pGreenMask = 0x07E0;

    LPDWORD pBlueMask = pGreenMask + 1;
    *pBlueMask = 0x001F;

    LPWORD p565Data = (LPWORD)(pBlueMask + 1);
    
    BYTE byBlue, byGreen, byRed;
    int nCount = (n16RowSize / 2) * nHeight;
    for( int i = 0; i < nCount; ++i, p555Data++, p565Data++ )
    {
         byBlue  = (BYTE)(((BYTE)(*p555Data >> 10)) & 0x1F);
         byGreen = (BYTE)(((BYTE)(*p555Data >> 5)) & 0x1F);
         byRed   = (BYTE)(((BYTE)(*p555Data)) & 0x1F);

         *p565Data  = (WORD)(byBlue << 11);
         *p565Data |= (WORD)(byGreen << 6);
         *p565Data |= (WORD)(byRed);
    }
    
    // 调试用
    //FILE * pfile = fopen("c:\\555to565.bmp", "wb");
    //fwrite(p16Bmp, 1, nSize, pfile);
    //fclose(pfile);
    return TRUE;
}
BOOL CGenerater::BmpConvertToBigEndian(LPBYTE p565Bmp, int nSize, BOOL bCompressed)
{
	UNUSED_ALWAYS(bCompressed);
	UNUSED_ALWAYS(nSize);
	int nImgSize = 0;//数据区大小，以字节为单位
	int nOffset = 0;
	CConverter convert;
	BITMAPFILEHEADER *pBmfh = (BITMAPFILEHEADER *)p565Bmp;
	nOffset = pBmfh->bfOffBits;
    pBmfh->bfType = convert.LitEdnToBigEdn(pBmfh->bfType);
	pBmfh->bfSize = convert.ConvEdn_INT(pBmfh->bfSize);
	pBmfh->bfReserved1= convert.LitEdnToBigEdn(pBmfh->bfReserved1);
	pBmfh->bfReserved2 = convert.LitEdnToBigEdn(pBmfh->bfReserved2);
	pBmfh->bfOffBits = convert.ConvEdn_INT(pBmfh->bfOffBits);
	
	BITMAPINFOHEADER *pBmih = (BITMAPINFOHEADER *)(pBmfh+1);
	nImgSize = pBmih->biSizeImage;   
	if(nImgSize == 0)
	{
		nImgSize = nSize-nOffset;
	}
	
	pBmih->biSize = convert.ConvEdn_INT(pBmih->biSize);
	pBmih->biWidth = convert.ConvEdn_INT(pBmih->biWidth);
	pBmih->biHeight = convert.ConvEdn_INT(pBmih->biHeight);
    pBmih->biPlanes = convert.LitEdnToBigEdn(pBmih->biPlanes);
	pBmih->biBitCount = convert.LitEdnToBigEdn(pBmih->biBitCount);
	
	pBmih->biCompression = convert.ConvEdn_INT(pBmih->biCompression);
	pBmih->biSizeImage = convert.ConvEdn_INT(pBmih->biSizeImage);
	pBmih->biXPelsPerMeter = convert.ConvEdn_INT(pBmih->biXPelsPerMeter);
	pBmih->biYPelsPerMeter = convert.ConvEdn_INT(pBmih->biYPelsPerMeter);
	pBmih->biClrUsed = convert.ConvEdn_INT(pBmih->biClrUsed);
	pBmih->biClrImportant = convert.ConvEdn_INT(pBmih->biClrImportant);
	
    if(nOffset == sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER) + 3*sizeof(DWORD))
	{
		//3个DWORD 颜色掩码
		LPDWORD lpdwClr = (LPDWORD)(pBmih+1);
		
		for(int i = 0; i < 3; i++)
		{
			*(lpdwClr+i) = convert.ConvEdn_INT(*(lpdwClr+i));
		}
	}
	
	//数据区
	LPWORD lpwData = (LPWORD)(p565Bmp+nOffset);
//	if(!bCompressed)
		convert.LitEdnToBigEdn(lpwData,nImgSize/2);

	return TRUE;
}
BOOL CGenerater::MovieConvertToBigEndian(LPBYTE pMovie, int nSize, BOOL bCompressed)
{
	UNUSED_ALWAYS(bCompressed);
	UNUSED_ALWAYS(nSize);

	int nOffset = 0;
	CConverter convert;
	BITMAPFILEHEADER *pBmfh = (BITMAPFILEHEADER *)pMovie;
	nOffset = pBmfh->bfOffBits;
    pBmfh->bfType = convert.LitEdnToBigEdn(pBmfh->bfType);
	pBmfh->bfSize = convert.ConvEdn_INT(pBmfh->bfSize);
	pBmfh->bfReserved1= convert.LitEdnToBigEdn(pBmfh->bfReserved1);
	pBmfh->bfReserved2 = convert.LitEdnToBigEdn(pBmfh->bfReserved2);
	pBmfh->bfOffBits = convert.ConvEdn_INT(pBmfh->bfOffBits);
	
	BITMAPINFOHEADER *pBmih = (BITMAPINFOHEADER *)(pBmfh+1);
	
	pBmih->biSize = convert.ConvEdn_INT(pBmih->biSize);
	pBmih->biWidth = convert.ConvEdn_INT(pBmih->biWidth);
	pBmih->biHeight = convert.ConvEdn_INT(pBmih->biHeight);
    pBmih->biPlanes = convert.LitEdnToBigEdn(pBmih->biPlanes);
	pBmih->biBitCount = convert.LitEdnToBigEdn(pBmih->biBitCount);
	
	pBmih->biCompression = convert.ConvEdn_INT(pBmih->biCompression);
	pBmih->biSizeImage = convert.ConvEdn_INT(pBmih->biSizeImage);
	pBmih->biXPelsPerMeter = convert.ConvEdn_INT(pBmih->biXPelsPerMeter);
	pBmih->biYPelsPerMeter = convert.ConvEdn_INT(pBmih->biYPelsPerMeter);
	pBmih->biClrUsed = convert.ConvEdn_INT(pBmih->biClrUsed);
	pBmih->biClrImportant = convert.ConvEdn_INT(pBmih->biClrImportant);

    if(nOffset==sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER) + 3*sizeof(DWORD))
	{
		//3个DWORD 颜色掩码
		LPDWORD lpdwClr = (LPDWORD)(pBmih+1);
		
		for(int i = 0; i < 3; i++)
		{
			*(lpdwClr+i) = convert.ConvEdn_INT(*(lpdwClr+i));
		}
	}

	MOVIEINFO *pMovieInfo = (MOVIEINFO *)(pMovie+nOffset);
	
	int nFrame = pMovieInfo->frame_num;

    pMovieInfo->frame_num = convert.ConvEdn_INT(pMovieInfo->frame_num);

	int nCodeSize = 0;
	int nCodeOffset = 0;
//	int nCount = 0;

	for(int i = 0;i< nFrame;i++)
	{
		nCodeSize = pMovieInfo->frame_infos[i].code_size;
		nCodeOffset = pMovieInfo->frame_infos[i].code_offset;

		pMovieInfo->frame_infos[i].code_size = convert.ConvEdn_INT(pMovieInfo->frame_infos[i].code_size );
		pMovieInfo->frame_infos[i].code_offset = convert.ConvEdn_INT(pMovieInfo->frame_infos[i].code_offset );

		//if(nCodeSize & 0x80000000)
		{
			int nCount =  nCodeSize & 0x7FFFFFFF;
			LPWORD pwData = (LPWORD)(pMovie + nCodeOffset);			
			convert.BigEdnToLitEdn(pwData,nCount/2);
		}
	}
	
	return TRUE;
}


BOOL CGenerater::DeleteDirectory(LPCTSTR lpszDirName)// DeleteDirectory(_T("c:\\aaa"))
{
    CFileFind tempFind;
    _TCHAR tempFileFind[MAX_PATH] = {0};
	_stprintf(tempFileFind,_T("%s\\*.*"),lpszDirName);
    BOOL IsFinded=(BOOL)tempFind.FindFile(tempFileFind);
    while(IsFinded)
    {
        IsFinded=(BOOL)tempFind.FindNextFile();
        if(!tempFind.IsDots())
        {
            _TCHAR foundFileName[MAX_PATH] = {0};
            _tcscpy(foundFileName,tempFind.GetFileName().GetBuffer(MAX_PATH));
            if(tempFind.IsDirectory())
            {
                _TCHAR tempDir[MAX_PATH] = {0};
                _stprintf(tempDir,_T("%s\\%s"),lpszDirName,foundFileName);
                DeleteDirectory(tempDir);
            }
            else
            {
                _TCHAR tempFileName[MAX_PATH] = {0};
                _stprintf(tempFileName,_T("%s\\%s"),lpszDirName,foundFileName);
                DeleteFile(tempFileName);
            }
        }
    }
    tempFind.Close();
    if(!RemoveDirectory(lpszDirName))
    {
        return FALSE;
    }
    return TRUE;
}

BOOL CGenerater::LoadImage(LPCTSTR pFileName, uint8 ** pCompressed, uint32 * pComressedLen,int nType)
{
	IMG_PARAM_T ipt;
	switch(nType)
	{
	case BMP565_RLE:
		ipt.nImgCmpType  = IMG_CMP_BMP_16_565_RLE;
		break;
	case SJPG:
		ipt.nQualityType = JPG_Q95;
		ipt.nYUVType	 = JPG_YUV422;
		ipt.nImgCmpType  = IMG_CMP_JPG_SCI;
		break;
	case SGIF:
		ipt.nImgCmpType  = IMG_CMP_GIF_SCI;
	    break;
	case SPNG:
		ipt.nImgCmpType  = IMG_CMP_PNG_ARGB;
	    break;
	default:
		return FALSE;
	    break;
	}

	HRESULT hr = m_pResConv->LoadImagFile(pFileName,&ipt);
	if(SUCCEEDED(hr))
	{
	
		m_pResConv->ReleaseMemory(ipt.pSrcData);
		if(m_bBigEndian && nType == BMP565_RLE)
		{
			BmpConvertToBigEndian(ipt.pDstData, ipt.dwDstSize,TRUE);
		}
		*pCompressed = ipt.pDstData;
		*pComressedLen = ipt.dwDstSize;
		return TRUE;
	}
	else
	{
		if(ipt.pSrcData!=NULL)
		{
			m_pResConv->ReleaseMemory(ipt.pSrcData);
		}
		if(ipt.pDstData!=NULL)
		{
			m_pResConv->ReleaseMemory(ipt.pDstData);
		}

		switch(hr)
		{
		case X_E_BMP_BIT_NOT_SUPPORT:
			_tcscpy(m_szErrMsg,  _T("BMP is not 16 or 24 bits!\n"));
			break;
		case X_E_NOT_BMP_TYPE:
			_tcscpy(m_szErrMsg,  _T("It is not a bmp file!\n"));
			break;	
		default:
			_tcscpy(m_szErrMsg,  _T("Load image file failed!\n"));
			break;
		}
		if(!g_theApp.m_bCmd)
		{
			AfxMessageBox(m_szErrMsg);
		}
		else
		{
			_tprintf(m_szErrMsg);
		}
		return FALSE;
	}

	
}

void CGenerater::FreeImage(uint8 *pBuf)
{
	if(pBuf != NULL)
	{
		m_pResConv->ReleaseMemory(pBuf);
	}
}

BOOL CGenerater::LoadAnim(LPCTSTR pAnimFolder, uint8 ** pCompressed, uint32 * pComressedLen,int nType)
{
	UNUSED_ALWAYS(nType);

	CString strAnimFrame1;
	CString strAnimFrame2;
	strAnimFrame1.Format(_T("%s\\1.bmp"),pAnimFolder);	
	strAnimFrame2.Format(_T("%s\\01.bmp"),pAnimFolder);

	int nCmpType = -1;
	CFileFind finder;
	if(finder.FindFile(strAnimFrame1) || finder.FindFile(strAnimFrame2))
	{
		CString strFileName = strAnimFrame1;
		if(finder.FindFile(strAnimFrame2))
		{
			strFileName = strAnimFrame2;
		}
	
		nCmpType = ANIM_CMP_BMP_MBMP;

		FILE *pFile = _tfopen(strFileName.operator LPCTSTR(),_T("rb"));
		if(pFile)
		{
			DWORD dwSize = 0;
			fseek(pFile,0,SEEK_END);
			dwSize = ftell(pFile);
			fseek(pFile,0,SEEK_SET);

			BYTE *pBuf = new BYTE[dwSize];
			if(pBuf != NULL)
			{
				fread(pBuf,1,dwSize,pFile);
			}
			fclose(pFile);
			if(pBuf != NULL && dwSize >= (sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)))
			{
				if(((BITMAPINFOHEADER*)(((BITMAPFILEHEADER*)pBuf)+1))->biBitCount == 32)
				{
					nCmpType = ANIM_CMP_BMP_ARGB;
				}
			}
			
			delete [] pBuf;
		}		
	}

	if(nCmpType == -1)
	{
		strAnimFrame1.Format(_T("%s\\1.png"),pAnimFolder);	
		strAnimFrame2.Format(_T("%s\\01.png"),pAnimFolder);

		if(finder.FindFile(strAnimFrame1) || finder.FindFile(strAnimFrame2))
		{
			nCmpType = ANIM_CMP_PNG_ARGB;
		}
	}

	if(nCmpType == -1)
	{
		_tcscpy(m_szErrMsg,  _T("The image file type in anim folder is not supported!\n"));

		if(!g_theApp.m_bCmd)
		{
			AfxMessageBox(m_szErrMsg);
		}
		else
		{
			_tprintf(m_szErrMsg);
		}
		return FALSE;
	}
	

	ANIM_PARAM_T apt;
	apt.nAnimCmpType = (BYTE)nCmpType;

	HRESULT hr = m_pResConv->LoadAnimFolder(pAnimFolder,&apt);
	if(SUCCEEDED(hr))
	{
		if(m_bBigEndian && nCmpType == ANIM_CMP_BMP_MBMP)
		{	
			m_pResConv->AnimConvertToBigEndian(apt.pDstData,apt.dwDstSize,TRUE);
		}
		*pCompressed = apt.pDstData;
		*pComressedLen = apt.dwDstSize;
		return TRUE;
	}
	else
	{
	
		if(apt.pDstData!=NULL)
		{
			m_pResConv->ReleaseMemory(apt.pDstData);
		}

		switch(hr)
		{
		case X_E_BMP_BIT_NOT_SUPPORT:
			_tcscpy(m_szErrMsg,  _T("BMP is not 16 or 24 bits!\n"));
			break;
		case X_E_NOT_BMP_TYPE:
			_tcscpy(m_szErrMsg,  _T("It is not a bmp file!\n"));
			break;	
		default:
			_tcscpy(m_szErrMsg,  _T("Load anim folder failed!\n"));
			break;
		}
		if(!g_theApp.m_bCmd)
		{
			AfxMessageBox(m_szErrMsg);
		}
		else
		{
			_tprintf(m_szErrMsg);
		}
		return FALSE;
	}
	
}

void CGenerater::FreeAnim(uint8 *pBuf)
{
	if(pBuf != NULL)
	{
		m_pResConv->ReleaseMemory(pBuf);
	}
}