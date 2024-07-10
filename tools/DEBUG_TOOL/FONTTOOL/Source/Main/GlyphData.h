// GlyphData.h: interface for the CGlyphData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLYPHDATA_H__7CEE885B_1F02_4E86_9779_5BA0414C4F7B__INCLUDED_)
#define AFX_GLYPHDATA_H__7CEE885B_1F02_4E86_9779_5BA0414C4F7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"
/*lint -save -e7 -e12 */
#include "ft2build.h"
#include FT_FREETYPE_H 
#include FT_GLYPH_H
#include FT_IMAGE_H
#include FT_OUTLINE_H
#include FT_BBOX_H

extern "C"
{
	#include "otf.h"
}
/*lint -restore */

#pragma warning( push,3 )
#include <list>
#include <vector>
#include <algorithm>
#pragma warning( pop )

#define FT_COLOR_MONO	0
#define FT_COLOR_ARGB	1
#define FT_COLOR_VECT   2

static const  DWORD g_hidi_sub[] ={
	/* FONT_GROUP_LATIN			*/	//OTF_TAG('l','a','t','n'),
	/* FONT_GROUP_CYRILLIC		*/	//OTF_TAG('c','y','r','l'),
	/* FONT_GROUP_GREEK			*/	//OTF_TAG('g','r','e','k'),
	/* FONT_GROUP_HAN			*/	//OTF_TAG('h','a','n','i'),
	/* FONT_GROUP_THAI			*/	//OTF_TAG('t','h','a','i'),
	/* FONT_GROUP_ARABIC		*/	//OTF_TAG('a','r','a','b'),
	/* FONT_GROUP_HEBREW		*/	//OTF_TAG('h','e','b','r'),
	/* FONT_GROUP_BENGALI		*/	OTF_TAG('b','e','n','g'),
	/* FONT_GROUP_DEVANAGARI	*/	OTF_TAG('d','e','v','a'),
	/* FONT_GROUP_GUJARATI		*/	OTF_TAG('g','u','j','r'),
	/* FONT_GROUP_GURMUKHI		*/	OTF_TAG('g','u','r','u'),
	/* FONT_GROUP_KANNADA		*/	OTF_TAG('k','n','d','a'),
	/* FONT_GROUP_MALAYALAM		*/	OTF_TAG('m','l','y','m'),
	/* FONT_GROUP_ORIYA			*/	OTF_TAG('o','r','y','a'),
	/* FONT_GROUP_TAMIL			*/	OTF_TAG('t','a','m','l'),
	/* FONT_GROUP_TELUGU		*/	OTF_TAG('t','e','l','u')
};

static const DWORD g_hidi = OTF_TAG('h','i','n','d');


typedef struct _BDF_ITEM_T
{
	int  nPTSize;
	int  nChars;
	BOOL bNormal;
	_TCHAR szBdfFile[_MAX_PATH];

	_BDF_ITEM_T()
	{
		memset(this,0,sizeof(_BDF_ITEM_T));
	}

}BDF_ITEM_T,*BDF_ITEM_PTR;

class CGlyphData  
{
public:
	CGlyphData();
	virtual ~CGlyphData();

public:
	void Init(HWND hWnd=NULL,BOOL bAdd = FALSE);
	void SetCharSetInfo(CHAR_SET_INFO_PTR pCSI);
	CHAR_SET_INFO_PTR GetCharSetInfo(){return m_pCSI;}
	BOOL Save(LPCTSTR lpszFileName, BOOL bBinFormat = TRUE,BOOL bBigEndian = TRUE);
	BOOL LoadFlib(LPCTSTR lpszFileName);
	BOOL LoadOtff(LPCTSTR lpszFileName);
	BOOL LoadBdfs(CStringArray &agNormalBdfs,CStringArray &agExtraBdfs);	
	BOOL SaveToBdf(LPCTSTR lpszFileName);
	void Clear(BOOL bExit = FALSE);
	GLYPH_BODY_PTR GetGlyph(UINT uCode,int nPtIndex,BOOL bUnicode = TRUE);

	void GetIDList(CUIntArray &agListID, BOOL bExIdList);
	
	DWORD GetIDFromUni(DWORD dwUnicode);
	DWORD GetUniFormID(DWORD dwID);

	BOOL IsReady(){return m_bReady;}

	int  GetPointSize(int nGlyfTblIdx);
	int  GetMaxBearingY(int nGlyfTblIdx);
	int  GetMaxHeight(int nGlyfTblIdx);

	GLYPH_TABLE_HDR_PTR GetGlyfTblHdr(int nGlyfTblIdx);
	
	BOOL IsVector();

	BOOL IsMergable(CGlyphData &gData);

	BOOL DoMerge(CGlyphData &gData);

	BOOL AdjustBearingXY(int nBXDelta,int nBYDelta, int nPtIndex);

	int  GetGlyfTblCount();

	BOOL IsLoadLib() {return m_bLoadLib;}

	BOOL RemoveFontSize(int nPtIndex);
	
