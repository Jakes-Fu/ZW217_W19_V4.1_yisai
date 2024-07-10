/******************************************************************************
**  File Name:      mmimtv_nv.c                                               *
**  Author:         xin.li                                                    *
**  Date:           2008/12                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:    This file defines the function about nv                   *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  12/2008       xin.li           Create.                                    *
******************************************************************************/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#ifdef MBBMS_SUPPORT
#ifdef WIN32
#include "sci_types.h"
#include "os_api.h"
#include "mbbms_auth.h"
#include "mbbms_sub.h"
#include "mbbms_sg_common.h"
#include "mbbms_sg_api.h"
#include "mmi_appmsg.h"
#include "mbbms_api_dummy.h"
#include "tasks_id.h"
#include "mua_cmd.h"

LOCAL MBMSMIDWARE_CALLBACK g_sub_callback = PNULL;
LOCAL MBMSMIDWARE_CALLBACK g_gba_callback = PNULL;

LOCAL uint32 g_net_id = 0;

PUBLIC void  MBBMS_SUBSetCallback(MBMSMIDWARE_CALLBACK sub_callback )
{
	g_sub_callback = sub_callback;
}

PUBLIC void  MBBMS_SUBCleanCallback()
{
	g_sub_callback = PNULL;
}
PUBLIC void  MBBMS_GBASetCallback(MBMSMIDWARE_CALLBACK gba_callback )
{
	//SCI_TRACE_LOW:"MBBMS GBA: set callback function\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MBBMS_API_DUMMY_42_112_2_18_2_39_24_0,(uint8*)"");
	g_gba_callback = gba_callback;
}
 
PUBLIC void  MBBMS_GBACleanCallback()
{
	//SCI_TRACE_LOW:"MBBMS GBA: clean callback function\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MBBMS_API_DUMMY_48_112_2_18_2_39_24_1,(uint8*)"");
	g_gba_callback = PNULL;
}
PUBLIC void MBBMS_Set_NetID(uint32 netid)
{
	//SCI_TRACE_LOW:"MBBMS GBA: set net id to [%d]\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MBBMS_API_DUMMY_53_112_2_18_2_39_24_2,(uint8*)"d",netid);
	g_net_id = netid;
}

PUBLIC void MBBMS_Get_NetID(uint32 *netid)
{
	*netid = g_net_id ;
}

PUBLIC BOOLEAN MBBMS_Check_GBA_Ready(void)
{
	return TRUE;
}
PUBLIC BOOLEAN MBBMS_Load_UpdateTable(void)
{
	return TRUE;
}
/*****************************************************************************/
//  Description : gba init
//  Global resource dependence : none
//  Author: lisa lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_GBA_Init(void)
{
	return TRUE;
} 
/*****************************************************************************/
//  Description :set wap address
//  Global resource dependence : none
//  Author: lisa lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Set_WAP(char *wap_addr, uint16 port)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : get wap address
//  Global resource dependence : none
//  Author: lisa lin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Get_WAP(char *wap_addr, uint16 *port)
{
	return TRUE;
}
/*****************************************************************************/
//  Description : set cmmb sn
//  Global resource dependence : none
//  Author: lisa lin 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Set_CMMBSN(char *sn, uint8 len)
{
	return TRUE;
} 
/*****************************************************************************/
//  Description : get cmmbsn
//  Global resource dependence : none
//  Author: lisa lin 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Get_CMMBSN(char *sn, uint8 *len)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : set naf address
//  Global resource dependence : none
//  Author: lisa lin 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Set_NAF(char *naf_addr, uint16 port)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : get naf address
//  Global resource dependence : none
//  Author: lisa lin  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Get_NAF(char *naf_addr, uint16 *port)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : set naf domain name
//  Global resource dependence : none
//  Author: lisa lin   
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Set_NAF_Domain_Name(char *naf_addr)
{
	return TRUE;
}
/*****************************************************************************/
//  Description :get naf domain name
//  Global resource dependence : none
//  Author: lisa lin  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Get_NAF_Domain_Name(char *naf_addr)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : gba get state
//  Global resource dependence : none
//  Author: lisa lin   
//  Note:
/*****************************************************************************/
 
