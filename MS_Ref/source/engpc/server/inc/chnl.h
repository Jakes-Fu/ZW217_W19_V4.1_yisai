/******************************************************************************
 ** File Name:      chnl.h                                                    *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    define chnl interface                                     *
 ******************************************************************************/

#ifndef _CHNL_H_
#define _CHNL_H_

#include "sci_types.h"

enum
{
    CHNL_TYPE_FT,
    CHNL_TYPE_DEBUG,

    CHNL_TYPE_NONE,
};

typedef enum
{
    CP_DIAG,
    CP_AT,
    WCN_DATA,
    WCN_AT,
    
    DATA_UNKNOWN
}CHNL_DATA_TYPE;

typedef enum
{
    CHNL_DIAG_PC_CP,
    CHNL_DIAG_CP_PC,
    CHNL_DIAG_PC_WCN,
    CHNL_DIAG_WCN_PC,

    CHNL_DIAG_TOTAL = 0x10,

    CHNL_AT_PC_CP,
    CHNL_AT_CP_PC,
    CHNL_AT_PC_WCN,
    CHNL_AT_WCN_PC,

    CHNL_AT_TOTAL
}CHNL_TOTAL;

typedef struct _chnl_type_name{
    int chnl_type;
    CHNL_DATA_TYPE data_type;
    char name[32];
}CHNL_TYPE_NAME, *LPCHNL_TYPE_NAME;

#define PRINT_NUM_MAX 20
#define PRINT_BRK_FLAG 1

#define DIAG_FLAG 0x7E

#define DATA_TO_DIAG_NEW(_DIAG_DST_, src, len) \
                (_DIAG_DST_) = (uint8 *)SCI_ALLOC_APP((len)+3); \
                SCI_ASSERT((_DIAG_DST_)); \
                SCI_MEMSET((_DIAG_DST_), 0, (len)+3); \
                SCI_MEMCPY((_DIAG_DST_)+1, (src), (len)); \
                *(_DIAG_DST_) = DIAG_FLAG; \
                *((_DIAG_DST_)+(len)+1) = DIAG_FLAG; \
                *((_DIAG_DST_)+(len)+2) = 0; 

#define DIAG_TO_DATA_NEW(_DATA_DST_, src, len) \
                (_DATA_DST_) = (uint8 *)SCI_ALLOC_APP((len)-2); \
                SCI_ASSERT((_DATA_DST_)); \
                SCI_MEMSET((_DATA_DST_), 0, (len)-2); \
                SCI_MEMCPY((_DATA_DST_), (src)+1, (len)-2);

#define DIAG_TO_DATA_MOVE(src, offset, len) \
                while((offset) <= (len)-3){ \
                    (src)[(offset)] = (src)[(offset)+1]; \
                    (offset)++; \
                } \
                (src)[(offset)] = 0x00; \
                (src)[(offset+1)] = 0x00; \
                (len) -= 2; 

#define AT_TO_DATA_MOVE(src, length) \
                    while((length) > 0){ \
                        (src)[sizeof(MSG_HEAD_T)+(length)-1] = (src)[(length-1)]; \
                        (length)--; \
                    } \

PUBLIC int chnl_diag_trans(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);
PUBLIC int chnl_at_trans(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);
PUBLIC char* findNameByChnlType(int chnl_type);
PUBLIC CHNL_DATA_TYPE findChnlDataTypeByChnlType(int chnl_type);
void printData(char* name, CHNL_DATA_TYPE dataType, char* buff, int len, int col, int brk);

PUBLIC void chnl_init();

#endif
