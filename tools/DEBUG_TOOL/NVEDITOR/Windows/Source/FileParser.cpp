// FileParser.cpp: implementation of the CFileParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "FileParser.h"
#include "msxml.h"/*xml file support by xmzhang add*/
#import  "msxml3.dll"/*xml file support by xmzhang add*/
#include <algorithm>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern "C"
{
#include "md5.h"
};


static const TCHAR *szItemType[] = 
{
	_T("NONE"),
	_T("CHAR"),
	_T("SHORT"),
	_T("NONE"),
	_T("LONG"),
	_T("ARRAY"),
	_T("ASTRING"),
	_T("STRUCT"),
	_T("WSTRING"),
};

static char szKey[] = "toBeoRNotTobeitisaqUesTion";

BOOL g_bBigEndian = TRUE;

#define MAX_DISP_ERR_MSG 80
#define ROOT_ELEMENT -1/*xml file support by xmzhang add*/
#define ATTRIBUTE_MAX 100
#define DECIMAL 10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// To decide lpszValue points to a valid decimal or hex 
// number string
BOOL ValueIsValid( TCHAR* lpszValue )
{
	ASSERT( lpszValue );
	
	CString strValue = lpszValue;
	
	if( strValue.Left(2).CompareNoCase( _T( "0x" ) ) )
	{
		// decimal,it must only include digits
		CString strValue1 = strValue.SpanIncluding( _T("0123456789") );
		if( strValue1 == strValue )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		// hex,it must only include digits and a,b,c,d,e,f
		strValue.Delete(0,2);
		strValue.MakeLower();
		CString strValue1 = strValue.SpanIncluding( _T("0123456789abcdef") );
		if( strValue1 == strValue )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

CFileParser::CFileParser()
{
    m_nAlignment   = 4;
    m_nModuleCount = 0;
	//    m_bDirty       = FALSE;
    
    
	//    m_lstModule.SetTypeSize(sizeof(Module));
	//    m_lstModuleItemPtr.SetTypeSize(sizeof(char *));
	
    m_hPrjFile = NULL;
    m_hModuleFile = NULL;
    m_szPrjDesc[0] = 0;
	
    // @ZHANG Ming : 2003-04-23
    // [[
    m_bWriteDsp = FALSE;
    // ]]
	
    // @ZHANG Ming : 2003-05-06
    // [[
    SetModified( FALSE );
    // ]]
    m_szappend = NULL;/*xml file support by xmzhang add*/

	//[[ by Joly
	m_bWriteMult = FALSE;
	m_nMulFileCount = 0;
	m_bHexFlag = TRUE;  // save as hexadecimal
	m_bTailFlag = TRUE;
	m_bStaticFlag = FALSE;
	/*memset(m_szNvContent, 0, MAX_TOTAL_ITEM_CONTENT);*/
	m_szNvContent = NULL;
	m_bIsNVImag = FALSE;
	m_bNVFlag = FALSE;
	m_dwNVLength = 0;
	m_dwFileLength = 0;
	m_bWriteFix = FALSE;
	m_bHashHeader = FALSE;
	m_bVerFlag = FALSE;
	//]]
}


CFileParser::~CFileParser()
{
	try
	{
		//Destory Item array object
		for(UINT Index = 0; Index < m_nModuleCount; Index++)
		{
			m_currModuleItemList = m_lstModuleItemPtr.GetAt(Index);
			for (INT Index2 = 0; Index2 < m_currModuleItemList->GetSize(); Index2++)
			{
				ModuleItem  tmpItem = m_currModuleItemList->GetAt(Index2);
				if (NULL != tmpItem.szItemDescription)
				{
					free(tmpItem.szItemDescription);
					tmpItem.szItemDescription = NULL;
				}
				
			}
			delete m_currModuleItemList;
		}
	}
	catch(...)
	{
		
	}
	m_vecFileInfo.clear();
    
}

/*xml file support by xmzhang add begin*/
void FeedTabs(MSXML2::IXMLDOMDocumentPtr pDoc, MSXML2::IXMLDOMNodePtr pParentNode, MSXML2::IXMLDOMNodePtr pRefNode, int tabs)
{
	_bstr_t str(_T("\n"));
	if(pRefNode)//μúò?×ó???°?ó\n\t\t\t...(tabs??)
	{
		for(int i=0; i<tabs; i++) 
		{
			str += _T("\t");
		}
		pParentNode->insertBefore(pDoc->createTextNode(str),(IUnknown *)pRefNode);
	}
	else//×?oóò?×óoó?ó\n\t\t\t...((tabs-1??))
	{
		for(int i=0; i<tabs-1; i++) 
		{
			str += _T("\t");
		}
		pParentNode->appendChild( pDoc->createTextNode(str) );
	}
}

void WellFormXml(MSXML2::IXMLDOMDocumentPtr pDoc, MSXML2::IXMLDOMNodePtr pParentNode, int tabs)
{
	if(pParentNode->hasChildNodes())
	{
		// μúò???o￠×ó???°?ó"\n\t.."(tabs??'\t')
		MSXML2::IXMLDOMNodePtr pFirstChild = pParentNode->GetfirstChild();
		FeedTabs(pDoc,pParentNode,pFirstChild,tabs);
		
		// ×?oóò???o￠×ó??oó?ó"\n\t.."(tabs-1??'\t')
		FeedTabs(pDoc,pParentNode,NULL,tabs);
		
		// μY1éo￠×ó
		WellFormXml(pDoc,pFirstChild,tabs+1);
	}
	
	// μY1éD?μü
	MSXML2::IXMLDOMNodePtr pNextSib = pParentNode->GetnextSibling();
	if( pNextSib ) 
	{
		MSXML2::IXMLDOMNodePtr pGrandParent = pParentNode->GetparentNode();
		if( (pGrandParent != NULL) && wcscmp(pNextSib->GetnodeTypeString(),L"text") ) 
		{
			// ?ú??ò???D?μü???°?ó"\n\t.."(tabs-1??'\t')
			FeedTabs(pDoc,pGrandParent,pNextSib,tabs-1);
			WellFormXml(pDoc,pNextSib,tabs);
		}
	}
}
/*xml file support by xmzhang add end*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CFileParser::CreateProject(const TCHAR *szPrjName, const TCHAR *szPrjFileName, 
                                const TCHAR *szTargetFileName,
                                const TCHAR *szPrjDesc)
{
    BOOL  bReturnStatus = TRUE;
    // @ZHANG Ming : 2003-04-18
    // [[
    // If Project is read-only
    BOOL bReadOnly = FALSE;
    // ]]
	
    _tcscpy(m_szPrjName, szPrjName);
    _tcscpy(m_szPrjFileName, szPrjFileName);
    if(NULL != szTargetFileName)
    {           
        _tcscpy(m_szTargetFileName , szTargetFileName);
    }
    else
    {
        _tcscpy(m_szTargetFileName , _T("") );
    }
	
    if(NULL != szPrjDesc)
    {           
        _tcscpy(m_szPrjDesc , szPrjDesc);
    }
    else
    {
        _tcscpy(m_szPrjDesc , _T("") );
    }
	
    if(NULL != m_hPrjFile)
    {
        //Notify that this file parser has loaded file 
        bReturnStatus =  FALSE;
    }
    else
    {
        // @ZHANG Ming : 2003-04-18
        // [[
        m_hPrjFile = OpenFileForWrite( m_szPrjFileName, &bReadOnly );
		
        if ( NULL == m_hPrjFile )
        {
            //Notify that open file failed
            _tprintf( _T("Can't open file %s!\n"), m_szPrjFileName );
            //MessageBox(NULL, strErr, _T("Error"), MB_OK);
			
            bReturnStatus =  FALSE;
            // ]]
        }
        else
        {       
            _ftprintf(m_hPrjFile, _T("%s\n%s\n\n%s\n"),
                _T("SpreadTrum NVEditor Project File, Format Version 1.0"),
                _T("# WARNING: DO NOT EDIT OR DELETE THIS PROJECT FILE!"),
                _T("################################################################################"));
			
            _ftprintf(m_hPrjFile, _T("\nProject: \"%s\"\n"), m_szPrjName);
            _ftprintf(m_hPrjFile, _T("\n# PROP Alignment %d\n"), m_nAlignment);
            if(NULL != szTargetFileName)
            {
                _ftprintf(m_hPrjFile, _T("# PROP Output_Dir \"%s\"\n"), szTargetFileName);
            }
            
            _ftprintf(m_hPrjFile, _T("\n%s\n"),
                _T("################################################################################"));
			
            _ftprintf(m_hPrjFile, _T("\n%s\n\n%s\n\n%s\n"),
                _T("# Begin Project Module"),
                _T("# End Project Module"),
                _T("################################################################################"));         
        }
		
    }
    
    if(NULL != m_hPrjFile)
    {
        fclose(m_hPrjFile);
        m_hPrjFile = NULL;
		
        // @ZHANG Ming : 2003-04-18
        // [[       
        if ( bReadOnly )
        {
            // Restore the file's read-only attribute
            VERIFY( ChangeFileRdOnlyAttri( m_szPrjFileName, TRUE ) );
        }
        // ]]
    }
	
    // @ZHANG Ming : 2003-05-06
    if ( bReturnStatus )
    {
        // a new project has been created
        SetModified( FALSE );
    }
	
    return bReturnStatus;
	
}								

/*xml file support by xmzhang add begin*/
BOOL CFileParser::CreateProjectXML(const TCHAR *szPrjName, const TCHAR *szPrjFileName, 
                                const TCHAR *szTargetFileName,
                                const TCHAR *szPrjDesc)
{
    BOOL  bReturnStatus = TRUE;
	
    _tcscpy(m_szPrjName, szPrjName);
    _tcscpy(m_szPrjFileName, szPrjFileName);
    if(NULL != szTargetFileName)
    {           
        _tcscpy(m_szTargetFileName , szTargetFileName);
    }
    else
    {
        _tcscpy(m_szTargetFileName , _T("") );
    }
	
    if(NULL != szPrjDesc)
    {           
        _tcscpy(m_szPrjDesc , szPrjDesc);
    }
    else
    {
        _tcscpy(m_szPrjDesc , _T("") );
    }
	
    MSXML2::IXMLDOMDocumentPtr pDoc;  
    HRESULT hr =pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));  
	
    if(!SUCCEEDED(hr))  
    {  
        //Notify that open file failed
        _tprintf( _T("Creat IXMLDOMDocumentPtr fail!\n"));
        //MessageBox(NULL, strErr, _T("Error"), MB_OK);
        return FALSE;
    }
	
    else
    {
        MSXML2::IXMLDOMElementPtr pDocElement=pDoc->createElement("PROJECT");
        MSXML2::IXMLDOMElementPtr pNewChildElement;
        MSXML2::IXMLDOMElementPtr pParentElement;
        char str[ATTRIBUTE_MAX]={0};
        pDocElement->setAttribute("PROJECT_NAME",m_szPrjName);
        pDocElement->setAttribute("ALIGNMENT",itoa(m_nAlignment,str,DECIMAL));
        //Remove the path info
        TCHAR *pResult = _tcsrchr(m_szTargetFileName,_T('\\'));
        if(pResult == NULL)
        {
            return S_FALSE;
        }
        pResult = pResult + 1;
        pDocElement->setAttribute("TARGET",pResult);
        pDocElement->setAttribute("DESCRIPTION",m_szPrjDesc);
        pDoc->appendChild(pDocElement);
        WellFormXml(pDoc,pDocElement,1);
        if(0==IsFileRdOnly(m_szPrjFileName))
        {
            pDoc->save(m_szPrjFileName);
        }
        else
        {
            if(ChangeFileForWrite(m_szPrjFileName))
            {
                pDoc->save(m_szPrjFileName);
                VERIFY( ChangeFileRdOnlyAttri( m_szPrjFileName, TRUE ) );
            }
            else
            {
                bReturnStatus = FALSE;
            }
        } 
        pDocElement.Release();
    }
    pDoc.Release();
	
    // @ZHANG Ming : 2003-05-06
    if ( bReturnStatus )
    {
        // a new project has been created
        SetModified( FALSE );
    }
	
    return bReturnStatus;
	
}		
/*xml file support by xmzhang add end*/


BOOL CFileParser::SaveProject()
{
	BOOL  bReturnStatus = TRUE;
	// @ZHANG Ming : 2003-04-18
	// [[
	BOOL bReadOnly = FALSE;
	// ]]
	
	if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
	// @ZHANG Ming : 2003-04-18
	// [[
	m_szappend=_tcsrchr(m_szPrjFileName,'.');			
	
	if(0==_tcsicmp(m_szappend,_T(".xprj")))
	{
		m_szappend++;
		_tcscpy(m_szappend,_T("prj"));
	}                  	
	m_hPrjFile = OpenFileForWrite(m_szPrjFileName, &bReadOnly );
	if(NULL == m_hPrjFile)
	{
		//Notify that open file failed
		_tprintf( _T("Can't open file %s!\n"), m_szPrjFileName );
		//MessageBox( NULL, strErr, _T("Error"), MB_OK );
		bReturnStatus =  FALSE;
	}
	else
	{       
		_ftprintf(m_hPrjFile, _T("%s\n%s\n\n%s\n"),
			_T("# SpreadTrum NVEditor Project File, Format Version 1.0"),
			_T("# WARNING: DO NOT EDIT OR DELETE THIS PROJECT FILE!"),
			_T("################################################################################"));
		
		_ftprintf(m_hPrjFile, _T("  PROJECT     =  %s\n"), m_szPrjName);
		_ftprintf(m_hPrjFile, _T("  ALIGNMENT   =  %d\n"), m_nAlignment);
		
		//Remove the path info
		TCHAR *pResult = _tcsrchr(m_szTargetFileName,_T('\\'));
		if(pResult == NULL)
		{
			return S_FALSE;
		}
		pResult = pResult + 1;
		
		_ftprintf(m_hPrjFile, _T("  TARGET      =  %s\n"), pResult);
		_ftprintf(m_hPrjFile, _T("  DESCRIPTION =  %s\n"), m_szPrjDesc);
		
		_ftprintf(m_hPrjFile, _T("\n%s\n"),
			_T("################################################################################"));
		
		_ftprintf(m_hPrjFile, _T("\n%s\n"),_T("# Begin Project Module"));
		_ftprintf(m_hPrjFile, _T("\n"));
		
		//write module list part
		for(UINT Index = 0; Index < m_nModuleCount; Index++)
		{
			m_CurrModule = m_lstModule.GetAt (Index);
			
			//Remove the path info
			TCHAR *pResult1 = _tcsrchr(m_CurrModule.szModuleFileName,_T('\\'));
			if(pResult1 == NULL)
			{
				return S_FALSE;
			}
			pResult1 = pResult1 + 1;

			/*xml file support by xmzhang modify begin*/			
			m_szappend=_tcsrchr(pResult1,'.');			   
			if(0==_tcsicmp(m_szappend,_T(".xml")))
			{
				m_szappend++;
				_tcscpy(m_szappend,_T("nvm"));
				m_lstModule.SetAt (Index, m_CurrModule);//save nvm 
			}     
			SaveModule(Index);
			/*xml file support by xmzhang modify end*/
			_ftprintf(m_hPrjFile, _T("  MODULE     =  %s\n"), pResult1);
			
		}
		
		_ftprintf(m_hPrjFile, _T("\n%s\n\n%s\n"),
			_T("# End Project Module"),
			_T("################################################################################"));         
	}
	
	if(NULL != m_hPrjFile)
	{        
		fclose(m_hPrjFile);
		m_hPrjFile = NULL;
		
		// @ZHANG Ming : 2003-04-18
		// [[       
		if ( bReadOnly )
		{
			// Restore the file's read-only attribute
			VERIFY( ChangeFileRdOnlyAttri( m_szPrjFileName, TRUE ) );
		}
		// ]]
	}
	
	if ( bReturnStatus )
	{
		// Save project successfully
		SetModified( FALSE );
	}
	
	return bReturnStatus;
}

/*xml file support by xmzhang add begin*/
BOOL CFileParser::SaveProjectXML()
{	
	
	if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
	// @ZHANG Ming : 2003-04-18
	// [[
	MSXML2::IXMLDOMDocumentPtr pDoc;  
	HRESULT hr =pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));  
	
	if(!SUCCEEDED(hr))  
	{  
		//Notify that open file failed
		_tprintf( _T("Creat IXMLDOMDocumentPtr fail!\n"));
		return FALSE;
	}
	
	else
	{
		MSXML2::IXMLDOMElementPtr pDocElement=pDoc->createElement("PROJECT");
		MSXML2::IXMLDOMElementPtr pNewChildElement;
		MSXML2::IXMLDOMElementPtr pParentElement;
		char str[ATTRIBUTE_MAX]={0};
		pDocElement->setAttribute("version",XML_VERSION);
		pDocElement->setAttribute("name",m_szPrjName);
		pDocElement->setAttribute("alignment",itoa(m_nAlignment,str,DECIMAL));
		//Remove the path info
		TCHAR *pResult = _tcsrchr(m_szTargetFileName,_T('\\'));
		if(pResult == NULL)
		{
			return S_FALSE;
		}
		pResult = pResult + 1;
		pDocElement->setAttribute("target",pResult);
		CString strDesc(m_szPrjDesc);
		strDesc.TrimLeft();
		strDesc.TrimRight();
		strDesc.TrimLeft(_T("\""));
		strDesc.TrimRight(_T("\""));

		pDocElement->setAttribute("description",strDesc.operator LPCTSTR());
        pDoc->appendChild(pDocElement);
		
		//write module list part
		for(UINT Index = 0; Index < m_nModuleCount; Index++)
		{
			m_CurrModule = m_lstModule.GetAt (Index);
			
			//Remove the path info
			TCHAR *pResult1 = _tcsrchr(m_CurrModule.szModuleFileName,_T('\\'));
			
			if(pResult1 == NULL)
			{
				return S_FALSE;
			}
			pResult1 = pResult1 + 1;
			
			m_szappend=_tcsrchr(pResult1,'.');			
			if(0==_tcsicmp(m_szappend,_T(".nvm")))
			{
				m_szappend++;
				_tcscpy(m_szappend,_T("xml"));
				m_lstModule.SetAt (Index, m_CurrModule);//save the modify into list,then refresh list
			}                  	
			SaveModuleXML(Index);
			pNewChildElement=pDoc->createElement("MODULE");
			pNewChildElement->setAttribute("name",pResult1);
			pDocElement->appendChild(pNewChildElement);
			pNewChildElement.Release();
			
		}
		m_szappend=_tcsrchr(m_szPrjFileName,'.');			
		
		if(0==_tcscmp(m_szappend,_T(".prj")))
		{
			m_szappend++;
			_tcscpy(m_szappend,_T("xprj"));
		}                  	
        
		WellFormXml(pDoc,pDocElement,1);
        
		if(0==IsFileRdOnly(m_szPrjFileName))
		{
			pDoc->save(m_szPrjFileName);
		}
		else
		{
			if(ChangeFileForWrite(m_szPrjFileName))
			{
				pDoc->save(m_szPrjFileName);
				VERIFY( ChangeFileRdOnlyAttri( m_szPrjFileName, TRUE ) );
				
			}
			else
			{
				return FALSE;
			}
		}
		pDocElement.Release();
	}
	
	SetModified( FALSE );
	pDoc.Release();
	
	return TRUE;
}
/*xml file support by xmzhang add end*/

BOOL CFileParser::SaveModule(INT nModuleIndex)
{
	//INT  bReturnCode = INVALID_MODULE_ID;
	FILE *fpFile = NULL;
	// @ZHANG Ming : 2003-04-18
	// [[
	BOOL bReadOnly;
	
	fpFile = OpenFileForWrite( m_CurrModule.szModuleFileName, &bReadOnly );
	if( NULL == fpFile )
	{
		//Notify that open file failed
		_tprintf( _T("Can't open file %s!\n"), m_CurrModule.szModuleFileName );
		//MessageBox(NULL, strErr, _T("Error"), MB_OK);
		// ]]
		return FALSE;
	}
	else
	{      
		
		_ftprintf(fpFile, _T("%s\n%s\n\n%s\n"),
			_T("# SpreadTrum NVEditor NV Module File, Format Version 1.0"),
			_T("# WARNING: DO NOT EDIT OR DELETE THIS NV Module FILE!"),
			_T("###############################################################################"));   
        
		CString strModuleDesc(m_CurrModule.szModuleDescription);
		strModuleDesc.TrimLeft();
		strModuleDesc.TrimRight();
		strModuleDesc.TrimLeft(_T("\""));
		strModuleDesc.TrimRight(_T("\""));
		
		_ftprintf(fpFile, _T("  MODULE      = %s\n"), m_CurrModule.szModuleName );
		_ftprintf(fpFile, _T("  DESCRIPTION = %s\n"), strModuleDesc);
		
		_ftprintf(fpFile, _T("\n%s\n"),
			_T("################################################################################"));
		
		_ftprintf(fpFile, _T("\n%s\n"),_T("# Begin Module Item"));
		
		m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
		m_CurrModule.nItemCount  = m_currModuleItemList->GetSize();
		
		for(UINT Index = 0; Index < m_CurrModule.nItemCount; Index++)
		{       
			m_CurrModuleItem = m_currModuleItemList->GetAt (Index);
			
			_ftprintf(fpFile, _T("  BEGIN_ITEM\n"));
			
			_ftprintf(fpFile, _T("    ITEM_NAME    = %s\n"), m_CurrModuleItem.szItemName  );
			_ftprintf(fpFile, _T("    ITEM_ID      = %d\n"), m_CurrModuleItem.nItemID );            
			_ftprintf(fpFile, _T("    ITEM_TYPE    = %d (%s)\n"), m_CurrModuleItem.eItemType, szItemType[m_CurrModuleItem.eItemType ] );
			if( ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				_ftprintf(fpFile, _T("    ITEM_SIZE    = %d\n"), m_CurrModuleItem.nArraySize  );
			}
			 
#ifdef _DEBUG
			_ftprintf(fpFile, _T("    ITEM_LENGTH  = %d\n"), m_CurrModuleItem.nItemLength );   
			_ftprintf(fpFile, _T("    ITEM_OFFSET  = %d\n"), m_CurrModuleItem.nOffset );  
#endif
			if( ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				STRUCT_ITEM_TYPE  == m_CurrModuleItem.eItemType ||  // by Joly
				ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType )  //  by Joly
			{
				_ftprintf(fpFile, _T("    ITEM_CONTENT = \n") );
			}
			else
			{	
				FormatContent();   //  by Joly
				_ftprintf(fpFile, _T("    ITEM_CONTENT = %s\n"), m_CurrModuleItem.szItemContent);
			}
			
			_ftprintf(fpFile, _T("    ITEM_INDEX   = %d\n"), m_CurrModuleItem.nItemIndex  );
			_ftprintf(fpFile, _T("    ITEM_PARENT  = %d\n"), m_CurrModuleItem.nParentItemIndex  );
			if (NULL != m_CurrModuleItem.szItemDescription)
			{
				_ftprintf(fpFile, _T("    ITEM_DESC    = %s\n"), m_CurrModuleItem.szItemDescription  );
			}
			else
			{
				_ftprintf(fpFile, _T("    ITEM_DESC    =  \n"));
			}
			
			if(m_CurrModuleItem.bReadOnly /*|| theApp.m_bWriteReadOnlyZeroItem*/) //MarkA
			{
				_ftprintf(fpFile, _T("    ITEM_READONLY = %d\n"), m_CurrModuleItem.bReadOnly  );
			}
			
			_ftprintf(fpFile, _T("  END_ITEM\n\n"), Index);
		}
		
		_ftprintf(fpFile, _T("\n%s\n\n%s\n"),
			_T("# End Module Item"),
			_T("################################################################################"));
		
		
		fclose(fpFile);
		fpFile = NULL;
		// @ZHANG Ming : 2003-04-18
		// [[       
		if ( bReadOnly )
		{
			// Restore the file's read-only attribute
			VERIFY( ChangeFileRdOnlyAttri( m_CurrModule.szModuleFileName, TRUE ) );
		}
		// ]]
	}
	
	return TRUE;
}

