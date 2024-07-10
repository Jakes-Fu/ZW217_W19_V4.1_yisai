#ifndef _MMIMRAPP_PHONEBOOK_H_ /*perl robot add*/
#define _MMIMRAPP_PHONEBOOK_H_ /*perl robot add*/
#ifdef MRAPP_SUPPORT

#include "sci_types.h"
#include "mrporting.h"

//�绰����������ֵ
#define MR_PHB_SUCCESS 			    (0+MR_PLAT_VALUE_BASE)
#define MR_PHB_ERROR 				    (1+MR_PLAT_VALUE_BASE)
#define MR_PHB_READY 				    (2+MR_PLAT_VALUE_BASE)
#define MR_PHB_NOT_SUPPORT 		  (3+MR_PLAT_VALUE_BASE)
#define MR_PHB_NOT_READY 			  (4+MR_PLAT_VALUE_BASE)
#define MR_PHB_NOT_FOUND 			  (5+MR_PLAT_VALUE_BASE)
#define MR_PHB_STORAGE_FULL 		(6+MR_PLAT_VALUE_BASE) 
#define MR_PHB_NUMBER_TOO_LONG 	(7+MR_PLAT_VALUE_BASE)
#define MR_PHB_OUT_OF_INDEX 		(8+MR_PLAT_VALUE_BASE)

#define MR_PHB_NONE 				    0
#define MR_PHB_SIM 				      1  /*��SIM������*/
#define MR_PHB_NVM 				      2  /*���ֻ�NV����*/
#define MR_PHB_BOTH 				    3  /*��SIM�����ֻ�NV����*/

#define MR_PHB_BY_NAME 			    1
#define MR_PHB_BY_NUMBER 			  2
 
#define MR_PHB_MAX_SEARCH_PATTERN 	(60*2)

typedef struct
{
    int8 pattern[MR_PHB_MAX_SEARCH_PATTERN]; /*�����������unicode*/
    uint16 search_type;                      /*�������ͣ�1ͨ������ 2ͨ������*/
    uint16 storage;                          /*������*/
}T_MR_PHB_SEARCH_REQ;

typedef struct
{
    uint16 index;              /*���б��е�λ�ã���ʼΪ0*/
    uint16 store_index;        /*��SIM�����ֻ��е�����λ��*/
    T_MR_PHB_ENTRY phb_entry;  /*������Ϣ*/
}T_MR_PHB_SEARCH_RSP;

typedef struct 
{
    uint16 storage;            /*��Ϣ�洢��λ�ã�SIM�������ֻ�*/
    uint16 index;              /*Ҫ�޸ĵ���Ŀ���б��е�����(0��ʼ)�������0xFFFF��ʾ����һ����Ŀ*/
    T_MR_PHB_ENTRY entry;      /*ע����������Ϣ�еĺ��������Ϊ��ʱ��ʾҪɾ��indexָ����Ǹ���Ŀ*/
}T_MR_PHB_SET_ENTRY_REQ;

typedef struct
{
	uint16 storage_index;
} T_MR_PHB_SET_ENTRY_RSP;

typedef struct
{
    uint16 index;             /*��Ŀ���б��е�����*/
    uint16 storage;           /*��Ŀ�ı���λ��*/
}T_MR_PHB_GET_ENTRY_REQ;

typedef struct
{
    T_MR_PHB_ENTRY entry;     /*����Ŀ�ľ�����Ϣ*/
}T_MR_PHB_GET_ENTRY_RSP;

typedef struct
{
    int32 storage;            /*1SIM����2�ֻ���3ȫ��*/
}T_MR_PHB_GET_COUNT_REQ;

typedef struct
{
    int32 count;              /*��¼�ĸ���*/
}T_MR_PHB_GET_COUNT_RSP;

typedef struct
{
    int32 op;                 /*��������*/
    int32 result;             /*�������*/
}T_MR_PHB_COMMON_RSP;

typedef struct
{
	int32 phone;
	int32 sim;
	int32 sim2;
	int32 sim3;
	int32 sim4;
}T_MR_PHB_GET_TOTAL_RSP;

typedef enum
{
	MR_PHB_OP_WRITE,
	MR_PHB_OP_READ,
	MR_PHB_OP_EDIT,
	MR_PHB_OP_DELETE
}MR_PHB_OP_E;

typedef enum
{
	MR_PHB_MOBILE,
	MR_PHB_HOME,
	MR_PHB_OFFICE,
	MR_PHB_FAX,
	MR_PHB_PHONE,
	MR_PHB_NUM_MAX
}MR_PHB_NUM_E;

PUBLIC void MMIMRAPP_StopPHBOperateTimer(void);


#endif
#endif  /*perl robot add*/

