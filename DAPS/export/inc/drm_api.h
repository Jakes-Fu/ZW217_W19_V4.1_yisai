/*****************************************************************************
** File Name:      drm_api.h                                              *
** Author:                                                                   *
** Date:           02/22/2011                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    OMA DRM v1.0: DRM API                    *
******************************************************************************/

#ifndef _DRM_API_H_
#define _DRM_API_H_

#ifdef DRM_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#include "mime_type.h"
#include "dal_time.h"
#include "mn_api.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//#define DRM_SURPORT_FL_TYPE
//#define DRM_SURPORT_CD_TYPE
//#define DRM_SURPORT_SD_TYPE

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//DISK RIGHTS PATH
#if defined(DRM_LITTLEBIAN_ENABLE)
#define DDISK_RIGHTS_DIR "D:\\DRM_LRO"
#define EDISK_RIGHTS_DIR "E:\\DRM_LRO"
#define FDISK_RIGHTS_DIR "F:\\DRM_LRO"
#else
#define DDISK_RIGHTS_DIR "D:\\DRM_BRO"
#define EDISK_RIGHTS_DIR "E:\\DRM_BRO"
#define FDISK_RIGHTS_DIR "F:\\DRM_BRO"
#endif

#define DRM_RIGHTS_DIR_LEN   11

#define DRM_SMALL_RIGHTS
#ifdef DRM_SMALL_RIGHTS
#define DRM_URL_MAX_LEN                      128         // rights Max url address len
#define DRM_CONTENT_ID_MAX_LEN         128         // rights and context file Max content id len    
#else
#define DRM_URL_MAX_LEN                      256         // rights Max url address len
#define DRM_CONTENT_ID_MAX_LEN         256         // rights and context file Max content id len 
#endif
#define DRM_INVALID_HANDLE                 0            // PNULL pointer

/*---------------------------------------------------------------------------*/
/*                          Enum			                                 */
/*---------------------------------------------------------------------------*/
// DRM constraint type
typedef enum
{
    DRM_CONSTRAINT_NONE = 0x00,    // no constraint
    DRM_CONSTRAINT_COUNT = 0x01,             // count
    DRM_CONSTRAINT_INTERVAL = 0x02,         // interval
    DRM_CONSTRAINT_DATETIME =0x04          // date time
}DRM_CONSTRAINT_MODE_E;


 // DRM permission type
typedef enum
{
    DRM_PERMISSION_NONE = 0x00, //rights file NONE
    DRM_PERMISSION_PLAY = 0x01, //rights file permission  play 
    DRM_PERMISSION_DISPLAY = 0x02,     //rights file permission  display
    DRM_PERMISSION_EXECUTE = 0x04,     //rightsfile permission  execute
    DRM_PERMISSION_PRINT = 0x08,           //rights file permission  print
    DRM_PERMISSION_RESERVE = 0x0F          //rights file no read
}DRM_PERMISSION_MODE_E;


// DRM download handler error code
typedef enum
{
    DRM_DH_SUCCESS = 0x00,                       // success
    DRM_DH_FAILED,                                  //1 failed
    DRM_DH_NOT_SUPPORTTED_MIME,         //2 mime type not support
    DRM_DH_INSUFFICIENT_MEMORY,         //3 insufficient memory
    DRM_DH_INVALID_FORMAT,                  //4 file format is invalid
    DRM_DH_IO_PENDING,                          //5 IO is pending
    DRM_DH_UNKNOWN_ERROR,                   //6 unknown error
    DRM_DH_SYSTEM_ERROR,                    //7 System error
    DRM_DH_NO_MEMORY,                           //8 no memory
    DRM_DH_OS_ERROR,                            //9 os error
    DRM_DH_INVALID_PARAM,                    //10 parameter is invalid
    DRM_DH_CREATE_RIGHTS_FILE_FAILED,  //11 create rights file failed
    DRM_DH_SYSTEM_DISK_NO_SPACE,  //12 Rights sava disk no space
    DRM_DH_FILE_ERROR ,                         //13 file operation error
    DRM_DH_XML_PARSE_FAILED,                //14 parse XML failed
    DRM_DH_WBXML_TOO_BIG,                   //15 WBXML size is too big
    DRM_DH_PARSE_WBXML_FAILED,          //16 parse WBXML failed
    DRM_DH_FILE_EOF,                                //17 reach the end of file
    DRM_DH_LINE_TOO_LONG,                    //18 has not get CRCF in file
    DRM_DH_INVALID_RIGHTS_FILE,             //19 DRM rights file is invalid
    DRM_DH_CREATE_FILE_FAILED,              //20 Create file failed
    DRM_DH_GENERATE_KEY_FAILED,             //21 Generate key failed
    DRM_DH_DRM_RIGHTS_INVALID,          //22 Invalid rights        
    DRM_DH_BASE64_FILE_ERROR,           //23 Base64 file invalid
    DRM_DH_FILE_NO_LINE,                    //24 file middle don't have CRLF                    
    DRM_DH_USER_CANCELLED,              //25 User canceled
    DRM_DH_FILE_HAVED_INSTALL,              //26 file is saved
    DRM_DH_NOT_SUPPORTTED_TYPE              //27 FILE type not support
}DRM_ERROR_E;


