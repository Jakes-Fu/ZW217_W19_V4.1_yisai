#ifdef WRE_SUPPORT

/******************************************************************************
 ** File Name:     ADAPT_pb.c                                          *
 ** Author:         qingjun.yu                                              *
 ** DATE:           02/29/2012                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file wraps phonebook-layer for mocor os			      *
 **																              *
 ******************************************************************************/
 
#include "os_api.h"
#include "mmk_kbd.h"
#include "mmk_app.h"
#include "tasks_id.h"
#include "guilcd.h"
#include "guiblock.h"
#include "mmi_module.h"
#include "mmitheme_update.h"
#include "dal_time.h"
#include "watchdog.h"
#include "scm_api.h"
#include "mmiwre_other.h"
#include "cfl_dlmalloc.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "app_tcp_if.h"
#include "sfs.h"
#include "sig_code.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmiwre_text.h"  
#include "mmk_tp.h"
#include "mmiwre.h"
#include "mmibt_export.h"
#include "mmk_msg.h"
#include "mmiwre_export.h"
#include "mmiwre_adaptor.h" 
#include "mmiwre_anim.h"   
#include "mmiwclk_export.h"
#include "mmiwre_nv.h"
#include "priority_app.h"
#include "mmiwre_pb.h"
#include "mmipb_export.h"
#include "mmipb_app.h"

const WREPB_STORAGE_TYPE s_wre_pbStorage[] =
{
    {WREPB_STORAGE_SIM1, L"SIM0"},             //SIM卡1
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE

    {WREPB_STORAGE_SIM2, L"SIM1"},             //SIM卡2
    
    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    {WREPB_STORAGE_SIM3, L"SIM2"},             //SIM卡3
    #endif
    
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
    {WREPB_STORAGE_SIM4, L"SIM3"},             //SIM卡4
    #endif
    
#endif

    {WREPB_STORAGE_PHONE, L"PHONE"}
};

#define MAX_WRE_PBOPC_NUM           20

PB_OPC s_wre_PbOPC_TBL[MAX_WRE_PBOPC_NUM] = {0};

LPWREPB_FIND_HANDLE s_wre_hFindHandle = NULL;
WREPB_ACCESS_CALLBACK_FUNC s_wre_access_callback = NULL;
//extern MMIPB_HANDLE_T MMIAPIPB_CreateHandle(void);    // changed by yifei

static void PbTbl_Zero()
{
    memset(s_wre_PbOPC_TBL, 0, sizeof(s_wre_PbOPC_TBL));
}

static int PbTbl_Add(LPPB_OPC lpPbOpc)
{
    int i = 0;
    if (lpPbOpc == NULL)
    {
        return -1;
    }

    for (; i < MAX_WRE_PBOPC_NUM; i++)
    {
        if (s_wre_PbOPC_TBL[i].handle == 0)
        {
            break;
        }
    }

    if (i == MAX_WRE_PBOPC_NUM)
    {
        return -1;
    }
    else
    {
        memcpy(&(s_wre_PbOPC_TBL[i]), lpPbOpc, sizeof(PB_OPC));
        return i;
    }
}

static void PbTbl_Del(LPPB_OPC lpPbOpc)
{
    int i = 0;
    if (lpPbOpc->handle == 0)
    {
        return ;
    }
    
    for (; i < MAX_WRE_PBOPC_NUM; i++)
    {
        if (s_wre_PbOPC_TBL[i].handle == lpPbOpc->handle)
        {
            memset(&(s_wre_PbOPC_TBL[i]), 0, sizeof(PB_OPC));
        }
    }
}

static LPPB_OPC PbTbl_Find(uint32 handle)
{
    int i = 0;
    if (handle == 0)
    {
        return NULL;
    }
    
    for (; i < MAX_WRE_PBOPC_NUM; i++)
    {
        if (s_wre_PbOPC_TBL[i].handle == handle)
        {
            return &(s_wre_PbOPC_TBL[i]);
        }
    }

    return NULL;
}

//extern void  MMIAPIPB_CloseHandle(MMIPB_HANDLE_T handle); // changed by yifei

