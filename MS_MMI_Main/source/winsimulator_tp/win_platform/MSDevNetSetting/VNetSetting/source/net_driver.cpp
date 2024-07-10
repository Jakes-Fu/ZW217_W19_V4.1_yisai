/*****************************************************************************
 ** File Name:     net_driver.cpp                                            *
 ** Author:        yifeng.wang                                               *
 ** Date:          2008/02/27                                                *
 ** Copyright:     2008 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:   interfaces for winpcap driver interfaces.                 *
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                        *
 ** 2008/02/27     yifeng.wang            Create                             *
 ** 2010/08/27     yifeng.wang            Amend for simulater check          *
 *****************************************************************************/

#include <stdafx.h>
#include "common.h"
#include "file_path.h"
#include "net_driver.h"

/*---------------------------------------------------------------------------*
 *                         MACRO DEFINITION                                  *
 *---------------------------------------------------------------------------*/
#define FILE_NAME           "net_driver.cpp"

// driver information
#define DRIVER_SYS64_PATH   "SysWOW64"
#define DRIVER_SYS_PATH     "drivers"
#define DRIVER_SYS_NAME     "npf.sys"
#define DRIVER_DLL_NAME     "packet.dll"
#define DRIVER_FILE_NAME    "WinPcap_4.0.2.exe"
#define DRIVER_FILEPATH_DEF "c:\\WinPcap_4.0.2.exe"


/*---------------------------------------------------------------------------*
 *                         GLOBAL DEFINITION                                 *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                         LOCAL FUNCTION DECLARE                            *
 *---------------------------------------------------------------------------*/
LOCAL BOOL ExecuteFile( CString strPathName );


/*---------------------------------------------------------------------------*
 *                         FUNCTION DEFINITION                               *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 检查 Winpcap 驱动是否已经安装
// Global : 
// Author : yifeng.wang
// Note : RETURN: TRUE - driver is installed; FALSE - not installed
/*****************************************************************************/
PUBLIC BOOL NETDRIVER_IsInstalled( void )
{
    CString strSysPath, strTempPath;
    BOOL bNpfSys(FALSE), bPacketDll(FALSE);
    char  chSysPath[MAX_PATH];

    // get system path
    UINT  nLen = GetSystemDirectory( NULL, 0 );
    if ( 0 == nLen )
    {
        TRACE("%s(%d) : NETDRIVER_IsInstalled() - Get system directory length failed!\n", FILE_NAME, __LINE__);
        return FALSE;
    }

    ASSERT( MAX_PATH > nLen );
    if ( 0 == GetSystemDirectory( chSysPath, MAX_PATH ) )
    {
        TRACE("%s(%d) : NETDRIVER_IsInstalled() - Get System Directory failed!\n", FILE_NAME, __LINE__);
        return FALSE;
    }

    // get windows path
//    if( 0 == GetWindowsDirectory( chWinPath, MAX_PATH ) )
//    {
//        TRACE("%s(%d) : NETDRIVER_IsInstalled() - Get Windows Directory failed!\n", FILE_NAME, __LINE__);
//        return FALSE;
//    }
//    strWinPath.Format(_T("%s"), chWinPath);
//    FILEPATH_FormatPath( strWinPath );

    strSysPath.Format(_T("%s"), chSysPath);
    FILEPATH_FormatPath( strSysPath );

    // check npf.sys
    CString strDrvPath;
    strDrvPath = strSysPath + DRIVER_SYS_PATH + "\\" + DRIVER_SYS_NAME;
    TRACE("%s(%d) : [ DRIVER FILE CHECK ] - %s\n", FILE_NAME, __LINE__, strDrvPath);
    bNpfSys = FILEPATH_IsExisted( strDrvPath );

    // check npf.sys in 64-bit system directory
    strTempPath = strSysPath;
    while( !bNpfSys )
    {
        if( !FILEPATH_ParentPath( strTempPath ) )
        {
            break;
        }

        strDrvPath = strTempPath + DRIVER_SYS64_PATH + "\\" + DRIVER_SYS_NAME;
        TRACE("%s(%d) : [ DRIVER FILE CHECK ] - %s\n", FILE_NAME, __LINE__, strDrvPath);
        bNpfSys = FILEPATH_IsExisted( strDrvPath );
    }

    if( !bNpfSys )
    {
        TRACE("%s(%d) : [ DRIVER FILE CHECK ] - %s is not found!\n", FILE_NAME, __LINE__, DRIVER_SYS_NAME);
    }

    // check packet.dll
    strDrvPath = strSysPath + DRIVER_DLL_NAME;
    TRACE("%s(%d) : [ DRIVER FILE CHECK ] - %s\n", FILE_NAME, __LINE__, strDrvPath);
    bPacketDll = FILEPATH_IsExisted( strDrvPath );

    // check packet.dll in 64-bit system directory
    strTempPath = strSysPath;
    while( !bPacketDll )
    {
        if( !FILEPATH_ParentPath( strTempPath ) )
        {
            break;
        }

        strDrvPath = strTempPath + DRIVER_SYS64_PATH + "\\" + DRIVER_DLL_NAME;
        TRACE("%s(%d) : [ DRIVER FILE CHECK ] - %s\n", FILE_NAME, __LINE__, strDrvPath);
        bPacketDll = FILEPATH_IsExisted( strDrvPath );
    }

    if( !bPacketDll )
    {
        TRACE("%s(%d) : [ DRIVER FILE CHECK ] - %s is not found!\n", FILE_NAME, __LINE__, DRIVER_SYS_NAME);
    }

    // NOTES: winpacket.dll is no longer existed in WinPcap_4.1.1 and later version
    return ( bNpfSys && bPacketDll ); 
}

