// FileParser.cpp: implementation of the CFileParser class.
//
//////////////////////////////////////////////////////////////////////
#include "FileParser.h"  
#include <assert.h>
#include <time.h>
#include <dirent.h>
#include <algorithm>
#include <errno.h>
#include "TCharOpr.h"
#include "GenDeltaNVIndex.h"

extern "C"
{
#include "md5.h"
};

//#include "msxml.h"/*xml file support by xmzhang add*/
//#import  "msxml2.dll"/*xml file support by xmzhang add*/

#ifdef WIN32
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#else
#include <string.h>
#define stricmp strcasecmp
#define _tcsicmp strcasecmp

char* strlower(char* p)
{
    char* p1 = p;
    while ('\0' != (*p1))
    {
        *p1 = (char)tolower(*p1);
        p1++;
    }
    return p;
}

#define strlwr  strlower
#define _tcslwr strlower

#endif  //WIN32


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
BOOL ValueIsValid(TCHAR* lpszValue)
{
	assert(lpszValue);

	TCHAR szTmp[MAX_PATH] = {0};
	_tcscpy(szTmp, lpszValue);
	_tcslwr(szTmp);

	int nLen = _tcslen(szTmp);
	if (nLen>=2 && szTmp[0]== _T( '\"' ))
	{
		//ASTRING ,such as "*******"
		if (szTmp[nLen-1] ==_T('\"'))
		{
			return TRUE;
		}
	}

    if(nLen>=2 && 0 == _tcsncmp(szTmp, _T( "0x" ), 2))
    {
    	 // hex,it must only include digits and a,b,c,d,e,f
        for (int i = 2; i < nLen; i++)
        {
            if (szTmp[i] < '0' || szTmp[i] > '9')
            {
                if (szTmp[i] < 'a' || szTmp[i] > 'f')
                    return FALSE;
            }
        }
        return TRUE;
    }
   else	
    {
        // decimal,it must only include digits
        for (int i = 0; i < nLen; i++)
        {
            if (szTmp[i] <'0' || szTmp[i] > '9')
                return FALSE;
        }
        return TRUE;
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
    SetModified(FALSE);
    // ]]
    //  m_szappend = NULL;/*xml file support by xmzhang add*/

    //[[ by Joly
    m_bWriteMult = FALSE;
    m_nMulFileCount = 0;
    m_bHexFlag = TRUE;
    m_bTailFlag = TRUE;
    memset(m_szNvContent, 0, MAX_TOTAL_ITEM_CONTENT);
    m_bIsNVImag = FALSE;
    m_bNVFlag = FALSE;
    m_dwNVLength = 0;
    m_dwFileLength = 0;
    m_bWriteFix = FALSE;
    m_bHashHeader = FALSE;
    m_bVerFlag = FALSE;
    m_strErrID = _T("");
    //]]
}


CFileParser::~CFileParser()
{
    try
    {
        //Destory Item array object
        for(UINT Index = 0; Index < m_nModuleCount; Index++)
        {
            m_currModuleItemList = m_lstModuleItemPtr[Index];
            delete m_currModuleItemList;
        }
    }
    catch(...)
    {

    }
   m_vecFileInfo.clear();

}

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

    TCHAR szError[MAX_PATH * 2] = {0};
    _tcscpy(m_szPrjName, szPrjName);
    _tcscpy(m_szPrjFileName, szPrjFileName);
    if(NULL != szTargetFileName)
    {           
        _tcscpy(m_szTargetFileName, szTargetFileName);
    }
    else
    {
        _tcscpy(m_szTargetFileName, _T(""));
    }

    if(NULL != szPrjDesc)
    {           
        _tcscpy(m_szPrjDesc, szPrjDesc);
    }
    else
    {
        _tcscpy(m_szPrjDesc, _T(""));
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
        m_hPrjFile = OpenFileForWrite(m_szPrjFileName, &bReadOnly);

        if (NULL == m_hPrjFile)
        {
            //Notify that open file failed
            //CString strErr;
            _tprintf( _T("Can't open file %s!\n"), m_szPrjFileName );
            _tprintf(_T("%s"), szError);

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
        if (bReadOnly)
        {
            // Restore the file's read-only attribute
            ChangeFileRdOnlyAttri(m_szPrjFileName, TRUE);
        }
        // ]]
    }

    // @ZHANG Ming : 2003-05-06
    if (bReturnStatus)
    {
        // a new project has been created
        SetModified(FALSE);
    }

    return bReturnStatus;	
}								

BOOL CFileParser::SaveProject()
{
    BOOL  bReturnStatus = TRUE;
    // @ZHANG Ming : 2003-04-18
    // [[
    BOOL bReadOnly = FALSE;
    // ]]

    TCHAR szError[MAX_PATH * 2] = {0};  

    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }

    // @ZHANG Ming : 2003-04-18
    // [[
    m_hPrjFile = OpenFileForWrite(m_szPrjFileName, &bReadOnly );
    if(NULL == m_hPrjFile)
    {
        //Notify that open file failed
        _stprintf(szError, _T("Can't open file %s!"), m_szPrjFileName);
        _tprintf(_T("%s"), szError);
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
#ifdef WIN32    	  
        TCHAR *pResult = _tcsrchr(m_szTargetFileName,_T('\\'));
#else
        TCHAR *pResult = _tcsrchr(m_szTargetFileName,_T('/'));
#endif
        if(pResult == NULL)
        {
            return FALSE;
        }
        pResult = pResult + 1;

        _ftprintf(m_hPrjFile, _T("  TARGET      =  %s\n"), pResult);
		CTCharOpr::StrRTrim(m_szPrjDesc,_T('\n'));
		CTCharOpr::StrRTrim(m_szPrjDesc,_T('\r'));
		
        _ftprintf(m_hPrjFile, _T("  DESCRIPTION =  %s\n"), m_szPrjDesc);

        _ftprintf(m_hPrjFile, _T("\n%s\n"),
            _T("################################################################################"));

        _ftprintf(m_hPrjFile, _T("\n%s\n"),_T("# Begin Project Module"));
        _ftprintf(m_hPrjFile, _T("\n"));

        //write module list part
        for(UINT Index = 0; Index < m_nModuleCount; Index++)
        {
            m_CurrModule = m_lstModule[Index];

            //Remove the path info
#ifdef WIN32    	  	
            TCHAR *pResult1 = _tcsrchr(m_CurrModule.szModuleFileName,_T('\\'));
#else
            TCHAR *pResult1 = _tcsrchr(m_CurrModule.szModuleFileName,_T('/'));
#endif
            if(pResult1 == NULL)
            {
                return FALSE;
            }
            pResult1 = pResult1 + 1;
            {
                SaveModule(Index);
            }

            _ftprintf(m_hPrjFile, _T("  MODULE     =  %s\n"), pResult1);
            //wc.Restore();

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
            ChangeFileRdOnlyAttri(m_szPrjFileName, TRUE);

        }
        // ]]
    }

    if (bReturnStatus)
    {
        // Save project successfully
        SetModified(FALSE);
    }

    return bReturnStatus;
}