void ADAPT_PBCallback (MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    WREPB_ERROR_E err = (WREPB_ERROR_E)error;
    MMIAPIPB_CloseHandle(handle);
    
    if (s_wre_access_callback)
    {          
        WREPB_ACCESS_CALLBACK_FUNC func = s_wre_access_callback;
        LPPB_OPC lpOpc = PbTbl_Find(handle);
        if (lpOpc && func)
        {
            func(err, lpOpc->dwFlag, lpOpc);
            PbTbl_Del(lpOpc);
        }        
    }
}

/*=============================================================
 函数名称 : PBInit
 功能 : 初始化电话本
 参数:无
 返回 : PB_ERROR_SUCCESS: 初始化成功
               其他:失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBInit()
{ 
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        if (FALSE == MMIAPIPB_IsPbReady())
        {
            bRes = MMIPB_ERROR_ERROR;
        }
        else
        {
            PbTbl_Zero();
        }

        return ADAPT_MocorError2WREError(bRes);
}

/*=============================================================
 函数名称 : PBUninit
 功能 : 初始化电话本
 参数:无
 返回 : PB_ERROR_SUCCESS: 初始化成功
               其他:失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBUninit()
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;

        PbTbl_Zero();

        return ADAPT_MocorError2WREError(bRes);
}

/*=============================================================
 函数名称 : PBEnumStorage
 功能 : 枚举电话本存储设备
 参数:
             lpPBStorageAttri [out]   存储设备的属性
             nPBStorageNum [out]   存储设备的个数

 返回 : PB_ERROR_SUCCESS 表示成功
               其他: 失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBEnumStorage(WREPB_STORAGE_TYPE *lpPBStorageType, uint8 *nPBStorageNum)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;

        *nPBStorageNum = sizeof(s_wre_pbStorage)/sizeof(WREPB_STORAGE_TYPE);
        if (lpPBStorageType != PNULL)
        {
            memcpy(lpPBStorageType, s_wre_pbStorage, sizeof(s_wre_pbStorage));
        }

        return ADAPT_MocorError2WREError(bRes);
}

/*=============================================================
 函数名称 : PBGetStorageAttri
 功能 : 获取电话本存储设备的属性
 参数:
              storage_id  [in]                存储设备ID
              lpPBStorageAttri [out]      存储设备的属性
              
 返回 : PB_ERROR_SUCCESS 表示成功
               其他:失败
=============================================================*/
WREPB_ERROR_E ADAPT_PBGetStorageAttri(uint8 storage_id, WREPB_STORAGE_ATTRI *lpPBStorageAttri)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        
        if (lpPBStorageAttri == PNULL || storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {

            bRes = MMIPB_ERROR_INVALID_PARAM;
        }
        else
        {
             MMIPB_STORAGE_INFO_T storage_info = {0};
             uint8 MocorStorage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
             
             memcpy(&(lpPBStorageAttri->storageType), &(s_wre_pbStorage[storage_id]), sizeof(WREPB_STORAGE_TYPE));
             storage_info =  MMIPB_GetContactStorageInfo(MocorStorage_id);

            lpPBStorageAttri->dwMaxSize = storage_info.max_record_num;
            lpPBStorageAttri->dwUsedSize= storage_info.used_record_num;
            lpPBStorageAttri->dwAvailSize = storage_info.max_record_num-storage_info.used_record_num;
        }

        return ADAPT_MocorError2WREError(bRes);
}


/*=============================================================
 函数名称 : PBGetFreeContactId
 功能: 获取特定电话本存储设备中联系人可用ID
 参数: 
              storage_id [in]   存储设备的ID

 返回: 存储设备中可用的联系人ID
              
=============================================================*/
DWORD ADAPT_PBGetFreeContactId(uint8 storage_id)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        if (storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            return -1; /*lint !e570*/
        }
        else
        {
            uint8 MocorStorage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
            return MMIAPIPB_GetFreeContactId(MocorStorage_id);
        }        
}

