// OsdGenerater.cpp: implementation of the COsdGenerater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BmpToArray.h"
#include "OsdGenerater.h"

#pragma warning(push,3)
#include <memory>
#include <algorithm>
#pragma warning(pop)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COsdGenerater::COsdGenerater():CGenerater(TRUE)
{
	memset(m_arrPalette,0,sizeof(m_arrPalette));
	
	m_nBmpHeight = 0;
	m_nBmpWidth  = 0;
	m_nPaletteTotalNum = 0;
	m_nTransparentColor = 0;
	
	m_pBmpRectPtr = NULL;
	m_nBmpRectNum = 0;
}

COsdGenerater::COsdGenerater(BOOL bBigEndian):CGenerater(bBigEndian)
{
	m_nBmpHeight = 0;
	m_nBmpWidth  = 0;
	m_nPaletteTotalNum = 0;
	m_nTransparentColor = 0;
	
	m_pBmpRectPtr = NULL;
	m_nBmpRectNum = 0;
	memset(m_arrPalette,0,sizeof(m_arrPalette));
}

COsdGenerater::~COsdGenerater()
{
	m_pBmpRectPtr=NULL;
}

BOOL COsdGenerater::LoadFromFile(LPCTSTR pFileName, LPBYTE *pContent, int &nContentSize, int nConvertType /*= 0*/)
{
	UNUSED_ALWAYS(nConvertType);

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

		m_nBmpWidth     = bmih.biWidth;
		m_nBmpHeight    = bmih.biHeight;
		int i=0;
		for( i = 0; i < m_nBmpRectNum; i++ )
		{
			if(m_pBmpRectPtr[i]->nHeight == m_nBmpHeight && 
				m_pBmpRectPtr[i]->nWidth == m_nBmpWidth)
			break;
		}
		if(i>=m_nBmpRectNum)
		{
			file.Close();
			CString strErr;
			strErr.Format(IDS_ERR_BMP_RECT,pFileName);
			AfxMessageBox(strErr);
			return FALSE;
		}

        int nBmp24RowSize = ( (m_nBmpWidth * 24 + 31) & ~31 ) / 8;  //24位，扫描行字节数
		int nBmp8RowSize = m_nBmpWidth;                            //8位， 扫描行字节数

        file.Seek(bmfh.bfOffBits, CFile::begin);
        
		int nBmp24Size = nBmp24RowSize * m_nBmpHeight;
		if( nBmp24Size > (int)(dwFileSize - bmfh.bfOffBits))
		{
			file.Close();
            AfxMessageBox(_T("file content size error!"));
            return FALSE;
		}


		std::auto_ptr<BYTE>Buffer24( new BYTE[nBmp24Size] );
		LPBYTE pBmp24 = Buffer24.get();
		if( pBmp24 == NULL )
        {			
            file.Close();
			AfxMessageBox(_T("memory no enough!"));
            return FALSE;
        }
	
        nContentSize = nBmp8RowSize * m_nBmpHeight;
        *pContent = new BYTE[ nContentSize ];
        if( *pContent == NULL )
        {
			file.Close();
			AfxMessageBox(_T("memory no enough!"));
            return FALSE;
        }

		std::auto_ptr<BYTE>Buf8Row( new BYTE[nBmp8RowSize] );
		LPBYTE pBmp8Row = Buf8Row.get();
		if( pBmp8Row == NULL )
		{
			delete [](*pContent);
			*pContent = NULL;
			AfxMessageBox(_T("memory no enough!"));
            file.Close();
            return FALSE;
		}

		VERIFY( (int)(file.Read(pBmp24, nBmp24Size)) == nBmp24Size );
        file.Close();

		for( i = m_nBmpHeight - 1; i >= 0; i--)
		{
			memset( pBmp8Row, 0, nBmp8RowSize );
			// 转换时，只转换有效位数的数据，对齐用的数据0忽略。
		    if(!ParsePalette(pBmp24 + i * nBmp24RowSize, m_nBmpWidth * 3, pBmp8Row, nBmp8RowSize))
			{
				delete [](*pContent);
				*pContent = NULL;
				return FALSE;
			}
			
			memcpy((*pContent + (m_nBmpHeight - 1 - i) * nBmp8RowSize), pBmp8Row, nBmp8RowSize);
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

void COsdGenerater::SetParameters(int nTransparentColor,BMP_RECT_PTR * pBmpRectPtr,int nBmpRectNum)
{
	m_nTransparentColor = nTransparentColor;
	m_pBmpRectPtr = pBmpRectPtr;
	m_nBmpRectNum = nBmpRectNum;
}

BOOL COsdGenerater::Generate(LPCTSTR pArrayName, const uint8 *pContent, uint32 nContentSize, BOOL bReverse /*= FALSE*/)
{
    _ASSERTE(pArrayName != NULL);
    _ASSERTE(pContent != NULL);
    _ASSERTE(nContentSize > 0);
	UNUSED_ALWAYS(bReverse);
	
    try{
        CString strLine;
        strLine.Format(_T("__align(4) const MMIDC_OSD_IMG_DATA_T %s =\n{\n    %d, %d,"), pArrayName,m_nBmpWidth,m_nBmpHeight);
        m_file.WriteString(strLine);
        
        char szValue[ 16 ] = { 0 };
		
		//        if( bReverse )
		//        {
		//            pContent += ( nContentSize - 1);
		//        }
		
        for(uint32 i = 0; i < nContentSize; ++i)
        {
            if( i % NUMPERLINE == 0 )
            {
                m_file.WriteString(_T("\n    "));
            }
            
			sprintf(szValue, "%02x", *pContent);
			strLine.Format(_T("0x%s,"), szValue);
			m_file.WriteString(strLine);  
			
			//            if( bReverse )
			//			{
			//				pContent--;
			//			}
			//            else
			//			{
			pContent++;
			//			}
        }
        
        m_file.WriteString(_T("\n};\n\n\n"));
    }
    catch(CFileException *pfex)
    {
        pfex->ReportError();
        pfex->Delete();
        return FALSE;
    }
    
    return TRUE;
}


BOOL COsdGenerater::ParsePalette(LPBYTE pSrc24, int nSrcSize, uint8 * pDst8, int nDstSize)
{
	UNREFERENCED_PARAMETER(nDstSize);
	_ASSERTE( pSrc24 != NULL && pDst8 != NULL );
	
	UINT wValue;
	
	int j = 0;
	
	UINT cr_palette = 0;
	
	for(int i = 0; i < (int)nSrcSize; i += 3)
	{
		if(m_nPaletteTotalNum > 64)
		{
			AfxMessageBox(_T("The number of colors in the palette is more than 64!"));
			return FALSE;
		}
		wValue = *pSrc24;
		cr_palette = wValue;
		pSrc24++;
		
		wValue = *pSrc24;
		wValue <<= 8;
		cr_palette |= wValue;
		pSrc24++;
		
		wValue = *pSrc24;
		wValue <<= 16;
		cr_palette |= wValue;
		pSrc24++;
		
		//判断是否是透明色
		//if(cr_palette == m_nTransparentColor)
		//{
		//	*pDst8 = 0x00;
		//	pDst8++;
		//	cr_palette = 0;
		//	continue;
		//}
		
		for (j = 0; j < m_nPaletteTotalNum; j++)
		{
			if (cr_palette == m_arrPalette[j])
			{
				*pDst8 = (BYTE)j;
				break;
			}
		}
		
		if (j == m_nPaletteTotalNum)
		{
			m_arrPalette[j] = cr_palette;
			*pDst8 = (BYTE)j;
			m_nPaletteTotalNum ++;
		}
		else if (j > m_nPaletteTotalNum)
		{
			_ASSERTE(0);
		}
		
		//*pDst8 |= 0xC0;	
		
		//判断是否是透明色
		if (m_arrPalette[j] != m_nTransparentColor)
		{
			*pDst8 |= 0xC0;
		}
		
		pDst8++;
		cr_palette = 0;
		
	}
	return TRUE;
}