BOOL CFileParser::SaveModule(INT nModuleIndex)
{
    //INT  bReturnCode = INVALID_MODULE_ID;
    FILE *fpFile = NULL;
    // @ZHANG Ming : 2003-04-18
    // [[
    BOOL bReadOnly;

    TCHAR szError[MAX_PATH * 2] = {0};

    fpFile = OpenFileForWrite( m_CurrModule.szModuleFileName, &bReadOnly );
    if( NULL == fpFile )
    {
        //Notify that open file failed
		_tprintf( _T("Can't open file %s!\n"), m_CurrModule.szModuleFileName );
        _tprintf(_T("%s"), szError);
        // ]]
        return FALSE;
    }
    else
    {      

        _ftprintf(fpFile, _T("%s\n%s\n\n%s\n"),
            _T("# SpreadTrum NVEditor NV Module File, Format Version 1.0"),
            _T("# WARNING: DO NOT EDIT OR DELETE THIS NV Module FILE!"),
            _T("###############################################################################"));           

        _ftprintf(fpFile, _T("  MODULE      = %s\n"), m_CurrModule.szModuleName );
		CTCharOpr::StrLTrim(m_CurrModule.szModuleDescription,_T('\"'));
		CTCharOpr::StrRTrim(m_CurrModule.szModuleDescription,_T('\"'));
		CTCharOpr::StrRTrim(m_CurrModule.szModuleDescription,_T('\n'));
		CTCharOpr::StrRTrim(m_CurrModule.szModuleDescription,_T('\r'));
        _ftprintf(fpFile, _T("  DESCRIPTION = %s\n"), m_CurrModule.szModuleDescription);

        _ftprintf(fpFile, _T("\n%s\n"),
            _T("################################################################################"));

        _ftprintf(fpFile, _T("\n%s\n"),_T("# Begin Module Item"));

        m_currModuleItemList = m_lstModuleItemPtr.at (nModuleIndex);
        m_CurrModule.nItemCount  = m_currModuleItemList->size();

        for(UINT Index = 0; Index < m_CurrModule.nItemCount; Index++)
        {       
            m_CurrModuleItem = m_currModuleItemList->at(Index);

            _ftprintf(fpFile, _T("  BEGIN_ITEM\n"));

            _ftprintf(fpFile, _T("    ITEM_NAME    = %s\n"), m_CurrModuleItem.szItemName  );
            _ftprintf(fpFile, _T("    ITEM_ID      = %d\n"), m_CurrModuleItem.nItemID );            
            _ftprintf(fpFile, _T("    ITEM_TYPE    = %d (%s)\n"), m_CurrModuleItem.eItemType, szItemType[m_CurrModuleItem.eItemType ] );
            if(ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType ||
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
			if (0  != _tcslen(m_CurrModuleItem.szItemDescription))
			{
				_ftprintf(fpFile, _T("    ITEM_DESC    = %s\n"), m_CurrModuleItem.szItemDescription  );
			}
			else
			{
				_ftprintf(fpFile, _T("    ITEM_DESC    =  \n"));
			}
            if(m_CurrModuleItem.bReadOnly)
            {
                _ftprintf(fpFile, _T("    ITEM_READONLY = %d\n"), m_CurrModuleItem.bReadOnly  );
            }

            _ftprintf(fpFile, _T("  END_ITEM\n\n"));
        }

        _ftprintf(fpFile, _T("\n%s\n\n%s\n"),
            _T("# End Module Item"),
            _T("################################################################################"));


        fclose(fpFile);
        fpFile = NULL;
        // @ZHANG Ming : 2003-04-18
        // [[       
        if (bReadOnly)
        {
            // Restore the file's read-only attribute
            ChangeFileRdOnlyAttri( m_CurrModule.szModuleFileName, TRUE );
        }
        // ]]
    }

    return TRUE;
}

BOOL CFileParser::LoadProject(TCHAR *szPrjFileName,BOOL bCheckItemName /*= TRUE*/)
{
    BOOL  bStatus = TRUE;
    TCHAR  szLineContent[MAX_PATH];
    TCHAR  *pszLine;
	TCHAR  szString[MAX_PATH];

    int nVer = 1;

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
            _tprintf("can not open project file.\n");
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

            if (NULL == pszLine)
            {

                fclose(m_hPrjFile);
                _tprintf(_T("Invalid project file or the file is corrupted\n"));
                return FALSE;
            }

            if (_tcsstr( pszLine, szVersion2 ) != NULL)
            {				
                nVer = 2;    
            }
            else if ( ::_tcsstr( pszLine, szVersion1 ) == NULL )
            {		
                _tprintf(_T("Invalid .prj file or the file corrupted"));

                fclose( m_hPrjFile );
                return FALSE;
            }

            while(NULL != pszLine)
            {
                int tempVer=nVer;/*xml file support by xmzhang add*/

                //		_tprintf(_T("line: %s"), pszLine);
                //Remove the pre space
                while(' ' == *pszLine || '\t' == *pszLine)
                {
                    pszLine++;
                }
                //This line is comment
                if(('#' != *pszLine) &&
                    ('\n' != *pszLine) && ('\r' != *pszLine))
                {
                    _stscanf(pszLine, _T("%s"), szString);
                    pszLine = _tcschr(pszLine, '=');
                    pszLine += 1;

                    //Remove the pre space
                    while(' ' == *pszLine || '\t' == *pszLine)
                    {
                        pszLine++;
                    }
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

                        TCHAR *pResult = _tcsrchr(szLineContent,'/');
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

                        TCHAR *pResult = _tcsrchr(szLineContent, '/');
                        if(pResult == NULL)
                        {
                            return FALSE;
                        }

                        *(pResult + 1) = 0;


                        _tcscat(szLineContent, szString);

                        if(nVer == 1)
                        {
                            //	_tprintf(_T("Load module: \"%s\" ...\n"), szLineContent);
                            if(LoadModule(szLineContent))
                            {
                                //		_tprintf(_T("Load module: \"%s\" success\n"), szLineContent);
                                m_nModuleCount++;
                            }
                            // [[ modify by Joly
                            else 
                            {
                                    _tprintf(_T("Load module: \"%s\" failed\n"), szLineContent);
                                return FALSE;
                            }
                        }
                        else if(nVer == 2)
                        {
                            //	_tprintf(_T("Load module: \"%s\" ...\n"), szLineContent);
                            if(LoadModule2(szLineContent))
                            {
                                //		_tprintf(_T("Load module: \"%s\" success\n"), szLineContent);
                                m_nModuleCount++;
                            }
                            // [[ modify by Joly
                            else
                            {
                                    _tprintf(_T("Load module: \"%s\" failed\n"), szLineContent);
                                return FALSE;
                            }
                        }
                        /*xml file support by xmzhang add begin
                        else if(nVer == 3)
                        {
                        _tprintf(_T("Load module: \"%s\" ...\n"), szLineContent);
                        if(LoadModuleXML(szLineContent))
                        {
                        _tprintf(_T("Load module: \"%s\" success\n"), szLineContent);
                        m_nModuleCount++;
                        }
                        //[[ modify by Joly
                        else 
                        {
                        _tprintf(_T("Load module: \"%s\" failed\n"), szLineContent);
                        return FALSE;
                        }
                        }
                        //xml file support by xmzhang add end*/
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
    //[[ for NV version check by MD5
    CountMD5(m_mapIDs);
    //]]

    if (!m_strErrID.empty())
    {
        _tprintf(_T("Exist repeated IDs: %s\n"), m_strErrID.c_str());
        return FALSE;
        //	m_agStrError.Add(strTmp);
    }	
    // @ZHANG Ming : 2003-05-06
    if (bStatus)
    {
        SetModified(FALSE);
	if(bCheckItemName)
	{
		CheckNVModule();
		CheckModuleItem();
	}
    }

    return bStatus;
}

BOOL CFileParser::LoadModule(TCHAR *szModuleFileName)
{
    BOOL  bStatus = TRUE;
    TCHAR  szLineContent[MAX_PATH];
    TCHAR  *pszLine;
    TCHAR  szString[MAX_PATH];
    BOOL  bHandleItem;


    if(NULL != m_hModuleFile)
    {
        //Notify that this file parser has loaded file 
        bStatus =  FALSE;
    }
    else
    {
        TCHAR szModuleFileNameTmp[MAX_PATH] = {0};
        _tcscpy(szModuleFileNameTmp, szModuleFileName);
        for (unsigned int n = 0; n < _tcslen(szModuleFileNameTmp); n++)
        {
            if ('\\' == szModuleFileNameTmp[n])
            {
                szModuleFileNameTmp[n] = '/';
            }
        }
        m_hModuleFile = _tfopen(szModuleFileNameTmp, _T("rt"));
        if(NULL == m_hModuleFile)
        {
            //The given file is not exist then create it
            _tprintf(_T("open module file failed\n"));
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
                while(' ' == *pszLine || '\t' == *pszLine)
                {
                    pszLine++;
                }
                //This line is comment
                if( ('#' != *pszLine) &&
                    ('\n' != *pszLine) && ('\r' != *pszLine))
                {
                    szString[0] = '\0';					
                    _stscanf(pszLine, _T("%[^=]"), szString);				
                    _stscanf(szString, _T("%s"), szString);
                    pszLine = _tcschr(pszLine, '=');
                    if(pszLine != NULL) 
                    {
                        pszLine += 1;
                        //Remove the pre space
                        while(' ' == *pszLine || '\t' == *pszLine)
                        {
                            pszLine++;
                        }
                    }

                    if(0 == (_tcscmp(szString, _T("MODULE"))))
                    {
                        _stscanf(pszLine, _T("%s"),  m_CurrModule.szModuleName);
                    }                    
                    else if(0 == (_tcscmp(szString, _T("DESCRIPTION"))))
                    {
                        if(pszLine && 0 !=_tcslen(pszLine))
                        {
							_tcscpy(m_CurrModule.szModuleDescription, pszLine);
                        }
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
                                //string strTmp;
                                TCHAR strTmp[8];
                                _stprintf((char*)strTmp, _T("%d "), m_CurrModuleItem.nItemID);
                                m_strErrID.append(strTmp);
                                //m_strErrID += strTmp;
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

                                _tprintf(_T("[WARNING] [NVM: %s]: [ItemName: %s] [ItemID: %d] not equal to [ParentID: %d] and fixed."),szModuleFileName,
                                    m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemID,nParentID);


                                m_CurrModuleItem.nItemID = nParentID;
                            }
                        }

                        if( m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE ||
                            m_CurrModuleItem.eItemType == SHORT_ITEM_TYPE || 
                            m_CurrModuleItem.eItemType == LONG_ITEM_TYPE)
                        {
                            if(!ValueIsValid(m_CurrModuleItem.szItemContent))
                            {
                                _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item content [%s] is not valid."),szModuleFileName,
                                    m_CurrModuleItem.szItemName,m_CurrModuleItem.szItemContent);
                                //AfxMessageBox(str);

                                bStatus = FALSE;
                                m_CurrModuleItem.bChanged = TRUE;
                                m_CurrModuleItem.bExpanded = TRUE;

                                m_CurrModule.bExpanded = TRUE;

                                int nPIdx = m_CurrModuleItem.nParentItemIndex;
                                while (nPIdx != -1)
                                {
                                    ModuleItem mi = m_currModuleItemList->at(nPIdx);
                                    mi.bExpanded = TRUE;
                                    (*m_currModuleItemList)[nPIdx] = mi;
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
                            _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent index [%d] is not smaller than its index [%d]."),szModuleFileName,
                                m_CurrModuleItem.szItemName,m_CurrModuleItem.nParentItemIndex,m_CurrModuleItem.nItemIndex);							


                        }

                        if (bOK && m_CurrModuleItem.nItemIndex != (UINT)m_currModuleItemList->size())
                        {
                            _tprintf(_T("[WARNING] [NVM: %s] [ItemName: %s]:item index [%d] is not valid and fixed it as [%d]."),szModuleFileName,
                                m_CurrModuleItem.szItemName, m_CurrModuleItem.nItemIndex,m_currModuleItemList->size());							

                            m_CurrModuleItem.nItemIndex = m_currModuleItemList->size();
                        }


                        if (bOK && m_CurrModuleItem.nParentItemIndex > -1)
                        {
                            assert(m_CurrModuleItem.nParentItemIndex < (int)m_currModuleItemList->size() );

                            ModuleItem miParent = m_currModuleItemList->at(m_CurrModuleItem.nParentItemIndex);
                            if( miParent.eItemType !=  ARRAY_ITEM_TYPE &&  
                                miParent.eItemType !=  ASTRING_ITEM_TYPE &&
                                miParent.eItemType !=  WSTRING_ITEM_TYPE &&
                                miParent.eItemType !=  STRUCT_ITEM_TYPE )
                            {									
                                bStatus = FALSE;
                                _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent [%s] is not valid."),szModuleFileName,
                                    m_CurrModuleItem.szItemName,miParent.szItemName);							

                            }

                        }
						FilterContent(szModuleFileName);
                        m_currModuleItemList->push_back(m_CurrModuleItem);
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
                            _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item ID [%s] is not valid."),szModuleFileName,
                                m_CurrModuleItem.szItemName, pszLine);							

                        }
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_TYPE"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &(m_CurrModuleItem.eItemType)  );
                    }
                    else if(0 == (_tcscmp(szString, _T("ITEM_SIZE"))))
                    {
                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nArraySize   );
                    }
                    /*else if(0 == (_tcscmp(szString, _T("ITEM_LENGTH"))))
	  	                    {
	  	                        _stscanf(pszLine, _T("%d"),  &m_CurrModuleItem.nItemLength);
	  	                        if (m_CurrModuleItem.nItemLength > 0xFFFF || 0 == m_CurrModuleItem.nItemLength)
	  	                        {
	  	                            _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item length [%s] is not valid."),szModuleFileName,
	  	                                m_CurrModuleItem.szItemName, pszLine);							
	  
	  	                        }
	  	                    }*/
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
                    	string strLine(pszLine);
                    	CTCharOpr::StrLTrim((TCHAR*)strLine.c_str());	
						CTCharOpr::StrRTrim((TCHAR*)strLine.c_str(),_T('\n'),FALSE);
						CTCharOpr::StrRTrim((TCHAR*)strLine.c_str(),_T('\r'),FALSE);
                        // erase redundant string : (null) 
                        
                        if ( 
							0 == _tcslen(strLine.c_str())					||
							0 == _tcsnicmp(strLine.c_str(), _T("(null)"),6) || 
							0 == _tcsicmp(strLine.c_str(),_T("\n")) )
                        {
                            continue;
                        }
                        _tcsncpy(m_CurrModuleItem.szItemDescription, strLine.c_str(),MAX_ITEM_CONTENT);
                    }
                    else if(0 == (_tcscmp(szString,_T("ITEM_READONLY"))))
                    {
                        _stscanf(pszLine,_T("%d"),&m_CurrModuleItem.bReadOnly);
                    }
                }

                pszLine = _fgetts(szLineContent, MAX_PATH, m_hModuleFile);
            }

            m_lstModule.push_back(m_CurrModule);
            m_lstModuleItemPtr.push_back(m_currModuleItemList);
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
			string strItemPath = m_CurrModuleItem.szItemName;
			strItemPath.insert(0,_T("\\"));
			GetNVItemPath(m_CurrModule.szModuleName,m_CurrModuleItem.nParentItemIndex,strItemPath);
			_tprintf(_T("[WARNING] [NVM: %s] [ItemName: %s]:item value %s[char] is out of bounds.\n"),
				szModuleFileName,
				strItemPath.c_str(),
				m_CurrModuleItem.szItemContent);			
		}
		
		break;
	case SHORT_ITEM_TYPE:
		if (0XFFFF < dwDstValue)
		{
			string strItemPath = m_CurrModuleItem.szItemName;
			strItemPath.insert(0,_T("\\"));
			GetNVItemPath(m_CurrModule.szModuleName,m_CurrModuleItem.nParentItemIndex,strItemPath);
			_tprintf(_T("[WARNING] [NVM: %s] [ItemName: %s]:item value %s[short] is out of bounds.\n"),
				szModuleFileName,
				strItemPath.c_str(),
				m_CurrModuleItem.szItemContent);		
		}
		break;
	case LONG_ITEM_TYPE:
		if (0XFFFFFFFF < dwDstValue)
		{
			string strItemPath = m_CurrModuleItem.szItemName;
			strItemPath.insert(0,_T("\\"));
			GetNVItemPath(m_CurrModule.szModuleName,m_CurrModuleItem.nParentItemIndex,strItemPath);

			_tprintf(_T("[WARNING] [NVM: %s] [ItemName: %s]:item value %s[long] is out of bounds.\n"),
				szModuleFileName,
				strItemPath.c_str(),
				m_CurrModuleItem.szItemContent);		
			
		}
		break;
	default:
		break;
	}
}
void CFileParser::CheckNVModule()
{
	int nModuleCount = m_lstModule.size();
	std::vector<string> agModule;
	int nAddIndex = 0;
	for (int i = 0; i < nModuleCount; ++i)
	{
		Module  tmpModule = m_lstModule[i];
		BOOL bExistModule = FALSE;
		for (int j = 0; j < agModule.size(); ++j )
		{
			string strModule = agModule[j];
			if (0 == _tcsicmp(strModule.c_str(),tmpModule.szModuleName))
			{	
				bExistModule = TRUE;				
				Module  tmpCmpModule = m_lstModule.at(j);
				_stprintf(tmpModule.szModuleName,_T("%s(%d)"),strModule.c_str() ,++nAddIndex);
				_tprintf(_T("[WARNING] [NVM: %s] module name [%s] is same as [NVM: %s],Change to [%s].\n"),
					tmpModule.szModuleFileName,
					tmpCmpModule.szModuleName,
					tmpCmpModule.szModuleFileName,
					tmpModule.szModuleName
					);
				m_lstModule[i] = tmpModule;
				break;
			}
		}
		if (!bExistModule)
		{
			agModule.push_back(tmpModule.szModuleName);
		}	
	}
}
void CFileParser::GetModuleParentInfo(int nModuleIndex,CMapID &mapParentIndex)
{
	ModuleItem_List_Ptr currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
	int nModuleItemCount = GetModuleItemCount(nModuleIndex);
	for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; ++nCurItemIndex)
	{
		ModuleItem stCurrModuleItem = currModuleItemList->at (nCurItemIndex);
		mapParentIndex[stCurrModuleItem.nParentItemIndex] = stCurrModuleItem.nParentItemIndex;
	}
}
void CFileParser::GetNVItemPath(TCHAR* szModuleName, int nItemParentIndex,string& strPath)
{
	string strCurrentName;
	if (-1 != nItemParentIndex)
	{
		ModuleItem stCurrModuleItem = m_currModuleItemList->at(nItemParentIndex);
		strPath.insert(0,stCurrModuleItem.szItemName);
		strPath.insert(0,_T("\\"));
		GetNVItemPath(szModuleName,stCurrModuleItem.nParentItemIndex,strPath);	
	}
	else
	{
		strPath.insert(0,szModuleName);
	}
}
void CFileParser::CheckModuleItem()
{
	int nModuleItemCount = 0;
	for(int nCurModuleIndex = 0; nCurModuleIndex < (int)m_nModuleCount; ++nCurModuleIndex )
    {               
		
		CMapID mapParentIndex;
		CMapID::iterator it;
		Module  stCurModule = m_lstModule.at(nCurModuleIndex);
		GetModuleParentInfo(nCurModuleIndex,mapParentIndex);
		
		nModuleItemCount = GetModuleItemCount(nCurModuleIndex);
		for(it = mapParentIndex.begin(); it != mapParentIndex.end(); it++)
		{
			std::vector<string> agModuleItem;
			int nAddIndex = 0;
			for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; ++nCurItemIndex)
			{
				ModuleItem stCurrModuleItem = m_currModuleItemList->at (nCurItemIndex);
				if ( it->second == stCurrModuleItem.nParentItemIndex)
				{

					BOOL bExistModuleItem = FALSE;
					for (int j = 0; j < agModuleItem.size(); ++j )
					{
						string strModuleItemName = agModuleItem[j];
						if (0==_tcsicmp(strModuleItemName.c_str(),stCurrModuleItem.szItemName))
						{
							string strItemPath = strModuleItemName;
							bExistModuleItem = TRUE;
							_stprintf(stCurrModuleItem.szItemName,_T("%s(%d)"),strModuleItemName.c_str(),++nAddIndex);

							strItemPath.insert(0,_T("\\"));
							GetNVItemPath(stCurModule.szModuleName,stCurrModuleItem.nParentItemIndex,strItemPath);
							
							_tprintf(_T("[WARNING] [NVM: %s] [ItemName: %s] is already exists,Change to [%s] .\n"),
								stCurModule.szModuleFileName,
								strItemPath.c_str(),
								stCurrModuleItem.szItemName);
							
							agModuleItem.push_back(stCurrModuleItem.szItemName);
							(*m_currModuleItemList)[nCurItemIndex] = stCurrModuleItem;
							break;
						}
					}
					if (!bExistModuleItem)
					{
						agModuleItem.push_back(stCurrModuleItem.szItemName);
					}	
				}
			}
			
		}
	}
}
Module & CFileParser::GetModule(INT nModuleIndex)
{
    m_CurrModule = m_lstModule.at(nModuleIndex);

    return m_CurrModule;
}

void CFileParser::SetModule(int nModuleIndex, const Module NewModule)
{    
    Module tmpModule = m_lstModule.at(nModuleIndex);
    memcpy(&tmpModule, &NewModule, sizeof(Module));
    m_lstModule[nModuleIndex] = tmpModule;
}

INT CFileParser::GetModuleCount()
{
    return m_lstModule.size();
}

ModuleItem & CFileParser::GetModuleItem(INT nModuleIndex, INT nItemIndex )
{
    //ModuleItem tmpModuleItem;
    //memcpy(&m_CurrModuleItem, &tmpModuleItem, sizeof(tmpModuleItem));

    if(nModuleIndex >= 0 && (unsigned int)nModuleIndex < m_lstModuleItemPtr.size())
    {
        m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
        if(nItemIndex>=0 && (unsigned int)nItemIndex < m_currModuleItemList->size())
        {
            m_CurrModuleItem = m_currModuleItemList->at(nItemIndex);
        }
    }

    return m_CurrModuleItem;
}

void CFileParser::SetModuleItem( int                nModuleIndex, 
                                int                nItemIndex, 
                                const ModuleItem & NewModuleItem,
                                BOOL               fSetModified )
{
    m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);    
    ModuleItem tmpItem = m_currModuleItemList->at(nItemIndex);
    memcpy(&tmpItem, &NewModuleItem, sizeof(ModuleItem));
    (*m_currModuleItemList)[nItemIndex] = tmpItem;


    // @ZHANG Ming : 2003-05-06
    if ( TRUE == fSetModified )
    {
        SetModified( TRUE );
    }
}

INT CFileParser::GetModuleItemCount(INT nModuleIndex)
{
    m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
    m_nModuleItemCount = m_currModuleItemList->size();

    return m_nModuleItemCount;
}


INT CFileParser::CreateModule(const TCHAR *szModuleName, const TCHAR *szModuleFileName, const TCHAR *szModuleDesc)
{  
    Module  tmpModule;

    tmpModule.nItemCount = 0;
    _tcscpy(tmpModule.szModuleName, szModuleName);
    _tcscpy(tmpModule.szModuleFileName, szModuleFileName);
    _tcscpy(tmpModule.szModuleDescription , szModuleDesc);
    m_lstModule.push_back(tmpModule);

    m_currModuleItemList = new ModuleItem_List;
    //m_currModuleItemList->SetTypeSize (sizeof(ModuleItem));
    m_lstModuleItemPtr.push_back(m_currModuleItemList);
    m_currModuleItemList = NULL;

    m_nModuleCount++;

    // @ZHANG Ming : 2003-05-06
    SetModified(TRUE);

    return m_nModuleCount;
}



void CFileParser::RemoveModule(INT nModuleIndex)
{
	if(0 <= nModuleIndex && nModuleIndex < m_lstModule.size())
	{
		m_lstModule.erase(m_lstModule.begin() + nModuleIndex);
	    m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
	    m_currModuleItemList->clear();
	    delete m_currModuleItemList;
	    m_currModuleItemList = NULL;

	    //m_nModuleCount++;
	    m_nModuleCount--; // @ZHANG Ming : 2003-05-06

	    // @ZHANG Ming : 2003-05-06
	    SetModified(TRUE);
	}
}