/*=============================================================
 函数名称 : PBGet
 功能:  获取一条电话本记录
 参数: 
               entry_id [in]                      要获取的记录所在存储设备中的序号id
               storage_id  [in]                  要获取的记录所在的存储设备id
               dwMaxSize [in]                   缓存区大小
               lpvCard [in][out]                指向一条vCard记录的指针
               dwvCardSize [in][out]         该条vCard记录的大小               
   
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBRead(uint32 entry_id, uint8 storage_id, uint32 dwMaxSize, char *lpvCard, uint32 *dwvCardSize)
{
    MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
    MMIPB_CONTACT_T contact = {0};
    uint8 MocorStorage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
#if 0    
    if (s_wre_access_callback != NULL)
    {
        bRes = MMIPB_ERROR_BUSY;
        
        return ADAPT_MocorError2WREError(bRes);
    }
#endif
    bRes = MMIAPIPB_GetContactInfo(entry_id, MocorStorage_id, &contact);
    if (bRes != MMIPB_ERROR_SUCCESS)
    {
        return ADAPT_MocorError2WREError(bRes);        
    }

    return ADAPT_PBContactToVcard(&contact, dwMaxSize, (uint8 *)lpvCard, dwvCardSize);
}


/*=============================================================
 函数名称 : PBAdd
 功能:  添加一条电话本记录
 参数: 
              lpvCard [in]             指向一条vCard记录的指针
              dwvCardSize [int]     该条vCard记录的大小
   
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBAdd(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize)
{
        WREPB_ERROR_E bRes = WREPB_ERROR_SUCCESS;       
        MMIPB_CONTACT_T contact = {0};
        if (lpvCard == NULL || dwvCardSize == 0)
        {
            bRes = WREPB_ERROR_INVALID_PARAM;

            return bRes;
        }
        
        bRes = ADAPT_PBVcardToContact((uint8 *)lpvCard, dwvCardSize, &contact);
        contact.storage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
        if (bRes == WREPB_ERROR_SUCCESS)
        {
            MMIPB_ERROR_E pbError = MMIAPIPB_AddContact(&contact);
            bRes = ADAPT_MocorError2WREError(pbError);
        }

        return bRes;
}


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
WREPB_ERROR_E ADAPT_PBAddAsync(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle)
{
        WREPB_ERROR_E bRes = WREPB_ERROR_SUCCESS;        
        MMIPB_CONTACT_T contact = {0};
#if 0
        if (s_wre_access_callback != NULL)
        {
             bRes = MMIPB_ERROR_BUSY;

             return ADAPT_MocorError2WREError(bRes);
        }
#endif        
        if (lpvCard == NULL || dwvCardSize == 0)
        {
            bRes = WREPB_ERROR_INVALID_PARAM;

            return bRes;
        }

        bRes = ADAPT_PBVcardToContact((uint8 *)lpvCard, dwvCardSize, &contact);
        contact.storage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
        if (bRes == WREPB_ERROR_SUCCESS)
        {
            DWORD dwEntryId = ADAPT_PBGetFreeContactId(storage_id);
            MMIPB_HANDLE_T handle = MMIAPIPB_CreateHandle();
            MMIPB_ERROR_E pbError = MMIAPIPB_AddContactAsyn(handle, &contact, ADAPT_PBCallback);
            PB_OPC pbOpc = {0};
            if (pbError == MMIPB_ERROR_SUCCESS || pbError == MMIPB_ERROR_PENDING)
            {
                s_wre_access_callback = lpAccessCallBack;
                pbOpc.handle = handle;
                pbOpc.entry_id = dwEntryId;
                pbOpc.storage_id = storage_id;
                pbOpc.dwAction = PB_OPC_ADD;
                pbOpc.dwFlag = dwHandle;
                PbTbl_Add(&pbOpc);
            }

            bRes = ADAPT_MocorError2WREError(pbError);
        }

        return bRes;
}


/*=============================================================
 函数名称 : PBDelete
 功能:  删除一条电话本记录
 参数:
               entry_id [in]          要删除的记录所在存储设备中的序号id
               storage_id  [in]      要删除的记录所在的存储设备id

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败


=============================================================*/
WREPB_ERROR_E ADAPT_PBDelete(uint32 entry_id, uint8 storage_id)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;       
        uint8 MocorStorage_id = 0;

        if (storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;
        }
        else
        {
            MocorStorage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
            bRes = MMIAPIPB_DeleteContact(entry_id, MocorStorage_id);
        }        
        
        return ADAPT_MocorError2WREError(bRes);
}


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
WREPB_ERROR_E ADAPT_PBDeleteAsync(uint32 entry_id, uint8 storage_id, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;       
        uint8 MocorStorage_id = 0;
        MMIPB_HANDLE_T handle = NULL;
        PB_OPC pbOpc = {0};
#if 0
        if (s_wre_access_callback != NULL)
        {
             bRes = MMIPB_ERROR_BUSY;

             return ADAPT_MocorError2WREError(bRes);
        }
#endif
        if (storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;

            return ADAPT_MocorError2WREError(bRes);
        }

        s_wre_access_callback = lpAccessCallBack;
        handle = MMIAPIPB_CreateHandle();
        MocorStorage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
        bRes = MMIAPIPB_DeleteContactAsyn(handle, entry_id, MocorStorage_id, ADAPT_PBCallback);

        if (bRes == MMIPB_ERROR_SUCCESS || bRes == MMIPB_ERROR_PENDING)
        {
            pbOpc.handle = handle;
            pbOpc.entry_id = entry_id;
            pbOpc.storage_id = storage_id;
            pbOpc.dwAction = PB_OPC_DEL;
            pbOpc.dwFlag = dwHandle;
            PbTbl_Add(&pbOpc);
        }
        
        return ADAPT_MocorError2WREError(bRes);
}


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
WREPB_ERROR_E ADAPT_PBModify(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize)
{
        WREPB_ERROR_E bRes = WREPB_ERROR_SUCCESS;       
        MMIPB_CONTACT_T contact = {0};
        if (storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            bRes = WREPB_ERROR_INVALID_PARAM;
        }
        else
        {
            bRes = ADAPT_PBVcardToContact((uint8 *)lpvCard, dwvCardSize, &contact);
            if (bRes == WREPB_ERROR_SUCCESS)
            {
                MMIPB_ERROR_E err = MMIPB_ERROR_SUCCESS;
                contact.storage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
                contact.contact_id = entry_id;
                err = MMIAPIPB_ModifyContact(&contact);            
                bRes = ADAPT_MocorError2WREError(err);
            }

        }

        return bRes;
}


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
WREPB_ERROR_E ADAPT_PBModifyAsync(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle)
{
        WREPB_ERROR_E bRes = WREPB_ERROR_SUCCESS;       
        MMIPB_CONTACT_T contact = {0};
#if 0
        if (s_wre_access_callback != NULL)
        {
             bRes = MMIPB_ERROR_BUSY;

             return ADAPT_MocorError2WREError(bRes);
        }
#endif
        if (storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            bRes = WREPB_ERROR_INVALID_PARAM;

            return bRes;
        }
        
        bRes = ADAPT_PBVcardToContact((uint8 *)lpvCard, dwvCardSize, &contact);
        if (bRes == WREPB_ERROR_SUCCESS)
        {
            PB_OPC pbOpc = {0};
            MMIPB_HANDLE_T handle = MMIAPIPB_CreateHandle();
            MMIPB_ERROR_E err = MMIPB_ERROR_SUCCESS;

            s_wre_access_callback = lpAccessCallBack;
            contact.storage_id = ADAPT_StorageIDWRE2Mocor(storage_id);
            contact.contact_id = entry_id;
            err = MMIAPIPB_ModifyContactAsyn(handle, &contact, ADAPT_PBCallback);

            if (err == MMIPB_ERROR_SUCCESS || err == MMIPB_ERROR_PENDING)
            {
                pbOpc.handle = handle;
                pbOpc.entry_id = entry_id;
                pbOpc.storage_id = storage_id;
                pbOpc.dwAction = PB_OPC_UPDATE;
                pbOpc.dwFlag = dwHandle;
                PbTbl_Add(&pbOpc);                
            }

            bRes = ADAPT_MocorError2WREError(err);
        }

        return bRes;
}


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
WREPB_ERROR_E ADAPT_PBCopy(uint32 entry_id, uint8 storage_id_to, uint8 storage_id_from)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        uint8 MocorStorage_id_to = 0;
        uint8 MocorStorage_id_from = 0;

        if (storage_id_to < WREPB_STORAGE_SIM1 || storage_id_to > WREPB_STORAGE_PHONE /*lint !e685*//*lint !e568*/
            || storage_id_from < WREPB_STORAGE_SIM1 || storage_id_from > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;
        }
        else
        {
            MocorStorage_id_to = ADAPT_StorageIDWRE2Mocor(storage_id_to);
            MocorStorage_id_from = ADAPT_StorageIDWRE2Mocor(storage_id_from);
            bRes = MMIAPIPB_CopyContact(storage_id_to, entry_id, storage_id_from);
        }        

        return ADAPT_MocorError2WREError(bRes);
}


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
WREPB_ERROR_E ADAPT_PBCopyAsync(uint32 entry_id, uint8 storage_id_to, uint8 storage_id_from,  WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        uint8 MocorStorage_id_to = 0;
        uint8 MocorStorage_id_from = 0;
        MMIPB_HANDLE_T handle = NULL;
#if 0
        if (s_wre_access_callback != NULL)
        {
             bRes = MMIPB_ERROR_BUSY;

             return ADAPT_MocorError2WREError(bRes);
        }
#endif
        if (storage_id_to < WREPB_STORAGE_SIM1 || storage_id_to > WREPB_STORAGE_PHONE /*lint !e685*//*lint !e568*/
            || storage_id_from < WREPB_STORAGE_SIM1 || storage_id_from > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;

            return ADAPT_MocorError2WREError(bRes);
        }

        //handle = MMIAPIPB_CreateHandle();

        MocorStorage_id_to = ADAPT_StorageIDWRE2Mocor(storage_id_to);
        MocorStorage_id_from = ADAPT_StorageIDWRE2Mocor(storage_id_from);
        s_wre_access_callback = lpAccessCallBack;
        //bRes = MMIPB_CopyOneContactAsyn(handle, MocorStorage_id_to, entry_id, MocorStorage_id_from);

        return ADAPT_MocorError2WREError(bRes);
}


