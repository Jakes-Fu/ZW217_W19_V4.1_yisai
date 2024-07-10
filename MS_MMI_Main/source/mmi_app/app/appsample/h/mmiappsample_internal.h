/*****************************************************************************
 ** File Name:   mmiappsample_internal.h
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe internal header
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/

#ifndef  _MMIAMMSAMPLE_INTERNAL_H_
#define  _MMIAMMSAMPLE_INTERNAL_H_

#include "mmiappsample_define.h"
#include "mmk_type.h"
#include "sci_types.h"
#include "mmi_nv.h"
#include "srvsample_export.h"
#include "os_api.h"


typedef enum
{
    VALUE_BIGGER,
    VALUE_EQUAL,
    VALUE_SMALLER,
} VALUE_FILTER_E;

typedef struct
{
    BOOLEAN isActived;
    VALUE_FILTER_E valueFilter;
    uint16  value;
} FilterItem_T;

typedef struct
{
    FilterItem_T  agefilter;
    FilterItem_T  sexfilter;
} MMIAPPSAMPLE_FILTER_T;

typedef void (*FUNCWINCALLBACK)(MMI_HANDLE_T win_handle, uint16 sel_index);

#define MMIAPPSAMPLE_TRACE SCI_TRACE_LOW

/*****************************************************************************/
//  Description : Register sample app menu group
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPPSAMPLE_RegMenuGroup(void);

/*****************************************************************************/
//  Description : register set module nv len and max item
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPPSAMPLE_RegNv(void);
/*****************************************************************************/
//  Description : Register app sample win and win name
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPPSAMPLE_RegWinIdNameArr(void);

//FuncWin Enter APIS
/******************************************************************************
//  Description : Entry sample app
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleMainWin(CAF_HANDLE_T app_handle);

/******************************************************************************
//  Description : filter win entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFilterWin(void);

/******************************************************************************
//  Description : windata funcwin entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleWindataWin(MMIAPPSAMPLE_FILTER_T filter);

/******************************************************************************
//  Description : event win entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleEventWin(void);


//FuncWin Sample Entrys
/******************************************************************************
//  Description : FuncWin list Entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFuncWinListWin(void);
/******************************************************************************
//  Description : FuncWin A Entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFuncWinModeAWin(void);
/******************************************************************************
//  Description : FuncWin B Entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFuncWinModeBWin(uint16 sel_index);
/******************************************************************************
//  Description : FuncWin C Entry
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC void EnterAppSampleFuncWinModeCWin(MMI_HANDLE_T src_handle, uint16 sel_index, FUNCWINCALLBACK callback);



//APP Evenet APIS
/********************************************************************
//  Description :sample app data changed event Register
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_RegisterDateChangeEvent(MMI_HANDLE_T win_handle, uint32 index);


/********************************************************************
//  Description :sample app data changed event Unregister
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_UnRegisterDateChangeEvent(uint32 index);

/********************************************************************
//  Description :sample app process event register
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_RegisterProcessEvent(MMI_HANDLE_T win_handle, uint32 index);

/********************************************************************
//  Description :sample app process event unregister
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_UnRegisterProcessEvent(uint32 index);


/********************************************************************
//  Description :sample app error/warning handler
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPPSAMPLE_RaiseError(uint32 errorcode);




#endif  //_MMIAMMSAMPLE_INTERNAL_H_
