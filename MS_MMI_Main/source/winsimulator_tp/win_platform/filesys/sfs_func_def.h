/******************************************************************************
 ** File Name:      sfs_func.h                                                *
 ** Author:         andy.bian                                                         *
 ** DATE:                                                                     *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file includes the definition of Func struct 		  *            
 ******************************************************************************/

#ifndef _FUN_CALL_H
#define _FUN_CALL_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sfs.h"

/**---------------------------------------------------------------------------*
 **                         func type                                          *
 **---------------------------------------------------------------------------*/
#define FFSIN
#define FFSOUT
#define FFSINOUT

typedef enum 
{
        FUN_ID_REGISTER			    = 0,
        FUN_ID_UNREGISTER			= 1,
        FUN_ID_FORMAT				= 2,
        FUN_ID_STATUS				= 3,
        FUN_ID_GETSPACE				= 4,
        
        FUN_ID_CREATE				= 6,
        FUN_ID_DELETE				= 7,
        FUN_ID_RENAME				= 8,

        FUN_ID_CREATE_DIR			= 9,
        FUN_ID_DELETE_DIR			= 10,

        FUN_ID_FINDFIRST			= 11,
        FUN_ID_FINDNEXT			    = 12,
		FUN_ID_FINDCOLSE			= 13,

        FUN_ID_GETFILESIZE			= 14,
        FUN_ID_SETFILESIZE			= 15,
		FUN_ID_READFILE			    = 17,
        FUN_ID_WRITEFILE			= 18,
        FUN_ID_SETFILEPOINT		    = 16,
        FUN_ID_GETFILEPOINT		    = 17,
        FUN_ID_FILECLOSE			= 18,

	    FUN_ID_READTXT			    = 19,

	    FUN_ID_ATTR			        = 20,

		//apple add
		FUN_ID_SELCHANGE            = 21,
		FUN_ID_READ_DIR             = 22,
		FUN_ID_OPENFILE             = 23,
        FUN_ID_REC_FIND_FILE   = 24,
        FUN_ID_REC_DEL_DIR_FILE   = 25,

}FUN_ID_E;


typedef struct 
{
	FFSIN TCHAR *dev_name;
}FUN_REGISTER_T;

typedef struct 
{
	FFSIN  TCHAR *dev_name;
}FUN_UNREGISTER_T;

typedef struct 
{
	FFSIN TCHAR *dev_name;
	FFSIN SFS_DEVICE_FORMAT_E format_type;
}FUN_FORMAT_T;

typedef struct
{
	FFSIN  TCHAR* dev_name;
}FUN_STATUS_T;


typedef struct 
{
	FFSIN  TCHAR *name;
	FFSIN  uint16 mode;
}FUN_CREATE_T;

typedef struct 
{   
	FFSIN  TCHAR *name;
}FUN_DELETE_T;

typedef struct 
{
	FFSIN  TCHAR *old_name;
	FFSIN  TCHAR *new_name;
}FUN_RENAME_T;

typedef struct
{
	FFSIN  TCHAR *path;
}FUN_CREATEDIR_T;

typedef struct
{
	FFSIN  TCHAR *path;
}FUN_DELETEDIR_T;

typedef struct
{
	FFSIN   TCHAR *find_name;
	FFSOUT SFS_FIND_DATA_T *find_ptr;
}FUN_FINDFIRST_T;

typedef struct 
{
	FFSIN  SFS_HANDLE sfs_handle;
	FFSOUT SFS_FIND_DATA_T *find_ptr;
}FUN_FINDNEXT_T;

typedef struct{
	FFSIN  SFS_HANDLE sfs_handle;
}FUN_CLOSE_T;

typedef struct
{
	FFSIN  SFS_HANDLE sfs_handle;
	FFSIN  uint32  currentpos;
	FFSIN  uint32  origin;
}FUN_GETFILEPOINT_T;

typedef struct
{
	FFSIN  SFS_HANDLE sfs_handle;
	FFSIN uint32 size;
}FUN_SETFILESIZE_T;

typedef struct
{
	FFSIN  SFS_HANDLE sfs_handle;
	FFSOUT uint32 size;
}FUN_GETFILESIZE_T;

typedef struct
{
	FFSIN   TCHAR *path_name;
	//apple add
	FFSIN   TCHAR *pc_path;
}FUN_READFILE_T;

typedef struct
{
	FFSIN   TCHAR *path_name;
	FFSIN	void    *buffer;
	FFSIN   uint32   bufsize;
			
}FUN_WRITEFILE_T;


typedef struct
{
	FFSIN  SFS_HANDLE sfs_handle;
	FFSIN int32   offset;
	FFSIN uint8   origin;
}FUN_SETFILEPOINT_T;



typedef struct
{
	FFSIN  TCHAR *dev_name;
}FUN_GETSPACE_T;


//this struct MUST be uint8 boundary
typedef struct 
{       
    FUN_ID_E         		    fun_id;
    union
    {       
		FUN_FORMAT_T        format;
        FUN_CREATE_T        create;
        FUN_CLOSE_T         close;
        FUN_DELETE_T        del;
        FUN_RENAME_T        rename;
        FUN_FINDFIRST_T     findfirst;
        FUN_FINDNEXT_T      findnext;
        FUN_CREATEDIR_T     createdir;
        FUN_DELETEDIR_T     deletedir;
        FUN_SETFILEPOINT_T  seek;
        FUN_SETFILESIZE_T   setfilesize;
        FUN_REGISTER_T      regster;
        FUN_UNREGISTER_T    unregister;
        FUN_STATUS_T		status;
        FUN_GETFILEPOINT_T  ftell;
        FUN_GETFILESIZE_T	getfilesize;
		FUN_READFILE_T		readfile;
		FUN_WRITEFILE_T		writefile;
        FUN_GETSPACE_T	    getspace;

    }fun_param;
}FUN_CALL_T;

typedef struct
{
    xSignalHeaderRec    sig;
    FUN_CALL_T          fun_info;
}FUN_CAll_SIG_T;


#endif        