BOOL CFileParser::LoadProject(TCHAR *szPrjFileName,CStringArray& agWarning,BOOL bCheckItemName/* = TRUE*/)
{
	BOOL  bStatus = TRUE;
	TCHAR  szLineContent[MAX_PATH] = {0};
	TCHAR  *pszLine;
	TCHAR  szString[MAX_PATH] = {0};
	agWarning.RemoveAll();
	
	int nVer = 1;
	
	// 
	ClearProjectInfo();
	
	
	if(NULL != m_hPrjFile)
	{
		//Notify that this file parser has loaded file 
		bStatus =  FALSE;
	}
	else
	{
		_tcscpy(m_szPrjFileName, szPrjFileName);
		_tcscpy(m_szTargetFileName, _T("") );
		
		m_hPrjFile = _tfopen(m_szPrjFileName, _T("r") );
		if(NULL == m_hPrjFile)
		{
			//The given file is not exist then create it
			bStatus = FALSE;            
		}
		else
		{
			//Get Project attribute
			// the first line contains version information
			pszLine = _fgetts(szLineContent, MAX_PATH, m_hPrjFile);
			// decide project file version from the first line
			TCHAR szVersion1[] = _T("# SpreadTrum NVEditor Project File, Format Version 1.0");
			TCHAR szVersion2[] = _T("# SpreadTrum NVEditor Project File, Format Version 2.0");
			
			if ( NULL == pszLine )
			{
				_tprintf( _T("Invalid .prj file or the file corrupted!") );
				
				::fclose( m_hPrjFile );
				return FALSE;
			}
			
			if ( ::_tcsstr( pszLine, szVersion2 ) != NULL )
			{
				_tprintf( _T("This prj file is for customer only\n"));
				
				::fclose( m_hPrjFile );
				return FALSE;
				
				nVer = 2;    
			}
			else if ( ::_tcsstr( pszLine, szVersion1 ) == NULL )
			{
				
				_tprintf( _T("Invalid .prj file or the file corrupted") );
				
				::fclose( m_hPrjFile );
				return FALSE;
			}
			
			while(NULL != pszLine)
			{
				int tempVer=nVer;/*xml file support by xmzhang add*/
				//Remove the pre space
				while(' ' == *pszLine)
				{
					pszLine++;
				}
				//This line is comment
				if( ('#' != *pszLine) &&
					('\n' != *pszLine))
				{
					_stscanf(pszLine, _T("%s"), szString);
					pszLine = _tcschr(pszLine, '=');
					pszLine += 1;
					if(0 == (_tcscmp(szString, _T("PROJECT"))))
					{
						_stscanf(pszLine, _T("%s"),  m_szPrjName);
					}
					else if(0 == (_tcscmp(szString, _T("ALIGNMENT"))))
					{
						_stscanf(pszLine, _T("%d"),  &m_nAlignment);
					}
					else if(0 == (_tcscmp(szString, _T("TARGET"))))
					{
						_stscanf(pszLine, _T("%s"),  szString);
						_tcscpy(szLineContent, m_szPrjFileName);
						
						TCHAR *pResult = _tcsrchr(szLineContent,'\\');
						if(pResult == NULL)
						{
							return FALSE;
						}
						
						*(pResult + 1) = 0;
						
						
						_tcscat(szLineContent, szString);
						
						_tcscpy(m_szTargetFileName, szLineContent);
					}
					else if(0 == (_tcscmp(szString, _T("DESCRIPTION"))))
					{
						_stscanf(pszLine, _T("%s"),  m_szPrjDesc);
					}
					else if(0 == (_tcscmp(szString, _T("MODULE"))))
					{
						_stscanf(pszLine, _T("%s"), szString);
						_tcscpy(szLineContent, m_szPrjFileName);
						
						TCHAR *pResult = _tcsrchr(szLineContent,'\\');
						if(pResult == NULL)
						{
							return FALSE;
						}
						
						*(pResult + 1) = 0;
						
						_tcscat(szLineContent, szString);
                        /*xml file support by xmzhang add begin*/
						m_szappend=_tcsrchr(szString,'.');
						if(0==_tcsicmp(m_szappend,_T(".xml")))
						{
							nVer = 3;
						}
						/*xml file support by xmzhang add end*/
						
						if(nVer == 1)
						{
							if(LoadModule(szLineContent))
							{
								m_nModuleCount++;
							}
							// [[ modify by Joly
							else 
							{						
								_tprintf(_T("[ERROR] LoadModule %s failed...\n"), szLineContent);
								
								return FALSE;
							}
						}
						else if(nVer == 2)
						{
							if(LoadModule2(szLineContent))
							{
								m_nModuleCount++;
							}
							// [[ modify by Joly
							else
							{
								_tprintf( _T("[ERROR] LoadModule %s failed...\n"), szLineContent );
								
								
								//]]
								return FALSE;
							}
						}
						/*xml file support by xmzhang add begin*/
						else if(nVer == 3)
						{
							if(LoadModuleXML(szLineContent))
							{
								m_nModuleCount++;
							}
							//[[ modify by Joly
							else 
							{
								
								_tprintf( _T("[ERROR]  LoadModule %s failed...\n"), szLineContent );
								
								return FALSE;
							}
						}
						/*xml file support by xmzhang add end*/
					}
					else
					{
						
					}
				}
				
				pszLine = _fgetts(szLineContent, MAX_PATH, m_hPrjFile);
				nVer = tempVer;/*xml file support by xmzhang add*/
			}
			
        }
    }
    
    //Close Project file
    if(NULL != m_hPrjFile)
    {
        fclose(m_hPrjFile);
        m_hPrjFile = NULL;
    }
	
	// add by Joly 2015.6.1
	m_lstModule.FreeExtra();
	m_lstModuleItemPtr.FreeExtra();
	//[[ for NV version check by MD5
	CountMD5(m_mapIDs);
	//]]

	if (!m_strErrID.IsEmpty())
	{
		_tprintf(_T("Exist repeated IDs: %s\n"), m_strErrID);
		
		return FALSE;
		//	m_agStrError.Add(strTmp);
	}
	
    // @ZHANG Ming : 2003-05-06
    if ( bStatus )
    {
        SetModified( FALSE );
		if(bCheckItemName)
		{
			CStringArray agModuleError;
			CStringArray agModuleItemError;
			
			CheckNVModule(agModuleError);
			CheckModuleItem(agModuleItemError);
			
			agWarning.Append(agModuleError);
			agWarning.Append(agModuleItemError);
		}
		agWarning.Append(m_agStrError);
    }

    return bStatus;
}
CString CFileParser::GetStrFileTitle( LPCTSTR lpFile,BOOL bPostfix/* = TRUE*/,BOOL bMakeUpper/*=FALSE*/)
{
	CString str(lpFile);
	if (str.Find(_T('\\')) != -1)
		str = str.Right(str.GetLength()-str.ReverseFind(_T('\\'))-1);
	else if (str.Find(_T('/')) != -1)
		str = str.Right(str.GetLength()-str.ReverseFind(_T('/'))-1);
	
	if (!bPostfix)
	{
		if (str.Find(_T('.')) != -1)
			str = str.Left(str.Find(_T('.')));
	}
	
	
	if (bMakeUpper)
		str.MakeUpper();
	
	return str;
}
void CFileParser::CheckNVModule(CStringArray& agError)
{
	int nModuleCount = m_lstModule.GetSize();
	agError.RemoveAll();
	CStringArray agModule;
	int nAddIndex = 0;
	for (int i = 0; i < nModuleCount; ++i)
	{
		Module  tmpModule = m_lstModule.GetAt(i);
		BOOL bExistModule = FALSE;
		for (int j = 0; j < agModule.GetSize(); ++j )
		{
			CString strModule = agModule.GetAt(j);
			if (0 == strModule.CompareNoCase(tmpModule.szModuleName))
			{	
				CString strTmp,strErr;
				bExistModule = TRUE;
				
				Module  tmpCmpModule = m_lstModule.GetAt(j);
				strTmp.Format(_T("(%d)"),++nAddIndex);
				_stprintf(tmpModule.szModuleName,_T("%s"),strModule + strTmp);

				strErr.Format(_T("[WARNING] [NVM: %s] module name [%s] is same as [NVM: %s],Change to [%s]."),
					GetStrFileTitle(tmpModule.szModuleFileName),
					tmpCmpModule.szModuleName,
					GetStrFileTitle(tmpCmpModule.szModuleFileName),
					tmpModule.szModuleName
					);
				agError.Add(strErr);
				agModule.Add(tmpModule.szModuleName);
				m_lstModule.SetAt(i,tmpModule);
				break;
			}
		}
		if (!bExistModule)
		{
			agModule.Add(tmpModule.szModuleName);
		}	
	}
}

void CFileParser::GetModuleParentInfo(int nModuleIndex,CMapID &mapParentIndex)
{
	ModuleItem_List_Ptr currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
	int nModuleItemCount = GetModuleItemCount(nModuleIndex);
	for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; ++nCurItemIndex)
	{
		ModuleItem stCurrModuleItem = currModuleItemList->GetAt (nCurItemIndex);
		mapParentIndex[stCurrModuleItem.nParentItemIndex] = stCurrModuleItem.nParentItemIndex;
	}
}
void CFileParser::GetNVItemPath(LPCTSTR szModuleName, int nItemParentIndex,CString& strPath)
{
	CString strCurrentName;
	if (-1 != nItemParentIndex)
	{
		ModuleItem stCurrModuleItem = m_currModuleItemList->GetAt (nItemParentIndex);
		strPath.Insert(0,stCurrModuleItem.szItemName);
		strPath.Insert(0,_T("\\"));
		GetNVItemPath(szModuleName,stCurrModuleItem.nParentItemIndex,strPath);	
	}
	else
	{
		strPath.Insert(0,szModuleName);
	}
}
void CFileParser::CheckModuleItem(CStringArray& agError)
{
	int nModuleItemCount = 0;
	agError.RemoveAll();
	for(int nCurModuleIndex = 0; nCurModuleIndex < (int)m_nModuleCount; ++nCurModuleIndex )
    {               
		
		CMapID mapParentIndex;
		CMapID::iterator it;
		Module  stCurModule = m_lstModule.GetAt(nCurModuleIndex);
		GetModuleParentInfo(nCurModuleIndex,mapParentIndex);
		
		nModuleItemCount = GetModuleItemCount(nCurModuleIndex);
		
		for(it = mapParentIndex.begin(); it != mapParentIndex.end(); it++)
		{
			CStringArray agModuleItem;
			int nAddIndex = 0;
			for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; ++nCurItemIndex)
			{
				ModuleItem stCurrModuleItem = m_currModuleItemList->GetAt (nCurItemIndex);
				if ( it->second == stCurrModuleItem.nParentItemIndex)
				{

					BOOL bExistModuleItem = FALSE;
					for (int j = 0; j < agModuleItem.GetSize(); ++j )
					{
						CString strModuleItemName = agModuleItem.GetAt(j);
						if (0==strModuleItemName.CompareNoCase(stCurrModuleItem.szItemName))
						{
							
							CString strTmp,strErr,strItemPath;
							bExistModuleItem = TRUE;

							strTmp.Format(_T("(%d)"),++nAddIndex);
							_stprintf(stCurrModuleItem.szItemName,_T("%s"),strModuleItemName + strTmp);

							strItemPath.Format(_T("\\%s"),strModuleItemName);
							GetNVItemPath(stCurModule.szModuleName,stCurrModuleItem.nParentItemIndex,strItemPath);
							
							strErr.Format(_T("[WARNING] [NVM: %s] [ItemName: %s] is already exists,Change to [%s] ."),
								GetStrFileTitle(stCurModule.szModuleFileName),
								strItemPath,
								stCurrModuleItem.szItemName);
							
							agError.Add(strErr);
							agModuleItem.Add(stCurrModuleItem.szItemName);
							m_currModuleItemList->SetAt (nCurItemIndex,stCurrModuleItem);
							break;
						}
					}
					if (!bExistModuleItem)
					{
						agModuleItem.Add(stCurrModuleItem.szItemName);
					}	
				}
			}
			
		}
	}
}
/*xml file support by xmzhang add begin*/
BOOL CFileParser::LoadProjectXML(TCHAR *szPrjFileName,CStringArray& agWarning,BOOL bCheckItemName/* = TRUE*/)
{
	BOOL  bStatus = TRUE;
	TCHAR  szLineContent[MAX_PATH] = {0};
	TCHAR  szString[MAX_PATH] = {0};
	agWarning.RemoveAll();
	
	int nVer = 1;
	
	ClearProjectInfo();
	
	MSXML2::IXMLDOMDocumentPtr pDoc;  
	HRESULT hr =pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));  
	
	if(!SUCCEEDED(hr))  
	{  
		_tprintf( _T("[Error] Creat IXMLDOMDocumentPtr fail!"));
		//MessageBox(NULL, strErr, _T("Error"), MB_OK);
		
		return FALSE;
	}
	
	else
	{
		MSXML2::IXMLDOMElementPtr pDocElement=NULL;
		_tcscpy(m_szPrjFileName, szPrjFileName);
		_tcscpy(m_szTargetFileName, _T("") );
		
		bStatus=pDoc->load(m_szPrjFileName);
		if(FALSE==bStatus)
		{
			
			_tprintf( _T("[Error] Load file %s fail!\n"), m_szPrjFileName);			 
			return FALSE;
		}
		
		pDocElement=pDoc->GetdocumentElement();
		
		if(pDocElement)
		{
			//_bstr_t str = pDocElement->nodeName;
			MSXML2::IXMLDOMNamedNodeMapPtr pAttributeMap = NULL;
			MSXML2::IXMLDOMNode *pIXMLDOMNode = NULL;
			MSXML2::IXMLDOMNodePtr pNode = NULL;
			MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
			VARIANT varValue;               
			pDocElement->get_attributes(&pAttributeMap);
			long length = pAttributeMap->length;
			char strin[20];
			long num =0;
			for(int i=0;i<length;i++)
			{
				pAttributeMap->get_item(i,&pIXMLDOMNode);
				if(pIXMLDOMNode)
				{
					_bstr_t s = pIXMLDOMNode->nodeName;
					pIXMLDOMNode->get_nodeValue(&varValue);

					if(0==_tcscmp(s,_T("version")))
					{
						TCHAR szPrjVer[MAX_PATH] = {0};
						_tcscpy(szPrjVer,_bstr_t(varValue));
						if (0 != _tcsicmp(szPrjVer,XML_VERSION))
						{
							_tprintf(_T("Current NV tool just support %s project's version,but this project version is %s,Please upgrade nv tool.\n"),XML_VERSION,szPrjVer);
							return FALSE;
						}
					}
					if(0==_tcscmp(s,_T("name")))
					{
						_tcscpy(m_szPrjName, _bstr_t(varValue) );
					}
					if(0==_tcscmp(s,_T("alignment")))
					{
						memset(strin,0,sizeof(strin));
						strcpy(strin, _bstr_t(varValue));
						m_nAlignment=atoi(strin);
					}
					if(0==_tcscmp(s,_T("target")))
					{
						_tcscpy(szString, _bstr_t(varValue) );
						_tcscpy(szLineContent, m_szPrjFileName);
						
						TCHAR *pResult = _tcsrchr(szLineContent,'\\');
						if(pResult == NULL)
						{
							return FALSE;
						}
						
						*(pResult + 1) = 0;
						
						_tcscat(szLineContent, szString);						
						_tcscpy(m_szTargetFileName, szLineContent);
					}
					if(0==_tcscmp(s,_T("description")))
					{
						_tcscpy(m_szPrjDesc, _bstr_t(varValue) );
					}
				}                
			}
			pDocElement->get_childNodes(&pNodeList);
			num =pNodeList->length;
			if(0==num )
			{
				return bStatus;
			}
			
			for (i = 0;i < num ;i++)
			{
				pNodeList->get_item(i, &pNode);
				//for each node,set it's attribute
				if(pNode)
				{
					//_bstr_t str = pNode->nodeName;
					pNode->get_attributes(&pAttributeMap);     
					
					long length = pAttributeMap->length; 
					for(int j=0;j<length;j++)
					{
						pAttributeMap->get_item(j,&pIXMLDOMNode);
						if(pIXMLDOMNode)
						{
							_bstr_t s = pIXMLDOMNode->nodeName;
							pIXMLDOMNode->get_nodeValue(&varValue);
							if(0==_tcscmp(s,_T("name")))
							{                   						
								_tcscpy(szString, _bstr_t(varValue) );
								_tcscpy(szLineContent, m_szPrjFileName);
								
								TCHAR *pResult = _tcsrchr(szLineContent,'\\');
								if(pResult == NULL)
								{
									return FALSE;
								}
								
								*(pResult + 1) = 0;
								
								_tcscat(szLineContent, szString);
								m_szappend=_tcsrchr(szString,'.');
								if(0==_tcsicmp(m_szappend,_T(".xml")))
								{
									nVer = 3;
								}
								
								if(nVer == 1)
								{
									if(LoadModule(szLineContent))
									{
										m_nModuleCount++;
									}
									else
									{
										_tprintf( _T("[ERROR] LoadModule %s failed...\n"), szLineContent);
										return FALSE;
									}
									
								}
								else if(nVer == 2)
								{
									if(LoadModule2(szLineContent))
									{
										m_nModuleCount++;
									}
									else
									{
										_tprintf( _T("[ERROR] LoadModule %s failed...\n"), szLineContent);
										return FALSE;
									}
									
								}
								else if(nVer == 3)
								{
									if(LoadModuleXML(szLineContent))
									{
										m_nModuleCount++;
									}
									else
									{
										_tprintf( _T("[ERROR] LoadModule %s failed...\n"), szLineContent);
										return FALSE;
									}
									
								}
							}
							else
							{
								
							}
						}  
						nVer = 1;
					}
				}
			}            
		}
	}
	if (!m_strErrID.IsEmpty())
	{
		_tprintf(_T("Exist repeated IDs: %s\n"), m_strErrID);
		
		return FALSE;
	}
    // @ZHANG Ming : 2003-05-06
    if ( bStatus )
    {
        SetModified( FALSE );
		if (bCheckItemName)
		{
			CStringArray agModuleError;
			CStringArray agModuleItemError;
			
			CheckNVModule(agModuleError);
			CheckModuleItem(agModuleItemError);
			
			agWarning.Append(agModuleError);
			agWarning.Append(agModuleItemError);
		}
		
		agWarning.Append(m_agStrError);
    }
	
    pDoc.Release();	
    return bStatus;
}


BOOL CFileParser::SaveModuleXML(INT nModuleIndex)
{
    BOOL  bStatus = TRUE;
    MSXML2::IXMLDOMDocumentPtr pDoc=NULL;
    MSXML2::IXMLDOMElementPtr pDocElement=NULL;
    MSXML2::IXMLDOMElementPtr pNewChildElement=NULL;
    MSXML2::IXMLDOMElementPtr pParentElement=NULL;
    HRESULT hr =pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));  
	
    if(!SUCCEEDED(hr))  
    {  
        //Notify that open file failed
        _tprintf( _T("Creat IXMLDOMDocumentPtr fail!\n"));
        //MessageBox(NULL, strErr, _T("Error"), MB_OK);
		
        return FALSE;
    }
	
    else
    {
        char str[ATTRIBUTE_MAX]={0};
        pDocElement=pDoc->createElement("MODULE");
		pDocElement->setAttribute("version",XML_VERSION);
        pDocElement->setAttribute("name",m_CurrModule.szModuleName);
		//[add by Joly
		int nLength = _tcslen(m_CurrModule.szModuleDescription);
		if (m_CurrModule.szModuleDescription[nLength - 1] == '\r' || m_CurrModule.szModuleDescription[nLength - 1] == '\t' || m_CurrModule.szModuleDescription[nLength - 1] == '\n')
		{
			m_CurrModule.szModuleDescription[nLength - 1] = ' ';
		}
		CString strModuleDesc(m_CurrModule.szModuleDescription);
		strModuleDesc.TrimLeft();
		strModuleDesc.TrimRight();
		strModuleDesc.TrimLeft(_T("\""));
		strModuleDesc.TrimRight(_T("\""));
		//]		
        pDocElement->setAttribute("description",strModuleDesc.operator LPCTSTR());
        pDoc->appendChild(pDocElement);
        
        m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
        m_CurrModule.nItemCount  = m_currModuleItemList->GetSize();
        for(UINT Index = 0; Index < m_CurrModule.nItemCount; Index++)
        {            
            ModuleItem     m_ParentModuleItem;
            m_CurrModuleItem = m_currModuleItemList->GetAt (Index);
			pNewChildElement=pDoc->createElement("ITEM");
            pNewChildElement->setAttribute("name",m_CurrModuleItem.szItemName);
            memset(str,0,sizeof(str));
            if(ROOT_ELEMENT!=m_CurrModuleItem.nParentItemIndex)
            {
                m_ParentModuleItem = m_currModuleItemList->GetAt(m_CurrModuleItem.nParentItemIndex);
                pParentElement = m_ParentModuleItem.nXmlPtr;
            }
			else
			{
				pNewChildElement->setAttribute("id",itoa(m_CurrModuleItem.nItemID,str,DECIMAL));
			}           
			
            if( 
				ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType	||
				ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType
			 )
            {
				
				if (ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType)
				{
					pNewChildElement->setAttribute("type",szItemType[m_CurrModuleItem.eItemType]);
					ModuleItem stChildModuleItem = m_currModuleItemList->GetAt (Index+1);
					pNewChildElement->setAttribute("subtype",szItemType[stChildModuleItem.eItemType]);
				}
				else
				{
					if (
						 (ROOT_ELEMENT != m_CurrModuleItem.nParentItemIndex && ARRAY_ITEM_TYPE != m_ParentModuleItem.eItemType) ||
						 ROOT_ELEMENT == m_CurrModuleItem.nParentItemIndex
						)
					{
						pNewChildElement->setAttribute("type",szItemType[m_CurrModuleItem.eItemType]);
					}
				}
				
                memset(str,0,sizeof(str));
                pNewChildElement->setAttribute("count",itoa(m_CurrModuleItem.nArraySize,str,DECIMAL));           
			}
			else
			{
				if(
					ROOT_ELEMENT != m_CurrModuleItem.nParentItemIndex	&&							
					(
					ARRAY_ITEM_TYPE   == m_ParentModuleItem.eItemType	||
					ASTRING_ITEM_TYPE == m_ParentModuleItem.eItemType	||
					WSTRING_ITEM_TYPE == m_ParentModuleItem.eItemType	
					)				
				 )
				{
					;
				}
				else
				{
					pNewChildElement->setAttribute("type",szItemType[m_CurrModuleItem.eItemType]);
				}
			}

			if( ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
                WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				STRUCT_ITEM_TYPE  == m_CurrModuleItem.eItemType ||// by Joly
				ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType )  //  by Joly
            {
                ;
            }
            else
            {			
				FormatContent();  // add by Joly 
				pNewChildElement->setAttribute("value", m_CurrModuleItem.szItemContent );				
            }
			if (NULL != m_CurrModuleItem.szItemDescription)
			{
				CString strDesc(m_CurrModuleItem.szItemDescription);
				strDesc.TrimLeft();
				strDesc.TrimRight();
				strDesc.TrimLeft(_T("\""));
				strDesc.TrimRight(_T("\""));
				pNewChildElement->setAttribute("desc", strDesc.operator LPCTSTR());
			}
			//[add by Joly
			else
			{
				pNewChildElement->setAttribute("desc", "");
			}
			//]
            
			
            if(m_CurrModuleItem.bReadOnly /*|| theApp.m_bWriteReadOnlyZeroItem*/)//MarkA
            {
                memset(str,0,sizeof(str));
                pNewChildElement->setAttribute("readOnly",itoa(m_CurrModuleItem.bReadOnly,str,DECIMAL));
            }
#ifdef _DEBUG
			memset(str,0,sizeof(str));
			pNewChildElement->setAttribute("offset",itoa(m_CurrModuleItem.nOffset,str,DECIMAL));
#endif
            m_CurrModuleItem.nXmlPtr=pNewChildElement;
            m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
            if(ROOT_ELEMENT!=m_CurrModuleItem.nParentItemIndex)
            {                
                pParentElement->appendChild(pNewChildElement);
            }
            else
            {
                pDocElement->appendChild(pNewChildElement);
            }
            pNewChildElement.Release();
        }    
    }
	
    WellFormXml(pDoc,pDocElement,1);
    
    if(0==IsFileRdOnly(m_CurrModule.szModuleFileName))
    {
        pDoc->save(m_CurrModule.szModuleFileName); 
    }
    else
    {
        if(ChangeFileForWrite(m_CurrModule.szModuleFileName))
        {
            pDoc->save(m_CurrModule.szModuleFileName); 
            VERIFY( ChangeFileRdOnlyAttri(m_CurrModule.szModuleFileName, TRUE ) );
        }
        else
        {
            return FALSE;
        }
    }
    pDocElement.Release();
    pDoc.Release();
	
    return bStatus;
}
/*xml file support by xmzhang add end*/

