#ifndef _INC_LOGELITF_H
#define _INC_LOGELITF_H

#pragma warning( disable : 4786 )
#include <map>
#include <list>
#include <string>
using namespace std;

struct ParserTreeNode_tag;
typedef list<ParserTreeNode_tag*> ParserNodeList;

typedef map<int, string>  CIDNameMap;
typedef map<string,int>	  CNameIDMap;


#define  PLAY_BACK_EVENT_ID         1
#define  PERIOD_RECORD_EVENT_ID     2
#define  AUTO_TEST_EVENT_ID         3
#define  AUTO_START_EVENT_ID        4
#define  FLUSH_FILE_EVENT_ID        5
//#define  PLAY_BACK_UNPACK_EVENT_ID 6

#define  AUTO_TEST_TIME_ELAPSE      3000
#define  AUTO_START_TIME_ELAPSE     5000
#define  FLUSH_FILE_TIME_ELAPSE     60000


#define MAX_LOG_LIST_ITEM_NUM    20
#define MAX_LIST_ITEM_NAME_LEN    64
typedef struct LogelViewItem_tag
{
	char strName[MAX_LIST_ITEM_NAME_LEN];
	int nShowSize;
	BOOL  bShow;

}LogelViewItem;


//typedef list<LogelViewItem_tag*>   LogelViewItemList;

typedef enum
{
    NONE_PARSER_NODE_TYPE,
	INVISIBLE_PARSER_NODE_TYPE,                //节点没有值
	VISIBLE_PARSER_NODE_TYPE                   //节点有值
}EParserNodeType;

typedef enum
{
    NONE_PARSER_DATA_TYPE,
	CHAR_PARSER_DATA_TYPE,                     //节点为字符类型，节点的值为整型
	SHORT_PARSER_DATA_TYPE,                    //节点为短整型类型，节点的值为整型
	INT_PARSER_DATA_TYPE = 4,                      //节点为整型类型，节点的值为整型
	ENUM_PARSER_DATA_TYPE                      //节点为枚举类型，节点的值为字符串
}EParserDataType;

typedef struct ParserNode_tag
{
    EParserNodeType     m_eNodeType;          //节点显示类型 
    EParserDataType     m_eDataType;          //节点数据类型
    int           m_iLevel;             //节点级别
    int           m_iOffset;            //节点偏移量
    int           m_iValue;             //节点的整型值
    CString       m_szValue;            //节点的字符值
    CString       m_szLeftName;         //节点的类型名称
    CString       m_szRightName;        //节点的名称
}ParserNode,*ParserNodePtr;


typedef struct ParserTreeNode_tag
{
	ParserNode  m_nodeParser;
	ParserNodeList m_nodeChild;
	ParserTreeNode_tag*  m_pParentNode;
}ParserTreeNode,*ParserTreeNodePtr;

typedef enum
{
	PARSER_BIT_STREAM,
	PARSER_BYTE_STREAM,
	PARSER_BYTE_EMBEDDED,
	PARSER_BIT_STRUCT,
	PARSER_BYTE_STRUCT,
	PARSER_LOCAL_MSG,
	PARSER_PEER_MSG,
	PARSER_TO_BIGENDIAN,
	PARSER_TO_LITTLEENDIAN
}ENUM_PARSER_TYPE_ID;

typedef struct SendParam_tag
{
	BYTE type;
	BYTE subtype;
	DWORD dwTimeout;
	BOOL  bBlocked;
	BOOL bNeedResponse;
	DWORD dwReserve;
}SendParam, *SendParamPtr;


#define  CONVERT_SHORT_BYENDIAN( Src, Dst, bBigEndian) \
	if( bBigEndian ) { \
		(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src)); \
		*(WORD*)&(Src) = (Dst); \
	}else{ \
		(Dst) = (Src); \
	}


#define  CONVERT_INT_BYENDIAN( Src, Dst, bBigEndian) \
	if( bBigEndian ) { \
	(Dst)  = MAKELONG( MAKEWORD( HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))), \
		MAKEWORD( HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src)))); \
	*(DWORD*)&(Src) = (Dst); \
	}else{ \
	(Dst) = (Src); \
}

#define WM_RCV_DATA			WM_USER + 500

#define  WM_RCV_CHANNEL_DATA              WM_APP + 301 // channel data received
#define  WM_CHANNEL_CLOSE                 WM_RCV_CHANNEL_DATA + 1

#define  WM_RCV_DSP_DATA              WM_APP + 601 // channel data received
#define  WM_RCV_DIAG_DATA              WM_APP + 501 // channel data received

#define WM_PLAYBACK_FINISH				WM_APP + 201



typedef bool (*ADLogelDllInitDeinitFn)(void);

#define LOST_ITEM_ID    0xFFFFFFFF

static const char gszwizardIniFile[]        = "LogelWizard.ini";