/*****************************************************************************/
// Description : 安装 Winpcap 驱动
// Global : 
// Author : yifeng.wang
// Note : RETURN: TRUE - install driver successfully; FALSE - failed
/*****************************************************************************/
PUBLIC BOOL NETDRIVER_InstallDriver( void )
{
    CString strDefDrvPathName, strDriverFilePath("c:\\");
    CString strDlgCaption("Open driver install file");

    // 获得默认打开路径
    if( !FILEPATH_GetCurrentAppPath(strDriverFilePath) )
    {
        TRACE("%s(%d) : NETDRIVER_InstallDriver() - driver file initialize failed!\n", FILE_NAME, __LINE__);
    }

    // 尝试打开默认驱动安装文件
    FILEPATH_FormatPath(strDriverFilePath);
    strDefDrvPathName.Format("%s%s", strDriverFilePath.GetBuffer(strDriverFilePath.GetLength()), DRIVER_FILE_NAME);
    if( FILEPATH_IsExisted( strDefDrvPathName) )
    {
        // 若默认驱动文件存在，则直接运行该驱动文件
        return ExecuteFile( strDefDrvPathName );
    }
    else
    {
        // 若默认驱动安装文件不存在，允许用户制定驱动安装程序
        // 首先设置文件打开对话框
        TCHAR szDriverPathName[MAX_PATH_LENGTH];
        TCHAR szFilter[] = "可执行文件 (*.exe)|*.exe||";
        sprintf(szDriverPathName, "%s", DRIVER_FILE_NAME);
        CFileDialog fileDlg(TRUE, "exe", NULL, (OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT), szFilter, NULL);
        fileDlg.m_ofn.lpstrTitle = strDlgCaption;           // 设置对话框标题
        fileDlg.m_ofn.lpstrInitialDir = strDriverFilePath;  // 设置初始打开路径
        fileDlg.m_ofn.nMaxFile = sizeof(szDriverPathName);  // 设置路径名缓存长度
        fileDlg.m_ofn.lpstrFile = szDriverPathName;         // 设置初始文件名

        // 打开文件对话框并响应“打开”操作
        if( fileDlg.DoModal() == IDOK )
        {
            return ExecuteFile( fileDlg.GetPathName() );
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
// Description : 运行文件
// Global : none
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
LOCAL BOOL ExecuteFile( CString strPathName )
{
    TRACE("%s(%d) : ExecuteFile() - Driver path '%s'\n", FILE_NAME, __LINE__, 
        strPathName.GetBuffer(strPathName.GetLength()));

    // 检查文件存在
    if( FILEPATH_IsExisted(strPathName) )
    {
        int n, left, err = 0;
        HINSTANCE ret_val = NULL;

        n = strPathName.ReverseFind('\\');      // 返回左起最后一个“\”的位置索引
        left = strPathName.GetLength() - n - 1;
        CString strFilePath = strPathName.Left(n+1);    // 取出前n+1个字符，即路径名
        CString strFileName = strPathName.Right(left);  // 取出索引n后的字符，即文件名

        // 运行目标文件
        ret_val = ShellExecute(
            NULL,
            NULL,
            _T(strFileName.GetBuffer(strFileName.GetLength())),
            NULL,
            _T(strFilePath.GetBuffer(strFilePath.GetLength())),
            SW_SHOWNORMAL);

        // 获得错误码
        err = GetLastError();
        if( 0 != err )
        {
            CString strMsg;
            //value - description
            //  2 - SE_ERR_FNF(ERROR_FILE_NOT_FOUND)
            //  3 - SE_ERR_PNF(ERROR_PATH_NOT_FOUND)
            //  5 - SE_ERR_ACCESSDENIED
            //  8 - SE_ERR_OOM
            // 11 - ERROR_BAD_FORMAT
            // 26 - SE_ERR_SHARE 
            // 27 - SE_ERR_ASSOCINCOMPLETE
            // 28 - SE_ERR_DDETIMEOUT
            // 29 - SE_ERR_DDEFAIL
            // 30 - SE_ERR_DDEBUSY
            // 31 - SE_ERR_NOASSOC
            // 32 - SE_ERR_DLLNOTFOUND
            TRACE("%s(%d) : ExecuteFile() - ShellExecute return %d, error %d\n", FILE_NAME, __LINE__, ret_val, err);
            strMsg.Format("File execute failed! Error code %d", err);
            AfxMessageBox(strMsg, MB_OK|MB_ICONERROR);
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        TRACE("%s(%d) : ExecuteFile() - Driver file not existed\n", FILE_NAME, __LINE__);
        AfxMessageBox(_T("Given file is not existed!"), MB_OK|MB_ICONERROR);
        return FALSE;
    }
}