BOOL CFileParser::LoadModule(TCHAR *szModuleFileName)
{
    BOOL  bStatus = TRUE;
    TCHAR  szLineContent[MAX_PATH] = {0};
    TCHAR  *pszLine;
    TCHAR  szString[MAX_PATH] = {0};
    BOOL  bHandleItem;

	m_bStaticFlag = FALSE;  // add for static nv
	
	
    if(NULL != m_hModuleFile)
    {
        //Notify that this file parser has loaded file 
        bStatus =  FALSE;
    }
    else
    {
        m_hModuleFile = _tfopen(szModuleFileName, _T("r"));
        if(NULL == m_hModuleFile)
        {
            //The given file is not exist then create it
            bStatus = FALSE;            
        }
        else
        {
            bHandleItem = FALSE;
			
            //Get Module attribute
            memset(&m_CurrModule, 0, sizeof(Module));
            _tcscpy(m_CurrModule.szModuleFileName , szModuleFileName);
            m_currModuleItemList = new ModuleItem_List;
            //m_currModuleItemList->SetTypeSize (sizeof(ModuleItem));
			
			UINT nParentID = 0;
			INT nParentIdx= -1;
			
            pszLine = _fgetts(szLineContent, MAX_PATH, m_hModuleFile);
            while(NULL != pszLine)
            {	
                //Remove the pre space
                while(' ' == *pszLine)
                {
					pszLine++;
                }
                //This line is comment
                if( ('#' != *pszLine) &&
                    ('\n' != *pszLine))
                {
                    szString[0] = '\0';					
                    _stscanf(pszLine, _T("%[^=]"), szString);				
					_stscanf(szString, _T("%s"), szString);
                    pszLine = _tcschr(pszLine, '=');
					if(pszLine != NULL) pszLine += 1;

                    if(0 == (_tcscmp(szString, _T("MODULE"))))
                    {
                        _stscanf(pszLine, _T("%s"),  m_CurrModule.szModuleName);
                    }                    
                    else if(0 == (_tcscmp(szString, _T("DESCRIPTION"))))
                    {
						// modify for Joly
						_tcscpy(m_CurrModule.szModuleDescription, pszLine);
						
                    }
                    else if(0 == (_tcscmp(szString, _T("BEGIN_ITEM"))))
                    {
                        memset(&m_CurrModuleItem, 0, sizeof(ModuleItem));
                    }
                    else if(0 == (_tcscmp(szString, _T("END_ITEM"))))
                    {
						
						// [[ add for check repeated ID
						if (-1 == m_CurrModuleItem.nParentItemIndex)
						{
							CMapID::iterator  it = m_mapIDs.find(m_CurrModuleItem.nItemID);
							if (m_mapIDs.end() != it)
							{
								CString strTmp;
								strTmp.Format(_T("%d  "), m_CurrModuleItem.nItemID);
								m_strErrID += strTmp;
							}
							else
							{
								m_mapIDs.insert(CMapID::value_type(m_CurrModuleItem.nItemID, m_CurrModuleItem.nItemLength));
							}
							
						}
						//]]

						if(m_CurrModuleItem.nParentItemIndex == 0)
						{
							nParentID = m_CurrModuleItem.nItemID;
							nParentIdx = m_CurrModuleItem.nItemIndex;
						}
						else if(m_CurrModuleItem.nParentItemIndex > 0)
						{
							if(m_CurrModuleItem.nParentItemIndex == nParentIdx &&
								m_CurrModuleItem.nItemID != nParentID)
							{
								CString str;
								str.Format(_T("[WARNING] [NVM: %s]: [ItemName: %s] [ItemID: %d] not equal to [ParentID: %d] and fixed."),GetStrFileTitle(szModuleFileName),
									m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemID,nParentID);
								//AfxMessageBox(str);
								m_agStrError.Add(str);
								m_CurrModuleItem.nItemID = nParentID;
							}
						}
						
						if( m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE ||
							m_CurrModuleItem.eItemType == SHORT_ITEM_TYPE || 
							m_CurrModuleItem.eItemType == LONG_ITEM_TYPE)
						{
							if(!ValueIsValid(m_CurrModuleItem.szItemContent))
							{
								CString str;
								str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item content [%s] is not valid."),GetStrFileTitle(szModuleFileName),
									m_CurrModuleItem.szItemName,m_CurrModuleItem.szItemContent);
								//AfxMessageBox(str);
								m_agStrError.Add(str);
								bStatus = FALSE;
								m_CurrModuleItem.bChanged = TRUE;
								m_CurrModuleItem.bExpanded = TRUE;
								
								m_CurrModule.bExpanded = TRUE;
								
								int nPIdx = m_CurrModuleItem.nParentItemIndex;
								while (nPIdx != -1)
								{
									ModuleItem mi = m_currModuleItemList->GetAt(nPIdx);
									mi.bExpanded = TRUE;
									m_currModuleItemList->SetAt(nPIdx,mi);
									nPIdx = mi.nParentItemIndex;
								}
							}
							if(m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE)
							{
								m_CurrModuleItem.nItemLength = 1;
							}
							else if(m_CurrModuleItem.eItemType == SHORT_ITEM_TYPE)
							{
								m_CurrModuleItem.nItemLength = 2;
							}
							else if(m_CurrModuleItem.eItemType == LONG_ITEM_TYPE)
							{
								m_CurrModuleItem.nItemLength = 4;
        					}
						}
						
						
						
						//_ASSERTE( m_CurrModuleItem.nParentItemIndex < (INT)m_CurrModuleItem.nItemIndex &&
						//          m_CurrModuleItem.nItemIndex == (UINT)m_currModuleItemList->GetSize());
						
						BOOL bOK = TRUE;
						
						
						if(m_CurrModuleItem.nParentItemIndex >= (INT)m_CurrModuleItem.nItemIndex)
						{
							bOK = FALSE;
							bStatus = FALSE;
							CString str;
							str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent index [%d] is not smaller than its index [%d]."),GetStrFileTitle(szModuleFileName),
								m_CurrModuleItem.szItemName,m_CurrModuleItem.nParentItemIndex,m_CurrModuleItem.nItemIndex);							
							m_agStrError.Add(str);
							
						}
						
						if(bOK && m_CurrModuleItem.nItemIndex != (UINT)m_currModuleItemList->GetSize())
						{
							CString str;
							str.Format(_T("[WARNING] [NVM: %s] [ItemName: %s]:item index [%d] is not valid and fixed it as [%d]."),GetStrFileTitle(szModuleFileName),
								m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemIndex,m_currModuleItemList->GetSize());							
							m_agStrError.Add(str);
							m_CurrModuleItem.nItemIndex = m_currModuleItemList->GetSize();
						}
						
						
						if( bOK && m_CurrModuleItem.nParentItemIndex > -1 )
						{
							_ASSERTE (m_CurrModuleItem.nParentItemIndex < m_currModuleItemList->GetSize() );
							
							ModuleItem miParent = m_currModuleItemList->GetAt(m_CurrModuleItem.nParentItemIndex);
							if( miParent.eItemType !=  ARRAY_ITEM_TYPE &&  
								miParent.eItemType !=  ASTRING_ITEM_TYPE &&
								miParent.eItemType !=  WSTRING_ITEM_TYPE &&
								miParent.eItemType !=  STRUCT_ITEM_TYPE )
							{									
								bStatus = FALSE;
								CString str;
								str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent [%s] is not valid."),GetStrFileTitle(szModuleFileName),
									m_CurrModuleItem.szItemName,miParent.szItemName);							
								m_agStrError.Add(str);
							}
							
						}

						FilterContent(szModuleFileName);
                        m_currModuleItemList->Add(m_CurrModuleItem);
                        m_CurrModule.nItemCount += 1;
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_NAME"))))
                    {
                        _stscanf(pszLine, _T("%s"),  m_CurrModuleItem.szItemName );
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_ID"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nItemID );
						if (m_CurrModuleItem.nItemID > 0xFFFF)
						{
							CString str;
							str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item ID [%s] is not valid."),GetStrFileTitle(szModuleFileName),
								m_CurrModuleItem.szItemName, pszLine);							
					    	m_agStrError.Add(str);
						}
						
		
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_TYPE"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.eItemType  );
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_SIZE"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nArraySize   );
                    }
                    /*else if(0 == (_tcscmp(szString, _T("ITEM_LENGTH"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nItemLength    );
						if (m_CurrModuleItem.nItemLength > 0xFFFF || 0 == m_CurrModuleItem.nItemLength)
						{
							CString str;
							str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item length [%s] is not valid."),GetStrFileTitle(szModuleFileName),
								m_CurrModuleItem.szItemName, pszLine);							
							m_agStrError.Add(str);
						}
                    }*/
                    else if(0 == (_tcscmp(szString, _T("ITEM_CONTENT"))))
                    {
                        _stscanf(pszLine, _T("%s"),  m_CurrModuleItem.szItemContent);
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_INDEX"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nItemIndex);
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_PARENT"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nParentItemIndex );
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_DESC"))))
                    {
						while (::isspace(*pszLine))
							++pszLine;

						CString strLine(pszLine);
						strLine.Replace(_T("\r"), _T(""));
						strLine.Replace(_T("\n"), _T(""));
						strLine.Replace(_T("\t"), _T(""));
						_tcscpy(pszLine, strLine.operator LPCTSTR());
						// erase redundant string : (null) 
						if (0 == _tcscmp(pszLine, _T("(null)")))
						{
							continue;
						}

	                    int nLen = ::_tcslen(pszLine) + 1;
						
						if (nLen > 1)
						{
							m_CurrModuleItem.szItemDescription = (TCHAR*)malloc(nLen * sizeof(TCHAR));
							_tcscpy(m_CurrModuleItem.szItemDescription, pszLine);

							if (m_CurrModuleItem.szItemContent[0] != 0)
							{
								//int nValue = GetContent(m_CurrModuleItem.szItemContent);
								if(!CheckValue(m_CurrModuleItem.szItemDescription, m_CurrModuleItem.szItemContent, m_CurrModuleItem.eItemType))
								{
									CString str;
									str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item value %s is not valid."),GetStrFileTitle(szModuleFileName),
										m_CurrModuleItem.szItemName, m_CurrModuleItem.szItemContent);							
									m_agStrError.Add(str);
								}
							}
							
						}
						
                    }
					else if(0 == (_tcscmp(szString,_T("ITEM_READONLY"))))
					{
						_stscanf(pszLine,_T("%d"),&m_CurrModuleItem.bReadOnly);
					}

                }
                
                pszLine = _fgetts(szLineContent, MAX_PATH, m_hModuleFile);
            }
			
            m_lstModule.Add (m_CurrModule);
			m_currModuleItemList->FreeExtra(); //2015.6.1
            m_lstModuleItemPtr.Add (m_currModuleItemList);
        }
    }

    
    //Close Project file
    if(NULL != m_hModuleFile)
    {
        fclose(m_hModuleFile);
        m_hModuleFile = NULL;
    }
	
    return bStatus;
	
}

void CFileParser::FilterContent(TCHAR *szModuleFileName)
{         
	DWORD  dwDstValue;						                  
	if((NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x')) ||                   
		(NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X')))                  
	{                 
		dwDstValue  = _tcstoul(m_CurrModuleItem.szItemContent, NULL, 16);       
	}                
	else                
	{                  
		dwDstValue  = _tcstoul(m_CurrModuleItem.szItemContent, NULL, 10);              
	}						
	
	
	switch (m_CurrModuleItem.eItemType)
	{
	case CHAR_ITEM_TYPE:
		if (0XFF < dwDstValue)
		{
			CString str;
			CString strItemPath;
			strItemPath.Format(_T("\\%s"),m_CurrModuleItem.szItemName);
			GetNVItemPath(m_CurrModule.szModuleName,m_CurrModuleItem.nParentItemIndex,strItemPath);
			str.Format(_T("[WARNING] [NVM: %s] [ItemName: %s]:item value %s[char] is out of bounds."),GetStrFileTitle(szModuleFileName),
				strItemPath, m_CurrModuleItem.szItemContent);
			m_agStrError.Add(str);
		}
		
		break;
	case SHORT_ITEM_TYPE:
		if (0XFFFF < dwDstValue)
		{
			CString str;
			CString strItemPath;
			strItemPath.Format(_T("\\%s"),m_CurrModuleItem.szItemName);
			GetNVItemPath(m_CurrModule.szModuleName,m_CurrModuleItem.nParentItemIndex,strItemPath);
			str.Format(_T("[WARNING] [NVM: %s] [ItemName: %s]:item value %s[short] is out of bounds."),GetStrFileTitle(szModuleFileName),
				strItemPath, m_CurrModuleItem.szItemContent);							
			m_agStrError.Add(str);
		}
		break;
	case LONG_ITEM_TYPE:
		if (0XFFFFFFFF < dwDstValue)
		{
			CString str;
			CString strItemPath;
			strItemPath.Format(_T("\\%s"),m_CurrModuleItem.szItemName);
			GetNVItemPath(m_CurrModule.szModuleName,m_CurrModuleItem.nParentItemIndex,strItemPath);
			
			str.Format(_T("[WARNING] [NVM: %s] [ItemName: %s]:item value %s[long] is out of bounds."),GetStrFileTitle(szModuleFileName),
				strItemPath, m_CurrModuleItem.szItemContent);							
			m_agStrError.Add(str);
		}
		break;
	default:
		break;
	}
}

/*xml file support by xmzhang add begin*/
BOOL CFileParser::LoadElement(MSXML2::IXMLDOMElementPtr pDocElement,UINT nParentItemIndex,TCHAR *szModuleFileName,UINT nItemID/*=0*/,ITEMTYPE eCurItemType/*=NONE_ITEM_TYPE*/)
{
    BOOL  bStatus = TRUE;
    UINT nParentIndex=nParentItemIndex;
    long num =0;
    MSXML2::IXMLDOMNodePtr pNode = NULL;
    MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
    MSXML2::IXMLDOMNamedNodeMapPtr pAttributeMap = NULL; 
    MSXML2::IXMLDOMNodePtr pChildListNode = NULL;
    UINT nParentID = 0;
    INT nParentIdx= -1;
	
    pDocElement->get_childNodes(&pNodeList);
    num =pNodeList->length;
    if(0==num )
    {
        return bStatus;
    }
	
    for (int i = 0;i < num ;i++)
    {
        pNodeList->get_item(i, &pNode);
		ITEMTYPE eSubItemType=eCurItemType;
        //for each node,set it's attribute
        if(pNode)
        {
            //_bstr_t str = pNode->nodeName;
            MSXML2::IXMLDOMNode *pIXMLDOMNode = NULL;
            VARIANT varValue;
            memset(&m_CurrModuleItem, 0, sizeof(ModuleItem));
            pNode->get_attributes(&pAttributeMap);     
            
			
            long length = pAttributeMap->length; 
            char strin[ATTRIBUTE_MAX];
            TCHAR     typeName[ATTRIBUTE_MAX]={0};
			if ((int)nParentItemIndex != ROOT_ELEMENT)
			{
				m_CurrModuleItem.nItemID=nItemID;
			}
			m_CurrModuleItem.eItemType = eCurItemType;
			if (ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				eSubItemType = CHAR_ITEM_TYPE;
			}
			else if (WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				eSubItemType = SHORT_ITEM_TYPE;
			}
            for(int j=0;j<length;j++)
            {
                pAttributeMap->get_item(j,&pIXMLDOMNode);
                if(pIXMLDOMNode)
                {
                    _bstr_t s = pIXMLDOMNode->nodeName;
                    int k = 0;
                    pIXMLDOMNode->get_nodeValue(&varValue);
                    if(0==_tcscmp(s,_T("name")))
                    {
                        _tcscpy(m_CurrModuleItem.szItemName, _bstr_t(varValue) );
                    }                            
                    if(0==_tcscmp(s,_T("id")))
                    {
                        memset(strin,0,sizeof(strin));
                        strcpy(strin, _bstr_t(varValue));
                        m_CurrModuleItem.nItemID=atoi(strin);
                    }
					
                    if(0==_tcscmp(s,_T("type")))
                    {
                        _tcscpy(typeName, _bstr_t(varValue));
                        for(k=0;k<9;k++)
                        {
                            if(k!=3)
                            {
                                if(0==_tcscmp(typeName,szItemType[k]))
                                {
                                    m_CurrModuleItem.eItemType=(ITEMTYPE)k;
									break;
                                }
                            }
                        }
						if (ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
						{
							eSubItemType = CHAR_ITEM_TYPE;
						}
						else if (WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
						{
							eSubItemType = SHORT_ITEM_TYPE;
						}

                    }

					if(0==_tcscmp(s,_T("subtype")))
                    {
                        _tcscpy(typeName, _bstr_t(varValue));
                        for(k=0;k<9;k++)
                        {
                            if(k!=3)
                            {
                                if(0==_tcscmp(typeName,szItemType[k]))
                                {
                                    eSubItemType=(ITEMTYPE)k;
									break;
                                }
                            }
                        }
                    }

					
					
                    if(0==_tcscmp(s,_T("count")))
                    {
                        memset(strin,0,sizeof(strin));
                        strcpy(strin, _bstr_t(varValue));
                        m_CurrModuleItem.nArraySize=atoi(strin);
                    }
					
                    if(0==_tcscmp(s,_T("length")))
                    {
						/*#ifdef _DEBUG*/
                        memset(strin,0,sizeof(strin));
                        strcpy(strin, _bstr_t(varValue));
                        m_CurrModuleItem.nItemLength=atoi(strin);
						/*#endif*/
                    }
					
                    if(0==_tcscmp(s,_T("offset")))
                    {
						/*#ifdef _DEBUG*/
                        memset(strin,0,sizeof(strin));
                        strcpy(strin, _bstr_t(varValue));
                        m_CurrModuleItem.nOffset=atoi(strin);
						/*#endif*/
                    }
					
                    if(0==_tcscmp(s,_T("value")))
                    {
                        _tcscpy(m_CurrModuleItem.szItemContent, _bstr_t(varValue) );
                    }
					
                    if(0==_tcscmp(s,_T("desc")))
                    {
						int nLen = ::_tcslen(_bstr_t(varValue)) + 1;
						
						if (nLen > 1)
						{
							m_CurrModuleItem.szItemDescription = (TCHAR*)malloc(nLen * sizeof(TCHAR));
							_tcscpy(m_CurrModuleItem.szItemDescription, _bstr_t(varValue));
							
							if (m_CurrModuleItem.szItemContent[0] != 0)
							{
								// add for MS check ItemValue
								if(!CheckValue(m_CurrModuleItem.szItemDescription, m_CurrModuleItem.szItemContent, m_CurrModuleItem.eItemType))
								{
									CString str;
									str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item value %s is not valid."),GetStrFileTitle(szModuleFileName),
										m_CurrModuleItem.szItemName, m_CurrModuleItem.szItemContent);							
									m_agStrError.Add(str);
								}
							}
						}
                    }
					
                    if(0==_tcscmp(s,_T("readOnly")))
                    {
                        memset(strin,0,sizeof(strin));
                        strcpy(strin, _bstr_t(varValue));
                        m_CurrModuleItem.bReadOnly=atoi(strin);
                    }
                }
            }
			// [[ add for check repeated ID
			if ((int)nParentItemIndex == ROOT_ELEMENT)
			{
				CMapID::iterator  it = m_mapIDs.find(m_CurrModuleItem.nItemID);
				if (m_mapIDs.end() != it)
				{
					CString strTmp;
					strTmp.Format(_T("%d  "), m_CurrModuleItem.nItemID);
					m_strErrID += strTmp;
				}
				else
				{
					m_mapIDs.insert(CMapID::value_type(m_CurrModuleItem.nItemID, m_CurrModuleItem.nItemLength));
				}
				
			}
			//]]
            //set nodeindex&ParentIndex
            m_CurrModuleItem.nItemIndex=m_CurrModule.nItemCount;
            m_CurrModuleItem.nParentItemIndex=nParentIndex;
			
            //m_CurrModuleItem check.same as nvm file
            if(m_CurrModuleItem.nParentItemIndex == 0)
            {
                nParentID = m_CurrModuleItem.nItemID;
                nParentIdx = m_CurrModuleItem.nItemIndex;
            }
            else if(m_CurrModuleItem.nParentItemIndex > 0)
            {
                if(m_CurrModuleItem.nParentItemIndex == nParentIdx &&
                    m_CurrModuleItem.nItemID != nParentID)
                {
                    CString str;
                    str.Format(_T("[WARNING] [NVM: %s]: [ItemName: %s] [ItemID: %d] not equal to [ParentID: %d] and fixed."),GetStrFileTitle(szModuleFileName),
                        m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemID,nParentID);
                    m_agStrError.Add(str);
                    m_CurrModuleItem.nItemID = nParentID;
                }
            }
			
            if( m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE ||
                m_CurrModuleItem.eItemType == SHORT_ITEM_TYPE || 
                m_CurrModuleItem.eItemType == LONG_ITEM_TYPE)
            {
				if(m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE)
				{
					m_CurrModuleItem.nItemLength = 1;
				}
				else if(m_CurrModuleItem.eItemType == SHORT_ITEM_TYPE)
				{
					m_CurrModuleItem.nItemLength = 2;
				}
				else if(m_CurrModuleItem.eItemType == LONG_ITEM_TYPE)
				{
					m_CurrModuleItem.nItemLength = 4;
        		}
                if(!ValueIsValid(m_CurrModuleItem.szItemContent))
                {
                    CString str;
                    str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item content [%s] is not valid."),GetStrFileTitle(szModuleFileName),
                        m_CurrModuleItem.szItemName,m_CurrModuleItem.szItemContent);
                    //AfxMessageBox(str);
                    m_agStrError.Add(str);
                    bStatus = FALSE;
                    m_CurrModuleItem.bChanged = TRUE;
                    m_CurrModuleItem.bExpanded = TRUE;
					
                    m_CurrModule.bExpanded = TRUE;
					
                    int nPIdx = m_CurrModuleItem.nParentItemIndex;
                    while (nPIdx != -1)
                    {
                        ModuleItem mi = m_currModuleItemList->GetAt(nPIdx);
                        mi.bExpanded = TRUE;
                        m_currModuleItemList->SetAt(nPIdx,mi);
                        nPIdx = mi.nParentItemIndex;
                    }
                }
            }
			
												
            //_ASSERTE( m_CurrModuleItem.nParentItemIndex < (INT)m_CurrModuleItem.nItemIndex &&
            //          m_CurrModuleItem.nItemIndex == (UINT)m_currModuleItemList->GetSize());
			
            BOOL bOK = TRUE;
												
            if(m_CurrModuleItem.nParentItemIndex >= (INT)m_CurrModuleItem.nItemIndex)
            {
                bOK = FALSE;
                bStatus = FALSE;
                CString str;
                str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent index [%d] is not smaller than its index [%d]."),GetStrFileTitle(szModuleFileName),
                    m_CurrModuleItem.szItemName,m_CurrModuleItem.nParentItemIndex,m_CurrModuleItem.nItemIndex);							
                m_agStrError.Add(str);
							
            }
			
            if(bOK && m_CurrModuleItem.nItemIndex != (UINT)m_currModuleItemList->GetSize())
            {
                CString str;
                str.Format(_T("[WARNING] [NVM: %s] [ItemName: %s]:item index [%d] is not valid and fixed it as [%d]."),GetStrFileTitle(szModuleFileName),
                    m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemIndex,m_currModuleItemList->GetSize());							
                m_agStrError.Add(str);
                m_CurrModuleItem.nItemIndex = m_currModuleItemList->GetSize();
            }
												
            if( bOK && m_CurrModuleItem.nParentItemIndex > -1 )
            {
                _ASSERTE (m_CurrModuleItem.nParentItemIndex < m_currModuleItemList->GetSize() );
				
                ModuleItem miParent = m_currModuleItemList->GetAt(m_CurrModuleItem.nParentItemIndex);
                if( miParent.eItemType !=  ARRAY_ITEM_TYPE &&  
                    miParent.eItemType !=  ASTRING_ITEM_TYPE &&
                    miParent.eItemType !=  WSTRING_ITEM_TYPE &&
                    miParent.eItemType !=  STRUCT_ITEM_TYPE )
                {									
                    bStatus = FALSE;
                    CString str;
                    str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent [%s] is not valid."),GetStrFileTitle(szModuleFileName),
                        m_CurrModuleItem.szItemName,miParent.szItemName);							
                    m_agStrError.Add(str);
                }							
            }
            
			FilterContent(szModuleFileName);
            m_currModuleItemList->Add(m_CurrModuleItem);//add current node
            m_CurrModule.nItemCount += 1;//nodecout++
        }
        //load child node,send this nodeindex as childnode's parentindex
        LoadElement(pNode,m_CurrModuleItem.nItemIndex,szModuleFileName,m_CurrModuleItem.nItemID,eSubItemType);
    } 
	
    return bStatus;
}

BOOL CFileParser::LoadModuleXML(TCHAR *szModuleFileName)
{
    BOOL  bStatus = TRUE;
    MSXML2::IXMLDOMDocumentPtr pDoc=NULL;  
    HRESULT hr =pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));  
	
    if(!SUCCEEDED(hr))  
    {  
        _tprintf( _T("[ERROR] Creat IXMLDOMDocumentPtr fail!\n"));
        return FALSE;
    }
	
    else
    {
        BOOL result = TRUE; 
        memset(&m_CurrModule, 0, sizeof(Module));
        _tcscpy(m_CurrModule.szModuleFileName , szModuleFileName);
        m_currModuleItemList = new ModuleItem_List;
        MSXML2::IXMLDOMElementPtr pDocElement=NULL;
		
        result=pDoc->load(szModuleFileName);        
        if(FALSE==result)
        {   
            _tprintf( _T("[ERROR] Load file %s fail!\n"), szModuleFileName);
            return FALSE;
        }
		
        pDocElement=pDoc->GetdocumentElement();
        if(pDocElement)
        {
            _bstr_t str = pDocElement->nodeName;
            MSXML2::IXMLDOMNamedNodeMapPtr pAttributeMap = NULL;
            MSXML2::IXMLDOMNode *pIXMLDOMNode = NULL;
            VARIANT varValue;               
            pDocElement->get_attributes(&pAttributeMap);
            long length = pAttributeMap->length;
            for(int i=0;i<length;i++)
			{
                pAttributeMap->get_item(i,&pIXMLDOMNode);
                if(pIXMLDOMNode)
                {
                    _bstr_t s = pIXMLDOMNode->nodeName;
                    pIXMLDOMNode->get_nodeValue(&varValue);

					if(0==_tcscmp(s,_T("version")))
                    {
						TCHAR szMduVer[MAX_PATH] = {0};
						_tcscpy(szMduVer,_bstr_t(varValue));
						if (0 != _tcsicmp(szMduVer,XML_VERSION))
						{
							_tprintf(_T("Current NV tool just support %s module's version,But this module [%s] version is %s,Please upgrade nv tool.\n"),
								XML_VERSION,szModuleFileName,szMduVer);
							return FALSE;
						}
                    }
                    if(0==_tcscmp(s,_T("name")))
                    {
                        _tcscpy(m_CurrModule.szModuleName, _bstr_t(varValue) );
                    }
                    if(0==_tcscmp(s,_T("description")))
                    {
                        _tcscpy(m_CurrModule.szModuleDescription, _bstr_t(varValue) );
                    }           
                }                
            }
            bStatus=LoadElement(pDocElement,(UINT)ROOT_ELEMENT,szModuleFileName);
        }
    }
	
    m_lstModule.Add (m_CurrModule);
    m_lstModuleItemPtr.Add (m_currModuleItemList);
	
    pDoc.Release();
	
    return bStatus;
	
}
/*xml file support by xmzhang add end*/

Module & CFileParser::GetModule(INT nModuleIndex)
{
    m_CurrModule = m_lstModule.GetAt(nModuleIndex);
	
    return m_CurrModule;
}

void CFileParser::SetModule(int nModuleIndex, const Module NewModule)
{    
    Module tmpModule = m_lstModule.GetAt(nModuleIndex);
    memcpy(&tmpModule, &NewModule, sizeof(Module));
    m_lstModule.SetAt (nModuleIndex, tmpModule);
}

INT CFileParser::GetModuleCount()
{
    return m_lstModule.GetSize();
}

ModuleItem & CFileParser::GetModuleItem(INT nModuleIndex, INT nItemIndex )
{
	//delete by Joly 2015.5.29
	// 	ModuleItem tmpModuleItem;
	// 	memcpy(&m_CurrModuleItem,&tmpModuleItem,sizeof(tmpModuleItem));
	
	if(nModuleIndex>=0 && nModuleIndex< m_lstModuleItemPtr.GetSize())
	{
		m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
		if(nItemIndex>=0 && nItemIndex < m_currModuleItemList->GetSize())
		{
			m_CurrModuleItem = m_currModuleItemList->GetAt(nItemIndex);
		}
	}
	
    return m_CurrModuleItem;
}

