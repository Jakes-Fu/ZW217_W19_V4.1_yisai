/*****************************************************************************
** File Name:       file_path.cpp
** Author:          Yifeng.Wang
** Date:            2008/02/22
** Copyright:       2008 Spreadtrum, Incorporated. All Rights Reserved.
** Description:     This file defines file and path operation interfaces.
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2008/02/22       Yifeng.Wang         Create
** 2010/08/23       Yifeng.Wang         Modify
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "stdafx.h"
#include "file_path.h"
#include "windows.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DEFAULT_ENTRY_PATH      "c:\\"      // default entry path


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/* TCPIP module entry path */
LOCAL char s_entry_path[MAX_PATH_LENGTH];

/*---------------------------------------------------------------------------*
/*                          static FUNCTION DECLARE                           *
/*---------------------------------------------------------------------------*/
LOCAL BOOL IsExisted( char* path_name );
LOCAL BOOL IsRootDir( char* path_name );
LOCAL void FormatPath( char* path_name, int max_len );
LOCAL BOOL IsInDir( char* file_name, char* path_name );
LOCAL BOOL RecursiveSearch( 
      char* found_path, int max_len, char* object_name, char* search_path );
LOCAL BOOL FindEntryPath( void );
LOCAL BOOL ParentPath( char* path_name );

/*---------------------------------------------------------------------------*
/*                          static FUNCTION DEFINITION                        *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: check if given path or file is existed
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL BOOL IsExisted( char* path_name )
{
    WIN32_FIND_DATA find_data;
	HANDLE          find_handle;
    
    if ( NULL == path_name )
        return FALSE;

    find_handle = FindFirstFile( path_name, &find_data );
	if ( find_handle != INVALID_HANDLE_VALUE )	
	{
		FindClose( find_handle );
		return TRUE;
	}
	else
		return FALSE;
}

/*****************************************************************************/
// Description: judge if a directory is a root directory
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL BOOL IsRootDir( char* path_name )
{
    char root[4];     // 4-chars is enough to describe a root directory  
    if ( (NULL == path_name) || (strlen(path_name) < 3) )
        return FALSE;

    sprintf( root, "%c:\\", path_name[0] );
    return ( (strcmp(root,path_name)==0)? TRUE : FALSE );
}

/*****************************************************************************/
// Description: format path string end with '\' 
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL void FormatPath( char* path_name, int max_len )
{
    int len = 0;
    ASSERT( (NULL!=path_name) && (max_len>0) );
    
    // if the end char is '\', the function directly return
    len = strlen( path_name );
    if ( '\\' == path_name[len-1] )
        return;
        
    // if the path is not root path, then add a '\' at the end
    if ( !IsRootDir( path_name ) )
    {
        //SCI_TRACE_HIGH("len = %d\n", len);
        ASSERT( max_len > ( len + (int)strlen("\\")) );
        strcat( path_name, "\\" );  
    }
}

/*****************************************************************************/
// Description: get parent path of the input path name
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL BOOL ParentPath( char* path_name )
{
    int i = 0, nLen = 0;
    char* p = path_name;
    BOOL ret = FALSE;

    if( NULL == path_name )
        return FALSE;

    nLen = strlen(path_name);
    if( nLen <= 0 )
        return FALSE;

    if( p[nLen-1] == '\\' )
        nLen--;

    for( i = nLen-1 ; i >= 0 ; i-- )
    {
        if( p[i] == '\\' )
        {
            p[i+1] = 0x00;
            ret = TRUE;
            break;
        }

        if( p[i] == ':' )
        {
            ret = FALSE;
            break;
        }
    }
    return ret;
}

/*****************************************************************************/
// Description: check if 'object_name' in the given 'path_name' 
// Global resource : none
// Author: yifeng.wang  
// Note: input 'object_name' can be a file or a directory name
/*****************************************************************************/
LOCAL BOOL IsInDir( char* object_name, char* path_name )
{
    WIN32_FIND_DATA find_data;
    char    find_name[ MAX_PATH ];
    HANDLE  find_handle = NULL;

    if ( NULL == object_name || NULL == path_name )
        return FALSE;

    ASSERT( MAX_PATH > (strlen(path_name)+strlen(object_name)) );
    memset( find_name, '\0', sizeof(find_name) );
    strncpy( find_name, path_name, strlen(path_name)+1 );
    FormatPath( find_name, MAX_PATH );
    strcat( find_name, object_name );  // construct full file path name

    // find file in path_name
    find_handle = FindFirstFile( find_name, &find_data );
    if ( find_handle == INVALID_HANDLE_VALUE ) 
    {
        return FALSE;
    }
    else  // file found
    {
        FindClose( find_handle );
        TRACE("\n[Find] \"%s\" in \"%s\"", object_name, path_name );
		return TRUE;
    }
}