typedef enum STATUS_BAR_INDEX_E
{
	HANDOVER_INFO_IND = 1,
	LOGFILE_INFO_IND
};


/**
 * IParserHandler interface defines  the basic operations to parser.
 * 
 *
 */

class IScriptHandler;
class IParserHandler
{
public:
    /**
     * Load a data parser, maybe from a filename
     * 
     *
     * @param cbstrFileName : file name of parser
     *
     * @return Returns TRUE if successful,Otherwise returns FALSE;
     */
	virtual BOOL LoadDataParser( LPCTSTR lpszFileName ) = 0;


	virtual BOOL UnloadDataParser(void) = 0;

	virtual BOOL IsParserMessage( DWORD dwID ) = 0 ;	
		
    virtual BOOL Decode( /*[in]*/DWORD dwFlag, /*[in]*/DWORD dwID, /*[in]*/LPVOID lpData, /*[in]*/ULONG ulDataSize ,
		/*[in]*/LPVOID lpEmbeddedData, /*[in]*/ULONG ulEmbeddedDataSize, /*[in/out]*/DWORD* pdwReserved ) = 0;

		
	virtual BOOL Encode( /*[in]*/DWORD dwFlag, /*[in]*/DWORD dwID, char* szMsgScript,
		/*[in]*/void** lppData, /*[in]*/ULONG *pulDataSize,
		/*[in]*/LPVOID lpEmbeddedData, /*[in]*/ULONG ulEmbeddedDataSize, /*[in/out]*/DWORD* pdwReserved ) = 0;   
		
    virtual BOOL IsSupportTree( void ) = 0;   

	virtual ParserTreeNodePtr GetRootNode() = 0;
			
	virtual ParserTreeNodePtr GetTreeNode( LPBYTE lpPosition, int nPosLen ) = 0 ;  	

	virtual ParserTreeNodePtr GetChildNode( ParserTreeNodePtr pTreeNode, int iIndex ) = 0;

	virtual ParserNode& GetNode( DWORD dwFlag, int nIndex  ) = 0;  
	
//	virtual IScriptHandler* GetScriptHandler() = 0;

	virtual BOOL DeconstructParserTree(  ) = 0;

	virtual DWORD GetNodeCount( DWORD dwFlag ) = 0;
	
	virtual const char* GetLastErrorMessage() = 0;

    /**
     * Get a property value of the parser. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. eg, version,
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pvarValue:This parameter cannot be NULL and must point to an uninitialized
     * VARIANT. If no error is returned, the VARIANT is initialized using VariantInit,
     * and then set to contain the property value.  The caller must call VariantClear 
     * on this pointer when the value is no longer required. If an error code is returned, 
     * the VARIANT pointed to by pValue is left unmodified.
     *
     * @return Returns BM_S_OK if successful,Otherwise returns BM_E_FAILED;
     */
     virtual BOOL GetProperty(   DWORD dwPropertyID, 
		 const VARIANT varParam, VARIANT* pvarValue ) = 0;

    /**
     * Put a property value of the parser. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. eg, version,
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pcvarValue: Points to a VARIANT that is treated as readonly. 
     * The value in the VARIANT becomes the named property value.
     *
     * @return Returns S_OK if successful,Otherwise returns BM_E_FAILED;
     */
    virtual  BOOL PutProperty (  DWORD dwPropertyID, 
		const VARIANT varParam, const VARIANT pvarValue ) = 0 ;
  
};

            
/**
 * ICommHandler interface defines  the basic operations to parser.
 * 
 *
 */

enum
{
	COMMTYPE_UART,
	COMMTYPE_SOCKET,
	COMMTYPE_GPIO,
	COMMTYPE_USB
};

#define MAX_RECEIVER_NUM		5

class ICommHandler
{		
public:

    virtual BOOL Initialize( LPCTSTR lpszChannelProgID, LPBYTE lpOpenParam,
        BOOL bBigEndian, DWORD dwCookie ) = 0;
    
    virtual void Uninitialize() = 0;
 
	
	virtual BOOL SendData( LPBYTE lpData, UINT uiDataLen, SendParamPtr lpSendParam ) = 0;
    
	virtual BOOL SetReceiver( BOOL bRcvThread, const LPVOID pReceiver ) = 0;
		
	virtual void StartOpr(  BOOL bPlayback,  LPCTSTR lpszFileName ) = 0;
    
	virtual void StopOpr() = 0;

 /**
     * Get a property value of the commhandler. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. eg, version,
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pvarValue:This parameter cannot be NULL and must point to an uninitialized
     * VARIANT. If no error is returned, the VARIANT is initialized using VariantInit,
     * and then set to contain the property value.  The caller must call VariantClear 
     * on this pointer when the value is no longer required. If an error code is returned, 
     * the VARIANT pointed to by pValue is left unmodified.
     *
     * @return Returns BM_S_OK if successful,Otherwise returns BM_E_FAILED;
     */
     virtual BOOL GetProperty(  const char* szPropertyName, 
		 WPARAM wParam, VARIANT* pvarValue ) = 0;