/*=============================================================
 函数名称 : PBClear
 功能: 清空特定存储区的电话本记录
 参数:
                storage_id [in]         要清空电话本记录的存储设备的id
                
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBClear(uint8 storage_id)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;

        if (storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;
        }
        else
        {
            if (storage_id == WREPB_STORAGE_PHONE)
            {
                MMIAPIPB_DeleteAllPBInfo();
            }
            else
            {
                bRes = MMIPB_ERROR_ERROR;
            }
        }

        return ADAPT_MocorError2WREError(bRes);
}


/*=============================================================
 函数名称 : PBFindFirst
 功能: 搜索电话本记录
 参数:
                search_str [in]     要搜索的姓名或者电话号码
                storage_id [in]     要搜索的存储设备
                lpFindData [in]     查找到的结果，

 返回: 搜索句柄，用于遍历搜索结果

=============================================================*/
WREPB_HANDLE ADAPT_PBFindFirst(wchar *search_str, uint8 storage_id, WREPB_FIND_DATA *lpFindData)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        
        if (lpFindData == NULL|| storage_id < WREPB_STORAGE_SIM1  /*lint !e685*//*lint !e568*/
            || storage_id > WREPB_STORAGE_PHONE ) /*lint !e685*//*lint !e568*/
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;
        }
        else
        {
            s_wre_hFindHandle = (LPWREPB_FIND_HANDLE)SCI_ALLOC_APP(sizeof(WREPB_FIND_HANDLE));
            if (s_wre_hFindHandle == NULL)
            {
                bRes = MMIPB_ERROR_ERROR;
            }
            else
            {
                uint16 count = 0;
                uint32 entry_id = 0;
                uint8  e_storage_id = 0;
                MMI_STRING_T mmistr = {0};
                mmistr.wstr_ptr = search_str;
                mmistr.wstr_len = MMIAPICOM_Wstrlen(search_str);
                
                memset(s_wre_hFindHandle, 0, sizeof(WREPB_FIND_HANDLE));
                s_wre_hFindHandle->handle = MMIAPIPB_SearchList(&mmistr, MMIPB_LIST_FIELD_WHOLE, &count);
                if (s_wre_hFindHandle->handle == 0)
                {
                    bRes = MMIPB_ERROR_ERROR;
                    SCI_FREE(s_wre_hFindHandle);
                    s_wre_hFindHandle = NULL;	// changed by yifei
                }
                else
                {
                      if (MMIAPIPB_GetSearchEntryID(s_wre_hFindHandle->handle, s_wre_hFindHandle->curIndex, \
                                                                    (uint16 *)(&entry_id), (uint16 *)(&e_storage_id)))
                      {
                          lpFindData->entry_id = entry_id;
                          lpFindData->storage_id = ADAPT_StorageIDMocor2WRE(e_storage_id);
                          s_wre_hFindHandle->curIndex = 0;  // changed by yifei
                      }
                }                
            }                        
        }

        return (uint32)s_wre_hFindHandle;
}


