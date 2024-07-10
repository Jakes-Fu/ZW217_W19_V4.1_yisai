#ifndef     _ADAPT_PB_H_
#define     _ADAPT_PB_H_

#include "mmipb_adapt.h"

typedef struct _tagPB_OPC
{
    uint32 handle;
    uint32 entry_id;
    uint8 storage_id;
    uint8 dwAction;
    uint32 dwFlag;
    void *pReserve;
}PB_OPC, *LPPB_OPC;

#define STORAGE_NAME_LEN      15

typedef struct _tagWREPB_STORAGE_TYPE
{
    uint8    storage_id;            //存储介质的ID，唯一的标志存储介质
    wchar  storage_name[STORAGE_NAME_LEN+1];     //存储介质的名称
}WREPB_STORAGE_TYPE, *LPWREPB_STORAGE_TYPE;

typedef struct _tagWREPB_STORAGE_ATTRI
{
    WREPB_STORAGE_TYPE storageType; //存储介质类型
    DWORD  dwMaxSize;           //总容量
    DWORD  dwUsedSize;          //已存储条数
    DWORD  dwAvailSize;          //可存储条数
}WREPB_STORAGE_ATTRI, *LPWREPB_STORAGE_ATTRI;

typedef struct _tagWREPB_FIND_DATA_T
{
    uint8   storage_id;
    uint32  entry_id;
    void    *pReserved;
}WREPB_FIND_DATA, *LPWREPB_FIND_DATA;

typedef struct _tagWREPB_HANDLE
{
    uint32 handle;
    uint32 curIndex;
    void    *pReserve;
}WREPB_FIND_HANDLE, *LPWREPB_FIND_HANDLE;

#define PB_OPC_ADD        0x0001
#define PB_OPC_DEL        0x0002
#define PB_OPC_UPDATE     0x0003
#define PB_OPC_READ       0x0004

#define WREPB_HANDLE uint32

typedef enum
{
    WREPB_STORAGE_SIM1,
#ifndef MMI_MULTI_SIM_SYS_SINGLE        
    WREPB_STORAGE_SIM2,
    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    WREPB_STORAGE_SIM3,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
    WREPB_STORAGE_SIM4,
    #endif
#endif    
    WREPB_STORAGE_PHONE
}WREPB_STORAGE;

typedef enum
{
    WREPB_ERROR_SUCCESS,          //成功
    WREPB_ERROR_ERROR         =1 ,//失败
    WREPB_ERROR_PENDING       =2, //操作中
    WREPB_ERROR_BUSY          =3,//上一个操作还没有结束
    WREPB_ERROR_FILE_NOT_EXIST=4,//文件不存在
    WREPB_ERROR_INVALID_PARAM =5,//参数错误

    WREPB_ERROR_INVALID_FORMAT=6,//格式错误
    WREPB_ERROR_PART_SUCCESS  =7,//部分成功
    WREPB_ERROR_NO_INITIAL    =8,//没有初始化
    WREPB_ERROR_NO_ENTRY      =9,//没有记录
    WREPB_ERROR_NO_NUMBER     =10,//没有号码
    WREPB_ERROR_NO_MSISDN     =11,//没有本机号码
    WREPB_ERROR_BLACKLIST_FULL=12,//黑名单满
    WREPB_ERROR_NUMBER_ALERDY_EXIST=13,//号码已经存在
    WREPB_ERROR_MAIL_LEN=14,//邮件长度错误
    WREPB_ERROR_SIM_ERROR= 15,//sim卡错误
    WREPB_ERROR_FDN_ENABLE = 16,//sim卡错误
    WREPB_ERROR_UDISK_ISRUN = 17,//U盘开启中
    WREPB_ERROR_NO_DEV = 18, //磁盘不存在   

    WREPB_ERROR_SFS_ERROR = 19,//文件系统错误
    WREPB_ERROR_SMS_TOO_LARGE = 20,
    WREPB_ERROR_INVALID_NUMBER = 21,//无效号码
    WREPB_ERROR_NO_SPACE      =22,//空间不足 
    WREPB_ERROR_PHONE_NO_SPACE =23, 
    WREPB_ERROR_SIM1_NO_SPACE=24,
    WREPB_ERROR_SIM2_NO_SPACE = 25,
    WREPB_ERROR_SIM3_NO_SPACE = 26,
    WREPB_ERROR_SIM4_NO_SPACE = 27,
    WREPB_ERROR_FMM_NO_SPACE=28,
    WREPB_ERROR_CANCEL=29,
    WREPB_ERROR_MAX
} WREPB_ERROR_E;
    