void CFileParser::SetModuleItem( int                nModuleIndex, 
								int                nItemIndex, 
								const ModuleItem & NewModuleItem,
								BOOL               fSetModified )
{
    m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);    
    ModuleItem tmpItem = m_currModuleItemList->GetAt (nItemIndex);
	
    memcpy(&tmpItem, &NewModuleItem, sizeof(ModuleItem));	
    m_currModuleItemList->SetAt (nItemIndex, tmpItem);
	
    // @ZHANG Ming : 2003-05-06
    if ( TRUE == fSetModified )
    {
        SetModified( TRUE );
    }
}

INT CFileParser::GetModuleItemCount(INT nModuleIndex)
{
	m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
	m_nModuleItemCount = m_currModuleItemList->GetSize();
	
	return m_nModuleItemCount;
}

INT CFileParser::CreateModule(const TCHAR *szModuleName, const TCHAR *szModuleFileName, const TCHAR *szModuleDesc)
{  
    Module  tmpModule;
    
    tmpModule.nItemCount = 0;
    _tcscpy(tmpModule.szModuleName, szModuleName);
    _tcscpy(tmpModule.szModuleFileName, szModuleFileName);
    _tcscpy(tmpModule.szModuleDescription , szModuleDesc);
    m_lstModule.Add(tmpModule);
    
    m_currModuleItemList = new ModuleItem_List;
    //m_currModuleItemList->SetTypeSize (sizeof(ModuleItem));
    m_lstModuleItemPtr.Add(m_currModuleItemList);
    m_currModuleItemList = NULL;
    
    m_nModuleCount++;
	
    // @ZHANG Ming : 2003-05-06
    SetModified( TRUE );
	
    return m_nModuleCount;
}



void CFileParser::RemoveModule(INT nModuleIndex)
{
    ASSERT(0 <= nModuleIndex);
    ASSERT(nModuleIndex < m_lstModule.GetSize());
	
    m_lstModule.RemoveAt(nModuleIndex);
    m_currModuleItemList = m_lstModuleItemPtr.GetAt(nModuleIndex);
	for (INT Index2 = 0; Index2 < m_currModuleItemList->GetSize(); Index2++)
	{
		ModuleItem  tmpItem = m_currModuleItemList->GetAt(Index2);
		if (NULL != tmpItem.szItemDescription)
		{
			free(tmpItem.szItemDescription);
			tmpItem.szItemDescription = NULL;
		}
		
	}
    m_currModuleItemList->RemoveAll();
    delete m_currModuleItemList;
    m_currModuleItemList = NULL;
	
    //m_nModuleCount++;
    m_nModuleCount--; // @ZHANG Ming : 2003-05-06
    
    // @ZHANG Ming : 2003-05-06
    SetModified( TRUE );
}

int CFileParser::InsertModuleItemAllByOnce(INT nModuleIndex, ModuleItem InsertModuleItem,INT nInsertAfter,BOOL bInsertAfter)
{
    ASSERT((UINT)nModuleIndex < m_nModuleCount);
    
    INT  bReturnCode = INVALID_MODULE_ID;
    
    m_currModuleItemList = m_lstModuleItemPtr.GetAt(nModuleIndex);
    if(INVALID_MODULE_ITEM_INDEX != nInsertAfter)
    {
        if(TRUE == bInsertAfter)
        {
            InsertModuleItem.nItemIndex = nInsertAfter + 1;
            nInsertAfter += 1;
        }
        else
        {
			InsertModuleItem.nItemIndex = nInsertAfter;
        }
    }
    else
    {
        InsertModuleItem.nItemIndex = m_currModuleItemList->GetSize ();
    }
	
    if(INVALID_MODULE_ID == InsertModuleItem.nParentItemIndex )
    {
        //thie item is one single item
        if(STRUCT_ITEM_TYPE == InsertModuleItem.eItemType )
        {
            InsertModuleItem.nOffset = 1;
        }
        else
        {
            InsertModuleItem.nOffset = 0;
        }
        
        if((INVALID_MODULE_ITEM_INDEX == nInsertAfter) ||
            ( m_currModuleItemList->GetSize () == nInsertAfter))
        {
            bReturnCode = m_currModuleItemList->Add(InsertModuleItem);
        }
        else
        {
            m_currModuleItemList->InsertAt(nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }
        
    }
    else
    {
        ASSERT(InsertModuleItem.nParentItemIndex < m_currModuleItemList->GetSize ());
        if(!(InsertModuleItem.nParentItemIndex < m_currModuleItemList->GetSize ()))
        {
            //Notify that the insert failed
            return bReturnCode;
        }
        
        ModuleItem tmpParentItem = m_currModuleItemList->GetAt (InsertModuleItem.nParentItemIndex);
        switch (InsertModuleItem.eItemType )
        {        
        case SHORT_ITEM_TYPE:
            if(tmpParentItem.nOffset < sizeof(SHORT))
            {
                tmpParentItem.nOffset = sizeof(SHORT);
            }
            break;
        case LONG_ITEM_TYPE:
            if(tmpParentItem.nOffset < sizeof(LONG))
            {
                tmpParentItem.nOffset = sizeof(LONG);
            }
            break;
		default:
			break;
        } 
        m_currModuleItemList->SetAt (InsertModuleItem.nParentItemIndex, tmpParentItem);
        while(INVALID_MODULE_ID != tmpParentItem.nParentItemIndex )
        {
            ModuleItem tmpItem = m_currModuleItemList->GetAt (tmpParentItem.nParentItemIndex);
            if(tmpItem.nOffset < tmpParentItem.nOffset )
            {
                tmpItem.nOffset = tmpParentItem.nOffset ;
                m_currModuleItemList->SetAt (tmpParentItem.nParentItemIndex, tmpItem);
                tmpParentItem = tmpItem;
            }
            else
            {               
                break;
            }
        }
        
        if(INVALID_MODULE_ITEM_INDEX == nInsertAfter)
        {
            bReturnCode = m_currModuleItemList->Add(InsertModuleItem);
        }
        else
        {
            m_currModuleItemList->InsertAt(nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }
    }
    
    if(INVALID_MODULE_ITEM_INDEX != nInsertAfter)
    {
        ModuleItem tmpItem;
		
        for(int Index = nInsertAfter + 1; Index < m_currModuleItemList->GetSize (); Index++)
        {
            tmpItem = m_currModuleItemList->GetAt (Index);            
            
            if(InsertModuleItem.nParentItemIndex < tmpItem.nParentItemIndex)
            {
				tmpItem.nParentItemIndex += 1;
            }
            
            tmpItem.nItemIndex += 1;
			
            m_currModuleItemList->SetAt (Index, tmpItem);
        }
    }
	
    // @ZHANG Ming : 2003-05-06 
    // [[
    if ( INVALID_MODULE_ID != bReturnCode )
    {
        // Add new module item successfully
        SetModified( TRUE );
    }
    // ]]
    return bReturnCode;
}

void CFileParser::InsertModuleItemAllByOnceHelper(int nIndexStart, int nParentItemIndex, UINT insertItemSize)
{
    int nInsertAfter = nIndexStart;
    if(INVALID_MODULE_ITEM_INDEX != nInsertAfter)
    {
        ModuleItem tmpItem;
        
        for(int Index = nInsertAfter + 1; Index < m_currModuleItemList->GetSize(); Index++)
        {
            ModuleItem tmpItem = m_currModuleItemList->GetAt (Index);          
            
            if(nParentItemIndex < tmpItem.nParentItemIndex)
            {
                tmpItem.nParentItemIndex += insertItemSize;
            }
            
            tmpItem.nItemIndex += insertItemSize;
            
            m_currModuleItemList->SetAt (Index, tmpItem);
        }
    }
	
}

int CFileParser::InsertModuleItem(INT nModuleIndex, ModuleItem InsertModuleItem,INT nInsertAfter,BOOL bInsertAfter) //lint -e578 
{
	ASSERT((UINT)nModuleIndex < m_nModuleCount);
    
    INT  bReturnCode = INVALID_MODULE_ID;
	
	//[[ repeated ID check
	if (-1 == InsertModuleItem.nParentItemIndex)
	{
		CMapID::iterator  it = m_mapIDs.find(InsertModuleItem.nItemID);
		if (m_mapIDs.end() != it)
		{
			_tprintf(_T("[ERROR] ItemID %d has existed, please input a unexisted ID."), InsertModuleItem.nItemID);
			
			return bReturnCode;
		}
		else
		{
			m_mapIDs.insert(CMapID::value_type(InsertModuleItem.nItemID, InsertModuleItem.nItemLength));
		}
		
	}
	//]]
    // check value if valid
	if (InsertModuleItem.szItemContent[0] != 0)
	{
		if(!CheckValue(InsertModuleItem.szItemDescription, InsertModuleItem.szItemContent, InsertModuleItem.eItemType))
		{
			_tprintf(_T("[ERROR][ItemName: %s]:item value %s is not valid."),
				InsertModuleItem.szItemName, InsertModuleItem.szItemContent);							
			
			return bReturnCode;
		}
	}
	
    m_currModuleItemList = m_lstModuleItemPtr.GetAt(nModuleIndex);
    if(INVALID_MODULE_ITEM_INDEX != nInsertAfter)
    {
        if(TRUE == bInsertAfter)
        {
            InsertModuleItem.nItemIndex = nInsertAfter + 1;
            nInsertAfter += 1;
        }
        else
        {
			InsertModuleItem.nItemIndex = nInsertAfter;
        }
    }
    else
    {
        InsertModuleItem.nItemIndex = m_currModuleItemList->GetSize ();
    }
	
    if(INVALID_MODULE_ID == InsertModuleItem.nParentItemIndex )
    {
        //thie item is one single item
        if(STRUCT_ITEM_TYPE == InsertModuleItem.eItemType )
        {
            InsertModuleItem.nOffset = 1;
        }
        else
        {
            InsertModuleItem.nOffset = 0;
        }
        
        if((INVALID_MODULE_ITEM_INDEX == nInsertAfter) ||
            ( m_currModuleItemList->GetSize () == nInsertAfter))
        {
            bReturnCode = m_currModuleItemList->Add(InsertModuleItem);
        }
        else
        {
            m_currModuleItemList->InsertAt(nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }
        
    }
    else
    {
        ASSERT(InsertModuleItem.nParentItemIndex < m_currModuleItemList->GetSize ());
        if(!(InsertModuleItem.nParentItemIndex < m_currModuleItemList->GetSize ()))
        {
            //Notify that the insert failed
            return bReturnCode;
        }
        
        ModuleItem tmpParentItem = m_currModuleItemList->GetAt (InsertModuleItem.nParentItemIndex);
        switch (InsertModuleItem.eItemType )
        {        
        case SHORT_ITEM_TYPE:
            if(tmpParentItem.nOffset < sizeof(SHORT))
            {
                tmpParentItem.nOffset = sizeof(SHORT);
            }
            break;
        case LONG_ITEM_TYPE:
            if(tmpParentItem.nOffset < sizeof(LONG))
            {
                tmpParentItem.nOffset = sizeof(LONG);
            }
            break;
		default:
			break;
        } 
        m_currModuleItemList->SetAt (InsertModuleItem.nParentItemIndex, tmpParentItem);
        while(INVALID_MODULE_ID != tmpParentItem.nParentItemIndex )
        {
            ModuleItem tmpItem = m_currModuleItemList->GetAt (tmpParentItem.nParentItemIndex);
            if(tmpItem.nOffset < tmpParentItem.nOffset )
            {
                tmpItem.nOffset = tmpParentItem.nOffset ;
                m_currModuleItemList->SetAt (tmpParentItem.nParentItemIndex, tmpItem);
                tmpParentItem = tmpItem;
            }
            else
            {               
                break;
            }
        }
        
        if(INVALID_MODULE_ITEM_INDEX == nInsertAfter)
        {
            bReturnCode = m_currModuleItemList->Add(InsertModuleItem);
        }
        else
        {
            m_currModuleItemList->InsertAt(nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }
    }
    
    if(INVALID_MODULE_ITEM_INDEX != nInsertAfter)
    {
        ModuleItem tmpItem;
		
        for(int Index = nInsertAfter + 1; Index < m_currModuleItemList->GetSize (); Index++)
        {
            tmpItem = m_currModuleItemList->GetAt (Index);            
            
            if(InsertModuleItem.nParentItemIndex < tmpItem.nParentItemIndex)
            {
				tmpItem.nParentItemIndex += 1;
            }
            
            tmpItem.nItemIndex += 1;
			
            m_currModuleItemList->SetAt (Index, tmpItem);
        }
    }
	
    // @ZHANG Ming : 2003-05-06 
    // [[
    if ( INVALID_MODULE_ID != bReturnCode )
    {
        // Add new module item successfully
        SetModified( TRUE );
    }
    // ]]
    return bReturnCode;
}

void CFileParser::RemoveModuleItem(INT nModuleIndex, INT nItemIndex)
{
	ModuleItem miItem = GetModuleItem( nModuleIndex, nItemIndex );
	if ( INVALID_MODULE_ID != miItem.nParentItemIndex )
	{
		ModuleItem miParent = GetModuleItem( nModuleIndex, miItem.nParentItemIndex );
		if ( ARRAY_ITEM_TYPE == miParent.eItemType ||
			ASTRING_ITEM_TYPE == miParent.eItemType ||
			WSTRING_ITEM_TYPE == miParent.eItemType)
		{
			ASSERT( miParent.nArraySize > 0 );
			--miParent.nArraySize;
			SetModuleItem( nModuleIndex, miParent.nItemIndex, miParent );
		}	
	}
	
    if(nItemIndex == (m_currModuleItemList->GetSize () - 1))
    {
        //The removed item is the last item
        m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
		
		ModuleItem tmpModleItem = m_currModuleItemList->GetAt(nItemIndex);
		if (NULL != tmpModleItem.szItemDescription)
		{
			free(tmpModleItem.szItemDescription);
			tmpModleItem.szItemDescription = NULL;
		}
		
        m_currModuleItemList->RemoveAt(nItemIndex);
    }
    else
    {
        ModuleItem tmpItem;
        INT        nRemovedItemParentIndex;
		
        //The removed item is not the last item
        m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
        tmpItem = m_currModuleItemList->GetAt(nItemIndex);
        nRemovedItemParentIndex = tmpItem.nParentItemIndex ;
		
        for(int Index = nItemIndex + 1; Index < m_currModuleItemList->GetSize (); Index++)
        {
            tmpItem = m_currModuleItemList->GetAt (Index);
            
            if(nRemovedItemParentIndex < tmpItem.nParentItemIndex)
            {
				tmpItem.nParentItemIndex -= 1;
            }
            
            tmpItem.nItemIndex -= 1;
			
            m_currModuleItemList->SetAt (Index, tmpItem);
        }
		
		ModuleItem tmpModleItem2 = m_currModuleItemList->GetAt(nItemIndex);
		if (NULL != tmpModleItem2.szItemDescription)
		{
			free(tmpModleItem2.szItemDescription);
			tmpModleItem2.szItemDescription = NULL;
		}
		
        m_currModuleItemList->RemoveAt(nItemIndex);
		
        
    }    
    // @ZHANG Ming : 2003-05-06
    SetModified( TRUE );
}

void CFileParser::RemoveAllNVltem(INT nModuleIndex)
{
    m_currModuleItemList = m_lstModuleItemPtr.GetAt (nModuleIndex);
    m_currModuleItemList->RemoveAll();
}


BOOL CFileParser::LoadFromImage(const TCHAR *szImageFileName)
{
    FILE *fpImage = NULL;
    LPBYTE lpNVContent = NULL;
    lpNVContent = new BYTE[MAX_TOTAL_ITEM_CONTENT];
    if(NULL == lpNVContent)
    {
    	return FALSE;
    }
    UINT nTotalImageLength = 0;   
    UINT nNVImageLength = 0;   
    ModuleItem tmpParentItem;
    
    if( (NULL == szImageFileName) ||
        (0 == _tcscmp(m_szTargetFileName, _T(""))) )
    { 
		//Notify that has not given target file
	SAFE_DELETE_ARRAY(lpNVContent);
        return FALSE;
    }
	
    fpImage = _tfopen(szImageFileName, _T("rb"));
    if(NULL == fpImage)
    {
		//Notify that can not open target file
	SAFE_DELETE_ARRAY(lpNVContent);
        return FALSE;
    }
	
    ZeroMemory(&tmpParentItem, sizeof(ModuleItem));
    ZeroMemory(lpNVContent, MAX_ITEM_CONTENT);
	
    nTotalImageLength = fread(lpNVContent, 1, MAX_TOTAL_ITEM_CONTENT, fpImage);
    
    //Remove the image file generating timing
    nNVImageLength += 4;
	//    nTotalImageLength -= 4;
	
	if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
    
    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {
        m_currModuleItemList = m_lstModuleItemPtr.GetAt (nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < GetModuleItemCount(nCurModuleIndex); nCurItemIndex++)
        {
            m_CurrModuleItem = m_currModuleItemList->GetAt (nCurItemIndex);
            if(INVALID_MODULE_ID != m_CurrModuleItem.nParentItemIndex )
            {
                switch(m_CurrModuleItem.eItemType)
                {
                case CHAR_ITEM_TYPE:                
                    {
                        if((m_CurrModuleItem.nOffset + nNVImageLength + 4 +1) > nTotalImageLength)
                        {
                            SAFE_DELETE_ARRAY(lpNVContent);
                            return FALSE;
                        }
						
                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),lpNVContent[m_CurrModuleItem.nOffset + nNVImageLength + 4 ]);
                    }
                    break;
                case SHORT_ITEM_TYPE:
                    {           
                        if((m_CurrModuleItem.nOffset + nNVImageLength + 4 + 2) > nTotalImageLength)
                        {
                            SAFE_DELETE_ARRAY(lpNVContent);
                            return FALSE;
                        }
						
                        WORD  tmpSrcValue = *(WORD *)&(lpNVContent[m_CurrModuleItem.nOffset + nNVImageLength + 4]);
                        WORD  tmpDstValue;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        
                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                        
                    }
                    break;
                case LONG_ITEM_TYPE:
                    {
                        if((m_CurrModuleItem.nOffset + nNVImageLength + 4 + 4) > nTotalImageLength)
                        {
                            SAFE_DELETE_ARRAY(lpNVContent);
                            return FALSE;
                        }
						
                        DWORD  tmpSrcValue = *(DWORD *)&(lpNVContent[m_CurrModuleItem.nOffset + nNVImageLength + 4]);
                        DWORD  tmpDstValue;
                        CONVERT_INT2(tmpSrcValue, tmpDstValue);
                        
                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                    }
                    break;  
                case ARRAY_ITEM_TYPE:
                    break;
				default:
					break;
                }
				
                m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
            }
            else
            {
                if(0 < tmpParentItem.nItemLength )
                {
                    nNVImageLength += tmpParentItem.nItemLength + 4;
                    nNVImageLength = ((nNVImageLength + 3) & 0xFFFFFFFC);
					
                    if(nNVImageLength >= nTotalImageLength)
                    {
                    	SAFE_DELETE_ARRAY(lpNVContent);
                        return FALSE;
                    }
                }
				
                tmpParentItem = m_CurrModuleItem;
                
                switch(m_CurrModuleItem.eItemType)
                {
                case CHAR_ITEM_TYPE:                
                    {
                        if((nNVImageLength + 4 + 1) > nTotalImageLength)
                        {
                            SAFE_DELETE_ARRAY(lpNVContent);
                            return FALSE;
                        }
						
                        WORD tmpSrcValue, tmpDstValue ;
                        tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength]);
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemID = tmpDstValue;
						
                        tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength + 2]);
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemLength = tmpDstValue;
						
                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),lpNVContent[ nNVImageLength + 4 ] );
						
                        m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
						
                    }
                    break;
                case SHORT_ITEM_TYPE:
                    {
                        if((nNVImageLength + 4 + 2) > nTotalImageLength)
                        {
                            SAFE_DELETE_ARRAY(lpNVContent);
                            return FALSE;
                        }
						
                        WORD tmpSrcValue, tmpDstValue ;
                        tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength]);
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemID = tmpDstValue;
						
                        tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength + 2]);
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemLength = tmpDstValue;
						
                        
                        tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength + 4]);
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        
                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                        m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
                    }
                    break;
                case LONG_ITEM_TYPE:
                    {
                        if((nNVImageLength + 4 + 4) > nTotalImageLength)
                        {
                            SAFE_DELETE_ARRAY(lpNVContent);
                            return FALSE;
                        }
						
                        
                        {
                            WORD tmpSrcValue, tmpDstValue ;
							
                            tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength]);
                            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                            m_CurrModuleItem.nItemID = tmpDstValue;
                            
                            tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength + 2]);
                            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                            m_CurrModuleItem.nItemLength = tmpDstValue;
                            
                        }
						
                        DWORD  tmpSrcValue;
                        DWORD tmpDstValue ;
						
                        tmpSrcValue = *(DWORD *)&(lpNVContent[nNVImageLength + 4]);                        
                        CONVERT_INT2(tmpSrcValue, tmpDstValue);
                        
                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                        m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
                    }
                    break;  
                case ARRAY_ITEM_TYPE:
					if((nNVImageLength + 4 ) > nTotalImageLength)
					{
						SAFE_DELETE_ARRAY(lpNVContent);
						return FALSE;
					}
					
					{
						WORD tmpSrcValue, tmpDstValue ;
						tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength]);
						CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
						m_CurrModuleItem.nItemID = tmpDstValue;
						
						tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength + 2]);
						CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
						m_CurrModuleItem.nItemLength = tmpDstValue;
					}
					
					/*m_CurrModuleItem.nItemID = *(WORD *)&(lpNVContent[ nNVImageLength];)
					m_CurrModuleItem.nItemLength = *(WORD *)&(lpNVContent[ nNVImageLength + 2];)*/
					m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
                    break;
				case ASTRING_ITEM_TYPE:
				case WSTRING_ITEM_TYPE:
					if((nNVImageLength + 4 ) > nTotalImageLength)
					{
						SAFE_DELETE_ARRAY(lpNVContent);
						return FALSE;
					}
					
					{
						WORD tmpSrcValue, tmpDstValue ;
						tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength]);
						CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
						m_CurrModuleItem.nItemID = tmpDstValue;
						
						tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength + 2]);
						CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
						m_CurrModuleItem.nItemLength = tmpDstValue;
					}
					
					/*m_CurrModuleItem.nItemID = *(WORD *)&(lpNVContent[ nNVImageLength]);
					m_CurrModuleItem.nItemLength = *(WORD *)&(lpNVContent[ nNVImageLength + 2];)*/
					m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
                    break;
                case STRUCT_ITEM_TYPE:
					if((nNVImageLength + 4 ) > nTotalImageLength)
					{
						SAFE_DELETE_ARRAY(lpNVContent);
						return FALSE;
					}
					
					{
						WORD tmpSrcValue, tmpDstValue ;
						tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength]);
						CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
						m_CurrModuleItem.nItemID = tmpDstValue;
						
						tmpSrcValue = *(WORD *)&(lpNVContent[ nNVImageLength + 2]);
						CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
						m_CurrModuleItem.nItemLength = tmpDstValue;
					}
					
					m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);                 
                    break;
				default:
					break;
                }
            }
        }
    }
	
    if(0 < tmpParentItem.nItemLength )
    {
        nNVImageLength += tmpParentItem.nItemLength + 4;
        nNVImageLength = ((nNVImageLength + 3) & 0xFFFFFFFC);
    }
	
    fclose(fpImage);
    SetModified( TRUE );
    SAFE_DELETE_ARRAY(lpNVContent);
    return TRUE;
}

inline DWORD RoundTo4Align( DWORD value )
{
	return ( ( value + 3 ) & 0xFFFFFFFC );
}

static BOOL GetNextArrayForMemFind( LPBYTE lpFindCode, 
								   int dwFindCodeSize , int* pNext )
{
    int j = 0; 
    int k = -1;
    pNext[0] = -1;
    while( j < dwFindCodeSize - 1 )
    {
        if( k == -1 || lpFindCode[j] == lpFindCode[k] )
        {
            ++j;++k;
            pNext[j] = k;
        }
        else
        {
            k = pNext[k];
        }
    }
    return TRUE;
}

static int MemFind( LPBYTE lpCode, DWORD dwCodeSize, DWORD& dwCurPos,
				   LPBYTE lpFindCode, int dwFindCodeSize, int* pNext )
{
    int j = 0;
    while( dwCurPos < dwCodeSize && j < dwFindCodeSize )
    {
        if( j == -1 || lpCode[dwCurPos] == lpFindCode[j] )
        {
            ++dwCurPos; ++j;
        }
        else
        {
            j = pNext[j];
        }
    }
    
    if( j == dwFindCodeSize )
    {
        return (int)((int)dwCurPos - dwFindCodeSize);
    }
    return -1;
}