int CFileParser::InsertModuleItemAllByOnce(INT nModuleIndex, ModuleItem InsertModuleItem,INT nInsertAfter,BOOL bInsertAfter)
{
    assert((UINT)nModuleIndex < m_nModuleCount);

    INT  bReturnCode = INVALID_MODULE_ID;

    m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
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
        InsertModuleItem.nItemIndex = m_currModuleItemList->size();
    }

    if(INVALID_MODULE_ID == InsertModuleItem.nParentItemIndex)
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
            ( m_currModuleItemList->size() == nInsertAfter))
        {
            m_currModuleItemList->push_back(InsertModuleItem);
            bReturnCode = m_currModuleItemList->size();
        }
        else
        {
            m_currModuleItemList->insert(m_currModuleItemList->begin() + nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }

    }
    else
    {
        assert(InsertModuleItem.nParentItemIndex < m_currModuleItemList->size());
        if(!(InsertModuleItem.nParentItemIndex < m_currModuleItemList->size()))
        {
            //Notify that the insert failed
            return bReturnCode;
        }

        ModuleItem tmpParentItem = m_currModuleItemList->at(InsertModuleItem.nParentItemIndex);
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
        (*m_currModuleItemList)[InsertModuleItem.nParentItemIndex] = tmpParentItem;
        while(INVALID_MODULE_ID != tmpParentItem.nParentItemIndex )
        {
            ModuleItem tmpItem = m_currModuleItemList->at(tmpParentItem.nParentItemIndex);
            if(tmpItem.nOffset < tmpParentItem.nOffset )
            {
                tmpItem.nOffset = tmpParentItem.nOffset ;
                (*m_currModuleItemList)[tmpParentItem.nParentItemIndex] = tmpItem;
                tmpParentItem = tmpItem;
            }
            else
            {               
                break;
            }
        }

        if(INVALID_MODULE_ITEM_INDEX == nInsertAfter)
        {
            m_currModuleItemList->push_back(InsertModuleItem);
            bReturnCode = m_currModuleItemList->size();
        }
        else
        {
            m_currModuleItemList->insert(m_currModuleItemList->begin() + nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }
    }

    if(INVALID_MODULE_ITEM_INDEX != nInsertAfter)
    {
        ModuleItem tmpItem;

        for(int Index = nInsertAfter + 1; Index < m_currModuleItemList->size(); Index++)
        {
            tmpItem = m_currModuleItemList->at(Index);            

            if(InsertModuleItem.nParentItemIndex < tmpItem.nParentItemIndex)
            {
                tmpItem.nParentItemIndex += 1;
            }

            tmpItem.nItemIndex += 1;

            (*m_currModuleItemList)[Index] = tmpItem;
        }
    }

    // @ZHANG Ming : 2003-05-06 
    // [[
    if ( INVALID_MODULE_ID != bReturnCode )
    {
        // Add new module item successfully
        SetModified(TRUE);
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

        for(int Index = nInsertAfter + 1; Index < m_currModuleItemList->size(); Index++)
        {
            ModuleItem tmpItem = m_currModuleItemList->at(Index);          

            if(nParentItemIndex < tmpItem.nParentItemIndex)
            {
                tmpItem.nParentItemIndex += insertItemSize;
            }

            tmpItem.nItemIndex += insertItemSize;

            (*m_currModuleItemList)[Index] = tmpItem;
        }
    }

}

int CFileParser::InsertModuleItem(INT nModuleIndex, ModuleItem InsertModuleItem,INT nInsertAfter,BOOL bInsertAfter) //lint -e578 
{
    assert((UINT)nModuleIndex < m_nModuleCount);

    INT  bReturnCode = INVALID_MODULE_ID;

    //[[ repeated ID check
    if (-1 == InsertModuleItem.nParentItemIndex)
    {
        CMapID::iterator  it = m_mapIDs.find(InsertModuleItem.nItemID);
        if (m_mapIDs.end() != it)
        {
			_tprintf(_T("[ERROR] ItemID %d has existed, please input a unexisted ID.\n"), InsertModuleItem.nItemID);

            return bReturnCode;
        }
        else
        {
            m_mapIDs.insert(CMapID::value_type(InsertModuleItem.nItemID, InsertModuleItem.nItemLength));
        }

    }
    //]]

    m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
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
        InsertModuleItem.nItemIndex = m_currModuleItemList->size();
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
            ( m_currModuleItemList->size() == nInsertAfter))
        {
            m_currModuleItemList->push_back(InsertModuleItem);
            bReturnCode = m_currModuleItemList->size();
        }
        else
        {
            m_currModuleItemList->insert(m_currModuleItemList->begin() + nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }

    }
    else
    {
        assert(InsertModuleItem.nParentItemIndex < m_currModuleItemList->size());
        if(!(InsertModuleItem.nParentItemIndex < m_currModuleItemList->size()))
        {
            //Notify that the insert failed
            return bReturnCode;
        }

        ModuleItem tmpParentItem = m_currModuleItemList->at(InsertModuleItem.nParentItemIndex);
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
        (*m_currModuleItemList)[InsertModuleItem.nParentItemIndex] = tmpParentItem;
        while(INVALID_MODULE_ID != tmpParentItem.nParentItemIndex )
        {
            ModuleItem tmpItem = m_currModuleItemList->at(tmpParentItem.nParentItemIndex);
            if(tmpItem.nOffset < tmpParentItem.nOffset )
            {
                tmpItem.nOffset = tmpParentItem.nOffset ;
                (*m_currModuleItemList)[tmpParentItem.nParentItemIndex] = tmpItem;
                tmpParentItem = tmpItem;
            }
            else
            {               
                break;
            }
        }

        if(INVALID_MODULE_ITEM_INDEX == nInsertAfter)
        {
            m_currModuleItemList->push_back(InsertModuleItem);
            bReturnCode = m_currModuleItemList->size();
        }
        else
        {
            m_currModuleItemList->insert(m_currModuleItemList->begin() + nInsertAfter, InsertModuleItem);
            bReturnCode = nInsertAfter;
        }
    }

    if(INVALID_MODULE_ITEM_INDEX != nInsertAfter)
    {
        ModuleItem tmpItem;

        for(int Index = nInsertAfter + 1; Index < m_currModuleItemList->size(); Index++)
        {
            tmpItem = m_currModuleItemList->at(Index);            

            if(InsertModuleItem.nParentItemIndex < tmpItem.nParentItemIndex)
            {
                tmpItem.nParentItemIndex += 1;
            }

            tmpItem.nItemIndex += 1;

            (*m_currModuleItemList)[Index] = tmpItem;
        }
    }

    // @ZHANG Ming : 2003-05-06 
    // [[
    if (INVALID_MODULE_ID != bReturnCode)
    {
        // Add new module item successfully
        SetModified(TRUE);
    }
    // ]]
    return bReturnCode;
}

void CFileParser::RemoveModuleItem(INT nModuleIndex, INT nItemIndex)
{
    ModuleItem miItem = GetModuleItem(nModuleIndex, nItemIndex);
    if (INVALID_MODULE_ID != miItem.nParentItemIndex)
    {
        ModuleItem miParent = GetModuleItem(nModuleIndex, miItem.nParentItemIndex);
        if ( ARRAY_ITEM_TYPE == miParent.eItemType ||
            ASTRING_ITEM_TYPE == miParent.eItemType ||
            WSTRING_ITEM_TYPE == miParent.eItemType)
        {
            assert(miParent.nArraySize > 0);
            --miParent.nArraySize;
            SetModuleItem(nModuleIndex, miParent.nItemIndex, miParent);
        }	
    }

    if(nItemIndex == (m_currModuleItemList->size() - 1))
    {
        //The removed item is the last item
        m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
        m_currModuleItemList->erase(m_currModuleItemList->begin() + nItemIndex);
    }
    else
    {
        ModuleItem tmpItem;
        INT        nRemovedItemParentIndex;

        //The removed item is not the last item
        m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
        tmpItem = m_currModuleItemList->at(nItemIndex);
        nRemovedItemParentIndex = tmpItem.nParentItemIndex ;

        for(int Index = nItemIndex + 1; Index < m_currModuleItemList->size(); Index++)
        {
            tmpItem = m_currModuleItemList->at(Index);

            if(nRemovedItemParentIndex < tmpItem.nParentItemIndex)
            {
                tmpItem.nParentItemIndex -= 1;
            }

            tmpItem.nItemIndex -= 1;

            (*m_currModuleItemList)[Index] = tmpItem;
        }

        m_currModuleItemList->erase(m_currModuleItemList->begin() + nItemIndex);


    }    
    // @ZHANG Ming : 2003-05-06
    SetModified(TRUE);
}

void CFileParser::RemoveAllNVltem(INT nModuleIndex)
{
    m_currModuleItemList = m_lstModuleItemPtr.at(nModuleIndex);
    m_currModuleItemList->clear();
}