/* DRM Method Level */
typedef enum
{
    DRM_LEVEL_FL = 0x01,      // Forward lock
    DRM_LEVEL_CD = 0x02,      // Combined delivery
    DRM_LEVEL_SD = 0x04,      // Separate delivery    
    DRM_LEVEL_INVALID = 0xFF  // Invalid
}DRM_METHOD_LEVEL_E;


// DRM rights status
typedef enum
{
    DMR_STATUS_NO_RIGHTS = 0x01,       // no rights
    DRM_STATUS_RIGHTS_PRESENT,  // rights is ok
    DRM_STATUS_RIGHTS_PENDING,  // content is here, need rights
    DRM_STATUS_CONTENT_PENDING,  // rights is here, need content
    DRM_STATUS_RIGHTS_EXPIRED  // rights is here, rights expired
}DRM_RIGHTS_STATUS_E;

//DRM Install file type
typedef enum
{
    DRM_CONTENTT_TYPE =0x01,   //DRM install file is content type
    DRM_RIGHTS_TYPE,     //DRM install file is rights type
    DRM_INVALID_TPYE    //DRM install file is other type
}DRM_FILE_TYPE_E;

//DRM delete rights file type
typedef enum
{
    DRM_DELETE_EXPIRED_RIGHTS_FILE =0x01,   //DRM delete expired rights file
    DRM_DELETE_ALL_SD_RIGHTS_FILE,   //DRM delete sd tpye rights file
    DRM_DELETE_ALL_RIGHTS_FILE    //DRM delete ALL tpye rights file
}DRM_DELETE_RIGHTS_TYPE_E;

//DRM net time handle type
typedef enum
{
    DRM_NET_TIME_ADD =0x01,   //DRM net time add
    DRM_NET_TIME_REDUCE,   //DRM net time reduce
}DRM_NET_TIME_HANDLE_TYPE_E;

/*---------------------------------------------------------------------------*/
/*                          Function pointer                                 */
/*---------------------------------------------------------------------------*/

// DRM network time
typedef struct _DRM_NETWORK_TIME
{
    BOOLEAN is_nettime; //system is nettime, TURE is nettime
    uint32 different_nettime;    //loacl system time different net time
    DRM_NET_TIME_HANDLE_TYPE_E net_time_hanle_type; //net time handle type
}DRM_NETWORK_TIME_T;

typedef BOOLEAN (*DRM_NETWORK_TIIME_FUN)(DRM_NETWORK_TIME_T *network_time_ptr);   //NTP function callback pointer

/*---------------------------------------------------------------------------*/
/*                          Struct				                             */
/*---------------------------------------------------------------------------*/
typedef uint32 DRM_HANDLE;      // For APP, the same as SFS_HANDLE


// DRM constraint
typedef struct _DRM_CONSTRAINT
{
    uint32 type;                // DRM_CONSTRAINT_XXX
    uint32 used_count;       // has used count
    uint32 total_count;       // total count
    uint32 start_time;        // start time
    uint32 end_time;          // end time
    uint32 interval;            // interval
    uint32 start_interval;   // interval start time
    uint32 end_interval;     // interval end time
}DRM_CONSTRAINT_T;


// DRM rights
typedef struct _DRM_RIGHTS
{
    char rights_issuer[ DRM_URL_MAX_LEN + 1]; // rights issuer
    char content_id[ DRM_CONTENT_ID_MAX_LEN + 1]; // content id
    DRM_CONSTRAINT_T constraint;              // constraint detail
    DRM_PERMISSION_MODE_E permission;       // permission
    DRM_METHOD_LEVEL_E  method_level;       // Method level
    DRM_RIGHTS_STATUS_E status;                 // Rights status
    MIME_TYPE_E mime_type;                      // content mime type
    MN_IMEI_T rights_imei;     //check imei
}DRM_RIGHTS_T;


