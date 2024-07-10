// XmlConfigParse.cpp: implementation of the CXmlConfigParse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlConfigParse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define FILE_OMIT_FLAG  2

#define GET_XML_VALUE(INSTR,OUTSTR) {\
	OUTSTR.Empty(); \
	OUTSTR=INSTR;\
	OUTSTR.TrimLeft();\
	OUTSTR.TrimRight();\
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXmlConfigParse::CXmlConfigParse()
{
	m_xmlConfig.Close();
	m_mapProductInfo.RemoveAll();
	m_mapProductEnable.RemoveAll();
	m_aProductList.RemoveAll();
	m_pCurProduct = NULL;
	m_bOK = FALSE;
	m_strBaseConfig = _T("");
	m_mapPrdCfg.RemoveAll();
}

CXmlConfigParse::CXmlConfigParse(LPCTSTR lpcstrFileName)
{
	if(!Init(lpcstrFileName))
	{
		return;	
	}
	m_pCurProduct = NULL;
}

CXmlConfigParse::~CXmlConfigParse()
{
	Clear();
}

void CXmlConfigParse::Clear()
{
	m_bOK = FALSE;
	m_pCurProduct = NULL;
	int nCount = m_aProductList.GetSize();
	
	for(int i = 0; i< nCount; i++)
	{
        PPRODUCT_INFO_T pProd = NULL;
		m_mapProductInfo.Lookup(m_aProductList[i],pProd);
		
		if(pProd != NULL)
		{
			pProd->Clear();
			delete pProd;
		}
	}
	m_mapProductInfo.RemoveAll();
	m_mapProductEnable.RemoveAll();
	m_aProductList.RemoveAll();	
	m_mapPrdCfg.RemoveAll();
}


BOOL CXmlConfigParse::Init(LPCTSTR lpszFileName,int nFlag /*= 0*/)
{
	if(!lpszFileName)
	{
		_tprintf( _T("The file name must not be empty!") );
		return FALSE;
	}

	m_strBaseConfig = lpszFileName;
    if(nFlag == 0)
	{
		Clear();
		CString strBasePath= lpszFileName;
		int nFind = strBasePath.ReverseFind(_T('\\'));
		strBasePath = strBasePath.Left(nFind);
		
		m_bOK = LoadAllProductInfo(strBasePath);
	}
	else
	{
// 		m_aProductList.RemoveAll();
// 		m_mapProductInfo.RemoveAll();
// 		m_mapProductEnable.RemoveAll();
// 		m_mapPrdCfg.RemoveAll();
		
		m_bOK = _LoadProductInfo(lpszFileName,TRUE);	
	}



	return m_bOK;
	
}
/**
 * Get the number of products
 *
 * @return Returns dwCount,the number of products
 */ 
DWORD CXmlConfigParse::GetProductCount()
{	
	return m_aProductList.GetSize();
}
/**
 * Get product names
 *
 * @param arrProductNameList: store the names; 
 */   
/*void CXmlConfigParse::GetProductNameList(CStringArray &arrProductNameList)
{
	LPXNode lpChild;
	lpChild = m_xmlConfig.GetChild(_T("ProductList") );
	XNodes childs;
	childs = lpChild->GetChilds(_T("Product") );
	int  nCount = childs.size();
	CString strAttValue;
	for( int i = 0; i < nCount; i++)
	{
		strAttValue= childs[i]->GetAttrValue(_T("name"));
		ASSERT(!strAttValue.IsEmpty());
		arrProductNameList.Add(strAttValue);
	}
	childs.clear();

}*/
/**
 * Get product names
 *
 * @param pProductNameList: store the names; 
 * @param dwSize: original pProductNameList spaces size; 
 * @param dwRealSize: real used size; 
 */ 
void CXmlConfigParse::GetProductNameList(LPTSTR pProductNameList,DWORD dwSize,DWORD &dwRealSize)
{
	int  nCount = m_aProductList.GetSize();
	CString strName;
	dwRealSize = 0;
	memset(pProductNameList,0,dwSize*sizeof(_TCHAR));
	for( int i = 0; i < nCount; i++)
	{
		strName= m_aProductList.GetAt(i);
		ASSERT(!strName.IsEmpty());
		dwRealSize+=strName.GetLength();
		if(dwRealSize >= dwSize)
		{
			break;
		}
		_tcscat(pProductNameList,(LPCTSTR)strName);
		pProductNameList[dwRealSize]=_T(';');
		dwRealSize++; //+';'
		//arrProductNameList.Add(strAttValue);
	}
	pProductNameList[dwRealSize] = _T('\0');
}

PPRODUCT_INFO_T CXmlConfigParse::GetProdInfo(LPCTSTR lpszProductName)
{
	ASSERT(lpszProductName != NULL);
    BOOL bOk = m_mapProductInfo.Lookup(lpszProductName,m_pCurProduct);
	if(!bOk)
	{
		m_pCurProduct = NULL;
	}
	return m_pCurProduct;
}