typedef  void (*WREPB_ACCESS_CALLBACK_FUNC) (WREPB_ERROR_E error, uint32 handle, LPPB_OPC lpPbOpc);


/*=============================================================
 函数名称 : PBInit
 功能 : 初始化电话本
 参数:无
 返回 : PB_ERROR_SUCCESS: 初始化成功
               其他:失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBInit();

/*=============================================================
 函数名称 : PBUninit
 功能 : 初始化电话本
 参数:无
 返回 : PB_ERROR_SUCCESS: 初始化成功
               其他:失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBUninit();

/*=============================================================
 函数名称 : PBEnumStorage
 功能 : 枚举电话本存储设备
 参数:l
             pPBStorageAttri [out]   存储设备的属性
             nPBStorageNum [out]   存储设备的个数

 返回 : PB_ERROR_SUCCESS 表示成功
               其他: 失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBEnumStorage(WREPB_STORAGE_TYPE *lpPBStorageAttri, uint8 *nPBStorageNum);

/*=============================================================
 函数名称 : PBGetStorageAttri
 功能 : 获取电话本存储设备的属性
 参数:
              storage_id  [in]                存储设备ID
              lpPBStorageAttri [out]      存储设备的属性
              
 返回 : PB_ERROR_SUCCESS 表示成功
               其他:失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBGetStorageAttri(uint8 storage_id, WREPB_STORAGE_ATTRI *lpPBStorageAttri);

/*=============================================================
 函数名称 : PBGetFreeContactId
 功能: 获取特定电话本存储设备中联系人可用ID
 参数: 
              storage_id [in]   存储设备的ID

 返回: 存储设备中可用的联系人ID
              
=============================================================*/
DWORD ADAPT_PBGetFreeContactId(uint8 storage_id);


/*=============================================================
 函数名称 : PBGet
 功能:  获取一条电话本记录
 参数: 
               entry_id [in]                      要获取的记录所在存储设备中的序号id
               storage_id  [in]                  要获取的记录所在的存储设备id
               dwMaxSize  [in]                  缓存区大小
               lpvCard [in][out]                指向一条vCard记录的指针
               dwvCardSize [in][out]         该条vCard记录的大小               
   
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBRead(uint32 entry_id, uint8 storage_id, uint32 dwMaxSize, char *lpvCard, uint32 *dwvCardSize);


/*=============================================================
 函数名称 : PBAdd
 功能:  添加一条电话本记录
 参数: 
              lpvCard [in]             指向一条vCard记录的指针
              dwvCardSize [int]     该条vCard记录的大小
   
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBAdd(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize);


/*=============================================================
 函数名称 : PBAddAsync
 功能: 异步添加一条电话本记录
 参数: 
               lpvCard [in]                指向一条vCard记录的指针
               dwvCardSize [in]        该条vCard记录的大小
               lpAccessCallBack [int   回调函数，在记录操作成功或失败后调用
  
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败
 

=============================================================*/
WREPB_ERROR_E ADAPT_PBAddAsync(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle);

/*=============================================================
 函数名称 : PBDelete
 功能:  删除一条电话本记录
 参数:
               entry_id [in]          要删除的记录所在存储设备中的序号id
               storage_id  [in]      要删除的记录所在的存储设备id

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败


=============================================================*/
WREPB_ERROR_E ADAPT_PBDelete(uint32 entry_id, uint8 storage_id);

