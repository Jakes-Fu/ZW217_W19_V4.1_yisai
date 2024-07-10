// GlyphData.cpp: implementation of the CGlyphData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fonttool.h"
#include "GlyphData.h"
#include "Font_Interface.h"
#include <math.h>
//#include "spml_outline.h"
//#include "ftgloadr.h"
#include "spml_xapi.h"

typedef struct  FT_SubGlyphRec_
{
    FT_Int     index;
    FT_UShort  flags;
    FT_Int     arg1;
    FT_Int     arg2;
    FT_Matrix  transform;
	
} FT_SubGlyphRec;

#define FT_PIX_FLOOR( x )     ( (x) & ~63 )
#define FT_PIX_ROUND( x )     FT_PIX_FLOOR( (x) + 32 )
#define FT_PIX_CEIL( x )      FT_PIX_FLOOR( (x) + 63 )

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _STARTFONT			_T("STARTFONT ")
#define _COMMENT			_T("COMMNET ")
#define _CONTENTVERSION		_T("CONTENTVERSION ")
#define _FONT				_T("FONT ")
#define _SIZE				_T("SIZE ")
#define _FONTBOUNDINGBOX	_T("FONTBOUNDINGBOX ")
#define _FAMILY_NAME		_T("FAMILY_NAME")
#define _METRICSSET			_T("METRICSSET ")
#define _SWIDTH				_T("SWIDTH ")
#define _DWIDTH				_T("DWIDTH ")
#define _SWIDTH1			_T("SWIDTH1 ")
#define _DWIDTH1			_T("DWIDTH1 ")
#define _VVECTOR			_T("VVECTOR ")
#define _STARTPROPERTIES	_T("STARTPROPERTIES ")
#define _ENDPROPERTIES		_T("ENDPROPERTIES")
#define _CHARS				_T("CHARS ")
#define _STARTCHAR			_T("STARTCHAR ")
#define _ENCODING			_T("ENCODING ")
#define _BBX				_T("BBX ")
#define _BITMAP				_T("BITMAP")
#define _ENDCHAR			_T("ENDCHAR")
#define _ENDFONT			_T("ENDFONT")
#define _CHARSET_REGISTRY	_T("CHARSET_REGISTRY")
#define _CHARSET_ENCODING	_T("CHARSET_ENCODING")
#define _BITCOUNT           _T("BITCOUNT ")

#define IS_IN_CHAR_RANG(data) ((data) >= (-128) && (data)<=127)
#define IS_IN_BYTE_RANG(data) ((data) >= (0) && (data)<=0xFF)


#define SET_FLAG(d,i)  (d)=(BYTE)(((BYTE)(d)) | ((BYTE)(1<<(i))))
#define UNSET_FLAG(d,i)  (d)=(BYTE)(((BYTE)(d)) & ~((BYTE)(1<<(i))))
#define GET_FLAG(d,i)  (BYTE)(((BYTE)(d)) & ((BYTE)(1<<(i))))

typedef struct _MERGE_T
{
	int nFontSize;
	BOOL bCur;
	int nIdx;
}MERGE_T,*MERGE_PTR;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlyphData::CGlyphData()
{
	m_cmap.Init();
	m_gdef.Init();
	m_gsub.Init();
	m_gpos.Init();

	m_fontHDR.Init();

	m_pGlyphTables = NULL;

	m_pCSI = NULL;

	memset(&m_tMetrics,0,sizeof(m_tMetrics));

	m_hWnd = NULL;

	m_bReady = FALSE;

	m_height = 0;

	m_nOutlineLongjmpNum  = 0;
	m_bZoomLargeFlag = FALSE;
	m_nOverOneByteNum = 0;
	
	m_pTestFile = NULL;
	m_bBigEndian = TRUE;
	m_bAdd = FALSE;
	m_bLoadLib = FALSE;
/*lint -e1401*/
}
/*lint restore*/

CGlyphData::~CGlyphData()
{
	Clear(TRUE);
	if(m_pTestFile != NULL)
	{
		fclose(m_pTestFile);
		m_pTestFile = NULL;
	}
	m_pCSI = NULL;
	m_hWnd = NULL;
}
void CGlyphData::Clear(BOOL bExit /*= FALSE*/)
{
	m_cmap.Clear();
	m_gdef.Clear();
	m_gsub.Clear();
	m_gpos.Clear();

	m_bReady = FALSE;

	m_nOutlineLongjmpNum  = 0;
	m_bZoomLargeFlag = FALSE;
	m_nOverOneByteNum = 0;

	MSG msg;

	int nCount = m_fontHDR.glyph_table_count;

	for(int i = 0; i< nCount && m_pGlyphTables != NULL;i++)
	{
		m_pGlyphTables[i].Clear();
	}
	m_fontHDR.Clear();

	SAFE_FREE_ARRAY(m_pGlyphTables);

	m_lstID.clear();
	m_mapUnid.RemoveAll();
	m_lstUnicode.clear();

	DWORD nID;
	GLYPH_BODY_PTR pGlyf= NULL;

	if(bExit)
	{
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN, m_mapGlyf.GetCount());
	}
	int nStep = 0;

	POSITION pos = m_mapGlyf.GetStartPosition();
	while(pos)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}

		if(bExit)
		{
			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);
		}

		m_mapGlyf.GetNextAssoc(pos,nID,pGlyf);
		if(pGlyf != NULL)
		{			
			for(int j=0; j< nCount;j++)
			{
				pGlyf[j].Clear();
			}		
			delete [] pGlyf;
			pGlyf = NULL;
		}
	}
	m_mapGlyf.RemoveAll();

	if(bExit)
	{
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	}

	nStep = 0;

	memset(&m_tMetrics,0,sizeof(m_tMetrics));
	
	UnicodeArray_ptr pUA= NULL;

	m_mapRidun.RemoveAll();

	pos = m_mapIdun.GetStartPosition();
	while(pos)
	{
		m_mapIdun.GetNextAssoc(pos,nID,pUA);
		if(pUA != NULL)
		{
			pUA->RemoveAll();
			delete pUA;
		}
	}
	m_mapIdun.RemoveAll();	
}

BOOL CGlyphData::Save(LPCTSTR lpszFileName,  BOOL bBinFormat/* = TRUE*/,BOOL bBigEndian/* = TRUE*/)
{
	if(lpszFileName == NULL)
	{
		return FALSE;
	}
	CFileFind finder;
	if(finder.FindFile(lpszFileName))
	{
		DWORD dwAtt = GetFileAttributes(lpszFileName);
		dwAtt &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(lpszFileName,dwAtt);
	}

	FILE * pFile = _tfopen(lpszFileName,bBinFormat?_T("wb"):_T("wt"));
	if(pFile == NULL)
	{
		return FALSE;
	}
	DWORD dwSize = 0;
	DWORD dwTmp = 0;
	m_bBigEndian = bBigEndian;
	/* write font header*/
	if(bBigEndian)
	{
		m_fontHDR.magic[0]='s';
		m_fontHDR.magic[1]='f';
		m_fontHDR.magic[2]='o';
		m_fontHDR.magic[3]='n';
	}
	else
	{
		m_fontHDR.magic[0]='n';
		m_fontHDR.magic[1]='o';
		m_fontHDR.magic[2]='f';
		m_fontHDR.magic[3]='s';
	}
	m_fontHDR.fver[0] = 0;
	m_fontHDR.fver[1] = 0;
	m_fontHDR.fver[2] = 0;
	m_fontHDR.fver[3] = 1;

	dwSize = WriteFontHdr(pFile,bBinFormat,bBigEndian);

	/* seek camp_offset and set its value*/
	fseek(pFile,((DWORD)&m_fontHDR.cmap_offset - (DWORD)&m_fontHDR),SEEK_SET);
	dwTmp = dwSize;
	if(bBigEndian)
	{
		dwTmp = Conv32Endian(dwTmp);
	}
	fwrite(&dwTmp,1,sizeof(dwTmp),pFile);
	fseek(pFile,0,SEEK_END);

	/* write cmap data */
	dwSize = WriteCmapTbl(pFile,bBinFormat,bBigEndian);
	dwSize = ftell(pFile);

	if(NeedOtherTble())
	{
		if(m_gdef.size != 0)
		{
			/* seek gdef_offset and set its value*/
			fseek(pFile,((DWORD)&m_fontHDR.gdef_offset - (DWORD)&m_fontHDR),SEEK_SET);
			dwTmp = dwSize;
			if(bBigEndian)
			{
				dwTmp = Conv32Endian(dwTmp);
			}
			fwrite(&dwTmp,1,sizeof(dwTmp),pFile);
			fseek(pFile,0,SEEK_END);
		}

		/* write gdef data */
		WriteGdefTbl(pFile,bBinFormat,bBigEndian);
		dwSize = ftell(pFile);

		if(m_gsub.size != 0)
		{
			/* seek gsub_offset and set its value*/
			fseek(pFile,((DWORD)&m_fontHDR.gsub_offset - (DWORD)&m_fontHDR),SEEK_SET);
			dwTmp = dwSize;
			if(bBigEndian)
			{
				dwTmp = Conv32Endian(dwTmp);
			}
			fwrite(&dwTmp,1,sizeof(dwTmp),pFile);
			fseek(pFile,0,SEEK_END);
		}

		/* write gsub data */
		WriteGsubTbl(pFile,bBinFormat,bBigEndian);
		dwSize = ftell(pFile);

		if(m_gpos.size != 0)
		{
			/* seek gpos_offset and set its value*/
			fseek(pFile,((DWORD)&m_fontHDR.gpos_offset - (DWORD)&m_fontHDR),SEEK_SET);
			dwTmp = dwSize;
			if(bBigEndian)
			{
				dwTmp = Conv32Endian(dwTmp);
			}
			fwrite(&dwTmp,1,sizeof(dwTmp),pFile);
			fseek(pFile,0,SEEK_END);
		}

		/* write gpos data */
		WriteGposTbl(pFile,bBinFormat,bBigEndian);

	}



	WriteGlyfTbl(pFile,bBinFormat,bBigEndian);
	fclose(pFile);

    DumpOutlineLog(lpszFileName,FALSE);
    

	return TRUE;

}


DWORD CGlyphData::WriteFontHdr(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian)
{
	UNREFERENCED_PARAMETER(bBinFormat);

	DWORD dwSize = 0;
	FONT_HDR_T fht;
	memcpy(&fht,&m_fontHDR,sizeof(fht));

	if(bBigEndian)
	{
		ConvFontHdr(fht);
	}

	dwSize += fwrite(&fht,1,sizeof(fht)-sizeof(DWORD),pFile);

	DWORD dwTmp;
	for(int i=0;i<m_fontHDR.glyph_table_count;i++)
	{
		dwTmp = m_fontHDR.gtbl_offset_ptr[i];
		if(bBigEndian)
		{
			dwTmp = Conv32Endian(dwTmp);
		}
		
		dwSize += fwrite(&dwTmp,1,sizeof(dwTmp),pFile);
	}

	return dwSize;
}
DWORD CGlyphData::WriteCmapTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian)
{
	
	UNREFERENCED_PARAMETER(bBinFormat);
//	BYTE padd[4] = {0};
	UINT i=0;
	WORD wTmp = 0;
	DWORD dwSize = 0;

	//////////////////////////////////////////////////////////////////////////
	// write cmap table data
	CMAP_DATA_T cmap;
	memcpy(&cmap,&m_cmap,sizeof(cmap));
	if(bBigEndian)
	{
		ConvCmapDat(cmap);
	}

	dwSize += fwrite(&cmap,1,sizeof(cmap)-sizeof(DWORD)*6-sizeof(WORD),pFile);

	for(i=0;i< (UINT)(m_cmap.seg_countX2/2);i++)
	{
		wTmp = m_cmap.end_count_ptr[i];
		if(bBigEndian)
		{
			wTmp = Conv16Endian(wTmp);
		}
		dwSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);	
	}

// USHORT segCountX2  2 x segCount. 
// USHORT searchRange  2 x (2**floor(log2(segCount))) 
// USHORT entrySelector  log2(searchRange/2) 
// USHORT rangeShift  2 x segCount - searchRange 
// USHORT endCount[segCount] End characterCode for each segment, last=0xFFFF. 
// USHORT reservedPad Set to 0. 
// USHORT startCount[segCount] Start character code for each segment. 
// SHORT idDelta[segCount] Delta for all character codes in segment. 
// USHORT idRangeOffset[segCount] Offsets into glyphIdArray or 0 
// USHORT glyphIdArray[ ] Glyph index array (arbitrary length) 

	// write reservedPad
	wTmp = 0;
	dwSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);

	for(i=0;i<(UINT)(m_cmap.seg_countX2/2);i++)
	{
		wTmp = m_cmap.start_count_ptr[i];
		if(bBigEndian)
		{
			wTmp = Conv16Endian(wTmp);
		}
		dwSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);
	}

	for(i=0;i<(UINT)(m_cmap.seg_countX2/2);i++)
	{			
		wTmp = (WORD)m_cmap.id_delta_ptr[i];
		if(bBigEndian)
		{
			wTmp = Conv16Endian(wTmp);
		}
		dwSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);
	}


	for(i=0;i<(UINT)(m_cmap.seg_countX2/2);i++)
	{
		wTmp = m_cmap.id_range_offset_ptr[i];
		if(bBigEndian)
		{
			wTmp = Conv16Endian(wTmp);
		}
		dwSize +=fwrite(&wTmp,1,sizeof(wTmp),pFile);		
	}



	if(m_cmap.glyf_id_count != 0)
	{
		for(i=0; i<m_cmap.glyf_id_count; i++)
		{
			wTmp = m_cmap.glyf_id_ptr[i];
			if(bBigEndian)
			{
				wTmp = Conv16Endian(wTmp);
			}

			dwSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);	
		}
	}

	return dwSize;
}

DWORD CGlyphData::WriteGdefTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian)
{
	UNREFERENCED_PARAMETER(bBinFormat);
	UNREFERENCED_PARAMETER(bBigEndian);
	//////////////////////////////////////////////////////////////////////////
	// write gdef table data
	fwrite(m_gdef.data_ptr,1,m_gdef.size,pFile);


	return m_gdef.size;
}

DWORD CGlyphData::WriteGsubTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian)
{
	UNREFERENCED_PARAMETER(bBinFormat);
	UNREFERENCED_PARAMETER(bBigEndian);
	//////////////////////////////////////////////////////////////////////////
	// write gsub table data
	fwrite(m_gsub.data_ptr,1,m_gsub.size,pFile);


	return m_gsub.size;
}

DWORD CGlyphData::WriteGposTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian)
{

	UNREFERENCED_PARAMETER(bBinFormat);
	UNREFERENCED_PARAMETER(bBigEndian);

	//////////////////////////////////////////////////////////////////////////
	// write gpos table data
	fwrite(m_gpos.data_ptr,1,m_gpos.size,pFile);
	return m_gpos.size;
}

DWORD CGlyphData::WriteGlyfTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian)
{
	UNREFERENCED_PARAMETER(bBinFormat);

	Align4(pFile);

	DWORD dwOffset = ftell(pFile);
	
	if(bBigEndian)
	{
		dwOffset= Conv32Endian(dwOffset);
	}

	fseek(pFile,((DWORD)&m_fontHDR.gtbl_offset_ptr - (DWORD)&m_fontHDR),SEEK_SET);
	fwrite(&dwOffset,1,sizeof(dwOffset),pFile);
	fseek(pFile,0,SEEK_END);


	int nCount = m_fontHDR.glyph_table_count;	
	MSG msg;
	DWORD dwSumSize= 0;
	int i=0,j=0;
	for(i=0;i<nCount;i++)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}
		DWORD dwSize = 0;
		GLYPH_TABLE_PTR pGlyfTble = m_pGlyphTables+i;
		GLYPH_TABLE_T gt;
		memcpy(&gt,pGlyfTble,sizeof(gt));
		if(pGlyfTble->hdr.type == FONT_TYPE_VECTOR_CMP || pGlyfTble->hdr.type == FONT_TYPE_VECTOR )
		{
			_ASSERTE(m_pCSI->dZoomRate==VECTOR_BASE_ZOOM || m_pCSI->dZoomRate == VECTOR_MID_ZOOM || m_pCSI->dZoomRate == VECTOR_SMALL_ZOOM);
			// this size is used by MMI, as "multi coefficient" to compute the real font size
			// so it divided by m_pCSI->dZoomRate, not multi.
			gt.hdr.size = (BYTE)(( m_pCSI->nVectorShrinkPercent * 0x80)/100/m_pCSI->dZoomRate);
		}

		
		if(bBigEndian)
		{
			ConvGlyfTbl(&gt);
		}

		dwSumSize += fwrite(&gt.hdr,1,sizeof(gt.hdr),pFile);

		//write page information
		dwSumSize += fwrite(&gt.page_count,1,sizeof(gt.page_count),pFile);
		for(j=0;j<pGlyfTble->page_count;j++)
		{
			WORD wTmp = pGlyfTble->page_ptr[j].end;
			if(bBigEndian)
			{
				wTmp =Conv16Endian(wTmp);
			}
			dwSumSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);
		}
		for(j=0;j<pGlyfTble->page_count;j++)
		{
			WORD wTmp = pGlyfTble->page_ptr[j].start;
			if(bBigEndian)
			{
				wTmp =Conv16Endian(wTmp);
			}
			dwSumSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);
		}
		for(j=0;j<pGlyfTble->page_count;j++)
		{
			WORD wTmp = pGlyfTble->page_ptr[j].offset;
			if(bBigEndian)
			{
				wTmp =Conv16Endian(wTmp);
			}
			dwSumSize += fwrite(&wTmp,1,sizeof(wTmp),pFile);
		}


		DWORD dwOffsetSize = pGlyfTble->hdr.glyph_count * sizeof(DWORD);
		pGlyfTble->offset_ptr = new DWORD[pGlyfTble->hdr.glyph_count];
		memset(pGlyfTble->offset_ptr,0,dwOffsetSize);

		DWORD dwOffsetBase = ftell(pFile);

		dwSumSize += fwrite(pGlyfTble->offset_ptr,1,sizeof(DWORD)*pGlyfTble->hdr.glyph_count,pFile);

		DWORD dwPreSize = 0;

		::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN, pGlyfTble->hdr.glyph_count);
		int nStep = 0;

		for(j=0; j< pGlyfTble->hdr.glyph_count;j++)
		{
			while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);	
			}
			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);

	
			int nID = pGlyfTble->pIDs[j];
			GLYPH_BODY_PTR pGlyf = NULL;
	
			m_mapGlyf.Lookup(nID,pGlyf);
			GLYPH_BODY_PTR pCurGlyf = pGlyf+i;
			BYTE *pBitmap = pCurGlyf->bitmap;			
			int nAlphGlyfSize = 0;
			BYTE *pAlphBitmap = NULL;
			if(pGlyfTble->hdr.type == FONT_TYPE_GRAY_RLE)
			{	
				int nFullSize= pCurGlyf->width * pCurGlyf->height; 
				if(nFullSize != 0 && pBitmap != NULL)
				{			
					pAlphBitmap = new BYTE[nFullSize*2];
					memset(pAlphBitmap,0,nFullSize*2);
					Font_Rle_Compress(pBitmap,pAlphBitmap,pCurGlyf->width,pCurGlyf->height,&nAlphGlyfSize);
					pCurGlyf->flag |= FLAG_SIZE;

				}
			}

			FixGlyfFlag(pCurGlyf,pGlyfTble);
			dwSize = 0;
			dwSize += fwrite(&pCurGlyf->flag,1,sizeof(BYTE),pFile);
			XGLYPH_METRICS_T metrics ={0};
			metrics.x = (char)pCurGlyf->x;
			metrics.y = (char)pCurGlyf->y;
			metrics.dwidth = (BYTE)pCurGlyf->dwidth;
			metrics.width = (BYTE)pCurGlyf->width;
			metrics.height = (BYTE)pCurGlyf->height;
			if(pGlyfTble->hdr.type == FONT_TYPE_VECTOR_CMP || pGlyfTble->hdr.type == FONT_TYPE_VECTOR)
			{
				memcpy(&metrics,&pCurGlyf->metrics,sizeof(metrics));
			}

			if(pCurGlyf->flag & FLAG_ADVANCE)
			{
				dwSize += fwrite(&metrics.dwidth,1,sizeof(BYTE),pFile);
			}
			if(pCurGlyf->flag & FLAG_BEARING_X)
			{
				dwSize += fwrite(&metrics.x,1,sizeof(BYTE),pFile);
			}
			if(pCurGlyf->flag & FLAG_BITMAP_WIDTH)
			{
				dwSize += fwrite(&metrics.width,1,sizeof(BYTE),pFile);
			}
			if(pCurGlyf->flag & FLAG_BEARING_Y)
			{
				dwSize += fwrite(&metrics.y,1,sizeof(BYTE),pFile);
			}
			if(pCurGlyf->flag & FLAG_BITMAP_HEIGHT)
			{
				dwSize += fwrite(&metrics.height,1,sizeof(BYTE),pFile);
			}
			dwSumSize += dwSize;
			if(pCurGlyf->bitmap != NULL || pCurGlyf->nNumberSubGlyph > 0)
			{
				if(pGlyfTble->hdr.type == FONT_TYPE_MONO)
				{
					int glyf_size = pCurGlyf->height * ((pCurGlyf->width+7)/8);
					dwSize += fwrite(pCurGlyf->bitmap,1,glyf_size,pFile);
					dwSumSize += dwSize;
				}
				else if(pGlyfTble->hdr.type == FONT_TYPE_GRAY_RLE)
				{
					if((pCurGlyf->flag & FLAG_SIZE) != 0)
					{
						WORD aglyf_size = Conv16Endian((WORD)nAlphGlyfSize);
						dwSize += fwrite(&aglyf_size,1,sizeof(WORD),pFile);		
					}
					dwSize += fwrite(pAlphBitmap,1,nAlphGlyfSize,pFile);
					dwSumSize += dwSize;
					delete [] pAlphBitmap;
					pAlphBitmap = NULL;
				}
				else if(pGlyfTble->hdr.type == FONT_TYPE_VECTOR_CMP)
				{
                    if ( m_pCSI->bVecCmpMore )
                    {
                        if(pCurGlyf->flag & FLAG_COMPOSE_GLYPH)
                        {
                            dwSize += (DWORD)WriteComposeGlyf(pCurGlyf,pFile,bBigEndian);
                        }
                        else
                        {
                            dwSize += (DWORD)WriteGlyfOutlineEx(pCurGlyf,pFile,bBigEndian);
                        }
                        dwSumSize += dwSize;	
                    }
                    else
                    {
                        dwSize += (DWORD)WriteGlyfOutline(pCurGlyf,pFile,bBigEndian);
                        dwSumSize += dwSize;	
                    }
				}
				else if(pGlyfTble->hdr.type == FONT_TYPE_VECTOR)
				{                    
                    if(pCurGlyf->flag & FLAG_COMPOSE_GLYPH)
                    {
                        dwSize += (DWORD)WriteComposeGlyf(pCurGlyf,pFile,bBigEndian);
                    }
                    else
                    {
                        dwSize += (DWORD)WriteGlyfOutlineFine(pCurGlyf,pFile,bBigEndian);
                    }
                    dwSumSize += dwSize;             
				}
			}
			else if(pGlyfTble->hdr.type == FONT_TYPE_VECTOR_CMP || pGlyfTble->hdr.type == FONT_TYPE_VECTOR)
			{
                if ( (m_pCSI->bVecCmp && m_pCSI->bVecCmpMore) || pGlyfTble->hdr.type == FONT_TYPE_VECTOR)
                {
                    BYTE flag = 0;
                    flag = (OF_CONTOURS_NUM_BYTE | OF_POINTS_NUM_BYTE);
                    dwSize += (DWORD)fwrite(&flag,1,sizeof(flag),pFile); 
                    BYTE wtmp = 0;
                    dwSize += (DWORD)fwrite(&wtmp,1,sizeof(wtmp),pFile); // write point_num = 0
                    dwSize += (DWORD)fwrite(&wtmp,1,sizeof(wtmp),pFile); // write n_conturs = 0
                }
                else
                {
                    WORD wtmp = 0;
                    dwSize += (DWORD)fwrite(&wtmp,1,sizeof(wtmp),pFile); // write point_num = 0
                    dwSize += (DWORD)fwrite(&wtmp,1,sizeof(wtmp),pFile); // write n_conturs = 0
                }
            }

			if(j==0)
			{	
				pGlyfTble->offset_ptr[j] = dwOffsetSize // bitmap data offset size
											  + sizeof(WORD)*3*pGlyfTble->page_count  // page data size
											  + sizeof(WORD); // page count
				dwPreSize = dwSize;
			}
			else
			{
				pGlyfTble->offset_ptr[j] = pGlyfTble->offset_ptr[j-1]+dwPreSize;
				dwPreSize = dwSize;
			}	

		}
		
		fseek(pFile,dwOffsetBase,SEEK_SET);
		for(j=0;j<pGlyfTble->hdr.glyph_count;j++)
		{
			DWORD dwTmp = pGlyfTble->offset_ptr[j];
			if(bBigEndian)
			{
				dwTmp = Conv32Endian(dwTmp);
			}
			fwrite(&dwTmp,1,sizeof(dwTmp),pFile);
		}
		fseek(pFile,0,SEEK_END);

		SAFE_FREE_ARRAY(pGlyfTble->offset_ptr);


		if(i != (m_fontHDR.glyph_table_count-1))
		{
			Align4(pFile);
			dwOffset = ftell(pFile);
			if(bBigEndian)
			{
				dwOffset= Conv32Endian(dwOffset);
			}

			fseek(pFile,((DWORD)&m_fontHDR.gtbl_offset_ptr - (DWORD)&m_fontHDR)+sizeof(DWORD)*(i+1),SEEK_SET);
			fwrite(&dwOffset,1,sizeof(dwOffset),pFile);
			fseek(pFile,0,SEEK_END);
		}

		::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	}

	return dwSumSize;
}

void CGlyphData::ConvFontHdr(FONT_HDR_T &fht)
{
	if(m_bBigEndian)
	{
		fht.glyph_table_count   = Conv16Endian(fht.glyph_table_count);
		fht.flags				= Conv16Endian(fht.flags);
		fht.units_per_em		= Conv16Endian(fht.units_per_em);
		fht.cmap_offset			= Conv32Endian(fht.cmap_offset);
		fht.gdef_offset			= Conv32Endian(fht.gdef_offset);
		fht.gsub_offset			= Conv32Endian(fht.gsub_offset);
		fht.gpos_offset			= Conv32Endian(fht.gpos_offset);
		fht.kern_offset         = Conv32Endian(fht.kern_offset);
	}
}
void CGlyphData::ConvCmapDat(CMAP_DATA_T &cmap)
{
	if(m_bBigEndian)
	{
		cmap.table_count	= Conv16Endian(cmap.table_count);
		cmap.platform_id	= Conv16Endian(cmap.platform_id);
		cmap.encoding_id	= Conv16Endian(cmap.encoding_id);   //only support 1
		cmap.table_offset	= Conv32Endian(cmap.table_offset);
		cmap.format			= Conv16Endian(cmap.format);
		cmap.length			= Conv16Endian(cmap.length);
		cmap.language		= Conv16Endian(cmap.language);
		cmap.seg_countX2	= Conv16Endian(cmap.seg_countX2);
		cmap.search_range	= Conv16Endian(cmap.search_range);
		cmap.entry_selector	= Conv16Endian(cmap.entry_selector);
		cmap.range_shift	= Conv16Endian(cmap.range_shift);	
		cmap.glyf_id_count     = Conv32Endian(cmap.glyf_id_count);	
	}
}

void CGlyphData::ConvGdefDat(GDEF_DATA_T &gdef)
{
	UNREFERENCED_PARAMETER(gdef);
}

void CGlyphData::ConvGposDat(GPOS_DATA_T &gpos)
{
	UNREFERENCED_PARAMETER(gpos);
}

void CGlyphData::ConvGsubDat(GSUB_DATA_T &gsub)
{
	UNREFERENCED_PARAMETER(gsub);
}

void CGlyphData::ConvGlyfTbl(GLYPH_TABLE_PTR pGlyftbl)
{
	if(m_bBigEndian)
	{
//	    pGlyftbl->hdr.reserved2			= Conv16Endian(pGlyftbl->hdr.reserved2);
//		pGlyftbl->hdr.reserved3			= Conv16Endian(pGlyftbl->hdr.reserved3);
		pGlyftbl->hdr.glyph_count			= Conv16Endian(pGlyftbl->hdr.glyph_count);
		pGlyftbl->hdr.page_offsets_base	= Conv32Endian(pGlyftbl->hdr.page_offsets_base);
		pGlyftbl->page_count  = Conv16Endian(pGlyftbl->page_count);
	}

}

BOOL CGlyphData::LoadOtff(LPCTSTR lpszFileName)
{
	if(NULL == m_pCSI || NULL == lpszFileName)
	{
		return FALSE;
	}
	Clear();
	m_bLoadLib = FALSE;

	m_strOtfFile = lpszFileName;
	BOOL bTTC = FALSE;
	if(m_strOtfFile.Right(4).CompareNoCase(_T(".ttc")) == 0)
	{
		bTTC = TRUE;
	}

	std::vector<int> vFtSize;
	CMap<int,int,int,int> mapNormalSize;
	int i=0;
	int nFtTblNum = m_pCSI->nFtTblNum + m_pCSI->nExFtTblNum;
	for(i=0;i<m_pCSI->nFtTblNum ;i++)
	{
		vFtSize.push_back(m_pCSI->pFtTblSize[i]);	
		mapNormalSize.SetAt(m_pCSI->pFtTblSize[i],m_pCSI->pFtTblSize[i]);
	}
	for(i=0;i<m_pCSI->nExFtTblNum ;i++)
	{
		vFtSize.push_back(m_pCSI->pExFtTblSize[i]);	
	}

	std::sort(vFtSize.begin(),vFtSize.end());

	memcpy(&m_fontHDR.tag,&m_pCSI->tag,4);
	m_fontHDR.version[1] = 3;
	m_fontHDR.flags = 0x0001;
	m_fontHDR.glyph_table_count = (WORD)nFtTblNum;
	m_fontHDR.gtbl_offset_ptr = new DWORD[nFtTblNum];
	memset(m_fontHDR.gtbl_offset_ptr,0,sizeof(DWORD)*nFtTblNum);
	m_pGlyphTables = new GLYPH_TABLE_T[nFtTblNum];
	for(i=0;i<nFtTblNum;i++)
	{
		m_pGlyphTables[i].Init();		
	}

	OTF *pOTF = NULL;
	char szFile[_MAX_PATH+1] = {0};
	WideCharToMultiByte(CP_ACP,0,lpszFileName,-1,szFile,_MAX_PATH,NULL,NULL);

	if(!bTTC)
	{	
		pOTF = OTF_open(szFile);
		if(pOTF== NULL)
		{
			Clear();
			AfxMessageBox(_T("OTF open file failed!"));
			return FALSE;
		}

		OTF_get_table (pOTF, "head");
		OTF_get_table (pOTF, "name");
		OTF_get_table (pOTF, "cmap");
		OTF_get_table (pOTF, "GDEF");
		OTF_get_table (pOTF, "GSUB");
		OTF_get_table (pOTF, "GPOS");
		
		m_fontHDR.units_per_em = (WORD)pOTF->head->unitsPerEm;
		int nNumTables = pOTF->offset_table.numTables;		

		if(!DumpCmap(pOTF->cmap))
		{		
			Clear();
			AfxMessageBox(_T("Invalide cmap table!"));
			return FALSE;
		}

		BOOL bOK = TRUE;

		FILE *pFile = fopen(szFile,"rb");

		if(pFile== NULL)
		{
			Clear();
			AfxMessageBox(_T("open file failed!"));
			return FALSE;
		}

		for(i=0;i<nNumTables;i++)
		{	
			switch(pOTF->table_dirs[i].tag)
			{
			case OTF_TAG('c','m','a','p'):
				break;
			case OTF_TAG('G','D','E','F'):
				m_gdef.offset = pOTF->table_dirs[i].offset;
				m_gdef.size   = pOTF->table_dirs[i].length;
				bOK =DumpGdef(pOTF->gdef,pFile);
				break;
			case OTF_TAG('G','S','U','B'):
				m_gsub.offset = pOTF->table_dirs[i].offset;
				m_gsub.size   = pOTF->table_dirs[i].length;
				bOK =DumpGsub(pOTF->gsub,pFile);
				break;
			case OTF_TAG('G','P','O','S'):
				m_gpos.offset = pOTF->table_dirs[i].offset;
				m_gpos.size   = pOTF->table_dirs[i].length;
				bOK =DumpGpos(pOTF->gpos,pFile);
				break;
			default:
				break;
			}
		}

		fclose(pFile);

		if(!bOK)
		{
			OTF_close(pOTF);
			Clear();
			return FALSE;
		}

	}

	if (FT_Init_FreeType( &m_library )) 
	{	
		if(pOTF != NULL)
		{
			OTF_close(pOTF);
			pOTF = NULL;
		}
		Clear();
		AfxMessageBox(_T("FT_Init_FreeType failed"));
		return FALSE;
	}	
	if (FT_New_Face( m_library, szFile, 0, &m_face )) 
	{
		if(pOTF != NULL)
		{
			OTF_close(pOTF);
			pOTF = NULL;
		}
		Clear();
		AfxMessageBox(_T("FT_New_Face failed (there is probably a problem with your font file)"));
		return FALSE;
	}	

	
	MSG msg;

	int nStep = 0;

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,m_pCSI->nCharCount);
	
	std::vector<UINT> vIDs;
	std::vector<UINT> vIDsEx;
	CMap<UINT,UINT,UINT,UINT> mapIDs;
	CMap<UINT,UINT,UINT,UINT> mapIDsEx;
	CMap<UINT,UINT,UINT,UINT> mapUnicode;
	//init the glyph data
	for( i= 0; i< m_pCSI->nSectorNum; i++)
	{
		for(UINT j= m_pCSI->pUCBoundTable[i].dwUCStart; j<= m_pCSI->pUCBoundTable[i].dwUCEnd;j++)
		{
			while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);	
			}	
			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);
			int glyph_id = FT_Get_Char_Index (m_face, (FT_ULong) j);
			if( glyph_id>0)
			{
				InitGlyfInfo(glyph_id,j);	
				UINT nTmp;
				UINT nID = glyph_id;
				if(!mapIDs.Lookup(nID,nTmp))
				{					
					mapIDs.SetAt(nID,nID);
					vIDs.push_back(nID);
				}

				if(!mapUnicode.Lookup(j,nTmp))
				{
					mapUnicode.SetAt(j,j);
					m_lstUnicode.push_back(j);
				}
			}
		}
	}

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

	nStep = 0;
	//init the glyph data
	for( i= 0; i< m_pCSI->nExSectorNum; i++)
	{
		for(UINT j= m_pCSI->pExUCBoundTable[i].dwUCStart; j<= m_pCSI->pExUCBoundTable[i].dwUCEnd;j++)
		{
			while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);	
			}
			int glyph_id = FT_Get_Char_Index (m_face, (FT_ULong) j);			
			if( glyph_id>0)
			{
				GLYPH_BODY_PTR pGlyph = NULL;
				if(!m_mapGlyf.Lookup(glyph_id,pGlyph))
				{		
					InitGlyfInfo(glyph_id,j);			
				}
				UINT nTmp;
				UINT nID = glyph_id;
				if(!mapIDsEx.Lookup(nID,nTmp))
				{					
					mapIDsEx.SetAt(nID,nID);
					vIDsEx.push_back(nID);
				}
			}
		}
	}

	BOOL bNeedOtherTbl = NeedOtherTble();
	
	if(bNeedOtherTbl && !bTTC)
	{
		// get extra glyph id from gsub table
		if(pOTF != NULL)
		{
			UINT nCurCount = m_lstID.size();
			GetExtGlyfID(pOTF->gsub,1);
			//GetExtGlyfID(pOTF->gpos,0);
		
			for(UINT k = nCurCount; k< m_lstID.size(); k++)
			{
				vIDs.push_back(m_lstID[k]);
				if(vIDsEx.size()>0)
				{
					vIDsEx.push_back(m_lstID[k]);
				}
			}			
		}
	}

	BOOL bLoadSubGlyph = FALSE;

	if(m_pCSI->bSupportSubGlyph && m_pCSI->bVector && /*m_pCSI->bVecCmpMore && */
		m_pCSI->dZoomRate == VECTOR_BASE_ZOOM)  // if not VECTOR_BASE_ZOOM, not use sub-glyph 
	{
		UINT nCurCount = m_lstID.size();
		bLoadSubGlyph = TRUE;
		GetSubGlyfID();
		for(UINT k = nCurCount; k< m_lstID.size(); k++)
		{
			vIDs.push_back(m_lstID[k]);
			if(vIDsEx.size()>0)
			{
				vIDsEx.push_back(m_lstID[k]);
			}
		}
	}

	if(pOTF != NULL)
	{
		OTF_close(pOTF);
		pOTF = NULL;
	}

	mapUnicode.RemoveAll();

	std::sort(m_lstUnicode.begin(),m_lstUnicode.end());
	std::sort(m_lstID.begin(),m_lstID.end());
	std::sort(vIDs.begin(),vIDs.end());
	std::sort(vIDsEx.begin(),vIDsEx.end());	

	if(m_mapGlyf.GetCount() == 0)
	{		
		FT_Done_Face(m_face);	
        FT_Done_FreeType(m_library);
		Clear();
		AfxMessageBox(_T("No validate glyph!"));
		return FALSE;
	}

	//if load sub-glyph, then glyph ID can not be modified
	//if load other table,then glyph ID can not be modified
	BOOL bModifyID = (bNeedOtherTbl || bLoadSubGlyph) ? FALSE : TRUE;
	

	for( i= 0; i< nFtTblNum; i++)
	{	
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}	

		int nPtSize = vFtSize[i];
		int nShrinkSize = 0;
		int nFontType = 0;
		int nValue = 0;
		m_pGlyphTables[i].hdr.size = (BYTE)nPtSize;
		if(mapNormalSize.Lookup(nPtSize,nValue))
		{
			m_pGlyphTables[i].hdr.glyph_count = (WORD)vIDs.size();	
			m_pGlyphTables[i].pIDs = new UINT[vIDs.size()];
			for(int j = 0; j< (int)vIDs.size(); j++)
			{
				m_pGlyphTables[i].pIDs[j] = vIDs[j];
			}
		}
		else
		{
			m_pGlyphTables[i].hdr.glyph_count = (WORD)vIDsEx.size();
			m_pGlyphTables[i].pIDs = new UINT[vIDsEx.size()];
			for(int j = 0; j< (int)vIDsEx.size(); j++)
			{
				m_pGlyphTables[i].pIDs[j] = vIDsEx[j];
			}
		}		

		nShrinkSize = nPtSize;

		FONT_PROPERTY_PTR pProp = NULL;
		if(m_pCSI->pmapFTProp != NULL && m_pCSI->pmapFTProp->Lookup(nPtSize,pProp))
		{
			if(pProp != NULL)
			{
				nPtSize =  pProp->nShrinkSize;
				nFontType = pProp->nFontType;
			}
		}

		m_pGlyphTables[i].hdr.ppem_x = (BYTE)m_face->size->metrics.x_ppem;
		m_pGlyphTables[i].hdr.ppem_y = (BYTE)m_face->size->metrics.y_ppem;	
		m_pGlyphTables[i].hdr.page_offsets_base = sizeof(m_pGlyphTables[i].hdr);

		m_pGlyphTables[i].hdr.type = (BYTE)( (nFontType == 0) ? FONT_TYPE_MONO : FONT_TYPE_GRAY_RLE);
		if(m_pCSI->bVector)
		{
			// MMI will use this two values to calulate gpos
			// so set them to zero, MMI set them on real time based on point size.
			m_pGlyphTables[i].hdr.ppem_x = 0;
			m_pGlyphTables[i].hdr.ppem_y = 0;            
            m_pGlyphTables[i].hdr.type = FONT_TYPE_VECTOR_CMP;
			m_pGlyphTables[i].hdr.shrink = (BYTE)(m_pCSI->nVectorShrinkPercent*0x80/100);

			//[[test
			//m_pCSI->dZoomRate = VECTOR_MID_ZOOM;
			//]]
			if(!m_pCSI->bVecCmp)
			{
				m_pGlyphTables[i].hdr.type = FONT_TYPE_VECTOR;
			}

			m_fontHDR.units_per_em = (WORD)m_face->units_per_EM;

			DumpGlyf(m_pGlyphTables+i,i,nPtSize,FT_COLOR_VECT);
			break;
		}
		else
		{
			FT_Set_Pixel_Sizes(m_face,0,nPtSize);	
			m_pGlyphTables[i].hdr.ppem_x = (BYTE)m_face->size->metrics.x_ppem;
			m_pGlyphTables[i].hdr.ppem_y = (BYTE)m_face->size->metrics.y_ppem;	
			DumpGlyf(m_pGlyphTables+i,i,nPtSize,nFontType);
		}
	}


	FT_Done_Face(m_face);	
    FT_Done_FreeType(m_library);

	m_bReady = TRUE;

	if(m_pCSI->bVector && m_pCSI->bVecCmp)
	{
		if(m_nOutlineLongjmpNum != 0)
		{
			CString strError;
			strError.Format(_T("The point size zoomed in is two large!\nThere are %d glyphs of total %d to be overflowed!\nIf you want to use this point size font in MMI, you need modify its buffer size!"),
							m_nOutlineLongjmpNum, m_lstID.size());
			AfxMessageBox(strError);
			return FALSE;
		}

		if(m_bZoomLargeFlag)
		{
			if(m_pCSI->dZoomRate == VECTOR_BASE_ZOOM)
			{
				m_pCSI->dZoomRate = VECTOR_MID_ZOOM;
			}
			else if(m_pCSI->dZoomRate == VECTOR_MID_ZOOM)
			{
				m_pCSI->dZoomRate = VECTOR_SMALL_ZOOM;
			}
			else
			{
				m_pCSI->dZoomRate = VECTOR_BASE_ZOOM;
				return FALSE;
			}
			if(!LoadOtff(lpszFileName))
			{
				return FALSE;
			}
		}
	}

	CalcMultiUnicodeOneID();
	FixCmap(bModifyID);	
	CalcPage(bModifyID);



	if(m_nOverOneByteNum != 0)
	{
		CString strError;
		strError.Format(_T("The point size is two large!\nThere are %d glyphs whoes parameters are over one byte range!"),
						m_nOverOneByteNum);
		AfxMessageBox(strError);
//		return FALSE;
	}

	return TRUE;
}

