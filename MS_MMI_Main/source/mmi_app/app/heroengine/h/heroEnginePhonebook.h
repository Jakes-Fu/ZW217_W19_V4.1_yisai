#ifdef HERO_ENGINE_SUPPORT

#include "sci_types.h"
#include "heroEngineApi.h"

//电话本操作返回值
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
#define HR_PHB_SIM 				      1  /*对SIM卡操作*/
#define HR_PHB_NVM 				      2  /*对手机NV操作*/
#define HR_PHB_BOTH 				    3  /*对SIM卡和手机NV操作*/

#define HR_PHB_BY_NAME 			    1
#define HR_PHB_BY_NUMBER 			  2
 
#define HR_PHB_MAX_SEARCH_PATTERN 	(60*2)

typedef struct
{
    int8 pattern[HR_PHB_MAX_SEARCH_PATTERN]; /*搜索条件大端unicode*/
    uint16 search_type;                      /*搜索类型：1通过名称 2通过号码*/
    uint16 storage;                          /*不关心*/
}T_HR_PHB_SEARCH_REQ;

typedef struct
{
    uint16 index;              /*在列表中的位置，起始为0*/
    uint16 store_index;        /*在SIM卡或手机中的物理位置*/
    T_HR_PHB_ENTRY phb_entry;  /*具体信息*/
}T_HR_PHB_SEARCH_RSP;

typedef struct 
{
    uint16 storage;            /*信息存储的位置，SIM卡或者手机*/
    uint16 index;              /*要修改的条目在列表中的索引(0开始)，如果是0xFFFF表示增加一个条目*/
    T_HR_PHB_ENTRY entry;      /*注：当具体信息中的号码和名称为空时表示要删除index指向的那个条目*/
}T_HR_PHB_SET_ENTRY_REQ;

typedef struct
{
	uint16 storage_index;
} T_HR_PHB_SET_ENTRY_RSP;

typedef struct
{
    uint16 index;             /*条目在列表中的索引*/
    uint16 storage;           /*条目的保存位置*/
}T_HR_PHB_GET_ENTRY_REQ;

typedef struct
{
    T_HR_PHB_ENTRY entry;     /*本条目的具体信息*/
}T_HR_PHB_GET_ENTRY_RSP;

typedef struct
{
    int32 storage;            /*1SIM卡，2手机，3全部*/
}T_HR_PHB_GET_COUNT_REQ;

typedef struct
{
    int32 count;              /*记录的个数*/
}T_HR_PHB_GET_COUNT_RSP;

typedef struct
{
    int32 op;                 /*操作类型*/
    int32 result;             /*操作结果*/
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
