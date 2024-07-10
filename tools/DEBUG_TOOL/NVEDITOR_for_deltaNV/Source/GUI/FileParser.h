// FileParser.h: interface for the CFileParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPARSER_H__590D6B21_39F3_4183_A578_43F1176C31C6__INCLUDED_)
#define AFX_FILEPARSER_H__590D6B21_39F3_4183_A578_43F1176C31C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

#include "NVItemDef.h"
#import  "msxml3.dll"/*xml file support by xmzhang add*/
#pragma warning(push,3)
#include <map>
#include <vector>
#pragma warning( pop )
using namespace std;


#define SAFE_DELETE_ARRAY(p)  \
	do \
{\
	if (p != NULL) {\
	delete []p; \
	p=NULL;\
	}\
} while(0) 

#define PUMP_MESSAGES()  \
    do \
{\
    MSG msg;\
	CWaitCursor wc;\
    while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))\
{\
	TranslateMessage(&msg);\
	DispatchMessage(&msg);\
}\
} while(0) 

//define item stucture
/*lint -save -e756 */
typedef struct ModuleItem_Stru
{  
	UINT      nOffset;
	UINT      nItemIndex;
	INT       nParentItemIndex;
	UINT      nItemID;
	ITEMTYPE  eItemType;
	UINT      nArraySize;
	UINT      nItemLength;
	TCHAR     szItemName[/*32*/MAX_NAME_LENGTH];          // Item Name
	TCHAR     szItemContent[/*32*/MAX_ITEM_CONTENT];      // Array Value
	TCHAR*    szItemDescription/*[/ *MAX_ITEM_CONTENT* /MAX_ITEM_DESCPT]*/;  
	BOOL      bReadOnly;
	BOOL      bExpanded;
	BOOL      bChanged;
	BOOL      bLoadFailed;
	BOOL	  bCustItem;
	BOOL      bCustBase;
	MSXML2::IXMLDOMElementPtr     nXmlPtr;/*xml file support by xmzhang add*/
	
	ModuleItem_Stru()
	{
		bCustBase = FALSE;
		bCustItem = FALSE;
		bExpanded = FALSE;
		bChanged = FALSE;
		bLoadFailed = FALSE;
		nOffset = 0;
		nItemIndex = (UINT)-1;
		nParentItemIndex = -1;
		nItemID = 0;
		eItemType = NONE_ITEM_TYPE;
		nArraySize = 0;
		bReadOnly = FALSE;
		szItemName[0] = 0;
		szItemContent[0] = 0;
		szItemDescription/*[0]*/ = /*0*/NULL;
		nItemLength = 0;
	}
}ModuleItem, *ModuleItem_Ptr;
/*lint -restore */

typedef CArray<ModuleItem, ModuleItem&>  ModuleItem_List;
typedef ModuleItem_List            *ModuleItem_List_Ptr;

typedef std::map<int, CString>  CMapData;  //Joly
typedef std::map<int, int>      CMapID;  

const TCHAR XML_VERSION[MAX_PATH] = _T("1.0");
//define NV Module structure
typedef struct Module_Stru
{   
	UINT nItemCount;
	TCHAR szModuleName[MAX_NAME_LENGTH];
	TCHAR szModuleFileName[MAX_PATH];
	TCHAR szModuleDescription[/*MAX_ITEM_CONTENT*/MAX_ITEM_DESCPT];  
	BOOL bAppendable;
	BOOL bExpanded;
	BOOL bLoadFailed;
	BOOL bCustBase;
	
	Module_Stru()
	{
		bCustBase = FALSE;
		bExpanded = FALSE;
		nItemCount = 0;
		szModuleName[0] = 0;
		szModuleFileName[0] = 0;
		szModuleDescription[0] = 0;
		bAppendable = FALSE;
		bLoadFailed = FALSE;
	}
}Module, *Module_Ptr;
/*lint -save -e756 */
typedef struct Project_Stru
{   
	UINT nModuleCount;
	TCHAR szPrjName[MAX_NAME_LENGTH];
	TCHAR szPrjFileName[MAX_PATH];
	TCHAR szPrjDescription[/*MAX_ITEM_CONTENT*/MAX_ITEM_DESCPT];  
}Project, *Prject_Ptr; 
/*lint -restore */