BOOL CGlyphData::DumpCmap(OTF_cmap *cmap)
{	
	if(cmap == NULL)
		return FALSE;

	m_cmap.version[0]	= HIBYTE((WORD)(cmap->version));
	m_cmap.version[1]	= LOBYTE((WORD)(cmap->version));
	m_cmap.table_count	= (WORD)1;//cmap->numTables;

	BOOL bFlag = FALSE;
/*	
Platform ID	Platform name		Platform-specific encoding IDs	Language IDs 
0			Unicode				Various							None 
1			Macintosh			Script manager code				Various 
2			ISO [deprecated]	ISO encoding [deprecated]		None 
3			Microsoft			Microsoft encoding				Various 
4			Custom				Custom							None 

Unicode platform-specific encoding IDs (platform ID = 0) 
Encoding ID	Description 
0			Unicode 1.0 semantics 
1			Unicode 1.1 semantics 
2			ISO 10646:1993 semantics 
3			Unicode 2.0 and onwards semantics, Unicode BMP only. 
4			Unicode 2.0 and onwards semantics, Unicode full repertoire. 

Microsoft platform-specific encoding IDs (platform ID= 3) 
Encoding ID	Description 
0			Symbol 
1			Unicode BMP only 
2			ShiftJIS 
3			PRC 
4			Big5 
5			Wansung 
6			Johab 
7			Reserved 
8			Reserved 
9			Reserved 
10			Unicode full repertoire 
*/


	for (int i = 0; (UINT)i < cmap->numTables; i++)
	{
		if( cmap->EncodingRecord[i].platformID != 0 &&  // unicode
			cmap->EncodingRecord[i].platformID != 3   ) // Microsoft
		{
			continue;
		}
		
		if(cmap->EncodingRecord[i].platformID == 3)
		{
			if( cmap->EncodingRecord[i].encodingID != 1 &&  //
				cmap->EncodingRecord[i].encodingID != 10 )
			{
				continue;
			}
		}
		
		// convert to 3.1 (Microsoft.Unicode BMP only)
		m_cmap.platform_id = (WORD)3/*cmap->EncodingRecord[i].platformID*/; //platformID
		m_cmap.encoding_id = (WORD)1/*cmap->EncodingRecord[i].encodingID*/; //encodingID
		//offset is 0x0c, version + numTable + platformID + encodingID
		m_cmap.table_offset = 0x0c;
		m_cmap.format = (WORD)cmap->EncodingRecord[i].subtable.format; //format
		m_cmap.length = (WORD)cmap->EncodingRecord[i].subtable.length; //length
		m_cmap.language = (WORD)cmap->EncodingRecord[i].subtable.language;           
		
		switch (m_cmap.format)
		{
		case 4:
			{
				OTF_EncodingSubtable4 *sub4 = cmap->EncodingRecord[i].subtable.f.f4;
				int j;
				
				m_cmap.seg_countX2  = (WORD)sub4->segCountX2;                
				m_cmap.search_range = (WORD)sub4->searchRange;
				m_cmap.entry_selector = (WORD)sub4->entrySelector;
				m_cmap.range_shift = (WORD)sub4->rangeShift; 
				int nCount = sub4->segCountX2 / 2;				
		
				m_cmap.end_count_ptr = new WORD[nCount];
				m_cmap.start_count_ptr = new WORD[nCount];
				m_cmap.id_delta_ptr    = new short[nCount];
				m_cmap.id_range_offset_ptr = new WORD[nCount];
				
				for (j = 0; j < nCount; j++)
				{ 
					m_cmap.end_count_ptr[j] = (WORD)sub4->segments[j].endCount;
					m_cmap.start_count_ptr[j] = (WORD)sub4->segments[j].startCount;
					m_cmap.id_delta_ptr[j] = (short)sub4->segments[j].idDelta;
					WORD wTmp = (WORD)sub4->segments[j].idRangeOffset;
					if(wTmp == 0xFFFF)
					{
						wTmp = 0;
					}
					else
					{
						wTmp = (WORD)(wTmp + (WORD)( nCount - j ));
						wTmp *= 2;
					}
					m_cmap.id_range_offset_ptr[j] = wTmp;
//					m_cmap.id_range_offset_ptr[j] = (WORD)sub4->segments[j].idDelta;
				}         
				
//				m_cmap.length = sizeof(WORD) * 6 + nCount * 4 * sizeof(WORD);

				m_cmap.glyf_id_count = sub4->GlyphCount;
				if(m_cmap.glyf_id_count > 0)
				{
					m_cmap.glyf_id_ptr = new WORD[m_cmap.glyf_id_count];
//					m_cmap.length += m_cmap.glyf_id_count* sizeof(WORD);
				}
				
				for (j = 0; j < sub4->GlyphCount; j++)
				{  
					m_cmap.glyf_id_ptr[j] = (WORD)sub4->glyphIdArray[j];
				}

				bFlag = TRUE;				
			}
			break;
		case 12:
            {
				// not to deal with it, it will convert to format4 in FixCmap later.

//                 OTF_EncodingSubtable12 *sub12
//                     = cmap->EncodingRecord[i].subtable.f.f12;
//                 UINT j;
                
//              for (j = 0; j < sub12->nGroups; j++)
//                {
//                     IPRINT ( file_fp, "(Group (0x%X) (startChar 0x%04X) (endChar 0x%04X) (startGlyphID 0x%X))",
//                         j,
//                     sub12->Groups[j].startCharCode,
//                     sub12->Groups[j].endCharCode,
//                     sub12->Groups[j].startGlyphID);
//                }
				bFlag = TRUE;
            }
            break;
		}
	}

	if(!bFlag)
	{
		AfxMessageBox(_T("It is not Windows Unicode encodings!"));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CGlyphData::DumpGdef(OTF_GDEF *gdef,FILE * pFile)
{
	UNREFERENCED_PARAMETER(gdef);
	if(m_gdef.offset == 0 || m_gdef.size == 0)
	{
		return TRUE;
	}
	m_gdef.data_ptr = new BYTE[m_gdef.size];
	fseek(pFile,m_gdef.offset,SEEK_SET);
	fread(m_gdef.data_ptr,m_gdef.size,1,pFile);

	return TRUE;

}

BOOL CGlyphData::DumpGsub(OTF_GSUB *gsub,FILE * pFile)
{
	UNREFERENCED_PARAMETER(gsub);

	if(m_gsub.offset == 0 || m_gsub.size == 0)
	{
		return TRUE;
	}
	m_gsub.data_ptr = new BYTE[m_gsub.size];
	fseek(pFile,m_gsub.offset,SEEK_SET);
	fread(m_gsub.data_ptr,m_gsub.size,1,pFile);

	return TRUE;
}

BOOL CGlyphData::DumpGpos(OTF_GPOS *gpos,FILE * pFile)
{
	UNREFERENCED_PARAMETER(gpos);

	if(m_gpos.offset == 0 || m_gpos.size == 0)
	{
		return TRUE;
	}
	m_gpos.data_ptr = new BYTE[m_gpos.size];
	fseek(pFile,m_gpos.offset,SEEK_SET);
	fread(m_gpos.data_ptr,m_gpos.size,1,pFile);

	return TRUE;
}


BOOL CGlyphData::DumpGlyf(GLYPH_TABLE_T *glyftbl,int nIndex,int nPtSize,int nColorType /*= FT_COLOR_MONO*/)
{	

	UNREFERENCED_PARAMETER(nColorType);
	UNREFERENCED_PARAMETER(nPtSize);

	int nMaxY = 0;
	int nMaxH = -1000;

	MSG msg;
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,glyftbl->hdr.glyph_count);
	int nStep = 0;
	memset(&m_tMetrics,0,sizeof(m_tMetrics));
	int nSumSize = 0;
	int nSumSize0 = 0;

	int i=0;

	for(i=0;i<glyftbl->hdr.glyph_count; i++)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);

		int nID = glyftbl->pIDs[i];
		GLYPH_BODY_PTR pGlyf = NULL;
		m_mapGlyf.Lookup(nID,pGlyf);
		GLYPH_BODY_PTR pCurGlyf = pGlyf+nIndex;

		int glyph_size = 0;

		XGLYPH_METRICS_T metrics ={0};

		if(FT_COLOR_MONO == nColorType)
		{
			LoadMonoGlyph(pCurGlyf,nID);
			FillMetrics(metrics,pCurGlyf);

			glyph_size = (pCurGlyf->width+7)/8 * pCurGlyf->height;
			
		}
		else if(FT_COLOR_ARGB == nColorType)
		{
			LoadArgbGlyph(pCurGlyf,nID);
			FillMetrics(metrics,pCurGlyf);
			if(pCurGlyf->bitmap != NULL)
			{
				glyph_size = pCurGlyf->width * pCurGlyf->height;
			}
		}
		else if(FT_COLOR_VECT == nColorType)
		{
			LoadVectorGlyph(pCurGlyf,nID,glyftbl->hdr.size);
			memcpy(&metrics,&pCurGlyf->metrics,sizeof(metrics));
			if(pCurGlyf->bitmap != NULL)
			{
				glyph_size = pCurGlyf->width * pCurGlyf->height;
			}
		}
		else
		{
			_ASSERTE(0);
		}
	

		if(((char)metrics.y) > nMaxY)
		{
			nMaxY = metrics.y;
		}

		if( (metrics.height - (metrics.y)*2) > nMaxH)
		{
			nMaxH = (metrics.height - (metrics.y)*2);
		}

		//test
		nSumSize += glyph_size;

		//test
		nSumSize0 += ((metrics.height * metrics.width)/2);

		//ASSERT(pCurGlyf->dwidth<=0xFF);

		m_tMetrics[(BYTE)(metrics.dwidth)].dwidth_sum ++;
		m_tMetrics[(BYTE)(metrics.x)].x_sum ++;
		m_tMetrics[(BYTE)(metrics.y)].y_sum ++;
		m_tMetrics[(BYTE)(metrics.width)].width_sum ++;
		m_tMetrics[(BYTE)(metrics.height)].height_sum ++;
	}

//	int nFontSize = nPtSize;

	int  nFontSize = glyftbl->hdr.size;
	if(FT_COLOR_VECT == nColorType)
	{
		nFontSize = VECTOR_POINT_SIZE;
	}	

	if(nMaxY<=0 || nMaxY>nFontSize)
	{
		nMaxY = nFontSize - m_pCSI->nVectorMaxBearingYDelta;
	}
	else
	{
		nMaxY -= m_pCSI->nVectorMaxBearingYDelta;
	}

	if(nMaxY < 0)
	{
		nMaxY = 0;
	}
	

	nMaxH += nMaxY;

	if(nMaxH<nFontSize)
	{
		nMaxH = nFontSize;
	}
	else if(nMaxH>(int)(nFontSize*1.2))
	{
		nMaxH = (int)(nFontSize*1.2);
	}

	glyftbl->hdr.max_bearing_y = (char)nMaxY;
	glyftbl->hdr.max_height = (BYTE)nMaxH;



//	CString strTmp;
//	strTmp.Format(_T("%d,%d,%.2f,%d"),nSumSize,nSumSize0,(float)nSumSize/(float)nSumSize0,m_lstID.size());
//	AfxMessageBox(strTmp);
	TEST_METRICS_T tMetrics;
	memset(&tMetrics,0,sizeof(tMetrics));

    //取出现次数最多的num做成默认的, 进行裁剪
    for ( i = 0; i < TEST_METRICS_MAX; i++)
    {
        if ( m_tMetrics[i].dwidth_sum > tMetrics.dwidth_sum )
        {
            tMetrics.dwidth_sum = m_tMetrics[i].dwidth_sum;   
			glyftbl->hdr.advance = (BYTE)i;
        }
		if ( m_tMetrics[i].x_sum > tMetrics.x_sum )
        {
            tMetrics.x_sum = m_tMetrics[i].x_sum;   
			glyftbl->hdr.bearing_x = (BYTE)i;
        }
		if ( m_tMetrics[i].y_sum > tMetrics.y_sum )
        {
            tMetrics.y_sum = m_tMetrics[i].y_sum;   
			glyftbl->hdr.bearing_y = (BYTE)i;
        }
		if ( m_tMetrics[i].width_sum > tMetrics.width_sum )
        {
            tMetrics.width_sum = m_tMetrics[i].width_sum;   
			glyftbl->hdr.bitmap_w = (BYTE)i;
        }
		if ( m_tMetrics[i].height_sum > tMetrics.height_sum )
        {
            tMetrics.height_sum = m_tMetrics[i].height_sum;   
			glyftbl->hdr.bitmap_h = (BYTE)i;
        }
    }

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

	return TRUE;
}
void CGlyphData::_CalcPage(std::vector<GLYPH_PAGE_PTR> &vctPage, std::vector<unsigned int> * pLstID, MAP_GLYPH & mapGlyph, int nFontTblIndex)
{	
	for(UINT i = 0; i< vctPage.size(); i++)
	{
		delete vctPage[i];
		vctPage[i] = NULL;
	}
	vctPage.clear();

	std::vector<unsigned int> vctIDs;
	std::vector<unsigned int>::iterator it;
	GLYPH_BODY_PTR pGlyf = NULL;
	GLYPH_BODY_PTR pCurGlyf = NULL;
	for(it = pLstID->begin(); it!= pLstID->end(); it++)
	{
		int nID = *it;
		mapGlyph.Lookup(nID,pGlyf);
		pCurGlyf = pGlyf+nFontTblIndex;
		if(pCurGlyf->initialized)
		{
			vctIDs.push_back(nID);
		}
	}
	
	int nBegin = -1;
	int nEnd = -1;
	for(it = vctIDs.begin(); it!= vctIDs.end(); it++)
	{
		int nID = *it;
		if(nBegin==-1)
		{
			nBegin = nID;
			nEnd = nID;
		}
		else
		{
			if((nID-nEnd)!=1)
			{
				GLYPH_PAGE_PTR pPage = new GLYPH_PAGE_T;
				pPage->start = (WORD)nBegin;
				pPage->end = (WORD)nEnd;
				pPage->offset = 0;
				vctPage.push_back(pPage);
				nBegin = nID;
				nEnd   = nID;
			}
			else
			{
				nEnd = nID;
			}
		}
	}

	GLYPH_PAGE_PTR pPage = new GLYPH_PAGE_T;
	pPage->start = (WORD)nBegin;
	pPage->end = (WORD)nEnd;
	pPage->offset = 0;
	vctPage.push_back(pPage);
}
void CGlyphData::CalcPage(BOOL bModifyID)
{
	CMap<UINT,UINT,UINT,UINT> mapMdfID;
	if(bModifyID)
	{
		for(UINT i = 0; i< m_lstID.size(); i++)
		{
			mapMdfID.SetAt(m_lstID[i],i+1);
		}		
	}



	int i = 0;
	int j = 0;

	for( i = 0 ; i< m_fontHDR.glyph_table_count; i++)
	{	
		std::vector<GLYPH_PAGE_PTR> vctPage;
		int nIDCount = m_pGlyphTables[i].hdr.glyph_count;
		UINT nBegin = 0;
		UINT nEnd =0;
		for(j=0; j< nIDCount; j++)
		{
			UINT nID = m_pGlyphTables[i].pIDs[j];
			if(bModifyID)
			{
				UINT nTmp;
				mapMdfID.Lookup(nID,nTmp);
				nID = nTmp;
			}
			if(nBegin==0)
			{
				nBegin = nID;
				nEnd = nID;
			}
			else
			{
				if((nID-nEnd)!=1)
				{
					GLYPH_PAGE_PTR pPage = new GLYPH_PAGE_T;
					pPage->start = (WORD)nBegin;
					pPage->end = (WORD)nEnd;
					pPage->offset = 0;
					vctPage.push_back(pPage);
					nBegin = nID;
					nEnd   = nID;
				}
				else
				{
					nEnd = nID;
				}
			}
		}

		GLYPH_PAGE_PTR pPage = new GLYPH_PAGE_T;
		pPage->start = (WORD)nBegin;
		pPage->end = (WORD)nEnd;
		pPage->offset = 0;
		vctPage.push_back(pPage);

		int nPageCount = vctPage.size();

		m_pGlyphTables[i].page_ptr = new GLYPH_PAGE_T[nPageCount];
		memset(m_pGlyphTables[i].page_ptr,0,sizeof(GLYPH_PAGE_T)*nPageCount);
		m_pGlyphTables[i].page_count = (WORD)nPageCount;
		for(j = 0; j< nPageCount;j++)
		{
			m_pGlyphTables[i].page_ptr[j].start = vctPage[j]->start;
			m_pGlyphTables[i].page_ptr[j].end = vctPage[j]->end;
			if(j>0)
			{
				m_pGlyphTables[i].page_ptr[j].offset = (WORD)(m_pGlyphTables[i].page_ptr[j-1].offset+
					                                   m_pGlyphTables[i].page_ptr[j-1].end - 
													   m_pGlyphTables[i].page_ptr[j-1].start + 1);
			}
			
			delete vctPage[j];			
		}
		vctPage.clear();
	}

	mapMdfID.RemoveAll();
}

DWORD CGlyphData::Align4(FILE *pFile)
{
	static BYTE padd[4]={0};

	int nPadd = ftell(pFile);
	nPadd = ( 4 - nPadd%4 ) % 4;
	if(nPadd>0)
	{
		fwrite(padd,1,nPadd,pFile);
	}

	return nPadd;
}

GLYPH_BODY_PTR CGlyphData::GetGlyph(UINT uCode,int nPtIndex,BOOL bUnicode /*= TRUE*/)
{
	if(m_pCSI == NULL || nPtIndex >= m_fontHDR.glyph_table_count)
	{
		return NULL;
	}

	GLYPH_BODY_PTR pGlyf= NULL;
	DWORD dwID;
	
	if(bUnicode)
	{
		if(m_mapUnid.Lookup(uCode,dwID))
		{
			if(IsIDInTable(m_pGlyphTables+nPtIndex,dwID))
			{
				if(m_mapGlyf.Lookup(dwID,pGlyf))
				{
					return (pGlyf + nPtIndex);
				}
			}
		}
	}
	else
	{
		if(IsIDInTable(m_pGlyphTables+nPtIndex,uCode))
		{
			if(m_mapGlyf.Lookup(uCode,pGlyf))
			{
				return (pGlyf + nPtIndex);
			}
		}
	}

	return pGlyf;
}

BOOL CGlyphData::FixGlyfFlag(GLYPH_BODY_PTR pGlyph,GLYPH_TABLE_PTR pGlyfTbl)
{
	if(m_pCSI == NULL || pGlyph == NULL || pGlyph == NULL)
	{
		return FALSE;
	}

	WORD flag = pGlyph->flag;
	BOOL bChange = FALSE;
	BOOL bVector = (pGlyfTbl->hdr.type == FONT_TYPE_VECTOR_CMP || pGlyfTbl->hdr.type == FONT_TYPE_VECTOR)?TRUE : FALSE;
    if(!bVector)
    {
	    if(pGlyph->dwidth == pGlyfTbl->hdr.advance/* && !bVector*/ )
	    {
		    pGlyph->flag &= ~FLAG_ADVANCE;	
	    }
		else
		{
			pGlyph->flag |= FLAG_ADVANCE;	
		}
    }
    else
    {
	    if(pGlyph->metrics.dwidth == pGlyfTbl->hdr.advance )
	    {
		    pGlyph->flag &= ~FLAG_ADVANCE;
	    }
		else
		{
			pGlyph->flag |= FLAG_ADVANCE;
		}

        if ( m_pCSI->bVecCmpMore )
        {
			pGlyph->flag |= FLAG_CMP_EX;	
        }

		if(pGlyph->nNumberSubGlyph > 0)
		{
			pGlyph->flag |= FLAG_COMPOSE_GLYPH;
		}
    }

	if(pGlyph->x == pGlyfTbl->hdr.bearing_x || bVector)
	{
		pGlyph->flag &= ~FLAG_BEARING_X;
	}
	else
	{
		pGlyph->flag |= FLAG_BEARING_X;
	}

	if(pGlyph->y == pGlyfTbl->hdr.bearing_y|| bVector)
	{
		pGlyph->flag &= ~FLAG_BEARING_Y;
	}
	else
	{
		pGlyph->flag |= FLAG_BEARING_Y;
	}

	if(pGlyph->width == pGlyfTbl->hdr.bitmap_w|| bVector)
	{
		pGlyph->flag &= ~FLAG_BITMAP_WIDTH;
	}
	else
	{
		pGlyph->flag |= FLAG_BITMAP_WIDTH;
	}

	if(pGlyph->height == pGlyfTbl->hdr.bitmap_h|| bVector)
	{
		pGlyph->flag &= ~FLAG_BITMAP_HEIGHT;
	}
	else
	{
		pGlyph->flag |= FLAG_BITMAP_HEIGHT;
	}

	if(pGlyph->flag != flag)
	{
		bChange =TRUE;
	}

	return bChange;
}

