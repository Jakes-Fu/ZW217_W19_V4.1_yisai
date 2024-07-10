// FileParser.h: interface for the CFileParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPARSER_H__590D6B21_39F3_4183_A578_43F1176C31C6__INCLUDED_)
#define AFX_FILEPARSER_H__590D6B21_39F3_4183_A578_43F1176C31C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;
#include "NVItemDef.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h> 
#include <map>
#include "TCHAR.h"
#include "tinyxml.h"
const TCHAR XML_VERSION[MAX_PATH] = _T("1.0");

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
   TCHAR     szItemName[MAX_NAME_LENGTH];          // Item Name
   TCHAR     szItemContent[MAX_ITEM_CONTENT];      // Array Value
   TCHAR     szItemDescription[MAX_ITEM_CONTENT];  
   BOOL      bReadOnly;
   BOOL      bExpanded;
   BOOL      bChanged;
   //MSXML2::IXMLDOMElementPtr     nXmlPtr;/*xml file support by xmzhang add*/
    //xmlDocPtr   nXmlPtr;  
   TiXmlElement* pItemEle;
    
   ModuleItem_Stru()
   {
       memset(this,0,sizeof(ModuleItem_Stru));
	   nItemIndex = (UINT)-1;
       nParentItemIndex = -1;
       eItemType = NONE_ITEM_TYPE;
   }
}ModuleItem, *ModuleItem_Ptr;
/*lint -restore */

typedef std::vector<ModuleItem>  ModuleItem_List;
typedef ModuleItem_List            *ModuleItem_List_Ptr;


typedef std::map<int, int>      CMapID;

//define NV Module structure
typedef struct Module_Stru
{   
   UINT nItemCount;
   TCHAR szModuleName[MAX_NAME_LENGTH];
   TCHAR szModuleFileName[MAX_PATH];
   TCHAR szModuleDescription[MAX_ITEM_CONTENT];  
   BOOL bAppendable;
   BOOL bExpanded;

   Module_Stru()
   {
       bExpanded = FALSE;
       nItemCount = 0;
       szModuleName[0] = 0;
       szModuleFileName[0] = 0;
       szModuleDescription[0] = 0;
       bAppendable = FALSE;
   }
}Module, *Module_Ptr;
/*lint -save -e756 */
typedef struct Project_Stru
{   
   UINT  nModuleCount;
   TCHAR szPrjName[MAX_NAME_LENGTH];
   TCHAR szPrjFileName[MAX_PATH];
   TCHAR szPrjDescription[MAX_ITEM_CONTENT];  
}Project, *Prject_Ptr; 
/*lint -restore */

#define  CONVERT_SHORT2(Src,Dst) {                      \
    extern BOOL g_bBigEndian;                             \
    if( g_bBigEndian )                                    \
	{                                                   \
		(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src));      \
	}                                                   \
	else                                                \
	{                                                   \
		(Dst) = (Src);                                  \
	}                                                   \
}


#define  CONVERT_INT2(Src,Dst)   {                      \
    extern BOOL g_bBigEndian;                             \
    if( g_bBigEndian )                                    \
	{                                                   \
		(Dst)  = MAKELONG(MAKEWORD(HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))),\
		MAKEWORD(HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src))));\
	}                                                   \
    else                                                \
	{                                                   \
		(Dst) = (Src);                                  \
	}                                                   \
}


class CFileParser  
{
public:
	CFileParser();
	virtual ~CFileParser();

public:
    //[[ by Joly
	void  SetHexFormat(BOOL bHexFlag);
	void  FormatContent();
	void  SetMulFileAg(BOOL bWriteMul,VEC_FILEINFO & vecFileInfo);
	BOOL  IsExistedID(CUIntArray& uIntArray);
	BOOL  IsExistedID(UINT nId);
	UINT  ReadOtherFiles(UINT& nNVImageLength);
	void  SetWriteTail(BOOL nFlag);
	BOOL GenerateConfigFile(const TCHAR* strPath, TCHAR* strFileName , BOOL bHeaderFile = TRUE);
        //]]

		//for fix bin merge
	void SetWriteFix(TCHAR* strFixBin);

	// for hash header
	void SetHashHeaderPath(TCHAR* strHash);

	//BOOL CompareHashValue(const unsigned char* pItem);

	void  SetVersionFlag(BOOL nFlag);

	//  for repeated IDs checks
	void  SetMapIDs(CMapID& mapIds);
	void  GetMapIDs(CMapID*& mapIds);
	void  AnalyzeFixBin(const unsigned char* pvFile, DWORD& cbFileSize, CMapID& mapIds);
	//for MD5 
	void  CountMD5(CMapID& mapIds);
	

