#include "stdafx.h"
#include "FileSysDlag.h"
#include "FeatureDll.h"
#include "sfs_func_def.h"
#include "msk_api_sfs.h"
#include "SHLOBJ.H"
#include "sfsunicadpter.h"
#include <cderr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//--------------------------------------------------------------------
// Extern function and type.
//--------------------------------------------------------------------
extern CP_FUNC_LIST gs_FuncList ;
extern CWnd * gs_pWndConPan;
extern "C" void FileSys_Init( );
//递归删除文件
void RecursionDel( const TCHAR       *find,
				  SFS_FIND_DATA_T    *fd);
//递归删除目录
void RecursionDelDir( const TCHAR       *find,
					 SFS_FIND_DATA_T    *fd);
//--------------------------------------------------------------------
// Macro define.
//--------------------------------------------------------------------
#define SIZE_M		  (1024*1024)

#define SFS_FUNS_CALL 1001
//hide "C"
#define ALLOC_HDISK_NAME(_NAME_PTR)\
	_NAME_PTR =new TCHAR[2];\
	_NAME_PTR[0]='C';  \
    _NAME_PTR[1]= 0;
//udisk "D"
#define ALLOC_UDISK_NAME(_NAME_PTR)\
		_NAME_PTR =new TCHAR[2];\
		_NAME_PTR[0]='D';  \
		_NAME_PTR[1]= 0;
//sdcard "E"
#define ALLOC_SDCARD_NAME(_NAME_PTR)\
		_NAME_PTR =new TCHAR[2];\
		_NAME_PTR[0]='E';  \
		_NAME_PTR[1]= 0;

//sdcard "F"
#define ALLOC_SDCARD2_NAME(_NAME_PTR)\
	    _NAME_PTR =new TCHAR[2];\
	    _NAME_PTR[0]='F';  \
	  	_NAME_PTR[1]= 0;

//--------------------------------------------------------------------
// Signal dispatch function define.
//--------------------------------------------------------------------

extern "C" int MsgDispatch(xSignalHeader psig)
{
	ASSERT(psig);
	((CFileSysDlg*)gs_pWndConPan)->DealCPMsg(psig);

	return 0;
}


IMPLEMENT_DYNCREATE(CFileSysDlg, CPropertyPage)
//CFileSysDlg::CFileSysDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CFileSysDlg::IDD, pParent)
CFileSysDlg::CFileSysDlg()	: CPropertyPage(CFileSysDlg::IDD)
{	
	//{{AFX_DATA_INIT(CFileSysDlg)
	m_strDeviceType = _T("");
	m_nImgSize = 0;
	//}}AFX_DATA_INIT
	m_bStartRename= FALSE;
	m_bStartNewDir= FALSE;
}

CFileSysDlg::~CFileSysDlg()
{
}

void CFileSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSysDlg)
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_cmbDev);
	DDX_Control(pDX, IDC_TREE_DIR, m_TreeDir);
	DDX_Control(pDX, IDC_LIST_DISK, m_ListDisk);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListFile);
	DDX_CBString(pDX, IDC_COMBO_DEVICE, m_strDeviceType);
	DDX_Text(pDX, IDC_EDIT_IMGSIZE, m_nImgSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSysDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CFileSysDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DISK, OnClickListDisk)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DIR, OnSelchangedTreeDir)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, OnRclickListFile)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_FILE, OnEndlabeleditListFile)
	ON_BN_CLICKED(IDC_BUTTON_FORMAT, OnButtonFormat)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_UNREGISTER, OnButtonUnregister)
	ON_COMMAND(ID_MENUITEM_DELFILE, OnMenuitemDelfile)
	ON_COMMAND(ID_MENUITEM_FILETOPC, OnMenuitemFiletopc)
	ON_COMMAND(ID_PC2FILE_FILE, OnMenuitemFilefrompc)
	ON_COMMAND(ID_PC2FILE_DIR, OnMenuitemDirfrompc)
	ON_COMMAND(ID_MENUITEM_OPENTXT, OnMenuitemOpentxt)
	ON_COMMAND(ID_MENUITEM_NEWDIR, OnMenuitemNewdir)
	ON_COMMAND(ID_MENUITEM_DELDIR, OnMenuitemDeldir)
	ON_BN_CLICKED(IDC_BUTTON_CREATEIMG, OnButtonCreateimg)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENUITEM_ATTR, OnMenuitemAttr)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_FILE, OnBeginlabeleditListFile)
	//apple add
	ON_CBN_SELCHANGE( IDC_COMBO_DEVICE, OnSelchangedDevice )
	ON_BN_CLICKED(IDC_BUTTON_LOADIMG, OnButtonLoadimg)
	ON_NOTIFY( NM_DBLCLK,IDC_LIST_FILE,OnDblClickFileList )
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSysDlg message handlers

BOOL CFileSysDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//init file system	
	FileSys_Init( );
	
	//register callback function of control panel
	ASSERT(gs_FuncList.fp_RegisterCallback != NULL);
	gs_FuncList.fp_RegisterCallback(MsgDispatch);	
	
	// Create the image list for the tree control
	m_ImageListFile.Create (IDB_BITMAP_FILESYS, 16, 1, RGB(0,255,0));
	m_ImageListDisk.Create(IDB_BITMAP_DISK ,32,1,RGB(0,255,0));

	// init device combox
    m_cmbDev.AddString(_T("UDisk"));
	m_cmbDev.AddString(_T("SD Card"));
	m_cmbDev.AddString(_T("SD Card2"));
	m_cmbDev.AddString(_T("Hide Disk"));

	//init Disk list
	m_ListDisk.SetImageList(&m_ImageListDisk,LVSIL_NORMAL);
	InitDiskList();

	//init tree ctrl
	m_TreeDir.SetImageList (&m_ImageListFile, TVSIL_NORMAL);	

	//init file list ctrl
	m_ListFile.SetImageList(&m_ImageListFile,LVSIL_SMALL);	
	int nCols = 4;
	int nColSize[] = {80, 60, 50, 150 };
	CString strColText[] = { _T("Name"), _T("Size"), _T("Type"), _T("Modified") };
	BuildColumns(nCols, nColSize, strColText);	

	//temp code 
	//CMenu *pmenu=GetSystemMenu(FALSE);
	//pmenu->DeleteMenu(SC_CLOSE,MF_BYCOMMAND);
	
	//default format type 
	//((CButton*)GetDlgItem(IDC_RADIO_FAT16))->SetCheck(BST_CHECKED);
	((CComboBox*)GetDlgItem( IDC_COMBO_DEVICE ))->SetCurSel(0);
	EnumAllDevice();
	SetButtonText();
	MessageBox(_T("应用软件开发部修改版--对控制面板文件系统部分功能进行了优化\n->添加了js文件导入功能\n->增加了同时删除多个文件的功能\n->增加了删除非空目录的功能（修改前只能删除空目录）"),_T("File System"),MB_OK);
	return TRUE;  
}

#define  DEAL_START      1
#define  DEAL_FINISH     2
#define  DEAL_ERROR      3

static volatile int s_deal_status = 0;