BOOL CGlyphData::NeedOtherTble()
{
	if(m_pCSI== NULL)
	{
		return TRUE;
	}

	_TCHAR szTag[5] = {0};
	szTag[0] = m_pCSI->tag[0];
	szTag[1] = m_pCSI->tag[1];
	szTag[2] = m_pCSI->tag[2];
	szTag[3] = m_pCSI->tag[3];

	CString strTag = szTag;

	if(strTag.CompareNoCase(_T("latn")) == 0 ||
		strTag.CompareNoCase(_T("hani")) == 0 )
	{
//         if(m_pCSI->bVector && m_pCSI->bVecCmpMore)
//         {
//             return TRUE;
//         }
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void CGlyphData::FixCmap(BOOL bModifyID,CUIntArray *pUnicode /*= NULL*/)
{
	CUIntArray agFS;

	int i = 0;
	DWORD dwGlyphID = 0;	
	
	CUIntArray agUid;

	// get real existed unicode of glyph
	if(pUnicode == NULL)
	{
		for(i=0;i<m_pCSI->nSectorNum;i++)
		{
			FT_SECTOR fs;		
			fs.start_count = (WORD)m_pCSI->pUCBoundTable[i].dwUCStart;
			fs.end_count = (WORD)m_pCSI->pUCBoundTable[i].dwUCEnd;	
			for(DWORD j = fs.start_count; j<=fs.end_count;j++)
			{
				if(m_mapUnid.Lookup(j,dwGlyphID))
				{
					agUid.Add(j);					
				}	
			}
		}
	}
	else
	{
		agUid.Copy(*pUnicode);
	}
	MAP_NUM mapGlyfID;
	if(bModifyID)
	{
		std::vector<unsigned int>::iterator it;
		UINT nIdx=1;
		for(it = m_lstID.begin(); it != m_lstID.end(); it++)
		{
			mapGlyfID.SetAt(*it,nIdx++);
		}
	}
	
	if(agUid.GetSize() == 0)
		return;

	// reset unicode sectors
	ResetUniSector(agUid,agFS,bModifyID,mapGlyfID);


	int nCount = agFS.GetSize();
	m_cmap.seg_countX2 = (WORD)(nCount*2);

	SAFE_FREE_ARRAY(m_cmap.start_count_ptr);
	SAFE_FREE_ARRAY(m_cmap.end_count_ptr);
	SAFE_FREE_ARRAY(m_cmap.id_delta_ptr);
	SAFE_FREE_ARRAY(m_cmap.id_range_offset_ptr);

	m_cmap.start_count_ptr = new WORD[nCount];
	m_cmap.end_count_ptr = new WORD[nCount];
	m_cmap.id_delta_ptr = new short[nCount];
	m_cmap.id_range_offset_ptr = new WORD[nCount];

	CUIntArray agGlyfID;

	UINT j = 0;

	//if exist repeated glyph ID, reset id_range_offset
	for(i = 0; i<nCount; i++)
	{
		FT_SECTOR * pfs = (FT_SECTOR * )agFS[i];
		m_cmap.start_count_ptr[i] = pfs->start_count;
		m_cmap.end_count_ptr[i] = pfs->end_count;
		m_cmap.id_delta_ptr[i] = pfs->id_delta;
		m_cmap.id_range_offset_ptr[i] = pfs->id_range_offset;


		if(IsIDRepeated(pfs) || !IsIDSequence(pfs,bModifyID,mapGlyfID))//exist repeated glyph ID
		{
			m_cmap.id_delta_ptr[i] = 0;

			m_cmap.id_range_offset_ptr[i] = (WORD)((agGlyfID.GetSize() + nCount-i )*2);

			if(!bModifyID)
			{
				for(j = pfs->start_count; j <= pfs->end_count;j++)
				{
					m_mapUnid.Lookup(j,dwGlyphID);
					agGlyfID.Add(dwGlyphID);				
				}
			}
			else
			{
				for(j = pfs->start_count; j <= pfs->end_count;j++)
				{
					VERIFY(m_mapUnid.Lookup(j,dwGlyphID));
					UINT glyf_id = 0;
					VERIFY(mapGlyfID.Lookup(dwGlyphID,glyf_id));
					agGlyfID.Add(glyf_id);				
				}
			}
		}

		delete pfs;
	}


	SAFE_FREE_ARRAY(m_cmap.glyf_id_ptr);
	m_cmap.glyf_id_count = agGlyfID.GetSize();
	if(m_cmap.glyf_id_count != 0)
	{
		m_cmap.glyf_id_ptr = new WORD[m_cmap.glyf_id_count];
		for( i= 0 ;(DWORD)(i) < m_cmap.glyf_id_count;i++)
		{
			m_cmap.glyf_id_ptr[i] = (WORD)agGlyfID[i];
		}
	}


/*

	FILE *pFile = _tfopen(_T("F:\\after.txt"),_T("wt"));
	MAP_UN_ID  mapUnid; //unicode -> id
	std::list<int> lstID;
	MAP_ID_UN  mapIdun;

	for(i=0;i<nCount;i++)
	{
		for(UINT j= m_cmap.start_count_ptr[i]; j<= m_cmap.end_count_ptr[i]; j++)
		{
			short nGlyfID = 0;
			if(m_cmap.id_range_offset_ptr[i] == 0)
			{
				nGlyfID = (short)(j + (short)m_cmap.id_delta_ptr[i]);
			}
			else
			{
				if(m_cmap.glyf_id_count == 0)
					break;

				int nIndex = m_cmap.id_range_offset_ptr[i]/2+ (j- m_cmap.start_count_ptr[i]) - nCount;
				nGlyfID = m_cmap.glyf_id_ptr[nIndex];
			}
			if(nGlyfID > 0)
			{
				mapUnid.SetAt(j,nGlyfID);


				UnicodeArray_ptr pUA= NULL;
				if(mapIdun.Lookup(nGlyfID,pUA))
				{
					pUA->Add(j);
				}
				else
				{
					pUA = new  CUIntArray;
					pUA->Add(j);
					mapIdun.SetAt(nGlyfID,pUA);

					//the IDS in m_lstID can not be repeated.
					lstID.push_back(nGlyfID);

					_ftprintf(pFile,_T("%d\r\n"),nGlyfID);
				}				
			}
		}
	}

	fclose(pFile);

	pFile = _tfopen(_T("F:\\before.txt"),_T("wt"));

	std::list<int>::iterator it1;
	for(it1 = m_lstID.begin(); it1 != m_lstID.end();it1++)
	{
		_ftprintf(pFile,_T("%d\r\n"),(*it1));
	}

	fclose(pFile);

*/


//	Type	Name						Description 
// 	USHORT	format						Format number is set to 4.  
// 	USHORT	length						This is the length in bytes of the subtable.  
// 
// 	USHORT	language					Please see "Note on the language field in 'cmap' subtables" in this document. 
// 	USHORT	segCountX2					2 x segCount. 
// 	USHORT	searchRange					2 x (2**floor(log2(segCount))) 
// 	USHORT	entrySelector				log2(searchRange/2) 
// 	USHORT	rangeShift					2 x segCount - searchRange 
// 	USHORT	endCount[segCount]			End characterCode for each segment, last=0xFFFF. 
// 	USHORT	reservedPad					Set to 0. 
// 	USHORT	startCount[segCount]		Start character code for each segment. 
// 	SHORT	idDelta[segCount]			Delta for all character codes in segment. 
// 	USHORT	idRangeOffset[segCount]		Offsets into glyphIdArray or 0 
// 	USHORT	glyphIdArray[ ]				Glyph index array (arbitrary length) 	
	
	m_cmap.format = 4;
	m_cmap.length = (WORD)(m_cmap.seg_countX2 * 4/*end,start,idDelta,idRangeOffset*/+ 
		            sizeof(WORD)*7);//format,length,language,segCountX2,
	                               //searchRange,entrySelector,rangeShift,reservedPad
	if(m_cmap.glyf_id_count!=0)
	{
		m_cmap.length = (WORD)(m_cmap.length  + (WORD)(m_cmap.glyf_id_count*sizeof(WORD)));
	}
					

	m_cmap.encoding_id = 1;
	m_cmap.platform_id = 3;
	m_cmap.table_count = 1;
	m_cmap.version[0] = 0;
	m_cmap.version[1] = 0;
	m_cmap.table_offset = 0x0c;	
	m_cmap.language =0;
	m_cmap.search_range = (WORD) (2 * pow(2.0,floor(log(nCount))));
	m_cmap.entry_selector = (WORD)log(m_cmap.search_range/2);
	m_cmap.range_shift = (WORD)(2 * nCount - m_cmap.search_range);
	
	mapGlyfID.RemoveAll();

}

int  CGlyphData::GetSectorIndex(WORD wUnicode)
{
	int nCount = m_cmap.seg_countX2/2;

	for(int i=0;i<nCount; i++)
	{
		if(m_cmap.end_count_ptr[i]>=wUnicode)
		{
			return i;
		}
	}

	return -1;
}

BOOL CGlyphData::LoadFlib(LPCTSTR lpszFileName)
{
	if(lpszFileName == NULL)
		return FALSE;

	CWaitCursor wait;

	BOOL bBigEndian = TRUE;

	FILE * pFile = _tfopen(lpszFileName,_T("rb"));
	fseek(pFile,0,SEEK_END);
	DWORD dwFileSize = ftell(pFile);

	if(dwFileSize <= sizeof(m_fontHDR))
	{
		AfxMessageBox(_T("File size is too small."));
		fclose(pFile);
		return FALSE;
	}

	Clear();

	fseek(pFile,0,SEEK_SET);

	fread(&m_fontHDR,sizeof(m_fontHDR)-sizeof(DWORD),1,pFile);
	if(m_fontHDR.magic[0]=='n' &&
		m_fontHDR.magic[1]=='o' &&
		m_fontHDR.magic[2]=='f' &&
		m_fontHDR.magic[3]=='s' )
	{
		bBigEndian = FALSE;
	}
	else if(m_fontHDR.magic[0]!='s' ||
		m_fontHDR.magic[1]!='f' ||
		m_fontHDR.magic[2]!='o' ||
		m_fontHDR.magic[3]!='n' )
	{
		AfxMessageBox(_T("File is not font lib."));
		fclose(pFile);
		Clear();
		return FALSE;
	}
    

	if(m_fontHDR.fver[0] != 0 ||
		m_fontHDR.fver[1] != 0 ||
		m_fontHDR.fver[2] != 0 ||
		m_fontHDR.fver[3] != 1)
	{
		AfxMessageBox(_T("Font lib version is unknown."));
		fclose(pFile);
		Clear();
		return FALSE;
	}

	m_bBigEndian = bBigEndian;
	
	ConvFontHdr(m_fontHDR);
	
	if(m_fontHDR.glyph_table_count == 0)
	{
		AfxMessageBox(_T("There is no glyph table in file."));
		fclose(pFile);
		Clear();
		return FALSE;
	}

	m_fontHDR.gtbl_offset_ptr = new DWORD[m_fontHDR.glyph_table_count];

	fread(m_fontHDR.gtbl_offset_ptr,sizeof(DWORD)*m_fontHDR.glyph_table_count,1,pFile);

	int i=0;

	for(i=0;i<m_fontHDR.glyph_table_count;i++)
	{
		m_fontHDR.gtbl_offset_ptr[i] = Conv32Endian(m_fontHDR.gtbl_offset_ptr[i]);
	}

	if(!ReadCmapTbl(pFile))
	{
		fclose(pFile);
		Clear();
		return FALSE;
	}

	if(!ReadGdefTbl(pFile))
	{
		fclose(pFile);
		Clear();
		return FALSE;
	}
	if(!ReadGsubTbl(pFile))
	{
		fclose(pFile);
		Clear();
		return FALSE;
	}
	if(!ReadGposTbl(pFile))
	{
		fclose(pFile);
		Clear();
		return FALSE;
	}

	int nTblCount= m_fontHDR.glyph_table_count;
	m_pGlyphTables = new GLYPH_TABLE_T[nTblCount];
	memset(m_pGlyphTables,0,sizeof(GLYPH_TABLE_T)*nTblCount);

	if(!ReadGlyfTbl(pFile))
	{	
		fclose(pFile);
		Clear();
		return FALSE;
	}
	fclose(pFile);



	if(m_pGlyphTables != NULL && 
		(m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR_CMP  || m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR ))
	{
		m_pGlyphTables[0].hdr.size = VECTOR_DISP_POINT_SIZE;
	}

	FixCSI();	

	m_bReady = TRUE;
  
    DumpOutlineLog(lpszFileName,TRUE);
  
	m_bLoadLib = TRUE;

	return TRUE;
}

BOOL  CGlyphData::ReadCmapTbl(FILE *pFile)
{
	if(m_fontHDR.cmap_offset != (DWORD)ftell(pFile))
	{
		AfxMessageBox(_T("CMAP offset is wrong."));
		return FALSE;
	}

	fread(&m_cmap,(DWORD)&m_cmap.padd - (DWORD)&m_cmap, 1, pFile);
	ConvCmapDat(m_cmap);

	int nCount = m_cmap.seg_countX2 /2;
    m_cmap.end_count_ptr = new WORD[nCount];
	m_cmap.start_count_ptr = new WORD[nCount];
	m_cmap.id_delta_ptr = new short[nCount];
	m_cmap.id_range_offset_ptr = new WORD[nCount];

	fread(m_cmap.end_count_ptr,nCount * sizeof(WORD),1,pFile);
	WORD wPadd;
	fread(&wPadd,sizeof(WORD),1,pFile);

	fread(m_cmap.start_count_ptr,nCount * sizeof(WORD),1,pFile);
	fread(m_cmap.id_delta_ptr,nCount * sizeof(short),1,pFile);
	fread(m_cmap.id_range_offset_ptr,nCount * sizeof(WORD),1,pFile);

	int i =0;
// 	for(i=0;i<nCount;i++)
// 	{
// 		m_cmap.end_count_ptr[i] = Conv16Endian(m_cmap.end_count_ptr[i]);
// 		m_cmap.start_count_ptr[i] = Conv16Endian(m_cmap.start_count_ptr[i]);
// 		m_cmap.id_delta_ptr[i] = Conv16Endian(m_cmap.id_delta_ptr[i]);
// 		m_cmap.id_range_offset_ptr[i] = Conv16Endian(m_cmap.id_range_offset_ptr[i]);
// 
// 	}

	DWORD dwCurOffset = ftell(pFile);

	DWORD *pNextOffset = &m_fontHDR.gdef_offset;
	for( i = 0; i< 4; i++)
	{
		if((*(pNextOffset)) != 0) //lint !e662 !e661
			break;
		pNextOffset++;
	}
	if(i>=4)
	{
		pNextOffset = &m_fontHDR.gtbl_offset_ptr[0];
	}

	if((*pNextOffset) != 0)
	{
		m_cmap.glyf_id_count = ((*pNextOffset)- dwCurOffset)/2;
		if(m_cmap.glyf_id_count != 0)
		{
			m_cmap.glyf_id_ptr = new WORD[	m_cmap.glyf_id_count ];
			fread(m_cmap.glyf_id_ptr,m_cmap.glyf_id_count*sizeof(WORD),1,pFile);

			for(i=0;(DWORD)(i)<m_cmap.glyf_id_count;i++)
			{
				m_cmap.glyf_id_ptr[i] = Conv16Endian(m_cmap.glyf_id_ptr[i]);
			}
		}
	}

	for(i=0;i<nCount;i++)
	{
		m_cmap.end_count_ptr[i] = Conv16Endian(m_cmap.end_count_ptr[i]);
		m_cmap.start_count_ptr[i] = Conv16Endian(m_cmap.start_count_ptr[i]);
		m_cmap.id_delta_ptr[i] = Conv16Endian(m_cmap.id_delta_ptr[i]);
		m_cmap.id_range_offset_ptr[i] = Conv16Endian(m_cmap.id_range_offset_ptr[i]);

		for(UINT j= m_cmap.start_count_ptr[i]; j<= m_cmap.end_count_ptr[i]; j++)
		{
			WORD nGlyfID = 0;
			if(m_cmap.id_range_offset_ptr[i] == 0)
			{
				nGlyfID = (WORD)(j + (short)m_cmap.id_delta_ptr[i]);
			}
			else
			{
				if(m_cmap.glyf_id_count == 0)
					return FALSE;

				int nIndex = m_cmap.id_range_offset_ptr[i]/2 + (j- m_cmap.start_count_ptr[i]) - (nCount-i);
				nGlyfID = m_cmap.glyf_id_ptr[nIndex];
			}
			if(nGlyfID > 0)
			{
				m_lstUnicode.push_back(j);
				m_mapUnid.SetAt(j,nGlyfID);


				UnicodeArray_ptr pUA= NULL;
				if(m_mapIdun.Lookup(nGlyfID,pUA))
				{
					pUA->Add(j);
				}
				else
				{
					pUA = new  CUIntArray;
					pUA->Add(j);
					m_mapIdun.SetAt(nGlyfID,pUA);

					//the IDS in m_lstID can not be repeated.
					m_lstID.push_back(nGlyfID);
				}				
			}
		}
	}

	return TRUE;
}

BOOL  CGlyphData::ReadGdefTbl(FILE *pFile)
{
	if(m_fontHDR.gdef_offset == 0)
	{
		return TRUE;
	}

	fseek(pFile,m_fontHDR.gdef_offset,SEEK_SET);
	
	DWORD *pNextOffset = &m_fontHDR.gsub_offset;
	int i=0;
	for( i = 0; i< 2; i++)
	{
		if((*(pNextOffset+i)) != 0) //lint !e661
			break;
	}
	if(i>=2)
	{
		pNextOffset = &m_fontHDR.gtbl_offset_ptr[0];
	}

	if((*pNextOffset) != 0)
	{
		m_gdef.size = ((*pNextOffset)- m_fontHDR.gdef_offset);
		if(m_gdef.size != 0)
		{
			m_gdef.data_ptr = new BYTE[m_gdef.size];
			fread(m_gdef.data_ptr,m_gdef.size,1,pFile);
		}
	}

	return TRUE;

}

BOOL  CGlyphData::ReadGsubTbl(FILE *pFile)
{
	if(m_fontHDR.gsub_offset == 0)
	{
		return TRUE;
	}

	fseek(pFile,m_fontHDR.gsub_offset,SEEK_SET);
	
	DWORD *pNextOffset = &m_fontHDR.gpos_offset;
//	int i=0;
	if((*pNextOffset)!= 0)
	{
		pNextOffset = &m_fontHDR.gtbl_offset_ptr[0];
	}
	if((*pNextOffset) != 0)
	{
		m_gsub.size = ((*pNextOffset)- m_fontHDR.gsub_offset);
		if(m_gsub.size != 0)
		{
			m_gsub.data_ptr = new BYTE[m_gsub.size];
			fread(m_gsub.data_ptr,m_gsub.size,1,pFile);
		}
	}
	return TRUE;
}

BOOL  CGlyphData::ReadGposTbl(FILE *pFile)
{
	if(m_fontHDR.gpos_offset == 0)
	{
		return TRUE;
	}

	fseek(pFile,m_fontHDR.gdef_offset,SEEK_SET);
	
	DWORD *pNextOffset = &m_fontHDR.gsub_offset;
//	int i=0;
	pNextOffset = &m_fontHDR.gtbl_offset_ptr[0];

	if((*pNextOffset) != 0)
	{
		m_gpos.size = ((*pNextOffset)- m_fontHDR.gpos_offset);
		if(m_gpos.size != 0)
		{
			m_gpos.data_ptr = new BYTE[m_gpos.size];
			fread(m_gpos.data_ptr,m_gpos.size,1,pFile);
		}
	}

	return TRUE;
}

BOOL  CGlyphData::ReadGlyfTbl(FILE *pFile)
{
	int nTblCount = m_fontHDR.glyph_table_count;

    BYTE is_cmp_ex = FALSE;
	MSG msg;	
	int i = 0;
	int j = 0;
	CUIntArray agID;
	for( i=0; i< nTblCount; i++)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}

		//int nIdx = pFtObj[i].nIdx;
		GLYPH_TABLE_PTR pGlyfTbl = m_pGlyphTables+i;

		pGlyfTbl->Init();

		fseek(pFile,m_fontHDR.gtbl_offset_ptr[i],SEEK_SET);
		fread(&(pGlyfTbl->hdr),sizeof(pGlyfTbl->hdr),1,pFile);
		fread(&(pGlyfTbl->page_count), sizeof(pGlyfTbl->page_count),1,pFile);	
		ConvGlyfTbl(pGlyfTbl);
		

// 		if(pGlyfTbl->hdr.type == FONT_TYPE_VECTOR_CMP)
// 		{
// 			//_ASSERTE(pGlyfTbl->hdr.size == VECTOR_POINT_SIZE);
// 			pGlyfTbl->hdr.size = VECTOR_DISP_POINT_SIZE;
// 		}

		int nPageCount = pGlyfTbl->page_count;

		pGlyfTbl->page_ptr = new GLYPH_PAGE_T[nPageCount];
		WORD *pBufEnd = new WORD[nPageCount];
		WORD *pBufStart = new WORD[nPageCount];
		WORD *pBufOffset = new WORD[nPageCount];
		fread(pBufEnd,sizeof(WORD)*nPageCount, 1,pFile);
		fread(pBufStart,sizeof(WORD)*nPageCount, 1,pFile);
		fread(pBufOffset,sizeof(WORD)*nPageCount, 1,pFile);

// 		int nCurFtTblNum = nFtSizeNum;
// 		if( pFtObj[i].bExtra)
// 		{
// 			nCurFtTblNum = nTblGount - nFtSizeNum;			
// 		}
		agID.RemoveAll();

		pGlyfTbl->pIDs = new UINT[pGlyfTbl->hdr.glyph_count];
		memset(pGlyfTbl->pIDs,0,sizeof(UINT)*pGlyfTbl->hdr.glyph_count);

		int nIDCount = 0;

		UnicodeArray_ptr pUA = NULL;

		for(j=0;j<nPageCount;j++)
		{		
			pGlyfTbl->page_ptr[j].end = Conv16Endian(pBufEnd[j]);
			pGlyfTbl->page_ptr[j].start = Conv16Endian(pBufStart[j]);
			pGlyfTbl->page_ptr[j].offset = Conv16Endian(pBufOffset[j]);
			

			WORD start = pGlyfTbl->page_ptr[j].start;
			WORD end = pGlyfTbl->page_ptr[j].end;
			
			for(UINT k=start; k<=end;k++)
			{
				if(nIDCount < pGlyfTbl->hdr.glyph_count)
				{
					pGlyfTbl->pIDs[nIDCount++] = k;
				}
				GLYPH_BODY_PTR pGlyf = NULL;
				if(!m_mapGlyf.Lookup(k,pGlyf))
				{
					pGlyf = new GLYPH_BODY_T[nTblCount];				
					for(int n = 0; n< nTblCount; n++)
					{
						pGlyf[n].Init();
					}
					m_mapGlyf.SetAt(k,pGlyf);
				}			
				agID.Add(k);
				//some ID maybe not include in m_mapIdun
				if(!m_mapIdun.Lookup(k,pUA))
				{
					m_lstID.push_back(k);
					pUA = new CUIntArray;
					pUA->Add(0);
					m_mapIdun.SetAt(k,pUA);
				}	
			}					
		}

		int nGlyfCount = pGlyfTbl->hdr.glyph_count;

		delete [] pBufEnd;
		delete [] pBufStart;
		delete [] pBufOffset;
		
		if(nGlyfCount != agID.GetSize() || ((UINT)nGlyfCount) != pGlyfTbl->hdr.glyph_count)
		{
			AfxMessageBox(_T("Glyph count in glyph table header does not match the glyph ID count!"));
			return FALSE;
		}

		fseek(pFile,sizeof(DWORD)*nGlyfCount,SEEK_CUR);


		::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,nGlyfCount);
		int nStep = 0;
		
		for(j=0;j<nGlyfCount;j++)
		{
			while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);	
			}
			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);

			GLYPH_BODY_PTR pGlyf = NULL;
			GLYPH_BODY_PTR pCurGlyf = NULL;
			m_mapGlyf.Lookup(agID[j],pGlyf);
			
			pCurGlyf = pGlyf+i;		

			BYTE   flag = 0;
			BYTE   data = 0;

			fread(&flag,1,1,pFile);

            pCurGlyf->flag = flag;

			if(flag & FLAG_ADVANCE)
			{
				fread(&data,1,1,pFile);
				pCurGlyf->dwidth = data;
			}
			else
			{
				pCurGlyf->dwidth = pGlyfTbl->hdr.advance;
			}

			if(flag & FLAG_BEARING_X)
			{
				fread(&data,1,1,pFile);
				pCurGlyf->x = (char)data;
			}
			else
			{
				pCurGlyf->x = pGlyfTbl->hdr.bearing_x;
			}
			if(flag & FLAG_BITMAP_WIDTH)
			{
				fread(&data,1,1,pFile);
				pCurGlyf->width = data;
			}
			else
			{
				pCurGlyf->width = pGlyfTbl->hdr.bitmap_w;
			}
			if(flag & FLAG_BEARING_Y)
			{
				fread(&data,1,1,pFile);
				pCurGlyf->y = (char)data;
			}
			else
			{
				pCurGlyf->y = pGlyfTbl->hdr.bearing_y;
			}
			if(flag & FLAG_BITMAP_HEIGHT)
			{
				fread(&data,1,1,pFile);
				pCurGlyf->height = data;
			}
			else
			{
				pCurGlyf->height = pGlyfTbl->hdr.bitmap_h;
			}

			if(pGlyfTbl->hdr.type == FONT_TYPE_MONO)
			{
				int nSize = ((pCurGlyf->width + 7) / 8 )* pCurGlyf->height;

				if(nSize > 0)
				{
					pCurGlyf->bitmap = new BYTE[nSize];
					fread(pCurGlyf->bitmap,nSize,1,pFile);
				}		
			}
			else if(pGlyfTbl->hdr.type == FONT_TYPE_GRAY_RLE)
			{
				int nBMSize = pCurGlyf->width * pCurGlyf->height;
				if(nBMSize > 0)
				{
					pCurGlyf->bitmap = new BYTE[nBMSize]; 
					memset(pCurGlyf->bitmap,0,nBMSize);
				
					if((flag & FLAG_SIZE) != 0)
					{
						WORD wSize = 0;
						fread(&wSize,1,sizeof(WORD),pFile);
						wSize = Conv16Endian(wSize);
						BYTE *pBuf = new BYTE[wSize];
						fread(pBuf,1,wSize,pFile);
						BYTE buf[1024]={0};
						memcpy(buf,pBuf,wSize);
						Font_Rle_Decompress(pBuf,pCurGlyf->bitmap,wSize,pCurGlyf->width,pCurGlyf->height);
						delete [] pBuf;
					}
					else
					{
						fread(pCurGlyf->bitmap,1,nBMSize,pFile);
					}
					
				}
			}
			else if(pGlyfTbl->hdr.type == FONT_TYPE_VECTOR_CMP )
			{		
				FillMetrics(pCurGlyf->metrics,pCurGlyf);
				int rate = ( VECTOR_POINT_SIZE/VECTOR_DISP_POINT_SIZE);
				pCurGlyf->dwidth			=  (BYTE)((pCurGlyf->dwidth + rate -1)/rate);
				pCurGlyf->x					=  (char)((pCurGlyf->x+rate -1)/rate);
				pCurGlyf->y					=  (char)((pCurGlyf->y+rate -1)/rate);
				pCurGlyf->width				=  (BYTE)((pCurGlyf->width+rate -1)/rate);
				pCurGlyf->height			=  (BYTE)((pCurGlyf->height+rate -1)/rate);
			
                if ( flag & FLAG_CMP_EX )
                {             
                    is_cmp_ex = TRUE;
                    if(flag & FLAG_COMPOSE_GLYPH)
                    {
                        ReadComposeGlyf(pCurGlyf,pFile,TRUE);
                    }
                    else
                    {
                        ReadGlyfOutlineEx(pCurGlyf,pFile,TRUE,agID[j]);
                    }
                }
                else
                {
                    ReadGlyfOutline(pCurGlyf,pFile,TRUE,agID[j]);
                }
          
			}
			else if(pGlyfTbl->hdr.type == FONT_TYPE_VECTOR )
			{		
				FillMetrics(pCurGlyf->metrics,pCurGlyf);
				int rate = ( VECTOR_POINT_SIZE/VECTOR_DISP_POINT_SIZE);
				pCurGlyf->dwidth			=  (BYTE)((pCurGlyf->dwidth + rate -1)/rate);
				pCurGlyf->x					=  (char)((pCurGlyf->x+rate -1)/rate);
				pCurGlyf->y					=  (char)((pCurGlyf->y+rate -1)/rate);
				pCurGlyf->width				=  (BYTE)((pCurGlyf->width+rate -1)/rate);
				pCurGlyf->height			=  (BYTE)((pCurGlyf->height+rate -1)/rate);	
				
				
           
                if(flag & FLAG_COMPOSE_GLYPH)
                {
                    ReadComposeGlyf(pCurGlyf,pFile,TRUE);
                }
                else
                {
                    ReadGlyfOutlineFine(pCurGlyf,pFile,TRUE,agID[j]);
                }            
			}
		}
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

         if( (pGlyfTbl->hdr.type == FONT_TYPE_VECTOR_CMP && is_cmp_ex) || pGlyfTbl->hdr.type == FONT_TYPE_VECTOR )
         {
                for(j=0;j<nGlyfCount;j++)
		        {
			        GLYPH_BODY_PTR pGlyf = NULL;
			        GLYPH_BODY_PTR pCurGlyf = NULL;
			        m_mapGlyf.Lookup(agID[j],pGlyf);			       	
				    pCurGlyf = pGlyf+i;			

                    if(pCurGlyf->nNumberSubGlyph>0 && pCurGlyf->outline.outline.point_num == 0)
                    {
                        ComposeOutline(pCurGlyf,agID[j]);
                    }
                }
         }
	}	



	return TRUE;
}

void CGlyphData::FixCSI()
{
	int i = 0;

	m_pCSI = new CHAR_SET_INFO;

	m_pCSI->tag[0] = m_fontHDR.tag[0];
	m_pCSI->tag[1] = m_fontHDR.tag[1];
	m_pCSI->tag[2] = m_fontHDR.tag[2];
	m_pCSI->tag[3] = m_fontHDR.tag[3];

	int nFtSizeNum = m_fontHDR.glyph_table_count;
	m_pCSI->nFtTblNum = nFtSizeNum;
	m_pCSI->pFtTblSize = new int[nFtSizeNum];

	for(i= 0; i< nFtSizeNum; i++)
	{
		m_pCSI->pFtTblSize[i] = m_pGlyphTables[i].hdr.size;
	}
	int  nSegCount = m_cmap.seg_countX2/2;
	m_pCSI->pUCBoundTable = new UC_BOUND_T[nSegCount];
	m_pCSI->nSectorNum = nSegCount;
	for(i= 0; i< nSegCount; i++)
	{
		m_pCSI->pUCBoundTable[i].dwUCStart = m_cmap.start_count_ptr[i];
		m_pCSI->pUCBoundTable[i].dwUCEnd = m_cmap.end_count_ptr[i];
		m_pCSI->nCharCount += (m_pCSI->pUCBoundTable[i].dwUCEnd - m_pCSI->pUCBoundTable[i].dwUCStart +1);
	}
	if(!m_bAdd)
	{
		_tcscpy(m_pCSI->szCharSetName,_T("_CUSTOMER"));
	}
	else
	{
		_tcscpy(m_pCSI->szCharSetName,_T("_CUSTOMER2"));
	}

	m_pCSI->pmapFTProp = new MAP_FTPROP;	


	CHAR_SET_INFO_PTR pCSI = NULL;
	if(g_theApp.m_mapCSI.Lookup(m_pCSI->szCharSetName,pCSI))
	{
		if(pCSI != NULL)
		{
			SAFE_FREE_ARRAY(pCSI->pUCBoundTable);		
			SAFE_FREE_ARRAY(pCSI->pFtTblSize);
			SAFE_FREE_ARRAY(pCSI->pExUCBoundTable);	
			SAFE_FREE_ARRAY(pCSI->pExFtTblSize);
			if(pCSI->pmapFTProp!=NULL)
			{
				int nKey;
				FONT_PROPERTY_PTR pProp = NULL;
				POSITION posFTP = pCSI->pmapFTProp->GetStartPosition();
				while(posFTP)
				{
					pCSI->pmapFTProp->GetNextAssoc(posFTP,nKey,pProp);
					if(pProp != NULL)
						delete pProp;
				}	
				pCSI->pmapFTProp->RemoveAll();
				delete pCSI->pmapFTProp;
				pCSI->pmapFTProp = NULL;
			}
			delete pCSI;
			pCSI = NULL;
		}
	}
	g_theApp.m_mapCSI.SetAt(m_pCSI->szCharSetName,m_pCSI);

	for(i= 0; i<g_theApp.m_aCharSetName.GetSize();i++)
	{
		if(g_theApp.m_aCharSetName.GetAt(i).CompareNoCase(m_pCSI->szCharSetName) == 0)
			break;
	}

	if(i>= g_theApp.m_aCharSetName.GetSize())
	{
		g_theApp.m_aCharSetName.Add(m_pCSI->szCharSetName);
	}
}

BOOL CGlyphData::LoadBdf(LPCTSTR lpszFileName,int nIdx,BOOL bNormal)
{
	_ASSERTE(lpszFileName != NULL);

	CStdioFile file;
    CFileException e;
	if(!file.Open(lpszFileName,CFile::modeRead | CFile::typeText /*| CFile::shareDenyWrite*/,&e))
	{
		CString strErr;
		strErr.Format(_T("Unable to open file [%s]. [Error: %d]"),lpszFileName,e.m_cause);
		AfxMessageBox(strErr);
		return FALSE;
	}

	DWORD dwGlobalDwdith = 0;
	BOOL  bGlobalDwidth = FALSE;

	BOOL bGray = FALSE;
	BDF_HEADER_T	m_tBH;
	BOOL bUnicodeEncoding = TRUE;
	BOOL bHasReadFisrtStartChar = FALSE;
	CString strLine;
	CString strTemp;
	while(file.ReadString(strLine))// read header
	{
		if(strLine.Find(_CHARS) != 0) 
		{
			/* Read FONT */
			if(strLine.Find(_FONT)==0 )
			{
				strTemp =strLine.Right(strLine.GetLength()-_tcslen(_FONT));
				if(strTemp.GetLength()>_MAX_PATH)
					strTemp = strTemp.Left(_MAX_PATH-1);
				_tcscpy(m_tBH.szFontName, strTemp);
				continue;
			}
			/* Read SIZE */
			if(strLine.Find(_SIZE) == 0)
			{
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_SIZE));
				_stscanf(strTemp,_T("%d %d %d"),&(m_tBH.dwPointSize),&(m_tBH.dwXres),&(m_tBH.dwYres));
				continue;
			}
			/* Read FONTBOUNDINGBOX */
			if(strLine.Find(_FONTBOUNDINGBOX) ==0)
			{
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_FONTBOUNDINGBOX));
				_stscanf(strTemp,_T("%d %d %d %d"),&(m_tBH.dwFBBX),&(m_tBH.dwFBBY),&(m_tBH.nXoff),&(m_tBH.nYoff));
				continue;
			}
			/* Read FAMILY_NAME font name */
			if(strLine.Find(_FAMILY_NAME) == 0)
			{
				memset(m_tBH.szFontName,0,sizeof(m_tBH.szFontName));
				strTemp =strLine.Right(strLine.GetLength()-_tcslen(_FAMILY_NAME));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				strTemp.TrimLeft(_T("\""));
				strTemp.TrimRight(_T("\""));
				if(strTemp.GetLength()>_MAX_PATH)
					strTemp = strTemp.Left(_MAX_PATH-1);
				strTemp.TrimLeft();
				strTemp.TrimRight();
				_tcscpy(m_tBH.szFontName, strTemp);
				continue;
			}
			/* Read Global DWIDTH */
			if(strLine.Find(_DWIDTH) == 0)
			{
				DWORD dwWidth = 0;
				DWORD dwTmp = 0;
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_DWIDTH));
				strTemp.TrimRight();
				strTemp.TrimLeft();
				_stscanf(strTemp,_T("%d %d"),&(dwWidth),&(dwTmp));
				bGlobalDwidth = TRUE;
				dwGlobalDwdith = (int)dwWidth;
				continue;
			}

			/* Read CHARSET_REGISTRY */
			if(strLine.Find(_CHARSET_REGISTRY) == 0)
			{
				memset(m_tBH.szFontName,0,sizeof(m_tBH.szFontName));
				strTemp =strLine.Right(strLine.GetLength()-_tcslen(_CHARSET_REGISTRY));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				strTemp.TrimLeft(_T("\""));
				strTemp.TrimRight(_T("\""));	
				strTemp.TrimLeft();
				strTemp.TrimRight();
				if(strTemp.CompareNoCase(_T("GB2312")) ==  0 || strTemp.CompareNoCase(_T("GBK")) == 0 )
				{
					bUnicodeEncoding  = FALSE;
				}
				else if(strTemp.CompareNoCase(_T("ISO10646")) == 0)
				{
					bUnicodeEncoding  = TRUE;
				}
				else
				{					
					file.Close();
					CString StrErr;
					StrErr.Format(_T("BDF file [%s] \nhas unknown CHARSET_REGISTRY \"%s\"!\n"),lpszFileName,strTemp);
					AfxMessageBox(StrErr);	
					return FALSE;
				}
				
				continue;
			}
			/* Read BITCOUNT */
			if(strLine.Find(_BITCOUNT) == 0)
			{
				int nBitCount = 0;
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_BITCOUNT));
				_stscanf(strTemp,_T("%d"),&nBitCount);
				if(nBitCount == 8)
				{
					bGray = TRUE;
				}
				
				continue;
			}
		}
		else
		{
			/* Read CHARS char count */
			strTemp = strLine.Right(strLine.GetLength()-_tcslen(_CHARS));
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if(!strTemp.IsEmpty())
			{
				_stscanf(strTemp,_T("%d"),&(m_tBH.dwCharCount));
				if(m_tBH.dwCharCount == 0)
				{				
					file.Close();
					CString StrErr;
					StrErr.Format(_T("BDF file [%s] has zero char!\n"),lpszFileName);
					AfxMessageBox(StrErr);	
					return FALSE;
				}
			}

			break;		
		}
		if(strLine.Find(_STARTCHAR) ==0)
		{
			bHasReadFisrtStartChar = TRUE;
			break;
		}
	}

	if(m_tBH.dwFBBX==0 || m_tBH.dwFBBY==0 )
	{
		file.Close();
		CString StrErr;
		StrErr.Format(_T("BDF file [%s] is not correct!\r\n [FONTBOUNDINGBOX %d %d ]\n Please correct the above param!"),lpszFileName,
			m_tBH.dwFBBX,m_tBH.dwFBBY);

		AfxMessageBox(StrErr);		
		return FALSE;
	}
	
	if(m_tBH.dwPointSize == 0)
	{
		m_tBH.dwPointSize = m_tBH.dwFBBY;
	}
	
	if(m_tBH.dwFBBX > 0xFF)
	{
		m_tBH.dwFBBX = 0xFF;
	}

	if(m_tBH.dwFBBY > 0xFF)
	{
		m_tBH.dwFBBY = 0xFF;
	}

	m_tBH.dwFBBXns = m_tBH.dwFBBX;
	m_tBH.dwFBBYns = m_tBH.dwFBBY;
	m_tBH.nXoffns  = m_tBH.nXoff;
 	m_tBH.nYoffns  = m_tBH.nYoff;
	
//	m_pCSI->pFtTblSize[nIdx] = m_tBH.dwFBBY;

	m_pGlyphTables[nIdx].hdr.size= (BYTE)m_tBH.dwFBBY;
	m_pGlyphTables[nIdx].hdr.type= (BYTE)(bGray?FONT_TYPE_GRAY_RLE : FONT_TYPE_MONO);


	if(!bHasReadFisrtStartChar)
	{
		while(file.ReadString(strLine))
		{
			if(strLine.Find(_STARTCHAR) ==0)
			{
				bHasReadFisrtStartChar = TRUE;
				break;
			}
		}
	}

	if(m_tBH.dwCharCount == 0)
	{
		m_tBH.dwCharCount = 0xFFFF;
	}
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,m_tBH.dwCharCount);

	GLYPH_BODY_PTR pGlyph = NULL;
	GLYPH_BODY_PTR pCurGlyph = NULL;
	m_tBH.dwUCBegin = 0xFFFFFFFF;
    m_tBH.dwUCEnd  = 0;
	int nMaxY = 0;
	int nMaxH = 0;
	int nGlyfId = 1;
	BOOL bFindDwidth = FALSE;
	MSG msg;
	int nCharCount = 0;

	if(m_lstID.size() != 0)
	{
		nGlyfId = m_lstID[m_lstID.size()-1] +1;
	}

	std::vector<UINT> vIDs;
	DWORD nID = 0;

	for(UINT i=0; i<m_tBH.dwCharCount && file.ReadString(strLine); i++)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}	
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,i+1);

		DWORD dwUCCode = 0;
		int   nFullWidth = 0;

        //omit STARTCHAR
		if(strLine.Find(_STARTCHAR) == 0)
		{
			file.ReadString(strLine);  //Read ENCODING
		}

		bFindDwidth = FALSE;
		
		if(strLine.Find(_ENCODING) == 0)
		{
			strTemp = strLine.Right(strLine.GetLength()-_tcslen(_ENCODING));
			_stscanf(strTemp,_T("%d"),&dwUCCode);
			if(!bUnicodeEncoding)
			{
				WORD wChar = (WORD)dwUCCode;				
				BYTE str[2] = {0};
				str[0] = HIBYTE(wChar);
				str[1] = LOBYTE(wChar);
				if(str[0]==0)
				{
					str[0] = str[1]; //lint !e415
					str[1] = 0;
				}
				WORD uChar = 0;

				MultiByteToWideChar(CP_ACP,0,(LPSTR)str,2,(LPWSTR)&uChar,1);
				dwUCCode = uChar;
			}
		}

		
		/* Get start unicode and end unicode */
		if(m_tBH.dwUCBegin > dwUCCode)
		{
			m_tBH.dwUCBegin = dwUCCode;
		}

		if(m_tBH.dwUCEnd < dwUCCode)
		{
			m_tBH.dwUCEnd = dwUCCode;
		}



		BOOL bNeedChar = IsInUCBound(dwUCCode,!bNormal);

		if(bNeedChar)
		{	
			nCharCount ++;
			
			if(!m_mapUnid.Lookup(dwUCCode,nID))
			{
				pGlyph = new GLYPH_BODY_T[m_fontHDR.glyph_table_count];
				for(int k=0; k< m_fontHDR.glyph_table_count; k++)
				{
					pGlyph[k].Init();
					pGlyph[k].flag = FLAG_GLYF_ALL;
				}
				
				m_lstID.push_back(nGlyfId);
				m_lstUnicode.push_back(dwUCCode);
				m_mapUnid.SetAt(dwUCCode,nGlyfId);			
				m_mapGlyf.SetAt(nGlyfId,pGlyph);
				pCurGlyph = pGlyph+nIdx;	

				UnicodeArray_ptr pUA= NULL;
				if(m_mapIdun.Lookup(nGlyfId,pUA))
				{
					pUA->Add(dwUCCode);
				}
				else
				{
					pUA = new  CUIntArray;
					pUA->Add(dwUCCode);
					m_mapIdun.SetAt(nGlyfId,pUA);
				}
			
				DWORD nID = nGlyfId;
				vIDs.push_back(nID);

				nGlyfId++;
			}
			else
			{
				vIDs.push_back(nID);
				m_mapGlyf.Lookup(nID,pGlyph);
				pCurGlyph = pGlyph+nIdx;
			}
	
		}
		else
		{
			pCurGlyph = NULL;
		}


		DWORD dwBBw = 0;
		DWORD dwBBh = 0;
		int nBBxoff0x  =0;
		int nBByoff0y = 0;
		

		while(file.ReadString(strLine))
		{			
			if(strLine.Find(_ENDCHAR) == 0)
				break;

			if(!bNeedChar || pCurGlyph == NULL)
				continue;

			if(strLine.Find(_DWIDTH) == 0)
			{
				bFindDwidth = TRUE;
				DWORD dwWidth = 0;
				DWORD dwTmp = 0;
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_DWIDTH));
				strTemp.TrimRight();
				strTemp.TrimLeft();
				_stscanf(strTemp,_T("%d %d"),&(dwWidth),&(dwTmp));
				pCurGlyph->dwidth = (BYTE)dwWidth;
				continue;

			}
			else if(strLine.Find(_BBX) == 0)
			{
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_BBX));
				_stscanf(strTemp,_T("%d %d %d %d"),&(dwBBw),&(dwBBh),&(nBBxoff0x),&(nBByoff0y));
				int nw = dwBBw;
				nFullWidth = (int)((nw + 7) / 8);
				if(bGray)
				{
					nFullWidth = nw;
				}
			
				if(dwBBw != 0 && dwBBh != 0)
				{
					pCurGlyph->bitmap = new BYTE[nFullWidth * dwBBh ];
					memset(pCurGlyph->bitmap,0,nFullWidth * dwBBh);
				}
				else
				{
					pCurGlyph->bitmap = NULL;
				}

				if(!bFindDwidth)
				{
					if(	bGlobalDwidth)
					{
						pCurGlyph->dwidth = (WORD)dwGlobalDwdith;
					}
					else
					{
						pCurGlyph->dwidth = (WORD)dwBBw;
					}
				}

				pCurGlyph->flag = FLAG_GLYF_ALL;
				pCurGlyph->width = (BYTE)dwBBw;
				pCurGlyph->height = (BYTE)dwBBh;				
				pCurGlyph->x =  (char)(nBBxoff0x - m_tBH.nXoff) ;
			    if(g_theApp.m_nBdfCalcBearingYStrategy == 0)
				{
					pCurGlyph->y = (char)((nBByoff0y - m_tBH.nYoff) + dwBBh);
				}
				else
				{
					pCurGlyph->y = (char)((nBByoff0y - m_tBH.nYoff) + m_tBH.dwFBBY);
				}
