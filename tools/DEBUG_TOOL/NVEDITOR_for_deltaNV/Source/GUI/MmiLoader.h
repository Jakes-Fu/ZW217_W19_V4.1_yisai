// MmiLoader.h: interface for the CMmiLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MMILOADER_H__B45A4E05_271A_415F_A2C2_E0251482FC34__INCLUDED_)
#define AFX_MMILOADER_H__B45A4E05_271A_415F_A2C2_E0251482FC34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMmiLoader  
{
public:
	CMmiLoader();
	virtual ~CMmiLoader();

public:
	unsigned int Load( BYTE *pBuff, const TCHAR *szMmiFile );

protected:

	inline WORD CONVERT_WORD( WORD wSrc) 
	{
		return MAKEWORD( HIBYTE( wSrc ), LOBYTE( wSrc ) );
	}


	inline DWORD CONVERT_DWORD( DWORD dwSrc )   
	{
		WORD wHigh = MAKEWORD( HIBYTE( HIWORD( dwSrc ) ), LOBYTE( HIWORD( dwSrc ) ) );
		WORD wLow  = MAKEWORD( HIBYTE( LOWORD( dwSrc ) ), LOBYTE( LOWORD( dwSrc ) ) );
		
		return MAKELONG( wHigh, wLow );                                
	}

	void InitItemHead( WORD wID, WORD wLen );
	int StringToBcd( const TCHAR *szNum );
	void FillWithDefault( int nLen );

	// MMINV_IPVALUE_INFO
	BOOL ReadLockIP( void );
	// stub
	BOOL ReadLockDistance( void );
	// stub
	BOOL ReadLockPhoneCard( void );
	// MMINV_PSW_INFO
	BOOL ReadPassword( void );
	// MMINV_CELL_INFO( MCC, MNC )
	BOOL ReadLockNetwork( void );
	// MMINV_CELL_INFO ( CELL )
	BOOL ReadLockCell( void );
	// MMINV_CELL_INFO ( LAC )
	BOOL ReadLockLac( void );
	// MMINV_LOCALNUM_INFO
	BOOL ReadLocalList( void );
	// MMINV_FORBINUM_INFO
	BOOL ReadForbidenList( void );
	// MMINV_SIMCARD_INFO
	BOOL ReadPin( void );
	// MMINV_FUNCFLAG_INFO
	BOOL CreateFuncFlag( void );

private:
	BYTE *m_pBuff;
	unsigned int  m_nOffset;
	FILE *m_pMmiFile;


	BOOL	m_bLockIP;
	BOOL	m_bLockDistance;
	BOOL	m_bLockPhone;
	BOOL	m_bLockCard;
	BOOL	m_bLockPhoneCard;
	BOOL    m_bLockNetwork;
	BOOL	m_bLockCell;
	BOOL	m_bLockLac;
	BOOL	m_bLocalList;
	BOOL	m_bForbidenList;

};

#endif // !defined(AFX_MMILOADER_H__B45A4E05_271A_415F_A2C2_E0251482FC34__INCLUDED_)