	BOOL ReplaceBDF(LPCTSTR lpszBDF,int nFontSizeIndex);

private:	
	CMAP_DATA_T  m_cmap;
	GDEF_DATA_T  m_gdef;
	GSUB_DATA_T  m_gsub;
	GPOS_DATA_T  m_gpos;
	FONT_HDR_T   m_fontHDR;
	GLYPH_TABLE_PTR m_pGlyphTables;

	CHAR_SET_INFO_PTR m_pCSI;
	MAP_GLYPH  m_mapGlyf; //id->glyf
//	MAP_GLYPH  m_mapExGlyf;
	HWND       m_hWnd;

	FILE *      m_pTestFile;
	BOOL        m_bAdd;
	BOOL        m_bLoadLib;


private:
	DWORD WriteFontHdr(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian);
	DWORD WriteCmapTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian);
	DWORD WriteGdefTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian);
	DWORD WriteGsubTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian);
	DWORD WriteGposTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian);
	DWORD WriteGlyfTbl(FILE *pFile, BOOL bBinFormat, BOOL bBigEndian);

	BOOL  ReadCmapTbl(FILE *pFile);
	BOOL  ReadGdefTbl(FILE *pFile);
	BOOL  ReadGsubTbl(FILE *pFile);
	BOOL  ReadGposTbl(FILE *pFile);
	BOOL  ReadGlyfTbl(FILE *pFile);

	void ConvFontHdr(FONT_HDR_T &fht);
	void ConvCmapDat(CMAP_DATA_T &cmap);
	void ConvGdefDat(GDEF_DATA_T &gdef);
	void ConvGposDat(GPOS_DATA_T &gpos);
	void ConvGsubDat(GSUB_DATA_T &gsub);
	void ConvGlyfTbl(GLYPH_TABLE_PTR pGlyftbl);

	BOOL LoadBdf(LPCTSTR lpszFileName,int nIdx,BOOL bNormal);

	

	void FixCSI( );	

	_inline DWORD Conv32Endian(DWORD dwValue)
	{
		if(m_bBigEndian)
		{
			return MAKELONG( MAKEWORD(HIBYTE(HIWORD(dwValue)), LOBYTE(HIWORD(dwValue))),
							 MAKEWORD(HIBYTE(LOWORD(dwValue)), LOBYTE(LOWORD(dwValue)))); 
		}
		else
		{
			return dwValue;
		}
	}
	_inline WORD  Conv16Endian(WORD wValue)
	{
		if(m_bBigEndian)
		{
			return MAKEWORD(HIBYTE(wValue), LOBYTE(wValue));
		}
		else
		{
			return wValue;
		}
	}

	_inline DWORD Align4(FILE *pFile);

private:
/*lint -save -e601*/
	BOOL DumpCmap(OTF_cmap *cmap);	
	BOOL DumpGdef(OTF_GDEF *gdef,FILE * pFile);
	BOOL DumpGsub(OTF_GSUB *gsub,FILE * pFile);
	BOOL DumpGpos(OTF_GPOS *gpos,FILE * pFile);
	BOOL DumpGlyf(GLYPH_TABLE_T *glyftbl,int nIndex,int nPtSize,int nColorType = FT_COLOR_MONO);
	BOOL NeedOtherTble();

	BOOL IsInUCBound(DWORD dwUCCode,BOOL bExtra = FALSE);


	BOOL HexCharToBinary(LPCTSTR lpszHex,LPBYTE lpbBuf);
	BOOL BinaryToHexChar(LPBYTE lpbBuf,int nSize,LPTSTR lpszHex);

	void CalcPage(BOOL bModifyID);
	void FixCmap(BOOL bModifyID,CUIntArray *pUnicode = NULL);
	int  GetSectorIndex(WORD wUnicode);

	BOOL FixGlyfFlag(GLYPH_BODY_PTR pGlyph,GLYPH_TABLE_PTR pGlyfTbl);
	
	BOOL LoadMonoGlyph(GLYPH_BODY_PTR pGlyf,int nID);
	BOOL LoadArgbGlyph(GLYPH_BODY_PTR pGlyf,int nID);
	BOOL LoadVectorGlyph(GLYPH_BODY_PTR pGlyf,int nID,int nDispPtSize);

	int  WriteGlyfOutline(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian);
	int  ReadGlyfOutline(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian,UINT nID);
	int  RendOutline(GLYPH_BODY_PTR pGlyf,int nDispPtSize, UINT nID);

	int  WriteGlyfOutlineEx(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian);
	BYTE CalcOutlineCmpFlag(GLYPH_BODY_PTR pGlyf);
	int  ReadGlyfOutlineEx(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian,UINT nID);
	int  WriteComposeGlyf(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian);
    int  ReadComposeGlyf(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian);
    void DumpOutlineLog(LPCTSTR lpszFileName, BOOL bRead);
    void ComposeOutline(GLYPH_BODY_PTR pGlyf,UINT nID);
	void GetSubGlyfID();
	
	int  ReadGlyfOutlineFine(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian,UINT nID);
	int  WriteGlyfOutlineFine(GLYPH_BODY_PTR pGlyf,FILE *pFile, BOOL bBigEndian);
	
	void _CalcPage(std::vector<GLYPH_PAGE_PTR> &vctPage, std::vector<unsigned int> * pLstID, MAP_GLYPH & mapGlyph, int nFontTblIndex);

	void CalcMultiUnicodeOneID();
	
	
	MAP_ID_UN  m_mapIdun;    //id      -> unicode
	MAP_UN_ID  m_mapUnid;    //unicode -> id

	CMap<DWORD,DWORD,DWORD,DWORD>  m_mapRidun;   //repeat id -> multi-unicode

	std::vector<unsigned int>  m_lstID;
	std::vector<unsigned int>  m_lstUnicode;

	
	BOOL       m_bReady;

