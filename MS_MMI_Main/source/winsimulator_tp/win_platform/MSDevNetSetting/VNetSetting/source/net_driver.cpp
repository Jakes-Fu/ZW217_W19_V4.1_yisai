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
// Description : ��� Winpcap �����Ƿ��Ѿ���װ
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
// Description : ��װ Winpcap ����
// Global : 
// Author : yifeng.wang
// Note : RETURN: TRUE - install driver successfully; FALSE - failed
/*****************************************************************************/
PUBLIC BOOL NETDRIVER_InstallDriver( void )
{
    CString strDefDrvPathName, strDriverFilePath("c:\\");
    CString strDlgCaption("Open driver install file");

    // ���Ĭ�ϴ�·��
    if( !FILEPATH_GetCurrentAppPath(strDriverFilePath) )
    {
        TRACE("%s(%d) : NETDRIVER_InstallDriver() - driver file initialize failed!\n", FILE_NAME, __LINE__);
    }

    // ���Դ�Ĭ��������װ�ļ�
    FILEPATH_FormatPath(strDriverFilePath);
    strDefDrvPathName.Format("%s%s", strDriverFilePath.GetBuffer(strDriverFilePath.GetLength()), DRIVER_FILE_NAME);
    if( FILEPATH_IsExisted( strDefDrvPathName) )
    {
        // ��Ĭ�������ļ����ڣ���ֱ�����и������ļ�
        return ExecuteFile( strDefDrvPathName );
    }
    else
    {
        // ��Ĭ��������װ�ļ������ڣ������û��ƶ�������װ����
        // ���������ļ��򿪶Ի���
        TCHAR szDriverPathName[MAX_PATH_LENGTH];
        TCHAR szFilter[] = "��ִ���ļ� (*.exe)|*.exe||";
        sprintf(szDriverPathName, "%s", DRIVER_FILE_NAME);
        CFileDialog fileDlg(TRUE, "exe", NULL, (OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT), szFilter, NULL);
        fileDlg.m_ofn.lpstrTitle = strDlgCaption;           // ���öԻ������
        fileDlg.m_ofn.lpstrInitialDir = strDriverFilePath;  // ���ó�ʼ��·��
        fileDlg.m_ofn.nMaxFile = sizeof(szDriverPathName);  // ����·�������泤��
        fileDlg.m_ofn.lpstrFile = szDriverPathName;         // ���ó�ʼ�ļ���

        // ���ļ��Ի�����Ӧ���򿪡�����
        if( fileDlg.DoModal() == IDOK )
        {
            return ExecuteFile( fileDlg.GetPathName() );
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
// Description : �����ļ�
// Global : none
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
LOCAL BOOL ExecuteFile( CString strPathName )
{
    TRACE("%s(%d) : ExecuteFile() - Driver path '%s'\n", FILE_NAME, __LINE__, 
        strPathName.GetBuffer(strPathName.GetLength()));

    // ����ļ�����
    if( FILEPATH_IsExisted(strPathName) )
    {
        int n, left, err = 0;
        HINSTANCE ret_val = NULL;

        n = strPathName.ReverseFind('\\');      // �����������һ����\����λ������
        left = strPathName.GetLength() - n - 1;
        CString strFilePath = strPathName.Left(n+1);    // ȡ��ǰn+1���ַ�����·����
        CString strFileName = strPathName.Right(left);  // ȡ������n����ַ������ļ���

        // ����Ŀ���ļ�
        ret_val = ShellExecute(
            NULL,
            NULL,
            _T(strFileName.GetBuffer(strFileName.GetLength())),
            NULL,
            _T(strFilePath.GetBuffer(strFilePath.GetLength())),
            SW_SHOWNORMAL);

        // ��ô�����
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