/*DRM Install handle file*/
typedef struct _DRM_INSTALL_FILE
{
    char  *header_type_ptr;               //DRM download method MIME type  
    uint16 *download_file_name_ptr; //DRM download file path name
    uint16 *new_file_name_ptr;          //DRM install new file name
    char *rights_buffer_ptr;                // DRM rights download buffer pointer 
    uint32 rights_buf_len;                   // DRM rights  buffer len 
		uint32  index;                    // instatll index num
    DRM_FILE_TYPE_E install_file_type; //DRM install file type
    BOOLEAN is_wbxml_rights;            //if DRM rights file encoded type, true is wbxml type
	BOOLEAN is_hava_saved;            //if DRM file haved svaed is true else is flase
}DRM_INSTALL_FILE_T;

//DRM get drm file context type
typedef struct _DRM_FILE_CONTEXT_TYPE
{
    char  *header_type_ptr;               //DRM download method MIME type  
    uint16 *download_file_name_ptr; // DRM download file name
} DRM_FILE_CONTEXT_TYPE_T;

//DRM Init Agent parm
typedef struct _DRM_INIT_AGENT
{
    DRM_NETWORK_TIIME_FUN  get_network_time_func; //network time function callback pointer  
    uint16 *rights_path_ptr; //save rights path
} DRM_INIT_AGENT_T; 

//DRM install file CNF
typedef struct _DRM_INSTALL_FILE_CNF_SIG
{
    _SIGNAL_VARS
		uint32  index;                    // instatll index num
    DRM_FILE_TYPE_E install_file_type; //DRM install file type
    DRM_ERROR_E error_code; //DRM file install  error code
} DRM_INSTALL_FILE_CNF_SIG_T;

//DRM get drm file context type REQ
typedef struct _DRM_FILE_CONTEXT_TYPE_REQ_SIG
{
    _SIGNAL_VARS
    uint32 media_type;  //DRM download method MIME type  
    uint16 *download_file_name_ptr; // DRM download file name
} DRM_FILE_CONTEXT_TYPE_REQ_SIG;

//DRM get drm file context type CNF
typedef struct _DRM_FILE_CONTEXT_TYPE_CNF_SIG
{
    _SIGNAL_VARS
    MIME_TYPE_E mime_type; //DRM context type
} DRM_FILE_CONTEXT_TYPE_CNF_SIG;

// DRM Rights INTERVAL FORMAT
typedef struct _DRM_RIGHTS_INTERVAL
{
    SCI_DATE_T date;    //INTERVAL DATE FORMAT
    SCI_TIME_T time;    //NTERVAL TIME FORMAT
}DRM_RIGHTS_INTERVAL_T;

// DRM delete rights file handle
typedef struct _DRM_DELETE_RIGHTS_FILE
{
    DRM_DELETE_RIGHTS_TYPE_E delete_type;   //delete rights type
    uint16* delete_rights_path_ptr;   //delete rights file path
}DRM_DELETE_RIGHTS_FILE_T;

// DRM delete rights file REQ sig
typedef struct _DRM_DELETEL_RIGHTS_FILE_REQ_SIG
{
    _SIGNAL_VARS
    DRM_DELETE_RIGHTS_TYPE_E delete_type;   //delete rights type
    uint16* delete_rights_path_ptr;   //delete rights file path
} DRM_DELETEL_RIGHTS_FILE_REQ_SIG;

// DRM delete rights file CNF sig
typedef struct _DRM_DELETEL_RIGHTS_FILE_CNF_SIG
{
    _SIGNAL_VARS
    DRM_ERROR_E error_code; //DRM file install  error code
} DRM_DELETEL_RIGHTS_FILE_CNF_SIG;

/*DRM cancel Install handle file*/
typedef struct _DRM_CANCEL_INSTALL_FILE
{
    uint16 *download_file_name_ptr; //DRM download file path name
    uint16 *new_file_name_ptr;          //DRM install new file name
}DRM_CANCEL_INSTALL_FILE;

/*DRM cancel Install handle file REQ*/
typedef struct _DRM_CANCEL_INSTALL_FILE_REQ
{
    _SIGNAL_VARS
    uint16 download_file_name[SFS_MAX_PATH+1];  //DRM download file path name
    uint16 new_file_name[SFS_MAX_PATH+1];    //DRM install new file name 
    uint32 original_task_id;
}DRM_CANCEL_INSTALL_FILE_REQ;

//DRM install file CNF
typedef struct _DRM_CANCEL_INSTALL_FILE_CNF_SIG
{
    _SIGNAL_VARS
    DRM_ERROR_E error_code; //DRM file install  error code
} DRM_CANCEL_INSTALL_FILE_CNF_SIG;

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
// Description: get file content mime type 
// Author: dewu jiang
// Return: 
//      DRM_DH_SUCCESS: Success
//      Other: failed reason
// Note: async function
/*****************************************************************************/
PUBLIC DRM_ERROR_E DRM_GetContentMimeTypeReq (const DRM_FILE_CONTEXT_TYPE_T *content_ptr ); //IN
/*****************************************************************************/
// Description: install DRM file
// Global resource dependence : 
// Author: 
// Return: 
//      DRM_DH_SUCCESS: Success
//      Other: failed reason
// Note: async function
/*****************************************************************************/
PUBLIC DRM_ERROR_E DRM_InstallFileReq(
                    const DRM_INSTALL_FILE_T *file_handle_ptr   //IN
                    );