// 				if(pCurGlyph->width != 0 && pCurGlyph->dwidth == 0)
// 				{
// 					pCurGlyph->dwidth = (BYTE)(m_tBH.dwFBBX);	
// 				}

				m_tMetrics[pCurGlyph->dwidth].dwidth_sum ++;
				m_tMetrics[(BYTE)(pCurGlyph->x)].x_sum ++;
				m_tMetrics[(BYTE)(pCurGlyph->y)].y_sum ++;
				m_tMetrics[pCurGlyph->width].width_sum ++;
				m_tMetrics[pCurGlyph->height].height_sum ++;

				if(((char)pCurGlyph->y) > nMaxY)
				{
					nMaxY = pCurGlyph->y;
				}

				if( (pCurGlyph->height - (pCurGlyph->y)*2) > nMaxH)
				{
					nMaxH = (pCurGlyph->height - (pCurGlyph->y)*2);
				}

				continue;
			}

			/* Read BITMAP data */
			if(strLine.Find(_BITMAP) == 0)
			{
				for(UINT j=0; j<dwBBh && dwBBw != 0; j++)
				{
					file.ReadString(strLine);
					strLine.TrimRight();
					HexCharToBinary(strLine,pCurGlyph->bitmap + j * nFullWidth);
				}
		
				int glyph_size = pCurGlyph->width * pCurGlyph->height;
				if(glyph_size!= 0 && bGray)
				{
					BYTE *pTmpBuf = new BYTE[glyph_size];
					memset(pTmpBuf,0,glyph_size);
					Font_Half_Sample(pCurGlyph->bitmap,pTmpBuf,pCurGlyph->width,pCurGlyph->height,0);
					memcpy(pCurGlyph->bitmap,pTmpBuf,glyph_size);
					delete [] pTmpBuf;
				}
				
//				FillDotMatrix(pCurChar); // fill the char with spaces 
//				AbstractGlyph(pCurChar); // abstract the glyph from the dot-matrix
			}
		}		
	}

	file.Close();

    ::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

	std::sort(vIDs.begin(),vIDs.end());

	m_pGlyphTables[nIdx].pIDs = new UINT[vIDs.size()];
	for(i = 0; i< vIDs.size(); i++)
	{
		m_pGlyphTables[nIdx].pIDs[i] =  vIDs[i];
	}


	TEST_METRICS_T tMetrics;
	memset(&tMetrics,0,sizeof(tMetrics));
	for ( int j = 0; j < TEST_METRICS_MAX; j++)
    {
        if ( m_tMetrics[j].dwidth_sum > tMetrics.dwidth_sum )
        {
            tMetrics.dwidth_sum = m_tMetrics[j].dwidth_sum;   
			m_pGlyphTables[nIdx].hdr.advance = (BYTE)j;
        }
		if ( m_tMetrics[j].x_sum > tMetrics.x_sum )
        {
            tMetrics.x_sum = m_tMetrics[j].x_sum;   
			m_pGlyphTables[nIdx].hdr.bearing_x = (BYTE)j;
        }
		if ( m_tMetrics[j].y_sum > tMetrics.y_sum )
        {
            tMetrics.y_sum = m_tMetrics[j].y_sum;   
			m_pGlyphTables[nIdx].hdr.bearing_y = (BYTE)j;
        }
		if ( m_tMetrics[j].width_sum > tMetrics.width_sum )
        {
            tMetrics.width_sum = m_tMetrics[j].width_sum;   
			m_pGlyphTables[nIdx].hdr.bitmap_w = (BYTE)j;
        }
		if ( m_tMetrics[j].height_sum > tMetrics.height_sum )
        {
            tMetrics.height_sum = m_tMetrics[j].height_sum;   
			m_pGlyphTables[nIdx].hdr.bitmap_h = (BYTE)j;
        }
    }

	m_pGlyphTables[nIdx].hdr.page_offsets_base = sizeof(m_pGlyphTables[nIdx].hdr);
	m_pGlyphTables[nIdx].hdr.glyph_count = (WORD)nCharCount;
	m_pGlyphTables[nIdx].hdr.ppem_x = m_pGlyphTables[nIdx].hdr.ppem_y = m_pGlyphTables[nIdx].hdr.size;

	if(nMaxY<=0 || nMaxY>m_pGlyphTables[nIdx].hdr.size)
	{
		nMaxY = m_pGlyphTables[nIdx].hdr.size;
	}

	nMaxH += nMaxY;

	if(nMaxH<m_pGlyphTables[nIdx].hdr.size)
	{
		nMaxH = m_pGlyphTables[nIdx].hdr.size;
	}
	else if(nMaxH>(int)(m_pGlyphTables[nIdx].hdr.size*1.5))
	{
		nMaxH = (int)(m_pGlyphTables[nIdx].hdr.size*1.5);
	}

	m_pGlyphTables[nIdx].hdr.max_bearing_y = (char)nMaxY;
	m_pGlyphTables[nIdx].hdr.max_height = (BYTE)nMaxH;


//	FixCmap(TRUE);

//	m_bReady = TRUE;

	return TRUE;
}

BOOL CGlyphData::IsInUCBound(DWORD dwUCCode,BOOL bExtra /*= FALSE*/)
{
	int nCount = bExtra ? m_pCSI->nExSectorNum : m_pCSI->nSectorNum;

	int nMid = 0;
	int nMin = 0;
	int nMax = nCount;

	UC_BOUND_PTR pUCBoundTable = bExtra ? m_pCSI->pExUCBoundTable : m_pCSI->pUCBoundTable;

	if(pUCBoundTable == NULL)
		return FALSE;

	while(nMin < nMax)
	{
		nMid = (nMax + nMin)/2;

		if(dwUCCode < pUCBoundTable[nMid].dwUCStart)
		{
			nMax = nMid;
		}
		else if(dwUCCode > pUCBoundTable[nMid].dwUCEnd)
		{
			nMin = nMid + 1;
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;	
}

BOOL CGlyphData::HexCharToBinary(LPCTSTR lpszHex,LPBYTE lpbBuf)
{
	int nLength = _tcslen(lpszHex);

	_ASSERTE(nLength % 2 == 0);

	int nSize = nLength / 2;
	
	CString strCurByte;
	CString strLeft = lpszHex;

	for(int i = 0; i< nSize; i++)
	{
		strCurByte = strLeft.Left(2);
		if( i != nSize -1)
			strLeft = strLeft.Right(nLength - (i+1)*2);
        int nByte = 0;
		_stscanf(strCurByte,_T("%02X"),&nByte);
		*(lpbBuf+i) = (BYTE)nByte;
	}

	return TRUE;
}

BOOL CGlyphData::SaveToBdf(LPCTSTR lpszFileName)
{
	_ASSERTE(lpszFileName != NULL);

	CString strErr;

	TCHAR szHex[256] = {0};
	
	int nCount = m_fontHDR.glyph_table_count;

	for(int i = 0; i< nCount; i++)
	{
		int nBBY = m_pGlyphTables[i].hdr.size;
		CString strSize;
		strSize.Format(_T("_%d"),nBBY);
		CString strFileName = lpszFileName;
		strFileName.Insert(strFileName.GetLength()-4,strSize);
		CStdioFile file;
		CFileException e;
		if(!file.Open(strFileName,CFile::modeWrite | CFile::modeCreate | CFile::typeText /*| CFile::shareDenyWrite*/,&e))
		{		
			strErr.Format(_T("Unable to open file [%s]. [Error: %d]"),strFileName,e.m_cause);		
			return FALSE;
		}
		CString strLine;
		strLine.Format(_T("%s2.1\n"),_STARTFONT);
		file.WriteString(strLine);

		strLine.Format(_T("%s%s\n"),_FONT,m_pCSI->szCharSetName);
		file.WriteString(strLine);

		strLine.Format(_T("%s%d 72 72\n"),_SIZE,nBBY);
		file.WriteString(strLine);

		strLine.Format(_T("%s%d %d 0 0\n"),_FONTBOUNDINGBOX,
			m_pGlyphTables[i].hdr.advance,
			m_pGlyphTables[i].hdr.size);
		file.WriteString(strLine);

		strLine.Format(_T("%s1\n"),_STARTPROPERTIES);
		file.WriteString(strLine);

		strLine.Format(_T("%s%d\n"),_BITCOUNT,
			m_pGlyphTables[i].hdr.type==FONT_TYPE_GRAY_RLE?8:1);
		file.WriteString(strLine);

		strLine.Format(_T("%s\n"),_ENDPROPERTIES);
		file.WriteString(strLine);
	
		GLYPH_BODY_PTR pGlyf = NULL;

		int nCharCount = 0;

//		int TestCount =  m_lstUnicode.size();

 		for(UINT j = 0; j< m_lstUnicode.size(); j++)
 		{
 			if(GetGlyph(m_lstUnicode[j],i,TRUE) != NULL)
			{
				nCharCount++;
			}
 		}
	
		strLine.Format(_T("%s%d\n"),_CHARS,nCharCount);
		file.WriteString(strLine);

		MSG msg;
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,nCharCount);

		int nStep = 0;
		DWORD dwUnicode = 0;
		for(j = 0; j< m_lstUnicode.size(); j++)
		{
			while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);	
			}	
			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);	

			dwUnicode = m_lstUnicode[j];			

			pGlyf = GetGlyph(dwUnicode,i,TRUE);

			if(pGlyf == NULL)
				continue;

			strLine.Format(_T("%s %04X\n"),_STARTCHAR,dwUnicode);
			file.WriteString(strLine);
			strLine.Format(_T("%s %d\n"),_ENCODING,dwUnicode);
			file.WriteString(strLine);
			strLine.Format(_T("%s 1000 0\n"),_SWIDTH);
			file.WriteString(strLine);
			strLine.Format(_T("%s %d 0\n"),_DWIDTH,pGlyf->dwidth);
			file.WriteString(strLine);
			strLine.Format(_T("%s %d %d %d %d\n"),_BBX,
				pGlyf->width,
				pGlyf->height,
				pGlyf->x,
				pGlyf->y - /*nBBY*/ pGlyf->height);
			file.WriteString(strLine);
			strLine.Format(_T("%s\n"),_BITMAP);
			file.WriteString(strLine);
			
			
			int nFullW = (pGlyf->width + 7) / 8;	
			if(m_pGlyphTables[i].hdr.type == FONT_TYPE_GRAY_RLE)
			{
				nFullW = pGlyf->width;
			}
			
			if(pGlyf->bitmap != NULL)
			{
				for(UINT l=0; l<pGlyf->height;l++)
				{
					BinaryToHexChar(pGlyf->bitmap + l*nFullW,nFullW,szHex);
					file.WriteString(szHex);
					file.WriteString(_T("\n"));
				}
			}

			strLine.Format(_T("%s\n"),_ENDCHAR);
			file.WriteString(strLine);	

		}
	
		strLine.Format(_T("%s\n"),_ENDFONT);
		file.WriteString(strLine);	
		file.Close();
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	}

	return TRUE;
}

BOOL CGlyphData::BinaryToHexChar(LPBYTE lpbBuf,int nSize,LPTSTR lpszHex)
{
	_ASSERTE(lpbBuf != NULL);

    _TCHAR szValue[3] = {0};
    for(int i = 0; i < nSize; ++i)
    {
        _stprintf(szValue, _T("%02X"), *lpbBuf);
        *lpszHex = szValue[0];
        lpszHex++;
        *lpszHex = szValue[1];
        lpszHex++;

        lpbBuf++;
    }
	*lpszHex = _T('\0');

	return TRUE;
}

void CGlyphData::GetIDList(CUIntArray &agListID, int nPtIndex)
{
	agListID.RemoveAll();
	
	for(int i= 0; i< m_pGlyphTables[nPtIndex].hdr.glyph_count ; i++)
	{
		agListID.Add(m_pGlyphTables[nPtIndex].pIDs[i]);
	}

}

int  CGlyphData::GetPointSize(int nGlyfTblIdx)
{
	if(m_pGlyphTables == NULL)
	{
		return 0;
	}
	if(nGlyfTblIdx >= m_fontHDR.glyph_table_count || nGlyfTblIdx<0)
	{
		return 0;
	}

	return m_pGlyphTables[nGlyfTblIdx].hdr.size;

}

int  CGlyphData::GetMaxBearingY(int nGlyfTblIdx)
{
	if(m_pGlyphTables == NULL)
	{
		return 0;
	}
	if(nGlyfTblIdx >= m_fontHDR.glyph_table_count || nGlyfTblIdx<0)
	{
		return 0;
	}

	return m_pGlyphTables[nGlyfTblIdx].hdr.max_bearing_y;
}

int  CGlyphData::GetMaxHeight(int nGlyfTblIdx)
{
	if(m_pGlyphTables == NULL)
	{
		return 0;
	}
	if(nGlyfTblIdx >= m_fontHDR.glyph_table_count || nGlyfTblIdx<0)
	{
		return 0;
	}

	return m_pGlyphTables[nGlyfTblIdx].hdr.max_height;
}

GLYPH_TABLE_HDR_PTR CGlyphData::GetGlyfTblHdr(int nGlyfTblIdx)
{
	if(m_pGlyphTables == NULL)
	{
		return NULL;
	}
	if(nGlyfTblIdx >= m_fontHDR.glyph_table_count || nGlyfTblIdx<0)
	{
		return NULL;
	}

	return &(m_pGlyphTables[nGlyfTblIdx].hdr);
}


void CGlyphData::Init(HWND hWnd/*=NULL*/,BOOL bAdd /*= FALSE*/)
{ 
	m_hWnd = hWnd;
	m_bAdd = bAdd;
}
void CGlyphData::SetCharSetInfo(CHAR_SET_INFO_PTR pCSI)
{
	Clear(); 
	m_pCSI = pCSI;
}

DWORD CGlyphData::GetIDFromUni(DWORD dwUnicode)
{
	DWORD dwID = 0;
	m_mapUnid.Lookup(dwUnicode,dwID);

	return dwID;
}
DWORD CGlyphData::GetUniFormID(DWORD dwID)
{
	DWORD dwUnicode = 0;
	UnicodeArray_ptr pagUni=NULL;
	if(m_mapIdun.Lookup(dwID,pagUni))
	{
		if(pagUni != NULL && pagUni->GetSize()>0)
		{
			dwUnicode = pagUni->GetAt(0);
		}
	}
	return dwUnicode;
}

BOOL CGlyphData::LoadBdfs(CStringArray &agNormalBdfs,CStringArray &agExtraBdfs)
{
	int nFtSizeNum = agNormalBdfs.GetSize();
	int nExFtSizeNum = agExtraBdfs.GetSize();

	if( 0 == nFtSizeNum)
	{
		return FALSE;
	}

	int i=0;

	CMap<int,int,BDF_ITEM_PTR,BDF_ITEM_PTR> mapSize;

	BOOL bOK = TRUE;

	std::vector<int> vSize;
	BDF_ITEM_PTR  pBDFItemTmp = NULL;
	for(i=0;i< nFtSizeNum; i++)
	{
		BDF_ITEM_PTR  pBDFItem = new BDF_ITEM_T;
		pBDFItem->bNormal = TRUE;
		_tcscpy(pBDFItem->szBdfFile,agNormalBdfs[i].operator LPCTSTR());
		if(!GetBDFParam(pBDFItem))
		{
			delete pBDFItem;
			bOK = FALSE;	
		}
		else
		{		
			if(mapSize.Lookup(pBDFItem->nPTSize,pBDFItemTmp))
			{
				AfxMessageBox(_T("There are the same Font Size in BDFS!"));
				delete pBDFItem;
				bOK = FALSE;
			}
			else
			{
				mapSize.SetAt(pBDFItem->nPTSize,pBDFItem);
				vSize.push_back(pBDFItem->nPTSize);
			}
		}
	}
	for(i=0;i< nExFtSizeNum; i++)
	{
		BDF_ITEM_PTR  pBDFItem = new BDF_ITEM_T;
		pBDFItem->bNormal = FALSE;
		_tcscpy(pBDFItem->szBdfFile,agExtraBdfs[i].operator LPCTSTR());
		if(!GetBDFParam(pBDFItem))
		{
			delete pBDFItem;
			bOK = FALSE;	
		}
		else
		{		
			if(mapSize.Lookup(pBDFItem->nPTSize,pBDFItemTmp))
			{
				AfxMessageBox(_T("There are the same Font Size in BDFS!"));
				delete pBDFItem;
				bOK = FALSE;
			}
			else
			{
				mapSize.SetAt(pBDFItem->nPTSize,pBDFItem);
				vSize.push_back(pBDFItem->nPTSize);
			}
		}
	}
	if(!bOK)
	{
		POSITION pos = mapSize.GetStartPosition();
		int nKey = 0;

		while(pos)
		{
			mapSize.GetNextAssoc(pos,nKey,pBDFItemTmp);
			delete pBDFItemTmp;
		}
		mapSize.RemoveAll();
		return FALSE;
	}

	std::sort(vSize.begin(),vSize.end());
	
	int nTotal = nFtSizeNum + nExFtSizeNum;

	Clear();
	m_bLoadLib = FALSE;

	m_fontHDR.glyph_table_count = (WORD)nTotal;	

	memcpy(&m_fontHDR.tag,&m_pCSI->tag,4);
	m_fontHDR.version[1] = 3;
	m_fontHDR.flags = 0x0001;
	m_fontHDR.gtbl_offset_ptr = new DWORD[nTotal];
	memset(m_fontHDR.gtbl_offset_ptr,0,sizeof(DWORD)*nTotal);
	m_pGlyphTables = new GLYPH_TABLE_T[nTotal];
	for(int k=0;k<nTotal;k++)
	{
		m_pGlyphTables[k].Init();
	}
	m_cmap.encoding_id = 1;
	m_cmap.platform_id = 3;	

	for(i=0; i< nTotal; i++)
	{
		int nPtSize = vSize[i];
		mapSize.Lookup(nPtSize,pBDFItemTmp);
	
		if(!LoadBdf(pBDFItemTmp->szBdfFile,i,pBDFItemTmp->bNormal))
		{
			Clear();
			{
				POSITION pos = mapSize.GetStartPosition();
				int nKey = 0;

				while(pos)
				{
					mapSize.GetNextAssoc(pos,nKey,pBDFItemTmp);
					delete pBDFItemTmp;
				}
				mapSize.RemoveAll();
			}
			return FALSE;
		}
	
	}

	std::sort(m_lstID.begin(),m_lstID.end());
	std::sort(m_lstUnicode.begin(),m_lstUnicode.end());

	BOOL bModifyID = FALSE;	// need not modify ID, IDs are sequence.

	// need not to call CalcMultiUnicodeOneID()
	FixCmap(bModifyID);	
	CalcPage(bModifyID);
	FixCSI();

	if(m_mapGlyf.GetCount()== 0)
	{
		Clear();
		AfxMessageBox(_T("No validate glyph in BDFs"));
		{
			POSITION pos = mapSize.GetStartPosition();
			int nKey = 0;

			while(pos)
			{
				mapSize.GetNextAssoc(pos,nKey,pBDFItemTmp);
				delete pBDFItemTmp;
			}
			mapSize.RemoveAll();
		}

		return FALSE;
	}

	{
		POSITION pos = mapSize.GetStartPosition();
		int nKey = 0;

		while(pos)
		{
			mapSize.GetNextAssoc(pos,nKey,pBDFItemTmp);
			delete pBDFItemTmp;
		}
		mapSize.RemoveAll();
	}

	m_bReady = TRUE;


	return TRUE;

}



BOOL CGlyphData::LoadMonoGlyph(GLYPH_BODY_PTR pGlyf,int nID)
{
	if(FT_Load_Glyph (m_face, nID, FT_LOAD_RENDER | FT_LOAD_MONOCHROME)!= FT_SUCCESS)
		return FALSE;

	FT_GlyphSlot glyph;
	glyph = m_face->glyph;
	pGlyf->flag = FLAG_GLYF_ALL;
	pGlyf->dwidth = (BYTE)(glyph->metrics.horiAdvance >> 6);
    pGlyf->x = (char)(glyph->metrics.horiBearingX >> 6);
	pGlyf->y = (char)(glyph->metrics.horiBearingY >> 6);
	pGlyf->width = (BYTE)glyph->bitmap.width;
	pGlyf->height = (BYTE)glyph->bitmap.rows;
	
	if(!IS_IN_BYTE_RANG( glyph->metrics.horiAdvance >> 6 ) || 
	   !IS_IN_CHAR_RANG( (glyph->metrics.horiBearingX >> 6) ) || 
	   !IS_IN_CHAR_RANG( (glyph->metrics.horiBearingY >> 6) ) || 
	   !IS_IN_BYTE_RANG( (glyph->bitmap.width) ) || 
	   !IS_IN_BYTE_RANG( (glyph->bitmap.rows) ) )
	{
		m_nOverOneByteNum++;
	}



	int nWidthByte = (pGlyf->width + 7)/8;
	int glyph_size = nWidthByte* pGlyf->height;

	if(glyph_size>0)
	{
		pGlyf->bitmap = new BYTE[glyph_size];
		for(int i=0;i<pGlyf->height;i++)
		{
			memcpy(pGlyf->bitmap+i*nWidthByte,
				glyph->bitmap.buffer+glyph->bitmap.pitch*i,
				nWidthByte);
		}
	}

	return TRUE;
}