/**
 * Release this
 */ 
void CXmlConfigParse::Release()
{
	Clear();
	delete this;
}

BOOL CXmlConfigParse::Success()
{
	return m_bOK;
}

BOOL CXmlConfigParse::LoadAllProductInfo(LPCTSTR lpszConfigBase)
{
	CString strWildPath = lpszConfigBase;
	strWildPath += _T("\\*.xml");
	CFileFind finder;
	BOOL bFound;
	bFound = finder.FindFile(strWildPath);
	CStringArray aFilePath;
	CObArray aTime;
	CString strPath;
	CString strName;
	CString strBaseConfg = m_strBaseConfig;
	int nFind = strBaseConfg.ReverseFind(_T('\\'));
    strBaseConfg = strBaseConfg.Right(strBaseConfg.GetLength() - nFind -1);
//	CTime *pCurTime;
	CTime lmCurTime;
	int i=0;
	while(bFound)
	{
		bFound = finder.FindNextFile();
	    strName = finder.GetFileName();
		if(strName.CompareNoCase(_T("BMAConfigSchema.xml")) == 0 ||
			strName.CompareNoCase(strBaseConfg)== 0)
		{
			continue;
		}
		else if(strName.Right(4).CompareNoCase(_T(".xml")) == 0)
		{
			strPath = finder.GetFilePath();
			finder.GetLastWriteTime(lmCurTime);
			
			int nCount = aFilePath.GetSize();
			if(nCount == 0)
			{
				aFilePath.Add(strPath);
				//pCurTime= new CTime(lmCurTime);
				aTime.Add((CObject *)&lmCurTime);
			}
			else
			{
				for(i=0; i<nCount;i++)
				{
					if(lmCurTime>= *(CTime*)aTime[i])
					{
						aTime.InsertAt(i,(CObject *)&lmCurTime);
						aFilePath.InsertAt(i,strPath);
						break;
					}
				}
				if(i>=nCount)
				{
					aTime.Add((CObject *)&lmCurTime);
					aFilePath.Add(strPath);
				}

			}
			
		}
	}
/*
	m_aProductList.RemoveAll();
	m_mapProductInfo.RemoveAll();
	m_mapProductEnable.RemoveAll();
	m_mapPrdCfg.RemoveAll();
*/
	aFilePath.Add(m_strBaseConfig);

    int nFileCount = aFilePath.GetSize();
	
	if(nFileCount == 0)
		return FALSE;
	
    BOOL bOk = TRUE;
	for(i=0;i<nFileCount;i++)
	{
		bOk = _LoadProductInfo(aFilePath.GetAt(i));
		if(!bOk)
			return FALSE;
	}

	return TRUE;
}

