#ifdef HERO_ENGINE_SUPPORT

#include "sci_types.h"
#include "heroEngineApi.h"

//�绰����������ֵ
#define HR_PHB_SUCCESS 			    (0+HR_PLAT_VALUE_BASE)
#define HR_PHB_ERROR 				    (1+HR_PLAT_VALUE_BASE)
#define HR_PHB_READY 				    (2+HR_PLAT_VALUE_BASE)
#define HR_PHB_NOT_SUPPORT 		  (3+HR_PLAT_VALUE_BASE)
#define HR_PHB_NOT_READY 			  (4+HR_PLAT_VALUE_BASE)
#define HR_PHB_NOT_FOUND 			  (5+HR_PLAT_VALUE_BASE)
#define HR_PHB_STORAGE_FULL 		(6+HR_PLAT_VALUE_BASE) 
#define HR_PHB_NUMBER_TOO_LONG 	(7+HR_PLAT_VALUE_BASE)
#define HR_PHB_OUT_OF_INDEX 		(8+HR_PLAT_VALUE_BASE)

#define HR_PHB_NONE 				    0
#define HR_PHB_SIM 				      1  /*��SIM������*/
#define HR_PHB_NVM 				      2  /*���ֻ�NV����*/
#define HR_PHB_BOTH 				    3  /*��SIM�����ֻ�NV����*/

#define HR_PHB_BY_NAME 			    1
#define HR_PHB_BY_NUMBER 			  2
 
#define HR_PHB_MAX_SEARCH_PATTERN 	(60*2)

typedef struct
{
    int8 pattern[HR_PHB_MAX_SEARCH_PATTERN]; /*�����������unicode*/
    uint16 search_type;                      /*�������ͣ�1ͨ������ 2ͨ������*/
    uint16 storage;                          /*������*/
}T_HR_PHB_SEARCH_REQ;

typedef struct
{
    uint16 index;              /*���б��е�λ�ã���ʼΪ0*/
    uint16 store_index;        /*��SIM�����ֻ��е�����λ��*/
    T_HR_PHB_ENTRY phb_entry;  /*������Ϣ*/
}T_HR_PHB_SEARCH_RSP;

typedef struct 
{
    uint16 storage;            /*��Ϣ�洢��λ�ã�SIM�������ֻ�*/
    uint16 index;              /*Ҫ�޸ĵ���Ŀ���б��е�����(0��ʼ)�������0xFFFF��ʾ����һ����Ŀ*/
    T_HR_PHB_ENTRY entry;      /*ע����������Ϣ�еĺ��������Ϊ��ʱ��ʾҪɾ��indexָ����Ǹ���Ŀ*/
}T_HR_PHB_SET_ENTRY_REQ;

typedef struct
{
	uint16 storage_index;
} T_HR_PHB_SET_ENTRY_RSP;

typedef struct
{
    uint16 index;             /*��Ŀ���б��е�����*/
    uint16 storage;           /*��Ŀ�ı���λ��*/
}T_HR_PHB_GET_ENTRY_REQ;

typedef struct
{
    T_HR_PHB_ENTRY entry;     /*����Ŀ�ľ�����Ϣ*/
}T_HR_PHB_GET_ENTRY_RSP;

typedef struct
{
    int32 storage;            /*1SIM����2�ֻ���3ȫ��*/
}T_HR_PHB_GET_COUNT_REQ;

typedef struct
{
    int32 count;              /*��¼�ĸ���*/
}T_HR_PHB_GET_COUNT_RSP;

typedef struct
{
    int32 op;                 /*��������*/
    int32 result;             /*�������*/
}T_HR_PHB_COMMON_RSP;

typedef struct
{
	int32 phone;
	int32 sim;
	int32 sim2;
	int32 sim3;
	int32 sim4;
}T_HR_PHB_GET_TOTAL_RSP;

typedef enum
{
	HR_PHB_OP_WRITE,
	HR_PHB_OP_READ,
	HR_PHB_OP_EDIT,
	HR_PHB_OP_DELETE
}HR_PHB_OP_E;

typedef enum
{
	HR_PHB_MOBILE,
	HR_PHB_HOME,
	HR_PHB_OFFICE,
	HR_PHB_FAX,
	HR_PHB_PHONE,
	HR_PHB_NUM_MAX
}HR_PHB_NUM_E;

PUBLIC void MMIHEROAPP_StopPHBOperateTimer(void);


#endif