//Generate binary file
// @ZHANG Ming : 2003-05-06
// Replace dsp file's default name by szDspFileName
BOOL CFileParser::SaveToImage( const TCHAR * szDspFileName  ,
							  const void * lpAppendage    ,
							  DWORD        cbSize         )
{
    FILE *fpTarget = NULL; 
	
    UINT nNVImageLength = 0;   
    ModuleItem tmpParentItem;
	CMapID     mapIds; // for repeated ID check
	CMapID     mapIDs = m_mapIDs; // for MD5
	
	m_szItemContent = new BYTE[MAX_TOTAL_ITEM_CONTENT];
	if (NULL == m_szItemContent)
	{
		return FALSE;
	}
	
    ZeroMemory(m_szItemContent, MAX_TOTAL_ITEM_CONTENT);
	
	if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
    if(0 == _tcscmp(m_szTargetFileName, _T("")))
    { 
		//Notify that has not given target file
		SAFE_DELETE_ARRAY(m_szItemContent);
        return FALSE;
    }
    
    BOOL bReadOnly;
	
    fpTarget = OpenFileForWrite( m_szTargetFileName, &bReadOnly, TRUE );
    if( NULL == fpTarget )
    {
		//Notify that open file failed
		
		_tprintf( _T("Can't open file %s!"), m_szTargetFileName );
		
		// ]]
		
		SAFE_DELETE_ARRAY(m_szItemContent);
        return FALSE;
    }
	
    ZeroMemory(&tmpParentItem, sizeof(ModuleItem));
#define HASH_LENGTH  40   
    //Generate the image file indentifier
	// 	//add for nv update
	// 	CString  strDs = m_szPrjDesc;
	//     if (0 == strDs.CompareNoCase(_T("stnv")))
	//     {		
	// 		int nLen = WideCharToMultiByte(CP_ACP, 0, strDs, strDs.GetLength(), NULL, 0, NULL, NULL);
	// 		char *pFileName = new char[nLen + 1];
	// 		//	char pFileName[FILE_NAME_LENGTH];
	// 		WideCharToMultiByte(CP_ACP, 0, strDs, strDs.GetLength(), pFileName, nLen, NULL, NULL);
	//     	pFileName[nLen] = '\0';
	// 		memcpy(m_szItemContent + nNVImageLength, pFileName, 4);
	// 
	// 		if (m_bVerFlag)
	// 		{	
	// 			memcpy(m_szItemContent + 8, pFileName, 4);// ID 0 reserved  
	// 		}
	// 
	// 		if (NULL != pFileName)
	// 		{
	// 			delete []pFileName;
	// 			pFileName = NULL;
	// 		}
	// 		nNVImageLength += 4;
	// 		
	//     }
	// 	else
	{    
        DWORD dwGenerateTime = GetTickCount();
        DWORD tmpDstValue;
        CONVERT_INT2(dwGenerateTime, tmpDstValue);
        *(LONG *)&m_szItemContent[ nNVImageLength ] = tmpDstValue;
        nNVImageLength += 4;
    }
	// add for ID version check and repeated IDs check.
	UINT  dwNVLength = nNVImageLength;
	if (m_bVerFlag)
	{
		nNVImageLength += HASH_LENGTH;
	}
	
	if (m_bWriteFix)
	{
		//CFileMap map(m_szFixBin);
		LPBYTE pFixBuf = NULL;
		DWORD dwFileSize;
		
		
		FILE *fpFixBin = _tfopen(m_szFixBin,  _T("rb"));
		if(NULL == fpFixBin)
        {
			//Notify that can not open target file
			_tprintf(_T("Can't open fixBin file, Please check the file is exist!"));
			fclose(fpTarget); // add by Joly
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
		}
		
		fseek(fpFixBin, 0, SEEK_END);
		dwFileSize = ftell(fpFixBin);
		pFixBuf = new BYTE[dwFileSize];
		if (NULL == pFixBuf)
		{
			_tprintf(_T("New buffer Failure!"));
			fclose(fpTarget); // add by Joly
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
		}
		ZeroMemory(pFixBuf,dwFileSize);
		
		//_tprintf(_T("%d"), dwFileSize);
		
		fseek(fpFixBin, 0, SEEK_SET);  
		
		int nRead = fread(pFixBuf, 1, dwFileSize, fpFixBin);
		
		fclose(fpFixBin);
		if ( nRead <= 0 )
		{
			_tprintf( _T("[ERROR] %s read %d failed "),  m_szFixBin, nRead);
			fclose(fpTarget); 
			SAFE_DELETE_ARRAY(m_szItemContent);
			SAFE_DELETE_ARRAY(pFixBuf);
			return FALSE;
		}
		
		AnalyzeFixBin((const unsigned char*)pFixBuf, dwFileSize, mapIDs);
		
		DWORD dwFixLength = dwFileSize - HASH_LENGTH - 4;
		memcpy(m_szItemContent+nNVImageLength, pFixBuf + HASH_LENGTH + 4, dwFixLength);  //merge fix bin to project bin
		nNVImageLength += dwFixLength;
		SAFE_DELETE_ARRAY(pFixBuf);
		if (!m_strErrID.IsEmpty())
		{
			_tprintf(_T("[ERROR] Existed repeated IDs: %s\n"), m_strErrID);	
			fclose(fpTarget); 
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
		}
	}
	
	//add nv version  2015/2/18
	if (m_bVerFlag)
	{
		CountMD5(mapIDs);
		WORD  tmpSrcValue, tmpDstValue;
		
		tmpSrcValue = (0x00);   //NV_VER_CODE
		CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
		*(WORD *)&m_szItemContent[dwNVLength] = tmpDstValue ;
		
		tmpSrcValue = 36;  //nv hash info struct 
		CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
		*(WORD *)&m_szItemContent[dwNVLength + 2] = tmpDstValue ;
		
        dwNVLength += 4;
		
		dwNVLength += 4;  // add reserved bytes
		
		memcpy(m_szItemContent+dwNVLength, m_digest, 16);
	}
    
    //Write the DSP Code into Download parmeter file
    // @ZHANG Ming : 2003-04-23
    // [[
    if ( m_bWriteDsp )   // ]]
    {
		if ( GetFileSize(szDspFileName) >= MAX_TOTAL_ITEM_CONTENT)
		{
			
			_tprintf(_T("[ERROR] Wrong DSP Code file,DSP ITEM1 [0X04] length can't over 64K!\n"));
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
		}
        // @ZHANG Ming : 2003-05-06  
        FILE *fpDspCode = _tfopen( szDspFileName, _T("rb") );
        if(NULL == fpDspCode)
        {
			//Notify that can not open target file
            _tprintf(_T("[ERROR] Can't open DSP Code file, Please check the file is exist!"));
            fclose(fpTarget); // add by Joly
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
        }
        
#define DSP_TOTAL_SIZE 0x20000
        BYTE tmp_dsp_code[DSP_TOTAL_SIZE];
        memset( tmp_dsp_code,0,DSP_TOTAL_SIZE );
		
        // Read DSP_TOTAL_SIZE bytes from dsp code file
        int nRead = fread( tmp_dsp_code, 1, DSP_TOTAL_SIZE, fpDspCode);
		
        fclose(fpDspCode);
		
        // There are one or two images in one dsp code file.If there are two,they are seperated by 
        // a given string.We must find the string.
		
        const int seperate_string_len = 4;
        const BYTE SEPERATE_STRING[] = { 0x5A,0x5A,0xA5,0xA5 };
        int find_next[seperate_string_len];
		
        GetNextArrayForMemFind( (LPBYTE)SEPERATE_STRING,seperate_string_len,find_next );
		
        ULONG lCurPos = 0;
        int nPos = MemFind(tmp_dsp_code, nRead, lCurPos, (LPBYTE)SEPERATE_STRING, seperate_string_len, find_next);
		
		//[ DSP File check
		if (0 == nRead || nRead > DSP_TOTAL_SIZE)
		{
			_tprintf(_T("[ERROR] Wrong DSP Code file, Please check the file!"));
			fclose(fpTarget); 
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
		}
		//]
		
        int nDspCodeLen1,nDspCodeLen2=0;
        if( nPos == -1 )
        {
            // We can not find sepearted string,that is mean there is only one image in the dsp code file
			if(nRead < FILE_TOTAL_SIZE)
			{
				nDspCodeLen1 = nRead;
			}
			else
			{
				_tprintf(_T("[ERROR] Wrong DSP Code file, DSP ITEM1 [0X04] length can't over 64K!"));
				fclose(fpTarget); 
				SAFE_DELETE_ARRAY(m_szItemContent);
				return FALSE;
			}
        }
        else
        {
			if (nPos< FILE_TOTAL_SIZE &&  (nRead - (int)lCurPos) < FILE_TOTAL_SIZE)
			{
				nDspCodeLen1 = nPos;
				nDspCodeLen2 = (int)(nRead - (int)lCurPos);
			}
			else
			{
				if (nPos>= FILE_TOTAL_SIZE)
				{
					_tprintf(_T("[ERROR] Wrong DSP Code file, DSP ITEM2-1 [0X04] length can't over 64K!"));
				}

				if ((nRead - (int)lCurPos) >= FILE_TOTAL_SIZE)
				{
					_tprintf(_T("[ERROR] Wrong DSP Code file, DSP ITEM2-2 [0X08] length can't over 64K!"));
				}

				fclose(fpTarget); 
				SAFE_DELETE_ARRAY(m_szItemContent);
				return FALSE;
			}
            
        }      
        
        //Write DSP Code NVItem Identifier
        {
            WORD  tmpSrcValue, tmpDstValue;
            
            tmpSrcValue = (0x04);   //NV_DSP_CODE
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[ nNVImageLength]    = tmpDstValue ;
            
            tmpSrcValue = (WORD)nDspCodeLen1;
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[ nNVImageLength + 2] = tmpDstValue ;
            
            nNVImageLength += 4;
        }
		
        memcpy( m_szItemContent+nNVImageLength,tmp_dsp_code,nDspCodeLen1 );
		
        for( int i= (int)((int)nNVImageLength+nDspCodeLen1);i>(int)nNVImageLength;i--)
        {
            if( 0 != m_szItemContent[i-1] )
            {
                break;
            }
        }
		
        i -= (int)nNVImageLength;
        // Round to 1K Bytes
        nDspCodeLen1 = ( ( i + 1023 ) & 0xFFFFFc00 );
		
        // Modify nv length field
        WORD tmpDstValue;
        WORD tmpSrcValue;
		tmpSrcValue = (WORD)nDspCodeLen1;
        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
        *(WORD *)&m_szItemContent[ nNVImageLength - 2] = tmpDstValue ;
		
        // Little endian to big endian
        for(int Index = 0; Index < nDspCodeLen1; Index+=2)
        {
            tmpSrcValue = *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index];
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index] = tmpDstValue;
        }
		
        nNVImageLength += nDspCodeLen1;
		
        if( nPos != -1 )
        {
            // Write dsp code image 2
            tmpSrcValue = (0x08);   //NV_DSP_CODE image 2
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[ nNVImageLength]    = tmpDstValue ;
			
            tmpSrcValue = (WORD)nDspCodeLen2;
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[ nNVImageLength + 2] = tmpDstValue ;
			
            nNVImageLength += 4;
			
            memcpy( m_szItemContent + nNVImageLength,tmp_dsp_code+lCurPos,nDspCodeLen2 );
			
            // Little endian to big endian
            for(Index = 0; Index < nDspCodeLen2; Index+=2 )
            {
                tmpSrcValue = *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index];
                CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index] = tmpDstValue;
            }
            
            nNVImageLength += RoundTo4Align(nDspCodeLen2);
        }
		
    }   // if ( m_bWriteDsp )
	//End of Writing DSP Code
	
	if ( ( NULL != lpAppendage ) && ( cbSize > 0 ) )
	{
        if( cbSize <= 4 )
        {
            
            fclose(fpTarget); // add by Joly
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
        }
		nNVImageLength = RoundTo4Align( nNVImageLength );
		::memmove( m_szItemContent + nNVImageLength, 
			(BYTE*)lpAppendage + 4,  // skip the time stamp in scinv.bin
			cbSize - 4 );
        nNVImageLength += ( cbSize - 4 );
	}
	
	nNVImageLength = RoundTo4Align( nNVImageLength );
	
    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {
        m_currModuleItemList = m_lstModuleItemPtr.GetAt (nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < GetModuleItemCount(nCurModuleIndex); nCurItemIndex++)
        {
            m_CurrModuleItem = m_currModuleItemList->GetAt (nCurItemIndex);
			if (m_CurrModuleItem.nItemID > 0xFFFF)
			{
				
				_tprintf(_T("[ERROR] [ItemName: %s]:item ID [%d] is not valid."),
					m_CurrModuleItem.szItemName, m_CurrModuleItem.nItemID);							
				
				fclose(fpTarget);
				SAFE_DELETE_ARRAY(m_szItemContent);
				return FALSE;
			}
			
            if(INVALID_MODULE_ID != m_CurrModuleItem.nParentItemIndex )
            {
                switch(m_CurrModuleItem.eItemType)
                {
                case CHAR_ITEM_TYPE:                
                    {
                        if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') )||
                            ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                        {
							m_szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4 ]          = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
                        }
                        else
                        {
							m_szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4 ]          = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10 );
                        }
                    }
                    break;
                case SHORT_ITEM_TYPE:
                    {
                        WORD  tmpSrcValue;
                        WORD  tmpDstValue;
						
                        if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') ) ||
                            ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                        {
							tmpSrcValue  = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
                        }
                        else
                        {
                            tmpSrcValue  = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10 );
                        }
                        
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4] = tmpDstValue;                        
                    }
                    break;
                case LONG_ITEM_TYPE:
                    {
                        DWORD  tmpSrcValue;
                        DWORD  tmpDstValue;
                        if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') ) ||
                            ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                        {
							// @ZHANG Ming : 2003-08-25
							// [[ chang strtol to strtoul
                            tmpSrcValue  = (DWORD)_tcstoul(m_CurrModuleItem.szItemContent, NULL, 16 );
							
                        }
                        else
                        {
                            tmpSrcValue  = (DWORD)_tcstoul(m_CurrModuleItem.szItemContent, NULL, 10 );
                        }
						// ]]
						
                        CONVERT_INT2(tmpSrcValue, tmpDstValue);
						// @ZHANG Ming : 2003-08-25
						// Change LONG to DWORD
						// [[
                        // *(LONG *)&m_szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4] = tmpDstValue;
						*(DWORD *)&m_szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4] = tmpDstValue;
						// ]]
                    }
                    break;  
                case ARRAY_ITEM_TYPE:
				case ASTRING_ITEM_TYPE:
				case WSTRING_ITEM_TYPE:
                    break;
				default:
					break;
                }
            }
            else
            {
				// [[ for repeated ID check
				
				
				CMapID::iterator  it = mapIds.find(m_CurrModuleItem.nItemID);
				if (mapIds.end() != it)
				{
					_tprintf(_T("%d had existed\n"), m_CurrModuleItem.nItemID);
					SAFE_DELETE_ARRAY(m_szItemContent);
					return FALSE;
				}
				else
				{
					mapIds.insert(CMapID::value_type(m_CurrModuleItem.nItemID, m_CurrModuleItem.nItemLength));
				}
				//]]
				
                if(0 < tmpParentItem.nItemLength )
                {
                    nNVImageLength += tmpParentItem.nItemLength + 4;
                    nNVImageLength = ((nNVImageLength + 3) & 0xFFFFFFFC);
                }
				
                tmpParentItem = m_CurrModuleItem;
                
                switch(m_CurrModuleItem.eItemType)
                {
                case CHAR_ITEM_TYPE:                
                    {                         
                        WORD tmpSrcValue, tmpDstValue ;						
						
                        tmpSrcValue = (WORD)m_CurrModuleItem.nItemID;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[ nNVImageLength]    = tmpDstValue ;
						
                        tmpSrcValue = (WORD)m_CurrModuleItem.nItemLength;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[ nNVImageLength + 2] = tmpDstValue ;
						
                        if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') ) ||
                            ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                        {
							m_szItemContent[ nNVImageLength + 4 ]          = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
                        }
                        else
                        {
							m_szItemContent[ nNVImageLength + 4 ]          = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10 );
                        }                        
                    }
                    break;
                case SHORT_ITEM_TYPE:
                    {
                        WORD  tmpSrcValue;
                        WORD  tmpDstValue;
						
                        tmpSrcValue = (WORD)m_CurrModuleItem.nItemID;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[ nNVImageLength]    = tmpDstValue ;
						
                        tmpSrcValue = (WORD)m_CurrModuleItem.nItemLength;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[ nNVImageLength + 2] = tmpDstValue ;
						
                        
						
                        if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') ) ||
                            ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                        {
							tmpSrcValue  = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
                        }
                        else
                        {
                            tmpSrcValue  = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10 );
                        }
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[ nNVImageLength + 4] = tmpDstValue;                        
                    }
                    break;
                case LONG_ITEM_TYPE:
                    {
                        
                        {
							WORD tmpSrcValue, tmpDstValue;
							tmpSrcValue = (WORD)m_CurrModuleItem.nItemID;
							CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
							*(WORD *)&m_szItemContent[ nNVImageLength]    = tmpDstValue ;
							
							
							tmpSrcValue = (WORD)m_CurrModuleItem.nItemLength;
							CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
							*(WORD *)&m_szItemContent[ nNVImageLength + 2] = tmpDstValue ;
                        }
						
                        
                        DWORD  dwSrcValue ;
                        DWORD  dwDstValue ;
						
						// @ZHANG Ming : 2003-8-25
						// change strtol to strtoul
						// [[
                        if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') ) ||
                            ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                        {
                            dwSrcValue  = (DWORD)_tcstoul(m_CurrModuleItem.szItemContent, NULL, 16 );
                        }
                        else
                        {
                            dwSrcValue  = (DWORD)_tcstoul(m_CurrModuleItem.szItemContent, NULL, 10 );
                        }
						// ]]
						
                        CONVERT_INT2(dwSrcValue, dwDstValue);
						// @ZHANG Ming : 2003-08-25
						// Change LONG to DWORD
						// [[
                        // *(LONG *)&m_szItemContent[ nNVImageLength + 4] = dwDstValue;   
						*(DWORD *)&m_szItemContent[ nNVImageLength + 4] = dwDstValue;
						// ]]
                    }
                    break;  
                case ARRAY_ITEM_TYPE:
				case ASTRING_ITEM_TYPE:
				case WSTRING_ITEM_TYPE:
                case STRUCT_ITEM_TYPE:
                    {
                        WORD  tmpSrcValue, tmpDstValue;
						
                        tmpSrcValue = (WORD)m_CurrModuleItem.nItemID;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[ nNVImageLength]    = tmpDstValue ;
						
                        tmpSrcValue = (WORD)m_CurrModuleItem.nItemLength;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[ nNVImageLength + 2] = tmpDstValue ;
						
						
                    }
                    break;
				default:
					break;
                }
            }
        }
    }
	
    if( 0 < tmpParentItem.nItemLength )
    {
        nNVImageLength += tmpParentItem.nItemLength + 4;
        nNVImageLength = ((nNVImageLength + 3) & 0xFFFFFFFC);
    }
	
	//[[ by Joly  write other bin file
	if ( m_bWriteMult )
	{
		int nRt = ReadOtherFiles( nNVImageLength );
		if ( 1 == nRt )
		{
			fclose(fpTarget);
			SAFE_DELETE_ARRAY(m_szItemContent);
			return FALSE;
		}
	}
	
	if (m_bVerFlag)
	{
		CountAllMD5(nNVImageLength);
	}
	
	
    fwrite(m_szItemContent, 1, nNVImageLength, fpTarget);
	
    if (m_bTailFlag)
    {
		BYTE  strend[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
        fwrite(strend,1,8,fpTarget);
    }
    //]]
	
    fclose(fpTarget);
	
	//[[ Joly
	if (m_bHashHeader)
	{
		WriteHashHeader();
	}
	//]]
    // @ZHANG Ming : 2003-04-18
    // [[       
    if ( bReadOnly )
    {
        // Restore the file's read-only attribute
        VERIFY( ChangeFileRdOnlyAttri( m_CurrModule.szModuleFileName, TRUE ) );
    }
    // ]]

	SAFE_DELETE_ARRAY(m_szItemContent);
	
    return TRUE;
}

//[[Joly
void CFileParser::WriteHashHeader()
{
	CString  strLine;
	CStdioFile file(m_szHashPath, CFile::modeCreate | CFile::modeWrite);
	strLine = _T("#ifndef _NV_COMPILE_HASH_H_ \n#define _NV_COMPILE_HASH_H_\n\nuint8 nv_compile_hash[16] = {");
	for (int i = 0; i < 16; i++)
	{
		CString strTmp;
		// length 28=  time (4) + ID0(2) + length(2) + version(2) + reserved(2) + struchHash(16)
#define  ALL_HASH_START  28
		if (15 == i)
		{
			strTmp.Format(_T("0x%02X};"), m_szItemContent[ALL_HASH_START + i]);
		}
		else
		{
			strTmp.Format(_T("0x%02X,"), m_szItemContent[ALL_HASH_START + i]);
		}
		strLine += strTmp;
	}
	strLine += _T("\n\n#endif //_NV_COMPILE_HASH_H_");
	file.WriteString(strLine);	
	file.Close();
}

void CFileParser::CountAllMD5(DWORD nNVLength)
{
	MD5_CTX context;	
	unsigned char digest[16];
	
	MD5Init (&context);
	
	DWORD nOffset = 0;
	// Insert item into CFileParser
	// Skip time stamp and MD5 ( 4 bytes long )
	nOffset += 44;
	
	while(nOffset < nNVLength)
	{
		// 		WORD wItemId = *( WORD* )(m_szItemContent + nOffset);	
		// 		CONVERT_SHORT2(wItemId, wItemId);
		
		MD5Update (&context, (unsigned char*)(m_szItemContent + nOffset), 2);
		
		nOffset += sizeof(WORD);
		if (nOffset >= nNVLength)
		{
			break;
		}
		
		WORD wLen = *(WORD*)(m_szItemContent + nOffset);
		CONVERT_SHORT2(wLen, wLen);
		
		MD5Update (&context, (unsigned char*)(m_szItemContent + nOffset), 2);
		
		nOffset += sizeof( WORD );
		if (nOffset + wLen > nNVLength)
		{
			break;
		}
		
		MD5Update (&context, (unsigned char*)(m_szItemContent + nOffset), wLen);
		
		nOffset += wLen;
		// Adjust for alignment of 4 bytes
		nOffset = (nOffset + 3) & 0xfffffffc ;		
	}
	MD5Final (digest, &context);
	
	// length 28=  time (4) + ID0(2) + length(2) + version(2) + reserved(2) + struchHash(16)
	memcpy(m_szItemContent + 28, digest, 16);
}

void  CFileParser::GetWriteTailFlag()
{
	BOOL  bFlag = FALSE;
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	TCHAR *p = _tcsrchr(szPath, '.');
	if(p)
	{
		*p='\0';
	}	
	_tcscat(szPath, _T(".ini") );
    bFlag = GetPrivateProfileInt(_T("Settings"), _T("WriteTail"), 1, szPath);
	
	m_bTailFlag = bFlag;
	
}
//]]