/*=============================================================
 函数名称 : PBFindNext
 功能: 遍历查找结果
 参数:
                pbHandle [in]           搜索句柄
                lpFindData[out]        搜索结果

 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBFindNext(WREPB_HANDLE pbHandle, WREPB_FIND_DATA *lpFindData)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        if ((WREPB_HANDLE)s_wre_hFindHandle != pbHandle || lpFindData == NULL)
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;
        }
        else
        {
            if (s_wre_hFindHandle->handle == NULL)
            {
                bRes = MMIPB_ERROR_INVALID_PARAM;
            }
            else
            {
                uint32 entry_id = 0;
                uint8 storage_id = 0;
                BOOLEAN bSearch = FALSE; 

                s_wre_hFindHandle->curIndex++;

                bSearch = MMIAPIPB_GetSearchEntryID((MMIPB_HANDLE_T)s_wre_hFindHandle->handle, s_wre_hFindHandle->curIndex, \
                                                            (uint16 *)(&entry_id), (uint16 *)(&storage_id));
                if (bSearch)
                {
                    lpFindData->entry_id = entry_id;
                    lpFindData->storage_id = ADAPT_StorageIDMocor2WRE(storage_id);
                    bRes = MMIPB_ERROR_SUCCESS;
                }
                else
                {
                    bRes = MMIPB_ERROR_ERROR;
                }
            }
        }


        return ADAPT_MocorError2WREError(bRes);
}


/*=============================================================
 函数名称 : PBFindClose
 功能: 结束查找
 参数:
                pbHandle [in]  搜索句柄
                
 返回:  PB_ERROR_SUCCESS 表示成功
               其他表示失败

=============================================================*/
WREPB_ERROR_E ADAPT_PBFindClose(WREPB_HANDLE pbHandle)
{
        MMIPB_ERROR_E bRes = MMIPB_ERROR_SUCCESS;
        if ((WREPB_HANDLE)s_wre_hFindHandle != pbHandle || s_wre_hFindHandle == NULL)
        {
            bRes = MMIPB_ERROR_INVALID_PARAM;
        }

        MMIAPIPB_SearchExit(s_wre_hFindHandle->handle);
        s_wre_hFindHandle->handle = NULL;
        memset(s_wre_hFindHandle, 0, sizeof(WREPB_FIND_HANDLE));
        SCI_FREE(s_wre_hFindHandle);
        s_wre_hFindHandle = NULL;

        return ADAPT_MocorError2WREError(bRes);
}