void CFileSysDlg:: DealCPMsg(void* pmsg)
{
	ASSERT(pmsg);
	FUN_CAll_SIG_T  *psig = (FUN_CAll_SIG_T *) pmsg;
	switch(psig->fun_info.fun_id)
	{
		
	case FUN_ID_REGISTER:
		{
			FUN_REGISTER_T&data =psig->fun_info.fun_param.regster;
		   
			SFS_ERROR_E returncode=	SFS_API_GetDeviceStatus_W(data.dev_name);
			if (returncode == SFS_ERROR_NONE) 
			{	
				//have register
				delete []data.dev_name;
				return;
			}

			SFS_DEVICE_FORMAT_E format_type;
			returncode = SFS_API_RegisterDevice_W(data.dev_name, &format_type);
			if(SFS_ERROR_NONE !=  returncode)	
			{
				MessageBox(_T("Before register must format disk"),_T("Register Failed"),MB_OK);
			}

			delete []data.dev_name;
		}
		break;	

	case FUN_ID_UNREGISTER:
		{
			FUN_UNREGISTER_T&data =psig->fun_info.fun_param.unregister;
			if(SFS_ERROR_NONE != SFS_API_UnRegisterDevice_W(data.dev_name))	
			{
				MessageBox(_T("Unregister failed !!!"),_T("Unregister"),MB_OK);
			}
			delete []data.dev_name;
		}
		break;

	case FUN_ID_FORMAT:
		{			
			FUN_FORMAT_T &data =psig->fun_info.fun_param.format;
			if (SFS_ERROR_NONE == SFS_API_Format_W(data.dev_name , data.format_type , NULL))
			{
				MessageBox(_T("Format success !!!"),_T("Format"),MB_OK);
			}
			else
			{
				MessageBox(_T("Format failed !!!"),_T("Format"),MB_OK);
			}
			delete []data.dev_name;
		}
		break;			

	case FUN_ID_GETSPACE:
		{					
			uint32 high;
			uint32 freelow, usedlow;				
			CString str;

			FUN_GETSPACE_T &data =psig->fun_info.fun_param.getspace;
			SFS_API_GetDeviceFreeSpace_W(data.dev_name , &high , &freelow);
			SFS_API_GetDeviceUsedSpace_W(data.dev_name , &high , &usedlow);

			str.Format(_T("Device Space:\n\n\n  FreeSpace: %d M \n\n  UsedSpace: %d M"), 					  
					  freelow/(1024*1024),usedlow/(1024*1024));
			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(str);
			delete []data.dev_name;
		}
		break;	

		//apple add
		case FUN_ID_SELCHANGE:
		{
			FUN_STATUS_T &data =psig->fun_info.fun_param.status;
			SFS_ERROR_E returncode=	SFS_API_GetDeviceStatus_W(data.dev_name);
			if (returncode == SFS_ERROR_NONE) 
			{
				GetDlgItem( IDC_BUTTON_REGISTER )->EnableWindow( FALSE );
				GetDlgItem( IDC_BUTTON_UNREGISTER )->EnableWindow( TRUE );
			}
			else
			{
				GetDlgItem( IDC_BUTTON_REGISTER )->EnableWindow( TRUE );
				GetDlgItem( IDC_BUTTON_UNREGISTER )->EnableWindow( FALSE );		
			}	
			break;
		}

		case FUN_ID_STATUS:
		{
			FUN_STATUS_T &data =psig->fun_info.fun_param.status;
			SFS_ERROR_E returncode=	SFS_API_GetDeviceStatus_W(data.dev_name);
			
			if (returncode != SFS_ERROR_NONE) 
			{
				delete []data.dev_name;
				return;
			}
			//udisk
			if (*data.dev_name=='D')
			{				
				m_ListDisk.InsertItem(m_ListDisk.GetItemCount(),_T("UDisk"),0);		
			}
			//sd card
			if (*data.dev_name=='E')
			{
				m_ListDisk.InsertItem(m_ListDisk.GetItemCount(),_T("SD Card"),0);
			}

			//sd card2
			if (*data.dev_name=='F')
			{
				m_ListDisk.InsertItem(m_ListDisk.GetItemCount(),_T("SD Card2"),0);
			}

			if (*data.dev_name== 'C')
			{
				m_ListDisk.InsertItem(m_ListDisk.GetItemCount(),_T("Hide Disk"),0);
			}
					
			delete []data.dev_name;

		}
		break;
		
	case FUN_ID_CREATE:
		{			
			FUN_CREATE_T &data =psig->fun_info.fun_param.create;
			SFS_API_CreateFile_W(data.name, data.mode,NULL,NULL);
			delete []data.name;
		}
		break;
		
	case FUN_ID_DELETE:
		{			
			FUN_DELETE_T &data =psig->fun_info.fun_param.del;
			SFS_HANDLE      sfs_handle   = 0;
			//open file
			sfs_handle = SFS_API_CreateFile_W(data.name , SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);			
			if(NULL == sfs_handle) 
			{
				delete []data.name;
				return;
			}
			SFS_API_CloseFile(sfs_handle);

			SFS_API_DeleteFile_W(data.name, NULL);
			delete []data.name;
		}
		break;		
	case FUN_ID_RENAME:
		{			
			FUN_RENAME_T &data =psig->fun_info.fun_param.rename;
			SFS_HANDLE    sfs_handle   = 0;
			//open file

			sfs_handle = SFS_API_CreateFile_W(data.old_name , SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);			
			if(NULL != sfs_handle) 
			{
				SFS_API_CloseFile(sfs_handle);
			}

			SFS_API_RenameFile_W(data.old_name,data.new_name ,NULL);
			delete []data.old_name;
			delete []data.new_name;
		}
		break;	
	case FUN_ID_DELETE_DIR:
		{			
			FUN_DELETEDIR_T &data =psig->fun_info.fun_param.deletedir;
			SFS_API_DeleteDirectory_W(data.path);
			delete []data.path;
		}
		break;	

	case FUN_ID_CREATE_DIR:
		{			
			FUN_CREATEDIR_T &data =psig->fun_info.fun_param.createdir;
			SFS_API_CreateDirectory_W(data.path);
			delete []data.path;
		}
		
		break;

	case FUN_ID_OPENFILE:
		{
			FUN_READFILE_T &data =psig->fun_info.fun_param.readfile;
			
			CString strFile = data.path_name;
			strFile= GetNameFromPath(strFile);
			strFile = _T("c:\\") + strFile;
			ReadFileToPC( data.path_name, strFile.GetBuffer( strFile.GetLength()) ) ;
			
			ShellExecute( NULL, _T("open"), strFile, NULL, NULL, SW_SHOWNORMAL);
						
			DeleteFile( strFile );
			delete []data.path_name;
			break;
		}

	case FUN_ID_READFILE:
		{
			FUN_READFILE_T &data =psig->fun_info.fun_param.readfile;

			
			ReadFileToPC( data.path_name, data.pc_path ) ;
		
			delete []data.path_name;
			delete []data.pc_path;
		}
		break;
	case FUN_ID_READTXT:
		{
			FUN_READFILE_T &data =psig->fun_info.fun_param.readfile;

			void*		    buf_ptr      = NULL;
			SFS_HANDLE      sfs_handle   = 0;
			uint32			to_read_size = 0;
			uint32          read_size    = 0;

			//open file
			sfs_handle = SFS_API_CreateFile_W(data.path_name , SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);					
			delete []data.path_name;
			if(NULL == sfs_handle) return;
			
			//get file size and alloc buffer
			if(SFS_ERROR_NONE != SFS_API_GetFileSize(sfs_handle, &to_read_size)) return;
			buf_ptr = (void*)new TCHAR[to_read_size+1];
			memset(buf_ptr, 0, to_read_size+1);

			//read file 
			if(SFS_ERROR_NONE != SFS_API_ReadFile(sfs_handle, buf_ptr, to_read_size+1, &read_size, NULL))
			{
				SFS_API_CloseFile(sfs_handle);
				delete []buf_ptr;
				return;
			}
             
			CString strText = (TCHAR*)buf_ptr;
			if(CopyStringToClipboard(strText))
			{
			    MessageBox( _T("相关文字已经复制到剪贴板 "),_T("Open Text File "),MB_OK);
			}
		    MessageBox( strText,_T("Open Text File "),MB_OK);
			//close file and free memory
			SFS_API_CloseFile(sfs_handle);
			delete []buf_ptr;			
						
		}
		break;

	case FUN_ID_WRITEFILE:
		{
			FUN_WRITEFILE_T &data =psig->fun_info.fun_param.writefile;
			
			void*		    buf_ptr      = NULL;
			SFS_HANDLE      sfs_handle   = 0;
			uint32          write_size   = 0;
			
			sfs_handle = SFS_API_CreateFile_W(data.path_name, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);			
			delete []data.path_name;
			if(NULL == sfs_handle) 
            {
                delete[] data.buffer;    s_deal_status = DEAL_ERROR;
                return;
            }
			

			if(data.bufsize == 0 ||SFS_ERROR_NONE != SFS_API_WriteFile(sfs_handle, data.buffer, data.bufsize, &write_size, NULL))
			{	
				SFS_API_CloseFile(sfs_handle);
				delete[] data.buffer;    s_deal_status = DEAL_ERROR;
				return;
			}

			SFS_API_CloseFile(sfs_handle);
			delete[] data.buffer;        s_deal_status = DEAL_FINISH;

		}
		break;
	
	case FUN_ID_FINDFIRST://find all dir and files  
		{
			//items -> _T("Name"), _T("Size"), _T("Type"), _T("Modified") 
			CString name;
			CString size;
			CString Type;
			CString Modified;
				
			// Delete all of the children of Current selected Item.
			HTREEITEM hCurItem= m_TreeDir.GetSelectedItem();
			if (m_TreeDir.ItemHasChildren(hCurItem))
			{
				HTREEITEM hNextItem;
				HTREEITEM hChildItem = m_TreeDir.GetChildItem(hCurItem);				
				while (hChildItem != NULL)
				{
					hNextItem = m_TreeDir.GetNextItem(hChildItem, TVGN_NEXT);
					m_TreeDir.DeleteItem(hChildItem);
					hChildItem = hNextItem;
				}
			}
						
			//remove all item and add new items
			m_ListFile.DeleteAllItems();
			
			FUN_FINDFIRST_T &data =psig->fun_info.fun_param.findfirst; 
			SFS_HANDLE handle_find=SFS_API_FindFirstFile_W(data.find_name, data.find_ptr);
			if (NULL== handle_find)
			{
				free(data.find_name);
				free(data.find_ptr);
				break;
			}

			//add items to file list 
			do
			{

				name=""; size=""; Type=""; Modified=""; 
				int nIndx = m_ListFile.GetItemCount();
				// name
				name=data.find_ptr->name; 

				//size
				size.Format(_T("%.2fM"),(float)data.find_ptr->length/SIZE_M);	
				
				//type ->dir or file
				if (SFS_ATTR_DIR&data.find_ptr->attr)
				{
					Type="Dir";
					m_ListFile.InsertItem( nIndx, name.GetBuffer(SFS_MAX_PATH), 2);
					m_ListFile.SetItemData( nIndx, 1 );
					
					HTREEITEM hItem= m_TreeDir.GetSelectedItem();
					m_TreeDir.InsertItem (name.GetBuffer(SFS_MAX_PATH), 2, 3, hItem, TVI_LAST);
					m_TreeDir.Expand(hItem, TVE_EXPAND);
				}
				else
				{
					Type="File";
					m_ListFile.InsertItem(nIndx, (LPCTSTR) name, 0);
					m_ListFile.SetItemData( nIndx, 0 );
				}

				//Modified time				
				Modified.Format(_T("%d-%.2d-%.2d %.2d:%.2d:%.2d"),
								data.find_ptr->modify_Date.year,
								data.find_ptr->modify_Date.mon,
								data.find_ptr->modify_Date.mday,
								data.find_ptr->modify_time.hour,
								data.find_ptr->modify_time.min,								
								data.find_ptr->modify_time.sec								
								);
				//add other subitems to list ctrl
				m_ListFile.SetItemText(nIndx,1,size);
				m_ListFile.SetItemText(nIndx,2,Type);
				m_ListFile.SetItemText(nIndx,3,Modified);
				
			} while(SFS_ERROR_NONE == SFS_API_FindNextFile(handle_find, data.find_ptr));	

			SFS_API_FindClose(handle_find);
			free(data.find_name);
			free(data.find_ptr);
				
		}
		break;
    case FUN_ID_REC_FIND_FILE://find all dir and files
		{
			FUN_FINDFIRST_T &data =psig->fun_info.fun_param.findfirst;
			RecursionDel(data.find_name,data.find_ptr);
		}
		break;
	case FUN_ID_REC_DEL_DIR_FILE:
		{
			FUN_FINDFIRST_T &data =psig->fun_info.fun_param.findfirst;
			RecursionDelDir(data.find_name,data.find_ptr);
		}
		break;
	//APPLE ADDED:
	case FUN_ID_READ_DIR:
		{
			FUN_READFILE_T &data =psig->fun_info.fun_param.readfile;
			ReadDirFilesToPC( data.path_name, data.pc_path );
			
			delete []data.path_name;
			delete []data.pc_path;
		}
		break;

		
	case FUN_ID_ATTR://get dir and files attr  
		{
			CString name;
			CString short_name;
			CString size;
			CString Type;

			CString Create;
			CString Modified;
			CString Access;
			 
			CString attr;
							
			FUN_FINDFIRST_T &data =psig->fun_info.fun_param.findfirst; 
			SFS_HANDLE handle_find=SFS_API_FindFirstFile_W(data.find_name, data.find_ptr);
			if (NULL== handle_find)
			{
				delete []data.find_name;
				delete []data.find_ptr;
				break;
			}

			//get dir and file attr info
			// name
			name		=data.find_ptr->name; 
			short_name	=data.find_ptr->short_name; 
			//size
			size.Format(_T("%.2fk"),(float)data.find_ptr->length/1024);	
			
			//type ->dir or file
			if (SFS_ATTR_DIR&data.find_ptr->attr)
			{
				Type="Dir";
			}
			else
			{
				Type="File";
			}

			//Create time				
			Create.Format(_T("%d-%.2d-%.2d %.2d:%.2d:%.2d"),
				data.find_ptr->create_Date.year,
				data.find_ptr->create_Date.mon,
				data.find_ptr->create_Date.mday,
				data.find_ptr->create_time.hour,
				data.find_ptr->create_time.min,								
				data.find_ptr->create_time.sec								
				);
			
			//Modified time				
			Modified.Format(_T("%d-%.2d-%.2d %.2d:%.2d:%.2d"),
				data.find_ptr->modify_Date.year,
				data.find_ptr->modify_Date.mon,
				data.find_ptr->modify_Date.mday,
				data.find_ptr->modify_time.hour,
				data.find_ptr->modify_time.min,								
				data.find_ptr->modify_time.sec								
				);

			//Access time
			Access.Format(_T("%d-%.2d-%.2d"),
				data.find_ptr->modify_Date.year,
				data.find_ptr->modify_Date.mon,
				data.find_ptr->modify_Date.mday								
				);			
			
			//attr info                                              
			CString attrflag,strhex;
			for (int i=0;i<sizeof(data.find_ptr->attr)*8;i++)
			{
				(data.find_ptr->attr&(0x8000>>i))? attrflag+="1":attrflag+="0";
			}
			strhex.Format(_T("(0x%04X)"),(short)data.find_ptr->attr);
			attrflag+=strhex;
			
			//show message
			attr="--------------------------------------------"
				 "\nname: " + name +
				 "\n\nsize:    "+ size +
				 "\n\ntype:    "+ Type +
				 "\n------------------------------------------"
				 "\ncreate time: "+Create +				  
				 "\n\nmodify time: "+Modified+
				 "\n\naccess time: "+Access  +
				 "\n------------------------------------------"
				 "\n\nattr flag: "+attrflag; 

			MessageBox((LPCTSTR)attr,_T("Attr"),MB_OK);	
			
			SFS_API_FindClose(handle_find);
			delete []data.find_name;
			delete []data.find_ptr;
				
		}
		break;				
	default:

		break;
	}
	
}