PUBLIC BOOLEAN MBBMS_GBA_Get_State(void)
{
	return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_SIM_Support_CAS(void)
{
	return TRUE;
}

/*****************************************************************************/
//  Description : gba operation
//  Global resource dependence : none
//  Author: lisa lin
//  Note:
/*****************************************************************************/
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_GBA_Operation(void)
{
	MBBMS_GBA_SIG_T  * sendSignal;		
	if (g_gba_callback!=PNULL)
	(*g_gba_callback)((uint32)MBBMS_MMI_GBA_RES,(uint32)GBA_RET_OK,
					NULL);
	else
		{
  	MmiCreateSignal(MBBMS_MMI_GBA_RES,sizeof(MBBMS_GBA_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->rst=MBBMS_SUCCESS;	
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	return GBA_RET_OK;
}

/*****************************************************************************/
//  Description :stop gba
//  Global resource dependence : none
//  Author: lisa lin
//  Note:
/*****************************************************************************/
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_GBA_Stop_Operation(void)
{
	MBBMS_GBA_SIG_T  * sendSignal;		
	if (g_gba_callback!=PNULL)
	(*g_gba_callback)((uint32)MBBMS_MMI_GBA_STOP_RES,(uint32)GBA_RET_OK,
					NULL);
	else
		{
  	MmiCreateSignal(MBBMS_MMI_GBA_STOP_RES,sizeof(MBBMS_GBA_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->rst=MBBMS_SUCCESS;	
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	return GBA_RET_OK;

}

/*****************************************************************************/
//  Description : mrk operation
//  Global resource dependence : none
//  Author: lisa lin
//  Note:
/*****************************************************************************/
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_MRK_Operation(void)
{
	MBBMS_GBA_SIG_T  * sendSignal;		
	if (g_gba_callback!=PNULL)
	(*g_gba_callback)((uint32)MBBMS_MMI_MRK_RES,(uint32)GBA_RET_OK,
					NULL);
	else
		{	
  	MmiCreateSignal(MBBMS_MMI_MRK_RES,sizeof(MBBMS_GBA_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->rst=MBBMS_SUCCESS;	
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	return GBA_RET_OK;

}

/*****************************************************************************/
//  Description : stop mrk
//  Global resource dependence : none
//  Author: lisa lin
//  Note:
/*****************************************************************************/
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_MRK_Stop_Operation(void)
{

	MBBMS_GBA_SIG_T  * sendSignal;		
	if (g_gba_callback!=PNULL)
	(*g_gba_callback)((uint32)MBBMS_MMI_MRK_STOP_RES,(uint32)GBA_RET_OK,
					NULL);
	else
		{
		MmiCreateSignal(MBBMS_MMI_MRK_STOP_RES,sizeof(MBBMS_GBA_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->rst=MBBMS_SUCCESS;	
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	return GBA_RET_OK;

}
			
/*****************************************************************************/
// Description : whether card is support mtv
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
BOOLEAN MUACMD_MBBMS_IsEnabled(void)
{
    return TRUE;
}

/*****************************************************************************/
// Description : whether gba is needed or not
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
BOOLEAN MUACMD_IsGBANeeded(void)
{

     return TRUE;
}
//sub

// init and exit
/*****************************************************************************/
// Description : sub & msk init
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Sub_MSK_Init()
{
	return TRUE;
}
/*****************************************************************************/
// Description : sub and msk exit
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_Sub_MSK_Exit()
{

	return TRUE;
}

//free data structure
/*****************************************************************************/
// Description : MBBMS_Free_Operation_Res_Ptr
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/

PUBLIC void MBBMS_Free_Operation_Res_Ptr(MBBMS_Operation_Res_t *ptr)
	{
		MBBMS_LTKM_List_t *ltkm_ptr = NULL;
		MBBMS_LTKM_List_t *next = NULL; 
		if (ptr == NULL) return;
		ltkm_ptr = ptr->ltkm_ptr;
		while(ltkm_ptr )
			{
			next = ltkm_ptr->next;
			SCI_FREE(ltkm_ptr);
			ltkm_ptr = next;
			}
		SCI_FREE(ptr);
		ptr = NULL;
	}
/*****************************************************************************/
// Description : free service request data structure
// Global resource dependence :
// Author:lisa lin
// Note:  copy from mbbms_auth.c
/*****************************************************************************/
PUBLIC void MBBMS_Free_Service_Request_Ptr(MBBMS_Service_Request_t *ptr)
{

	struct MBBMS_Purchase_Data_Reference_Service_tag *next = NULL;
	struct MBBMS_Purchase_Data_Reference_Service_tag *cur = NULL;
	//SCI_TRACE_LOW:"MBBMS_Free_Service_Request_Ptr ... \n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MBBMS_API_DUMMY_314_112_2_18_2_39_24_3,(uint8*)"");
		if (ptr == NULL) return;
		if (ptr->data_ref_ptr != NULL)
			{
				next = cur = ptr->data_ref_ptr->next;
				while (next != NULL)
					{
					cur = next->next;
					SCI_FREE(next);
					next = cur;
					}
				SCI_FREE(ptr->data_ref_ptr);
			}
		SCI_FREE(ptr);
		ptr = NULL;
	//SCI_TRACE_LOW:"MBBMS_Free_Service_Request_Ptr END \n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MBBMS_API_DUMMY_329_112_2_18_2_39_24_4,(uint8*)"");


}
/*****************************************************************************/
// Description :  free service  response data structure
// Global resource dependence :
// Author: lisa lin
// Note: copy from mbbms_auth.c
/*****************************************************************************/
PUBLIC void MBBMS_Free_Service_Request_Res_Ptr(MBBMS_Service_Request_Res_t *ptr)
{

 	MBBMS_Purchase_Item_t *cur_item = NULL;
 	MBBMS_Purchase_Item_t *next_item = NULL;
	MBBMS_LTKM_List_t *cur_ltkm = NULL;
	MBBMS_LTKM_List_t *next_ltkm = NULL;
	       //SCI_TRACE_LOW:"MBBMS_Free_Service_Request_Res_Ptr ...\n"
	       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MBBMS_API_DUMMY_344_112_2_18_2_39_24_5,(uint8*)"");
		if (ptr == NULL) return;
		if (ptr->purchase_item_ptr != NULL)
			{
				cur_item = next_item = ptr->purchase_item_ptr->next;
				while( next_item != NULL)
					{
						cur_item = next_item->next;
						SCI_FREE(next_item);
						next_item = cur_item;
					}
				SCI_FREE(ptr->purchase_item_ptr);
			}
		if (ptr->smartcard_profile_ptr != NULL)
			{
				cur_ltkm = next_ltkm = ptr->smartcard_profile_ptr->next;
				while(next_ltkm != NULL)
					{
						cur_ltkm = next_ltkm->next;
						SCI_FREE(next_ltkm);
						next_ltkm = cur_ltkm;
					}
				SCI_FREE(ptr->smartcard_profile_ptr);
			}
		SCI_FREE(ptr);
		ptr = NULL;
		//SCI_TRACE_LOW:"MBBMS_Free_Service_Request_Res_Ptr END \n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MBBMS_API_DUMMY_370_112_2_18_2_39_24_6,(uint8*)"");

}
/*****************************************************************************/
// Description : free subupdate request data structure
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC void MBBMS_Free_SubUpdate_Request_Ptr(MBBMS_Subscription_Update_t *ptr){}

/*****************************************************************************/
// Description : free subupdate response data structure
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MBBMS_Free_Subupdate_Res_Ptr(MBBMS_Subscription_Update_Res_t *ptr){}

PUBLIC void MBBMS_GBA_Free_Data(mbbms_gba_data_t *data_ptr){}

/*****************************************************************************/
// Description : free account query data structure
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MBBMS_Free_Account_Query_Ptr(MBBMS_Account_Inquiry_t *ptr){}

/*****************************************************************************/
// Description : free account query response data structure
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MBBMS_Free_Account_Query_Res_Ptr(MBBMS_Account_Inquiry_Res_t *ptr){}

/*****************************************************************************/
// Description : free msk request data structure
// Global resource dependence :
// Author: lisa lin
// Note: copy from mbbms_auth.c
/*****************************************************************************/
PUBLIC void MBBMS_Free_MSK_Ptr(MBBMS_Msk_Request_t *ptr)
{
 		MBBMS_MSK_domain_t *cur = NULL;
		MBBMS_MSK_domain_t *next = NULL;
		if (ptr == NULL) return;
		cur = next = ptr->msk_domain_ptr;
		while(next != NULL)
			{
			cur = next->next;
			SCI_FREE(next);
			next = cur;
			}
		SCI_FREE(ptr);
		ptr = NULL;

}

/*****************************************************************************/
// Description : free msk response data structure
// Global resource dependence :
// Author: lisa lin
// Note: copy from mbbms_auth.c
/*****************************************************************************/
PUBLIC void MBBMS_Free_MSK_Res_Ptr(MBBMS_Msk_Response_t *ptr)
{

		MBBMS_LTKM_List_t * cur = NULL;
		MBBMS_LTKM_List_t *next = NULL;
		MBBMS_Msk_Response_t *msk_cur = NULL;
		MBBMS_Msk_Response_t *msk_next = NULL;
		if (ptr == NULL) return;
		msk_cur = msk_next = ptr;
		while (msk_next != NULL)
			{
				cur = next = msk_next->ltkm_ptr;
				while (next != NULL)
					{
					cur = next->next;
					SCI_FREE(next);
					next = cur;
					}
				msk_cur = msk_next->next;
				SCI_FREE(msk_next);
				msk_next = msk_cur;
			}
		SCI_FREE(ptr);
		ptr = NULL;

}


/*****************************************************************************/
// Description : operation 
// Global resource dependence :
// Author: lisa lin
// Note: 
/*****************************************************************************/
//Entry to Open/Cancel/Suspend/Resume the MBBMS MTV. 异步函数
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Operation(MBBMS_OPERATION_E  Op_ptr )
{
	MBBMS_MSK_SERVICE_REQUEST_SIG_T* sendSignal;		
	MBBMS_MSK_SUBSCRIBER_TYPE_E	type=MBBMS_MTV_IDLE;
	MBBMS_Operation_Res_t  *operation_result_ptr=PNULL;

	operation_result_ptr=SCI_ALLOC(sizeof(MBBMS_Operation_Res_t));
	SCI_MEMSET(operation_result_ptr,0,sizeof(MBBMS_Operation_Res_t));
	
	switch(Op_ptr)
	{

		case MBBMS_OPEN:
			type=MBBMS_MTV_OPEN;
			operation_result_ptr->result=128;	
			break;

		case MBBMS_PAUSE:
			type=MBBMS_MTV_SUSPEND;
			operation_result_ptr->result=130;
	
			break;
			
		case MBBMS_CANCEL:
			type=MBBMS_MTV_CANCEL;
			operation_result_ptr->result=129;
	
			break;
		case MBBMS_RESUME:
			type=MBBMS_MTV_RESUME;
			operation_result_ptr->result=128;	
			break;
		default:
			break;
	}
	
  	MmiCreateSignal(MBBMS_MSK_SUB_SIGNAL,sizeof(MBBMS_MSK_SERVICE_REQUEST_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->type=type;
	sendSignal->result=MBBMS_MSK_SUB_SUCCESS;	
	sendSignal->data_ptr=(void*)operation_result_ptr;
	
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);

	return MBBMS_ACCEPT;

}

MBBMS_OPERATION_E operation_status = MBBMS_OPEN;

/*****************************************************************************/
// Description : Get the MBBMS MTV state: Open, cancel, suspending. 
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC MBBMS_OPERATION_E MBBMS_Operation_Status(void)
{
	return  operation_status;
}

/*****************************************************************************/
// Description : Get the MBBMS MTV state: Open, cancel, suspending. 
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC void MBBMS_Set_Operation_Status(MBBMS_OPERATION_E ope_status)
{
	operation_status = ope_status;
}

//业务订购请求,退订,查询和信息更新.异步函数
/*****************************************************************************/
// Description : subscrible request
// Global resource dependence :
// Author: lisa lin
// Note: 
/*****************************************************************************/
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Service_Request( MBBMS_Service_Request_t* request_ptr)
{
	MBBMS_MSK_SERVICE_REQUEST_SIG_T* sendSignal;	
	MBBMS_Service_Request_Res_t *	service_request_res_ptr=PNULL;
	
	service_request_res_ptr=(MBBMS_Service_Request_Res_t*)SCI_ALLOC(sizeof(MBBMS_Service_Request_Res_t));
	
	service_request_res_ptr->global_status_code=0;
	service_request_res_ptr->purchase_item_ptr=(MBBMS_Purchase_Item_t*)SCI_ALLOC(sizeof(MBBMS_Purchase_Item_t));

//	SCI_MEMCPY(service_request_res_ptr->purchase_item_ptr->global_id_ref,request_ptr->data_ref_ptr->gloabl_id_ref,MBBMS_IDREF_LEN);
	service_request_res_ptr->purchase_item_ptr->itemwiseStatusCode=0;
	service_request_res_ptr->purchase_item_ptr->next=PNULL;
	service_request_res_ptr->smartcard_profile_ptr=PNULL;
	
	if (g_sub_callback!=PNULL)
	(*g_sub_callback)((uint32)MBBMS_SUB_SERVICE_REQUEST,(uint32)MBBMS_MSK_SUB_SUCCESS,
					service_request_res_ptr);
	else
		{
	MmiCreateSignal(MBBMS_MSK_SUB_SIGNAL,sizeof(MBBMS_MSK_SERVICE_REQUEST_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->type=MBBMS_SUB_SERVICE_REQUEST;
	
	sendSignal->result=MBBMS_MSK_SUB_SUCCESS;	
	sendSignal->data_ptr=(void*)service_request_res_ptr;
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}

	return MBBMS_ACCEPT;

}

/*****************************************************************************/
// Description : Unsubscribe reqest
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Unsubscribe_Request( MBBMS_Service_Request_t* request_ptr)
{

	MBBMS_MSK_SERVICE_REQUEST_SIG_T* sendSignal;	
	MBBMS_Service_Request_Res_t *	service_request_res_ptr=PNULL;
	
	service_request_res_ptr=(MBBMS_Service_Request_Res_t*)SCI_ALLOC(sizeof(MBBMS_Service_Request_Res_t));
	
	service_request_res_ptr->global_status_code=0;
	service_request_res_ptr->purchase_item_ptr=(MBBMS_Purchase_Item_t*)SCI_ALLOC(sizeof(MBBMS_Purchase_Item_t));

//	SCI_MEMCPY(service_request_res_ptr->purchase_item_ptr->global_id_ref,request_ptr->data_ref_ptr->gloabl_id_ref,MBBMS_IDREF_LEN);
	
	service_request_res_ptr->purchase_item_ptr->itemwiseStatusCode=0;
	
	service_request_res_ptr->purchase_item_ptr->next=PNULL;

	service_request_res_ptr->smartcard_profile_ptr=PNULL;

		if (g_sub_callback!=PNULL)
	(*g_sub_callback)((uint32)MBBMS_SUB_SERVICE_UNSUB,(uint32)MBBMS_MSK_SUB_SUCCESS,
					service_request_res_ptr);
	else
		{
	MmiCreateSignal(MBBMS_MSK_SUB_SIGNAL,sizeof(MBBMS_MSK_SERVICE_REQUEST_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->type=MBBMS_SUB_SERVICE_UNSUB;
	
	sendSignal->result=MBBMS_MSK_SUB_SUCCESS;	
	sendSignal->data_ptr=(void*)service_request_res_ptr;	
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	return MBBMS_ACCEPT;

}

/*****************************************************************************/
// Description : subscription table update
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Subscription_Update(void)
{

	MBBMS_MSK_SERVICE_REQUEST_SIG_T* sendSignal;		
	if (g_sub_callback!=PNULL)
		(*g_sub_callback)((uint32)MBBMS_SUB_UPDATE,(uint32)MBBMS_MSK_SUB_SUCCESS,
					NULL);
	else
		{
		MmiCreateSignal(MBBMS_MSK_SUB_SIGNAL,sizeof(MBBMS_MSK_SERVICE_REQUEST_SIG_T),(MmiSignalS**)&sendSignal);	
		sendSignal->type=MBBMS_SUB_UPDATE;
		sendSignal->result=MBBMS_MSK_SUB_SUCCESS;
		sendSignal->data_ptr=NULL;
		MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	
	return MBBMS_ACCEPT;
}

/*****************************************************************************/
// Description : Acount inquiry
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Account_Inquiry(MBBMS_Account_Inquiry_t* request_ptr)
{
	MBBMS_MSK_SERVICE_REQUEST_SIG_T* sendSignal;		
	if (g_sub_callback!=PNULL)
		(*g_sub_callback)((uint32)MBBMS_SUB_QUIRY_ACCOUNT,(uint32)MBBMS_MSK_SUB_SUCCESS,
					NULL);
	else
		{
  	MmiCreateSignal(MBBMS_MSK_SUB_SIGNAL,sizeof(MBBMS_MSK_SERVICE_REQUEST_SIG_T),(MmiSignalS**)&sendSignal);	
	sendSignal->type=MBBMS_SUB_QUIRY_ACCOUNT;
	sendSignal->result=MBBMS_MSK_SUB_SUCCESS;	
	sendSignal->data_ptr=NULL;
	MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	return MBBMS_ACCEPT;	
}
/*****************************************************************************/
// Description : Get Account Info
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
//MBBMS_Get_Account_Info is not supported now.
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_Get_Account_Info(MBBMS_Account_Inquiry_Res_t *ptr)
{
	return MBBMS_ACCEPT;
}

/*****************************************************************************/
// Description : msk request
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
//业务密钥获取和播放时设置
//the process entry of retrieve the serivce keys from network.
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_MSK_Request(MBBMS_Msk_Request_t *data_ptr)
{

	MBBMS_MSK_SERVICE_REQUEST_SIG_T* sendSignal;		
	if (g_sub_callback!=PNULL)
		(*g_sub_callback)((uint32)MBBMS_MSK_GET_MSK,(uint32)MBBMS_MSK_SUB_SUCCESS,
					NULL);
	else
		{
		MmiCreateSignal(MBBMS_MSK_SUB_SIGNAL,sizeof(MBBMS_MSK_SERVICE_REQUEST_SIG_T),(MmiSignalS**)&sendSignal);	
		sendSignal->type=MBBMS_MSK_GET_MSK;
		sendSignal->result=MBBMS_MSK_SUB_SUCCESS;	
		sendSignal->data_ptr=NULL;
		MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}

	return MBBMS_ACCEPT;

}
/*****************************************************************************/
// Description : to stop http process
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC MBBMS_MSK_SUBSCRIBER_TYPE_E MBBMS_MSK_SUB_Stop()
{

	MBBMS_MSK_SERVICE_REQUEST_SIG_T* sendSignal;		
	if (g_sub_callback!=PNULL)
		(*g_sub_callback)((uint32)MBBMS_MTV_STOP_OPERATION,(uint32)MBBMS_MSK_SUB_SUCCESS,
					NULL);
	else
		{	
	  	MmiCreateSignal(MBBMS_MSK_SUB_SIGNAL,sizeof(MBBMS_MSK_SERVICE_REQUEST_SIG_T),(MmiSignalS**)&sendSignal);	
		sendSignal->type=MBBMS_MTV_STOP_OPERATION;
		sendSignal->result=MBBMS_MSK_SUB_SUCCESS;	
		sendSignal->data_ptr=NULL;
		MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
		}
	return MBBMS_ACCEPT;

}

/*****************************************************************************/
// Description : Set the msk_id's service keys before playing the encrypted service.
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
BOOLEAN MBBMS_Set_MSK(uint32 keydomainid, uint32 msk_id)
{
	return TRUE;
}

/*****************************************************************************/
// Description : check if msk_id's service key is available.
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
 BOOLEAN MBBMS_Check_MSK(uint32 keydomainid, uint32 msk_id)
{
	return TRUE;
}

/*****************************************************************************/
// Description : MBBMS_GBA_Exit
// Global resource dependence :
// Author:lisa lin
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MBBMS_GBA_Exit(void)
{
	return TRUE;
}


/*****************************************************************************/
//  Description : Create ISMA data
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note: 
/*****************************************************************************/
PUBLIC SG_ERROR_E MBBMSSDP_CreateIsma(uint8  cmmb_frqNo, uint16  cmmb_serviceId)
{
    return SG_ERROR_NONE;
}




#if 0
PUBLIC SG_ERROR_E  MBBMSSG_FreePurchaseItemByContent(SG_LIST_PTR_T *itemList_head_ptr    // [OUT] 
                                                    )
{
    return SG_ERROR_NONE;
}

void MBBMSSG_ManualSearchServiceGuide(void){}

PUBLIC SG_ERROR_E  MBBMSSG_FreePurchaseItemByService(SG_LIST_PTR_T *itemList_head_ptr    // [OUT] 
                                                    )
{
    return SG_ERROR_NONE;
}

uint8*  MBBMSSG_GetWelcomeInfo(uint32 *data_len_ptr)
{
    return PNULL;
}
#endif

#endif
#endif




