/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      LogFile.h
 *
 * Description:
 *      The class CLogFile log all the information from dspkernel to the disk
 *      
 *
 * 
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-12-8   Kai Liu     Created
 *============================================================================*/

#if !defined(AFX_LOGFILE_H__EC0EFE14_9365_4A53_AB9A_E93DBF3734C1__INCLUDED_)
#define AFX_LOGFILE_H__EC0EFE14_9365_4A53_AB9A_E93DBF3734C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include <stdlib.h>

#define HEADER_SIZE 16

class CLogFile  
{
public:
	CLogFile();
	virtual ~CLogFile();

public:
// -----------------------------------------------------------------------------
// Name:
//      CreateLogFile
//
// Function:
//      Create the log file to log information
// 
// Discription:
//      This function only create a tempary file to log information,
//      if want to save the log information,it will move the tempary
//      file to the position you sepecified in SaveLogFile function
// -----------------------------------------------------------------------------
    BOOL CreateLogFile(char* pExName = NULL);

// -----------------------------------------------------------------------------
// Name:
//      CloseLogFile
//
// Function:
//      flush all buffers to the disk ,then close the log file
// 
// -----------------------------------------------------------------------------
    BOOL CloseLogFile();

// -----------------------------------------------------------------------------
// Name:
//      SaveLogFile
//
// Function:
//      Save the log information to the disk
// 
// Discription:
//      Save the tempary file to the position specified by lpFileName
// -----------------------------------------------------------------------------
    BOOL SaveLogFile(LPCTSTR lpFileName);

// -----------------------------------------------------------------------------
// Name:
//      LoadLogFile
//
// Function:
//      Load the log information from the disk
// 
// Discription:
//      Open the log file
// -----------------------------------------------------------------------------
    BOOL LoadLogFile(LPCTSTR lpFileName);

// -----------------------------------------------------------------------------
// Name:
//      Log
//
// Function:
//      log the incoming information
// 
// -----------------------------------------------------------------------------
    BOOL Log(
        void* pData,        //information to be logged
        DWORD dwSize        //information size
        );

// -----------------------------------------------------------------------------
// Name:
//      Read
//
// Function:
//      read some log data
// 
// -----------------------------------------------------------------------------
    BOOL Read(
            LPVOID pData,   //buffer to contain the data
            DWORD dwSize    //number of bytes to read
            )
    {
        DWORD dwRead;
        if(!::ReadFile(m_hLogFile,pData,dwSize,&dwRead,NULL))
            return FALSE;

        if(dwRead != dwSize)
            return FALSE;

        return TRUE;
    }

    char* GetLogFilePathName()
    {
        return m_strTempFile;
    }

protected:
// -----------------------------------------------------------------------------
// Name:
//      WriteHeader
//
// Function:
//      Write the file header
// 
// Note:
//      Use the file header to decide whether the file is 
//      a valid log file
// -----------------------------------------------------------------------------
    BOOL WriteHeader()
    {
        DWORD dwReal;
        return ::WriteFile(m_hLogFile,(const void*)m_strHeader,HEADER_SIZE,&dwReal,NULL);
    }

// -----------------------------------------------------------------------------
// Name:
//      ReadHeader
//
// Function:
//      Read the file header
// 
// Note:
//      Read the header to see if the file is a valid log file
// -----------------------------------------------------------------------------
    BOOL ReadHeader()
    {
        char buffer[HEADER_SIZE];
        DWORD dwRead;
        if(!::ReadFile(m_hLogFile,buffer,HEADER_SIZE,&dwRead,NULL))
            return FALSE;
        if(dwRead != HEADER_SIZE)
            return FALSE;

        return !strcmp(buffer,m_strHeader);

    }

// -----------------------------------------------------------------------------
// Name:
//      GenTempFileName
//
// Function:
//      Generat tempoary file name
// 
// -----------------------------------------------------------------------------
    void GenTempFileName(char* pExName);

protected:
    //log file handle
    HANDLE m_hLogFile;

    char m_strTempFile[_MAX_PATH];

    static char m_strHeader[HEADER_SIZE];
};

#endif // !defined(AFX_LOGFILE_H__EC0EFE14_9365_4A53_AB9A_E93DBF3734C1__INCLUDED_)