/*****************************************************************************/
//    Description: Is it a DRM file
//    Author: 
//	  Return: 
//		TRUE: it is
//		FALSE: it is not
//     Note: sync function 
/*****************************************************************************/
PUBLIC BOOLEAN DRM_IsDRMFile ( DRM_HANDLE handle );     // [IN]

/*****************************************************************************/
//    Description: get DRM file type
//    Author: 
//	  Return: DRM_METHOD_LEVEL_E
//     Note: sync function 
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E DRM_GetDRMFileType(
                            DRM_HANDLE handle   //IN
                            );

/*****************************************************************************/
//    Description:is rights expired
//    Author: 
//	  Return: TURE is expired, FALSE isn't expired
//     Note: sync function 
/*****************************************************************************/
PUBLIC BOOLEAN DRM_IsRightsExpired(
                    DRM_HANDLE handle, //IN
                    uint32 permission_flag  //IN
                    );

/*****************************************************************************/
//    Description: get rights constraint infor
//    Author: dewu jiang
//	  Return: 
//     Note: sync function 
/*****************************************************************************/
PUBLIC BOOLEAN DRM_GetRightsConstraint(DRM_HANDLE handle,   //IN
                                                    DRM_CONSTRAINT_T* constraint_ptr);  //OUT

/*****************************************************************************/
//    Description:get rights infor
//    Author: dewu jiang
//	 Return:
//    Note: sync function 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_GetRightsInfo( 
                    SFS_HANDLE handle, //IN
                    DRM_RIGHTS_T* rights_ptr //OUT
                    );

/*****************************************************************************/
//    Description:Consume rights
//    Author: dewu jiang
//	 Return:
//    Note: sync function
/*****************************************************************************/
PUBLIC BOOLEAN DRM_ConsumeRights( SFS_HANDLE handle );  //IN

/*****************************************************************************/
//    Description: set SD file encrpt 
//    Author:
//    Return: 
//      SFS_ERROR_NONE: write success
//      Other: failed reason
//     Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_SDSetEncrypt( SFS_HANDLE handle, BOOLEAN is_encrpt_Read);

/*****************************************************************************/

/*****************************************************************************/
//    Description:get rights interval val
//    Author: dewu jiang
//	 Return:
//     Note: sync function 
/*****************************************************************************/
PUBLIC SFS_ERROR_E DRM_GetDateTimeFormInterval( SFS_HANDLE handle,    //IN
                                                            DRM_RIGHTS_INTERVAL_T* interval_ptr );  //OUT

/****************************************************************************/
// Description: delete right file
// Author: dewu jiang
// Return: 
//      DRM_DH_SUCCESS: Success
//      Other: failed reason
// Note: async function
/*****************************************************************************/
PUBLIC DRM_ERROR_E DRM_DeleteRightsHandleReq (const DRM_DELETE_RIGHTS_FILE_T *delete_right_ptr);  //IN

/*****************************************************************************/
// Description: cancel install DRM file
// Global resource dependence : 
// Author: 
// Return: 
//      DRM_DH_SUCCESS: Success
//      Other: failed reason
// Note: async function
/*****************************************************************************/
PUBLIC DRM_ERROR_E DRM_CancelInstallFileReq(const DRM_CANCEL_INSTALL_FILE *file_install_cancel_ptr);      // IN

// Description: Init DRM agent callback and path info
// Global resource dependence : 
// Author: 
// Return:
// Note: sync function 
/*****************************************************************************/
PUBLIC void DRM_InitAgent( const DRM_INIT_AGENT_T *init_agent_ptr ); //[IN]


/*****************************************************************************/
// Description: deInit DRM agent callback and path info
// Global resource dependence : 
// Author: 
// Return:
// Note: sync function 
/*****************************************************************************/
PUBLIC void DRM_DeInitAgent(void); 

/*****************************************************************************/
// Description: Init DRM agent MGR task
// Global resource dependence : 
// Author: 
// Return: TRUE: init success, FALSE: init failed
// Note: Before start download handler task and IO task, init agent first
/*****************************************************************************/
PUBLIC void DRM_InitDrmTaskMGR( void );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif // DRM_SUPPORT

#endif //_DRM_API_H_