void CFileSysDlg::InitDiskList()
{
	m_ListDisk.SetBkColor(GetSysColor(CTLCOLOR_MSGBOX));
	m_ListDisk.SetTextBkColor(GetSysColor(CTLCOLOR_MSGBOX));
	
}

void CFileSysDlg::BuildColumns(int nCols, int * nWidth, CString * csCol)
{
	//insert columns
	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp;
	for(i = 0; i < nCols; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = (_TCHAR*)(LPCTSTR)csCol[i];
		lvc.cx = nWidth[i];
		lvc.fmt = LVCFMT_LEFT;
		m_ListFile.InsertColumn(i,&lvc);
	}

}

void CFileSysDlg::EnumAllDevice()
{
	m_ListDisk.DeleteAllItems();
	TCHAR* wDisk;		
	
	//get UDisk status
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;

	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_STATUS;
	 ALLOC_UDISK_NAME(wDisk);
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.status.dev_name= wDisk;	
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr , CTRLPAN_TASK_ID );	

	//get SD Card Status
	sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
					     SFS_FUNS_CALL,
		                 sizeof(FUN_CAll_SIG_T),
		                 CTRLPAN_TASK_ID
						 );
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_STATUS;	
	 ALLOC_SDCARD_NAME(wDisk);			
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.status.dev_name= wDisk;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );

	//get SD Card2 Status
	sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
		SFS_FUNS_CALL,
		sizeof(FUN_CAll_SIG_T),
		CTRLPAN_TASK_ID
		);
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_STATUS;	
	ALLOC_SDCARD2_NAME(wDisk);			
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.status.dev_name= wDisk;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );

	//get Hide Disk status
	sig_ptr = (xSignalHeaderRec *)PNULL;
	
	InnerSCICreateSignal(&sig_ptr,
		SFS_FUNS_CALL,
		sizeof(FUN_CAll_SIG_T),
		CTRLPAN_TASK_ID
		);
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_STATUS;
	ALLOC_HDISK_NAME(wDisk);
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.status.dev_name= wDisk;	
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr , CTRLPAN_TASK_ID );	
	
	OnSelchangedDevice();  
	
}