    /**
     * Put a property value of the ICommHandler. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. eg, version,
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pcvarValue: Points to a VARIANT that is treated as readonly. 
     * The value in the VARIANT becomes the named property value.
     *
     * @return Returns S_OK if successful,Otherwise returns BM_E_FAILED;
     */
    virtual  BOOL PutProperty ( const char* szPropertyName,
		WPARAM wParam, const VARIANT* pvarValue ) = 0 ;

	virtual void SetWorkMode( int nWorkMode , int nRecordMode ) = 0 ;

	
  
};

class ILogelFileHandler;


struct CCompleteFilePath
{
	TCHAR m_szFilePath[ MAX_PATH ];
	
	CCompleteFilePath( LPCTSTR szPrefix, 
		LPCTSTR szPostfix, 
					   LPCTSTR lpszAppender = NULL )
	{
		_ASSERTE( NULL != szPrefix );
		_ASSERTE( NULL != szPostfix );
		
		::_tcscpy( m_szFilePath, szPrefix );
		if ( NULL != lpszAppender )
			::_tcscat( m_szFilePath, lpszAppender );
		//		::_tcscat( m_szFilePath, TEXT(".") );
		::_tcscat( m_szFilePath, szPostfix );
	};
	
	operator LPCTSTR( void )
	{
		return m_szFilePath;
	};
};

class ILogelConsoleWindow;

class ILogelUIManager;

class ILogelCommandHandler;
enum
{
	SNAPIN_LOGEL,
	SNAPIN_SCRIPT,
	SNAPIN_ANALYZE,
	SNAPIN_MESSAGE_FLOW,
	SNAPIN_COMMON
};

class ILogelOprSubscriber;
class ITestFunctionSnapin
{
public:
	virtual BOOL Init(  LONG lSnapInCookie, 
		HWND hwndOwner,
		ILogelConsoleWindow*   pConsoleWindow,  
		ILogelUIManager* pUIManager,
		ILogelOprSubscriber* pSubscriber,
		BOOL bBigEndian ) = 0;	
	virtual BOOL Deinit() = 0;

	virtual BOOL OnStartWork() = 0;
	virtual BOOL OnStopWork() = 0;

	virtual BOOL SetEndianMode( BOOL bBigEndian ) = 0;

	virtual ILogelCommandHandler* GetLogelCommandHandler() = 0;

	virtual int GetSnapinType() = 0;

	/**
	 * Get different type of strings of the snapin.
	 *
	 * @param pbstrStrings DisplayName\nDescriptons
	 *
	 * @retval LOGEL_E_FAILED Generic failure.
	 * @retval LOGEL_S_OK	Success.
	 */
	virtual HRESULT GetStrings( CString& strSnapin ) = 0 ;

	/**
	 * Get the icon for the snapin. 
	 *
	 * @param bSmall Indicates which type of icon is required. Small icon is 16*16, large icon is 32*32.
	 *
	 * @param pIcon Receives the handle to the icon.
	 * 
	 * @retval LOGEL_E_FAILED Generic failure.
	 * @retval LOGEL_S_OK	Success.
	 */
	virtual HRESULT GetIcon( BOOL bSmall, HICON* pIcon ) = 0 ;


    /*!
	* Invoked when the view is set to active.
	* After calling this function, console will show the view window.
	*
	* @return  : The return value is ignored.
	*/
	virtual HRESULT OnActivate( )  = 0;
	
    /*!
	* Invoked when the view is set to inactive.
	* After calling this function, console will hide the view window.
	*
	* @return  The return value is ignored.
	*/
	virtual HRESULT OnDeactivate( ) = 0 ;

	virtual HRESULT GetProperty(   DWORD dwPropertyID, 
		const VARIANT varParam, VARIANT* pvarValue )= 0;

    virtual  HRESULT PutProperty (  DWORD dwPropertyID, 
		const VARIANT varParam, const VARIANT pvarValue ) = 0 ;
	
	
};


