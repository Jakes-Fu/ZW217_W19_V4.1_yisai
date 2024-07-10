#ifndef _COMM_OMA_DOWNLOAD_H_ /*perl robot add*/
#define _COMM_OMA_DOWNLOAD_H_ /*perl robot add*/
/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_oma_download.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#include "comm_platform.h"

#define OMA_DOWNLOAD_TAG_LEN          20
#define OMA_DOWNLOAD_MAX_URL_LEN  512
#define OMA_MAX_FILE_SIZE                    10

#define OMA_NOTIFY_REPORT_MAX_LEN  200

typedef enum
{
    OMA_STATUS_NONE,

    OMA_STATUS_SUCCESS=900,	              //success
    OMA_STATUS_LACK_MEM,	                    //Insufficient memory
    OMA_STATUS_CANCEL,	                          //User Cancelled
    OMA_STATUS_LOSS_SERVICE,	            //Loss of Service 
    OMA_STATUS_MISMATCH=905,               //Attribute mismatch 
    OMA_STATUS_INVALID_DD,                    //Invalid descriptor
    OMA_STATUS_INVALID_VERSION=951,   //Invalid DDVersion 
    OMA_STATUS_ABORT,                            //Device Aborted 
    OMA_STATUS_NON_ACCEPTABLE,          //Non-Acceptable Content
    OMA_STATUS_LOADER_ERR,                 //Loader Error Indicates
    OMA_STATUS_DRM_REL_ERR               //DRM analysis err

}enum_oma_dl_status;


typedef enum
{
    TAG_TYPE,		                     //object type ,there maybe many type. it is Mandatory
    TAG_NAME,		                     //object name,it is optional, it can be used for name of restore object file     
    TAG_SIZE,		                           //the size of object.it is Mandatory
    TAG_OBJECT_URI,		             //the uri of object.it is Mandatory
    TAG_INSTALL_NOTIFY_URI,		//the  destinaton uri for user to notify the download state, it is optional  
#ifdef OMA_DOWNLOAD_FULL_SUPPORT    
    TAG_DD_VERSION,		//the version of the descriptor file ,it is optional
    TAG_DESCRIPTION,		//A short textual description of the media object, it is optional
    TAG_NEXT_URL,		//the url for use to browse after download object, it is optional
    TAG_VENDOR,		//The organisation that provides the media object, it is optional
    TAG_INFO_URL,		//A URL for further describing the media object, it is optional
    TAG_ICON_URI,		//The URI of an icon
    TAG_INSTALL_PARAM	,	//An installation parameter associated with the downloaded media object Status ,it is optional
#endif
    TAG_DD_TAG_NUM                  //parameter number

}enum_dd_tag;



#define OMA_DL_SUCCESS                  "900  Success"
#define OMA_DL_NO_MEMORY            "901  Insufficient memory"
#define OMA_DL_USER_CANCEL          "902  User Cancelled"
#define OMA_DL_LOSS_SERVICE         "903  Loss of Service"
#define OMA_DL_ATTRIBUTE_ERR      "905  Attribute mismatch"
#define OMA_DL_DD_INVALID             "906  Invalid descriptor"
#define OMA_DL_VERSION_INVALID    "951  Invalid DDVersion"
#define OMA_DL_DEVICE_ABORT         "952  Device Aborted"
#define OMA_DL_ERR_CONTENT          "953  Non-Acceptable Content"
#define OMA_DL_LOADER_ERR            "954  Loader Error"


typedef struct _oma_download_desc_t
{
    comm_boolean              id_downloading;
    char                             objectURI[OMA_DOWNLOAD_MAX_URL_LEN + 1 ];
    char                             notifyURI[OMA_DOWNLOAD_MAX_URL_LEN + 1 ];
    comm_wchar                  file_name[MMIFILE_FILE_NAME_MAX_LEN + 1];
    char*                            dd_element[TAG_DD_TAG_NUM];
    comm_uint32                 file_size;
    enum_oma_dl_status     download_status;
    comm_boolean              download_result;

}oma_download_desc_t;


/*==========================================================
 * Function 	: comm_start_oma_download
 * Author		: sunguochen      
 * Description : 开始oma download
                      目前不支持多个 oma download并发,
                      如果发现之前有download，根据状态处理
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_boolean comm_start_oma_download(const char* dd_data, oma_download_desc_t* oma_dd_data);


/*==========================================================
 * Function 	: Music_PostSuccessResult
 * Author		: sunguochen      
 * Description : 发送状态报告
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_PostSuccessResult (oma_download_desc_t* oma_dd_data);


/*==========================================================
 * Function 	: Music_PostFailResult
 * Author		: sunguochen      
 * Description : 发送状态报告
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
void comm_PostFailResult (oma_download_desc_t* oma_dd_data);




#endif  /*perl robot add*/

