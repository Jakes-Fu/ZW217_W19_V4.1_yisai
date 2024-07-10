// MmiLoader.cpp: implementation of the CMmiLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "nveditor.h"
#include "MmiLoader.h"

#include "nv_type.h"
#include "mmi_nv.h"
#include "mmi_pt.h"

int g_bCmdBoot = 0;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMmiLoader::CMmiLoader()
{
	m_pMmiFile = NULL;
}

CMmiLoader::~CMmiLoader()
{
	if ( NULL != m_pMmiFile )
	{
		::fclose( m_pMmiFile );
		m_pMmiFile = NULL;
	}
}

unsigned int CMmiLoader::Load( BYTE *pBuff, const TCHAR *szMmiFile )
{
	ASSERT( NULL != pBuff );
	ASSERT( NULL != szMmiFile );

	m_pMmiFile = ::_tfopen( szMmiFile, _T("rb") ); 
	if ( NULL == m_pMmiFile )
	{
		CString strErr;
		strErr.Format( _T("Can't open file %s"), szMmiFile );
		::AfxMessageBox( strErr );
		return -1;
	}

	// Check Mmi File header
	const char DPA_FILE_HEADER[] =  "SCDPAFILEVER0001";

	char szHead[ 20 ];
	int nLen = ::strlen( DPA_FILE_HEADER );
	::fread( szHead, nLen, 1, m_pMmiFile );
	szHead[ nLen ] = '\0';

	if ( ::strcmp( szHead, DPA_FILE_HEADER ) != 0 )
	{
		CString strErr;
		strErr.Format( _T("%s is not a invalid MMI NVItem file"), szMmiFile );
		::AfxMessageBox( strErr );
		return -1;
	}

	m_pBuff = pBuff;
	m_nOffset  = 0;
	
	if ( !ReadLockIP() )
	{
		::AfxMessageBox( _T("Read Lock IP fail") );
		return -1;
	}
	
	if ( !ReadLockDistance() )
	{
		::AfxMessageBox( _T("Read Lock distance fail") );
		return -1;
	}

	if ( !ReadLockPhoneCard() )
	{
		::AfxMessageBox( _T("Read Lock Phone Card fail") );
		return -1;
	}
	
	if ( !ReadPassword() )
	{
		::AfxMessageBox( _T("Read Password fail") );
		return -1;
	}
	
	if ( !ReadLockNetwork() )
	{
		::AfxMessageBox( _T("Read Lock Netword fail") );
		return -1;
	}
	
	if ( !ReadLockCell() )
	{
		::AfxMessageBox( _T("Read Lock Cell fail") );
		return -1;
	}
	
	if ( !ReadLockLac() )
	{
		::AfxMessageBox( _T("Read Lock Lac fail") );
		return -1;
	}
	
	if ( !ReadLocalList() )
	{
		::AfxMessageBox( _T("Read Local List fail") );
		return -1;
	}
	
	if ( !ReadForbidenList() )
	{
		::AfxMessageBox( _T("Read Forbiden List fail" ) );
		return -1;
	}
	
	if ( !ReadPin() )
	{
		::AfxMessageBox( _T("Read Pin fail" ) );
		return -1;
	}
	
	if ( !CreateFuncFlag() )
	{
		::AfxMessageBox( _T("Create Func flag fail") );
		return -1;
	}

	// Make sure 4-bytes alignment
//	m_nOffset = ( m_nOffset + 3 ) & 0xFFFFFFFC;

	return m_nOffset;
}


void CMmiLoader::InitItemHead( WORD wID, WORD wLen )
{
	// Make sure 4-bytes alignment
	m_nOffset = ( m_nOffset + 3 ) & 0xFFFFFFFC;

	// Write ID
	*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( wID );
	m_nOffset += sizeof( WORD );

	// Write len
	*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( wLen );
	m_nOffset += sizeof( WORD );
	// m_nOffset is still at 4-bytes alignment
}