class ITestFunctionSnapinEx
{
public:
	virtual HRESULT Reset() = 0 ;
	virtual HRESULT CleanUp() = 0;
	virtual void SetWorkMode( int nWorkMode , int nRecordMode ) = 0 ;
	
};
#define TRACE_RECORD_FLAG       TEXT("0")
/*
class IScriptSnapin : public ITestFunctionSnapin
{
public:
//	virtual BOOL SetScriptHandler( IScriptHandler* pScriptHandler ) = 0;	
//	virtual ICommHandler*  GetCommHandler() = 0;
//	virtual BOOL IsScriptRunning() = 0;
	
	

};


class IAnalyzeSnapin : public ITestFunctionSnapin
{
public:
	virtual BOOL OpenAnalyzeFiles( LPCTSTR lpszFilePath ) = 0 ;
	virtual void SetWorkMode( int nWorkMode , int nRecordMode ) = 0 ;
	virtual BOOL IsSupportMsgtype( BYTE bType, BYTE bSubType ) = 0;
	virtual BOOL Cleanup() = 0;
	virtual BOOL DoDispatchData( WPARAM wParam, LPARAM lParam, UINT& nLogmsgNum ) = 0;
	
};

class IMessageFlowSnapin : public IAnalyzeSnapin
{
public:
	virtual BOOL LoadMessageFlowEntity( LPCTSTR lpszEntity ) = 0;
	virtual BOOL ShowMessageFlowItem( LPCTSTR lpszFrom, LPCTSTR lpszTo, LPCTSTR lpszMessage ) = 0;
};


class ILogelSnapin :public ITestFunctionSnapin
{
public:
	virtual BOOL ResetLogel() = 0;
//	virtual BOOL GetConfiguration() = 0;
//	virtual ILogelFileHandler* GetLogelFileHandlerPtr(   LPTSTR szMainFilePostfix ) = 0;
	virtual BOOL Cleanup() = 0;
	virtual BOOL DoDisconnect() = 0;
	virtual BOOL DoConnect() = 0;

	virtual BOOL IsSupportMsgtype( BYTE bType, BYTE bSubType ) = 0;

//	virtual BOOL OnCreate( CWnd* pMainWnd ) = 0;

	virtual BOOL SetLogViewItem( LogelViewItem* pViewItemAry, int nArySize ) = 0;
	virtual char* GetLogViewItem( LogelViewItem* pViewItemAry, int& nArySize,
					int& nSenderIndex, int& nReceiverIndex, int& nMsgIndex ) = 0;
	
	virtual int ShowMsgContent( LPBYTE lpBuffer, DWORD dwSize  ) = 0;

	virtual void DoOpenFiles() = 0 ; 
	
	virtual void DoCloseFiles( BOOL bDelete ) = 0;
	
	virtual BOOL DoDispatchData( WPARAM wParam, LPARAM lParam, UINT& nLogmsgNum ) = 0;
		
//	virtual void PlayBack( UINT  uiCurrent ) = 0;
	
//	virtual BOOL DoPlaybackEvent( UINT uiPlaybackTo, BOOL bFastFoward   ) = 0 ;

	virtual COLORREF GetMsgShowingColor( CStringArray& strItemArray ) = 0;


	virtual void SetWorkMode( int nWorkMode , int nRecordMode ) = 0 ;


//	virtual CPropertyPage* GetFilterPage() = 0;
	
	

};
*/
enum OPEN_MODE
{
	// Create new logel files for read and write. If the logel 
	// files exist, new records will be appended to it.
	eCREATE_NEW,
	// Open existing logel files for read only.
	eOPEN_EXISTING,
};

/*
class	ILogelWizard
{
public:
	virtual BOOL LoadWizard() = 0;
	virtual BOOL UnLoadWizard() = 0;

	virtual int GetSnapinCount() = 0 ;
	
	virtual BOOL GetCommunication() = 0;
};
*/

typedef enum {
	ORIGINAL_MODE = 0,
	UNPACK_MODE,
	LIST_MODE,
	ANALYZE_MODE  
}WORK_MODE_E;

#define  RECORD_NULL_MODE       0
#define  RECORD_ORIGINAL_MODE    2
#define  RECORD_UNPACK_MODE     4
#define  RECORD_LIST_MODE       8
#define  RECORD_ANALYZE_MODE    16


#define TIME_MODE   0
#define COUNT_MODE   1
#define ALL_MODE   2

typedef enum 
{
	SNAPIN_RUN_SCRIPT,
}SNAPIN_STATUS_E;

enum
{
	ITEM_NO,
	ITEM_SN,
	ITEM_TYPE,
	ITEM_SENDER,
	ITEM_RECEIVER,
	ITEM_MESSAGE,
	FIXED_LOGVIEW_ITEM_COUNT
};

class ILogelConsoleWindow
{
public:
	virtual CWnd* CreateLogWindow( LPCTSTR strTitle , FILE *pShowIndFile ) = 0;
	virtual BOOL SetMsgContentViews( int nViewCount,  char** lppViewName  ) = 0;
	virtual BOOL ShowMessageContent( int nMemValueIndex, ParserTreeNodePtr pNode , 
		LPCTSTR lpszShow, 
		LPBYTE lpMemBuffer, DWORD dwMemSize ) = 0;
//	virtual BOOL ClearUpMsgContents() = 0;
	/*
	virtual BOOL ShowLogItem( DWORD dwID, LPCTSTR lpszTitle, 
							 UINT* aryInd,  int nSize, CStringArray& strItemArray,
							 BOOL bShowIntegWnd  ) = 0;
	*/