WREPB_ERROR_E ADAPT_PBContactToVcard(MMIPB_CONTACT_T *contact_ptr,  //[IN] 
                                      uint32 data_max_size,//[IN]
                                      uint8  *data_ptr,//[OUT]
                                      uint32 *data_len_ptr//[OUT]
                                     )
{
	MMIPB_ERROR_E error;
	
 #ifdef  MMI_VCARD_SUPPORT
    error = MMIAPIPB_ContactToVcard(contact_ptr, data_max_size, data_ptr, data_len_ptr);
 #endif
 
    return ADAPT_MocorError2WREError(error);
}


WREPB_ERROR_E ADAPT_PBVcardToContact(uint8 *data_ptr,//[IN]
                                     uint32  data_len,//[IN]
                                     MMIPB_CONTACT_T *contact_ptr//[OUT]
                                     )
{
#ifdef MMI_VCARD_SUPPORT     
    MMIPB_ERROR_E error = MMIAPIPB_VcardToContact(data_ptr, data_len, contact_ptr);

    return ADAPT_MocorError2WREError(error);
#else
   return MMIPB_ERROR_ERROR;
#endif    
}


uint8 ADAPT_StorageIDWRE2Mocor(uint8 storage_id)
{
    uint8  dual_sys = storage_id;
    uint8  storage_type = 0;
    
    if (storage_id < WREPB_STORAGE_SIM1 || storage_id > WREPB_STORAGE_PHONE) /*lint !e685*//*lint !e568*/
    {
        return -1; /*lint !e570*/
    }

    if (storage_id == WREPB_STORAGE_PHONE)
    {
        storage_type = PHONEBOOK_NV_STORAGE;
    }
    else
    {
        storage_type = PHONEBOOK_SIM_STORAGE;
    }

    return (uint8)MMIAPIPB_GetStorageId((MN_DUAL_SYS_E)dual_sys, (PHONEBOOK_STORAGE_E)storage_type);
}

