#include "stdafx.h"

#include "DLFile.h"


BOOL LoadBMFileInfo( PBMFileInfo lpBMFileInfo ) 
{
    if( lpBMFileInfo == NULL )
        return FALSE;
    
    if( FALSE == lpBMFileInfo->bLoadCodeFromFile )
    {
        return TRUE;
    }
    
    CString strFDLFileName = (LPCTSTR)lpBMFileInfo->szFileName;
    if( strFDLFileName.IsEmpty() )
    {
        return FALSE;
    }
    CString strMapName = strFDLFileName ;
    strMapName.Replace('\\','.');
    strMapName = "Local\\" + strMapName;

    HANDLE hFDLCode = INVALID_HANDLE_VALUE;
    HANDLE hFDLCodeMapView = NULL;
    
    hFDLCode = CreateFile(strFDLFileName.operator LPCTSTR (),
            GENERIC_READ,
            FILE_SHARE_READ,                         //Exclusive Open
            NULL,                      //Can't Be inherited
            OPEN_EXISTING,             //If not existing then failed
            FILE_ATTRIBUTE_READONLY,   //Read Only
            NULL);
    if( hFDLCode == INVALID_HANDLE_VALUE)
    {   
        return FALSE;
    }
        

    unsigned __int64 llCodeSize	= 0;
    LARGE_INTEGER liFileSize;
    liFileSize.HighPart = liFileSize.LowPart = 0;

    if( !GetFileSizeEx(hFDLCode,&liFileSize))
    {
        CloseHandle( hFDLCode);
        return FALSE;
    }
        
    llCodeSize = liFileSize.HighPart ;
    llCodeSize = llCodeSize<<32;
    llCodeSize |= liFileSize.LowPart;
    lpBMFileInfo->llCodeSize = llCodeSize;

    hFDLCodeMapView = CreateFileMapping( hFDLCode,   //The Handle of Opened File
        NULL,          //Security
        PAGE_READONLY, //Read Only Access
        0,             //Max Size
        0,             //Min Size
        strMapName);   //Object Name
    
    if( hFDLCodeMapView == NULL)
    {
        CloseHandle( hFDLCode);
        return FALSE;
    }
    
    void* lpCode = ::MapViewOfFile( hFDLCodeMapView, FILE_MAP_READ,  0, 0, 0);    
    if( lpCode == NULL)
    {
        CloseHandle( hFDLCode );
        CloseHandle( hFDLCodeMapView );
        return FALSE;
    }      
    
    if( lpBMFileInfo->bChangeCode )
    {
        lpBMFileInfo->lpCode = new BYTE[ (unsigned int)llCodeSize ];
        memcpy( lpBMFileInfo->lpCode, lpCode, (unsigned int)llCodeSize);
        ::UnmapViewOfFile( lpCode );
        CloseHandle( hFDLCode);
        CloseHandle( hFDLCodeMapView );
    }
    else
    {
        lpBMFileInfo->lpCode = lpCode;
        lpBMFileInfo->hFDLCode = hFDLCode;
        lpBMFileInfo->hFDLCodeMapView = hFDLCodeMapView;
    }
    
    return TRUE;    
}

void UnloadBMFileInfo( PBMFileInfo lpBMFileInfo  )
{
    if( lpBMFileInfo == NULL )
        return;

    if( lpBMFileInfo->bLoadCodeFromFile )
    {
        if( lpBMFileInfo->bChangeCode )
        {
            if( lpBMFileInfo->lpCode != NULL )
            {
                delete lpBMFileInfo->lpCode;
            }
        }
        else
        {
            if( lpBMFileInfo->lpCode != NULL )
            {
                ::UnmapViewOfFile( lpBMFileInfo->lpCode  );
            }
            if( lpBMFileInfo->hFDLCode != NULL )
            {
                ::CloseHandle( lpBMFileInfo->hFDLCode );
            }
            if( lpBMFileInfo->hFDLCodeMapView != NULL )
            {
                ::CloseHandle( lpBMFileInfo->hFDLCodeMapView );
            }
        }
    }
}