int CFileParser::GenerateOffset()
{
	
	m_lstTopItem.RemoveAll();
	
    int  nOffset = 0;
	
    // @ZHANG Ming : 2003-04-23
    // [[
    UINT nNVImageLength = 0;
    nNVImageLength = ( m_bWriteDsp ? 4 + (DSP_CODE_SIZE + 4) : 4 ); //The File Identifier and the DSP Code
    // ]]
    ModuleItem tmpLastParent;
    ModuleItem tmpLastItem;
	
	int nModuleItemCount = 0;
	int nItemMaxLen = 0;
	
    ZeroMemory(&tmpLastItem, sizeof(ModuleItem));
	
    //write module list part
    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {               
        m_currModuleItemList = m_lstModuleItemPtr.GetAt (nCurModuleIndex);
		nModuleItemCount = GetModuleItemCount(nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; nCurItemIndex++)
        {
			nOffset = 0;
			
			m_CurrModuleItem = m_currModuleItemList->GetAt (nCurItemIndex);
			
			if( STRUCT_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				nItemMaxLen = 0;
				tmpLastParent = m_CurrModuleItem;
				GenerateStructOffset(nCurModuleIndex, &nCurItemIndex, &nOffset,&nItemMaxLen);
				m_CurrModuleItem = tmpLastParent;
				m_CurrModuleItem.nItemLength = nOffset;
			}
			else if(ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType  )
			{
				nCurItemIndex++;
				
				ASSERT(nCurItemIndex < nModuleItemCount);
				if(nCurItemIndex >= nModuleItemCount)
				{
					break;
				}
				
				tmpLastParent = m_CurrModuleItem;
				for(UINT i = 0; i< tmpLastParent.nArraySize; i++)
				{
					m_CurrModuleItem = m_currModuleItemList->GetAt (nCurItemIndex);
					switch(m_CurrModuleItem.eItemType )
					{
					case CHAR_ITEM_TYPE:                
						{          
							m_CurrModuleItem.nOffset = nOffset;
							nOffset++;
						}
						break;
					case SHORT_ITEM_TYPE:
						{
							nOffset = (int)(((UINT)(nOffset + 1)) & 0xFFFFFFFE);
							m_CurrModuleItem.nOffset = nOffset;   
							nOffset += 2;
						}   
						break;
					case LONG_ITEM_TYPE:
						{
							nOffset =(int)(((UINT)(nOffset + 3)) & 0xFFFFFFFC);
							m_CurrModuleItem.nOffset = nOffset;
							nOffset += 4;
						}
						break;
					case ASTRING_ITEM_TYPE:     
					case WSTRING_ITEM_TYPE:
						{
							nItemMaxLen = 0;
							ModuleItem tmpSubParentItem = m_CurrModuleItem;
							GenerateStructOffset(nCurModuleIndex,&nCurItemIndex,&nOffset,&nItemMaxLen); 
							m_CurrModuleItem = tmpSubParentItem;
							m_CurrModuleItem.nItemLength = m_CurrModuleItem.nArraySize*nItemMaxLen;
						}
						break;
					default:
						break;
					}
					
					m_currModuleItemList->SetAt(m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
					nCurItemIndex += 1;
					
					//ASSERT(nCurItemIndex < nModuleItemCount);
					if(nCurItemIndex >= nModuleItemCount)
					{
						break;
					}
					
				}
				
				nCurItemIndex -= 1;
				
				tmpLastParent.nItemLength = m_CurrModuleItem.nItemLength * tmpLastParent.nArraySize ;
				m_currModuleItemList->SetAt(tmpLastParent.nItemIndex , tmpLastParent);                
				
				m_CurrModuleItem = tmpLastParent;
			}
			else if (CHAR_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				m_currModuleItemList->SetAt(m_CurrModuleItem.nItemIndex, m_CurrModuleItem);  
			}
			else if (SHORT_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				m_currModuleItemList->SetAt(m_CurrModuleItem.nItemIndex, m_CurrModuleItem);  
			}
			else if (LONG_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				m_currModuleItemList->SetAt(m_CurrModuleItem.nItemIndex, m_CurrModuleItem);  
			}
			
			//Aligned by 4 bytes
			nNVImageLength = ((nNVImageLength + 3) & 0xFFFFFFFC);
			m_CurrModuleItem.nOffset = nNVImageLength + 4;
			//At this the 'nParentItemIndex' indicates module index
			m_CurrModuleItem.nParentItemIndex = nCurModuleIndex;
			nNVImageLength = m_CurrModuleItem.nOffset + m_CurrModuleItem.nItemLength;

			if (m_CurrModuleItem.nItemLength > 0xFFFF || 0 == m_CurrModuleItem.nItemLength)
			{
				Module  stCurModule = m_lstModule.GetAt(nCurModuleIndex);
				_tprintf( _T("[ERROR] [NVM: %s] [ItemName: %s]:nv item data length [%d] is invalid.\n") ,
					GetStrFileTitle(stCurModule.szModuleFileName),
					m_CurrModuleItem.szItemName, 
					m_CurrModuleItem.nItemLength);
				return 0;
			}
			m_lstTopItem.Add(m_CurrModuleItem);
			
        }
		
        //int a = 0;
    }   
	
    return 1;
	
}

void CFileParser::GenerateStructOffset(int nCurModuleIndex, int *nCurItemIndex, int *nFirstOffset,int *nItemMaxLen)
{    
    int nItemIndex;
    int nTotalModuleItemCount;
    int nParentIndex;
    int nOffset;
	int nMaxLen=0;
	
    nOffset = *nFirstOffset;
    nParentIndex = *nCurItemIndex;
    ModuleItem tmpParentModuleItem = GetModuleItem(nCurModuleIndex, nParentIndex);
    /*CR233310 by xmzhang add begin*/
    tmpParentModuleItem.nOffset = 0;
    /*CR233310 by xmzhang add end*/	
    nTotalModuleItemCount = GetModuleItemCount(nCurModuleIndex);
	
    for(nItemIndex = nParentIndex + 1; nItemIndex < nTotalModuleItemCount; nItemIndex++)
    {
        ModuleItem tmpModuleItem = GetModuleItem(nCurModuleIndex, nItemIndex);
        
        if((int)tmpModuleItem.nParentItemIndex < nParentIndex)
        {
            *nCurItemIndex = nItemIndex - 1;
			
            if(tmpParentModuleItem.nOffset == 2)
            {
                nOffset = (int)(((UINT)(nOffset + 1)) & 0xFFFFFFFE);
            }
            else if(tmpParentModuleItem.nOffset == 4)
            {
                nOffset = (int)(((UINT)(nOffset + 3)) & 0xFFFFFFFC);
            }  
			// modify for length 
            *nFirstOffset = nOffset;
            tmpParentModuleItem.nItemLength  = nOffset;
            m_currModuleItemList->SetAt(tmpParentModuleItem.nItemIndex , tmpParentModuleItem);
			
            return;
			
        }        
        else
        {
            if( (STRUCT_ITEM_TYPE == tmpModuleItem.eItemType ) ||
                (ARRAY_ITEM_TYPE == tmpModuleItem.eItemType ) ||
				(ASTRING_ITEM_TYPE == tmpModuleItem.eItemType ) ||
				(WSTRING_ITEM_TYPE == tmpModuleItem.eItemType ))
            {
				nMaxLen = 0;
				/*CR233310 by xmzhang modify begin*/
				int tmp = nOffset;
				int tmpIndex = nItemIndex;
				GenerateStructOffset(nCurModuleIndex, &nItemIndex, &nOffset,&nMaxLen);
				tmpModuleItem.nItemLength = nOffset - tmp;
				tmpModuleItem.nOffset = nMaxLen;
				m_currModuleItemList->SetAt(tmpModuleItem.nItemIndex , tmpModuleItem);
				if(tmpParentModuleItem.nOffset < (UINT)nMaxLen)
				{
					tmpParentModuleItem.nOffset = (UINT)nMaxLen;
					if(tmpParentModuleItem.nOffset == 1)
					{
						nOffset = tmp;
					}
					else if(tmpParentModuleItem.nOffset == 2)
					{
						nOffset = (int)(((UINT)(tmp + 1)) & 0xFFFFFFFE);
					}
					else if(tmpParentModuleItem.nOffset == 4)
					{
						nOffset = (int)(((UINT)(tmp + 3)) & 0xFFFFFFFC);
					}  
					nItemIndex = tmpIndex;
					GenerateStructOffset(nCurModuleIndex, &nItemIndex, &nOffset,&nMaxLen);
				}
				/*CR233310 by xmzhang modify end*/
            }			
            else
            {
                switch(tmpModuleItem.eItemType)
                {
                case CHAR_ITEM_TYPE:                
                    {
                        tmpModuleItem.nOffset = nOffset;
                        nOffset++;						
                        if(tmpParentModuleItem.nOffset <= 1)
						{
							tmpParentModuleItem.nOffset = 1;
							
						}
						if(nMaxLen <= 1)
							nMaxLen = 1;
                    }
                    break;
                case SHORT_ITEM_TYPE:
                    {
                        nOffset = (int)(((UINT)(nOffset + 1)) & 0xFFFFFFFE);
                        tmpModuleItem.nOffset = nOffset;  
                        nOffset += 2;           
                        if(tmpParentModuleItem.nOffset <= 2)
							tmpParentModuleItem.nOffset = 2;
						
						if(nMaxLen <= 2)
							nMaxLen = 2;
                    }   
                    break;
                case LONG_ITEM_TYPE:
                    {
                        nOffset = (int)(((UINT)(nOffset + 3)) & 0xFFFFFFFC);
                        tmpModuleItem.nOffset = nOffset;
                        nOffset += 4;
                        if(tmpParentModuleItem.nOffset <= 4)
							tmpParentModuleItem.nOffset = 4;
						
						if(nMaxLen <= 4)
							nMaxLen = 4;
                    }
                    break; 
				default:
					break;
                }
				
				*nItemMaxLen = (*nItemMaxLen < nMaxLen) ? nMaxLen : *nItemMaxLen;
                
				_ASSERTE(tmpModuleItem.nItemIndex < (UINT)m_currModuleItemList->GetSize());
                m_currModuleItemList->SetAt(tmpModuleItem.nItemIndex , tmpModuleItem);
                
            }
        }
    }
	
	
    *nCurItemIndex = nItemIndex - 1;
    if(tmpParentModuleItem.nOffset == 2)
    {
        nOffset = (int)(((UINT)(nOffset + 1)) & 0xFFFFFFFE);
    }
    else if(tmpParentModuleItem.nOffset == 4)
    {
        nOffset = (int)(((UINT)(nOffset + 3)) & 0xFFFFFFFC);
    }  
    
    *nFirstOffset = nOffset;
    tmpParentModuleItem.nItemLength  = nOffset;
    m_currModuleItemList->SetAt(tmpParentModuleItem.nItemIndex , tmpParentModuleItem);
	
}

int CFileParser::InsertModule(TCHAR *szMOduleFileName)
{
    if(NULL != szMOduleFileName)
    {
        //@HONGLINAG XIN: 2009-5-8
        m_agStrError.RemoveAll();
		
        /*xml file support by xmzhang modify begin*/
        m_szappend=_tcsrchr(szMOduleFileName,'.');
        if(0==_tcsicmp(m_szappend,_T(".xml")))
        {
            LoadModuleXML(szMOduleFileName);
        }
        
        else if(0==_tcsicmp(m_szappend,_T(".nvm")))
        {
            LoadModule(szMOduleFileName);
        }
		
        else
        {
            return INVALID_MODULE_ID;
        }
        /*xml file support by xmzhang modify end*/
        m_nModuleCount++;
		
		//@HONGLINAG XIN: 2009-5-8
		if(m_agStrError.GetSize() != 0)
		{
			CString str = _T("");
			for(int k=0;k<m_agStrError.GetSize() && k< MAX_DISP_ERR_MSG;k++)
			{
				str += m_agStrError[k];
				str += _T("\n");
			}
			
			if(m_agStrError.GetSize() > MAX_DISP_ERR_MSG)
			{
				CString strTmp;			
				strTmp.Format( _T("... there were %d errors!\n" ),m_agStrError.GetSize() );
				str += strTmp;
			}
			
			_tprintf(str);
			
		}
		
        // @ZHANG Ming : 2003-05-06
        SetModified( TRUE );
		
        return m_nModuleCount;
    }
	
    return INVALID_MODULE_ID;
}

BOOL CFileParser::SaveProjectAs(TCHAR *szPrjFileName)
{
    if( (NULL == szPrjFileName) ||
        (!_tcscmp(szPrjFileName, _T(""))))
    {
        return FALSE;
    }    
	
    TCHAR szPrjPath[MAX_PATH];
    _tcscpy(szPrjPath,szPrjFileName);
    
    TCHAR* pPrjPath = _tcsrchr(szPrjPath,'\\');
    if(pPrjPath == NULL)
    {
        // Must full path name
        return FALSE;
    }
    
    _tcscpy(m_szPrjFileName, szPrjFileName);
    *pPrjPath = 0;
    
    TCHAR szModuleName[MAX_PATH];
    
    for(UINT Index = 0; Index < m_nModuleCount; Index++)
    {
        m_CurrModule = m_lstModule.GetAt (Index);
        _tcscpy(szModuleName,m_CurrModule.szModuleFileName);
        
        TCHAR *pResult = _tcsrchr(szModuleName,'\\');
        if(pResult == NULL)
        {
            // Relative path,only insert project path to the head
            _tcscpy(m_CurrModule.szModuleFileName,szPrjPath);
            _tcscat(m_CurrModule.szModuleFileName,szModuleName);
        }
        else
        {
            // Absolute path,replace the project path
            _tcscpy(m_CurrModule.szModuleFileName,szPrjPath);
            _tcscat(m_CurrModule.szModuleFileName,pResult);
        }
        
        m_lstModule.SetAt(Index,m_CurrModule);
    }  
    /*xml file support by xmzhang add begin*/	
    m_szappend=_tcsrchr(szPrjFileName,'.');
    if(0==_tcscmp(m_szappend,_T(".xprj")))
    {
        return (SaveProjectXML());
    }
    else
    {
        return (SaveProject());
    }
    /*xml file support by xmzhang add end*/    
}

void CFileParser::FillWithDefault()
{    
    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {
        m_currModuleItemList = m_lstModuleItemPtr.GetAt (nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < GetModuleItemCount(nCurModuleIndex); nCurItemIndex++)
        {
            m_CurrModuleItem = m_currModuleItemList->GetAt (nCurItemIndex);
            switch(m_CurrModuleItem.eItemType)
            {
            case CHAR_ITEM_TYPE:
            case SHORT_ITEM_TYPE:
            case LONG_ITEM_TYPE:
                _tcscpy(m_CurrModuleItem.szItemContent , _T("0"));
                break;
			default:
				break;
            }
            m_currModuleItemList->SetAt (nCurItemIndex, m_CurrModuleItem);
        }
    }
    // @ZHANG Ming : 2003-05-06
    SetModified( TRUE );
}

BOOL CFileParser::WriteNVItem(UINT nItemID, LPVOID lpBuffer, 
							  DWORD nNumberOfBytesToWrite, 
							  BOOL bBigEndian)
{
	UNREFERENCED_PARAMETER(bBigEndian);
    ModuleItem hTopItem;     
    BYTE *pData;
	
    if(NULL == lpBuffer)
    {
        return FALSE;
    }
	
    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
    pData = (BYTE *)lpBuffer;
	
    for(int nIndex = 0; nIndex < m_lstTopItem.GetSize (); nIndex++)
    {
        hTopItem = m_lstTopItem.GetAt (nIndex);
        if(nItemID == hTopItem.nItemID )
        {
            if(nNumberOfBytesToWrite == hTopItem.nItemLength )
            {
                m_currModuleItemList = m_lstModuleItemPtr.GetAt (hTopItem.nParentItemIndex);
                int nCurr = hTopItem.nItemIndex;
                if ( LONG_ITEM_TYPE < hTopItem.eItemType ) 
                {
                    nCurr++;
                }
				
                for(; nCurr < m_currModuleItemList->GetSize (); nCurr++)
                {
                    m_CurrModuleItem = m_currModuleItemList->GetAt (nCurr);
                    if(nItemID != m_CurrModuleItem.nItemID )
                    {
                        // @ZHANG Ming : 2003-05-06
                        SetModified( TRUE );
                        return TRUE;
                    }			
					
					if(m_CurrModuleItem.bReadOnly /*&& !theApp.m_bSupper*/) //MarkA
					{
						continue;
					}
					
                    switch(m_CurrModuleItem.eItemType)
                    {
                    case CHAR_ITEM_TYPE:                
                        {
                            //@ Liu Kai 2004-8-25 CR12684
                            unsigned char c = (unsigned char)_tcstol( m_CurrModuleItem.szItemContent, NULL, 0 );
                            m_CurrModuleItem.bChanged = ( c != pData[m_CurrModuleItem.nOffset] );
							
                            _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"), pData[m_CurrModuleItem.nOffset]);
                        }
                        break;
                    case SHORT_ITEM_TYPE:
                        {  
                            WORD  tmpSrcValue = *(WORD *)&pData[m_CurrModuleItem.nOffset];
                            WORD  tmpDstValue;
							
                            if(TRUE == g_bBigEndian)
                            {
                                CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                            }
                            else
                            {
                                tmpDstValue = tmpSrcValue;
                            }
							
                            //@ Liu Kai 2004-8-25 CR12684
                           // WORD w = (WORD)_tcstol( m_CurrModuleItem.szItemContent, NULL, 0 );
							DWORD w = _tcstoul( m_CurrModuleItem.szItemContent,NULL,0 ); // Joly 2015/8/20
                            m_CurrModuleItem.bChanged = ( w != tmpDstValue );
                            
                            _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
							
                        }
                        break;
                    case LONG_ITEM_TYPE:
                        {
                            DWORD  tmpSrcValue = *(DWORD *)&pData[m_CurrModuleItem.nOffset];
                            DWORD  tmpDstValue;
							
                            if(TRUE == g_bBigEndian)
                            {
                                CONVERT_INT2(tmpSrcValue, tmpDstValue);
                            }
                            else
                            {
                                tmpDstValue = tmpSrcValue;
                            }
							
                            //@ Liu Kai 2004-8-25 CR12684
                            DWORD l = _tcstoul( m_CurrModuleItem.szItemContent,NULL,0 );
                            m_CurrModuleItem.bChanged = ( l != tmpDstValue );
                            
                            _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                        }
                        break;  
					default:
						break;
                    }
                    
                    m_currModuleItemList->SetAt (m_CurrModuleItem.nItemIndex , m_CurrModuleItem);
                    
                }
                // @ZHANG Ming : 2003-05-06
                SetModified( TRUE );
				
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
	
    return FALSE;
}
BOOL CFileParser::ReadNVItem(UINT nItemID, 
                             LPVOID lpBuffer,
                             DWORD nNumberOfBytesToRead,
                             LPDWORD lpNumberOfBytesRead,
                             BOOL bBigEndian)
{
	UNREFERENCED_PARAMETER(bBigEndian);
    ModuleItem hTopItem;     
    unsigned char *pData;
	
    if((NULL == lpBuffer) || 
		(NULL == lpNumberOfBytesRead) )
    {
        return FALSE;
    }
	
    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
	*lpNumberOfBytesRead = 0;
	
    pData = (unsigned char *)lpBuffer;
    for(int nIndex = 0; nIndex < m_lstTopItem.GetSize (); nIndex++)
    {
        hTopItem = m_lstTopItem.GetAt (nIndex);
        if(nItemID == hTopItem.nItemID )
        {
            if(nNumberOfBytesToRead >= hTopItem.nItemLength )
            {
                m_currModuleItemList = m_lstModuleItemPtr.GetAt (hTopItem.nParentItemIndex);
                for(int nCurr = hTopItem.nItemIndex; nCurr < m_currModuleItemList->GetSize (); nCurr++)
                {
                    m_CurrModuleItem = m_currModuleItemList->GetAt (nCurr);
                    if(nItemID != m_CurrModuleItem.nItemID )
                    {
                        *lpNumberOfBytesRead = hTopItem.nItemLength;
                        return TRUE;
                    }
                    switch(m_CurrModuleItem.eItemType)
                    {
                    case CHAR_ITEM_TYPE:                
                        {
                            if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') )||
                                ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                            {
                                pData[m_CurrModuleItem.nOffset]          = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
                            }
                            else
                            {
                                pData[m_CurrModuleItem.nOffset]          = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10 );
                            }
                        }
                        break;
                    case SHORT_ITEM_TYPE:
                        {
                            WORD  tmpSrcValue;
                            WORD  tmpDstValue;
                            
                            if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') ) ||
                                ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                            {
                                tmpSrcValue  = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
                            }
                            else
                            {
                                tmpSrcValue  = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10 );
                            }
                            
                            if(TRUE == g_bBigEndian)
                            {
                                CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                            }
                            else
                            {
                                tmpDstValue = tmpSrcValue;
                            }
                            
                            *(WORD *)&pData[m_CurrModuleItem.nOffset] = tmpDstValue;                        
                        }
                        break;
                    case LONG_ITEM_TYPE:
                        {
                            DWORD  tmpSrcValue;
                            DWORD  tmpDstValue;
                            if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') ) ||
                                ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                            {
								// @ZHNAG Ming : 2003-08-29
								// [[
                                tmpSrcValue  = (DWORD)_tcstoul(m_CurrModuleItem.szItemContent, NULL, 16 );
								// ]]
                            }
                            else
                            {
								// @ZHANG Ming : 2003-08-29
								// [[
                                tmpSrcValue  = (DWORD)_tcstoul(m_CurrModuleItem.szItemContent, NULL, 10 );
								// ]]
                            }
                            
                            if(TRUE == g_bBigEndian)
                            {
                                CONVERT_INT2(tmpSrcValue, tmpDstValue);
                            }
                            else
                            {
                                tmpDstValue = tmpSrcValue;
                            }
                            
                            
							// @ZHANG Ming : 2003-08-25
							// [[ 
                            // *(LONG *)&pData[m_CurrModuleItem.nOffset] = tmpDstValue;
							*(DWORD *)&pData[m_CurrModuleItem.nOffset] = tmpDstValue;
							// ]]
                        }
                        break;  
					default:
						break;
                    }
                    
                }
                
            }
			
			*lpNumberOfBytesRead = hTopItem.nItemLength;			
        }		
    }
	
	if((*lpNumberOfBytesRead)>0)
	{
		return TRUE;
	}
	
    return  FALSE;
}

DWORD CFileParser::GetNVItemOffset(UINT nItemID)
{
    ModuleItem hTopItem;     
	
    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
    for(int nIndex = 0; nIndex < m_lstTopItem.GetSize (); nIndex++)
    {
        hTopItem = m_lstTopItem.GetAt (nIndex);
        if(nItemID == hTopItem.nItemID )
        {
            return hTopItem.nOffset;
        }
    }
	
    return 0L;
}

DWORD CFileParser::GetNVItemLength(UINT nItemID)
{
    ModuleItem hTopItem;     
	
    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
    for(int nIndex = 0; nIndex < m_lstTopItem.GetSize (); nIndex++)
    {
        hTopItem = m_lstTopItem.GetAt (nIndex);
        if(nItemID == hTopItem.nItemID )
        {
            return hTopItem.nItemLength;
        }
    }
	
    return 0L;
}

// @ZHANG Ming : 2003-04-16
// [[

int CFileParser::GetNVItemCount( void )
{
	
    //m_lstTopItem.RemoveAll(); // @ZHANG Ming : 2003-04-18
	
	if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	
    return m_lstTopItem.GetSize();
}
// [[ add by Joly 
DWORD CFileParser::GetNVItemLength(int nIndex)
{
	return m_lstTopItem.GetAt( nIndex ).nItemLength;
}
//]]

UINT CFileParser::GetNVItemID( int nIndex ) const
{
    return m_lstTopItem.GetAt( nIndex ).nItemID;
}

void CFileParser::ClearNVLoadState()
{
	for (INT i = 0; i < m_lstModule.GetSize(); i++)
	{
		Module  tmpModerm = m_lstModule.GetAt(i);
		tmpModerm.bLoadFailed = FALSE;
		m_lstModule.SetAt(i, tmpModerm);
		
		
		m_currModuleItemList = m_lstModuleItemPtr.GetAt(i);
		for (INT j = 0; j < m_currModuleItemList->GetSize(); j++)
		{
			m_CurrModuleItem = m_currModuleItemList->GetAt(j);
			
			m_CurrModuleItem.bLoadFailed = FALSE;
			m_currModuleItemList->SetAt(j, m_CurrModuleItem);
		}
	}
	
}

void CFileParser::SetNVLoadState(int nIndex, BOOL bFailed)
{
	ModuleItem tmpItem = m_lstTopItem.GetAt(nIndex);
	tmpItem.bLoadFailed = bFailed;
	
	UINT nItemId = GetNVItemID(nIndex);
	
	m_currModuleItemList = m_lstModuleItemPtr.GetAt(tmpItem.nParentItemIndex);
	
	Module  tmpModerm = m_lstModule.GetAt(tmpItem.nParentItemIndex);
	tmpModerm.bLoadFailed = bFailed;
	m_lstModule.SetAt(tmpItem.nParentItemIndex, tmpModerm);
	
	int nCurr = tmpItem.nItemIndex;
				
	for(; nCurr < m_currModuleItemList->GetSize (); nCurr++)
	{
		m_CurrModuleItem = m_currModuleItemList->GetAt(nCurr);
		if(nItemId != m_CurrModuleItem.nItemID )
		{
			return;
		}			
		m_CurrModuleItem.bLoadFailed = bFailed;
		m_currModuleItemList->SetAt(m_CurrModuleItem.nItemIndex, m_CurrModuleItem);
	}
	
} 

const TCHAR* CFileParser::GetNVItemName( int nIndex ) const
{
	return m_lstTopItem.GetAt( nIndex ).szItemName;
}

// ]]
/*xml file support by xmzhang add begin*/
BOOL CFileParser::IsFileRdOnly( const TCHAR *szFileName)
{
    ASSERT( NULL != szFileName );
	
    DWORD dwAttri = ::GetFileAttributes( szFileName );
	
    if ( (DWORD)(-1) == dwAttri )
    {
        // Get file attribute fail
        return FALSE;
    }
	
    if(0!=(dwAttri&FILE_ATTRIBUTE_READONLY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
	
}
/*xml file support by xmzhang add end*/
// @ZHANG Ming : 2003-04-18
// [[
BOOL CFileParser::ChangeFileRdOnlyAttri( const TCHAR *szFileName, BOOL bReadOnly )
{
    ASSERT( NULL != szFileName );
	
    DWORD dwAttri = ::GetFileAttributes( szFileName );
	
    if ( (DWORD)(-1) == dwAttri )
    {
        // Get file attribute fail
        return FALSE;
    }
	
    if ( bReadOnly )
    {
        TRACE( _T("CFileParser::ChangeFileRdOnlyAttri : \n")
			_T("\tAdd read-only attribute to %s\n"), 
			szFileName );
		
        dwAttri |= FILE_ATTRIBUTE_READONLY; // Add read-only attribute
    }
    else
    {
        TRACE( _T("CFileParser::ChangeFileRdOnlyAttri : \n")
			_T("\tStrip read-only attribute from %s\n"), 
			szFileName );
		
        dwAttri &= ~FILE_ATTRIBUTE_READONLY; // Strip off read-only attribute
    }
	
    return ::SetFileAttributes( szFileName, dwAttri );
}

// Note:
// bAttriChanged records whether the function has changed
// the file's read-only attribute for writing
FILE* CFileParser::OpenFileForWrite( const TCHAR *szFileName, 
									BOOL *bAttriChanged, 
									BOOL bBinaryMode /* = FALSE */ )
{
    ASSERT( NULL != szFileName );    
    
    if ( NULL != bAttriChanged )
    {
        *bAttriChanged = FALSE;
    }
	
    FILE *pFile = _tfopen( szFileName, bBinaryMode ? _T("wb") : _T("w") );
    if( NULL == pFile )
    {
		// Change the file's read-only attribute and re-opne it
		if ( ChangeFileRdOnlyAttri( szFileName, FALSE ) )
		{
			if ( NULL != bAttriChanged )
			{
				*bAttriChanged = TRUE;
			}
			
			// Re-open the file
			pFile = _tfopen( szFileName, bBinaryMode ? _T("wb") : _T("w") );
		}
    }
	
    return pFile;
}

// ]]
/*xml file support by xmzhang add begin*/
BOOL CFileParser::ChangeFileForWrite( const TCHAR *szFileName)
{
    BOOL bStatus = FALSE;
    ASSERT( NULL != szFileName );    	
	
    
	// Change the file's read-only attribute and re-opne it
    if ( ChangeFileRdOnlyAttri( szFileName, FALSE ) )
    {
        bStatus = TRUE;
    }
    
    
    return bStatus;
}
/*xml file support by xmzhang add end*/

// @ZHANG Ming : 2003-04-23
// [[
void CFileParser::SetWriteDsp( BOOL bWriteDsp )
{
    m_bWriteDsp = bWriteDsp;
}

BOOL CFileParser::IsWriteDsp( void )
{
    return m_bWriteDsp;
}

// ]]

void CFileParser::ClearProjectInfo()
{
    memset(m_szPrjName, 0, MAX_NAME_LENGTH);
    memset(m_szPrjFileName, 0, MAX_PATH);
    memset(m_szTargetFileName, 0, MAX_PATH); 
    memset(m_szPrjDesc, 0, /*MAX_ITEM_CONTENT*/MAX_ITEM_DESCPT);  
	m_agStrError.RemoveAll();
	m_mapIDs.clear();
	m_strErrID.Empty();
}

BOOL CFileParser::LoadModule2(TCHAR *szModuleFileName)
{
    BOOL  bStatus = TRUE;
    TCHAR  *pszLine;
    TCHAR  szString[MAX_PATH] = {0};
    BOOL  bHandleItem;
	
	m_bStaticFlag = FALSE;  // add for static nv
	
    if(NULL != m_hModuleFile)
    {
        //Notify that this file parser has loaded file 
        bStatus =  FALSE;
    }
    else
    {
        HANDLE hFile = ::CreateFile(szModuleFileName,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
        if(hFile == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }
        
        DWORD dwSizeHigh,dwSizeLow;
        dwSizeLow = ::GetFileSize(hFile,&dwSizeHigh);
        
        char* pOriFile = new char[dwSizeLow];
        ReadFile(hFile,pOriFile,dwSizeLow,&dwSizeHigh,NULL);
        CloseHandle(hFile);
        
        DWORD dwSize = dwSizeLow / 2;
        TCHAR* pDesFile = new TCHAR[dwSize];
        DWORD dwPos;
        
        int nLength = strlen(szKey);
        
        for(int i=0;i<(int)dwSize;i++)
        {
            dwPos = i * 2;
            pDesFile[i] = (TCHAR)(( ( pOriFile[dwPos] & 0xf ) << 4 ) | ( ( pOriFile[dwPos + 1] & 0xf0 ) >> 4 ));
            pDesFile[i] ^= szKey[i % nLength];
        }
        
        delete[] pOriFile;
        
#ifdef _DEBUG
		CString strTempFile = szModuleFileName;
		strTempFile += _T(".tmp" );
		FILE *pTempFile = ::_tfopen( strTempFile, _T("w") );
		if ( pTempFile != NULL )
		{
			::fwrite( pDesFile, dwSize, 1, pTempFile );
			::fclose( pTempFile );
		}
#endif // _DEBUG
        TCHAR szToken[] = _T("\n");
        pszLine = _tcstok(pDesFile,szToken);
        
        
        bHandleItem = FALSE;
        
        //Get Module attribute
        memset(&m_CurrModule, 0, sizeof(Module));
        _tcscpy(m_CurrModule.szModuleFileName , szModuleFileName);
        m_currModuleItemList = new ModuleItem_List;
        //m_currModuleItemList->SetTypeSize (sizeof(ModuleItem));
		
		UINT nParentID = 0;
		INT nParentIdx= -1;
        
        //pszLine = fgets(szLineContent, MAX_PATH, m_hModuleFile);
        while(NULL != pszLine)
        {
            //Remove the pre space
            while(' ' == *pszLine)
            {
                pszLine++;
            }
            //This line is comment
            if( ('#' != *pszLine) &&
                ('\n' != *pszLine))
            {
				szString[0] = '\0';					
                _stscanf(pszLine, _T("%[^=]"), szString);				
				_stscanf(szString, _T("%s"), szString);
                pszLine = _tcschr(pszLine, '=');
				if(pszLine != NULL) pszLine += 1;
				
				if(0 == (_tcscmp(szString, _T("MODULE"))))
				{
                    _stscanf(pszLine, _T("%s"),  m_CurrModule.szModuleName);
                }                    
                else if(0 == (_tcscmp(szString, _T("DESCRIPTION"))))
                {
					// modify for Joly
					_tcscpy(m_CurrModule.szModuleDescription, pszLine);
					//[[add for static nv
					_tcslwr(pszLine);
					if (NULL != _tcsstr(pszLine, _T("static")))
					{
						m_bStaticFlag = TRUE;
					}
					//]]
                }
                else if(0 == (_tcscmp(szString, _T("APPENDABLE"))))
                {
                    _stscanf(pszLine,_T("%d"),&m_CurrModule.bAppendable);
                }
                else if(0 == (_tcscmp(szString, _T("BEGIN_ITEM"))))
                {
                    memset(&m_CurrModuleItem, 0, sizeof(ModuleItem));
                    m_CurrModuleItem.bReadOnly = TRUE;
                }
                else if(0 == (_tcscmp(szString, _T("END_ITEM"))))
                {
					//[[ add by Joly for static Nv
					if (-1 == m_CurrModuleItem.nParentItemIndex && m_bStaticFlag)
					{
						m_mapStaticData.insert(CMapData::value_type(m_CurrModuleItem.nItemID, m_CurrModuleItem.szItemName));
					}
					//]]
					// @ HONGLIANG XIN, 2008-5-8
					if(m_CurrModuleItem.nParentItemIndex == 0)
					{
						nParentID = m_CurrModuleItem.nItemID;
						nParentIdx = m_CurrModuleItem.nItemIndex;
					}
					else if(m_CurrModuleItem.nParentItemIndex > 0)
					{
						if(m_CurrModuleItem.nParentItemIndex == nParentIdx &&
							m_CurrModuleItem.nItemID != nParentID)
						{
							CString str;
							str.Format(_T("[WARNING] [NVM: %s]: [ItemName: %s] [ItemID: %d] not equal to [ParentID: %d]"),GetStrFileTitle(szModuleFileName),
								m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemID,nParentID);
							//AfxMessageBox(str);
							m_agStrError.Add(str);
							
							// set the subitem ID to the parent ID
							m_CurrModuleItem.nItemID = nParentID;
						}
					}
					// check the item content
					if( m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE ||
						m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE || 
						m_CurrModuleItem.eItemType == LONG_ITEM_TYPE)
					{
						if(!ValueIsValid(m_CurrModuleItem.szItemContent))
						{
							CString str;
							str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]: item content [%s] is not valid."),GetStrFileTitle(szModuleFileName),
								m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemID,m_CurrModuleItem.szItemContent);
							//AfxMessageBox(str);
							m_agStrError.Add(str);
							bStatus = FALSE;
							m_CurrModuleItem.bChanged = TRUE;
							m_CurrModuleItem.bExpanded = TRUE;
						}
						if(m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE)
						{
							m_CurrModuleItem.nItemLength = 1;
						}
						else if(m_CurrModuleItem.eItemType == SHORT_ITEM_TYPE)
						{
							m_CurrModuleItem.nItemLength = 2;
						}
						else if(m_CurrModuleItem.eItemType == LONG_ITEM_TYPE)
						{
							m_CurrModuleItem.nItemLength = 4;
        				}
					}
					//]]
					
					BOOL bOK = TRUE;
					
					
					if(m_CurrModuleItem.nParentItemIndex >= (INT)m_CurrModuleItem.nItemIndex)
					{
						bOK = FALSE;
						bStatus = FALSE;
						CString str;
						str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent index [%d] is not smaller than its index [%d]."),GetStrFileTitle(szModuleFileName),
							m_CurrModuleItem.szItemName,m_CurrModuleItem.nParentItemIndex,m_CurrModuleItem.nItemIndex);							
						m_agStrError.Add(str);
						
					}
					
					if(bOK && m_CurrModuleItem.nItemIndex != (UINT)m_currModuleItemList->GetSize())
					{
						CString str;
						str.Format(_T("[WARNING] [NVM: %s] [ItemName: %s]:item index [%d] is not valid and fixed it as [%d]."),GetStrFileTitle(szModuleFileName),
							m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemIndex,m_currModuleItemList->GetSize());							
						m_agStrError.Add(str);
						m_CurrModuleItem.nItemIndex = m_currModuleItemList->GetSize();
					}
					
					
					if( bOK && m_CurrModuleItem.nParentItemIndex > -1 )
					{
						_ASSERTE (m_CurrModuleItem.nParentItemIndex < m_currModuleItemList->GetSize() );
						
						ModuleItem miParent = m_currModuleItemList->GetAt(m_CurrModuleItem.nParentItemIndex);
						if( miParent.eItemType !=  ARRAY_ITEM_TYPE &&  
							miParent.eItemType !=  ASTRING_ITEM_TYPE &&
							miParent.eItemType !=  WSTRING_ITEM_TYPE &&
							miParent.eItemType !=  STRUCT_ITEM_TYPE )
						{									
							bStatus = FALSE;
							CString str;
							str.Format(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent [%s] is not valid."),GetStrFileTitle(szModuleFileName),
								m_CurrModuleItem.szItemName,miParent.szItemName);							
							m_agStrError.Add(str);
						}
						
					}
					FilterContent(szModuleFileName);
                    m_currModuleItemList->Add(m_CurrModuleItem);
                    m_CurrModule.nItemCount += 1;
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_NAME"))))
                {
                    _stscanf(pszLine, _T("%s"),  m_CurrModuleItem.szItemName );
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_ID"))))
                {
                    _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nItemID );
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_TYPE"))))
                {
                    _stscanf(pszLine,_T("%d"),  &m_CurrModuleItem.eItemType  );
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_SIZE"))))
                {
                    _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nArraySize   );
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_LENGTH"))))
                {
                    _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nItemLength    );
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_CONTENT"))))
                {
                    _stscanf(pszLine, _T("%s"),  m_CurrModuleItem.szItemContent  );
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_INDEX"))))
                {
                    _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nItemIndex);
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_PARENT"))))
                {
                    _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nParentItemIndex );
                }
                else if(0 == (_tcscmp(szString, _T("ITEM_DESC"))))
                {
					int nLen = ::_tcslen(pszLine) + 1;
					
					if (nLen > 1)
					{
						m_CurrModuleItem.szItemDescription = (TCHAR*)malloc(nLen * sizeof(TCHAR));
						_tcscpy(m_CurrModuleItem.szItemDescription, pszLine);
						
					}
                }
                else if(0 == (_tcscmp(szString,_T("ITEM_READONLY"))))
                {
                    _stscanf(pszLine,_T("%d"),&m_CurrModuleItem.bReadOnly);
                }
            }
            
            //pszLine = fgets(szLineContent, MAX_PATH, m_hModuleFile);
            pszLine = _tcstok(NULL,szToken);
        }
        
        m_lstModule.Add (m_CurrModule);
        m_lstModuleItemPtr.Add (m_currModuleItemList);
        
        delete[] pDesFile;
    }
	
    //Close Project file
    if(NULL != m_hModuleFile)
    {
        fclose(m_hModuleFile);
        m_hModuleFile = NULL;
    }
	
    return bStatus;
	
}

void CFileParser::SetModuleAppendable(UINT uIndex,BOOL bAppend)
{
    if(uIndex >= m_nModuleCount)
    {
        return;
    }
	
	m_CurrModule = m_lstModule.GetAt (uIndex);
	m_CurrModule.bAppendable = bAppend;
	m_lstModule.SetAt(uIndex,m_CurrModule);
}

BOOL CFileParser::GetItemPath( int nModuleIndex,int nIndex,CString& strPath )
{
    ModuleItem item = GetModuleItem( nModuleIndex,nIndex );
    if( !strPath.IsEmpty() )
    {
        strPath.Insert( 0,'.' );
    }
    strPath.Insert( 0,item.szItemName );
	
    if( -1 != item.nParentItemIndex )
    {
        GetItemPath( nModuleIndex,item.nParentItemIndex,strPath );
    }
	
    return TRUE;
}

BOOL CFileParser::GetItemByName( CString strItemPath,int& nModuleIndex,int& nItemIndex )
{
	// Split item path by "\\"
	TCHAR szItemPath[256];
	_tcscpy( szItemPath,strItemPath );
	
	CStringArray strPaths;
	
	CString strPath = _tcstok( szItemPath,_T("\\") );
	
	while( !strPath.IsEmpty() )
	{
		strPaths.Add( strPath );
		strPath = _tcstok( NULL,_T("\\" ) );
	}
	
	//int nL = strPaths.GetSize();
	
	// Find module
	NVTreeItem* pModule = NULL;
	for( int i=0;i<m_TreeRoot.pChildren.GetSize();i++ )
	{
		pModule = m_TreeRoot.pChildren[i];
		
		if( pModule->strName.Compare( strPaths[0] ) )
		{
			continue;
		}
		
		// Find
		nModuleIndex = i;
		break;
	}
	
	if( i == m_TreeRoot.pChildren.GetSize() )
	{
		// Can not find module
		return FALSE;
	}
	
	// Find item in module
	
	nItemIndex = FindItem( pModule,&strPaths,1 );
	
	if( -1 != nItemIndex )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CFileParser::FindItem( NVTreeItem* pParent,CStringArray* pArrName,int nLevel )
{
	ASSERT( pParent );
	ASSERT( pArrName );
	if(pParent == NULL || pArrName == NULL)
	{
		return -1;
	}
	
	for( int i=0;i<pParent->pChildren.GetSize();i++ )
	{
		NVTreeItem* pItem = pParent->pChildren[i];
		if( pItem->strName.Compare( pArrName->GetAt( nLevel ) ) )
		{
			continue;
		}
		else
		{
			if( nLevel == pArrName->GetSize() - 1 )
			{
				// Find
				return pItem->nIndex;
			}
			else
			{
				// Find in child array
				return FindItem( pItem,pArrName,nLevel+1 );
			}
		}
	}
	
	return -1;
}

void CFileParser::ConstructItemTree()
{
	m_TreeRoot.pParent = NULL;
	
	// Construct module by module
	for( int i=0;i<GetModuleCount();i++ )
	{
		NVTreeItem* pModuleItem = new NVTreeItem;
		pModuleItem->pParent = &m_TreeRoot;
		pModuleItem->strName = GetModule(i).szModuleName;
		pModuleItem->nIndex = i;
		m_TreeRoot.pChildren.Add( pModuleItem );
		
		CArray< NVTreeItem*,NVTreeItem* > itemList;
		
		// Construct tree item by item in one module
		for( int j=0;j<GetModuleItemCount(i);j++ )
		{
			NVTreeItem* pItem = new NVTreeItem;
			ModuleItem mi = GetModuleItem( i,j );
			
			if( mi.nParentItemIndex == -1 )
			{
				// first level children
				pItem->pParent = pModuleItem;
				pModuleItem->pChildren.Add( pItem );
			}
			else
			{
				// lower level children
				NVTreeItem* pParent = itemList[mi.nParentItemIndex];
				pItem->pParent = pParent;
				pParent->pChildren.Add( pItem );
			}
			
			pItem->nIndex = j;
			pItem->strName = mi.szItemName;
			itemList.Add( pItem );
		}
	}
}

void CFileParser::ClearCalibrationFlag()
{
	TCHAR szCaliFlag[][MAX_PATH] = 
	{
		_T("GSM_CalibrationParam\\calibration_param_T\\adc\\adc_reserved[7]"),
		_T("CalibrationParam\\calibration_param_T\\adc\\adc_reserved[7]"),
		_T("WCDMA_CalibrationParam\\W_calibration_params\\w_cal_flag"),
		_T("LTE_CalibrationParam\\LTE_Cali_common\\LTE_CaliParam_flag"),
	};
	TCHAR szValue[MAX_PATH]= _T("0x0");
	ConstructItemTree();
	for (int i=0; i<4; ++i)
	{
		int nModuleIndex = 0,nItemIndex = 0;
		if(GetItemByName(szCaliFlag[i], nModuleIndex, nItemIndex))
		{
			ModuleItem mi = GetModuleItem(nModuleIndex, nItemIndex);
			if( ARRAY_ITEM_TYPE != mi.eItemType && 
				STRUCT_ITEM_TYPE != mi.eItemType &&
				ASTRING_ITEM_TYPE != mi.eItemType &&
				WSTRING_ITEM_TYPE != mi.eItemType )
			{
				mi.bChanged = _tcsicmp(mi.szItemContent, szValue);
				_tcscpy(mi.szItemContent, szValue);
				SetModuleItem(nModuleIndex,nItemIndex, mi);
			}
		 }
	}
	DestructItemTree();
	
}

BOOL CFileParser::ImportItem( CString strFile )	// used for command line mode only
{
	ConstructItemTree();
	
	BOOL bHasError = FALSE;
	
	try
	{
		CStdioFile file( strFile.GetBuffer(0),CFile::modeRead | CFile::typeText );
		CString strLine;
		CString strError = _T("");
		int nLineNo = 0;
		while( file.ReadString( strLine ) )
		{
			nLineNo++;
			strLine.TrimLeft();
			if( strLine.IsEmpty() )
			{
				// Blank line
				continue;
			}
			
			if( strLine.Find(_T("//"),0) == 0 || strLine.Find(_T("#"),0) == 0 )
			{
				// Comment line
				continue;
			}
			
			TCHAR szPath[256]={0},szValue[256]={0};
			_stscanf( strLine,_T("%[^'=']=%s"),szPath,szValue );
			
			/*CR212418 xmzhang add begin*/
			TCHAR *tmp = szPath;
			UINT lentmp = _tcslen(szPath)-1;
			tmp += (_tcslen(szPath)-1);
			//去除=号前的空格和制表符
			while((0x20 == *tmp||0x09 == *tmp)&&( lentmp > 0))
			{
				*tmp = 0x00;
				tmp--;
				lentmp--;
			}
			/*CR212418 xmzhang add end*/
			
			
			if( !ValueIsValid( szValue ) )
			{
				// szValue is not a valid number
				CString strText;
				strText.Format( _T( "LineNo: %d,%s has not a valid number!\n" ), nLineNo, szPath);// modify by Joly
				strError += strText;
				bHasError = TRUE;
				break; // 
				//continue;
			}
			
			int nModuleIndex,nItemIndex;
			BOOL bRet = GetItemByName( CString(szPath),nModuleIndex,nItemIndex );
			if( bRet )
			{
				ModuleItem mi = GetModuleItem( nModuleIndex,nItemIndex );
				if( ARRAY_ITEM_TYPE != mi.eItemType && 
					STRUCT_ITEM_TYPE != mi.eItemType &&
					ASTRING_ITEM_TYPE != mi.eItemType &&
					WSTRING_ITEM_TYPE != mi.eItemType )
				{
					mi.bChanged = _tcsicmp( mi.szItemContent,szValue );
					_tcscpy( mi.szItemContent,szValue );
					SetModuleItem( nModuleIndex,nItemIndex,mi );					
				}
				else
				{
					bHasError = TRUE;
					CString strText;
					strText.Format( _T( "%s %s\n" ),szPath,_T( "has child items,can not be imported!" ) );
					strError += strText;			
				}
			}
			else
			{
				// Could not find specified item
				bHasError = TRUE;				
				
				CString strText;
				strText.Format( _T( "%s %s\n" ),_T("Can not find item:"),szPath );
				strError += strText;
				
				
			}			
		}
		
		if(!strError.IsEmpty())
		{
			_tprintf(strError);
		}
	}
	catch( CFileException* e)
	{
		UNUSED_ALWAYS(e);
		// Error occurs
		bHasError = TRUE;
	}
	
	DestructItemTree();
	
	
	return !bHasError;
}

void CFileParser::DestructItemTree()
{
	for( int i=0;i<m_TreeRoot.pChildren.GetSize();i++ )
	{
		DestructSubTree( m_TreeRoot.pChildren.GetAt(i) );
	}
	
	m_TreeRoot.pChildren.RemoveAll();
}

void CFileParser::DestructSubTree( NVTreeItem* pRoot )
{
	ASSERT( pRoot );
	if(pRoot == NULL)
		return;
	
	for( int i=0;i<pRoot->pChildren.GetSize();i++ )
	{
		DestructSubTree( pRoot->pChildren[i] );
	}
	
	delete pRoot;
}

// [[ by Joly  
void CFileParser::SetHexFormat(BOOL bHexFlag)
{
	m_bHexFlag = bHexFlag;
}

//format content item
void CFileParser::FormatContent()
{
	TCHAR szContent[MAX_ITEM_CONTENT] = {0};
	CString strContent = (CString)m_CurrModuleItem.szItemContent;
	
	unsigned int nValue = 0;
	strContent.MakeUpper();
	if( 0 == strContent.Find(_T("0X")) )
	{
		_stscanf( strContent.operator LPCTSTR(), _T("0X%X"), &nValue );
	}
	else
	{
		_stscanf( strContent.operator LPCTSTR(), _T("%d"), &nValue );
	}
	if(m_bHexFlag)
	{
		_stprintf( szContent, _T("0x%X"), nValue );
	}
	else
	{
		_stprintf( szContent, _T("%u"), nValue );
	}
	
	_tcscpy( m_CurrModuleItem.szItemContent, szContent );
	
}

void CFileParser::SetMulFileAg(BOOL bWriteMul,VEC_FILEINFO & vecFileInfo)
{
	m_bWriteMult = bWriteMul;
	m_vecFileInfo.clear();
	m_nMulFileCount = vecFileInfo.size();
	m_vecFileInfo.assign(vecFileInfo.begin(), vecFileInfo.end());
	
	
}
BOOL CFileParser::IsExistedID(CUIntArray& uIntArray)
{		
	int nNumber = 0;
	for ( int i = 0; i < m_nMulFileCount; i++ )
	{
		if ( IsExistedID(m_vecFileInfo[i].nFileID) )
		{
			uIntArray.Add(m_vecFileInfo[i].nFileID);
			++nNumber;
		}		
	}
	if ( 0 == nNumber )
	{
		return FALSE;
	}
	return TRUE;
}
BOOL  CFileParser::IsExistedID(UINT nId)
{
	int nModuleItemCount = 0;
	for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
	{               
		m_currModuleItemList = m_lstModuleItemPtr.GetAt(nCurModuleIndex);
		nModuleItemCount = GetModuleItemCount(nCurModuleIndex);
		for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; nCurItemIndex++)
		{
			m_CurrModuleItem = m_currModuleItemList->GetAt(nCurItemIndex);
			if ( m_CurrModuleItem.nItemID == nId )
			{				
				return TRUE;
			}
		}
	}
	return FALSE;
}

UINT CFileParser::ReadOtherFiles(UINT& nNVImageLength)
{
	CString strMsg;
	for ( int i = 0; i < m_nMulFileCount; i++ )
	{
		ST_FILE_INFO sFileInfo = m_vecFileInfo[i];
		
		FILE *fpFileCode = _tfopen( sFileInfo.szFilePath, _T("rb") );
		if( NULL == fpFileCode )
		{
			CString strTmp;
			strTmp.Format(_T("Can't open the file %s, Please check the file is exist!\n"),sFileInfo.szFilePath);
			//Notify that can not open target file
			strMsg += strTmp;
			continue;
		}
		
		
		BYTE tmp_file_code[FILE_TOTAL_SIZE];
		int  nFileSize = 0;
		memset( tmp_file_code, 0, FILE_TOTAL_SIZE );
		
		fseek( fpFileCode, 0, SEEK_END);
		nFileSize = ftell(fpFileCode);
		
		if ( nFileSize >= FILE_TOTAL_SIZE )
		{
			CString strTmpMsg;
			strTmpMsg.Format( _T(" FileID %d (file size exceeded)\n "), sFileInfo.nFileID );
			strMsg += strTmpMsg;
			fclose(fpFileCode);
			continue;
		}
		
		fseek( fpFileCode, 0, SEEK_SET);  
		
		int nRead = fread( tmp_file_code, sizeof(char), FILE_TOTAL_SIZE, fpFileCode );
		
		if ( nRead <= 0 )
		{
			CString strTmpMsg;
			strTmpMsg.Format( _T(" FileID %d (read failed)\n "), sFileInfo.nFileID );
			strMsg += strTmpMsg;
			fclose(fpFileCode);
			continue;
		}
		fclose(fpFileCode);
		
		WORD  tmpSrcValue, tmpDstValue;
		
        tmpSrcValue = (WORD)sFileInfo.nFileID;
        CONVERT_SHORT2( tmpSrcValue, tmpDstValue );           
		*(WORD *)&m_szItemContent[nNVImageLength] = tmpDstValue ;
		
        tmpSrcValue = (WORD)nFileSize;
        CONVERT_SHORT2( tmpSrcValue, tmpDstValue );
		*(WORD *)&m_szItemContent[nNVImageLength + 2] = tmpDstValue ;
		
        nNVImageLength += 4;
		
		memcpy( m_szItemContent+nNVImageLength, tmp_file_code, nFileSize );
		// Little endian to big endian
        for(int Index = 0; Index < nFileSize; Index += 2 )
        {
			tmpSrcValue = *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index];    
			CONVERT_SHORT2( tmpSrcValue, tmpDstValue );
            *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index] = tmpDstValue;
		}
		nFileSize = RoundTo4Align(nFileSize);
		nNVImageLength += nFileSize;						
	}
	
	if ( !strMsg.IsEmpty() )
	{
		CString strErrorMsg;
		strErrorMsg.Format( _T("[ERROR] %s write failed! \n MaxFileSize:64KB"), strMsg);
		fprintf(stderr,"%s\n",strErrorMsg.operator LPCTSTR());
		
		return 1;
		
	}
	return 0;
	
}

void  CFileParser::SetWriteTail(BOOL nFlag)
{
	m_bTailFlag = nFlag;
}

void  CFileParser::SetVersionFlag(BOOL nFlag)
{
	m_bVerFlag = nFlag;
}

void CFileParser::GetStaticNVID(CMapData*& mapStaticNv)
{
	mapStaticNv = &m_mapStaticData;
	
}

// for fix bin merge
void CFileParser::SetWriteFix(TCHAR* szFixBin)
{
	if ( szFixBin && 0 < _tcslen(szFixBin))
	{
		m_bWriteFix = TRUE;
		_tcscpy(m_szFixBin, szFixBin);
	}
}

// for hash header path
void CFileParser::SetHashHeaderPath(TCHAR* szHash)
{
	if ( szHash && 0 < _tcslen(szHash))
	{
		m_bHashHeader = TRUE;
		_tcscpy(m_szHashPath, szHash);
	}
	
}

BOOL CFileParser::CompareHashValue(const unsigned char* pItem)
{
	//	memcpy(szOrgHash, (const char*)pItem + 4, 16);
	
	// 	AfxMessageBox(m_szPrjFileName);
	// 	TCHAR szPath[ MAX_PATH ];
	// 
	// 	TCHAR *pResult = _tcsrchr(m_szPrjFileName,_T('\\'));
	// 	if(pResult == NULL)
	// 	{
	// 		return S_FALSE;
	// 	}
	// 
	// 	CString strPath = _tcstok(m_szPrjFileName,_T("\\") );
	// 	AfxMessageBox(strPath);
	// 	AfxMessageBox(m_szPrjFileName);
	
	CountMD5(m_mapIDs);
	CString strOrg, strNew;
	for (int i = 0; i < 16; i++)
	{
		CString strTmp;
		strTmp.Format(_T("%02X"), pItem[i + 4]);
		strOrg += strTmp;
		
		strTmp.Format(_T("%02X"), m_digest[i]);
		strNew += strTmp;
	}
	
	if (0 == strOrg.CompareNoCase(strNew))
	{
		return TRUE;
	}
	return FALSE;
}

//[[ for repeated ID check
void  CFileParser::SetMapIDs(CMapID& mapIds)
{
	m_mapIDs = mapIds;
}

void  CFileParser::GetMapIDs(CMapID*& mapIds)
{
	mapIds = &m_mapIDs;
}

void  CFileParser::AnalyzeFixBin(const unsigned char* pvFile, DWORD& cbFileSize, CMapID& mapIDs)
{
	DWORD nOffset = 0;
	// Insert item into CFileParser
	// Skip time stamp( 4 bytes long )
    if(m_bVerFlag)
        nOffset += 44;
	
    //mapIDs = m_mapIDs;
	//	mapIDs.insert(m_mapIDs.begin(), m_mapIDs.end());
	
	static const WORD NV_END = 0xffff;
	while (nOffset < cbFileSize)
	{
		WORD wItemId = *(WORD*)(pvFile + nOffset);
		if (NV_END == wItemId)
		{
			cbFileSize = nOffset;
			break;
		}
		
		CONVERT_SHORT2(wItemId, wItemId);
		
		nOffset += sizeof(WORD);
		if (nOffset >= cbFileSize)
		{
			break;
		}
		
		WORD wLen = *(WORD*)(pvFile + nOffset);
		CONVERT_SHORT2(wLen, wLen);
		
		nOffset += sizeof(WORD);
		if (nOffset + wLen > cbFileSize)
		{
			break;
		}
		
		CMapID::iterator  it = mapIDs.find(wItemId);
		if (mapIDs.end() != it)
		{
			CString strTmp;
			strTmp.Format(_T("%d  "), wItemId);
			m_strErrID += strTmp;
		}
		else
		{
			mapIDs.insert(CMapID::value_type(wItemId, wLen));
		}
		
		nOffset += wLen;
		// Adjust for alignment of 4 bytes
		nOffset = (nOffset + 3) & 0xfffffffc;
		
	}
	
	
	
}

void  CFileParser::CountMD5(CMapID& mapIds)
{
	MD5_CTX context;	
	MD5Init (&context);
	
	CMapID::iterator iter;
	for (iter = mapIds.begin(); iter != mapIds.end(); ++iter)
	{		
		BYTE  tmpValue[8] = {0};
		int nSize = 0;
		
		sprintf((char*)tmpValue, "%d", iter->first);
		nSize = strlen((char*)tmpValue);
		MD5Update (&context, (unsigned char*)tmpValue, nSize);
		
		
		sprintf((char*)tmpValue, "%d", iter->second);
		nSize = strlen((char*)tmpValue);
		MD5Update (&context, (unsigned char*)tmpValue, nSize);
		
	}
	MD5Final (m_digest, &context);
}
//]]


//for CP update NV 
BOOL CFileParser::GenerateConfigFile(const TCHAR* strPath, TCHAR* strFileName, BOOL bHeaderFile)
{
	BOOL bRet = TRUE;
	CFileFind finder;
	CString strPathNv;
	CString strDetails;
	
	m_szNvContent = new BYTE[MAX_TOTAL_ITEM_CONTENT];
	memset(m_szNvContent, 0, MAX_TOTAL_ITEM_CONTENT);
	
	if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	ConstructItemTree();
	
	strPathNv.Format(_T("%s\\*.nv"), strPath);
	BOOL bExist = finder.FindFile(strPathNv);
	// add by 4/17
	std::vector<CString>  vecFileName, vecFilePath;
	
#define FILE_NAME_LENGTH 20
	while (bExist)
	{
		m_bNVFlag = FALSE;
		
		bExist = finder.FindNextFile();
		CString strFileNvi = finder.GetFilePath();
		
		CString strNamei = finder.GetFileName();
		//modify by 4/17
		vecFileName.push_back(strNamei);
		vecFilePath.push_back(strFileNvi);
		
	}
	std::sort(vecFileName.begin(), vecFileName.end());
	std::sort(vecFilePath.begin(), vecFilePath.end());
	int nCountFile = vecFileName.size();
	for (int j = 0; j < nCountFile; ++j)
	{
        CString strName = vecFileName[j];
		CString strFileNv = vecFilePath[j];
		
		TCHAR szToken[] = _T(".");
		strName = _tcstok(strName.GetBuffer(0), szToken);
		int nLen = WideCharToMultiByte(CP_ACP, 0, (unsigned short*)strName.GetBuffer(0), strName.GetLength(), NULL, 0, NULL, NULL);
		//	int nLen = strName.GetLength();
		char *pFileName = new char[nLen + 1];
		//	char pFileName[FILE_NAME_LENGTH];
		WideCharToMultiByte(CP_ACP, 0, (unsigned short*)strName.GetBuffer(0), strName.GetLength(), pFileName, nLen, NULL, NULL);
		pFileName[nLen] = '\0';
		//	pFileName = _tcstok(pFileName, szToken);
		
		memcpy(m_szNvContent + m_dwNVLength, pFileName, nLen);
		
		m_dwNVLength += FILE_NAME_LENGTH; 
		m_dwNVLength += 6;
		bRet = GenerateStruct(strFileNv);
		
		if (NULL != pFileName)
		{
			delete []pFileName;
			pFileName = NULL;
		}
		
		if (!bRet || !m_bNVFlag)
		{			
			if (!m_bNVFlag)
			{			
				_tprintf(_T("[ERROR] File: %s, No valid NV Flag"), strFileNv);
				
				// 				ReportError(strErr);
			}		
			ClearNVHeader();
			return FALSE;
		}
	}
	if (0 == m_dwNVLength)
	{
		// 		CString strErr(_T("Wrong Path or No valid NV File"));
		// 		ReportError(strErr);
		_tprintf(_T("[ERROR] Wrong Path or No valid NV File"));
		ClearNVHeader();
		return FALSE;
	}
	if (!bHeaderFile)
	{
		bRet = MergeModerm(strPath, strFileName);
		ClearNVHeader();
		return bRet;
	}
	
	CStdioFile file(strFileName, CFile::modeCreate | CFile::modeWrite);
	for (UINT i = 0; i < m_dwNVLength; i++)
	{
		CString strTmp;
		if (m_dwNVLength -1 == i)
		{
			strTmp.Format(_T("0X%02X"), m_szNvContent[i]);
			
		}
		else
		{
			strTmp.Format(_T("0X%02X,"), m_szNvContent[i]);
		}
		if (0 == (i + 1)%16)
		{
			strTmp += "\n";
		}
		m_strNv += strTmp;
	}
	file.WriteString(m_strNv);	
	file.Close();
	ClearNVHeader();
	return bRet;
}

// nv force update for merge to moderm.bin
BOOL CFileParser::MergeModerm(const TCHAR* strPath, TCHAR* strFileName)
{
	FILE*  fpTarget = NULL;
	if (0 == _tcslen(strFileName))
	{
		CString strPathFile;
		strPathFile.Format(_T("%s\\delta_nv.bin"), strPath);
		
		fpTarget = _tfopen(strPathFile, _T("wb"));
		if(NULL == fpTarget)
		{
			_tprintf(_T("[ERROR] Can't open file %s, write bin failed!"), strPathFile);
			
			return FALSE;
		}
		
		fwrite(m_szNvContent, 1, m_dwNVLength, fpTarget);	
		fclose(fpTarget);
		return TRUE;
	}
	// merge to moderm.bin
	static const char SEARCH_START_DATA[] = "###DELTA_NV_BEGIN$$$";
	static const char SEARCH_END_DATA[] = "###DELTA_NV_END$$$";
	
	fpTarget = _tfopen(strFileName, _T("rb"));
	if(NULL == fpTarget)
	{
		_tprintf(_T("[ERROR] Can't open file %s, write bin failed!"), strFileName);
		
		return FALSE;
	}
	
	fseek(fpTarget, 0, SEEK_END);
	DWORD lSize = ftell(fpTarget);
	
	
	BYTE* buff;
	buff = (BYTE*)malloc(lSize * sizeof(BYTE));
    memset(buff, 0, lSize * sizeof(BYTE));
	
    fseek(fpTarget, 0, SEEK_SET);
    fread(buff, 1, lSize, fpTarget);
	BYTE* pDataS = NULL;
	pDataS = std::search(buff, buff + lSize, SEARCH_START_DATA, SEARCH_START_DATA + strlen(SEARCH_START_DATA) - 1);
	BYTE* pDataE = NULL;
	pDataE = std::search(buff, buff + lSize, SEARCH_END_DATA, SEARCH_END_DATA + strlen(SEARCH_END_DATA) - 1);
	if (pDataS >= buff + lSize || pDataE >= buff + lSize)
	{
		_tprintf(_T("Not Found DetlaNV Mark in Moderm Bin!"));
		free(buff);
		return FALSE;
	}
	
	DWORD lBufflen = pDataE - pDataS - 32;
	if (lBufflen < m_dwNVLength)
	{
		_tprintf(_T("[ERROR] Morderm Bin Not Enough Memory!"));
		return FALSE;
	}
	memset(pDataS + 32,0,lBufflen);
	memcpy(pDataS + 32, m_szNvContent, m_dwNVLength);
	
	fclose(fpTarget);
	fpTarget = _tfopen(strFileName, _T("wb"));
	if(NULL == fpTarget)
	{
		_tprintf(_T("[ERROR] Can't open file %s, write bin failed!"), strFileName);
		
		return FALSE;
	}
	
	fwrite(buff, 1, lSize, fpTarget);
	
	fclose(fpTarget);
	
	free(buff);
	
	return TRUE;
	
}

void CFileParser::ClearNVHeader()
{
	DestructItemTree();
	//memset(m_szNvContent, 0, MAX_TOTAL_ITEM_CONTENT);
	free(m_szNvContent);
	m_szNvContent = NULL;
	m_bIsNVImag = FALSE;
	m_bNVFlag = FALSE;
	m_dwFileLength = 0;
	m_dwNVLength = 0;
	m_strNv.Empty();
}

BOOL CFileParser::GenerateStruct(CString& strName)
{
	BOOL bRet = TRUE;
	m_bIsNVImag = TRUE;
	bRet = ImportNV(strName);
	if (bRet)
	{
		//add end mark for single nv file
		BYTE strend[2]={0xff,0xff};	
		memcpy(m_szNvContent + m_dwNVLength, strend, 2);
		
		DWORD dwDstValue;
		m_dwNVLength += 2;
		
		// write single nv file's length
		DWORD dwTmpLength = m_dwNVLength - m_dwFileLength;
		
		CONVERT_INT2(dwTmpLength, dwDstValue);
		*(DWORD *)&m_szNvContent[m_dwFileLength + FILE_NAME_LENGTH + 2] = dwDstValue;
		m_dwFileLength = m_dwNVLength;
		
	}
	m_bIsNVImag = FALSE;
	return bRet;
	
}

BOOL CFileParser::SetNVBin(const ModuleItem & NewModuleItem, TCHAR* szValue)
{
	BOOL bRt = TRUE;
	switch(NewModuleItem.eItemType)
	{
	case CHAR_ITEM_TYPE:                
		{                         
			WORD tmpSrcValue, tmpDstValue;
			tmpSrcValue = (WORD)NewModuleItem.nItemID;
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			*(WORD *)&m_szNvContent[m_dwNVLength] = tmpDstValue;
			
			tmpSrcValue = (WORD)NewModuleItem.nOffset;
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szNvContent[m_dwNVLength + 2] = tmpDstValue;
			
			tmpSrcValue = sizeof(char);
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tmpDstValue;
			
			m_dwNVLength += 6;
			
			
			if((NULL != _tcschr(szValue, 'x')) ||
				(NULL != _tcschr(szValue, 'X')))
			{
				tmpDstValue  = (WORD)_tcstol(szValue, NULL, 16);
			}
			else
            {
				tmpDstValue = (WORD)_tcstol(szValue, NULL, 10);
			}   
			if (0XFF < tmpDstValue)
			{
				return FALSE;
			}
			m_szNvContent[m_dwNVLength] = (unsigned char)tmpDstValue;
		}
		m_dwNVLength += 1;
		break;
	case SHORT_ITEM_TYPE:
		{
			WORD  tmpSrcValue;
            WORD  tmpDstValue;
			
            tmpSrcValue = (WORD)NewModuleItem.nItemID;
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szNvContent[m_dwNVLength] = tmpDstValue ;
			
            tmpSrcValue = (WORD)NewModuleItem.nOffset;
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szNvContent[m_dwNVLength + 2] = tmpDstValue ;
			
			tmpSrcValue = sizeof(short);
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tmpDstValue;
			
			m_dwNVLength += 6;
			
			DWORD tmpSrc;
            if((NULL != _tcschr(szValue, 'x')) ||
				(NULL != _tcschr(szValue, 'X')))
			{
				tmpSrc  = _tcstol(szValue, NULL, 16);
            }
			else
			{
				tmpSrc  = _tcstol(szValue, NULL, 10);
			}
			if (0XFFFF < tmpSrc)
			{
				return FALSE;
			}
			CONVERT_SHORT2(((WORD)tmpSrc), tmpDstValue);
			*(WORD *)&m_szNvContent[m_dwNVLength] = tmpDstValue;                                  
		}
		m_dwNVLength += 2;
		break;
	case LONG_ITEM_TYPE:
		{            										
			WORD tmpSrcValue, tmpDstValue;						
			tmpSrcValue = (WORD)NewModuleItem.nItemID;					
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);					
			*(WORD *)&m_szNvContent[m_dwNVLength] = tmpDstValue;
			
			tmpSrcValue = (WORD)NewModuleItem.nOffset;					
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);					
			*(WORD *)&m_szNvContent[m_dwNVLength + 2] = tmpDstValue ; 
			
			tmpSrcValue = sizeof(long);
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tmpDstValue;
			
			m_dwNVLength += 6;
			
		}				
		DWORD  dwSrcValue ;                
		DWORD  dwDstValue ;						                  
		if((NULL != _tcschr(szValue, 'x')) ||                   
			(NULL != _tcschr(szValue, 'X')))                  
		{                 
			dwSrcValue  = _tcstoul(szValue, NULL, 16);       
		}                
		else                
		{                  
			dwSrcValue  = _tcstoul(szValue, NULL, 10);              
		}						
		
		if (0XFFFFFFFF < dwSrcValue)
		{
			return FALSE;
		}
		CONVERT_INT2(dwSrcValue, dwDstValue);							
		*(DWORD *)&m_szNvContent[m_dwNVLength] = dwDstValue;  
		m_dwNVLength += 4;
		break;  			
	default:
		bRt = FALSE;
		break;
		
    }     
    return bRt;
	
}

BOOL CFileParser::ImportNV(CString strFile)
{
	BOOL bHasError = FALSE;
	DWORD dwLength = m_dwNVLength - 6; // add by Joly
	
	try
	{
		CStdioFile file( strFile.GetBuffer(0),CFile::modeRead | CFile::typeText );
		CString strLine;
		CString strError = _T("");
		int nLineNo = 0;
		while( file.ReadString( strLine ) )
		{
			nLineNo++;
			strLine.TrimLeft();
			if( strLine.IsEmpty() )
			{
				// Blank line
				continue;
			}
			
			if( strLine.Find(_T("//"),0) == 0 || strLine.Find(_T("#"),0) == 0 )
			{
				// Comment line
				continue;
			}
			
			TCHAR szPath[256]={0},szValue[256]={0};
			_stscanf( strLine,_T("%[^'=']=%s"),szPath,szValue );
			
			/*CR212418 xmzhang add begin*/
			TCHAR *tmp = szPath;
			UINT lentmp = _tcslen(szPath)-1;
			tmp += (_tcslen(szPath)-1);
			//去除=号前的空格和制表符
			while((0x20 == *tmp||0x09 == *tmp)&&( lentmp > 0))
			{
				*tmp = 0x00;
				tmp--;
				lentmp--;
			}
			/*CR212418 xmzhang add end*/
			
			
			if( !ValueIsValid( szValue ) )
			{
				// szValue is not a valid number
				CString strText;
				strText.Format( _T( "FileName: %s,LineNo: %d,%s has not a valid number!\n" ), strFile, nLineNo, szPath);// modify by Joly
				strError += strText;
				bHasError = TRUE;
				break; // 
				//continue;
			}
			
			//[[ add for samsung nv update
			
			if (m_bIsNVImag && NULL != _tcsstr(szPath, _T("nv_ver_flag")))
			{
				DWORD  tmpSrcValue;
				WORD  tmpDstValue;
				
				if( ( NULL != _tcschr(szValue, 'x') ) ||
					( NULL != _tcschr(szValue, 'X') ) )
				{
					tmpSrcValue  = _tcstol(szValue, NULL, 16 );
				}
				else
				{
					tmpSrcValue  = _tcstol(szValue, NULL, 10 );
				}
				if (0XFFFF < tmpSrcValue)
				{
					CString strText;
					strText.Format( _T( "File: %s, %s : wrong flag value\n" ), strFile, szPath );
					strError += strText; 
					break;
				}
				CONVERT_SHORT2(((WORD)tmpSrcValue), tmpDstValue);
				*(WORD *)&m_szNvContent[dwLength] = tmpDstValue;  
				m_bNVFlag = TRUE;
				continue;				
			}
			//]]
			
			int nModuleIndex,nItemIndex;
			BOOL bRet = GetItemByName( CString(szPath),nModuleIndex,nItemIndex );
			if( bRet )
			{
				ModuleItem mi = GetModuleItem( nModuleIndex,nItemIndex );
				if( ARRAY_ITEM_TYPE != mi.eItemType && 
					STRUCT_ITEM_TYPE != mi.eItemType &&
					ASTRING_ITEM_TYPE != mi.eItemType &&
					WSTRING_ITEM_TYPE != mi.eItemType )
				{
					//[[ modify by for SamSung nv update
					if (m_bIsNVImag) 
					{
						if (!SetNVBin(mi, szValue))
						{
							CString strText;
							strText.Format(_T( "File:%s, Line: %d, %s wrong item value\n" ), strFile, nLineNo, szPath);
							strError += strText;
							break;
						}
						
					}
					else
					{
						mi.bChanged = _tcsicmp( mi.szItemContent,szValue );
						_tcscpy( mi.szItemContent,szValue );
						SetModuleItem( nModuleIndex,nItemIndex,mi );
					}
					//]]
					
				}
				else
				{
					bHasError = TRUE;
					CString strText;
					strText.Format( _T( "File: %s,Line: %d, %s %s\n" ), strFile, nLineNo, szPath,_T( "has child items,can not be imported!" ) );
					strError += strText;			
				}
			}
			else
			{
				// Could not find specified item
				bHasError = TRUE;				
				
				CString strText;
				strText.Format( _T( "%s File: %s, Line:%d, %s\n" ), _T("Can not find item:"), strFile, nLineNo, szPath );
				strError += strText;
				
				
			}			
		}
		
		if(!strError.IsEmpty())
		{
			
			_tprintf(strError);
		}
	}
	
	catch( CFileException* e)
	{
		UNUSED_ALWAYS(e);
		// Error occurs
		bHasError = TRUE;
	}
	return !bHasError;
	
}

BOOL CFileParser::CheckValue(TCHAR* strDes, TCHAR* szValue, ITEMTYPE_ENUM itemType)
{
	TCHAR    szStart;
	TCHAR    szEnd;
	CString  strMid;
	BOOL     bRight = FALSE;
	
	CString strTmpDes(strDes);
	
	strTmpDes.TrimLeft();
	strTmpDes.TrimRight();
	
	int nPos = strTmpDes.Find(_T(":"));
	
	if ((-1 != nPos) && (0 == strTmpDes.Left(nPos).CompareNoCase(_T("#value"))))
	{
		CString strRule = strTmpDes.Right(strTmpDes.GetLength() - nPos -1);
		
		// strRule at least more than 4, otherwise equal to no rules
		if (strRule.GetLength() < 5)
		{
			return TRUE;
		}
		
		szStart = *(strDes + nPos + 1);
		
		szEnd = *(strDes + strTmpDes.GetLength() - 1);
		
		strMid = strRule.Mid(1, strRule.GetLength() - 2);
		
		bRight = ParseRule(szStart, szEnd, strMid, szValue, itemType);
		return bRight;
	}
	return TRUE;
	
}

//不符合规则的不作检查，return true;
BOOL CFileParser::ParseRule(const TCHAR szStart, const TCHAR szEnd, CString& strMid, TCHAR* szValue, ITEMTYPE_ENUM itemType)
{
	BOOL bRet = TRUE;
	switch (szStart)
	{
	case '(':
		if (')' == szEnd)
		{
			bRet = IsRange(strMid, szValue, CLOSE_INTERVAL_TYPE, itemType);
		}
		else if (']' == szEnd)
		{
			bRet = IsRange(strMid, szValue, HALF_CLOSE_INTERVAL_TYPE, itemType);
		}
		break;
	case '[':
		if (')' == szEnd)
		{
			bRet = IsRange(strMid, szValue, HALF_OPEN_INTERVAL_TYPE, itemType);
		}
		else if (']' == szEnd)
		{
			bRet = IsRange(strMid, szValue, OPEN_INTERVAL_TYPE, itemType);
		}
		break;
	case '{':
		if ('}' == szEnd)
		{
			bRet = IsRange(strMid, szValue, SET_RANGE_TYPE, itemType);
		}
		break;
	default:
		break;
	}
	return bRet;
}

int CFileParser::SplitStr(LPCTSTR lpszStr,CStringArray &agStrs,TCHAR chSeparate /*= _T(',')*/)
{
	agStrs.RemoveAll();
    CString strSrc(lpszStr);
	
	int nLen = _tcslen(lpszStr);
	
	if(nLen == 0)
		return 0;
	
	LPTSTR  lpBuf  = (LPTSTR)strSrc.operator LPCTSTR();
	LPTSTR  lpFind = _tcschr(lpBuf, chSeparate);
	CString strTmp;
	while(lpFind != NULL)
	{
		*lpFind = _T('\0');
		strTmp = lpBuf;
		strTmp.TrimLeft();
		strTmp.TrimRight();
		if(!strTmp.IsEmpty())
			agStrs.Add(strTmp);
		lpBuf = lpFind + 1;
		lpFind = _tcschr(lpBuf, chSeparate);
	}
	
	strTmp = lpBuf;
	strTmp.TrimLeft();
	strTmp.TrimRight();
	if(!strTmp.IsEmpty())
		agStrs.Add(strTmp);
	
	return agStrs.GetSize();
}

BOOL  CFileParser::IsRange(CString& strMid, TCHAR* szValue, ITEMDES_ENUM desType, ITEMTYPE_ENUM itemType)
{
	BOOL bRet = FALSE;
	
	int nValue = -1;
	if (SHORT_ITEM_TYPE == itemType || LONG_ITEM_TYPE ==itemType || CHAR_ITEM_TYPE == itemType)
	{
		nValue = GetContent(szValue);
	}
	
	CStringArray strArray;
	SplitStr(strMid, strArray);
	if (SET_RANGE_TYPE == desType)
	{
		for (int i = 0; i < strArray.GetSize(); i++)
		{
			if (SHORT_ITEM_TYPE == itemType || LONG_ITEM_TYPE == itemType)
			{
				int nRange = GetContent(strArray[i].GetBuffer(0));
				strArray[i].ReleaseBuffer(0);
				if (nRange == nValue)
				{
					return TRUE;
				}
			}
			else if (CHAR_ITEM_TYPE == itemType)
			{
				strArray[i].TrimRight(_T("\'"));
				strArray[i].TrimLeft(_T("\'"));
				if (1 == strArray[i].GetLength())
				{
					TCHAR  szChar[1] = {0};
					memcpy((void*)szChar, strArray[i].GetBuffer(0), 1);
					if (szChar[0] == nValue)
					{
						return TRUE;
					}
				}
				
				
			}
			else
			{
				strArray[i].TrimRight(_T("\""));
				strArray[i].TrimLeft(_T("\""));
				if (0 == strArray[i].CompareNoCase(szValue))
				{
					return TRUE;
				}
				
			}
			
		}
	}
	else
	{
		// 不符合规则
		if (2 != strArray.GetSize())
		{
			return TRUE;
		}
		int nLeft =0, nRight = 0;
		nLeft = GetContent(strArray[0].GetBuffer(0));
		nRight = GetContent(strArray[1].GetBuffer(0));
		
		strArray[0].ReleaseBuffer(0);
		strArray[1].ReleaseBuffer(0);
		
		switch(desType)
		{
		case CLOSE_INTERVAL_TYPE:
			if (nValue > nLeft && nValue < nRight)
			{
				return TRUE;
			}
			break;
		case HALF_CLOSE_INTERVAL_TYPE:
			if (nValue > nLeft && nValue <= nRight)
			{
				return TRUE;
			}
			break;
		case HALF_OPEN_INTERVAL_TYPE:
			if (nValue >= nLeft && nValue < nRight)
			{
				return TRUE;
			}
			break;
		case  OPEN_INTERVAL_TYPE:
			if (nValue >= nLeft && nValue <= nRight)
			{
				return TRUE;
			}
			break;
		default:
			break;
		}	
	}
	return bRet;
}

int CFileParser::GetContent(TCHAR* szContent)
{
	//	TCHAR szContent[MAX_ITEM_CONTENT] = {0};
	CString strContent(szContent);
	
	int nValue = 0;
	strContent.MakeUpper();
	if( 0 == strContent.Find(_T("0X")) )
	{
		_stscanf(strContent.operator LPCTSTR(), _T("0X%X"), &nValue);
	}
	else
	{
		_stscanf(strContent.operator LPCTSTR(), _T("%d"), &nValue);
	}
	return nValue;
}
// ]]