BOOL CGlyphData::LoadArgbGlyph(GLYPH_BODY_PTR pGlyf,int nID)
{
	if(FT_Load_Glyph (m_face, nID, FT_LOAD_RENDER /*| FT_LOAD_NO_HINTING */) != FT_SUCCESS)
		return FALSE;

	FT_Glyph glyph;

    if(FT_Get_Glyph( m_face->glyph, &glyph ) != FT_SUCCESS)
		return FALSE;
            
    if(FT_Render_Glyph( m_face->glyph,   FT_RENDER_MODE_NORMAL )!= FT_SUCCESS)
		return FALSE;

//  test
// 	FILE *pFile = _tfopen(_T("D:\\1.txt"),_T("wt"));
// 	int k= 0;
// 	for(k = 0; k<m_face->glyph->outline.n_points;k++)
// 	{
// 		_ftprintf(pFile,_T("{0x%04X,0x%04X},"),m_face->glyph->outline.points[k].x,m_face->glyph->outline.points[k].y);
// 		if(k!=0 && k%8==0)
// 		_ftprintf(pFile,_T("\r\n"));
// 
// 	}
// 	_ftprintf(pFile,_T("\r\n"));
// 
// 	fclose(pFile);
//

    if(FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 )!= FT_SUCCESS)
		return FALSE;

    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;          
    FT_Bitmap& bitmap=bitmap_glyph->bitmap;


	pGlyf->flag = FLAG_GLYF_ALL;
	pGlyf->dwidth = (BYTE)(m_face->glyph->metrics.horiAdvance >> 6);
    pGlyf->x = (char)(m_face->glyph->metrics.horiBearingX >> 6);
	pGlyf->y = (char)(m_face->glyph->metrics.horiBearingY >> 6);
	pGlyf->width = (BYTE)bitmap.width;
	pGlyf->height = (BYTE)bitmap.rows;

	if(!IS_IN_BYTE_RANG( (m_face->glyph->metrics.horiAdvance >> 6) ) || 
	   !IS_IN_CHAR_RANG( (m_face->glyph->metrics.horiBearingX >> 6) ) || 
	   !IS_IN_CHAR_RANG( (m_face->glyph->metrics.horiBearingY >> 6) ) || 
	   !IS_IN_BYTE_RANG( (bitmap.width) ) || 
	   !IS_IN_BYTE_RANG( (bitmap.rows) ) )
	{
		m_nOverOneByteNum++;
	}

           
    int width = bitmap.width;
    int height = bitmap.rows;

	int glyph_size = width* height;
	if(glyph_size > 0)
	{
		pGlyf->bitmap = new BYTE[glyph_size];
		memset(pGlyf->bitmap,0,glyph_size);
		if(bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
		{
			memcpy(pGlyf->bitmap, bitmap.buffer,glyph_size);
		}
		else if(bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
		{
			for(int i = 0; i< height;i++)
			{
				for(int j=0;j< bitmap.pitch;j++)
				{
					BYTE tmp = bitmap.buffer[i*bitmap.pitch + j];
					for(int k = 0; k< 8 && (j*8 + k)<width; k++)
					{
						BYTE pixel = (BYTE)((tmp&0x80)>>7);
						pGlyf->bitmap[i*width + j*8 + k] = (BYTE)(pixel*0xFF);
						tmp = (BYTE)(tmp << 1);
					}
				}
			}
		}
		else
		{
			delete [] pGlyf->bitmap;
			memset(pGlyf,0,sizeof(GLYPH_BODY_T));
			_ASSERTE(0);
		}

		if(pGlyf->bitmap != NULL)
		{
			BYTE *pTmpBuf = new BYTE[glyph_size];
			memset(pTmpBuf,0,glyph_size);
			Font_Half_Sample(pGlyf->bitmap,pTmpBuf,width,height,0);
			memcpy(pGlyf->bitmap,pTmpBuf,glyph_size);
			delete [] pTmpBuf;
		}

	}

	FT_Done_Glyph(glyph);

	return TRUE;
}



BOOL CGlyphData::LoadVectorGlyph(GLYPH_BODY_PTR pGlyf,int nID,int nDispPtSize)
{
	if(m_pCSI == NULL)
		return FALSE;

	if(FT_Load_Glyph (m_face, nID,FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE) != FT_SUCCESS)
		return FALSE;
	_ASSERTE(m_pCSI != NULL);
    if( m_pCSI->bSupportSubGlyph && 
		m_pCSI->dZoomRate == VECTOR_BASE_ZOOM && // only VECTOR_BASE_ZOOM can be used sub-glyph
		/*m_pCSI->bVecCmpMore && */m_face->glyph->num_subglyphs > 0 )
    {
        int i = 0;

		pGlyf->nNumberSubGlyph = (WORD)m_face->glyph->num_subglyphs;
		pGlyf->pSubGlyf = new X_SUBGLYPH[pGlyf->nNumberSubGlyph];

        pGlyf->outline.max_points = SUB_GLYPH_POINT_MAX;
        pGlyf->outline.max_contours = SUB_GLYPH_CONTOURS_MAX;

        pGlyf->outline.outline.points      = new XFT_VECTOR[pGlyf->outline.max_points];	
        pGlyf->outline.outline.tags        = new BYTE[pGlyf->outline.max_points];
        pGlyf->outline.outline.contours   = new short[pGlyf->outline.max_contours];

        int nOrgPintSize = (m_face->units_per_EM >> 6);
        double dRate = m_pCSI->dZoomRate / (double)(nOrgPintSize); 
        double dDelRate = (VECTOR_POINT_SIZE) / nOrgPintSize * m_pCSI->dZoomRate;	
        
        pGlyf->flag = FLAG_GLYF_ALL;
        UINT advance = (UINT)((m_face->glyph->metrics.horiAdvance* dDelRate)/64);	
		advance = FixedFromDouble((m_face->glyph->metrics.horiAdvance* dDelRate)/64) >> 16;
        pGlyf->metrics.dwidth = (BYTE)advance;
		//pGlyf->metrics.advance_org = m_face->glyph->metrics.horiAdvance;
		pGlyf->metrics.advance_org = (FixedFromDouble((advance << 6) / (dDelRate))>>16);

		//UINT advance = (m_face->glyph->metrics.horiAdvance >> 64);
		//pGlyf->metrics.dwidth = (BYTE)advance;
        
        if(!IS_IN_BYTE_RANG(advance))
        {
            m_nOverOneByteNum ++;
        }

		for( i = 0; i < pGlyf->nNumberSubGlyph; i++ )
		{
			FT_SubGlyph  subg = m_face->glyph->subglyphs + i;
		
 			pGlyf->pSubGlyf[i].index     = (WORD)subg->index;
			pGlyf->pSubGlyf[i].flags     = subg->flags;
			pGlyf->pSubGlyf[i].arg1      = (short)subg->arg1;
			pGlyf->pSubGlyf[i].arg2      = (short)subg->arg2;
			
		    pGlyf->pSubGlyf[i].transform.xx = subg->transform.xx;
			pGlyf->pSubGlyf[i].transform.xy = subg->transform.xy;
			pGlyf->pSubGlyf[i].transform.yy = subg->transform.yy;
			pGlyf->pSubGlyf[i].transform.yx = subg->transform.yx;
		}

        for( i = 0; i< pGlyf->nNumberSubGlyph; i++)
        {	           
            if(FT_Load_Glyph ( m_face, pGlyf->pSubGlyf[i].index, FT_LOAD_NO_SCALE ) != FT_SUCCESS )
                return FALSE;

            //_ASSERTE(0 == m_face->glyph->num_subglyphs);

            XFT_OUTLINE _outline = {0};
            
            _outline.outline.contour_num = m_face->glyph->outline.n_contours;
            _outline.outline.point_num   = m_face->glyph->outline.n_points;  
            //_outline.outline.flags		= m_face->glyph->outline.flags;
			if(_outline.outline.point_num >0)
			{
				_outline.outline.points    = new SPML_VECTOR_T[_outline.outline.point_num];
				_outline.outline.tags		= new BYTE[_outline.outline.point_num];
				memcpy(_outline.outline.points,m_face->glyph->outline.points, _outline.outline.point_num * sizeof(SPML_VECTOR_T));
				memcpy(_outline.outline.tags,m_face->glyph->outline.tags, _outline.outline.point_num * sizeof(BYTE));
			}
			if(_outline.outline.contour_num>0)
			{
				_outline.outline.contours	= new short[_outline.outline.contour_num];
				memcpy(_outline.outline.contours,m_face->glyph->outline.contours,_outline.outline.contour_num * sizeof(short));
			}           
            
            
            /* now perform the transform required for this subglyph */
            if ( pGlyf->pSubGlyf[i].flags & ( FT_SUBGLYPH_FLAG_SCALE |FT_SUBGLYPH_FLAG_XY_SCALE | FT_SUBGLYPH_FLAG_2X2 ) )
            {               
                X_OutlineTransform( &_outline.outline, &pGlyf->pSubGlyf[i].transform );
            }
            
            /* apply offset */
            if ( pGlyf->pSubGlyf[i].flags & FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES )
            {
                int x = pGlyf->pSubGlyf[i].arg1;
                int y = pGlyf->pSubGlyf[i].arg2;

                if ( x || y )
                {
                    X_TranslateArray( _outline.outline.point_num, _outline.outline.points, x, y );
                }
            }

			if(m_pCSI->bVecCmp)
			{ 
				//zoom out point size 1
				SPML_MATRIX_T matrix;
				matrix.xx = FixedFromDouble(dRate);
				matrix.xy = FixedFromDouble(0);
				matrix.yx = FixedFromDouble(0);
				matrix.yy = FixedFromDouble(dRate); 
				X_OutlineTransform(&_outline.outline,&matrix);
			}

            X_AddGlyphOutline( &pGlyf->outline, &_outline.outline );

            SAFE_FREE_ARRAY(_outline.outline.points);
            SAFE_FREE_ARRAY(_outline.outline.tags);
            SAFE_FREE_ARRAY(_outline.outline.contours);
        }
    }
    else
    {
        if(FT_Load_Glyph (m_face, nID,FT_LOAD_NO_SCALE) != FT_SUCCESS)
            return FALSE;
        
        FT_Glyph glyph;
        
        if(FT_Get_Glyph( m_face->glyph, &glyph ) != FT_SUCCESS)
            return FALSE;
        
        FT_OutlineGlyph glyfOutline = (FT_OutlineGlyph)glyph;
        
        FT_Orientation ori = FT_Outline_Get_Orientation(&glyfOutline->outline);
        if(ori == FT_ORIENTATION_NONE )
        {
            _ASSERTE(0);
            return FALSE;
        }
        
        FT_Outline _outline={0};
        
        _outline.n_contours = glyfOutline->outline.n_contours;
        _outline.n_points   = glyfOutline->outline.n_points;
        _outline.flags		= glyfOutline->outline.flags;

		if(_outline.n_points >0)
		{
			_outline.points		= new FT_Vector[_outline.n_points];
			_outline.tags		= new char[_outline.n_points];
			memcpy(_outline.points,glyfOutline->outline.points, _outline.n_points * sizeof(FT_Vector));
			memcpy(_outline.tags,glyfOutline->outline.tags, _outline.n_points * sizeof(char));
		}
		if(_outline.n_contours>0)
		{
			_outline.contours	= new short[_outline.n_contours];
			memcpy(_outline.contours,glyfOutline->outline.contours,_outline.n_contours * sizeof(short));
		}
        
        int nOrgPintSize = (m_face->units_per_EM >> 6);
        double dRate = m_pCSI->dZoomRate / (double)(nOrgPintSize); 
        double dDelRate = (VECTOR_POINT_SIZE) / nOrgPintSize * m_pCSI->dZoomRate;	
        
        pGlyf->flag = FLAG_GLYF_ALL;
        UINT advance = (UINT)((m_face->glyph->metrics.horiAdvance* dDelRate)/64);	
        pGlyf->metrics.dwidth = (BYTE)advance;
		//pGlyf->metrics.advance_org = m_face->glyph->metrics.horiAdvance;
		pGlyf->metrics.advance_org = (DWORD)((advance << 6) / (dDelRate));

		//UINT advance = (UINT)(m_face->glyph->metrics.horiAdvance >> 6);	
		//pGlyf->metrics.dwidth = (BYTE)advance;
        
        if(!IS_IN_BYTE_RANG(advance))
        {
            m_nOverOneByteNum ++;
        }

		if(m_pCSI->bVecCmp)
		{        
			//zoom out point size 1
			FT_Matrix matrix;
			matrix.xx = (FT_Fixed)FixedFromDouble(dRate);
			matrix.xy = (FT_Fixed)FixedFromDouble(0);
			matrix.yx = (FT_Fixed)FixedFromDouble(0);
			matrix.yy = (FT_Fixed)FixedFromDouble(dRate); 
			FT_Outline_Transform(&_outline,&matrix);
        
			// must check the points zoomed can be store in one BYTE.
			char cMin = (char)((BYTE)(0x80));
			char cMax = (char)((BYTE)(0x7F));
			for(int k=0; k< _outline.n_points; k++)
			{
				if( (_outline.points[k].x < cMin || _outline.points[k].x > cMax) ||
					(_outline.points[k].y < cMin || _outline.points[k].y > cMax))
				{
					//_ASSERTE(0);
					FT_Done_Glyph(glyph);
                
					SAFE_FREE_ARRAY(_outline.points);
					SAFE_FREE_ARRAY(_outline.tags);
					SAFE_FREE_ARRAY(_outline.contours);
                
					// too large points 
					m_bZoomLargeFlag = TRUE;
					return FALSE;
                
				}
			}
		}
        
        pGlyf->outline.outline.contour_num = _outline.n_contours;
        pGlyf->outline.outline.point_num   = _outline.n_points;
        //pGlyf->outline.outline.flags      = _outline.flags;        
        
		if(_outline.n_points>0)
		{
			pGlyf->outline.outline.points     = new XFT_VECTOR[_outline.n_points];	
			pGlyf->outline.outline.tags       = new BYTE[_outline.n_points];
			memcpy(pGlyf->outline.outline.points,_outline.points, _outline.n_points * sizeof(XFT_VECTOR));
			memcpy(pGlyf->outline.outline.tags,_outline.tags, _outline.n_points * sizeof(BYTE));
		}
		if(_outline.n_contours>0)
		{
			pGlyf->outline.outline.contours   = new short[_outline.n_contours];
			memcpy(pGlyf->outline.outline.contours,_outline.contours,_outline.n_contours * sizeof(short));
		}        
        
        //must release glyph
        FT_Done_Glyph(glyph);        

		SAFE_FREE_ARRAY(_outline.points);
		SAFE_FREE_ARRAY(_outline.tags);
		SAFE_FREE_ARRAY(_outline.contours);        
    }

	int error = RendOutline(pGlyf,nDispPtSize,nID);
	if(error != 0)
	{		
// 		CUIntArray * pag = NULL;
// 		if(m_mapIdun.Lookup(nID,pag))
// 		{
// 			CString str;
// 			str.Format(_T("0x%x"),pag->GetAt(0));
// 			AfxMessageBox(str);
// 		}
	}

	return TRUE;	
}

BOOL CGlyphData::IsIDRepeated(FT_SECTOR* pSec)
{
	DWORD dwGlyphID= 0;
	DWORD dwValue = 0;
	for(UINT j = pSec->start_count; j<= pSec->end_count;j++)
	{
		m_mapUnid.Lookup(j,dwGlyphID);
		if(m_mapRidun.Lookup(dwGlyphID,dwValue))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CGlyphData::IsIDSequence(FT_SECTOR* pSec,BOOL bModifyID,MAP_NUM &mapGlyfID)
{
	DWORD dwBaseID= 0;
	DWORD dwNextID= 0;


	m_mapUnid.Lookup(pSec->start_count,dwBaseID);

	if(bModifyID)
	{
		UINT dwBaseID2 = 0;
		mapGlyfID.Lookup(dwBaseID,dwBaseID2);
		dwBaseID = dwBaseID2;
	}

	for(UINT j = pSec->start_count+1; j<= pSec->end_count;j++)
	{
		m_mapUnid.Lookup(j,dwNextID);
		if(bModifyID)
		{
			UINT dwNextID2 = 0;
			mapGlyfID.Lookup(dwNextID,dwNextID2);
			dwNextID = dwNextID2;
		}
		if(( dwNextID - dwBaseID ) != 1)
		{
			return FALSE;
		}
		dwBaseID = dwNextID;
	}

	return TRUE;
}

void CGlyphData::ResetUniSector(CUIntArray &agUnicode,CUIntArray &agSector,BOOL bModifyID,MAP_NUM &mapGlyfID)
{
	UINT id_delta = 1;
	DWORD nBegin = 0;
	DWORD nEnd = 0;	
	DWORD glyf_id = 0;

	for(int k = 0 ; k< agUnicode.GetSize(); k++)
	{
		DWORD nUID = agUnicode[k];
		if(k==0)
		{
			nBegin = nUID;
			nEnd = nUID;
		}
		else
		{
			if((nUID-nEnd)!=1)
			{
				FT_SECTOR * pFS = new FT_SECTOR;
				pFS->start_count = (WORD)nBegin;
				pFS->end_count = (WORD)nEnd;

				if(bModifyID)
				{
					id_delta = 0;
					glyf_id=0;
					VERIFY(m_mapUnid.Lookup(nBegin,glyf_id));
					_ASSERTE(glyf_id != 0 );
					VERIFY(mapGlyfID.Lookup(glyf_id,id_delta));
					_ASSERTE(id_delta != 0 );
					pFS->id_delta  = (short)((WORD)id_delta - (WORD)nBegin);	
					pFS->id_range_offset = 0;
					//id_delta += (nEnd - nBegin +1);		
				}
				else
				{
					if(IsIDSequence(pFS,bModifyID,mapGlyfID))
					{
						glyf_id = 0;
						m_mapUnid.Lookup(nBegin,glyf_id);
						pFS->id_delta  = (short)((WORD)glyf_id-(WORD)nBegin);	
						pFS->id_range_offset = 0;
					}
					else
					{
						pFS->id_delta  = 0;
						pFS->id_range_offset = 0;
					}
				}

				agSector.Add((DWORD)pFS);
				
				nBegin = nUID;
				nEnd   = nUID;
			}
			else
			{
				nEnd = nUID;	
			}
		}
	}
	FT_SECTOR * pFS = new FT_SECTOR;
	pFS->start_count = (WORD)nBegin;
	pFS->end_count = (WORD)nEnd;
	if(bModifyID)
	{
		glyf_id=0;
		VERIFY(m_mapUnid.Lookup(nBegin,glyf_id));
		_ASSERTE(glyf_id != 0 );
		VERIFY(mapGlyfID.Lookup(glyf_id,id_delta));
		_ASSERTE(id_delta != 0 );
		pFS->id_delta  = (short)((WORD)id_delta - (WORD)nBegin);		
		pFS->id_range_offset =0;
		//id_delta += (nEnd - nBegin +1);

	}
	else
	{			
		if(IsIDSequence(pFS,bModifyID,mapGlyfID))
		{
			glyf_id = 0;
			m_mapUnid.Lookup(nBegin,glyf_id);
			pFS->id_delta  = (short)((WORD)glyf_id-(WORD)nBegin);	
			pFS->id_range_offset = 0;
		}
		else
		{
			pFS->id_delta  = 0;
			pFS->id_range_offset = 0;
		}
	}
	agSector.Add((DWORD)pFS);

	//must exist a sector 0xFFFF-0xFFFF
	if(((FT_SECTOR *)agSector[agSector.GetSize()-1])->end_count != 0xFFFF)
	{
		FT_SECTOR * pFSTmp = new FT_SECTOR;
		pFSTmp->end_count = 0xFFFF;
		pFSTmp->start_count = 0xFFFF;		
		pFSTmp->id_delta = 1;
		pFSTmp->id_range_offset = 0;
		agSector.Add((DWORD)pFSTmp);
	}

}

void CGlyphData::GetExtGlyfID(OTF_GSUB_GPOS *pGsubGpos,int gsub)
{
	if(pGsubGpos ==NULL)
		return;

	UINT ScriptCount = pGsubGpos->ScriptList.ScriptCount;
	if(ScriptCount == 0)
		return;

// 	{
// 		UINT nLookupCount = pGsubGpos->LookupList.LookupCount;
// 		for(UINT i=0; i< nLookupCount; i++)
// 		{
// 			OTF_Lookup * pLookup = pGsubGpos->LookupList.Lookup+i;
// 			UINT nSubTableCount = pLookup->SubTableCount;
// 			for(UINT n = 0; n< nSubTableCount; n++)
// 			{
// 				if(gsub)
// 				{
// 					DumpGsubGlyfID(pLookup->SubTable.gsub+n,pLookup->LookupType,pGsubGpos);
// 				}
// 				else
// 				{
// 					DumpGposGlyfID(pLookup->SubTable.gpos+n,pLookup->LookupType,pGsubGpos);
// 				}
// 			}
// 		}
// 	}
// 
// 	return;

	for(UINT i = 0; i< ScriptCount ; i++)
	{
		if(IsUsefulScriptTag(pGsubGpos->ScriptList.Script[i].ScriptTag))
		{
			UINT nFeatureCount = pGsubGpos->ScriptList.Script[i].DefaultLangSys.FeatureCount;
			UINT * pFeatureIndex = pGsubGpos->ScriptList.Script[i].DefaultLangSys.FeatureIndex;
			UINT nReqFeatureIndex = pGsubGpos->ScriptList.Script[i].DefaultLangSys.ReqFeatureIndex;

			MAP_NUM mapFeatureIdx;
			UINT j=0;
			for(j = 0;j<nFeatureCount;j++)
			{
				UINT nFI = pFeatureIndex[j];
				mapFeatureIdx.SetAt(nFI,nFI);
				UINT nLookupCount = pGsubGpos->FeatureList.Feature[nFI].LookupCount;
				for(UINT k = 0; k< nLookupCount;k++)
				{
					UINT nLI = pGsubGpos->FeatureList.Feature[nFI].LookupListIndex[k];
					OTF_Lookup * pLookup = pGsubGpos->LookupList.Lookup+nLI;
					TRACE(_T("-+-+-+-+-+li: %d\n"),nLI);
					UINT nSubTableCount = pLookup->SubTableCount;
					for(UINT n = 0; n< nSubTableCount; n++)
					{
						if(gsub)
						{
							DumpGsubGlyfID(pLookup->SubTable.gsub+n,pLookup->LookupType,pGsubGpos);
						}
						else
						{
							DumpGposGlyfID(pLookup->SubTable.gpos+n,pLookup->LookupType,pGsubGpos);
						}
					}
				}
			}
			UINT nValue;
			if(nReqFeatureIndex != 0xFFFF && !mapFeatureIdx.Lookup(nReqFeatureIndex,nValue) )
			{
				mapFeatureIdx.SetAt(nReqFeatureIndex,nReqFeatureIndex);
				UINT nLookupCount = pGsubGpos->FeatureList.Feature[nReqFeatureIndex].LookupCount;
				for(UINT k = 0; k< nLookupCount;k++)
				{
					UINT nLI = pGsubGpos->FeatureList.Feature[nReqFeatureIndex].LookupListIndex[k];
					OTF_Lookup * pLookup = pGsubGpos->LookupList.Lookup+nLI;				
					TRACE(_T("-+-+-+-+-+li: %d\n"),nLI);
					UINT nSubTableCount = pLookup->SubTableCount;
					for(UINT n = 0; n< nSubTableCount; n++)
					{
						if(gsub)
						{
							DumpGsubGlyfID(pLookup->SubTable.gsub+n,pLookup->LookupType,pGsubGpos);
						}
						else
						{
							DumpGposGlyfID(pLookup->SubTable.gpos+n,pLookup->LookupType,pGsubGpos);
						}
					}
				}
			}

			UINT nLangSysCount = pGsubGpos->ScriptList.Script[i].LangSysCount;
			OTF_LangSys * pLangSys = pGsubGpos->ScriptList.Script[i].LangSys;
			for(j=0;j<nLangSysCount;j++)
			{
				nFeatureCount = pLangSys[j].FeatureCount;
				pFeatureIndex = pLangSys[j].FeatureIndex;
				nReqFeatureIndex = pLangSys[j].ReqFeatureIndex;
				for(UINT k=0; k<nFeatureCount;k++)
				{
					UINT nFI = pFeatureIndex[k];	
					if(mapFeatureIdx.Lookup(nFI,nValue))
						continue;

					UINT nLookupCount = pGsubGpos->FeatureList.Feature[nFI].LookupCount;

					for(UINT l = 0; l< nLookupCount;l++)
					{
						UINT nLI = pGsubGpos->FeatureList.Feature[nFI].LookupListIndex[l];
						OTF_Lookup * pLookup = pGsubGpos->LookupList.Lookup+nLI;	
						TRACE(_T("-+-+-+-+-+li: %d\n"),nLI);
						UINT nSubTableCount = pLookup->SubTableCount;
						for(UINT n = 0; n< nSubTableCount; n++)
						{
							if(gsub)
							{
								DumpGsubGlyfID(pLookup->SubTable.gsub+n,pLookup->LookupType,pGsubGpos);
							}
							else
							{
								DumpGposGlyfID(pLookup->SubTable.gpos+n,pLookup->LookupType,pGsubGpos);
							}
						}
					}
				}

				if(nReqFeatureIndex != 0xFFFF && !mapFeatureIdx.Lookup(nReqFeatureIndex,nValue) )
				{
					mapFeatureIdx.SetAt(nReqFeatureIndex,nReqFeatureIndex);
					UINT nLookupCount = pGsubGpos->FeatureList.Feature[nReqFeatureIndex].LookupCount;
					for(UINT l = 0; l< nLookupCount;l++)
					{
						UINT nLI = pGsubGpos->FeatureList.Feature[nReqFeatureIndex].LookupListIndex[l];
						OTF_Lookup * pLookup = pGsubGpos->LookupList.Lookup+nLI;
						TRACE(_T("-+-+-+-+-+li: %d\n"),nLI);
						UINT nSubTableCount = pLookup->SubTableCount;
						for(UINT n = 0; n< nSubTableCount; n++)
						{
							if(gsub)
							{
								DumpGsubGlyfID(pLookup->SubTable.gsub+n,pLookup->LookupType,pGsubGpos);
							}
							else
							{
								DumpGposGlyfID(pLookup->SubTable.gpos+n,pLookup->LookupType,pGsubGpos);
							}
						}
					}
				}

			}

			
		}
	}
}

BOOL CGlyphData::IsUsefulScriptTag(UINT tag)
{
	if(m_pCSI == NULL)
		return FALSE;

	UINT stag = OTF_TAG(m_pCSI->tag[0],m_pCSI->tag[1],m_pCSI->tag[2],m_pCSI->tag[3]);

	if(stag == g_hidi)
	{
		for(int i = 0; i< sizeof(g_hidi_sub)/sizeof(g_hidi_sub[0]); i++)
		{
			if(tag == g_hidi_sub[i])
			{
				return TRUE;
			}
		}

	}
	else
	{
		if(tag == stag)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CGlyphData::DumpGsubGlyfID(OTF_LookupSubTableGSUB * subtable,UINT type,OTF_GSUB *gsub)
{
	switch (type)
	{
	case 1:
		if (subtable->Format == 1)
		{
			int* char_list = NULL;
			dump_coverage ( &subtable->Coverage,&char_list);
			//IPRINT ( file_fp, "(DeltaGlyhpID 0x%04X)",
			//	subtable->u.single1.DeltaGlyphID);
			//dump_glyph_ids((UINT*)&subtable->u.single1.DeltaGlyphID,1,0);
			for(int i = 0; char_list != NULL && char_list[i] != -1; i++)
			{
				InitGlyfInfo((UINT)(char_list[i]+subtable->u.single1.DeltaGlyphID),0);
			}
			if(char_list != NULL)
			{
				free(char_list);
			}
		}
		else if (subtable->Format == 2)
		{
			dump_coverage ( &subtable->Coverage,NULL);
			dump_glyph_ids ( subtable->u.single2.Substitute, subtable->u.single2.GlyphCount, 0);
		}
		else{			
			_ASSERTE(0);
		}
		break;
		
	case 2:
		if (subtable->Format == 1)
		{
			dump_coverage ( &subtable->Coverage,NULL);
			dump_sequence_list ( subtable->u.multiple1.Sequence,subtable->u.multiple1.SequenceCount);
		}
		else
			_ASSERTE(0);
		break;
		
	case 3:
		if (subtable->Format == 1)
		{
			dump_coverage ( &subtable->Coverage,NULL);
			dump_alternate_set_list (subtable->u.alternate1.AlternateSet,subtable->u.alternate1.AlternateSetCount );
		}
		else
			_ASSERTE(0);
		break;
		
	case 4:
		if (subtable->Format == 1)
		{			
			dump_coverage ( &subtable->Coverage, NULL);
			dump_ligature_set_list ( NULL,	subtable->u.ligature1.LigatureSet,
				                     subtable->u.ligature1.LigSetCount);
		}
		else
			_ASSERTE(0);
		break;
		
	case 5:
		if (subtable->Format == 1)
		{
			dump_coverage ( &subtable->Coverage, NULL);
			dump_rule_set_list (subtable->u.context1.RuleSet,
				                subtable->u.context1.RuleSetCount,gsub,1); 
		}
		else if (subtable->Format == 2)
		{
			dump_coverage ( &subtable->Coverage, NULL );
			dump_class_def ( &subtable->u.context2.ClassDef);
			dump_class_set_list ( subtable->u.context2.ClassSet,
				subtable->u.context2.ClassSetCnt,gsub,1);
		}
		else if (subtable->Format == 3)
		{
			dump_coverage_list ( subtable->u.context3.Coverage,	subtable->u.context3.GlyphCount);
			dump_lookup_record_list ( subtable->u.context3.LookupRecord,
				subtable->u.context3.LookupCount,gsub,1);
		}
		else
			_ASSERTE(0);
		break;
		
	case 6:
		if (subtable->Format == 1)
		{
			dump_coverage ( &subtable->Coverage, NULL);
			dump_chain_rule_set_list( subtable->u.chain_context1.ChainRuleSet,
				subtable->u.chain_context1.ChainRuleSetCount,gsub,1);
		}
		else if (subtable->Format == 2)
		{
			dump_coverage ( &subtable->Coverage, NULL);
			dump_class_def ( &subtable->u.chain_context2.BacktrackClassDef);
			dump_class_def ( &subtable->u.chain_context2.InputClassDef);
			dump_class_def (&subtable->u.chain_context2.LookaheadClassDef);
			dump_chain_class_set_list(subtable->u.chain_context2.ChainClassSet,
				subtable->u.chain_context2.ChainClassSetCnt,gsub,1);
		}
		else if (subtable->Format == 3)
		{
			dump_coverage_list
				( subtable->u.chain_context3.Backtrack,
				subtable->u.chain_context3.BacktrackGlyphCount);
			dump_coverage_list
				( subtable->u.chain_context3.Input,
				subtable->u.chain_context3.InputGlyphCount);
			dump_coverage_list
				(subtable->u.chain_context3.LookAhead,
				subtable->u.chain_context3.LookaheadGlyphCount);
			dump_lookup_record_list
				( subtable->u.chain_context3.LookupRecord,
				subtable->u.chain_context3.LookupCount,gsub,1);
		}
		else
			_ASSERTE(0);
		break;
		
	case 7:
		//IPRINT ( file_fp, "(ExtensionLookupType %d)",
		//	subtable->u.extension1.ExtensionLookupType);
		//IPRINT ( file_fp, "(ExtensionOffset %d)",
		//	subtable->u.extension1.ExtensionOffset);
		//dump_lookup_subtable_gsub ( file_fp, indent, index, 
		//	subtable->u.extension1.ExtensionLookupType,
		//	subtable->u.extension1.ExtensionSubtable, is_fontdata );		
		DumpGsubGlyfID(subtable->u.extension1.ExtensionSubtable,
			subtable->u.extension1.ExtensionLookupType,gsub);
		break;
		
	case 8:
		if (subtable->Format == 1)
		{
			dump_coverage ( &subtable->Coverage, NULL );
			
			dump_coverage_list( subtable->u.reverse_chain1.Backtrack,
				subtable->u.reverse_chain1.BacktrackGlyphCount);
			
			dump_coverage_list(subtable->u.reverse_chain1.LookAhead,
				subtable->u.reverse_chain1.LookaheadGlyphCount );
			
			dump_glyph_ids (subtable->u.reverse_chain1.Substitute,
				subtable->u.reverse_chain1.GlyphCount, 0);
		}

		break;
		
	default:
		break;
    }

}

void CGlyphData::DumpGposGlyfID(OTF_LookupSubTableGPOS * subtable,UINT type,OTF_GPOS *gpos)
{
	switch (type)
    {
    case 1:
		if (subtable->Format == 1)
		{		
			dump_coverage (&subtable->Coverage, NULL);
			//dump_value_record (indent, "Value", &subtable->u.single1.Value);
		}
		else if (subtable->Format == 2)
		{
			//int i;		
			dump_coverage (&subtable->Coverage,NULL);
			//for (i = 0; i < subtable->u.single2.ValueCount; i++)
			//	dump_value_record (indent, "Value", &subtable->u.single2.Value[i]);
		}
		else
		{
			//fprintf ( s_write_fp, " invalid");
			_ASSERTE(0);
		}
		break;
		
    case 2:
		if (subtable->Format == 1)
		{		
			dump_coverage (&subtable->Coverage, NULL);		
			dump_pair_set_list (subtable->u.pair1.PairSet, subtable->u.pair1.PairSetCount);
		}
		else if (subtable->Format == 2)
		{			
			dump_coverage (&subtable->Coverage, NULL);	

			//IPRINT ( s_write_fp, "(ValueFormat1 #x%04X)",
			//	subtable->u.pair2.ValueFormat1);
			//IPRINT ( s_write_fp, "(ValueFormat2 #x%04X)",
			//	subtable->u.pair2.ValueFormat2);
			dump_class_def (&subtable->u.pair2.ClassDef1);
			dump_class_def (&subtable->u.pair2.ClassDef2);
			//IPRINT ( s_write_fp, "(Class1Count %d)",
			//	subtable->u.pair2.Class1Count);
			//IPRINT ( s_write_fp, "(Class2Count %d)",
			//	subtable->u.pair2.Class2Count);
			//dump_class1_record_list (indent,
			//	subtable->u.pair2.Class1Count,
			//	subtable->u.pair2.Class2Count,
			//	subtable->u.pair2.Class1Record);
		}
		else
		{
			_ASSERTE(0);
			//fprintf ( s_write_fp, " invalid");
		}
		break;
		
    case 3:
		if (subtable->Format == 1)
		{
			dump_coverage (&subtable->Coverage, NULL);	
			//dump_entry_exit_list (indent, subtable->u.cursive1.EntryExitCount,
			//	subtable->u.cursive1.EntryExitRecord);
		}
		else
		{
			_ASSERTE(0);
			//fprintf ( s_write_fp, " invalid");
		}
		break;
		
    case 4:
		if (subtable->Format == 1)
		{
			dump_coverage (&subtable->Coverage, NULL);			
			dump_coverage (&subtable->u.mark_base1.BaseCoverage, NULL);	
			
			//IPRINT ( s_write_fp, "(ClassCount %d)",
			//	subtable->u.mark_base1.ClassCount);
			//dump_mark_array (indent, &subtable->u.mark_base1.MarkArray);
			//dump_anchor_array (indent, subtable->u.mark_base1.ClassCount,
			//	&subtable->u.mark_base1.BaseArray);
		}
		break;
		
    case 5:
		if (subtable->Format == 1)
		{
			OTF_GPOS_MarkLig1 *mark_lig1 = &subtable->u.mark_lig1;
			//UINT i/*, j, k*/;			
	
			dump_coverage (&subtable->Coverage, NULL);
			dump_coverage (&mark_lig1->LigatureCoverage, NULL);
		
			//IPRINT ( s_write_fp, "(ClassCount %d)", mark_lig1->ClassCount);
			//dump_mark_array (indent, &mark_lig1->MarkArray);
			//IPRINT ( s_write_fp, "(LigatureArray (%d)",
			//	mark_lig1->LigatureArray.LigatureCount);
			//indent++;
			//for (i = 0; i < mark_lig1->LigatureArray.LigatureCount; i++)
			//{
				//OTF_LigatureAttach *attach
				//	= mark_lig1->LigatureArray.LigatureAttach + i;
				
				//IPRINT ( s_write_fp, "(LigatureAttach (%d)", attach->ComponentCount);
				//indent++;
				//for (j = 0; j < attach->ComponentCount; j++)
				//{
				//	OTF_ComponentRecord *rec = attach->ComponentRecord + j;
				//	
				//	IPRINT ( s_write_fp, "(LigatureAnchor (%d)", mark_lig1->ClassCount);
				//	for (k = 0; k < mark_lig1->ClassCount; k++)
				//		if (rec->LigatureAnchor[k].AnchorFormat)
				//			dump_anchor (indent + 1, rec->LigatureAnchor + k);
				//		fprintf ( s_write_fp, ")");
				//}
				//fprintf ( s_write_fp, ")");
				//indent--;
			//}
			//fprintf ( s_write_fp, ")");
		}
		else
		{
			_ASSERTE(0);
			//fprintf ( s_write_fp, " invalid");
		}
		break;
		
    case 6:
		if (subtable->Format == 1)
		{
			dump_coverage (&subtable->Coverage, NULL);
			dump_coverage (&subtable->u.mark_mark1.Mark2Coverage, NULL);

			//IPRINT ( s_write_fp, "(ClassCount %d)",
			//	subtable->u.mark_mark1.ClassCount);
			//dump_mark_array (indent, &subtable->u.mark_mark1.Mark1Array);
			//dump_anchor_array (indent, subtable->u.mark_mark1.ClassCount,
			//	&subtable->u.mark_mark1.Mark2Array);
		}
		else
		{
			_ASSERTE(0);
			//fprintf ( s_write_fp, " invalid");
		}
		break;
		
    case 7:
		if (subtable->Format == 1)
		{		
			dump_coverage (&subtable->Coverage, NULL);		
			dump_rule_set_list (subtable->u.context1.RuleSet,
				subtable->u.context1.RuleSetCount,gpos,0); 
		}
		else if (subtable->Format == 2)
		{
		
			dump_coverage (&subtable->Coverage, NULL);
			dump_class_def (&subtable->u.context2.ClassDef);
			dump_class_set_list (subtable->u.context2.ClassSet,
				subtable->u.context2.ClassSetCnt,gpos,0);
		}
		else if (subtable->Format == 3)
		{
			dump_coverage_list (subtable->u.context3.Coverage,
				subtable->u.context3.GlyphCount);
			dump_lookup_record_list (subtable->u.context3.LookupRecord,
				subtable->u.context3.LookupCount,gpos,0);
		}
		else
		{
			_ASSERTE(0);
			//fprintf ( s_write_fp, " invalid");
		}
		break;
		
    case 8:
		if (subtable->Format == 1)
		{		
			dump_coverage (&subtable->Coverage, NULL);		
			dump_chain_rule_set_list
				(subtable->u.chain_context1.ChainRuleSet,
				subtable->u.chain_context1.ChainRuleSetCount,gpos,0);
		}
		else if (subtable->Format == 2)
		{		
			dump_coverage (&subtable->Coverage, NULL);	
			
			dump_class_def (&subtable->u.chain_context2.BacktrackClassDef);
			dump_class_def (&subtable->u.chain_context2.InputClassDef);
			dump_class_def (&subtable->u.chain_context2.LookaheadClassDef);
			dump_chain_class_set_list
				(subtable->u.chain_context2.ChainClassSet,
				subtable->u.chain_context2.ChainClassSetCnt,gpos,0);
		}
		else if (subtable->Format == 3)
		{
			dump_coverage_list
				(subtable->u.chain_context3.Backtrack,
				subtable->u.chain_context3.BacktrackGlyphCount);
			dump_coverage_list
				(subtable->u.chain_context3.Input,
				subtable->u.chain_context3.InputGlyphCount);
			dump_coverage_list
				(subtable->u.chain_context3.LookAhead,
				subtable->u.chain_context3.LookaheadGlyphCount);
			dump_lookup_record_list
				(subtable->u.chain_context3.LookupRecord,
				subtable->u.chain_context3.LookupCount,gpos,0);
		}
		else
		{
			_ASSERTE(0);
			//fprintf ( s_write_fp, " invalid");
		}
		break;
		
    case 9:
		if (subtable->Format == 1)
		{
			//IPRINT ( s_write_fp, "(ExtensionLookupType %d)",
			//	subtable->u.extension1.ExtensionLookupType);
			//IPRINT ( s_write_fp, "(ExtensionOffset %d)",
			//	subtable->u.extension1.ExtensionOffset);
			DumpGposGlyfID(	subtable->u.extension1.ExtensionSubtable,
				subtable->u.extension1.ExtensionLookupType,gpos);
		}
		else
		{
			_ASSERTE(0);
			//fprintf ( s_write_fp, " invalid");
		}
    }
	//fprintf ( s_write_fp, ")");
}

void CGlyphData::dump_glyph_ids ( OTF_GlyphID *ids, unsigned count, unsigned end_num/*最后循环的数值, 0或1*/ )
{

    //IPRINT ( file_fp, "(%s (count %d)", title, count);

    while (count-- > end_num)
    {
        //fprintf ( file_fp, " 0x%04X", *ids);
		InitGlyfInfo(*ids,0);
		TRACE(_T("-----id: 0x%X\n"),*ids);
        ids++;
    }
}

void CGlyphData::dump_coverage ( OTF_Coverage *coverage,int** char_list_pptr)
{
    UINT i = 0;
    int *char_list = NULL;    

    if (coverage->CoverageFormat == 1)
    {
        dump_glyph_ids ( coverage->table.GlyphArray, coverage->Count, 0);
        
        if ( NULL != char_list_pptr )
        {
            char_list = (int*)malloc (sizeof (int) * (coverage->Count + 1));
            for (i = 0; i < coverage->Count; i++)
            {
                char_list[i] = coverage->table.GlyphArray[i];
				TRACE(_T("++++++++id: 0x%X\n"),coverage->table.GlyphArray[i]);	
            }
            char_list[i] = -1;

            *char_list_pptr = char_list;
        }
    }
    else
    {
        UINT n, c;
        

		for (i = n = 0; i < coverage->Count; i++)
		{
			//IPRINT ( file_fp, "(Range (%d) (Start 0x%04X) (End 0x%04X)", i,
			//	coverage->table.RangeRecord[i].Start,
			//	coverage->table.RangeRecord[i].End);
			//indent++;
			//IPRINT ( file_fp, "(StartCoverageIndex %d))",
			//	coverage->table.RangeRecord[i].StartCoverageIndex);
			//indent--;
			n += (coverage->table.RangeRecord[i].End
				- coverage->table.RangeRecord[i].Start + 1);
		}
		
        if ( NULL != char_list_pptr )
        {
            char_list = (int *)malloc (sizeof (int) * (n + 1));
            for (i = n = 0; i < coverage->Count; i++)
            {
                for (c = coverage->table.RangeRecord[i].Start;
                c <= coverage->table.RangeRecord[i].End;
                c++)
                {
                    char_list[n++] = c;
					TRACE(_T("++++++++id: 0x%X\n"),c);
					InitGlyfInfo(c,0);
                }
            }
            char_list[n] = -1;
            
            *char_list_pptr = char_list;
        }

    }

}

void CGlyphData::dump_coverage_list ( OTF_Coverage *coverage, unsigned num )
{
    //IPRINT ( file_fp, "(%s %d)", title, num);
    for (UINT i = 0; i < num; i++)
    {
        dump_coverage ( coverage + i,NULL);
	}
}

void CGlyphData::dump_sequence_list ( OTF_Sequence *sequence, unsigned num)
{
    UINT i = 0;    

    //IPRINT ( file_fp, "(SequenceCount %d)", num);
    
    for (i = 0; i < num; i++)
    {
        //IPRINT ( file_fp, "(Sequence (%d) (offset 0x%04X)",
        //    i, sequence[i].offset);
        dump_glyph_ids ( sequence[i].Substitute, sequence[i].GlyphCount, 0);
       // fprintf ( file_fp, ")");
    }    

}

void CGlyphData::dump_alternate_set_list ( OTF_AlternateSet *altset, unsigned num )
{

    UINT i = 0;    

    //IPRINT ( file_fp, "(AlternateSetCount %d)", num);
    for (i = 0; i < num; i++)
    {
        //IPRINT ( file_fp, "(AlternateSet (%d) (offset 0x%04X)",
        //    i, altset[i].offset);
        dump_glyph_ids ( altset[i].Alternate,altset[i].GlyphCount, 0);
        //fprintf ( file_fp, ")");
    }
   
}

void CGlyphData::dump_ligature_set_list ( int *char_list, OTF_LigatureSet *ligset, unsigned num )
{
    UNREFERENCED_PARAMETER(char_list);
    UINT i, j;// k = 0;

    //IPRINT ( file_fp, "(LigSetCount %d)", num);
    for (i = 0; i < num; i++)
    {
        //IPRINT ( file_fp, "(LigatureSet (%d) (offset 0x%04X) (count %d)",
        //    i, ligset[i].offset, ligset[i].LigatureCount);
        //indent++;
        for (j = 0; j < ligset[i].LigatureCount; j++)
        {
            //IPRINT ( file_fp, "(Ligature (%d) (offset 0x%04X)",
            //    j, ligset[i].Ligature[j].offset);
            //indent++;
            //IPRINT ( file_fp, "(LigGlyph 0x%04X)",
            //    ligset[i].Ligature[j].LigGlyph);
            dump_glyph_ids ( ligset[i].Ligature[j].Component, ligset[i].Ligature[j].CompCount, 1 );
			dump_glyph_ids ( &ligset[i].Ligature[j].LigGlyph,1,0);
            
            //IPRINT ( file_fp, "(i.e. 0x%04X", char_list[i]);
            //for (k = 0; k < ligset[i].Ligature[j].CompCount - 1; k++)
             //   fprintf ( file_fp, " 0x%04X", ligset[i].Ligature[j].Component[k]);
            //fprintf ( file_fp, " = 0x%04X)", ligset[i].Ligature[j].LigGlyph);
            //fprintf ( file_fp, ")");
            //indent--;
        }
        //indent--;
        //fprintf ( file_fp, ")");
    }
}
void CGlyphData::dump_rule_list ( OTF_Rule *rule, int count,OTF_GSUB_GPOS * gsubgpos ,int gsub)
{
    int i = 0;   

    //IPRINT ( file_fp, "(RuleCount %d)", count);
    for (i = 0; i < count; i++)
    {
//        IPRINT ( file_fp, "(Rule (%d)", i);
//        indent++;
//        IPRINT ( file_fp, "(GlyphCount %d)", rule[i].GlyphCount);
//       IPRINT ( file_fp, "(LookupCount %d)", rule[i].LookupCount);
        dump_glyph_ids (  rule[i].Input, rule[i].GlyphCount, 1 );
        dump_lookup_record_list (  rule[i].LookupRecord,
            rule[i].LookupCount, gsubgpos,gsub);
//        fprintf ( file_fp, ")");
//        indent--;
    }

    

}

void CGlyphData::dump_rule_set_list ( OTF_RuleSet *set, int count,OTF_GSUB_GPOS * gsubgpos, int gsub )
{
    int i = 0;    

//    IPRINT ( file_fp, "(RuleSetCount %d)", count);
    for (i = 0; i < count; i++)
    {
//        IPRINT ( file_fp, "(RuleSet (%d)", i);
        dump_rule_list ( set[i].Rule, set[i].RuleCount,gsubgpos,gsub);
//        fprintf ( file_fp, ")");
    }
}

void CGlyphData::dump_chain_rule_list (OTF_ChainRule *rule, int count,OTF_GSUB_GPOS * gsubgpos, int gsub )
{
    int i = 0;   

    //IPRINT ( file_fp, "(ChainRuleCount %d)", count);
    for (i = 0; i < count; i++)
    {
    //    IPRINT ( file_fp, "(ChainRule (%d)", i);
        dump_glyph_ids ( rule[i].Backtrack,
            rule[i].BacktrackGlyphCount, 0);
        dump_glyph_ids ( rule[i].Input,
            rule[i].InputGlyphCount, 1 );
        dump_glyph_ids (rule[i].LookAhead,
            rule[i].LookaheadGlyphCount, 0);
        dump_lookup_record_list ( rule[i].LookupRecord,
            rule[i].LookupCount, gsubgpos,gsub );
    //    fprintf ( file_fp, ")");
    }

}

void CGlyphData::dump_chain_rule_set_list ( OTF_ChainRuleSet *set, int count,OTF_GSUB_GPOS * gsubgpos, int gsub )
{
    int i = 0;
//    IPRINT ( file_fp, "(ChainRuleSetCount %d)", count);
    for (i = 0; i < count; i++)
    {
//        IPRINT ( file_fp, "(ChainRuleSet (%d)", i);
        dump_chain_rule_list ( set[i].ChainRule, set[i].ChainRuleCount,gsubgpos,gsub);
//        fprintf ( file_fp, ")");
    }
}
void CGlyphData::dump_chain_class_rule_list (OTF_ChainClassRule *rule, int count,OTF_GSUB_GPOS * gsubgpos, int gsub)
{
    int i = 0;
    //IPRINT ( file_fp, "(ChainClassRuleCount %d)", count);
    for (i = 0; i < count; i++)
    {
        //IPRINT ( file_fp, "(ChainClassRule (%d)", i);
        dump_glyph_ids ( rule[i].Backtrack, rule[i].BacktrackGlyphCount, 0 );
        dump_glyph_ids ( rule[i].Input, rule[i].InputGlyphCount, 1 );
        dump_glyph_ids ( rule[i].LookAhead, rule[i].LookaheadGlyphCount, 0 );
        dump_lookup_record_list (  rule[i].LookupRecord,
            rule[i].LookupCount, gsubgpos, gsub );
        //fprintf ( file_fp, ")");
    }   
}

void CGlyphData::dump_chain_class_set_list ( OTF_ChainClassSet *set, int count,OTF_GSUB_GPOS * gsubgposg, int gsub)
{
    int i = 0;

    //IPRINT ( file_fp, "(ChainClassSetCount %d)", count);
    for (i = 0; i < count; i++)
    {
//        IPRINT ( file_fp, "(ChainClassSet (%d)", i);
        if ( set[i].offset )
        {
            dump_chain_class_rule_list ( set[i].ChainClassRule, set[i].ChainClassRuleCnt,gsubgposg,gsub);
        }
//        fprintf ( file_fp, ")");
    }

}

void CGlyphData::dump_class_def (OTF_ClassDef *class_ptr )
{
  
//    IPRINT ( file_fp, "(%s (offset 0x%04X) (ClassFormat %d)",
//        (title ? title : "ClassDef"),
//        class_ptr->offset, class_ptr->ClassFormat);
    if (class_ptr->offset)
    {
//        indent++;
        if (class_ptr->ClassFormat == 1)
        {
//            IPRINT ( file_fp, "(StartGlyph 0x%04X)", class_ptr->f.f1.StartGlyph);
              dump_glyph_ids ( (OTF_GlyphID *) class_ptr->f.f1.ClassValueArray,
                class_ptr->f.f1.GlyphCount, 0);
        }
        else if (class_ptr->ClassFormat == 2)
        {
//            int i;
            
//             IPRINT ( file_fp, "(ClassRangeCount %d)", class_ptr->f.f2.ClassRangeCount);
//             IPRINT ( file_fp, "(ClassRangeRecord");
//             indent++;
//             for (i = 0; i < class_ptr->f.f2.ClassRangeCount; i++)
//                 IPRINT ( file_fp, "((Start 0x%04X) (End 0x%04X) (class_ptr %d))",
//                 class_ptr->f.f2.ClassRangeRecord[i].Start,
//                 class_ptr->f.f2.ClassRangeRecord[i].End,
//                 class_ptr->f.f2.ClassRangeRecord[i].Class);
//             fprintf ( file_fp, ")");
        }
//        else
//            fprintf ( file_fp, " UnknownClassFormat");
    }
//    fprintf ( file_fp, ")");

}

void CGlyphData::dump_class_set_list ( OTF_ClassSet *set, int count, OTF_GSUB_GPOS * gsubgpos, int gsub)
{

    int i = 0;    

//    IPRINT ( file_fp, "(ClassSetCount %d)", count);
    for (i = 0; i < count; i++)
    {
        /* Offset can be zero.  */
        if (set[i].offset)
        {
 //           IPRINT ( file_fp, "(ClassSet (%d)", i);
            dump_class_rule_list (set[i].ClassRule,   set[i].ClassRuleCnt, gsubgpos,gsub);
  //          fprintf ( file_fp, ")");
        }
    }

}

void CGlyphData::dump_class_rule_list ( OTF_ClassRule *rule, int count, OTF_GSUB_GPOS * gsubgpos, int gsub)
{
	UNREFERENCED_PARAMETER(rule);

    int i;// j = 0;
 
//     IPRINT ( file_fp, "(ClassRuleCnt %d)", count);
    for (i = 0; i < count; i++)
    {
//         IPRINT ( file_fp, "(ClassRule (%d)", i);
//         indent++;
//         IPRINT ( file_fp, "(GlyphCount %d)", rule[i].GlyphCount);
//         IPRINT ( file_fp, "(LookupCount %d)", rule[i].LookupCount);
//         IPRINT ( file_fp, "(Class");
//         for (j = 0; j < rule[i].GlyphCount - 1; j++)
//         {
//             fprintf ( file_fp, " %d", rule[i].Class[j]);
//         }
//         fprintf ( file_fp, ")");
        dump_lookup_record_list ( rule[i].LookupRecord,
            rule[i].LookupCount, gsubgpos,gsub);
//         fprintf ( file_fp, ")");
//         indent--;
    }

}

void CGlyphData::dump_lookup_record_list ( OTF_LookupRecord *rec, unsigned num,OTF_GSUB_GPOS * gsubgpos /*= NULL*/,int gsub)
{
  
    UINT i = 0;    

//    IPRINT ( file_fp, "(LookupCount %d)", num);
    for (i = 0; i < num; i++)
    {
//        IPRINT ( file_fp, "(LookupRecord (%d)", i);
//        indent++;
//        IPRINT ( file_fp, "(SequenceIndex %d)", rec[i].SequenceIndex);
//        IPRINT ( file_fp, "(LookupListIndex %d))", rec[i].LookupListIndex);

		
		OTF_Lookup * pLookup = gsubgpos->LookupList.Lookup+ rec[i].LookupListIndex;

		TRACE(_T("-+-+-+-+-+li: %d\n"),rec[i].LookupListIndex);

		UINT nSubTableCount = pLookup->SubTableCount;
		for(UINT n = 0; n< nSubTableCount; n++)
		{
			if(gsub)
			{
				DumpGsubGlyfID(pLookup->SubTable.gsub+n,pLookup->LookupType,gsubgpos);
			}
			else
			{
				DumpGposGlyfID(pLookup->SubTable.gpos+n,pLookup->LookupType,gsubgpos);
			}
		}

//        indent--;
    }

}

void CGlyphData::dump_pair_set_list (OTF_PairSet *set, unsigned count)
{
	UINT i, j;
	
	for (i = 0; i < count; i++)
    {
		//IPRINT ( s_write_fp, "(PairSet (%d)", i);
		//indent++;
		for (j = 0; j < set[i].PairValueCount; j++)
		{
			//IPRINT ( s_write_fp, "(PairValueRecord (%d)", j);
			//indent++;
			//IPRINT ( s_write_fp, "(SecondGlyph #x%04X)",
			//	set[i].PairValueRecord[j].SecondGlyph);
			//dump_value_record (indent, "Value1",
			//	&set[i].PairValueRecord[j].Value1);
			//dump_value_record (indent, "Value2",
			//	&set[i].PairValueRecord[j].Value2);
			//fprintf ( s_write_fp, ")");
			//indent--;
			InitGlyfInfo(set[i].PairValueRecord[j].SecondGlyph,0);
		}
		//fprintf ( s_write_fp, ")");
		//indent--;
    }
}


void CGlyphData::InitGlyfInfo(UINT glyfid,UINT unicode)
{
	if(glyfid >= 0xFFFF)
		return;

	int nFtNum = m_fontHDR.glyph_table_count;
	GLYPH_BODY_PTR pGlyph = NULL;
	if(!m_mapGlyf.Lookup(glyfid,pGlyph))
	{
		pGlyph = new GLYPH_BODY_T[nFtNum];
		for(int k=0; k<nFtNum;k++)
		{
			pGlyph[k].Init();	
		}
		m_mapGlyf.SetAt(glyfid,pGlyph);
		m_lstID.push_back(glyfid);
	}

	/*  map unicode to glyph id */
	m_mapUnid.SetAt(unicode,glyfid);

	/*  map glyph id to unicode */
	UnicodeArray_ptr pIDUA= NULL;
	if(m_mapIdun.Lookup(glyfid,pIDUA))
	{
		int nSize = pIDUA->GetSize();
		for(int i=0; i< nSize ; i++)
		{
			if(unicode== pIDUA->GetAt(i))
			{
				break;
			}
		}
		if(i>=nSize)
		{
			pIDUA->Add(unicode);
		}
	}
	else
	{
		pIDUA = new CUIntArray;
		pIDUA->Add(unicode);
		m_mapIdun.SetAt(glyfid,pIDUA);				
	}
}


long CGlyphData::FixedFromDouble(double d)
{
	long l;
	l = (long)(d*65536L);
	return l;
}

void CGlyphData::DumpOutlineLog(LPCTSTR lpszFileName, BOOL bRead)
{    
    if( g_theApp.m_nLogMode==0 ||m_pGlyphTables == NULL || 
		!( m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR_CMP || m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR ) )
    {
    	return;
    }
    

	CString strLog = lpszFileName;
	int nFind = strLog.ReverseFind(_T('.'));
	if(nFind != -1)
	{
		strLog = strLog.Left(nFind);			
	}

    if(bRead)
    {
		strLog += _T("_r.log");
    }
    else
    {
        strLog += _T("_w.log");
    }

	m_pTestFile = _tfopen(strLog,_T("wt"));
    if(NULL == m_pTestFile )
    {
        return;
    }


    CUIntArray agID;
    CUIntArray agUni;

    int i = 0;

    DWORD dwGlyphID;

	// get real existed unicode of glyph
	for(i=0;i<m_pCSI->nSectorNum;i++)
	{
		FT_SECTOR fs;		
		fs.start_count = (WORD)m_pCSI->pUCBoundTable[i].dwUCStart;
		fs.end_count = (WORD)m_pCSI->pUCBoundTable[i].dwUCEnd;	
		for(DWORD j = fs.start_count; j<=fs.end_count;j++)
		{
			if(m_mapUnid.Lookup(j,dwGlyphID))
			{
				agID.Add(dwGlyphID);
                agUni.Add(j);
			}	
		}
	}


    for(int j = 0; j< agID.GetSize(); j++)
    {     
        GLYPH_BODY_PTR pGlyf = NULL;
        if(!m_mapGlyf.Lookup(agID[j],pGlyf))
        {
            continue;
        }
        
        short nTmp2 = 0;
        
        if(m_pTestFile != NULL)
        {
            fprintf(m_pTestFile,"unicode: 0x%X\r\n",agUni[j]);
			if(g_theApp.m_nLogMode == 1) // outline
			{
				fprintf(m_pTestFile,"advance: 0x%X\r\n",pGlyf->metrics.dwidth);

				fprintf(m_pTestFile,"n_contous:\t%d\r\npoint_num:\t%d\r\n",pGlyf->outline.outline.contour_num,pGlyf->outline.outline.point_num);
            
				fprintf(m_pTestFile,"points(x):\r\n");
				for(i=0;i<pGlyf->outline.outline.point_num;i++)
				{
					nTmp2 = (short)pGlyf->outline.outline.points[i].x;
                
					if( (i%16) == 0 && i!= 0)
					{
						fprintf(m_pTestFile,"\r\n");
					}
					fprintf(m_pTestFile,"%04X ",nTmp2);			
				}
				fprintf(m_pTestFile,"\r\n");
				fprintf(m_pTestFile,"points(y):\r\n");
				for(i=0;i<pGlyf->outline.outline.point_num;i++)
				{
					nTmp2 = (short)pGlyf->outline.outline.points[i].y;
                
                
					if( (i%16) == 0 && i!= 0)
					{
						fprintf(m_pTestFile,"\r\n");
					}
                
					fprintf(m_pTestFile,"%04X ",nTmp2);		
                
				}
				fprintf(m_pTestFile,"\r\n");
				fprintf(m_pTestFile,"flags:\r\n");
				for(i=0;i<pGlyf->outline.outline.point_num;i++)
				{
					nTmp2 = pGlyf->outline.outline.tags[i];
					if( (i%16) == 0 && i!= 0)
					{
						fprintf(m_pTestFile,"\r\n");
					}
                
					fprintf(m_pTestFile,"%02X ",(BYTE)nTmp2);
                
                
				}
				fprintf(m_pTestFile,"\r\n");
				fprintf(m_pTestFile,"contours:\r\n");
				for(i=0;i<pGlyf->outline.outline.contour_num;i++)
				{
					nTmp2 = pGlyf->outline.outline.contours[i];
					if( (i%16) == 0 && i!= 0)
					{
						fprintf(m_pTestFile,"\r\n");
					}
					fprintf(m_pTestFile,"%04X ",(WORD)nTmp2);
                
				}
				fprintf(m_pTestFile,"\r\n");
				fprintf(m_pTestFile,"-----------------------------------------------\r\n");
			}
			else if(g_theApp.m_nLogMode == 2) // bitmap
			{
				for(int h =  0; h< pGlyf->height; h++)
				{
					for(int w= 0; w< pGlyf->width; w++)
					{
						fprintf(m_pTestFile,"%02X ",(BYTE)pGlyf->bitmap[h*pGlyf->width + w]);
					}
					fprintf(m_pTestFile,"\r\n");
				}
			}


        }
    }

    if(NULL != m_pTestFile)
    {

        fclose(m_pTestFile);
        m_pTestFile = NULL;
    }
}

int  CGlyphData::WriteGlyfOutline(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian)
{
//	int nTmp4 = 0;
	short nTmp2 = 0;
	char nTmp1 = 0;

	int nSize = 0;
	int i= 0;
	nTmp2 = pGlyf->outline.outline.point_num;
	if(bBigEndian)
	{
		nTmp2 = Conv16Endian(nTmp2);
	}
	nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);

	nTmp2 = pGlyf->outline.outline.contour_num;
	if(bBigEndian)
	{
		nTmp2 = Conv16Endian(nTmp2);
	}
	nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);

	for(i=0; i< pGlyf->outline.outline.point_num; i++)
	{
// 		nTmp4 = pGlyf->outline.outline.points[i].x;
// 		nTmp2 = (short)nTmp4;
// 		if(bBigEndian)
// 		{
// 			nTmp2 = Conv16Endian(nTmp2);
// 		}
// 		nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
// 		
// 		nTmp4 = pGlyf->outline.outline.points[i].y;
// 		nTmp2 = (short)nTmp4;
// 		if(bBigEndian)
// 		{
// 			nTmp2 = Conv16Endian(nTmp2);
// 		}
// 		nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);	
		nTmp1 = (char)pGlyf->outline.outline.points[i].x;
		nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);

		nTmp1 = (char)pGlyf->outline.outline.points[i].y;
		nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);

	}


//	nSize +=fwrite(pGlyf->outline.tags,1,sizeof(char)*pGlyf->outline.outline.point_num,pFile);
#if 0
	int nTagCmpNum = (pGlyf->outline.outline.point_num + 1)/2;
	for(i=0;i<nTagCmpNum;i++)
	{
		nTmp1 = 0;
		nTmp1 = pGlyf->outline.tags[2*i];
		nTmp1 = (char)(nTmp1 << 4);
		if(pGlyf->outline.outline.point_num > (2*i+1))
		{
			nTmp1 = (char)(nTmp1 | (pGlyf->outline.tags[2*i+1] & 0xF));
		}

		nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
	}
#else
	int nTagCmpNum = (pGlyf->outline.outline.point_num )/4;
    int nTagLeft = pGlyf->outline.outline.point_num%4;
	
	for(i=0;i<nTagCmpNum;i++)
	{
        nTmp1 = (char)(pGlyf->outline.outline.tags[4*i]<<6);
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*i+1]<<4));
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*i+2]<<2));
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*i+3]));
	
        nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
	}
	nTmp1 = 0;
    for(i=0;i<nTagLeft;i++)
	{		
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*nTagCmpNum + i])<<(6-2*i));
		if(i== (nTagLeft - 1))
		{
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
	}
