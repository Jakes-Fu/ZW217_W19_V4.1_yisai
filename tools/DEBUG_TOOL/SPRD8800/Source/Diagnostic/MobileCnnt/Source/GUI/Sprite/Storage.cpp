// Storage.cpp: implementation of the CStorage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\mobilecnnt.h"
#include "Storage.h"
#include "InfoWrapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const _TCHAR CStorage::STORAGE_FLAG[] = _T("SCI_MOBILESPRITE_STORAGE_20040907");

const _TCHAR CStorage::STORAGE_FLAG_V1[] = _T("SCI_MOBILESPRITE_STORAGE_20080219");

const _TCHAR CStorage::STORAGE_PRESETSMS_FLAG[] = 
			_T("SCI_MOBILESPRITE_STG_PRESETSMS_20041117");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStorage::CStorage()
{
	m_pIW			= NULL;
	m_nSMSCount		= 0;	
	m_nPBCount		= 0;		
	m_nMemoCount	= 0;	
	m_nSchdlCount	= 0;
}

CStorage::~CStorage()
{
	m_pIW			= NULL;
}

BOOL CStorage::SavePresetSMSToFile(LPCTSTR pszFileName)
{
	_ASSERTE( pszFileName != NULL );
	_ASSERTE( m_file.m_hFile == CFile::hFileNull );

	m_pIW = theApp.GetInfoWrapper();
	if(NULL == m_pIW)
	{
		return TRUE;
	}

	try{
		if( !m_file.Open(pszFileName, CFile::modeCreate | CFile::modeWrite) )
		{
			AfxMessageBox(IDS_ERR_OPENFILE);
			return FALSE;
		}
	    
		m_file.Write( STORAGE_PRESETSMS_FLAG, sizeof(STORAGE_PRESETSMS_FLAG) );
	
		int nPresetSMSCount = m_pIW->GetPresetSMSCount();
		m_file.Write(&nPresetSMSCount, sizeof(nPresetSMSCount));

		PPRESETSMS psms = NULL;
		int nSize = sizeof(PRESETSMS);
		m_file.Write(&nSize, sizeof(nSize));

		for( int i = 0; i < nPresetSMSCount; ++i )
		{
			psms = m_pIW->GetPresetSMS(i);    
			m_file.Write(psms, nSize);
		}

		m_file.Close();
	}
	catch( CFileException * pfe )
	{
		pfe->ReportError();
		pfe->Delete();
		return FALSE;
	}
	catch( ... )
	{
		AfxMessageBox(IDS_ERR_UNKNOWN);
		return FALSE;
	}

	return TRUE;
}

BOOL CStorage::ReadPresetSMSFromFile(LPCTSTR pszFileName)
{
	_ASSERTE( pszFileName != NULL );
	_ASSERTE( m_file.m_hFile == CFile::hFileNull );

	m_pIW = theApp.GetInfoWrapper();
		
	if(NULL == m_pIW)
	{
		return TRUE;
	}

	try{
		if( !m_file.Open(pszFileName, CFile::modeRead) )
		{
			return FALSE;
		}
	    
		_TCHAR szFlag[128] = { 0 };
		m_file.Read( szFlag, sizeof(STORAGE_PRESETSMS_FLAG) );
		if( _tcscmp( szFlag, STORAGE_PRESETSMS_FLAG) != 0 )
		{
			AfxMessageBox(IDS_ERR_FILTTYPE);
			m_file.Close();
			return FALSE;
		}
		int nPresetSMSCount = 0;
		m_file.Read( &nPresetSMSCount, sizeof(nPresetSMSCount) );



		int nSize = 0;
		m_file.Read(&nSize, sizeof(nSize));
		
		PPRESETSMS psms = NULL;
		for( int i = 0; i < nPresetSMSCount; ++i )
		{
			psms = new PRESETSMS;    
			if( NULL == psms )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				return FALSE;
			}
			
			if( (int)m_file.Read(psms, nSize) < nSize )
			{
				delete psms;
				return FALSE;
			}
			m_pIW->AddPresetSMS(psms);
		}

		m_file.Close();
	}
	catch( CFileException * pfe )
	{
		pfe->ReportError();
		pfe->Delete();
		return FALSE;
	}
	catch( ... )
	{
		AfxMessageBox(IDS_ERR_UNKNOWN);
		return FALSE;
	}

	return TRUE;
}