BOOL CXmlConfigParse::_Init(LPCTSTR lpszConfigName)
{
	_ASSERTE(lpszConfigName != NULL);

	// Open file
	CString strText;
	CString strNotes;
	CFile file;
	if ( !file.Open( lpszConfigName, CFile::modeRead ) )
	{
        _tprintf(_T("Unable to open xml config file"));
		return FALSE;
	}
	int nFileLen = (int)file.GetLength();
	
	// Allocate buffer for binary file data
	char* pBuffer = new char[nFileLen + 1];
	memset(pBuffer,0,nFileLen + 1);
	nFileLen = file.Read( pBuffer, nFileLen );
	file.Close();
	pBuffer[nFileLen] = '\0';
	

	_TCHAR *pBufferU = NULL;
	if(pBuffer[0] != (UCHAR)0xFF || pBuffer[1] != (UCHAR)0xFE)//not unicode 
	{
		pBufferU = new _TCHAR[nFileLen+1];
		memset(pBufferU,0,sizeof(_TCHAR)*(nFileLen + 1));
		MultiByteToWideChar(CP_ACP,0,pBuffer,nFileLen+1,pBufferU,nFileLen+1);
		strText.Empty();
		strText = pBufferU;
	}
	else
	{
		strText = (LPTSTR)pBuffer;
	}
	
	delete [] pBuffer;
	pBuffer = NULL;
	if(pBufferU != NULL)
	{
		delete [] pBufferU;
		pBufferU = NULL;
	}
	
	CString strXmlRet;
	strXmlRet = m_xmlConfig.Load(strText);
	strText.Empty();

	if(strXmlRet.IsEmpty())
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CXmlConfigParse::_LoadProductInfo(LPCTSTR lpszConfigName,BOOL bChangePrdName/*= FALSE*/)
{
	USES_CONVERSION;
	BOOL bRet = _Init(lpszConfigName);
	if(!bRet)
		return FALSE;

	PPRODUCT_INFO_T pProuctInfo = NULL;
	PPRODUCT_INFO_T pFindProuctInfo = NULL;

	LPXNode lpChild;
	lpChild = m_xmlConfig.GetChild(_T("ProductList") );

	if(lpChild == NULL) //check
		return FALSE;

	XNodes xnProductNodes;
	xnProductNodes = lpChild->GetChilds(_T("Product") );

	int  nCount = xnProductNodes.size();
	CString strCurSchemeName;
	CString strValue;
	int  nValue;
	int  nNvBkpItem;
	int i,j,k;
	UINT uEnable;


	for( i = 0; i < nCount; i++)
	{		
		strValue= xnProductNodes[i]->GetAttrValue(_T("name"));
		ASSERT(!strValue.IsEmpty());
		if(strValue.IsEmpty())
		{
			continue;
		}

		if(bChangePrdName)
		{
			strValue.TrimLeft();
			strValue.Insert(0,_T("PAC_"));
		}

		CString strEnable = xnProductNodes[i]->GetAttrValue(_T("enable"));
		nValue = 1;
		_stscanf( strEnable, _T("%d"), &nValue);

	
		pFindProuctInfo = NULL;
		if(!bChangePrdName)
		{
			//already exist
			if(m_mapProductInfo.Lookup(strValue,pFindProuctInfo))
			{		
				continue;
			}
			uEnable = 1;
			if(m_mapProductEnable.Lookup(strValue,uEnable) && uEnable == 0 )
			{				
				continue;				
			}
			else
			{
				m_mapProductEnable.SetAt(strValue,(UINT)nValue);
				if(nValue == 0) //
				{
					continue;
				}
			}
		}
		else // PAC file config must be high level
		{
			//already exist,must remove it and use this one
			if(m_mapProductInfo.Lookup(strValue,pFindProuctInfo))
			{		
				if(pFindProuctInfo != NULL)
				{
					pFindProuctInfo->Clear();
					delete pFindProuctInfo;
					pFindProuctInfo = NULL;
				}

				m_mapProductInfo.RemoveKey(strValue);
			}

			m_mapProductEnable.SetAt(strValue,(UINT)1);
		}


        pProuctInfo = new PRODUCT_INFO_T;

		m_aProductList.Add(strValue);
		m_mapProductInfo.SetAt(strValue,pProuctInfo);
		m_mapPrdCfg.SetAt(strValue,lpszConfigName);
		_tcscpy(pProuctInfo->szProductName,strValue);

		strCurSchemeName=xnProductNodes[i]->GetChildValue(_T("SchemeName"));
			
		strValue = xnProductNodes[i]->GetChildValue(_T("FlashTypeID"));
        _stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->dwFlashType = nValue;

		strValue = xnProductNodes[i]->GetChildValue(_T("Mode"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->dwMode = nValue;

		strValue = xnProductNodes[i]->GetChildValue(_T("ProductComment"));
		lstrcpyn(pProuctInfo->szComment,strValue,MAX_PATH+1);

        nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("NvBaseAddrChangeFlag"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->dwNvBaseChangeFlag = nValue;

		nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("NvNewBasePosition"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->dwNvNewBasePosition= nValue;
		
		nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("NVOrgFlag"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->dwNVOrgFlag = nValue;

		nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("NVOrgBasePosition"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->dwNVOrgBasePosition= nValue;
		
		nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("OmaDMFlag"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->dwOmaDMFlag= nValue;	
		
		nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("RebootByAT"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->bRebootByAT = (BYTE)nValue;

		nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("APR"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->bEnableAPR = (BYTE)nValue;

		nValue = 0;
		strValue = xnProductNodes[i]->GetChildValue(_T("PowerOff"));
		_stscanf( strValue, _T("%d"), &nValue);
		pProuctInfo->bPowerOff = (BYTE)nValue;

		// for NV backup        
        LPXNode lpNvBackup = NULL;
		lpNvBackup = xnProductNodes[i]->GetChild(_T("NVBackup"));
		if(lpNvBackup != NULL) // if "NVBackup" exist
		{
			nValue = 0;
			strValue = lpNvBackup->GetAttrValue(_T("backup"));
			_stscanf( strValue, _T("%d"), &nValue);
			pProuctInfo->dwNvBackupFlag = nValue;
			
			if(nValue != 0)
			{
				XNodes xnNvBkpList;
				xnNvBkpList = lpNvBackup->GetChilds(_T("NVItem"));
				pProuctInfo->dwNvBackupItemCount = xnNvBkpList.size();
				nNvBkpItem = pProuctInfo->dwNvBackupItemCount;
				if(nNvBkpItem != 0)
				{
					pProuctInfo->paNvBackupItem = new NV_BACKUP_ITEM_T[nNvBkpItem];
					memset(pProuctInfo->paNvBackupItem,0,sizeof(NV_BACKUP_ITEM_T)*nNvBkpItem);
					
					for(j = 0; j<nNvBkpItem;j++)
					{
						PNV_BACKUP_ITEM_T pNBIT = pProuctInfo->paNvBackupItem + j;
							
						strValue = xnNvBkpList[j]->GetAttrValue(_T("name"));
						_tcscpy(pNBIT->szItemName,strValue);
						
						strValue = xnNvBkpList[j]->GetAttrValue(_T("backup"));
						nValue = 0;
						_stscanf(strValue,_T("%d"),&nValue);
						pNBIT->wIsBackup = (WORD)nValue;

						strValue.Empty();
                        strValue = xnNvBkpList[j]->GetAttrValue(_T("scene"));
                        if (!strValue.IsEmpty())
                        {
                            strValue += _T(",");
                        }
                        _tcscpy(pNBIT->szScene,strValue.operator LPCTSTR());
                        

						strValue = xnNvBkpList[j]->GetChildAttrValue(_T("BackupFlag"),_T("use"));
						nValue = 0;
						_stscanf(strValue,_T("%d"),&nValue);
						pNBIT->wIsUseFlag = (WORD)nValue;

						strValue = xnNvBkpList[j]->GetChildValue(_T("ID"));
						_stscanf(strValue,_T("0x%x"),&(pNBIT->dwID));
						
						if(pNBIT->wIsUseFlag == 1)
						{
							XNodes xnNvFlagList;
							LPXNode lpxnFlag;
							lpxnFlag = xnNvBkpList[j]->GetChild(_T("BackupFlag"));
							if(lpxnFlag == NULL)
							{
								delete [] pProuctInfo->paNvBackupItem;
								pProuctInfo->paNvBackupItem = NULL;
								delete pProuctInfo;
								pProuctInfo = NULL;
								return FALSE;							
							}
							
							xnNvFlagList = lpxnFlag->GetChilds(_T("NVFlag"));
							int nNvFlagCount = xnNvFlagList.size();
							if(nNvFlagCount > MAX_NV_BACKUP_FALG_NUM)
							{
								nNvFlagCount = MAX_NV_BACKUP_FALG_NUM;
							}

							pNBIT->dwFlagCount = (DWORD)nNvFlagCount;	
							//TRACE(_T("%s: NvFlagCount:%d\n"),pProuctInfo->szProductName,nNvFlagCount);
						
							for(k=0; k<nNvFlagCount;k++)
							{		
								strValue = xnNvFlagList[k]->GetAttrValue(_T("name"));
								_tcscpy(pNBIT->nbftArray[k].szFlagName,strValue);
								
								strValue = xnNvFlagList[k]->GetAttrValue(_T("check"));
								_stscanf( strValue, _T("%d"), &(pNBIT->nbftArray[k].dwCheck));
							}
							xnNvFlagList.clear();
						}
					}
				}
				xnNvBkpList.clear();
			}			
		}
//////////////////////////////////////////////////////////////////////////
		// for Chips
		/*
		 *  <Chips enable="0">
		 *		<ChipItem id="0x222" name="L2"/>
		 *		<ChipItem id="0x777" name="L7"/>
		 *  </Chips>
		*/
        LPXNode lpChips = NULL;
		lpChips = xnProductNodes[i]->GetChild(_T("Chips"));
		if(lpChips != NULL) // if "Chips" exist
		{
			nValue = 0;
			strValue = lpChips->GetAttrValue(_T("enable"));
			_stscanf( strValue, _T("%d"), &nValue);
			pProuctInfo->tChips.bEnable = nValue;
			
			if(nValue != 0)
			{
				XNodes xnChipList;
				xnChipList = lpChips->GetChilds(_T("ChipItem"));
				UINT nChipCount = xnChipList.size(); 
				pProuctInfo->tChips.dwCount = nChipCount;			
				if(nChipCount != 0)
				{
					pProuctInfo->tChips.pChips = new CHIPITEM_T[nChipCount];
					memset(pProuctInfo->tChips.pChips,0,sizeof(CHIPITEM_T)*nChipCount);
					
					for(j = 0; j<(int)nChipCount;j++)
					{
						CHIPITEM_PTR pChipItem = pProuctInfo->tChips.pChips + j;					
						strValue.Empty();
						strValue = xnChipList[j]->GetAttrValue(_T("id"));
						nValue = 0;
						_stscanf(strValue,_T("0x%X"),&nValue);
						pChipItem->dwID = (DWORD)nValue;

						strValue.Empty();
						strValue = xnChipList[j]->GetAttrValue(_T("name"));
						_tcscpy(pChipItem->szName,strValue);
					}					
				}
				xnChipList.clear();
			}			
		}

		// for RF
		/*
		 *  <RF enable="1">
		 *		<Transceiver id="0x0" name="BOCA"/>
		 *		<Transceiver id="0x1" name="NAVAJO"/>
		 *  </RF>
		*/

		lpChips = xnProductNodes[i]->GetChild(_T("RF"));
		if(lpChips != NULL) // if "Chips" exist
		{
			nValue = 0;
			strValue = lpChips->GetAttrValue(_T("enable"));
			_stscanf( strValue, _T("%d"), &nValue);
			pProuctInfo->tRfChips.bEnable = nValue;

			if(nValue != 0)
			{
				XNodes xnChipList;
				xnChipList = lpChips->GetChilds(_T("Transceiver"));
				UINT nChipCount = xnChipList.size(); 
				pProuctInfo->tRfChips.dwCount = nChipCount;			
				if(nChipCount != 0)
				{
					pProuctInfo->tRfChips.pChips = new CHIPITEM_T[nChipCount];
					memset(pProuctInfo->tRfChips.pChips,0,sizeof(CHIPITEM_T)*nChipCount);

					for(j = 0; j<(int)nChipCount;j++)
					{
						CHIPITEM_PTR pChipItem = pProuctInfo->tRfChips.pChips + j;					
						strValue.Empty();
						strValue = xnChipList[j]->GetAttrValue(_T("id"));
						nValue = 0;
						_stscanf(strValue,_T("0x%X"),&nValue);
						pChipItem->dwID = (DWORD)nValue;

						strValue.Empty();
						strValue = xnChipList[j]->GetAttrValue(_T("name"));
						_tcscpy(pChipItem->szName,strValue);
					}					
				}
				xnChipList.clear();
			}			
		}
//////////////////////////////////////////////////////////////////////////     
        LPXNode lpSpecialStrings = NULL;
		lpSpecialStrings = xnProductNodes[i]->GetChild(_T("SpecialStrings"));
		if(lpSpecialStrings != NULL) // if "SpecialStrings" exist
		{
			nValue = 0;
			strValue = lpSpecialStrings->GetAttrValue(_T("enable"));
			_stscanf( strValue, _T("%d"), &nValue);			
			if(nValue != 0)
			{
				XNodes xnSpecialStrList;
				xnSpecialStrList = lpSpecialStrings->GetChilds(_T("SString"));
				int nSSItem =  xnSpecialStrList.size();				
				if(nSSItem != 0)
				{
					pProuctInfo->pSpecialStrings = new SPECIAL_STRING_ARR;
					
					for(j = 0; j<nSSItem;j++)
					{
						SPECIAL_STRING_PTR pSString = new SPECIAL_STRING_T;
							
						strValue = xnSpecialStrList[j]->GetAttrValue(_T("name"));
						_tcscpy(pSString->szName,strValue);
						
						strValue = xnSpecialStrList[j]->GetAttrValue(_T("content"));
						_tcscpy(pSString->szContent,strValue);

						strValue = xnSpecialStrList[j]->GetAttrValue(_T("included"));
						_tcscpy(pSString->szIncluedFileID,strValue);
						
				
						XNodes xnLinkedFileList;						
						xnLinkedFileList = xnSpecialStrList[j]->GetChilds(_T("LinkedFile"));
						int nLFNodeCount = xnLinkedFileList.size();
						
						if(nLFNodeCount>0)
						{
							pSString->pLinkedFileIDs = new LINKED_FILE_ARR;

							for(k=0; k<nLFNodeCount;k++)
							{	
								LINKED_FILE_PTR pLinkedFile = new LINKED_FILE_T;
								strValue = xnLinkedFileList[k]->GetAttrValue(_T("id"));
								_tcscpy(pLinkedFile->szFileID,strValue);
								
								strValue = xnLinkedFileList[k]->GetAttrValue(_T("download"));
								_stscanf( strValue, _T("%d"), &(pLinkedFile->dwDLFlag));

								pSString->pLinkedFileIDs->push_back(pLinkedFile);
							}
						}					
						pProuctInfo->pSpecialStrings->push_back(pSString);
						
						xnLinkedFileList.clear();
					}
				
				}
				xnSpecialStrList.clear();
			}
		}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
		// for Partitions
		/*
		 *  <Partitions>
		 *		<Partition id="modem" size="0x8000"/>
		 *		<Partition id="dsp" size="0x8000"/>
		 *  </Partitions>
		*/
		/* NEW SPPT
		<Partitions unit="0"> <!-- 0, 1M Byte; 1, 512K Byte; 2, 1K Byte; 3, 1 Byte; 4, 1Sector -->
            <Partition id="prodnv"	 size="5" gap="0"/>
			<Partition id="miscdata" size="1" gap="1"/>
		</Partitions>
		*/
        LPXNode lpPartitions = NULL;
		lpPartitions = xnProductNodes[i]->GetChild(_T("Partitions"));
		if(lpPartitions != NULL) // if "Chips" exist
		{			

			int nUnit = -1;
			strValue = lpPartitions->GetAttrValue(_T("unit"));
			_stscanf( strValue, _T("%d"), &nUnit);

			pProuctInfo->byteSPPTUnit = (BYTE)nUnit;
			switch(nUnit)
			{
			case 0:
				nUnit = 1024*1024;	//1M Bytes
				break;
			case 1:
				nUnit = 512*1024;	//512K Bytes
				break;
			case 2:
				nUnit = 1024;		//1K Bytes
				break;
			case -1:
			case 3:
			case 4:					//1 Sector
			default:
				nUnit = 1;			//1 Byte or Sector
				break;
			}

			XNodes xnPartList;
			xnPartList = lpPartitions->GetChilds(_T("Partition"));
			UINT nPartCount = xnPartList.size(); 
			pProuctInfo->dwPartitionCount = nPartCount;		
			if(nPartCount != 0)
			{
				pProuctInfo->pPartitions = new PARTITION_T[nPartCount];
				memset(pProuctInfo->pPartitions,0,sizeof(PARTITION_T)*nPartCount);
				
				for(j = 0; j<(int)nPartCount;j++)
				{
					PARTITION_T* pPart = pProuctInfo->pPartitions + j;					
					strValue.Empty();
					strValue = xnPartList[j]->GetAttrValue(_T("id"));
					strValue.TrimLeft();
					strValue.TrimRight();
					_tcsncpy(pPart->szID,strValue,MAX_REP_ID_LEN-1);

					GET_XML_VALUE(xnPartList[j]->GetAttrValue(_T("size")),strValue);
					strValue.MakeLower();
					if(strValue.CompareNoCase(_T("-1")) == 0)
					{
						pPart->llSize = -1;
					}
					else
					{
						if(strValue.Find(_T("0x")) == 0 )
						{
							_stscanf( strValue, _T("0x%llx"), &(pPart->llSize));
						}
						else
						{
							_stscanf( strValue, _T("%lld"), &(pPart->llSize));
						}
					}
					
					if(pPart->llSize != -1)
					{
						pPart->llSize = pPart->llSize * nUnit;
					}


					GET_XML_VALUE(xnPartList[j]->GetAttrValue(_T("gap")),strValue);
					strValue.MakeLower();
					if(strValue.Find(_T("0x")) == 0 )
					{
						_stscanf( strValue, _T("0x%llx"), &(pPart->llGap));
					}
					else
					{
						_stscanf( strValue, _T("%lld"), &(pPart->llGap));
					}	
					if(pPart->llGap != -1)
					{
						pPart->llGap = pPart->llGap * nUnit;
					}
				}					
			}
			xnPartList.clear();					
		}

		//////////////////////////////////////////////////////////////////////////
		// Start for ExtTable  @polo.jiang on 20141001
		/*
		 *  <ExtTable>
		 *		<PartitionType tag="paty">
		 *			<!--type: 0,IMG_RAW; 1,IMG_NV; 2,IMG_SPARSE -->
		 *			<Partition id="fixnv1"		id2="fixnv2"	type="1"  size="0xFFFFFFFF"/>
		 *			<Partition id="system"		id2=""			type="0"  />
         *		    <Partition id="userdata"	id2=""          type="2"/>
		 *		</PartitionType>
		 *		<OtherType tag="pat2">
		 *			<Other />
		 *		</OtherType>
		 *  </ExtTable>
		*/
		
		LPXNode lpExtTblNode = NULL;
		lpExtTblNode = xnProductNodes[i]->GetChild(_T("ExtTable"));
		if(lpExtTblNode != NULL) // if "ExtTable" exist
		{
			int nExtTblCounts = lpExtTblNode->GetChildCount();
			if (0 == nExtTblCounts)
			{
				return TRUE;
			}

			pProuctInfo->pExtTblHeader		= new EXTTBL_HEADER_T[nExtTblCounts];
			pProuctInfo->pExtTblData		= new EXTTBL_DATA_T[nExtTblCounts];
			if(NULL == pProuctInfo->pExtTblHeader || NULL == pProuctInfo->pExtTblData)
			{
				if (pProuctInfo->pExtTblHeader)
				{
					delete []pProuctInfo->pExtTblHeader;
					pProuctInfo->pExtTblHeader = NULL;
				}
				if (pProuctInfo->pExtTblData)
				{
					delete []pProuctInfo->pExtTblData;
					pProuctInfo->pExtTblData = NULL;
				}
				return FALSE;							
			}

			memset(pProuctInfo->pExtTblHeader,0,sizeof(EXTTBL_HEADER_T)*nExtTblCounts);
			memset(pProuctInfo->pExtTblData,0,sizeof(EXTTBL_DATA_T)*nExtTblCounts);

			pProuctInfo->dwExtTblCount		= nExtTblCounts;
			pProuctInfo->dwExtTblDataSize	= 0;
			int	nOffset						= EXTTABLE_COUNT_LEN;	// 4Byte table-count;
			int	nIndex						= 0;


			// Start to get PartitionType info 
			LPXNode lpPtnType = NULL;
			lpPtnType = lpExtTblNode->GetChild(_T("PartitionType"));
			if(lpPtnType != NULL) // if "PartitionType" exist
			{
				EXTTBL_HEADER_T* pItemHeader= pProuctInfo->pExtTblHeader+nIndex;
				EXTTBL_DATA_T* pItemData	= pProuctInfo->pExtTblData+nIndex;
				nOffset						+= sizeof(EXTTBL_HEADER_T);

				strValue = lpPtnType->GetAttrValue(_T("tag"));
				int nTagLen = strValue.GetLength();
				char* pTag = T2A(strValue.operator LPCTSTR());
				for (int n=0 ; n<4 && n<nTagLen; ++n)
				{
					pItemHeader->byTag[n] = pTag[n];
				}		
				
				//Start to get PartitionType items
				XNodes xnPtnItemList;
				xnPtnItemList = lpPtnType->GetChilds(_T("Partition"));
				int nPtnItemCount = xnPtnItemList.size(); 
				
				pItemHeader->dwOffset = nOffset;
				pItemHeader->dwSize	  = sizeof(PARTITION_INFO_T)*nPtnItemCount;
				
				if ( 0 != nPtnItemCount )
				{
					pItemData->dwSize = sizeof(PARTITION_INFO_T)*nPtnItemCount;
					pItemData->pData  = (LPBYTE) new PARTITION_INFO_T[nPtnItemCount];
					if ( NULL == pItemData->pData )
					{
						pProuctInfo->Clear();
						return FALSE;
					}
					memset(pItemData->pData,0,pItemData->dwSize);
					pProuctInfo->dwExtTblDataSize += pItemData->dwSize;

					PARTITION_INFO_T* pPtnInfo = (PARTITION_INFO_T*)(pItemData->pData);
					
					for ( int i=0; i < nPtnItemCount; ++i )
					{
						PARTITION_INFO_T* pPart = pPtnInfo + i;	
						GET_XML_VALUE(xnPtnItemList[i]->GetAttrValue(_T("id")),strValue);
						_tcsncpy(pPart->szID1,strValue,MAX_REP_ID_LEN-1);
						
						GET_XML_VALUE(xnPtnItemList[i]->GetAttrValue(_T("id2")),strValue);
						_tcsncpy(pPart->szID2,strValue,MAX_REP_ID_LEN-1);

						GET_XML_VALUE(xnPtnItemList[i]->GetAttrValue(_T("size")),strValue);
						strValue.MakeLower();
						if(strValue.Find(_T("0x")) == 0 )
						{
							_stscanf( strValue, _T("0x%x"), &(pPart->dwSize));
						}
						else if (!strValue.IsEmpty())
						{
							_stscanf( strValue, _T("%d"), &(pPart->dwSize));
						}				
						
						GET_XML_VALUE(xnPtnItemList[i]->GetAttrValue(_T("type")),strValue);
						if ( strValue.GetLength() >0 )
						{
							pPart->type = (BYTE)(T2A(strValue.operator LPCTSTR())[0]-'0');
						}	
					}
					
					nOffset += sizeof(PARTITION_INFO_T)*nPtnItemCount;	
				}
				xnPtnItemList.clear();
				//End get PartitionType items
				++nIndex;
			}
			
			//End get PartitionType info 
			
			//when need to add other info,just modified nOffset,nIndex and pProuctInfo->dwExtTblDataSize parts.

			/* such as
			LPXNode lpOtherType = NULL;
			lpOtherType = lpExtTblNode->GetChild(_T("XXXX"));
			if(lpPtnType != NULL) // if "XXXX" exist
			{
			}
			nOffset;
			pProuctInfo->dwExtTblDataSize;
			++nIndex;
			*/
		}
		
		//	end for ExtTable @ polo.jiang on 20141001
		////////////////////////////////////////////////////////////////////////// 
		
		LPXNode lpSchemeListNode = m_xmlConfig.GetChild( _T("SchemeList") );
		XNodes xnSchemeNodeList;
		xnSchemeNodeList = lpSchemeListNode->GetChilds(_T("Scheme"));
		int nSchemeCount = xnSchemeNodeList.size();
		
		for(j = 0; j<nSchemeCount;j++)
		{
			strValue= xnSchemeNodeList[j]->GetAttrValue(_T("name"));
			ASSERT(!strValue.IsEmpty());
			
			if(strValue.Compare(strCurSchemeName)==0)
			{
				XNodes xnFileNodeList;
				xnFileNodeList = xnSchemeNodeList[j]->GetChilds(_T("File"));
				pProuctInfo->dwFileCount = xnFileNodeList.size();
				int nFileCount = (int)(pProuctInfo->dwFileCount);
				if(nFileCount > 0)
				{
					pProuctInfo->pFileInfoArr = new FILE_INFO_T[nFileCount];
					memset(pProuctInfo->pFileInfoArr,0,sizeof(FILE_INFO_T)*nFileCount);
					for(k=0;k<nFileCount;k++)
					{
						PFILE_INFO_T pFIT = pProuctInfo->pFileInfoArr + k;
						
						nValue = 255; //backup default value
						strValue = xnFileNodeList[k]->GetAttrValue(_T("backup"));
						_stscanf( strValue, _T("%d"), &nValue);
						pFIT->isBackup = (BYTE)nValue;
	
						nValue = 0;
						strValue = xnFileNodeList[k]->GetAttrValue(_T("selbyflash"));
						_stscanf( strValue, _T("%d"), &nValue);
						pFIT->isSelByFlashInfo = (BYTE)nValue;

						nValue = 0;
						strValue = xnFileNodeList[k]->GetAttrValue(_T("selbyrf"));
						_stscanf( strValue, _T("%d"), &nValue);
						pFIT->isSelByRf = (BYTE)nValue;	

						strValue = xnFileNodeList[k]->GetAttrValue(_T("CheckCali"));
						if (strValue.IsEmpty())
						{
							strValue = _T("GSM");
						}
						SetCaliFlag(pFIT,strValue.operator LPCTSTR());

						strValue = xnFileNodeList[k]->GetChildValue(_T("ID"));	
						_tcscpy(pFIT->szID,strValue);
						
						strValue.Empty();
						strValue = xnFileNodeList[k]->GetChildValue(_T("IDAlias"));	
						if(!strValue.IsEmpty())
						{
							_tcscpy(pFIT->szIDAlias,strValue);
						}
						else
						{
							_tcscpy(pFIT->szIDAlias,pFIT->szID);
						}
						

						strValue = xnFileNodeList[k]->GetChildValue(_T("Type"));
						_tcscpy(pFIT->szType,strValue);
						
						strValue = xnFileNodeList[k]->GetChildValue(_T("Flag"));
						pFIT->dwFlag = (DWORD)_ttoi(strValue);
						
						strValue = xnFileNodeList[k]->GetChildValue(_T("CheckFlag"));
						pFIT->dwCheckFlag = (DWORD)_ttoi(strValue);
						
						strValue = xnFileNodeList[k]->GetChildValue(_T("Description"));
						_tcscpy(pFIT->szFileDescript,strValue);

						XNodes nxBlockList;
						nxBlockList = xnFileNodeList[k]->GetChilds(_T("Block"));
						int nBlockCount = nxBlockList.size();

						if(nBlockCount > MAX_BLOCK_NUM)
							nBlockCount = MAX_BLOCK_NUM;
						
						pFIT->dwBlockCount = (DWORD)nBlockCount;

						for(int t=0; t<nBlockCount;t++)
						{		
                            strValue = nxBlockList[t]->GetChildValue(_T("Base"));
                            _stscanf( strValue, _T("0x%llX"), &(pFIT->arrBlock[t].llBase));

                            strValue = nxBlockList[t]->GetChildValue(_T("Size"));
                            _stscanf( strValue, _T("0x%llX"), &(pFIT->arrBlock[t].llSize));

							strValue = nxBlockList[t]->GetAttrValue(_T("id"));			
							_tcsncpy(pFIT->arrBlock[t].szRepID,strValue,MAX_REP_ID_LEN-1);							
							
						}
						nxBlockList.clear();
					}
				}

				xnFileNodeList.clear();
				
				break;
			}
		}
        xnSchemeNodeList.clear();

		if(j >= nSchemeCount)
		{
			//TRACE(_T("Product scheme[%s] is not exist!\r\n"),strCurSchemeName);			
			continue;
		}
	}
	xnProductNodes.clear();	

	_Close();

	return TRUE;
}
//
//--------------------------------------------
//
void  CXmlConfigParse::SetCaliFlag(PFILE_INFO_T pFIT,LPCTSTR lpszCaliFlag)
{
	ASSERT(pFIT != NULL && lpszCaliFlag != NULL);
	CString strCali(lpszCaliFlag);
	pFIT->byCaliFlag = 0x00;
	CStringArray agCheckCali;
	int nCount = SplitStr(lpszCaliFlag,agCheckCali);

	for (int i = 0; i< nCount; ++i)
	{
		if (0 == agCheckCali[i].CompareNoCase(_T("GSM")))
		{
			pFIT->byCaliFlag |= 0x01;
		}
		else if (0 == agCheckCali[i].CompareNoCase(_T("TD")))
		{
			pFIT->byCaliFlag |= 0x02;
		}
		else if (0 == agCheckCali[i].CompareNoCase(_T("WCDMA")))
		{
			pFIT->byCaliFlag |= 0x04;
		}
		else if (0 == agCheckCali[i].CompareNoCase(_T("LTE")))
		{
			pFIT->byCaliFlag |= 0x08;
		}
		
	}

}

void  CXmlConfigParse::_Close()
{
	m_xmlConfig.Close();
}

LPTSTR  CXmlConfigParse::GetConfigFile(LPCTSTR lpszProductName)
{
	ASSERT(lpszProductName != NULL);
	static CString strConfigFile = _T("");

    BOOL bOk = m_mapPrdCfg.Lookup(lpszProductName,strConfigFile);
	if(!bOk)
	{
		return NULL;
	}
	return (LPTSTR)(LPCTSTR)strConfigFile;
}