#endif


	for(i=0;i<pGlyf->outline.outline.contour_num;i++)
	{
		nTmp2 = pGlyf->outline.outline.contours[i];
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
	}
	
	return nSize;
}

int  CGlyphData::WriteComposeGlyf(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian)
{
	int nSize = 0;
	short nTmp2 = 0;
	char  nTmp1 = 0;

	nTmp2 = pGlyf->nNumberSubGlyph;
	if(bBigEndian)
	{
		nTmp2 = Conv16Endian(nTmp2);
	}
	nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);

	for(int i = 0;i< pGlyf->nNumberSubGlyph; i++)
	{
		X_SUBGLYPH_PTR pSub = pGlyf->pSubGlyf + i;

        //flag
        nTmp2 = pSub->flags;
        if(bBigEndian)
        {
            nTmp2 = Conv16Endian(nTmp2);
        }
        nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
        
        //index
        nTmp2 = pSub->index;
        if(bBigEndian)
        {
            nTmp2 = Conv16Endian(nTmp2);
        }
        nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);

        /* write arguments */
        if ( pSub->flags & SUBGLYPH_ARGS_ARE_WORDS )
        {
            nTmp2 = pSub->arg1;
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
            
            nTmp2 = pSub->arg2;
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
        }
        else
        {
            nTmp1 = (char)pSub->arg1;
            nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
            
            nTmp1 = (char)pSub->arg2;
            nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
        }
        
        /* write transform */
        if ( pSub->flags & SUBGLYPH_WE_HAVE_A_SCALE )
        {
            nTmp2 = (short)(pSub->transform.xx >> 2);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
        }
        else if ( pSub->flags & SUBGLYPH_WE_HAVE_AN_XY_SCALE )
        {
            nTmp2 = (short)(pSub->transform.xx >> 2);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
            
            nTmp2 = (short)(pSub->transform.yy >> 2);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
        }
        else if ( pSub->flags & SUBGLYPH_WE_HAVE_A_2X2 )
        {
            nTmp2 = (short)(pSub->transform.xx >> 2);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
            
            nTmp2 = (short)(pSub->transform.yx >> 2);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
            
            nTmp2 = (short)(pSub->transform.xy >> 2);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
            
            nTmp2 = (short)(pSub->transform.yy >> 2);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
        }
	}
	
	return nSize;
}


int  CGlyphData::WriteGlyfOutlineEx(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian)
{
	// pre-analyze if need to compress
	BYTE flag = CalcOutlineCmpFlag(pGlyf);

	short nTmp2 = 0;
	char nTmp1 = 0;
	BYTE nTmpB   = 0;

	int nSize = 0;
	int i= 0;

	// write flag first
	nSize += fwrite(&flag,1,sizeof(flag),pFile);

	// write points number
	if(flag & OF_POINTS_NUM_BYTE)
	{
		nTmp1 = (char)pGlyf->outline.outline.point_num;
		nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
	}
	else
	{
		nTmp2 = pGlyf->outline.outline.point_num;
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
	}

	
	// write contours number
	if(flag & OF_CONTOURS_NUM_BYTE)
	{
		nTmp1 = (char)pGlyf->outline.outline.contour_num;
		nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
	}
	else
	{
		nTmp2 = pGlyf->outline.outline.contour_num;
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
	}

	int nPointsNum = pGlyf->outline.outline.point_num;
	int nFlagNum = nPointsNum-1; // not store first flag
	int nFlagSize = (nFlagNum + 7)/8; 
	int nLeft = nFlagNum % 8;

	// write X flag, 1,repeated X; 0, noraml
	if(flag & OF_CMP_X )
	{		
		_ASSERTE(nFlagNum != 0);

		int nCount = 0;
		BYTE *pFlagX = new BYTE[nFlagNum];
		memset(pFlagX,0,nFlagNum);
		for(i = 1; i< nPointsNum; i++)
		{
			if( pGlyf->outline.outline.points[i].x == pGlyf->outline.outline.points[i-1].x)
			{
				pFlagX[i-1]=1;
			}
			if( (i % 8) ==0) // 8 Bytes compress to 1 Byte
			{
				nTmpB = 0;
				for(int j = 0; j< 8; j++)
				{
					nTmpB |= (pFlagX[i-j-1] << j);
				}
				nSize += fwrite(&nTmpB,1,sizeof(nTmpB),pFile);
				nCount ++;
			}
			else if(i== (nPointsNum -1)) // deal with left bytes
			{
				nTmpB = 0;				
				for(int j = 0; j< nLeft; j++)
				{
					nTmpB |= (pFlagX[i-j-1] << (j+8-nLeft));
				}
				nSize += fwrite(&nTmpB,1,sizeof(nTmpB),pFile);
				nCount ++;
			}
		}
		_ASSERTE(nCount == nFlagSize);
		delete []pFlagX;

	}

	// write Y flag, 1,repeated X; 0, noraml
	if(flag & OF_CMP_Y)
	{
		int nCount = 0;

		BYTE *pFlagY = new BYTE[nFlagNum];
		memset(pFlagY,0,nFlagNum);

		for( i = 1; i< nPointsNum; i++)
		{
			if(pGlyf->outline.outline.points[i].y == pGlyf->outline.outline.points[i-1].y)
			{
				pFlagY[i-1]=1;
			}
			if( (i % 8)==0) // 8 Bytes compress to 1 Byte
			{
				nTmpB = 0;
				for(int j = 0; j< 8; j++)
				{
					nTmpB |= (pFlagY[i-j-1] << j);
				}
				nSize += fwrite(&nTmpB,1,sizeof(nTmpB),pFile);
				nCount ++;
			}
			else if(i== (nPointsNum -1)) // deal with left bytes
			{
				nTmpB = 0;
				for(int j = 0; j< nLeft; j++)
				{
					nTmpB |= (pFlagY[i-j-1] << (j+8-nLeft));
				}
				nSize += fwrite(&nTmpB,1,sizeof(nTmpB),pFile);
				nCount ++;
			}

		}
		_ASSERTE(nCount == nFlagSize);

		delete []pFlagY;

	}

	// write X coordinate 
	if(flag & OF_CMP_X) // not write repeated X
	{
		if(nPointsNum > 0)
		{
			nTmp1 = (char)pGlyf->outline.outline.points[0].x;
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
		for(i=1; i< nPointsNum; i++)
		{		
			if(pGlyf->outline.outline.points[i].x != pGlyf->outline.outline.points[i-1].x)
			{
				nTmp1 = (char)pGlyf->outline.outline.points[i].x;
				nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
			}
		}
	}
	else
	{
		for(i=0; i< nPointsNum; i++)
		{
			nTmp1 = (char)pGlyf->outline.outline.points[i].x;
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
	}

	// write Y coordinate 
	if(flag & OF_CMP_Y)// not write repeated Y
	{
		if(nPointsNum > 0)
		{
			nTmp1 = (char)pGlyf->outline.outline.points[0].y;
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
		for(i=1; i< nPointsNum; i++)
		{		
			if(pGlyf->outline.outline.points[i].y != pGlyf->outline.outline.points[i-1].y)
			{
				nTmp1 = (char)pGlyf->outline.outline.points[i].y;
				nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
			}
		}
	}
	else
	{
		for(i=0; i< nPointsNum; i++)
		{
			nTmp1 = (char)pGlyf->outline.outline.points[i].y;
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
	}

	
	//write Tags
	int nTagCmpNum = (pGlyf->outline.outline.point_num )/4;
    int nTagLeft = pGlyf->outline.outline.point_num%4;
	
	for(i=0;i<nTagCmpNum;i++)
	{
        nTmp1 = (char)(pGlyf->outline.outline.tags[4*i]<<6);
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*i+1]<<4));
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*i+2]<<2));
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*i+3]));
	
        nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
	}
	nTmp1 = 0;
    for(i=0;i<nTagLeft;i++)
	{		
        nTmp1 = (char)(nTmp1 | (pGlyf->outline.outline.tags[4*nTagCmpNum + i])<<(6-2*i));
		if(i== (nTagLeft - 1))
		{
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
	}

	//write contours
	if( flag & OF_CONTOURS_BYTE)
	{
		for(i=0;i<pGlyf->outline.outline.contour_num;i++)
		{
			nTmp1 = (char)pGlyf->outline.outline.contours[i];		
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
	}
	else
	{
		for(i=0;i<pGlyf->outline.outline.contour_num;i++)
		{
			nTmp2 = pGlyf->outline.outline.contours[i];
			if(bBigEndian)
			{
				nTmp2 = Conv16Endian(nTmp2);
			}
			nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
		}
	}

	
	return nSize;
}

int CGlyphData::ReadGlyfOutline(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian,UINT nID)
{
//	int nTmp4 = 0;
	short nTmp2 = 0;
	char nTmp1 = 0;

	int nSize = 0;
	int i= 0;
	nTmp2 = 0;
	nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
	if(bBigEndian)
	{
		nTmp2 = Conv16Endian(nTmp2);
	}
	pGlyf->outline.outline.point_num = nTmp2;

	nTmp2 = 0;
	nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);	
	if(bBigEndian)
	{
		nTmp2 = Conv16Endian(nTmp2);
	}
	pGlyf->outline.outline.contour_num = nTmp2;

	if(pGlyf->outline.outline.point_num == 0 || pGlyf->outline.outline.contour_num == 0)
	{
		pGlyf->width  = 0;
		pGlyf->height = 0;
		pGlyf->x = 0;
		pGlyf->y = 0;
		return nSize;
	}


	pGlyf->outline.outline.points = new XFT_VECTOR[pGlyf->outline.outline.point_num];
	pGlyf->outline.outline.tags = new BYTE[pGlyf->outline.outline.point_num];
	pGlyf->outline.outline.contours = new short[pGlyf->outline.outline.contour_num];

	for(i=0; i< pGlyf->outline.outline.point_num; i++)
	{
// 		nTmp2 = 0;
// 		nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);		
// 		if(bBigEndian)
// 		{
// 			nTmp2 = Conv16Endian(nTmp2);
// 		}
// 		pGlyf->outline.outline.points[i].x = nTmp2;
// 		
// 		nTmp2 = 0;
// 		nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);		
// 		if(bBigEndian)
// 		{
// 			nTmp2 = Conv16Endian(nTmp2);
// 		}
// 		pGlyf->outline.outline.points[i].y = nTmp2;	
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
		pGlyf->outline.outline.points[i].x = nTmp1;

		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
		pGlyf->outline.outline.points[i].y = nTmp1;
	}


	//nSize +=fread(pGlyf->outline.tags,1,sizeof(char)*pGlyf->outline.outline.point_num,pFile);	
#if 0
	int nTagCmpNum = (pGlyf->outline.outline.point_num + 1)/2;
	for(i=0;i<nTagCmpNum;i++)
	{
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		BYTE tmp = (BYTE)nTmp1;
		pGlyf->outline.tags[2*i] = (char)((tmp & 0xF0)>>4);
		if(pGlyf->outline.outline.point_num > (2*i+1))
		{
			pGlyf->outline.tags[2*i+1] = (char)(tmp & 0xF);
		}
	}

#else
	int nTagCmpNum = (pGlyf->outline.outline.point_num )/4;
    int nTagLeft = (pGlyf->outline.outline.point_num) % 4;
	BYTE tmp = 0;
	for(i=0;i<nTagCmpNum;i++)
	{
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		tmp = (BYTE)nTmp1;
		pGlyf->outline.outline.tags[4*i] = (char)((tmp & 0xC0)>>6);
        pGlyf->outline.outline.tags[4*i+1] = (char)((tmp & (0xC0>>2))>>4);
        pGlyf->outline.outline.tags[4*i+2] = (char)((tmp & (0xC0>>4))>>2);
        pGlyf->outline.outline.tags[4*i+3] = (char)((tmp & (0xC0>>6)));	
	}	
	
    for(i=0;i<nTagLeft;i++)
	{		
		if(i==0)
		{
			nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
			tmp = (BYTE)nTmp1;
		}		
		pGlyf->outline.outline.tags[4*nTagCmpNum + i] = (char)((tmp & (0xC0>>(2*i)))>>(6-2*i));
	}
#endif

	for(i=0;i<pGlyf->outline.outline.contour_num;i++)
	{
		nTmp2 = 0;
		nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);		
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		pGlyf->outline.outline.contours[i] = nTmp2;
		
	}

// 	nTmp4 = 0;
// 	nSize += fread(&nTmp4,1,sizeof(nTmp4),pFile);	
// 	if(bBigEndian)
// 	{
// 		nTmp4 = Conv32Endian(nTmp4);
// 	}
// 	pGlyf->outline.flags = nTmp4;

	RendOutline(pGlyf,VECTOR_DISP_POINT_SIZE,nID);
	
	
	return nSize;
}

int CGlyphData::ReadComposeGlyf(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian)
{
    int nSize = 0;

    short nTmp2 = 0;
    char nTmp1 = 0;
    int i= 0;

    nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
    if(bBigEndian)
    {
        nTmp2 = Conv16Endian(nTmp2);
    }

    pGlyf->nNumberSubGlyph = nTmp2;

    pGlyf->pSubGlyf = new X_SUBGLYPH[pGlyf->nNumberSubGlyph];

	for( i = 0;i< pGlyf->nNumberSubGlyph; i++ )
	{
		X_SUBGLYPH_PTR pSub = pGlyf->pSubGlyf + i;

        //flag
        nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
        if(bBigEndian)
        {
            nTmp2 = Conv16Endian(nTmp2);
        }
        pSub->flags = nTmp2;
        
        //index
        nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
        if(bBigEndian)
        {
            nTmp2 = Conv16Endian(nTmp2);
        }
        pSub->index = nTmp2;

        if ( pSub->flags & SUBGLYPH_ARGS_ARE_WORDS )
        {
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->arg1 = nTmp2;
            
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->arg2 = nTmp2;
        }
        else
        {
            nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
            pSub->arg1 = nTmp1;

            nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
            pSub->arg2 = nTmp1;
        }

        if ( pSub->flags & SUBGLYPH_WE_HAVE_A_SCALE )
        {
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->transform.xx = nTmp2 << 2;
			pSub->transform.yy = pSub->transform.xx;
        }
        else if ( pSub->flags & SUBGLYPH_WE_HAVE_AN_XY_SCALE )
        {
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->transform.xx = nTmp2 << 2;
            
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->transform.yy = nTmp2 << 2;
        }
        else if ( pSub->flags & SUBGLYPH_WE_HAVE_A_2X2 )
        {
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->transform.xx = nTmp2 << 2;
            
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->transform.yx = nTmp2 << 2;
            
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->transform.xy = nTmp2 << 2;
            
            nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
            if(bBigEndian)
            {
                nTmp2 = Conv16Endian(nTmp2);
            }
            pSub->transform.yy = nTmp2 << 2;
        }
	}
	
	return nSize;
}

int CGlyphData::ReadGlyfOutlineEx(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian,UINT nID)
{
	short nTmp2 = 0;
	char nTmp1 = 0;

	int nSize = 0;
	int i= 0;

	BYTE flag = 0;
	nSize += fread(&flag,1,sizeof(flag),pFile); 

	_ASSERTE(flag <= 0x1F);

	if(flag & OF_POINTS_NUM_BYTE)
	{
		nTmp1 = 0;
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		pGlyf->outline.outline.point_num = (BYTE)nTmp1;
	}
	else
	{
		nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		pGlyf->outline.outline.point_num = nTmp2;
	}

	if(flag & OF_CONTOURS_NUM_BYTE)
	{
		nTmp1 = 0;
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		pGlyf->outline.outline.contour_num = (BYTE)nTmp1;

	}
	else
	{
		nTmp2 = 0;
		nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);	
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		pGlyf->outline.outline.contour_num = nTmp2;
	}

	if(pGlyf->outline.outline.point_num == 0 || pGlyf->outline.outline.contour_num == 0)
	{
		pGlyf->width  = 0;
		pGlyf->height = 0;
		pGlyf->x = 0;
		pGlyf->y = 0;
		return nSize;
	}

	int nPointsNum = pGlyf->outline.outline.point_num;
	int nContoursNum = pGlyf->outline.outline.contour_num;

	LPBYTE pFlagX = NULL;
	LPBYTE pFlagY = NULL;
	LPBYTE pFlagX_temp = NULL;
	LPBYTE pFlagY_temp = NULL;
    int point_quotient  = ( nPointsNum - 1 ) / 8;  //循环展开的商
    int point_remainder = ( nPointsNum - 1 ) % 8;  //循环展开的余数
    int point_flag_offset = ( point_remainder == 0 ) ? point_quotient : point_quotient + 1;

    if(flag & OF_CMP_X)
    {
        pFlagX = pFlagX_temp = new BYTE[point_flag_offset];
        
        nSize += fread(pFlagX,1,point_flag_offset,pFile);
    }
    
    if(flag & OF_CMP_Y)
    {
        pFlagY = pFlagY_temp = new BYTE[point_flag_offset];
        
        nSize += fread(pFlagY,1,point_flag_offset,pFile);
    }

	pGlyf->outline.outline.points = new XFT_VECTOR[pGlyf->outline.outline.point_num];
	pGlyf->outline.outline.tags = new BYTE[pGlyf->outline.outline.point_num];
	pGlyf->outline.outline.contours = new short[pGlyf->outline.outline.contour_num];	


    if(flag & OF_CMP_X)
    {
        nTmp1 = 0;
        nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
        pGlyf->outline.outline.points[0].x = nTmp1;	

        for ( i = 0; i < point_quotient; i++ )
        {
            BYTE x_flag = *pFlagX_temp++;
            
            for ( int j = 0; j < 8; j++ )
            {
                if ( x_flag & ( 1 << ( 7 - j ) ) )
                {
                    pGlyf->outline.outline.points[i * 8 + j + 1].x = pGlyf->outline.outline.points[i * 8 + j].x;
                }
                else
                {
                    nTmp1 = 0;
                    nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
                    
                    pGlyf->outline.outline.points[i * 8 + j + 1].x = nTmp1;
                }
            }
        }
        
        if ( point_remainder > 0 )
        {
            BYTE x_flag = *pFlagX_temp;
            
            for ( int j = 0; j < point_remainder; j++ )
            {
                if ( x_flag & ( 1 << ( 7 - j ) ) )
                {
                    pGlyf->outline.outline.points[i * 8 + j + 1].x = pGlyf->outline.outline.points[i * 8 + j].x;
                }
                else
                {
                    nTmp1 = 0;
                    nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
                    
                    pGlyf->outline.outline.points[i * 8 + j + 1].x = nTmp1;
                }
            }
        }		
    }
    else
    {
        for(i=0; i< nPointsNum; i++)
        {
            nTmp1 = 0;
            nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
            pGlyf->outline.outline.points[i].x = nTmp1;		
            
        }
    }
    
    if(flag & OF_CMP_Y)
    {
        if(nPointsNum > 0)
        {
            nTmp1 = 0;
            nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
            pGlyf->outline.outline.points[0].y = nTmp1;	
            
        }        
        for ( i = 0; i < point_quotient; i++ )
        {
            BYTE y_flag = *pFlagY_temp++;
            
            for ( int j = 0; j < 8; j++ )
            {
                if ( y_flag & ( 1 << ( 7 - j ) ) )
                {
                    pGlyf->outline.outline.points[i * 8 + j + 1].y = pGlyf->outline.outline.points[i * 8 + j].y;
                }
                else
                {
                    nTmp1 = 0;
                    nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
                    
                    pGlyf->outline.outline.points[i * 8 + j + 1].y = nTmp1;
                }
            }
        }
        
        if ( point_remainder > 0 )
        {
            BYTE y_flag = *pFlagY_temp;
            
            for ( int j = 0; j < point_remainder; j++ )
            {
                if ( y_flag & ( 1 << ( 7 - j ) ) )
                {
                    pGlyf->outline.outline.points[i * 8 + j + 1].y = pGlyf->outline.outline.points[i * 8 + j].y;
                }
                else
                {
                    nTmp1 = 0;
                    nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
                    
                    pGlyf->outline.outline.points[i * 8 + j + 1].y = nTmp1;
                }
            }
        }		
    }
    else
    {
        for(i=0; i< nPointsNum; i++)
        {
            nTmp1 = 0;
            nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
            pGlyf->outline.outline.points[i].y = nTmp1;
        }
    }

	int nTagCmpNum = (pGlyf->outline.outline.point_num )/4;
    int nTagLeft = (pGlyf->outline.outline.point_num) % 4;
	BYTE tmp = 0;
	for(i=0;i<nTagCmpNum;i++)
	{
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		tmp = (BYTE)nTmp1;
		pGlyf->outline.outline.tags[4*i] = (char)((tmp & 0xC0)>>6);
        pGlyf->outline.outline.tags[4*i+1] = (char)((tmp & (0xC0>>2))>>4);
        pGlyf->outline.outline.tags[4*i+2] = (char)((tmp & (0xC0>>4))>>2);
        pGlyf->outline.outline.tags[4*i+3] = (char)((tmp & (0xC0>>6)));	
	}	
	
    for(i=0;i<nTagLeft;i++)
	{		
		if(i==0)
		{
			nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
			tmp = (BYTE)nTmp1;
		}		
		pGlyf->outline.outline.tags[4*nTagCmpNum + i] = (char)((tmp & (0xC0>>(2*i)))>>(6-2*i));
	}


	if(flag & OF_CONTOURS_BYTE)
	{
		for(i=0;i<nContoursNum;i++)
		{
			nTmp1 = 0;
			nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
			pGlyf->outline.outline.contours[i] = (BYTE)nTmp1;			
		}
	}
	else
	{
		for(i=0;i<nContoursNum;i++)
		{
			nTmp2 = 0;
			nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);		
			if(bBigEndian)
			{
				nTmp2 = Conv16Endian(nTmp2);
			}
			pGlyf->outline.outline.contours[i] = nTmp2;
			
		}
	}


	if(pFlagX != NULL)
		delete [] pFlagX;
	if(pFlagY != NULL)
		delete [] pFlagY;

	RendOutline(pGlyf,VECTOR_DISP_POINT_SIZE,nID);


	return nSize;
}