    DWORD GetNVItemLength(UINT nItemID);
    
    DWORD GetNVItemOffset(UINT nItemID);

    BOOL WriteNVItem(UINT nItemID, 
                     LPVOID lpBuffer, 
                     DWORD nNumberOfBytesToWrite, 
                     BOOL bBigEndian);

    BOOL ReadNVItem(UINT nItemID, 
                    LPVOID lpBuffer, 
                    DWORD nNumberOfBytesToRead, 
                    LPDWORD lpNumberOfBytesRead,
                    BOOL bBigEndian);

    INT GetModuleItemCount(INT nModuleIndex);
	  ModuleItem & GetModuleItem(INT nModuleIndex, INT nItemIndex );
	
    INT GetModuleCount();
    Module & GetModule(INT nModuleIndex);


	  void RemoveAllNVltem(INT nModuleIndex);
	  void RemoveModuleItem(INT nModuleIndex, INT nItemIndex);
    int  InsertModuleItem(INT nModuleIndex, ModuleItem InsertModuleItem, INT nInsertAfter = INVALID_MODULE_ITEM_INDEX, BOOL bInsertAfter = TRUE);
    int  InsertModuleItemAllByOnce(INT nModuleIndex, ModuleItem InsertModuleItem, INT nInsertAfter = INVALID_MODULE_ITEM_INDEX, BOOL bInsertAfter = TRUE);
    void InsertModuleItemAllByOnceHelper(int nIndexStart, int itemParentIndex, UINT insertItemSize);

	  void RemoveModule(INT nModuleIndex);
    
    BOOL LoadModule(TCHAR *szModuleFileName);
	/*xml file support by xmzhang add begin*/
	BOOL LoadProjectXML(TCHAR *szPrjFileName,BOOL bCheckItemName = TRUE);
   	BOOL LoadModuleXML(TCHAR *szModuleFileName);
	BOOL LoadElement(TiXmlElement* pParentEle,int nParentItemIndex,TCHAR *szModuleFile,UINT nItemID=0,ITEMTYPE eCurItemType=NONE_ITEM_TYPE);
	BOOL SaveProjectXML();
	BOOL SaveModuleXML(INT nModuleIndex);
	  /*
	 
	  	
	  
	  BOOL CreateProjectXML(const TCHAR *szPrjName, const TCHAR *szPrjFileName, 
                         const TCHAR *szTargetFileName, 
                         const TCHAR *szPrjDesc);//xmzhang*/
                       
	  /*xml file support by xmzhang add end*/
	  
	  BOOL LoadProject(TCHAR *szPrjFileName,BOOL bCheckItemName = TRUE);
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

    BOOL GetProjectPath(TCHAR strPrjPath[MAX_PATH])
    {
        if(_tcslen(m_szPrjFileName) == 0)
        {
            return FALSE;
        }

        _tcscpy(strPrjPath, m_szPrjFileName);
        
        TCHAR *pPos = _tcsrchr(strPrjPath, '\\');
        if(NULL == pPos)
        {
            // Have no path info
            return FALSE;
        }

       *pPos = '\0';

        return TRUE;
    }

    // @ZHANG Ming : 2003-04-16
    // [[
    int GetNVItemCount(void);
    UINT GetNVItemID(int nIndex) const;
   	const TCHAR* GetNVItemName(int nIndex) const;
    // ]]

    // @ZHANG Ming : 2003-04-16
    // [[
    BOOL  ChangeFileRdOnlyAttri(const TCHAR *szFileName, BOOL bReadOnly);
    FILE* OpenFileForWrite(const TCHAR *szFileName, 
                            BOOL *bAttriChanged,
                            BOOL bBinaryMode = FALSE);
    // ]]
    /*xml file support by xmzhang add begin*/
    //BOOL  ChangeFileForWrite(const TCHAR *szFileName);
    //BOOL  IsFileRdOnly(const TCHAR *szFileName);
    /*xml file support by xmzhang add end*/

    // @ZHANG Ming : 2003-04-23
    // [[
    void SetWriteDsp(BOOL bWriteDsp);
    BOOL IsWriteDsp(void);
    // ]]

    // @ZHANG Ming : 2003-05-06
    // [[
    inline BOOL IsModified(void) const
    {
        return m_bModified;
    }

    inline void SetModified(BOOL bModified)
    {
        m_bModified = bModified;
    }
    // ]]

    BOOL LoadModule2(TCHAR* szModuleFileName);

