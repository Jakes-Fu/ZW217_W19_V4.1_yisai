

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_common.h"
#include "http_api.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef __cplusplus
extern   "C"
    {
#endif

typedef struct
{
  char *data_url;
  char *data;
  char *postBody;
  uint32 data_len;
} ZTE_DATA_CONTENT_T;

typedef void (*ZTEHTTPGETREQUEST) (ZTE_DATA_CONTENT_T * data,BOOLEAN success,uint32 idx);

typedef struct
{
  HTTP_CONTEXT_ID_T context_id;

  uint32 request_win_idx;

  ZTE_DATA_CONTENT_T *chatdata;

  BOOLEAN issuccess;

  uint8 m_request_time_id;

  ZTEHTTPGETREQUEST callback;

  uint8 type;

} MMIZTE_HTTP_CONTEXT_T;



/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