int CGlyphData::RendOutline(GLYPH_BODY_PTR pGlyf,int nDispPtSize, UINT nID)
{
	if(m_pGlyphTables == NULL  || pGlyf== NULL /*|| pGlyf->outline.outline.point_num == 0 || pGlyf->outline.outline.contour_num==0*/)
	{
		return 0;
	}

	SPML_OUTLINE_T outline={0};

	outline.contour_num  = pGlyf->outline.outline.contour_num;
	outline.point_num    = pGlyf->outline.outline.point_num;	
	//outline.flags		= pGlyf->outline.outline.flags;
	if(outline.point_num >0)
	{
		outline.points		= new SPML_VECTOR_T[outline.point_num];
		outline.tags		= new BYTE[outline.point_num];
		memcpy(outline.points,pGlyf->outline.outline.points, outline.point_num * sizeof(SPML_VECTOR_T));
		memcpy(outline.tags,pGlyf->outline.outline.tags, outline.point_num * sizeof(char));
	}
	if(outline.contour_num>0)
	{
		outline.contours	= new short[outline.contour_num];
		memcpy(outline.contours,pGlyf->outline.outline.contours,outline.contour_num * sizeof(short));
	}

	// ( shrink-percent * 0x80 / 100 ) is as the point size in glyph-table-header
	// so real point size for display is ( nDispPtSize * ( shrink-percent * 0x80 / 100 ) / 0x80)
	double rate = nDispPtSize;
	int rate2 = 0;
	int nOrgPtSize = VECTOR_POINT_SIZE;
	if(m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR)
	{		
		nOrgPtSize = m_fontHDR.units_per_em >> 6;
	}
	if(m_pCSI != NULL)
	{
		rate2 = (int)(nDispPtSize * (m_pCSI->nVectorShrinkPercent * 0x80 / 100 / m_pCSI->dZoomRate) / 0x80);
		rate = (double)rate2;
		if(!m_pCSI->bVecCmp)
		{		
			rate /= nOrgPtSize;			
		}
	}
	else
	{
		if(m_pGlyphTables != NULL && m_pGlyphTables[0].hdr.size != 0)
		{
			rate2 = (int)nDispPtSize * m_pGlyphTables[0].hdr.size / 0x80;
			rate = (double)rate2;
			if(m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR)
			{
				rate /= nOrgPtSize;
			}
		}
	}


// auto-hinter
#if 1
	{  
		double rate = 1.0;//double(m_pCSI->nVectorShrinkPercent) / 100 / m_pCSI->dZoomRate;
		SPML_GLYPH_IMAGE_T image = {0};
		SPML_FONT_T        font = {0};
		
		font.em  = m_fontHDR.units_per_em;
		
		if(m_pCSI != NULL)
		{
			rate = double(m_pCSI->nVectorShrinkPercent) / 100 / m_pCSI->dZoomRate;
		}
		else
		{
			rate = (double)m_pGlyphTables[0].hdr.size / 0x80;
		}
		
		if ( m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR_CMP )
		{
			rate = rate * (m_fontHDR.units_per_em >> 6);
		}
		
		image.code = (uint16)GetUniFormID(nID);
		image.advance = (uint16)(pGlyf->metrics.advance_org );
		image.size = (uint16)nDispPtSize;
		image.font = &font;
		
		X_OutlineAutohinter(&outline, &image, rate);

		pGlyf->dwidth = (uint16)(image.advance >> 6);
		if (m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR_CMP)
		{
 			if((m_fontHDR.units_per_em >> 6)!=0)
 			{
 				pGlyf->dwidth = (uint16)(pGlyf->dwidth /(m_fontHDR.units_per_em >> 6));
 			}
 			else
 			{
 				pGlyf->dwidth = 0;
 			}
			
			//pGlyf->dwidth =  pGlyf->dwidth * (VECTOR_POINT_SIZE/rate2) ;
		}

// 		pGlyf->metrics.dwidth = (uint8)(image.advance >> 6);
// 		if (m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR_CMP)
// 		{
// 			if((m_fontHDR.units_per_em >> 6)!=0)
// 			{
// 				pGlyf->metrics.dwidth = (uint8)(pGlyf->metrics.dwidth /(m_fontHDR.units_per_em >> 6));
// 			}
// 			else
// 			{
// 				pGlyf->metrics.dwidth = 0;
// 			}
// 			
// 		}
	}
#else
	{
		// zoom the outline
		SPML_MATRIX_T matrix;
		matrix.xx = (FT_Fixed)FixedFromDouble(rate);
		matrix.xy = (FT_Fixed)FixedFromDouble(0);
		matrix.yx = (FT_Fixed)FixedFromDouble(0);
		matrix.yy = (FT_Fixed)FixedFromDouble(rate); 
		X_OutlineTransform(&outline,&matrix);
	}
#endif

    SPML_BBOX_T bbox = {0};
	SPML_BBOX_T cbox = {0};
	X_OutlineGetBBox(&outline,&bbox,&cbox);

//  bbox.xMin = FT_PIX_FLOOR( bbox.xMin );
//  bbox.yMin = FT_PIX_FLOOR( bbox.yMin );
//  bbox.xMax = FT_PIX_CEIL(  bbox.xMax );
//  bbox.yMax = FT_PIX_CEIL(  bbox.yMax );
// 
// 	SPML_Pos xoffset = 0;
// 	SPML_Pos yoffset = 0;
// 	yoffset = -bbox.yMin;
// 	xoffset = -bbox.xMin;
// 
// 	pGlyf->x = (short)((bbox.xMin) >> 6);
// 	pGlyf->y = (short)((bbox.yMax) >> 6);
// 	int height = (bbox.yMax-bbox.yMin)>>6;
// 	int width  = (bbox.xMax-bbox.xMin)>>6;

	cbox.xMin = FT_PIX_FLOOR( cbox.xMin );
    cbox.yMin = FT_PIX_FLOOR( cbox.yMin );
    cbox.xMax = FT_PIX_CEIL(  cbox.xMax );
    cbox.yMax = FT_PIX_CEIL(  cbox.yMax );

	SPML_Pos xoffset = 0;
	SPML_Pos yoffset = 0;
	yoffset = -cbox.yMin;
	xoffset = -cbox.xMin;

	pGlyf->x = (short)((cbox.xMin) >> 6);
	pGlyf->y = (short)((cbox.yMax) >> 6);
	int height = (cbox.yMax-cbox.yMin)>>6;
	int width  = (cbox.xMax-cbox.xMin)>>6;

//  height+=3;
// 	pGlyf->y+=3;
// 
// 	width+=3;
// 	pGlyf->x-=2;

	int glyph_size = width* height;

    _ASSERTE( height < 256 && width < 256 );

	// move the points, make their (x,y) positive, for rending the whole glyph.
	// otherwise only can rend the positive points.
	X_OutlineTranslate(&outline,xoffset,yoffset);

	int nPich = width;//((_width+3)/4)*4;

	SPML_BITMAP_T bitmap = {0};

	bitmap.rows = height;
	bitmap.width = width;
	bitmap.pitch  = nPich;
	if(height*nPich>0)
	{
		bitmap.buffer = new BYTE[height*nPich];    
		memset(bitmap.buffer,0,height*nPich);
	}

    SPML_RASTER_PARAMS_T raster = {0};
	raster.target = &bitmap;
	raster.source = &outline;
//	raster.flags  = SPML_RASTER_FLAG_AA;
	raster.vector_buffer_ptr = g_buf_ptr;
	raster.size_of_vector_buffer = g_buf_size;

	int error = 0;
#if 1
	error = X_GrayRasterRender( &raster );
 	if(error != 0)
 		goto exit;
#else	

	bitmap.pixel_mode = FT_PIXEL_MODE_GRAY;
	bitmap.num_grays = 256;
	FT_Outline _outline={0};
	_outline.n_contours = outline.contour_num;
	_outline.n_points = outline.point_num;
	_outline.points = new FT_Vector[_outline.n_points];
	_outline.tags = new char[_outline.n_points];
	_outline.contours =  new short[_outline.n_contours];
	_outline.flags = 1;
	memcpy(_outline.points,outline.points,sizeof(FT_Vector)*_outline.n_points);
	memcpy(_outline.tags,outline.tags,sizeof(char)*_outline.n_points);
	memcpy(_outline.contours,outline.contours,sizeof(short)*_outline.n_contours);
	FT_Raster_Params _raster = {0};
	_raster.target = (FT_Bitmap *)&bitmap;
	_raster.source = &_outline;
	_raster.flags = FT_RASTER_FLAG_AA;
	error = FT_Outline_Render(m_library,&_outline,&_raster);
	SAFE_FREE_ARRAY(_outline.points);
	SAFE_FREE_ARRAY(_outline.tags);
	SAFE_FREE_ARRAY(_outline.contours);

	if(error != 0)
 		goto exit;
#endif

// 	int nOrgPtSize =  (m_fontHDR.units_per_em >> 6);
// 
// #define MULTI (nDispPtSize / VECOTR_POINT_SIZE)
// #define DIV   (VECOTR_POINT_SIZE / nDispPtSize)

	pGlyf->width = (WORD)bitmap.width;
	pGlyf->height = (WORD)bitmap.rows;
	//pGlyf->dwidth = (WORD)(pGlyf->metrics.dwidth*rate2/VECTOR_POINT_SIZE);
	//pGlyf->dwidth = pGlyf->metrics.dwidth;
         
	if(glyph_size > 0)
	{
		pGlyf->bitmap = new BYTE[glyph_size];
		memcpy(pGlyf->bitmap, bitmap.buffer,glyph_size);
	}

	SAFE_FREE_ARRAY(bitmap.buffer);//lint !e1043 !e63
	SAFE_FREE_ARRAY(outline.points);//lint !e1043 !e63
	SAFE_FREE_ARRAY(outline.tags);//lint !e1043 !e63
	SAFE_FREE_ARRAY(outline.contours);//lint !e1043 !e63

	return 0; // success

exit:
	m_nOutlineLongjmpNum ++;

	pGlyf->Clear();
	SAFE_FREE_ARRAY(bitmap.buffer);//lint !e1043 !e63
	SAFE_FREE_ARRAY(outline.points);//lint !e1043 !e63
	SAFE_FREE_ARRAY(outline.tags);		//lint !e1043 !e63
	SAFE_FREE_ARRAY(outline.contours); //lint !e1043 !e63
	return error;
}

void CGlyphData::FillMetrics(XGLYPH_METRICS_T &metrics,GLYPH_BODY_PTR pGlyf)
{
	_ASSERTE(pGlyf != NULL);
	if(pGlyf == NULL)
		return;
	metrics.x = (char)pGlyf->x;
	metrics.y = (char)pGlyf->y;
	metrics.dwidth = (BYTE)pGlyf->dwidth;
	metrics.height = (BYTE)pGlyf->height;
	metrics.width  = (BYTE)pGlyf->width;

	int nOrgPintSize = (m_fontHDR.units_per_em >> 6);

	//gt.hdr.size = (BYTE)(( m_pCSI->nVectorShrinkPercent * 0x80)/100/m_pCSI->dZoomRate);

	double zoom = m_pGlyphTables[0].hdr.shrink / m_pGlyphTables[0].hdr.size;

	double dDelRate = (VECTOR_POINT_SIZE) / nOrgPintSize * (zoom);
	//metrics.advance_org = (DWORD)((pGlyf->dwidth <<6 ) / dDelRate) ;
	metrics.advance_org = FixedFromDouble((pGlyf->dwidth <<6 ) / dDelRate) >> 16;
	
}

BYTE  CGlyphData::CalcOutlineCmpFlag(GLYPH_BODY_PTR pGlyf)
{
	BYTE flag = 0;
	int i =0;
	int nPoints = pGlyf->outline.outline.point_num;
	int nContous = pGlyf->outline.outline.contour_num;

	if(IS_IN_BYTE_RANG(nPoints))
	{
		flag |= OF_POINTS_NUM_BYTE;
	}

	if(IS_IN_BYTE_RANG(nContous))
	{
		flag |= OF_CONTOURS_NUM_BYTE;
	}

	BOOL bByte = TRUE;

	for(i=0;i<nContous; i++)
	{
		if(!IS_IN_BYTE_RANG(pGlyf->outline.outline.contours[i]))
		{
			bByte = FALSE;
			break;
		}
	}

	if(bByte)
	{
		flag |= OF_CONTOURS_BYTE;
	}

	int nRepeatX = 0;
	int nRepeatY = 0;

	for(i=1;i<nPoints; i++)
	{
		if(pGlyf->outline.outline.points[i].x == pGlyf->outline.outline.points[i-1].x)
		{
			nRepeatX ++;
		}

		if(pGlyf->outline.outline.points[i].y == pGlyf->outline.outline.points[i-1].y)
		{
			nRepeatY ++;
		}
	}

	int temp = (nPoints + 7)/8;

    if( nRepeatX > temp )
    {
        flag |= OF_CMP_X;
    }
    
    if( nRepeatY > temp )
    {
        flag |= OF_CMP_Y;
    }

	return flag;
}
void CGlyphData::GetSubGlyfID()
{
	std::vector<unsigned int> vSubGlyfID;
	std::vector<unsigned int>::iterator it;

	MSG msg;
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,m_lstID.size());
	int nStep = 0;

	UINT nIDCount = m_lstID.size();

	for(it = m_lstID.begin(); it != m_lstID.end(); it++)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);

		int nID = *it;	

		if(FT_Load_Glyph (m_face, nID,FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE) != FT_SUCCESS)
		{
			continue;
		}

		for(UINT i = 0; i< m_face->glyph->num_subglyphs; i++  )
		{
			FT_SubGlyph  subg = m_face->glyph->subglyphs + i; 	
			UINT nSubID =  subg->index;
			vSubGlyfID.push_back(nSubID);
		}
	}

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	
	for(it = vSubGlyfID.begin(); it != vSubGlyfID.end(); it++)
	{
		int nID = *it;
		InitGlyfInfo(nID,0);
		TRACE(_T("=======sub id: 0x%X\n"),nID);
	}

	if(nIDCount != m_lstID.size())
	{
		GetSubGlyfID();
	}
}

void CGlyphData::ComposeOutline(GLYPH_BODY_PTR pGlyf,UINT nID)
{    
    pGlyf->outline.max_points   = SUB_GLYPH_POINT_MAX;
    pGlyf->outline.max_contours = SUB_GLYPH_CONTOURS_MAX;
    
    pGlyf->outline.outline.points     = new XFT_VECTOR[pGlyf->outline.max_points];	
    pGlyf->outline.outline.tags       = new BYTE[pGlyf->outline.max_points];
    pGlyf->outline.outline.contours   = new short[pGlyf->outline.max_contours];

	// get the rate of zoom for restoring to the original size
    int nOrgPintSize  = m_fontHDR.units_per_em >> 6;
    double dShink  = 1.0;
    if(m_pGlyphTables != NULL && m_pGlyphTables[0].hdr.size != 0)
	{
		dShink =  ((double)m_pGlyphTables[0].hdr.size) / (double)0x80 ;
	}
    double dRate = nOrgPintSize/ dShink;
    
    for( int i = 0;i< pGlyf->nNumberSubGlyph; i++ )
    {
        GLYPH_BODY_PTR pSubGlyf = NULL;
        int index = pGlyf->pSubGlyf[i].index;

        m_mapGlyf.Lookup(index,pSubGlyf);

        _ASSERTE(pSubGlyf != NULL);

		if(pSubGlyf == NULL)
		{
			continue;
		}

		if(pSubGlyf->nNumberSubGlyph != 0 && pSubGlyf->outline.outline.point_num == 0)
		{
			ComposeOutline(pSubGlyf,nID);
		}

        XFT_OUTLINE _outline={0};
        
        _outline.outline.contour_num = pSubGlyf->outline.outline.contour_num;
        _outline.outline.point_num   = pSubGlyf->outline.outline.point_num;        
//      _outline.outline.flags		 = pSubGlyf->outline.outline.flags;

		if(_outline.outline.point_num>0)
		{
			_outline.outline.points    = new SPML_VECTOR_T[_outline.outline.point_num];
			_outline.outline.tags		= new BYTE[_outline.outline.point_num];
			memcpy(_outline.outline.points,pSubGlyf->outline.outline.points, _outline.outline.point_num * sizeof(SPML_VECTOR_T));
			memcpy(_outline.outline.tags,pSubGlyf->outline.outline.tags, _outline.outline.point_num * sizeof(char));
        }
		
		if(_outline.outline.contour_num>0)
		{
			_outline.outline.contours	= new short[_outline.outline.contour_num];
			memcpy(_outline.outline.contours,pSubGlyf->outline.outline.contours,_outline.outline.contour_num * sizeof(short));
		}
        /* now perform the transform required for this subglyph */
        if ( pGlyf->pSubGlyf[i].flags & ( FT_SUBGLYPH_FLAG_SCALE |FT_SUBGLYPH_FLAG_XY_SCALE | FT_SUBGLYPH_FLAG_2X2 ) )
        {               
            X_OutlineTransform( &_outline.outline, &pGlyf->pSubGlyf[i].transform );
        }
        
        /* apply offset */
        if ( pGlyf->pSubGlyf[i].flags & FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES )
        {
            int x = pGlyf->pSubGlyf[i].arg1;
            int y = pGlyf->pSubGlyf[i].arg2;
            
            if ( x || y )
            {
				if(m_pGlyphTables[0].hdr.type == FONT_TYPE_VECTOR_CMP)
				{
					// zoom out the x and y offset
					SPML_VECTOR_T vec = {0};
					vec.x = x;
					vec.y = y; 
					SPML_MATRIX_T matrix;                
					matrix.xx = FixedFromDouble((double)1.0/dRate);
					matrix.xy = FixedFromDouble(0);
					matrix.yx = FixedFromDouble(0);
					matrix.yy = FixedFromDouble((double)1.0/dRate);                 
					X_VectorTransform( &vec, &matrix );                
					x = vec.x;
					y = vec.y;
				}
				// Translate the x and y coordinate
                X_TranslateArray( _outline.outline.point_num, _outline.outline.points, x, y );
            }
        }

        X_AddGlyphOutline( &pGlyf->outline, &_outline.outline );
        

        SAFE_FREE_ARRAY(_outline.outline.points);
        SAFE_FREE_ARRAY(_outline.outline.tags);
        SAFE_FREE_ARRAY(_outline.outline.contours);
    }

	RendOutline(pGlyf,VECTOR_DISP_POINT_SIZE,nID);

}

int CGlyphData::WriteGlyfOutlineFine(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian)
{
	int nPointsNum = pGlyf->outline.outline.point_num;
	int nContourNum = pGlyf->outline.outline.contour_num;	

	BYTE flag = 0;
	if(IS_IN_BYTE_RANG(nPointsNum))
	{
		flag |= OF_POINTS_NUM_BYTE;
	}
	if(IS_IN_BYTE_RANG(nContourNum))
	{
		flag |= OF_CONTOURS_NUM_BYTE;
	}

	int i= 0;
	BOOL bByte = TRUE;
	for(i=0;i<nContourNum; i++)
	{
		if(!IS_IN_BYTE_RANG(pGlyf->outline.outline.contours[i]))
		{
			bByte = FALSE;
			break;
		}
	}

	if(bByte)
	{
		flag |= OF_CONTOURS_BYTE;
	}

	short nTmp2 = 0;
	BYTE nTmp1   = 0;

	int nSize = 0;
	

	// write flag first
	nSize += fwrite(&flag,1,sizeof(flag),pFile);

	// write points number
	if(flag & OF_POINTS_NUM_BYTE)
	{
		nTmp1 = (char)pGlyf->outline.outline.point_num;
		nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
	}
	else
	{
		nTmp2 = pGlyf->outline.outline.point_num;
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
	}
	
	// write contours number
	if(flag & OF_CONTOURS_NUM_BYTE)
	{
		nTmp1 = (char)pGlyf->outline.outline.contour_num;
		nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
	}
	else
	{
		nTmp2 = pGlyf->outline.outline.contour_num;
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
	}

	if(nPointsNum == 0 || nContourNum == 0)
	{
		return nSize;
	}

	
	BYTE *pFlag  = NULL;
	BYTE *pFlagEx  = NULL;
	int nTotalFlagByte = 0;
	BYTE bFlag = 0;
	BYTE bTag = 0;
	int x =0; 
	int y =0;
	SPML_VECTOR_T* points = NULL;
	// write XY flag
	if(nPointsNum !=0)
	{		
		pFlag = new BYTE[nPointsNum];
		memset(pFlag,0,nPointsNum);
		pFlagEx = new BYTE[nPointsNum];
		memset(pFlagEx,0,nPointsNum);
		
		x = pGlyf->outline.outline.points[0].x;
		y = pGlyf->outline.outline.points[0].y;

		points = new SPML_VECTOR_T[nPointsNum];
		points[0].x = x;
		points[0].y = y;
		
		for(i=1;i<nPointsNum;i++)
		{
			points[i].x = pGlyf->outline.outline.points[i].x - pGlyf->outline.outline.points[i-1].x;
			points[i].y = pGlyf->outline.outline.points[i].y - pGlyf->outline.outline.points[i-1].y;
		}
		
		bTag = pGlyf->outline.outline.tags[0];
		
		if(GET_FLAG(bTag,0))
		{
			SET_FLAG(bFlag,0);
		}
		if(GET_FLAG(bTag,1))
		{
			SET_FLAG(bFlag,6);
		}

		if(x >= 0)
		{
			if(IS_IN_BYTE_RANG(x))
			{
				SET_FLAG(bFlag,1);
				SET_FLAG(bFlag,4);
			}
		}
		else
		{
			x = -x;
			if(IS_IN_BYTE_RANG(x))
			{
				SET_FLAG(bFlag,1);
			}
		}		

		if(y >= 0)
		{
			if(IS_IN_BYTE_RANG(y))
			{
				SET_FLAG(bFlag,2);
				SET_FLAG(bFlag,5);
			}
		}
		else
		{
			y = -y;
			if(IS_IN_BYTE_RANG(y))
			{
				SET_FLAG(bFlag,2);
			}
		}

		pFlag[0] = bFlag;

		for(i = 1; i< nPointsNum; i++)
		{
			bFlag = 0;
			x = points[i].x;
			y = points[i].y;

			bTag = pGlyf->outline.outline.tags[i];		
			if(GET_FLAG(bTag,0))
			{
				SET_FLAG(bFlag,0);
			}
			if(GET_FLAG(bTag,1))
			{
				SET_FLAG(bFlag,6);
			}

			if(x >= 0)
			{
				if(IS_IN_BYTE_RANG(x))
				{
					SET_FLAG(bFlag,1);
					SET_FLAG(bFlag,4);
				}
			}
			else
			{
				x = -x;
				if(IS_IN_BYTE_RANG(x))
				{
					SET_FLAG(bFlag,1);
				}
			}
			if( GET_FLAG(bFlag,1) == 0 && pGlyf->outline.outline.points[i].x == pGlyf->outline.outline.points[i-1].x)
			{
				SET_FLAG(bFlag,4);
			}

			if(y >= 0)
			{
				if(IS_IN_BYTE_RANG(y))
				{
					SET_FLAG(bFlag,2);
					SET_FLAG(bFlag,5);
				}
			}
			else
			{
				y = -y;
				if(IS_IN_BYTE_RANG(y))
				{
					SET_FLAG(bFlag,2);
				}
			}

			if( GET_FLAG(bFlag,2) == 0 && pGlyf->outline.outline.points[i].y == pGlyf->outline.outline.points[i-1].y)
			{
				SET_FLAG(bFlag,5);
			}

			pFlag[i] = bFlag;
		
		}	

		
		pFlagEx[0] = pFlag[0];
		int nCur = 0;
		int nCurEx = 0;
		nTotalFlagByte  = 0;
		int nRepeat = 0;

		for(i = 1; i< nPointsNum; i++)
		{
			if(pFlag[i] == pFlag[nCur])
			{
				nRepeat++;
				if(i== (nPointsNum-1))
				{
					bFlag = pFlag[nCur];
					SET_FLAG(bFlag,3);
					pFlagEx[nCurEx] = bFlag;
					pFlagEx[nCurEx+1] = (BYTE)nRepeat; 
					nCurEx += 2;
					nTotalFlagByte += 2;
					nRepeat = 0;					
				}
			}
			else
			{
				if(nRepeat > 0)
				{
					bFlag = pFlag[nCur];
					SET_FLAG(bFlag,3);
					pFlagEx[nCurEx] = bFlag;
					pFlagEx[nCurEx+1] = (BYTE)nRepeat; 
					nCurEx += 2;
					nTotalFlagByte += 2;
					nRepeat = 0;
					
				}
				else
				{
					pFlagEx[nCurEx] = pFlag[nCur];
					nCurEx++;		
					nTotalFlagByte++;
					nRepeat =0;
				}
				nCur = i;
				if(i== (nPointsNum-1))
				{								
					pFlagEx[nCurEx] = pFlag[nCur];				
					nCurEx++;
					nTotalFlagByte++;
					nRepeat = 0;					
				}
			}
		}
		
		nSize += fwrite(pFlagEx,1,nTotalFlagByte,pFile);
	}


	// write X coordinate 
	for(i=0; i< nPointsNum; i++)
	{
		bFlag = pFlag[i];
		x = points[i].x;

		if(GET_FLAG(bFlag,1) != 0)
		{
			if(GET_FLAG(bFlag,4) != 0)
			{
				nTmp1 = (BYTE)x;
				nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
			}
			else
			{
				nTmp1 = (BYTE)(-x);
				nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
			}
		}
		else
		{
			if(GET_FLAG(bFlag,4) == 0)
			{
				nTmp2 = (short)x;
				if(bBigEndian)
				{
					nTmp2 = Conv16Endian(nTmp2);
				}
				nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
			}
		}		
	}


	// write Y coordinate 
	for(i=0; i< nPointsNum; i++)
	{
		bFlag = pFlag[i];
		y = points[i].y;

		if(GET_FLAG(bFlag,2) != 0)
		{
			if(GET_FLAG(bFlag,5) != 0)
			{
				nTmp1 = (BYTE)y;
				nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
			}
			else
			{
				nTmp1 = (BYTE)(-y);
				nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
			}
		}
		else
		{
			if(GET_FLAG(bFlag,5) == 0)
			{
				nTmp2 = (short)y;
				if(bBigEndian)
				{
					nTmp2 = Conv16Endian(nTmp2);
				}
				nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
			}
		}		
	}

	
	//write Tags
// 	int nTagCmpNum = (pGlyf->outline.outline.point_num )/4;
//     int nTagLeft = pGlyf->outline.outline.point_num%4;
// 	
// 	for(i=0;i<nTagCmpNum;i++)
// 	{
//         nTmp1 = (BYTE)(pGlyf->outline.outline.tags[4*i]<<6);
//         nTmp1 = (BYTE)(nTmp1 | (pGlyf->outline.outline.tags[4*i+1]<<4));
//         nTmp1 = (BYTE)(nTmp1 | (pGlyf->outline.outline.tags[4*i+2]<<2));
//         nTmp1 = (BYTE)(nTmp1 | (pGlyf->outline.outline.tags[4*i+3]));
// 	
//         nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
// 	}
// 	nTmp1 = 0;
//     for(i=0;i<nTagLeft;i++)
// 	{		
//         nTmp1 = (BYTE)(nTmp1 | (pGlyf->outline.outline.tags[4*nTagCmpNum + i])<<(6-2*i));
// 		if(i== (nTagLeft - 1))
// 		{
// 			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
// 		}
// 	}

	//write contours
	if( flag & OF_CONTOURS_BYTE)
	{
		for(i=0;i<pGlyf->outline.outline.contour_num;i++)
		{
			nTmp1 = (BYTE)pGlyf->outline.outline.contours[i];		
			nSize += fwrite(&nTmp1,1,sizeof(nTmp1),pFile);
		}
	}
	else
	{
		for(i=0;i<pGlyf->outline.outline.contour_num;i++)
		{
			nTmp2 = pGlyf->outline.outline.contours[i];
			if(bBigEndian)
			{
				nTmp2 = Conv16Endian(nTmp2);
			}
			nSize += fwrite(&nTmp2,1,sizeof(nTmp2),pFile);
		}
	}

	SAFE_FREE_ARRAY(pFlag);
	SAFE_FREE_ARRAY(pFlagEx);
	SAFE_FREE_ARRAY(points);

	
	return nSize;
}

int CGlyphData::ReadGlyfOutlineFine(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian, UINT nID)
{
	short nTmp2 = 0;
	BYTE nTmp1 = 0;

	int nSize = 0;
	int i= 0;
	int j=0;

	BYTE flag = 0;
	nSize += fread(&flag,1,sizeof(flag),pFile); 

	_ASSERTE(flag <= 0x1F);

	if(flag & OF_POINTS_NUM_BYTE)
	{
		nTmp1 = 0;
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		pGlyf->outline.outline.point_num = nTmp1;
	}
	else
	{
		nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		pGlyf->outline.outline.point_num = nTmp2;
	}

	if(flag & OF_CONTOURS_NUM_BYTE)
	{
		nTmp1 = 0;
		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		pGlyf->outline.outline.contour_num = nTmp1;
	}
	else
	{
		nTmp2 = 0;
		nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);	
		if(bBigEndian)
		{
			nTmp2 = Conv16Endian(nTmp2);
		}
		pGlyf->outline.outline.contour_num = nTmp2;
	}

	if(pGlyf->outline.outline.point_num == 0 || pGlyf->outline.outline.contour_num == 0)
	{
		pGlyf->width  = 0;
		pGlyf->height = 0;
		pGlyf->x = 0;
		pGlyf->y = 0;
		return nSize;
	}

	int nPointsNum = pGlyf->outline.outline.point_num;
	int nContoursNum = pGlyf->outline.outline.contour_num;

	pGlyf->outline.outline.points = new XFT_VECTOR[nPointsNum];
	pGlyf->outline.outline.tags = new BYTE[nPointsNum];
	pGlyf->outline.outline.contours = new short[nContoursNum];

	memset(pGlyf->outline.outline.points,0,nPointsNum*sizeof(XFT_VECTOR));
	memset(pGlyf->outline.outline.tags,0,nPointsNum*sizeof(BYTE));
	memset(pGlyf->outline.outline.contours,0,nContoursNum*sizeof(short));


	LPBYTE pFlag = pGlyf->outline.outline.tags;

	for(i=0;i< nPointsNum; i++)
	{
		nTmp1 = 0;
        nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
		pFlag[i] = nTmp1;

		if(GET_FLAG(nTmp1,3) != 0)
		{
			BYTE c = 0;
			nSize += fread(&c,1,sizeof(c),pFile);
			for(j=1;j<=c;j++)
			{
				pFlag[i+j] = pFlag[i];
			}			
			i += c;
		}
	} 


	BYTE bFlag;
	int x = 0;
	
    for(i=0; i< nPointsNum; i++)
    {
		int y = 0;
		bFlag = pFlag[i];
		if(GET_FLAG(bFlag,1) != 0)
		{
			if(GET_FLAG(bFlag,4) != 0)
			{				
				nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
				y = (int)nTmp1;
			}
			else
			{			
				nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
				y = (int)nTmp1;
				y = -y;
			}
		}
		else
		{
			if(GET_FLAG(bFlag,4) == 0)
			{			
				nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
				if(bBigEndian)
				{
					nTmp2 = Conv16Endian(nTmp2);
				}
				y = (int)nTmp2;
			}
		}	
		
		x += y;

		pGlyf->outline.outline.points[i].x = x;
		UNSET_FLAG(bFlag,1);
		UNSET_FLAG(bFlag,4);
		pFlag[i] = bFlag;
    }  
    
	x = 0;	
    for(i=0; i< nPointsNum; i++)
    {
		int y = 0;
		bFlag = pFlag[i];
		if(GET_FLAG(bFlag,2) != 0)
		{
			if(GET_FLAG(bFlag,5) != 0)
			{				
				nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
				y = (int)nTmp1;
			}
			else
			{			
				nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
				y = (int)nTmp1;
				y = -y;
			}
		}
		else
		{
			if(GET_FLAG(bFlag,5) == 0)
			{			
				nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);
				if(bBigEndian)
				{
					nTmp2 = Conv16Endian(nTmp2);
				}
				y = (int)nTmp2;
			}
		}	
		
		x += y;

		pGlyf->outline.outline.points[i].y = x; 

       	if(GET_FLAG(bFlag,6) != 0)
		{
			SET_FLAG(bFlag,1);
		}
        
		bFlag = (BYTE)(bFlag & 3);
		pFlag[i] = bFlag;
    }

// 	int nTagCmpNum = (pGlyf->outline.outline.point_num )/4;
//  int nTagLeft = (pGlyf->outline.outline.point_num) % 4;
// 	BYTE tmp = 0;
// 	for(i=0;i<nTagCmpNum;i++)
// 	{
// 		nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
// 		tmp = (BYTE)nTmp1;
// 		pGlyf->outline.outline.tags[4*i] = (char)((tmp & 0xC0)>>6);
//         pGlyf->outline.outline.tags[4*i+1] = (char)((tmp & (0xC0>>2))>>4);
//         pGlyf->outline.outline.tags[4*i+2] = (char)((tmp & (0xC0>>4))>>2);
//         pGlyf->outline.outline.tags[4*i+3] = (char)((tmp & (0xC0>>6)));	
// 	}	
// 	
//     for(i=0;i<nTagLeft;i++)
// 	{		
// 		if(i==0)
// 		{
// 			nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);
// 			tmp = (BYTE)nTmp1;
// 		}		
// 		pGlyf->outline.outline.tags[4*nTagCmpNum + i] = (char)((tmp & (0xC0>>(2*i)))>>(6-2*i));
// 	}


	if(flag & OF_CONTOURS_BYTE)
	{
		for(i=0;i<nContoursNum;i++)
		{
			nTmp1 = 0;
			nSize += fread(&nTmp1,1,sizeof(nTmp1),pFile);	
			pGlyf->outline.outline.contours[i] = nTmp1; // unsigned byte
			
		}
	}
	else
	{
		for(i=0;i<nContoursNum;i++)
		{
			nTmp2 = 0;
			nSize += fread(&nTmp2,1,sizeof(nTmp2),pFile);		
			if(bBigEndian)
			{
				nTmp2 = Conv16Endian(nTmp2);
			}
			pGlyf->outline.outline.contours[i] = nTmp2;
			
		}
	}

	RendOutline(pGlyf,VECTOR_DISP_POINT_SIZE,nID);

	return nSize;
}

BOOL CGlyphData::IsVector()
{
	if(m_pGlyphTables == NULL)
	{
		return TRUE;
	}

	for(UINT i=0; i< m_fontHDR.glyph_table_count; i++)
	{
		if( m_pGlyphTables->hdr.type != FONT_TYPE_VECTOR && 
			m_pGlyphTables->hdr.type != FONT_TYPE_VECTOR_CMP )
		{
			return FALSE;
		}
	}

	return TRUE;

}