	virtual void SetStatusText( int nIndex,LPCTSTR strStatus, COLORREF clrText = RGB(0, 255, 0) ) = 0;

//	virtual BOOL SendDataFromComm( LPBYTE lpData, UINT uiDataLen, 
//		SendParamPtr lpSendParam ) = 0;	

//	virtual UINT WriteShowLogFile( CStringArray& strItemArray, UINT *aryInd ) = 0;	

//	virtual BOOL IsRecordInKeyRecords( LPCTSTR lpszRecordNum ) = 0;	

	virtual BOOL CreateBarWindow( CWnd* pWnd , 
		// The ID of the control bar.
		UINT& nID,
		// Points to a null-terminated character string that contains the 
		// control bar title.
		LPCTSTR lpszWindowName = NULL,
		// The default size the control bar is to be.
		CSize sizeDefault = CSize(200,100),
		// Specifies the window style attributes.
		DWORD dwStyle = CBRS_LEFT,
		// TRUE for flat style control bars.
		BOOL bFlatBorders = FALSE) = 0;

	virtual BOOL ShowTraceItem( CStringArray& traceItem, LPCTSTR lpszSeqence, LPCTSTR lpszTrace ) = 0 ;	
	

	virtual BOOL ShowLogFrameItem( CStringArray& frameItem, DWORD  dwSender, DWORD dwReceived,
		LPBYTE lpMessage, DWORD dwSize ) = 0;	

	virtual BOOL ShowLostMsgNum(  WORD wLostFrmNum , int nShowNumItemIndex ) = 0;

	virtual ICommHandler*  GetCommHandler( DWORD dwCookie ) = 0;

	virtual void SetSnapinStatus( SNAPIN_STATUS_E eStatus, BOOL bAtStatus ) = 0;
	
	virtual HRESULT TranslateAccelerator(  int  nLevel,  LPMSG   lpmsg ) = 0;  

	virtual HRESULT GetWindow( HWND * pwnd  ) = 0;
	virtual HRESULT ExecuteCommand (LPCTSTR lpcszCommand )=0 ;

};


/**
 * String types to be retrived. See ILogelContextMenu::GetCommandString()
 */
enum{
	LOGEL_GCS_HELPTEXT = 0x01,
	LOGEL_GCS_VERB = 0x02,
	LOGEL_GCS_VALIDATE = 0x04,
};

/**
 * Flags of OpenManagementView()
 */
enum{
	LOGEL_OMV_NOACTIVATE = 0x01,
};

#define LOGEL_MENU_MAIN		_T("Main")
#define LOGEL_MENU_LOG		_T("Log")
#define LOGEL_MENU_OPTION		_T("Option")
#define LOGEL_MENU_SERVER		_T("Server")
#define LOGEL_MENU_ASSERT		_T("Assert")
#define LOGEL_MENU_VIEW		_T("View")
#define LOGEL_MENU_TOOL		_T("Tool")
#define LOGEL_MENU_WINDOW		_T("Window")
#define LOGEL_MENU_HELP		_T("Help")

#define LOGEL_MENU_TOOLBARS	_T("Toolbars")

#define LOGEL_TOOLBAR_STANDARD	_T("Standard")
#define LOGEL_TOOLBAR_TOOLS		_T("Tools")

/**
 * Status code of command. LOGEL_ENALBED is a flag, it can be set with any of LOGEL_UNCHECKED,
 * LOGEL_CHECKED and LOGEL_INDETERMINATE
 */
typedef enum tagLOGEL_COMMAND_STATE{	
	LOGEL_COMMAND_ENABLED	= 0x10,

	LOGEL_COMMAND_UNCHECKED = 0x01,
	LOGEL_COMMAND_CHECKED	= 0x02,
	LOGEL_COMMAND_INDETERMINATE	= 0x03,

	LOGEL_COMMAND_CHECK_MASK = 0x03
} LOGEL_COMMAND_STATE;

/**
 * The type used to identifying a command when invoking a command. See ILogelContextMenu::InvokeCommand()
 * and ILogelCommandHandler::InvokeCommand().
 */
enum{
	LOGEL_COMMAND_BYID = 0x01,
	LOGEL_COMMAND_BYNAME = 0x02,
};

#define LOGEL_LEVEL_1_ACCELATOR	0x1
#define LOGEL_LEVEL_2_ACCELATOR	0x2
#define LOGEL_ALL_ACCELATOR		0x3

#define LOGEL_PRIVATE_COMMAND		0x0
#define LOGEL_SHARED_COMMAND		0x01


class ILogelUIManager
{
public:
	
	/**
	 * Set the command bitmap.
	 *
	 * @param lSnapInCookie The cookie which is passed into snap-in on the initialization.
	 *
	 * @param hbmpLarge The large image for toolbar buttons.
	 *
	 * @param hbmpSmall The small image for toolbar buttons.
	 *
	 * @param crMask Mask color for the image.
	 *
	 */
	virtual HRESULT AddCommandBitmap( LONG lSnapInCookie,
								HBITMAP hbmpLarge,
								HBITMAP hbmpSmall,
								COLORREF crMask) = 0;

