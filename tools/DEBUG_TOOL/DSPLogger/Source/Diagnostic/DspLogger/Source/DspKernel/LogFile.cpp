// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "LogFile.h"

#include <stdio.h>

char CLogFile::m_strHeader[HEADER_SIZE] = {'K','L','S','C','I','D','S','P',
                                  'L','O','G','F','I','L','E',0};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{
    m_hLogFile      = INVALID_HANDLE_VALUE;

}//lint !e1401

CLogFile::~CLogFile()
{

}//lint !e1540


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
BOOL CLogFile::CreateLogFile(char* pExName/*= NULL*/)
{
    GenTempFileName(pExName);
    m_hLogFile = ::CreateFile(m_strTempFile,
                           GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ,
                           NULL,
                           CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);

    if(INVALID_HANDLE_VALUE == m_hLogFile)
        return FALSE;

    return TRUE;

    //write the file header
//    return WriteHeader();
}

// -----------------------------------------------------------------------------
// Name:
//      CloseLogFile
//
// Function:
//      flush all buffers to the disk ,then close the log file
// 
// -----------------------------------------------------------------------------
BOOL CLogFile::CloseLogFile()
{
    return ::CloseHandle(m_hLogFile);
}

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
BOOL CLogFile::SaveLogFile(
                           LPCTSTR lpFileName       //where to save logfile
                           )
{
    ::FlushFileBuffers(m_hLogFile);
    return ::CopyFile(m_strTempFile,lpFileName,FALSE);
}

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
BOOL CLogFile::LoadLogFile(
                           LPCTSTR lpFileName
                          )
{
    m_hLogFile = ::CreateFile(
                              lpFileName,
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    if(INVALID_HANDLE_VALUE == m_hLogFile)
        return FALSE;
/*
    //a valid log file?
    if(!ReadHeader())
    {
        ::CloseHandle(m_hLogFile);
        return FALSE;
    }
*/
    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      Log
//
// Function:
//      log the incoming information
// 
// -----------------------------------------------------------------------------
BOOL CLogFile::Log(
        void* pData,        //information to be logged
        DWORD dwSize        //information size
        )
{
    if(pData == NULL || dwSize == 0)
        return FALSE;

    DWORD dwWrite;
    ::WriteFile(m_hLogFile,pData,dwSize,&dwWrite,NULL);

    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      GenTempFileName
//
// Function:
//      Generat tempoary file name
// 
// -----------------------------------------------------------------------------
void CLogFile::GenTempFileName(char* pExName)
{
    //Get tempoary directory
    char* pTempDir = getenv("Temp");
    strcpy(m_strTempFile,pTempDir);
    strcat(m_strTempFile,"\\");

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    char name[_MAX_FNAME];
    sprintf(name,
            "%d-%d-%d-%d-%d-%d-%d.",
            sysTime.wYear,
            sysTime.wMonth,
            sysTime.wDay,
            sysTime.wHour,
            sysTime.wMinute,
            sysTime.wSecond,
            sysTime.wMilliseconds);

    strcat(m_strTempFile,name);

    if(pExName == NULL)
    {
        strcat(m_strTempFile,"log");
    }
    else
    {
        strcat(m_strTempFile,pExName);
    }
}