/*****************************************************************************/
// Description: search file or dir fully in a given directory 
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL BOOL RecursiveSearch( char* found_path, int max_len, char* object_name, char* search_path )
{
    WIN32_FIND_DATA find_data;
    char    find_name[MAX_PATH];
    HANDLE  find_handle = NULL;

    ASSERT( NULL != found_path );
    if ( NULL == object_name || NULL == search_path )
        return FALSE; 

    // find object in search_path 
    if ( IsInDir( object_name, search_path ) )
    {
        strncpy( found_path, search_path, strlen(search_path)+1 );
        FormatPath( found_path, max_len );
        ASSERT( max_len > (int)(strlen(object_name)+strlen(search_path)) );
        strcat( found_path, object_name ); 
        return TRUE;
    }

    // find sub-directory  
    memset( find_name, '\0', MAX_PATH );
    strcpy( find_name, search_path );
    FormatPath( find_name, MAX_PATH );
    ASSERT( MAX_PATH > (strlen(find_name)+strlen("*.*")));
    strcat( find_name, "*.*" );     // find all

    find_handle = FindFirstFile( find_name, &find_data );
    if ( find_handle == INVALID_HANDLE_VALUE) 
    {
        return FALSE;
    }
    
    // search object in sub-directories
    do
    {
        // omit directories '.' and '..'
        if ( find_data.cFileName[0] == '.')
            continue;   
        
        // check if the found is directory
        if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            char sub_dir[MAX_PATH];  
            memset( sub_dir, '\0', MAX_PATH );
            strcpy( sub_dir, search_path );
            FormatPath( sub_dir, MAX_PATH );
            ASSERT( MAX_PATH > (strlen(sub_dir)+strlen(find_data.cFileName)) );
            strcat( sub_dir, find_data.cFileName );

            // recursively search in sub-directory
            if ( RecursiveSearch( found_path, max_len, object_name, sub_dir ) )
            {
                FindClose( find_handle );   // close search handle  
                return TRUE; 
            }
        }
    } 
    while ( FindNextFile(find_handle, &find_data) );

    FindClose( find_handle );   // close search handle  
    return FALSE;
}

/*****************************************************************************/
// Description: find entry path for ethernet and driver directory
// Global resource : s_entry_path
// Author: yifeng.wang  
// Note: entry path is used to find ethernet configure file
/*****************************************************************************/
LOCAL BOOL FindEntryPath( void )
{
    char curr_path[MAX_PATH_LENGTH], temp_path[MAX_PATH_LENGTH];

    memset( curr_path, '\0', sizeof(curr_path) );
    memset( temp_path, '\0', sizeof(temp_path) );
    
    // get current programme path name
    DWORD ret_val = GetCurrentDirectory( NULL, curr_path );
    if ( 0 == ret_val )
    {
        TRACE("GetCurrentDirectory() failed!\n");
        return FALSE;
    }
    
    ASSERT( ret_val <= MAX_PATH_LENGTH );
    ret_val = GetCurrentDirectory( MAX_PATH_LENGTH, curr_path );
    if ( 0 == ret_val )
    {
        TRACE("GetCurrentDirectory() failed!\n");
        return FALSE;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
/*                               FUNCTIONS                                   *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: check if the file or path is existed
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
PUBLIC BOOL FILEPATH_IsExisted( CString strPathName )
{
    char    chPath[MAX_PATH_LENGTH];
    memset( chPath, '\0', sizeof(chPath) );

    int len = strPathName.GetLength();
    ASSERT( MAX_PATH_LENGTH > len );
    strcpy( chPath, strPathName.GetBuffer(len) );
    strPathName.ReleaseBuffer();
    return IsExisted( chPath );
}

/*****************************************************************************/
// Description: format Path (e.g. non-root path end with '\')
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
PUBLIC void FILEPATH_FormatPath( CString& strPathName )
{
    char    chPath[MAX_PATH_LENGTH];
    memset( chPath, '\0', sizeof(chPath) );

    int len = strPathName.GetLength();
    ASSERT( MAX_PATH_LENGTH > len );
    strcpy( chPath, strPathName.GetBuffer(len) );
    strPathName.ReleaseBuffer();

    FormatPath( chPath, MAX_PATH_LENGTH );
    strPathName.Format("%s", chPath );
}

/*****************************************************************************/
// Description: get parent path (e.g. non-root path end with '\')
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
PUBLIC BOOL FILEPATH_ParentPath( CString& strPathName )
{
    BOOL    res = FALSE;
    char    chPath[MAX_PATH_LENGTH];
    memset( chPath, '\0', sizeof(chPath) );

    int len = strPathName.GetLength();
    ASSERT( MAX_PATH_LENGTH > len );
    strcpy( chPath, strPathName.GetBuffer(len) );
    strPathName.ReleaseBuffer();

    res = ParentPath( chPath );
    if( res )
    {
        strPathName.Format("%s", chPath);
    }
    return res;
}

/*****************************************************************************/
// Description: get current app path 
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
PUBLIC BOOL FILEPATH_GetCurrentAppPath( CString& strCfgPath )
{
    char curr_path[MAX_PATH_LENGTH];
    DWORD ret_val = 0;

    memset( curr_path, '\0', sizeof(curr_path) );

    // get current programme path name
    ret_val = GetCurrentDirectory( NULL, curr_path );
    if ( 0 == ret_val )
    {
        TRACE("FILEPATH_GetCurrentAppPath: GetCurrentDirectory() failed with buffer len 0!\n");
        goto error;
    }

    // check whether file path buffer is long enough
    if( ret_val <= MAX_PATH_LENGTH )
    {
        ret_val = GetCurrentDirectory( MAX_PATH_LENGTH, curr_path );
        if ( 0 == ret_val )
        {
            TRACE("FILEPATH_GetCurrentAppPath: GetCurrentDirectory() failed with buffer len %d!\n",
                MAX_PATH_LENGTH);
            goto error;
        }
        strCfgPath.Format("%s", curr_path);
    }
    else
    {
        // if buffer not long enough, using default entry path
        TRACE("FILEPATH_GetCurrentAppPath: App path is too long, using default file path!\n");
        goto error;
    }
    return TRUE;

error:
    strCfgPath.Format("%s", DEFAULT_ENTRY_PATH);
    return FALSE;
}