BOOL CStorage::WriteSMS()
{
	_ASSERTE( m_file.m_hFile != CFile::hFileNull );

	PSMS psms = NULL;
	int nSize = sizeof(SMS);
	m_file.Write(&nSize, sizeof(nSize));

	WORD wStore = 0x00;
	DWORD nAddr=0;
	int nItem =0;
	PSMS pNext = NULL;
	for( int i = 0; i < m_nSMSCount; ++i )
	{
		psms = m_pIW->GetSMS(i);
		if(psms->pPrev !=NULL)
			continue;
		wStore = psms->wStore;
		psms->wStore = ID_STORE_PC;
		if(psms->pNext == NULL)
			m_file.Write(psms, nSize);
        else
		{
			nAddr = (DWORD)psms->pNext;
			pNext = psms->pNext;
			while(pNext != NULL)
			{
				nItem++;
				pNext = pNext->pNext;
			}
			psms->pNext = (PSMS)nItem;
			m_file.Write(psms, nSize);
			psms->pNext = (PSMS)nAddr;
			psms->wStore = wStore;	

			pNext = psms->pNext;
			while(pNext != NULL)
			{
				nAddr = (DWORD)pNext->pNext;
				pNext->pNext=NULL;
				wStore = pNext->wStore;
				pNext->wStore = ID_STORE_PC;
				m_file.Write(pNext, nSize);
				pNext->pNext = (PSMS)nAddr;
				pNext->wStore = wStore;
				pNext = pNext->pNext;				
			}			
		}

			
	}

	return TRUE;
}

BOOL CStorage::WritePB()
{
	_ASSERTE( m_file.m_hFile != CFile::hFileNull );

	PPB pPB = NULL;
	int nSize = sizeof(PB);
	m_file.Write(&nSize, sizeof(nSize));
    int nCount = m_pIW->GetPBCount(ID_STORE_ALL);

	//WORD wStore = 0x00;
	for( int i = 0; i < nCount; ++i )
	{
		pPB = m_pIW->GetPB(i);    
		//wStore = pPB->wStore;
		if(pPB->wStore == ID_STORE_PC)
			m_file.Write(pPB, nSize);
		//pPB->wStore = wStore;
	}

	return TRUE;
}

BOOL CStorage::WriteMemo()
{
	_ASSERTE( m_file.m_hFile != CFile::hFileNull );

	PMEMO pMEMO = NULL;
	int nSize = sizeof(MEMO);
	m_file.Write(&nSize, sizeof(nSize));
    int nCount = m_pIW->GetMEMOCount(ID_STORE_ALL);
	//WORD wStore = 0x00;
	for( int i = 0; i < nCount; ++i )
	{
		pMEMO = m_pIW->GetMEMO(i);
		//wStore = pMEMO->wStore;
		if(pMEMO->wStore == ID_STORE_PC)		
			m_file.Write(pMEMO, nSize);		
		//pMEMO->wStore = wStore;
	}

	return TRUE;
}

//DEL BOOL CStorage::WriteSchdl()
//DEL {
//DEL 	_ASSERTE( m_file.m_hFile != CFile::hFileNull );
//DEL 
//DEL 	PSCHDL pSCHDL = NULL;
//DEL 	int nSize = sizeof(SCHDL);
//DEL 	m_file.Write(&nSize, sizeof(nSize));
//DEL 
//DEL 	WORD wStore = 0x00;
//DEL 	for( int i = 0; i < m_nSchdlCount; ++i )
//DEL 	{
//DEL 		pSCHDL = m_pIW->GetSCHDL(i);   
//DEL 		if( pSCHDL->bUsed )
//DEL 		{
//DEL 			wStore = pSCHDL->wStore;
//DEL 			pSCHDL->wStore = ID_STORE_PC;
//DEL 
//DEL 			m_file.Write(pSCHDL, nSize);
//DEL 
//DEL 			pSCHDL->wStore = wStore;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	return TRUE;
//DEL }

BOOL CStorage::ReadSMS()
{
	_ASSERTE( m_file.m_hFile != CFile::hFileNull );

	int nSize = 0;
	m_file.Read(&nSize, sizeof(nSize));
	
	PSMS psms = NULL;
	PSMS ps = NULL;
	PSMS pPrev = NULL;
	PSMS pNext = NULL;
	for( int i = 0; i < m_nSMSCount; ++i )
	{
		psms = new SMS;    
		if( NULL == psms )
		{
			AfxMessageBox(IDS_ERR_RES_NOENOUGH);
			return FALSE;
		}

		if( (int)m_file.Read(psms, nSize) < nSize )
		{
			delete psms;
			return FALSE;
		}
		if(psms->pNext!=NULL)
		{
			ps = psms;
            int nItem = (DWORD)psms->pNext;
            for(int j=0;j<nItem;j++)
			{
				pNext = new SMS;
				if( NULL == pNext )
				{
					AfxMessageBox(IDS_ERR_RES_NOENOUGH);
					return FALSE;
				}

				if( (int)m_file.Read(pNext, nSize) < nSize )
				{
					delete pNext;
					return FALSE;
				}
				ps->pPrev = pPrev;
				ps->pNext = pNext;
				m_pIW->AddSMS(ps);

				pPrev = ps;
				ps = pNext;

				pNext->pNext = NULL;
				pNext->pPrev = pPrev;


			}
			m_pIW->AddSMS(pNext);

			i=i+nItem;

		}
		else
			m_pIW->AddSMS(psms);
	}

	return TRUE;
}