#define  CONVERT_SHORT2(Src,Dst) {					\
    extern BOOL g_bBigEndian;						\
    if( g_bBigEndian )								\
{                                                   \
	(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src));      \
}                                                   \
	else											\
{                                                   \
	(Dst) = (Src);                                  \
}                                                   \
}


#define  CONVERT_INT2(Src,Dst)   {					\
    extern BOOL g_bBigEndian;						\
    if( g_bBigEndian )								\
{                                                   \
	(Dst)  = MAKELONG(MAKEWORD(HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))),\
	MAKEWORD(HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src))));\
}                                                   \
    else                                            \
{                                                   \
	(Dst) = (Src);                                  \
}                                                   \
}

typedef struct _DELTA_ITEM_INFO_T_
{
	WORD wID;
	WORD wOffset;
	WORD wLen;

	_DELTA_ITEM_INFO_T_()
	{
		ZeroMemory(this,sizeof(_DELTA_ITEM_INFO_T_));
	}
}DELTA_ITEM_INFO_T;

class CFileParser
{
public:
	CFileParser();
	virtual ~CFileParser();
	
public:
	BOOL GetItemByName( CString strItemPath,int& nModuleIndex,int& nItemIndex );
	BOOL  ReCalcNVStruct(){ return GenerateOffset();}
    //[[ by Joly
	void  SetHexFormat(BOOL bHexFlag);
	void  FormatContent();
	void  SetMulFileAg(BOOL bWriteMul,VEC_FILEINFO & vecFileInfo);
	BOOL  IsExistedID(CUIntArray& uIntArray);
	BOOL  IsExistedID(UINT nId);
	UINT  ReadOtherFiles(UINT& nNVImageLength);
	
	void  SetWriteTail(BOOL nFlag);
	void  GetWriteTailFlag(); // for UI
	
	void  SetVersionFlag(BOOL nFlag);
	
	//  for repeated IDs checks
	void  SetMapIDs(CMapID& mapIds);
	void  GetMapIDs(CMapID*& mapIds);
	void  AnalyzeFixBin(const unsigned char* pvFile, DWORD& cbFileSize, CMapID& mapIds);
	//for MD5 
	void  CountMD5(CMapID& mapIds);
    //[[ for export static nv excel
	void GetStaticNVID(CMapData*& mapStaticNv);
	//]]
	
	void GetCalibrationNVID(CMapData*& mapCalibrationNv);  // for calibration nv

	BOOL CheckValue(TCHAR* strDes, TCHAR* szValue, ITEMTYPE_ENUM itemType);
	
	//for fix bin merge
	void SetWriteFix(LPCTSTR szFixBin);
	
	// for hash header
	void SetHashHeaderPath(LPCTSTR szHash);
	
	BOOL CompareHashValue(const unsigned char* pItem);
	
	BOOL GenerateConfigFile(const TCHAR* strPath, TCHAR* strFileName, BOOL bHeaderFile = TRUE); // add for SamSung NV update
	
	
    DWORD GetNVItemLength(UINT nItemID);
	
	DWORD GetNVItemLength(int nIndex); // add by Joly
    
    DWORD GetNVItemOffset(UINT nItemID);
	
    BOOL WriteNVItem(UINT nItemID, 
                     LPVOID lpBuffer, 
                     DWORD nNumberOfBytesToWrite, 
                     BOOL bBigEndian,
					 BOOL bGenOffset = TRUE);

    BOOL ReadNVItem(UINT nItemID, 
                    LPVOID lpBuffer, 
                    DWORD nNumberOfBytesToRead, 
                    LPDWORD lpNumberOfBytesRead,
                    BOOL bBigEndian,
					BOOL bGenOffset = TRUE);