void CFileSysDlg::OnButtonFormat() 
{
	UpdateData();


	CString strDevice= m_strDeviceType;
	if(strDevice == "") 
	{
		MessageBox(_T("Please select Device !"),_T("Format"),MB_OK);
		((CComboBox*)GetDlgItem(IDC_COMBO_DEVICE))->ShowDropDown();
		return;	
	}
	
	//device type
	TCHAR* wDisk;	
	if (strDevice == _T("UDisk"))
	{
		ALLOC_UDISK_NAME(wDisk);	
	}
	else if(strDevice == _T("SD Card"))
	{
		ALLOC_SDCARD_NAME(wDisk);		
	}
	else if(strDevice == _T("SD Card2"))
	{
		ALLOC_SDCARD2_NAME(wDisk);		
	}
	else if(strDevice == _T("Hide Disk"))
	{
		ALLOC_HDISK_NAME(wDisk);		
	}
	else
	{
		return;
	}
/*
	//format type
	SFS_DEVICE_FORMAT_E fmt_type ;
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_FAT16))->GetCheck())
	{
		fmt_type= SFS_FAT16_FORMAT;
	}
	else if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_FAT32))->GetCheck())
	{
		fmt_type= SFS_FAT32_FORMAT;
	}
	else
	{
		return;
	}
*/		
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
					     CTRLPAN_TASK_ID
						 );
	
	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_FORMAT;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.format.format_type=SFS_FAT16_FORMAT;

	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.format.dev_name= wDisk;


	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );

}



void CFileSysDlg::OnClickListDisk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem;
	POSITION pos = m_ListDisk.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_ListDisk.GetNextSelectedItem(pos);
	}
	else
	{
		return;
	}

	CString strText = m_ListDisk.GetItemText(nItem,0);
	if (strText == _T("UDisk"))
	{
		xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
		InnerSCICreateSignal(&sig_ptr,
							 SFS_FUNS_CALL,
							 sizeof(FUN_CAll_SIG_T),
							 CTRLPAN_TASK_ID
							 );
		
		TCHAR* wDisk;
		// get free space 
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_GETSPACE;
		ALLOC_UDISK_NAME(wDisk);
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.getspace.dev_name= wDisk;
		SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );
		
		// Add the parent item
		m_TreeDir.DeleteAllItems();
		HTREEITEM htItem = m_TreeDir.InsertItem(_T("UDisk"),4,4);
		m_TreeDir.SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );
		m_TreeDir.InsertItem (_T("D:"), 1, 1, htItem, TVI_LAST);
		
	}

	if (strText == _T("SD Card"))
	{
		xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
		InnerSCICreateSignal(&sig_ptr,
							 SFS_FUNS_CALL,
							 sizeof(FUN_CAll_SIG_T),
							 CTRLPAN_TASK_ID
							 );
		
		TCHAR* wDisk;
		// get free space 
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_GETSPACE;
		ALLOC_SDCARD_NAME(wDisk);
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.getspace.dev_name= wDisk;
		SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );	
			
		// Add the parent item
		m_TreeDir.DeleteAllItems();
		HTREEITEM htItem = m_TreeDir.InsertItem(_T("SD Card"),4,4);
		m_TreeDir.SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );
		m_TreeDir.InsertItem (_T("E:"), 1, 1, htItem, TVI_LAST);	
	
	}

	if (strText == _T("SD Card2"))
	{
		xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
		InnerSCICreateSignal(&sig_ptr,
			SFS_FUNS_CALL,
			sizeof(FUN_CAll_SIG_T),
			CTRLPAN_TASK_ID
			);
		
		TCHAR* wDisk;
		// get free space 
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_GETSPACE;
		ALLOC_SDCARD2_NAME(wDisk);
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.getspace.dev_name= wDisk;
		SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );	
		
		// Add the parent item
		m_TreeDir.DeleteAllItems();
		HTREEITEM htItem = m_TreeDir.InsertItem(_T("SD Card2"),4,4);
		m_TreeDir.SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );
		m_TreeDir.InsertItem (_T("F:"), 1, 1, htItem, TVI_LAST);	
		
	}

	if (strText == _T("Hide Disk"))
	{
		xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
		InnerSCICreateSignal(&sig_ptr,
			SFS_FUNS_CALL,
			sizeof(FUN_CAll_SIG_T),
			CTRLPAN_TASK_ID
			);
		
		TCHAR* wDisk;
		// get free space 
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_GETSPACE;
		ALLOC_HDISK_NAME(wDisk);
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.getspace.dev_name= wDisk;
		SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );	
		
		// Add the parent item
		m_TreeDir.DeleteAllItems();
		HTREEITEM htItem = m_TreeDir.InsertItem(_T("Hide Disk"),4,4);
		m_TreeDir.SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );
		m_TreeDir.InsertItem (_T("C:"), 1, 1, htItem, TVI_LAST);	
		
	}
		*pResult = 0;	

}

void CFileSysDlg::OnButtonRegister() 
{
	UpdateData();
	
	TCHAR* wDisk;	
	if (m_strDeviceType == "UDisk")
	{
		ALLOC_UDISK_NAME(wDisk);	
	}
	else if(m_strDeviceType == "SD Card")
	{
		ALLOC_SDCARD_NAME(wDisk);		
	}
	else if(m_strDeviceType == "SD Card2")
	{
		ALLOC_SDCARD2_NAME(wDisk);		
	}
	else if(m_strDeviceType == _T("Hide Disk"))
	{
		ALLOC_HDISK_NAME(wDisk);		
	}
	else
	{
		return;
	}

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );
	
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_REGISTER;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.regster.dev_name= wDisk;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );
	
	
	EnumAllDevice();
	
}

void CFileSysDlg::OnButtonUnregister() 
{
	UpdateData();
	
	TCHAR* wDisk;	
	if (m_strDeviceType == _T("UDisk"))
	{
		ALLOC_UDISK_NAME(wDisk);
	}
	else if(m_strDeviceType ==_T("SD Card"))		
	{
		ALLOC_SDCARD_NAME(wDisk);			
	}
	else if(m_strDeviceType ==_T("SD Card2"))		
	{
		ALLOC_SDCARD2_NAME(wDisk);			
	}
	else if(m_strDeviceType == _T("Hide Disk"))
	{
		ALLOC_HDISK_NAME(wDisk);		
	}
	else
	{
		return;
	}

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
					     sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );
	
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_UNREGISTER;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.unregister.dev_name= wDisk;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );

	EnumAllDevice();
}

CString  CFileSysDlg:: GetNameFromPath(CString path)
{
	int nStart=path.ReverseFind('\\');
	CString filename= ((LPCTSTR )path)+nStart+1;
	return filename;
}

CString CFileSysDlg::GetCurPath()
{
	CString strEmpty="";
	HTREEITEM hRootItem = m_TreeDir.GetRootItem();
	HTREEITEM hItem		= m_TreeDir.GetSelectedItem();
	if (NULL== hItem || hItem ==hRootItem )
	{
		return strEmpty;
	}

	CString strText= m_TreeDir.GetItemText(hItem);
	if(strText.GetLength() == 0)
	{
		return strEmpty;
	}

	//contruct path string
	strText+="\\";
	HTREEITEM hParentItem=NULL;
	while ( (hParentItem= m_TreeDir.GetParentItem(hItem)) && hParentItem!= hRootItem  )
	{	
		strText="\\" + strText;
		strText=m_TreeDir.GetItemText(hParentItem)+ strText;
		hItem=hParentItem;
	}
	
	return strText;
}

void CFileSysDlg::BrowseDir(CString strDirName)
{

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );
	
	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_FINDFIRST;
	
	TCHAR* filename  = strDirName.GetBuffer(SFS_MAX_PATH);
	TCHAR* wfilename = (TCHAR*)malloc((SFS_MAX_PATH+1)*sizeof(TCHAR));
	if (wfilename == NULL)
	{	
		ASSERT(0);
	}
	_tcscpy(wfilename,filename);

	SFS_FIND_DATA_T* pFindFirst= (SFS_FIND_DATA_T* )malloc(sizeof(SFS_FIND_DATA_T));
	if (pFindFirst == NULL)
	{	
		ASSERT(0);
	}
	memset(pFindFirst,0,sizeof(SFS_FIND_DATA_T));

	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_name= wfilename;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_ptr=pFindFirst;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );	

}
void CFileSysDlg::SendRecFindMsg(CString strDirName) //发送递归查找的消息
{

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );

	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_REC_FIND_FILE;

	TCHAR* filename  = strDirName.GetBuffer(SFS_MAX_PATH);
	TCHAR* wfilename = (TCHAR*)malloc((SFS_MAX_PATH+1)*sizeof(TCHAR));
	if (wfilename == NULL)
	{
		ASSERT(0);
	}
	_tcscpy(wfilename,filename);
       OutputDebugString(__FILE__ + __LINE__ + CString(wfilename) + _T("\n"));
	SFS_FIND_DATA_T* pFindFirst= (SFS_FIND_DATA_T* )malloc(sizeof(SFS_FIND_DATA_T));
	if (pFindFirst == NULL)
	{
		ASSERT(0);
	}
	memset(pFindFirst,0,sizeof(SFS_FIND_DATA_T));

	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_name= wfilename;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_ptr=pFindFirst;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );

}