	/**
	 * Add a command.
	 * @param lFlags Shared command or not.
	 *
	 * @param cbstrName Command name which identify the command.
	 *
	 * @param cbstrStrings "DisplayName\nDescription on status bar\nTooltips\nCategory"
	 *					Category is used in customization of commands in future version.
	 *
	 * @param nBitmapOffset The offset of the image which is associated to this command. The image
	 *						is stored in hbmpLarge/hbmpSmall which is passed in at SetSnapInUIInfo().
	 *
	 * @param lSnapInCookie The cookie for the snap-in.
	 */
	virtual HRESULT AddCommand( LONG lFlags, LPCTSTR lpcszName, LPCTSTR lpcszStrings,
		int nBitmapOffset, LONG lSnapInCookie, UINT* pnID ) = 0 ;

	/**
	 * Add a short-cut key for the command.
	 * 
	 * Not implemented currently.
	 */
	virtual HRESULT AddKeyBinding( LPCTSTR lpcszCommandName,
							 LPCTSTR lpcszKeyName ) = 0 ;

	/**
	 * Set the command state. 
	 *
	 * @param cbstrCommandName Points to the command name.
	 *
	 * @param nCommandState The command state. Please see also LOGEL_COMMAND_STATE.
	 */
	virtual HRESULT SetCommandState( LPCTSTR lpcszCommandName,
								int nCommandState ) = 0 ;
	/**
	 * Create a submenu.
	 *
	 * @param cbstrName The name that identifies the sub menu.
	 *
	 * @param cbstrDisplayName The name that is displayed.
	 *
	 * @retval LOGEL_S_FALSE The menu already exists.
	 * @retval LOGEL_S_OK The menu is created successfully.
	 */
	virtual HRESULT CreateSubMenu( LPCTSTR lpcszName,
		LPCTSTR lpcszDisplayName ) = 0 ;

	/**
	 * Add a sub menu to a parent menu.
	 *
	 * @param cbstrParentMenu The parent menu name.
	 *
	 * @param nIndex The position which the sub menu is to insert at.
	 *
	 * @param cbstrSubMenu The sub menu name that is to be added.
	 *
	 * @retval<LOGEL_E_MENU_NOT_FOUND> The parent menu or sub menu is not found.
	 */
	virtual HRESULT AddSubMenu( LPCTSTR lpcszParentMenu,
		int nIndex,
		LPCTSTR lpcszSubMenu ) = 0 ;

	/**
	 * Returns the sub menu index in the parent menu.
	 *
	 * @param cbstrParentMenu The parent menu name.
	 *
	 * @param cbstrSubMenu The sub-menu name whose index returns.
	 *
	 * @param pnIndex If the submenu is found, pnIndex stores the submenu index, otherwise,
	 *				pnIndex is stored with -1.
	 *
	 * @retval<LOGEL_E_MENU_NOT_FOUND> The parent menu or sub menu is not found.
	 * @retval<LOGEL_E_FAILED>	The sub menu is not in the parent menu.
	 */
	virtual HRESULT SubMenuIndex( LPCTSTR lpcszParentMenu,
		LPCTSTR lpcszSubMenu,
		int* pnIndex ) = 0 ;

	/**
	 * Add an item to the menu.
	 * 
	 * @param cbstrMenuName Points to the menu name.
	 *
	 * @param nIndex Indicates the position at which the item is added.
	 *
	 * @param bSeperator Indicates whether the item is a seperation. If TRUE cbstrCommandName is ignored.
	 *
	 * @param cbstrCommandName Points to the command name that is to be added to the menu.
	 */
	virtual HRESULT AddMenuItem( LPCTSTR lpcszMenuName,
		int nIndex,
		BOOL bSeperator,
		LPCTSTR lpcszCommandName ) = 0 ;

	/**
	 * Remove a menu item.
	 *
	 * @param cbstrMenuName Points to the menu name.
	 *
	 * @param cbstrCommandName Points to the command name that is to be removed from the menu.
	 */
	virtual HRESULT RemoveMenuItem( LPCTSTR lpcszMenuName,
		int nIndex ) = 0 ;

	/**
	 * Returns the menu item index.
	 *
	 * @param cbstrMenuName Points to the menu name.
	 *
	 * @param cbstrCommandName Points to command name.
	 *
	 * @param pnIndex If the command is found, pnIndex stores the menu item index, otherwise,
	 *		pnIndex is stored with -1.
	 */
	virtual HRESULT MenuItemIndex( LPCTSTR lpcszMenuName,
		LPCTSTR lpcszCommandName ,
		int* pnIndex ) = 0 ;