    INT GetModuleItemCount(INT nModuleIndex);
	ModuleItem & GetModuleItem(INT nModuleIndex, INT nItemIndex );
	
    INT GetModuleCount();
    Module & GetModule(INT nModuleIndex);


	void RemoveAllNVltem(INT nModuleIndex);
	void RemoveModuleItem(INT nModuleIndex, INT nItemIndex);
    int  InsertModuleItem(INT nModuleIndex, ModuleItem InsertModuleItem, INT nInsertAfter = INVALID_MODULE_ITEM_INDEX, BOOL bInsertAfter = TRUE,BOOL bUpdateTree = FALSE);
    int  InsertModuleItemAllByOnce(INT nModuleIndex, ModuleItem InsertModuleItem, INT nInsertAfter = INVALID_MODULE_ITEM_INDEX, BOOL bInsertAfter = TRUE);
    void InsertModuleItemAllByOnceHelper(int nIndexStart, int itemParentIndex, UINT insertItemSize);

	void RemoveModule(INT nModuleIndex);
    
    BOOL LoadModule(TCHAR *szModuleFileName);
	/*xml file support by xmzhang add begin*/
	BOOL LoadModuleXML(TCHAR *szModuleFileName);
	BOOL LoadProjectXML(TCHAR *szPrjFileName,CStringArray& agWarning,BOOL bCheckItemName = TRUE);
	BOOL SaveModuleXML(INT nModuleIndex);
	BOOL SaveProjectXML();	
	BOOL LoadElement(MSXML2::IXMLDOMElementPtr pDocElement,UINT nParentItemIndex,TCHAR *szModuleFileName,UINT nItemID=0,ITEMTYPE eCurItemType=NONE_ITEM_TYPE);
	BOOL CreateProjectXML(const TCHAR *szPrjName, const TCHAR *szPrjFileName, 
                       const TCHAR *szTargetFileName, 
                       const TCHAR *szPrjDesc);//xmzhang
	/*xml file support by xmzhang add end*/
	BOOL LoadProject(TCHAR *szPrjFileName,CStringArray& agWarning,BOOL bCheckItemName = TRUE);
	BOOL SaveModule(INT nModuleIndex);
	BOOL SaveProject();
	BOOL CreateProject(const TCHAR *szPrjName, const TCHAR *szPrjFileName, 
                       const TCHAR *szTargetFileName, 
                       const TCHAR *szPrjDesc);

    
    INT CreateModule(const TCHAR *szModuleName, 
                     const TCHAR *szModuleFileName, 
                     const TCHAR *szModuleDesc);
    
    Module         m_CurrModule;
    ModuleItem     m_CurrModuleItem;

    BOOL GetProjectPath(CString& strPrjPath)
    {
        if(_tcslen(m_szPrjFileName) == 0)
        {
            return FALSE;
        }

        strPrjPath = m_szPrjFileName;
        int nIndex = strPrjPath.ReverseFind('\\');
        if(nIndex == -1)
        {
            // Have no path info
            return FALSE;
        }

        strPrjPath.Delete(nIndex,strPrjPath.GetLength() - nIndex);

        return TRUE;
    }

    // @ZHANG Ming : 2003-04-16
    // [[
    int GetNVItemCount( BOOL bCalcChangeFlag = FALSE );
    UINT GetNVItemID( int nIndex ) const;
	BOOL IsChangedNVItem(int nIndex ) const;
	const TCHAR* GetNVItemName( int nIndex ) const;
    // ]]

	void SetNVLoadState(int nIndex, BOOL bFailed);
	void ClearNVLoadState();
    // @ZHANG Ming : 2003-04-16
    // [[
    BOOL  ChangeFileRdOnlyAttri( const TCHAR *szFileName, BOOL bReadOnly );
    FILE* OpenFileForWrite( const TCHAR *szFileName, 
                            BOOL *bAttriChanged,
                            BOOL bBinaryMode = FALSE );
    // ]]
    /*xml file support by xmzhang add begin*/
    BOOL  ChangeFileForWrite( const TCHAR *szFileName);
    BOOL  IsFileRdOnly( const TCHAR *szFileName);
    /*xml file support by xmzhang add end*/

