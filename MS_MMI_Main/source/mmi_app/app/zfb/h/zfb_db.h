
#ifndef _ZFB_ZFB_DB_H_
#define _ZFB_ZFB_DB_H_

#include "zfb_common.h"
#define ZFB_DB_NEED_RSVD
#define ZFB_DB_NEED_PROV
#define ZFB_DB_NEED_KV

#define ZFB_DB_DIR "ZFBDB"

#ifdef ZFB_DB_NEED_RSVD
#define ZFB_DB_RSVD_FS_FILE "zfb_rsvd_DBFile.db"

#define MAX_ZFB_RSVD_SIZE 2048

typedef struct _ZFB_DB_RSVD_Type
{
    uint8  rsvd_data[MAX_ZFB_RSVD_SIZE+8];
}ZFB_DB_RSVD_Type;

extern ZFB_DB_RSVD_Type zfb_DB_Rsvd_Rec;
extern BOOLEAN ZFB_DB_RSVD_ListInit(void);
extern BOOLEAN ZFB_DB_RSVD_ListModify(void);
extern BOOLEAN ZFB_DB_RSVD_ListDel(void);
#endif

#ifdef ZFB_DB_NEED_PROV
#define ZFB_DB_PROV_FS_FILE "zfb_prov_DBFile.db"

#define MAX_ZFB_PROV_SIZE 512

typedef struct _ZFB_DB_PROV_Type
{
    uint8  prov_data[MAX_ZFB_PROV_SIZE+8];
}ZFB_DB_PROV_Type;

extern ZFB_DB_PROV_Type zfb_DB_Prov_Rec;
extern BOOLEAN ZFB_DB_PROV_ListInit(void);
extern BOOLEAN ZFB_DB_PROV_ListModify(void);
extern BOOLEAN ZFB_DB_PROV_ListDel(void);
extern char * ZFB_DB_PROV_Get(void);
extern void ZFB_DB_PROV_Set(char *provdata,uint16 prov_len);
extern uint16 ZFB_DB_PROV_GetLen(void);
#endif

#ifdef ZFB_DB_NEED_KV

#define ZFB_DB_KV_FS_FILE "zfb_kv_DBFile.db"

#define ZFB_DB_KV_MAX_SUM                     10
#define ZFB_DB_KV_MAX_KEY_SIZE             64
#define ZFB_DB_KV_MAX_VAL_SIZE             384

typedef struct _sst_kv_t
{
    uint8 key[ZFB_DB_KV_MAX_KEY_SIZE+8];
    uint8 value[ZFB_DB_KV_MAX_VAL_SIZE+8];
    uint32 len;
    uint32 used;
} sst_kv_t;

extern sst_kv_t g_kv_list[ZFB_DB_KV_MAX_SUM];
extern BOOLEAN ZFB_DB_KV_ListInit(void);
extern BOOLEAN ZFB_DB_KV_ListModify(void);
extern BOOLEAN ZFB_DB_KV_ListDelAll(void);
#endif

#define ZFB_DB_INIT_OK_FS_FILE "zfb_init_ok.db"

extern BOOLEAN ZFB_DB_INIT_OK_Set(void);
extern BOOLEAN ZFB_DB_INIT_OK_Get(void);
extern BOOLEAN ZFB_DB_INIT_OK_Delelte(void);
extern BOOLEAN ZFB_DB_INIT_OK_Del(void);
extern BOOLEAN ZFB_DB_INIT_OK_Reset(void);

BOOLEAN ZFB_DB_Init(void);
BOOLEAN ZFB_DB_DelAll(void);
BOOLEAN ZFB_DB_Free(void);


#endif /*_ZFB_YX_DB_H_*/