BOOL CFileParser::LoadFromImage(const TCHAR *szImageFileName)
{
    FILE *fpImage = NULL;
    BYTE szItemContent[MAX_TOTAL_ITEM_CONTENT];
    UINT nTotalImageLength = 0;   
    UINT nNVImageLength = 0;   
    ModuleItem tmpParentItem;

    if( (NULL == szImageFileName) ||
        (0 == _tcscmp(m_szTargetFileName, _T(""))) )
    { 
        //Notify that has not given target file
        return FALSE;
    }

    fpImage = _tfopen(szImageFileName, _T("rb"));
    if(NULL == fpImage)
    {
        //Notify that can not open target file
        return FALSE;
    }

    memset(&tmpParentItem, 0, sizeof(ModuleItem));
    memset(szItemContent, 0, MAX_ITEM_CONTENT);

    nTotalImageLength = fread(szItemContent, 1, MAX_TOTAL_ITEM_CONTENT, fpImage);

    //Remove the image file generating timing
    nNVImageLength += 4;
    //    nTotalImageLength -= 4;

    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }

    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {
        m_currModuleItemList = m_lstModuleItemPtr.at(nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < GetModuleItemCount(nCurModuleIndex); nCurItemIndex++)
        {
            m_CurrModuleItem = m_currModuleItemList->at(nCurItemIndex);
            if(INVALID_MODULE_ID != m_CurrModuleItem.nParentItemIndex )
            {
                switch(m_CurrModuleItem.eItemType)
                {
                case CHAR_ITEM_TYPE:                
                    {
                        if((m_CurrModuleItem.nOffset + nNVImageLength + 4 +1) > nTotalImageLength)
                        {
                            return FALSE;
                        }

                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4 ]);
                    }
                    break;
                case SHORT_ITEM_TYPE:
                    {           
                        if((m_CurrModuleItem.nOffset + nNVImageLength + 4 + 2) > nTotalImageLength)
                        {
                            return FALSE;
                        }

                        WORD  tmpSrcValue = *(WORD*)&szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4];
                        WORD  tmpDstValue;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);

                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);

                    }
                    break;
                case LONG_ITEM_TYPE:
                    {
                        if((m_CurrModuleItem.nOffset + nNVImageLength + 4 + 4) > nTotalImageLength)
                        {
                            return FALSE;
                        }

                        DWORD  tmpSrcValue = *(DWORD *)&szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4];
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

                (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
            }
            else
            {
                if(0 < tmpParentItem.nItemLength )
                {
                    nNVImageLength += tmpParentItem.nItemLength + 4;
                    nNVImageLength = ((nNVImageLength + 3) & 0xFFFFFFFC);

                    if(nNVImageLength >= nTotalImageLength)
                    {
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
                            return FALSE;
                        }

                        WORD tmpSrcValue, tmpDstValue ;
                        tmpSrcValue = *(WORD *)&szItemContent[nNVImageLength];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemID = tmpDstValue;

                        tmpSrcValue = *(WORD *)&szItemContent[nNVImageLength + 2];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemLength = tmpDstValue;

                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),szItemContent[ nNVImageLength + 4 ] );

                        (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;

                    }
                    break;
                case SHORT_ITEM_TYPE:
                    {
                        if((nNVImageLength + 4 + 2) > nTotalImageLength)
                        {
                            return FALSE;
                        }

                        WORD tmpSrcValue, tmpDstValue ;
                        tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemID = tmpDstValue;

                        tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength + 2];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemLength = tmpDstValue;


                        tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength + 4];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);

                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                        (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
                    }
                    break;
                case LONG_ITEM_TYPE:
                    {
                        if((nNVImageLength + 4 + 4) > nTotalImageLength)
                        {
                            return FALSE;
                        }


                        {
                            WORD tmpSrcValue, tmpDstValue ;

                            tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength];
                            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                            m_CurrModuleItem.nItemID = tmpDstValue;

                            tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength + 2];
                            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                            m_CurrModuleItem.nItemLength = tmpDstValue;

                        }

                        DWORD  tmpSrcValue;
                        DWORD tmpDstValue ;

                        tmpSrcValue = *(DWORD *)&szItemContent[nNVImageLength + 4];                        
                        CONVERT_INT2(tmpSrcValue, tmpDstValue);

                        _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                        (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
                    }
                    break;  
                case ARRAY_ITEM_TYPE:
                    if((nNVImageLength + 4 ) > nTotalImageLength)
                    {
                        return FALSE;
                    }

                    {
                        WORD tmpSrcValue, tmpDstValue ;
                        tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemID = tmpDstValue;

                        tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength + 2];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemLength = tmpDstValue;
                    }

                    /*m_CurrModuleItem.nItemID = *(WORD *)&szItemContent[ nNVImageLength];
                    m_CurrModuleItem.nItemLength = *(WORD *)&szItemContent[ nNVImageLength + 2];*/
                    (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
                    break;
                case ASTRING_ITEM_TYPE:
                case WSTRING_ITEM_TYPE:
                    if((nNVImageLength + 4 ) > nTotalImageLength)
                    {
                        return FALSE;
                    }

                    {
                        WORD tmpSrcValue, tmpDstValue ;
                        tmpSrcValue = *(WORD *)&szItemContent[nNVImageLength];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemID = tmpDstValue;

                        tmpSrcValue = *(WORD *)&szItemContent[nNVImageLength + 2];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemLength = tmpDstValue;
                    }

                    /*m_CurrModuleItem.nItemID = *(WORD *)&szItemContent[ nNVImageLength];
                    m_CurrModuleItem.nItemLength = *(WORD *)&szItemContent[ nNVImageLength + 2];*/
                    (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
                    break;
                case STRUCT_ITEM_TYPE:
                    if((nNVImageLength + 4 ) > nTotalImageLength)
                    {
                        return FALSE;
                    }

                    {
                        WORD tmpSrcValue, tmpDstValue ;
                        tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemID = tmpDstValue;

                        tmpSrcValue = *(WORD *)&szItemContent[ nNVImageLength + 2];
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        m_CurrModuleItem.nItemLength = tmpDstValue;
                    }

                    /*m_CurrModuleItem.nItemID = *(WORD *)&szItemContent[ nNVImageLength];
                    m_CurrModuleItem.nItemLength = *(WORD *)&szItemContent[ nNVImageLength + 2];*/
                    (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;                 
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

    memset(m_szItemContent, 0, MAX_TOTAL_ITEM_CONTENT);

    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }

    if(0 == _tcscmp(m_szTargetFileName, _T("")))
    { 
        //Notify that has not given target file
        return FALSE;
    }

    BOOL bReadOnly;

    fpTarget = OpenFileForWrite( m_szTargetFileName, &bReadOnly, TRUE );
    if( NULL == fpTarget )
    {

        //Notify that open file failed
        _tprintf( _T("Can't open file %s!"), m_szTargetFileName );
        return FALSE;
    }

    memset(&tmpParentItem, 0, sizeof(ModuleItem));
#define HASH_LENGTH  40   

    time_t  t;
    time(&t);


    //Generate the image file indentifier
    {    
        DWORD dwGenerateTime = t;
        DWORD tmpDstValue;
        CONVERT_INT2(dwGenerateTime, tmpDstValue);
        *(LONG *)&(m_szItemContent[nNVImageLength]) = tmpDstValue;
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
        BYTE szItemContent[MAX_TOTAL_ITEM_CONTENT];
        DWORD dwFileSize;


        FILE *fpFixBin = _tfopen(m_szFixBin,  _T("rb"));
        if(NULL == fpFixBin)
        {
            //Notify that can not open target file
            _tprintf(_T("Can't open fixBin file, Please check the file is exist!"));
            fclose(fpTarget); // add by Joly
            return FALSE;
        }

        fseek(fpFixBin, 0, SEEK_END);
        dwFileSize = ftell(fpFixBin);

        //_tprintf(_T("%d"), dwFileSize);

        fseek(fpFixBin, 0, SEEK_SET);  

        memset(szItemContent, 0, MAX_TOTAL_ITEM_CONTENT);

        int nRead = fread(szItemContent, 1, MAX_TOTAL_ITEM_CONTENT, fpFixBin);

        fclose(fpFixBin);
        if ( nRead <= 0 )
        {
            _tprintf( _T("%s read %d failed "),  m_szFixBin, nRead);
            fclose(fpTarget); 
            return FALSE;
        }

        AnalyzeFixBin((const unsigned char*)szItemContent, dwFileSize, mapIDs);

        DWORD dwFixLength = dwFileSize - HASH_LENGTH - 4;
        memcpy(m_szItemContent+nNVImageLength, (const unsigned char*)szItemContent + HASH_LENGTH + 4, dwFixLength);  //merge fix bin to project bin
        nNVImageLength += dwFixLength;
        if (!m_strErrID.empty())
        {
            _tprintf(_T("Existed repeated IDs: %s\n"), m_strErrID.c_str());	
            fclose(fpTarget); 
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
        // @ZHANG Ming : 2003-05-06  
        FILE *fpDspCode = _tfopen( szDspFileName, _T("rb") );
        if(NULL == fpDspCode)
        {
            //Notify that can not open target file
            _tprintf(_T("Can't open DSP Code file, Please check the file is exist!"));
            fclose(fpTarget); // add by Joly
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

        GetNextArrayForMemFind((LPBYTE)SEPERATE_STRING, seperate_string_len, find_next);

        ULONG lCurPos = 0;
        int nPos = MemFind(tmp_dsp_code, nRead, lCurPos, (LPBYTE)SEPERATE_STRING, seperate_string_len, find_next);

        //[ DSP File check
        if (0 == nRead || nRead > DSP_TOTAL_SIZE)
        {
            _tprintf(_T("Wrong DSP Code file, Please check the file!"));
            fclose(fpTarget); 
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
				return FALSE;
			}
        }      

        //Write DSP Code NVItem Identifier
        {
            WORD  tmpSrcValue, tmpDstValue;

            tmpSrcValue = (0x04);   //NV_DSP_CODE
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[nNVImageLength] = tmpDstValue ;

            tmpSrcValue = (WORD)nDspCodeLen1;
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[nNVImageLength + 2] = tmpDstValue ;

            nNVImageLength += 4;
        }

        memcpy(m_szItemContent+nNVImageLength,tmp_dsp_code,nDspCodeLen1 );

        int i= (int)((int)nNVImageLength+nDspCodeLen1);
        for(; i > (int)nNVImageLength; i--)
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
        *(WORD *)&m_szItemContent[nNVImageLength - 2] = tmpDstValue ;

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
            *(WORD *)&m_szItemContent[nNVImageLength] = tmpDstValue ;

            tmpSrcValue = (WORD)nDspCodeLen2;
            CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
            *(WORD *)&m_szItemContent[ nNVImageLength + 2] = tmpDstValue ;

            nNVImageLength += 4;

            memcpy(m_szItemContent + nNVImageLength,tmp_dsp_code+lCurPos,nDspCodeLen2);

            // Little endian to big endian
            int Index = 0;
            for(; Index < nDspCodeLen2; Index+=2)
            {
                tmpSrcValue = *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index];
                CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                *(WORD *)&m_szItemContent[(int)nNVImageLength  + Index] = tmpDstValue;
            }

            nNVImageLength += RoundTo4Align(nDspCodeLen2);
        }

    } 
    // if ( m_bWriteDsp )
    //End of Writing DSP Code
    if ( ( NULL != lpAppendage ) && ( cbSize > 0 ) )
    {
        if( cbSize <= 4 )
        {

            fclose(fpTarget); // add by Joly
            return FALSE;
        }
        nNVImageLength = RoundTo4Align( nNVImageLength );
        memmove( m_szItemContent + nNVImageLength, 
            (BYTE*)lpAppendage + 4,  // skip the time stamp in scinv.bin
            cbSize - 4 );
        nNVImageLength += ( cbSize - 4 );
    }

    nNVImageLength = RoundTo4Align(nNVImageLength);


    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {
        m_currModuleItemList = m_lstModuleItemPtr.at(nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < GetModuleItemCount(nCurModuleIndex); nCurItemIndex++)
        {
            m_CurrModuleItem = m_currModuleItemList->at(nCurItemIndex);
            if (m_CurrModuleItem.nItemID > 0xFFFF)
            {
                _tprintf(_T("[ERROR] [ItemName: %s]:item ID [%d] is not valid."),
                    m_CurrModuleItem.szItemName, m_CurrModuleItem.nItemID);							
                fclose(fpTarget);
                return FALSE;
            }
			/*
	            if (0 == m_CurrModuleItem.nItemLength || m_CurrModuleItem.nItemLength > 0xFFFF)
	            {
	                _tprintf(_T("[ERROR] [Module: %s] [ItemName: %s] [ItemID: %d]:item Length [%d] is not valid."),m_lstModule[nCurModuleIndex].szModuleFileName,
	                    m_CurrModuleItem.szItemName, m_CurrModuleItem.nItemID, m_CurrModuleItem.nItemLength);							
	                fclose(fpTarget);
	                return FALSE;
	            }
	            */
            if(INVALID_MODULE_ID != m_CurrModuleItem.nParentItemIndex )
            {
                switch(m_CurrModuleItem.eItemType)
                {
                case CHAR_ITEM_TYPE:                
                    {
                        if( ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x') )||
                            ( NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X') ) )
                        {
                            m_szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4 ] = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
                        }
                        else
                        {
                            m_szItemContent[m_CurrModuleItem.nOffset + nNVImageLength + 4 ] = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10 );
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
                            m_szItemContent[ nNVImageLength + 4 ]    = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16 );
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



                        if( (NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x')) ||
                            (NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X')))
                        {
                            tmpSrcValue  = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16);
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
                        *(DWORD *)&m_szItemContent[nNVImageLength + 4] = dwDstValue;
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
                        *(WORD *)&m_szItemContent[nNVImageLength] = tmpDstValue ;

                        tmpSrcValue = (WORD)m_CurrModuleItem.nItemLength;
                        CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
                        *(WORD *)&m_szItemContent[nNVImageLength + 2] = tmpDstValue ;
                    }
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
    //[[ by Joly  write other bin file
    if (m_bWriteMult)
    {
        int nRt = ReadOtherFiles( nNVImageLength );
        if (1 == nRt)
        {
            fclose(fpTarget);
            return FALSE;
        }
    }
    //]]

    if (m_bVerFlag)
    {
        CountAllMD5(nNVImageLength);
    }


    fwrite(m_szItemContent, 1, nNVImageLength, fpTarget);
    if (m_bTailFlag)
    {
        BYTE       strend[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
        fwrite(strend,1,8,fpTarget);
    }
    fclose(fpTarget);

    //[[ Joly
    if (m_bHashHeader)
    {
        WriteHashHeader();
    }
    // @ZHANG Ming : 2003-04-18
    // [[       
    if (bReadOnly)
    {
        // Restore the file's read-only attribute
        ChangeFileRdOnlyAttri(m_CurrModule.szModuleFileName, TRUE);
    }
    // ]]

    return TRUE;
}

void CFileParser::WriteHashHeader()
{
    string  strLine;
    FILE* pFile = _tfopen(m_szHashPath,  _T("w") );
    if(NULL == pFile)
    {
        _tprintf("opendir failed!\n");	 
        ClearNVHeader();
        return;
    }
    char strNV[MAX_TOTAL_ITEM_CONTENT];


    strLine = _T("#ifndef _NV_COMPILE_HASH_H_ \n#define _NV_COMPILE_HASH_H_\n\nuint8 nv_compile_hash[16] = {");

    for (int i = 0; i < 16; i++)
    {
        char strTmp[MAX_PATH];
#define  ALL_HASH_START  28		   
        if (15 == i)
        { 

            _stprintf(strTmp, "0X%02X", m_szItemContent[ALL_HASH_START + i]);

        }	
        else
        {
            _stprintf(strTmp, "0X%02X,", m_szItemContent[ALL_HASH_START + i]);
        }

        _stprintf(strNV, "%s%s",  strLine.c_str(), strTmp);
        strLine = strNV;
        //_tprintf("Debug Joly[%d] = %s", i, strLine.c_str());
    }
    strLine += _T("}\n\n#endif //_NV_COMPILE_HASH_H_");
    // _tprintf("%s\n\n",  strLine.c_str());

    fwrite(strLine.c_str(), strLine.size(), 1, pFile);

    _tprintf(_T("VersionInfo headerFile  %s  generate finished\n"), m_szHashPath);

    fclose(pFile);	
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
        //WORD wItemId = *( WORD* )(m_szItemContent + nOffset);	
        //CONVERT_SHORT2(wItemId, wItemId);

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

int CFileParser::GenerateOffset()
{
    m_lstTopItem.clear();
    int  nOffset = 0;
    // @ZHANG Ming : 2003-04-23
    // [[
    UINT nNVImageLength = 0;
    nNVImageLength = (m_bWriteDsp ? 4 + (DSP_CODE_SIZE + 4) : 4 ); //The File Identifier and the DSP Code
    // ]]
    ModuleItem tmpLastParent;
    ModuleItem tmpLastItem;

    int nModuleItemCount = 0;
    int nItemMaxLen = 0;

    memset(&tmpLastItem, 0, sizeof(ModuleItem));

    //write module list part
    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {               
        m_currModuleItemList = m_lstModuleItemPtr.at(nCurModuleIndex);
        nModuleItemCount = GetModuleItemCount(nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; nCurItemIndex++)
        {
            nOffset = 0;			

            m_CurrModuleItem = m_currModuleItemList->at(nCurItemIndex);
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

                assert(nCurItemIndex < nModuleItemCount);
                if(nCurItemIndex >= nModuleItemCount)
                {
                    break;
                }

                tmpLastParent = m_CurrModuleItem;
                for(UINT i = 0; i< tmpLastParent.nArraySize; i++)
                {
                    m_CurrModuleItem = m_currModuleItemList->at(nCurItemIndex);
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

                    (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
                    nCurItemIndex += 1;

                    //ASSERT(nCurItemIndex < nModuleItemCount);
                    if(nCurItemIndex >= nModuleItemCount)
                    {
                        break;
                    }

                }

                nCurItemIndex -= 1;

                tmpLastParent.nItemLength = m_CurrModuleItem.nItemLength * tmpLastParent.nArraySize ;
                (*m_currModuleItemList)[tmpLastParent.nItemIndex] = tmpLastParent;                

                m_CurrModuleItem = tmpLastParent;
            }
            else if(CHAR_ITEM_TYPE == m_CurrModuleItem.eItemType  )
            {
                (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
            }
            else if(SHORT_ITEM_TYPE == m_CurrModuleItem.eItemType  )
            {
                (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
            }
            else if(LONG_ITEM_TYPE == m_CurrModuleItem.eItemType  )
            {
                (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
            }

            //Aligned by 4 bytes
            nNVImageLength = ((nNVImageLength + 3) & 0xFFFFFFFC);
            m_CurrModuleItem.nOffset = nNVImageLength + 4;
            //At this the 'nParentItemIndex' indicates module index
            m_CurrModuleItem.nParentItemIndex = nCurModuleIndex;
            nNVImageLength = m_CurrModuleItem.nOffset + m_CurrModuleItem.nItemLength;
			if (m_CurrModuleItem.nItemLength > 0xFFFF || 0 == m_CurrModuleItem.nItemLength)
			{
				Module  stCurModule = m_lstModule.at(nCurModuleIndex);
				_tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:nv item data length [%d] is invalid."),
					stCurModule.szModuleFileName,
					m_CurrModuleItem.szItemName, 
					m_CurrModuleItem.nItemLength);
				return 0;
			}
            m_lstTopItem.push_back(m_CurrModuleItem);

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

        if((int)tmpModuleItem.nParentItemIndex < nParentIndex )
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
		tmpParentModuleItem.nItemLength  = nOffset - *nFirstOffset;
		tmpParentModuleItem.nOffset = nOffset - tmpParentModuleItem.nItemLength;
            *nFirstOffset = nOffset;
            (*m_currModuleItemList)[tmpParentModuleItem.nItemIndex] = tmpParentModuleItem;

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
                tmpModuleItem.nOffset = tmp;//nMaxLen;
                (*m_currModuleItemList)[tmpModuleItem.nItemIndex] = tmpModuleItem;
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

                assert(tmpModuleItem.nItemIndex < (UINT)m_currModuleItemList->size());
                (*m_currModuleItemList)[tmpModuleItem.nItemIndex] = tmpModuleItem;

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

	tmpParentModuleItem.nItemLength  = nOffset - *nFirstOffset;
	tmpParentModuleItem.nOffset = nOffset - tmpParentModuleItem.nItemLength;
    *nFirstOffset = nOffset;
    (*m_currModuleItemList)[tmpParentModuleItem.nItemIndex] = tmpParentModuleItem;

}

int CFileParser::InsertModule(TCHAR *szMOduleFileName)
{
    if(NULL != szMOduleFileName)
    {
        if(0==_tcscmp(m_szappend,_T(".nvm")))
        {
            LoadModule(szMOduleFileName);
        }

        else
        {
            return INVALID_MODULE_ID;
        }

        m_nModuleCount++;
        SetModified(TRUE);

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
    _tcscpy(szPrjPath, szPrjFileName);

    TCHAR* pPrjPath = _tcsrchr(szPrjPath, '\\');
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
        m_CurrModule = m_lstModule.at(Index);
        _tcscpy(szModuleName, m_CurrModule.szModuleFileName);

        TCHAR *pResult = _tcsrchr(szModuleName, '\\');
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

        m_lstModule[Index] = m_CurrModule;
    }  
    /*xml file support by xmzhang add begin*	
    m_szappend = _tcsrchr(szPrjFileName,'.');
    if(0 == _tcscmp(m_szappend,_T(".xprj")))
    {
    return (SaveProjectXML());
    }
    else   //xml file support by xmzhang add end*/    
    {
        return (SaveProject());
    }

}

void CFileParser::FillWithDefault()
{    
    for(UINT nCurModuleIndex = 0; nCurModuleIndex < m_nModuleCount; nCurModuleIndex++)
    {
        m_currModuleItemList = m_lstModuleItemPtr.at(nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < GetModuleItemCount(nCurModuleIndex); nCurItemIndex++)
        {
            m_CurrModuleItem = m_currModuleItemList->at(nCurItemIndex);
            switch(m_CurrModuleItem.eItemType)
            {
            case CHAR_ITEM_TYPE:
            case SHORT_ITEM_TYPE:
            case LONG_ITEM_TYPE:
                _tcscpy(m_CurrModuleItem.szItemContent, _T("0"));
                break;
            default:
                break;
            }
            (*m_currModuleItemList)[nCurItemIndex] = m_CurrModuleItem;
        }
    }
    // @ZHANG Ming : 2003-05-06
    SetModified(TRUE);
}

BOOL CFileParser::WriteNVItem(UINT nItemID, LPVOID lpBuffer, 
                              DWORD nNumberOfBytesToWrite, 
                              BOOL bBigEndian)
{

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

    for(int nIndex = 0; nIndex < m_lstTopItem.size(); nIndex++)
    {
        hTopItem = m_lstTopItem.at(nIndex);
        if(nItemID == hTopItem.nItemID )
        {
            if(nNumberOfBytesToWrite == hTopItem.nItemLength )
            {
                m_currModuleItemList = m_lstModuleItemPtr.at(hTopItem.nParentItemIndex);
                int nCurr = hTopItem.nItemIndex;
                if ( LONG_ITEM_TYPE < hTopItem.eItemType ) 
                {
                    nCurr++;
                }

                for(; nCurr < m_currModuleItemList->size(); nCurr++)
                {
                    m_CurrModuleItem = m_currModuleItemList->at(nCurr);
                    if(nItemID != m_CurrModuleItem.nItemID )
                    {
                        // @ZHANG Ming : 2003-05-06
                        SetModified(TRUE);
                        return TRUE;
                    }

                    if(m_CurrModuleItem.bReadOnly )
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
                            WORD w = (WORD)_tcstol(m_CurrModuleItem.szItemContent, NULL, 0);
                            m_CurrModuleItem.bChanged = (w != tmpDstValue);

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
                            DWORD l = _tcstoul(m_CurrModuleItem.szItemContent,NULL,0);
                            m_CurrModuleItem.bChanged = (l != tmpDstValue);

                            _stprintf(m_CurrModuleItem.szItemContent, _T("0x%X"),tmpDstValue);
                        }
                        break;  
                    default:
                        break;
                    }

                    (*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;

                }
                // @ZHANG Ming : 2003-05-06
                SetModified(TRUE);

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

    ModuleItem hTopItem;     
    unsigned char *pData;

    if((NULL == lpBuffer) || 
        (NULL == lpNumberOfBytesRead))
    {
        return FALSE;
    }

    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }

    *lpNumberOfBytesRead = 0;

    pData = (unsigned char *)lpBuffer;
    for(int nIndex = 0; nIndex < m_lstTopItem.size(); nIndex++)
    {
        hTopItem = m_lstTopItem.at(nIndex);
        if(nItemID == hTopItem.nItemID )
        {
            if(nNumberOfBytesToRead >= hTopItem.nItemLength )
            {
                m_currModuleItemList = m_lstModuleItemPtr.at(hTopItem.nParentItemIndex);
                for(int nCurr = hTopItem.nItemIndex; nCurr < m_currModuleItemList->size(); nCurr++)
                {
                    m_CurrModuleItem = m_currModuleItemList->at(nCurr);
                    if(nItemID != m_CurrModuleItem.nItemID)
                    {
                        *lpNumberOfBytesRead = hTopItem.nItemLength;
                        return TRUE;
                    }
                    switch(m_CurrModuleItem.eItemType)
                    {
                    case CHAR_ITEM_TYPE:                
                        {
                            if((NULL != _tcschr(m_CurrModuleItem.szItemContent, 'x'))||
                                (NULL != _tcschr(m_CurrModuleItem.szItemContent, 'X')))
                            {
                                pData[m_CurrModuleItem.nOffset] = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 16);
                            }
                            else
                            {
                                pData[m_CurrModuleItem.nOffset] = (unsigned char)_tcstol(m_CurrModuleItem.szItemContent, NULL, 10);
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

    for(int nIndex = 0; nIndex < m_lstTopItem.size(); nIndex++)
    {
        hTopItem = m_lstTopItem.at(nIndex);
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

    for(int nIndex = 0; nIndex < m_lstTopItem.size(); nIndex++)
    {
        hTopItem = m_lstTopItem.at(nIndex);
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

    return m_lstTopItem.size();
}


UINT CFileParser::GetNVItemID(int nIndex) const
{
    return m_lstTopItem.at(nIndex).nItemID;
}

const TCHAR* CFileParser::GetNVItemName(int nIndex) const
{
    return m_lstTopItem.at(nIndex).szItemName;
}

// ]]
/*xml file support by xmzhang add begin*
BOOL CFileParser::IsFileRdOnly(const TCHAR *szFileName)
{
assert( NULL != szFileName );

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
//xml file support by xmzhang add end*/
// @ZHANG Ming : 2003-04-18
// [[
BOOL CFileParser::ChangeFileRdOnlyAttri( const TCHAR *szFileName, BOOL bReadOnly )
{
    /*
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

    return ::SetFileAttributes( szFileName, dwAttri );*/
    return TRUE;
}

// Note:
// bAttriChanged records whether the function has changed
// the file's read-only attribute for writing
FILE* CFileParser::OpenFileForWrite(const TCHAR *szFileName, 
                                    BOOL *bAttriChanged, 
                                    BOOL bBinaryMode /* = FALSE */)
{
    assert(NULL != szFileName);    

    if (NULL != bAttriChanged)
    {
        *bAttriChanged = FALSE;
    }

    FILE *pFile = _tfopen( szFileName, bBinaryMode ? _T("wb") : _T("w") );
    if(NULL == pFile)
    {

        // Change the file's read-only attribute and re-opne it
        if (ChangeFileRdOnlyAttri(szFileName, FALSE))
        {
            if (NULL != bAttriChanged)
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
/*xml file support by xmzhang add begin*
BOOL CFileParser::ChangeFileForWrite( const TCHAR *szFileName)
{
BOOL bStatus = FALSE;
assert(NULL != szFileName);    	


// Change the file's read-only attribute and re-opne it
if (ChangeFileRdOnlyAttri(szFileName, FALSE))
{
bStatus = TRUE;
}


return bStatus;
}
//xml file support by xmzhang add end*/

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
    memset(m_szPrjDesc, 0, MAX_ITEM_CONTENT);  
    m_mapIDs.clear();
    m_strErrID = _T("");
    //m_agStrError.RemoveAll();
}

BOOL CFileParser::LoadModule2(TCHAR *szModuleFileName)
{
    BOOL  bStatus = TRUE;
    TCHAR  *pszLine;
    TCHAR  szString[0x40];
    BOOL  bHandleItem;

    if(NULL != m_hModuleFile)
    {
        //Notify that this file parser has loaded file 
        bStatus =  FALSE;
    }
    else
    {
        TCHAR szModuleFileNameTmp[MAX_PATH] = {0};
        _tcscpy(szModuleFileNameTmp, szModuleFileName);
        for (int n = 0; n < _tcslen(szModuleFileNameTmp); n++)
        {
            if ('\\' == szModuleFileNameTmp[n])
            {
                szModuleFileNameTmp[n] == '/';
            }
        }

        FILE *pFile = _tfopen(szModuleFileNameTmp, "rb");

        if(NULL == pFile)
        {
            return FALSE;
        }

        DWORD dwSizeHigh = 0,dwSizeLow = 0;
        fseek(pFile, 0, SEEK_END);
        dwSizeLow = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);


        char* pOriFile = new char[dwSizeLow];
        fread(pOriFile, 1, dwSizeLow, pFile);
        fclose(pFile);

        DWORD dwSize = dwSizeLow / 2;
        TCHAR* pDesFile = new TCHAR[dwSize];
        DWORD dwPos;

        int nLength = strlen(szKey);

        for(int i = 0; i < (int)dwSize; i++)
        {
            dwPos = i * 2;
            pDesFile[i] = (TCHAR)(((pOriFile[dwPos] & 0xf) << 4 ) | ((pOriFile[dwPos + 1] & 0xf0) >> 4));
            pDesFile[i] ^= szKey[i % nLength];
        }

        delete[] pOriFile;

#ifdef _DEBUG
        string strTempFile = szModuleFileName;
        strTempFile += _T(".tmp" );
        FILE *pTempFile = _tfopen( strTempFile, _T("w") );
        if ( pTempFile != NULL )
        {
            fwrite( pDesFile, dwSize, 1, pTempFile );
            fclose( pTempFile );
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
            while(' ' == *pszLine || '\t' == *pszLine)
            {
                pszLine++;
            }
            //This line is comment
            if( ('#' != *pszLine) &&
                ('\n' != *pszLine) && ('\r' != *pszLine))
            {
                szString[0] = '\0';					
                _stscanf(pszLine, _T("%[^=]"), szString);				
                _stscanf(szString, _T("%s"), szString);
                pszLine = _tcschr(pszLine, '=');
                if(pszLine != NULL)
                {
                    pszLine += 1;
                    while (' ' == *pszLine || '\t' == *pszLine)
                    {
                        pszLine++;
                    }

                }

                if(0 == (_tcscmp(szString, _T("MODULE"))))
                {
                    _stscanf(pszLine, _T("%s"),  m_CurrModule.szModuleName);
                }                    
                else if(0 == (_tcscmp(szString, _T("DESCRIPTION"))))
                {
                    _stscanf(pszLine, _T("%s"),  m_CurrModule.szModuleDescription);
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
                            _tprintf(_T("[WARNING] [NVM: %s]: [ItemName: %s] [ItemID: %d] not equal to [ParentID: %d]"),szModuleFileName,
                                m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemID,nParentID);
                            //AfxMessageBox(str);
                            //m_agStrError.push_back(str);

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
                            _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]: item content [%s] is not valid."),szModuleFileName,
                                m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemID,m_CurrModuleItem.szItemContent);
                            //AfxMessageBox(str);
                            //m_agStrError.push_back(str);
                            m_nError++;
                            bStatus = FALSE;
                            m_CurrModuleItem.bChanged = TRUE;
                            m_CurrModuleItem.bExpanded = TRUE;
                        }
                    }
                    //]]

                    BOOL bOK = TRUE;


                    if(m_CurrModuleItem.nParentItemIndex >= (INT)m_CurrModuleItem.nItemIndex)
                    {
                        bOK = FALSE;
                        bStatus = FALSE;
                        _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent index [%d] is not smaller than its index [%d]."),szModuleFileName,
                            m_CurrModuleItem.szItemName,m_CurrModuleItem.nParentItemIndex,m_CurrModuleItem.nItemIndex);							
                        //m_agStrError.Add(str);
                        m_nError++;

                    }

                    if(bOK && m_CurrModuleItem.nItemIndex != (UINT)m_currModuleItemList->size())
                    {
                        _tprintf(_T("[WARNING] [NVM: %s] [ItemName: %s]:item index [%d] is not valid and fixed it as [%d]."),szModuleFileName,
                            m_CurrModuleItem.szItemName,m_CurrModuleItem.nItemIndex,m_currModuleItemList->size());							
                        //m_agStrError.Add(str);
                        m_nError++;
                        m_CurrModuleItem.nItemIndex = m_currModuleItemList->size();
                    }


                    if( bOK && m_CurrModuleItem.nParentItemIndex > -1 )
                    {
                        assert (m_CurrModuleItem.nParentItemIndex < m_currModuleItemList->size() );

                        ModuleItem miParent = m_currModuleItemList->at(m_CurrModuleItem.nParentItemIndex);
                        if( miParent.eItemType !=  ARRAY_ITEM_TYPE &&  
                            miParent.eItemType !=  ASTRING_ITEM_TYPE &&
                            miParent.eItemType !=  WSTRING_ITEM_TYPE &&
                            miParent.eItemType !=  STRUCT_ITEM_TYPE )
                        {									
                            bStatus = FALSE;
                            _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent [%s] is not valid."),szModuleFileName,
                                m_CurrModuleItem.szItemName,miParent.szItemName);		
                            m_nError++;
                            //	m_agStrError.Add(str);
                        }

                    }

                    m_currModuleItemList->push_back(m_CurrModuleItem);
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
                	string strLine(pszLine);
                    CTCharOpr::StrLTrim((TCHAR*)strLine.c_str());	
					CTCharOpr::StrRTrim((TCHAR*)strLine.c_str(),_T('\n'));
					CTCharOpr::StrRTrim((TCHAR*)strLine.c_str(),_T('\r'));
                    // erase redundant string : (null) 
                        
                    if ( 
						0 == _tcslen(strLine.c_str())					||
						0 == _tcsnicmp(strLine.c_str(), _T("(null)"),6) || 
						0 == _tcsicmp(strLine.c_str(),_T("\n")) )
                    {
                        continue;
                    }
                    _tcsncpy(m_CurrModuleItem.szItemDescription, strLine.c_str(),MAX_ITEM_CONTENT);
                }
                else if(0 == (_tcscmp(szString,_T("ITEM_READONLY"))))
                {
                    _stscanf(pszLine,_T("%d"),&m_CurrModuleItem.bReadOnly);
                }
            }

            //pszLine = fgets(szLineContent, MAX_PATH, m_hModuleFile);
            pszLine = _tcstok(NULL,szToken);
        }

        m_lstModule.push_back(m_CurrModule);
        m_lstModuleItemPtr.push_back(m_currModuleItemList);

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

    m_CurrModule = m_lstModule.at(uIndex);
    m_CurrModule.bAppendable = bAppend;
    m_lstModule[uIndex] = m_CurrModule;
}

BOOL CFileParser::GetItemPath(int nModuleIndex, int nIndex,  TCHAR strPath[MAX_PATH])
{
    ModuleItem item = GetModuleItem(nModuleIndex,nIndex);

    TCHAR strTmp[MAX_PATH] = {0};
    if(0 != _tcslen(strPath))
    {
        _stprintf(strTmp, _T(".%s"), strPath);
    }
    else
    {
        _tcscpy(strTmp, strPath);
    }

    _stprintf(strPath, _T("%s%s"), item.szItemName, strTmp);

    if( -1 != item.nParentItemIndex )
    {
        GetItemPath( nModuleIndex,item.nParentItemIndex,strPath );
    }

    return TRUE;
}

BOOL CFileParser::GetItemByName(TCHAR strItemPath[MAX_PATH], int& nModuleIndex, int& nItemIndex)
{
    // Split item path by "\\"
    TCHAR szItemPath[MAX_PATH] = {0};
    _tcscpy(szItemPath,strItemPath);

    vector<string> strPaths;

    TCHAR* pTok = _tcstok(szItemPath, _T("\\"));

    while(0 != pTok)
    {
        strPaths.push_back(pTok);
        pTok = _tcstok(NULL, _T("\\" ));
    }

    //int nL = strPaths.GetSize();

    // Find module
    NVTreeItem* pModule = NULL;
    int i = 0;
    for(; i < m_TreeRoot.pChildren.size(); i++)
    {
        pModule = m_TreeRoot.pChildren[i];

        if(0 != _tcscmp(strPaths[0].c_str(), pModule->strName))
        {
            continue;
        }

        // Find
        nModuleIndex = i;
        break;
    }

    if( i == m_TreeRoot.pChildren.size() )
    {
        // Can not find module
        return FALSE;
    }

    // Find item in module

    nItemIndex = FindItem(pModule,&strPaths,1);

    if( -1 != nItemIndex )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int CFileParser::FindItem( NVTreeItem* pParent,vector<string>* pArrName,int nLevel )
{
    assert(pParent);
    assert(pArrName);
    if(pParent == NULL || pArrName == NULL)
    {
        return -1;
    }

    for( int i=0;i<pParent->pChildren.size();i++ )
    {
        NVTreeItem* pItem = pParent->pChildren[i];
        if(0 != _tcscmp(pItem->strName, pArrName->at(nLevel).c_str()))
        {
            continue;
        }
        else
        {
            if( nLevel == pArrName->size() - 1 )
            {
                // Find
                return pItem->nIndex;
            }
            else
            {
                // Find in child array
                return FindItem(pItem, pArrName, nLevel + 1);
            }
        }
    }

    return -1;
}

void CFileParser::ConstructItemTree()
{
    m_TreeRoot.pParent = NULL;

    // Construct module by module
    for(int i = 0; i < GetModuleCount(); i++)
    {
        NVTreeItem* pModuleItem = new NVTreeItem;
        pModuleItem->pParent = &m_TreeRoot;
        _tcscpy(pModuleItem->strName, GetModule(i).szModuleName);
        pModuleItem->nIndex = i;
        m_TreeRoot.pChildren.push_back(pModuleItem);

        vector<NVTreeItem*> itemList;

        // Construct tree item by item in one module
        for (int j = 0; j < GetModuleItemCount(i); j++)
        {
            NVTreeItem* pItem = new NVTreeItem;
            ModuleItem mi = GetModuleItem(i, j);

            if(mi.nParentItemIndex == -1)
            {
                // first level children
                pItem->pParent = pModuleItem;
                pModuleItem->pChildren.push_back(pItem);
            }
            else
            {
                // lower level children
                NVTreeItem* pParent = itemList[mi.nParentItemIndex];
                pItem->pParent = pParent;
                pParent->pChildren.push_back( pItem );
            }

            pItem->nIndex = j;
            _tcscpy(pItem->strName, mi.szItemName);
            itemList.push_back(pItem);
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
BOOL CFileParser::ImportItem(TCHAR* strFile)	// used for command line mode only
{
    ConstructItemTree();

    BOOL bHasError = FALSE;

    FILE* pFile = _tfopen(strFile, _T("rt")); 

    if(NULL == pFile)
    {

        _tprintf(_T("Open file [%s] failed,error code: %d,\"%s\""),strFile,errno,strerror(errno));
        return FALSE;
    }

    TCHAR strLine[MAX_PATH] = {0};

    int nLineNo = 0;

    while(fgets(strLine, MAX_PATH, pFile))
    {
        nLineNo++;

        TCHAR* pLine = strLine;

        while (0 != *pLine && pLine != (strLine + MAX_PATH))
        {
            if (' ' != (*pLine) && '\t' != (*pLine) && '\r' != (*pLine) && '\n' != (*pLine))
            {
                break;
            }
            pLine++;
        }

        if (0 == _tcslen(pLine))
        {
            memset(strLine, 0, sizeof(strLine));
            continue;
        }


        if(_tcsstr(pLine, _T("//")) == pLine || _tcsstr(pLine, _T("#")) == pLine)
        {
            // Comment line
            memset(strLine, 0, sizeof(strLine));
            continue;
        }

        TCHAR szPath[MAX_PATH]={0},szValue[MAX_PATH]={0};
        _stscanf(pLine,_T("%[^'=']=%s"), szPath, szValue);
		int nLeftLen = _tcslen(szPath)+1;
	 	int nValueLen = _tcslen(strLine) - nLeftLen;
	 	_tcscpy(szValue,strLine+nLeftLen);
		 CTCharOpr::StrLTrim(szValue);
		 CTCharOpr::StrRTrim(szValue);
		 CTCharOpr::StrRTrim(szValue,'\n');
		 CTCharOpr::StrRTrim(szValue,'\r');

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


        if(!ValueIsValid(szValue))
        {
            // szValue is not a valid number
            _tprintf( _T( "The value in line %d is not a number!\n" ),nLineNo );
            //strError += strText;
            bHasError = TRUE; 

            memset(strLine, 0, sizeof(strLine));    
            continue;
        }

        int nModuleIndex,nItemIndex;
        BOOL bRet = GetItemByName(szPath, nModuleIndex, nItemIndex);
        if(bRet)
        {
            ModuleItem mi = GetModuleItem(nModuleIndex, nItemIndex);
		if ( ASTRING_ITEM_TYPE == mi.eItemType)
		{
			CTCharOpr::StrLTrim(szValue);
			CTCharOpr::StrRTrim(szValue);
			TCHAR* pValue=szValue;
			int nLen = _tcslen(pValue);
			if( nLen<2 || (pValue[0] != _T('\"') && pValue[nLen-1] != _T('\"')) )
			{
				bHasError = TRUE;
                		_tprintf( _T( "%s's value is invalid format string : %s\n" ),szPath,szValue);
			}
			else
			{
				if (mi.nArraySize > nLen - 2)
				{
					for(int i=1 ; i<nLen-1; ++i )
					{
						int nChildItemIndex = nItemIndex+i;
						ModuleItem cChildItem = GetModuleItem( nModuleIndex, nChildItemIndex);
						if (CHAR_ITEM_TYPE == cChildItem.eItemType)
						{	
							TCHAR ch = pValue[i];
							memset(cChildItem.szItemContent,0x0,sizeof(cChildItem.szItemContent));
							_stprintf(cChildItem.szItemContent,_T("0x%X"),ch);
							SetModuleItem( nModuleIndex,nChildItemIndex,cChildItem );
						}
					}
					
					//set '\0'
					int nChildItemIndex = nItemIndex+nLen-1;
					if (nChildItemIndex < mi.nArraySize)
					{
						ModuleItem cEndChildItem = GetModuleItem( nModuleIndex,nChildItemIndex );
						memset(cEndChildItem.szItemContent,0x0,sizeof(cEndChildItem.szItemContent));
						_stprintf(cEndChildItem.szItemContent,_T("0x0"));
						SetModuleItem( nModuleIndex,nChildItemIndex,cEndChildItem );
						
					}
					
				}
				else
				{
					bHasError = TRUE;	
					_tprintf( _T( "%s's value length is invalid [nArraySize %d <= %d ]\n" ),szPath,mi.nArraySize,nLen-2);
				}
			}	
			
		}
		else if( ARRAY_ITEM_TYPE != mi.eItemType && 
                STRUCT_ITEM_TYPE != mi.eItemType &&
                ASTRING_ITEM_TYPE != mi.eItemType &&
                WSTRING_ITEM_TYPE != mi.eItemType )
            {
                mi.bChanged = _tcsicmp(mi.szItemContent, szValue);
                _tcscpy(mi.szItemContent, szValue);
                SetModuleItem(nModuleIndex,nItemIndex, mi);
            }
            else
            {
                bHasError = TRUE;
                _tprintf(_T( "%s %s\n"), szPath, _T( "has child items,can not be imported!" ));
                //strError += strText;			
            }
        }
        else
        {
            // Could not find specified item
            bHasError = TRUE;				

            _tprintf(_T( "%s %s\n" ),_T("Can not find item:"), szPath);
            //strError += strText;


        }			
    }





    DestructItemTree();


    return !bHasError;
}

void CFileParser::DestructItemTree()
{
    for( int i=0;i<m_TreeRoot.pChildren.size();i++ )
    {
        DestructSubTree( m_TreeRoot.pChildren.at(i) );
    }

    m_TreeRoot.pChildren.clear();
}

void CFileParser::DestructSubTree( NVTreeItem* pRoot )
{
    assert( pRoot );
    if(pRoot == NULL)
        return;

    for(int i = 0; i < pRoot->pChildren.size(); i++)
    {
        DestructSubTree(pRoot->pChildren[i]);
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
    unsigned int nValue = 0;

    TCHAR szContentTmp[MAX_ITEM_CONTENT]  = {0};

    _tcscpy(szContentTmp, m_CurrModuleItem.szItemContent);

    _tcslwr(szContentTmp);


    string strContent = (string)szContentTmp;
    if( 0 == strContent.find(_T("0x"), 0) )
    {
        _stscanf(szContentTmp, _T("0x%x"), &nValue);
    }
    else
    {
        _stscanf(szContentTmp, _T("%d"), &nValue);
    }
    if(m_bHexFlag)
    {
        _stprintf( szContent, _T("0x%X"), nValue);
    }
    else
    {
        _stprintf( szContent, _T("%u"), nValue);
    }

    _tcscpy(m_CurrModuleItem.szItemContent, szContent);

}
void CFileParser::SetMulFileAg(BOOL bWriteMul,VEC_FILEINFO & vecFileInfo)
{
     m_bWriteMult = bWriteMul;
	m_vecFileInfo.clear();
	m_nMulFileCount = vecFileInfo.size();
    if (m_nMulFileCount > 0)
    {
		m_vecFileInfo.assign(vecFileInfo.begin(), vecFileInfo.end());
    }

}
BOOL CFileParser::IsExistedID(CUIntArray& uIntArray)
{		
    int nNumber = 0;
    for ( int i = 0; i < m_nMulFileCount; i++ )
    {
		if ( IsExistedID(m_vecFileInfo[i].nFileID) )
        {
            uIntArray.push_back(m_vecFileInfo[i].nFileID);
            nNumber++;
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
        m_currModuleItemList = m_lstModuleItemPtr.at(nCurModuleIndex);
        nModuleItemCount = GetModuleItemCount(nCurModuleIndex);
        for(int nCurItemIndex = 0; nCurItemIndex < nModuleItemCount; nCurItemIndex++)
        {
            m_CurrModuleItem = m_currModuleItemList->at(nCurItemIndex);
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
    bool  bFailedFlag = FALSE;
    //string strMsg;
    for ( int i = 0; i < m_nMulFileCount; i++ )
    {
	 ST_FILE_INFO sFileInfo = m_vecFileInfo[i];
        FILE *fpFileCode = _tfopen( sFileInfo.szFilePath, _T("rb") );
        if( NULL == fpFileCode )
        {
            _tprintf(_T("Can't open the file %s, Please check the file is exist!\n"),sFileInfo.szFilePath);
            //Notify that can not open target file
            //strMsg += strTmp;
            bFailedFlag = TRUE;
            continue;
        }
		

		BYTE tmp_file_code[FILE_TOTAL_SIZE];
		int  nFileSize = 0;
		memset( tmp_file_code, 0, FILE_TOTAL_SIZE );
		
		fseek( fpFileCode, 0, SEEK_END);
		nFileSize = ftell(fpFileCode);
		
		if ( nFileSize >= FILE_TOTAL_SIZE )
		{
			_tprintf( _T("FileID %d (file size exceeded) "), sFileInfo.nFileID );
			//strMsg += strTmpMsg;
			bFailedFlag = TRUE;
			fclose(fpFileCode);
			continue;
		}
		
		fseek( fpFileCode, 0, SEEK_SET);  
		
		int nRead = fread( tmp_file_code, sizeof(char), FILE_TOTAL_SIZE, fpFileCode );
		
		if ( nRead <= 0 )
		{
			_tprintf( _T("FileID %d (read failed) "), sFileInfo.nFileID );
			//strMsg += strTmpMsg;
			bFailedFlag = TRUE;
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

    if (bFailedFlag)
    {	      
        return 1;	
    }
    return 0;

}

void CFileParser::SetWriteTail(BOOL nFlag)
{
    m_bTailFlag = nFlag;
}

void  CFileParser::SetVersionFlag(BOOL nFlag)
{
    m_bVerFlag = nFlag;
}

// for fix bin merge
void CFileParser::SetWriteFix(TCHAR* strFixBin)
{
    m_bWriteFix = TRUE;
    _tcscpy(m_szFixBin, strFixBin);	
}

// for hash header path
void CFileParser::SetHashHeaderPath(TCHAR* strHash)
{
    m_bHashHeader = TRUE;
    _tcscpy(m_szHashPath, strHash);

}

/*BOOL CFileParser::CompareHashValue(const unsigned char* pItem)
{
CountMD5(m_mapIDs);
string strOrg, strNew;
for (int i = 0; i < 16; i++)
{
TCHAR strTmp[8];
sprintf((char*)strTmp, _T("%02X"), pItem[i + 4]);
strOrg.append(strTmp);

sprintf((char*)strTmp, _T("%02X"), m_digest[i]);
strNew.append(strTmp);
}

if (0 == strOrg.compare(strNew))
{
return TRUE;
}
return FALSE;
}*/

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
        if (nOffset >=  cbFileSize)
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
        /*
        nOffset += wLen;
        // Adjust for alignment of 4 bytes
        nOffset = (nOffset + 3) & 0xfffffffc;

        if(0 == wItemId)
        continue;
        */
        CMapID::iterator  it = mapIDs.find(wItemId);
        if (mapIDs.end() != it)
        {
            TCHAR strTmp[8];
            sprintf((char*)strTmp, _T("%d  "), wItemId);
            m_strErrID.append(strTmp);
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
        int  nSize = 0;

        sprintf((char*)tmpValue, "%d", iter->first);	
        nSize = strlen((char*)tmpValue);
        MD5Update (&context, (unsigned char*)tmpValue, nSize);


        sprintf((char*)tmpValue, "%d", iter->second);
        nSize = strlen((char*)tmpValue);
        MD5Update (&context, (unsigned char*)tmpValue, nSize);
    }
    MD5Final (m_digest, &context);
}

BOOL CFileParser::GenDeltaIndex( const TCHAR* lpDeltaPath)
{
	
	TCHAR szIndexXml[MAX_PATH] = {0};
	_stprintf(szIndexXml,_T("%s/%s"),lpDeltaPath,OPERATOR_INDEX_XML_NAME);
	 if( -1 == access(szIndexXml, 0) )
	{
		_tprintf("Can not find xml :%s\n",szIndexXml);
		return TRUE;
	}
	CGenDeltaNVIndex cGenIndex;
	
	return cGenIndex.GenDeltaIndex(m_szNvContent,m_dwNVLength,szIndexXml);
}

BOOL CFileParser::GenerateConfigFile(const TCHAR* strPath, TCHAR* strFileName, BOOL bHeaderFile)
{
    BOOL bRet = TRUE;
    string strPathNV;
    string strDetails;
    string strFileNv;

    if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
    ConstructItemTree();
    // find .nv files in strPathNV , and parse them one by one.
    struct dirent *pDirEntry = NULL;

    DIR    *pDir = NULL;
#define FILE_NAME_LENGTH  20
    // debug
    //strcpy(strPath1, "../nvitem/tt");
    if( (pDir = opendir(strPath)) == NULL )
    {
        _tprintf("opendir failed!\n");	  
        return FALSE;
    }
    else
    {    
        std::vector<string>   vecFileName;
        while( NULL != (pDirEntry = readdir(pDir)))
        {
            //_tprintf("pDirEntry->d_name = %s\n", pDirEntry->d_name);
            if(strcmp(pDirEntry->d_name, "..")==0 || strcmp(pDirEntry->d_name, ".")==0)
                continue;

            string strFileName = pDirEntry->d_name;
            vecFileName.push_back(strFileName);
        }
        std::sort(vecFileName.begin(), vecFileName.end());
        int nCountFile = vecFileName.size();
        for (int j = 0; j < nCountFile; ++j)
        {

            m_bNVFlag = FALSE;
            char pFileName[MAX_PATH];	
            char* pTmpName;

            _tcscpy(pFileName, strPath);	
            _tcscat(pFileName, vecFileName[j].c_str());

            //_tprintf("pFileName =   %s \n ", pFileName);

            _tcstok((char*)vecFileName[j].c_str(),  "."); 
            pTmpName = _tcstok(NULL, ".");	  

            if (NULL == pTmpName)
                continue;
            string strTmp(pTmpName);
            if (0 != strTmp.compare("nv"))
                continue;

            memcpy(m_szNvContent + m_dwNVLength,  vecFileName[j].c_str(), strlen(vecFileName[j].c_str()));

            m_dwNVLength += FILE_NAME_LENGTH; 
			*(SHORT*)(m_szNvContent + m_dwNVLength) = 0x01;
            m_dwNVLength += 6;

            bRet = GenerateStruct(pFileName);
			if (!bRet)
			{			
				ClearNVHeader();
				DestructItemTree();
				return FALSE;
			}
			if (!m_bNVFlag)
			{			
				_tprintf(_T("[Warning] File: %s, Not find nv_ver_flag\n"),pFileName);
			}		

        }
        if (0 == m_dwNVLength)
        {
            _tprintf(_T("Wrong Path or No valid NV File\n"));
            ClearNVHeader();
	      	DestructItemTree();
            closedir(pDir);
            return FALSE;
        }

        closedir(pDir);

	if (!GenDeltaIndex(strPath))
	{
		_tprintf(_T("Generate deltanv index fail.\n"));
		ClearNVHeader();
		DestructItemTree();
		return FALSE;
	}

        if (!bHeaderFile)
        {

            bRet = MergeModerm(strPath, strFileName);
            ClearNVHeader();
			DestructItemTree();
            return bRet;
        }

        FILE* pFile = _tfopen(strFileName,  _T("w") );
        if(NULL == pFile)
        {
            _tprintf("opendir failed!\n");	 
            ClearNVHeader();
	      	DestructItemTree();
            return FALSE;
        }
        char strNV[MAX_TOTAL_ITEM_CONTENT];


        m_strNv = "";
        for (int i = 0; i < m_dwNVLength; i++)
        {
            char strTmp[24];

            if (m_dwNVLength -1 == i)
            { 

                _stprintf(strTmp, "0X%02X", m_szNvContent[i]);

            }	
            else
            {
                _stprintf(strTmp, "0X%02X,", m_szNvContent[i]);
            }

            if (0 == (i + 1)%16)
            {
                _tcscat(strTmp, "\n");
            }

            _stprintf(strNV, "%s%s",  m_strNv.c_str(), strTmp);
            m_strNv = strNV;
            //_tprintf("Debug Joly[%d] = %s", i, m_strNv.c_str());
        }	
        // _tprintf("%s\n\n",  m_strNv.c_str());
        fwrite(m_strNv.c_str(), m_strNv.size(), 1, pFile);
        fclose(pFile);

    }

    DestructItemTree();
    return TRUE;
}

// nv force update for merge to moderm.bin
BOOL CFileParser::MergeModerm(const TCHAR* strPath, TCHAR* strFileName)
{
    FILE*  fpTarget = NULL;
    if (0 == _tcslen(strFileName))
    {
        TCHAR strPathFile[MAX_PATH];
        _tcscpy(strPathFile, strPath);
        _tcscat(strPathFile, _T("\delta_nv.bin"));
        //strPathFile.Format(_T("%s\\delta_nv.bin"), strPath);

       
        fpTarget = _tfopen(strPathFile, _T("wb"));
        if(NULL == fpTarget)
        {
            _tprintf(_T("Can't open file %s, write bin failed!\n"), strPathFile);

            return FALSE;
        }

        fwrite(m_szNvContent, 1, m_dwNVLength, fpTarget);	
        fclose(fpTarget);
	  _tprintf(_T("Gen %s\n"),strPathFile);
        return TRUE;
    }
    // merge to moderm.bin
    static const char SEARCH_START_DATA[] = "###DELTA_NV_BEGIN$$$";
    static const char SEARCH_END_DATA[] = "###DELTA_NV_END$$$";

    fpTarget = _tfopen(strFileName, _T("rb"));
    if(NULL == fpTarget)
    {
        _tprintf(_T("Can't open file %s, write bin failed!"), strFileName);
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
        _tprintf(_T("Morderm Bin Not Enough Memory!\n"));
        return FALSE;
    }
	memset(pDataS + 32,0,lBufflen);
    memcpy(pDataS + 32, m_szNvContent, m_dwNVLength);

    fclose(fpTarget);
    fpTarget = _tfopen(strFileName, _T("wb"));
    if(NULL == fpTarget)
    {
        _tprintf(_T("Can't open file %s, write bin failed!\n"), strFileName);

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
    memset(m_szNvContent, 0, MAX_TOTAL_ITEM_CONTENT);
    m_bIsNVImag = FALSE;
    m_bNVFlag = FALSE;
    m_dwFileLength = 0;
    m_dwNVLength = 0;
    m_strNv = "";
}


BOOL CFileParser::GenerateStruct(TCHAR*  strName)
{
    BOOL bRet = TRUE;
    m_bIsNVImag = TRUE;

    bRet = ImportNV(strName);
    if (bRet)
    {
        //add end mark for single nv file
        BYTE strend[2]={0xff,0xff};	
        memcpy(m_szNvContent + m_dwNVLength, strend, 2);

        LONG dwDstValue;
        m_dwNVLength += 2;

        // write single nv file's length
        LONG dwTmpLength = m_dwNVLength - m_dwFileLength;
        CONVERT_INT2(dwTmpLength, dwDstValue);
        *(INT *)&m_szNvContent[m_dwFileLength + 20 + 2] = dwDstValue;

        m_dwFileLength = m_dwNVLength;

    }
    m_bIsNVImag = FALSE;
    return bRet;

}

BOOL CFileParser::SetNVBin(const ModuleItem & NewModuleItem, TCHAR* szValue,DELTA_ITEM_INFO_T& tDeltaItem)
{
	BOOL bRt = TRUE;
	DELTA_ITEM_INFO_T tTmpItemInfo;
	WORD tmpSrcValue, tmpDstValue;
	tmpSrcValue = (WORD)NewModuleItem.nItemID;
	CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
	tTmpItemInfo.wID = tmpDstValue;
	//*(WORD *)&m_szNvContent[m_dwNVLength] = tmpDstValue;

	tmpSrcValue = (WORD)NewModuleItem.nOffset;
	CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
	tTmpItemInfo.wOffset = tmpDstValue;		
	switch(NewModuleItem.eItemType)
	{
	case CHAR_ITEM_TYPE:                
		{          
			tmpSrcValue = sizeof(char);
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			tTmpItemInfo.wLen = tmpDstValue;
			//*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tmpDstValue;
			
			
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
			
			if (  
				tDeltaItem.wID &&
				tDeltaItem.wID == tTmpItemInfo.wID && 
				tTmpItemInfo.wOffset == tDeltaItem.wOffset + tDeltaItem.wLen
				)
			{
				WORD wOrgLen = *(WORD *)&m_szNvContent[m_dwNVLength - tDeltaItem.wLen -2] ;
				*(WORD *)&(m_szNvContent[m_dwNVLength - tDeltaItem.wLen - 2]) = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
				tDeltaItem.wLen = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
			}
			else
			{

				tDeltaItem.wID = tTmpItemInfo.wID;
				tDeltaItem.wOffset = tTmpItemInfo.wOffset;
				tDeltaItem.wLen = tTmpItemInfo.wLen;
				*(WORD *)&m_szNvContent[m_dwNVLength] = tTmpItemInfo.wID;
				*(WORD *)&m_szNvContent[m_dwNVLength + 2] = tTmpItemInfo.wOffset;
				*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tTmpItemInfo.wLen;
				m_dwNVLength += 6;
			
			}

			m_szNvContent[m_dwNVLength] = (unsigned char)tmpDstValue;
		}
		m_dwNVLength += 1;
		break;
	case SHORT_ITEM_TYPE:
		{	
			tmpSrcValue = sizeof(short);
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			tTmpItemInfo.wLen = tmpDstValue;
			//*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tmpDstValue;
			
			//m_dwNVLength += 6;
			
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
			if (  
				tDeltaItem.wID &&
				tDeltaItem.wID == tTmpItemInfo.wID && 
				tTmpItemInfo.wOffset == tDeltaItem.wOffset + tDeltaItem.wLen
				)
			{
				WORD wOrgLen = *(WORD *)&m_szNvContent[m_dwNVLength - tDeltaItem.wLen -2] ;
				*(WORD *)&m_szNvContent[m_dwNVLength-tDeltaItem.wLen - 2] = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
				tDeltaItem.wLen = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
			}
			else
			{
				
				tDeltaItem.wID = tTmpItemInfo.wID;
				tDeltaItem.wOffset = tTmpItemInfo.wOffset;
				tDeltaItem.wLen = tTmpItemInfo.wLen;
				*(WORD *)&m_szNvContent[m_dwNVLength] = tTmpItemInfo.wID;
				*(WORD *)&m_szNvContent[m_dwNVLength + 2] = tTmpItemInfo.wOffset;
				*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tTmpItemInfo.wLen;
				m_dwNVLength += 6;
				
			}

			CONVERT_SHORT2(((WORD)tmpSrc), tmpDstValue);
			*(WORD *)&m_szNvContent[m_dwNVLength] = tmpDstValue;                                  
		}
		m_dwNVLength += 2;
		break;
	case LONG_ITEM_TYPE:
		{            										
			
			tmpSrcValue = sizeof(long);
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			tTmpItemInfo.wLen = tmpDstValue;
			//*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tmpDstValue;
			
			//m_dwNVLength += 6;
			
						
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
			if (  
				tDeltaItem.wID &&
				tDeltaItem.wID == tTmpItemInfo.wID && 
				tTmpItemInfo.wOffset == tDeltaItem.wOffset + tDeltaItem.wLen
				)
			{
				WORD wOrgLen = *(WORD *)&m_szNvContent[m_dwNVLength - tDeltaItem.wLen -2] ;
				*(WORD *)&m_szNvContent[m_dwNVLength-tDeltaItem.wLen - 2] = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
				tDeltaItem.wLen = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
			}
			else
			{	
				tDeltaItem.wID = tTmpItemInfo.wID;
				tDeltaItem.wOffset = tTmpItemInfo.wOffset;
				tDeltaItem.wLen = tTmpItemInfo.wLen;
				*(WORD *)&m_szNvContent[m_dwNVLength] = tTmpItemInfo.wID;
				*(WORD *)&m_szNvContent[m_dwNVLength + 2] = tTmpItemInfo.wOffset;
				*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tTmpItemInfo.wLen;
				m_dwNVLength += 6;				
			}
			CONVERT_INT2(dwSrcValue, dwDstValue);							
			*(DWORD *)&m_szNvContent[m_dwNVLength] = dwDstValue;  
		}
		m_dwNVLength += 4;
		break;  	
	case ASTRING_ITEM_TYPE:
		{
			int nLen = _tcslen(szValue);
			
			if( nLen<2 || (szValue[0] != '\"' && szValue[nLen-1] != '\"') )
			{
				_tprintf(_T("Invalid format string : %s \n"),szValue);
				return FALSE;
			}
			const char* pValue = szValue;
			
			tmpSrcValue = nLen - 2;
			if (tmpSrcValue > NewModuleItem.nArraySize)
			{
				return FALSE;
			}
			CONVERT_SHORT2(tmpSrcValue, tmpDstValue);
			tTmpItemInfo.wLen = tmpDstValue;
			if (tmpSrcValue<NewModuleItem.nArraySize)
			{
				CONVERT_SHORT2(tmpSrcValue+1, tmpDstValue);
				tTmpItemInfo.wLen = tmpDstValue;
			}

			if (  
				tDeltaItem.wID &&
				tDeltaItem.wID == tTmpItemInfo.wID && 
				tTmpItemInfo.wOffset == tDeltaItem.wOffset + tDeltaItem.wLen
				)
			{
				WORD wOrgLen = *(WORD *)&m_szNvContent[m_dwNVLength - tDeltaItem.wLen -2] ;
				*(WORD *)&m_szNvContent[m_dwNVLength-tDeltaItem.wLen - 2] = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
				tDeltaItem.wLen = (WORD)(wOrgLen + tTmpItemInfo.wLen); 
			}
			else
			{	
				tDeltaItem.wID = tTmpItemInfo.wID;
				tDeltaItem.wOffset = tTmpItemInfo.wOffset;
				tDeltaItem.wLen = tTmpItemInfo.wLen;
				*(WORD *)&m_szNvContent[m_dwNVLength] = tTmpItemInfo.wID;
				*(WORD *)&m_szNvContent[m_dwNVLength + 2] = tTmpItemInfo.wOffset;
				*(WORD *)&m_szNvContent[m_dwNVLength + 4] = tTmpItemInfo.wLen;
				m_dwNVLength += 6;				
			}
			if (tmpSrcValue)
			{
				memcpy(m_szNvContent+m_dwNVLength,pValue+1,tmpSrcValue);
				m_dwNVLength += tmpSrcValue;
			}
			if (tmpSrcValue<NewModuleItem.nArraySize)
			{
				m_szNvContent[m_dwNVLength] = '\0';
				m_dwNVLength +=1;
			}
			
		}
		break;
	default:
		bRt = FALSE;
		break;
		
    }     
    return bRt;
	
}

BOOL CFileParser::ImportNV(TCHAR*  strFile)
{

    BOOL bHasError = FALSE;
    DWORD dwLength = m_dwNVLength - 6; // add by Joly
    DELTA_ITEM_INFO_T tDeltaItem;

    FILE* pFile = _tfopen(strFile, _T("rt")); 		
    if(NULL == pFile)
    {
        return FALSE;
    }

    TCHAR strLine[MAX_PATH] = {0};

    int nLineNo = 0;

    while(fgets(strLine, MAX_PATH, pFile))
    {
        nLineNo++;

        TCHAR* pLine = strLine;

        //_tprintf("Start Import NV, strLine = %s", strLine);

        while (0 != *pLine && pLine != (strLine + MAX_PATH))
        {
            if (' ' != (*pLine) && '\t' != (*pLine) && '\r' != (*pLine) && '\n' != (*pLine))
            {
                break;
            }
            pLine++;
        }

        if (0 == _tcslen(pLine))
        {
            memset(strLine, 0, sizeof(strLine));
            continue;
        }


        if(_tcsstr(pLine, _T("//")) == pLine || _tcsstr(pLine, _T("#")) == pLine)
        {
            // Comment line
            memset(strLine, 0, sizeof(strLine));
            continue;
        }

        TCHAR szPath[MAX_PATH]={0},szValue[MAX_PATH]={0};
        _stscanf(pLine,_T("%[^'=']=%s"), szPath, szValue);
		 int nLeftLen = _tcslen(szPath)+1;
		 int nValueLen = _tcslen(strLine) - nLeftLen;
		 _tcscpy(szValue,strLine+nLeftLen);
		 CTCharOpr::StrLTrim(szValue);
		 CTCharOpr::StrRTrim(szValue);
		 CTCharOpr::StrRTrim(szValue,'\n');
		 CTCharOpr::StrRTrim(szValue,'\r');

        /*CR212418 xmzhang add begin*/
        TCHAR *tmp = szPath;
        UINT lentmp = _tcslen(szPath)-1;
        tmp += (_tcslen(szPath)-1);
		
        while((0x20 == *tmp||0x09 == *tmp)&&( lentmp > 0))
        {
            *tmp = 0x00;
            tmp--;
            lentmp--;
        }
        /*CR212418 xmzhang add end*/


        if(!ValueIsValid(szValue))
        {
            // szValue is not a valid number	
            _tprintf( _T( "FileName: %s,LineNo: %d,%s has not a valid number!\n" ), strFile, nLineNo, szPath);
            //strError += strText;
            bHasError = TRUE; 

            memset(strLine, 0, sizeof(strLine));    
            break;
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
                _tprintf( _T( "File: %s, %s : wrong flag value\n" ), strFile, szPath );

                break;
            }
            CONVERT_SHORT2(((WORD)tmpSrcValue), tmpDstValue);
            *(WORD *)&m_szNvContent[dwLength] = tmpDstValue;  
            m_bNVFlag = TRUE;
            continue;				
        }
        //]]

        int nModuleIndex,nItemIndex;
        BOOL bRet = GetItemByName(szPath, nModuleIndex, nItemIndex);
        if(bRet)
        {
            ModuleItem mi = GetModuleItem(nModuleIndex, nItemIndex);
            if( ARRAY_ITEM_TYPE != mi.eItemType && 
                STRUCT_ITEM_TYPE != mi.eItemType &&
                //ASTRING_ITEM_TYPE != mi.eItemType &&
                WSTRING_ITEM_TYPE != mi.eItemType )
            {
                //[[ modify by for SamSung nv update
                if (m_bIsNVImag) 
                {
                    CTCharOpr::StrLTrim(szValue);
			 		CTCharOpr::StrRTrim(szValue);
                    if (!SetNVBin(mi, szValue,tDeltaItem))
                    {
                        _tprintf(_T( "File:%s, Line: %d, %s wrong item value\n" ), strFile, nLineNo, szPath);
                        break;
                    }

                }
                else
                {
                    mi.bChanged = _tcsicmp(mi.szItemContent, szValue);
                    _tcscpy(mi.szItemContent, szValue);
                    SetModuleItem(nModuleIndex,nItemIndex, mi);
                }
                //]]
            }
            else
            {
                bHasError = TRUE;
                _tprintf(_T(  "File: %s,Line: %d, %s %s\n" ), strFile, nLineNo, szPath, _T( "has child items,can not be imported!" ));
                //strError += strText;			
            }
        }
        else
        {
            // Could not find specified item
            bHasError = TRUE;				

            _tprintf(_T( "File: %s,Line: %d, %s %s\n" ), strFile, nLineNo, _T("Can not find item:"), szPath);
            //strError += strText;


        }			

    }

    return !bHasError;

}
// ]]

BOOL CFileParser::LoadProjectXML(TCHAR *szPrjFile,BOOL bCheckItemName /*= TRUE*/)
{
	BOOL   bRet = TRUE;
	TCHAR  szPrjDir[MAX_PATH]	= {0};
    ClearProjectInfo();

    TiXmlDocument doc;
    if(!doc.LoadFile(szPrjFile))
    {
        _tprintf(_T("LoadProjectXML [%s] fail. \n"),szPrjFile);
        return FALSE;
    }

	_tcscpy(m_szPrjFileName, szPrjFile);
    _tcscpy(m_szTargetFileName, _T("") );
	_tcscpy(szPrjDir, m_szPrjFileName);
    TCHAR *pResult = _tcsrchr(szPrjDir,_T('/'));
    if( NULL == pResult)
    {
    	_tprintf(_T("Get project path [%s] fail. \n"),szPrjFile);
        return FALSE;
    }
    *(pResult + 1) = _T('\0');
    TiXmlElement * pRoot = doc.RootElement();
    if (NULL == pRoot )
    {
        _tprintf(_T("No find Root Element in ProjectXML[%s]. \n"),szPrjFile);
        return FALSE;
    }

	//Parse project attribute
	TCHAR szPrjVer[MAX_PATH] = {0};
	_tcscpy(szPrjVer, pRoot->Attribute("version") );
	if(0 != _tcsicmp(szPrjVer,XML_VERSION))
	{
		_tprintf(_T("Current NV tool just support %s project's version,but this project version is %s,Please upgrade nv tool.\n"),XML_VERSION,szPrjVer);
		return FALSE;
	}
	_tcscpy(m_szPrjName, pRoot->Attribute("name") );
	m_nAlignment=atoi(pRoot->Attribute("alignment"));
    _tcscpy(m_szTargetFileName, szPrjDir);
	_tcscat(m_szTargetFileName, pRoot->Attribute("target"));
	_tcscpy(m_szPrjDesc,pRoot->Attribute("description"));


	//Load module attribute
    TiXmlElement * pModuleEle = NULL;
    for(
		pModuleEle = pRoot->FirstChildElement(_T("MODULE"));
		pModuleEle != NULL;
		pModuleEle = pModuleEle->NextSiblingElement(_T("MODULE")) 
	  )
    {
        //_tprintf(_T("MODULES name:%s \n"),pModuleEle->Attribute("name"));
		TCHAR szModuleFile[MAX_PATH] = {0};
		_tcscpy(szModuleFile, szPrjDir);
		_tcscat(szModuleFile, pModuleEle->Attribute("name"));
		
		if (!LoadModuleXML(szModuleFile))
		{
			_tprintf(_T("LoadModuleXML [%s] fail. \n"),szModuleFile);
			bRet = FALSE;
			break;
		}
		++m_nModuleCount;
    }
	if (!m_strErrID.empty())
    {
        _tprintf(_T("Exist repeated IDs: %s\n"), m_strErrID.c_str());
        return FALSE;
    }	
	if(bRet)
	{
		if(bCheckItemName)
		{
			CheckNVModule();
			CheckModuleItem();
		}
	}
    return bRet;
}
BOOL CFileParser::LoadModuleXML(TCHAR *szModuleFile)
{
	BOOL   bRet = TRUE;
    TiXmlDocument doc;
    if(!doc.LoadFile(szModuleFile))
    {
        _tprintf(_T("LoadModuleXML [%s] fail. \n"),szModuleFile);
        return FALSE;
    }
	memset(&m_CurrModule, 0, sizeof(Module));
	_tcscpy(m_CurrModule.szModuleFileName , szModuleFile);
	m_currModuleItemList = new ModuleItem_List;
	if(NULL == m_currModuleItemList)
	{
		_tprintf(_T("new ModuleItem_List fail. \n"));
        return FALSE;
	}
    TiXmlElement * pRoot = doc.RootElement();
    if (NULL == pRoot )
    {
        _tprintf(_T("No find Root Element in ModuleXML[%s]. \n"),szModuleFile);
        return FALSE;
    }
	
	//parse module attribute
	TCHAR szMduVer[MAX_PATH] = {0};
	_tcscpy(szMduVer,pRoot->Attribute("version"));
	if (0 != _tcsicmp(szMduVer,XML_VERSION))
	{
		_tprintf(_T("Current NV tool just support %s module's version,But this module [%s] version is %s,Please upgrade nv tool.\n"),
			XML_VERSION,szModuleFile,szMduVer);
		return FALSE;
	}
	
	_tcscpy(m_CurrModule.szModuleName,pRoot->Attribute("name"));
	_tcscpy(m_CurrModule.szModuleDescription,pRoot->Attribute("description"));

	//Load items in module
    bRet = LoadElement(pRoot,ROOT_ELEMENT,szModuleFile);
	m_lstModule.push_back(m_CurrModule);
	m_lstModuleItemPtr.push_back(m_currModuleItemList);
    return bRet;

}

ITEMTYPE CFileParser::GetItemType(const TCHAR* szType)
{
	ITEMTYPE eType = NONE_ITEM_TYPE;
	if(NULL == szType || 0 == _tcslen(szType))
	{
		return eType;
	}
    for(int k=0;k<9;++k)
    {
        if(k!=3)
        {
            if(0==_tcsicmp(szType,szItemType[k]))
            {
                eType=(ITEMTYPE)k;
				break;
            }
        }
    }
	return eType; 
}
BOOL CFileParser::LoadElement(TiXmlElement* pParentEle,int nParentItemIndex,TCHAR *szModuleFile,UINT nItemID/*=0*/,ITEMTYPE eCurItemType/*=NONE_ITEM_TYPE*/)
{
    if (NULL == pParentEle)
    {
        return TRUE;
    }
	BOOL bRet 			= TRUE;
	int nParentIndex	= nParentItemIndex;
	UINT nParentID 		= 0;
    INT  nParentIdx		= -1;
    TiXmlElement * pItemEle = NULL;
    for( 
		pItemEle = pParentEle->FirstChildElement(_T("ITEM"));
        pItemEle != NULL;
        pItemEle = pItemEle->NextSiblingElement(_T("ITEM")) 
	  )
    {
    	memset(&m_CurrModuleItem, 0, sizeof(ModuleItem));
		ITEMTYPE eSubItemType=eCurItemType;
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
			
		_tcscpy(m_CurrModuleItem.szItemName, pItemEle->Attribute("name") );
		if(pItemEle->Attribute("id"))
		{
			m_CurrModuleItem.nItemID=atoi(pItemEle->Attribute("id"));
		}
		
		if(pItemEle->Attribute("count"))
		{
			m_CurrModuleItem.nArraySize=atoi(pItemEle->Attribute("count"));
		}
		if(pItemEle->Attribute("length"))
		{
			m_CurrModuleItem.nItemLength=atoi(pItemEle->Attribute("length"));
		}
		if(pItemEle->Attribute("offset"))
		{
			m_CurrModuleItem.nOffset=atoi(pItemEle->Attribute("offset"));
		}
		if(pItemEle->Attribute("value"))
		{
			_tcscpy(m_CurrModuleItem.szItemContent, pItemEle->Attribute("value") );
		}
		if(pItemEle->Attribute("readOnly"))
		{
			m_CurrModuleItem.bReadOnly=atoi(pItemEle->Attribute("readOnly"));
		}

		if(pItemEle->Attribute("type"))
		{
			m_CurrModuleItem.eItemType=GetItemType(pItemEle->Attribute("type"));
			if (ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				eSubItemType = CHAR_ITEM_TYPE;
			}
			else if (WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType)
			{
				eSubItemType = SHORT_ITEM_TYPE;
			}
		}
		if(pItemEle->Attribute("subtype"))
		{
			eSubItemType=GetItemType(pItemEle->Attribute("subtype"));
		}
		
		
		TCHAR* pDesc = (TCHAR*)(pItemEle->Attribute("desc"));
		CTCharOpr::StrLTrim(pDesc);
		if (pDesc && _tcsncmp(pDesc, _T("(null)"),6))
		{
			_tcsncpy(m_CurrModuleItem.szItemDescription, pDesc,MAX_ITEM_CONTENT);
		}

		// [[ add for check repeated ID
		if ((int)nParentItemIndex == ROOT_ELEMENT)
        {
            CMapID::iterator  it = m_mapIDs.find(m_CurrModuleItem.nItemID);
            if (m_mapIDs.end() != it)
            {
                //string strTmp;
                TCHAR strTmp[8];
                _stprintf((char*)strTmp, _T("%d "), m_CurrModuleItem.nItemID);
                m_strErrID.append(strTmp);
                //m_strErrID += strTmp;
            }
            else
            {
                m_mapIDs.insert(CMapID::value_type(m_CurrModuleItem.nItemID, m_CurrModuleItem.nItemLength));
            }
        }
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
                
				_tprintf(_T("[WARNING] [NVM: %s]: [ItemName: %s] [ItemID: %d] not equal to [ParentID: %d] and fixed."),
					szModuleFile,
				    m_CurrModuleItem.szItemName,
				    m_CurrModuleItem.nItemID,
				    nParentID);
				m_CurrModuleItem.nItemID = nParentID;
                
            }
        }

		if( 
			m_CurrModuleItem.eItemType == CHAR_ITEM_TYPE ||
            m_CurrModuleItem.eItemType == SHORT_ITEM_TYPE || 
            m_CurrModuleItem.eItemType == LONG_ITEM_TYPE
          )
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
                _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item content [%s] is not valid."),szModuleFile,
                    m_CurrModuleItem.szItemName,m_CurrModuleItem.szItemContent);
                //AfxMessageBox(str);

                bRet = FALSE;
                m_CurrModuleItem.bChanged = TRUE;
                m_CurrModuleItem.bExpanded = TRUE;

                m_CurrModule.bExpanded = TRUE;

                int nPIdx = m_CurrModuleItem.nParentItemIndex;
                while (nPIdx != -1)
                {
                    ModuleItem mi = m_currModuleItemList->at(nPIdx);
                    mi.bExpanded = TRUE;
                    (*m_currModuleItemList)[nPIdx] = mi;
                    nPIdx = mi.nParentItemIndex;
                }
				break;
            }
		}
		BOOL bOK = TRUE;

        if(m_CurrModuleItem.nParentItemIndex >= (INT)m_CurrModuleItem.nItemIndex)
        {
            bOK = FALSE;
            bRet = FALSE;
            _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent index [%d] is not smaller than its index [%d]."),szModuleFile,
                m_CurrModuleItem.szItemName,m_CurrModuleItem.nParentItemIndex,m_CurrModuleItem.nItemIndex);		
			break;


        }

        if (bOK && m_CurrModuleItem.nItemIndex != (UINT)m_currModuleItemList->size())
        {
            _tprintf(_T("[WARNING] [NVM: %s] [ItemName: %s]:item index [%d] is not valid and fixed it as [%d]."),szModuleFile,
                m_CurrModuleItem.szItemName, m_CurrModuleItem.nItemIndex,m_currModuleItemList->size());							

            m_CurrModuleItem.nItemIndex = m_currModuleItemList->size();
        }


        if (bOK && m_CurrModuleItem.nParentItemIndex > -1)
        {
            assert(m_CurrModuleItem.nParentItemIndex < (int)m_currModuleItemList->size() );

            ModuleItem miParent = m_currModuleItemList->at(m_CurrModuleItem.nParentItemIndex);
            if( miParent.eItemType !=  ARRAY_ITEM_TYPE &&  
                miParent.eItemType !=  ASTRING_ITEM_TYPE &&
                miParent.eItemType !=  WSTRING_ITEM_TYPE &&
                miParent.eItemType !=  STRUCT_ITEM_TYPE )
            {									
                bRet = FALSE;
                _tprintf(_T("[ERROR] [NVM: %s] [ItemName: %s]:item parent [%s] is not valid."),szModuleFile,
                    m_CurrModuleItem.szItemName,miParent.szItemName);
				break;

            }

        }

		FilterContent(szModuleFile);
        m_currModuleItemList->push_back(m_CurrModuleItem);
        m_CurrModule.nItemCount += 1;
		
		if(!LoadElement(pItemEle,m_CurrModuleItem.nItemIndex,szModuleFile,m_CurrModuleItem.nItemID,eSubItemType))
		{
			_tprintf(_T("LoadElement [%s] [index=%d] fail. \n"),szModuleFile,m_CurrModuleItem.nItemIndex);
			bRet = FALSE;
			break;
		}
        
    }

	//m_CurrModuleItem.nItemIndex=m_CurrModule.nItemCount;
    //m_CurrModuleItem.nParentItemIndex=nParentIndex;
    return bRet;
}

BOOL CFileParser::SaveProjectXML()
{	
	BOOL bOK = FALSE;
	if(FALSE == GenerateOffset())
    {
        return FALSE;
    }
	TCHAR*			pFileName 	= NULL;
	TiXmlDocument* 	pXmlDocs 	= NULL;  
    TiXmlElement* 	pPrjEle  	= NULL;
	do
	{
		pXmlDocs = new TiXmlDocument();  
		if(NULL == pXmlDocs)
		{
			_tprintf(_T("new TiXmlDocument fail on SaveProjectXML.\n"));
			break;
		}
    	pPrjEle  = new TiXmlElement("PROJECT");
		if(NULL == pXmlDocs)
		{
			_tprintf(_T("new TiXmlElement fail on SaveProjectXML.\n"));
			break;
		}

		//add project attribute 
		pFileName = _tcsrchr(m_szTargetFileName,_T('/'));
		if(NULL == pFileName)
		{
			_tprintf(_T("Invalid Target file name [%s].\n"),m_szTargetFileName);
			break;
		}
		pFileName = pFileName + 1;
		pPrjEle->SetAttribute("version",XML_VERSION);
		pPrjEle->SetAttribute("name", m_szPrjName);
		pPrjEle->SetAttribute("alignment", m_nAlignment);  
		pPrjEle->SetAttribute("target",pFileName);
		
		CTCharOpr::StrLTrim(m_szPrjDesc,_T('\"'));
		CTCharOpr::StrRTrim(m_szPrjDesc,_T('\"'));
		pPrjEle->SetAttribute("description", m_szPrjDesc); 
		pXmlDocs->LinkEndChild(pPrjEle);

		//write module list part
		BOOL bBreaked = FALSE;
		for(UINT Index = 0; Index < m_nModuleCount; Index++)
		{
			m_CurrModule = m_lstModule[Index];
			
			//Remove the path info
			pFileName = _tcsrchr(m_CurrModule.szModuleFileName,_T('/'));
			
			if( NULL == pFileName )
			{
				_tprintf(_T("Invalid module file name [%s].\n"),m_CurrModule.szModuleFileName);
				bBreaked = TRUE;
				break;
			}
			pFileName = pFileName + 1;
			if(!SaveModuleXML(Index))
			{
				_tprintf(_T("SaveModuleXML fail [Index=%d].\n"),Index);
				bBreaked = TRUE;
			}

			TiXmlElement* pTmpEle = new TiXmlElement("MODULE");  
			if(NULL == pTmpEle)
			{
				_tprintf(_T("new TiXmlElement[%d] fail.\n"),Index);
				bBreaked = TRUE;
				break;
			}
			pTmpEle->SetAttribute("name", pFileName);  
			pPrjEle->LinkEndChild(pTmpEle);  
			
		}
		if(!bBreaked)
		{
			bOK = pXmlDocs->SaveFile(m_szPrjFileName);
		}

	}while(0);
	SAFE_DELETE(pXmlDocs);
	return bOK;
}

BOOL CFileParser::SaveModuleXML(INT nModuleIndex)
{
	BOOL bOK = FALSE;
    TiXmlElement* 	pModuleEle 	= NULL;  
	TiXmlElement* 	pParentElement	= NULL;
	TiXmlElement* 	pNewChildElement= NULL;
	TiXmlDocument 	cXmlDocs;
	do
	{
		m_currModuleItemList 		= m_lstModuleItemPtr.at(nModuleIndex);
        m_CurrModule.nItemCount  	= m_currModuleItemList->size();
		
    	pModuleEle 	= new TiXmlElement("MODULE"); 
		if(NULL == pModuleEle)
		{
			_tprintf(_T("new TiXmlElement fail on SaveModuleXML.\n"));
			break;
		}
		//add module attribute 
		pModuleEle->SetAttribute("version",XML_VERSION);
		pModuleEle->SetAttribute("name",m_CurrModule.szModuleName);
		CTCharOpr::StrLTrim(m_CurrModule.szModuleDescription,_T('\"'));
		CTCharOpr::StrRTrim(m_CurrModule.szModuleDescription,_T('\"'));
		pModuleEle->SetAttribute("description", m_CurrModule.szModuleDescription); 
		cXmlDocs.LinkEndChild(pModuleEle);


		//add item to module
		BOOL bBreaked 				= FALSE;
		for(UINT Index = 0; Index < m_CurrModule.nItemCount; ++Index)
        {       
        	ModuleItem     cParentModuleItem;
			TCHAR szItemID[MAX_PATH] 	= {0};
			TCHAR szTmpStr[MAX_PATH]	= {0};
			pNewChildElement = new TiXmlElement("ITEM");
			if(NULL == pNewChildElement)
			{
				_tprintf(_T("new pNewChildElement fail on SaveModuleXML.\n"));
				bBreaked = TRUE;
				break;
			}
            m_CurrModuleItem = m_currModuleItemList->at(Index);
			pNewChildElement->SetAttribute("name", m_CurrModuleItem.szItemName);  
            if(ROOT_ELEMENT!=m_CurrModuleItem.nParentItemIndex)
            {
                cParentModuleItem = m_currModuleItemList->at(m_CurrModuleItem.nParentItemIndex);
                pParentElement = cParentModuleItem.pItemEle;
            }
			else
			{
				_stprintf(szTmpStr,_T("%u"),m_CurrModuleItem.nItemID);
				pNewChildElement->SetAttribute("id", szTmpStr);  
			}
			
			
			if( 
				ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType	||
				ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType
			 )
            {
				
				if (ARRAY_ITEM_TYPE == m_CurrModuleItem.eItemType)
				{
					pNewChildElement->SetAttribute("type",szItemType[m_CurrModuleItem.eItemType]); 
					ModuleItem stChildModuleItem = m_currModuleItemList->at(Index+1);
					pNewChildElement->SetAttribute("subtype",szItemType[stChildModuleItem.eItemType]);
				}
				else
				{
					if ( 
						(ROOT_ELEMENT != m_CurrModuleItem.nParentItemIndex && ARRAY_ITEM_TYPE != cParentModuleItem.eItemType) ||
						ROOT_ELEMENT == m_CurrModuleItem.nParentItemIndex
					  )
					{
						pNewChildElement->SetAttribute("type",szItemType[m_CurrModuleItem.eItemType]);
					}
				}     
				_stprintf(szTmpStr,_T("%u"),m_CurrModuleItem.nArraySize);
                pNewChildElement->SetAttribute("count",szTmpStr);           
			}
			else
			{
				if(
					ROOT_ELEMENT != m_CurrModuleItem.nParentItemIndex	&&							
					(
					ARRAY_ITEM_TYPE   == cParentModuleItem.eItemType	||
					ASTRING_ITEM_TYPE == cParentModuleItem.eItemType	||
					WSTRING_ITEM_TYPE == cParentModuleItem.eItemType	
					)				
				 )
				{
					;
				}
				else
				{
					pNewChildElement->SetAttribute("type",szItemType[m_CurrModuleItem.eItemType]);
				}
			}

			if(
				ASTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
                WSTRING_ITEM_TYPE == m_CurrModuleItem.eItemType ||
				STRUCT_ITEM_TYPE  == m_CurrModuleItem.eItemType ||// by Joly
				ARRAY_ITEM_TYPE   == m_CurrModuleItem.eItemType 
			  )  //  by Joly
            {
                ;
            }
            else
            {				
				FormatContent();  // add by Joly 
                pNewChildElement->SetAttribute("value", m_CurrModuleItem.szItemContent );
            }
			if ( 0 != _tcslen(m_CurrModuleItem.szItemDescription))
			{
				CTCharOpr::StrLTrim(m_CurrModuleItem.szItemDescription,_T('\"'));
				CTCharOpr::StrRTrim(m_CurrModuleItem.szItemDescription,_T('\"'));
				pNewChildElement->SetAttribute("desc", m_CurrModuleItem.szItemDescription  );
			}
			else
			{
				pNewChildElement->SetAttribute("desc", "");
			}
			if(m_CurrModuleItem.bReadOnly)
            {
                pNewChildElement->SetAttribute("readOnly",m_CurrModuleItem.bReadOnly);
            }
            m_CurrModuleItem.pItemEle = pNewChildElement;
			(*m_currModuleItemList)[m_CurrModuleItem.nItemIndex] = m_CurrModuleItem;
			if(ROOT_ELEMENT !=m_CurrModuleItem.nParentItemIndex)
            {                
                pParentElement->LinkEndChild(pNewChildElement);
            }
            else
            {
                pModuleEle->LinkEndChild(pNewChildElement); 
            }

		}

		if(!bBreaked)
		{
			bOK = cXmlDocs.SaveFile(m_CurrModule.szModuleFileName);
		}
		
	}while(0);
	return bOK;
}
