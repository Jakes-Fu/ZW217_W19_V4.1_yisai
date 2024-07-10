/****************************************************************************
** File Name:      srvsample_export.h                                        *
** Author:                                                                 *
** Date:                                                        *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the mmi cc internal api   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
**                 Create
****************************************************************************/
#ifndef  _SRVSAMPLE_EXPORT_H_
#define  _SRVSAMPLE_EXPORT_H_

//#include "srvsample_define.h"
#include "mmk_type.h"
#include "sci_types.h"
//#include "mmi_nv.h"

#define SRVSAMPLE_MAX_RECORD_NUM 20

//nv item 
typedef struct
{
    uint16 age;
    BOOLEAN sex;
} SRVSAMPLE_PERSON_ITEM_DATA_T;

//nv data
typedef struct
{
    SRVSAMPLE_PERSON_ITEM_DATA_T person_item[SRVSAMPLE_MAX_RECORD_NUM];
} SRVSAMPLE_PERSON_DATA_T;

typedef enum
{
   SRV_OPERATION_READ,
   SRV_OPERATION_WRITE,
   SRV_OPERATION_COPY,

   SRV_OPERATION_MAX
}SRV_OPERATION_ENUM_E;

typedef struct
{
    BOOLEAN result;                     /* Result of the operation: TRUE: success; FALSE: failed */
    SRV_OPERATION_ENUM_E operation;     /* indicate the operation (read/save/Delete...)*/
    void *opreation_cb;                 /* Callback data*/
    void *user_data;                    /* User data */
} SRV_CALLBACK_STRUCT_T;

typedef BOOLEAN (*SrvCallbackFunc)(SRV_CALLBACK_STRUCT_T* callback_data);

typedef struct
{
    SRV_CALLBACK_STRUCT_T oper_cb_data;
    SrvCallbackFunc       callback_func;

}SRV_OPERATION_DATA_T;


typedef struct SAMPLE_SRV_HANDLE_TAG* SAMPLE_HANDLE_T;


//Data Srv

PUBLIC void SAMPLESRV_Init(void);

//Sync
//Read one
PUBLIC BOOLEAN SAMPLESRV_ReadOnebyIndex(uint16 index,
                                        SRVSAMPLE_PERSON_ITEM_DATA_T *oneitem_ptr);
//Edit One
PUBLIC BOOLEAN SAMPLESRV_EditOnebyIndex(uint16 index,
                                        SRVSAMPLE_PERSON_ITEM_DATA_T *oneitem_ptr);
//Edit One age
PUBLIC BOOLEAN SAMPLESRV_EditOneAgebyIndex(uint16 index,uint16 age);
//Delete One
PUBLIC BOOLEAN SAMPLESRV_DeleteOnebyIndex(uint16 index);
//Read all
PUBLIC uint16 SAMPLESRV_ReadAll(uint16 *output_array, uint16 array_index);
//Read Age by input index
PUBLIC BOOLEAN SAMPLESRV_ReadAgebyIndex(uint16 index, uint16 *age);


//Aysn
PUBLIC SAMPLE_HANDLE_T SAMPLESRV_AysnReadOnebyIndex(
    uint32 index,
    SrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);

PUBLIC BOOLEAN SAMPLESRV_AysnDeleteOnebyIndex(uint16 index);


//Process Srv
PUBLIC BOOLEAN SrvSample_ChangeAgeAsyn(void *srv_handle,uint16 index,uint16 age);

PUBLIC void* SrvSample_CreateSrv(void);

PUBLIC void SrvSample_FreeSrv(void *srv_handle);


#endif  //_SRVSAMPLE_EXPORT_H_