int CMmiLoader::StringToBcd( const TCHAR *szNum )
{
	int i = 0;
	while ( szNum[ i ] != '\0' )
	{
		if ( i % 2 != 0 )
		{
			m_pBuff[ m_nOffset++ ] = ( ( ( ( szNum[ i - 1 ] - '0' ) << 4 ) & 0xF0 ) |
				                     ( ( szNum[ i ] - '0' ) & 0x0F ) );
		}
		i++;
	}

	// there is odd character in szNum
	if ( i % 2 != 0 )
	{
		m_pBuff[ m_nOffset++ ] = ( ( ( ( szNum[ i - 1 ] - '0' ) << 4 ) & 0xF0 ) |
			                       0x0F );
	}

	return i;
}

// Fill nLen bytes with 0xFF
void CMmiLoader::FillWithDefault( int nLen )
{
	while ( nLen-- > 0 )
	{
		m_pBuff[ m_nOffset++ ] = 0xFF;
	}
}

// MMINV_IPVALUE_INFO
//
// #define MMIPT_MAX_IPVALUE_LEN      6
//
// typedef struct
// {
//     char ip[MMIPT_MAX_IPVALUE_LEN];   
// }MMIPT_IPVALUE_T;

BOOL CMmiLoader::ReadLockIP( void )
{
	static const WORD ITEM_ID = MMINV_IPVALUE_INFO;
	static const WORD ITEM_LEN = MMIPT_MAX_IPVALUE_LEN;

	if ( ::fread( &m_bLockIP, sizeof( m_bLockIP ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	unsigned int nIP; 
	if ( ::fread( &nIP, sizeof( nIP ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	InitItemHead( ITEM_ID, ITEM_LEN );

	if( nIP )
    {
		if ( ::fread( &nIP, sizeof( nIP ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}
            
		char szIP[ 10 ];
        ::itoa( nIP, szIP, 10 );

		// + 1 to skip the first char
		int nLen = StringToBcd( szIP + 1 ); 
		if ( nLen >  ITEM_LEN * 2 )
		{
			return FALSE;
		}
    
		FillWithDefault( ITEM_LEN - ( nLen + 1 ) / 2 );
	}
	else
	{
		FillWithDefault( ITEM_LEN );
	}

	return TRUE;
}

// stub
BOOL CMmiLoader::ReadLockDistance( void )
{
	if ( ::fread( &m_bLockDistance, sizeof( m_bLockDistance ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	return TRUE;
}

// stub
BOOL CMmiLoader::ReadLockPhoneCard( void )
{
	if ( ::fread( &m_bLockPhone, sizeof( m_bLockPhone ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
	
	if ( ::fread( &m_bLockCard, sizeof( m_bLockCard ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
        
	int nValue;
    if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

    if ( nValue )
    {
		if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}
    }

    m_bLockPhoneCard = m_bLockPhone && m_bLockCard;

	return TRUE;
}

	
// MMINV_PSW_INFO
//
// #define MMIPT_MAX_PSW_LEN               7
// 
// typedef struct
// {
//     char psw[MMIPT_MAX_PSW_LEN];   
// }MMIPT_PSW_T;

BOOL CMmiLoader::ReadPassword( void )
{
	static const WORD ITEM_ID = MMINV_PSW_INFO;
	static const WORD ITEM_LEN = MMIPT_MAX_PSW_LEN;

	int nValue;
    if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	char Buff[ 10 ];
    ::itoa( nValue, Buff, 10 );
	char *szValue = Buff + 1; // +1 to skip the leading '1' added by liu kai

    InitItemHead( ITEM_ID, ITEM_LEN );

/*	
	int nLen = StringToBcd( szValue + 1 ); // + 1 to skip the leading '1'
	if ( nLen > ITEM_LEN * 2 )
	{
		return FALSE;
	}

	FillWithDefault( ITEM_LEN - ( nLen + 1 ) / 2 );
*/
	int nLen = ::strlen( szValue );
	if ( nLen + 1 > ITEM_LEN ) // the last element is for '\0'
	{
		return FALSE;
	}

	int i;
	for ( i=0; i<nLen; i++ )
	{
		m_pBuff[ m_nOffset++ ] = szValue[ i ];
	}
	for (; i<ITEM_LEN; i++ )
	{
		m_pBuff[ m_nOffset++ ] = '\0';
	}

	// Skip other field
	BOOL bChangePassword;
	if ( ::fread( &bChangePassword, sizeof( bChangePassword ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	return TRUE;
}
	
// MMINV_CELL_INFO( mcc, mnc )
//
// #define MMIPT_MAX_CELL_NUM              20
// #define MMIPT_MAX_LAC_NUM               10
// 
// typedef struct
// {
//     uint16 mcc;
//     uint16 mnc;
//     uint16 max_cell_num;
//     uint16 cell_id[MMIPT_MAX_CELL_NUM];
//     uint16 max_lac_num;
// 	   uint16 lac_id[MMIPT_MAX_LAC_NUM];
// }MMIPT_CELL_T;

BOOL CMmiLoader::ReadLockNetwork( void )
{
	static const ITEM_ID = MMINV_CELL_INFO;
	static const CELL_NUM = MMIPT_MAX_CELL_NUM;
	static const LAC_NUM = MMIPT_MAX_LAC_NUM;
	static const ITEM_LEN = 2 /* mcc */ + 2 /* mnc */  + 2 /* max_cell_num */ 
		                    + CELL_NUM * 2 /* cell_id */ + 2 /* max_lc_num */
							+ LAC_NUM * 2 /* lac_id */;

	if ( ::fread( &m_bLockNetwork, sizeof( m_bLockNetwork ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	int nIndex;
	if ( ::fread( &nIndex, sizeof( nIndex ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	InitItemHead( ITEM_ID, ITEM_LEN );

	if ( m_bLockNetwork )
	{
		// mcc
		WORD mcc = nIndex / 100;
		*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( mcc );
		m_nOffset += sizeof( WORD );
		// mnc
		WORD mnc = nIndex % 100;
		*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( mnc );
		m_nOffset += sizeof( WORD );
	}
	else
	{
		// Fill mcc, mnc with 0xFF
		FillWithDefault( 2 * sizeof( WORD ) );
	}

	return TRUE;
}
	
// MMINV_CELL_INFO ( max_cell_num, cell_id )
BOOL CMmiLoader::ReadLockCell( void )
{
	static const CELL_NUM = MMIPT_MAX_CELL_NUM;

	if ( ::fread( &m_bLockCell, sizeof( m_bLockCell ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	UINT uMaxCellCount;
	if ( ::fread( &uMaxCellCount, sizeof( uMaxCellCount ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
	ASSERT( uMaxCellCount <= CELL_NUM );

	// max_cell_num
	*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( (WORD)uMaxCellCount );
	m_nOffset += sizeof( WORD );

	int nCount;
	if ( ::fread( &nCount, sizeof( nCount ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
	ASSERT( nCount <= CELL_NUM );
	
	// cell_id
	for ( int i=0; i<nCount; i++ )
	{
		int nValue;
		char szValue[ 20 ];
		if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}
		::itoa( nValue, szValue, 10 );
		WORD wId = ( WORD )::atoi( szValue + 1 ); // Skip the leading '1'

		*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( wId ); 
		m_nOffset += sizeof( WORD );
	}
	
	FillWithDefault( ( CELL_NUM - nCount ) * 2 ); // * 2 bks Length is in bytes

	return TRUE;
}

// MMINV_CELL_INFO ( max_lac_num, lac_id )
BOOL CMmiLoader::ReadLockLac( void )
{
	static const WORD LAC_NUM = MMIPT_MAX_LAC_NUM;

	if ( ::fread( &m_bLockLac, sizeof( m_bLockLac ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	UINT uMaxLacCount;
	if ( ::fread( &uMaxLacCount, sizeof( uMaxLacCount ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
	ASSERT( uMaxLacCount <= LAC_NUM );

	// max_lac_num
	*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( (WORD)uMaxLacCount );
	m_nOffset += sizeof( WORD );

	int nCount;
	if ( ::fread( &nCount, sizeof( nCount ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
	ASSERT( nCount <= LAC_NUM );
	
	// lac_id
	for ( int i=0; i<nCount; i++ )
	{
		int nValue;
		char szValue[ 20 ];
		if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}
		::itoa( nValue, szValue, 10 );
		WORD wId = ( WORD )::atoi( szValue + 1 ); // Skip the leading '1'

		*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( wId ); 
		m_nOffset += sizeof( WORD );
	}
	
	FillWithDefault( ( LAC_NUM - nCount ) * 2 ); // * 2 bks Length is in byte

	return TRUE;
}	
	
#define NUMBER_RATE         1000000

// MMINV_LOCALNUM_INFO
//
// #define MMIPT_MAX_LOCALNUM_LEN     7
// #define MMIPT_MAX_LOCALNUM_NUM     200
// 
// typedef struct
// {
//     char local_num[MMIPT_MAX_LOCALNUM_NUM][MMIPT_MAX_LOCALNUM_LEN];   
// }MMIPT_LOCALNUM_T;

BOOL CMmiLoader::ReadLocalList( void )
{
	static const WORD ITEM_ID = MMINV_LOCALNUM_INFO;
	static const WORD ITEM_LEN = MMIPT_MAX_LOCALNUM_LEN;
	static const WORD ITEM_NUM = MMIPT_MAX_LOCALNUM_NUM;

	if ( ::fread( &m_bLocalList, sizeof( m_bLocalList ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	int nCount;
	if ( ::fread( &nCount, sizeof( nCount ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
	ASSERT( nCount <= ITEM_NUM );

	InitItemHead( ITEM_ID, ITEM_NUM * ITEM_LEN ); // two dimensional array

	// local_num
	for ( int i=0; i<nCount; i++ )
	{
		int nValue;
		if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}

		// the first byte of every row of local_num is for tag of area code
		if ( nValue )
		{
			m_pBuff[ m_nOffset++ ] = 0x01;
		}
		else
		{
			m_pBuff[ m_nOffset++ ] = 0x00;
		}

		// local number
		int nHighValue;
		int nLowValue;
		if ( ::fread( &nHighValue, sizeof( nHighValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}
		if ( ::fread( &nLowValue, sizeof( nLowValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}
		char szValue[ 20 ];

		unsigned __int64 value = nHighValue;
        value *= NUMBER_RATE;
        value += nLowValue;

		::_i64toa( value, szValue, 10 );
		int nLen = StringToBcd( szValue + 1 ); // + 1 to skip the leading '1'
		if ( nLen > ( ITEM_LEN - 1 ) * 2 ) // - 1 bks the first byte is area code
		{
			return FALSE;
		}

		FillWithDefault( ITEM_LEN - 1 - ( nLen + 1 ) / 2 );
	}
	
	FillWithDefault( ( ITEM_NUM - nCount ) * ITEM_LEN );

	return TRUE;

}
	
// MMINV_FORBINUM_INFO
//
// #define MMIPT_MAX_FORBINUM_NUM     100
// #define MMIPT_MAX_FORBINUM_LEN     6
// 
// typedef struct
// {
//     char forbi_num[MMIPT_MAX_FORBINUM_NUM][MMIPT_MAX_FORBINUM_LEN];   
// }MMIPT_FORBINUM_T;

BOOL CMmiLoader::ReadForbidenList( void )
{
	static const WORD ITEM_ID = MMINV_FORBINUM_INFO;
	static const WORD ITEM_LEN = MMIPT_MAX_FORBINUM_LEN;
	static const WORD ITEM_NUM = MMIPT_MAX_FORBINUM_NUM;

	if ( ::fread( &m_bForbidenList, sizeof( m_bForbidenList ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	int nCount;
	if ( ::fread( &nCount, sizeof( nCount ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}
	ASSERT( nCount <= ITEM_NUM );

	InitItemHead( ITEM_ID, ITEM_NUM * ITEM_LEN ); // two dimensional array of char

	for( int i=0; i<nCount; i++ )
	{		
		// forbiden number
		int nHighValue;
		int nLowValue;
		if ( ::fread( &nHighValue, sizeof( nHighValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}
		if ( ::fread( &nLowValue, sizeof( nLowValue ), 1, m_pMmiFile ) != 1 )
		{
			return FALSE;
		}

		unsigned __int64 value = nHighValue;
        value *= NUMBER_RATE;
        value += nLowValue;

		char szValue[ 20 ];
		::_i64toa( value, szValue, 10 );
		int nLen = StringToBcd( szValue + 1 ); // + 1 to skip the leading '1'
		if ( nLen > ITEM_LEN * 2 )
		{
			return FALSE;
		}

		FillWithDefault( ITEM_LEN - ( nLen + 1 ) / 2 );
	}
	
	FillWithDefault( ( ITEM_NUM - nCount ) * ITEM_LEN );

	return TRUE;
}

	
// MMINV_SIMCARD_INFO
// 
// #define MMIPT_MAX_PIN_NUM               2
// #define MMIPT_MAX_PIN_LEN               8
//
// typedef struct
// {
//     char pin1[MMIPT_MAX_PIN_NUM][MMIPT_MAX_PIN_LEN];   //the initial pin1 of the sim.
// }MMIPT_SIM_T;

BOOL CMmiLoader::ReadPin( void )
{
	static const WORD ITEM_ID = MMINV_SIMCARD_INFO;
	static const WORD ITEM_LEN = MMIPT_MAX_PIN_LEN;
	static const WORD ITEM_NUM = MMIPT_MAX_PIN_NUM;

	BOOL bChangePin;
	if ( ::fread( &bChangePin, sizeof( bChangePin ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	int nValue;
	if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	char szValue[ 10 ];

	if ( bChangePin )
	{
		::itoa( nValue, szValue, 10 );
	}
	else
	{
		::strcpy( szValue, "11234" ); // we add a '1' at the head of the string
		                              // to comply with data read from file 
	}

	InitItemHead( ITEM_ID, ITEM_NUM * ITEM_LEN );

	int nLen = StringToBcd( szValue + 1 ); // + 1 to skip the leading '1'
	if ( nLen > ITEM_LEN * 2 )
	{
		return FALSE;
	}
	FillWithDefault( ITEM_LEN - ( nLen + 1 ) / 2 );

	FillWithDefault( ( ITEM_NUM - 1 ) * ITEM_LEN ); // Fill other items other than pin1[ 0 ] with default value 0xFF

	// Skip other field
//	int nValue;
	if ( ::fread( &nValue, sizeof( nValue ), 1, m_pMmiFile ) != 1 )
	{
		return FALSE;
	}

	return TRUE;
}

// MMINV_FUNCFLAG_INFO
// uint16
BOOL CMmiLoader::CreateFuncFlag( void )
{
	static const WORD ITEM_ID = MMINV_FUNCFLAG_INFO;
	static const WORD ITEM_LEN = sizeof( WORD );

	static const WORD LOCKCARD = 0x0004; 
	static const WORD LOCKPHONE = 0x0008;
	static const WORD LOCKCELL = 0x0010;
	static const WORD LOCKLAC = 0x0020;
	static const WORD LOCKNETWORK = 0x0040;
	static const WORD LOCKDISTANCE = 0x0100;

	InitItemHead( ITEM_ID, ITEM_LEN );

	WORD wFlag = 0; // default value of func flag

	if ( m_bLockCard		)	wFlag |= LOCKCARD;
	if ( m_bLockPhone		)	wFlag |= LOCKPHONE;
	if ( m_bLockCell		)	wFlag |= LOCKCELL;
	if ( m_bLockLac			)	wFlag |= LOCKLAC;
	if ( m_bLockNetwork		)	wFlag |= LOCKNETWORK;
	if ( m_bLockDistance	)	wFlag |= LOCKDISTANCE;

	*( WORD* )&m_pBuff[ m_nOffset ] = CONVERT_WORD( wFlag );
	m_nOffset += sizeof( WORD );

	return TRUE;
}