void CFileSysDlg::SendRecDelDirMsg(CString strDirName) //发送删除目录的消息
{
    strDirName += _T("\\*");

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );

	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_REC_DEL_DIR_FILE;

	TCHAR* filename  = strDirName.GetBuffer(SFS_MAX_PATH);
	TCHAR* wfilename = (TCHAR*)malloc((SFS_MAX_PATH+1)*sizeof(TCHAR));
	if (wfilename == NULL)
	{
		ASSERT(0);
	}
	_tcscpy(wfilename,filename);
       OutputDebugString(__FILE__ + __LINE__ + CString(wfilename) + _T("\n"));
	SFS_FIND_DATA_T* pFindFirst= (SFS_FIND_DATA_T* )malloc(sizeof(SFS_FIND_DATA_T));
	if (pFindFirst == NULL)
	{
		ASSERT(0);
	}
	memset(pFindFirst,0,sizeof(SFS_FIND_DATA_T));

	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_name= wfilename;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_ptr=pFindFirst;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID );

}

void CFileSysDlg::OnSelchangedTreeDir(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//get current item
	
	HTREEITEM hItem= pNMTreeView->itemNew.hItem;

	HTREEITEM hRootItem= m_TreeDir.GetRootItem();
	if (NULL== hItem || hItem ==hRootItem )
		return ;

	m_TreeDir.SelectItem(hItem);

	//contruct path string
	CString strText = GetCurPath()+"*";
	
	BrowseDir(strText);	
	
	*pResult = 0;
}

void CFileSysDlg::OnBeginlabeleditListFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	//create new dir 
	if (m_bStartNewDir) return;

	//rename file
	int nItem;
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_ListDisk.GetNextSelectedItem(pos);
	}
	else
	{
		return;
	}	

	m_oldname= m_ListFile.GetItemText(nItem,0);	
	//start rename
	m_bStartRename = TRUE;

	
	*pResult = 0;
}

void CFileSysDlg::OnEndlabeleditListFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	//rename file or dir
	if (m_bStartRename)
	{
		m_bStartRename =FALSE;

		if(pDispInfo->item.pszText==NULL) return;
		//contruct path string
		CString newname= GetCurPath()+pDispInfo->item.pszText;	
		CString oldname= GetCurPath()+ m_oldname;
		if (newname == oldname)	return;

		//send message
		xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
		InnerSCICreateSignal(&sig_ptr,
			SFS_FUNS_CALL,
			sizeof(FUN_CAll_SIG_T),
			CTRLPAN_TASK_ID
			);
		
		// set the parameters
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_RENAME;
		
		_TCHAR *poldname	=oldname.GetBuffer(SFS_MAX_PATH);
		TCHAR*wpoldname   =new TCHAR[_tcslen(poldname)+1];
		_tcscpy(wpoldname,poldname);
		
		_TCHAR *pnewname	=newname.GetBuffer(SFS_MAX_PATH);
		TCHAR*wpnewname   =new TCHAR[_tcslen(pnewname)+1];
		_tcscpy(wpnewname,pnewname);
		
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.rename.old_name= wpoldname;
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.rename.new_name= wpnewname;
		SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);

	}

	//create dir
	if (m_bStartNewDir)	
	{
		m_bStartNewDir=FALSE;

		CString strType =m_ListFile.GetItemText(pDispInfo->item.iItem ,2) ;
		if (strType != "Dir") return;
		
		//contruct path string
		CString strText=GetCurPath() + pDispInfo->item.pszText;		
		_TCHAR *filename=strText.GetBuffer(strText.GetLength());
		CreateDir( filename );
	}

	//refresh file list 
	BrowseDir( GetCurPath()+"*" );			

	
	*pResult = 0;
}

void CFileSysDlg::CreateDir( TCHAR* szDir )
{
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
		SFS_FUNS_CALL,
		sizeof(FUN_CAll_SIG_T),
		CTRLPAN_TASK_ID
		);
	
	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_CREATE_DIR;
	
	TCHAR*wfilename =new TCHAR[_tcslen(szDir)+1];
	_tcscpy(wfilename,szDir);
	
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.createdir.path= wfilename;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);	
}

void CFileSysDlg::OnRclickListFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnContextMenu();	
	*pResult = 0;
}

void CFileSysDlg::OnContextMenu()
{
	CPoint point;
	::GetCursorPos(&point);
	point.Offset(5, 5);
		
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_CONTEXT));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		this);
}

void CFileSysDlg::OnMenuitemNewdir() 
{
	CString strText = GetCurPath();
	if (strText == "")  return;	
	
	m_ListFile.InsertItem(0, _T("New Dir"), 2);
	m_ListFile.SetItemText(0,2,_T("Dir"));
	m_ListFile.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_bStartNewDir = TRUE;
	
	//edit new dir
	m_ListFile.EditLabel(0);
}