    // @ZHANG Ming : 2003-04-23
    // [[
    void SetWriteDsp( BOOL bWriteDsp );
    BOOL IsWriteDsp( void );
    // ]]

    // @ZHANG Ming : 2003-05-06
    // [[
    inline BOOL IsModified( void ) const
    {
        return m_bModified;
    }

    inline void SetModified( BOOL bModified )
    {
        m_bModified = bModified;
    }
    // ]]

    BOOL LoadModule2(TCHAR* szModuleFileName);

    void SetModuleAppendable(UINT nIndex,BOOL bAppend);
    
    BOOL GetItemPath( int nModuleIndex,int nIndex,CString& strPath );

private:

    ModuleItem_List  m_lstTopItem;
    
    CArray<Module, Module&>m_lstModule;    
    CArray<ModuleItem_List_Ptr, ModuleItem_List_Ptr&>m_lstModuleItemPtr;

    ModuleItem_List_Ptr m_currModuleItemList;

	CStringArray m_agStrError;
	//[[ by Joly
	BOOL m_bHexFlag;
	BOOL m_bWriteMult;
	VEC_FILEINFO		m_vecFileInfo;
	int  m_nMulFileCount;
	BOOL m_bTailFlag;
	BOOL m_bVerFlag;
	BOOL m_bStaticFlag;  // for static nv
	BOOL m_bCalibration; // for Calibration NV
	CMapData  m_mapStaticData;   // for static nv
	CMapData  m_mapCalibrationData; // for CalibrationData;
	CMapID          m_mapIDs; // add for check IDs
	unsigned char   m_digest[16];
	CString         m_strErrID;
	BOOL            m_bWriteFix;
	BOOL            m_bHashHeader;
	//]]

private:   
	bool CompareItem(LPCTSTR lpszOpt1, LPCTSTR lpszOpt2);
	unsigned int StrToInt(CString& strContent);
	std::string TString2Astring(LPCTSTR lpszString);
	CString TrimLeftRight(LPCTSTR lpString);
	BOOL  GenDeltaIndex(LPCTSTR lpDeltaPath);
	VOID  UpdatedNVChangeFlag();
    void  GenerateStructOffset(int nCurModuleIndex, int *nCurItemIndex, int *nFirstOffset, int *nItemMaxLen);
	int   GenerateOffset();
    bool  GetAtList(int nPIdx, ModuleItem& mi) const;
    bool  SetAtList(int nPIdx, ModuleItem& mi);
	
	//[[ add for SamSung update NV
	BOOL    GenerateStruct(CString& strName); 
	BOOL    SetNVBin(const ModuleItem & NewModuleItem, TCHAR* szValue,DELTA_ITEM_INFO_T& tDeltaItem);
	void    ClearNVHeader();
	void    ReportError(CString& strError);

	BOOL    ParseRule(const TCHAR szStart, const TCHAR szEnd, CString& strMid, TCHAR* szValue, ITEMTYPE_ENUM itemType);
	int     GetContent(TCHAR* szContent);
	BOOL    IsRange(CString& strMid, TCHAR* szValue, ITEMDES_ENUM desType, ITEMTYPE_ENUM itemType);

	int     SplitStr(LPCTSTR lpszStr,CStringArray &agStrs,TCHAR chSeparate = _T(','));
	void    CountAllMD5(DWORD nNVLength);
	void    WriteHashHeader();
	
	BOOL    MergeModerm(const TCHAR* strPath, TCHAR* strFileName);
	