private:
	UINT		m_height ;
	FT_Library	m_library;    
    FT_Face		m_face;
	FT_Matrix   m_matrix;              /* transformation matrix */
	FT_Vector   m_vector;
	CString     m_strOtfFile;
	BOOL        m_bBigEndian;

	typedef struct  
	{
		DWORD dwidth_sum;
		DWORD x_sum;
		DWORD y_sum;
		DWORD width_sum;
		DWORD height_sum;
	}TEST_METRICS_T;

	#define TEST_METRICS_MAX 0x100
	TEST_METRICS_T m_tMetrics[TEST_METRICS_MAX];

	typedef struct  FT_SECTOR_T
	{
		WORD start_count;
		WORD end_count;
		short id_delta;
		WORD id_range_offset;
	}FT_SECTOR;

	inline BOOL IsIDRepeated(FT_SECTOR* pSec);
	inline BOOL IsIDSequence(FT_SECTOR* pSec,BOOL bModifyID,MAP_NUM &mapGlyfID);
	void   ResetUniSector(CUIntArray &agUnicode, CUIntArray &agSector,BOOL bModifyID,MAP_NUM &mapGlyfID);
	void   GetExtGlyfID(OTF_GSUB_GPOS *pGsubGpos, int gsub);
	BOOL   IsUsefulScriptTag(UINT tag);
	void   DumpGsubGlyfID(OTF_LookupSubTableGSUB * subtable,UINT type,OTF_GSUB *gsub);
	void   DumpGposGlyfID(OTF_LookupSubTableGPOS * subtable,UINT type,OTF_GPOS *gpos);

	void dump_ligature_set_list ( int *char_list, OTF_LigatureSet *ligset, unsigned num );
	void dump_alternate_set_list ( OTF_AlternateSet *altset, unsigned num );
	void dump_sequence_list ( OTF_Sequence *sequence, unsigned num);
	void dump_coverage_list ( OTF_Coverage *coverage, unsigned num );
	void dump_coverage ( OTF_Coverage *coverage,int** char_list_pptr);
	void dump_glyph_ids ( OTF_GlyphID *ids, unsigned count, unsigned end_num/*最后循环的数值, 0或1*/ );
	void dump_class_def (OTF_ClassDef *class_ptr );
	void dump_chain_class_set_list ( OTF_ChainClassSet *set, int count,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_chain_class_rule_list (OTF_ChainClassRule *rule, int count,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_chain_rule_set_list ( OTF_ChainRuleSet *set, int count ,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_chain_rule_list (OTF_ChainRule *rule, int count ,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_rule_set_list ( OTF_RuleSet *set, int count ,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_rule_list ( OTF_Rule *rule, int count,OTF_GSUB_GPOS * gsubgpos , int gsub);
	void dump_class_set_list ( OTF_ClassSet *set, int count,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_class_rule_list ( OTF_ClassRule *rule, int count ,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_lookup_record_list ( OTF_LookupRecord *rec, unsigned num,OTF_GSUB_GPOS * gsubgpos, int gsub);
	void dump_pair_set_list ( OTF_PairSet *set,unsigned count);


	void InitGlyfInfo(UINT glyfid,UINT unicode);
	long FixedFromDouble(double d);
	
	inline void FillMetrics(XGLYPH_METRICS_T &metrics,GLYPH_BODY_PTR pGlyf);

	void CopyGlyph(GLYPH_BODY_PTR pDst,GLYPH_BODY_PTR pSrc,GLYPH_TABLE_PTR pGlyphTbl);

	void MergeClear();

	BOOL GetBDFParam(BDF_ITEM_PTR pBDFItem);
	
	BOOL IsIDInTable(GLYPH_TABLE_PTR pTbl,UINT nID);

/*lint -restore*/

private:
	int m_nOutlineLongjmpNum;
	BOOL m_bZoomLargeFlag;
	int m_nOverOneByteNum;
};

#endif // !defined(AFX_GLYPHDATA_H__7CEE885B_1F02_4E86_9779_5BA0414C4F7B__INCLUDED_)