/*=============================================================
 函数名称 : PBDeleteAsync
 功能: 异步删除一条电话本记录
 参数:
              entry_id [in]                     要删除的记录所在存储设备中的序号id
              storage_id  [in]                 要删除的记录所在的存储设备id
              lpAccessCallBack [in]        回调函数，在记录操作成功或失败后调用

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败
                  

=============================================================*/
WREPB_ERROR_E ADAPT_PBDeleteAsync(uint32 entry_id, uint8 storage_id, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle);

/*=============================================================
 函数名称 : PBModify
 功能:   修改一条电话本记录
 参数:
                entry_id [in]                 要修改的记录所在存储设备中的序号Id
                storage_id  [in]             要修改的记录所在的存储设备Id
                lpvCard  [in]                 要修改的vCard记录的内容

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBModify(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize);

/*=============================================================
 函数名称 : PBModifyAsync
 功能: 异步修改一条电话本记录
 参数:
                entry_id   [in]                         要修改的记录所在存储设备中的序号Id
                storage_id  [in]                       要修改的记录所在的存储设备Id
                lpvCard  [in]                           要修改的vCard记录的内容
                lpAccessCallBack [in]               回调函数，在记录操作成功或失败后调用

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBModifyAsync(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle);

/*=============================================================
 函数名称 : PBCopy
 功能:拷贝一条电话本记录
 参数:
                entry_id  [in]                            要拷贝的记录所在存储设备中的序号Id
                storage_id_to [in]                     要拷贝的记录的目的存储设备Id
                storage_id _from [in]                要拷贝的记录的源存储设备Id

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBCopy(uint32 entry_id, uint8 storage_id_to, uint8 storage_id_from);

/*=============================================================
 函数名称 : PBCopyAsync
 功能:  异步拷贝一条电话记录
 参数:
                entry_id [int]                   要拷贝的记录所在存储设备中的序号Id
                storage_id_to [in]            要拷贝的记录的目的存储设备Id
                storage_id _from [in]        要拷贝的记录的源存储设备Id
                lpAccessCallBack[in]         回调函数，在记录操作成功或失败后调用

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBCopyAsync(uint32 entry_id, uint8 storage_id_to, uint8 storage_id_from,  WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack);

/*=============================================================
 函数名称 : PBClear
 功能: 清空特定存储区的电话本记录
 参数:
                storage_id [in]         要清空电话本记录的存储设备的id
                
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBClear(uint8 storage_id);

/*=============================================================
 函数名称 : PBFindFirst
 功能: 搜索电话本记录
 参数:
                search_str [in]     要搜索的姓名或者电话号码
                storage_id [in]     要搜索的存储设备
                lpFindData [in]     查找到的结果，

 返回: 搜索句柄，用于遍历搜索结果

=============================================================*/
WREPB_HANDLE ADAPT_PBFindFirst(uint16 *search_str, uint8 storage_id, WREPB_FIND_DATA *lpFindData);

/*=============================================================
 函数名称 : PBFindNext
 功能: 遍历查找结果
 参数:
                pbHandle [in]           搜索句柄
                lpFindData[out]        搜索结果

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBFindNext(WREPB_HANDLE pbHandle, WREPB_FIND_DATA *lpFindData);

/*=============================================================
 函数名称 : PBFindClose
 功能: 结束查找
 参数:
                pbHandle [in]  搜索句柄
                
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBFindClose(WREPB_HANDLE pbHandle);


WREPB_ERROR_E ADAPT_PBContactToVcard(MMIPB_CONTACT_T *contact_ptr,  //[IN] 
                                                                             uint32          data_max_size,//[IN]
                                                                             uint8 *data_ptr,//[OUT]
                                                                             uint32 *data_len_ptr//[OUT]
                                                                            );

WREPB_ERROR_E ADAPT_PBVcardToContact(uint8 *data_ptr,//[IN]
                                                                             uint32  data_len,//[IN]
                                                                             MMIPB_CONTACT_T *contact_ptr//[OUT]
                                                                             );

uint8 ADAPT_StorageIDWRE2Mocor(uint8 storage_id);
uint8 ADAPT_StorageIDMocor2WRE(uint8 storage_id);

WREPB_ERROR_E ADAPT_MocorError2WREError(MMIPB_ERROR_E error);

#endif
