// Generater.h: interface for the CGenerater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERATER_H__3912A388_C695_4BF0_861C_B92F4976DD44__INCLUDED_)
#define AFX_GENERATER_H__3912A388_C695_4BF0_861C_B92F4976DD44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sci_types.h"
#include "IImagWrapper.h"
#include "img_def.h"
#include "IResConverter.h"

#define MAX_FRAME_NUM 99

typedef struct     /*****  Frame infomation structure  ****/
{
	uint32 code_size;       /*code size */ //Zhemin.Lin, the MSB indicate if the code stream isn't commpressed
	                                       //if bit[31] == 0, it's run length encoding, else not encoding
	uint32 code_offset;     /*code start address*/
}FRAMEINFO;

typedef  struct                             /*****  Movie infomation structure  ****/
{
	uint32 frame_num;                       /* frame number */
	FRAMEINFO frame_infos[MAX_FRAME_NUM];   /* frame infomations*/
}MOVIEINFO;

class CGenerater  
{
public:
	CGenerater();
	CGenerater(BOOL bBigEndian);
	virtual ~CGenerater();
	
	BOOL BeginGenerate(LPCTSTR pFileName);
	BOOL Generate(LPCTSTR pArrayName, const uint8 * pContent, uint32 nContentSize, BOOL bReverse = FALSE );
	void EndGenerate(void);

	BOOL LoadImage(LPCTSTR pFileName, uint8 ** pCompressed, uint32 * pComressedLen,int nType);
	void FreeImage(uint8 *pBuf);

	BOOL LoadAnim(LPCTSTR pAnimFolder, uint8 ** pCompressed, uint32 * pComressedLen,int nType);
	void FreeAnim(uint8 *pBuf);

	BOOL GenerateBinFile(LPCTSTR lpszDstFile, const uint8 *pContent, uint32 nContentSize);

public:
	void DeleteLoadContent(LPBYTE pContent);	
	BOOL LoadFromFile(LPCTSTR pFileName, LPBYTE * pContent, int &nContentSize, int nConvertType);
	void FreeCompressed(uint8 * pCompressed);
	BOOL LoadBmpFile( LPCTSTR pszFileName, LPBYTE * pBmpStream, int * pSize = NULL, BOOL b565 = TRUE );
    void FreeBmpStream( LPBYTE pBmpStream ) { _ASSERTE( pBmpStream != NULL ); free(pBmpStream); }	

protected:
    CStdioFile			m_file;
    static const int	NUMPERLINE;	
	BOOL				m_bBigEndian;
	IImagWrapper	*	m_pImgWrapper;
	IResConverter   *   m_pResConv;
	TCHAR               m_szErrMsg[_MAX_PATH];


protected:
	void Bmp_888To565(LPBYTE p888, int n888Size, WORD * p565, int n565Size);
	void Bmp_888To444(LPBYTE p888, int n888Size, WORD * p444, int n444Size);
    BOOL Convert24To565Stream( LPBYTE p24Bmp, LPBYTE &p16Bmp, int &nSize );
    BOOL Convert555To565Stream( LPBYTE p555Bmp, LPBYTE &p565Bmp, int &nSize );
	BOOL BmpConvertToBigEndian(LPBYTE p565Bmp, int nSize, BOOL bCompressed);
	BOOL MovieConvertToBigEndian(LPBYTE pMovie, int nSize, BOOL bCompressed = TRUE);
	void ForceConvertBmpFile( LPCTSTR pszFileName );
	BOOL DeleteDirectory(LPCTSTR lpszDirName);

};

#endif // !defined(AFX_GENERATER_H__3912A388_C695_4BF0_861C_B92F4976DD44__INCLUDED_)
