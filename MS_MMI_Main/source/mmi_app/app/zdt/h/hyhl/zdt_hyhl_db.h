
#ifndef _ZDT_HYHL_DB_H_
#define _ZDT_HYHL_DB_H_

#include "zdt_common.h"

#ifdef ZDT_PLAT_HYHL_SOS_CALL
#define HYHL_DB_NEED_SOS
#endif
#define HYHL_DB_NEED_WHITE
#define HYHL_DB_NEED_TIMESET
#define HYHL_DB_NEED_HIDDEN

#ifdef HYHL_DB_NEED_SOS
#define HYHL_DB_SOS_MAX_SUM                     9
#define HYHL_DB_SOS_MAX_NUMBER_SIZE             20
#define HYHL_DB_SOS_MAX_NAME_SIZE             40

#define HYHL_DB_SOS_FS_FILE "brew/hyhlsos_DBFile.db\0"

typedef struct _HYHL_DB_SOS_Type
{
    uint8 db_id;
    char  sos_num[HYHL_DB_SOS_MAX_NUMBER_SIZE];
    char  sos_name[HYHL_DB_SOS_MAX_NAME_SIZE];
}HYHL_DB_SOS_Type;

extern HYHL_DB_SOS_Type hyhl_DB_Sos_Reclist[HYHL_DB_SOS_MAX_SUM];

extern BOOLEAN HYHL_DB_SOS_ListInit(void);
extern BOOLEAN HYHL_DB_SOS_ListModify(uint8 sos_idx,char * sos_num,uint16 num_len,char * sos_name,uint16 name_len);
extern BOOLEAN HYHL_DB_SOS_ListDel(uint8  sos_idx);
extern BOOLEAN HYHL_DB_SOS_ListDelAll(void);
extern BOOLEAN HYHL_DB_SOS_ListModifyBuf(uint8 sos_idx,char * sos_num,uint16 num_len,char * sos_name,uint16 name_len);
extern BOOLEAN HYHL_DB_SOS_ListDelBuf(uint8  sos_idx);
extern BOOLEAN HYHL_DB_SOS_ListDelAllBuf(void);
extern BOOLEAN HYHL_DB_SOS_ListSaveBuf(void);
extern int HYHL_DB_SOS_IsValidNumber(char * num);
#endif

#ifdef HYHL_DB_NEED_WHITE
#define HYHL_DB_WHITE_MAX_SUM                     50
#define HYHL_DB_WHITE_MAX_NUMBER_SIZE             20
#define HYHL_DB_WHITE_MAX_NAME_SIZE             40

#define HYHL_DB_WHITE_FS_FILE "brew/hyhlwhite_DBFile.db\0"

typedef struct _HYHL_DB_WHITE_Type
{
    uint8 db_id;
    char  white_num[HYHL_DB_WHITE_MAX_NUMBER_SIZE];
    char  white_name[HYHL_DB_WHITE_MAX_NAME_SIZE];
}HYHL_DB_WHITE_Type;

extern HYHL_DB_WHITE_Type hyhl_DB_White_Reclist[HYHL_DB_WHITE_MAX_SUM];

extern BOOLEAN HYHL_DB_WHITE_ListInit(void);
extern BOOLEAN HYHL_DB_WHITE_ListModify(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len);
extern BOOLEAN HYHL_DB_WHITE_ListDel(uint8  white_idx);
extern BOOLEAN HYHL_DB_WHITE_ListDelAll(void);
extern BOOLEAN HYHL_DB_WHITE_ListModifyBuf(uint8 white_idx,char * white_num,uint16 num_len,char * white_name,uint16 name_len);
extern BOOLEAN HYHL_DB_WHITE_ListDelBuf(uint8  white_idx);
extern BOOLEAN HYHL_DB_WHITE_ListDelAllBuf(void);
extern BOOLEAN HYHL_DB_WHITE_ListSaveBuf(void);
extern int HYHL_DB_WHITE_IsValidNumber(char * num);
#endif

#ifdef HYHL_DB_NEED_TIMESET

#define HYHL_DB_TIMESET_FS_FILE "brew/hyhltimeset_DBFile.db\0"

typedef struct _HYHL_DB_TIMESET_Type
{
    uint8 db_id;
    uint32 time_start;
    uint32 time_end;
}HYHL_DB_TIMESET_Type;


extern BOOLEAN HYHL_DB_TIMESET_ListInit(void);
extern BOOLEAN HYHL_DB_TIMESET_ListModify(uint32 time_start,uint32 time_end);
extern BOOLEAN HYHL_DB_TIMESET_ListDel(void);
extern BOOLEAN HYHL_DB_TIMESET_CurTimeIsValid(void);
#endif

#ifdef HYHL_DB_NEED_HIDDEN
#define HYHL_DB_HIDDEN_MAX_SUM                     10

#define HYHL_DB_HIDDEN_FS_FILE "brew/hyhlhidden_DBFile.db\0"

typedef struct _HYHL_DB_HIDDEN_Type
{
    uint8 db_id;
    uint8 time_set;
    uint32 time_start;
    uint32 time_end;
}HYHL_DB_HIDDEN_Type;

extern BOOLEAN HYHL_DB_HIDDEN_ListInit(void);
extern BOOLEAN HYHL_DB_HIDDEN_ListModify(uint8 hidden_idx,uint8 time_set,uint32 time_start,uint32 time_end);
extern BOOLEAN HYHL_DB_HIDDEN_ListDel(uint8  hidden_idx);
extern BOOLEAN HYHL_DB_HIDDEN_ListDelAll(void);
extern BOOLEAN HYHL_DB_HIDDEN_ListSave(void);
extern BOOLEAN HYHL_DB_HIDDEN_IsValid(void);
#endif

BOOLEAN HYHL_DB_Init(HYHL_APP_T* pMe);
BOOLEAN HYHL_DB_DelAll(HYHL_APP_T* pMe);
BOOLEAN HYHL_DB_Free(HYHL_APP_T* pMe);


#endif /*_ZDT_HYHL_DB_H_*/