    void SetModuleAppendable(UINT nIndex,BOOL bAppend);
    
    BOOL GetItemPath(int nModuleIndex,int nIndex, TCHAR strPath[MAX_PATH]);

private:

    ModuleItem_List              m_lstTopItem;
    
    vector<Module>               m_lstModule;    
    vector<ModuleItem_List_Ptr>  m_lstModuleItemPtr;

    ModuleItem_List_Ptr          m_currModuleItemList;

	  vector<string>               m_agStrError;
	  int                          m_nError;
	  
	//[[ by Joly
    BOOL                         m_bHexFlag;
    BOOL                         m_bWriteMult;
	VEC_FILEINFO				 m_vecFileInfo;
    int                          m_nMulFileCount;
    BOOL                         m_bTailFlag;
    BOOL m_bVerFlag;

    CMapID          m_mapIDs; // add for check IDs
     unsigned char   m_digest[16];
     string         m_strErrID;
     BOOL            m_bWriteFix;
     BOOL            m_bHashHeader;
	//]]

private:    
    void  GenerateStructOffset(int nCurModuleIndex, int *nCurItemIndex, int *nFirstOffset, int *nItemMaxLen);
	  int   GenerateOffset();
    bool  GetAtList(int nPIdx, ModuleItem& mi) const;
    bool  SetAtList(int nPIdx, ModuleItem& mi);

    		//[[ add for SamSung update NV
	BOOL    GenerateStruct(TCHAR* strName); 
	BOOL    SetNVBin(const ModuleItem & NewModuleItem, TCHAR* szValue);
	void    ClearNVHeader();
	BOOL  ImportNV(TCHAR*  strFile);


	void    CountAllMD5(DWORD nNVLength);
	void    WriteHashHeader();

	BOOL    MergeModerm(const TCHAR* strPath, TCHAR* strFileName);
	ITEMTYPE GetItemType(const TCHAR* szType);


	string   m_strNv;

	BOOL    m_bIsNVImag;
	BOOL    m_bNVFlag;
	BYTE    m_szNvContent[MAX_TOTAL_ITEM_CONTENT];
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
    
    BYTE       m_szItemContent[MAX_TOTAL_ITEM_CONTENT];
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
    TCHAR       *m_szappend;/*xml file support by xmzhang add*/
    // ]]

public:
	
	void FillWithDefault();
	BOOL SaveProjectAs(TCHAR * szPrjFileName);
	int  InsertModule(TCHAR *szMOduleFileName);
	BOOL SaveToImage(const TCHAR *szDspFileName,
		               const void  *lpAppendage = NULL ,
					         DWORD        cbSize = 0);
  BOOL LoadFromImage(const TCHAR * szImageFileName);

	void SetModule(int nModuleIndex, const Module NewModule);
	void SetModuleItem( int                  nModuleIndex, 
                      int                nItemIndex, 
                      const ModuleItem & NewModuleItem,
                      BOOL               fSetModified = TRUE);

    TCHAR       m_szPrjName[MAX_NAME_LENGTH];
    TCHAR       m_szPrjFileName[MAX_PATH];
    TCHAR       m_szTargetFileName[MAX_PATH]; 
    TCHAR       m_szPrjDesc[MAX_ITEM_CONTENT];  
    TCHAR       m_szFixBin[MAX_PATH]; // add fix bin to project
    TCHAR       m_szHashPath[MAX_PATH];

protected:
	void ClearProjectInfo(void);
	void CheckNVModule();
	void GetModuleParentInfo(int nModuleIndex,CMapID &mapParentIndex);
	void GetNVItemPath(TCHAR* szModuleName, int nItemParentIndex,string& strPath);
	void CheckModuleItem();
	void FilterContent(TCHAR *szModuleFileName);

protected:
	struct NVTreeItem
	{
		NVTreeItem* pParent;
		std::vector<NVTreeItem*> pChildren;
		TCHAR strName[MAX_PATH];
		int nIndex;
	};
	
	NVTreeItem m_TreeRoot;

	void ConstructItemTree();
	void DestructItemTree();
	void DestructSubTree( NVTreeItem* );

	BOOL GetItemByName(TCHAR strItemPath[MAX_PATH],int& nModuleIndex,int& nItemIndex);
	int FindItem(NVTreeItem* pParent, vector<string>* pArrName,int nLevel);

public:
	BOOL ImportItem(TCHAR* strFile);	// used for command line mode only
	void ClearCalibrationFlag();
};

#endif // !defined(AFX_FILEPARSER_H__590D6B21_39F3_4183_A578_43F1176C31C6__INCLUDED_)