	/**
	 * Create a toolbar.
	 *
	 * @param bstrName Points to the name of the tool bar. Can't be NULL.
	 *
	 * @param bstrCaption Points to the caption of the tool bar. Can be NULL. If NULL the name will be used as the caption.
	 *
	 * @retval LOGEL_S_FALSE The toolbar already exists.
	 * @retval LOGEL_S_OK The toolbar is created successfully.
	*/
	virtual HRESULT CreateToolBar( LPCTSTR lpcszName,
		LPCTSTR lpcszCaption ) = 0 ;

	/**
	 * Shows or hides the specified tool bar.
	 */
	virtual HRESULT ShowToolBar( LPCTSTR lpcszName, BOOL bShow ) = 0 ;
	
	/**
	 * Add a command to toolbar.
	 *
	 * @param cbstrToolBarName Points to the toolbar name.
	 *
	 * @param nIndex Indicates the position at which the button is inserted.
	 *
	 * @param fsStyle Button Style. TBSTYLE_XXXX See MSDN for more information.
	 *
	 * @param nFlags Reserved.
	 *
	 * @param cbstrCommand Points to the command name that is to be added to the toolbar.
	 */
	virtual HRESULT AddToolBarButton( LPCTSTR lpcszBarName,
								int nIndex,
								BYTE fsStyle,
								int nFlags, //reserved
								LPCTSTR lpcszCommandName ) = 0 ;

	/**
	 * Remove a toolbar button.
	 *
	 * @param cbstrToolBarName Points to the toolbar name.
	 *
	 * @param cbstrCommandName Points to the command name.
	 */
	virtual HRESULT RemoveToolBarItem( LPCTSTR lpcszToolBarName,
		int nIndex ) = 0 ;
	/**
	 * Returns the toolbar button index.
	 * 
	 * @param cbstrCommandName Points to the toolbar name.
	 *
	 * @param cbstrCommandName Points to command name.
	 *
	 * @param pnIndex If the command is found, pnIndex stores the index of the command, otherwise
	 *		pnIndex is stored with -1.
	 */
	virtual HRESULT ToolBarButtonIndex( LPCTSTR lpcszToolBarName,
								LPCTSTR lpcszCommandName ,
								int* pnIndex ) = 0 ;
};

class ILogelCommandHandler
{
public:

	/**
	 * Invoke a command.
	 *
	 * @param uFlags Specifies the flags that controls the identifying type of command.
	 *
	 * @param nCmdID The command id which is to be invoked. Only valid if LOGEL_IC_BYID is set. 
	 *
	 * @param bstrCommand The command name which is to be invoked. Only valid if LOGEL_IC_BYNAME is set.
	 *
	 * @return The return value is ignored.
	 */
	STDMETHOD(InvokeCommand)( THIS_ UINT uFlags,
								const BSTR bstrCommand,
								UINT nCmdID ) PURE;

	/**
	 * Update the status of a specified command.
	 *
	 * @param nCmdID [in] The command id which is to be invoked. Only valid if LOGEL_IC_BYID is set. 
	 *
	 * @param bstrCommand The command name which is to be invoked. Only valid if LOGEL_IC_BYNAME is set.
	 *
	 * @param pnStatus	Receives the command status. See LOGEL_COMMAND_STATE
	 *
	 * @retval LOGEL_E_FAILED Ingore the value in pnStatus.
	 * @retval LOGEL_S_OK		Update the command status with pnStatus.
	 */
	STDMETHOD(UpdateCommand)( THIS_ UINT uFlags,
								const BSTR bstrCommand,
								UINT nCmdID,
								int* pnStatus ) PURE;
};

/**
 * This interfaces should be implemented along with ILogelCommandHandler if the snapin wants to handle
 * specific toolbar message, e.g. DROPDOWN notification.
 */
class ILogelToolBarHandler
{
public:

	/**
	 * Handle the drop down message of toolbar.
	 *
	 * @param idCommand Command identifier of the button whose drop down arrow is pressed.
	 *
	 * @param prcItem The rect of the item whose drop down arrow is pressed. It is in screen coordinates.
	 *
	 * @param plResult The result of the message handling. See TBN_DROPDOWN in MSDN.
	 *
	 * @return Return Logel_S_OK if the command is handled. Returns Logel_E_FAILED if the command is not handled.
	 * If the command is not handled, Logel will try to let the successive handler handle the command.
	 */
	STDMETHOD(HandleDropDownMsg)( THIS_ UINT nFlags,
								const BSTR bstrCommandName,
								UINT nCommandID,
								RECT* prcItem, 
								LRESULT* plResult ) PURE;
};

typedef enum 
{
	SCRIPT_OK,
	SCRIPT_ERROR,
	SCRIPT_CONTINUE
}SCRIPT_RESULT_E;