	void    FilterContent(TCHAR *szModuleFileName);
	void	CheckNVModule(CStringArray& agError);
	void	CheckModuleItem(CStringArray& agError);
	void	GetModuleParentInfo(int nModuleIndex,CMapID &mapParentIndex);
	CString	GetStrFileTitle(LPCTSTR lpFile,BOOL bPostfix = TRUE, BOOL bMakeUpper =FALSE);
	void	GetNVItemPath(LPCTSTR szModuleName, int nItemParentIndex,CString& strPath);
	
	CString m_strNv;
	
	BOOL    m_bIsNVImag;
	BOOL    m_bNVFlag;
	BYTE*   m_szNvContent/*[MAX_TOTAL_ITEM_CONTENT]*/;
    DWORD   m_dwNVLength;
	DWORD   m_dwFileLength;
	
	//]]
	
	
    BOOL       m_bReadOnly;
	//    BOOL       m_bDirty;
	
    UINT       m_nModuleCount;
    UINT       m_nModuleItemCount;
    UINT       m_nAlignment;
	
    FILE       *m_hPrjFile;
    FILE       *m_hModuleFile;
    
    BYTE*       m_szItemContent/*[MAX_TOTAL_ITEM_CONTENT]*/;
    DWORD      m_dwNVImageLength;
	
    // @ZHANG Ming : 2003-04-23
    // [[
    BOOL       m_bWriteDsp;
    // ]]
	
    // @ZHANG Ming : 2003-05-06
    // m_bModified is used to record whether current project has been 
    // modified and whether the file should be saved when closed.
    // I add SetModified( TRUE ) in the following member functions :
    // CreateModule()
    // InsertModule()
    // InsertMoudleItem()
    // RemoveModule()
    // FillwithDefault()
    // RemoveModuleItem()
    // WriteNVItem()
    // SetModuleItem()]
    //
    // And SetModified( FALSE ) is added in the following functions :
    // CFileParser()
    // LoadProject()
    // CreateProject()
    // [[
    BOOL        m_bModified;
    TCHAR *m_szappend;/*xml file support by xmzhang add*/
    // ]]
	
public:
	
	void FillWithDefault();
	BOOL SaveProjectAs(TCHAR * szPrjFileName);
	int  InsertModule(TCHAR *szMOduleFileName,CStringArray& agWarning,BOOL bCheckItemName = TRUE);
	BOOL SaveToImage( const TCHAR * szDspFileName      ,
		const void * lpAppendage = NULL ,
		DWORD        cbSize = 0         );
    BOOL LoadFromImage( const TCHAR * szImageFileName );
	
	void SetModule(int nModuleIndex, const Module NewModule);
	void SetModuleItem( int                nModuleIndex, 
		int                nItemIndex, 
		const ModuleItem & NewModuleItem,
		BOOL               fSetModified = TRUE );
	
    TCHAR       m_szPrjName[MAX_NAME_LENGTH];
    TCHAR       m_szPrjFileName[MAX_PATH];
    TCHAR       m_szTargetFileName[MAX_PATH]; 
    TCHAR       m_szPrjDesc[/*MAX_ITEM_CONTENT*/MAX_ITEM_DESCPT]; 
	TCHAR       m_szFixBin[MAX_PATH]; // add fix bin to project
	TCHAR       m_szHashPath[MAX_PATH];
	
	
	
protected:
	void ClearProjectInfo(void);
	
protected:
	struct NVTreeItem
	{
		NVTreeItem* pParent;
		CArray<NVTreeItem*,NVTreeItem*> pChildren;
		CString strName;
		int nIndex;
	};
	
	NVTreeItem m_TreeRoot;
	
	void ConstructItemTree();
	void DestructItemTree();
	void DestructSubTree( NVTreeItem* );
	
	
	BOOL FindItem( NVTreeItem* pParent,CStringArray* pArrName,int nLevel );
	
	public:
		BOOL ImportSingleNVFile( CString strFile );	// used for command line mode only
		BOOL ImportDeltaNV(CString strFile); // used for samsung
		void ClearCalibrationFlag();
};

#endif // !defined(AFX_FILEPARSER_H__590D6B21_39F3_4183_A578_43F1176C31C6__INCLUDED_)