uint8 ADAPT_StorageIDMocor2WRE(uint8 storage_id)
{       
    if (MMIPB_IsInNvGroupByStorage(storage_id))
    {
        return WREPB_STORAGE_PHONE;
    }
    else
    {
        MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
        dual_sys = MMIPB_GetDualSysByStorageId(storage_id);

        return dual_sys;
    }    
}

WREPB_ERROR_E ADAPT_MocorError2WREError(MMIPB_ERROR_E error)
{
    WREPB_ERROR_E PBError = WREPB_ERROR_SUCCESS;
    PBError = (WREPB_ERROR_E)error;
    
#if 0    
    switch (error)
    {
    case MMIPB_ERROR_ERROR:
        PBError = WREPB_ERROR_ERROR;
        break;
    case MMIPB_ERROR_PENDING:
        PBError = WREPB_ERROR_PENDING;
        break;
    case MMIPB_ERROR_BUSY:
        PBError = WREPB_ERROR_BUSY;
        break;
    case MMIPB_ERROR_FILE_NOT_EXIST:
        PBError = WREPB_ERROR_FILE_NOT_EXIST;
        break;
    case MMIPB_ERROR_INVALID_PARAM:
        PBError = WREPB_ERROR_INVALID_PARAM;
        break;
    case MMIPB_ERROR_INVALID_FORMAT:
        PBError = WREPB_ERROR_INVALID_FORMAT;
        break;
    case MMIPB_ERROR_PART_SUCCESS:
        PBError = WREPB_ERROR_PART_SUCCESS;
        break;
    case MMIPB_ERROR_NO_INITIAL:
        PBError = WREPB_ERROR_NO_INITIAL;
        break;
    case MMIPB_ERROR_NO_ENTRY:
        PBError = WREPB_ERROR_NO_ENTRY;
        break;
    case MMIPB_ERROR_NO_NUMBER:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_NO_MSISDN:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_BLACKLIST_FULL:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_NUMBER_ALERDY_EXIST:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_MAIL_LEN:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_SIM_ERROR:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_FDN_ENABLE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_UDISK_ISRUN:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_NO_DEV:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_SFS_ERROR:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_SMS_TOO_LARGE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_INVALID_NUMBER:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_NO_SPACE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_PHONE_NO_SPACE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_SIM1_NO_SPACE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_SIM2_NO_SPACE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_SIM3_NO_SPACE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_SIM4_NO_SPACE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_FMM_NO_SPACE:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    case MMIPB_ERROR_CANCEL:
        PBError = WREPB_ERROR_NO_NUMBER;
        break;
    default:
        break;
    }
#endif        

    return (WREPB_ERROR_E)PBError;
}

#endif