class IScriptHandler
{
public:
	virtual BOOL LoadScript( HWND hReceiverWnd , DWORD dwReserved ) = 0;
	virtual BOOL UnloadScript() = 0;

	virtual SCRIPT_RESULT_E ExcuteFile( LPTSTR lpszFileName ) = 0;
	virtual SCRIPT_RESULT_E ExcuteCommand( LPTSTR lpszCommand ) = 0;
	virtual BOOL StopExcuteScript() = 0;
	virtual char* GetScriptResultString() = 0;
	virtual BOOL ResetScriptStatus() = 0;

	
};


class ILogelOprObserver
{
public:
	STDMETHOD(OnReset)( THIS_ DWORD dwOprCookie , WPARAM dwOprWParam, LPARAM dwOprLParam ) PURE;
	STDMETHOD(OnCleanup)( THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;
	
	/**
    * Invoked when parser opened.
    *
    * @param dwOprCookie: identifiers of working cookie .
    *
    * @param dwResult :show  whether the parser open operation is success .
    *
    * @return The return value is ignored.
    */
    STDMETHOD(OnParserOpened)(  THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;     
    
    /**
    * Invoked when parser closed.
    *
    * @param dwOprCookie: identifiers of working cookie .
    *
    * @param dwResult :show  whether the end work is success .
    *
    * @return The return value is ignored.
    */
    STDMETHOD(OnParserClosed)(  THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam  ) PURE;  

    STDMETHOD(OnConnected)(  THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam  ) PURE;     
    
    STDMETHOD(OnDisconnected)(  THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;  

    STDMETHOD( OnStartLoging )( THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;     
    
    STDMETHOD( OnStopLogging )(  THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;  

	STDMETHOD(OnRunningScript)( THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;

	STDMETHOD(OnOpenFiles)( THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;
	STDMETHOD(OnRenameFiles)( THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;
	STDMETHOD(OnCloseFiles)( THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;

	STDMETHOD( OnLogHandledData)( THIS_ DWORD dwOprCookie, CStringArray& strLogItems, 
		LPBYTE lpBuffer, DWORD dwSize )PURE;

	STDMETHOD( OnClickLogListItem )( DWORD dwOprCookie, CStringArray& strLogItems, 
		LPBYTE lpBuffer, DWORD dwSize ) PURE;
	
	STDMETHOD( OnReceivedData)( THIS_ DWORD dwOprCookie , WPARAM dwOprParam, LPARAM dwOprLParam ) PURE;
	
};
 

class ILogelOprSubscriber
{
public:

	/**
	 * Subscribe an observer object to receive notification . 
	 *
	 * @param pSink Points to the sink object which will receive the notification.
	 *
	 * @param uFlags :reserved
	 *
	 * @param lpdwCookie Points to the cookie assigned to the sink, which can be used to unregister the sink.
	 * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */
    virtual HRESULT SubscribeOperationObserver( ILogelOprObserver* pSink,
					ULONG uFlags,
					LPDWORD lpdwCookie )  = 0;

	/**
	 * Unsubscribe an observer object subscribed previously with SubscribeOperationObserver().
	 * 
	 * @param dwCook The cookie returned by SubscribeOperationObserver().
	 * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */    
    virtual HRESULT UnsubscribeOperationObserver ( DWORD dwCookie)  = 0;
				  
};



class ILogelDataObserver
{
public:
	/**
    * Invoked when parser opened.
    *
    * @param dwOprCookie: identifiers of working cookie .
    *
    * @param dwResult :show  whether the parser open operation is success .
    *
    * @return The return value is ignored.
    */
    STDMETHOD(OnDataReceived)(  THIS_ DWORD dwOprCookie , DWORD dwResult ) PURE;     
    
    /**
    * Invoked when parser closed.
    *
    * @param dwOprCookie: identifiers of working cookie .
    *
    * @param dwResult :show  whether the end work is success .
    *
    * @return The return value is ignored.
    */
    STDMETHOD(OnDataReceiveCanceled)(  THIS_ DWORD dwOprCookie, DWORD dwResult ) PURE;  

};
 

class ILogelDataSubscriber
{
public:

	/**
	 * Subscribe an observer object to receive notification . 
	 *
	 * @param pSink Points to the sink object which will receive the notification.
	 *
	 * @param uFlags :reserved
	 *
	 * @param lpdwCookie Points to the cookie assigned to the sink, which can be used to unregister the sink.
	 * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */
    virtual HRESULT SubscribeDataObserver( ILogelDataObserver* pSink,
					ULONG uFlags,
					LPDWORD lpdwCookie )  = 0;

	/**
	 * Unsubscribe an observer object subscribed previously with SubscribeOperationObserver().
	 * 
	 * @param dwCook The cookie returned by SubscribeOperationObserver().
	 * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */    
    virtual HRESULT UnsubscribeDataObserver ( DWORD dwCookie)  = 0;
				  
};

#endif