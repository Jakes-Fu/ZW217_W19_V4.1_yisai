
#ifndef _ZDT_ZDT_DB_H_
#define _ZDT_ZDT_DB_H_

#include "zdt_common.h"

#define ZDT_DB_DIR "ZDTDB"

#define ZDT_DB_NEED_APPCOUNT

#ifdef ZDT_DB_NEED_APPCOUNT

#define ZDT_DB_APPCOUNT_FS_FILE "zdt_appcount_DBFile.db"

typedef struct _ZDT_DB_APPCOUNT_Type
{
    uint8 version;
    uint8 db_id;
    uint32 index;
    uint32 PEDOMETER; //�Ʋ���ʹ�ã��ַ�����
    uint32 ALIPAY; //֧����ʹ�ã��ַ�����
    uint32 CLOCK; //����ʹ�ã��ַ�����
    uint32 CALENDAR; //����ʹ�ã��ַ�����
    uint32 MESSAGE; //����ʹ�ã��ַ�����
    uint32 CALL_RECORD; //ͨ����¼ʹ�ã��ַ�����
    uint32 HOMEWORK; //��ҵʹ�ã��ַ�����
    uint32 ANSWERCARD; //���⿨ʹ�ã��ַ�����
    uint32 ADDRESSBOOK; //ͨѶ¼ʹ�ã��ַ�����
    uint32 HEALTHCODE; //������ʹ�ã��ַ�����
    uint32 IDCODE; //�����ʹ�ã��ַ�����
}ZDT_DB_APPCOUNT_Type;

extern ZDT_DB_APPCOUNT_Type zdt_DB_AppCount_Rec;
extern ZDT_DB_APPCOUNT_Type zdt_DB_AppCount_Bak;
extern BOOLEAN ZDT_DB_APPCOUNT_ListInit(void);
extern BOOLEAN ZDT_DB_APPCOUNT_ListModify(void);
extern BOOLEAN ZDT_DB_APPCOUNT_ListDel(void);
extern BOOLEAN ZDT_DB_APPCOUNT_IsChange(void);
#endif

#ifdef ZDT_ADD_FIRST_POWER_RECORD
#define DB_POWERON_TIME_FS_FILE "poweron_DBFile.db"
typedef struct _DB_Poweron_Time_Type
{
    uint8 is_save;
    uint32  poweron_time;
}DB_Poweron_Time_Type;
extern DB_Poweron_Time_Type DB_Poweron_time;
extern BOOLEAN DB_Poweron_Time_ListSaveBuf(void);
extern BOOLEAN DB_Poweron_Time_ListModifyBuf(uint32 times);
extern BOOLEAN DB_Poweron_Time_ListDel(void);
#endif

BOOLEAN ZDT_DB_Init(void);
BOOLEAN ZDT_DB_DelAll(void);
BOOLEAN ZDT_DB_Free(void);


#endif /*_ZDT_YX_DB_H_*/
