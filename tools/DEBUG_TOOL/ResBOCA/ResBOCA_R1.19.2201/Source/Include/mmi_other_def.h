#ifndef _MMI_OTHER_DEF_H__
#define _MMI_OTHER_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

// 定义Other的类型
typedef enum _OTHER_DATA_TYPE_E
{
    OTHER_DATA_UNKNOWN,          // unknown                           
    OTHER_DATA_TYPE_MAX       // Reserved, user can't use it.
} OTHER_DATA_TYPE_E;

typedef struct _OTHER_DATA_INFO_T
{
    OTHER_DATA_TYPE_E  type;
    uint32             data_len;  
    uint8	         * data_ptr;
} OTHER_DATA_INFO_T, * POTHER_DATA_INFO_T;

#ifdef __cplusplus
}
#endif

#endif // _MMI_OTHER_DEF_H__