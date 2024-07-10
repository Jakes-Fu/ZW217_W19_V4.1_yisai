/****************************************************************************
** File Name:      mmise.h                                                 
** Author:         xuanchen.jiang                                                    
** Date:              	                            
** Description:    
****************************************************************************
**                         Important Edit History                         	 	
** ------------------------------------------------------------------------*
** DATE           	        NAME                                DESCRIPTION                       	
** 2011.05.11        	        xuanchen.jiang                   Created.
** 
****************************************************************************/
#ifndef _MMISE_H_
#define _MMISE_H_ 
 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "sig_code.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif 

/****************************************************************************************
MARCO DEFINITION
*****************************************************************************************/

typedef enum 
{
    MMISE_SIG_BEGIN = (0x4001),
    MMISE_CALC_REQ,
    MMISE_CALC_NOTIFY,
//    MMISE_NOTIFY_IND,
    MMISE_CALC_EXIT,
    MMISE_SIG_END
} MMISE_SIGNALCODE_E;

typedef void    (*calc_cb_func)(uint32 type,void* ptr,uint32 size); 

typedef struct
{
    uint32 calc_type;  
    void*  data_ptr;
    uint32 data_size;
    calc_cb_func calc_func;     /*计算回调函数*/
}MMISE_CALC_PARAM_T;


// ODM_DOWNLOAD_GET_SESSION_LIST_CNF
typedef struct {
	_SIGNAL_VARS		// header part
	uint32 count;
	uint32 session_id_arry[1];	
} MMISE_CALC_MSG_T;

PUBLIC void MMISE_CalculateTaskCreate(void);

PUBLIC void MMISE_CalculateTaskDestroy(void);

PUBLIC void MMISE_StartCalculate(MMISE_CALC_PARAM_T* param_ptr);

PUBLIC void  MMISE_SendMsgToMMI(uint16 MsgId,void* param_ptr,uint32 param_size);

PUBLIC void  MMISE_SendCalculateReq(MMISE_CALC_PARAM_T* calc_param_ptr);

PUBLIC void  MMISE_SendLinearCalculateCnf(void);

PUBLIC void  MMISE_SendPopCalculateCnf(void);

PUBLIC void  MMISE_SendMsgToMMI_For_Livewallpaper(uint16 MsgId,void* param_ptr,uint32 param_size);

/*****************************************************************************/
//  Description : 算法TASK是否存在
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISE_IsCalculateTaskExist(void);

#ifdef   __cplusplus
    }
#endif

#endif //


