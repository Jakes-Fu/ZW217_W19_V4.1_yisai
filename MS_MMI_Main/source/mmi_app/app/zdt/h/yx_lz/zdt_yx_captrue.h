#ifndef __ZDT_YX_CAPTRUE_H__
#define __ZDT_YX_CAPTRUE_H__

#include "zdt_common.h"
#include "zdt_yx_api.h"


#define MAX_YX_CAP_FILENAME_SIZE 100

#define YX_CAPTRUE_SEND_DIR "D:\\Photos"

typedef struct _YX_CAP_SEND_DATA_t
{
    uint8          * pFilename;
    uint16         len;
} YX_CAP_SEND_DATA_T;

typedef struct _YX_CAP_SEND_NODE_t
{
       YX_CAP_SEND_DATA_T  data;
       struct _YX_CAP_SEND_NODE_t * next_ptr;
} YX_CAP_SEND_NODE_T;

typedef enum  _YX_CapSendErr_STAT
{
	YX_CAPSEND_SUCCESS,
	YX_CAPSEND_ERR,
	YX_CAPSEND_ERR_NET,
	YX_CAPSEND_ERR_FILE,
	YX_CAPSEND_ERR_UNREG,
	YX_CAPSEND_ERR_MAX
}YXCapSendErrStatus;

extern BOOLEAN  YX_CAP_Send_Link_DelAll(YX_APP_T *pMe);
extern int32 YX_Net_Send_CapFile_Start(YX_APP_T *pMe);
extern void YX_Net_Send_CapFile_End(YX_APP_T *pMe,uint8 err_id);

extern BOOLEAN YX_Captrue_Send_Start(YX_APP_T * pMe);
extern int YX_Captrue_Handle(YX_APP_T *pMe,DPARAM param);
extern BOOLEAN YX_API_Captrue_Start(uint8 is_local);
extern int YX_Captrue_Send_Handle(YX_APP_T *pMe,DPARAM param);
extern BOOLEAN YX_API_Captrue_Send(uint16 * full_path, uint16  full_path_len);

#endif