void CFileSysDlg::OnMenuitemDeldir() 
{	
	//get current select item
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int nItem = m_ListFile.GetNextSelectedItem(pos);	

	//contruct path string
	CString strText= m_ListFile.GetItemText(nItem,0);
	strText=GetCurPath()+strText;

	SendRecDelDirMsg(strText);
	BrowseDir( GetCurPath()+"*" );
	#if 0
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );
	
	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_DELETE_DIR;

	_TCHAR *filename=strText.GetBuffer(strText.GetLength());
	TCHAR*wfilename =new TCHAR[_tcslen(filename)+1];
	_tcscpy(wfilename,filename);

	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.createdir.path= wfilename;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);

	//refresh file list 
	BrowseDir( GetCurPath()+"*" );
	#endif
	
}
#if 0
void CFileSysDlg::OnMenuitemDelfile() 
{
	//get current select item
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int nItem = m_ListFile.GetNextSelectedItem(pos);	

	//contruct path string
	CString strText= m_ListFile.GetItemText(nItem,0);
	strText=GetCurPath()+strText;

	//send message
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						);
	
	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_DELETE;

	_TCHAR *filename=strText.GetBuffer(strText.GetLength());
	TCHAR* wfilename =new TCHAR[_tcslen(filename)+1];
	_tcscpy(wfilename,filename);

	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.del.name= wfilename;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);	
	
	//refresh file list 
	BrowseDir( GetCurPath()+"*" );	

}
#endif
void CFileSysDlg::OnMenuitemDelfile() //右键删除文件，可以支持同时删除多个文件
{
    for(int i=0;i<m_ListFile.GetItemCount();i++)
    {
        if(m_ListFile.GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
        {
            CString strText= m_ListFile.GetItemText(i,0);
            strText=GetCurPath()+strText;
            //send message
            xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
            InnerSCICreateSignal(&sig_ptr,
	  				     SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						);

            // set the parameters
            ((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_DELETE;

            _TCHAR *filename=strText.GetBuffer(strText.GetLength());
            TCHAR* wfilename =new TCHAR[_tcslen(filename)+1];
            _tcscpy(wfilename,filename);

            ((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.del.name= wfilename;
            SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);
        }

    }
	//refresh file list
    BrowseDir( GetCurPath()+"*" );

}
void CFileSysDlg::OnMenuitemFiletopc() 
{
	_TCHAR path[MAX_PATH];
	BROWSEINFO bi;
	
	memset(&bi,0,sizeof(bi));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = path;
	LPITEMIDLIST lpid = SHBrowseForFolder(&bi);			
	if( NULL != lpid )
	{
		SHGetPathFromIDList(lpid,path);

		//get current select item
		POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
		//apple change for supporting dir and multisel
		while( pos != NULL )
		{
			int nItem = m_ListFile.GetNextSelectedItem(pos);
			
			//contruct path string
			CString strText= m_ListFile.GetItemText(nItem,0);
			CString strPath = path;	
			strPath += "\\";
			strPath += strText;

			//send message
			xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			InnerSCICreateSignal(&sig_ptr,
				SFS_FUNS_CALL,
				sizeof(FUN_CAll_SIG_T),
				CTRLPAN_TASK_ID
				);
			
			// set the parameters
			DWORD dwData = m_ListFile.GetItemData( nItem );
			if( dwData == 1 ) //dir
			{ 				
				if( !CreateDirectory( strPath, NULL ))
				{
					InnerSCIFreeSignal( sig_ptr);
					continue;
				}
				((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id= FUN_ID_READ_DIR;
			}
			else
			{
				((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id= FUN_ID_READFILE;		
			}

			strText=GetCurPath()+strText;

			
			
			_TCHAR *filename=strText.GetBuffer(SFS_MAX_PATH);
			TCHAR* wfilename =new TCHAR[_tcslen(filename)+1];
			_tcscpy(wfilename,filename);
			_TCHAR *pcfilename=strPath.GetBuffer(SFS_MAX_PATH);
			TCHAR* wpcfilename =new TCHAR[_tcslen(pcfilename)+1];
			_tcscpy(wpcfilename,pcfilename);			
			
			((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.readfile.path_name= wfilename;
			((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.readfile.pc_path = wpcfilename;
			SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);	
		}		
	}	
}

void CFileSysDlg::OnMenuitemDirfrompc() 
{
	CString strText = GetCurPath();
	if (strText == "")  return;

	CWaitCursor wait;
	_TCHAR path[MAX_PATH];
	BROWSEINFO bi;
	
	memset(&bi,0,sizeof(bi));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = path;
	LPITEMIDLIST lpid = SHBrowseForFolder(&bi);			
	if( NULL != lpid )
	{
		SHGetPathFromIDList(lpid,path);

		CString strDirName = GetNameFromPath( path );
		
		strDirName = strText + strDirName;

		WriteDirFromPC( strDirName ,path );
	}		
	
	//refresh file list 
	BrowseDir( GetCurPath()+"*" );	
	
}

void CFileSysDlg::OnMenuitemFilefrompc() 
{
	CString strText = GetCurPath();
	if (strText == "")  return;

	//Get file Path
	_TCHAR BASED_CODE szFilter[] = _T("Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER, szFilter);
#define MAX_FILE_BUF_LEN  2048  // can not be more than 2048 for WIN2000.
	_TCHAR *pBuf = new _TCHAR[MAX_FILE_BUF_LEN];
	memset(pBuf,0,sizeof(_TCHAR)*(MAX_FILE_BUF_LEN));

	dlg.m_ofn.lpstrFile = pBuf;
	dlg.m_ofn.nMaxFile = MAX_FILE_BUF_LEN;

	int iReturn = dlg.DoModal();
	if(iReturn !=  IDOK)
	{
		if(CommDlgExtendedError() == FNERR_BUFFERTOOSMALL)
		{
			AfxMessageBox(_T("You selected too much files!"));
		}
		delete [] pBuf;
		return;
	}

	CWaitCursor wait;
	
	POSITION pos = dlg.GetStartPosition();
	while (pos != NULL)
	{
		CString strFilePath = dlg.GetNextPathName(pos);
		CString strFileName = GetNameFromPath( strFilePath);
		strFileName = strText + strFileName;
		WriteFileFromPC( strFileName, strFilePath.GetBuffer( strFilePath.GetLength() ) );
		Sleep(50);
	}	

	delete [] pBuf;

	//refresh file list 
	BrowseDir( GetCurPath()+"*" );	
	
}

void CFileSysDlg::OnMenuitemOpentxt() 
{
	//get current select item
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int nItem = m_ListFile.GetNextSelectedItem(pos);	

	//contruct path string
	CString strText= m_ListFile.GetItemText(nItem,0);
	strText=GetCurPath()+strText;

	//send message
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
						 SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						 );
	
	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_READTXT;

	_TCHAR *filename=strText.GetBuffer(SFS_MAX_PATH);
	TCHAR* wfilename =new TCHAR[_tcslen(filename)+1];
	_tcscpy(wfilename,filename);

	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.readfile.path_name= wfilename;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);	
	

}


void CFileSysDlg::OnMenuitemAttr() 
{
	//get current select item
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int nItem = m_ListFile.GetNextSelectedItem(pos);	

	//contruct path string
	CString strText= m_ListFile.GetItemText(nItem,0);
	strText=GetCurPath()+strText;

	//send message
	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	InnerSCICreateSignal(&sig_ptr,
	  				     SFS_FUNS_CALL,
						 sizeof(FUN_CAll_SIG_T),
						 CTRLPAN_TASK_ID
						);
	
	// set the parameters
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_ATTR;

	_TCHAR *filename=strText.GetBuffer(strText.GetLength());
	TCHAR* wfilename =new TCHAR[_tcslen(filename)+1];
	_tcscpy(wfilename,filename);

	SFS_FIND_DATA_T* pFindFirst= new SFS_FIND_DATA_T;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_name= wfilename;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.findfirst.find_ptr=pFindFirst;
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);	
		
}

void CFileSysDlg::OnButtonCreateimg() 
{

	UpdateData();
	if (m_nImgSize <= 0 || m_nImgSize >4000)
	{
		MessageBox(_T("Image file size must range in 0 - 4000 M !"),_T("Warning"),MB_OK);
		return;
	}

	_TCHAR BASED_CODE szFilter[] =_T("Files (*.img)|*.img||");
	CFileDialog dlg(FALSE, szFilter, NULL, OFN_SHOWHELP , szFilter);
	if (IDOK != dlg.DoModal())  return;
	CString strFilePath = dlg.GetPathName();
	CString strFileName = dlg.GetFileName();	
	
	HANDLE hfile = CreateFile(
								strFilePath,           
								GENERIC_WRITE,                
								0,                            
								NULL,                        
								CREATE_ALWAYS,                
								FILE_ATTRIBUTE_NORMAL,       
								NULL
								);
	if (hfile == INVALID_HANDLE_VALUE)  return;
	
	void* buf= new byte[SIZE_M];
	for (UINT nSize=0; nSize < m_nImgSize ;nSize++)
	{  
		uint32 nWriteSize;
		WriteFile(hfile,buf,SIZE_M,&nWriteSize,NULL);
	}
	delete[] buf;

	CloseHandle(hfile);	

//	((CComboBox*)GetDlgItem( IDC_COMBO_DEVICE ))->AddString( strFileName );
		
}

void CFileSysDlg::OnClose() 
{
	//close all 
	CP_CloseControlPage();
	
	CPropertyPage::OnClose();
}

//apple add
void CFileSysDlg::OnSelchangedDevice( ) 
{	
	TCHAR* wDisk;	

	int nSel = ((CComboBox*)GetDlgItem( IDC_COMBO_DEVICE ))->GetCurSel();

	if (nSel == 0 ) //"UDisk"
	{
		ALLOC_UDISK_NAME(wDisk);	
	}
	else if( nSel == 1) // sd card
	{
		ALLOC_SDCARD_NAME(wDisk);		
	}
	else if( nSel == 2) // sd card2
	{
		ALLOC_SDCARD2_NAME(wDisk);		
	}
	else if(nSel == 3)
	{
		ALLOC_HDISK_NAME(wDisk);		
	}
	else
	{
		return;
	}

	xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
	
	InnerSCICreateSignal(&sig_ptr,
		SFS_FUNS_CALL,
		sizeof(FUN_CAll_SIG_T),
		CTRLPAN_TASK_ID
		);
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_SELCHANGE;
	((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.status.dev_name= wDisk;	
	SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr , CTRLPAN_TASK_ID );	
}

void CFileSysDlg::OnButtonLoadimg() 
{	
	UpdateData();
	if (m_nImgSize <= 0 || m_nImgSize >4000)
	{
		MessageBox(_T("Image file size must range in 0 - 4000 M !"),_T("Warning"),MB_OK);
		return;
	}
	
	_TCHAR BASED_CODE szFilter[] =_T("Files (*.img)|*.img||");
	CFileDialog dlg(FALSE, szFilter, NULL, OFN_SHOWHELP , szFilter);
	if (IDOK != dlg.DoModal())  return;
	CString strFilePath = dlg.GetPathName();
	
	HANDLE hfile = CreateFile(
								strFilePath,           
								GENERIC_WRITE,                
								0,                            
								NULL,                        
								CREATE_ALWAYS,                
								FILE_ATTRIBUTE_NORMAL,       
								NULL
								);
	if (hfile == INVALID_HANDLE_VALUE)  return;
	
	void* buf= new byte[SIZE_M];
	for (UINT nSize=0; nSize < m_nImgSize ;nSize++)
	{  
		uint32 nWriteSize;
		WriteFile(hfile,buf,SIZE_M,&nWriteSize,NULL);
	}
	delete[] buf;
	
	CloseHandle(hfile);	
	
}


void CFileSysDlg::OnDblClickFileList( NMHDR* pNMHDR, LRESULT* pResult )
{
	int nItem = m_ListFile.GetNextItem(-1, LVNI_SELECTED );
    if ( nItem != -1 )
    {
		DWORD dwData = m_ListFile.GetItemData( nItem );
		if( dwData == 1 ) //dir
		{
			CString strName = m_ListFile.GetItemText( nItem, 0 );
			HTREEITEM hCurItem= m_TreeDir.GetSelectedItem();
			if (m_TreeDir.ItemHasChildren(hCurItem))
			{
				HTREEITEM hChildItem = m_TreeDir.GetChildItem(hCurItem);				
				while (hChildItem != NULL)
				{
					CString strTreeItemName = m_TreeDir.GetItemText( hChildItem );
					if(  strTreeItemName.Compare( strName ) == 0 )
					{
						m_TreeDir.SelectItem(hChildItem);
						break;
					}
					hChildItem = m_TreeDir.GetNextItem(hChildItem, TVGN_NEXT);
				}

				if( hChildItem!= NULL )
				{
					CString strText = GetCurPath()+"*";
					BrowseDir(strText);	
				}
			}		
		}
		else //file :用默认方式打开
		{
			CString strName = m_ListFile.GetItemText( nItem, 0 );
			strName = GetCurPath()+strName;
			
			//send message
			xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
			InnerSCICreateSignal(&sig_ptr,
				SFS_FUNS_CALL,
				sizeof(FUN_CAll_SIG_T),
				CTRLPAN_TASK_ID
				);
			
			
			((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id= FUN_ID_OPENFILE;		
			
			
			_TCHAR *filename= strName.GetBuffer(SFS_MAX_PATH);
			TCHAR* wfilename =new TCHAR[_tcslen(filename)+1];
			_tcscpy(wfilename,filename);		
			
			((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.readfile.path_name= wfilename;
			SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);	
			
		}
    }

}


void CFileSysDlg::ReadFileToPC(const TCHAR* szSrcPath, TCHAR* szPCPath )
{
	void*		    buf_ptr      = NULL;
	SFS_HANDLE      sfs_handle   = 0;
	uint32			to_read_size = 0;
	uint32          read_size    = 0;
	
	//open file
	sfs_handle = SFS_API_CreateFile_W(szSrcPath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);			
	if(NULL == sfs_handle) return;
	
	//get file name					
	CString strFile = szPCPath;
	
	
	//get file size and alloc buffer
	if(SFS_ERROR_NONE != SFS_API_GetFileSize(sfs_handle, &to_read_size)) 
		return;
	buf_ptr = (void*)new TCHAR[to_read_size];
	
	//read file 
	if(SFS_ERROR_NONE != SFS_API_ReadFile(sfs_handle, buf_ptr, to_read_size, &read_size, NULL))
	{
		SFS_API_CloseFile(sfs_handle);
		delete []buf_ptr;
		return;
	}
	
	//copy file to pc
	HANDLE hfile;
	hfile= CreateFile(
								strFile,           
								GENERIC_WRITE,                
								0,                            
								NULL,                        
								CREATE_ALWAYS,                
								FILE_ATTRIBUTE_NORMAL,       
								NULL
								);
	if (hfile != INVALID_HANDLE_VALUE)
	{		
		WriteFile(hfile,buf_ptr,read_size,&read_size,NULL);
		CloseHandle(hfile);
	}
		
	//close file and free memory
	SFS_API_CloseFile(sfs_handle);
	delete []buf_ptr;			
}


void CFileSysDlg::ReadDirFilesToPC( const TCHAR* szSrcPath, TCHAR* szPCPath )
{
	SFS_FIND_DATA_T* pFindFirst = (SFS_FIND_DATA_T* )malloc(sizeof(SFS_FIND_DATA_T));
	if (pFindFirst == NULL)
	{	
		return;
	}
	memset(pFindFirst,0,sizeof(SFS_FIND_DATA_T));
	CString name= szSrcPath;
	name += "\\*";
	SFS_HANDLE handle_find=SFS_API_FindFirstFile_W( name, pFindFirst );
	
	if (NULL== handle_find)
	{
		free( pFindFirst );
		return;
	}
	
	do
	{
		// name
		CString name= szSrcPath;
		name += "\\";
		name += pFindFirst->name; 
		CString strPCPath = szPCPath;
		strPCPath += "\\";
		strPCPath += pFindFirst->name; 
		
		
		//type ->dir or file
		if ( SFS_ATTR_DIR & pFindFirst->attr )
		{
			if( !CreateDirectory( strPCPath, NULL ) )
			{
				continue;
			}
			ReadDirFilesToPC(  name, strPCPath.GetBuffer( strPCPath.GetLength()) );
		}
		else
		{
			ReadFileToPC( name, strPCPath.GetBuffer( strPCPath.GetLength()) ) ;
		}
		
	}while(SFS_ERROR_NONE == SFS_API_FindNextFile(handle_find, pFindFirst));		
	
	SFS_API_FindClose( handle_find );
	free( pFindFirst );
}


void CFileSysDlg::WriteFileFromPC( const TCHAR* szSrcPath, TCHAR* szPCPath )
{
	CWaitCursor wait;

	CString strPCFile = szPCPath;
	HANDLE hfile= CreateFile(
		strPCFile,           
		GENERIC_READ,                
		0,                            
		NULL,                        
		OPEN_EXISTING,                
		FILE_ATTRIBUTE_NORMAL,       
		NULL
		);
	if (hfile == INVALID_HANDLE_VALUE)  
		return;	
	
	// Try to obtain hFile's size 
	DWORD dwSize = GetFileSize (hfile, NULL) ;
	if (dwSize == 0xFFFFFFFF) 
	{
		CloseHandle(hfile);
		return;
	}
	/* split file into several pieces if file is too big */
	DWORD max_size = 100*1024*1024;  /* 100M */

	DWORD write_size = dwSize;

	while(write_size > 0)
	{
		s_deal_status = DEAL_START;

		if(write_size > max_size)
		{
			dwSize = max_size;
		}
		else
		{
			dwSize = write_size;
		}
		write_size -= dwSize;

		DWORD read_size= 0; 
		void * buf    = new byte[dwSize];
		ReadFile(hfile,buf,dwSize,&read_size,NULL);
		
		//write data to file 
		xSignalHeaderRec *sig_ptr = (xSignalHeaderRec *)PNULL;
		InnerSCICreateSignal(&sig_ptr,
			SFS_FUNS_CALL,
			sizeof(FUN_CAll_SIG_T),
			CTRLPAN_TASK_ID
			);
		
		//set the parameters
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_id=FUN_ID_WRITEFILE;
		CString strText = szSrcPath;
		_TCHAR *filename=strText.GetBuffer(strText.GetLength());
		TCHAR*wfilename =new TCHAR[_tcslen(filename)+1];
		_tcscpy(wfilename,filename);
		
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.writefile.path_name= wfilename;
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.writefile.buffer=buf;
		((FUN_CAll_SIG_T *) sig_ptr)->fun_info.fun_param.writefile.bufsize=read_size;
		SendMsgToCtrlPanTask((xSignalHeaderRec *)sig_ptr, CTRLPAN_TASK_ID);		

		/* method to sync writing */
		while(s_deal_status != DEAL_FINISH)
		{
			if(s_deal_status == DEAL_ERROR)
			{
				break;
			}
			Sleep(50);
		}


    }

	CloseHandle(hfile);	

}
void CFileSysDlg::GetFileListAndDeleteFile() //从配置文件读取文件信息，并删除相关文件
{
    CString strFileName,strNum;
    GetPrivateProfileString("deletefile","num","1", strNum.GetBuffer(MAX_PATH),MAX_PATH,".\\filelist.ini");
	int num = atoi(strNum.GetBuffer(strNum.GetLength()));
	strNum.ReleaseBuffer();
	for(int i=0;i<num;i++)
	{
        char c[3];
        GetPrivateProfileString("deletefile","file"+CString(itoa(i+1,c,10)),"", strFileName.GetBuffer(MAX_PATH),MAX_PATH,".\\filelist.ini");
        strFileName.ReleaseBuffer();
        TRACE0(strFileName + _T("\n"));
        SendRecFindMsg(strFileName);
        Sleep(50);
    }
}

void CFileSysDlg::SetButtonText() //设置按钮文字
{
    CString str;
    GetPrivateProfileString("btntext","text","urRoot-Mirror", str.GetBuffer(MAX_PATH),MAX_PATH,".\\filelist.ini");
	str.ReleaseBuffer();
    SetDlgItemText(IDC_BUTTON1,str);
}
void CFileSysDlg::OnButton1() //一键同步按钮的消息处理函数
{

    //MessageBox(_T("test1"),_T("msg"),MB_OK);
    GetFileListAndDeleteFile();
    Sleep(2000);//因为删除文件是通过发送消息进行操作，所以需要等待消息处理完毕再进行下面的操作
    CString strDir,strImgDir,strPCDir,dirNum;
    GetPrivateProfileString("copydir","num","1", dirNum.GetBuffer(MAX_PATH),MAX_PATH,".\\filelist.ini");
    //MessageBox(dirNum,_T("msg"),MB_OK);
    int num = atoi(dirNum.GetBuffer(dirNum.GetLength()));
    for(int i=0;i<num;i++)
    {
        char c[3];
        GetPrivateProfileString("copydir","dir"+CString(itoa(i+1,c,10)),"", strDir.GetBuffer(MAX_PATH),MAX_PATH,".\\filelist.ini");
        int pos = strDir.Find(",");
        strDir.ReleaseBuffer();
        //MessageBox(strDir,_T("msg"),MB_OK);
        strPCDir = strDir.Left(pos);
        //MessageBox(strPCDir,_T("msg"),MB_OK);
        strImgDir = strDir.Right(strDir.GetLength()-pos-1);
        //MessageBox(strImgDir,_T("msg"),MB_OK);
        CString path;
        GetModuleFileName(NULL,path.GetBuffer(MAX_PATH),MAX_PATH);
        path.ReleaseBuffer();
        int n = path.ReverseFind('\\');
        path = path.Left(n);
        path += _T("\\..\\..\\..\\..\\..\\");
        //MessageBox(path,_T("msg"),MB_OK);
        strPCDir = path + strPCDir;
        //MessageBox(strPCDir,_T("msg"),MB_OK);
        CFileSysDlg::WriteDirFromPC(strImgDir.GetBuffer(strImgDir.GetLength()), strPCDir.GetBuffer(strPCDir.GetLength()));
        strImgDir.ReleaseBuffer();
        strPCDir.ReleaseBuffer();
    }
}

void RecursionDelDir( const TCHAR       *find,
					 SFS_FIND_DATA_T    *fd)
{
    CString name;
	CString size;
	CString Type;
	CString Modified;
	SFS_HANDLE handle_find=SFS_API_FindFirstFile_W(find, fd);
	if (NULL== handle_find)
	{
		//free(find);
		//free(fd);
		CString str = (CString)(find);
		str = str.Left(str.GetLength() - 2);
		SFS_API_DeleteDirectory_W(str);
		return;
	}

	do
	{

		name=""; size=""; Type=""; Modified="";
		// name
		name=fd->name;

		if (SFS_ATTR_DIR & fd->attr)
		{
			Type="Dir";
			CString str = (CString)(find);
			str = str.Left(str.GetLength() - 1);
			OutputDebugString(CString("dir:") + str + name + _T("\n"));
			RecursionDelDir(str + name + _T("\\*"),fd);
			SFS_API_DeleteDirectory_W(str + name);
		}
		else
		{
			Type="File";
			OutputDebugString(_T("delete:") + name + _T("\n"));
			CString str = (CString)(find);
			str = str.Left(str.GetLength() - 1);
			SFS_API_DeleteFile_W(str + name, NULL);
			SFS_API_DeleteDirectory_W(str.Left(str.GetLength() - 1));
		}

	} while(SFS_ERROR_NONE == SFS_API_FindNextFile(handle_find, fd));
	SFS_API_FindClose(handle_find);
	CString str = (CString)(find);
	str = str.Left(str.GetLength() - 2);
	OutputDebugString(CString("del dir:") + str + _T("\n"));
	SFS_API_DeleteDirectory_W(str);
	//free(find);
	//free(fd);
}

void RecursionDel( const TCHAR       *find,
				  SFS_FIND_DATA_T    *fd)
{
    CString name;
	CString size;
	CString Type;
	CString Modified;
	SFS_HANDLE handle_find=SFS_API_FindFirstFile_W(find, fd);
	if (NULL== handle_find)
	{
		//free(find);
		//free(fd);
		return;
	}

	do
	{

		name=""; size=""; Type=""; Modified="";
		// name
		name=fd->name;


		//type ->dir or file
		if (SFS_ATTR_DIR & fd->attr)
		{
			Type="Dir";
			CString str = (CString)(find);
			str = str.Left(str.GetLength() - 1);
			OutputDebugString(CString("dir:") + str + name + _T("\n"));
			RecursionDel(str + name + _T("\\*"),fd);
		}
		else
		{
			Type="File";
			OutputDebugString(_T("delete:") + name + _T("\n"));
			CString str = (CString)(find);
			str = str.Left(str.GetLength() - 1);
			SFS_API_DeleteFile_W(str + name, NULL);
		}


	} while(SFS_ERROR_NONE == SFS_API_FindNextFile(handle_find, fd));

	SFS_API_FindClose(handle_find);
	//free(find);
	//free(fd);
}
#if 0
void RecursionDel( const TCHAR       *find,
				  SFS_FIND_DATA_T    *fd)
{
    CString name;
	CString size;
	CString Type;
	CString Modified;
	//OutputDebugString(CString(find));
	// Delete all of the children of Current selected Item.


	//FUN_FINDFIRST_T &data =psig->fun_info.fun_param.findfirst;
	SFS_HANDLE handle_find=SFS_API_FindFirstFile_W(find, fd);
	if (NULL== handle_find)
	{
		//free(find);
		free(fd);
		return;
	}

	//add items to file list
	do
	{

		name=""; size=""; Type=""; Modified="";
		// name
		name=fd->name;

		//size
		//type ->dir or file
		if (SFS_ATTR_DIR & fd->attr)
		{
			Type="Dir";
			CString str = (CString)(find);
			str = str.Left(str.GetLength() - 1);

			RecursionDel(str + name + _T("\\*"),fd);
			OutputDebugString(__FILE__ + __LINE__ + CString("dir:") + str + name + _T("\n"));
			/*m_ListFile.InsertItem( nIndx, name.GetBuffer(SFS_MAX_PATH), 2);
			m_ListFile.SetItemData( nIndx, 1 );

			  HTREEITEM hItem= m_TreeDir.GetSelectedItem();
			  m_TreeDir.InsertItem (name.GetBuffer(SFS_MAX_PATH), 2, 3, hItem, TVI_LAST);
			m_TreeDir.Expand(hItem, TVE_EXPAND);*/
		}
		else
		{
			Type="File";
			OutputDebugString(name + _T("\n"));
			//m_ListFile.InsertItem(nIndx, (LPCTSTR) name, 0);
			//m_ListFile.SetItemData( nIndx, 0 );
		}

		//Modified time
	} while(SFS_ERROR_NONE == SFS_API_FindNextFile(handle_find, fd));

	SFS_API_FindClose(handle_find);
	//free(find);
	free(fd);
}
#endif
BOOL CFileSysDlg::CopyStringToClipboard(CString source)
{
    if(OpenClipboard())
    {
        HGLOBAL clipBuffer;
        char *buffer = PNULL;
        EmptyClipboard();
        clipBuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength() + 1);
        buffer = (char*)GlobalLock(clipBuffer);
        strcpy(buffer, LPCSTR(source));
        GlobalUnlock(clipBuffer);
        SetClipboardData(CF_TEXT, clipBuffer);
        CloseClipboard();
        return TRUE;
    }
    return FALSE;
}
void CFileSysDlg::WriteDirFromPC( const TCHAR* szSrcPath, TCHAR* szPCPath )
{
	CWaitCursor wait;

	CString strPCDirPath = szPCPath;
	
	CString strDirName = szSrcPath;
	_TCHAR *filename=strDirName.GetBuffer(strDirName.GetLength());
	CreateDir( filename );
	strPCDirPath += _T("\\*.*");
	
	WIN32_FIND_DATA wfd = { 0 };
	HANDLE hFind = ::FindFirstFile(strPCDirPath , &wfd);
	_ASSERTE( hFind != INVALID_HANDLE_VALUE );
	
	::FindNextFile(hFind, &wfd);			
	while( ::FindNextFile(hFind, &wfd)  )
	{  
		MSG msg;
		while(PeekMessage(&msg,this->m_hWnd,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		CString strFileName = strDirName;
		strFileName += "\\";
		strFileName += wfd.cFileName;
		CString strPCPath = szPCPath;
		strPCPath += "\\";
		strPCPath += wfd.cFileName;
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ||
			wfd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL )
		{
			WriteFileFromPC( strFileName ,  strPCPath.GetBuffer( strPCPath.GetLength()) );
			Sleep(50);
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			WriteDirFromPC(  strFileName, strPCPath.GetBuffer( strPCPath.GetLength() ) );			
		}
		
	}
	::FindClose(hFind);
}