BOOL CStorage::ReadPB()
{
	_ASSERTE( m_file.m_hFile != CFile::hFileNull );

	int nSize = 0;
	m_file.Read(&nSize, sizeof(nSize));
	
	PPB pPB = NULL;
	PB_V0::PB pbv0;

	int i= 0;
	if(nSize == sizeof(PB))
	{
		for( i = 0; i < m_nPBCount; ++i )
		{
			pPB = new PB;    
			if( NULL == pPB )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				return FALSE;
			}
			
			if( (int)m_file.Read(pPB, nSize) < nSize )
			{
				delete pPB;
				return FALSE;
			}
			m_pIW->AddPB(pPB);
		}
	}
	else
	{
		for( i = 0; i < m_nPBCount; ++i )
		{
			memset(&pbv0,0,sizeof(pbv0));
			pPB = new PB;    
			if( NULL == pPB )
			{
				AfxMessageBox(IDS_ERR_RES_NOENOUGH);
				return FALSE;
			}
			
			if( (int)m_file.Read(&pbv0, nSize) < nSize )
			{
				delete pPB;
				return FALSE;
			}
			pPB->pNext = (PPB)pbv0.pNext;
			pPB->pPrev = (PPB)pbv0.pPrev;
			pPB->nID   = pbv0.nID;
			pPB->nGroupIndex = pbv0.nGroupIndex;
			pPB->wStore = pbv0.wStore;
			pPB->wReserve = pbv0.wReserve;

			_tcscpy(pPB->szName,pbv0.szName);
			_tcscpy(pPB->szPhone,pbv0.szPhone);
			_tcscpy(pPB->szHomePhone,pbv0.szHomePhone);
			_tcscpy(pPB->szOfficePhone,pbv0.szOfficePhone);

			m_pIW->AddPB(pPB);
		}
	}
	
	

	return TRUE;
}

BOOL CStorage::ReadMemo()
{
	_ASSERTE( m_file.m_hFile != CFile::hFileNull );

	int nSize = 0;
	m_file.Read(&nSize, sizeof(nSize));
	
	PMEMO pMEMO = NULL;
	for( int i = 0; i < m_nMemoCount; ++i )
	{
		pMEMO = new MEMO;    
		if( NULL == pMEMO )
		{
			AfxMessageBox(IDS_ERR_RES_NOENOUGH);
			return FALSE;
		}

		if( (int)m_file.Read(pMEMO, nSize) < nSize )
		{
			delete pMEMO;
			return FALSE;
		}
		m_pIW->AddMEMO(pMEMO);
	}

	return TRUE;
}

//DEL BOOL CStorage::ReadSchdl()
//DEL {
//DEL 	_ASSERTE( m_file.m_hFile != CFile::hFileNull );
//DEL 
//DEL 	int nSize = 0;
//DEL 	m_file.Read(&nSize, sizeof(nSize));
//DEL 	
//DEL 	PSCHDL pSCHDL = NULL;
//DEL 	for( int i = 0; i < m_nSchdlCount; ++i )
//DEL 	{
//DEL 		pSCHDL = new SCHDL;    
//DEL 		if( NULL == pSCHDL )
//DEL 		{
//DEL 			AfxMessageBox(IDS_ERR_RES_NOENOUGH);
//DEL 			return FALSE;
//DEL 		}
//DEL 
//DEL 		if( (int)m_file.Read(pSCHDL, nSize) < nSize )
//DEL 		{
//DEL 			delete pSCHDL;
//DEL 			return FALSE;
//DEL 		}
//DEL 		m_pIW->AddSCHDL(pSCHDL);
//DEL 	}
//DEL 
//DEL 	return TRUE;
//DEL }