BOOL CGlyphData::IsMergable(CGlyphData &gData)
{
	if(!this->IsReady() || !gData.IsReady())
	{		
		return FALSE;
	}

	if(this->IsVector() || gData.IsVector())
	{
		AfxMessageBox(_T("Not support to merge vector font!"));
		return FALSE;
	}

	if(memcmp(&this->m_fontHDR.magic[0],&gData.m_fontHDR.magic[0],12) != 0)
	{
		AfxMessageBox(_T("The font libs not belong to the same character set!"));
		return FALSE;
	}
	if( /*this->m_fontHDR.units_per_em != gData.m_fontHDR.units_per_em ||*/
		this->m_fontHDR.flags != gData.m_fontHDR.flags )
	{
		return FALSE;
	}	

	int i = 0;
	//Check Font Size
	CMap<int,int,int,int> mapFontSize;
	for(i = 0 ; i< m_fontHDR.glyph_table_count;i++)
	{
		mapFontSize.SetAt(m_pGlyphTables[i].hdr.size,m_pGlyphTables[i].hdr.size);
	}

	int nValue;
	for(i = 0 ; i< gData.m_fontHDR.glyph_table_count;i++)
	{
		if(mapFontSize.Lookup(gData.m_pGlyphTables[i].hdr.size,nValue))
		{
			AfxMessageBox(_T("There are exist the same font size in font libs!"));
			return FALSE;
		}
	}

	BOOL bNeedOtherTable = this->NeedOtherTble();

	if(bNeedOtherTable != gData.NeedOtherTble())
	{
		AfxMessageBox(_T("There are no GSUB, GPOS, GDEF table in one font lib!"));
		return FALSE;
	}

	if(bNeedOtherTable)
	{
		// check GDEF
		if(this->m_gdef.size != gData.m_gdef.size)
		{
			AfxMessageBox(_T("GDEF is not the same!"));
			return FALSE;
		}
		if(this->m_gdef.size != 0)
		{
			if(memcmp(this->m_gdef.data_ptr,gData.m_gdef.data_ptr,this->m_gdef.size) != 0)
			{
				AfxMessageBox(_T("GDEF is not the same!"));
				return FALSE;
			}
		}

		// check GSUB
		if(this->m_gsub.size != gData.m_gsub.size)
		{
			AfxMessageBox(_T("GSUB is not the same!"));
			return FALSE;
		}
		if(this->m_gsub.size != 0)
		{
			if(memcmp(this->m_gsub.data_ptr,gData.m_gsub.data_ptr,this->m_gsub.size) != 0)
			{
				AfxMessageBox(_T("GSUB is not the same!"));
				return FALSE;
			}
		}

		// check GPOS
		if(this->m_gpos.size != gData.m_gpos.size)
		{
			AfxMessageBox(_T("GPOS is not the same!"));
			return FALSE;
		}
		if(this->m_gpos.size != 0)
		{
			if(memcmp(this->m_gpos.data_ptr,gData.m_gpos.data_ptr,this->m_gpos.size) != 0)
			{
				AfxMessageBox(_T("GPOS is not the same!"));
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CGlyphData::DoMerge(CGlyphData &gData)
{
	if(!IsMergable(gData))
	{
		AfxMessageBox(_T("Can not merge!"));
		return FALSE;
	}

	BOOL bModifyID = !NeedOtherTble();

	int nFtNum = m_fontHDR.glyph_table_count+gData.m_fontHDR.glyph_table_count;
	UINT i = 0;
	UINT j= 0;
	std::vector<int> vSize;
	CMap<int,int,MERGE_PTR,MERGE_PTR> mapSize;

	MERGE_PTR pM = NULL;
	for(i = 0; i< m_fontHDR.glyph_table_count; i++)
	{
		int size = m_pGlyphTables[i].hdr.size;
		vSize.push_back(size);
		pM = new MERGE_T;
		pM->bCur = TRUE;
		pM->nFontSize = size;
		pM->nIdx = i;
		mapSize.SetAt(size,pM);
	}
	for(i = 0; i< gData.m_fontHDR.glyph_table_count; i++)
	{
		int size = gData.m_pGlyphTables[i].hdr.size;
		vSize.push_back(size);
		pM = new MERGE_T;
		pM->bCur = FALSE;
		pM->nFontSize = size;
		pM->nIdx = i;
		mapSize.SetAt(size,pM);
	}

	std::sort(vSize.begin(),vSize.end());

	GLYPH_TABLE_PTR pGlyftbl = new GLYPH_TABLE_T[nFtNum];
	CUIntArray agIndex;
	agIndex.SetSize(nFtNum);
	for(i = 0; i< (UINT)nFtNum; i++)
	{
		int size = vSize[i];
		mapSize.Lookup(size,pM);
		if(pM->bCur)
		{
			memcpy(pGlyftbl+i,m_pGlyphTables+pM->nIdx,sizeof(GLYPH_TABLE_T));
		}
		else
		{
			memcpy(pGlyftbl+i,gData.m_pGlyphTables+pM->nIdx,sizeof(GLYPH_TABLE_T));
		}
		pGlyftbl[i].glyphs_ptr = NULL;
		pGlyftbl[i].page_count = 0;
		pGlyftbl[i].page_ptr = NULL;
		pGlyftbl[i].offset_ptr = NULL;	
		pGlyftbl[i].pIDs = new UINT[pGlyftbl[i].hdr.glyph_count];
		agIndex[i] = 0;
	}

	MAP_UN_ID  mapUnid;
	MAP_ID_UN  mapIdun;
	DWORD  nUnicode;
//	DWORD  nValue;
	DWORD  nID;
	POSITION pos;
	std::vector<UINT> vUnicode;
	std::vector<int>  vIds;
	MAP_GLYPH  mapGlyfs;


	GLYPH_BODY_PTR pGlyf = NULL;

	MSG msg;

	int nStep = 0;

	CUIntArray agUnicode;

	UINT nTestCount = m_lstUnicode.size();
	nTestCount = m_lstID.size();

	CMap<DWORD,DWORD,DWORD,DWORD> mapUnicodes;
	// get current Unicodes
	for( j = 0; j< m_lstUnicode.size(); j++)
	{
		if(!mapUnicodes.Lookup(m_lstUnicode[j],nUnicode))
		{
			mapUnicodes.SetAt(m_lstUnicode[j],m_lstUnicode[j]);
			vUnicode.push_back(m_lstUnicode[j]);
		}
	}
	nTestCount = gData.m_lstUnicode.size();
	// get gData Unicodes
	for(j = 0; j< gData.m_lstUnicode.size(); j++)
	{
		if(!mapUnicodes.Lookup(gData.m_lstUnicode[j],nUnicode))
		{
			mapUnicodes.SetAt(gData.m_lstUnicode[j],gData.m_lstUnicode[j]);
			vUnicode.push_back(gData.m_lstUnicode[j]);
		}
	}
	mapUnicodes.RemoveAll();

	// sort the Unicodes
	std::sort(vUnicode.begin(),vUnicode.end());
	agUnicode.SetSize(vUnicode.size());
	for(i=0;i< vUnicode.size(); i++)
	{
		agUnicode[i] = vUnicode[i];
	}		

	if(bModifyID)
	{
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN, vUnicode.size());
		agUnicode.SetSize(vUnicode.size());
		// assign ID for unicode 
		for(i=0;i< vUnicode.size();i++)
		{
			while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);	
			}

			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);	

			nID = i+1;
			nUnicode = vUnicode[i];
			mapUnid.SetAt(nUnicode,nID);
			UnicodeArray_ptr pUA = new CUIntArray;
			pUA->Add(nUnicode);
			mapIdun.SetAt(nID,pUA);

			vIds.push_back(nID);

			GLYPH_BODY_PTR pGlyfs = new GLYPH_BODY_T[nFtNum];
			for(int k=0; k<nFtNum;k++)
			{
				pGlyfs[k].Init();	
			}

			GLYPH_BODY_PTR pCurGlyf = NULL;	

			for(j = 0; j< (UINT)nFtNum; j++)
			{
				int size = vSize[j];
				mapSize.Lookup(size,pM);
				if(pM->bCur)
				{
					pCurGlyf = GetGlyph(nUnicode,pM->nIdx,TRUE);
					if(pCurGlyf != NULL)
					{
						CopyGlyph(pGlyfs+j,pCurGlyf,m_pGlyphTables+pM->nIdx);
						pGlyftbl[j].pIDs[agIndex[j]] = nID;
						agIndex[j] = agIndex[j]+1;
					}
				}
				else
				{
					pCurGlyf = gData.GetGlyph(nUnicode,pM->nIdx,TRUE);
					if(pCurGlyf != NULL)
					{
						CopyGlyph(pGlyfs+j,pCurGlyf,gData.m_pGlyphTables+pM->nIdx);
						pGlyftbl[j].pIDs[agIndex[j]] = nID;
						agIndex[j] = agIndex[j]+1;
					}
				}
			}
			
			mapGlyfs.SetAt(nID,pGlyfs);
		}
		
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	}
	else
	{		
		CMap<DWORD,DWORD,DWORD,DWORD> mapIDs;
		// get current IDs
		for( j = 0; j< m_lstID.size(); j++)
		{
			if(!mapIDs.Lookup(m_lstID[j],nID))
			{
				mapIDs.SetAt(m_lstID[j],m_lstID[j]);
				vIds.push_back(m_lstID[j]);
			}
		}
		// get gData IDs
		for(j = 0; j< gData.m_lstID.size(); j++)
		{
			if(!mapIDs.Lookup(gData.m_lstID[j],nID))
			{
				mapIDs.SetAt(gData.m_lstID[j],gData.m_lstID[j]);
				vIds.push_back(gData.m_lstID[j]);
			}
		}
		mapIDs.RemoveAll();
		// sort the IDs		
		std::sort(vIds.begin(),vIds.end());

		// merge map UNICODE-ID
		pos = m_mapUnid.GetStartPosition();
		while(pos)
		{
			m_mapUnid.GetNextAssoc(pos,nUnicode,nID);
			mapUnid.SetAt(nUnicode,nID);
		}
		pos = gData.m_mapUnid.GetStartPosition();
		while(pos)
		{
			gData.m_mapUnid.GetNextAssoc(pos,nUnicode,nID);
			mapUnid.SetAt(nUnicode,nID);
		}

		// merge map ID-UNICODE
		UnicodeArray_ptr pUA = NULL;
		UnicodeArray_ptr pUATmp = NULL;
		pos = m_mapIdun.GetStartPosition();
		while(pos)
		{
			m_mapIdun.GetNextAssoc(pos,nID,pUA);
			pUATmp = new CUIntArray;
			pUATmp->Copy(*pUA);
			mapIdun.SetAt(nID,pUATmp);
		}
		pos = gData.m_mapIdun.GetStartPosition();
		while(pos)
		{
			gData.m_mapIdun.GetNextAssoc(pos,nID,pUA);
			
			if(!mapIdun.Lookup(nID,pUATmp))
			{
				pUATmp = new CUIntArray;
				pUATmp->Copy(*pUA);
				mapIdun.SetAt(nID,pUATmp);	
			}
			else
			{
				CUIntArray agMore;
				for(int n = 0 ;n< pUA->GetSize();n++)
				{
					for(int m = 0; m< pUATmp->GetSize(); m++)
					{
						if(pUA->GetAt(n) == pUATmp->GetAt(m))
						{
							break;
						}
					}
					if(m>= pUATmp->GetSize())
					{
						agMore.Add(pUA->GetAt(n));
					}
				}

				UnicodeArray_ptr pUATmp2 = new CUIntArray;
				pUATmp2->Copy(*pUATmp);				

				for(n=0; n< agMore.GetSize(); n++)
				{
					pUATmp2->Add(agMore[n]);
				}
				mapIdun.SetAt(nID,pUATmp2);	
			}
		}


		::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN, vIds.size());
	
		for(i=0;i< vIds.size();i++)
		{
			while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);	
			}

			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);	

			nID = vIds[i];	

			GLYPH_BODY_PTR pGlyfs = new GLYPH_BODY_T[nFtNum];
			for(int k=0; k<nFtNum;k++)
			{
				pGlyfs[k].Init();	
			}

			GLYPH_BODY_PTR pCurGlyf = NULL;	

			for(j = 0; j< (UINT)nFtNum; j++)
			{
				int size = vSize[j];
				mapSize.Lookup(size,pM);
				if(pM->bCur)
				{
					pCurGlyf = GetGlyph(nID,pM->nIdx,FALSE);
					if(pCurGlyf != NULL)
					{
						CopyGlyph(pGlyfs+j,pCurGlyf,m_pGlyphTables+pM->nIdx);
						pGlyftbl[j].pIDs[agIndex[j]] = nID;
						agIndex[j] = agIndex[j]+1;
					}
				}
				else
				{
					pCurGlyf = gData.GetGlyph(nID,pM->nIdx,FALSE);
					if(pCurGlyf != NULL)
					{
						CopyGlyph(pGlyfs+j,pCurGlyf,gData.m_pGlyphTables+pM->nIdx);
						pGlyftbl[j].pIDs[agIndex[j]] = nID;
						agIndex[j] = agIndex[j]+1;
					}
				}
			}
			
			mapGlyfs.SetAt(nID,pGlyfs);
		}
		
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	}

	MergeClear();
	m_fontHDR.glyph_table_count = (WORD)nFtNum;
	m_fontHDR.gtbl_offset_ptr = new DWORD[nFtNum];
	memset(m_fontHDR.gtbl_offset_ptr,0,sizeof(DWORD)*(nFtNum));

	m_pGlyphTables = pGlyftbl;


	pos = mapUnid.GetStartPosition();
	while(pos)
	{
		mapUnid.GetNextAssoc(pos,nUnicode,nID);
		m_mapUnid.SetAt(nUnicode,nID);
	}

	mapUnid.RemoveAll();

	UnicodeArray_ptr pUATmp = NULL;
	pos = mapIdun.GetStartPosition();
	while(pos)
	{
		mapIdun.GetNextAssoc(pos,nID,pUATmp);
		m_mapIdun.SetAt(nID,pUATmp);
	}
	mapIdun.RemoveAll();

	pos = mapGlyfs.GetStartPosition();
	while(pos)
	{
		mapGlyfs.GetNextAssoc(pos,nID,pGlyf);
		m_mapGlyf.SetAt(nID,pGlyf);
	}
	mapGlyfs.RemoveAll();

	for(i=0;i<(int)vIds.size();i++)
	{
		m_lstID.push_back(vIds[i]);
	}

	for(i=0;i<(int)vUnicode.size();i++)
	{
		m_lstUnicode.push_back(vUnicode[i]);
	}

	CalcMultiUnicodeOneID();
	FixCmap(bModifyID,&agUnicode);	
	CalcPage(bModifyID);
	FixCSI();

	return TRUE;
}

BOOL CGlyphData::AdjustBearingXY(int nBXDelta,int nBYDelta, int nPtIndex)
{
	if(!IsReady())
	{
		return FALSE;
	}
	if(IsVector())
	{
		return FALSE;
	}

	DWORD nID;
	GLYPH_BODY_PTR pGlyf= NULL;

	MSG msg;

	int nStep = 0;
	POSITION pos;

	if(nPtIndex == -1)
	{
		if(m_mapGlyf.GetCount() != 0)
		{
			nStep = 0;
			::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN, m_mapGlyf.GetCount());

			pos = m_mapGlyf.GetStartPosition();

			while(pos)
			{
				while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);	
				}	
				::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);

				m_mapGlyf.GetNextAssoc(pos,nID,pGlyf);
				if(pGlyf != NULL)
				{
					if(m_pCSI != NULL)
					{
						for(int j=0; j< m_pCSI->nFtTblNum;j++)
						{
							pGlyf[j].x = (short)(pGlyf[j].x + nBXDelta);
							pGlyf[j].y = (short)(pGlyf[j].y + nBYDelta);
						}
					}
				}
			}
			if(m_pCSI != NULL)
			{
				for(int j=0; j< m_pCSI->nFtTblNum;j++)
				{
					m_pGlyphTables[j].hdr.bearing_x = (char)(m_pGlyphTables[j].hdr.bearing_x  + nBXDelta);
					m_pGlyphTables[j].hdr.bearing_y = (char)(m_pGlyphTables[j].hdr.bearing_y  + nBYDelta);
				}
			}

			::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
		}
	}
	else
	{	
		if(m_pCSI!= NULL)
		{			
			if(m_mapGlyf.GetCount()!=0)
			{
				nStep = 0;

				::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN, m_mapGlyf.GetCount());

				pos = m_mapGlyf.GetStartPosition();

				while(pos)
				{
					while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);	
					}	
					::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);

					m_mapGlyf.GetNextAssoc(pos,nID,pGlyf);
					if(pGlyf != NULL)
					{						
						pGlyf[nPtIndex].x = (short)(pGlyf[nPtIndex].x + nBXDelta);
						pGlyf[nPtIndex].y = (short)(pGlyf[nPtIndex].y + nBYDelta);						
					
					}
				}

				m_pGlyphTables[nPtIndex].hdr.bearing_x = (char)(m_pGlyphTables[nPtIndex].hdr.bearing_x  + nBXDelta);
				m_pGlyphTables[nPtIndex].hdr.bearing_y = (char)(m_pGlyphTables[nPtIndex].hdr.bearing_y  + nBYDelta);

				::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
			}
		}
	}

	return TRUE;
}

void CGlyphData::CopyGlyph(GLYPH_BODY_PTR pDst,GLYPH_BODY_PTR pSrc,GLYPH_TABLE_PTR pGlyphTbl)
{
	if(pSrc== NULL ||  pDst==NULL || pGlyphTbl == NULL)
		return;

	memcpy(pDst,pSrc,sizeof(GLYPH_BODY_T));

	int nBmpSize = 0;
	if(pGlyphTbl->hdr.type == FONT_TYPE_MONO)
	{
		nBmpSize = ((pSrc->width+7)/8)*pSrc->height;
	}
	else
	{
		nBmpSize = pSrc->width * pSrc->height;
	}

	if(nBmpSize != 0)
	{
		pDst->bitmap = new BYTE[nBmpSize];
		memcpy(pDst->bitmap,pSrc->bitmap,nBmpSize);
	}

	if(pSrc->nNumberSubGlyph > 0)
	{
		pDst->pSubGlyf = new X_SUBGLYPH[pSrc->nNumberSubGlyph];
		memcpy(pDst->pSubGlyf,pSrc->pSubGlyf,sizeof(X_SUBGLYPH)*pSrc->nNumberSubGlyph);
	}

	if(pSrc->outline.outline.contour_num != 0)
	{
		memcpy(pDst->outline.outline.contours,pSrc->outline.outline.contours,sizeof(int16)*pSrc->outline.outline.contour_num);
	}

	if(pSrc->outline.outline.point_num != 0)
	{
		memcpy(pDst->outline.outline.points,pSrc->outline.outline.points,sizeof(SPML_VECTOR_T)*pSrc->outline.outline.point_num);
		memcpy(pDst->outline.outline.tags,pSrc->outline.outline.tags,sizeof(uint8)*pSrc->outline.outline.point_num);
	}
}

void CGlyphData::MergeClear()
{
	MSG msg;

	int nCount = m_fontHDR.glyph_table_count;

	for(int i = 0; i< nCount && m_pGlyphTables != NULL;i++)
	{
		m_pGlyphTables[i].Clear();
	}

	SAFE_FREE_ARRAY(m_fontHDR.gtbl_offset_ptr);
	SAFE_FREE_ARRAY(m_pGlyphTables);

	m_lstID.clear();
	m_mapUnid.RemoveAll();
	m_lstUnicode.clear();

	DWORD nID;
	GLYPH_BODY_PTR pGlyf= NULL;

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN, m_mapGlyf.GetCount());

	int nStep = 0;

	POSITION pos = m_mapGlyf.GetStartPosition();
	while(pos)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}
		
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);		

		m_mapGlyf.GetNextAssoc(pos,nID,pGlyf);
		if(pGlyf != NULL)
		{
			if(m_pCSI != NULL)
			{
				for(int j=0; j< m_pCSI->nFtTblNum;j++)
				{
					pGlyf[j].Clear();
				}
			}
			delete [] pGlyf;
			pGlyf = NULL;
		}
	}
	m_mapGlyf.RemoveAll();
		
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);



	memset(&m_tMetrics,0,sizeof(m_tMetrics));
	
	UnicodeArray_ptr pUA= NULL;

	m_mapRidun.RemoveAll();

	pos = m_mapIdun.GetStartPosition();
	while(pos)
	{
		m_mapIdun.GetNextAssoc(pos,nID,pUA);
		if(pUA != NULL)
		{
			pUA->RemoveAll();
			delete pUA;
		}
	}
	m_mapIdun.RemoveAll();	
}

BOOL CGlyphData::GetBDFParam(BDF_ITEM_PTR pBDFItem)
{
	CStdioFile file;
    CFileException e;
	if(!file.Open(pBDFItem->szBdfFile,CFile::modeRead | CFile::typeText /*| CFile::shareDenyWrite*/,&e))
	{
		CString strErr;
		strErr.Format(_T("Unable to open file [%s]. [Error: %d]"),pBDFItem->szBdfFile,e.m_cause);
		AfxMessageBox(strErr);
		return FALSE;
	}
	CString strLine;
	CString strTemp;
	int nPtSize = 0;
	int nXRes;
	int nYRes;
	int nChars = 0;
	BOOL bOK = FALSE;
	while(file.ReadString(strLine))// read header
	{
		if(strLine.Find(_CHARS) != 0) 
		{
			/* Read SIZE */
			if(strLine.Find(_SIZE) == 0)
			{
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_SIZE));
				_stscanf(strTemp,_T("%d %d %d"),&nPtSize,&nXRes,&nYRes);
				continue;
			}
		}
		else
		{
			/* Read CHARS char count */
			strTemp = strLine.Right(strLine.GetLength()-_tcslen(_CHARS));
			_stscanf(strTemp,_T("%d"),&nChars);
			if( nChars == 0)
			{				
				file.Close();
				CString StrErr;
				StrErr.Format(_T("BDF file [%s] has zero char!\n"),pBDFItem->szBdfFile);
				AfxMessageBox(StrErr);	
				return FALSE;
			}
			bOK = TRUE;
			break;		
		}	
	}

	file.Close();

	pBDFItem->nChars = nChars;
	pBDFItem->nPTSize = nPtSize;

	return bOK;
}

int  CGlyphData::GetGlyfTblCount()
{
	return m_fontHDR.glyph_table_count;
}

BOOL CGlyphData::IsIDInTable(GLYPH_TABLE_PTR pTbl,UINT nID)
{
	int mid,front=0,back= pTbl->hdr.glyph_count-1;

	while(front<=back)
	{
		mid = (front+back)/2;
		if(pTbl->pIDs[mid] == nID)
		{
			return TRUE;
		}
		
		if(pTbl->pIDs[mid]<nID)
		{
			front = mid+1;
		}
		else
		{
			back = mid-1;
		}
	}

	return FALSE;
}

void CGlyphData::CalcMultiUnicodeOneID()
{
	POSITION pos = m_mapIdun.GetStartPosition();
	UnicodeArray_ptr pUA= NULL;
	DWORD dwID;
	while(pos)
	{
		m_mapIdun.GetNextAssoc(pos,dwID,pUA);
		if(pUA->GetSize()>1)
		{
			m_mapRidun.SetAt(dwID,dwID);
		}
	}
}

BOOL CGlyphData::RemoveFontSize(int nPtIndex)
{
	POSITION pos = m_mapGlyf.GetStartPosition();
	DWORD dwID;
	GLYPH_BODY_PTR pGlyfs = NULL;
	int nTblCount = m_fontHDR.glyph_table_count - 1;

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,m_mapGlyf.GetCount());	
	int nStep = 0;
	MSG msg;
	while(pos)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}
		
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,++nStep);	

		m_mapGlyf.GetNextAssoc(pos,dwID,pGlyfs);
		GLYPH_BODY_PTR pNewGlyfs = new GLYPH_BODY_T[nTblCount];
		int j = 0;
		for(int i = 0; i< m_fontHDR.glyph_table_count; i++)
		{
			if(i!= nPtIndex)
			{
				CopyGlyph(pNewGlyfs+j,pGlyfs+i,m_pGlyphTables+i);
				j++;				
			}
			pGlyfs[i].Clear();
		}
		SAFE_FREE_ARRAY(pGlyfs);
		m_mapGlyf.SetAt(dwID,pNewGlyfs);
	}

	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

	GLYPH_TABLE_PTR pGlyfTbl = new GLYPH_TABLE_T[nTblCount];

	int j = 0;
	for(int i = 0; i< m_fontHDR.glyph_table_count; i++)
	{
		if(i!= nPtIndex)
		{
			memcpy(pGlyfTbl+j,m_pGlyphTables+i,sizeof(GLYPH_TABLE_T));
			pGlyfTbl[j].glyphs_ptr = NULL;
			pGlyfTbl[j].page_count = m_pGlyphTables[i].page_count;
			pGlyfTbl[j].page_ptr = NULL;
			pGlyfTbl[j].offset_ptr = NULL;	
			pGlyfTbl[j].pIDs = new UINT[m_pGlyphTables[i].hdr.glyph_count];
			memcpy(pGlyfTbl[j].pIDs,m_pGlyphTables[i].pIDs,sizeof(UINT)*m_pGlyphTables[i].hdr.glyph_count);
			if(pGlyfTbl[j].page_count != 0)
			{
				pGlyfTbl[j].page_ptr = new GLYPH_PAGE_T[pGlyfTbl[j].page_count];
				memcpy( pGlyfTbl[j].page_ptr,
						m_pGlyphTables[i].page_ptr, sizeof(GLYPH_PAGE_T)*m_pGlyphTables[i].page_count);
			}
			j++;
		}
		
		m_pGlyphTables[i].Clear();	
	} 

	SAFE_FREE_ARRAY(m_pGlyphTables);
	m_pGlyphTables = pGlyfTbl;

	m_fontHDR.glyph_table_count = (WORD)nTblCount;

	FixCSI();

	return TRUE;
}

BOOL CGlyphData::ReplaceBDF(LPCTSTR lpszBDF,int nFontSizeIndex)
{
	_ASSERTE(lpszBDF != NULL);

	CStdioFile file;
    CFileException e;
	if(!file.Open(lpszBDF,CFile::modeRead | CFile::typeText /*| CFile::shareDenyWrite*/,&e))
	{
		CString strErr;
		strErr.Format(_T("Unable to open file [%s]. [Error: %d]"),lpszBDF,e.m_cause);
		AfxMessageBox(strErr);
		return FALSE;
	}

	DWORD dwGlobalDwdith = 0;
	BOOL  bGlobalDwidth = FALSE;

	BOOL bGray = FALSE;
	BDF_HEADER_T	m_tBH;
	BOOL bUnicodeEncoding = TRUE;
	BOOL bHasReadFisrtStartChar = FALSE;
	CString strLine;
	CString strTemp;
	while(file.ReadString(strLine))// read header
	{
		if(strLine.Find(_CHARS) != 0) 
		{
			/* Read FONT */
			if(strLine.Find(_FONT)==0 )
			{
				strTemp =strLine.Right(strLine.GetLength()-_tcslen(_FONT));
				if(strTemp.GetLength()>_MAX_PATH)
					strTemp = strTemp.Left(_MAX_PATH-1);
				_tcscpy(m_tBH.szFontName, strTemp);
				continue;
			}
			/* Read SIZE */
			if(strLine.Find(_SIZE) == 0)
			{
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_SIZE));
				_stscanf(strTemp,_T("%d %d %d"),&(m_tBH.dwPointSize),&(m_tBH.dwXres),&(m_tBH.dwYres));
				continue;
			}
			/* Read FONTBOUNDINGBOX */
			if(strLine.Find(_FONTBOUNDINGBOX) ==0)
			{
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_FONTBOUNDINGBOX));
				_stscanf(strTemp,_T("%d %d %d %d"),&(m_tBH.dwFBBX),&(m_tBH.dwFBBY),&(m_tBH.nXoff),&(m_tBH.nYoff));
				continue;
			}
			/* Read FAMILY_NAME font name */
			if(strLine.Find(_FAMILY_NAME) == 0)
			{
				memset(m_tBH.szFontName,0,sizeof(m_tBH.szFontName));
				strTemp =strLine.Right(strLine.GetLength()-_tcslen(_FAMILY_NAME));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				strTemp.TrimLeft(_T("\""));
				strTemp.TrimRight(_T("\""));
				if(strTemp.GetLength()>_MAX_PATH)
					strTemp = strTemp.Left(_MAX_PATH-1);
				strTemp.TrimLeft();
				strTemp.TrimRight();
				_tcscpy(m_tBH.szFontName, strTemp);
				continue;
			}
			/* Read Global DWIDTH */
			if(strLine.Find(_DWIDTH) == 0)
			{
				DWORD dwWidth = 0;
				DWORD dwTmp = 0;
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_DWIDTH));
				strTemp.TrimRight();
				strTemp.TrimLeft();
				_stscanf(strTemp,_T("%d %d"),&(dwWidth),&(dwTmp));
				bGlobalDwidth = TRUE;
				dwGlobalDwdith = (int)dwWidth;
				continue;
			}

			/* Read CHARSET_REGISTRY */
			if(strLine.Find(_CHARSET_REGISTRY) == 0)
			{
				memset(m_tBH.szFontName,0,sizeof(m_tBH.szFontName));
				strTemp =strLine.Right(strLine.GetLength()-_tcslen(_CHARSET_REGISTRY));
				strTemp.TrimLeft();
				strTemp.TrimRight();
				strTemp.TrimLeft(_T("\""));
				strTemp.TrimRight(_T("\""));	
				strTemp.TrimLeft();
				strTemp.TrimRight();
				if(strTemp.CompareNoCase(_T("GB2312")) ==  0 || strTemp.CompareNoCase(_T("GBK")) == 0 )
				{
					bUnicodeEncoding  = FALSE;
				}
				else if(strTemp.CompareNoCase(_T("ISO10646")) == 0)
				{
					bUnicodeEncoding  = TRUE;
				}
				else
				{					
					file.Close();
					CString StrErr;
					StrErr.Format(_T("BDF file [%s] \nhas unknown CHARSET_REGISTRY \"%s\"!\n"),lpszBDF,strTemp);
					AfxMessageBox(StrErr);	
					return FALSE;
				}
				
				continue;
			}
			/* Read BITCOUNT */
			if(strLine.Find(_BITCOUNT) == 0)
			{
				int nBitCount = 0;
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_BITCOUNT));
				_stscanf(strTemp,_T("%d"),&nBitCount);
				if(nBitCount == 8)
				{
					bGray = TRUE;
				}
				
				continue;
			}
		}
		else
		{
			/* Read CHARS char count */
			strTemp = strLine.Right(strLine.GetLength()-_tcslen(_CHARS));
			strTemp.TrimRight();
			strTemp.TrimLeft();
			if(!strTemp.IsEmpty())
			{
				_stscanf(strTemp,_T("%d"),&(m_tBH.dwCharCount));
				if(m_tBH.dwCharCount == 0)
				{				
					file.Close();
					CString StrErr;
					StrErr.Format(_T("BDF file [%s] has zero char!\n"),lpszBDF);
					AfxMessageBox(StrErr);	
					return FALSE;
				}
			}
	
			break;		
		}
		if(strLine.Find(_STARTCHAR) ==0)
		{
			bHasReadFisrtStartChar = TRUE;
			break;
		}
	}

	if(m_tBH.dwFBBX==0 || m_tBH.dwFBBY==0 )
	{
		file.Close();
		CString StrErr;
		StrErr.Format(_T("BDF file [%s] is not correct!\r\n [FONTBOUNDINGBOX %d %d ]\n Please correct the above param!"),lpszBDF,
			m_tBH.dwFBBX,m_tBH.dwFBBY);

		AfxMessageBox(StrErr);		
		return FALSE;
	}
	
	if(m_tBH.dwPointSize == 0)
	{
		m_tBH.dwPointSize = m_tBH.dwFBBY;
	}
	
	if(m_tBH.dwFBBX > 0xFF)
	{
		m_tBH.dwFBBX = 0xFF;
	}

	if(m_tBH.dwFBBY > 0xFF)
	{
		m_tBH.dwFBBY = 0xFF;
	}

	m_tBH.dwFBBXns = m_tBH.dwFBBX;
	m_tBH.dwFBBYns = m_tBH.dwFBBY;
	m_tBH.nXoffns  = m_tBH.nXoff;
 	m_tBH.nYoffns  = m_tBH.nYoff;	

	BOOL bOldGray = FALSE;
	if(m_pGlyphTables[nFontSizeIndex].hdr.type == FONT_TYPE_GRAY_RLE ||
	   m_pGlyphTables[nFontSizeIndex].hdr.type == FONT_TYPE_GRAY )
	{
		bOldGray = TRUE;
	}
	if(bOldGray != bGray)
	{
		AfxMessageBox(_T("Font type (gray and mono) not match!"));	
		return FALSE;
	}
	
	//m_pGlyphTables[nFontSizeIndex].hdr.size= (BYTE)m_tBH.dwFBBY;


	//m_pGlyphTables[nFontSizeIndex].hdr.type= (BYTE)(bGray?FONT_TYPE_GRAY_RLE : FONT_TYPE_MONO);


	if(!bHasReadFisrtStartChar)
	{
		while(file.ReadString(strLine))
		{
			if(strLine.Find(_STARTCHAR) ==0)
			{
				bHasReadFisrtStartChar = TRUE;
				break;
			}
		}
	}

	if(m_tBH.dwCharCount == 0)
	{
		m_tBH.dwCharCount = 0xFFFF;
	}
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,m_tBH.dwCharCount);

	GLYPH_BODY_PTR pGlyph = NULL;
	GLYPH_BODY_PTR pCurGlyph = NULL;
	m_tBH.dwUCBegin = 0xFFFFFFFF;
    m_tBH.dwUCEnd  = 0;
	int nMaxY = 0;
	int nMaxH = 0;
//	int nGlyfId = 1;
	BOOL bFindDwidth = FALSE;
	MSG msg;
	int nCharCount = 0;

	//if(m_lstID.size() != 0)
	//{
	//	nGlyfId = m_lstID[m_lstID.size()-1] +1;
	//}

	DWORD nIdx = nFontSizeIndex;

	std::vector<UINT> vIDs;
	DWORD nID = 0;

	for(UINT i=0; i<m_tBH.dwCharCount && file.ReadString(strLine); i++)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}	
		::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,i+1);

		DWORD dwUCCode = 0;
		int   nFullWidth = 0;

        //omit STARTCHAR
		if(strLine.Find(_STARTCHAR) == 0)
		{
			if(!file.ReadString(strLine))  //Read ENCODING
			{
				break;
			}
		}

		bFindDwidth = FALSE;
		
		if(strLine.Find(_ENCODING) == 0)
		{
			strTemp = strLine.Right(strLine.GetLength()-_tcslen(_ENCODING));
			_stscanf(strTemp,_T("%d"),&dwUCCode);
			if(!bUnicodeEncoding)
			{
				WORD wChar = (WORD)dwUCCode;				
				BYTE str[2] = {0};
				str[0] = HIBYTE(wChar);
				str[1] = LOBYTE(wChar);
				if(str[0]==0)
				{
					str[0] = str[1]; //lint !e415
					str[1] = 0;
				}
				WORD uChar = 0;

				MultiByteToWideChar(CP_ACP,0,(LPSTR)str,2,(LPWSTR)&uChar,1);
				dwUCCode = uChar;
			}
		}

		
		/* Get start unicode and end unicode */
		if(m_tBH.dwUCBegin > dwUCCode)
		{
			m_tBH.dwUCBegin = dwUCCode;
		}

		if(m_tBH.dwUCEnd < dwUCCode)
		{
			m_tBH.dwUCEnd = dwUCCode;
		}
		
		BOOL bNeedChar = FALSE;
		if(!m_mapUnid.Lookup(dwUCCode,nID))
		{
			pCurGlyph = NULL;
		}
		else
		{
			bNeedChar = TRUE;
			nCharCount ++;
			vIDs.push_back(nID);
			m_mapGlyf.Lookup(nID,pGlyph);
			pCurGlyph = pGlyph+nIdx;
		}	


		DWORD dwBBw = 0;
		DWORD dwBBh = 0;
		int nBBxoff0x  =0;
		int nBByoff0y = 0;
		

		while(file.ReadString(strLine))
		{			
			if(strLine.Find(_ENDCHAR) == 0)
				break;

			if(!bNeedChar || pCurGlyph == NULL)
				continue;

			if(strLine.Find(_DWIDTH) == 0)
			{
				bFindDwidth = TRUE;
				DWORD dwWidth = 0;
				DWORD dwTmp = 0;
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_DWIDTH));
				strTemp.TrimRight();
				strTemp.TrimLeft();
				_stscanf(strTemp,_T("%d %d"),&(dwWidth),&(dwTmp));
				pCurGlyph->dwidth = (BYTE)dwWidth;
				continue;

			}
			else if(strLine.Find(_BBX) == 0)
			{
				strTemp = strLine.Right(strLine.GetLength()-_tcslen(_BBX));
				_stscanf(strTemp,_T("%d %d %d %d"),&(dwBBw),&(dwBBh),&(nBBxoff0x),&(nBByoff0y));
				int nw = dwBBw;
				nFullWidth = (int)((nw + 7) / 8);
				if(bGray)
				{
					nFullWidth = nw;
				}

				if(pCurGlyph->bitmap != NULL)
				{
					delete [] pCurGlyph->bitmap;
					pCurGlyph->bitmap = NULL;
				}
			
				if(dwBBw != 0 && dwBBh != 0)
				{
					pCurGlyph->bitmap = new BYTE[nFullWidth * dwBBh ];
					memset(pCurGlyph->bitmap,0,nFullWidth * dwBBh);
				}
				else
				{
					pCurGlyph->bitmap = NULL;
				}

				if(!bFindDwidth)
				{
					if(	bGlobalDwidth)
					{
						pCurGlyph->dwidth = (WORD)dwGlobalDwdith;
					}
					else
					{
						pCurGlyph->dwidth = (WORD)dwBBw;
					}
				}

				pCurGlyph->flag = FLAG_GLYF_ALL;
				pCurGlyph->width = (BYTE)dwBBw;
				pCurGlyph->height = (BYTE)dwBBh;				
				pCurGlyph->x =  (char)(nBBxoff0x - m_tBH.nXoff) ;
			    if(g_theApp.m_nBdfCalcBearingYStrategy == 0)
				{
					pCurGlyph->y = (char)((nBByoff0y - m_tBH.nYoff) + dwBBh);
				}
				else
				{
					pCurGlyph->y = (char)((nBByoff0y - m_tBH.nYoff) + m_tBH.dwFBBY);
				}
// 				if(pCurGlyph->width != 0 && pCurGlyph->dwidth == 0)
// 				{
// 					pCurGlyph->dwidth = (BYTE)(m_tBH.dwFBBX);	
// 				}
/*
				m_tMetrics[pCurGlyph->dwidth].dwidth_sum ++;
				m_tMetrics[(BYTE)(pCurGlyph->x)].x_sum ++;
				m_tMetrics[(BYTE)(pCurGlyph->y)].y_sum ++;
				m_tMetrics[pCurGlyph->width].width_sum ++;
				m_tMetrics[pCurGlyph->height].height_sum ++;

				if(((char)pCurGlyph->y) > nMaxY)
				{
					nMaxY = pCurGlyph->y;
				}

				if( (pCurGlyph->height - (pCurGlyph->y)*2) > nMaxH)
				{
					nMaxH = (pCurGlyph->height - (pCurGlyph->y)*2);
				}
*/
				continue;
			}

			/* Read BITMAP data */
			if(strLine.Find(_BITMAP) == 0)
			{
				for(UINT j=0; j<dwBBh && dwBBw != 0; j++)
				{
					if(!file.ReadString(strLine))
					{
						break;
					}
					strLine.TrimRight();
					HexCharToBinary(strLine,pCurGlyph->bitmap + j * nFullWidth);
				}
		
				int glyph_size = pCurGlyph->width * pCurGlyph->height;
				if(glyph_size!= 0 && bGray)
				{
					BYTE *pTmpBuf = new BYTE[glyph_size];
					memset(pTmpBuf,0,glyph_size);
					Font_Half_Sample(pCurGlyph->bitmap,pTmpBuf,pCurGlyph->width,pCurGlyph->height,0);
					memcpy(pCurGlyph->bitmap,pTmpBuf,glyph_size);
					delete [] pTmpBuf;
				}
				
//				FillDotMatrix(pCurChar); // fill the char with spaces 
//				AbstractGlyph(pCurChar); // abstract the glyph from the dot-matrix
			}
		}		
	}

	file.Close();

    ::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);

//	std::sort(vIDs.begin(),vIDs.end());

//	m_pGlyphTables[nIdx].pIDs = new UINT[vIDs.size()];
//	for(i = 0; i< vIDs.size(); i++)
//	{
//		m_pGlyphTables[nIdx].pIDs[i] =  vIDs[i];
//	}

	memset(&m_tMetrics,0,sizeof(m_tMetrics));
	int j = 0;
	int nCount = m_lstID.size();
	for( j = 0; j< nCount; j++ )
	{
		if(m_mapGlyf.Lookup(m_lstID[j],pCurGlyph) && pCurGlyph != NULL)
		{
			m_tMetrics[pCurGlyph->dwidth].dwidth_sum ++;
			m_tMetrics[(BYTE)(pCurGlyph->x)].x_sum ++;
			m_tMetrics[(BYTE)(pCurGlyph->y)].y_sum ++;
			m_tMetrics[pCurGlyph->width].width_sum ++;
			m_tMetrics[pCurGlyph->height].height_sum ++;
		}	
	}
	


	TEST_METRICS_T tMetrics;
	memset(&tMetrics,0,sizeof(tMetrics));
	for ( j = 0; j < TEST_METRICS_MAX; j++)
    {
        if ( m_tMetrics[j].dwidth_sum > tMetrics.dwidth_sum )
        {
            tMetrics.dwidth_sum = m_tMetrics[j].dwidth_sum;   
			m_pGlyphTables[nIdx].hdr.advance = (BYTE)j;
        }
		if ( m_tMetrics[j].x_sum > tMetrics.x_sum )
        {
            tMetrics.x_sum = m_tMetrics[j].x_sum;   
			m_pGlyphTables[nIdx].hdr.bearing_x = (BYTE)j;
        }
		if ( m_tMetrics[j].y_sum > tMetrics.y_sum )
        {
            tMetrics.y_sum = m_tMetrics[j].y_sum;   
			m_pGlyphTables[nIdx].hdr.bearing_y = (BYTE)j;
        }
		if ( m_tMetrics[j].width_sum > tMetrics.width_sum )
        {
            tMetrics.width_sum = m_tMetrics[j].width_sum;   
			m_pGlyphTables[nIdx].hdr.bitmap_w = (BYTE)j;
        }
		if ( m_tMetrics[j].height_sum > tMetrics.height_sum )
        {
            tMetrics.height_sum = m_tMetrics[j].height_sum;   
			m_pGlyphTables[nIdx].hdr.bitmap_h = (BYTE)j;
        }
    }

	//m_pGlyphTables[nIdx].hdr.page_offsets_base = sizeof(m_pGlyphTables[nIdx].hdr);
	//m_pGlyphTables[nIdx].hdr.glyph_count = (WORD)nCharCount;
	//m_pGlyphTables[nIdx].hdr.ppem_x = m_pGlyphTables[nIdx].hdr.ppem_y = m_pGlyphTables[nIdx].hdr.size;

	if(nMaxY<=0 || nMaxY>m_pGlyphTables[nIdx].hdr.size)
	{
		nMaxY = m_pGlyphTables[nIdx].hdr.size;
	}

	nMaxH += nMaxY;

	if(nMaxH<m_pGlyphTables[nIdx].hdr.size)
	{
		nMaxH = m_pGlyphTables[nIdx].hdr.size;
	}
	else if(nMaxH>(int)(m_pGlyphTables[nIdx].hdr.size*1.5))
	{
		nMaxH = (int)(m_pGlyphTables[nIdx].hdr.size*1.5);
	}

	m_pGlyphTables[nIdx].hdr.max_bearing_y = (char)nMaxY;
	m_pGlyphTables[nIdx].hdr.max_height = (BYTE)nMaxH;


	return TRUE;	
}

