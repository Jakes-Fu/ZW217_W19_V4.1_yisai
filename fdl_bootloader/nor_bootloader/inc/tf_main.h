/******************************************************************************
 ** File Name:      TF_main.h                                                *
 ** Author:           tao.feng                                                 *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces of TF download process  *
            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 20/12/2010     tao.feng         Create.                                   *
 **                                                                           *
 ******************************************************************************/


#ifndef _TF_MAIN__H_
#define _TF_MAIN__H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "ff.h"
#include "integer.h"
#include "tf_load.h"

/**---------------------------------------------------------------------------*
 **                         Macro Defination                                  *
 **---------------------------------------------------------------------------*/
#define MAX_BUFF_SIZE              		(1024 * 256)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/*.pac include file  structure*/
typedef struct _FILE_T
{
    DWORD  dwSize;              // size of this struct itself
    WCHAR  szFileID[256];        // file ID,such as FDL,Fdl2,NV and etc.
    WCHAR  szFileName[256];     // file name,in the packet bin file,it only stores file name
                                // but after unpacketing, it stores the full path of bin file
    WCHAR  szFileVersion[256];  // Reserved now
    int    nFileSize;           // file size
    int    nFileFlag;           // if "0", means that it need not a file, and
                                			// it is only an operation or a list of operations, such as file ID is "FLASH"
                                			// if "1", means that it need a file
    DWORD  nCheckFlag;          // if "1", this file must be downloaded;
                                			// if "0", this file can not be downloaded;
    DWORD  dwDataOffset;        // the offset from the packet file header to this file data
    DWORD  dwCanOmitFlag;       // if "1", this file can not be downloaded and not check it as "All files"
                                //   in download and spupgrade tool.
    DWORD  dwReserved[255];     // Reserved for future,not used now
}FILE_T, *PFILE_T;

/*.pac file  structure*/
typedef struct _BIN_PACKET_HEADER_T
{
    WCHAR  szVersion[24];        // packet struct version
    DWORD  dwSize;              // the whole packet size;
    WCHAR  szPrdName[256];       // product name
    WCHAR  szPrdVersion[256];   // product version
    int    nFileCount;          // the number of files that will be downloaded, the file may be an operation
    DWORD  dwFileOffset;        // the offset from the packet file header to the array of FILE_T struct buffer
    DWORD  dwMode;
    DWORD  dwFlashType;
    DWORD  dwNandStrategy;
    DWORD  dwIsNvBackup;
    DWORD  dwNandPageType;
    WCHAR  szPrdAlias[100];    // product alias
    DWORD  dwOmaDmProductFlag;
    DWORD  dwIsOmaDM;
    DWORD  dwIsPreload;
    DWORD  dwReserved[202];
}BIN_PACKET_HEADER_T,*PBIN_PACKET_HEADER_T;


/*****************************************************************************/
//  Discription: TF download process err string generate
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
//PUBLIC char* TF_Errstring_Gen(TFLOAD_ERR_TYPE errno);


/*****************************************************************************/
//  Discription: TF get update info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  TF_UPDATE_INFO_PTR TF_GetUpdateInfo(void);


/*****************************************************************************/
// Description : Send message to uart1 for boot-up log
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
PUBLIC void TF_UartTrace(const char *x_format, ...);


/*****************************************************************************/
// Description :TF_SDPwrCtrl
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
PUBLIC void TF_SDPwrCtrl(BOOLEAN open);

#endif //_TF_MAIN__H_