BOOL CStorage::SaveMSToFile(LPCTSTR pszFileName)
{
	_ASSERTE( pszFileName != NULL );
	_ASSERTE( m_file.m_hFile == CFile::hFileNull );

	m_pIW = theApp.GetInfoWrapper();		
	if(NULL == m_pIW)
	{
		return TRUE;
	}

	try{
		if( !m_file.Open(pszFileName, CFile::modeCreate | CFile::modeWrite) )
		{
			AfxMessageBox(IDS_ERR_OPENFILE);
			return FALSE;
		}
	    
		m_file.Write( STORAGE_FLAG_V1, sizeof(STORAGE_FLAG_V1) );

		m_nSMSCount       = m_pIW->GetSMSCount(ID_STORE_ALL,FALSE);
		m_nPBCount        = m_pIW->GetPBCount(ID_STORE_ALL);
		m_nMemoCount      = m_pIW->GetMEMOCount(ID_STORE_ALL);
		//m_nSchdlCount     = m_pIW->GetSCHDLCount();
		PPB pPB =NULL;
		int nPBInPC =0;
		for(int j = 0;j<m_nPBCount;j++)
		{
			pPB = m_pIW->GetPB(j);
			if(pPB->wStore == ID_STORE_PC)
				nPBInPC++;
		}
        m_nPBCount=nPBInPC;

		PMEMO pMemo =NULL;
		int nMemoInPC =0;
		for(int k = 0;k<m_nMemoCount;k++)
		{
			pMemo = m_pIW->GetMEMO(k);
			if(pMemo->wStore == ID_STORE_PC)
				nMemoInPC++;
		}
        m_nMemoCount=nMemoInPC;

		m_file.Write( &m_nSMSCount,       sizeof(m_nSMSCount) );
		m_file.Write( &m_nPBCount,        sizeof(m_nPBCount) );
		m_file.Write( &m_nMemoCount,      sizeof(m_nMemoCount) );
		//m_file.Write( &m_nSchdlCount,     sizeof(m_nSchdlCount) );

		static const int nFunNum = 3;
		typedef BOOL (CStorage::*PFUN)(void);
		PFUN pWriteFun[nFunNum] = { 
							  WriteSMS,
							  WritePB,
							  WriteMemo };
		for( int i = 0; i < nFunNum; ++i )
		{
			if( ! (this->*(pWriteFun[i]))() )
			{
				break;
			}
		}
		
		m_file.Close();
	}
	catch( CFileException * pfe )
	{
		pfe->ReportError();
		pfe->Delete();
		return FALSE;
	}
	catch( ... )
	{
		AfxMessageBox(IDS_ERR_UNKNOWN);
		return FALSE;
	}
    
	return TRUE;
}

BOOL CStorage::ReadMSFromFile(LPCTSTR pszFileName)
{
	_ASSERTE( pszFileName != NULL );
	_ASSERTE( m_file.m_hFile == CFile::hFileNull );

	m_pIW = theApp.GetInfoWrapper();
		
	if(NULL == m_pIW)
	{
		return TRUE;
	}

	try{
		if( !m_file.Open(pszFileName, CFile::modeRead) )
		{
			AfxMessageBox(IDS_ERR_OPENFILE);
			return FALSE;
		}
	    
		_TCHAR szFlag[128] = { 0 };
		m_file.Read( szFlag, sizeof(STORAGE_FLAG_V1) );
		if( _tcscmp( szFlag, STORAGE_FLAG) != 0 && _tcscmp( szFlag, STORAGE_FLAG_V1) != 0  )
		{
			AfxMessageBox(IDS_ERR_FILTTYPE);
			m_file.Close();
			return FALSE;
		}

		m_file.Read( &m_nSMSCount,       sizeof(m_nSMSCount) );
		m_file.Read( &m_nPBCount,        sizeof(m_nPBCount) );
		m_file.Read( &m_nMemoCount,      sizeof(m_nMemoCount) );
		//m_file.Read( &m_nSchdlCount,     sizeof(m_nSchdlCount) );

		static const int nFunNum = 3;
		typedef BOOL (CStorage::*PFUN)(void);
		PFUN pReadFun[nFunNum] = { 
							  ReadSMS,
							  ReadPB,
							  ReadMemo };
		for( int i = 0; i < nFunNum; ++i )
		{
			if( ! (this->*(pReadFun[i]))() )
			{
				break;
			}
		}

		m_file.Close();
	}
	catch( CFileException * pfe )
	{
		pfe->ReportError();
		pfe->Delete();
		return FALSE;
	}
	catch( ... )
	{
		AfxMessageBox(IDS_ERR_UNKNOWN);
		return FALSE;
	}

	return TRUE;
}
